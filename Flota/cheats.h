#ifndef CHEATS_H
#define CHEATS_H

#include <SFML/Graphics.hpp>
#include <vector>

typedef void (CheatCallback)(const std::vector<sf::Keyboard::Key>&, void* data);

class Cheats{
public:
    void clear();
    void erase(std::vector<sf::Keyboard::Key> cheat);
    void add(std::vector<sf::Keyboard::Key> cheat, CheatCallback* callback, void* data);

    int parseEvent(const sf::Event& event);
};

#endif // CHEATS_H
