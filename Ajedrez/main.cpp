#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "board.h"

using namespace std;


int main(){
    sf::RenderWindow window(sf::VideoMode(500,500),"Chess");
    window.setFramerateLimit(60);
    window.setActive();
    Board b(8,8, 50, "");
    b.addClass("DLL.dll", "Test");
    sf::Image* img;
    sf::Texture tx;
    img = b.draw();

    while(window.isOpen()){
        tx.loadFromImage(*img);
        window.draw(sf::Sprite(tx));
        window.display();

        sf::Event ev;
        while(window.pollEvent(ev)){
            switch(ev.type){
            case sf::Event::EventType::Closed:
                window.close();
                break;
            }
        }
    }
    return 0;
}
