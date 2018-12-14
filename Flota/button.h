#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <SFML/Graphics.hpp>

class Button:public sf::Drawable{
    bool _mouseHover, _buttonPressed, _wasPressed;

public:
    std::string text;
    int x,y;
    uint32_t width, height,
        textSize;
    sf::Color normalColor,
              hoverColor,
              pressedColor,
              textColor;

    Button(std::string _text = "", int _x = 0, int _y = 0, uint32_t _width = 0, uint32_t _height = 0, uint32_t _textSize = 20,
           sf::Color _normalColor = sf::Color::Yellow, sf::Color _hoverColor = sf::Color::Green,
           sf::Color _pressedColor = sf::Color::Cyan, sf::Color _textColor = sf::Color::Black);



    virtual void draw(sf::RenderTarget&, sf::RenderStates)const;

    void parseEvent(const sf::Event&);

    bool isHover()const;
    bool isPressed()const;
    bool wasPressed();
};

#endif // BUTTON_H
