#include "bullet.h"

#include <GL/glfw.h>
#include <vector>

#include "vec.hpp"
#include "nave.h"


std::vector<nave>* bullet::naves = 0;

bullet::bullet(vec2d pos, vec2d vel, double radius, int team)
:_pos(pos), _vel(vel), _radius(radius), _team(team){}

int bullet::move(){
    _pos += _vel;
    if(naves!=0)
        for(int i=0; i<naves->size(); i++)
            if((*naves)[i].getLife()>0 && distance(_pos, (*naves)[i].getPos()) <= _radius){
                return i;
            }
    return -1;
}

void bullet::render(){
    glColor3ub(255, 128,  128);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(_pos.x, _pos.y);
    float deltaAngle = (2.0f*3.14159)/20;
    for (float angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle){
        glVertex2f(_pos.x + cosf(angle) * _radius, _pos.y + sinf(angle) * _radius);
    }
    glEnd();
}

vec2d bullet::getPos(){
    return _pos;
}

vec2d bullet::getVel(){
    return _vel;
}

int bullet::getRadius(){
    return _radius;
}

int bullet::getTeam(){
    return _team;
}
