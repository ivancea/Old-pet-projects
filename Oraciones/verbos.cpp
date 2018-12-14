#include "verbos.h"

#include <iostream>

using std::string;

string personas[7]{
    "", /// Va de 1 a 6
    "",
    "s",
    "",
    "mos",
    "is",
    "n"
};

int getConjugacion(const string& s){
    if(s[s.size()-1]!='r')
        return -1;
    switch(s[s.size()-2]){
        case 'a': return 0;
        case 'e': return 1;
        case 'i': return 2;
    }
    return -1;
}

string VT(int conj){
    switch(conj){
    case 0: return "a";
    case 1: return "e";
    case 2: return "i";
    }
    return "";
}

string getConjugacion(int n){
    switch(n){
    case 0: return "ar";
    case 1: return "er";
    case 2: return "ir";
    default: return "";
    }
}

void inicializarMorfemas(string (&morfemas)[3][6], string *uno, string *dos, string *tres){

    for(int i=0; i<6; i++){
        morfemas[0][i] = uno[i];
        morfemas[1][i] = dos[i];
        morfemas[2][i] = tres[i];
    }

    delete[] uno;
    delete[] dos;
    delete[] tres;
}

string nombreTiempo(tiempos_t tiempo){
    switch(tiempo){
    case Infinitivo:          return "infinitivo";
    case InfinitivoComp:      return "infinitivo compuesto";
    case Gerundio:            return "gerundio";
    case GerundioComp:        return "gerundio compuesto";
    case Participio:          return "participio";
    case Presente_ind:        return "presente de indicativo";
    case Futuro_ind:          return "futuro de indicativo";
    case FuturoPerf_ind:      return "futuro perfecto de indicativo";
    case Condicional_ind:     return "condicional de indicativo";
    case CondicionalPerf_ind: return "condicional perfecto de indicativo";
    case PretPerf_ind:        return "pretérito perfecto de indicativo";
    case PretPerfComp_ind:    return "pretérito perfecto compuesto de indicativo";
    case PretPlus_ind:        return "pretérito pluscuamperfecto de indicativo";
    case PretAnt_ind:         return "pretérito anterior de indicativo";
    case PretImp_ind:         return "pretérito imperfecto de indicativo";
    case Presente_sub:        return "presente de subjuntivo";
    case Futuro_sub:          return "futuro de subjuntivo";
    case FuturoPerf_sub:      return "futuro perfecto de subjuntivo";
    case PretImp1_sub:        return "pretérito imperfecto de subjuntivo";
    case PretImp2_sub:        return "pretérito imperfecto de subjuntivo";
    case PretPlus1_sub:       return "pretérito pluscuamperfecto de subjuntivo";
    case PretPlus2_sub:       return "pretérito pluscuamperfecto de subjuntivo";
    case Imperativo:          return "imperativo";
    default: return "";
    }
}

bool isPersonal(tiempos_t tiempo){
    switch(tiempo){
    case Infinitivo:
    case InfinitivoComp:
    case Gerundio:
    case GerundioComp:
    case Participio:
        return false;
    default: return true;
    }
}

bool isIndicativo(tiempos_t tiempo){
    switch(tiempo){
    case Presente_ind:
    case Futuro_ind:
    case FuturoPerf_ind:
    case Condicional_ind:
    case CondicionalPerf_ind:
    case PretPerf_ind:
    case PretPerfComp_ind:
    case PretPlus_ind:
    case PretAnt_ind:
    case PretImp_ind:
        return true;
    default: return false;
    }
}

bool isSubjuntivo(tiempos_t tiempo){
    switch(tiempo){
    case Presente_sub:
    case Futuro_sub:
    case FuturoPerf_sub:
    case PretImp1_sub:
    case PretImp2_sub:
    case PretPlus1_sub:
    case PretPlus2_sub:
        return true;
    default: return false;
    }
}


string conjugar(string verbo, tiempos_t tiempo, int persona, int plural){
    if(persona<1 || persona>6 || verbo.size()<2)
        return "";
    int conj = getConjugacion(verbo);
    if(conj == -1)
        return "";
    if(persona<4 && plural==1)
        persona+=3;
    for(int i=0; i<verbo.size(); i++)
        switch(verbo[i]){
        case 'á': verbo[i]='a';break;
        case 'é': verbo[i]='e';break;
        case 'í': verbo[i]='i';break;
        case 'ó': verbo[i]='o';break;
        case 'ú': verbo[i]='u';break;
        }
    string lex = verbo;
    lex.erase(lex.size()-2,2);

    string morfemas[3][6];

    switch(tiempo){
    case Infinitivo:
        return verbo;
    case InfinitivoComp:
        return conjugar("haber", Infinitivo, persona) + " " + conjugar(verbo, Participio,persona);
    case Gerundio:
        return lex + (conj==0?"ando":"iendo");
    case GerundioComp:
        return conjugar("haber", Gerundio, persona) + " " + conjugar(verbo, Participio, persona);
    case Participio:
        return lex + (conj==0?"ado":"ido");

    case Presente_ind:
        if(conj==2 && persona==5)
            return lex + "ís";
        inicializarMorfemas(morfemas,
            new string[6]{"o","a","a","a","á","a"},
            new string[6]{"o","e","e","e","e","e"},
            new string[6]{"o","e","e","i","i","e"}
        );
        return lex + morfemas[conj][persona-1] + personas[persona];
    case Futuro_ind:
        inicializarMorfemas(morfemas,
            new string[6]{"aré","ará","ará","are","aré","ará"},
            new string[6]{"eré","erá","erá","ere","eré","erá"},
            new string[6]{"iré","irá","irá","ire","iré","irá"}
        );
        return lex + morfemas[conj][persona-1] + personas[persona];
    case FuturoPerf_ind:
        return conjugar("haber",Futuro_ind, persona) + " " + conjugar(verbo, Participio, persona);
    case Condicional_ind:
        return lex + VT(conj) + "ría" + personas[persona];
    case CondicionalPerf_ind:
        return conjugar("haber",Condicional_ind, persona) + " " + conjugar(verbo, Participio, persona);

    case PretPerf_ind:
        inicializarMorfemas(morfemas,
            new string[6]{"é","aste","ó","a","aste","aro"},
            new string[6]{"í","iste","ió","i","iste","iero"},
            new string[6]{"í","iste","ió","i","iste","iero"}
        );
        return lex + morfemas[conj][persona-1] + (persona==2?"":personas[persona]);
    case PretPerfComp_ind:
        return conjugar("haber",Presente_ind, persona) + " " + conjugar(verbo, Participio, persona);
    case PretPlus_ind:
        return conjugar("haber",PretImp_ind, persona) + " " + conjugar(verbo, Participio, persona);
    case PretAnt_ind:
        return conjugar("haber",PretPerf_ind, persona) + " " + conjugar(verbo, Participio, persona);
    case PretImp_ind:
        return lex + (conj==0?(persona==4?"ába":"aba"):"ía") + personas[persona];


    case Presente_sub:
        return lex + (conj==0?"e":"a") + personas[persona];
    case Futuro_sub:
        return lex + (conj==0?(persona==4?"áre":"are"):(persona==4?"iére":"iere")) + personas[persona];
    case FuturoPerf_sub:
        return conjugar("haber",Futuro_sub, persona) + " " + conjugar(verbo, Participio, persona);
    case PretImp1_sub:
        return lex + (conj==0?(persona==4?"ára":"ara"):(persona==4?"iéra":"iera")) + personas[persona];
    case PretImp2_sub:
        return lex + (conj==0?(persona==4?"áse":"ase"):(persona==4?"iése":"iese")) + personas[persona];
    case PretPlus1_sub:
        return conjugar("haber",PretImp1_sub, persona) + " " + conjugar(verbo, Participio, persona);
    case PretPlus2_sub:
        return conjugar("haber",PretImp2_sub, persona) + " " + conjugar(verbo, Participio, persona);


    case Imperativo:
        if(persona==1) return "";
        inicializarMorfemas(morfemas,
            new string[6]{"","a","e","e","a","e"},
            new string[6]{"","e","a","a","e","a"},
            new string[6]{"","e","a","a","i","a"}
        );
        return lex + morfemas[conj][persona-1] + personas[persona];

    default: return "";
    }

}
