#include <GL/glfw.h>

#include <iostream>
#include <list>
#include <thread>
#include <windows.h>

#include "tickwaiter.h"
#include "fpsmanager.h"

#include "particle.h"

using namespace std;

int width = 400, height = 400;
bool running = true;

string title = "Physics";


tickWaiter tw;
fpsManager fps(10);

list<Particle*> particles;

Vec2d mousePosition;


void mousePositionCallback(int x, int y){
    mousePosition = Vec2d(x,y);
    (*particles.begin())->position = mousePosition;
}

void mouseButtonCallback(int button, int action){
    if(button==GLFW_MOUSE_BUTTON_1){
        if(action==GLFW_PRESS)
            (*particles.begin())->weight = 50;
        else
            (*particles.begin())->weight = 0;
    }else if(button==GLFW_PRESS){
        if(action==GLFW_MOUSE_BUTTON_2){
            particles.push_back(new Particle(mousePosition,Vec2d(), 10,(rand()%2==0?0.01:-0.01), true, Vec3ub(rand()%255,rand()%255,rand()%255)));
        }
    }
}

void keyCallback(int key, int action){
    if(action==GLFW_PRESS){
        switch(key){
        case GLFW_KEY_UP:
            fps.setFps(fps.getTicksPerSecond()+1);
            break;
        case GLFW_KEY_DOWN:
            if(fps.getTicksPerSecond()>1)
                fps.setFps(fps.getTicksPerSecond()-1);
            break;
        case GLFW_KEY_SPACE:
            for(Physics* p:Physics::getList())
                p->velocity.clear();
        }
    }
}

bool init(){
    if(!glfwInit())
        return false;
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, true);
    if(!glfwOpenWindow(width,height, 8,8,8,8, 24, 0, GLFW_WINDOW)){
        glfwTerminate();
        return false;
    }
	glfwSetWindowTitle(title.c_str());
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    glOrtho(0,width,height,0,0,1);

    tw.start(20);
    fps.start(40);

    /**particles.push_back(new Particle(Vec2d(250,250),Vec2d(), 5.972e24, 0, false));

    particles.push_back(new Particle(Vec2d(400,250),Vec2d(0,1), 1.0, 0, true));
    particles.push_back(new Particle(Vec2d(100,250),Vec2d(0,-1), 1.0, 0, true));
    particles.push_back(new Particle(Vec2d(250,300),Vec2d(-2,0), 1.0, 0, true));
    particles.push_back(new Particle(Vec2d(250,200),Vec2d(2,0), 1.0, 0, true));**/

    particles.push_back(new Particle(Vec2d(), Vec2d(), 0, 0, false, Vec3ub(0,0,0)));
    (*particles.begin())->toDraw = false;

    for(int i=0; i<width; i+=5){
        particles.push_back(new Particle(Vec2d(i,0),Vec2d(), 1,(rand()%2==0?0.01:-0.01), false, Vec3ub(255,255,255)));
        particles.push_back(new Particle(Vec2d(width-i,height),Vec2d(), 1,(rand()%2==0?0.01:-0.01), false, Vec3ub(255,255,255)));
        particles.push_back(new Particle(Vec2d(0,height-i),Vec2d(), 1,(rand()%2==0?0.01:-0.01), false, Vec3ub(255,255,255)));
        particles.push_back(new Particle(Vec2d(width,i),Vec2d(), 1,(rand()%2==0?0.01:-0.01), false, Vec3ub(255,255,255)));
    }
    for(int i=0; i<400; i++)
        particles.push_back(new Particle(Vec2d(rand()%width,rand()%height),Vec2d(), 1,(rand()%2==0?0.01:-0.01), true, Vec3ub(rand()%255,rand()%255,rand()%255)));

    /* PROTON - ELECTRON
    particles.push_back(new Particle(Vec2d(250,250), Vec2d(), 9.109e-31, -1.6e-19, false, Vec3ub(255,255,255)));
    particles.push_back(new Particle(Vec2d(250,250), Vec2d(), 1.672e-27, 1.6e-19, false, Vec3ub(255,0,255)));*/

    return true;
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /** PARA CAMPOS **/
    /*float arr[width*height];

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            const list<Physics*> &l = Physics::getList();
            double t = 0;
            for(Physics* p:l){
                double d = p->position.distance(Vec2d(i,j));
                t += Physics::C_ELEC*p->charge/(d*d);
            }
            arr[i+(height-j-1)*width] = (t>50000.0?1.0:(t<-50000.0?0.0:(t+50000.0)/100000.0));
        }
    }*/

    /** PARA CAMPOS CON THREADS **/
    /*thread *ths[width];

    for(int i=0; i<width; i++)
    ths[i] = new thread([&](){
                            for(int j=0; j<height; j++){
                                const list<Physics*> &l = Physics::getList();
                                double t = 0;
                                for(Physics* p:l){
                                    double d = p->position.distance(Vec2d(i,j));
                                    t += Physics::C_ELEC*p->charge/(d*d);
                                }
                                arr[i+(height-j-1)*width] = (t>50.0?1.0:t/50.0);
                            }
                        });
    for(thread* th:ths){
        if(th->joinable())
            th->join();
        delete th;
    }*/
    /**glDrawPixels(width, height, GL_BLUE, GL_FLOAT, arr);**/

    Drawable::drawAll();

    glfwSwapBuffers();
    glMatrixMode(GL_MODELVIEW);
    glfwPollEvents();
}

void tick(){

    if(tw.getTicks()%(tw.getTicksPerSecond())==0)
        cout <<"Fps: " << fps.getTicksPerSecond() << " (Real: " << fps.getFps() << ")" << endl;

    Physics::moveAll(2);

    //if(particles.size()<1000)
    //    particles.push_back(new Particle(Vec2d(1,1),Vec2d(1,1), 1,(rand()%2==0?0.01:-0.01), true, Vec3ub(rand()%255,rand()%255,rand()%255)));

    auto it = particles.begin();
    while(it!=particles.end()){
        if((*it)->canMove){
            if((*it)->position.x>width){
                (*it)->position.x = width;
                (*it)->velocity.x *= -0.95;
            }else if((*it)->position.x<0){
                (*it)->position.x = 0;
                (*it)->velocity.x *= -0.95;
            }
            if((*it)->position.y>height){
                (*it)->position.y = height;
                (*it)->velocity.y *= -0.95;
            }else if((*it)->position.y<0){
                (*it)->position.y = 0;
                (*it)->velocity.y *= -0.95;
            }
        }
        ++it;
    }

    if(!glfwGetWindowParam(GLFW_OPENED))
        running = false;
}

void threadTicks(){
    while(running){
        tw.wait();
        tick();
    }
}

int main(){
    srand(time(0));
    if(!init()){
        cout << "Error";
        return 1;
    }

    glfwSetMousePosCallback(&mousePositionCallback);
    glfwSetMouseButtonCallback(&mouseButtonCallback);
    glfwSetKeyCallback(&keyCallback);

    /**while(running){
        if(fps.ready())
            draw();
        if(tw.ready())
            tick();
    }**/

    thread thTicks(&threadTicks);

    while(running){
        fps.wait();
        draw();
    }
    if(thTicks.joinable())
        thTicks.join();

    glfwTerminate();
    return 0;
}
