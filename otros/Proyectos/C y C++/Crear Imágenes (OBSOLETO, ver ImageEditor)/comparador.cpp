#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    if(argc==3){
            cout << argv[1] << endl << endl << argv[2] << endl << endl;
        ifstream a(argv[1],ios::binary), b(argv[2],ios::binary);
        if(!a || !b){
            cout << "Error abriendo archivos..." << endl;
            system("pause");
            return 1;
        }
        unsigned int n=0;
        char ca, cb;
        while(!a.eof() && !b.eof()){
            a.read(&ca,1);
            b.read(&cb,1);
            if(ca!=cb) ++n;
        }
        if(a.eof() && b.eof()){
            cout << "Bytes diferentes: " << n << endl;
            system("pause");
            return 1;
        }
        n=0;
        if(a.eof()){
            while(!b.eof()){
                b.read(&cb,1);
                ++n;
            }
        }else if(b.eof()){
            while(!a.eof()){
                a.read(&ca,1);
                ++n;
            }
        }
        cout << "La diferencia de tamanio entre los archivos es de " << n << " bytes." << endl;
        system("pause");
        return 0;
    }
}
