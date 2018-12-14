#include <iostream>
#include <map>

#include "windowHandle.cpp"
#include "processHandle.cpp"

using namespace std;

enum Values{
	NS=0x10,
	EO=0x14,
	ALTURA=0x18,
	MOVIMIENTO=0x38,
	VCARRERA=0x88,
	VVOLAR=0x98
};

map<string,unsigned int> values = {
	{"N-S",0x10},
	{"E-O",0x14},
	{"Altura",0x18},
	{"Rotacion horizontal",0x20},
	{"Rotacion vertical",0x24},
	{"Movimiento y volar",0x38},
	{"Velocidad actual",0x80},
	{"Velocidad andando",0x84},
	{"Velocidad corriendo",0x88},
	{"Velocidad corriendo hacia atras",0x8C},
	{"Velocidad de nado",0x90},
	{"Velocidad de nado hacia atras",0x94},
	{"Velocidad de vuelo",0x98},
	{"Velocidad de vuelo hacia atras",0x9C},
	{"Velocidad de giro",0xA0},
	{"Escalar",0xD4}};

int main(){
	ProcessHandle proc;
	if(proc.attach("WoF.exe")){
		cout << "Attached..." << endl;
		
		/*string t;
		getline(cin, t);
		string command = t.substr(0, t.find(' '));
		float value =*/
		
		for(auto t : values){
			unsigned long int addr = proc.getAddress(0x00400000 + 0x00B217CC, {0x44, 0x24, 0xE4, 0x00 + t.second});
			cout << t.first << ": " << proc.readFloat(addr) << endl;
		}
		/*while(true){
			if(!proc.writeFloat(addr, 200.f)){
				cout << "ERROR" << endl;
				break;
			}
			Sleep(100);
		}*/
		
		/*while((GetAsyncKeyState(VK_F5)&0x8000) == 0)
			Sleep(1);
		proc.terminate();*/
		
		cout << "Finished." << endl;
	}else cout << "ERROR: Process not found" << endl;
}