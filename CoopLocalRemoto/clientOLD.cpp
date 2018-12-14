#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include "sockets.cpp"

using namespace std;

inline bool isKeyPressed(int virtualKey){
    return GetAsyncKeyState(virtualKey)&0x1000000000000000;
}

inline void sendKeyState(TCPClient &client, int vk, bool state){
    client.send(string((char*)&vk,    sizeof(vk)));
    client.send(string((char*)&state, sizeof(state)));
}

int main(){
    /** CONNECTION **/
    TCPClient client;
    string ip;
    short port;
    {
        string t;
        do{
            cout << "Port: ";
            getline(cin,t);
        }while((port=stoi(t))<=0);
    }
    do{
        if(ip!="")
            cerr << "ERROR while connecting to " << ip << "..." << endl;
        cout << "Server IP or DNS: ";
        getline(cin,ip);
    }while(!client.connect(ip, port));
    cout << "Connected." << endl;

    client.setBlocking(true);


    /** GET KEYS FROM SERVER **/
    map<int,bool> keys; /* <VK,PRESSED> */
    string nKeysStr = client.recv(4);
    if(nKeysStr.size()!=4){
        cerr << "ERROR while receiving number of keys..." << endl;
        return -1;
    }

    int nKeys = *(int*)nKeysStr.c_str();
    cout << nKeys << " keys enabled: ";
    for(int i=0; i<nKeys; i++){
        string keyStr = client.recv(4);
        if(keyStr.size()!=4){
            cerr << "ERROR while receiving key..." << endl;
            return -1;
        }
        cout << (char)*(int*)keyStr.c_str();
        keys[*(int*)keyStr.c_str()];
    }
    cout << endl;

    /** GET AND SEND INITIAL KEYS STATE **/
    for(pair<const int, bool> p:keys){
        p.second = isKeyPressed(p.first);
        sendKeyState(client, p.first, p.second);
    }

    /** MAIN LOOP **/
    while(client.isConnected()){
        for(pair<const int, bool>& p:keys){
            bool temp;
            if((temp=isKeyPressed(p.first))!=p.second){
                sendKeyState(client, p.first, temp);
                p.second = temp;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    cout << "Client disconnected..." << endl;
    cin.get();
    cin.get();
}
