#ifndef VOLATIL_H
#define VOLATIL_H

#include <list>

class volatil
{
    int _ticks;
    int _maxTicks;
public:
    volatil(int maxTicks):_ticks(0),_maxTicks(maxTicks){};
    virtual ~volatil(){}
    int getTicks()const;
    int getMaxTicks()const;
    void clear();
    bool isFinished();
    virtual bool render()=0;
};

class volatilManager{
    std::list<volatil*> _v;
public:
    volatilManager(){}
    ~volatilManager();
    void add(volatil* v);
    void clear();
    void render();
};

#endif // VOLATIL_H
