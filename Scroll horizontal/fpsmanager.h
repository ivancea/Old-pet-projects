#ifndef FPSMANAGER_H
#define FPSMANAGER_H

#include "tickwaiter.h"
#include <ctime>


class fpsManager:public tickWaiter{
    unsigned int *_ms, _precision;
    clock_t _last;

    void addMs();
public:
    fpsManager(unsigned int precision);
    ~fpsManager();

    virtual void start(unsigned int ticksPerSecond);

    unsigned int getPrecision()const;
    unsigned int getFps()const;

    void setFps(unsigned int fps);

    virtual bool ready();
    virtual unsigned int wait();
};

#endif // FPSMANAGER_H
