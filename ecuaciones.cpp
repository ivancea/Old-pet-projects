#include <iostream>
#include <cstring>

using namespace std;

int many(const string &s, char c){
    int t=0;
    for(int i=0; i<s.size(); i++)
        if(s[i]==c) ++t;
    return t;
}

bool isInt(string s){
    if(s[0]=='-') s.erase(0);
    for(int i=0; i<s.size(); i++)
        if(s[i]<48 || s[i]>57) return false;
    return true;
}

int solve(string ec){
    int p=0, t=0;
    if(isInt(ec)) return stoi(ec);
    if((p=many(ec, '('))!=many(ec, ')')) return 0;

    if(p){/** Paréntesis **/
        for(int i=0; i<ec.size(); i++)
            if(ec[i]=='('){
                --p;
                if(!p){
                    t=i+1;
                    break;
                }
            }
        for(int i=t; i<ec.size(); i++)
            if(ec[i]==')'){
                string m=ec.substr(t, i-t);
                ec.erase(t-1, m.size()+2);
                ec.insert(t-1, to_string(solve(m)));
                return solve(ec);
            }
    }

    for(int i=0; i<ec.size(); i++)
        if(ec[i]=='^') return 1;
}

int main(){
    cout << solve("(((489)))");
}
