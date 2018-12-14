#include <iostream>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#include "processHandle.h"

using namespace std;

ProcessHandle::ProcessHandle():_handle(NULL){}
ProcessHandle::ProcessHandle(HANDLE handle):_handle(handle){}
ProcessHandle::ProcessHandle(long unsigned int pid){
	attach(pid);
}
ProcessHandle::ProcessHandle(const ProcessHandle& processHandle):_handle(processHandle.getHandle()){}
	
ProcessHandle::~ProcessHandle(){
	detach();
}
	
bool ProcessHandle::terminate(){
	if(_handle==NULL)
		return false;
	if(TerminateProcess(_handle, 0)!=0){
		detach();
		_handle = NULL;
		return true;
	}
	return false;
}
	
bool ProcessHandle::attach(long unsigned int pid){
	if(pid==0)
		return false;
	detach();
	_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	return _handle!=NULL;
}
	
bool ProcessHandle::attach(string processName){
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	long unsigned int pid = 0;
	if (Process32First(snapshot, &entry) == TRUE){
		do{
			if (processName == entry.szExeFile){
				pid = entry.th32ProcessID;
				break;
			}
		}while(Process32Next(snapshot, &entry) == TRUE);
	}
	CloseHandle(snapshot);
	
	return attach(pid);
}
	
void ProcessHandle::detach(){
	if(_handle != NULL){
		CloseHandle(_handle);
		_handle = NULL;
	}
}
	
long unsigned int ProcessHandle::getProcessId() const{
	if(_handle == NULL)
		return 0;
	return GetProcessId(_handle);
}
	
string ProcessHandle::getProcessName() const{
	if(_handle==NULL)
		return "";
	char buff[256];
	int size = GetModuleBaseNameA(_handle, NULL, buff, 256);
	return string(buff,size);
}
	
bool ProcessHandle::isAttached() const{
	return _handle != NULL;
}

string ProcessHandle::getExecutable() const{
	if(_handle==NULL)
		return "";
	char buff [256];
	int n = GetModuleFileNameEx(_handle, NULL, buff, 256);
	if(n<=0)
		return "";
	return string(buff, n);
}

HANDLE ProcessHandle::getHandle() const{
	return _handle;
}
	
	
/////////////////////// READ ///////////////////////
	
string ProcessHandle::read(long unsigned int address, unsigned long int size) const{
	if(_handle == NULL || size<=0)
		return "";
	char buff[size];
	if(!ReadProcessMemory(_handle, (void*)address, buff, size, &size)){
		return "";
	}
	return string(buff,size);
}
	
char ProcessHandle::readChar(long unsigned int address) const{
	if(_handle == NULL)
		return -1;
	char buff[4]={0};
	unsigned long int size = sizeof(char);
	if(!ReadProcessMemory(_handle, (void*)address, buff, size, &size)){
		return 0;
	}
	return *(char*)buff;
}
	
int ProcessHandle::readInt(long unsigned int address) const{
	if(_handle == NULL)
		return -1;
	char buff[4]={0};
	unsigned long int size = sizeof(int);
	if(!ReadProcessMemory(_handle, (void*)address, buff, size, &size)){
		return 0;
	}
	return *(int*)buff;
}
	
float ProcessHandle::readFloat(long unsigned int address) const{
	if(_handle == NULL)
		return -1;
	char buff[4]={0};
	unsigned long int size = sizeof(float);
	if(!ReadProcessMemory(_handle, (void*)address, buff, size, &size)){
		return 0;
	}
	return *(float*)buff;
}
	
double ProcessHandle::readDouble(long unsigned int address) const{
	if(_handle == NULL)
		return -1;
	char buff[4]={0};
	unsigned long int size = sizeof(double);
	if(!ReadProcessMemory(_handle, (void*)address, buff, size, &size)){
		return 0;
	}
	return *(double*)buff;
}
	
	
/////////////////////// WRITE ///////////////////////
	
unsigned long int ProcessHandle::write(long unsigned int address, string data){
	if(_handle == NULL)
		return 0;
	if(data.size()<=0)
		return 0;
	unsigned long int written = 0;
	WriteProcessMemory(_handle, (void*)address, data.c_str(), data.size(), &written);
	return written;
}
	
bool ProcessHandle::writeInt(long unsigned int address, int data){
	if(_handle == NULL)
		return 0;
	unsigned long int written = 0;
	WriteProcessMemory(_handle, (void*)address, (char*)&data, sizeof(data), &written);
	return written == sizeof(data);
}
	
bool ProcessHandle::writeFloat(long unsigned int address, float data){
	if(_handle == NULL)
		return 0;
	unsigned long int written = 0;
	WriteProcessMemory(_handle, (void*)address, (char*)&data, sizeof(data), &written);
	return written == sizeof(data);
}
	
bool ProcessHandle::writeDouble(long unsigned int address, double data){
	if(_handle == NULL)
		return 0;
	unsigned long int written = 0;
	WriteProcessMemory(_handle, (void*)address, (char*)&data, sizeof(data), &written);
	return written == sizeof(data);
}
	
	
/////////////////////// UTILS ///////////////////////
	
long unsigned int ProcessHandle::getAddress(long unsigned int address, vector<long unsigned int> offsets) const{
	for(long unsigned int offset:offsets){
		address = readInt(address)+offset;
	}
	return address;
}

vector<WindowHandle> ProcessHandle::getWindows() const{
	if(_handle==NULL)
		return vector<WindowHandle>();
	vector<WindowHandle> windows;
	
	long unsigned int pid = getProcessId();
	
	void* data[2];
	data[0] = &windows;
	data[1] = &pid;
	
	EnumWindows(enumWindowsCallback, (LPARAM)data);
	
	return windows;
}
	
BOOL CALLBACK ProcessHandle::enumWindowsCallback(HWND hwnd, LPARAM lparam){
	long unsigned int tpid = 0;
	 GetWindowThreadProcessId(hwnd, &tpid);
	 if(*((int*)((void**)lparam)[1])==tpid)
		 ((vector<WindowHandle>*)((void**)lparam)[0])->push_back(WindowHandle(hwnd));
}