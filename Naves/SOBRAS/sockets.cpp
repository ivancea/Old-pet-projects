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

void setBlocking(SOCKET sock, bool blocking){
    u_long block = blocking ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &block);
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

bool TCPClient::send(string msg){
    if(!_connected) return false;
    if(::send(_socket,msg.c_str(),msg.size(),0)==SOCKET_ERROR)
        return false;
    return true;
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

TCPServer::TCPServer():_listener(0),_port(0),_on(0){}

TCPServer::TCPServer(unsigned short port):_listener(0),_port(0),_on(0){
    start(port);
}

TCPServer::~TCPServer(){
    finish();
}

bool TCPServer::start(unsigned short port){

    finish();

    addrinfo *result = NULL, *ptr = NULL, hints;

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

void TCPServer::finish(){
    if(!_on) return;

    for(auto it:_clients)
        closesocket(it.socket);
    _clients.clear();
    closesocket(_listener);
    _listener=0;
    _port=0;
    _blocking = false;
    _on=false;
}

bool TCPServer::newClient(){
    if(!_on) return false;

    _client c;

    SOCKADDR_IN clientInfo = {0};
    int addrsize = sizeof(clientInfo);


    if((c.socket = accept(_listener, (sockaddr*)&clientInfo, &addrsize))==INVALID_SOCKET)
        return false;

    c.ip = string(inet_ntoa(clientInfo.sin_addr));
    ::setBlocking(c.socket, false);
}

void TCPServer::disconnectClient(int clientN){
    if(clientN<0 || clientN>=_clients.size()) return;
    closesocket(_clients[clientN].socket);
    _clients.erase(_clients.begin()+clientN);
}

string TCPServer::recv(int clientN, int maxChars){
    if(clientN<0 || clientN>=_clients.size()) return "";
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

bool TCPServer::send(int clientN, string msg){
    if(clientN<0 || clientN>=_clients.size()) return false;
    if(::send(_clients[clientN].socket,msg.c_str(),msg.size(),0)==SOCKET_ERROR)
        return false;
    return true;
}

bool TCPServer::isOn()const{
    return _on;
}

vector<string>* TCPServer::getData(int clientN){
    if(clientN<0 || clientN>=_clients.size()) return 0;
    return &_clients[clientN].data;
}

string TCPServer::getIp(int clientN)const{
    if(clientN<0 || clientN>=_clients.size()) return "";
    return _clients[clientN].ip;
}

unsigned short TCPServer::getPort()const{
    return _port;
}

void TCPServer::setBlocking(int clientN, bool blocking){
    if(clientN<0 || clientN>=_clients.size()) return;
    ::setBlocking(_clients[clientN].socket, blocking);
}

bool TCPServer::isBlocking(int clientN)const{
    if(clientN<0 || clientN>=_clients.size()) return false;
    return _clients[clientN].blocking;
}

void TCPServer::setBlocking(bool blocking){
    if(!_on) return;
    ::setBlocking(_listener, blocking);
}

bool TCPServer::isBlocking()const{
    return _blocking;
}

size_t TCPServer::getClientCount()const{
    return _clients.size();
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
