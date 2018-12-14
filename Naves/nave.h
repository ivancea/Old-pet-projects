#ifndef NAVE_H
#define NAVE_H
#include "vec.hpp"

const vec2d NULLPOS(-1,-1);

class nave
{
    vec2d _pos;
    double _angle;
    int _team;
    int _recoil;
    int _life;
    nave* _target;
    vec2d _targetPos;
    bool _selected;
    bool _disabled;

    static void (*_damageEventCallback)(nave*,nave*);

public:
    static int maxLife;
    static double attackRange;
    static double velocity;
    static int totalRecoil;

    static void setDamageEventCallback(void (*damageEventCallback)(nave* damager, nave* receiver));

    nave()=delete;
    nave(int team, vec2d pos);
    nave(const nave& n);
    ~nave(){}

    vec2d getPos()const;
    double getAngle()const;
    void setAngle(double angle);
    void setTarget(nave* target);
    nave* getTarget()const;
    void setTargetPos(vec2d target);
    vec2d getTargetPos()const;
    void setSelected(bool selected);
    void setDisabled(bool disabled);
    int getLife()const;
    int getTeam()const;
    void addRecoil(int moreRecoil=1);
    int getRecoil()const;
    bool isAlive()const;
    bool isSelected()const;
    bool isDisabled()const;

    void move();
    bool attack();
    bool damaged(nave* n);
    void action();

    void render()const;
};


#endif // NAVE_H
