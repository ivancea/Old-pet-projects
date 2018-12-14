#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>
#include <mutex>

#include "thmanager.h"
#include "timers.h"

#include "sockets.h"
#include "question.h"

#define PORT 12368
#define TIMEOUT 1000
#define FOLDER_QUESTS "quests/"

using namespace std;

void mutexCout(string s, bool coutEndl = true){
    static mutex mCout;
    lock_guard<mutex> lock(mCout);
    cout << s;
    if(coutEndl)
        cout << endl;
}

string recvTimeout(SOCKET s, uint32_t ms, uint32_t length=0){
    string t;
    clock_t time = clock();
    if(length==0){
        while((t=recv(s))==""){
            if((clock()-time)*1000/CLOCKS_PER_SEC>TIMEOUT){
                closesocket(s);
                break;
            }
            thSleep(1);
        }
    }else{
        while((t+=recv(s, length-t.size())).size()<length){
            if((clock()-time)*1000/CLOCKS_PER_SEC>TIMEOUT){
                closesocket(s);
                break;
            }
            thSleep(1);
        }
    }
    return t;
}

void listener(connection s){
    setBlocking(s.sock, false);
    clock_t time = clock();
    string t;
    t = recvTimeout(s.sock, TIMEOUT, 4);
    if(t.size()!=4){
        send(s.sock, string(1,1));
        closesocket(s.sock);
        return;
    }
    uint32_t nBytes = *((uint32_t*)(void*)t.c_str());
    mutexCout(to_string(nBytes));
    t = recvTimeout(s.sock, TIMEOUT, nBytes);
    container c;
    if(!c.deserialize(t)){
        send(s.sock, string(1,2));
        closesocket(s.sock);
        return;
    }
    mutexCout(s.ip + " sent " + to_string(c.q.size()) + " questions.");
    ofstream fOut(FOLDER_QUESTS + s.ip, ios::binary|ios::app);
    uint32_t size = t.size();
    fOut.write((char*)(void*)&size,4);
    fOut << t;
    send(s.sock, string(1,0));
    closesocket(s.sock);
}

int main(){
    /**container c;
    question q;
    for(int i=0; i<4; i++){
        q.clear();
        q.q = "Pregunta numero " + to_string(i);
        q.ans.push_back("Respuesta 1");
        q.ans.push_back("Respuesta 2");
        q.tag.push_back("programacion");
        q.tag.push_back("hacking");
        c.add(q);
    }**/
    threadManager<connection> th(&listener, 20, true);
    TCPRawServer srv;
    if(!srv.start(PORT)){
        mutexCout("Error opening server.");
        system("pause");
    }
    mutexCout("Server started on port " + to_string(PORT) + ".");
    srv.setBlocking(true);
    while(true){
        connection c;
        c = srv.newClient();
        th.add(c);
    }
}
