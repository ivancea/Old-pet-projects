#ifndef SCENE_H
#define SCENE_H

#include <map>
#include <string>
#include <iostream>

enum SceneType{
    Menu = 0,
    Game = 1
};

enum EventType{
    Nothing = 0,
    Exit = 1,
    ChangeScene = 2
};

struct Event{
    int type;
    void* data;


    Event():type(EventType::Nothing),data(nullptr){}
    Event(int _type):type(_type),data(nullptr){}
    Event(int _type, void* _data):type(_type),data(_data){}

    operator bool(){
        return type==EventType::Nothing;
    }
};


class Scene{
public:
    std::map<std::string,void*> data;

    virtual ~Scene(){}

    virtual Event process(uint32_t deltaTime)=0;
    virtual void render()=0;
};


#include "sceneMenu.h"
#include "sceneGame.h"


#endif // SCENE_H
