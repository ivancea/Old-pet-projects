#include "drawable.h"

#include <list>

using namespace std;

list<Drawable*> Drawable::_li;


Drawable::Drawable(){
    _li.push_front(this);
    toDraw=true;
}

Drawable::Drawable(const Drawable& w){
    _li.push_front(this);
}

Drawable::~Drawable(){
    auto it = _li.begin();
    while((*it)!=this)
        ++it;
    _li.erase(it);
}

void Drawable::drawAll(){
    for(Drawable* d:_li){
        if(d->toDraw)
            d->draw();
    }
}

const list<Drawable*>& Drawable::getList(){
    return _li;
}
