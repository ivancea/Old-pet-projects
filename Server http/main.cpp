#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <mutex>
#include <windows.h>
#include <Wininet.h>

#include "thmanager.h"
#include "strings.h"
#include "sockets.cpp"

#include "shared/protocol.h"

using namespace std;

int TIMEOUT = 2,
    LONG_TIMEOUT = 5;

bool finish=false;
vector<string> ips;

const string E404 = "HTTP/1.1 404 NOT FOUND\r\nconnection: close\r\n\r\n";
const string OK = "HTTP/1.1 200 OK\r\nconnection: close\r\n";
string defaultPage;

bool consoleLog = true,
     fileLog = true;
ofstream loggingFile;

bool DLLAsDynamicContent = false;

void mutexOut(string str){
    static mutex mu;
    static string last;
    if(str!=last){
        mu.lock();
        if(consoleLog)
            cout << str << endl;
        if(fileLog)
            loggingFile << str << endl;
        last = str;
        mu.unlock();
    }
}

string getFullDate(){
    char buff[21];
    time_t tim = time(0);
    strftime(buff, 21, "%d-%m-%y, %H:%M:%S", localtime(&tim));
    return buff;
}

string getTime(){
    char buff[8];
    time_t tim = time(0);
    strftime(buff, 8, "%H:%M", localtime(&tim));
    return buff;
}

inline void thSleep(uint32_t ms){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

bool equalIgnoreCase(const string& a, const string& b){
    if(a.size()!=b.size()) return false;
    for(int i=0; i<a.size(); i++)
        if(tolower(a[i])!=tolower(b[i])) return false;
    return true;
}
/// %C3%A1 %C3%81 %C3%A9 %C3%89 %C3%AD %C3%8D %C3%B3 %C3%93 %C3%BA %C3%9A %C2%BF %C2%A1
/// á Á é É í Í ó Ó ú Ú ¿ ¡

string __htmlChars[24] ={
    "%C3%A1", "á", "%C3%81", "Á",
    "%C3%A9", "é", "%C3%89", "É",
    "%C3%AD", "í", "%C3%8D", "Í",
    "%C3%B3", "ó", "%C3%93", "Ó",
    "%C3%BA", "ú", "%C3%9A", "Ú",
    "%C2%BF", "¿", "%C2%A1", "¡"
};

wstring parseChars(string s){
    wchar_t w[MAX_PATH];
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.size(), w, MAX_PATH);
    return wstring(w, n);
}

string multiByteToChar(string s){
    char b[MAX_PATH];
    int n = wcstombs(b, parseChars(s).c_str(), MAX_PATH);
    return string(b, n);
}

void replaceAll(string& s){
    DWORD n=s.size();
    wstring s2 = parseChars(s);
    wchar_t ch[s.size()];
    if(InternetCanonicalizeUrlW(s2.c_str(), ch, &n, ICU_DECODE| ICU_NO_ENCODE)){
        s2 = wstring(ch, n);
        char b[s2.size()];
        n = wcstombs(b, s2.c_str(), s2.size());
        s = string(b, n);
    }
}

map<string,uint64_t> getList(string dir){
    map<string,uint64_t> t;
    int i=0;
    for(i=dir.size()-1;i>=0; i--)
        if(dir[i]=='/')
            break;
    if(i<0) i=0;
    for(;i<dir.size(); i++)
        if(dir[i]=='.'){
            return t;
        }
    if(dir.size()>=2 && dir[dir.size()-2]!='/' && dir[dir.size()-1]!='*')
        dir+="/*";
    else if(dir.size()>=1 && dir[dir.size()-1]=='/')
        dir+='*';
    else dir+= "/*";
    if(dir=="/*") dir="*";
    HANDLE hFile = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW ffd;
    wstring a = parseChars(dir);
    hFile = FindFirstFileW(a.c_str(), &ffd);
    if(hFile == INVALID_HANDLE_VALUE)
        return t;
    do{
        char b[MAX_PATH];
        wcstombs(b, ffd.cFileName, MAX_PATH);
        if(!strcmp(b,".") || !strcmp(b,".."))
            continue;
        t[b]=-1;
        if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            t[b] = (uint64_t)ffd.nFileSizeHigh*(uint64_t(MAXDWORD)+1)+ffd.nFileSizeLow;
    }while(FindNextFileW(hFile,&ffd));
    if(dir != "*")
        t[".."] = -1;
    FindClose(hFile);
    return t;
}

uint64_t fileSize(ifstream& file){
    streampos t=file.cur;
    uint64_t size=0;
    if(file){
        file.seekg (0, file.end);
        size = file.tellg();
        file.seekg(t-1);
    }
    return size;
}

void parseHeaders(const vector<string>& headers, map<string, string>& parsedHeaders){
    for(const string& str : headers){
        size_t pos = str.find(":");
        parsedHeaders[tolower(trim(str.substr(0, pos)))] = (pos == string::npos? "" : tolower(trim(str.substr(pos+1))));
    }
}

bool recvAndParse(TCPClient* client, RequestData& data){
    data.clear();

    data.ip = client->getIp();
    data.port = client->getPort();

    string buffer;
    string temp;
    size_t pos;
    clock_t startTime=clock();

    do{
        while((temp = client->recv()) == ""){
            if((clock()-startTime)/CLOCKS_PER_SEC > TIMEOUT || !client->isConnected()){
                return false;
            }else thSleep(1);
        }

        if((clock()-startTime)/CLOCKS_PER_SEC > LONG_TIMEOUT || !client->isConnected())
            return false;

        buffer += temp;
        pos = buffer.find("\r\n\r\n");
    }while(pos == string::npos);
    //replaceAll(buffer); // TODO: Rename function

    vector<string> headers = split(buffer.substr(0, pos), "\r\n");
    if(headers.size() == 0)
        return false;

    string request = headers[0];
    headers.erase(headers.begin());
    parseHeaders(headers, data.headers);
    data.fullRequest = buffer + "\r\n\r\n";
    buffer.erase(0, pos + 4);


    // Parse GET and file
    if(request.substr(0,5)!="GET /" && request.substr(0,6)!="POST /")
        return false;

    // GET /foo/index.html HTTP/1.1
    // xxxxx^^^^^^^^^^^^^^xxxxxxxxx
    pos = request.find(' ');
    data.protocol = request.substr(0, pos);
    data.file = request.substr(pos+2, request.find(' ', pos+1) - (pos+2));

    char charBuffer[1024];
    long unsigned int charBufferSize = 1024;
    if(InternetCanonicalizeUrl(data.file.c_str(), charBuffer, &charBufferSize, ICU_DECODE | ICU_NO_ENCODE)){
        charBuffer[charBufferSize] = '\0';
        data.file = charBuffer;
    } else {
        cout << "Error at InternetCanonicalizeUrl: " << GetLastError() << ", Url: " << endl;
    }
    // TODO: Parse GET params

    auto it = data.headers.find("content-length");
    if(it != data.headers.end()){ // POST
        int contentLength = atoi(it->second.data());
        while(buffer.size() < contentLength){
            while((temp = client->recv()) == ""){
                if((clock()-startTime)/CLOCKS_PER_SEC > TIMEOUT || !client->isConnected()){
                    client->disconnect();
                    return false;
                }else thSleep(1);
            }
            if((clock()-startTime)/CLOCKS_PER_SEC > LONG_TIMEOUT || !client->isConnected())
                return false;

            buffer += temp;
        }
        replaceAll(buffer);
        data.fullRequest += buffer;
        // TODO: Parse POST data (buffer)
    }

    return true;
}

bool callDLL(TCPClient* client, const RequestData& data){
    HINSTANCE hDll = LoadLibrary(data.file.c_str());
    if(hDll != NULL){

        VersionGetter versionGetter = (VersionGetter)GetProcAddress(hDll, VERSION_GETTER_STR);
        if(versionGetter == NULL){
            FreeLibrary(hDll);
            return false;
        }
        int dllVersion = versionGetter();
        if(dllVersion != PROTOCOL_VERSION){
            mutexOut("# DLL \""  + data.file +
                     "\" has an incompatible version: " + to_string(dllVersion) +
                     " (Version " + to_string(PROTOCOL_VERSION) + " required)");
            FreeLibrary(hDll);
            return false;
        }

        RequestHandler requestHandler = (RequestHandler)GetProcAddress(hDll, REQUEST_HANDLER_STR);
        if(requestHandler != NULL){
            const char *response = requestHandler(&data);
            int size = strlen(response);
            client->send(OK+"content-length:"+to_string(size)+"\r\n\r\n");
            for(int i=0; i<size; i+=1024)
                client->send(string(response+i, (size-i>1024?1024:size-i)));

            FreeLibrary(hDll);
            return true;
        }

        FreeLibrary(hDll);
    }

    return false;
}

void sender(TCPClient* client){
    RequestData data;
    if(!recvAndParse(client, data)){
        client->disconnect();
        delete client;
        return;
    }

    auto it = data.headers.find("host");
    if(it == data.headers.end()){
        client->disconnect();
        delete client;
        return;
    }
    string host = it->second;

    ifstream f(multiByteToChar(data.file),ios::binary);

    client->setBlocking(true);

    mutexOut("[" + getTime() + "] " + client->getIp() + " requesting: /" + data.file);

    if(!f){
        map<string,uint64_t> files = getList(data.file);
        if(files.size()==0){
            client->send(OK + "content-length:15\r\n\r\nEmpty directory");
            client->disconnect();
            delete client;
            return;
        }
        bool def=false;
        if(defaultPage!="")
            def = files.find(defaultPage)!=files.end();
        if(def){
            if(data.file.size() > 0 && *data.file.rbegin() != '/' && *data.file.rbegin() != '\\')
                data.file += '/';
            data.file += defaultPage;
            f.open(data.file, ios::binary);
            if(!f){
                client->send(E404);
                client->disconnect();
                delete client;
                return;
            }
        }else{
            string toSend;
            string dir = data.file;
            if(dir.size() > 0 && *dir.rbegin() != '/' && *dir.rbegin() != '\\')
                dir += '/';
            for(pair<string,uint64_t> it : files){
                if(it.second==-1)
                    it.first += "/";
                string t = host + "/" + multiByteToChar(dir) + it.first;
                char ch[MAX_PATH];
                DWORD n=t.size();
                if(InternetCanonicalizeUrl(t.c_str(), ch, &n, ICU_ENCODE_PERCENT)){
                    ch[n] = '\0';
                    t = ch;
                }
                if(it.second==-1)
                    toSend+="<a href=\"http://" + t + "\">" +it.first + "</a><br/>";
                else if(it.second>1024*1024*1024)
                    toSend+="<a href=\"http://" + t + "\">" +it.first + "   -   Size: "+ to_string(it.second/(1024*1024*1024)) +" GB</a><br/>";
                else if(it.second>1024*1024)
                    toSend+="<a href=\"http://" + t + "\">" +it.first + "   -   Size: "+ to_string(it.second/(1024*1024)) +" MB</a><br/>";
                else if(it.second>1024)
                    toSend+="<a href=\"http://" + t + "\">" +it.first + "   -   Size: "+ to_string(it.second/1024) +" KB</a><br/>";
                else
                    toSend+="<a href=\"http://" + t + "\">" +it.first + "   -   Size: "+ to_string(it.second) +" bytes</a><br/>";
            }
            client->send(OK + "content-length:" + to_string(toSend.size()) + "\r\n\r\n" + toSend);
            client->disconnect();
            delete client;
            return;
        }
    }
    if(DLLAsDynamicContent && data.file.size()>4 && data.file.substr(data.file.size()-4,4) == ".dll"){
        if(callDLL(client, data)){
            delete client;
            return;
        }
    }
    uint64_t size = fileSize(f);
    client->send(OK + "content-length:" + to_string(size) + "\r\n\r\n");
    char arr[1024];
    while(f && client->isConnected()){
        f.read(arr,1024);
        client->send(string(arr, f.gcount()));
    }

    client->disconnect();
    delete client;
    return;
}

int main(){
    unsigned short port=0;
    do{
        string t;
        cout << "Port: ";
        getline(cin,t);
        port = atoi(t.c_str());
    }while(port==0);

    { /// SET OPTIONS
        string t;
        cout << "Default page: ";
        getline(cin, defaultPage);
        cout << "Don't log (0), log to file only (1), log to file and console (2) or log to console only (default, other): ";
        getline(cin, t);
        if(t.size()==1){
            if(t[0]=='0')
                consoleLog = fileLog = false;
            else if(t[0]=='1')
                consoleLog = !(fileLog = true);
            else if(t[0]=='2')
                consoleLog = fileLog = true;
        }else consoleLog = !(fileLog = false);

        if(fileLog){
            cout << "Log file name (default \"Server http.log\":";
            getline(cin, t);
            if(t=="")
                t = "Server http.log";
            loggingFile.open(t, ios::out | ios::ate | ios::app);
            if(!loggingFile){
                cout << "Error opening/creating logging file..." << endl;
                fileLog = false;
            }else{
                if(loggingFile.tellp()!=0)
                    loggingFile << endl;
                loggingFile << "#" << getFullDate() << "# Server listening to port " << port << endl;
            }
        }
        cout << "Use DLL as dynamic content (Y/other): ";
        getline(cin, t);
        DLLAsDynamicContent = (t.size()==1 && (t[0]=='y'||t[0]=='Y'));
    }

    TCPRawServer server;
    while(!server.start(port));
    cout << "Server listening to port " << port << '.' << endl;
    server.setBlocking(true);

    ThreadManager<TCPClient*> clientThreads(&sender, 20, true),
                             localThreads(&sender, 2, true);
    while(!finish){
        Connection c = server.newClient();
        if(c.sock!=INVALID_SOCKET){
            if(find(ips.begin(), ips.end(), c.ip)==ips.end()){
                ips.push_back(c.ip);
                mutexOut("New client from " + c.ip);
            }
            if(c.ip == "127.0.0.1"){
                localThreads.add(new TCPClient(c.sock, c.ip, server.getPort()));
            }else{
                clientThreads.add(new TCPClient(c.sock, c.ip, server.getPort()));
            }
        }
        Sleep(10);
    }
}
