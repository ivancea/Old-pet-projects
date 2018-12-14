#include "mapCreator.h"

#include "block.h"
#include "vec.h"

#include <fstream>

using namespace std;

std::fstream file;


template<class T>
void write(T var){
    file.write((char*)&var, sizeof(T));
}

template<class T>
T read(){
    T var;
    file.read((char*)&var, sizeof(T));
    return var;
}

template<class T>
void read(T &var){
    file.read((char*)&var, sizeof(T));
}


template<class T>
void writeVec2(Vec2<T> var){
    write(var.x);
    write(var.y);
}

template<class T>
Vec2<T> readVec2(){
    Vec2<T> var;
    read<T>(var.x);
    read<T>(var.y);
    return var;
}

template<class T>
void readVec2(Vec2<T> &var){
    read<T>(var.x);
    read<T>(var.y);
}


void writeBlock(Block *b){
    writeVec2(b->getPos());
    write(b->canCollide());
}

Block* readBlock(){
    Vec2i pos = readVec2<int>();
    bool canCollide = read<bool>();
    return new Block(pos, canCollide, 0);
}

bool toFile(Block ***m, int mX, int mY,
            Vec2d player, Vec2d camera, string fileName){
    file.open(fileName, ios::out|ios::trunc|ios::binary);
    if(!file.is_open())
        return false;

    write(mX);
    write(mY);
    writeVec2(Block::size);
    writeVec2(player);
    writeVec2(camera);
    size_t pos = file.tellg();
    write<int>(0);
    int nBlocks = 0;
    for(int i=0; i<mX; i++)
        for(int j=0; j<mY; j++)
            if(m[i][j]){
                ++nBlocks;
                writeBlock(m[i][j]);
            }
    file.seekg(pos);
    write(nBlocks);

    file.close();
    return true;
}

bool fromFile(Block ***&m, int &mX, int &mY,
              Vec2d &player, Vec2d &camera, string fileName){
    file.open(fileName, ios::in|ios::binary);
    if(!file)
        return false;

    read(mX);
    read(mY);
    readVec2(Block::size);
    readVec2(player);
    readVec2(camera);

    m = new Block**[mX];
    for(int i=0; i<mX; i++){
        m[i] = new Block*[mY];
        for(int j=0; j<mY; j++)
            m[i][j] = 0;
    }

    int nBlocks = read<int>();
    for(int i=0; i<nBlocks; i++){
        Block* b = readBlock();
        if(b->getPos().x>=0 && b->getPos().x<mX &&
           b->getPos().y>=0 && b->getPos().y<mY)
            m[b->getPos().x][b->getPos().y] = b;
        else delete b;
    }

    file.close();

    return true;
}


/************

Map X,Y
Block size
Player Pos
Camera Pos

N_BLOCKS
{BLOCKS}(N)

*************/
