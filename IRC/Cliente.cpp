#include "C:\includes\ivancea.h"

string owner = "ivancea96";

vector<string> ignores;
bool onlyme=false;

bool isHex(string s){
    for(auto it:s)
        if(!((it>='0' && it<='9') || (it>='a' && it<='f') || (it>='A' && it<='F')))
            return false;
    return true;
}

vector< pair<string,bool> > getNames(irc &ir, string channel){
    vector< pair<string,bool> > v;
    if(channel.size()==0) return v;
    if(channel[0]!='#') channel = "#"+channel;
    string t;
    ir.send("NAMES " + channel);
    clock_t cl=clock();
    while(!contains((t=ir.getClient()->recvChars()), " 353 ")) if(((clock()-cl)*1000)/CLOCKS_PER_SEC>1000) return v;
    while(!contains(t,"\r\n"))
         if(((clock()-cl)*1000)/CLOCKS_PER_SEC>1000) return v;
        else t+=ir.getClient()->recvChars();
    vector<string> temp = split(t, ':');
    t = temp[temp.size()-1].substr(0,temp[temp.size()-1].size()-2);
    temp = split(t, ' ');
    for(auto it:temp)
        if(it[0]=='@') v.push_back(pair<string,bool>(it.substr(1,it.size()),true));
        else v.push_back(pair<string,bool>((it[0]=='+'?it.substr(1,it.size()):it),false));
    return v;
}

bool interpretar(irc &ir, string sender, string msg){
    string temp;
    if(equalIgnoreCase(msg.substr(0, 7), "hello\r\n") || equalIgnoreCase(msg.substr(0, 4), "hi\r\n")
       || equalIgnoreCase(msg.substr(0, 6), "hola\r\n")){
        ir.sendMsg("## Hello friend!");
    }
    else if(msg.substr(0, 10)=="BOT date\r\n"){
        time_t rawtime;
        time (&rawtime);
        ir.sendMsg("## Time and date:" + string(ctime(&rawtime)));
    }
    else if(msg.substr(0, 9)=="BOT SEND "){
        if(contains(msg, "LIST", 9)){ir.sendMsg("## No usar comando LIST."); return false;}
        for(size_t i=0; i<msg.size()-4; i++){
            if(msg.substr(i,2)=="\\x" && isHex(msg.substr(i+2,2))){
                char c = strtoull(msg.substr(i+(msg[i+2]==0?3:2),2).c_str(),NULL,16);
                msg.erase(i+1,3);
                msg[i] = c;
                if(c=='\r' || c=='\n'){
                    ir.sendMsg("## Caracteres prohibidos insertados.");
                    return false;
                }
            }
        }
        replaceAll(msg, "<ch>", ir.getChannel());
        replaceAll(msg, "<bot>", ir.getNick());
        replaceAll(msg, "<owner>", owner);
        replaceAll(msg, "<sender>", sender);
        if(tolower(msg.substr(0,13))=="bot send quit")
            return false;
        ir.send(msg.substr(9, msg.size()-11));
    }
    else if(msg.substr(0, 8)=="BOT SAY "){
        replaceAll(msg, "<ch>", ir.getChannel());
        replaceAll(msg, "<bot>", ir.getNick());
        replaceAll(msg, "<owner>", owner);
        replaceAll(msg, "<sender>", sender);
        ir.sendMsg(msg.substr(8, msg.size()));
        irc::log(ir.getNick(), msg.substr(8, msg.size()) + "\r\n");
    }
    else if(msg.substr(0, 7)=="BOT KEY"){
        cout << ir.getClient()->getData(0) << endl;
        ir.sendMsg("## KEY wrote.");
    }
    else if(msg.substr(0, 12)=="BOT CLEANLOG"){
        irc::cleanLog();
        ir.sendMsg("## Log Cleaned.");
    }
    else if(msg.substr(0, 11)=="BOT IGNORE "){
        if(msg.substr(11,msg.size()-13)==owner){
            ir.sendMsg("## I can't ignore My Lord " + owner + ".");
            return false;
        }
        ignores.push_back(msg.substr(11,msg.size()-13));
        ir.sendMsg("## " + msg.substr(11,msg.size()-13) + " is now ignored.");
    }
    else if(msg.substr(0, 13)=="BOT UNIGNORE "){
        int cont=0;
        for(auto it=ignores.begin(); it!=ignores.end();)
            if(*it==msg.substr(13,msg.size()-15)){
                ++cont;
                ignores.erase(it);
            }else ++it;
        if(cont)
            ir.sendMsg("## " + msg.substr(13,msg.size()-15) + " is now unignored.");
    }
    else if(msg.substr(0, 10)=="BOT ONLYME"){
        if(sender!=owner)
            ir.sendMsg("## You are not My Lord.");
        else
        if(!onlyme){
            ir.sendMsg("## Now I only hear My Lord " + owner + ".");
            onlyme=true;
        }else{
            ir.sendMsg("## Now I hear everybody but ignored.");
            onlyme=false;
        }
    }else if(msg.substr(0, 14)=="BOT FACTORIZA "){
        uint64_t n = strtoull(msg.substr(14,msg.size()).c_str(),NULL,0), contador=0;
        if(n==0){
            ir.sendMsg("## Bad number.");
            return false;
        }
        ir.sendMsg("## Los factores primos de " +to_string(n) + " son:");
        string msg = "##  1,";
        if(n==1){
            ir.sendMsg(msg.substr(0,msg.size()-1)+".");
            return false;
        }
        for(uint64_t i = 2;i<(n/2)+1;){
            if(n%i==0){
                if(n==i){
                    break;
                }else{
                    n=n/i;
                    msg += " " + to_string(i) + ",";
                }
            }else if(contador==0){contador++; i++;}else{i+=2;}
        }
        msg+=" " + to_string(n) + ".";
        ir.sendMsg(msg);
    }else if(msg.substr(0,13) == "BOT SETOWNER "){
        if(sender==owner){
            owner = msg.substr(13,msg.size()-15);
            ir.sendMsg("## My new lord is " + owner + ".");
            ir.send("NICK "+owner+'-'+"Guardian");
        }else{
            ir.sendMsg("## You are not my owner.");
        }
    }else if(msg.substr(0,10) == "BOT PEOPLE"){
        auto v = getNames(ir, ir.getChannel());
        string t;
        for(auto it:v)
            t+=" " + it.first;
        ir.sendMsg("## People:" + t);
    }else if(msg == "BOT MAKE A KING OF ME\r\n"){
        if(sender!=owner){
            ir.sendMsg("## You won't be a king, poor " + sender + ".");
            return false;
        }
        auto names = getNames(ir, ir.getChannel());
        for(auto it:names)
            if(it.first==ir.getNick())
                if(it.second){
                    ir.sendMsg("## Yes, My Lord.");
                    break;
                }else{
                    ir.sendMsg("## Sorry, My Lord, but I am not OP. I can't make a king of you...");
                    return false;
                }
        for(auto it:names){
            if(it.second && it.first!=owner && it.first!=ir.getNick())
                ir.send("MODE " + ir.getChannel() + " -o " + it.first);
        }
        ir.send("MODE " + ir.getChannel() + " +o " + owner);
    }else if(msg == "BOT OP ALL\r\n"){
        if(sender!=owner){
            ir.sendMsg("## You can't use that command, " + sender + ".");
            return false;
        }
        auto names = getNames(ir, ir.getChannel());
        string t;
        for(auto it:names)
            if(!it.second)
                t+=" "+it.first;
        if(t.size()==0) return false;
        ir.send("MODE " + ir.getChannel() + " +o" + t);
    }else if(msg == "BOT DEOP ALL\r\n"){
        if(sender!=owner){
            ir.sendMsg("## You can't use that command, " + sender + ".");
            return false;
        }
        auto names = getNames(ir, ir.getChannel());
        string t;
        for(auto it:names)
            if(it.second && it.first!=ir.getNick())
                t+=" "+it.first;
        if(t.size()==0) return false;
        ir.send("MODE " + ir.getChannel() + " -o" + t);
    }else if(msg == "BOT KICK ALL\r\n"){
        if(sender!=owner){
            ir.sendMsg("## You can't use that command, " + sender + ".");
            return false;
        }
        auto names = getNames(ir, ir.getChannel());
        for(auto it:names)
            if(it.first!=owner && it.first!=ir.getNick())
                ir.send("KICK " + ir.getChannel() + " " + it.first);
    }else if(msg=="BOT, deja de molestar de una vez, cojones\r\n"){
        if(sender!=owner) return false;
        ir.sendMsg("## Lo siento, mi señor...");
        return true;
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
    ir.getClient()->setData(parseInt(rand()), 0);
    cout << "Clave: " << ir.getClient()->getData(0) << endl << endl;
    while(!ir.start("ivancea", "0", s, "irc.eu.freenode.net"));
    thSleep(1000);
    ir.connect("#" + canal);
    while(!contains(s=ir.getClient()->recvChars(), "329")) if(s!="") cout << s << endl;
    cout << "Conectado." << endl;
        do{
            thSleep(10);
            if((s=ir.getClient()->recvChars())!=""){
                cout << s << endl;
                if(contains(s, "PRIVMSG " + ir.getChannel() + " :"))
                    if(contains(s, "PRIVMSG " + ir.getChannel() + " :")==1){
                        if(split(&temp, s, "PRIVMSG " + ir.getChannel() + " :", 2)==2){
                            bool found=false;
                            string name;
                            for(int i=1; i<temp[0].size(); i++)
                                if(temp[0][i]=='!'){
                                    name = temp[0].substr(1,i-1);
                                    break;
                                }
                            if(onlyme && name!=owner) continue;
                            for(auto it:ignores)
                                if(name==it){
                                    found = true;
                                    break;
                                }
                            if(found) continue;
                            if(interpretar(ir, name, temp[1])) break;
                            irc::log(s);
                        }
                        delete[] temp;
                    }
                    else if(contains(s, "PRIVMSG " + ir.getChannel() + " :")>1){
                        string name;
                        for(int i=1; i<s.size(); i++)
                            if(s[i]=='!'){
                                name = s.substr(1,i-1);
                                break;
                            }
                        for(int i=0; i<s.size(); i++){
                            if(contains(s, "PRIVMSG " + ir.getChannel() + " :", i)){
                                if(interpretar(ir, name, s.substr(i+("PRIVMSG " + ir.getChannel() + " :").size(), s.size()))) break;
                                irc::log(s);
                                break;
                            }
                        }
                    }
                if(s.substr(0,4) == "PING")
                    ir.send("PONG " + s.substr(5, s.size()) + "\r\n");
            }
            if((contains(s, ir.getNick()) && contains(s, "PART " + ir.getChannel())) || contains(s, "KICK " + ir.getChannel() + " " + ir.getNick()))
                ir.connect();
        }while(!contains(s, "BOT " + ir.getClient()->getData(0) + "\r\n"));

    ir.sendMsg("## Bot disconnected. Bye bye. ##");
    return 0;
}
