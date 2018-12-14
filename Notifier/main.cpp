#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <windows.h>

#include "JSON.hpp"
#include "DllData.hpp"
#include "ServiceData.hpp"

using namespace std;

string getHour(){
    time_t t = time(NULL);
    tm tim = *localtime(&t);
    char c[20];
    return string(c, strftime(c, 20, "[%H:%M:%S]", &tim));
}

JSON::Value* containsKeyOfType(JSON::Value& json, const string& key, const JSON::ValueType& type){
    if(json.isObject()){
        auto it = json.getObject().find(key);

        if(it != json.getObject().end()){
            if(it->second.getType() == type){
                return &it->second;
            }
        }
    }

    return nullptr;
}

bool parseArrayOfString(JSON::Value& json, vector<string>& parsedArray){
    parsedArray.clear();

    if(!json.isArray()){
        return false;
    }

    parsedArray.reserve(json.getArray().size());

    for(auto& value : json.getArray()){
        if(!value.isString()){
            return false;
        }

        parsedArray.push_back(value.getString());
    }

    return true;
}

JSON::Value loadConfig(){
    JSON::Value config;
    ifstream configFile{"config.json"};

    if(!configFile){
        cout << "Error opening file \"config.json\"" << endl;

        return config;
    }

    string unparsedJson{
        istreambuf_iterator<char>{configFile},
        istreambuf_iterator<char>{}
    };

    if(config.loadFromString(unparsedJson) == -1){
        cout << "Error parsing JSON" << endl;

        return config;
    }

    if(!config.isObject()){
        cout << "JSON root must be an object" << endl;

        config.setType(JSON::ValueType::Null);
    }

    return config;
}

bool parseService(string serviceName,
                  JSON::Value& json,
                  map<string, DllData>& libraries,
                  map<string, ServiceData>& fetchers,
                  map<string, ServiceData>& notifiers,
                  map<string, ServiceData>& controllers){
    if(!json.isObject()){
        cout << "Service \"" << serviceName << "\" is not an object" << endl;

        return false;
    }

    int type;
    string fileName;
    vector<string> arguments;

    auto it = json.getObject().find("type");

    if(it == json.getObject().end()){
        cout << "Service \"" << serviceName << "\" does not have a type" << endl;

        return false;
    }

    if(!it->second.isNumber() || it->second.getNumber() < 0 || it->second.getNumber() > 2){
        cout << "Service \"" << serviceName << "\" does not have a number between 0 and 2" << endl;

        return false;
    }

    type = it->second.getNumber();

    it = json.getObject().find("filename");

    if(it == json.getObject().end() || !it->second.isString()){
        cout << "Service \"" << serviceName << "\" does not have a correct filename" << endl;

        return false;
    }

    fileName = it->second.getString();

    it = json.getObject().find("arguments");

    if(it != json.getObject().end() && it->second.isArray()){
        if(!parseArrayOfString(it->second, arguments)){
            cout << "Service \"" << serviceName << "\" does not have a correct array of strings as arguments" << endl;

            return false;
        }
    }

    ServiceData serviceData;

    serviceData.type = type;
    serviceData.name = serviceName;
    serviceData.arguments = arguments;

    it = json.getObject().find("delay");

    if(it != json.getObject().end() && it->second.isNumber() && it->second.getNumber() > 0){
        serviceData.delay = it->second.getNumber();
    }else if(type == 0 || type == 2){
        cout << "Service \"" << serviceName << "\" does not have a correct delay" << endl;

        return false;
    }else{
        serviceData.delay = -1;
    }

    if(libraries.count(fileName) == 0){
        auto& library = libraries[fileName];

        library.fileName = fileName;
        serviceData.dll = &library;
    }

    switch(type){
    case 0:
        fetchers[serviceName] = move(serviceData);
        break;

    case 1:
        notifiers[serviceName] = move(serviceData);
        break;

    case 2:
        controllers[serviceName] = move(serviceData);
        break;
    }

    return true;
}

bool parseBinding(JSON::Value& config,
                  map<string, vector<string> >& bindings,
                  map<string, ServiceData>& fetchers,
                  map<string, ServiceData>& notifiers){
    JSON::Value* temp;
    vector<string> from, to;

    if((temp = containsKeyOfType(config, "from", JSON::ValueType::Array)) == nullptr){
        cout << "\"from\" array not found in binding" << endl;

        return false;
    }else{
        if(!parseArrayOfString(*temp, from)){
            cout << "Invalid \"from\" array in binding" << endl;

            return false;
        }
    }

    if((temp = containsKeyOfType(config, "to", JSON::ValueType::Array)) == nullptr){
        cout << "\"to\" array not found in binding" << endl;

        return false;
    }else{
        if(!parseArrayOfString(*temp, to)){
            cout << "Invalid \"to\" array in binding" << endl;

            return false;
        }
    }

    for(auto& fromService : from){
        auto& toServices = bindings[fromService];

        if(fetchers.count(fromService) == 0){
            cout << "No fetch service found with name \"" << fromService << "\"" << endl;

            return false;
        }

        for(auto& toService : to){
            if(notifiers.count(toService) == 0){
                cout << "No notify service found with name \"" << toService << "\"" << endl;

                return false;
            }

            if(find(toServices.begin(), toServices.end(), toService) != toServices.end()){
                cout << "Repeated binding from \"" << fromService << "\" to \"" << toService << "\"" << endl;

                return false;
            }

            toServices.push_back(toService);
        }
    }

    return true;
}

bool parseJSON(JSON::Value& config,
               map<string, DllData>& libraries,
               map<string, ServiceData>& fetchers,
               map<string, ServiceData>& notifiers,
               map<string, ServiceData>& controllers,
               map<string, vector<string> >& bindings){
    JSON::Value* temp;

    if((temp = containsKeyOfType(config, "services", JSON::ValueType::Object)) == nullptr){
        cout << "\"services\" object not found" << endl;

        return false;
    }else{
        for(auto& service : temp->getObject()){
            if(!parseService(service.first, service.second, libraries, fetchers, notifiers, controllers)){
                return false;
            }
        }
    }

    if((temp = containsKeyOfType(config, "bindings", JSON::ValueType::Array)) == nullptr){
        cout << "\"bindings\" array not found" << endl;

        return false;
    }else{
        for(auto& binding : temp->getArray()){
            if(!parseBinding(binding, bindings, fetchers, notifiers)){
                return false;
            }
        }
    }

    return true;
}

bool initializeLibrary(DllData& library){
    library.handle = LoadLibrary(library.fileName.c_str());

    if(library.handle == NULL){
        cout << "Could not open library \"" << library.fileName << "\"" << endl;

        return false;
    }

    library.getVersion = (DllGetVersion)GetProcAddress(library.handle, DLL_GET_VERSION_STR);

    if(library.getVersion == NULL || library.getVersion() != DLL_PROTOCOL_VERSION){
        cout << "Library \"" << library.fileName << "\" has a different protocol version\n"
             << "Library version: " << library.getVersion() << ", Program version: " << DLL_PROTOCOL_VERSION << endl;

        return false;
    }

    library.showHelp = (DllShowHelp)GetProcAddress(library.handle, DLL_SHOW_HELP_STR);
    library.start = (DllStart)GetProcAddress(library.handle, DLL_START_STR);
    library.stop = (DllStop)GetProcAddress(library.handle, DLL_STOP_STR);
    library.tick = (DllTick)GetProcAddress(library.handle, DLL_TICK_STR);
    library.notify = (DllNotify)GetProcAddress(library.handle, DLL_NOTIFY_STR);

    return true;
}

void startEngine(map<string, DllData>& libraries,
                 map<string, ServiceData>& fetchers,
                 map<string, ServiceData>& notifiers,
                 map<string, ServiceData>& controllers,
                 map<string, vector<string> >& bindings){
    multimap<time_t, ServiceData*> serviceQueue;

    for(auto& fetcher : fetchers){
        serviceQueue.insert(make_pair(0, &fetcher.second));
    }

    for(auto& notifier : notifiers){
        if(notifier.second.delay > 0){
            serviceQueue.insert(make_pair(0, &notifier.second));
        }
    }

    while(serviceQueue.size() > 0){
        time_t now = time(NULL);

        while(serviceQueue.size() > 0 && serviceQueue.begin()->first <= now){
            ServiceData* service = serviceQueue.begin()->second;

            cout << getHour() << " Tick on service \"" << service->name << "\"..." << flush;

            serviceQueue.erase(serviceQueue.begin());

            auto fetchedData = service->dll->tick((int)service, service->name, service->arguments);

            if(service->type != 1 && fetchedData != nullptr && fetchedData->size() > 0){
                cout << " Notifying..." << endl;

                for(auto& notifyServiceName : bindings[service->name]){
                    auto& notifyService = notifiers[notifyServiceName];

                    for(auto& fetchedString : *fetchedData){
                        notifyService.dll->notify((int)&notifyService, notifyService.name, notifyService.arguments, fetchedString);
                    }
                }
            }else{
                cout << " Nothing to notify..." << endl;
            }

            if(fetchedData == nullptr){
                cout << "Library \"" << service->dll->fileName << "\" returned error on tick\n - Arguments:";

                for(auto& argument : service->arguments){
                    cout << "\n       - " << argument;
                }

                cout << endl;
            }else{
                serviceQueue.insert(make_pair(now + service->delay, service));
            }
        }

        if(serviceQueue.size() > 0){
            Sleep(serviceQueue.begin()->first - now);
        }
    }
}

int main(int argc, char** argv){
    if(argc >= 2 && strcmp(argv[1], "/help") == 0){
        if(argc == 3){
            DllData dll;

            dll.fileName = argv[2];

            if(initializeLibrary(dll)){
                if(dll.showHelp == nullptr){
                    cout << "This library does not have help" << endl;
                }else{
                    dll.showHelp();
                }
            }
        }else{
            cout << "Usage: \"" << argv[0] << "\" /help <DLL path>" << endl;
        }

        return 0;
    }

    JSON::Value config = loadConfig();

    if(config.isNull()){
        return 1;
    }

    map<string, DllData> libraries;
    map<string, ServiceData> fetchers, notifiers, controllers;
    map<string, vector<string> > bindings;

    if(!parseJSON(config, libraries, fetchers, notifiers, controllers, bindings)){
        return 1;
    }

    for(auto& dllPair : libraries){
        if(!initializeLibrary(dllPair.second)){
            return 1;
        }
    }

    for(auto& binding : bindings){
        auto& fromService = fetchers[binding.first];

        if(fromService.dll->tick == NULL){
            cout << "Library \"" << fromService.dll->fileName << "\" doesn't have the function tick" << endl;

            return 1;
        }

        for(auto& to : binding.second){
            auto& toService = notifiers[to];

            if(toService.dll->notify == NULL){
                cout << "Library \"" << toService.dll->fileName << "\" doesn't have the function notify" << endl;

                return 1;
            }

            if(toService.delay > 0 && toService.dll->tick == NULL){
                cout << "Library \"" << toService.dll->fileName << "\" doesn't have the function tick" << endl;

                return 1;
            }
        }
    }

    if(libraries.size() == 0){
        cout << "No services defined" << endl;

        return 1;
    }

    for(auto& library : libraries){
        if(library.second.start != NULL){
            library.second.start();
        }
    }

    cout << "Started" << endl;

    startEngine(libraries, fetchers, notifiers, controllers, bindings);

    for(auto& library : libraries){
        if(library.second.start != NULL){
            library.second.stop();
        }

        FreeLibrary(library.second.handle);
    }

    cout << "Ended" << endl;

    cin.get();

    return 0;
}

/*


{
    services: {
        elhackernet: {
            type: 0, // (fetcher)
            delay: 60 (every 60 seconds),
            filename: "asd.dll",
            arguments: ["a9d48sa46d5a4s6d5 (session token)"]
        },

        telegramBotToIvancea: {
            type: 1, // (notifier)
            filename: "telegramBot.dll",
            arguments: ["ivancea"]
        },

        telegramBotAdminController: {
            type: 2, // (controller)
            delay: 5,
            filename: "telegramBot.dll",
            arguments: ["-a"]
        }
    },

    bindings: [
        {
            from: ["elhackernet"],
            to: ["telegramBotToIvancea"]
        }
    ],

    controllers: [
        {
            name: "telegramBotAdminController",
            permissions: [0, 1, 2, 3] // to-do
        }
    ]
}

TODO: Add fetchers on-request
*/
