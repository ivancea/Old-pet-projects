#include <iostream>

using namespace std;

class uint128_t{
    uint64_t _high, _low;
public:
    uint128_t():_high(0), _low(0){}
    uint128_t(uint64_t high, uint64_t low):_high(high), _low(low){}

    uint128_t(int8_t num):_high(0), _low(num){}
    uint128_t(int16_t num):_high(0), _low(num){}
    uint128_t(int32_t num):_high(0), _low(num){}
    uint128_t(int64_t num):_high(0), _low(num){}
    uint128_t(uint8_t num):_high(0), _low(num){}
    uint128_t(uint16_t num):_high(0), _low(num){}
    uint128_t(uint32_t num):_high(0), _low(num){}
    uint128_t(uint64_t num):_high(0), _low(num){}

    uint128_t(const uint128_t& u){_high = u.high(); _low = u.low();}

    uint64_t high()const{return _high;}
    uint64_t low()const{return _low;}
    void set(uint64_t high, uint64_t low){_high = high; _low = low;}

    /** OPERATORS SIMPLES **/
    uint128_t operator=(const uint128_t &u){_high = u.high(); _low = u.low();}
    uint128_t operator+=(const uint128_t &u){return *this = *this + u;}
    uint128_t operator-=(const uint128_t &u){return *this = *this - u;}
    //uint128_t operator*=(const uint128_t &u){return *this = *this * u;}
    //uint128_t operator/=(const uint128_t &u){return *this = *this / u;}
    uint128_t operator+(const uint128_t &u)const{
        uint128_t t = *this;;
        uint64_t temp=t.low();
        t.set(t.high(), t.low() + u.low());
        if(t.low() < temp) t.set(t.high() + 1, t.low());
        t.set(t.high() + u.high(), t.low());
        return t;
    }
    uint128_t operator-(const uint128_t &u)const{
        uint128_t t = *this;;
        uint64_t temp=t.low();
        t.set(t.high(), t.low() - u.low());
        if(t.low() > temp) t.set(t.high() - 1, t.low());
        t.set(t.high() - u.high(), t.low());
        return t;
    }

};

char getChar(int n){
    if(n<10) return '0'+n;
    if(n>=10 && n<16) return 'a'+n-10;
    return 0;
}

ostream& operator<<(ostream& o, uint128_t n){
    string t;
    uint64_t a = n.high(), b=n.low();
    for(int i=0; i<16; i++){
        t = getChar(b%0x10)+t;
        b /= 0x10;
    }
    while(a>0){
         t = getChar(a%0x10)+t;
         a /= 0x10;
    }
    return o << t;
}

int main(){
    uint128_t u(1236,11566);
    cout << u;
    return 0;
}
