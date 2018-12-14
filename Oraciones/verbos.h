#ifndef VERBOS_H
#define VERBOS_H

#include <iostream>

enum tiempos_t{
    Infinitivo,
    InfinitivoComp,
    Gerundio,
    GerundioComp,
    Participio,

    Presente_ind,
    Futuro_ind,
    FuturoPerf_ind,
    Condicional_ind,
    CondicionalPerf_ind,

    PretPerf_ind,
    PretPerfComp_ind,
    PretPlus_ind,
    PretAnt_ind,
    PretImp_ind,


    Presente_sub,
    Futuro_sub,
    FuturoPerf_sub,
    PretImp1_sub,
    PretImp2_sub,
    PretPlus1_sub,
    PretPlus2_sub,

    Imperativo,

    NUM_TIEMPOS
};

std::string nombreTiempo(tiempos_t tiempo);

std::string conjugar(std::string infinitivo, tiempos_t tiempos, int persona, int plural = -1);

bool isPersonal(tiempos_t tiempo);

bool isIndicativo(tiempos_t tiempo);

bool isSubjuntivo(tiempos_t tiempo);

#endif // VERBOS_H
