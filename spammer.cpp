#include <windows.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

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

inline void key(int vk){
    INPUT i[2];
    i[0].type = INPUT_KEYBOARD;
    i[0].ki.dwFlags = KEYEVENTF_UNICODE;
    i[0].ki.wVk = vk;
    i[1].type = INPUT_KEYBOARD;
    i[1].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
    i[1].ki.wVk = vk;
    SendInput(2, i, sizeof(INPUT));
}

inline void mayusKey(int vk){
    INPUT i[4];
    i[0].type = INPUT_KEYBOARD;
    i[0].ki.dwFlags = 0;
    i[0].ki.wVk = VK_SHIFT;

    i[1].type = INPUT_KEYBOARD;
    i[1].ki.dwFlags = 0;
    i[1].ki.wVk = vk;

    i[2].type = INPUT_KEYBOARD;
    i[2].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
    i[2].ki.wVk = vk;

    i[3].type = INPUT_KEYBOARD;
    i[3].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
    i[3].ki.wVk = VK_SHIFT;
    SendInput(4, i, sizeof(INPUT));
}

inline bool isKeyToggled(int virtualKey){
    return GetKeyState(virtualKey)&1;
}

inline bool isKeyPressed(int virtualKey){
    return GetAsyncKeyState(virtualKey)&0x1000000000000000;
}

void sendString(string s){
    for(int i=0; i<s.size(); i++){
        if(s[i]>='A' && s[i]<='Z'){
            mayusKey(s[i]);
        }else if(s[i]>='a' && s[i]<='z'){
            key(toupper(s[i]));
        }else if(s[i]>='0' && s[i]<='9'){
            key(s[i]);
        }else{
            switch(s[i]){
            case '.': key(VK_OEM_PERIOD);break;
            case ',': key(VK_OEM_COMMA);break;
            case '+': key(VK_OEM_PLUS);break;
            case '-': key(VK_OEM_MINUS);break;
            case '*': key(VK_MULTIPLY);break;
            case '/': key(VK_OEM_2);break;
            case ' ': key(VK_SPACE);break;
            case '\n':key(VK_RETURN);break;
            }
        }
    }
}

uint64_t fib(uint64_t n){
    static vector<uint64_t> v;
    if(!v.size()){
        v.push_back(0);
        v.push_back(1);
    }
    if(v.size()<=n)
        for(uint64_t i=v.size(); i<=n; i++)
            v.push_back(v[i-1] + v[i-2]);
    return v[n];
}

int main(){
    cout << "Frase: ";
    string t,s;
    int n;
    getline(cin,s);
    cout << "Repeticiones: ";
    getline(cin,t);
    n = stoi(t);
    cout << "Pulsa F2 cuando estes listo para repetir la frase \""+s+"\", "+to_string(n)+" veces..." << endl;
    while(!isKeyPressed(VK_F2));
    for(int i=0; i<n; i++)
        sendString(s+'\n');
}
