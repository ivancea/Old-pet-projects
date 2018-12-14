#include "adjetival.h"

#include <iostream>
#include <vector>

#include "../cargaPalabras.h"
#include "../verbos.h"

using std::string;
using std::vector;

string adverbioCantidad(){
    vector<string> v{"muy","poco","demasiado","bastante"};
    return v[rand()%v.size()];
}

string adjetivo(bool masculino){
    if(masculino){
        if(rand()%2==0){
            return conjugar(verbos[rand()%verbos.size()], Participio, 1);
        }
        vector<string> v{"grande","hermitaño","duro","grande","blanco","liso","resbaladizo","enorme","grueso"};
        return v[rand()%v.size()];
    }else{
        vector<string> v{"bonita","pequeña","enana","limpia","energúmena","corta","grande","enorme"};
        return v[rand()%v.size()];
    }
}

string sintagmaAdjetival(bool masculino){
    string t = adjetivo(masculino);
    if(rand()%2==0)
        t = adverbioCantidad() + " " + t;
    return t;
}
