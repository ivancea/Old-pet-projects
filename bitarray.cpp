#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

class bitArray{
    char *_bits;
    size_t bits_size;

    void deleteSecure(){
        if(bits_size>1)
            delete[] _bits;
        else if(bits_size==1)
            delete _bits;
    }

    char testByte(int n){
        switch(n){
            case 0:return 0b1;
            case 1:return 0b10;
            case 2:return 0b100;
            case 3:return 0b1000;
            case 4:return 0b10000;
            case 5:return 0b100000;
            case 6:return 0b1000000;
            case 7:return 0b10000000;
        }
        return 0;
    }

    void newSize(size_t s){
        if(s<=bits_size*8) return;
        char *t = new char[(s/8)+1];
        for(int i=0; i<(s/8)+1; i++)
            t[i]=0;
        for(int i=0; i<bits_size; i++)
            t[i]=_bits[i];
        deleteSecure();
        _bits = t;
        bits_size = (s/8)+1;
    }

    void setBytes(const void* bytes, size_t n){
        deleteSecure();
        bits_size = n;
        if(n==0)
            return;
        _bits = new char[n];
        for(int i=0; i<n; i++)
            _bits[i] = ((const char*)bytes)[i];
    }

public:
    bitArray():bits_size(0),_bits(0){}
    bitArray(const char* c){setBytes(c,strlen(c));}
    bitArray(string s){setBytes(s.c_str(),s.size());}
    template<class T> bitArray(vector<T> s){setBytes(s.begin(),s.end()-s.begin());}
    template<class T> bitArray(T c){setBytes(&c,sizeof(T));}
    template<class T> bitArray(T* c, size_t n){setBytes(c,n*sizeof(T));}
    bool operator[](size_t index){
        if(index>=bits_size*8){
            newSize(index+1);
            return 0;
        }
        return _bits[index/8]&testByte(index%8);
    }

    void set(size_t index, bool value){
        if(index>=bits_size*8)
            newSize(index+1);
        if(value) _bits[index/8]|=testByte(index%8);
        else _bits[index/8]&=0b11111111^testByte(index%8);
    }

    bool get(size_t index){
        if(index>=bits_size*8){
            newSize(index+1);
            return 0;
        }
        return _bits[index/8]&testByte(index%8);
    }

    size_t size()const{return bits_size;}

    const char* getArray()const{return _bits;}


};

int main(){
}
