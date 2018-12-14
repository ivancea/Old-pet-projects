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

    unsigned int getPrecision()const;
    unsigned int getFps()const;

    virtual bool ready();
    virtual void wait();
};

#endif // FPSMANAGER_H
