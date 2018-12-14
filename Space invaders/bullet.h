#ifndef BULLET_H
#define BULLET_H

#include <vector>

#include "vec.h"
#include "drawable.h"

class Bullet:Drawable{

    Vec2d _pos, _vel;
    double _radius;
    Vec3ub _color;

public:
    Bullet(Vec2d pos, Vec2d vel, double radius, Vec3ub color);
    void move();
    virtual void draw();

    Vec2d getPosition();
    Vec2d getVelocity();
    int getRadius();
    Vec3ub getColor();

};

#endif // BULLET_H
