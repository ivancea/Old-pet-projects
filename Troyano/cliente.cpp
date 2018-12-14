#include "C:/includes/ivancea.h"

void enviar(string a, client *c){
    a.erase(0,8);
    cout << "Enviando \"" << a << "\"..." << endl;
    clock_t s = clock();
    c->sendFile(a);
    cout << "Archivo enviado. (" << float(clock()-s)/CLOCKS_PER_SEC << "segundos transcurridos)" << endl;
}

void remoteCmd(client *c){
    string g;
    while(g!="exit"){
        while((g=c->recv<string>())=="") thSleep(100);
        cout << g <<">";
        getline(cin, g);
        c->send(g);
        if(g == "exit") return;
        g = c->recv<string>();
        while(g!="ENDOFSTREAM"){
            cout << g;
            g = c->recv<string>();
        }
    }
}

void ask(client *a){
    string t;
    while((t = a->recv<string>())== "");
    cout << endl << "Respuesta:" << t << endl;
}

int main(){
    cout << "Conectando con el servidor...";
    client a("85.84.155.31", 27015);
    cout << endl << "-Conectado-" << endl << endl;
    string h;
    do{
        if(contains(h, "/enviar ", 0)) enviar(h, &a);
        if(contains(h, "/ask", 0)) ask(&a);
        if(h=="/cmd") remoteCmd(&a);
        if(h=="/exit") break;
        if(h=="/fondo") a.sendFile("fondo.bmp");
        getline(cin, h, '\n');
    }while(a.send(h));
    return 0;
}
