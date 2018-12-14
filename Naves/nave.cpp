#include <GL/glfw.h>
#include <cmath>
#include "nave.h"
#include "vec.hpp"

unsigned char teamColors[5][3] = {{125,125,125},{255,0,0},{0,255,0},{0,0,255},{255,0,255}};

int nave::maxLife=8;
double nave::attackRange=50;
double nave::velocity=0.8;
int nave::totalRecoil=25;

void (*nave::_damageEventCallback)(nave*,nave*) = 0;

void nave::setDamageEventCallback(void (*damageEventCallback)(nave*,nave*)){
    _damageEventCallback = damageEventCallback;
}

double getTrueAngle(const vec2d a, const vec2d b){
    if(b.x==a.x) return a.y>b.y?-PI/2:PI/2;
    return atan((b.y-a.y)/(b.x-a.x)) + (b.x<a.x?PI:0);
}

nave::nave(int team, vec2d pos):_pos(pos),_angle(0),_team(team),_recoil(0),_life(maxLife),_target(0),_targetPos(NULLPOS),_selected(false),_disabled(false){}

nave::nave(const nave& n){
    _pos = n.getPos();
    _angle = n.getAngle();
    _team = n.getTeam();
    _life = n.getLife();
    _target = n.getTarget();
    _targetPos = n.getTargetPos();
    _recoil = n.getRecoil();
    _selected = n.isSelected();
    _disabled = n.isDisabled();
}

vec2d nave::getPos()const{
    return _pos;
}
double nave::getAngle()const{
    return _angle;
}
void nave::setAngle(double angle){
    if(_target!=0) _target=0;
    _angle=angle;
}
void nave::setTarget(nave* target){
    if(!isAlive() || isDisabled()) return;
    if(target==0) return;
    if(target->getTeam()==_team || !target->isAlive()) return;
    //_angle = getTrueAngle(_pos,target->getPos());
    _target = target;
    _targetPos = vec2d(-1,-1);
}
nave* nave::getTarget()const{
    return _target;
}
void nave::setTargetPos(vec2d target){
    if(!isAlive() || isDisabled()) return;
    //_angle = getTrueAngle(_pos,target);
    _targetPos = target;
    _target = 0;
}
vec2d nave::getTargetPos()const{
    return _targetPos;
}
int nave::getLife()const{
    return _life;
}
int nave::getTeam()const{
    return _team;
}
void nave::addRecoil(int moreRecoil){
    _recoil += moreRecoil;
}
int nave::getRecoil()const{
    return _recoil;
}
bool nave::isAlive()const{
    return _team;
}
void nave::setSelected(bool selected){
    _selected = selected;
}
bool nave::isSelected()const{
    return _selected;
}
void nave::setDisabled(bool disabled){
    _disabled = disabled;
}
bool nave::isDisabled()const{
    return _disabled;
}

void nave::move(){
    if((_targetPos==NULLPOS && _target==0) || !isAlive()) return;
    vec2d t;
    if(_targetPos!=NULLPOS){
        t = _targetPos-_pos;
        _angle = getTrueAngle(_pos,_targetPos);
    }else{
        t = _target->getPos()-_pos;
        _angle = getTrueAngle(_pos,_target->getPos());
    }
    if(t.distance()<velocity && _target==0){
        _pos = _targetPos;
        _targetPos = NULLPOS;
        return;
    }
    t.normalize();
    _pos+=t*velocity;
}
bool nave::attack(){
    if(_target==0 || !isAlive()) return false;
    if(!_target->isAlive()){
        _target = 0;
        return false;
    }
    if(distance(_pos,_target->getPos())>attackRange) return false;
    _angle = getTrueAngle(_pos,_target->getPos());
    if(_recoil>0){
        --_recoil;
        return true;
    }
    if(_target->damaged(this)) _target=0;
    _recoil = totalRecoil;
    return true;
}

bool nave::damaged(nave* n){
    _life-=1;
    (*_damageEventCallback)(n,this);
    if(_life<=0){
        _team = 0;
        _target = 0;
        _targetPos = NULLPOS;
        return true;
    }
    if(_target==0 && _targetPos==NULLPOS) setTarget(n);
    return false;
}
void nave::action(){
    if(!isAlive() || isDisabled()) return;
    if(attack()) return;
    move();
    if(_recoil>0)--_recoil;
}

void nave::render()const{
    if(isDisabled()) return;
    glPushMatrix();
    glLoadIdentity();
    glColor3ub(teamColors[_team][0],teamColors[_team][1],teamColors[_team][2]);
    glTranslated(_pos.x,_pos.y,0);
    glRotated(_angle*(180/PI),0,0,1);
    glBegin(GL_TRIANGLES);
    glVertex2d(-4,4);
    glVertex2d(-4,-4);
    glVertex2d(4,0);
    glEnd();
    glPopMatrix();
    if(_selected){
        glPushMatrix();
        glLoadIdentity();
        glColor3ub(255,255,255);
        glTranslated(_pos.x,_pos.y,0);
        glRotated(_angle*(180/PI),0,0,1);
        glBegin(GL_LINES);
        glVertex2d(-4,4);
        glVertex2d(-4,-4);
        glVertex2d(-4,-4);
        glVertex2d(4,0);
        glVertex2d(4,0);
        glVertex2d(-4,4);
        glEnd();
        glPopMatrix();
    }
}
