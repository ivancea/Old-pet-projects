#include "fpsmanager.h"

#include <windows.h>

fpsManager::fpsManager(unsigned int precision):_ms(new unsigned int[precision]),_precision(precision),_last(0){
    for(int i=0; i<precision; i++)
        _ms[i] = 0;
}

fpsManager::~fpsManager(){
    if(_precision>1) delete[] _ms;
    else delete _ms;
}

void fpsManager::start(unsigned int ticksPerSecond){
    tickWaiter::start(ticksPerSecond);
    _last = _initial;
}

unsigned int fpsManager::getPrecision()const{
    return _precision;
}

unsigned int fpsManager::getFps()const{
    if(_ms[0]==0) return getTicksPerSecond();
    unsigned int count = 0;
    for(int i=0; i<_precision; i++)
        count+=_ms[i];
    return (_precision*1000)/count;
}

void fpsManager::addMs(){
    for(int i=_precision-1; i>0; i--)
        _ms[i] = _ms[i-1];
    _ms[0] = clock()-_last;
    _last += _ms[0];
}

bool fpsManager::ready(){
    if(_tps==0) return false;
    clock_t ms = (1000/_tps);
    if(_last+ms<=clock()){
        addMs();
        return true;
    }
    return false;
}

unsigned int fpsManager::wait(){
    if(_tps==0) return 0;
    clock_t ms = (1000/_tps);
    clock_t actual = clock();
    if(_last+ms>actual)
        Sleep(_last+ms-actual);
    addMs();
    return (_last+ms-actual>0?_last+ms-actual:0);
}

void fpsManager::setFps(unsigned int fps){
    start(fps);
}
