#include <iostream>

using namespace std;

template<class T>
class icf_queue{
    T** _list;
    size_t _size, _listSize;

    void shift(uint32_t n=1){
        for(uint32_t i=0; i<_size-n; i++)
            _list[i] = _list[i+n];
    }

    void move(T** from, T** to, size_t n = 0){
        if(!n) n=_size;
        for(uint32_t i=0; i<n; i++)
            to[i] = from[i];
    }

public:
    icf_queue():_size(0),_listSize(0){}
    icf_queue(const icf_queue&)=delete;

    T& front(){
        return *_list[0];
    }

    T& back(){
        return *_list[_size-1];
    }

    void push(T element=T()){
        if(_listSize<=_size){
            T** temp = new T*[_listSize+16];
            move(_list, temp);
            if(_listSize==1)
                delete _list;
            else if(_listSize>1)
                delete[] _list;
            _listSize+=16;
            _list = temp;
        }
        _list[_size] = new T(element);
        ++_size;
    }

    void pop(){
        if(!_size) return;
        delete _list[0];
        shift();
        --_size;
    }

    bool empty(){
        return !_size;
    }

    size_t size(){
        return _size;
    }
};

int main(){
    icf_queue<double> q;
    for(int i=0; i<11; i+=2)
        q.push(156.15561*(i*5));
    while(!q.empty()){
        cout << q.front() << endl;
        q.pop();
    }
}
