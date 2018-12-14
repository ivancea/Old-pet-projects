#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "http.h"
#include "strings.h"

using namespace std;

struct Bundle{
    string type;
    string name;
    vector<string> basic,
                   average,
                   fixed;
    double averageCost;
};

enum GameCostType{
    Basic,
    Average,
    Fixed
};

bool parseBundle(Bundle& b, http::response r){
    if(r.getCode()!=200)
        return false;
    string body = r.getBody();

    size_t posA = body.find("<title>")+7;
    if(posA == string::npos)
        return false;
    size_t posB = body.find(" (", posA);
    if(posB == string::npos)
        return false;
    b.name = body.substr(posA, posB-posA); /** Set NAME from title **/

    posA = body.find("section game-border", posA);
    if(posA == string::npos)
        return false;

    for(int i=0; i<3; i++){
        posB = body.find("section game-border", posA+1);
        if(posB == string::npos)
            break;
        size_t posTemp = body.find("\"", posA);

        GameCostType costType;
        if(body.substr(posTemp-3,3) == "bta"){
            costType = GameCostType::Average;
        }else if(body.substr(posTemp-5,5) == "fixed"){
            costType = GameCostType::Fixed;
        }else{
            costType = GameCostType::Basic;
        }
        posA = body.find("<div class='game-description'>", posA+1);
        while(posA<posB){
            posTemp = body.find("<h2>", posA)+4;
            switch(costType){
            case Basic:
                b.basic.push_back(body.substr(posTemp, body.find("</h2>", posTemp)-posTemp));
                break;
            case Average:
                b.average.push_back(body.substr(posTemp, body.find("</h2>", posTemp)-posTemp));
                break;
            case Fixed:
                b.fixed.push_back(body.substr(posTemp, body.find("</h2>", posTemp)-posTemp));
                break;
            }
            posA = body.find("<div class='game-description'>", posA+1);
        }
        posA = posB;
    }
    replaceAll(b.name,"&#39;", "'");
    replaceAll(b.name,"&amp;", "&");
    for(string& s:b.basic){
        replaceAll(s,"&#39;", "'");
        replaceAll(s,"&amp;", "&");
    }
    for(string& s:b.average){
        replaceAll(s,"&#39;", "'");
        replaceAll(s,"&amp;", "&");
    }
    for(string& s:b.fixed){
        replaceAll(s,"&#39;", "'");
        replaceAll(s,"&amp;", "&");
    }
}

int main(int args, char** argc){
    http::GETRequest req;
    vector<string> bundlePages = {"",
                                  "weekly",
                                  "books",
                                  "mobile",
                                  "flash"};

    if(args>1){
        bundlePages.clear();
        for(int i=1; i<args; i++)
            if(argc[i][0]=='-' && find(bundlePages.begin(), bundlePages.end(), string(argc[i]+1))==bundlePages.end())
                bundlePages.push_back(string(argc[i]+1));
    }

    vector<Bundle> bundles;
    for(string& page:bundlePages){
        Bundle b;
        if(page=="")
            b.type = "common";
        else
            b.type = page;
        cout << "Bundle " << b.type << "... ";
        req.setUrl("https://www.humblebundle.com/" + page);
        if(parseBundle(b, http::sendRequest(req))){
            bundles.push_back(b);
            cout << "OK";
        }else{
            cout << "FAILED";
        }
        cout << endl;
    }

    ofstream file("bundles.html", ios::trunc);
    file << "<html><head><style>.bundle{border: 5px solid green;background-color: #AAFFAA;}</style></head><body>";
    for(Bundle& b:bundles){
        file << "<div class=\"bundle " + b.type + "\">";
        file << "<h1>" << b.name << "</h1>";
        if(b.basic.size()>0){
            file << "<h3>Basic</h3>" << endl;
            file << "<ul>";
            for(string& s:b.basic)
                file << "<li>" << s << "</li>" << endl;
            file << "</ul>";
        }
        if(b.average.size()>0){
            file << "<h3>Average</h3>" << endl;
            file << "<ul>";
            for(string& s:b.average)
                file << "<li>" << s << "</li>" << endl;
            file << "</ul>";
        }
        if(b.fixed.size()>0){
            file << "<h3>Fixed</h3>" << endl;
            file << "<ul>";
            for(string& s:b.fixed)
                file << "<li>" << s << "</li>" << endl;
            file << "</ul>";
        }
        file << "</div>";
    }
    file << "</body></html>";
    file.close();
}
