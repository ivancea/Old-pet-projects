#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>

using namespace std;


class Stats{
    map<string,double> _m;

public:
    Stats(){}
    void add(string s, double n){
        _m[s] = n;
    }
    void erase(string s){
        auto it = _m.find(s);
        if(it!=_m.end())
            _m.erase(it);
    }
    void show(){
        double total=0;
        for(auto it:_m)
            total += it.second;
        for(auto it:_m){
            vector<string> v(it.first.size()/24+1,"");
            for(int i=0; i<it.first.size()/24+1; i++)
                v[i] = it.first.substr(i*24,24);
            uint32_t len = round(100*it.second/total);
            cout << string(24-v[0].size(), ' ') << v[0]
                  << ' ' << string(3-to_string(len).size(), ' ') << to_string(len) << '%'
                  << ' ' << string(len/2, (char)219);
            if(len<100)
                cout << endl;
            for(int i=1; i<v.size(); i++)
                cout << v[i] << endl;
        }
    }
};


int main(){
    srand(time(0));
    map<uint32_t, uint32_t> m;
    for(int i=0; i<100; i++)
        ++m[rand()%10];
    Stats st;
    for(auto it:m)
        st.add(to_string(it.first), it.second);
    st.show();
}
