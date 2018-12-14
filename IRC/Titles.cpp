#include "irc.h"
#include "http.h"
#include "strings.h"
#include <iostream>

using namespace std;
using namespace http;

string getTitle(string s){
    size_t pos = s.find("<title>");
    if(pos==string::npos)
        return "";
    return s.substr(pos+7, s.find("</title>",pos+7)-pos-7);
}

bool interpretar(string& msg, string &res){

    GETRequest gr;
    response re;

    size_t pos = msg.find("http://");
    if(pos==string::npos)
        pos = msg.find("https://");
    if(pos==string::npos)
        return false;
    size_t pos2 = msg.find(" ",pos);
    if(pos2==string::npos)
        pos2=msg.size();
    gr.setUrl(msg.substr(pos, pos2-pos));
    re = sendRequest(gr);
    cout << re.getCode() << endl;
    if(re.getCode()!=200)
        return false;
    string t = getTitle(re.getBody());
    cout << t << endl;
    if(t!=""){
        res = trim(t, " \n\r");
        return true;
    }
    return false;
}

int main(){
    Irc ir;
    string user, channel;
    cout << "User: ";
    getline(cin, user);
    cout << "Channel: ";
    getline(cin, channel);

    ir.setAutoPong(true);
    ir.setChannelMsgCallback([](Irc &irc, string channel, string sender, string msg){
        cout << "<" << channel << "> " << sender << ": " << msg << endl;
        string res;
        if(interpretar(msg, res)){
            cout << res << endl;
            irc.sendChannelMsg(channel, res);
        }
    });

    ir.start(user, "0", user, "irc.eu.freenode.net");
    ir.connect(channel);

    string t;
    while(true){
        while(!ir.receive(t))
            thSleep(20);
    }
}
