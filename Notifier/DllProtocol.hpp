#ifndef DLLPROTOCOL_HPP
#define DLLPROTOCOL_HPP

#include <string>
#include <vector>

const int DLL_PROTOCOL_VERSION = 1;

#define DLL_GET_VERSION_NAME getVersion
#define DLL_GET_VERSION_STR "getVersion"
#define DLL_EXPORT_GET_VERSION int __declspec(dllexport) getVersion(){ return DLL_PROTOCOL_VERSION; }
using DllGetVersion = int(__stdcall *)();

#define DLL_SHOW_HELP_NAME showHelp
#define DLL_SHOW_HELP_STR "showHelp"
#define DLL_EXPORT_SHOW_HELP void __declspec(dllexport) showHelp();
using DllShowHelp = void(__stdcall *)();

#define DLL_START_NAME start
#define DLL_START_STR "start"
#define DLL_EXPORT_START void __declspec(dllexport) start();
using DllStart = void(__stdcall *)();

#define DLL_STOP_NAME stop
#define DLL_STOP_STR "stop"
#define DLL_EXPORT_STOP void __declspec(dllexport) stop();
using DllStop = void(__stdcall *)();

#define DLL_TICK_NAME tick
#define DLL_TICK_STR "tick"
#define DLL_EXPORT_TICK const std::vector<std::string>* __declspec(dllexport) tick(int, const std::string&, const std::vector<std::string>&);
using DllTick = const std::vector<std::string>*(__stdcall *)(int, const std::string&, const std::vector<std::string>&);

#define DLL_NOTIFY_NAME notify
#define DLL_NOTIFY_STR "notify"
#define DLL_EXPORT_NOTIFY void __declspec(dllexport) notify(int, const std::string&, const std::vector<std::string>&, const std::string&);
using DllNotify = void(__stdcall *)(int, const std::string&, const std::vector<std::string>&, const std::string&);

#endif // DLLPROTOCOL_HPP
