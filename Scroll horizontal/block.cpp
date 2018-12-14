#include "block.h"

#include <GL/glfw.h>

#include "vec.h"
#include <random>

Vec2d Block::size(1,1);

Block::Block(Vec2i pos, bool canCollide, GLuint texture):_pos(pos),_size(size),_canCollide(canCollide),_texture(texture){
}

void Block::draw(){
    double x=_pos.x*size.x,
            y=_pos.y*size.y;
    glColor3ub(0,255,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex2d(x, y);
    glTexCoord2f(1.,0);
    glVertex2d(x+size.x, y);
    glTexCoord2f(1.,1.);
    glVertex2d(x+size.x, y+size.y);
    glTexCoord2f(0,1.);
    glVertex2d(x, y+size.y);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    /*glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x+20, y);
    glVertex2d(x+20, y+20);
    glVertex2d(x, y+20);
    glEnd();*/
}


void Block::setTexture(GLuint texture){ _texture = texture; }

const Vec2i Block::getPos(){ return _pos; }
const bool Block::canCollide(){ return _canCollide; }
const GLuint Block::getTexture(){ return _texture; }
