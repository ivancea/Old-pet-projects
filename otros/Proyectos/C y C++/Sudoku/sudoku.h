#ifndef SUDOKU_H
#define SUDOKU_H

#include<iostream>
#include <vector>

using namespace std;

class casilla{
    vector<bool> _p;

public:
    casilla():_p(9,true){}
    casilla(const casilla& c):_p(getVector()){}
    bool get(int n)const{
        if(n>=0 && n<_p.size())
            return _p[n];
        return false;
    }
    vector<int> getIndexes()const{
        vector<int> v;
        for(int i=0; i<_p.size(); i++)
            if(_p[i]) v.push_back(i);
        return v;
    }
    bool set(int n){
        for(int i=0; i<_p.size(); i++)
            if(i!=n) _p[i]=false;
            else _p[i]=true;
        cout << getCount() << endl;
        return getCount();
    }
    bool erase(int n){
        if(n>=0 && n<_p.size())
            _p[n]=false;
        return getCount();
    }
    int getCount()const{
        int c=0;
        for(int i=0; i<_p.size(); i++)
            if(_p[i]) ++c;
        return c;
    }
    void fill(){
        for(auto it:_p)
            it=true;
    }
    const vector<bool>& getVector()const{
        return _p;
    }
};

class sudoku
{
    vector<casilla> _casillas;

public:
    sudoku():_casillas(81){}
    sudoku(const string& s):_casillas(81){
        leer(s);
    }
    sudoku(const sudoku& s){
        _casillas = s.getCasillas();
    }
    const vector<casilla>& getCasillas()const{
        return _casillas;
    }
    bool set(int nCasilla, int num){
        int x=nCasilla%9, y=nCasilla/9;
        /*if(!_casillas[nCasilla].set(num)) return false;
        for(int i=0; i<9; i++){
            if(i!=x && !_casillas[i+y*9].erase(num))
                return false;
            if(i!=y && !_casillas[x+i*9].erase(num))
                return false;
            int pX = (x/3)*3+i%3, pY = (y/3)*3 + i/3;
            int fi = pX + pY*9;
            if((pX!=x || pY!=y) && !_casillas[fi].erase(num))
                return false;
        }*/
        return true;
    }
    bool leer(const string& s){
        if(s.size()!=81) return false;
        for(int i=0; i<81; i++)
            if(s[i]>='1' && s[i]<='9'){
                if(!set(i,s[i]-'1')){
                    return false;
                }
            }else _casillas[i].fill();
        return true;
    }
    bool resuelto(){
        for(auto it:_casillas)
            if(it.getCount()!=1)
            return false;
        return true;
    }
    int findSmaller(){
        int c=9, index=-1;
        for(int i=0; i<_casillas.size(); i++)
            if(index==-1 || (_casillas[i].getCount()<c && _casillas[i].getCount()>1)){
                c = _casillas[i].getCount();
                index = i;
            }
        return index;
    }
};

sudoku* resolver(sudoku* s){
    if(s==0 || s->resuelto()) return s;
    int c = s->findSmaller();
    if(c==-1) return s;
    auto v = s->getCasillas()[c].getIndexes();
    for(auto it:v){
        sudoku* s1 = new sudoku(*s);
        if(s1->set(c,it)){
            sudoku* s2 = resolver(s1);
            if(s2!=0){
                if(s2!=s1) delete s1;
                return s2;
            }
        }
        delete s1;
    }
    return 0;
}

char parseCasilla(const casilla& c){
    if(c.getCount()>1) return '.';
    else if(c.getCount()<1) return 'x';
    else return c.getIndexes()[0]+'1';
}

ostream& operator<<(ostream& o,const sudoku& s){
    for(int i=0; i<81; i++){
        cout << parseCasilla(s.getCasillas()[i]) << ' ';
        if((i+1)%9==0) cout << endl;
    }
}

#endif // SUDOKU_H
