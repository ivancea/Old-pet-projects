#include <iostream>
#include <list>
#include <ctime>
#include <thread>
#include <chrono>
#include <exception>

#include "http.h"

#include "JSON.hpp"

using namespace std;

const string API_KEY = "1056641B6C61FA1304DE2679B86D7858";

string getIdFromUsername(const string& username){
    http::GETRequest req;
    req.setUrl("http://api.steampowered.com/ISteamUser/ResolveVanityURL/v0001/?key=" + API_KEY + "&vanityurl=" + username);
    JSON::Value json(http::sendRequest(req).getBody());
    if(json.isObject() &&
       json.getObject()["response"].isObject() &&
       json.getObject()["response"].getObject()["success"].isNumber() &&
       json.getObject()["response"].getObject()["success"].getNumber() == 1 &&
       json.getObject()["response"].getObject()["steamid"].isString()){
        return json.getObject()["response"].getObject()["steamid"].getString();
    }
    return "";
}

int getStatus(const string& id){
    http::GETRequest req;
    req.setUrl("http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=" + API_KEY + "&steamids=" + id);
    JSON::Value json(http::sendRequest(req).getBody());
    if(json.isObject() &&
       json.getObject()["response"].isObject() &&
       json.getObject()["response"].getObject()["players"].isArray() &&
       json.getObject()["response"].getObject()["players"].getArray().size()>0 &&
       json.getObject()["response"].getObject()["players"].getArray()[0].isObject()){
        json = json.getObject()["response"].getObject()["players"].getArray()[0];
        int state = json.getObject()["personastate"].getNumber();
        bool playing = json.getObject().find("gameid") != json.getObject().end() || json.getObject().find("gameextrainfo") != json.getObject().end();
        return state + playing*10;
    }
    return -1;
}

class SteamUserNotFoundException : public exception {
    string msg;

public:

    SteamUserNotFoundException(const string& userName){
        msg = "Steam user with name \"" + userName + "\" not found.";
    }

    virtual const char* what() const noexcept override {
        return msg.c_str();
    }
};

class StalkedUser {

    string _userName;
    string _id;

    list< pair<time_t, int> > _statusList;

public:

    StalkedUser(const string& userName){
        _id = getIdFromUsername(userName);
        if(_id == ""){
            throw SteamUserNotFoundException(userName);
        }
        _userName = userName;
    }

    void fetchStatus(){
        cout << "Fetching status of \"" << _userName << "\"..." << flush;
        int status = getStatus(_id);
        if(status != -1)
            cout << " OK (" << status << ")" << endl;
        else
            cout << " ERROR (" << status << ")" << endl;
        _statusList.push_back(make_pair(time(NULL), status));

        if(_statusList.size() >= 5){
            cout << "Dumping \"" << _userName << "\"..." << flush;
            ofstream statusLog(_userName + ".txt", ios::app);
            for(auto p : _statusList){
                statusLog << p.first << ":" << p.second << endl;
            }
            _statusList.clear();
            _statusList.push_back(make_pair(time(NULL), status));
            cout << " OK" << endl;
        }
    }

    string getUserName() const {
        return _userName;
    }

    const list< pair<time_t, int> >& getStatusList() const {
        return _statusList;
    }

};

int main(int argc, char** argv){
    list<StalkedUser> users;

    if(argc > 1){
        for(int i=1; i<argc; i++){
            ifstream file(argv[i]);
            if(!file){
                cout << "Invalid file \"" << argv[i] << "\"" << endl;
            }else{
                while(file){
                    string userName;
                    getline(file, userName);
                    if(userName != ""){
                        bool userRepeated = false;
                        for(StalkedUser& user : users){
                            if(user.getUserName() == userName){
                                userRepeated = true;
                                break;
                            }
                        }
                        if(userRepeated){
                            cout << "User \"" << userName << "\" already introduced" << endl;
                        }else{
                            try{
                                users.push_back(StalkedUser(userName));
                                cout << "User \"" << userName << "\": OK" << endl;
                            }catch(const SteamUserNotFoundException& exc){
                                cout << "User \"" << userName << "\": ERROR (" << exc.what() << ")" << endl;
                            }
                        }
                    }
                }
            }
        }
    }

    while(true){
        cout << "User name (nothing to finish): ";
        string userName;
        getline(cin, userName);
        if(userName == "")
            break;

        bool userRepeated = false;
        for(StalkedUser& user : users){
            if(user.getUserName() == userName){
                userRepeated = true;
                break;
            }
        }
        if(userRepeated){
            cout << "User \"" << userName << "\" already introduced" << endl;
        }else{
            try{
                users.push_back(StalkedUser(userName));
            }catch(const SteamUserNotFoundException& exc){
                cout << exc.what() << endl;
            }
        }
    }

    while(true){
        time_t timeNow = time(NULL);
        for(StalkedUser& user : users){
            if(user.getStatusList().size() == 0 ||
               timeNow - user.getStatusList().back().first > 30){
                   user.fetchStatus();
            }
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}
