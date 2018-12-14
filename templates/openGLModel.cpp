#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

const float PI = 3.14159265359;

int width = 800,
    height = 600;
sf::RenderWindow window;
bool running = true;
sf::Vector2f camera = {0,-15};
sf::Vector3f cameraTarget = {0,1,0};
float zoom = 5;

/** For keys, movement **/
sf::Vector2f movementMul;
float zoomMul=0;
float movementVelocity = 3,
      zoomVelocity = 0.1;

sf::Vector3f boxSize, initialPosition;
list< vector< sf::Vector3f > > polygons;
list< pair< sf::Vector3f,sf::Vector3f > > lines;

float toRad(float degrees){
    return degrees*PI/180.0;
}

float toDeg(float rad){
    return rad*180.0/PI;
}

void moveCamera(){
    sf::Vector3f position;
    position.x = sin(toRad(camera.x))*cos(toRad(camera.y));
    position.y = -sin(toRad(camera.y));
    position.z = -cos(toRad(camera.x))*cos(toRad(camera.y));

    glRotatef(-camera.y, 1.0, 0, 0);
    glRotatef(180+camera.x, 0, 1.0, 0);
    glTranslatef(-position.x*zoom-cameraTarget.x, -position.y*zoom-cameraTarget.y, -position.z*zoom-cameraTarget.z);
}

void draw(){
    window.clear();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    moveCamera();

    glColor3ub(255,255,255);
    glBegin(GL_LINES);
    for(auto& it:lines){
        glVertex3f(it.first.x, it.first.y, it.first.z);
        glVertex3f(it.second.x, it.second.y, it.second.z);
    }
    glEnd();

    glColor3ub(127,127,127);
    for(auto& it:polygons){
        glBegin(GL_POLYGON);
        for(sf::Vector3f v:it)
            glVertex3f(v.x, v.y, v.z);
        glEnd();
    }
    glPopMatrix();
}

void initializeOpenGL(){
    glViewport(0, 0, width, height);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    GLfloat fieldOfView = 60;
	float tnear = 0.025;
	float tfar = 200;
	GLfloat aspectRatio = (width > height)? float(width)/float(height) : float(height)/float(width);
	GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * tnear;
	GLfloat fW = fH * aspectRatio;
	glFrustum(-fW, fW, -fH, fH, tnear, tfar);
    glMatrixMode(GL_MODELVIEW);
}

void events(){
    for(sf::Event ev; window.pollEvent(ev);){
        float inc = 1;
        switch(ev.type){
        case sf::Event::Closed:
            running = false;
            break;
        case sf::Event::KeyPressed:
            switch(ev.key.code){
            case sf::Keyboard::S:
                movementMul.y = 1;
                break;
            case sf::Keyboard::W:
                movementMul.y = -1;
                break;
            case sf::Keyboard::A:
                movementMul.x = 1;
                break;
            case sf::Keyboard::D:
                movementMul.x = -1;
                break;
            case sf::Keyboard::Add:
                zoomMul = -1;
                break;
            case sf::Keyboard::Subtract:
                zoomMul = 1;
                break;
            }
            break;
        case sf::Event::KeyReleased:
            switch(ev.key.code){
            case sf::Keyboard::S:
            case sf::Keyboard::W:
                movementMul.y = 0;
                break;
            case sf::Keyboard::A:
            case sf::Keyboard::D:
                movementMul.x = 0;
                break;
            case sf::Keyboard::Add:
            case sf::Keyboard::Subtract:
                zoomMul = 0;
                break;
            }
        }
    }
}

void wireframePolygon(const vector< sf::Vector3f >& vec){
    sf::Vector3f temp;
    bool first = true;
    for(sf::Vector3f v : vec){
        if(!first)
            lines.push_front(pair< sf::Vector3f,sf::Vector3f >(temp,v));
        else
            first = false;
        temp = v;
    }
    lines.push_front(pair< sf::Vector3f,sf::Vector3f >(vec.front(),vec.back()));
}

void create(){
    /**     Your code and models here     **
     ** Use lists 'polygons' and 'lines' **
     **        for auto-rendering         **/

     /* Default grid */
     for(float i=-10; i<=10; i+=1){
        pair< sf::Vector3f,sf::Vector3f > t;
        t.first = {i, 0, -10};
        t.second = {i, 0, 10};
        lines.push_front(t);
        t.first = {-10, 0, i};
        t.second = {10, 0, i};
        lines.push_front(t);
    }
}

int main(){
    srand(time(0));

    window.create(sf::VideoMode(width, height), "Rosca", sf::Style::Close);
    window.setFramerateLimit(60);
    initializeOpenGL();

    create();

    while(running){
        events();
        camera.x += movementMul.x*movementVelocity;
        camera.y += movementMul.y*movementVelocity;
        if(camera.y>90)
            camera.y = 90;
        else if(camera.y<-90)
            camera.y = -90;
        zoom += zoomMul*zoomVelocity;
        if(zoom<0)
            zoom = 0;

        draw();
        window.display();
        sf::sleep(sf::milliseconds(10));
    }
}
