#include "sceneMenu.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "button.h"

#include <iostream>
using namespace std;

SceneMenu::SceneMenu(sf::RenderWindow* mainWindow)
:_mainWindow(mainWindow){
    sf::Event ev;
    while(_mainWindow->pollEvent(ev));

    sf::Vector2u size = _mainWindow->getSize();

    _buttonPlay.text = "Play";
    _buttonPlay.x = size.x/2-100;
    _buttonPlay.y = size.y/2-90;
    _buttonPlay.width = 200;
    _buttonPlay.height = 80;

    _buttonExit.text = "Exit";
    _buttonExit.x = size.x/2-100;
    _buttonExit.y = size.y/2+10;
    _buttonExit.width = 200;
    _buttonExit.height = 80;

    glViewport(0, 0, size.x, size.y);
    glOrtho(0,size.x, size.y,0,0,1);
}

SceneMenu::~SceneMenu(){}

Event SceneMenu::process(uint32_t deltaTime){
    sf::Vector2u size = _mainWindow->getSize();
    for(sf::Event ev; _mainWindow->pollEvent(ev);){
        switch(ev.type){
        case sf::Event::KeyPressed:
            if(ev.key.code == sf::Keyboard::Escape){
                return Event(EventType::Exit);
            }
            break;
        case sf::Event::MouseMoved:
            mouse.x = ev.mouseMove.x;
            mouse.y = ev.mouseMove.y;
            break;
        case sf::Event::Closed:
            return Event(EventType::Exit);
        default:
            break;
        }
        _buttonPlay.parseEvent(ev);
        _buttonExit.parseEvent(ev);
        if(_buttonPlay.wasPressed())
            return Event(EventType::ChangeScene, new pair<SceneType,void*>(SceneType::Game,nullptr));
        if(_buttonExit.wasPressed())
            return Event(EventType::Exit,nullptr);
    }
    return Event();
}

void SceneMenu::render(){
    if(_mainWindow==nullptr)
        return;

    _mainWindow->draw(_buttonPlay);
    _mainWindow->draw(_buttonExit);
}
