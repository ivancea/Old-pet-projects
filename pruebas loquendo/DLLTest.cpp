#include <iostream>
#include <queue>
#include <thread>

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

    r = ttsSetAudio(hReader, "AudioTest", NULL, 1000, tts_LINEAR, tts_MONO, 0);

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

int main(){
    thread speakerTh(&speakerThread, "Jorge");
    addString("a");
    while(true)
        thSleep(1000);
}
