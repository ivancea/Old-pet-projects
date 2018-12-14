#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include <cmath>

using namespace std;

char getRand(string& a){
    int i = rand()%a.size();
    char c = a[i];
    a.erase(i,1);
    return c;
}

string XORCrypt(string a, string b){
    string t = a.size()>=b.size()?a:b;
    if(a.size()<b.size())
        b = a;
    while(b.size()<a.size())
        b+=b;
    for(int i=b.size()-a.size(), j=0; i<b.size() && j<a.size(); i++, j++){
        t[j] ^= b[i];
    }
    return t;
}

bool isAnagram(string a, string b, bool with_spaces=false){
    if(!with_spaces){
        for(int i=0; i<a.size(); i++)
            if(a[i]==' '){
                a.erase(i,1);
                --i;
            }
        for(int i=0; i<b.size(); i++)
            if(b[i]==' '){
                b.erase(i,1);
                --i;
            }
    }
    if(a.size()!=b.size()) return false;
    for(int i=0; i<a.size(); i++)
        for(int j=0; j<b.size(); j++)
            if(tolower(b[j])==tolower(a[i])){
                b.erase(j,1);
                break;
            }
    return b.size()==0;
}

string makeAnagram(string a){
    string vocales;
	for(char& c : a)
		c = tolower(c);
    for(int i=0; i<a.size(); i++)
        if(a[i]=='a'||a[i]=='e'||a[i]=='i'||a[i]=='o'||a[i]=='u'){
            vocales+=a[i];
            a.erase(i,1);
            --i;
        }
    string t;
    while(a.size()>0 || vocales.size()>0){
        if(a.size()>0){
            t+=getRand(a);
            if(t[t.size()-1]==' ') continue;
        }
        if(vocales.size()>0) t+=getRand(vocales);
    }
    return t;
}

int main(){
    srand(time(0));
    string a = "I am Vader";
    string b = "Merida va";
    while(true){
        cout << makeAnagram(a) << endl;
        cin.get();
    }
}
