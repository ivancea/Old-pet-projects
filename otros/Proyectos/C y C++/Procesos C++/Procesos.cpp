#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <typeinfo>

using namespace std;



class process{
private:/********PRIVATE********/

    HWND _hwnd;
    HANDLE _handle;


public: /********PUBLIC********/

    process():_hwnd(0),_handle(0){}

    ~process(){
        closeHandle();
        closeHwnd();
    }

    void closeHandle(){
        CloseHandle(_handle);
        _handle=NULL;
    }

    void closeHwnd(){
        _hwnd=NULL;
    }

    bool openWindow(string title = "Desktop"){
        if(title == "Desktop"){
            char temp[100];
            _hwnd = GetDesktopWindow();
        }else _hwnd = FindWindow(NULL, title.c_str());
        return _hwnd!=NULL;
    }

    bool openProcess(){
        if(_hwnd==NULL) return false;
        return openProcess(getPid());
    }

    bool openProcess(DWORD pid){
        _handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
        if(_handle==NULL)
            return false;
        return true;
    }

    bool openProcess(string name){
        HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe;
        if(Process32First(h, &pe))
            do{
                if(!strcmp(pe.szExeFile, name.c_str())){
                    if(openProcess(pe.th32ProcessID))
                        return true;
                }
            }while(Process32Next(h, &pe));
        return false;
    }

    bool processKill(){
        if(_handle==NULL) return false;
        if(!TerminateProcess(_handle, 0)){
            closeHandle();
            closeHwnd();
            return true;
        }
        return false;
    }

    void keyDown(char vkcode){
        if(_hwnd==NULL) return;
        PostMessageA(_hwnd, WM_KEYDOWN, vkcode, 1);
    }

    void keyUp(char vkcode){
        if(_hwnd==NULL) return;
        PostMessageA(_hwnd, WM_KEYUP, vkcode, 1);
    }

    /********WRITE********/
    template<class var>
    void write(int address, var value, int addressSize = sizeof(var)){
        if(_handle==NULL) return;
        WriteProcessMemory(_handle, (LPVOID)address, &value, addressSize, NULL);
    }


    /********READ********/
    template<class T>
    T read(uint32_t address, size_t bytesRead=16){
        if(_handle==NULL) return T();
        if(typeid(T)!=typeid(string)){
            T val = T();
            ReadProcessMemory(_handle, (LPVOID)address, (PVOID)&val, sizeof(T), NULL);
            return T();
        }
        if(bytesRead==0) return T();
        SIZE_T bytesReaded=0;
        char c[bytesRead];
        ReadProcessMemory(_handle, (LPVOID)address, (PVOID)c, bytesRead, &bytesReaded);
        c[bytesReaded]='\0';
        return T(c);
    }

    int readAsPointer(int base, int offsets[], int num){
        if(_handle==NULL) return 0;
        for(int i=0;i<num;i++)
            base = read<int>(base)+offsets[i];
        return base;
    }


    /********RETURN********/

    string getTitle(){
        if(_hwnd==NULL) return "";
        char temp[MAX_PATH];
        GetWindowText(_hwnd, temp, MAX_PATH);
        return temp;
    }

    DWORD getPid(){
        DWORD id=0;
        id=getPidFromProcess();
        if(id==0)
            id=getPidFromWindow();
        return id;
    }

    DWORD getPidFromWindow(){
        if(_hwnd==NULL) return 0;
        return GetWindowThreadProcessId(_hwnd,NULL);
    }

    DWORD getPidFromProcess(){
        if(_handle==NULL) return 0;
        return GetProcessId(_handle);
    }

    HWND getHwnd(){
        return _hwnd;
    }

    HANDLE getHandle(){
        return _handle;
    }
};

void spam(process proc, string msg, int ms_delay, int times, char vk_open_chat, char vk_send_chat){
    if(ms_delay-100<50) ms_delay = 50;
    else ms_delay -=100;
    for(int i=0; i<times;i++){
        if(vk_open_chat != 0){
        proc.keyDown(vk_open_chat);
        proc.keyUp(vk_open_chat);
        }
        Sleep(50);
        for(int j=0; j<msg.length(); j++){
            proc.keyUp(msg[j]);
        }
        Sleep(50);
        if(vk_send_chat != 0){
        proc.keyDown(vk_send_chat);
        proc.keyUp(vk_send_chat);
        }
        Sleep(ms_delay);
    }
}


int main(){
    string temp="";
    int tem=0;
    cout << "Nombre de la ventana: ";
    getline(cin,temp);
    process a;
    a.openWindow(temp);
    cout << "Mensaje en mayusculas (solo letras): ";
    getline(cin, temp);
    cout << endl << "Numero de repeticiones: ";
    cin >> tem;
    spam(a, temp, 150, tem, VK_RETURN, VK_RETURN);
    int address;
    {
        int off[] = {0x34, 0x24, 0x764, 0x0, 0x0};
        address = a.readAsPointer(0x00CD87A8, off, 5);
    }
    return 0;
}
