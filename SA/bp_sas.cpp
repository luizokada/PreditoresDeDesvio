
#include "bp.h"
#include "bp_helper.h"
#include <bitset>
#include <math.h>
#include <vector>
#define M 4
#define I 4
#define K 12

uintptr_t last_target;

vector<tableBHR> BHR;

vector<tablePHT> PHT;

void BP::init()
{
    int numBHRs = pow(2, (float)I);
    for (int i = 0; i < numBHRs; i++)
    {
        tableBHR aux;
        aux.historico = 0;
        BHR.push_back(aux);
    }
    int numPHTs = pow(2, (float)M);
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
    int indexBHR = getBitsMaisSignificativos(br.inst_ptr, I);
    int indexPHT = getBitsMaisSignificativos(br.inst_ptr, M);
    taken = isTaken(PHT[indexPHT].cont[BHR[indexBHR].historico]);

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
    int indexBHR = getBitsMaisSignificativos(br.inst_ptr, I);
    int indexPHT = getBitsMaisSignificativos(br.inst_ptr, M);
    //cout << "ANTES: " << BHR[index].historico << "TAKEN :" << br.taken << "\n";
    if (br.taken)
    {
        if (PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() < 3)
        {
            PHT[indexPHT].cont[BHR[indexBHR].historico].estado = PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() + 1;
        }
        deslocaBitsBHR(BHR[indexBHR]);
        BHR[indexBHR].historico = BHR[indexBHR].historico + 1;
    }
    else
    {
        if (PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() > 0)
        {
            PHT[indexPHT].cont[BHR[indexBHR].historico].estado = PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() - 1;
        }
        deslocaBitsBHR(BHR[indexBHR]);
    }
    //cout << "DEPOIS: " << BHR[index].historico << "TAKEN :" << br.taken << "\n";
}
