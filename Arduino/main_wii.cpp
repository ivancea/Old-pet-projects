#include <iostream>
#include <map>
#include <windows.h>

#include "selector.h"

#include "serial.h"

using namespace std;

Serial se;

string port = "\\\\.\\COM3";

struct WiiChuckData{
    short roll, pitch;
    short accelX, accelY, accelZ;
    short joyX, joyY;
    bool buttonC, buttonZ;
};

int readWiiChuck(WiiChuckData &data){
    se.writeData("\x01");
    string t;
    while(t.size()!=20)
        t = se.readDataLength(20-t.size(),500, 5);
    const char* p = t.c_str();

    data.roll    = *(short*)(p);
    data.pitch   = *(short*)(p+2);
    data.accelX  = *(short*)(p+4);
    data.accelY  = *(short*)(p+6);
    data.accelZ  = *(short*)(p+8);
    data.joyX    = *(short*)(p+10);
    data.joyY    = *(short*)(p+12);
    data.buttonC = *(short*)(p+14);
    data.buttonZ = *(short*)(p+16);

    return (*(short*)(p+18));
}

struct WiiClassicData{
    bool leftShoulder, rightShoulder;
    short leftShoulderPressure, rightShoulderPressure;
    bool leftZ, rightZ;
    bool up, right, down, left;
    bool x, y, a, b;
    bool select, home, start;
    short leftJoyX, leftJoyY;
    short rightJoyX, rightJoyY;
};

int readWiiClassic(WiiClassicData &data){
    se.writeData("\x01");
    string t;
    while(t.size()!=44)
        t = se.readDataLength(44-t.size(),500, 2);
    const char* p = t.c_str();

    data.leftShoulder          = *(short*)(p);
    data.rightShoulder         = *(short*)(p+2);
    data.leftShoulderPressure  = *(short*)(p+4);
    data.rightShoulderPressure = *(short*)(p+6);
    data.leftZ                 = *(short*)(p+8);
    data.rightZ                = *(short*)(p+10);
    data.up                    = *(short*)(p+12);
    data.right                 = *(short*)(p+14);
    data.down                  = *(short*)(p+16);
    data.left                  = *(short*)(p+18);
    data.x                     = *(short*)(p+20);
    data.y                     = *(short*)(p+22);
    data.a                     = *(short*)(p+24);
    data.b                     = *(short*)(p+26);
    data.select                = *(short*)(p+28);
    data.home                  = *(short*)(p+30);
    data.start                 = *(short*)(p+32);
    data.leftJoyX              = *(short*)(p+34);
    data.leftJoyY              = *(short*)(p+36);
    data.rightJoyX             = *(short*)(p+38);
    data.rightJoyY             = *(short*)(p+40);

    return (*(short*)(p+42));
}

map<int,bool> buttons;

void processButton(int vk, bool state){
    auto it = buttons.begin();
    if((it = buttons.find(vk)) == buttons.end())
        it = buttons.insert(pair<int,bool>(vk, false)).first;

    if(state && !it->second){
        it->second = true;
        INPUT input;
        ZeroMemory(&input, sizeof(input));
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        input.ki.wVk = vk;
        SendInput(1, &input, sizeof(input));
    }else if(!state && it->second){
        it->second = false;
        INPUT input;
        ZeroMemory(&input, sizeof(input));
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        input.ki.wVk = vk;
        SendInput(1, &input, sizeof(input));
    }
}

void safeClearButtons(){
    for(auto it:buttons)
        processButton(it.first, false);
}

bool getButton(WiiClassicData &data, int n){
    bool t[] = {data.up, data.right, data.down, data.left,
                data.a, data.b, data.x, data.y,
                data.select, data.home, data.start,
                data.leftZ, data.rightZ,
                data.leftShoulder, data.rightShoulder};
    if(n>=0 && n<15)
        return t[n];
    return false;
}

int main(){
    cout << "Connecting to " << port << "..." << endl;
    while(!se.start(port))
        Sleep(500);
    cout << "Conected" << endl;

    /**WiiChuckData data;
    bool leftClicked = false,
          rightClicked = false;

    while((GetAsyncKeyState(VK_F10)&0x8000) == 0){
        Sleep(5);

        unsigned int increment = readWiiChuck(data)/100+1;

        if(data.buttonC && !leftClicked){
            leftClicked = true;
            INPUT input;
            ZeroMemory(&input, sizeof(input));
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(input));
        }else if(!data.buttonC && leftClicked){
            leftClicked = false;
            INPUT input;
            ZeroMemory(&input, sizeof(input));
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(input));
        }
        if(data.buttonZ && !rightClicked){
            rightClicked = true;
            INPUT input;
            ZeroMemory(&input, sizeof(input));
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1, &input, sizeof(input));
        }else if(!data.buttonZ && rightClicked){
            rightClicked = false;
            INPUT input;
            ZeroMemory(&input, sizeof(input));
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1, &input, sizeof(input));
        }

        INPUT input;
        ZeroMemory(&input, sizeof(input));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;

        if(data.joyX>20)
            input.mi.dx = (data.joyX/30+1)*increment;
        else if(data.joyX<-20)
            input.mi.dx = (data.joyX/30-1)*increment;

        if(data.joyY>20)
            input.mi.dy = -(data.joyY/30+1)*increment;
        else if(data.joyY<-20)
            input.mi.dy = -(data.joyY/30-1)*increment;

        if(input.mi.dx != 0 || input.mi.dy != 0)
            SendInput(1, &input, sizeof(input));
    }**/

    map<int,int> vk_button;
    WiiClassicData data;

    options opt;
    opt.setTitle("Configure your keys...");
    opt.setOptions({"Manual",
                    "Config: BROFORCE"});

    int selectedOption;
    selectedOption = opt.show();

    opt.setOptions({"Add key link",
                    "Start"});

    if(selectedOption==1){
        vk_button[VK_UP]      = 0;//data.up;
        vk_button[VK_RIGHT]   = 1;//data.right;
        vk_button[VK_DOWN]    = 2;//data.down;
        vk_button[VK_LEFT]    = 3;//data.left;
        vk_button[VK_ESCAPE]  = 9;//data.home;
        vk_button[VK_RETURN]  = 10;//data.start;
        vk_button[VK_NUMPAD1] = 4;//data.a;
        vk_button[VK_NUMPAD2] = 6;//data.x;
        vk_button[VK_NUMPAD3] = 5;//data.b;
    }else while(selectedOption!=1){
        int i=15;
        cout << "Press the Wii Classic key you want to link..." << endl;
        while(i==15){
            Sleep(10);
            readWiiClassic(data);
            for(i=0; i<15; i++)
                if(getButton(data, i))
                    break;
        }

        int j;
        cout << "Press the Keyboard key you want to link..." << endl;
        for(j=0; j<255;){
            if(GetAsyncKeyState(j)&0x8000)
                break;
            if(j==254)
                j=0;
            else ++j;
        }

        vk_button[j] = i;
        cout << j << endl;

        cout << "Wait..." << endl;
        Sleep(500);

        selectedOption = opt.show();
    }


    do{
        Sleep(5);
        unsigned int increment = readWiiClassic(data)/100+1;

        for(auto it:vk_button)
            processButton(it.first, getButton(data, it.second));

    }while((GetAsyncKeyState(VK_F10)&0x8000) == 0);
    safeClearButtons();
}
