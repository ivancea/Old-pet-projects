#include <iostream>
#include <set>
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
 
bool recvBool(TCPClient& client){
        string t;
        while((t = client.recv(sizeof(bool))) == "");
        return *(bool*)t.c_str();
}
 
int main(){
        string allKeys = "abcdefghijklmnopqrstuvwxyz0123456789.,+-*/ \\u\\r\\d\\l\\-\\n\\s\\c\\t\\a";
        set<int> keys;
        string chars;
        do{
                keys.clear();
                chars = Read::asString("Insert keys to receive from client, <nothing> for all keys:");
                if(chars.size()<=0)
                        chars = allKeys;
                for(int i=0; i<chars.size(); i++){
                        if(chars[i]>='a' && chars[i]<='z'){
                                chars[i] -= 'a'-'A';
                        }
                       
                        if(chars[i]>='A' && chars[i]<='Z'){
                                keys.insert(chars[i]);
                        }else if(chars[i]>='0' && chars[i]<='9'){
                                keys.insert(VK_NUMPAD0+chars[i]-'0');
                                keys.insert(chars[i]);
                        }else if(chars[i]=='\\' && i+1<chars.size()){
                                switch(chars[i+1]){
                                case 'U':
                                case 'u': keys.insert(VK_UP); break;
 
                                case 'R':
                                case 'r': keys.insert(VK_RIGHT); break;
                               
                                case 'D':
                                case 'd': keys.insert(VK_DOWN); break;
                               
                                case 'L':
                                case 'l': keys.insert(VK_LEFT); break;
                               
                                case 'N':
                                case 'n': keys.insert(VK_RETURN); break;
                               
                                case '-': keys.insert(VK_DELETE); break;
                               
                                case 'T':
                                case 't': keys.insert(VK_TAB); break;
                               
                                /*case 'S':
                                case 's': keys.insert(VK_SHIFT); break;
                               
                                case 'C':
                                case 'c': keys.insert(VK_CONTROL); break;
                               
                                case 'A':
                                case 'a': keys.insert(VK_MENU); break;*/
                                }
                                ++i;
                        }else{
                                switch(chars[i]){
                                case '+': keys.insert(VK_OEM_PLUS);
                                          keys.insert(VK_ADD);break;
                                case '-': keys.insert(VK_OEM_MINUS);
                                          keys.insert(VK_SUBTRACT);break;
                                case '*': keys.insert(VK_MULTIPLY); break;
                                case '/': keys.insert(VK_DIVIDE); break;
                                case ',': keys.insert(VK_OEM_COMMA); break;
                                case '.': keys.insert(VK_OEM_PERIOD); break;
                                case ' ': keys.insert(VK_SPACE); break;
                                default: chars.erase(i,1); --i; break;
                                }
                        }
                }
                if(keys.size()<=0)
                        cout << "No keys found." << endl << endl;
        }while(keys.size()<=0);
       
        cout << endl << keys.size() << " keys found: " << chars << "." << endl << endl;
       
        TCPClient client;
       
        {
                TCPRawServer server;
                while(!server.start(Read::asInt("Server port:"))){
                        cout << "Couldn't open server" << endl
                                 << "Insert server data again..." << endl;
                }
               
                cout << "Waiting for a client..." << endl;
               
                Connection c;
               
                while((c=server.newClient()).sock == INVALID_SOCKET)
                        Sleep(50);
                client.connect(c.sock, c.ip, server.getPort());
        }
       
        int n = chars.size();
        client.send(string((char*)&n, sizeof(n)));
        client.send(chars);
       
        n = keys.size();
        client.send(string((char*)&n, sizeof(n)));
        for(int key : keys){
                client.send(string((char*)&key, sizeof(key)));
        }
       
        cout << "Client connected." << endl
             << "Started receiving keys..." << endl;
       
        client.setBlocking(true);
        while(n = recvInt(client)){
                bool pressed = recvBool(client);
                if(keys.find(n) != keys.end()){
                        INPUT input;
                        input.type = INPUT_KEYBOARD;
                        input.ki.wVk = n;
                        if(pressed){
                                input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
                        }else{
                                input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;
                        }
                       
                        SendInput(1, &input, sizeof(INPUT));
                }
        }
}