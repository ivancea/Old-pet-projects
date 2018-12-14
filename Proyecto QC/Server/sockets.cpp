#include "sockets.h"

#include <cstring>
#include <vector>
#include <ws2tcpip.h>

using namespace std;

unsigned int resolveAddress(string addr){
    if(addr=="255.255.255.255")
        return INADDR_BROADCAST;
    unsigned int ip=0;
    if((ip=inet_addr(addr.c_str()))!=INADDR_NONE)
        return ip;
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    addrinfo* result = NULL;
    if (getaddrinfo(addr.c_str(), NULL, &hints, &result) == 0)
        if (result){
            ip = ((sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
            freeaddrinfo(result);
            return ip;
        }
    return 0;
}

string recv(SOCKET s, size_t maxChars){
    if(!s==INVALID_SOCKET) return "";
    char buff[maxChars];
    int n = recv(s,buff,maxChars,0);
    if(n<=0)
        return "";
    string t(n,0);
    for(int i=0; i<n; i++)
        t[i]=buff[i];
    return t;
}

bool send(SOCKET s, string msg){
    if(s==INVALID_SOCKET) return false;
    size_t sentBytes = 0;
    while(sentBytes<msg.size()){
        size_t sentNow = send(s,msg.c_str()+sentBytes,msg.size()-sentBytes,0);
        if(sentNow==SOCKET_ERROR) return false;
        sentBytes+=sentNow;
    }
    return true;
}

void setBlocking(SOCKET sock, bool blocking){
    u_long block = blocking ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &block);
}

string getIp(SOCKET s){
    sockaddr sa;
    int n = sizeof(sa);
    if(getpeername(s,&sa,&n)!=SOCKET_ERROR){
        return sa.sa_data;
    }
}

/**   TCPCLIENT   **/
/*
    SOCKET _socket;
    string _ip;
    unsigned short _port;
    bool _connected;
    bool _blocking;
*/

TCPClient::TCPClient():_socket(0),_port(0),_connected(0),_blocking(0){}

TCPClient::TCPClient(string ip, unsigned short port):_socket(0),_port(0),_connected(0),_blocking(0){
    connect(ip,port);
}

TCPClient::~TCPClient(){
    disconnect();
}

bool TCPClient::connect(string ip, unsigned short port){

    disconnect();

    SOCKET sock = INVALID_SOCKET;
    sockaddr_in sockInfo;

    if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET){
        return false;
    }

    sockInfo.sin_family = AF_INET;
    sockInfo.sin_addr.s_addr = resolveAddress(ip);
    sockInfo.sin_port=htons(port);

    if(::connect(sock,(SOCKADDR*)&sockInfo,sizeof(sockInfo))==SOCKET_ERROR){
        closesocket(sock);
        return false;
    }

    _ip = ip;
    _port = port;
    _socket = sock;
    _connected=true;
    setBlocking(false);
    _blocking = false;

    return true;
}

void TCPClient::disconnect(){
    if(!_connected) return;
    closesocket(_socket);
    _ip.clear();
    _port = 0;
    _socket = 0;
    _connected=false;
    _blocking = false;
}

string TCPClient::recv(int maxChars){
    if(!_connected) return "";
    char buff[maxChars];
    int n = ::recv(_socket,buff,maxChars,0);
    if(n==0){
        disconnect();
        return "";
    }else if(n<0) return "";
    string t(n,0);
    for(int i=0; i<n; i++)
        t[i]=buff[i];
    return t;
}

bool TCPClient::send(const string& msg){
    if(!_connected) return false;
    return ::send(_socket, msg);
}

bool TCPClient::isConnected()const{
    return _connected;
}

string TCPClient::getIp()const{
    return _ip;
}

unsigned short TCPClient::getPort()const{
    return _port;
}

void TCPClient::setBlocking(bool blocking){
    if(!_connected) return;
    ::setBlocking(_socket, blocking);
}

bool TCPClient::isBlocking()const{
    return _blocking;
}



/**   TCPServer   **/
/*
    struct _client{
        SOCKET socket;
        string ip;
        bool blocking;
        vector<string> data;
    };
    SOCKET _listener;
    vector<_client> _clients;
    unsigned short _port;
    bool _on;
*/

TCPServer::TCPServer():TCPRawServer(){}

TCPServer::TCPServer(unsigned short port):TCPRawServer(port){
}

TCPServer::~TCPServer(){
    finish();
}

bool TCPServer::newClient(){
    if(!isOn()) return false;

    _client c;

    connection t;
    if((t = TCPRawServer::newClient()).sock==INVALID_SOCKET)
        return false;

    c.socket = t.sock;
    c.ip = t.ip;
    ::setBlocking(c.socket, false);
    _clients.push_back(c);
    return true;
}

void TCPServer::disconnectClient(size_t clientN){
    if(clientN<0 || clientN>=_clients.size()) return;
    closesocket(_clients[clientN].socket);
    _clients.erase(_clients.begin()+clientN);
}

string TCPServer::recv(size_t clientN, size_t maxChars){
    if(clientN>=_clients.size()) return "";
    char buff[maxChars];
    int n = ::recv(_clients[clientN].socket,buff,maxChars,0);
    if(n==0){
        disconnectClient(clientN);
        return "";
    }else if(n<0) return "";
    string t(n,0);
    for(int i=0; i<n; i++)
        t[i]=buff[i];
    return t;
}

bool TCPServer::send(size_t clientN, string msg){
    if(clientN<0 || clientN>=_clients.size()) return false;
    return ::send(_clients[clientN].socket, msg);
}

vector<string>* TCPServer::getData(size_t clientN){
    if(clientN<0 || clientN>=_clients.size()) return 0;
    return &_clients[clientN].data;
}

string TCPServer::getIp(size_t clientN)const{
    if(clientN<0 || clientN>=_clients.size()) return "";
    return _clients[clientN].ip;
}

void TCPServer::setBlocking(size_t clientN, bool blocking){
    if(clientN<0 || clientN>=_clients.size()) return;
    ::setBlocking(_clients[clientN].socket, blocking);
}

bool TCPServer::isBlocking(size_t clientN)const{
    if(clientN<0 || clientN>=_clients.size()) return false;
    return _clients[clientN].blocking;
}

size_t TCPServer::getClientCount()const{
    return _clients.size();
}



/**SOCKET _listener;

unsigned short _port;

bool _blocking;

bool _on;**/

TCPRawServer::TCPRawServer():_listener(0),_port(0),_on(0){
}

TCPRawServer::TCPRawServer(unsigned short port):_listener(0),_port(0),_on(0){
    start(port);
}

TCPRawServer::~TCPRawServer(){
    finish();
}

bool TCPRawServer::start(unsigned short port){
    finish();

    addrinfo *result = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, to_string(port).c_str(), &hints, &result)!=0)
        return false;

    SOCKET sock = INVALID_SOCKET;

    if((sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol))==INVALID_SOCKET){
        freeaddrinfo(result);
        return false;
    }

    if(bind(sock, result->ai_addr, result->ai_addrlen)==SOCKET_ERROR){
        freeaddrinfo(result);
        closesocket(sock);
        return false;
    }

    freeaddrinfo(result);

    if(listen(sock,SOMAXCONN)==SOCKET_ERROR){
        closesocket(sock);
        return false;
    }

    _listener = sock;
    _port = port;
    _on=true;
    return true;
}

void TCPRawServer::finish(){
    if(!_on) return;

    closesocket(_listener);
    _listener=0;
    _port=0;
    _blocking = false;
    _on=false;
}

connection TCPRawServer::newClient(){
    connection t;
    if(!_on) return t;

    SOCKADDR_IN clientInfo = {0};
    int addrsize = sizeof(clientInfo);
    if((t.sock=accept(_listener, (sockaddr*)&clientInfo, &addrsize))==INVALID_SOCKET)
        return t;

    t.ip = inet_ntoa(clientInfo.sin_addr);
    return t;
}

bool TCPRawServer::isOn()const{
    return _on;
}

unsigned short TCPRawServer::getPort()const{
    return _port;
}

void TCPRawServer::setBlocking(bool blocking){
    if(!_on) return;
    ::setBlocking(_listener,blocking);
    _blocking = blocking;
}

bool TCPRawServer::isBlocking()const{
    return _blocking;
}



struct WSAInitializer{
    WSAInitializer(){
        WSAData wsa;
        WSAStartup(MAKEWORD(2,2),&wsa);
    };
    ~WSAInitializer(){
        WSACleanup();
    }
}WSAIni;
