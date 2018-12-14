#ifndef SHOOT_H
#define SHOOT_H

#include "vec.hpp"
#include "volatil.h"

class shoot:public volatil
{
    vec2d _pos;
    double _size;
    double _angle;
public:
    shoot()=delete;
    shoot(vec2d shooter, vec2d point, int maxTicks);
    shoot(const shoot& s);
    ~shoot(){}

    vec2d getPos()const;
    double getSize()const;
    double getAngle()const;

    virtual bool render();
};

#endif // SHOOT_H
