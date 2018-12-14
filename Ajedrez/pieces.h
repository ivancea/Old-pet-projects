#ifndef PIECES_H
#define PIECES_H

#include <cstring>
#include <SFML/Graphics.hpp>
#include <windows.h>

typedef bool(*CanMoveFunction)(uint16_t x, uint16_t y, uint16_t **board,
                               bool player, uint16_t pieceX, uint16_t pieceY,
                               uint16_t toX, uint16_t toY);

struct Piece{
    bool team;   // 1 or bigger
    uint16_t pClass; // Board's vector index

    Piece(uint16_t _team, uint16_t _pClass):team(_team),pClass(_pClass){};
};

struct PieceClass{
    std::string name;
    sf::Image imgW, imgB;
    HINSTANCE dll;
    CanMoveFunction canMove;

    PieceClass(std::string _name,
               sf::Image _imgW,
               sf::Image _imgB,
               HINSTANCE _dll,
               CanMoveFunction _canMove
               ):name(_name),imgW(_imgW),imgB(_imgB),dll(_dll),canMove(_canMove){}

    ~PieceClass(){
        if(dll!=0)
            FreeLibrary(dll);
    }
};

#endif // PIECES_H
