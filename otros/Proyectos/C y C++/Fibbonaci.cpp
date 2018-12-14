#include <iostream>
#include <vector>
#include "C:\includes\ivancea.h"
using namespace std;

uint64_t fib_recursivo(uint64_t n){
    if(n==0) return 0;
    if(n==1) return 1;
    return fib_recursivo(n-1) + fib_recursivo(n-2);
}

uint64_t fib_iterativo(uint64_t n){
    if(n==0) return 0;
    if(n==1) return 1;
    uint64_t a=0, b=1, c=0;;
    for(int i=2; i<=n; i++){
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

uint64_t fib_mezcla(uint64_t n){
    static vector<uint64_t> v;
    if(!v.size()){
        v.push_back(0);
        v.push_back(1);
    }
    if(v.size()<=n)
        for(uint64_t i=v.size(); i<=n; i++)
            v.push_back(v[i-1] + v[i-2]);
    return v[n];
}

#define PRUEBA1 40
#define PRUEBA2 85

int main(){
    uint32_t time=0, last=0;
    timer(*(new thread()), &time, &(*(new bool())=true), 25);
    cout << "-PRUEBA 1: Fib("<< PRUEBA1 <<")-" << endl << endl;
    cout << "Recursivo: " << fib_recursivo(PRUEBA1) << "  ->  ";
    cout << time-last << "ms" << endl;
    last = time;
    cout << "Iterativo: " << fib_iterativo(PRUEBA1) << "  ->  ";
    cout << time-last << "ms" << endl;
    last = time;
    cout << "Mezcla:    " << fib_mezcla(PRUEBA1) << "  ->  ";
    cout << time-last << "ms" << endl;
    cout << endl << endl << endl <<"-PRUEBA 2: Fib("<< PRUEBA2 <<") 1.000.000 veces-" << endl << endl;
    last = time;
    for(int i=0; i<999999; i++)
        fib_iterativo(PRUEBA2);
    cout << "Iterativo: " << fib_iterativo(PRUEBA2) << "  ->  ";
    cout << time-last << "ms" << endl;
    last = time;
    for(int i=0; i<999999; i++)
        fib_mezcla(PRUEBA2);
    cout << "Mezcla:    " << fib_mezcla(PRUEBA2) << "  ->  ";
    cout << time-last << "ms" << endl;
    cin.get();
    return 0;
}

/**

--Fibonacci:--

 f(0) = 0
 f(1) = 1
 f(n) = f(n-1) + f(n+2)

-- -- -- -- --
**/
