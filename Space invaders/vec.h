#ifndef VEC_H
#define VEC_H

#include<cmath>

template<class T> struct Vec3{
    T x, y, z;

    Vec3():x(0), y(0), z(0){}
    Vec3(T _x, T _y, T _z):x(_x), y(_y), z(_z){}
    Vec3(const Vec3<T> &v){x=v.x; y=v.y; z=v.z;}
    void clear(){x=0;y=0;z=0;}
    Vec3<T>& normalize(){
        T temp = distance();
        if(temp==0)
            return *this;
        x/=temp;
        y/=temp;
        z/=temp;
        return *this;
    }
    inline double distance()const{return distance(Vec3<T>());}
    inline double angle()const{return angle(*this, Vec3<T>());}
    template<class G> inline double distance(const Vec3<G> v)const{return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z));}
    template<class G> inline double angle(const Vec3<G> v)const{return angle(*this, v);}

    template<class G> Vec3<T> operator+(const Vec3<G> v)const{return Vec3<T>(x+v.x, y+v.y, z+v.z);}
    template<class G> Vec3<T> operator-(const Vec3<G> v)const{return Vec3<T>(x-v.x, y-v.y, z-v.z);}
    template<class G> Vec3<T> operator*(const G d)const{return Vec3<T>(x*d, y*d, z*d);}
    template<class G> Vec3<T> operator/(const G d)const{return Vec3<T>(x/d, y/d, z/d);}
    template<class G> T operator*(const Vec3<G> v)const{return x*v.x + y*v.y + z*v.z;}
    template<class G> Vec3<T> operator^(const Vec3<G> v)const{return Vec3<T>(y*v.z - z*v.y, x*v.z - z*v.x, x*v.y - y*v.x);}

    template<class G> Vec3<T> operator=(const Vec3<G> v){x=v.x; y=v.y; z=v.z; return *this;}
    template<class G> Vec3<T> operator+=(const Vec3<G> v){x+=v.x; y+=v.y; z+=v.z; return *this;}
    template<class G> Vec3<T> operator-=(const Vec3<G> v){x-=v.x; y-=v.y; z-=v.z; return *this;}
    template<class G> Vec3<T> operator*=(const G d){x*=d; y*=d; z*=d; return *this;}
    template<class G> Vec3<T> operator/=(const G d){x/=d; y/=d; z/=d; return *this;}
    template<class G> Vec3<T> operator^=(const Vec3<G> v){return *this=*this^v;}
};

template<class T> struct Vec2{
    T x, y;

    Vec2():x(0), y(0){}
    Vec2(T _x, T _y):x(_x), y(_y){}
    Vec2(const Vec2 &v){x=v.x; y=v.y;}
    void clear(){x=0;y=0;}
    Vec2<T>& normalize(){
        T temp = distance();
        if(temp==0)
            return *this;
        x/=temp;
        y/=temp;
        return *this;
    }
    inline double distance()const{return distance(Vec2<T>());}
    inline double angle()const{return angle(*this, Vec2<T>());}
    template<class G> T distance(const Vec2<G> v)const{return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y));}
    template<class G> T angle(const Vec2<G> v){return angle(*this, v);}

    template<class G> Vec2<T> operator+(const Vec2<G> v)const{return Vec2(x+v.x, y+v.y);}
    template<class G> Vec2<T> operator-(const Vec2<G> v)const{return Vec2(x-v.x, y-v.y);}
    template<class G> Vec2<T> operator*(const G d)const{return Vec2(x*d, y*d);}
    template<class G> Vec2<T> operator/(const G d)const{return Vec2(x/d, y/d);}
    template<class G> T operator*(const Vec2<G> v)const{return x*v.x + y*v.y;}

    template<class G> Vec2<T> operator=(const Vec2<G> v){x=v.x; y=v.y; return *this;}
    template<class G> Vec2<T> operator+=(const Vec2<G> v){x+=v.x; y+=v.y; return *this;}
    template<class G> Vec2<T> operator-=(const Vec2<G> v){x-=v.x; y-=v.y; return *this;}
    template<class G> Vec2<T> operator*=(const G d){x*=d; y*=d; return *this;}
    template<class G> Vec2<T> operator/=(const G d){x/=d; y/=d; return *this;}
};

template<class G> Vec3<G> operator-(Vec3<G> v){return v*(-1);}
template<class G> Vec2<G> operator-(Vec2<G> v){return v*(-1);}

template<class G, class N, class M> inline double mixedProduct(const Vec3<G> v, const Vec3<N> u, const Vec3<M> w){return v*(u^w);}
template<class G, class N> inline double distance(const Vec3<G> v, const Vec3<N> u=Vec3<N>()){return sqrt((v.x-u.x)*(v.x-u.x)+(v.y-u.y)*(v.y-u.y)+(v.z-u.z)*(v.z-u.z));}
template<class G, class N> inline double angle(const Vec3<G> v, const Vec3<N> u){return acos((v*u)/(v.distance()*u.distance()));}

template<class G, class N> static double distance(const Vec2<G> v, const Vec2<N> u=Vec2<N>()){return sqrt((v.x-u.x)*(v.x-u.x)+(v.y-u.y)*(v.y-u.y));}
template<class G, class N> static double angle(const Vec2<G> v, const Vec2<N> u){return acos((v*u)/(v.distance()*u.distance()));}


typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;
typedef Vec3<long long> Vec3l;
typedef Vec3<unsigned char> Vec3ub;

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;
typedef Vec2<long long> Vec2l;
typedef Vec2<unsigned char> Vec2ub;

#endif // VEC_H
