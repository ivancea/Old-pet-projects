#include "bullet.h"

#include <GL/glfw.h>
#include <vector>

#include "vec.h"


Bullet::Bullet(Vec2d pos, Vec2d vel, double radius, Vec3ub color):_pos(pos), _vel(vel), _radius(radius),_color(color){}

void Bullet::move(){
    _pos += _vel;
}

void Bullet::draw(){
    glColor3ub(_color.x, _color.y, _color.z);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(_pos.x, _pos.y);
    float deltaAngle = (2.0f*3.14159)/20;
    for (float angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle){
        glVertex2f(_pos.x + cosf(angle) * _radius, _pos.y + sinf(angle) * _radius);
    }
    glEnd();
}

Vec2d Bullet::getPosition(){
    return _pos;
}

Vec2d Bullet::getVelocity(){
    return _vel;
}

int Bullet::getRadius(){
    return _radius;
}

Vec3ub Bullet::getColor(){
    return _color;
}
