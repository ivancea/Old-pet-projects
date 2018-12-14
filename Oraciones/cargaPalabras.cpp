#include "cargaPalabras.h"

#include <fstream>
#include <vector>

std::vector<std::string> verbos;

struct cargaPalabras_STRUCT{
    cargaPalabras_STRUCT(){
        std::ifstream file("verbos.txt");
        if(!file) return;
        std::string t;
        while(file){
            getline(file,t,'\n');
            verbos.push_back(t);
        }
    }
} cargaPalabras_STRUCT_OBJECT;
