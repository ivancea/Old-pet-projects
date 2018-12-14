#ifndef TICKWAITER_H
#define TICKWAITER_H

#include <ctime>

class tickWaiter
{
protected:
    clock_t _initial;
    unsigned int _tps, _ticks;

public:
    tickWaiter();

    virtual void start(unsigned int ticksPerSecond);

    unsigned int getTicks()const;
    unsigned int getTicksPerSecond()const;

    virtual bool ready();
    virtual unsigned int wait();
};

#endif // TICKWAITER_H
