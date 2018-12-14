#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

#define CRYPT(a) (char)(255-((255-((255-a)^32))^31))

struct pixel{
    unsigned char r,g,b;
    pixel():r(0),g(0),b(0){}
    pixel(unsigned char _r, unsigned char _g, unsigned char _b):r(_r),g(_g),b(_b){}
    pixel(const pixel& p){
        r = p.r;
        g = p.g;
        b = p.b;
    }
    void set(unsigned char _r, unsigned char _g, unsigned char _b){
        r = _r;
        g = _g;
        b = _b;
    }
    void set(pixel p){
        r = p.r;
        g = p.g;
        b = p.b;
    }
    bool is(unsigned char _r, unsigned char _g, unsigned char _b){
        return r==_r && g==_g && b==_b;
    }
    bool operator==(const pixel& p){
        return r==p.r && g==p.g && b==p.b;
    }
    bool operator!=(const pixel& p){
        return !(*this==p);
    }
};

ostream& operator<<(ostream& o,const pixel& p){
    o << (uint16_t)p.r << ' ' << (uint16_t)p.g << ' ' << (uint16_t)p.b;
    return o;
}

class pbm{
    int _tipo;
    pixel **_imagen;
    int _x, _y;

    void fill(pixel**& arr, int x, int y){
        if(x==0 || y==0){
            arr=0;
            return;
        }
        arr = new pixel*[x];
        for(int i=0; i<x; i++)
            arr[i] = new pixel[y];
    }
    void destroy(pixel** arr, int x, int y){
        if(arr==0) return;
        if(y>0){
            for(int i=0; i<x; i++)
                if(y==1) delete arr[i];
                else delete[] arr[i];
        }
        if(x==1) delete arr;
        else if(x>1) delete[] arr;
    }

public:
    pbm():_tipo(0),_imagen(0),_x(0),_y(0){}
    pbm(int x, int y, pixel p = pixel(0,0,0)):_tipo(0),_imagen(0),_x(0),_y(0){
        crear(x,y,p);
    }
    ~pbm(){
        destroy(_imagen, _x, _y);
    }
    int x()const{return _x;}
    int y()const{return _y;}
    int tipo(){return _tipo;}
    bool valido(){
        return _tipo>=1 && _tipo<=6 && _x>0 && _y>0;
    }
    pixel getPixel(int x, int y){
        if(x<0 || x>=_x || y<0 || y>=_y) return pixel();
        return _imagen[x][y];
    }
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b){
        if(x<0 || x>=_x || y<0 || y>=_y) return;
        _imagen[x][y].set(r,g,b);
    }
    void crear(int x, int y, pixel p = pixel(0,0,0)){
        _tipo = 6;
        if(x<1||y<1) return;
        if(_x>0 && _y>0)
            destroy(_imagen, _x, _y);
        fill(_imagen, x, y);
        for(int i=0; i<x; i++)
            for(int j=0; j<y; j++)
                _imagen[i][j] = p;
        _x=x;
        _y=y;
    }
    bool leer(string archivo){
        int x=0, y=0;
        int tipo;
        ifstream f(archivo,ios::binary);
        if(!f) return false;
        string t;
        getline(f,t,(char)0x0A);
        if(t.size()!=2 || (t[0]!='P' && t[0]!='p') || t[1]<'1' || t[1]>'6')
            return false;
        tipo = t[1]-'0';
        getline(f,t,(char)0x0A);
        x = atoi(t.c_str());
        getline(f,t,(char)0x0A);
        y = atoi(t.c_str());
        getline(f,t,(char)0x0A);//profundidad
        if(y==0 || x==0)
            return false;
        fill(_imagen,x,y);
        for(int j=0;j<y;j++)
            for(int i=0;i<x;i++){
                f.read((char*)&_imagen[i][j].r, 1);
                f.read((char*)&_imagen[i][j].g, 1);
                f.read((char*)&_imagen[i][j].b, 1);
            }
        _x=x;
        _y=y;
        _tipo = tipo;
        return true;
    }
    bool exportar(string archivo){
        if(!valido()) return false;
        ofstream f(archivo,ios::trunc|ios::binary);
        if(!f) return false;
        char salto = 0x0A;
        f << "P" << _tipo;
        f.write(&salto,1);
        f << _x;
        f.write(&salto,1);
        f << _y;
        f.write(&salto,1);
        f << "255";
        f.write(&salto,1);
        for(int j=0;j<_y;j++)
            for(int i=0;i<_x;i++){
                f.write((char*)&_imagen[i][j].r,1);
                f.write((char*)&_imagen[i][j].g,1);
                f.write((char*)&_imagen[i][j].b,1);
            }
        return true;
    }
};

bool encriptar(string from, string to){
    pbm img;
    ifstream file(from,ios::binary|ios::ate);
    if(!file) return false;
    uint32_t size = file.tellg();
    file.seekg(ios::beg);
    uint32_t over = 3-(size%3);
    if(over==3) over=0;
    double sq = sqrt((size+over)/3);
    if(sq==int(sq)){
        img.crear(sq,sq,pixel(0,0,0));
    }else{
        if((int(sq)+1)*int(sq)*3>size+over)
            img.crear(int(sq)+1,int(sq));
        else
            img.crear(int(sq)+1,int(sq)+1);
        over += img.x()*img.y()*3-(size+over);
    }
    for(uint32_t i=0; i<img.x(); i++)
        for(uint32_t j=0; j<img.y(); j++){
            if(size>=3){
                char t[3];
                file.read(t,3);
                img.setPixel(i,j,CRYPT(t[2]),CRYPT(t[1]),CRYPT(t[0]));
                size-=3;
            }else if(size==2){
                char t[2];
                file.read(t,2);
                img.setPixel(i,j,CRYPT(0),CRYPT(t[1]),CRYPT(t[0]));
                size-=2;
            }else if(size==1){
                char t[1];
                file.read(t,1);
                img.setPixel(i,j,CRYPT(0),CRYPT(0),CRYPT(t[0]));
                size-=1;
            }else{
                img.setPixel(i,j,CRYPT(0),CRYPT(0),CRYPT(0));
            }
        }
    return img.exportar(to);
}

bool desencriptar(string from, string to){
    pbm img;
    ofstream file(to,ios::binary|ios::trunc);
    if(!img.leer(from) || !file) return false;
    for(uint32_t i=0; i<img.x(); i++)
        for(uint32_t j=0; j<img.y(); j++){
            pixel p(img.getPixel(i,j));
            char t[3]={CRYPT(p.b),CRYPT(p.g),CRYPT(p.r)};
            file.write(t,3);
        }
    return true;
}

string changeExtension(string t) throw(){
    for(uint32_t i=t.size()-1; i>=0; i++)
        if(t[i]=='.'){
            if(t.size()-i<3) throw exception();
            if(t.substr(i+1,3)=="pbm"){
                t = '1' + t.substr(0,i+1) + "txt" + t.substr(i+4,t.size());
            }else{
                t = '0' + t.substr(0,i+1) + "pbm" + t.substr(i+4,t.size());
            }
        }
    return t;
}

int main(int argc, char** args){
    for(uint32_t i=0; i<argc; i++){
        try{
            string name = args[i];
            string newName = changeExtension(name);
            if(newName[0]=='1')
                desencriptar(name,newName);
            else
                encriptar(name,newName);
        }catch(exception e){
            cout << "Error with " << args[i] << endl;
        }
    }
    system("pause");
}
