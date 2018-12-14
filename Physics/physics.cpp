#include "physics.h"

#include <list>

using namespace std;

list<Physics*> Physics::_li;

const double Physics::C_GRAV = 6.67e-23;
const double Physics::C_ELEC = 9e+9;


Physics::Physics():weight(0),charge(0),canMove(false){
    _acc = Vec2d();
    _li.push_front(this);
}

Physics::Physics(Vec2d _position, Vec2d _velocity, double _weight, double _charge, bool _canMove)
:position(_position),velocity(_velocity),weight(_weight),charge(_charge),canMove(_canMove){
    _acc = Vec2d();
    _li.push_front(this);
}

Physics::Physics(const Physics& w)
:position(w.position),velocity(w.velocity),weight(w.weight),charge(w.charge),canMove(w.canMove){
    _acc = Vec2d();
    _li.push_front(this);
}

Physics::~Physics(){
    auto it = _li.begin();
    while((*it)!=this)
        ++it;
    _li.erase(it);
}

const list<Physics*>& Physics::getList(){
    return _li;
}

void Physics::moveAll(double ms){
    double total = ms;
    ms = 1.0;
    do{
        if(total<1.0)
            ms = total;
        total -= 1.0;
        for(auto it = _li.begin(); it!=_li.end();){
            Physics* p = *it;
            ++it;
            if(!p->canMove || p->weight==0.0)
                continue;
            for(auto it2 = it; it2!=_li.end(); it2++){
                Physics* t = *it2;
                if(t->weight==0.0)
                    continue;
                double d = p->position.distance(t->position);
                if(d==0 /**|| d>100**/)
                    continue;
                /**if(d<1)
                    d = 100;*/
                double f = 400000.0*(t->weight*p->weight)/(d*d);
                /*double f = -C_GRAV*(t->weight*p->weight)/(d*d);
                f += C_ELEC*(t->charge*p->charge)/(d*d);*/
                p->_acc.x += (f/p->weight)*((p->position.x - t->position.x)/d);
                p->_acc.y += (f/p->weight)*((p->position.y - t->position.y)/d);
                t->_acc.x += (f/t->weight)*((t->position.x - p->position.x)/d);
                t->_acc.y += (f/t->weight)*((t->position.y - p->position.y)/d);
            }
        }

        for(Physics* p:_li){
            if(p->canMove){
                p->velocity += p->_acc*ms/1000.0;
                p->velocity *= 0.99; /// Rozamiento
                p->position += p->velocity*ms/1000.0;
            }
            p->_acc.clear();
        }
    }while(ms>=1.0);
}
