/*
 * bp_helper.h
 * Computer Architecture - Lab 7: Branch Prediction
 * Derek Chiou
 * Alex Hsu, Chirag Sakhuja, Tommy Huynh
 * Spring 2016
 *
 * This file contains any common helper functions you might need.
 * YOU CAN CHANGE ANYTHING IN THIS FILE.
 */
#ifndef _BP_HELPER_H_
#define _BP_HELPER_H_
#include "bp.h"
#include <vector>
#include <bitset>
#include <math.h>
#define K 10
typedef struct maquinaEstado
{
    bitset<2> estado;
};

typedef struct tablePHT
{
    vector<maquinaEstado> cont;
};
typedef struct tableBHR
{
    int historico : 11;
};

bool isTaken(maquinaEstado M)
{
    return M.estado[1] == 1;
}

uintptr_t getBitsMenosSignificativos(uintptr_t PC, int numBITs)
{
    int mascara = pow(2, (float)numBITs) - 1;
    int adress = PC & mascara;
    return adress;
}

uintptr_t getBitsMaisSignificativos(uintptr_t PC, int numBITs)
{
    int mascara = pow(2, (float)numBITs) - 1;
    int numBItsPC = (int)log2(PC) + 1;
    int desloca = 0;
    if (numBItsPC > numBITs)
    {

        desloca = numBItsPC - numBITs;
    }
    PC = PC >> desloca;
    int adress = PC & mascara;
    return adress;
}

void deslocaBitsBHR(tableBHR &BHR)
{
    BHR.historico = BHR.historico << 1;
    if (BHR.historico < 0)
    {
        tableBHR aux;
        aux.historico = pow(2, (float)K) - 1;
        BHR.historico = BHR.historico & aux.historico;
    }
}

#endif /* _BP_HELPER_H_ */
