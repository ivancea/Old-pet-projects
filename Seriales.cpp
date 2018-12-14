#include <iostream>
#include <cmath>
#include <ctime>
#include <map>

using namespace std;

uint32_t getRand(uint32_t newSeed=-1){
    static uint64_t n = 15368974050;
    if(newSeed!=-1) n=newSeed;
    n = (n+15368974177)*2%35984712771;
    return n;
}

string generateSerial(unsigned short seed){
    const string letters = "0123456789BCDFGHJKLMNPQRSTVWXYZ";
    string serial = to_string(seed);
    serial = string(5-serial.size(),'0')+serial;
    for(uint16_t i=0; i<serial.size(); i++)
        serial[i] = letters[serial[i]-'0'+10];
    getRand(seed);//srand(seed);
    for(uint16_t i=0; i<4; i++){
        serial += '-';
        uint32_t k = 3+getRand()%3;
        for(uint16_t j=0; j<k; j++)
            serial += letters[getRand()%letters.size()];
    }
    return serial;
}

bool validateSerial(const string& serial){
    const string letters = "0123456789BCDFGHJKLMNPQRSTVWXYZ";
    if(serial.size()<21 || serial.size()>29) return false;
    string t = serial.substr(0,5);
    for(auto c:t)
        if(c<'B' || c>'M')
            return false;
    for(uint16_t i=0; i<t.size(); i++){
        size_t pos = letters.find(t[i]);
        if(pos==letters.npos) return false;
        t[i] = pos+'0'-10;
    }
    unsigned short seed = stoi(t);
    return serial==generateSerial(seed);
}

int main(){
    /*cout << "Introduzca el serial: " << endl;
    while(true){
        string t;
        cin >> t;
        cout << (validateSerial(t)?"Verdadero":"Falso") << endl;
    }*/
    /**for(uint32_t i=0; i<50; i++)
        cout << generateSerial(i) << endl;**/
    map<string,bool> m;
    for(uint32_t i=0; i<65536; i++)
        m[generateSerial(i)];
    cout << m.size();
}
