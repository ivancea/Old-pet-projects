#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include "strings.h"

#include "question.h"

using namespace std;


void question::clear(){
    q.clear();
    ans.clear();
    tag.clear();
}

string question::serialize()const{
    string t = q + '\0';
    for(auto it:ans)
        t += it + '\0';
    if(tag.size()==0) return t+'\0';
    for(auto it:tag)
        t += '\0' + it;
    return t;
}

bool question::deserialize(string s){
    string _q;

    size_t a=0, b=0;

    b = s.find('\0');
    if(b==s.npos) return false;
    _q = s.substr(0,(a=++b)-1);

    b = s.find("\0\0", b);
    q=_q;
    ans = split(s.substr(a,b-a), '\0');
    if(b==s.npos){
        tag.clear();
        return true;
    }
    tag = split(s.substr(b+2, s.size()), '\0');
    return true;
}

void container::add(const question& qu){
    q.push_back(qu);
}

void container::add(const container& c){
    q.insert(q.begin(),c.q.begin(),c.q.end());
}

void container::clear(){
    q.clear();
}

container container::selectByTag(string tag)const{
    container t;
    for(auto it:q)
        if(find(it.tag.begin(),it.tag.end(),tag) != it.tag.end())
            t.add(it);
}

container container::selectByTag(const vector<string>& tags)const{
    container t;
    for(auto it:q){
        bool detect = false;
        for(auto tg:tags)
            if(find(it.tag.begin(),it.tag.end(),tg)!=it.tag.end()){
                detect=true;
                break;
            }
        if(detect)
            t.add(it);
    }
}

string container::serialize()const{
    string t;
    for(auto it:q){
        string ser = it.serialize();
        unsigned short n = ser.size();
        t+=string((const char*)(void*)&n, 2) + ser;
    }
    return t;
}

bool container::deserialize(string s){
    list<question> t;
    while(s.size()){
        if(s.size()<2) return false;
        unsigned short tam = s.c_str()[0]+s.c_str()[1]*256;
        if(s.size()<2+tam) return false;
        question temp;
        if(!temp.deserialize(s.substr(2,tam))) return false;
        t.push_back(temp);
        s.erase(0,2+tam);
    }
    q = t;
    return true;
}
