#include "tickwaiter.h"
#include <ctime>
#include <windows.h>

tickWaiter::tickWaiter():_tps(0),_ticks(0),_initial(0){}

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
    if(_tps==0) return true;
    clock_t ms = (1000/_tps)*(1000/CLOCKS_PER_SEC);
    clock_t actual = clock();
    if((_initial+ms*_ticks)>actual)
        return false;
    ++_ticks;
    return true;
}

unsigned int tickWaiter::wait(){
    if(_tps==0) return 0;
    clock_t ms = (1000/_tps)*(1000/CLOCKS_PER_SEC);
    clock_t actual = clock();
    if((_initial+ms*_ticks)>actual)
        Sleep((_initial+ms*_ticks)-actual);
    ++_ticks;
    return ((_initial+ms*_ticks)-actual>0?(_initial+ms*_ticks)-actual:0);
}
