#include "fpsmanager.h"

fpsManager::fpsManager(unsigned int precision):_ms(new unsigned int[precision]),_precision(precision),_last(0){
    for(int i=0; i<precision; i++)
        _ms[i] = 0;
}

fpsManager::~fpsManager(){
    if(_precision>1) delete[] _ms;
    else delete _ms;
}

unsigned int fpsManager::getPrecision()const{
    return _precision;
}

unsigned int fpsManager::getFps()const{
    if(_ms[0]==0) return getTicksPerSecond();
    unsigned int count = 0;
    int t=-1;
    for(int i=0; i<_precision; i++){
        count+=_ms[i];
    }
    if(t==-1) t = _precision;
    count = (count*1000)/CLOCKS_PER_SEC;
     return (t*1000)/count;
}

void fpsManager::addMs(){
    for(int i=_precision-1; i>0; i--)
        _ms[i] = _ms[i-1];
    _ms[0] = clock()-_last;
    _last += _ms[0];
}

bool fpsManager::ready(){
    if(tickWaiter::ready()){
        addMs();
        return true;
    }
    return false;
}
void fpsManager::wait(){
    tickWaiter::wait();
    addMs();
}
