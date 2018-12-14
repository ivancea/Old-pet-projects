#include <iostream>
#include <cmath>
#include <limits.h>

using namespace std;


int main () {
    while(true){
        unsigned long long int fin=0, i=2, hh=0;
        bool dos = true;
        cout << "Pon el numero a factorizar: ";
        cin >> hh;
        while(hh<2 || hh>ULLONG_MAX-1){
            cin.clear();
            cout << "Debe ser un numero entre 2 y 18446744073709551615: ";
            cin >> hh;
            cout << endl;
        }
        cout << endl << "Numero a factorizar: " << hh << endl << 1 << " ";
        uint64_t sq = sqrt(hh);
        for(;i<sq;){
            if(hh%i==0)
                if(hh==i){
                    break;
                }else{
                    hh=hh/i;
                    sq = sqrt(hh);
                    cout << i << " ";
                }
            else
                if(dos){
                    dos=false;
                    ++i;
                }else
                    i+=2;
        }
        cout << hh << endl << "Acabado." << endl << endl << endl;
    }
}
