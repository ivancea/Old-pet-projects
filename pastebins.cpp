#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <algorithm>
#include <regex>

#include "http.h"

using namespace std;
using namespace http;

const string PASTEBIN = "http://pastebin.com/";

list<string> words = {"leak","hacked","steam","CVV","gift","paypal","credit","bank",
                      "/regex/([0-9]+(-|.| ))+[0-9]+"};

map<string,bool> sites;
ofstream file("log.txt", ios::app);


bool containsIgnoreCase(string& container, string& str){
    auto it = search(container.begin(), container.end(),
                      str.begin(), str.end(),
                     [](char c1, char c2){return toupper(c1)==toupper(c2);});
    return it!=container.end();
}

void loadSites(){
    string body;
    {
        GETRequest get(PASTEBIN+"archive");
        response re = sendRequest(get);

        body = re.getBody();
    }

    size_t pos = body.find("<tr class=\"top\">");
    for(int i=0; i<150; i++){
        pos = body.find("</tr>",pos);
        pos = body.find("<a href=\"/", pos)+10;
        if(pos==string::npos)
            break;
        size_t t = body.find("\"", pos);
        if(t==string::npos || t<=pos)
            break;
        sites[body.substr(pos, t-pos)];
    }
}

void readPastes(list<string>& new_sites){
    GETRequest get;
    response re;
    int count = 1;
    for(auto it:new_sites){
        cout << count++ << '/' << new_sites.size() << endl;
        get.setUrl(PASTEBIN+it);
        string body = sendRequest(get).getBody();
        size_t pos = body.find("(this,event)\">")+14;
        size_t pos2 = body.find("</textarea>", pos);
        if(pos==string::npos || pos2==string::npos || pos2<=pos)
            continue;
        body = body.substr(pos, pos2-pos);
        for(string& s:words){
            if(s.substr(0,7)=="/regex/"){
                if(regex_search(body, regex(s.substr(7)))){
                    cout << "Found paste that matches regex \"" + s.substr(7) + "\"" << endl;
                    file << s.substr(7) << " -> " << it << endl;
                    break;
                }
            }else if(containsIgnoreCase(body,s)){
                cout << "Found paste with word \"" + s + "\"" << endl;
                file << s << " -> " << it << endl;
                break;
            }
        }
    }
}

int main(int args, char** argc){
    if(args==2){
        words.clear();
        ifstream f(argc[1]);
        while(f){
            string t;
            getline(f,t);
            words.push_front(t);
        }
    }
    while(true){
        loadSites();
        list<string> v;
        for(pair<const string,bool>& it:sites)
            if(!it.second){
                v.push_front(it.first);
                it.second = true;
            }
        cout << v.size() << " pastes found..." << endl;
        readPastes(v);
        this_thread::sleep_for(chrono::seconds(10));
    }
}
