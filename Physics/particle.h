#ifndef PARTICLE_H
#define PARTICLE_H


#include "vec.hpp"
#include "drawable.h"
#include "physics.h"

class Particle:public Drawable, public Physics{
public:

    Vec3ub color;

    Particle(Vec2d _position, Vec2d _velocity, double _weight, double _charge, bool canMove, Vec3ub _color);

    virtual void draw();
};

#endif // PARTICLE_H
