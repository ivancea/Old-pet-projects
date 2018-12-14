#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <Wininet.h>

#include "strings.h"
#include "http.h"
using namespace http;

using namespace std;

struct Question{
    string question;
    vector<string> answers;
    short correctAnswer;

    Question():correctAnswer(-1){}

    void clear(){
        question.clear();
        answers.clear();
        correctAnswer = -1;
    }
};

string serializeQuestion(Question &q){
    string ret;
    size_t size = q.question.size();
    ret.append((char*)&size, sizeof(size));
    ret.append(q.question);

    size = q.answers.size();
    ret.append((char*)&size, sizeof(size));
    for(string &s:q.answers){
        size = s.size();
        ret.append((char*)&size, sizeof(size));
        ret.append(s);
    }
    ret.append((char*)&q.correctAnswer, sizeof(short));
    return ret;
}

bool deserializeQuestion(string str, Question &q){
    if(str.size()<sizeof(size_t)*2+sizeof(short))
        return false;

    string temp;
    size_t pos=0;
    size_t size;

    size = *(size_t*)str.c_str();
    pos += sizeof(size_t);
    if(str.size()<pos+size+sizeof(size_t)+sizeof(short))
        return false;
    q.question = str.substr(pos, size);
    pos += size;

    temp = str.substr(pos, sizeof(size_t));
    size_t elemCount = *(size_t*)temp.c_str();
    pos += sizeof(size_t);
    if(str.size()<pos+sizeof(size_t)*elemCount+sizeof(short))
        return false;
    while(elemCount--){
        temp = str.substr(pos, sizeof(size_t));
        size = *(size_t*)temp.c_str();
        pos += sizeof(size_t);
        if(str.size()<pos+size+sizeof(size_t)*elemCount+sizeof(short))
            return false;
        q.answers.push_back(str.substr(pos, size));
        pos += size;
    }
    if(str.size()<pos+sizeof(short))
        return false;
    temp = str.substr(pos, sizeof(short));
    q.correctAnswer = *(short*)temp.c_str();

    return true;
}

string serializeQuestions(vector<Question> &v){
    string ret;
    size_t size = v.size();
    ret.append((char*)&size, sizeof(size));
    for(Question &q:v){
        string temp = serializeQuestion(q);
        size = temp.size();
        ret.append((char*)&size, sizeof(size));
        ret.append(temp);
    }
    return ret;
}

bool deserializeQuestions(string str, vector<Question> &v){
    if(str.size()<sizeof(size_t))
        return false;

    size_t size;
    string temp;
    size_t elemCount = *(size_t*)str.c_str();
    size_t pos = sizeof(size_t);
    if(str.size()<pos+elemCount*(sizeof(size_t)*3+sizeof(short)))
        return false;
    while(elemCount--){
        temp = str.substr(pos, sizeof(size_t));
        size = *(size_t*)temp.c_str();
        pos += sizeof(size_t);
        if(str.size()<pos+size+elemCount*(sizeof(size_t)*3+sizeof(short)))
            return false;
        Question q;
        if(!deserializeQuestion(str.substr(pos, size), q))
            return false;
        pos += size;
        v.push_back(q);
    }

    return true;
}

void prepare(string &str){
    replaceAll(str, "\n", "");
    replaceAll(str, "\r", "");
    str = trim(str, ".-_ ");
}

void prepareQuestion(Question &q){
    prepare(q.question);
    for(string &s:q.answers)
        prepare(s);
}

void load(vector<Question> &v, string fileName){
    ifstream file(fileName);
    if(!file)
        return;

    Question q;
    q.answers.resize(3);
    string t;
    char phase=0;

    while(file){
        getline(file, t);

        switch(phase){
        case 0:
            if(isdigit(t[0])){
                phase = 1;
                if(t.size()>=3){
                    q.question += t.substr((isdigit(t[1])?3:2));
                }
            }
            break;
        case 1:
            if(t.substr(0,2)=="A)"){
                q.answers[0] += t.substr(2);
                phase=2;
            }else q.question += t;
            break;
        case 2:
            if(t.substr(0,2)=="B)"){
                q.answers[1] += t.substr(2);
                phase=3;
            }else q.answers[0] += t;
            break;
        case 3:
            if(t.substr(0,2)=="C)"){
                q.answers[2] += t.substr(2);
                phase=4;
            }else q.answers[1] += t;
            break;
        case 4:
            if(isdigit(t[0])){
                phase = 1;
                prepareQuestion(q);
                v.push_back(q);
                q.clear();
                q.answers.resize(3);
                if(t.size()>=3){
                    q.question += t.substr((isdigit(t[1])?3:2));
                }
            }else if(t.size()==0 || (!isalpha(t[0]) && t[0]!=' ') || (isalpha(t[0]) && isupper(t[0]))){
                phase = 0;
                prepareQuestion(q);
                v.push_back(q);
                q.clear();
                q.answers.resize(3);
            }else q.answers[2] += t;
            break;
        }
    }
}

void saveChanges(vector<Question> &v, string fileName){
    ofstream file(fileName, ios::binary|ios::trunc);
    file << serializeQuestions(v);
}

void reloadData(vector<Question> &v, string fileName){
    ifstream file(fileName, ios::binary);
    char t[1024];
    string fin;
    while(file){
        file.read(t, 1024);
        fin.append(t, file.gcount());
    }
    deserializeQuestions(fin, v);
}

string canonicalizeUrl(string str){
    char t[1000];
    DWORD n = 1000;
    InternetCanonicalizeUrlA((str+'\0').c_str(), t, &n, 0);
    return string(t,n);
}

void sendToDB(vector<Question> &v, int lesson){
    for(int i=0; i<v.size(); i++){
        POSTRequest re("http://ivancea.hol.es/privado/temp.php");
        string question = canonicalizeUrl(v[i].question),
               ans1 = canonicalizeUrl(v[i].answers[0]),
               ans2 = canonicalizeUrl(v[i].answers[1]),
               ans3 = canonicalizeUrl(v[i].answers[2]);
        re.setBodyField("question", question);
        re.setBodyField("lesson", to_string(lesson));
        re.setBodyField("ans1", ans1);
        re.setBodyField("ans2", ans2);
        re.setBodyField("ans3", ans3);
        re.setBodyField("correct", to_string(v[i].correctAnswer+1));
        response res = sendRequest(re);
        cout << (i+1) << ": " << res.getBody() << endl << endl;
    }
}

int main(){
    setlocale(LC_ALL, "spanish");

    string fileName = "tema6.icf";
    vector<Question> v;

    reloadData(v, fileName);

    int countAnswered = 0;
    for(Question &q:v)
        if(q.correctAnswer!=-1)
            ++countAnswered;

    for(int i=0; i<v.size(); i++){
        if(v[i].correctAnswer!=-1)
            continue;
        cout << countAnswered << '/' << v.size() << " answered questions..." << endl << endl << endl;

        cout << i+1 << ") " << v[i].question << endl << endl;
        for(int j=0; j<v[i].answers.size(); j++)
            cout << (char)('A'+j) << ") " << v[i].answers[j] << endl;

        cout << endl << "Respuesta: ";
        string t;
        getline(cin,t);
        if(t.size()>0){
            switch(tolower(t[0])){
            case 'a':
                v[i].correctAnswer = 0;
                break;
            case 'b':
                v[i].correctAnswer = 1;
                break;
            case 'c':
                v[i].correctAnswer = 2;
                break;
            default:
                v[i].correctAnswer = -1;
            }
            if(v[i].correctAnswer!=-1){
                saveChanges(v, fileName);
                ++countAnswered;
            }
        }
        system("cls");
    }
}
