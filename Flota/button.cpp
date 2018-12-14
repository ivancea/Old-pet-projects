#include "button.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "util.h"

Button::Button(std::string _text, int _x, int _y, uint32_t _width, uint32_t _height, uint32_t _textSize,
               sf::Color _normalColor, sf::Color _hoverColor,
               sf::Color _pressedColor, sf::Color _textColor)
:_mouseHover(false), _buttonPressed(false), _wasPressed(false), text(_text), x(_x), y(_y), width(_width), height(_height), textSize(_textSize), normalColor(_normalColor),
hoverColor(_hoverColor), pressedColor(_pressedColor), textColor(_textColor){
}


void Button::draw(sf::RenderTarget& rt, sf::RenderStates rs)const{

    if(_buttonPressed)
        glColor3ub(pressedColor.r, pressedColor.g, pressedColor.b);
    else if(_mouseHover)
        glColor3ub(hoverColor.r, hoverColor.g, hoverColor.b);
    else
        glColor3ub(normalColor.r, normalColor.g, normalColor.b);

    glBegin(GL_POLYGON);
        glVertex2i(x, y);
        glVertex2i(x+width, y);
        glVertex2i(x+width, y+height);
        glVertex2i(x, y+height);
    glEnd();
    rt.pushGLStates();
        sf::Text textButton;
        textButton.setFont(fontArial);
        textButton.setString(text);
        textButton.setColor(textColor);
        textButton.setCharacterSize(textSize);
        sf::FloatRect bounds = textButton.getGlobalBounds();
        textButton.setPosition(x+width/2-bounds.width/2, y+height/2-bounds.height);
        rt.draw(textButton);
    rt.popGLStates();
}

void Button::parseEvent(const sf::Event& event){
    switch(event.type){
    case sf::Event::MouseMoved:
        _mouseHover = (event.mouseMove.x>x && event.mouseMove.x<x+width &&
                       event.mouseMove.y>y && event.mouseMove.y<y+height);
        break;
    case sf::Event::MouseButtonPressed:
        if(event.mouseButton.button == sf::Mouse::Left
           && _mouseHover){
           _buttonPressed = true;
           _wasPressed = true;
        }
        break;
    case sf::Event::MouseButtonReleased:
        if(event.mouseButton.button == sf::Mouse::Left)
           _buttonPressed = false;
        break;
    }
}

bool Button::isHover()const{
    return _mouseHover;
}

bool Button::isPressed()const{
    return _buttonPressed;
}

bool Button::wasPressed(){
    bool temp = _wasPressed;
    _wasPressed = false;
    return temp;
}
