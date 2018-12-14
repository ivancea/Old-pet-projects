#include <GL/glfw.h>

#include <iostream>
#include <thread>
#include <windows.h>
#include <list>
#include <map>
#include <cmath>

#include "tickwaiter.h"
#include "fpsmanager.h"
#include "vec.h"
#include "drawable.h"
#include "bullet.h"
#include "block.h"
#include "mapcreator.h"

using namespace std;

/**** CONFIGURATION AND VARIABLES ****/

int width = 800, height = 600;
bool running = true;

string title = "Title";

const int tps = 200,
           fps = 30;
tickWaiter tw;
fpsManager fpsm(10);

Vec2d mousePosition;
map<int,bool> keys;

Block ***m;
int mX=120, mY=60;
double mW=10, mH=10;
Vec2d camera(0,0);
double cameraMargin = width/4;

Vec2d player(100,300);
bool facingRight=true;
double pW = 17, pH = 35;
double velocity = 0,
        velocityOnJump = -4,
        gravity = 0.075;

list<Bullet*> playerBullets;

GLuint texture;

/*************************************/

GLuint LoadTexture(const char* TextureName){

   GLuint Texture;  //variable for texture
   glGenTextures(1,&Texture); //allocate the memory for texture
   glBindTexture(GL_TEXTURE_2D,Texture); //Binding the texture

   if(glfwLoadTexture2D(TextureName, GLFW_BUILD_MIPMAPS_BIT)){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      return Texture;
   } else {
       cerr << "Error loading: " << TextureName << endl;
       return -1;
   }
}

double getTop(Block*** m, int mX, int mY, double mW, double mH,
                            double pX, double pY, double pW, double pH){
    int minX = pX/mW,
        minY = pY/mH-1,
        maxX = (pX+pW-1)/mW,
        i;
    bool brk = false;
    for(i=minY; i>=0 && !brk; i--){
        for(int j=minX; j<=maxX; j++)
            if(j>=0 && j<mX && i<mY && m[j][i]){
                brk = true;
                break;
            }
        if(brk)
            break;
    }
    if(i<0)
        return 0;
    return (i+1)*mH;
}

double getBottom(Block*** m, int mX, int mY, double mW, double mH,
                              double pX, double pY, double pW, double pH){
    int minX = pX/mW,
        maxX = (pX+pW-1)/mW,
        maxY = (pY+pH-1)/mH,
        i;
    bool brk = false;
    for(i=maxY; i<mY; i++){
        for(int j=minX; j<=maxX; j++)
            if(j>=0 && j<mX && i>=0 && m[j][i]){
                brk = true;
                break;
            }
        if(brk)
            break;
    }

    if(i>=mY)
        i=mY;
    return i*mH-pH;
}

bool checkCollision(Block*** m, int mX, int mY, double mW, double mH,
                                 double pX, double pY, double pW, double pH){
    if(pX+pW>mX*mW || pY+pH>mY*mH || pX<0 || pY<0)
        return true;
    int minX = pX/mW,
        minY = pY/mH,
        maxX = (pX+pW-1)/mW,
        maxY = (pY+pH-1)/mH;
    for(int i=minX; i<=maxX; i++)
        for(int j=minY; j<=maxY; j++)
            if(m[i][j])
                return true;
    return false;
}

void mousePositionCallback(int x, int y){                            /** MOUSE CALLBACK **/
    mousePosition.x = x;
    mousePosition.y = y;
}

void mouseButtonCallback(int button, int action){
    if(button==GLFW_MOUSE_BUTTON_1){
        if(action==GLFW_PRESS){
            int x=(mousePosition.x+camera.x)/mW,
                y=(mousePosition.y+camera.y)/mH;
            if(x>=0 && y>=0 && x<mX && y<mY){
                if(m[x][y]){
                    delete m[x][y];
                    m[x][y] = nullptr;
                }else m[x][y] = new Block(Vec2i(x,y), true, texture);
                if(m[x][y] && checkCollision(m, mX,mY, mW,mH,
                                             player.x, player.y, pW, pH)){
                    delete m[x][y];
                    m[x][y] = nullptr;
                }
            }
        }else{

        }
    }else if(button==GLFW_MOUSE_BUTTON_2){
        if(action==GLFW_PRESS){

        }else{

        }
    }
}

void keyCallback(int key, int action){                              /** KEY CALLBACK **/
    if(action==GLFW_PRESS){
        keys[key] = true;
        switch(key){
        case GLFW_KEY_ESC:
            running = false;
            break;
        case GLFW_KEY_UP:
            if(getBottom(m, mX,mY, mW,mH,
                               player.x, player.y, pW,pH) == player.y)
            velocity=velocityOnJump;
            break;
        case 'Z':
            playerBullets.push_back(new Bullet(Vec2d(player.x+facingRight*pW, player.y+pH/4), Vec2d(facingRight*4-2,0), 2, Vec3ub(0,255,255)));
            break;
        case 'M':
            toFile(m, mX, mY, player, camera, "map.icf");
        }
    }else if(action==GLFW_RELEASE){
        keys[key] = false;
    }
}

bool init(){                                                         /** INIT **/
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

    tw.start(tps);
    fpsm.start(fps);

    texture = LoadTexture("bloque.tga");

    if(fromFile(m, mX, mY, player, camera, "map.icf")){
        for(int i=0; i<mX; i++)
            for(int j=0; j<mY; j++)
                if(m[i][j])
                    m[i][j]->setTexture(texture);
    }else{
        Block::size = Vec2d(mW,mH);

        m = new Block**[mX];
        for(int i=0; i<mX; i++){
            m[i] = new Block*[mY];
            for(int j=0; j<mY; j++)
                m[i][j] = (sin((double)i/3)*(double)mY/6+(double)mY/2<j?new Block(Vec2i(i,j), true, texture):nullptr);//(j>20 || (j==20 && rand()%2)?new Block(Vec2i(i,j), true, texture):nullptr);
        }
    }

    return true;
}

void draw(){                                                         /** DRAW **/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslated(-camera.x, -camera.y, 0);

    Drawable::drawAll();

    /*for(int i=0; i<mX; i++)
        for(int j=0; j<mY; j++)
            if(m[i][j]){
                glColor3ub(128,255,128);
                glBegin(GL_POLYGON);
                glVertex2d(i*mW, j*mH);
                glVertex2d((i+1)*mW, j*mH);
                glVertex2d((i+1)*mW, (j+1)*mH);
                glVertex2d(i*mW, (j+1)*mH);
                glEnd();
            }*/

    glColor3ub(255,255,255);
    glBegin(GL_POLYGON);
    glVertex2d(player.x, player.y);
    glVertex2d(player.x+pW, player.y);
    glVertex2d(player.x+pW, player.y+pH);
    glVertex2d(player.x, player.y+pH);
    glEnd();

    glfwSwapBuffers();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glfwPollEvents();
}

void tick(){                                                         /** TICK **/
    if(!glfwGetWindowParam(GLFW_OPENED))
        running = false;

    /** MOVE BULLETS **/
    for(auto it=playerBullets.begin(); it!=playerBullets.end();){
        (*it)->move();
        if((*it)->getPosition().x<0 || (*it)->getPosition().y<0
           || (*it)->getPosition().x>=mW*mX || (*it)->getPosition().y>=mH*mY){
            delete *it;
            it = playerBullets.erase(it);
            continue;
        }
        if(m[int((*it)->getPosition().x/mW)][int((*it)->getPosition().y/mW)]){
            delete m[int((*it)->getPosition().x/mW)][int((*it)->getPosition().y/mW)];
            m[int((*it)->getPosition().x/mW)][int((*it)->getPosition().y/mW)] = false;
            delete *it;
            it = playerBullets.erase(it);
        }else ++it;
    }

    /** MOVE PLAYER (Y) **/
    velocity += gravity;
    if(false && checkCollision(m, mX,mY, mW,mH,
                      player.x, player.y, pW,pH)){
        velocity=0;
    }else if(velocity<0){
        double t = getTop(m, mX,mY, mW,mH,
                      player.x, player.y, pW,pH);
        if(player.y-t>(-velocity))
            player.y += velocity;
        else{
            player.y = t;
            velocity = 0;
        }
    }else if(velocity>0){
        double t = getBottom(m, mX,mY, mW,mH,
                      player.x, player.y, pW,pH);
        if(t-player.y>velocity)
            player.y += velocity;
        else{
            player.y = t;
            velocity=0;
        }
    }
    /*for(double t = velocity; abs(t)>0; t-=t/abs(t)){
        if(t==0)
            break;
        double movement = (abs(t)>=1?t/abs(t):t);
        if(!checkCollision(m, mX,mY, mW,mH,
                               player.x, player.y+movement, pW,pH))
            player.y+=movement;
        else{
            velocity=0;
            break;
        }
        if(abs(movement)!=1)
            break;
    }
    if(checkCollision(m, mX,mY, mW,mH,
                          player.x, ceil(player.y), pW,pH))
        player.y = floor(player.y);*/

    /** MOVE PLAYER (X) **/
    if(keys[GLFW_KEY_RIGHT]){
        if(!checkCollision(m, mX,mY, mW,mH,
                               player.x+1, ceil(player.y), pW,pH))
            player.x+=1;
        facingRight=true;
    }else if(keys[GLFW_KEY_LEFT]){
        if(!checkCollision(m, mX,mY, mW,mH,
                               player.x-1, ceil(player.y), pW,pH))
            player.x-=1;
        facingRight=false;
    }

    /** MOVE CAMERA **/
    if(player.x<cameraMargin+camera.x)
        camera.x=player.x-cameraMargin;
    else if(player.x>width-cameraMargin+camera.x)
        camera.x=player.x-(width-cameraMargin);
    if(player.y<cameraMargin+camera.y)
        camera.y=player.y-cameraMargin;
    else if(player.y>height-cameraMargin+camera.y)
        camera.y=player.y-(height-cameraMargin);
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
        cerr << "Error";
        return 1;
    }

    glfwSetMousePosCallback(&mousePositionCallback);
    glfwSetMouseButtonCallback(&mouseButtonCallback);
    glfwSetKeyCallback(&keyCallback);

    while(running){
        if(fpsm.ready())
            draw();
        if(tw.ready())
            tick();
    }

    /**thread thTicks(&threadTicks);

    while(running){
        fpsm.wait();
        draw();
    }
    if(thTicks.joinable())
        thTicks.join();**/

    glfwTerminate();
    return 0;
}
