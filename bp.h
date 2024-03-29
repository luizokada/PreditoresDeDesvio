
#ifndef _BP_BTFN_H_
#define _BP_BTFN_H_

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "pin.H"

using std::cout;
using std::endl;
using std::ofstream;
using std::ostream;
using std::string;
using namespace std;

// branch trace
ofstream br_trace;

enum TraceLevels
{
    TRACE_LEVEL_NONE,
    TRACE_LEVEL_WRONG_TARGETS,
    TRACE_LEVEL_WRONG_DIRECTIONS,
    TRACE_LEVEL_ALL
};
uint8_t br_trace_level;

// Shell class
class Shell
{
private:
    static void dump_stats(std::ostream &output);

public:
    static void start();
    static void finish(INT32, VOID *);
    static void branch_instruction(INS, ADDRINT,
                                   USIZE, BOOL, ADDRINT, BOOL, BOOL, BOOL, BOOL);
    static void instruction(INS, VOID *);
};

// results
class Results
{
    friend class Shell;
    uint64_t total;
    uint64_t uncond;
    uint64_t cond;
    uint64_t direct;
    uint64_t indirect;
    uint64_t call;
    uint64_t ret;
    uint64_t taken;
    uint64_t ntaken;
    uint64_t p_taken;
    uint64_t p_ntaken;
    uint64_t hit;
    uint64_t miss;
    uint64_t d_hit;
    uint64_t d_miss;
    uint64_t t_hit;
    uint64_t t_miss;
};
Results res;

// define branch predictor info types
struct EntInfo
{
    uintptr_t inst_ptr;
    uintptr_t next_inst_ptr;
    uintptr_t target;
    bool uncond;
    bool direct;
    bool call;
    bool ret;
    string dasm;
};

struct ResInfo
{
    uintptr_t inst_ptr;
    uintptr_t next_inst_ptr;
    bool taken;
    uintptr_t target;
    bool uncond;
    bool direct;
    bool call;
    bool ret;
    string dasm;
};

// define prediction type
struct Prediction
{
    bool taken;
    uintptr_t target;

    Prediction(bool taken, uintptr_t target)
    {
        this->taken = taken;
        this->target = target;
    }
};

// BP class
class BP
{
public:
    static void init();
    static Prediction predict(EntInfo);
    static void update(ResInfo);
};

// initialization tasks
void Shell::start()
{
    res.total = 0;
    res.uncond = 0;
    res.cond = 0;
    res.direct = 0;
    res.indirect = 0;
    res.call = 0;
    res.ret = 0;
    res.taken = 0;
    res.ntaken = 0;
    res.p_taken = 0;
    res.p_ntaken = 0;
    res.hit = 0;
    res.miss = 0;
    res.d_hit = 0;
    res.d_miss = 0;
    res.t_hit = 0;
    res.t_miss = 0;
}

// output statistics
void Shell::dump_stats(ostream &os)
{
    double total = res.total;
    ios::fmtflags f(os.flags());
    os << std::dec;
    os << "------------------------- Statistics -------------------------" << endl;
    os << "Total Branches: " << total << endl;
    os << "Overall Hits:   "
       << std::setw(15) << std::setfill(' ') << res.hit
       << " | Accuracy: "
       << std::setw(8) << std::setprecision(6) << (res.hit / total * 100) << "%" << endl;
    os << "Direction Hits: "
       << std::setw(15) << std::setfill(' ') << res.d_hit
       << " | Accuracy: "
       << std::setw(8) << std::setprecision(6) << (res.d_hit / total * 100) << "%" << endl;
    os << "Target Hits:    "
       << std::setw(15) << std::setfill(' ') << res.t_hit
       << " | Accuracy: "
       << std::setw(8) << std::setprecision(6) << (res.t_hit / total * 100) << "%" << endl;
    os << endl;
    os << "    Taken Branches: " << res.taken << " |     Taken Predicted: " << res.p_taken << endl;
    os << "Not Taken Branches: " << res.ntaken << " | Not Taken Predicted: " << res.p_ntaken << endl;
    os << endl;
    os << "Unconditional Branches: " << res.uncond << endl;
    os << "Conditional   Branches: " << res.cond << endl;
    os << "Direct        Branches: " << res.direct << endl;
    os << "Indirect      Branches: " << res.indirect << endl;
    os << "Call          Branches: " << res.call << endl;
    os << "Return        Branches: " << res.ret << endl;
    os << "------------------------- Statistics -------------------------" << endl;
    os.flags(f);
}

// cleanup tasks
void Shell::finish(INT32 code, VOID *)
{
    br_trace.close();

    ofstream stats;
    stats.open("stats.out");
    dump_stats(stats);
    stats.close();
    dump_stats(cout);
}

// branch callback function
void Shell::branch_instruction(
    INS ins,
    ADDRINT inst_ptr,
    USIZE inst_size,
    BOOL taken,
    ADDRINT target,
    BOOL uncond,
    BOOL direct,
    BOOL call,
    BOOL ret)
{
    uintptr_t next_inst_ptr = inst_ptr + inst_size;
    string dasm = INS_Disassemble(ins);

    EntInfo e;
    e.inst_ptr = inst_ptr;
    e.next_inst_ptr = next_inst_ptr;
    e.target = direct ? target : 0;
    e.uncond = uncond;
    e.direct = direct;
    e.call = call;
    e.ret = ret;
    e.dasm = dasm;
    ResInfo r;
    r.inst_ptr = inst_ptr;
    r.next_inst_ptr = next_inst_ptr;
    r.taken = taken;
    r.target = target;
    r.uncond = uncond;
    r.direct = direct;
    r.call = call;
    r.ret = ret;
    r.dasm = dasm;
    Prediction p = BP::predict(e);
    BP::update(r);

    res.total++;
    if (uncond)
        res.uncond++;
    else
        res.cond++;
    if (direct)
        res.direct++;
    else
        res.indirect++;
    if (call)
        res.call++;
    if (ret)
        res.ret++;
    if (taken)
        res.taken++;
    else
        res.ntaken++;
    if (p.taken)
        res.p_taken++;
    else
        res.p_ntaken++;
    if ((taken == p.taken) && (!taken || (target == p.target)))
        res.hit++;
    else
        res.miss++;
    if (taken == p.taken)
        res.d_hit++;
    else
        res.d_miss++;
    if (target == p.target)
        res.t_hit++;
    else
        res.t_miss++;
    if (((br_trace_level == TRACE_LEVEL_WRONG_DIRECTIONS) && (taken != p.taken)) ||
        ((br_trace_level == TRACE_LEVEL_WRONG_TARGETS) && (target != p.target)) ||
        (br_trace_level == TRACE_LEVEL_ALL))
    {
        br_trace << "@ 0x" << std::hex << std::setfill('0') << std::setw(12) << inst_ptr;
        br_trace << " | Instruction: " << dasm << endl;
        br_trace << "    Predicted: " << (p.taken ? "    taken" : "not taken")
                 << ", 0x" << std::hex << std::setfill('0') << std::setw(12) << p.target << endl;
        br_trace << "    Actual   : " << (taken ? "    taken" : "not taken")
                 << ", 0x" << std::hex << std::setfill('0') << std::setw(12) << target << endl;
        br_trace << endl;
    }
}

// instruction instrumentation function
void Shell::instruction(INS ins, VOID *)
{
    if ((INS_IsBranchOrCall(ins) || INS_IsRet(ins)))
    {
        USIZE inst_size = INS_Size(ins);
        BOOL uncond = !INS_HasFallThrough(ins);
        BOOL direct = INS_IsDirectBranchOrCall(ins);
        BOOL call = INS_IsCall(ins);
        BOOL ret = INS_IsRet(ins);
        INS_InsertCall(
            ins, IPOINT_BEFORE, (AFUNPTR)branch_instruction,
            IARG_PTR, ins,
            IARG_INST_PTR,
            IARG_UINT32, inst_size,
            IARG_BRANCH_TAKEN,
            IARG_BRANCH_TARGET_ADDR,
            IARG_BOOL, uncond,
            IARG_BOOL, direct,
            IARG_BOOL, call,
            IARG_BOOL, ret,
            IARG_END);
    }
}

// main
int main(int argc, char **argv)
{
    if (PIN_Init(argc, argv))
        return 1;

    br_trace.open("branch_trace.out");
    Shell::start();
    BP::init();

    INS_AddInstrumentFunction(Shell::instruction, 0);
    PIN_AddFiniFunction(Shell::finish, 0);
    PIN_StartProgram();
}

#endif /* _BP_BTFN_H_ */
