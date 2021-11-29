
#include "bp.h"
#include "bp_helper.h"
#include <bitset>
#include <math.h>
#include <vector>
#define K 12
#define M 8

uintptr_t last_target;

tableBHR BHR;
vector<tablePHT> PHT;

void BP::init()
{
    BHR.historico = 0;
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
    br_trace << "GAp 2-level Branch Predictor!" << endl;
}

Prediction BP::predict(EntInfo br)
{
    bool taken;
    uintptr_t target;
    int index = getBitsMaisSignificativos(br.inst_ptr, M);

    taken = isTaken(PHT[index].cont[BHR.historico]);

    if (br.direct)
    {
        target = br.target;
    }
    else
    {
        target = last_target;
    }

    // return Prediction
    return Prediction(taken, target);
}

void BP::update(ResInfo br)
{
    if (!br.direct)
    {
        last_target = br.target;
    }
    int index = getBitsMaisSignificativos(br.inst_ptr, M);
    //cout << "ANTES: " << BHR.historico << " TAKEN :" << br.taken << " INDEX: " << index << "ENDERECO: " << br.inst_ptr << "\n";
    if (br.taken)
    {
        if (PHT[index].cont[BHR.historico].estado.to_ulong() < 3)
        {
            PHT[index].cont[BHR.historico].estado = PHT[index].cont[BHR.historico].estado.to_ulong() + 1;
        }
        deslocaBitsBHR(BHR);
        BHR.historico = BHR.historico + 1;
    }
    else
    {
        if (PHT[index].cont[BHR.historico].estado.to_ulong() > 0)
        {
            PHT[index].cont[BHR.historico].estado = PHT[index].cont[BHR.historico].estado.to_ulong() - 1;
        }
        deslocaBitsBHR(BHR);
    }
    //cout << "DEPOIS: " << BHR.historico << " TAKEN :" << br.taken << "\n";
}
