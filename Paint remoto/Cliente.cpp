#include "C:\includes\ivancea.h"
#include <iostream>
#include <thread>

using namespace std;

class Line{

public:

    sf::Vector2<uint16_t> p1, p2;
    uint16_t Grosor;
    sf::Color color;

    Line(sf::Vector2<uint16_t> po1,sf::Vector2<uint16_t> po2, uint16_t Grosoor, sf::Color color1){
        p1 = po1;
        p2 = po2;
        Grosor = Grosoor;
        color = color1;
    }

    Line(const Line& l){
        p1 = l.p1;
        p2 = l.p2;
        Grosor = l.Grosor;
        color = l.color;
    }

    void toImage(sf::Image &Imagen){
        float dX,dY,Pasos;
        float xInc,yInc,i,x,y;

        dX = p1.x - p2.x;
        dY = p1.y - p2.y;

        if (fabs(dX) > fabs(dY)){
            Pasos = fabs(dX);
        }else
            Pasos = fabs(dY);

        xInc = dX/Pasos;
        yInc = dY/Pasos;

        x = p1.x;
        y = p1.y;

        /** PIXEL INICIAL **/
        Imagen.setPixel(x,y,color);

        for (i=1; i<=Pasos; i++){
            x -= xInc;
            y -= yInc;

            /** GROSOR **/
            if(Grosor > 1)
            for (int j=x-Grosor/2; j<=x+Grosor/2; j++)
                for (int z=y-Grosor/2; z<=y+Grosor/2; z++)
                    if (!(j<0 || j>Imagen.getSize().x || z<0 || z>Imagen.getSize().x))
                        Imagen.setPixel(j,z,color);
            //Imagen.setPixel(x,y,color);
        }
    }

    void toPixelVector(pixelArray &pixeles){

        pixeles.clear();

        float dX,dY,Pasos;
        float xInc,yInc,i,x,y;

        dX = p1.x - p2.x;
        dY = p1.y - p2.y;

        if (fabs(dX) > fabs(dY)){
            Pasos = fabs(dX);
        }else{
            Pasos = fabs(dY);
        }

        xInc = dX/Pasos;
        yInc = dY/Pasos;

        x = p1.x;
        y = p1.y;

        for (i=1; i<=Pasos; i++){
            pixeles.add(x,y,color);
            x -= xInc;
            y -= yInc;
        }
    }
};

    //std::string IP = "84.122.114.5";
    std::string IP = "85.84.242.76";
    int Puerto = 27015;
    sf::Packet paquete;

    uint16_t width, height;
    sf::Color bgcolor = sf::Color(255,255,255,255);

    uint16_t Grosor = 3;
    bool Dibujando = false;
    sf::Vector2<uint16_t> AntiguaPos;
    sf::Image Imagen;
    sf::Texture Textura;

    Line linea(sf::Vector2<uint16_t>(0,0),sf::Vector2<uint16_t>(0,0),0,sf::Color(0,0,0,255));

    client Cliente;

void RecibirDatos(){

    sf::Packet packet;
    uint16_t x,y;
    uint8_t r,g,b,a;
    while(true){
    if(Cliente.recv(packet)){

        packet >> x >> y >> r >> g >> b >> a;
        //cout << "Paquete recibido: " << (int)x << " " << (int)y << " " << (int)r << " " << (int)g << " " << (int)b << " " << (int)a << endl;

        Imagen.setPixel(x,y,sf::Color(r,g,b,a));
        packet.clear();
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(){

    string respuesta;
    cout << "IP: ";
    cin >> IP;

    cout << "Conectandose a " << IP << " : " << Puerto << endl;

    while(!Cliente.start(IP,Puerto));

    cout << "Recibiendo datos de la pizarra" << endl;

    while(!Cliente.recv(paquete));
    paquete >> width >> height >> bgcolor.r >> bgcolor.g >> bgcolor.b >> bgcolor.a;
    paquete.clear();

    //width = 500; height = 500; bgcolor = sf::Color(255,255,255,255);

    Imagen.create(width,height,bgcolor);

    while(!Cliente.recv(paquete));

    sf::Color ColorTemp;

    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++){
            paquete >> ColorTemp.r >> ColorTemp.g >> ColorTemp.b >> ColorTemp.a;
            Imagen.setPixel(i,j,ColorTemp);
        }

    paquete.clear();

    cout << "Pizarra creada, a dibujar!" << endl;

    thread Recepcion(&RecibirDatos);
    Recepcion.detach();

    sf::RenderWindow window(sf::VideoMode(width,height,32), "PepoPintame", sf::Style::Close);
    window.setFramerateLimit(60);
    //sf::RenderWindow toolbar(sf::VideoMode(96,640,32), "Toolbar", sf::Style::Close);

    sf::Color myColor = sf::Color::Red;

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                Cliente.getSocket().disconnect();
                window.close();
            break;

            case sf::Event::MouseButtonPressed:
                Dibujando = true;
            break;

            case sf::Event::MouseButtonReleased:
                Dibujando = false;
                AntiguaPos = sf::Vector2<uint16_t>(0,0);
            break;

            case sf::Event::KeyPressed:

                if (event.key.code == sf::Keyboard::Escape){
                    Imagen.create(width,height,bgcolor);
                }

            default:
            break;
            }

        }

        uint16_t Mousex, Mousey;
        Mousex = (uint16_t)sf::Mouse::getPosition(window).x;
        Mousey = (uint16_t)sf::Mouse::getPosition(window).y;

    if (Dibujando && Mousex >= 0 && Mousex < width && Mousey >= 0 && Mousey < height){
        if (AntiguaPos.x != 0 && AntiguaPos.y != 0){
            if(Mousex != AntiguaPos.x || Mousey != AntiguaPos.y){

            linea = Line(sf::Vector2<uint16_t>(AntiguaPos.x,AntiguaPos.y),sf::Vector2<uint16_t>(Mousex,Mousey),Grosor,myColor);
            linea.toImage(Imagen);

            paquete << (uint16_t)AntiguaPos.x << (uint16_t)AntiguaPos.y <<  (uint16_t)Mousex << (uint16_t)Mousey << (uint16_t)Grosor << (uint8_t)myColor.r << (uint8_t)myColor.g << (uint8_t)myColor.b << (uint8_t)myColor.a;
            Cliente.send(paquete);
            //cout << "Paquete enviado: " << (int)AntiguaPos.x << " " << (int)AntiguaPos.y << " " << (int)Mousex << " " << (int)Mousey << " " << (int)Grosor << " " << (int)myColor.r << " " << (int)myColor.g << " " << (int)myColor.b << " " << (int)myColor.a << endl;
            paquete.clear();
            }
        }
        AntiguaPos = sf::Vector2<uint16_t>(Mousex, Mousey);
    }

    Textura.loadFromImage(Imagen);
    window.clear(bgcolor);
    window.draw(sf::Sprite(Textura));

    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
    return 0;
}
