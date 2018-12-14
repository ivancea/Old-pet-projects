#include <iostream>
#include <thread>

#include "ivancea.h"
#include "strings.h"

using namespace std;

void conexion(server *s, int n){
    string *temp, t, ruta, a, body;
    int bodySize = 0, code = 200;
    while(true)while(true){
        bodySize = 0; code = 200;
        t = ""; ruta = ""; a = ""; body = "";
        while((t = s->recvChars(n))=="");
        split(&temp, t, '\n');
        ruta = temp[0];
        delete[] temp;
        if(!contains(t, "GET /", 0)) code = 400;
        split(&temp, ruta, ' ');
        ruta = temp[1];
        delete[] temp;
        if(ruta == "/") ruta = "default.html";
        if(ruta[0]=='/') ruta.erase(0,1);
        ifstream file(ruta, ios::in | ios::binary);
        if(file.fail()) code = 404;
        file.close();
        file.open(ruta, ios::in | ios::ate | ios::binary);
        bodySize = file.tellg();
        file.close();
        if(contains(ruta, ".jpg") || contains(ruta, ".png") || contains(ruta, ".mp3") || contains(ruta, ".rar") || contains(ruta, ".ico")) code = 403;
        if(code!=200 || bodySize<0) bodySize = 0;
        a = "HTTP/1.1 " + parseInt(code) + " OK\r\n";
        a+= "Content-Length: " + parseInt(bodySize) + "\r\n";
        a+= "\r\n";
        for(int i=0; i<a.length(); i+=50)
            while(!s->sendChars(a.substr(i, 50), n));
        if(code != 200){
            cout << "-Peticion de \"" + s->getSocket(n).getRemoteAddress().toString() + "\" para \"" + ruta + "\".  Codigo " << code << endl << endl;
            break;
        }
        file.open(ruta, ios::in | ios::binary);
        while(!file.eof()){
            getline(file, t);
            for(int i=0; i<t.length(); i+=50)
                while(!s->sendChars(t.substr(i, 50), n));
        }
        file.close();
        cout << "-Peticion de \"" + s->getSocket(n).getRemoteAddress().toString() + "\" para \"" + ruta + "\"." << endl << endl;
    }
}

int main(){
    string a, body;
    thread *t;
    server s(27015);
    int nc = 0;
    cout << "Servidor conectado al puerto 27015. Escuchando..." << endl;
    /*while(true){
        if(s.newClient(10)){
            t = new thread(&conexion, &s, nc);
            ++nc;
            t->detach();
        }
    }*/
    while(true) cout << s.recvChars(0);
    body = "<html>Hola</html>";
    a = "HTTP/1.1 200 OK\r\n";
    //a+= "Content-Length: " + parseInt(body.size()) + "\r\n";
    a+= "\r\n";
    a+= body;
    while(s.getSocket(0).send(a.c_str(), a.size())!=sf::Socket::Done);
    cout << "Petición de ";
    cin >> a;
    return 0;
}
