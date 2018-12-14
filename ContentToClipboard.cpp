#define  _WIN32_WINNT  0x0600
#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

int regAdd(HKEY type, std::string ruta, const char* valor, std::string data){
    HKEY key;
    if(RegCreateKey(type,ruta.c_str(),&key) != ERROR_SUCCESS)
		return 1;
    if(RegSetValueEx(key, valor, 0, REG_SZ, (const BYTE*)data.c_str(),data.size()) != ERROR_SUCCESS){
		RegCloseKey(key);
		return 2;
	}
	RegCloseKey(key);
	return 0;
}

void regDelKey(HKEY type, std::string ruta, std::string subKey){
    HKEY key;
    long lng = RegOpenKey(type,ruta.c_str(),&key);
    if(lng == ERROR_SUCCESS){
        RegDeleteTree(key, subKey.c_str());
        RegCloseKey(key);
    }
}

void showError(const char* error){
	std::cerr << error << std::endl;
	MessageBox(NULL, error, "ContentToClipboard::ERROR", MB_OK);
}

int copyFile(const char* fileName){
	int errorLevel = 0;
	std::ifstream in(fileName, std::ios::binary|std::ios::ate);
	if(!in){
		showError("Couldn't open file for reading");
		errorLevel = 2;
	}else{
		size_t size = (int)in.tellg() + 1;
		in.seekg(0);
		
		HGLOBAL glob = (char*)GlobalAlloc(GMEM_MOVEABLE, size);
		if(glob == NULL || GlobalSize(glob)<size){
			showError("Couldn't alloc enough memory");
			errorLevel = 3;
		}else{
			char* buff = new char[4096];
			char* clipboard = (char*)GlobalLock(glob);
			char* tt = clipboard;
			if(clipboard==NULL){
				showError("Couldn't lock global memory");
				GlobalFree(glob);
				errorLevel = 4;
			}else{
				do{
					in.read(buff, 4096);
					memcpy(clipboard, buff, in.gcount());
					clipboard += in.gcount();
				}while(in);
				*clipboard = '\0';
				GlobalUnlock(glob);
				in.close();
				
				if(!OpenClipboard(GetDesktopWindow())){
					showError("Couldn't open clipboard");
					errorLevel = 5;
				}else{
					EmptyClipboard();
					SetClipboardData(CF_TEXT, glob);
					CloseClipboard();
				}
			}
			delete[] buff;
		}
	}
	return errorLevel;
}

int main(int argc, char** argv){
	const char* contextMenuName = "Copiar contenido"; // For multi-language purposes
	int errorLevel = 0;
	if(argc == 1){
		char buffer[MAX_PATH]={0};
		switch(MessageBox(NULL, "Do you want to install this program (YES) or uninstall (NO)? (Requires explicit admin rights)", "ContentToClipboard::Installation", MB_YESNOCANCEL)){
			case IDYES:
				GetModuleFileName(NULL, buffer, MAX_PATH);
				ShellExecute(NULL, NULL, "cmd.exe", "/c mkdir \"C:\\Program Files\\ContentToClipboard\"", NULL, 0);
				ShellExecute(NULL, NULL, "cmd.exe", (std::string("/c copy \"") + buffer + "\" \"C:\\Program Files\\ContentToClipboard\\ContentToClipboard.exe\"").c_str(), NULL, 0);
				if(regAdd(HKEY_CLASSES_ROOT, "*\\shell\\ContentToClipboard", NULL, contextMenuName) != 0
				|| regAdd(HKEY_CLASSES_ROOT, "*\\shell\\ContentToClipboard\\command", NULL, "\"C:\\Program Files\\ContentToClipboard\\ContentToClipboard.exe\" \"%1\"") != 0){
					showError("Couldn't create registry keys");
					errorLevel = 1;
				}else{
					MessageBox(NULL, "Installed. Now, you can access the option for copy the contents of a file to the clipboard (may fail in binary files)", "ContentToClipboard::Installation OK", MB_OK);
				}
				break;
			case IDNO:
				ShellExecute(NULL, NULL, "cmd.exe", "/c rd /S /Q \"C:\\Program Files\\ContentToClipboard", NULL, 0);
				regDelKey(HKEY_CLASSES_ROOT, "*\\shell", "ContentToClipboard");
					MessageBox(NULL, "Uninstalled", "ContentToClipboard::Uninstalled", MB_OK);
				break;
			default:
				break;
		}
	}else if(argc == 2){
		errorLevel = copyFile(argv[1]);
	}
	return errorLevel;
}