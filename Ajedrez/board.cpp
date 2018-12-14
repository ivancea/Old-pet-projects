#include "board.h"

#include <windows.h>
#include <SFML/Graphics.hpp>
#include <cstring>

#include "pieces.h"


using namespace std;

template<class T>
T** loadMatrix(uint16_t x, uint16_t y){
    T** mat = new T*[x];
    for(uint16_t i=0; i<x; i++){
        mat[i] = new T[y];
        for(int j=0; j<y; j++)
            mat[i][j] = T();
    }
    return mat;
}

void paste(sf::Image* to, sf::Image* source, uint16_t x, uint16_t y, uint16_t sqSize){
    for(uint16_t i=0; i+x<to->getSize().x && i<source->getSize().x && i<sqSize; i++)
        for(uint16_t j=0; j+y<to->getSize().y && j<source->getSize().y && j<sqSize; j++){
            to->setPixel(i+x,j+y, source->getPixel(i,j));
        }
}

bool kingCanMoveFunction(uint16_t x, uint16_t y, uint16_t **board, bool player, uint16_t pieceX, uint16_t pieceY, uint16_t toX, uint16_t toY){
    if((pieceX+1==toX || toX+1==pieceX || toX==pieceX)
       && (pieceY+1==toY || toY+1==pieceY || toY==pieceY))
       return true;

       /**
            ENROQUE
       **/

    return false;
}

Board::Board(uint16_t x, uint16_t y, uint16_t squareSize, string kingPath):_x(x),_y(y),_squareSize(squareSize){
    if(x>0 && y>0)
        _board = loadMatrix<Piece*>(x,y);
    addClass(kingPath, "king", &kingCanMoveFunction);
}

Board::~Board(){
    for(int i=0; i<_x; i++){
        for(int j=0; j<_y; j++)
            if(_board[i][j]!=nullptr)
                delete _board[i][j];
        if(_x>1)
            delete[] _board[i];
        else
            delete _board[i];
    }
    if(_y>1)
        delete[] _board;
    else
        delete _board;
    for(PieceClass* c:_classes)
        if(c!=0)
            delete c;
}

int Board::addClass(string path, string pieceName, CanMoveFunction canMoveF){
    for(size_t i=0; i<_classes.size(); i++)
        if(_classes[i]->name==pieceName)
            return i;

    HINSTANCE hDll = 0;
    if(canMoveF==0){
        hDll = LoadLibrary((path+pieceName+".dll").c_str());
        if(hDll==NULL) return -1;
        canMoveF = (CanMoveFunction)GetProcAddress(hDll, "canMoveFunction");
        if(!canMoveF){
            FreeLibrary(hDll);
            return -1;
        }
    }

    sf::Image imgW, imgB;
    imgW.loadFromFile(path+pieceName+"W.bmp");
    imgB.loadFromFile(path+pieceName+"B.bmp");

    for(int i=0; i<pieceName.size(); i++)
        pieceName[i] = tolower(pieceName[i]);

    _classes.push_back(new PieceClass(pieceName, imgW, imgB, hDll, canMoveF));
    return _classes.size()-1;
}

const PieceClass* Board::getClass(uint16_t i){
    if(_classes.size()>i)
        return _classes[i];
    return 0;
}

uint16_t Board::getClassCount(){
    return _classes.size();
}

const Piece* Board::operator()(uint16_t x, uint16_t y){
    if(_x>x && _y>y)
        return _board[x][y];
    return 0;
}

bool Board::move(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY){
    if(!_x || !_y || _board[fromX][fromY]==0
       || fromX>=_x || fromY>=_y
       || toX>=_x || toY>=_y)
        return false;

    uint16_t **simpleBoard = loadMatrix<uint16_t>(_x,_y);

    for(uint16_t i=0; i<_x; i++)
        for(uint16_t j=0; j<_y; j++){

        }

    if(_classes[_board[fromX][fromY]->pClass]->canMove(_x,_y, simpleBoard, _board[fromX][fromY]->team, fromX,fromY, toX,toY)){
        if(_board[fromX][fromY]!=0)
            delete _board[fromX][fromY];
        _board[fromX][fromY] = 0;
        _board[toX][toY] = _board[fromX][fromY];
        return true;
    }
    return false;
}

void Board::forceMove(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY){
    if(_board[fromX][fromY]!=0)
        delete _board[fromX][fromY];
    _board[fromX][fromY] = 0;
    _board[toX][toY] = _board[fromX][fromY];
}

sf::Image* Board::draw(){
    if(!_x||!_y)
        return 0;
    sf::Image* img = new sf::Image();
    img->create(1+_x*(_squareSize+1),1+_y*(_squareSize+1));
    sf::Image white;
    white.create(_squareSize, _squareSize, sf::Color(255,255,255));

    for(uint16_t i=0; i<_y; i++)
        for(uint16_t j=0; j<_x; j++)
            if(_board[j][i]){
                paste(img, (_board[j][i]->team?&_classes[_board[j][i]->pClass]->imgB:&_classes[_board[j][i]->pClass]->imgW),
                      1+j*(_squareSize+1), 1+i*(_squareSize+1), _squareSize);
            }else{
                paste(img, &white, 1+j*(_squareSize+1), 1+i*(_squareSize+1), _squareSize);
            }
    return img;
}
