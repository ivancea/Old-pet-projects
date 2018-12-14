#ifndef VEC_H
#define VEC_H

#include<cmath>
#define PI 3.14159265358979323846

template<class T> struct vec3;
template<class T> struct vec2;

template<class G> vec3<G> operator-(vec3<G> v){return v*(-1);}
template<class G> vec2<G> operator-(vec2<G> v){return v*(-1);}

template<class G, class N, class M> inline double mixedProduct(const vec3<G> v, const vec3<N> u, const vec3<M> w){return v*(u^w);}
template<class G, class N> inline double distance(const vec3<G> v, const vec3<N> u=vec3<N>()){return sqrt((v.x-u.x)*(v.x-u.x)+(v.y-u.y)*(v.y-u.y)+(v.z-u.z)*(v.z-u.z));}
template<class G, class N> inline double angle(const vec3<G> v, const vec3<N> u){return acos((v*u)/(v.distance()*u.distance()));}

template<class G, class N> static double distance(const vec2<G> v, const vec2<N> u=vec2<N>()){return sqrt((v.x-u.x)*(v.x-u.x)+(v.y-u.y)*(v.y-u.y));}
template<class G, class N> static double angle(const vec2<G> v, const vec2<N> u){return acos((v*u)/(v.distance()*u.distance()));}

template<class T> struct vec3{
    T x, y, z;

    vec3():x(0), y(0), z(0){}
    vec3(T _x, T _y, T _z):x(_x), y(_y), z(_z){}
    vec3(const vec3<T> &v){x=v.x; y=v.y; z=v.z;}
    void clear(){x=0;y=0;z=0;}
    void normalize(){
        T temp = distance();
        if(temp==0) return;
        x/=temp;
        y/=temp;
        z/=temp;
    }
    inline double distance()const{return distance(vec3<T>());}
    inline double angle()const{return angle(*this, vec3<T>());}
    template<class G> inline double distance(const vec3<G> v)const{return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z));}
    template<class G> inline double angle(const vec3<G> v)const{return angle(*this, v);}

    template<class G> vec3<T> operator+(const vec3<G> v)const{return vec3<T>(x+v.x, y+v.y, z+v.z);}
    template<class G> vec3<T> operator-(const vec3<G> v)const{return vec3<T>(x-v.x, y-v.y, z-v.z);}
    template<class G> vec3<T> operator*(const G d)const{return vec3<T>(x*d, y*d, z*d);}
    template<class G> vec3<T> operator/(const G d)const{return vec3<T>(x/d, y/d, z/d);}
    template<class G> T operator*(const vec3<G> v)const{return x*v.x + y*v.y + z*v.z;}
    template<class G> vec3<T> operator^(const vec3<G> v)const{return vec3<T>(y*v.z - z*v.y, x*v.z - z*v.x, x*v.y - y*v.x);}

    template<class G> vec3<T> operator=(const vec3<G> v){x=v.x; y=v.y; z=v.z; return *this;}
    template<class G> vec3<T> operator+=(const vec3<G> v){x+=v.x; y+=v.y; z+=v.z; return *this;}
    template<class G> vec3<T> operator-=(const vec3<G> v){x-=v.x; y-=v.y; z-=v.z; return *this;}
    template<class G> vec3<T> operator*=(const G d){x*=d; y*=d; z*=d; return *this;}
    template<class G> vec3<T> operator/=(const G d){x/=d; y/=d; z/=d; return *this;}
    template<class G> vec3<T> operator^=(const vec3<G> v){return *this=*this^v;}

    template<class G> bool operator==(const vec3<G> v)const{return v.x==x && v.y==y && v.z==z;}
    template<class G> bool operator!=(const vec3<G> v)const{return !(v.x==x && v.y==y && v.z==z);}
};

template<class T> struct vec2{
    T x, y;

    vec2():x(0), y(0){}
    vec2(T _x, T _y):x(_x), y(_y){}
    vec2(const vec2 &v){x=v.x; y=v.y;}
    void clear(){x=0;y=0;}
    void normalize(){
        T temp = distance();
        if(temp==0) return;
        x/=temp;
        y/=temp;
    }
    inline double distance()const{return distance(vec2<T>());}
    inline double angle()const{return angle(*this, vec2<T>());}
    template<class G> T distance(const vec2<G> v)const{return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y));}
    template<class G>double angle(const vec2<G> v)const{return angle(*this, v);}

    template<class G> vec2<T> operator+(const vec2<G> v)const{return vec2(x+v.x, y+v.y);}
    template<class G> vec2<T> operator-(const vec2<G> v)const{return vec2(x-v.x, y-v.y);}
    template<class G> vec2<T> operator*(const G d)const{return vec2(x*d, y*d);}
    template<class G> vec2<T> operator/(const G d)const{return vec2(x/d, y/d);}
    template<class G> T operator*(const vec2<G> v)const{return x*v.x + y*v.y;}

    template<class G> vec2<T> operator=(const vec2<G> v){x=v.x; y=v.y; return *this;}
    template<class G> vec2<T> operator+=(const vec2<G> v){x+=v.x; y+=v.y; return *this;}
    template<class G> vec2<T> operator-=(const vec2<G> v){x-=v.x; y-=v.y; return *this;}
    template<class G> vec2<T> operator*=(const G d){x*=d; y*=d; return *this;}
    template<class G> vec2<T> operator/=(const G d){x/=d; y/=d; return *this;}

    template<class G> bool operator==(const vec2<G> v)const{return v.x==x && v.y==y;}
    template<class G> bool operator!=(const vec2<G> v)const{return !(v.x==x && v.y==y);}
};


typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<int> vec3i;
typedef vec3<long long> vec3l;

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;
typedef vec2<long long> vec2l;

#endif // VEC_H
