#include "C:\includes\ivancea.h"

class replacedOut{
    vector<string> _start, _fin;
    string _str;

public:
    replacedOut(){}
    replacedOut(const replacedOut &ro){
        for(int i=0; i<ro.count(); i++)
            insert(ro[i][0], ro[i][1]);
    }
    void set(string s){_str = s;}
    void insert(string start, string fin){
        if(start == "") return;
        _start.push_back(start);
        _fin.push_back(fin);
    }
    void insertInsult(string insult){
        string temp;
        _start.push_back(insult);
        for(int i=0; i<insult.size(); i++)
            temp+="*";
        _fin.push_back(temp);
    }
    void insertInsult(vector<string> insults){
        for(int i=0; i<insults.size(); i++)
            if(insults[i]!=""){
                string temp;
                _start.push_back(insults[i]);
                for(int i=0; i<insults[i].size(); i++)
                    temp+="*";
                _fin.push_back(temp);
            }
    }
    bool erase(int n){
        if(_start.size()<=n || n<0) return false;
        _start.erase(_start.begin()+n);
        _fin.erase(_fin.begin()+n);
        return true;
    }
    int count()const{return _start.size();}
    vector<string> operator[](int n)const{
        vector<string> temp;
        if(_start.size()<=n || n<0){
            temp.push_back("");
            temp.push_back("");
        }else{
            temp.push_back(_start[n]);
            temp.push_back(_fin[n]);
        }
        return temp;
    }
    string get(string s) const{
        if(s=="") return "";
        for(int i=0; i<_start.size(); i++)
            if(_start[i]!="")
                replaceAll(s, _start[i], _fin[i]);
        return s;
    }
    string get()const{
        return get(_str);
    }
    void replace(string &s)const{
        s = get(s);
    }
    void operator()()const{
        cout << get();
    }
    void operator()(string s)const{
        cout << get(s);
    }
    void operator<<(string s){
        cout << get(s);
    }
    void operator>>(string &s){
        replace(s);
    }
}rout;

/// REPLACE reemplaza una string por referencia
/// GET retorna una string reemplazada
/// operator() y operator<< hacen COUT de una string tras reemplazarla

int main(){
    rout.insert("_", " ");
    rout.insert("jijiji", "jajaja");
    rout.insertInsult("puta");
    //rout.set("Serás hijo_de_puta! jajaja\n");
    //rout();
    rout << "Hola puta jijiji__jijiji__puta tu__putatu";
    return 0;
}
