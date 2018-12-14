#ifndef QUESTION_H
#define QUESTION_H

#include <iostream>
#include <vector>
#include <list>

using namespace std;

struct question{
    string q;
    vector<string> ans;
    vector<string> tag;

    void clear();

    string serialize()const;
    bool deserialize(string s);
};

struct container{
    list<question> q;

    void add(const question& qu);
    void add(const container& c);

    void clear();

    container selectByTag(string tag)const;
    container selectByTag(const vector<string>& tags)const;

    string serialize()const;
    bool deserialize(string s);
};

#endif // QUESTION_H
