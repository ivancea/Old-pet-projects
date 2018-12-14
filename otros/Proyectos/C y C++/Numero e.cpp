#include <iostream>
#include <fstream>

using namespace std;


double eRecursive(uint8_t max, int a=0){ //[2;1,2,1,1,4,1,1,6,1,1,8,]
    if(a==max) return 1;
    if(a==0) return 2+(1/eRecursive(max, a+1));
    switch(a%3){
        case 2: return (2*(a+1)/3)+(1/eRecursive(max, a+1));
        default: return 1+(1/eRecursive(max, a+1));
    }
}

double eCicle(uint8_t max){
    double e=1.0;
    uint64_t t=1;
    if(max>20) max=20;
    for(int i=1; i<max; i++){
        t=1;
        for(int j=1; j<=i; j++)
            t*=j;
        e+=(1/(double)t);
    }
    return e;
}

int main(){
    cout << eRecursive(100) << endl << eCicle(100);
    return 0;
}
