#ifndef SCENEMENU_H
#define SCENEMENU_H

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scene.h"

class SceneMenu:public Scene{
    sf::RenderWindow* _mainWindow;
    sf::Vector2u mouse;

    Button _buttonPlay,
           _buttonExit;

public:
    SceneMenu(sf::RenderWindow* mainWindow);
    virtual ~SceneMenu();

    virtual Event process(uint32_t deltaTime);
    virtual void render();
};

#endif // SCENEMENU_H
