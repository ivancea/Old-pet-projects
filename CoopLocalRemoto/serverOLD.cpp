#include <iostream>
#include <algorithm>

#include "sockets.cpp"

using namespace std;

inline bool isKeyPressed(int virtualKey){
    return GetAsyncKeyState(virtualKey)&0x1000000000000000;
}

inline void keyDown(int vk){
    INPUT i;
    i.type = INPUT_KEYBOARD;
    i.ki.dwFlags = KEYEVENTF_UNICODE;
    i.ki.wVk = vk;
    SendInput(1, &i, sizeof(INPUT));
}

inline void keyUp(int vk){
    INPUT i;
    i.type = INPUT_KEYBOARD;
    i.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
    i.ki.wVk = vk;
    SendInput(1, &i, sizeof(INPUT));
}

int main(){
    /** REQUEST KEYS **/
    cout << "Write the letters that client will send (Example: akneofm)" << endl << '-';
    string keys;
    getline(cin,keys);
    for(int i=0; i<keys.size(); ++i){
        if(keys[i]>='a' && keys[i]<='z'){
            keys[i] -= 'a'-'A';
        }else if((keys[i]<'A' || keys[i]>'Z') && keys[i]!=' '){
            keys.erase(i, 1);
            --i;
            continue;
        }
        if(keys.find(keys[i], i+1)!=string::npos || keys.find(keys[i]+'a'-'A', i+1)!=string::npos){
            keys.erase(i,1);
            --i;
        }
    }
    sort(keys.begin(), keys.end(), [](char a, char b)->bool{return a<b;});
    cout << "The keys are: " << keys << endl;


    /** CONNECTION **/
    TCPClient client;
    {
        string t;
        short port;
        do{
            cout << "Port: ";
            getline(cin,t);
        }while((port=stoi(t))<=0);
        TCPRawServer server;
        while(!server.start(port))
            cerr << "ERROR starting server. Retrying..." << endl;
        cout << "Server connected and listening to port " << server.getPort() << "..." << endl;
        server.setBlocking(true);
        Connection temp = server.newClient();
        client.connect(temp.sock, temp.ip, server.getPort());
    }
    client.setBlocking(true);
    cout << "Connected from: " << client.getIp() << endl;


    /** SEND KEYS TO CLIENT **/
    int nKeys = keys.size();
    client.send(string((char*)&nKeys, sizeof(nKeys)));
    for(char c:keys){
        int vk = c;
        client.send(string((char*)&vk, sizeof(vk)));
    }

    /** MAIN LOOP **/
    while(client.isConnected()){
        string vkStr = client.recv(4);
        string stateStr = client.recv(1);
        if(vkStr.size()!=4 || stateStr.size()!=1){
            cerr << "ERROR receiving keys" << endl;
            return 1;
        }
        char vk = *(const int*)vkStr.c_str();
        bool state = *(const bool*)stateStr.c_str();
        if(keys.find(vk)==string::npos)
            continue;
        if(state)
            keyDown(vk);
        else
            keyUp(vk);
    }
    cout << "Client disconnected..." << endl;
    cin.get();
    cin.get();
}
