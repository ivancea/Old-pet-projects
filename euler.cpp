#include "http.h"

using namespace http;

const string script="<head><script type=\"text/x-mathjax-config\">"
"   MathJax.Hub.Config({"
"      jax: [\"input/TeX\", \"output/HTML-CSS\"],"
"      tex2jax: {"
"         inlineMath: [ [\"$\",\"$\"], [\"\\\\(\",\"\\\\)\"] ],"
"        displayMath: [ [\"$$\",\"$$\"], [\"\\\\[\",\"\\\\]\"] ],"
"         processEscapes: true"
"      },"
"      \"HTML-CSS\": { availableFonts: [\"TeX\"] }"
"   });"
"</script>"
"<script type=\"text/javascript\" src=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS_HTML\">"
"</script>"
"</head>"
"<body>";

bool parseBody(const string& s, uint32_t contador){///<h2>  -  </div>
    if(s.size()==0) return false;
    uint32_t a=0, b=0;
    string title;
    for(;a<s.size(); a++)
        if(s.substr(a,4)=="<h2>")
            break;
    for(b=a;b<s.size(); b++)
        if(s.substr(b,5)=="</h2>")
            break;
    if(a==s.size()||b==s.size()) return false;
    title = s.substr(a+4,b-a-4);
    for(int i=0; i<title.size(); i++)
        if(title[i]=='/' || title[i]=='\\' || title[i]=='*' || title[i]==':' || title[i]=='<'
           || title[i]=='>' || title[i]=='?' || title[i]=='"' || title[i]=='|')
            title[i]='.';
    uint32_t count=0;
    for(;b<s.size();b++)
        if(s.substr(b,6)=="</div>"){
            b+=6;
            break;
        }
    for(;b<s.size();b++)
        if(s.substr(b,6)=="</div>" && --count==0){
            b+=6;
            break;
        }else if(s.substr(b,4)=="<div")
            ++count;
    if(b==a || title.size()==0) return false;
    ofstream file("Problemas Euler/"+to_string(contador)+" - "+title+".html",ios::trunc);
    string t = s.substr(a,b-a);
    for(int i=0; i<t.size(); i++)
        if(t.substr(i,6)==" src=\"" || t.substr(i,6)==" src='")
            t.insert(i+6,"http://projecteuler.net/");
    file << script << t << "<body>";
    return true;
}

const string euler = "http://projecteuler.net/problem=";

int main(){
    GETRequest gr;
    uint32_t contador=1;
    while(true){
        gr.setUrl(euler+to_string(contador));
        response re;
        while(re.getCode()!=200){
            re = sendRequest(gr);
            if(parseBody(re.getBody(), contador))
                break;
            else
                re.setCode(0);
        }
        cout << contador << " -> " << re.getCode() << endl;
        if(re.getCode()==302){
            cout << "Finished with " << contador-1 << " problems." << endl;
            break;
        }else ++contador;
    }
    system("pause");
}
