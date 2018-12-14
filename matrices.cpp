#include <iostream>

using namespace std;

template<class T>
class matrix{
    T** _mat;
    const size_t _x, _y;

    void fill(){
        _mat = new T*[_x];
        for(size_t i=0; i<_x; i++)
            _mat[i] = new T[_y];
    }

public:
    matrix(size_t x, size_t y, bool identity=0):_mat(0),_x(x),_y(y){
        if(_x==0 || _y==0) return;
        fill();
        for(size_t i=0; i<x; i++)
            for(size_t j=0; j<y; j++)
                if(identity && x==y && i==j)
                    _mat[i][j]=1;
                else
                    _mat[i][j]=0;
    }
    matrix(const matrix<T>& m):_x(m.x()),_y(m.y()){
        if(_x==0 || _y==0) return;
        fill();
        for(size_t i=0; i<_x; i++)
            for(size_t j=0; j<_y; j++)
                _mat[i][j] = m.get(i,j);
    }
    ~matrix(){
        if(_x==0 || _y==0) return;
        if(_y==1){
            for(size_t i=0; i<_x; i++)
                delete _mat[i];
        }else
            for(size_t i=0; i<_x; i++)
                delete[] _mat[i];
        if(_x==1)
            delete _mat;
        else
            delete[] _mat;
    }
    T& operator()(size_t x, size_t y){
        return _mat[x][y];
    }
    T get(size_t x, size_t y)const{
        return _mat[x][y];
    }
    size_t x()const{
        return _x;
    }
    size_t y()const{
        return _y;
    }
    matrix<T> submatrix(size_t iX, size_t iY, size_t width, size_t height){
        if(iX+width>_x || iY+height>_y) return matrix<T>(0,0);
        matrix<T> t(width, height);
        for(size_t i=0; i<width; i++)
            for(size_t j=0; j<height; j++)
                t(i,j)=_mat[iX+i][iY+j];
        return t;
    }
    matrix<T> transposed(){
        if(_x==0||_y==0) return matrix<T>(0,0);
        matrix<T> t(_y,_x);
        for(size_t i=0; i<_x; i++)
            for(size_t j=0; j<_y; j++)
                t(j,i)=_mat[i][j];
        return t;
    }
    template<class P>
    matrix<T> operator+(const matrix<P>& m)const{
        if(m.x()!=_x || m.y()!=_y || m.null())
            return matrix<T>(0,0);
        matrix<T> t(_x,_y);
        for(int i=0; i<_x; i++)
            for(int j=0; j<_y; j++)
                t(i,j)=_mat[i][j]+m(i,j);
        return t;
    }
    template<class P>
    matrix<T> operator-(const matrix<P>& m)const{
        if(m.x()!=_x || m.y()!=_y || m.null())
            return matrix<T>(0,0);
        matrix<T> t(_x,_y);
        for(int i=0; i<_x; i++)
            for(int j=0; j<_y; j++)
                t(i,j)=_mat[i][j]-m(i,j);
        return t;
    }
    template<class P>
    matrix<T> operator*(P m)const{
        if(_x==0 || _y==0) return *this;
        matrix<T> t(_x,_y);
        for(int i=0; i<_x; i++)
            for(int j=0; j<_y; j++)
                t(i,j)*=m;
        return t;
    }
    template<class P>
    matrix<T> operator/(P m)const{
        if(_x==0 || _y==0) return *this;
        if(m==0) return matrix<T>(0,0);
        matrix<T> t(_x,_y);
        for(int i=0; i<_x; i++)
            for(int j=0; j<_y; j++)
                t(i,j)/=m;
        return t;
    }
    bool null()const{
        return (_x==0||_y==0);
    }
    operator bool()const{
        return (_x!=0&&_y!=0);
    }
    template<class P>
    matrix<T> operator*(const matrix<P>& m){
        if(_x!=m.y()) return matrix<T>(0,0);
        matrix<T> t()
        for(int i=0; i<_x; i++)
            for(int j=0; j<_y; j++)
    }
};

template<class T>
ostream& operator<<(ostream& o, const matrix<T>& m){
    if(!m){
        o<<"null";
        return o;
    }
    for(size_t j=0; j<m.y(); j++){
        for(size_t i=0; i<m.x(); i++){
            o << m.get(i,j);
            if(i!=m.x()-1) o << ' ';
        }
        if(j!=m.y()) o << endl;
    }
    return o;
}

int main(){
    matrix<int> a(2,4,true);
    a(1,3) = 9;
    return 0;
}
