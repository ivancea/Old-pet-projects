#include <iostream>
#include <fstream>
#include <map>
#include <windows.h>

using namespace std;

bool callForEachValue(HKEY regKey, bool(*fn)(HKEY key, string path, string valueName, void* data), void* data, string currentPath = ""){
	DWORD valueSize = 16400;
	char valueName[valueSize] = {0};
	DWORD type;
	DWORD dataSize = 0;
	//unsigned char valueData[dataSize];
	
	int n = 0;
	while(true){
		int t = RegEnumValue(regKey, n, valueName, &valueSize, NULL, &type, NULL, &dataSize);
		if(t==ERROR_SUCCESS || t==ERROR_MORE_DATA){
			if(fn(regKey, currentPath, valueName, data))
				break;
			valueSize = 16400;
			dataSize = 0;
			valueName[0] = '\0';
			++n;
		}else break;
	}
	return false;
}

bool searchKeys(HKEY regKey, bool(*fn)(HKEY key, string path, string valueName, void* data), void* data, string currentPath = ""){
	if(regKey==NULL)
		return false;
	if(callForEachValue(regKey, fn, data, currentPath))
		return true;
	char buff[MAX_PATH+1];
	int n = 0;
	while(RegEnumKey(regKey, n, buff, MAX_PATH+1) == ERROR_SUCCESS){
		HKEY nextKey;
		if(RegOpenKey(regKey, buff, &nextKey) == ERROR_SUCCESS){
			bool t = searchKeys(nextKey, fn, data, currentPath + "\\" + buff);
			RegCloseKey(nextKey);
			if(t)
				break;
		}
		++n;
	}
	return false;
}
ofstream out("registro.txt", ios::trunc);
int main(){
	map<HKEY, string> keys = {{HKEY_CLASSES_ROOT,  "HKEY_CLASSES_ROOT"},
	                          {HKEY_CURRENT_CONFIG,"HKEY_CURRENT_CONFIG"},
	                          {HKEY_CURRENT_USER,  "HKEY_CURRENT_USER"},
	                          {HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE"},
	                          {HKEY_USERS,         "HKEY_USERS"}};
	int n = 0;
	void* values[2];
	values[0] = &n;
	for(pair<const HKEY, string> &keyPair : keys){
		cout << "Starting with " << keyPair.second << "..." << endl;
		values[1] = &keyPair;
		n = 0;
		searchKeys(keyPair.first, [](HKEY key, string path, string valueName, void* data)->bool{
			out << ((pair<const HKEY, string>*)((void**)data)[1])->second << path << "\\" << (valueName==""?"(default)":valueName) << "\r\n";
			++(*(int*)(((void**)data)[0]));
			if(valueName.find("C:\\Users\\ivancea\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs")!=string::npos || path.find("C:\\Users\\ivancea\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs")!=string::npos)
				cout << "Found: " << ((pair<const HKEY, string>*)((void**)data)[1])->second << path << "\\" << (valueName==""?"(default)":valueName) << endl;
			return false;
		}, values);
		cout << n << " values scanned." << endl << endl;
	}
	cout << "Finished. Press ENTER for exit...";
	cin.get();
}