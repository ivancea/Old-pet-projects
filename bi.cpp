#include <iostream>
#include <vector>

using namespace std;

class factorial{
    vector<uint32_t> mul, div;
    bool divided;

    public: static void f(vector<uint32_t> &v, uint32_t n){
        for(uint32_t i=2; i<(n/2)+1;)
            if(n%i==0){
                v.push_back(i);
                n/=i;
            }else ++i;
            v.push_back(n);
    }
public:
    factorial():divided(false){mul.clear(); div.clear();}

    int getMulCount()const{return mul.size();}
    uint32_t getMul(int n)const{if(n>=0 && n<mul.size()) return mul[n]; else return 0;}
    vector<uint32_t> getMul()const{return mul;}
    void addMul(uint32_t m){mul.push_back(m);divided=false;}
    void addMulFactorial(uint32_t f){if(f)for(int i=1; i<=f; i++) mul.push_back(i);divided=false;}

    int getDivCount()const{return div.size();}
    uint32_t getDiv(int n)const{if(n>=0 && n<div.size()) return div[n]; else return 0;}
    vector<uint32_t> getDiv()const{return div;}
    void addDiv(uint32_t d){div.push_back(d);divided=false;}
    void addDivFactorial(uint32_t f){if(f)for(int i=1; i<=f; i++) div.push_back(i);divided=false;}

    void fact(){factMul();factDiv();}
    void factMul(){
        vector<uint32_t> temp(mul);
        mul.clear();
        for(int i=0; i<temp.size(); i++)
            f(mul, temp[i]);
        for(int i=0; i<mul.size();)
            if(mul[i]==1)
                mul.erase(mul.begin()+i);
            else ++i;
    }
    void factDiv(){
        vector<uint32_t> temp(div);
        div.clear();
        for(int i=0; i<temp.size(); i++)
            f(div, temp[i]);
        for(int i=0; i<div.size();)
            if(div[i]==1)
                div.erase(div.begin()+i);
            else ++i;
    }
    void divide(){
        fact();
        for(int i=0; i<mul.size(); i++)
            for(int j=0; j<div.size(); j++)
                if(mul[i]==div[j]){
                    mul.erase(mul.begin()+i);
                    div.erase(div.begin()+j);
                    --i; --j;
                    break;
                }
        divided=true;
    }
    uint64_t get(){
        uint64_t t=1;
        if(!divided) divide();
        for(int i=0; i<mul.size(); i++)
            t*=mul[i];
        for(int i=0; i<div.size(); i++)
            t/=div[i];
        return t;
    }
    void clear(){mul.clear(); div.clear();divided=false;}
};

void FilaTrianguloPascal(uint64_t *&arr, uint32_t n){
    factorial f;
    arr = new uint64_t[n+1];
    for(int i=0; i<=n; i++){
        f.addMulFactorial(n);
        f.addDivFactorial(i);
        f.addDivFactorial(n-i);
        f.divide();
        arr[i] = f.get();
        f.clear();
    }
}

int main(){
    uint64_t *v;
    FilaTrianguloPascal(v, 67);
    for(int j=0; j<=67; j++)
        cout << v[j] << " ";
}
