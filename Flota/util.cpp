#include "util.h"

sf::Font fontArial;

PieceMove::PieceMove(){}

PieceMove::PieceMove(sf::Vector2i from, sf::Vector2i to)
:from(from),to(to){}

PieceMove::PieceMove(int fromX, int fromY, int toX, int toY)
:from(fromX,fromY),to(toX,toY){}

PieceMove::PieceMove(const PieceMove& p)
:from(p.from),to(p.to){}

bool PieceMove::operator==(const PieceMove& p)const{
    return from.x == p.from.x
        && from.y == p.from.y
        && to.x == p.to.x
        && to.y == p.to.y;
}

bool PieceMove::operator!=(const PieceMove& p)const{
    return from.x != p.from.x
        || from.y != p.from.y
        || to.x != p.to.x
        || to.y != p.to.y;
}

size_t PieceMove::size()const{
    return sizeof(int)*4;
}

std::string PieceMove::serialice()const{
    return std::string((char*)&from.x,4)
         + std::string((char*)&from.y,4)
         + std::string((char*)&to.x,4)
         + std::string((char*)&to.y,4);
}

bool PieceMove::deserialice(std::string str){
    if(str.size()<this->size())
        return false;
    const char* temp = str.c_str();
    from.x = *(int*) temp;
    from.y = *(int*)(temp+sizeof(int));
    to.x =   *(int*)(temp+sizeof(int)*2);
    to.y =   *(int*)(temp+sizeof(int)*3);
    return true;
}
