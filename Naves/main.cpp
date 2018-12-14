#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <GL/glfw.h>
#include <windows.h>
#include <thread>

#include "vec.hpp"
#include "volatil.h"
#include "nave.h"
#include "shoot.h"
#include "res.h"
#include "tickwaiter.h"
#include "fpsmanager.h"
#include "bullet.h"

using namespace std;

/*******************************************************************************/

string windowTitle = "Naves";

GLint width   = 600;              // Width of our window
GLint height  = 600;              // Height of our window

unsigned int ticksPerSecond = 40;
unsigned int frapsPerSecond = 60;

unsigned int naveMaxLife = 3;

tickWaiter tw;
fpsManager fps(10);

bool running = true;
bool paused = false;

bool mouseEnabled=true;
int mouseX=0, mouseY=0;
bool mouseL=false, mouseR=false;
vec2d lastClick;
uint32_t longClick=0;
bool longClicked=false;
bool windowActive = true;
bool shift=false;
bool playShoot=false;

int shootTicks = 5;
vector<nave> naves;
list<bullet> bullets;
volatilManager vm;

/*******************************************************************************/

void selectAllTeam(){
    int team=-1;
    for(int i=0; i<naves.size(); i++)
        if(distance(naves[i].getPos(),vec2d(mouseX,mouseY))<12 && naves[i].isAlive() && !naves[i].isDisabled()){
            team = naves[i].getTeam();
            break;
        }
    if(team==-1) return;
    for(int i=0; i<naves.size(); i++)
        if(naves[i].getTeam()==team)
        naves[i].setSelected(true);
}

void disableDeadNaves(){
    for(int i=0; i<naves.size(); i++)
        if(!naves[i].isAlive())
            naves[i].setDisabled(true);
}

vec2d getSelectionCenter(){
    if(naves.size()==0) return vec2d();
    vec2d max,min;
    for(auto it:naves)
        if(it.isSelected()){
            max = min = it.getPos();
            break;
        }
    for(auto it:naves){
        if(it.isSelected()){
        vec2d pos = it.getPos();
            if(pos.x<min.x)
                min.x=pos.x;
            if(pos.x>max.x)
                max.x=pos.x;
            if(pos.y<min.y)
                min.y=pos.y;
            if(pos.y>max.y)
                max.y=pos.y;
        }
    }
    return min+(max-min)/2;
}

bool isIntoRectangle(const nave &n){
    bool xOk=true, yOk=true;
    if(mouseX>lastClick.x)
        xOk = n.getPos().x<mouseX+4 && n.getPos().x>lastClick.x-4;
    else
        xOk = n.getPos().x>mouseX-4 && n.getPos().x<lastClick.x+4;
    if(mouseY>lastClick.y)
        yOk = n.getPos().y<mouseY+4 && n.getPos().y>lastClick.y-4;
    else
        yOk = n.getPos().y>mouseY-4 && n.getPos().y<lastClick.y+4;
    return xOk&&yOk;
}

int selectedNaves(){
    int t=0;
    for(auto it:naves)
        if(it.isSelected()) ++t;
    return t;
}

void unselectAll(){
    for(int i=0; i<naves.size(); i++)
        naves[i].setSelected(false);
}

void handleKeypress(int key, int action){
	if (action == GLFW_PRESS){
		switch(key){
		case GLFW_KEY_ESC:
		    running=false;
			break;
        case GLFW_KEY_LSHIFT:
            shift = true;
            break;
        case ' ':
            disableDeadNaves(); /** **/ cout << fps.getFps() << endl;
            break;
        case 'P':
            paused=!paused;
            break;
		default:
			break;
		}
	}else if(action==GLFW_RELEASE){
	    switch(key){
        case GLFW_KEY_LSHIFT:
            shift=false;
            break;
        default:
            break;
	    }
	}
}

void handleMouseMove(int x, int y){
    mouseX=x;
    mouseY=y;
    if(mouseL){
        if(!longClicked){
            if(clock()-longClick>100 || distance(vec2d(mouseX,mouseY),lastClick)>5)
                longClicked=true;
        }
    }
    static int enableX=0, enableY=0;
    static int disableX=0, disableY=0;
    if(mouseX>=0 && mouseX<width && mouseY>=0 && mouseY<height){
        //if(mouseEnabled){
            mouseEnabled = false;
            disableX=mouseX; disableY=mouseY;
            glfwDisable(GLFW_MOUSE_CURSOR);
            mouseX = disableX; mouseY = disableY;
        //}
    }else if(!mouseEnabled){
        mouseEnabled = true;
        enableX=mouseX; enableY=mouseY;
        glfwEnable(GLFW_MOUSE_CURSOR);
        mouseX = enableX; mouseY = enableY;
        glfwSetMousePos(enableX,enableY);
    }
}
void handleMouseButton(int button, int action){
    if(action==GLFW_PRESS){
        if(button==GLFW_MOUSE_BUTTON_LEFT){
            mouseL=true;
            if(!shift) unselectAll();
            if(clock()-longClick<200){
                selectAllTeam();
            }else{
                for(int i=0; i<naves.size(); i++)
                    if(naves[i].isAlive() && distance(naves[i].getPos(),vec2d(mouseX,mouseY))<12){
                        naves[i].setSelected(!naves[i].isSelected());
                        break;
                    }else if(i==naves.size()-1 && !shift){
                        if(!shift) unselectAll();
                    }
            }
            longClick = clock();
            lastClick = vec2d(mouseX,mouseY);
        }else if(button==GLFW_MOUSE_BUTTON_RIGHT){
            mouseR=true;
            vec2d midPos = getSelectionCenter();
            int t=-1;
            for(int i=0; i<naves.size(); i++)
                if(naves[i].isAlive() && distance(naves[i].getPos(),vec2d(mouseX,mouseY))<12){
                    t = i;
                    break;
                }
            //for(int n=0; n<naves.size(); n++){
            for(int i=0; i<naves.size(); i++)
                if(!naves[i].isSelected()) continue;
                else if(t!=-1 && i!=t && naves[i].getTeam()!=naves[t].getTeam()){
                    naves[i].setTarget(&naves[t]);
                }else{
                    vec2d pos = vec2d(mouseX,mouseY)+naves[i].getPos()-midPos;
                    /*if(pos.x>=width) pos.x=width-1;
                    else if(pos.x<0) pos.x=0;
                    if(pos.y>=height) pos.y=height-1;
                    else if(pos.y<0) pos.y=0;*/
                    naves[i].setTargetPos(pos);
                }

        }
    }else if(action==GLFW_RELEASE){
        if(button==GLFW_MOUSE_BUTTON_LEFT){
            mouseL=false;
            if(longClicked){
                for(int i=0; i<naves.size(); i++)
                    if(naves[i].isAlive() && isIntoRectangle(naves[i]))
                        naves[i].setSelected(true);
                longClicked = false;
            }
        }else if(button==GLFW_MOUSE_BUTTON_RIGHT){
            mouseR=false;
        }
    }
}

void initGL(){
	glfwSetWindowTitle(windowTitle.c_str());
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    glOrtho(0,width,height,0,0,1);
}


void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i=0; i<naves.size(); i++)
        naves[i].render();
    vm.render();
    if(longClicked){
        glPushMatrix();
        glLoadIdentity();
        glColor3ub(150,150,150);
        glBegin(GL_LINES);
        glVertex2d(lastClick.x,lastClick.y);
        glVertex2d(lastClick.x,mouseY);
        glVertex2d(mouseX,mouseY);
        glVertex2d(mouseX,lastClick.y);
        glVertex2d(lastClick.x,lastClick.y);
        glVertex2d(mouseX,lastClick.y);
        glVertex2d(lastClick.x,mouseY);
        glVertex2d(mouseX,mouseY);
        glEnd();
        glPopMatrix();
    }
    if(!mouseEnabled){
        glPushMatrix();
        glLoadIdentity();
        glTranslated(mouseX,mouseY,0);
        glColor3ub(240,240,240);
        glBegin(GL_LINES);
        glVertex2d(-10,0);
        glVertex2d(10,0);
        glVertex2d(0,-10);
        glVertex2d(0,10);
        glEnd();
        glPopMatrix();
    }

    for(auto it:bullets)
        it.render();

    glfwSwapBuffers();
    glMatrixMode(GL_MODELVIEW);
    glfwPollEvents();
}

void moveAll(){
    if(paused)
        return;

    for(int i=0; i<naves.size(); i++)
        if(naves[i].getTarget()!=0 && (!naves[i].isAlive() || naves[i].isDisabled()))
            naves[i].setTarget(0);
    for(int i=0; i<naves.size(); i++)
        naves[i].action();
    for(int i=0; i<naves.size(); i++){
        if(naves[i].getTarget()!=0 || naves[i].getTargetPos()!=NULLPOS || !naves[i].isAlive() || naves[i].isDisabled()) continue;
        //int minimo = -1;
        for(int j=0; j<naves.size(); j++)
            if(i!=j && naves[i].getTeam()!=naves[j].getTeam() && naves[j].isAlive() && distance(naves[i].getPos(),naves[j].getPos()) < nave::attackRange){
               // if(minimo==-1) minimo = j;
               // else if(distance(naves[i].getPos(),naves[j].getPos())<distance(naves[i].getPos(),naves[minimo].getPos()))
               //     minimo = j;
               naves[i].setTarget(&(naves[j]));
               naves[i].addRecoil();
               break;
            }
        //if(minimo!=-1) naves[i].setTarget(&(naves[minimo]));
    }
    {
        auto it = bullets.begin();
        while(it!=bullets.end()){
            int t = it->move();
            if(t!=-1)
                naves[t].damaged(0);
            if(it->getPos().x<-it->getRadius() || it->getPos().x>width+it->getRadius()
            || it->getPos().y<-it->getRadius() || it->getPos().y>height+it->getRadius()
            || t!=-1)
                it = bullets.erase(it);
            else ++it;
        }
    }
    bullets.push_back(bullet(vec2d(-5,rand()%height),vec2d(2,rand()%5-2),5,-1));
}

void damageEventHandler(nave* damager, nave* receiver){
    if(receiver->getLife()<=0){
        receiver->setSelected(false);
    }
    if(damager!=0){ // Another nave
        vm.add(new shoot(damager->getPos(),receiver->getPos(), shootTicks));
        playShoot=true;
    }
}

void initGame(){
    nave::setDamageEventCallback(damageEventHandler);
    nave::maxLife = naveMaxLife;
    bullet::naves = &naves;
    /*for(int i=0; i<8; i++)
        for(int j=0; j<8; j++){
            if(i<4&&j<4)        naves.push_back(nave(1,vec2d(20+i*80,20+j*80)));
            else if(i<4&&j>=4)  naves.push_back(nave(2,vec2d(20+i*80,20+j*80)));
            else if(i>=4&&j<4)  naves.push_back(nave(3,vec2d(20+i*80,20+j*80)));
            else if(i>=4&&j>=4) naves.push_back(nave(4,vec2d(20+i*80,20+j*80)));
        }*/
    for(int i=0; i<9; i++)
        naves.push_back(nave(1, vec2d(380+(i%3)*20,height/4-20+(i/3)*20)));
    for(int i=0; i<9; i++)
        naves.push_back(nave(2, vec2d(380+(i%3)*20,3*height/4-20+(i/3)*20)));
}

void soundPlayer(){
    while(running)
        if(playShoot){
            PlaySound(MAKEINTRESOURCE(SHOOT),NULL,SND_RESOURCE|SND_NODEFAULT|SND_ASYNC);
            playShoot=false;
        }
}

int main(int argc, char **argv){
    srand(time(0));
	if (!glfwInit() )
	{
		cout << "Fallo al inicializar GLFW." << endl;
		glfwTerminate();
		return -1;
	}
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, true);
	if( !glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, GLFW_WINDOW))
	{
		cout << "Fallo al crear la ventana." << endl;
		glfwTerminate();
		return -1;
	}
	initGL();
	initGame();
	thread th(&soundPlayer);
	th.detach();
	glfwSetKeyCallback(handleKeypress);
	glfwSetMousePosCallback(handleMouseMove);
	glfwSetMouseButtonCallback(handleMouseButton);

    tw.start(ticksPerSecond);
    fps.start(frapsPerSecond);


	while (running){
	    if(glfwGetWindowParam(GLFW_ACTIVE)){
            if(!windowActive){
                windowActive = true;
            }
	    }else windowActive = false;
	    if(tw.ready()) moveAll();
		if(fps.ready()) draw();
        if(!glfwGetWindowParam(GLFW_OPENED))
            running=false;
	}

	glfwTerminate();

	return 0;
}
