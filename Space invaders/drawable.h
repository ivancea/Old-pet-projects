#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <list>

class Drawable{
    static std::list<Drawable*> _li;


public:

    bool toDraw;

    Drawable();
    Drawable(const Drawable& w);
    ~Drawable();

    virtual void draw()=0;

    static const std::list<Drawable*>& getList();

    static void drawAll();
};

#endif // DRAWABLE_H
