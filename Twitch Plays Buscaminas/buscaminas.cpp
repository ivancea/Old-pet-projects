#include <iostream>
#include <SFML/Graphics.hpp>

#include "irc.h"
#include "timers.h"
#include "strings.h"

class buscaminas:public sf::Drawable{
    vector<string> _mapa;
    vector< vector<sf::Text> > _numeros;
    sf::Font _font;
    int _x, _y;
    int _numMinas;
    int _abiertas;
    int _size;
    bool _seeAll;

public:
    enum{
        MINA='2',
        ABIERTO='1',
        CERRADO='0',
        FALLO='e'
    };
    buscaminas(int x, int y, int numMinas, int size=20){
        start(x,y,numMinas,size);
    }

    void start(int x, int y, int numMinas, int size=20){
        _mapa.clear();
        _numeros.clear();
        _x=x;
        _y=y;
        _numMinas=numMinas;
        _abiertas=0;
        _size=size;
        _seeAll=false;
        string t;
        x=_x;
        y=_y;
        _font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
        if(numMinas>=x*y){
            t.resize(x*y,MINA);
        }else{
            t.resize(x*y,CERRADO);
            for(int i=0; i<numMinas; i++){
                int z = rand()%(x*y-i);
                int n=-1;
                for(int j=0; z>=0; j++,z--){
                    if(t[j]==MINA){
                        ++z;
                    }
                    ++n;
                }
                t[n] = MINA;
            }
        }
        for(int i=0; i<x; i++){
            _mapa.push_back(t.substr(0,y));
            t.erase(0,y);
        }
        for(int i=0; i<x; i++){
            _numeros.push_back(vector<sf::Text>(y,sf::Text("",_font, _size)));
            for(int j=0; j<y; j++){
                _numeros[i][j].setString(to_string(getNum(i,j)));
                _numeros[i][j].setPosition(i*_size+i+1+_size/5,j*_size+j+1-_size/8);
                _numeros[i][j].setColor(getNumberColor(getNum(i,j)));
            }
        }
        /*for(int j=0; j<y; j++){
            for(int i=0; i<x; i++)
                cout << _mapa[i][j];
            cout << endl;
        }*/
    }

    int getNumMinas()const{return _numMinas;}
    int getX()const{return _x;}
    int getY()const{return _y;}
    int getSize()const{return _size;}
    bool isSetSeeAll()const{return _seeAll;}
    void setSize(int size){_size=size;}
    void setSeeAll(bool seeAll){_seeAll=seeAll;}

    sf::Vector2f getWindowSize(){return sf::Vector2f(1+_size*_x+_x,1+_size*_y+_y);}

    char getCell(int x, int y)const{
        if(x>=_x||y>=_y || x<0||y<0) return FALLO;
        return _mapa[x][y];
    }

    int getNum(int x, int y)const{
        int count=0;
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++)
                if(x+i>=0 && x+i<_x && y+j>=0 && y+j<_y && (i||j))
                    if(_mapa[x+i][y+j]==MINA)
                        ++count;
        return count;
    }

    bool abrir(int x, int y){
        if(x<0||y<0 || x>=_x||y>=_y)
            return false;

        if(_mapa[x][y]==MINA)
            return true;
        else{
            ++_abiertas;
            _mapa[x][y]=ABIERTO;
            if(getNum(x,y)==0){
                for(int i=-1; i<=1; i++)
                    for(int j=-1; j<=1; j++)
                        if(x+i>=0 && x+i<_x && y+j>=0 && y+j<_y && (i||j))
                            if(_mapa[x+i][y+j]==CERRADO)
                                abrir(x+i,y+j);
            }
        }
        return false;
    }

    bool isFinished()const{return !(_x*_y-_numMinas-_abiertas);}

    virtual void draw(sf::RenderTarget& rt, sf::RenderStates rs)const{
        sf::Texture txMina, txAbierto, txCerrado;
        sf::Image img;
        sf::Sprite mina, cerrado, abierto;
        {
            sf::Image bgImg;
            sf::Texture bg;
            bgImg.create(_size*_x+_x+1, _size*_y+_y+1, sf::Color(40,40,40));
            bg.loadFromImage(bgImg);
            rt.draw(sf::Sprite(bg));/** Pintar base **/
        }

        txMina.loadFromFile("mina.bmp");
        mina.setTexture(txMina);/** mina **/
        mina.scale(_size/txMina.getSize().x,_size/txMina.getSize().y);

        img.create(_size,_size,sf::Color(200,200,200));
        txAbierto.loadFromImage(img);
        abierto.setTexture(txAbierto);/** abierto **/

        img.create(_size,_size,sf::Color(120,120,120));
        txCerrado.loadFromImage(img);
        cerrado.setTexture(txCerrado);/** cerrado **/

        for(int i=0; i<_x; i++)
            for(int j=0; j<_y; j++){
                switch(_mapa[i][j]){
                case MINA:
                    if(_seeAll){
                        mina.setPosition(i*_size+i+1,j*_size+j+1);
                        rt.draw(mina);
                        break;
                    }
                case CERRADO:
                    cerrado.setPosition(i*_size+i+1,j*_size+j+1);
                    rt.draw(cerrado);
                    break;

                case ABIERTO:
                    abierto.setPosition(i*_size+i+1,j*_size+j+1);
                    rt.draw(abierto);
                    if(getNum(i,j)>0){
                        rt.draw(_numeros[i][j]);
                    }
                    break;
                }
            }
    }

    static sf::Color getNumberColor(int n){
        switch(n){
            case 1:return sf::Color::Blue;
            case 2:return sf::Color::Green;
            case 3:return sf::Color::Red;
            case 4:return sf::Color::Magenta;
            case 5:return sf::Color::Black;
            case 6:return sf::Color(30,30,30);
            case 7:return sf::Color::Yellow;
            case 8:return sf::Color::Cyan;
        }
    }
};


#define MAPX 20
#define MAPY 20
#define NUMMINAS 50
#define SIZE 20

const string nick="ivancea96";
const string pass="";
const string channel="ivancea96";

sf::Vector2i mousePos;
bool reloading = false;

void readUserInput(buscaminas& bm);
void ircChannelCallback(Irc &ir, string channel, string sender, string msg, void* data);


int main(){
    srand(time(NULL));
    buscaminas bm(MAPX,MAPY,NUMMINAS,SIZE);
    Irc ir;
    while(!ir.start("", pass, nick, "irc.twitch.tv", 6667)) thSleep(1);
    ir.setAutoPong(true);
    ir.setChannelMsgCallback(&ircChannelCallback, &bm);
    sf::RenderWindow window;
    window.create(sf::VideoMode(1+MAPX+SIZE*MAPX,1+MAPY+MAPY*SIZE), "Buscaminas (By Ivancea)");
    window.clear();
    window.draw(bm);
    thread(&readUserInput,ref(bm)).detach();
    while (window.isOpen()){
        window.clear();
        window.draw(bm);
        window.display();
        sf::Event event;
        if (window.pollEvent(event)){
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseMoved:
                mousePos.x = event.mouseMove.x;
                mousePos.y = event.mouseMove.y;
                break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left){
                    if(reloading)
                        break;
                    int x = mousePos.x/(SIZE+1),
                        y = mousePos.y/(SIZE+1);
                    if(bm.abrir(x,y)){
                        bm.setSeeAll(true);
                        reloading = true;
                        callbackAfter<buscaminas*>(5000, [](buscaminas* b){
                            b->start(b->getX(),b->getY(),b->getNumMinas(), b->getSize());
                            reloading = false;
                        }, &bm);
                    }
                }
                break;
            default:
            break;
            }
        }
        thSleep(20);
    }
    return 0;
}

void ircChannelCallback(Irc &ir, string channel, string sender, string msg, void* data){
    if(reloading)
        return;
    buscaminas *bm = (buscaminas*)data;
    msg = trim(msg, " ,.()-_\n\r");
    vector<string> v = split(msg, ',');
    if(v.size()==2){
        int x = stoi(trim(v[0])), y = stoi(trim(v[1]));
        if(bm->abrir(x-1,y-1)){
            bm->setSeeAll(true);
            reloading = true;
            callbackAfter<buscaminas*>(5000, [](buscaminas* b){
                b->start(b->getX(),b->getY(),b->getNumMinas(), b->getSize());
                reloading = false;
            }, bm);
        }
    }
}

void readUserInput(buscaminas& bm){
    cout << endl << endl;
    while(true){
        string t;
        int x=0 ,y=0;
        cin >> t;
        if(t.size()>=3 && t.size()<=5){
            if(t[1]==','){
                if(bm.abrir(stoi(t.substr(0,1))-1,stoi(t.substr(2,t.size()))-1)){
                    callbackAfter<buscaminas*>(5000, [](buscaminas* b){
                        b->start(b->getX(),b->getY(),b->getNumMinas(), b->getSize());
                        reloading = false;
                    }, &bm);
                }
            }else if(t[2]==','){
                if(bm.abrir(stoi(t.substr(0,2))-1,stoi(t.substr(3,t.size()))-1)){
                    callbackAfter<buscaminas*>(5000, [](buscaminas* b){
                        b->start(b->getX(),b->getY(),b->getNumMinas(), b->getSize());
                        reloading = false;
                    }, &bm);
                }
            }
        }
        while(reloading) thSleep(50);
    }
}

void readIRCInput(buscaminas&bm, irc& ir){
    string s;
    while(true){
        while(!ir.receive(s)){
            thSleep(10);
            if(bm.isFinished() || bm.isSetSeeAll()){
                thSleep(5000);
                bm.start(bm.getX(),bm.getY(),bm.getNumMinas(), bm.getSize());
                while(ir.receive(s));
            }
        }
        vector<string> v = split(s, ' ', 2);
        if(v.size()==2 && contains(v[1], "PRIVMSG", 0)){
            v = split(v[1], ':');
            string coords = trim(v[1]);
            if(coords.size()<=5 && coords.size()>=3){
                v = split(coords, ',');
                if(v.size()==2){
                    int x=stoi(v[0]), y=stoi(v[1]);
                    if(bm.abrir(x-1,y-1)){
                        bm.setSeeAll(true);
                    }
                }
            }
        }
    }
}
