#include <iostream>
#include <thread>
#include "C:\includes\ivancea.h"

using namespace std;

void funcChat(client *a){
    string s;
    while((s = a->recv<string>())!="CHAT\nEND" && s!="")
        cout << s << endl;
    cout << "<CHAT: Sesion de chat privada finalizada>" << endl;
    a->setData("NORMAL", 0);
}

void toChat(client *a){
    a->setData("CHAT", 0);
    string r;
    string c = a->recv<string>();
    if(c=="SERVER"){cout << "El servidor quiere mantener una conversación privada contigo." << endl; r = "A";}
    else if(c=="WAIT\nRESPONSE"){cout << "Esperando respuesta..." << endl; r = "A";}
    else cout << "El cliente " << c << " quiere chatear contigo.\nAceptar(A) / Rechazar(R): ";
    while(r!="A" && r!="a" && r!="R" && r!="r")
        getline(cin, r);
    if(r=="R" || r=="r"){a->send<string>("CHAT\nREJECT"); return;}
    else a->send("CHAT\nACCEPT");
    cout << "<INFO: Escribe \"/fin\" para terminar el chat>" << endl;
    thread charla(&funcChat, a);
    charla.detach();
    while((r!="/fin" || c=="SERVER") && a->getData(0) == "CHAT"){
        getline(cin, r);
        if(a->getData(0) == "CHAT")
            a->send(r);
    }
    a->setData("NORMAL", 0);
}

void func(client *a){
    string t;
    while(true)
        if((t =a->recv<string>())!="")
            if(t == "CHAT\nREQUEST") toChat(a);
            else cout << t << endl;
}

int main(){
    string msg;
    client a;
    cout << "IP: ";
    getline(cin,msg);
    cout << "Conectando al servidor..." << endl;
    while(!a.start(msg, 27015));
    cout << "-Conectado-" << endl << endl;
    cout << "Hay " << a.recv<int>() << " personas conectadas." << endl;
    a.setData("NORMAL", 0);
    thread th(&func, &a);
    th.detach();
    while(true){
        while(a.getData(0) == "NORMAL"){
            getline(cin, msg);
            if(a.getData(0) == "CHAT") break;
            if(contains(msg, "/chat ", 0)){
                if(!a.send<string>("CHAT\nASK") || !a.send<string>(msg)) cout << "<ERROR: No se ha podido enviar el mensaje>\n(Servidor desconectado?)" << endl;
                break;
            }
            if(!a.send<string>("NORMAL\nMSG") ||!a.send<string>(msg)) cout << "<ERROR: No se ha podido enviar el mensaje>\n(Servidor desconectado?)" << endl;
        }
    }
    return 0;
}
