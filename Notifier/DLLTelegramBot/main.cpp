#include "main.hpp"

#include <iostream>
#include <map>
#include <list>
#include <thread>
#include <chrono>

#include "../JSON.hpp"

#include "http.h"

using namespace std;

const string url = "https://api.telegram.org/bot";
const string token = "423444449:AAEcGN8Zc3izpez2rwq76hOZlurppsLJmTY";


http::response makeRequest(const string& method, const map<string,string>& params){
	http::RawPOSTRequest req(url + token + "/" + method);
	req.setField("content-type", "application/json");
	JSON::Value temp;
	temp.setType(JSON::Object);
	for(auto it:params){
        JSON::Value t;
        if(t.loadFromString(it.second) == -1)
            t = '"' + it.second + '"';
        temp.getObject()[it.first] = t;
	}
	req.setBody(temp.toRawString());
	return sendRequest(req);
}

bool sendMessage(const string& chatId, const string& text, const JSON::Value* replyMarkup = nullptr){
    map<string,string> params;
    params["chat_id"] = chatId;
    params["text"] = text;
    if(replyMarkup != nullptr){
        params["reply_markup"] = replyMarkup->toRawString();
    }
    http::response resp = makeRequest("sendMessage", params);
    JSON::Value t;
    t.loadFromString(resp.getBody());
    return t.isObject() && t.getObject()["ok"].isTrue();
}

void DLL_NOTIFY_NAME(int id, const std::string& serviceName, const std::vector<std::string>& arguments, const std::string& message){
    if(arguments.size() == 1){
        sendMessage(arguments[0], message);
    }
}

void DLL_SHOW_HELP_NAME(){
    cout << "First argument: user or group ID to notify" << endl;
}
