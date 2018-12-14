#include <iostream>
#include <queue>
#include <thread>

#include "sockets.h"
#include "sockets.cpp"
#include "timers.h"
#include "mutextype.h"

#include "loqtts.h"

using namespace std;

ttsHandleType hReader;
MutexType<queue<string>> mut;

bool speakerThread(string voice){
    ttsResultType r;
    r = ttsNewReader(&hReader, NULL);
    if (r != tts_OK){
        cerr << ttsGetErrorMessage(r) << endl;
        ttsDeleteSession(NULL);
        return r;
    }
    r = ttsLoadPersona(hReader, voice.c_str(), NULL, NULL);
    if(r != tts_OK){
        cerr << ttsGetErrorMessage(r) << endl;
        ttsDeleteSession(NULL);
        return r;
    }

    r = ttsSetAudio(hReader, "LTTS7AudioBoard", NULL, 32000, tts_LINEAR, tts_STEREO, 0);

    if(r != tts_OK){
        cerr << ttsGetErrorMessage(r) << endl;
        ttsDeleteSession(NULL);
        return r;
    }

    while(true){
        queue<string> &var = mut.lock();
        if(!var.empty()){
            string toRead = var.front();
            var.pop();
            mut.unlock();
            r = ttsRead(
                    hReader,	/* Reader handle */
                    toRead.c_str(), /* Input */
                    ttsFALSE, /* bAsync */
                    ttsFALSE, /* bFromFile */
                    NULL);	/* ttsRead keeps control until the end */
        }else mut.unlock();
        thSleep(20);
    }

    ttsDeleteSession(NULL);
}

void addString(string s){
    mut.lock().push(s);
    mut.unlock();
}

void serverWorker(connection con){
    setBlocking(con.sock, false);
    clock_t last = clock();
    string temp;
    while((temp+=recv(con.sock, 2-temp.size())).size()<2){
        thSleep(1);
        if((clock()-last)*1000/CLOCKS_PER_SEC>1000){
            closesocket(con.sock);
            return;
        }
    }
    uint16_t chars = *(uint16_t*)temp.c_str();
    temp.clear();
    last = clock();
    while((temp+=recv(con.sock, chars-temp.size())).size()<chars){
        thSleep(1);
        if((clock()-last)*1000/CLOCKS_PER_SEC>1000){
            closesocket(con.sock);
            return;
        }
    }
    cout << "Client " << con.ip << " said: \"" << temp << "\"" << endl;
    addString(temp);
}

int main(){
    uint16_t port = 666;
    thread speakerTh(&speakerThread, "Jorge");
    TCPRawServer server;
    cout << "Starting server on port " << port << "..." << endl;
    while(!server.start(port));
    server.setBlocking(true);
    cout << "Server started and listening..." << endl;
    while(true){
        connection t = server.newClient();
        thread(&serverWorker, t).detach();
        thSleep(5);
    }
}
