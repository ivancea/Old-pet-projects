#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "timers.h"

using namespace std;

struct item{
    sf::Vector2f pos, v;
    sf::Color c;
    item(sf::Vector2f _pos=sf::Vector2f(0,0), sf::Vector2f _v=sf::Vector2f(0,0), sf::Color _c = sf::Color(0,0,0)):pos(_pos), v(_v), c(_c){}
};

class physics{
    uint16_t _pixelSize;
    float _acelleration, _maxVelocity;
    vector<item> _items;
public:
    physics(uint16_t pixelSize=0, float acelleration=0, float maxVelocity=0):_acelleration(acelleration), _maxVelocity(maxVelocity){if(pixelSize<1) _pixelSize=1; else _pixelSize = pixelSize;}
    void addItem(float x, float y, sf::Color c = sf::Color(0,0,0), sf::Vector2f v=sf::Vector2f(0,0)){_items.push_back(item(sf::Vector2f(x,y), v, c));}
    void moveTo(float x, float y){
        float temp = 0;
        if(_acelleration)
            for(int i=0; i<_items.size(); i++){
                _items[i].v.x *= 0.97;
                _items[i].v.y *= 0.97;
                _items[i].v += sf::Vector2f(-(_items[i].pos.x-x)*(_acelleration/20),-(_items[i].pos.y-y)*(_acelleration/20));
                if(_maxVelocity)
                    if((temp=sqrt(_items[i].v.x*_items[i].v.x + _items[i].v.y*_items[i].v.y))>_maxVelocity){
                        _items[i].v = sf::Vector2f(_maxVelocity*_items[i].v.x/temp, _maxVelocity*_items[i].v.y/temp);
                    }
                _items[i].pos += _items[i].v;
            }
    }
    sf::Image get(int x, int y){
        sf::Image img;
        img.create(x, y, sf::Color(255,255,255));
        for(int n=0; n<_items.size(); n++)
            for(int i=-_pixelSize/2; i<=_pixelSize/2; i++)
                for(int j=-_pixelSize/2; j<=_pixelSize/2; j++)
                    if(_items[n].pos.x+i>=0 && _items[n].pos.x+i < x && _items[n].pos.y+j>=0 && _items[n].pos.y+j<y)
                        img.setPixel(_items[n].pos.x+i, _items[n].pos.y+j, _items[n].c);
        return img;
    }
};

int main(){
    srand(time(0));
    physics ph(7, 0.5, 0);

    /*int elements = 0;
    while(elements<=0){
        cout << "Elements: ";
        string t;
        getline(cin,t);
        elements = stoi(t);
    }

    for(int i=0; i<elements; i++)
        ph.addItem(rand()%2000-600, rand()%2000-700, sf::Color(rand()%256,rand()%256,rand()%256));*/
    ph.addItem(1,1, sf::Color(255,0,0));
    ph.addItem(20,20, sf::Color(0,0,255));
    ph.addItem(800,200, sf::Color(0,255,255));
    ph.addItem(300,2, sf::Color(255,0,255));
    ph.addItem(99,40, sf::Color(255,255,0));
    ph.addItem(500,230);
    sf::RenderWindow window;
    window.create(sf::VideoMode(800,600), "Titulo");
    sf::Texture t;
    sf::Sprite s;
    sf::Vector2f last(0,0);
    while (window.isOpen()){
        clock_t cl = clock();
        ph.moveTo(last.x, last.y);
        t.loadFromImage(ph.get(800,600));
        window.draw(sf::Sprite(t));
        window.display();
        sf::Event event;
        if (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            break;

            case sf::Event::MouseMoved:
                last = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            break;

            default:
            break;
            }

        }
        thSleep(20);
    }
    return 0;
}
