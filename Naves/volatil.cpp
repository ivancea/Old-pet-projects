#include "volatil.h"

int volatil::getTicks()const{
    return _ticks;
}
int volatil::getMaxTicks()const{
    return _maxTicks;
}
void volatil::clear(){
    _ticks=_maxTicks;
}
bool volatil::isFinished(){
    return ++_ticks>=_maxTicks;
}


volatilManager::~volatilManager(){
    clear();
}
void volatilManager::add(volatil* v){
    if(v->getTicks()<v->getMaxTicks())
        _v.push_back(v);
    else delete v;
}
void volatilManager::clear(){
    for(auto it=_v.begin(); it!=_v.end(); it++)
        delete *it;
    _v.clear();
}
void volatilManager::render(){
    for(auto it=_v.begin(); it!=_v.end();)
        if((*it)->render()){
            delete (*it);
            it = _v.erase(it);
        }else ++it;
}
