
#include "bp.h"
#include "bp_helper.h"
#include <bitset>
#include <math.h>
#include <vector>

#define K 12

uintptr_t last_target;

tableBHR BHR;

vector<maquinaEstado> PHT;

void BP::init()
{
    BHR.historico = 0;
    int tamanho;
    tamanho = pow(2, (float)K);
    for (int i = 0; i < tamanho; i++)
    {
        maquinaEstado ME;
        ME.estado = 0b00;
        PHT.push_back(ME);
    }
    br_trace_level = TRACE_LEVEL_NONE;
    br_trace << "GAg 2-level Branch Predictor!" << endl;
}

Prediction BP::predict(EntInfo br)
{
    bool taken;
    uintptr_t target;
    taken = isTaken(PHT[BHR.historico]);

    if (br.direct)
    {
        target = br.target;
    }
    else
    {
        target = last_target;
    }

    return Prediction(taken, target);
}

void BP::update(ResInfo br)
{
    if (!br.direct)
    {
        last_target = br.target;
    }
    if (br.taken)
    {
        if (PHT[BHR.historico].estado.to_ulong() < 3)
        {
            PHT[BHR.historico].estado = PHT[BHR.historico].estado.to_ulong() + 1;
        }
        deslocaBitsBHR(BHR);
        BHR.historico = BHR.historico + 1;
    }
    else
    {
        if (PHT[BHR.historico].estado.to_ulong() > 0)
        {
            PHT[BHR.historico].estado = PHT[BHR.historico].estado.to_ulong() - 1;
        }
        deslocaBitsBHR(BHR);
    }
}
