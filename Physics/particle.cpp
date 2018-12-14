#include "particle.h"

#include <GL/glfw.h>

#include "vec.hpp"

Particle::Particle(Vec2d _position, Vec2d _velocity, double _weight, double _charge, bool _canMove, Vec3ub _color)
:Physics(_position,_velocity,_weight,_charge,_canMove),color(_color){}

void Particle::draw(){
    glColor3ub(color.x, color.y, color.z);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y);
    float deltaAngle = (2.0f*3.14159)/20;
    for (float angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle){
        glVertex2f(position.x + cosf(angle)*2, position.y + sinf(angle)*2);
    }
    glEnd();
}
