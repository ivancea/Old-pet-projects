#include "C:\includes\ivancea.h"

/************************************/
/** VALORES INICIALES DE LA IMAGEN **/
/************************************/
/**/                                  /**/
/**/    uint16_t  width = 500;       /**/
/**/    uint16_t height = 500;       /**/
/**/    uint8_t     red = 255;       /**/
/**/    uint8_t   green = 255;       /**/
/**/    uint8_t    blue = 255;       /**/
/**/    uint8_t   alpha = 255;       /**/
/**/                                  /**/
/************************************/
/************************************/



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


pixelArray getDiferences(sf::Image &after, sf::Image &before){
    pixelArray temp;
    for(uint16_t i=0; i<width; i++)
        for(uint16_t j=0; j<height; j++)
            if(after.getPixel(i, j) != before.getPixel(i, j))
                temp.add(i, j, after.getPixel(i, j));
    return temp;
}

void recibir(server *s, sf::Image *img, sf::Image *before){
    sf::Packet p;
    sf::Image t1, t2;
    bool changed=false;
    int n = 0;
    uint16_t x, y, x2, y2, gr;
    uint8_t r, g, b, a;
    t1 = *img; t2 = *before;
    Line li(sf::Vector2<uint16_t>(0,0), sf::Vector2<uint16_t>(0,0), 0, sf::Color(0,0,0,0));
    while(true){
        t1 = *img; t2 = *before;
        x = y = x2 = y2 = gr = 0;
        r = g = b = a = 0;
        n=s->getClients();
        do{
            for(int i=0; i<n; i++){
                p.clear();
                if(s->recv(p, i))
                    if(p.getDataSize()==14){
                        changed=true;
                        p >> x >> y >> x2 >> y2 >> gr >> r >> g >> b >> a;
                        //cout << endl << x << endl << y << endl  << x2 << endl  << y2 << endl
                        //     << gr << endl  << (int)r << endl  << (int)g << endl  << (int)b << endl  << (int)a << endl << endl;
                        if(!(x<width && y<height && x2<width && y2<height)) break;
                        li = Line(sf::Vector2<uint16_t>(x,y), sf::Vector2<uint16_t>(x2,y2), gr, sf::Color(r, g, b, a));
                        li.toImage(t1);
                    }
            }
            thSleep(10);
        }while(s->getData(0)!="0" && changed);
        *img = t1; //*before = t2;
        s->setData("1", 0);
        thSleep(10);
    }
}

void enviar(server *a, sf::Image *img, sf::Image *before){
    pixelArray pixArr;
    sf::Packet p;
    while(true){
        while(a->getData(0)!="1") thSleep(20);
        pixArr.clear();
        p.clear();
        pixArr = getDiferences(*img, *before);
        for(int i=0; i<pixArr.size(); i++){
            p.clear();
            if(pixArr.getPos(i).x<width && pixArr.getPos(i).y < height){
                p << pixArr.getPos(i).x << pixArr.getPos(i).y << pixArr.getColor(i).r
                  << pixArr.getColor(i).g << pixArr.getColor(i).b << pixArr.getColor(i).a;
                for(int j=0; j<a->getClients();j++){
                        a->send(p, j);
                }
            }else cout << endl << "<ERROR: Enviando coordenadas " << pixArr.getPos(i).x << " - " << pixArr.getPos(i).y << ">" << endl;
        }
        *before = *img;
        a->setData("0", 0);
        thSleep(20);
    }
}

void clientes(server *a, sf::Image *img){
    sf::Packet pBase, pTemp;
    pBase << width << height << red << green << blue << alpha;
    pixelArray pixArr;
    sf::Image imgBase;
    imgBase.create(width, height, sf::Color(red, green, blue, alpha));
    while(true){
        pTemp.clear();
        thSleep(50);
        if(a->newClient(50)){
            cout << "<INFO: Cliente conectado. (" + a->getIp(a->getClients()-1) + ")>\n";
            pTemp = pBase;
            a->send(pTemp, a->getClients()-1);
            /*pixArr = getDiferences(*img, imgBase);
            for(int i=0; i<pixArr.size(); i++){
                pTemp.clear();
                pTemp << pixArr.getPos(i).x << pixArr.getPos(i).y << pixArr.getColor(i).r
                      << pixArr.getColor(i).g << pixArr.getColor(i).b << pixArr.getColor(i).a;
                      a->send(pTemp, a->getClients()-1);
            }*/
            for(int i=0; i<width; i++)
                for(int j=0; j<height;j++)
                    pTemp << img->getPixel(i, j).r << img->getPixel(i, j).g
                          << img->getPixel(i, j).b << img->getPixel(i, j).a;
            a->send(pTemp, a->getClients()-1);
        }
    }
}

int main(){
    int puerto = 27015;
    server a;
    sf::Image img, before;
    while(!a.start(27015));
    cout << "Servidor iniciado.  -  (Puerto: " << puerto << ")." << endl;
    a.setData("0");
    img.create(width, height, sf::Color(red, green, blue, alpha));
    before = img;
    thread re(&recibir, &a, &img, &before);
    thread en(&enviar, &a, &img, &before);
    thread cl(&clientes, &a, &img);
    re.detach();
    en.detach();
    cl.detach();
    while(true) thSleep(2000);
    /**
    sf::Image asd; // Calcular tiempo del algoritmo.
    sf::Image asdf;
    for(int i=0; i<1000; i++){
    if(i%10==0) cout << i;
    asd.create(500, 500, sf::Color(200, 200, 200, 200));
    asdf.create(500, 500, sf::Color(100, 100, 100, 100));
    asd.setPixel(5,5,sf::Color(100,100,100,100));
    pixelArray pixelA = getDiferences(asdf, asd);
    }**/
    return 0;
}

/* Servidor -> Cliente
Packet << uint16_t X << uint16_t Y
       << uint8_t red << uint8_t green
       << uint8_t blue << uint8_t alpha;
*/

/* Cliente -> Servidor
Packet << uint16_t X << uint16_t Y
       << uint16_t X2 << uint16_t Y2
       << uint16_t Gr << uint8_t red
       << uint8_t green << uint8_t blue
       << uint8_t alpha;
*/

/**
-Thread 1, (ENVIAR): Enviar a clientes.
-Thread 2, (RECIBIR): Dibujar en la imagen con los envios de los clientes.
-Thread 3, (CLIENTES): Recibir nuevos clientes.
    -CONEXIÓN: Server (a) Data (0) -> 0(Imagen sin montar) / 1(Imagen montada)
**/
