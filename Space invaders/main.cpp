#include <GL/glfw.h>

#include <iostream>
#include <thread>
#include <windows.h>
#include <vector>

#include "tickwaiter.h"
#include "fpsmanager.h"
#include "vec.h"
#include "drawable.h"
#include "bullet.h"

using namespace std;

int width = 800, height = 600;
bool running = true;

string title = "Title";


tickWaiter tw;
fpsManager fps(10);

Vec2d mousePosition;

int playerRadius = 10;
int playerBulletRadius = 4;

Vec2i player(width/2,height-5);

vector<Bullet*> enemyBullets, playerBullets;


void mousePositionCallback(int x, int y){
    mousePosition.x = x;
    mousePosition.y = y;
    player.x=x;
    if(player.x>width-2)
        player.x=width-2;
    else if(player.x<2)
        player.x=2;
}

void mouseButtonCallback(int button, int action){
    if(button==GLFW_MOUSE_BUTTON_1){
        if(action==GLFW_PRESS){
            if(playerBullets.size()==0 || playerBullets[playerBullets.size()-1]->getPosition().y<height-100)
                playerBullets.push_back(new Bullet(Vec2d(player.x, player.y-playerRadius), Vec2d(0,-20), playerBulletRadius, Vec3ub(200,200,255)));
        }else{

        }
    }else if(button==GLFW_PRESS){
        if(action==GLFW_MOUSE_BUTTON_2){

        }else{

        }
    }
}

void keyCallback(int key, int action){
    if(action==GLFW_PRESS){
        switch(key){
        case GLFW_KEY_ESC:
            running = false;
        }
    }else if(action==GLFW_RELEASE){
        switch(key){
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

    return true;
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Drawable::drawAll();

    glColor3ub(255,255,255);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(player.x, player.y);
    float deltaAngle = (2.0f*3.14159)/20;
    for (float angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle){
        glVertex2f(player.x + cosf(angle) * playerRadius, player.y + sinf(angle) * playerRadius);
    }
    glEnd();


    glfwSwapBuffers();
    glMatrixMode(GL_MODELVIEW);
    glfwPollEvents();
}

void tick(){
    enemyBullets.push_back(new Bullet(Vec2d(rand()%width,0), Vec2d(rand()%5-2,rand()%20+8), 3+rand()%6, Vec3ub(255,128,128)));

    for(int i=0; i<playerBullets.size(); i++){
        playerBullets[i]->move();
        if(playerBullets[i]->getPosition().y<-playerBullets[i]->getRadius()){
            delete playerBullets[i];
            playerBullets.erase(playerBullets.begin()+i);
        }else
            for(int j=0; j<enemyBullets.size(); j++)
                if(abs(enemyBullets[j]->getPosition().x-playerBullets[i]->getPosition().x)<enemyBullets[j]->getRadius()+playerBullets[i]->getRadius()
                   && enemyBullets[j]->getPosition().y<=playerBullets[i]->getPosition().y-playerBullets[i]->getVelocity().y
                   && enemyBullets[j]->getPosition().y>=playerBullets[i]->getPosition().y){
                    delete playerBullets[i];
                    delete enemyBullets[j];
                    enemyBullets.erase(enemyBullets.begin()+j);
                    playerBullets.erase(playerBullets.begin()+i);
                    --i;
                    break;
                }
    }

    for(int i=0; i<enemyBullets.size();){
        enemyBullets[i]->move();
        if(enemyBullets[i]->getPosition().distance(player)<enemyBullets[i]->getRadius()+playerRadius){
            for(i=0; i<enemyBullets.size(); i++)
                delete enemyBullets[i];
            enemyBullets.clear();
            break;
        }
        if(enemyBullets[i]->getPosition().y>height+enemyBullets[i]->getRadius()){
            delete enemyBullets[i];
            enemyBullets.erase(enemyBullets.begin()+i);
        }else{
            for(int j=0; j<playerBullets.size(); j++)
                if(abs(enemyBullets[i]->getPosition().x-playerBullets[j]->getPosition().x)<enemyBullets[i]->getRadius()+playerBullets[j]->getRadius()
                   && enemyBullets[i]->getPosition().y<=playerBullets[j]->getPosition().y-playerBullets[j]->getVelocity().y
                   && enemyBullets[i]->getPosition().y>=playerBullets[j]->getPosition().y){
                    delete playerBullets[j];
                    delete enemyBullets[i];
                    enemyBullets.erase(enemyBullets.begin()+i);
                    playerBullets.erase(playerBullets.begin()+j);
                    --j;
                    break;
                }
            ++i;
        }
    }

    if(!glfwGetWindowParam(GLFW_OPENED))
        running = false;
}

int main(){
    srand(time(0));
    if(!init()){
        cerr << "Error";
        return 1;
    }

    glfwSetMousePosCallback(&mousePositionCallback);
    glfwSetMouseButtonCallback(&mouseButtonCallback);
    glfwSetKeyCallback(&keyCallback);

    while(running){
        if(fps.ready())
            draw();
        if(tw.ready())
            tick();
        Sleep(2);
    }

    glfwTerminate();
    return 0;
}
