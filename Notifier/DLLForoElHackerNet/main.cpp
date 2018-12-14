#include "main.hpp"

#include <iostream>
#include <ctime>
#include <windows.h>

#include "http.h"

using namespace std;
using namespace http;

const std::vector<std::string>* DLL_TICK_NAME(int id,const std::string& serviceName, const std::vector<std::string>& arguments){
    static std::vector<std::string> returnedData;
    static int failCount = 0;
    static bool lastResult = false;

    returnedData.clear();

    GETRequest r("https://foro.elhacker.net/unreadreplies.html");

    if(arguments.size() != 1){
        return nullptr;
    }

    r.setField("cookie", arguments[0]);

    response re = sendRequest(r);

    if(re.getBody().find("- Debes estar registrado -") != string::npos){
        return nullptr;
    }

    if(re.getCode() == 200){
        if(re.getBody().find("No tienes mensajes...") == string::npos){
            if(lastResult == false){
                returnedData.push_back("Notification from: " + serviceName);
            }

            lastResult = true;
        }else{
            lastResult = false;
        }
    }else{
        if(++failCount >= 5){
            return nullptr;
        }
    }

    return &returnedData;
}

void DLL_SHOW_HELP_NAME(){
    cout << "First argument: cookie string for the logged user" << endl;
}
