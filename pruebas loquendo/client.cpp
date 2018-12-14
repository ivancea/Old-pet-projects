#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "sockets.h"
#include "sockets.cpp"

using namespace std;

string toWords(uint64_t n){
    static const string num[] = {"cero", "una" /* "uno" para masculino */, "dos", "tres", "cuatro", "cinco",
                                 "seis", "siete", "ocho", "nueve", "diez", "once",
                                 "doce", "trece", "catorce", "quince", "dieciseis",
                                 "diecisiete", "dieciocho", "diecinueve", "veinte"};
    static const string ten[] = {"cero", "dieci", "veinti", "treinta", "cuarenta",
                                 "cincuenta", "sesenta", "setenta", "ochenta", "noventa"};
    string t;
    if(n==0)
        return num[0];
    if(n<=20 && n>0){
        return t + num[n];
    }
    if(n>20 && n<30){
        return ten[n/10]+num[n%10];
    }
    if(n/10>0){
        t += ten[n/10];
        n%=10;
        if(n>0)
            t += " y ";
    }
    if(n>0){
        t += num[n];
    }
    return t;
}

int main(){
    int lastMinute=60;
    while(true){
        time_t tim;
        tm *tstr;
        time(&tim);
        tstr = localtime(&tim);

        if(tstr->tm_min==lastMinute || tstr->tm_min%5 != 0){
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }
        lastMinute = tstr->tm_min;

        cout << '<' << tstr->tm_hour << ':' << (tstr->tm_min/10>0?"":"0") << tstr->tm_min << '>' << endl;

        if(tstr->tm_hour>12 || (tstr->tm_hour==12 && tstr->tm_min>30))
            tstr->tm_hour -= 12;
        if(tstr->tm_min>30)
            tstr->tm_hour += 1;

        string toRead = (tstr->tm_hour!=1?"Son las ":"Es la ") + toWords(tstr->tm_hour);
        if(tstr->tm_min == 0)
            toRead += " en punto";
        else if(tstr->tm_min == 30)
            toRead += " y media";
        else if(tstr->tm_min > 30)
            toRead += " menos " + (tstr->tm_min==45?"cuarto":toWords(60-tstr->tm_min));
        else
            toRead += " y " + (tstr->tm_min==15?"cuarto":toWords(tstr->tm_min));

        TCPClient client;
        client.connect("127.0.0.1", 666);
        uint16_t chars = toRead.size();
        client.send(string((char*)&chars, 2));
        client.send(toRead);
    }
}
