#include <iostream>
#include <fstream>
#include <map>
#include <set>

using namespace std;

map<string, set<string> > urls;

string trim(string s, char c=' '){
    if(s.size()==0) return s;
    for(int i=0; i<s.size(); i++)
        if(s[i]!=c){
            if(i!=0) s.erase(0,i);
            break;
        }
    if(s.size()==0) return s;
    for(int i=s.size()-1; i>=0; i--)
        if(s[i]!=c){
            if(i!=s.size()-1) s.erase(i,s.size());
            break;
        }else if(i==0) return "";
    return s;
}

void parseUrl(string s){
    s = trim(s,'.');
    int dots=0;
    for(char c:s) if(c=='.') ++dots;
    if(dots==0) return;
    if(s.substr(0,4)=="www." && dots>1)
        s.erase(0,4);
    int i;
    for(i=s.size()-1, dots = 1+(s.find(".com.es")==s.size()-7); i>=0 && dots>=0; i--)
        if(s[i]=='.')
            --dots;
    if(i==-1){
        urls[s];
        return;
    }
    ++i;
    urls[s.substr(i+1,s.size())].insert(s.substr(0,i));
}

int main(){
    system("ipconfig /displaydns > rawdns");
    ifstream fi("rawdns");
    ofstream fo("dns.txt",ios::trunc);
    if(!fi||!fo){
        cout << "Error opening files." << endl;
        system("pause");
    }
    string t;
    while(!fi.eof()){
        getline(fi,t);
        t = trim(t);
        if(t.find("Nombre de registro")!=0)
            continue;
        for(int i=0; i<t.size(); i++)
            if(t[i]==':'){
                t.erase(0,i+2);
                break;
            }
        parseUrl(trim(t));
    }
    fo << "-DNS LIST-" << endl;
    for(auto it:urls){
        fo << endl << it.first << endl;
        for(auto str:it.second)
            fo << "    " << str << '.' << it.first << endl;
    }
    fi.close();
    fo.close();
    system("del rawdns");
}
