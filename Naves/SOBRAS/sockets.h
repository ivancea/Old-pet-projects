#ifndef SOCKETS_H
#define SOCKETS_H

#include <string>
#include <vector>
#include <windows.h>

using namespace std;

class TCPServer{
    struct _client{
        SOCKET socket;
        string ip;
        bool blocking;
        vector<string> data;

        _client():socket(INVALID_SOCKET),blocking(0){}
    };
    SOCKET _listener;
    vector<_client> _clients;
    unsigned short _port;
    bool _blocking;
    bool _on;

public:
    TCPServer();
    TCPServer(unsigned short port);
    ~TCPServer();
    bool start(unsigned short port);
    void finish();
    bool newClient();
    void disconnectClient(int clientN);
    string recv(int clientN, int maxChars=1024);
    bool send(int clientN, string msg);
    bool isOn()const;
    vector<string>* getData(int clientN);
    string getIp(int clientN)const;
    unsigned short getPort()const;
    void setBlocking(int clientN, bool blocking);
    bool isBlocking(int clientN)const;
    void setBlocking(bool blocking);
    bool isBlocking()const;
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
    bool send(string msg);
    bool isConnected()const;
    string getIp()const;
    unsigned short getPort()const;
    void setBlocking(bool blocking);
    bool isBlocking()const;
};

#endif // SOCKETS_H
