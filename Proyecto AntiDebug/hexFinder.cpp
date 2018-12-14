#include <fstream>
#include <iostream>

using namespace std;

int main(){
    ifstream in("prueba.exe",ios::binary);
    ofstream out("pruebaGood.exe",ios::binary|ios::trunc);
    string file;
    char buff[1024];
    while(in){
        in.read(buff,1024);
        file += string(buff,in.gcount());
    }
    int count = 0;
    size_t pos = 0;
    while((pos = file.find("\xEB\x15\xE8\x10\x00\x00\x00\xE8\x0B\x00\x00\x00\xEB\x09\x90\xE8\x03\x00\x00\x00\xC3\xC3\x90\xE8\x03\x00\x00\x00\xC3\xEB\x05\x66\xFF\x04\x24\xC3", pos+1))!= string::npos){
        file[pos+28] = '\x8E';
        cout << pos << endl;
        ++count;
    }
    cout << "Found: " << count << endl;
    out.write(file.c_str(), file.size());
    in.close();
    out.close();
}
