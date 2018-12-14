#include "RequestData.h"

void RequestData::clear(){
    ip.clear();
    protocol.clear();
    file.clear();
    headers.clear();
    get.clear();
    post.clear();
    fullRequest.clear();
}
