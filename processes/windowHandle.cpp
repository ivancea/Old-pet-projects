#include <string>
#include <windows.h>

#include "windowhandle.h"

#include "processHandle.h"

using namespace std;

WindowHandle::WindowHandle():_hwnd(NULL){}
WindowHandle::WindowHandle(HWND hwnd):_hwnd(hwnd){}
WindowHandle::WindowHandle(const WindowHandle& windowHandle):_hwnd(windowHandle.getHwnd()){}
	
WindowHandle::~WindowHandle(){
	detach();
}
	
bool WindowHandle::attach(string windowTitle){
	if(_hwnd!=NULL)
		detach();
	_hwnd = FindWindow(NULL, windowTitle.c_str());
	return _hwnd!=NULL;
}
	
void WindowHandle::detach(){
	_hwnd = NULL;
}
	
string WindowHandle::getTitle() const{
	if(_hwnd==NULL)
		return "";
	char buff[256];
	int n = GetWindowText(_hwnd, buff, 256);
	if(n==0)
		return "";
	return string(buff, n);
}
	
HWND WindowHandle::getHwnd() const{
	return _hwnd;
}

ProcessHandle WindowHandle::getProcess() const{
	if(_hwnd==NULL)
		return ProcessHandle();
	
	long unsigned int pid = 0;
	GetWindowThreadProcessId(_hwnd, &pid);
	
	return ProcessHandle(pid);
}
	
LRESULT WindowHandle::sendMessage(unsigned int msg, WPARAM wparam, LPARAM lparam){
	return SendMessage(_hwnd, msg, wparam, lparam);
}