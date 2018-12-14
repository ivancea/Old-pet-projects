#include <iostream>
#include <vector>
#include <typeinfo>

using namespace std;

template<typename T> class vars{
    vector<T> v;
public:
    vars(){}
    vars(const vars &c){
        for(int i=0; i<c.count(); i++)
            v.push_back(c[i]);
    }
    void add(T a){
        v.push_back(a);
    }
    int count()const{return v.size();}
    T operator[](int n)const{
        if(n<0 || n>=count()) return T();
        return v[n];
    }
    vars<T> operator(T a){
        vars<T> v;
        v.add(a);
        return v;
    }
};

template<class T> vars<T> operator,(vars<T> a, vars<T> b){
    for(int i=0; i<b.count(); i++)
        a.add(b[i]);
    return a;
}

template<class T> vars<T> operator,(vars<T> a, T b){
    a.add(b);
    return a;
}

template<class T> vars<T> operator,(T a, vars<T> b){
    b.add(a);
    return b;
}

template<class T> void func(vars<T> c){
    for(int i=0; i<c.count(); i++)
        cout << c[i] << endl;
}

int main(){
    [](vars<int> c){
    for(int i=0; i<c.count(); i++)
        cout << c[i] << endl;
    }((vars<int>(),849,1,8,5,0,4,21,1,1212));
    return 0;
}
