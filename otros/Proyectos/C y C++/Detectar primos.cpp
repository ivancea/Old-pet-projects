#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

using namespace std;

inline bool primo(uint64_t n){
    if(n%2==0) return false;
    uint64_t i=3;
    uint64_t sq = sqrt(n);
    for (;n%i!=0; i+=2){
        if(i>sq){
            i=n;
            break;
        }
    }
    return i==n;
}

int main () {
    while(true){
        uint64_t u=2,lastU=u, g=0, h=0;
        string t;
        cout << "Numero de primos a conseguir (0 para ver ultimo primo): ";
        getline(cin,t);
        g = strtoull(t.c_str(),NULL,0);
        /*************************************/
        ifstream leer("primos.txt", ios::ate);
        char n='0';
        if(leer){
            while(n!='\n'){
                leer.unget();
                n = leer.get();
                leer.unget();
            }
            leer.get();
            leer >> u;
        }
        leer.close();
        if(g==0){
            cout << "Ultimo primo, o numero de primos almacenados? (U/N): ";
            getline(cin,t);
            if(t=="U")
                cout << "Ultimo primo conseguido: " << u << endl << endl;
            else{
                leer.close();
                leer.open("primos.txt");

                uint64_t num=0;
                while(leer){
                    getline(leer, t);
                    ++num;
                }
                cout << "Numero de primos almacenados: " << num << endl << endl;
            }
            continue;
        }
        /*************************************/
        ofstream escribir("primos.txt", ios::app);
        if(u==2){
            escribir << u++;
        }

        clock_t timer = clock();
        while(h<g){
            lastU=u;
            u+=2;
            if(u<lastU)
                break;
            if(primo(u)){
                escribir << endl << u;
                ++h;
            }
        }
        cout << endl << "Conseguidos " << h << " primos en " << ((clock()-timer)*1000)/CLOCKS_PER_SEC << " milisegundos." << endl << endl;
    }
}
