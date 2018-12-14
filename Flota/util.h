#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <SFML/Graphics.hpp>

extern sf::Font fontArial;

struct PieceMove{
    sf::Vector2i from;
    sf::Vector2i to;

    PieceMove();
    PieceMove(sf::Vector2i from, sf::Vector2i to);
    PieceMove(int fromX, int fromY, int toX, int toY);

    PieceMove(const PieceMove&);

    bool operator==(const PieceMove&)const;
    bool operator!=(const PieceMove&)const;

    size_t size()const;
    std::string serialice()const;
    bool deserialice(std::string);
};

#endif // UTIL_H
