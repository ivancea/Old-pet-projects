#ifndef SOCKETS_H
#define SOCKETS_H

#include <string>
#include <vector>
#include <windows.h>

using namespace std;

struct connection{
    SOCKET sock;
    string ip;

    connection():sock(INVALID_SOCKET){}
};

string recv(SOCKET s, size_t maxChars=1024);

bool send(SOCKET s, string msg);

void setBlocking(SOCKET sock, bool blocking);

class TCPRawServer{
    SOCKET _listener;
    unsigned short _port;
    bool _blocking;
    bool _on;

public:
    TCPRawServer();
    TCPRawServer(unsigned short port);
    ~TCPRawServer();
    bool start(unsigned short port);
    void finish();
    connection newClient();
    bool isOn()const;
    unsigned short getPort()const;
    void setBlocking(bool blocking);
    bool isBlocking()const;
};

class TCPServer:public TCPRawServer{
    struct _client{
        SOCKET socket;
        string ip;
        bool blocking;
        vector<string> data;

        _client():socket(INVALID_SOCKET),blocking(0){}
    };
    vector<_client> _clients;

public:
    TCPServer();
    TCPServer(unsigned short port);
    ~TCPServer();
    bool newClient();
    void disconnectClient(size_t clientN);
    string recv(size_t clientN, size_t maxChars=1024);
    bool send(size_t clientN, string msg);
    vector<string>* getData(size_t clientN);
    string getIp(size_t clientN)const;
    void setBlocking(size_t clientN, bool blocking);
    bool isBlocking(size_t clientN)const;
    size_t getClientCount()const;

};

class TCPClient{
    SOCKET _socket;
    string _ip;
    unsigned short _port;
    bool _connected;
    bool _blocking;

public:
    TCPClient();
    TCPClient(string ip, unsigned short port);
    ~TCPClient();
    bool connect(string ip, unsigned short port);
    void disconnect();
    string recv(int maxChars=1024);
    bool send(const string& msg);
    bool isConnected()const;
    string getIp()const;
    unsigned short getPort()const;
    void setBlocking(bool blocking);
    bool isBlocking()const;
};

#endif // SOCKETS_H
