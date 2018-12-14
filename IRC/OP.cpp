#include "C:\includes\ivancea.h"

int main(){
    string s, host="irc.eu.freenode.net", canal="#ircehn", nickA="BOTivancea1", nickB="BOTivancea2";
    irc a, b;
    while(!a.start(nickA, "0", nickA, host));
    while(!b.start(nickB, "0", nickB, host));
    cout << "Conexion establecida." << endl;
    thSleep(1000);
    a.connect(canal);
    b.connect(canal);
    while(!contains(s=a.getClient()->recvChars(), "329"));
    cout << nickA << " conectado." << endl;
    while(!contains(s=b.getClient()->recvChars(), "329"));
    cout << nickB << " conectado." << endl;
    while(true){
        /** BOT A **/
        if((s=a.getClient()->recvChars())!=""){
            if(s.substr(0,4) == "PING")
                a.send("PONG " + s.substr(5, s.size()) + "\r\n");
            if((contains(s, a.getNick()) && contains(s, "PART " + a.getChannel())) || contains(s, "KICK " + a.getChannel() + " " + a.getNick())
                || contains(s, "QUIT"))
                a.connect();
            if(contains(s, "MODE " + b.getChannel() + " -o " + b.getNick()))
               a.send("MODE " + b.getChannel() + " +o " + b.getNick());
            if(contains(s, "JOIN " + b.getChannel()) && contains(s, b.getNick()))
               a.send("MODE " + b.getChannel() + " +o " + b.getNick());
        }
        /** BOT B **/
        if((s=b.getClient()->recvChars())!=""){
            if(s.substr(0,4) == "PING")
                b.send("PONG " + s.substr(5, s.size()) + "\r\n");
            if((contains(s, b.getNick()) && contains(s, "PART " + b.getChannel())) || contains(s, "KICK " + b.getChannel() + " " + b.getNick())
                || contains(s, "QUIT"))
                b.connect();
            if(contains(s, "MODE " + a.getChannel() + " -o " + a.getNick()))
               b.send("MODE " + a.getChannel() + " +o " + a.getNick());
            if(contains(s, "JOIN " + a.getChannel()) && contains(s, a.getNick()))
               b.send("MODE " + a.getChannel() + " +o " + a.getNick());
        }
    }
    return 0;
}
