class ProcessHandle;

#ifndef WINDOWHANDLE_H
#define WINDOWHANDLE_H

#include <string>
#include <windows.h>

#include "processHandle.h"

class WindowHandle{
	
	HWND _hwnd;
	
public:
	
	WindowHandle();
	WindowHandle(HWND hwnd);
	WindowHandle(const WindowHandle& windowHandle);
	
	~WindowHandle();
	
	bool attach(std::string windowTitle);
	
	void detach();
	
	std::string getTitle() const;
	
	HWND getHwnd() const;

	ProcessHandle getProcess() const;
	
	LRESULT sendMessage(unsigned int msg, WPARAM wparam, LPARAM lparam);

};

#endif