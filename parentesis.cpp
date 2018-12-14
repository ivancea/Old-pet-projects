#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


bool isAllWellClosed(istream& s){
	if(!s)
		return false;
	static const string chars = "()[]{}";
    string t, temp;
    string chain;
    while(!s.eof()){
        getline(s,t);
        temp+=t;
    }
    for(int i=0; i<t.size(); i++){
        if(chain[chain.size()-1]=='"'){
            if(t[i]=='\"'){
                int cont=0;
                for(int j=i-1; j>=0 && t[j]=='\\'; j--)
                    ++cont;
                if(cont%2==0){
                    chain.erase(chain.size()-1,1);
                }
            }
            continue;
        }
        if(chain[chain.size()-1]=='\''){
            if(t[i]=='\''){
                int cont=0;
                for(int j=i-1; j>=0 && t[j]=='\\'; j--)
                    ++cont;
                if(cont%2==0){
                    chain.erase(chain.size()-1,1);
                }
            }
            continue;
        }
        if(t[i]=='"'){
            chain+='"';
            continue;
        }
        if(t[i]=='\''){
            chain+='\'';
            continue;
        }
        size_t p;
        if((p=chars.find(t[i]))==string::npos)
            continue;
        if(p%2==0){
            chain+=chars[p];
        }else{
            if(chain[chain.size()-1]==chars[p-1]){
                chain.erase(chain.size()-1,1);
            }else return false;
        }
    }
    cout << chain;
    return chain.size()==0;
}

int main(){
    ifstream f("parentesis.cpp");
    istringstream ss("{}{{{'}'}}}{{[]}}");
    cout << isAllWellClosed(ss);
}
