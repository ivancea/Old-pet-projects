#include <iostream>
#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "timers.h"

#include "scene.h"
#include "util.h"

using namespace std;

bool running = true;
uint32_t width  = 600,
         height = 600;

int main(){
    srand(time(0));
    sf::RenderWindow window;

    window.create(sf::VideoMode(width, height), "Flota", sf::Style::Titlebar|sf::Style::Close);
    window.setFramerateLimit(60);

    fontArial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");

    Scene *currentScene = new SceneMenu(&window);
    Event sceneEvent;

    DeltaTimer dt;

    while(running){
        if(currentScene==nullptr){
            cerr << "ERROR: null pointer on currentScene" << endl;
            break;
        }
        sceneEvent = currentScene->process(dt.next());

        switch(sceneEvent.type){
        case ChangeScene:
            {
                pair<SceneType,void*>* newSceneData = (pair<SceneType,void*>*)sceneEvent.data;
                switch(newSceneData->first){
                case SceneType::Game:
                    try{
                        Scene *nextScene = new SceneGame(&window, sf::Vector2u(15,15));
                        delete currentScene;
                        currentScene = nextScene;
                    }catch(string &s){
                        cerr << s << endl;
                    }
                    break;
                case SceneType::Menu:
                    delete currentScene;
                    currentScene = new SceneMenu(&window);
                    break;
                }
                delete newSceneData;
            }
            break;
        case Exit:
            running = false;
            break;
        case Nothing:
            break;
        default:
            cerr << "WARNING: scene returning a uncaught event type." << endl;
        }
        if(!running)
            break;

        window.clear();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        currentScene->render();
        window.display();
        thSleep(10);
    }
    if(currentScene)
        delete currentScene;
}
