#include <iostream>
#include <vector>

#include "ivancea.h"
#include "canciones.h"
#include "selector.h"

using namespace std;

class mapa{
    vector<string> _map;
    int _tamX, _tamY, _pixelSize, _moved, _blocks, _type;
    double _x, _y;
    double _velocity, _maxVelocity;
    double _grav;
    sf::Color _player, _walls, _background;
    int _generate, _generate2;

    inline string voidString()const{
        string t = "1";
        for(int i=0; i<_tamY-2; i++)
            t+='0';
        t+='1';
        return t;
    }

    inline string makeTunnel(int index, int size){
        string t = voidString();
        for(int i=1; i<index; i++)
            t[i]='1';
        for(int i=index+size; i<_tamY-1; i++)
            t[i]='1';
        return t;
    }

    string generateRandom()const{
        string t = voidString();
        for(int c = _blocks; c>0 && _tamY-2-_blocks+c>0; c--){
            int r = rand()%(_tamY-2-_blocks+c);
            for(int i=1+r; i<_tamY-1; i++)
                if(t[i]=='0'){
                    t[i]='1';
                    break;
                }
        }
        return t;
    }

    string generateHoles(){
        if(_generate>=10 || _generate<0){
            _generate=0;
            return makeTunnel(1+rand()%(_tamY-2-_blocks),_blocks);
        }else ++_generate;
        return voidString();
    }

    string generateTunnel(){
        string t = voidString();
        if(_generate==0){
            _generate = rand()%(_tamY-_blocks-2) + 1;
            return makeTunnel(_generate,_blocks);
        }
        if(_generate<=1)
            _generate+=rand()%2;
        else if(_generate>=_tamY-1-_blocks)
            _generate+=(rand()%2)-1;
        else
            _generate+=(rand()%3)-1;
        return makeTunnel(_generate,_blocks);
    }

    string generateAsteroids(){
            string t = voidString();
        if(_generate==0){
            _generate2 = rand()%(_tamY-_blocks-2);
        }
        if(_generate<_blocks){
            for(int i=0;i<_blocks;i++)
                if(_generate2+i+1<t.size())
                    t[_generate2+i+1]='1';
            _generate++;
        }else if(_generate>=_blocks*2-1)
            _generate=0;
        else ++_generate;
        return t;
    }

public:
    mapa(int tamX, int tamY, int pixelSize, double x, double y, double grav, double maxVelocity=0, int blocks=1, int type=0)
        :_tamX(tamX),_tamY(tamY),_pixelSize(pixelSize), _type(type),
         _x(x),_y(y),_velocity(0),_grav(grav),_moved(0), _blocks(blocks),
         _maxVelocity(maxVelocity),_player(sf::Color(50,125,255)),
         _walls(sf::Color(0,0,0)),_background(sf::Color(255,255,255)){
        srand(time(NULL));
        string t = voidString();
        for(int i=0; i<_tamX+1; i++)
            _map.push_back(t);

        _generate = 0;
        _generate2 = 0;
    }

    void restart(int tamX, int tamY, int pixelSize, double x, double y, double grav, double maxVelocity=0, int blocks=1, int type=0){
        _tamX=tamX;
        _tamY=tamY;
        _pixelSize=pixelSize;
        _type=type;
        _x=x;
        _y=y;
        _velocity=0;
        _grav=grav;
        _moved=0,
        _blocks=blocks;
        _maxVelocity=maxVelocity;
        _player=sf::Color(50,125,255);
        _walls=sf::Color(0,0,0);
        _background=sf::Color(255,255,255);
        srand(time(NULL));
        string t = voidString();
        _map.clear();
        for(int i=0; i<_tamX+1; i++)
            _map.push_back(t);
        _generate=0;
        _generate2 = 0;
    }

    int getSizeX()const{return _tamX;}
    int getSizeY()const{return _tamY;}
    int getPixelSize()const{return _pixelSize;}
    double getX()const{return _x;}
    double getY()const{return _y;}
    double getVelocity()const{return _velocity;}
    double getGravity()const{return _grav;}
    int getBlocks()const{return _blocks;}
    int getType()const{return _type;}

    void setX(double x){_x=x;}
    void setY(double y){_y=y;}
    void setVelocity(double velocity){_velocity=velocity;}
    void setGravity(double grav){_grav=grav;}
    void getBlocks(int blocks){_blocks=blocks;}
    void setWallsColor(sf::Color walls){_walls=walls;}
    void setWallsColor(uint8_t r,uint8_t g,uint8_t b){_walls = sf::Color(r,g,b);}
    void setPlayerColor(sf::Color player){_player=player;}
    void setPlayerColor(uint8_t r,uint8_t g,uint8_t b){_player = sf::Color(r,g,b);}
    void setBackgroundColor(sf::Color background){_background=background;}
    void setBackgroundColor(uint8_t r,uint8_t g,uint8_t b){_background = sf::Color(r,g,b);}
    void setType(int type){_type=type;_generate=0;_generate2=0;}
    void setBlocks(int blocks){_blocks=blocks;_generate=0;_generate2=0;}

    sf::Image get()const{
        sf::Image t;
        t.create(_tamX*_pixelSize,_tamY*_pixelSize,_background);
        for(int i=0; i<_map.size(); i++)
            for(int j=0; j<_tamY; j++)
                if(_map[i][j]=='1')
                    for(int n=-_moved; n<_pixelSize-_moved;n++)
                        for(int m=0; m<_pixelSize;m++)
                            if(t.getSize().x > i*_pixelSize + n && t.getSize().y > j*_pixelSize + m)
                                t.setPixel(i*_pixelSize + n, j*_pixelSize + m, _walls);
        for(int i=0; i<_pixelSize; i++)
            for(int j=0; j<_pixelSize; j++)
                if(_x+i<t.getSize().x && _y+j<t.getSize().y)
                    t.setPixel(_x+i,_y+j,_player);
        return t;
    }

    void get(sf::Image &img){
        for(int i=0; i<_map.size(); i++)
            for(int j=0; j<_tamY; j++)
                if(_map[i][j]=='1'){
                    for(int n=-_moved; n<_pixelSize-_moved;n++)
                        for(int m=0; m<_pixelSize;m++)
                            if(img.getSize().x > i*_pixelSize + n && img.getSize().y > j*_pixelSize + m)
                                img.setPixel(i*_pixelSize + n, j*_pixelSize + m, _walls);
                }else{
                    for(int n=-_moved; n<_pixelSize-_moved;n++)
                        for(int m=0; m<_pixelSize;m++)
                            if(img.getSize().x > i*_pixelSize + n && img.getSize().y > j*_pixelSize + m)
                                img.setPixel(i*_pixelSize + n, j*_pixelSize + m, _background);
                }
        for(int i=0; i<_pixelSize; i++)
            for(int j=0; j<_pixelSize; j++)
                if(_x+i<img.getSize().x && _y+j<img.getSize().y)
                    img.setPixel(_x+i,_y+j,_player);
    }

    int next(){
        if(_moved >= _pixelSize){
            _map.erase(_map.begin());
            switch(_type){
            case FULL:
                    _map.push_back(string(_tamY,'1'));
                break;
            case RANDOM:
                    _map.push_back(generateRandom());
                break;
            case HOLES:
                    _map.push_back(generateHoles());
                break;
            case TUNNEL:
                    _map.push_back(generateTunnel());
                break;
            case ASTEROIDS:
                    _map.push_back(generateAsteroids());
                break;
            default:
                    _map.push_back(voidString());
                break;
            }
            _moved -= _pixelSize;
        }else ++_moved;

        if(_maxVelocity)
            if(_velocity > _maxVelocity)
                _velocity = _maxVelocity;
            else if(_velocity<0 && -_velocity > _maxVelocity)
                _velocity = -_maxVelocity;
        _y += _velocity;
        _velocity += _grav;
        if(_y<0) _y = 0;
        else if(_y>=_tamY*_pixelSize) _y = (_tamY-1)*_pixelSize;

        for(int i=0; i<2; i++)
            for(int j=0; j<2; j++)
                if(_map[_x/_pixelSize + i][_y/_pixelSize + j]=='1')
                    return 1;
        return 0;
    }
    enum type{
        FULL=-1,
        NONE=0,
        RANDOM=1,
        HOLES=2,
        TUNNEL=3,
        ASTEROIDS=4
    };
};

int record(unsigned int now){
    ifstream f("FlyBoxRecord.icf",ios::binary);
    char *n;
    n = (char*)&now;
    if(!f){
        f.close();
        ofstream fi("FlyBoxRecord.icf",ios::trunc|ios::binary);
        fi << n[0] << n[1] << n[2] << n[3];
        fi.close();
        return now;
    }
    char t[4];
    f >> t[0] >> t[1] >> t[2] >> t[3];
    int te = *((int*)&t);
    if(te<now){
        f.close();
        ofstream fi("FlyBoxRecord.icf",ios::trunc|ios::binary);
        fi << n[0] << n[1] << n[2] << n[3];
        fi.close();
        return now;
    }
    f.close();
    return te;
}

#define MAPX 60
#define MAPY 40
#define PIXEL 10
#define GRAVITY 0.12
#define MAXVELOCITY 8

int main(){
    srand(time(NULL));
    int option = 0;
    {
        string opTitle = "BIENVENIDO al FlyBox!!!\n\n"
                          "ENTER: Reiniciar\nParar/Reanudar musica: P\n"
                          "Otra tecla: Volar/cambiar gravedad (Arriba y abajo en caso de modo levitar)\n"
                          "\nElige un modo de juego... (W/S)";
        options op;
        op.setTitle(opTitle);
        op.addOption("Modo vuelo");
        op.addOption("Modo gravedad");
        op.addOption("Modo levitar");
        option = op.show();
    }
    ShowWindow(GetConsoleWindow(),SW_HIDE);
    bool music=true;
    thread([&](){
        srand(time(NULL));
        void (*f[])(bool&,int) = {himnoAleman, himnoSovietico, tetris};
        while(true){
            f[rand()%3](music,250);
            thSleep(1000);
        }
    }).detach();
    int blocks = 8, type=mapa::HOLES, movimiento=0;
    uint32_t contador=0, cont2=PIXEL*10;
    bool muerto = false, justDied=false, space=false;
    mapa m(MAPX, MAPY, PIXEL, 5*PIXEL, MAPY*PIXEL/2, GRAVITY, MAXVELOCITY, blocks, type);
    sf::RenderWindow window;
    sf::Texture t;
    sf::Sprite s;
    sf::Text text;
    sf::Font font;
    sf::Image img;
    img.create(MAPX*PIXEL,MAPY*PIXEL);
    window.create(sf::VideoMode(MAPX*PIXEL,MAPY*PIXEL), "Titulo");
    font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
    text.setFont(font);
    text.setPosition(10,10);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color(255,255,50));

    while (window.isOpen()){
        if(!muerto)
            text.setString(to_string(contador/PIXEL));
        if(muerto && justDied){
            justDied = false;
            text.setString("¡"+text.getString()+" puntos!\nRecord: "+to_string(record(contador/PIXEL))+" puntos");
        }
        m.get(img);
        t.loadFromImage(img);
        window.draw(sf::Sprite(t));
        window.draw(text);
        window.display();

        if(option==2 && !muerto){ ///PARA MOVIMIENTO FLUCTUANTE
            if((contador+60)%40==0)
                m.setGravity(-m.getGravity());
            m.setY(m.getY()+movimiento*3);
        }

        if(!muerto){ /** CAMBIAR LAS FASES **/
            if(cont2>=PIXEL*MAPX){
                space = true;
                cont2 = 0;
                m.setType(mapa::NONE);
            }else if(space && cont2>=PIXEL*10){
                space=false;
                switch(rand()%4){
                default:
                case 0:
                    m.setType(mapa::TUNNEL);
                    m.setBlocks(12);
                    break;
                case 1:
                    m.setType(mapa::ASTEROIDS);
                    m.setBlocks(1);
                    break;
                case 2:
                    m.setType(mapa::HOLES);
                    m.setBlocks(8);
                    break;
                case 3:
                    m.setType(mapa::ASTEROIDS);
                    m.setBlocks(2);
                    break;
                }
                cont2=0;
            }else ++cont2;
        }

        if(m.next()&&!muerto){
            m.setBackgroundColor(255,50,25);
            m.setVelocity(0);
            m.setGravity(0);
            m.setType(mapa::FULL);
            justDied=true;
            muerto=true;
        }
        if(!muerto){
            ++contador;
            if(contador/(PIXEL*4)<255)
                m.setBackgroundColor(sf::Color(255,255-contador/(PIXEL*4),255-contador/(PIXEL*4)));
        }
        sf::Event event;
        if (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            break;

            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Key::Return){
                    m.restart(MAPX, MAPY, PIXEL, 5*PIXEL, MAPY*PIXEL/2, GRAVITY, MAXVELOCITY, blocks, type);
                    contador = 0;
                    muerto = false;
                    justDied=false;
                }else if(event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
                else if(event.key.code == sf::Keyboard::Key::P)
                    music = !music;
                else if(option==2 && event.key.code == sf::Keyboard::Key::Up) //PARA MOVIMIENTO FLUCTUANTE
                    movimiento=-1;
                else if(option==2 && event.key.code == sf::Keyboard::Key::Down)
                    movimiento=1;

                else
                if(option==0 && !muerto) m.setVelocity(m.getVelocity()-MAXVELOCITY);
                if(option==1 && !muerto) m.setGravity(-m.getGravity());  ///PARA CAMBIAR GRAVEDAD
            break;

            case sf::Event::KeyReleased: //PARA MOVIMIENTO FLUCTUANTE
                if(option==2 && (event.key.code == sf::Keyboard::Key::Up || event.key.code == sf::Keyboard::Key::Down))
                    movimiento=0;
            break;

            default:
            break;
            }

        }
        thSleep(5);
    }
    ShowWindow(GetConsoleWindow(),SW_SHOW);
    return 0;
}
