#include "C:\includes\ivancea.h"

int main(){
    string s, nick, spam;
    int minim, maxim;
    vector<string> channels;
    int j;
    irc ir;
    cout << "Pon el nick: ";
    cin >> nick;
    cout << "Pon el min: ";
    cin >> minim;
    cout << "Pon el max: ";
    cin >> maxim;
    cout << "Pon el mensaje a spamear ('_' sera cambiada por un spacio): ";
    cin >> spam;
    replaceAll(spam, "_", " ");
    while(!ir.start(nick, "0", nick, "irc.eu.freenode.net"))thSleep(20);
    cout << "Conectado." << endl;
    ir.connect("#ircehn");
    while(!contains(s=ir.getClient()->recvChars(), "329")) thSleep(5);
    cout << "Sincronizado con el servidor." << endl;
    thSleep(1000);
    ir.send("PART " + ir.getChannel());
    ir.send("LIST >" + parseInt(minim) + ",<" + parseInt(maxim));
    cout << "LIST enviado. Comenzando..." << endl;
    while(!contains(s, ":End of /LIST")){
        thSleep(1);
        while((s=ir.getClient()->recvChars())=="") thSleep(1);
        if(contains(s, " 322 ")){
            for(int i=0; i<s.size(); i++){
                if(contains(s, " 322 ", i) && i+6+ir.getNick().size()<s.size())
                    if(s[i+6+ir.getNick().size()] == '#'){
                        i+=6+ir.getNick().size();
                        for(j=i; j<s.size() && s[j]!=' ';j++);
                        if(j==s.size()) break;
                        channels.push_back(s.substr(i, j-i));
                    }
            }
        }
    }
    cout << "LIST completada. Total: " << endl << channels.size() << " canales." << endl;
    cout << "User: " << nick << endl << "Mensaje: " << spam << endl;
    for(int i=0; i<channels.size(); i++){
        ir.connect(channels[i]);
        thSleep(1500);
        if(channels[i]!=ir.getChannel()){
            cout << "ERROR : " << channels[i] << " - " << ir.getChannel();
            return 1;
        }
        while((s=ir.getClient()->recvChars())!=""){
            if(s.substr(0,4) == "PING")
                ir.send("PONG " + s.substr(5, s.size()) + "\r\n");
            cout << endl << s << endl << endl;
        }
        thSleep(1500);
        ir.sendMsg(spam);
        thSleep(1500);
        ir.send("PART " + ir.getChannel());
        cout << "-Enviado a: " << ir.getChannel() << endl;
        thSleep(1500);
    }
}
