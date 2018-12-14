#include "irc.h"
#include "strings.h"
#include "timers.h"
#include "mutextype.h"

#include <iostream>
#include <queue>
#include <thread>

#include "loqtts.h"

using namespace std;

bool interpretar(irc &ir, string sender, string msg){
    replaceAll(msg, "hacker", "jaker");
    if(sender!="ivancea96"){
        string toSend = sender + " dice: " + msg;
        TCPClient client;
        client.connect("127.0.0.1", 666);
        uint16_t chars = toSend.size();
        client.send(string((char*)&chars, 2));
        client.send(toSend);
    }
    return false;
}

int main(){
    srand(time(0));
    irc ir;
    string s, canal, *temp;
    cout << "Nick: ";
    cin >> s;
    cout << endl << "Canal: ";
    cin >> canal;
    cout << endl;
    while(!ir.start("ivancea"+to_string(rand()%1000), "0", s, "irc.eu.freenode.net"));
    thSleep(1000);
    ir.connect("#" + canal);
    s.clear();
    do{
        if(ir.receive(s)){
           cout << s << endl;
        }else thSleep(10);
    }while(!contains(s, "329"));
    cout << "Conectado." << endl << endl;
    while(true){
        thSleep(25);
        if(ir.receive(s)){
            cout << s << endl;
            if(contains(s, "PRIVMSG " + ir.getChannel() + " :")){
                vector<string> temp;
                if((temp = split(s, "PRIVMSG " + ir.getChannel() + " :", 2)).size()==2){
                    bool found=false;
                    string name;
                    for(int i=1; i<temp[0].size(); i++)
                        if(temp[0][i]=='!'){
                            name = temp[0].substr(1,i-1);
                            break;
                        }
                    if(interpretar(ir, name, temp[1])) break;
                }
            }
            if(s.substr(0,4) == "PING")
                ir.send("PONG " + s.substr(5, s.size()) + "\r\n");
            if((contains(s, ir.getNick()) && contains(s, "PART " + ir.getChannel())) || contains(s, "KICK " + ir.getChannel() + " " + ir.getNick()))
                ir.connect("#" + canal);
        }
    }
}
