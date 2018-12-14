#include <fstream>
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

#define CHARS_NUM 2

void coutIt(char* str, size_t n){
    static size_t line=2;
    bool contains=false;
    int i=0, t=0;
    while(i<n){
        if(str[i]=='\n'){
            cout.write(str+t,i-t);
            cout << endl << line++ << "  ";
            t=i+1;
        }
        ++i;
    }
    cout.write(str+t,i-t);
}

int main(int args, char** argc){
    if(args==1){
        cout << "Por lo menos 1 archivo de parametro." << endl;
        cin.get();
        return 1;
    }
    char *chars = new char[CHARS_NUM];
    cout << "1  ";
    for(int i=1; i<args; i++){
        ifstream file(argc[i]);
        if(!file) continue;
        while(file){
            getch();
            file.read(chars,CHARS_NUM);
            coutIt(chars,file.gcount());
        }
        cout << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(300));
    cout << "Compilando... " << endl;
    cout << "               0%                    50%                     100%" << endl;
    cout << "               ";
    for(uint16_t i=0; i<50; i++){
        cout << (char)219;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    cout << endl << "Finalizado." << endl;
    cin.get();
}
