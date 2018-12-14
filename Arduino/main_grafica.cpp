#include <iostream>
#include <list>

#include "ivancea.h"
#include "timers.h"

#include "serial.h"

using namespace std;

map<uint64_t,uint32_t> m;

Serial se("\\\\.\\COM3");

#define MAPX 1000
#define MAPY 600


uint32_t getValue(uint8_t media=1){
    uint32_t times = 0;
    uint32_t total = 0;
    while(true){
        se.writeData(string("\x04\x0E"));
        string t = se.readDataLength(2,100);
        if(t.size()!=2) return 0;
        total += *(uint16_t*)(void*)t.c_str();
        ++times;
		if(times==media) return total/media;
	}
	return 0;
}

void clearImage(sf::Image& img, list<uint32_t>& l){
    uint32_t n=0, count=0;
    for(uint32_t t:l){
            if(count==0 && t<MAPY)
                img.setPixel(count, t, sf::Color::White);
            else{
                if(n<t){
                    for(uint32_t i=n; i<=t; i++)
                        if(i<MAPY)
                            img.setPixel(count, i, sf::Color::White);
                        else break;
                }else{
                    for(uint32_t i=n; i>=t; i--)
                        if(i<MAPY)
                            img.setPixel(count, i, sf::Color::White);
                        else break;
                }
            }
            ++count;
            n = t;
        }
}

int main(){
    sf::RenderWindow window;
    sf::Texture t;
    sf::Image img;
    img.create(MAPX,MAPY,sf::Color::White);
    window.create(sf::VideoMode(MAPX,MAPY), "Grafica");

    list<uint32_t> l(MAPX,0);

    se.writeData("\x00\x0E\x01"); // PINMODE A0 INPUT

    while (window.isOpen()){
        clearImage(img, l);
        l.pop_front();
        l.push_back(((getValue()-300)*10+300)/**(1024.0/double(MAPY))*/);
        cout << l.back() << endl;
        int n=0, count=0;
        for(uint32_t t:l){
            if(count==0 && t<MAPY)
                img.setPixel(count, t, sf::Color::Black);
            else{
                if(n<t){
                    for(uint32_t i=n; i<=t; i++)
                        if(i<MAPY)
                            img.setPixel(count, i, sf::Color::Black);
                        else break;
                }else{
                    for(uint32_t i=n; i>=t; i--)
                        if(i<MAPY)
                            img.setPixel(count, i, sf::Color::Black);
                        else break;
                }
            }
            ++count;
            n = t;
        }
        t.loadFromImage(img);
        window.draw(sf::Sprite(t));
        window.display();
        sf::Event event;
        if (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            break;

            /**case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Space){
                }
            break;**/

            default:
            break;
            }
        }
    }
}
