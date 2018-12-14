#include "C:\includes\ivancea.h"

uint16_t width=500, height=500, grosor=1;
uint8_t red=255, green=255, blue=255, alpha=255;

int paint(sf::Image &img, uint8_t now, uint16_t &xA, uint16_t &yA, uint16_t &xB, uint16_t &yB){
    int temp = 0;
    switch(now/16){
    case 1: if(yA-1>=width){++temp; break;}
        if(img.getPixel(xA, yA-1)!=sf::Color(red, green, blue, alpha)) {++temp; break;}
        --yA; break;
    case 2: if(xA+1>=width){++temp; break;}
        if(img.getPixel(xA+1, yA)!=sf::Color(red, green, blue, alpha)) {++temp; break;}
        ++xA; break;
    case 3: if(yA+1>=width){++temp; break;}
        if(img.getPixel(xA, yA+1)!=sf::Color(red, green, blue, alpha)) {++temp; break;}
        ++yA; break;
    case 4: if(xA-1>=width){++temp; break;}
        if(img.getPixel(xA-1, yA)!=sf::Color(red, green, blue, alpha)) {++temp; break;}
        --xA; break;
    }
    switch(now&15){
    case 1: if(yB-1>=width){temp+=2; break;}
        if(img.getPixel(xB, yB-1)!=sf::Color(red, green, blue, alpha)) {temp+=2; break;}
        --yB; break;
    case 2: if(xB+1>=width){temp+=2; break;}
        if(img.getPixel(xB+1, yB)!=sf::Color(red, green, blue, alpha)) {temp+=2; break;}
        ++xB; break;
    case 3: if(yB+1>=width){temp+=2; break;}
        if(img.getPixel(xB, yB+1)!=sf::Color(red, green, blue, alpha)) {temp+=2; break;}
        ++yB; break;
    case 4: if(xB-1>=width){temp+=2; break;}
        if(img.getPixel(xB-1, yB)!=sf::Color(red, green, blue, alpha)) {temp+=2; break;}
        --xB; break;
    }
    img.setPixel(xA, yA, sf::Color(0,0,0,0));
    img.setPixel(xB, yB, sf::Color(0,0,0,0));
    return temp;
}

void base(server *srv, uint8_t a, uint8_t b){
    uint16_t xA=width/4, yA=height/2, xB=(width*3)/4, yB=height/2; // Iniciar las coordenadas, y enviarlas al cliente
    uint8_t last=0, now=0, t=0;
    int fin=0;
    sf::Image img;
    img.create(width, height, sf::Color(red, green, blue, alpha));/** Iniciar valores al cliente **/
    {
        sf::Packet pA, pB, pA2, pB2;
        pA << xA << yA << (uint8_t)2; // El 2 y el 4 es la direccion inicial
        pB << xB << yB << (uint8_t)4;
        pA2 = pB;
        pB2 = pA;
        paint(img, 0, xA, yA, xB, yB);
        srv->send(pA, a);
        srv->send(pA2, a);
        srv->send(pB, b);
        srv->send(pB2, b);
    }
    while(fin==0){
        last = now;
        now=0; fin=0;
        thSleep(100);  /** Enviar los datos de forma 0xAB, donde A es al que le mandas **/
        if(t=srv->recv<uint8_t>(a))
            if((t==1 && last/16!=3) || (t==2 && last/16!=4)
              || (t==3 && last/16!=1) || (t==4 && last/16!=2))
                now+=t*16;
        if(t=srv->recv<uint8_t>(b))
            if((t==1 && last&15!=3) || (t==2 && last&15!=4)
              || (t==3 && last&15!=1) || (t==4 && last&15!=2))
                now+=t;
        /** Ahora "now" tiene guardadas las direcciones que recibió **/
        /**  Y "last" tiene las direcciones antiguas **/
        fin = paint(img, now, xA, yA, xB, yB);
        if(fin==1) now|=240;
        else if(fin==2) now|=15;
        else if(fin==3) now=255;
        srv->send(now, a);
        srv->send((now&15)*16+now/16, b);
    }
}

int main(){
    sf::Packet p;
    server srv;
    while(!srv.start(27015));
    cout << "Servidor iniciado. Puerto " << srv.getPort() << "." << endl;
    srv.setData("0");
    while(true){
        thSleep(50);
        if(srv.newClient(100)){/** Aqui emparejar de 2 en 2 a los contrincantes **/
            p.clear();        /** Y crear un thread para ellos. Cuando acaben, el thread se cierra **/
            cout << "Nuevo cliente con IP: \"" << srv.getIp(srv.getClients()-1) << "\"." << endl;
            cout << char(200) << "Cliente numero: " << srv.getClients() << "." << endl;
            p << width << height << grosor << red << green << blue << alpha;
            srv.send(p, srv.getClients()-1);
            if(srv.getClients()%2==0)
                thread(&base, &srv, srv.getClients()-1, srv.getClients()-2)->detach();
        }
    }
    return 0;
}
