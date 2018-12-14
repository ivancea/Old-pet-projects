#include "nominal.h"

#include "adjetival.h"

#include <iostream>
#include <vector>

using std::string;
using std::vector;

string determinante(int masculino=-1){
    if(masculino==-1){
        vector<string> v{"lo"};
        return v[rand()%v.size()];
    }else if(masculino==0){
        vector<string> v{"la","esta","esa","aquella"};
        return v[rand()%v.size()];
    }else{
        vector<string> v{"el","este","ese","aquel"};
        return v[rand()%v.size()];
    }
}

string sustantivo(){
    vector<string> v{
        "f_prueba","f_casa","f_muerte","f_ventana","f_bolsa","f_estructura","f_puerta","f_persiana","f_lata","f_persona",
        "m_árbol","m_sacacorchos","m_mono","m_teléfono","m_gorro","m_cajón","m_rotulador","m_programador","m_tornillo","m_humano"
    };
    return v[rand()%v.size()];
}

int parseSustantivo(string& s){
    int genero = 0;
    if(s[0]=='m'){
        genero = 1;
    }
    s.erase(0,2);
    return genero;
}

string sintagmaNominal(){
    string t = sustantivo();
    int genero = parseSustantivo(t);
    t = determinante(genero) + " " + t;
    if(rand()%2==0){
        t += " " + sintagmaAdjetival(genero);
    }else if(rand()%4==0){
        t += " " + sintagmaAdjetival(genero);
        while(rand()%3==0){
            t += ", " + sintagmaAdjetival(genero);
        }
        t += " y " + sintagmaAdjetival(genero);
    }
    return t;
}
