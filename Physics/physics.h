#ifndef PHYSICS_H
#define PHYSICS_H

#include <list>

#include "vec.hpp"

class Physics{
    static std::list<Physics*> _li;

    Vec2d _acc;

public:

    Vec2d position, velocity;
    double weight, charge;
    bool canMove;

    Physics();
    Physics(Vec2d _position, Vec2d _velocity, double _weight, double _charge, bool _canmove);
    Physics(const Physics& w);
    ~Physics();

    static const double C_GRAV, C_ELEC;

    static const std::list<Physics*>& getList();

    static void moveAll(double ms);
};

#endif // PHYSICS_H
