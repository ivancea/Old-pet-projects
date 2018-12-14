#include "C:\includes\ivancea.h"

string ip = "ivancea.hol.es";

string urlProxy(string &msg){
    int pos = 0;
    string t;
    if(contains(msg, "GET http://", 0)){
        for(int i=11; i<msg.size(); i++)
            if(msg[i]=='/'){
                pos = i;
                break;
            }
        if(pos==0) return "";
        t = msg.substr(11, pos-11);
        msg.erase(4, pos-4);
        return t;
    }
    return "";
}

void modificarLocation(string &msg, string cambio){
    string *temp, t, body;
    if(contains(msg, "\r\n\r\n")){
        int n = split(&temp, msg, "\r\n\r\n");
        if(n>=2)
            for(int i=1; i<n; i++)
                body += temp[i];
        n = split(&temp, temp[0], '\n');
        for(int i=0; i<n; i++){
            if(contains(temp[i], "Location: ", 0)){
                t = temp[i].substr(10, temp[i].size());
                temp[i].erase(10, temp[i].size());
                while(t.size()>0 && contains(t, ".") && !contains(t, ":")){
                    t.erase(0, 1);
                    if(t[0]=='/')
                        break;
                }
                temp[i] += cambio + t;
                break;
            }else
            if(contains(temp[i], "Location:", 0)){
                t = temp[i].substr(10, temp[i].size());
                temp[i].erase(9, temp[i].size());
                while(t.size()>0 && contains(t, ".") && !contains(t, ":")){
                    t.erase(0, 1);
                    if(t[0]=='/')
                        break;
                }
                temp[i] += cambio + t;
                break;
            }
        }
        msg = "";
        for(int i=0; i<n; i++)
            msg += temp[i] + "\n";
        msg += "\n" + body;
    }
}
string leerCabecera(string msg, string buscar){
    string *temp, t;
    int i=0;
    int n = split(&temp, msg, '\n');
    for(i=0; i<n; i++){
        if(contains(temp[i], buscar + ": ", 0)){
            t = temp[i].substr(buscar.size()+2, temp[i].size());
            break;
        }else if(contains(temp[i], buscar + ":", 0)){
            t = temp[i].substr(buscar.size()+1, temp[i].size());
            break;
        }
    }
    if(i==n) return "";
    if(t[t.size()-1]=='\r') t.erase(t.size()-1, 1);
    return t;
}

void modificarCabecera(string &msg, string buscar, string cambio){
    string *temp, body;
    if(contains(msg, "\r\n\r\n")){
        int n = split(&temp, msg, "\r\n\r\n");
        if(n>=2)
            for(int i=1; i<n; i++)
                body += temp[i];
        n = split(&temp, temp[0], '\n');
        for(int i=0; i<n; i++){
            if(contains(temp[i], buscar + ": ", 0)){
                temp[i].erase(buscar.size()+2, temp[i].size());
                temp[i] += cambio;
                break;
            }else
            if(contains(temp[i], buscar + ":", 0)){
                temp[i].erase(buscar.size()+1, temp[i].size());
                temp[i] += cambio;
                break;
            }
        }
        msg = "";
        for(int i=0; i<n; i++)
            msg += temp[i] + "\n";
        msg += "\n" + body;
    }
}

void modificar(string &msg){
    string *temp;
    int n = split(&temp, msg, '\n');
    for(int i=0; i<n; i++){
        if(contains(temp[i], "Host: ", 0)){
            temp[i].erase(6, temp[i].size());
            temp[i] += ip;
            break;
        }else
        if(contains(temp[i], "Host:", 0)){
            temp[i].erase(5, temp[i].size());
            temp[i] += ip;
            break;
        }
    }
    msg = "";
    for(int i=0; i<n; i++)
        msg += temp[i] + "\n";
    msg += "\n";
}

int main(){
    string p;
    server s;
    vector<client*> c;
    for(int i=27015; !s.start(i); i++);
    cout << "Servidor establecido en el puerto " << s.getPort() << "." << endl;
    while(true){
        if(s.newClient(1)){
            cout << "Nuevo cliente con IP: " << s.getIp(s.getClients()-1) << endl;
            c.push_back(new client());
        }
        for(int cl=0; cl<s.getClients(); cl++){
            thSleep(5);
            if((p = s.recvChars(cl, 100000))!=""){
                cout << p.size() <<" bytes recibidos." << endl;
                if(contains(p, "GET ", 0)){
                    c[cl]->start(urlProxy(p), 80);
                }
                c[cl]->sendChars(p);
            }
            if((p = c[cl]->recvChars(100000))!=""){
                cout << p.size() <<" bytes recibidos." << endl;
                replaceAll(p, "Multijugador", "<a href=\"http://ivancea.hol.es\">CLICK</a>");
                s.sendChars(p, cl);
            }
        }
        thSleep(1);
    }
    return 0;
}
