#include <iostream>
#include "c:/includes/http.h"

using namespace std;

string encodeUrl(const string& s){
    string t;
    for(int i=0; i<s.size(); i++){
        string c = toHex((unsigned char)s[i]);
        while(c.size()<2) c = "0"+c;
        t+='%'+c;
    }
    return t;
}

int main(){
    http::GETRequest g("ivancea.hol.es/privado/txttoimage.php");
    cout << "Texto a convertir en imagen: (las '|' se considerarán saltos de linea)" << endl;
    string t;
    getline(cin,t);
    replaceAll(t,"|","\n");
    t = encodeUrl(t);
    cout << endl << "Tamaño de letra: (12 por defecto)";
    string s="12";
    getline(cin,s);
    g.setUrl(g.getUrl()+"?text="+t+"&size="+s);
    http::sendRequestAndBodyToFile(g,"imagen.png");
}
