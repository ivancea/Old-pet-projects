#include "C:\includes\ivancea.h"

string ip = "ivancea.hol.es";

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
    cout << "-IP/DNS para el proxy: ";
    cin >> ip;
    cout << endl;
    if(contains(ip, "http://", 0)) ip.erase(0, 7);
    else if(contains(ip, "https://", 0)) ip.erase(0, 8);
    if(!contains(ip, "www.", 0))
        ip = "www." + ip;
    cout << "Conectado a la web " << ip << "." << endl;
    while(true){
        if(s.newClient(1)){
            cout << "Nuevo cliente con IP: " << s.getIp(s.getClients()-1) << endl;
            c.push_back(new client(ip, 80));
        }
        for(int cl=0; cl<s.getClients(); cl++){
            thSleep(20);
            if((p = s.recvChars(cl, 100000))!=""){
                cout << p << endl;
                cout << "A: " << p.size() << endl;
                if(contains(p, "GET ", 0)) modificarCabecera(p, "Host", ip);
                c[cl]->sendChars(p);
            }
            thSleep(50);
            if((p = c[cl]->recvChars(100000))!=""){
                cout << "B: " << p.size() << endl;
                if(contains(p, "HTTP/", 0)) modificarLocation(p, "");
                s.sendChars(p, cl);
            }
        }
        thSleep(10);
    }
    return 0;
}
