#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template<class T>int contains(const vector<T> &v, T c){
    int a=0;
    for(int i=0; i<v.size(); i++)
        if(v[i]==c) ++a;
    return a;
}

void sort(vector<string> &v){
    string t;
    for(int n=0; n<v.size(); n++)
    for(int i=0; i<v.size(); i++)
        for(int j=i+1; j<v.size(); j++)
        if(v[i][0]>v[j][0]){
            t=v[j];
            v[j]=v[i];
            v[i]=t;
            break;
        }

}

int main(){
    string t;
    vector<string> v;
    vector<char> c;
    ifstream  f("keylog.txt");
    while(!f.eof()){
        getline(f, t, (char)0x0A);
        if(t.size()==3)
            if(t[0]>47 && t[0]<59 && t[1]>47 && t[1]<59 && t[2]>47 && t[2]<59){
                v.push_back(string()+t[0]+t[1]);
                v.push_back(string()+t[0]+t[2]);
                v.push_back(string()+t[1]+t[2]);
            }
    }
    f.close();
    cout << "Total: " << v.size() << endl; /** Recoger los pares **/
    for(int i=0; i<v.size(); i++)
        for(int j=i+1; j<v.size(); j++)
            if(v[i]==v[j]){
                v.erase(v.begin()+j);
                --j;
            }
    cout << "Total limpio: " << v.size() << endl; /** Limpiar repetidos **/
    for(int i=0; i<v.size(); i++){
        if(v[i][0]==v[i][1]){
            if(!(contains(c, v[i][0])>=2))
                c.push_back(v[i][0]);
        }else{
            if(!contains(c, v[i][0])) c.push_back(v[i][0]);
            if(!contains(c, v[i][1])) c.push_back(v[i][1]);
        }
    }
    sort(v);
    for(int i=0; i<v.size(); i++)
        cout << v[i] << endl;
    return 0;
}
