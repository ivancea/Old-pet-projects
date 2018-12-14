#include "tickwaiter.h"
#include <ctime>
#include <windows.h>

tickWaiter::tickWaiter():_tps(-1),_ticks(0),_initial(0){}

void tickWaiter::start(unsigned int ticksPerSecond){
    _tps = ticksPerSecond;
    _ticks = 0;
    _initial = clock()*1000/CLOCKS_PER_SEC;
}

unsigned int tickWaiter::getTicks()const{
    return _ticks;
}

unsigned int tickWaiter::getTicksPerSecond()const{
    return _tps;
}

bool tickWaiter::ready(){
    if(_tps==-1) return true;
    clock_t ms = (1000/_tps)*(1000/CLOCKS_PER_SEC);
    clock_t actual = clock();
    if((_initial+ms*_ticks)>actual)
        return false;
    ++_ticks;
    return true;
}

void tickWaiter::wait(){
    if(_tps==-1) return;
    clock_t ms = (1000/_tps)*(1000/CLOCKS_PER_SEC);
    clock_t actual = clock();
    if((_initial+ms*_ticks)>actual)
        Sleep((_initial+ms*_ticks)-actual);
    ++_ticks;
}
