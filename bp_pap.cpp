
#include "bp.h"
#include "bp_helper.h"
#include <bitset>
#include <math.h>
#include <vector>
#define A 6
#define K 12

uintptr_t last_target;

vector<tableBHR> BHR;

vector<tablePHT> PHT;

void BP::init()
{
    int numBHRs = pow(2, (float)A);
    for (int i = 0; i < numBHRs; i++)
    {
        tableBHR aux;
        aux.historico = 0;
        BHR.push_back(aux);
    }
    int numPHTs = pow(2, (float)A);
    int tamanho;
    tamanho = pow(2, (float)K);
    for (int i = 0; i < numPHTs; i++)
    {
        tablePHT aux;
        for (int j = 0; j < tamanho; j++)
        {
            maquinaEstado ME;
            ME.estado = 0b00;
            aux.cont.push_back(ME);
        }
        PHT.push_back(aux);
    }
    br_trace_level = TRACE_LEVEL_NONE;
    br_trace << "GAg 2-level Branch Predictor!" << endl;
}

Prediction BP::predict(EntInfo br)
{
    bool taken;
    uintptr_t target;
    int index = getBitsMenosSignificativos(br.inst_ptr, A);
    taken = isTaken(PHT[index].cont[BHR[index].historico]);

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
    int index = getBitsMenosSignificativos(br.inst_ptr, A);
    if (br.taken)
    {
        if (PHT[index].cont[BHR[index].historico].estado.to_ulong() < 3)
        {
            PHT[index].cont[BHR[index].historico].estado = PHT[index].cont[BHR[index].historico].estado.to_ulong() + 1;
        }
        deslocaBitsBHR(BHR[index]);
        BHR[index].historico = BHR[index].historico + 1;
    }
    else
    {
        if (PHT[index].cont[BHR[index].historico].estado.to_ulong() > 0)
        {
            PHT[index].cont[BHR[index].historico].estado = PHT[index].cont[BHR[index].historico].estado.to_ulong() - 1;
        }
        deslocaBitsBHR(BHR[index]);
    }
}
