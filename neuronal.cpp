#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

struct Case{
    vector<double> input;
    double result;
};

Case getCase(){
    double a=rand()%2, b=rand()%2;
    return {{1,a,b}, (a && b)};
}

int main(){
    srand(time(0));
    uint32_t minNoErrorCasesConsecutives=10000;
    uint32_t X=3;
    double w[X];
    double learn=0.01;
    for(int i=0; i<X; i++)
        w[i]=(rand()%11)/10;

    int noError=0;

    /** SET INITIAL WEIGHT **/
    do{
        Case c = getCase();
        if(c.input.size()!=X){
                cerr << "ERROR WITH CASE SIZE INPUT" << endl;
            break;
        }

        double accum=0;
        for(int i=0; i<X; i++)
            accum += c.input[i]*w[i];
        accum = accum>0.5;

        double error = c.result-accum;
        if(abs(error)>0.0){
            for(int i=0; i<X; i++)
                w[i] += learn*error*c.input[i];
            noError=0;
        }else ++noError;
    }while(noError<minNoErrorCasesConsecutives);

    vector<Case> tests = {{{1,0,0},0},
                          {{1,0,1},0},
                          {{1,1,0},0},
                          {{1,1,1},1}};
    for(Case& test:tests){
        for(int i=0; i<X; i++){
            cout << test.input[i];
            if(X-1!=i)
                cout << ",";
        }
        cout << endl << endl;
        double fin=0;
        for(int i=0; i<X; i++)
            fin += test.input[i]*w[i];
        cout << (fin>0.5) << endl << endl;
    }
}
