#include "C:\includes\ivancea.h"

using namespace std;

void clear(string &s){
    for(int i=0; i<s.size(); i++)
        if(s[i]==' '){
            s.erase(i,1);
            --i;
        }else break;

    for(int i=s.size()-1; i>=0; i--)
        if(s[i]==' '){
            s.erase(i,1);
        }else break;
}

string getNewFileName(string name){
    for(int i=name.size()-1; i>=0; i--)
        if(name[i]=='/' || name[i]=='\\'){
            name.insert(i+1, "Precomp_");
            break;
        }
    return name;
}

int main(int args, char** argc){
    if(args!=2){
        cout << "Debe pasar un archivo al programa.";
        cin.get();
        return 1;
    }
    ifstream f(argc[1]);
    if(f.fail()){
        cout << "El archivo no ha sido abierto correctamente.";
        cin.get();
        return 1;
    }
    ofstream out(getNewFileName(argc[1]), ios::trunc);
    cout << getNewFileName(argc[1]) << endl;
    if(out.fail()){
        cout << "El archivo de salida no ha sido abierto correctamente.";
        cin.get();
        return 1;
    }
    vector< pair<string,string> > def;
    string t, save;
    while(!f.eof()){
        getline(f,t);
        save = t;
        clear(t);
        if(t.substr(0,8)=="#define "){
            t.erase(0,8);
            for(int i=0; i<t.size(); i++)
                if(t[i]==' ' || (i+1<t.size() && t[i]=='/' && (t[i+1]=='/' || t[i+1]=='*'))){
                    def.push_back(pair<string,string>(t.substr(0,i), t.substr(i+1, t.size())));
                    break;
                }
        }else{
            t = save;
            for(int i=0;i<t.size(); i++)
                for(int j=0; j<def.size(); j++)
                    if(contains(t, (i?" ":"")+def[j].first+" ", i) || (i==0 && contains(t," "+def[j].first+" ", i))||
                       ((contains(t, (i?" ":"")+def[j].first, i) || (i==0 && contains(t," "+def[j].first, i))) && i+def[j].first.size()+(i?1:0)==t.size())){
                        t.erase(i,def[j].first.size()+(i?2:1));
                        t.insert(i,(i?" ":"")+def[j].second+" ");
                        i+=def[j].second.size()-1;
                        break;
                    }
            out << t << '\n';
        }
    }
    f.close();
    out.close();
}
