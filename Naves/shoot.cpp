#include <GL/glfw.h>
#include "shoot.h"

shoot::shoot(vec2d shooter, vec2d point, int maxTicks):volatil(maxTicks){
    _pos = shooter;
    _size = distance(shooter,point)-4;
    if(point.x==shooter.x)
        _angle = shooter.y>point.y?-PI/2:PI/2;
    else
        _angle = atan((point.y-shooter.y)/(point.x-shooter.x)) + (point.x<shooter.x?PI:0);

}
shoot::shoot(const shoot& s):volatil(s.getMaxTicks()){
    _pos = s.getPos();
    _size = s.getSize();
    _angle = s.getAngle();
}

vec2d shoot::getPos()const{return _pos;}
double shoot::getSize()const{return _size;}
double shoot::getAngle()const{return _angle;}

bool shoot::render(){
    glPushMatrix();
    glLoadIdentity();
    glColor3ub(150,255,150);
    glTranslated(_pos.x,_pos.y,0);
    glRotated(_angle*(180/PI),0,0,1);
    glBegin(GL_QUADS);
    glVertex2d(4,1);
    glVertex2d(_size+4,1);
    glVertex2d(_size+4,-1);
    glVertex2d(4,-1);
    glEnd();
    glPopMatrix();

    return isFinished();
}
