#include "main.h"
#include <iostream>
#include <list>
#include <regex>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "http.h"

using namespace std;
using namespace http;

string retBody;

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
    int t = atoi(s.c_str());
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
        a.credits = atoi(t.c_str());
        a.sector = trim(sm[n-2].str(), "\r\n\t ");
        a.ending = a.created + getSeconds(sm[n-1].str());
    }
    return a;
}

const char* REQUEST_HANDLER_NAME(const RequestData* data){
	const regex reg("<title>[ \\t]*(?:([^\\-<]+)[ \\t]*)(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?(?:[ \\t]*\\-[ \\t]*([^\\-<]+))?[ \\t]*<\\/title>");
    const regex pubReg("<pubDate>([^<]*)<\\/pubDate>");


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

    retBody = "<html><head><style>table,td,tr,th{border:1px solid black;border-collapse: collapse;padding: 4px;}</style></head>"
              "<body><h1>Warframe: Alerts and invasions</h1><table>";
    retBody += "<tr><th>Type</th><th>Resource</th><th>Credits</th><th>Ending in...</th></tr>";
    for(Alert& a : alerts){
        retBody += "<tr>";
        if(a.isInvasion){
            retBody += "<td>INVASION</td><td>" + a.resource + "</td><td></td><td></td>";
        }else{
            time_t k = a.ending-time(NULL);
            retBody += "<td>ALERT</td><td>" + a.resource + "</td><td>" + to_string(a.credits) + "</td><td>" + (k>60?to_string(k/60)+" minutes":"ENDING") + "</td>";
        }
        retBody += "</tr>";
    }
    retBody += "</table></body></html>";
    return retBody.c_str();
}

/*extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}*/
