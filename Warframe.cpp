#include <iostream>
#include <list>
#include <regex>
#include <ctime>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

#include "http.h"

using namespace std;
using namespace http;

struct Alert{
    string rawData;

    string resource;
    int credits;
    string sector;
    time_t created;
    time_t ending;
    bool isInvasion;

    Alert():credits(0),created(0),ending(0),isInvasion(false){}
};

time_t parsePubDate(string pubDate){
    istringstream ss(pubDate);
    tm t={};
    ss >> get_time(&t, "%a, %d %b %Y %H:%M:%S %z");
    t.tm_hour += 1;
    return mktime(&t);
}

int getSeconds(string s){
    int t = stoi(s);
    if(s[s.size()-1]=='m')
        t *= 60;
    return t;
}

Alert parseAlert(const smatch& sm, string pubDate){
    int n = sm.size();
    for(int i=sm.size()-1; i>=0; i--)
        if(sm[i].str()=="")
            --n;
    if(n<=2)
        throw string("Invalid match: " + sm[0].str());
    Alert a;
    a.rawData = sm[0];
    a.created = parsePubDate(pubDate);
    if(n==3){
        a.isInvasion = true;
        a.resource = trim(sm[1].str(), "\r\n\t ");
        a.sector = trim(sm[2].str(), "\r\n\t ");
    }else{
        if(n==5)
            a.resource = trim(sm[n-4].str(), "\r\n\t ");
        string t = sm[n-3].str().substr(0,sm[n-3].str().size()-2);
        a.credits = stoi(t);
        a.sector = trim(sm[n-2].str(), "\r\n\t ");
        a.ending = a.created + getSeconds(sm[n-1].str());
    }
    return a;
}

int main(){
    int reloadSeconds = 0;
	const regex reg("<title>[ \\t]*(?:([^\\-<]+)[ \\t]*)(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?[ \\t]*<\\/title>");
    const regex pubReg("<pubDate>([^<]*)<\\/pubDate>");

    cout << "Seconds between requests (1 or more, default 20s): ";
    do{
        string t;
        getline(cin, t);
        try{
            reloadSeconds = stoi(t);
        }catch(exception e){
            reloadSeconds = 20;
        }
        if(reloadSeconds<=0)
            cout << "Incorrect number (write an integer greater than 0): ";
    }while(reloadSeconds<=0);

    cout << "Requesting data from server..." << endl;
    while(true){
        list<Alert> alerts;
        GETRequest req;
        req.setUrl("http://content.warframe.com/dynamic/rss.php");
        response r = sendRequest(req);
        string body = r.getBody();

        smatch sm,sm2;
        regex_search(body, sm, reg); // Extract first title tag
        body = sm.suffix().str();

        while(regex_search(body, sm, reg)){
            regex_search(body, sm2, pubReg);
            try{
                if(sm2.size()==2){
                    Alert alert = parseAlert(sm, sm2[1].str());
                    if(alert.isInvasion || alert.ending > time(0))
                        alerts.push_back(alert);
                }
            }catch(string str){
                cout << "::ERROR:" << str << "::" << endl;
                for(int i=0; i<sm.size(); i++){
                    if(sm[i]!="")
                        cout << sm[i] << endl;
                }
                cout << endl;
            }
            body = sm2.suffix().str();
        }

        system("cls");
        for(Alert& a : alerts){
            if(a.isInvasion)
                cout << "Invasion: " << a.resource << endl;
            else{
                cout << "Alert: ";
                if(a.resource!="")
                    cout << a.resource << " - ";
                time_t k = a.ending-time(NULL);
                cout <<"Credits: " << a.credits << "  :: ENDS IN ";
                if(k>60)
                    cout << (k/60) << "m" << endl;
                else
                    cout << "ENDING" << endl;
            }
        }
        cout << endl;
        for(int i=0; i<reloadSeconds; i++){
            cout << '\r' << "Next request in " << (reloadSeconds-i) << " seconds... ";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}
