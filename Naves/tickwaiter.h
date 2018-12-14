#ifndef TICKWAITER_H
#define TICKWAITER_H

#include <ctime>

class tickWaiter
{
    unsigned int _tps, _ticks;

protected:
    clock_t _initial;

public:
    tickWaiter();

    void start(unsigned int ticksPerSecond);

    unsigned int getTicks()const;
    unsigned int getTicksPerSecond()const;

    virtual bool ready();
    virtual void wait();
};

#endif // TICKWAITER_H
