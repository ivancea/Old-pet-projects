#include <iostream>
#include <windows.h>
#include <ctime>

#include "timers.h"
#include "http.h"

using namespace std;
using namespace http;

string getHour(){
    time_t t = time(NULL);
    tm tim = *localtime(&t);
    char c[20];
    return string(c, strftime(c, 20, "[%H:%M:%S]", &tim));
}

int main(){

    GETRequest r("https://foro.elhacker.net/unreadreplies.html");
    response re;
    while(true){
        cout << "Cookie: ";
        string cookie;
        getline(cin,cookie);

        r.setField("cookie",cookie);

        re = sendRequest(r);

        if(re.getBody().find("- Debes estar registrado -")!=re.getBody().npos){
            cout << "Error en la cookie." << endl << endl;
        }else break;
    }

    string t;
    cout << "Cada cuantos segundos se actualiza? ";
    getline(cin,t);

    uint32_t ms = atoi(t.c_str())*1000;
    if(ms==0) ms=10000;

    while(true){
        cout << getHour() << " Buscando..." << endl;
        if(re.getBody().find("No tienes mensajes...")==re.getBody().npos && re.getCode()==200){
            cout << getHour() << " Nuevos mensajes!!!" << endl << endl;
            Beep(440,300);
            thSleep(200);
            Beep(600,300);
            thSleep(200);
            Beep(700,300);
            thSleep(200);
            Beep(350,500);
            thSleep(ms);
        }else if(re.getCode()!=200)
            cout << getHour() << " Error en la conexion: " << re.getCode() << endl << endl;
        else cout << getHour() << " Sin mensajes nuevos." << endl << endl;
        thSleep(ms);
        re = sendRequest(r);
    }
}
