#include <windows.h>
#include <iostream>

using namespace std;

inline void click(){
    INPUT i;
    i.type = INPUT_MOUSE;
    i.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &i, sizeof(INPUT));

    ZeroMemory(&i, sizeof(INPUT));
    i.type = INPUT_MOUSE;
    i.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &i, sizeof(INPUT));
}

inline bool key(int virtualKey){
    return GetKeyState(virtualKey)&1;//&0x1000000000000000;
}

int main(){
    int n;
    do{
        string t;
        cout << "Milliseconds: ";
        getline(cin,t);
        n = stoi(t);
    }while(n<=0);
    cout << "Working...  Press F2 por make a left click every " + to_string(n) + " milliseconds." << endl;
    cout << " ~" + to_string(1000/n) + " clicks per second.";
    while(true){
        if(key(VK_F2)){
            click();
            Sleep(n);
        }else Sleep(200);
    }
}
