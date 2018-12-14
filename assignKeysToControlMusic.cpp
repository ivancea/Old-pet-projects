#include <windows.h>
#include <iostream>
#include <map>

using namespace std;

void send(int vk){
    INPUT i[2];
    i[0].type = INPUT_KEYBOARD;
    i[0].ki.dwFlags = KEYEVENTF_UNICODE;
    i[0].ki.wVk = vk;
    i[1].type = INPUT_KEYBOARD;
    i[1].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
    i[1].ki.wVk = vk;
    SendInput(2, i, sizeof(INPUT));
}

string getKeyName(int vk){
	char buff[100];
	return string(buff, GetKeyNameText(MapVirtualKey(vk,MAPVK_VK_TO_VSC)<<16, buff, 100));
}

bool pressed(int vk, bool state){ // Return true if changes from not-pressed to pressed
	static map<int, bool> keys;
	if(keys.find(vk) == keys.end())
		keys[vk] = state;
	bool& t = keys[vk];
	if(state ^ t){
		t = state;
		return state;
	}
	keys[vk] = state;
	return false;
}

int main(){
	map<int,int> links = {{VK_F9,  VK_MEDIA_PLAY_PAUSE},
						  {VK_F10, VK_MEDIA_PREV_TRACK},
						  {VK_F11, VK_MEDIA_NEXT_TRACK}};
	cout << "Links:" << endl;
	for(auto it:links)
		cout << "- " << getKeyName(it.first) << " to " << getKeyName(it.second) << endl;
	cout << endl;
	
	while(true){
		for(auto it:links){
			if(pressed(it.first, GetAsyncKeyState(it.first)&0x8000)){
				send(it.second);
				cout << getKeyName(it.second) << endl;
			}
		}
		/*if(pressed(VK_F9, GetAsyncKeyState(VK_F9)&0x8000)){
			send(VK_MEDIA_PLAY_PAUSE);
			cout << "F9 -> PLAY-PAUSE" << endl;
		}else if(pressed(VK_F10, GetAsyncKeyState(VK_F10)&0x8000)){
			send(VK_MEDIA_PREV_TRACK);
			cout << "F10 -> PREV-TRACK" << endl;
		}else if(pressed(VK_F11, GetAsyncKeyState(VK_F11)&0x8000)){
			send(VK_MEDIA_NEXT_TRACK);
			cout << "F11 -> NEXT-TRACK" << endl;
		}*/
		
		Sleep(1);
	}
}