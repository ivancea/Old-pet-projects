#include <iostream>
#include <vector>

#include "sockets.h"
#include "question.h"

#define TIMEOUT 1000
#define FOLDER_QUESTS "quests/"

using namespace std;

int main(){
    container c;
    question q;
    for(int i=0; i<4; i++){
        q.clear();
        q.q = "Pregunta numero " + to_string(i);
        q.ans.push_back("Respuesta 1");
        q.ans.push_back("Respuesta 2");
        q.tag.push_back("programacion");
        q.tag.push_back("hacking");
        c.add(q);
    }

    TCPClient cl;
    while(!cl.connect("192.168.1.135",12368));
    cl.setBlocking(true);
    string ser = c.serialize();
    uint32_t size = ser.size();
    cl.send(string((char*)(void*)&size,4) + ser);
    cout << (int)cl.recv()[0] << endl;
    system("pause");
}
