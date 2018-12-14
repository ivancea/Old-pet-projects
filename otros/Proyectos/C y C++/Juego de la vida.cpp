#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>

using namespace std;

inline void thSleep(unsigned int ms){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

class lifeGame{
    uint16_t _live, _born;
    bool **_game;
    uint32_t _x, _y, _pixelSize;
    sf::Color _screen, _pixel;

    bool scan(uint32_t xx, uint32_t yy){
        uint8_t count=0;
        long long x=xx, y=yy;
        for(int i=-1; i<2; i++)
            for(int j=-1; j<2; j++)
                if(x+i>=0 && x+i<_x && y+j>=0 && y+j<_y && (i||j))
                    if(_game[x+i][y+j]) ++count;
        if(_game[x][y])
            if(_live&(2<<count))
                return true;
            else return false;
        else
            if(_born&(2<<count))
                return true;
            else return false;
        return false;
    }

public:
    lifeGame(uint32_t x, uint32_t y, uint32_t pixelSize=1, sf::Color screen=sf::Color(255,255,255), sf::Color pixel=sf::Color(0,0,0)):_x(x), _y(y), _pixelSize(pixelSize), _live(0), _born(0), _screen(screen), _pixel(pixel){
        _game = new bool*[x];
        for(uint32_t i=0; i<x; i++)
            _game[i] = new bool[y];
        for(uint32_t i=0; i<_x; i++)
            for(uint32_t j=0; j<_y; j++)
                _game[i][j] = false;
    }

    void setRules(uint16_t live, uint16_t born){
        _live = live;
        _born = born;
    }

    void reset(uint32_t x=0, uint32_t y=0, uint32_t pixelSize=0){
        if(_y>1)
            for(uint32_t i=0; i<_x; i++)
                delete[] _game[i];
        else if(y==1)
            for(uint32_t i=0; i<_x; i++)
                delete _game[i];
        if(_x>1)
            delete[] _game;
        else if(_x==1)
            delete _game;
        if(x>0) _x = x;
        if(y>0) _y = y;
        _game = new bool*[_x];
        for(uint32_t i=0; i<_x; i++)
            _game[i] = new bool[_y];
        if(pixelSize>0) _pixelSize = pixelSize;
        for(uint32_t i=0; i<_x; i++)
            for(uint32_t j=0; j<_y; j++)
                _game[i][j] = false;
    }

    sf::Image get(){
        sf::Image img;
        img.create(_x*_pixelSize, _y*_pixelSize, _screen);
        for(uint32_t i=0; i<_x; i++)
            for(uint32_t j=0; j<_y; j++)
                if(_game[i][j])
                    for(uint32_t n=0; n<_pixelSize; n++)
                        for(uint32_t m=0; m<_pixelSize; m++)
                            img.setPixel((i*_pixelSize)+n, (j*_pixelSize)+m, _pixel);
                else
                    for(uint32_t n=0; n<_pixelSize; n++)
                        for(uint32_t m=0; m<_pixelSize; m++)
                            img.setPixel((i*_pixelSize)+n, (j*_pixelSize)+m, _screen);
        return img;
    }

    void next(){
        bool **temp = new bool*[_x];
        for(uint32_t i=0; i<_x; i++)
            temp[i] = new bool[_y];
        for(uint32_t i=0; i<_x; i++)
            for(uint32_t j=0; j<_y; j++)
                temp[i][j] = scan(i, j);
        for(uint32_t i=0; i<_x; i++)
            for(uint32_t j=0; j<_y; j++)
                _game[i][j] = temp[i][j];
        if(_y>1)
            for(uint32_t i=0; i<_x; i++)
                delete[] temp[i];
        else if(_y==1)
            for(uint32_t i=0; i<_x; i++)
                delete temp[i];
        if(_x>1)
            delete[] temp;
        else if(_x==1)
            delete temp;
    }

    void setPixel(uint32_t x, uint32_t y, bool state){
        if(x<_x && y<_y)
            _game[x][y] = state;
    }
    bool setPixel(uint32_t x, uint32_t y){
        if(x<_x && y<_y){
            if(_game[x][y])
                _game[x][y] = false;
            else
                _game[x][y] = true;
            return _game[x][y];
        }
        return false;
    }
    void setPixelSize(uint32_t pixelSize){if(pixelSize) _pixelSize = pixelSize;}
    uint32_t getPixelSize(){return _pixelSize;}
};

#define X 50
#define Y 50
#define P 15

int main(){
    lifeGame lg(X,Y,P);
    lg.setRules(0b1100, 0b1000);
    sf::RenderWindow window;
    window.create(sf::VideoMode(X*P,Y*P), "Titulo");
    sf::Texture t;
    bool changed = false, nexted = false, wait = false, first = false, pressed = false;
    (new thread([](bool *n, bool *w){while(true){while(*w)thSleep(50); thSleep(250); *n = true;}},&nexted, &wait))->detach();
    while (window.isOpen()){
        if(changed||nexted){
            if(nexted) lg.next();
            t.loadFromImage(lg.get());
            window.draw(sf::Sprite(t));
            window.display();
            changed = false;
            nexted = false;
        }
        sf::Event event;
        if (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            break;

            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Button::Left){
                    first = lg.setPixel(event.mouseButton.x/lg.getPixelSize(), event.mouseButton.y/lg.getPixelSize());
                    changed = true;
                    pressed = true;
                }
            break;

            case sf::Event::MouseButtonReleased:
                if(event.mouseButton.button == sf::Mouse::Button::Left){
                    pressed = false;
                }
            break;

            case sf::Event::MouseMoved:
                if(pressed){
                    lg.setPixel(event.mouseMove.x/lg.getPixelSize(), event.mouseMove.y/lg.getPixelSize(), first);
                    changed = true;
                }
            break;

            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Space){
                    if(wait)
                        wait = false;
                    else
                        wait = true;
                }else if(event.key.code == sf::Keyboard::R){
                    lg.reset();
                    changed=true;
                }

            default:
            break;
            }

        }
        thSleep(20);
    }
    return 0;
}
