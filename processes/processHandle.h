class WindowHandle;

#ifndef PROCESSHANDLE_H
#define PROCESSHANDLE_H

#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#include "windowhandle.h"

using namespace std;

class ProcessHandle{
	
	HANDLE _handle;
	
public:
	
	ProcessHandle();
	ProcessHandle(HANDLE handle);
	ProcessHandle(long unsigned int pid);
	ProcessHandle(const ProcessHandle& processHandle);
	
	~ProcessHandle();
	
	bool terminate();
	
	bool attach(long unsigned int pid);
	
	bool attach(std::string processName);
	
	void detach();
	
	long unsigned int getProcessId() const;
	
	std::string getProcessName() const;
	
	std::string getExecutable() const;
	
	bool isAttached() const;
	
	HANDLE getHandle() const;
	
	
	/////////////////////// READ ///////////////////////
	
	std::string read(long unsigned int address, unsigned long int size) const;
	
	char readChar(long unsigned int address) const;
	
	int readInt(long unsigned int address) const;
	
	float readFloat(long unsigned int address) const;
	
	double readDouble(long unsigned int address) const;
	
	
	/////////////////////// WRITE ///////////////////////
	
	unsigned long int write(long unsigned int address, std::string data);
	
	bool writeInt(long unsigned int address, int data);
	
	bool writeFloat(long unsigned int address, float data);
	
	bool writeDouble(long unsigned int address, double data);
	
	
	/////////////////////// UTILS ///////////////////////
	
	long unsigned int getAddress(long unsigned int address, std::vector<long unsigned int> offsets) const;

	std::vector<WindowHandle> getWindows() const;
	
private:
	
	static BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lparam);
	
};

#endif