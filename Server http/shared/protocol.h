#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "RequestData.h"

#define VERSION_GETTER_NAME getVersion
#define VERSION_GETTER_STR "getVersion"

#define REQUEST_HANDLER_NAME parseRequest
#define REQUEST_HANDLER_STR "parseRequest"

const int PROTOCOL_VERSION = 1;

using VersionGetter = int(__stdcall *)();
using RequestHandler = const char*(__stdcall *)(const RequestData*);


#define EXPORT_VERSION_GETTER int __declspec(dllexport) getVersion(){ return PROTOCOL_VERSION; }
#define EXPORT_REQUEST_HANDLER const char* __declspec(dllexport) parseRequest(const RequestData*);

#endif // PROTOCOL_HPP
