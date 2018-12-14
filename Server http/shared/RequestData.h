#ifndef REQUESTDATA_H
#define REQUESTDATA_H

#include <map>
#include <vector>
#include <string>

class RequestData{
public:
    std::string ip;
    unsigned short port;
    std::string protocol;
    std::string file;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::vector<std::string> > get;
    std::map<std::string, std::vector<std::string> > post;

    std::string fullRequest;

    void clear();
};

#endif // REQUESTDATA_H
