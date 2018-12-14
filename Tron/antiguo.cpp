#include "C:\includes\ivancea.h"
#include <windows.h>

void paint(server *srv, sf::Image *img, uint8_t *c){
}

void base(server *srv, sf::Image *img){
    uint8_t m[16]={0}, last[16]={0}, temp[16]={0}, t;
    while(true){
        thSleep(200);  /** Variar este "thSleep" para diminuir la velocidad **/
        for(int i=0; i<srv->getClients(); i++)
            if(t=srv->recv<uint8_t>(i) && m[i]){
                m[i] = 0;
                if((t==1 && last[i]!=3) || (t==2 && last[i]!=4)
                   || (t==3 && last[i]!=1) || (t==4 && last[i]!=2)){
                    last[i] = t;
                    m[i] = i + t*16;
                   }
            }
        for(int i=0; i<srv->getClients(); i++)
            if(m[i])
            for(int j=0; j<srv->getClients(); j++)
                srv->send(m[i], j);
        for(int i=0; i<16; i++)
            temp[i] = m[i];
        paint(srv, img, temp);
    }
}

int main(){
    FreeConsole();
    sf::Packet p;
    uint16_t width=500, height=500, grosor=1;
    uint8_t red=255, green=255, blue=255, alpha=255;
    server srv;
    while(!srv.start(27015));
    cout << "Servidor iniciado. Puerto " << srv.getPort() << "." << endl;
    srv.setData("0");
    sf::Image img;
    img.create(width, height, sf::Color(red, green, blue, alpha));
    thread tr(&base, &srv, &img);
    tr.detach();
    while(true){
        thSleep(100);
        if(srv.newClient(20)){
            p.clear();
            cout << "Nuevo cliente con IP: \"" << srv.getIp(srv.getClients()-1) << "\"." << endl;
            cout << char(200) << "Cliente numero: " << srv.getClients() << "." << endl;
            p << width << height << grosor << red << green << blue << alpha;
            srv.send(p, srv.getClients()-1);
        }
    }
    return 0;
}
