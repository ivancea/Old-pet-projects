#include "ivancea.h"

struct fractal_point{
    double angle, proportion, x, y;
    fractal_point(double _x, double _y, double _angle, double _proportion):x(_x),y(_y),angle(_angle),proportion(_proportion){}
    fractal_point(const fractal_point& fp):x(fp.x),y(fp.y),angle(fp.angle),proportion(fp.proportion){}
};

class fractal:public sf::Drawable{
    sf::Texture _tx;
    int _times;
    int _posX, _posY;
    double _size, _rotation;
    vector<fractal_point> _points;

    void draw(sf::RenderTarget& rt, int times, int x, int y, double angle, double size)const{
        if(times<0) return;
        sf::Sprite spr(_tx);

        spr.setPosition(x,y);
        spr.setRotation(angle);
        spr.scale(size, size);
        rt.draw(spr);

        if(times==0) return;

        for(int i=0; i<_points.size(); i++){
            //draw(rt, times-1,x+_points[i].x*size*_tx.getSize().x, y+_points[i].y*size*_tx.getSize().y, _points[i].angle+angle, size*_points[i].proportion);
            draw(rt, times-1,x+_tx.getSize().x*size*sqrt(_points[i].x*_points[i].x+_points[i].y*_points[i].y)*cos(acos(_points[i].x/sqrt(_points[i].x*_points[i].x+_points[i].y*_points[i].y))+toRad(-angle)), y+(_points[i].y/abs(_points[i].y))*_tx.getSize().y*size*sqrt(_points[i].x*_points[i].x+_points[i].y*_points[i].y)*sin(acos(_points[i].x/sqrt(_points[i].x*_points[i].x+_points[i].y*_points[i].y))+toRad(-angle)), _points[i].angle+angle, size*_points[i].proportion);

        }


    }

public:

    fractal(string texture, int times):_times(times),_posX(0),_posY(0),_size(1),_rotation(0){
        _tx.loadFromFile(texture);
    }

    void setPosition(int x, int y){_posX = x; _posY = y;}
    void setPosition(sf::Vector2i& v){_posX = v.x; _posY = v.y;}
    void setScale(double size){_size=size;}
    void setRotation(double rotation){_rotation = rotation;}

    void addPoint(double x, double y, double angle, double proportion){
        _points.push_back(fractal_point(x,y,angle,proportion));
    }

    void addPoint(const fractal_point& fp){
        _points.push_back(fp);
    }

    virtual void draw(sf::RenderTarget& rt, sf::RenderStates rs)const{
        draw(rt, _times, _posX, _posY, _rotation, _size);
    }
};

int main(){
    srand(time(NULL));
    irc ir;
    fractal fr("fractal.png", 20);
    fr.setPosition(50,400);
    fr.setRotation(0);
    fr.setScale(10);
    fr.addPoint(0.11, -0.4, 3, 0.8);
    sf::RenderWindow window;
    window.create(sf::VideoMode(500,500), "Fractal");
    while (window.isOpen()){
        window.clear();
        window.draw(fr);
        window.display();
        sf::Event event;
        if (window.pollEvent(event)){
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            break;
            default:
            break;
            }

        }
        thSleep(100);
    }
    return 0;
}
