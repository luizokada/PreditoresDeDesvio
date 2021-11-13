
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
    int indexPHT = getBitsMenosSignificativos(br.inst_ptr, M);
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
    int indexBHR = getBitsMaisSignificativos(br.inst_ptr, I);
    int indexPHT = getBitsMenosSignificativos(br.inst_ptr, M);
    cout << "ANTES: " << BHR[indexBHR].historico << "TAKEN :" << br.taken << "\n";
    if (br.taken)
    {
        if (PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() < 3)
        {
            PHT[indexPHT].cont[BHR[indexBHR].historico].estado = PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() + 1;
        }
        BHR[indexBHR].historico = BHR[indexBHR].historico << 1;
        if (BHR[indexBHR].historico < 0)
        {
            tableBHR aux;
            aux.historico = pow(2, (float)K) - 1;
            BHR[indexBHR].historico = BHR[indexBHR].historico & aux.historico;
        }
        BHR[indexBHR].historico = BHR[indexBHR].historico + 1;
    }
    else
    {
        if (PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() > 0)
        {
            PHT[indexPHT].cont[BHR[indexBHR].historico].estado = PHT[indexPHT].cont[BHR[indexBHR].historico].estado.to_ulong() - 1;
        }
        BHR[indexBHR].historico = BHR[indexBHR].historico << 1;
        if (BHR[indexBHR].historico < 0)
        {
            tableBHR aux;
            aux.historico = pow(2, (float)K) - 1;
            BHR[indexBHR].historico = BHR[indexBHR].historico & aux.historico;
        }
    }
    cout << "DEPOIS: " << BHR[indexBHR].historico << "TAKEN :" << br.taken << "\n";
}
