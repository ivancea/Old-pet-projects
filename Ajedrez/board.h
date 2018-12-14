#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "pieces.h"

class Board{
    std::vector<PieceClass*> _classes;

    uint16_t _x, _y, _squareSize;
    Piece ***_board;

public:
    Board(uint16_t x, uint16_t y, uint16_t squareSize, std::string kingPath);
    ~Board();
    int addClass(std::string path, std::string pieceName, CanMoveFunction canMoveF = 0); // -1 if error. If exists, return index

    const Piece* operator()(uint16_t x, uint16_t y);
    const PieceClass* getClass(uint16_t i);
    uint16_t getClassCount();

    bool move(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY); // False if can't move
    void forceMove(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY);

    sf::Image* draw();
};

#endif // BOARD_H
