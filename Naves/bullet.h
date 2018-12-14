#ifndef BULLET_H
#define BULLET_H

#include <vector>

#include "vec.hpp"
#include "nave.h"

class bullet{

    vec2d _pos, _vel;
    double _radius;
    int _team;

public:
    bullet(vec2d pos, vec2d vel, double radius, int team);
    int move(); /// index of naves, else -1
    void render();

    vec2d getPos();
    vec2d getVel();
    int getRadius();
    int getTeam();

    static std::vector<nave>* naves;

};

#endif // BULLET_H
