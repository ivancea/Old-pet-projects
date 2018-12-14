#include <iostream>
#include <ctime>
#include <clocale>
#include <fstream>
#include <vector>

#include "cargaPalabras.h"
#include "verbos.h"
#include "sintagmas/nominal.h"

using namespace std;


int main(){
    setlocale(LC_ALL, "spanish");

    srand(time(0));

    /*while(true){
        size_t verboRand = rand()%v.size();
        tiempos_t tiempo = tiempos_t(rand()%NUM_TIEMPOS);
        cout << "Verbo: " << v[verboRand] << endl;
        cout << "Tiempo: " << nombreTiempo(tiempo) << endl << endl;
        if(!isPersonal(tiempo))
            cout << "-> " << conjugar(v[verboRand], tiempo, 1) << endl;
        else
        for(int i=1; i<=6; i++){
            cout <<"-> " << conjugar(v[verboRand], tiempo, i) << endl;
        }
        cout << endl;
        cin.get();
        cout << endl;
    }*/

    if(verbos.size()==0){
        cout << "No se ha encontrado el archivo de verbos.";
        return 1;
    }

    while(true){
        tiempos_t tiempo;
        do{
            tiempo = tiempos_t(rand()%NUM_TIEMPOS);
        }while(!isPersonal(tiempo) && tiempo!=Imperativo);
        string t = (isSubjuntivo(tiempo)?"ojalá ":"") + sintagmaNominal() + " " + conjugar(verbos[rand()%verbos.size()], tiempo, 3) + " " + sintagmaNominal() + ".";
        t[0] = toupper(t[0]);
        cout << t << endl;
        if(t.size()%80!=0)
            cout << endl;
        cin.get();
    }

    return 0;
}
