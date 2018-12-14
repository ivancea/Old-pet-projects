#include <iostream>
#include <map>
#include <windows.h>
 
#include "sockets.cpp"
 
using namespace std;
 
class Read {
public:
        Read() = delete;
       
        static string asString(string text, istream& in = cin){
                cout << text << ' ';
                getline(in,text);
                return text;
        }
       
        static int asInt(string text, istream& in = cin){
                cout << text << ' ';
                getline(in,text);
                return stoi(text);
        }
};
 
int recvInt(TCPClient& client){
        string t;
        while((t = client.recv(sizeof(int))) == "");
        return *(int*)t.c_str();
}
 
int main(){
        TCPClient client;
        map<int,bool> keys;
        while(!client.connect(Read::asString("Server IP:"),
                              Read::asInt("Server port:"))){
                cout << "Couldn't connect to server..." << endl
                     << "Insert connection data again..." << endl;
        }
        client.setBlocking(true);
        cout << "Connected." << endl
             << "Receiving keys...: ";
       
        int n = recvInt(client);
        cout << client.recv(n) << endl
             << "Receiving key codes...: ";
        n = recvInt(client);
        for(int i=0; i<n; i++)
                keys[recvInt(client)] = false;
        cout << keys.size() << " keys received." << endl << endl
             << "Start sending keys." << endl;
       
       
        while(client.isConnected()){
                for(pair<const int,bool>& key : keys){
                        bool pressed= GetAsyncKeyState(key.first) & 0x8000;
                        if(pressed != key.second){
                                if(!client.send(string((char*)&key.first, sizeof(int)))
                                || !client.send(string((char*)&pressed, sizeof(bool))))
                                        client.disconnect();
                                key.second = pressed;
                        }
                }
                Sleep(1);
        }
       
}