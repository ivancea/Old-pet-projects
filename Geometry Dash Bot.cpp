#include <windows.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include <list>

#include "timers.h"

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

inline void clickDown(){
    INPUT i;
    i.type = INPUT_MOUSE;
    i.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &i, sizeof(INPUT));
}

inline void clickUp(){
    INPUT i;
    i.type = INPUT_MOUSE;
    i.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &i, sizeof(INPUT));
}

void play(){
    ifstream file("record.gmd", ios::binary);
    list<uint32_t> laps;
    while(file){
        uint32_t n;
        file.read((char*)&n, 4);
        if(file.gcount()!=4)
            break;
        laps.push_back(n);
    }
    cout << laps.size() << " laps loaded." << endl;
    cout << "Waiting for press F2 and clicking restart button..." << endl;
    while(!isKeyPressed(VK_F2))
        thSleep(1);
    while(isKeyPressed(VK_F2))
        thSleep(1);
    while(!isKeyPressed(VK_LBUTTON))
        thSleep(1);
    while(isKeyPressed(VK_LBUTTON))
        thSleep(1);

    bool spacePressed=false;
    uint32_t nextClock = clock();
    for(uint32_t time:laps){
        nextClock += time;
        while(clock()<nextClock)
            thSleep(1);
        if(!spacePressed){
            spacePressed = true;
            clickDown();
        }else if(spacePressed){
            spacePressed = false;
            clickUp();
        }
    }
    cout << "Finished..." << endl;
}

int record(){
    Chronometer chr;
    cout << "The record file will be overwritten." << endl;
    cout << "Record pressing SPACE when playing (not left mouse click or other configuration)" << endl;
    cout << "Press F2 when you finish recording." << endl;
    cout << "Waiting for press F2 and clicking restart button..." << endl;
    while(!isKeyPressed(VK_F2))
        thSleep(1);
    while(isKeyPressed(VK_F2))
        thSleep(1);
    while(!isKeyPressed(VK_LBUTTON))
        thSleep(1);
    while(isKeyPressed(VK_LBUTTON))
        thSleep(1);

    bool spacePressed=false;
    chr.start();
    while(!isKeyPressed(VK_F2)){
        bool temp = isKeyPressed(VK_SPACE);
        if(!spacePressed && temp){
            spacePressed = true;
            chr.nextLap();
        }else if(spacePressed && !temp){
            spacePressed = false;
            chr.nextLap();
        }else thSleep(1);
    }
    chr.stop();

    cout << "Total time: " << chr.getTotal() << endl;
    cout << "Total laps: " << chr.getLapCount() << endl;

    ofstream file("record.gmd", ios::binary|ios::trunc);
    for(int i=0; i<chr.getLapCount(); i++){
        uint32_t n = chr.getLap(i);
        file.write((char*)&n, sizeof(n));
    }
    file.close();
}

int main(){
    cout << "Disable the \"Auto-Retry\" option. Then, lose." << endl;
    cout << "What do you want? Record (R/r) or play... ";
    char c = cin.get();
    if(c=='R' || c=='r'){
        record();
    }else{
        play();
    }
    while(true)
        cin.get();
}
