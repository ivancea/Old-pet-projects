#include <iostream>
#include <cmath>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#define PI 3.14159265358979323846

double toDeg(double rad){return (rad*180)/PI;}
double toRad(double deg){return (deg*PI)/180;}

using namespace std;

struct vector3D{
    double x, y, z;

    vector3D():x(0), y(0), z(0){}
    vector3D(double _x, double _y, double _z):x(_x), y(_y), z(_z){}
    vector3D(const vector3D &v){x=v.x; y=v.y; z=v.z;}
    inline double distance(const vector3D v=vector3D())const{return distance(*this, v);}
    inline double angle(const vector3D v=vector3D())const{return angle(*this, v);}

    vector3D operator+(const vector3D v)const{return vector3D(x+v.x, y+v.y, z+v.z);}
    vector3D operator-(const vector3D v)const{return vector3D(x-v.x, y-v.y, z-v.z);}
    vector3D operator*(const double d)const{return vector3D(x*d, y*d, z*d);}
    vector3D operator/(const double d)const{return vector3D(x/d, y/d, z/d);}
    double operator*(const vector3D v)const{return x*v.x + y*v.y + z*v.z;}
    vector3D operator^(const vector3D v)const{return vector3D(y*v.z - z*v.y, x*v.z - z*v.x, x*v.y - y*v.x);}

    vector3D operator=(const vector3D v){x=v.x; y=v.y; z=v.z; return *this;}
    vector3D operator+=(const vector3D v){x+=v.x; y+=v.y; z+=v.z; return *this;}
    vector3D operator-=(const vector3D v){x-=v.x; y-=v.y; z-=v.z; return *this;}
    vector3D operator*=(const double d){x*=d; y*=d; z*=d; return *this;}
    vector3D operator/=(const double d){x/=d; y/=d; z/=d; return *this;}
    vector3D operator^=(const vector3D v){return *this=*this^v;}

    static inline double mixedProduct(const vector3D v, const vector3D u, const vector3D w){return v*(u^w);}
    static inline double distance(const vector3D v, const vector3D u=vector3D()){return sqrt((v.x-u.x)*(v.x-u.x)+(v.y-u.y)*(v.y-u.y)+(v.z-u.z)*(v.z-u.z));}
    static inline double angle(const vector3D v, const vector3D u){return acos((v*u)/(v.distance()*u.distance()));}
};

struct vector2D{
    double x, y;

    vector2D():x(0), y(0){}
    vector2D(double _x, double _y):x(_x), y(_y){}
    vector2D(const vector2D &v){x=v.x; y=v.y;}
    double distance(const vector2D v=vector2D())const{return distance(*this, v);}
    double angle(const vector2D v=vector2D()){return angle(*this, v);}

    vector2D operator+(const vector2D v)const{return vector2D(x+v.x, y+v.y);}
    vector2D operator-(const vector2D v)const{return vector2D(x-v.x, y-v.y);}
    vector2D operator*(const double d)const{return vector2D(x*d, y*d);}
    vector2D operator/(const double d)const{return vector2D(x/d, y/d);}
    double operator*(const vector2D v)const{return x*v.x + y*v.y;}

    vector2D operator=(const vector2D v){x=v.x; y=v.y; return *this;}
    vector2D operator+=(const vector2D v){x+=v.x; y+=v.y; return *this;}
    vector2D operator-=(const vector2D v){x-=v.x; y-=v.y; return *this;}
    vector2D operator*=(const double d){x*=d; y*=d; return *this;}
    vector2D operator/=(const double d){x/=d; y/=d; return *this;}

    static double distance(const vector2D v, const vector2D u=vector2D()){return sqrt((v.x-u.x)*(v.x-u.x)+(v.y-u.y)*(v.y-u.y));}
    static double angle(const vector2D v, const vector2D u){return acos((v*u)/(v.distance()*u.distance()));}
};

void drawLine(sf::Image &Imagen, vector2D v, vector2D u, sf::Color c = sf::Color(0,0,0)){
    float dX,dY,Pasos;
    float xInc,yInc,i,x,y;
    dX = v.x - u.x;
    dY = v.y - u.y;
    if (fabs(dX) > fabs(dY))
        Pasos = fabs(dX);
    else
        Pasos = fabs(dY);
    xInc = dX/Pasos;
    yInc = dY/Pasos;
    x = v.x;
    y = v.y;
    if(x>=0 && x<Imagen.getSize().x && y>=0 && y<Imagen.getSize().y)
        Imagen.setPixel(x,y,c);
    for (i=1; i<=Pasos; i++){
        x -= xInc;
        y -= yInc;
        if(x>=0 && x<Imagen.getSize().x && y>=0 && y<Imagen.getSize().y)
            Imagen.setPixel(x,y,c);
    }
}

struct camera{
    double x, y, z;
    double v, h;
    camera():x(0), y(0), z(0), v(0), h(0){}
    camera(double _x, double _y, double _z):x(_x), y(_y), z(_z), v(0), h(0){}
    camera(double _x, double _y, double _z, double _v, double _h):x(_x), y(_y), z(_z), v(_v), h(_h){}
    camera(vector3D v):x(v.x), y(v.y), z(v.z), v(0), h(0){}
    camera(vector3D v, double _v, double _h):x(v.x), y(v.y), z(v.z), v(_v), h(_h){}
    vector3D getVector(){return vector3D(x,y,z);}
};

vector2D point(camera c, vector3D point, uint32_t imageX, uint32_t imageY){
    double vertical = 0, horizontal = 0;
    double x=point.x - c.x, y=point.y - c.y, z=point.z - c.z;
    double tempX = sqrt(x*x + z*z)*cos(acos(x/sqrt(x*x+z*z))+toRad(c.h));
    double tempZ = z=sqrt(x*x + z*z)*sin(acos(x/sqrt(x*x+z*z))+toRad(c.h));
    x=tempX;
    z=tempZ;
    vertical = imageY/2-(90*y/z)*(imageX/120);
    horizontal = imageX/2+(90*x/z)*(imageX/120);
    return vector2D((int)horizontal, (int)vertical);
}

void line(sf::Image &img, camera &c, vector3D a, vector3D b){
    /**double vertical = 0, toVertical = 0;
    double horizontal = 0, toHorizontal = 0;
    double x=a.x - c.x, y=a.y - c.y, z=a.z - c.z; /**comprobar que los puntos estean en el campo de vision (delante)**
    double toX=b.x - c.x, toY=b.y - c.y, toZ=b.z - c.z;
    if(toDeg(asin(x/sqrt(x*x+z*z)))-c.h>90 || toDeg(asin(x/sqrt(x*x+z*z)))-c.h<-90
     || (toDeg(asin(toX/sqrt(toX*toX+toZ*toZ)))-c.h)>90 || (toDeg(asin(toX/sqrt(toX*toX+toZ*toZ)))-c.h)<-90)
        return;
    double temp = sqrt(x*x + z*z)*cos(acos(x/sqrt(x*x+z*z))+toRad(c.h));
    z=sqrt(x*x + z*z)*sin(acos(x/sqrt(x*x+z*z))+toRad(c.h));
    x=temp;
    temp = sqrt(toX*toX + toZ*toZ)*cos(acos(toX/sqrt(toX*toX+toZ*toZ))+toRad(c.h));
    toZ=sqrt(toX*toX + toZ*toZ)*sin(acos(toX/sqrt(toX*toX+toZ*toZ))+toRad(c.h));
    toX=temp;
    /*vertical = img.getSize().y/2-toDeg(tan(y/z))*(img.getSize().x/120);
    horizontal = img.getSize().x/2+toDeg(tan(x/z))*(img.getSize().x/120);
    toVertical = img.getSize().y/2-toDeg(tan(toY/toZ))*(img.getSize().x/120);
    toHorizontal = img.getSize().x/2+toDeg(tan(toX/toZ))*(img.getSize().x/120);*
    if(acos(x*cos(c.h)+y*sin(c.v)+z*sin(c.h))/(sqrt(x*x+y*y+z*z))>90
       || acos(toX*cos(c.h)+toY*sin(c.v)+toZ*sin(c.h))/(sqrt(toX*toX+toY*toY+toZ*toZ))>90)
        return;
    vertical = img.getSize().y/2-(90*y/z)*(img.getSize().x/120);
    horizontal = img.getSize().x/2+(90*x/z)*(img.getSize().x/120);
    toVertical = img.getSize().y/2-(90*toY/toZ)*(img.getSize().x/120);
    toHorizontal = img.getSize().x/2+(90*toX/toZ)*(img.getSize().x/120);
    drawLine(img, vector2D((int)horizontal, (int)vertical), vector2D((int)toHorizontal, (int)toVertical));**/
    drawLine(img, point(c, a, img.getSize().x, img.getSize().y), point(c, b, img.getSize().x, img.getSize().y));
}

void square(sf::Image &img, camera &c, vector3D v, double l){
    double x=v.x, y=v.y, z=v.z;
    line(img, c, vector3D(x,y,z), vector3D(x,l+y,z));
    line(img, c, vector3D(l+x,y,z), vector3D(x,y,z));
    line(img, c, vector3D(x,l+y,z), vector3D(l+x,l+y,z));
    line(img, c, vector3D(l+x,l+y,z), vector3D(l+x,y,z));
}

void cube(sf::Image &img, camera &c, vector3D v, double l){
    double x=v.x, y=v.y, z=v.z;
    line(img, c, vector3D(x,y,z), vector3D(x,l+y,z));
    line(img, c, vector3D(l+x,y,z), vector3D(x,y,z));
    line(img, c, vector3D(x,l+y,z), vector3D(l+x,l+y,z));
    line(img, c, vector3D(l+x,l+y,z), vector3D(l+x,y,z));

    line(img, c, vector3D(x,l+y,l+z), vector3D(l+x,l+y,l+z));
    line(img, c, vector3D(l+x,l+y,l+z), vector3D(l+x,y,l+z));
    line(img, c, vector3D(l+x,y,l+z), vector3D(x,y,l+z));
    line(img, c, vector3D(x,y,l+z), vector3D(x,l+y,l+z));

    line(img, c, vector3D(x,l+y,z), vector3D(x,l+y,l+z));
    line(img, c, vector3D(l+x,l+y,z), vector3D(l+x,l+y,l+z));
    line(img, c, vector3D(l+x,y,z), vector3D(l+x,y,l+z));
    line(img, c, vector3D(x,y,z), vector3D(x,y,l+z));
}

void draw(sf::Image &img, camera &me){
    img.create(img.getSize().x, img.getSize().y, sf::Color(255,255,255));
    cube(img, me, vector3D(-1,-1,20), 2);
    cube(img, me, vector3D(-1,-1,24), 2);
    cube(img, me, vector3D(3,-1,20), 2);
    cube(img, me, vector3D(-1,3,20), 2);
    cube(img, me, vector3D(-3,3,-20), 2);
}

int main(){
    sf::RenderWindow w(sf::VideoMode(800,600), "Prueba");
    sf::Texture t;
    sf::Image img;
    camera me;
    img.create(800,600, sf::Color(255,255,255));
    bool changed = true;
    while (w.isOpen())
    {
        if(changed){
            draw(img, me);
            t.loadFromImage(img);
            w.draw(sf::Sprite(t));
            w.display();
            changed = false;
        }
        sf::Event event;
        while (w.pollEvent(event))
        {
            switch(event.type){
            case sf::Event::Closed:
                w.close();
                break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::W){
                    me.x += cos(-toRad(me.h)+PI/2)*cos(toRad(me.v));
                    me.z += sin(-toRad(me.h)+PI/2)*cos(toRad(me.v));
                    me.y += sin(toRad(me.v));
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::S){
                    me.x -= cos(-toRad(me.h)+PI/2)*cos(toRad(me.v));
                    me.z -= sin(-toRad(me.h)+PI/2)*cos(toRad(me.v));
                    me.y -= sin(toRad(me.v));
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::D){
                    me.x += sin(toRad(me.h+90));
                    me.z += cos(toRad(me.h+90));
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::A){
                    me.x -= sin(toRad(me.h+90));
                    me.z -= cos(toRad(me.h+90));
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::Key::Left){
                    me.h-=2;
                    if(me.h<-180)
                        me.h+=360;
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::Right){
                    me.h+=2;
                    if(me.h>180)
                        me.h-=360;
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::Up){
                    if(me.v+2<=90)
                        me.v+=2;
                    else me.v=90;
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::Down){
                    if(me.v-2>=-90)
                        me.v-=2;
                    else me.v = -90;
                    changed = true;
                }/*else
                if(event.key.code == sf::Keyboard::Q){
                    me.x += cos(toRad(me.h)+PI);
                    me.z += sin(toRad(me.h)+PI);
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::E){
                    me.x += cos(toRad(me.h));
                    me.z += sin(toRad(me.h));
                    changed = true;
                }*/else
                if(event.key.code == sf::Keyboard::F){
                    me.y += cos(toRad(me.v)+PI);
                    me.z += sin(toRad(me.v)+PI);
                    changed = true;
                }else
                if(event.key.code == sf::Keyboard::R){
                    me.y -= cos(toRad(me.v)+PI);
                    me.z -= sin(toRad(me.v)+PI);
                    changed = true;
                }
                cout << me.x << " " << me.y << " " << me.z << endl;
                break;
            }
        }
    }
    return 0;
}
