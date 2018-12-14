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
    srand(66);
    for(auto& it:polygons){
        glColor3ub(rand()%256,rand()%256,rand()%256);
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

    /**
    float length = 2,
          increment = 0.005,
          radius = 0.5,
          rounds = 5, // Rounds each length unit
          rotationIncrement = 0.1, // Radians
          topSize = 0.5,
          topThickness = PI/2; // Radians
    rounds *= PI;
    for(float i=0; i<length; i+=increment){
        vector< sf::Vector3f > t;
        for(float rad=0; rad<PI*2; rad+=rotationIncrement)
            t.push_back({cos(rad)*radius+cos(i*50)*0.1, i, sin(rad)*radius+sin(i*50)*0.1});
        //t.push_back({cos(i*rounds)*(radius+topSize), i, sin(i*rounds)*(radius+topSize)});
        polygons.push_front(t);
    }**/

    /* Create layers */
    /**float length = 2,
          increment = 0.005,
          radius = 0.2,
          rounds = 1,
          rotationIncrement = 0.1, // Radians
          topSize = 0.75,
          topThickness = PI; // Radians
    rounds *= PI;
    /**for(float i=0; i<length; i+=increment){
        vector< sf::Vector3f > t;
        for(float rad=topThickness/2+i*rounds; rad<PI*2-topThickness/2+i*rounds; rad+=rotationIncrement)
            t.push_back({cos(rad)*radius, i, sin(rad)*radius});
        t.push_back({cos(i*rounds)*(radius+topSize), i, sin(i*rounds)*(radius+topSize)});
        polygons.push_front(t);

        t.clear();
        t.push_back({cos(topThickness/2+i*rounds)*radius, i, sin(topThickness/2+i*rounds)*radius});
        t.push_back({cos(i*rounds+PI)*(radius+topSize), i, sin(i*rounds+PI)*(radius+topSize)});
        t.push_back({cos(PI*2-topThickness/2+i*rounds)*radius, i, sin(PI*2-topThickness/2+i*rounds)*radius});
        polygons.push_front(t);
    }**/

    float length = 2,
          increment = 0.005,
          radius = 0.2,
          rounds = 1,
          topSize = 0.75;
    rounds *= PI;
    for(float i=0; i<length; i+=increment){
        vector< sf::Vector3f > t;
        t.push_back({cos(i*rounds)*radius, i, sin(i*rounds)*radius});
        t.push_back({cos(i*rounds+PI/2)*(radius+topSize), i, sin(i*rounds+PI/2)*(radius+topSize)});
        t.push_back({cos(i*rounds+PI)*radius, i, sin(i*rounds+PI)*radius});
        t.push_back({cos(i*rounds-PI/2)*(radius+topSize), i, sin(i*rounds-PI/2)*(radius+topSize)});
        polygons.push_front(t);
    }

    /* Create lines */
    /*sf::Vector3f a(-1,-1,-1),b,c;
    for(const vector< sf::Vector3f >& v:polygons){
        if(v.size()<3)
            continue;
        if(a.x!=-1){
            lines.push_front(pair< sf::Vector3f,sf::Vector3f >(a,v.back()));
            lines.push_front(pair< sf::Vector3f,sf::Vector3f >(b,v[v.size()-2]));
            lines.push_front(pair< sf::Vector3f,sf::Vector3f >(c,v.front()));
        }
        a = v.back();
        b = v[v.size()-2];
        c = v.front();
    }
    wireframePolygon(polygons.front());
    wireframePolygon(polygons.back());*/

    int numLayers = polygons.size();
    for(auto it = polygons.begin();; it++){
        auto it2 = it;
        it2++;
        if(it2==polygons.end())
            break;
        for(int i=0; i<it->size()-1; i++){
            vector< sf::Vector3f > t;
            t.push_back((*it)[i]);
            t.push_back((*it)[i+1]);
            t.push_back((*it2)[i+1]);
            t.push_back((*it2)[i]);
            polygons.push_front(t);
        }
        vector< sf::Vector3f > t;
        t.push_back((*it)[0]);
        t.push_back((*it)[it->size()-1]);
        t.push_back((*it2)[it2->size()-1]);
        t.push_back((*it2)[0]);
        polygons.push_front(t);
    }
    auto it = polygons.end();
    auto it2 = polygons.end();
    it2--;
    for(int i=0; i<numLayers-1; i++)
        it--;
    polygons.erase(it,it2);

    cout << polygons.size() << endl;
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
