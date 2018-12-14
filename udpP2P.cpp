#include <iostream>
#include <set>
#include <thread>
#include <chrono>

#include "strings.h"
#include "udp.h"

using namespace std;


struct __WSA{
    __WSA(){
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2,2), &wsaData);
    }
    ~__WSA(){
        WSACleanup();
    }
}__WSA__;

string ipToChar(string ip){
    if(!validIp(ip)) return "";
    string t;
    ip+='.';
    for(int i=0; i<ip.size(); i++)
        if(ip[i]=='.'){
            t+=(unsigned char)atoi(ip.substr(0,i).c_str());
            ip.erase(0,i+1);
        }
    return t;
}

string charToIp(string chars){
    if(chars.size()!=4) return "";
    string t;
    for(unsigned char c:chars){
        t+=(unsigned int)c+'.';
    }
    t.erase(t.size()-1,1);
    return t;
}

string reduceInt(int n){
    return string((char*)&n,4);
}

int expandInt(string str){
    return *(int*)str.c_str();
}

const int PORT = 12368;

set<string> ips;

void listener(bool*);

int main(){
    bool terminar=false;
    thread ls(&listener,&terminar);
    cout << "#Started#" << endl;
    while(true){
        string t;
        getline(cin,t);
        if(equalIgnoreCase(t.substr(0,8),"connect ") && t.size()>8){
            string ip = t.substr(8,t.size());
            if(UDPSend(ip, PORT, "\x00\x01") == 0){
                cout << "Command sent." << endl;
            }else{
                cout << "Error sending the command..." << endl;
            }
        }else if(equalIgnoreCase(t.substr(0,7),"listips")){
            cout << "IPS: " << ips.size() << endl;
            for(const string& ip:ips)
                cout << ip << endl;
            cout << "END" << endl;
        }else cout << "Error with command..." << endl;
    }
    terminar = true;
    ls.join();
}

void listener(bool* terminar){ // Thread
    string t;
    string ip;
    while(!(*terminar)){
        this_thread::sleep_for(chrono::milliseconds(5));
        if(!UDPRecv(&t, &ip, PORT, true) || t.size()==0)
           continue;

        if(t[0]==0){
            if(t.size()==2 && t[1]==1){ //Quiere entrar a la red
                cout << 1 << endl;
                string vectorIps = "\x00\x03" + reduceInt(ips.size());
                for(string s:ips){
                    vectorIps+=ipToChar(s);
                }
                UDPSend(ip,PORT,vectorIps);
                ips.insert(ip);
                cout << ip << endl;
            }else if(t.size()>=9 && t.size()<=17 && t[1]==2){ //Agregar una ip
                cout << 2 << endl;
                t.erase(0,2);
                if(validIp(t))
                    ips.insert(t);
            }else if(t.size()>=9 && t[1]==3){ //Agregar ips de toda la red
                cout << 3 << endl;
                t.erase(0,2);
                ips.insert(ip);
                int numIps = expandInt(t.substr(0,4));
                t.erase(0,4);
                for(int c=0;t.size()>=4 && c<numIps; c++){
                    string tIp = charToIp(t.substr(0,4));
                    ips.insert(tIp);
                    UDPSend(tIp,PORT,string()+'\0'+(char)4);
                    t.erase(0,4);
                }
            }else if(t.size()==2 && t[1]==4){ //Agregar a la ip que se conecta
                cout << 4 << endl;
                ips.insert(ip);
            }
        }

    }
}
