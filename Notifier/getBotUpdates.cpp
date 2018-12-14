#include <iostream>
#include <map>
#include <list>
#include <thread>
#include <chrono>

#include "JSON.cpp"

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

JSON::Value makeReplyMarkup(const vector< vector<string> >& buttonRows){
    JSON::Value json;
    json.setType(JSON::Object);
    JSON::Value& arrays = json.getObject()["keyboard"];
    arrays.setType(JSON::Array);
    vector<JSON::Value>& rows = arrays.getArray();
    rows.resize(buttonRows.size());
    for(int i=0; i<rows.size(); i++){
        JSON::Value& arr = rows[i];
        arr.setType(JSON::Array);
        arr.getArray().resize(buttonRows[i].size());
        for(int j=0; j<buttonRows[i].size(); j++){
            JSON::Value& obj = arr.getArray()[j];
            obj.setType(JSON::Object);
            obj.getObject()["text"].setType(JSON::String).getString() = buttonRows[i][j];
        }
    }
    return json;
}

map<long long, long long> users;
long long unpairedUser = -1;

const JSON::Value commandsMarkup = makeReplyMarkup({{"/join","/leave","/stats"}});

const string welcomeMessage = "Welcome to the chat bot. You will be paired with random people."
                              "\nThe messages sent by the other guy will be preceeded by \"> \"."
                              "\nSend /join search a/another mate."
                              "\nSend /leave for end the chat session."
                              "\nSend /stats for see bot user stats."
                              "\nRemember: only text messages."
                              "\nHave fun!";
const string newPairMessage = "Starting a chat session. Have fun! (Commands: /join or /leave)";
const string pairLeftMessage = "Your pair left the session. /join for find another pair.";
const string waitingMessage = "Wait while we find a pair for you";
const string exitMessage = "You left the session. /join for find another pair.";
const string exitNoSessionMessage = "You aren't in a session. /join for find a pair.";

list<time_t> lastMessages;

void parseUpdate(JSON::Value json){
    cout << "UPDATE" << endl;
    cout << json.toString() << endl << endl;
    if(!json.isObject())
        return;
    auto t = json.getObject().find("message");
    if(t == json.getObject().end())
        return;
    auto t2 = t->second.getObject().find("text");
    if(t2 == t->second.getObject().end() || !t2->second.isString())
        return;
    string text = t2->second.toString();
    text.erase(text.size()-1);
    text.erase(0,1);

    t2 = t->second.getObject().find("from");
    if(t2 == t->second.getObject().end())
        return;
    t = t2->second.getObject().find("id");
    if(t == t2->second.getObject().end() || !t->second.isNumber())
        return;

    time_t now = time(NULL);
    for(auto it=lastMessages.begin(); it!=lastMessages.end() && (*it)+3600 < now;)
        it = lastMessages.erase(it);

    long long id = t->second.getInteger();
    auto it = users.find(id);
    if(text == "/start"){
        sendMessage(to_string(id), welcomeMessage, &commandsMarkup);
    }else if(text == "/leave"){
        if(unpairedUser == id){
            unpairedUser = -1;
            sendMessage(to_string(id), exitMessage);
        }else if(it==users.end()){
            sendMessage(to_string(id), exitNoSessionMessage);
        }else if(unpairedUser == id){
            unpairedUser = -1;
            sendMessage(to_string(id), exitMessage);
        }else{
            sendMessage(to_string(it->second), pairLeftMessage);
            long long temp = it->second;
            users.erase(it);
            it = users.find(temp);
            if(it != users.end())
                users.erase(it);
            sendMessage(to_string(id), exitMessage);
        }
    }else if(text == "/join"){
        if(it!=users.end()){
            sendMessage(to_string(it->second), pairLeftMessage);
            long long temp = it->second;
            users.erase(it);
            it = users.find(temp);
            if(it != users.end())
                users.erase(it);
        }
        if(unpairedUser == -1){
            unpairedUser = id;
            sendMessage(to_string(id), waitingMessage);
        }else if(unpairedUser != id){
            sendMessage(to_string(id), newPairMessage);
            sendMessage(to_string(unpairedUser), newPairMessage);
            users[id] = unpairedUser;
            users[unpairedUser] = id;
            unpairedUser = -1;
        }
    }else if(text == "/stats"){
        sendMessage(to_string(id), "Users paired: " + to_string(users.size())
                                 + "\nMessages sent last hour: " + to_string(lastMessages.size())
                                 + (unpairedUser==-1?"\nNo people waiting now"
                                                    :"\nOne person waiting for a mate" + string(unpairedUser==id?" (You)":"")));
    }else{
        if(it!=users.end()){
            sendMessage(to_string(it->second), "> " + text);
            lastMessages.push_back(now);
        }
    }

}

unsigned long long lastId = 0;

int main(){

    http::response resp = makeRequest("getUpdates", {{"offset",to_string(lastId+1)}});

    JSON::Value json;
    int res = json.loadFromString(resp.getBody());
    if(res==-1 || json.getType()!=JSON::Object){
        cout << "Error parsing JSON" << endl;
    }else{
        cout << json.toString() << endl;
    }
}
