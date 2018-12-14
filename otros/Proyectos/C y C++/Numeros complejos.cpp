#include "C:\includes\ivancea.h"
#include <sstream>

class complejo{
public:
    double r, i;
    static int out;
    static void binomial(){out = 0;}
    static void polarRad(){out = 1;}
    static void polarDeg(){out = 2;}
    complejo(double real=0.0, double imaginario=0.0):r(real), i(imaginario){}
    complejo(const complejo &c):r(c.r), i(c.i){}

    complejo operator=(const complejo &c){r=c.r; i=c.i; return *this;}
    complejo operator=(const double &d){r=d; return *this;}

    complejo operator+(const complejo &c)const{return complejo(c.r+r, c.i+i);}
    complejo operator+(const double &d)const{return complejo(d+r, i);}

    complejo operator-(const complejo &c)const{return complejo(r-c.r, i-c.i);}
    complejo operator-(const double &d)const{return complejo(r-d, i);}

    complejo operator*(const complejo &c)const{
        double num(0), arg(0);
        num = sqrt(r*r + i*i)*sqrt(c.r*c.r + c.i*c.i);
        if(r!=0) arg += atan(i/r);
        else arg += PI;
        if(c.r!=0) arg += atan(c.i/c.r);
        else arg += PI;
        return complejo(cos(arg)*num, sin(arg)*num);
    }
    complejo operator*(const double &d)const{
        return complejo(d, 0) * *this;
    }

    complejo operator/(const complejo &c)const{
        double num(0), arg(0);
        num = sqrt(r*r + i*i)/sqrt(c.r*c.r + c.i*c.i);
        if(r!=0) arg += atan(i/r);
        else arg += PI;
        if(r<0) arg += PI;
        if(c.r!=0) arg -= atan(c.i/c.r);
        else arg -= PI;
        if(c.r<0) arg += PI;
        return complejo(cos(arg)*num, sin(arg)*num);
    }
    complejo operator/(const double &d)const{
        return complejo(d, 0) / *this;
    }

    string toBinomial(){
        stringstream temp;
        if(r==0){
            temp << i << "j";
            return temp.str();
        }else
        if(i==0){
            temp << r;
            return temp.str();
        }
        temp << r << " ";
        if(i<0){
            temp << "- " << -1*i << "j";
            return temp.str();
        }
        temp << "+ " << i << "j";
        return temp.str();
    }
    string toPolarDeg(){
        stringstream temp;
        double num(0), arg(0);
        num = sqrt(i*i + r*r);
        if(r!=0) arg = atan(i/r);
        else arg = PI;
        if(r<0) arg += PI;
        temp << num << " //" << (arg*360)/(PI*2) << "deg";
        return temp.str();
    }
    string toPolarRad(){
        stringstream temp;
        double num(0), arg(0);
        num = sqrt(i*i + r*r);
        if(r!=0) arg = atan(i/r);
        else arg = PI;
        if(r<0) arg += PI;
        temp << num << " //" << arg << "rad";
        return temp.str();
    }

    operator const char*(){
        switch(out){
        case 0: return toBinomial().c_str();
        case 1: return toPolarRad().c_str();
        case 2: return toPolarDeg().c_str();
        default: return "";
        }
    }
};
int complejo::out = 0;

int main(){
    complejo::polarDeg();
    complejo a(6, 6);
    a = (a * a)/(a+complejo(84, 51.201)*5);
    cout << a << endl;
    return 0;
}
