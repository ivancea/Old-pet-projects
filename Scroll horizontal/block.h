#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glfw.h>

#include "vec.h"
#include "drawable.h"

class Block:public Drawable{
    Vec2i _pos;
    Vec2d _size;
    bool _canCollide;
    GLuint _texture;

public:
    Block(Vec2i pos, bool canCollide, GLuint texture);

    virtual void draw();

    void setTexture(GLuint texture);

    const Vec2i getPos();
    const bool canCollide();
    const GLuint getTexture();

    static Vec2d size;
};

#endif // BLOCK_H
