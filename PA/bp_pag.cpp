#include "bp.h"
#include "bp_helper.h"
#include <bitset>
#include <math.h>
#include <vector>
#define M 10
#define K 10
#define I 10

uintptr_t last_target;

vector<tableBHR> BHR;

vector<maquinaEstado> PHT;

void BP::init()
{
    int numBHRs = pow(2, (float)M);
    for (int i = 0; i < numBHRs; i++)
    {
        tableBHR aux;
        aux.historico = 0;
        BHR.push_back(aux);
    }
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
    int index = getBitsMenosSignificativos(br.inst_ptr, M);
    taken = isTaken(PHT[BHR[index].historico]);

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
    int index = getBitsMenosSignificativos(br.inst_ptr, M);
    cout << "ANTES: " << BHR[index].historico << "TAKEN :" << br.taken << "\n";
    if (br.taken)
    {
        if (PHT[BHR[index].historico].estado.to_ulong() < 3)
        {
            PHT[BHR[index].historico].estado = PHT[BHR[index].historico].estado.to_ulong() + 1;
        }
        BHR[index].historico = BHR[index].historico << 1;
        if (BHR[index].historico < 0)
        {
            tableBHR aux;
            aux.historico = pow(2, (float)K) - 1;
            BHR[index].historico = BHR[index].historico & aux.historico;
        }
        BHR[index].historico = BHR[index].historico + 1;
    }
    else
    {
        if (PHT[BHR[index].historico].estado.to_ulong() > 0)
        {
            PHT[BHR[index].historico].estado = PHT[BHR[index].historico].estado.to_ulong() - 1;
        }
        BHR[index].historico = BHR[index].historico << 1;
        if (BHR[index].historico < 0)
        {
            tableBHR aux;
            aux.historico = pow(2, (float)K) - 1;
            BHR[index].historico = BHR[index].historico & aux.historico;
        }
    }
    cout << "DEPOIS: " << BHR[index].historico << "TAKEN :" << br.taken << "\n";
}
