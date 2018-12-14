#include "C:/includes/ivancea.h"

/*int main(){
    client c;
    string a = "ivancea.hol.es";
    cout << "Conectando a " << a << "..." << endl;
    while(!c.start(a, 80));
    cout << "Conectado" << endl;
    a = "GET / HTTP/1.1\nHost: ivancea.hol.es\n\n";
    while(!c.send(a));
    cout << "Paquete enviado." << endl;
    while(true)
        cout << c.recv<string>();
    return 0;
}*/

int main(){
    client a;
    string p, ip = "192.168.1.1";
    while(!a.start(ip, 80));
    cout << "Conectando con " << ip << "..." << endl;
    p = "GET / HTTP/1.1\r\nHost: 192.168.1.1\r\n\r\n";
    while(a.getSocket().send(p.c_str(), p.length())!=sf::Socket::Done);
    cout << "Peticion enviada." << endl;
    while(true)
            cout << a.recvChars();
    return 0;
}

