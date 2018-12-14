#include "C:\includes\ivancea.h"

void chatFunc(server*a, int recibe, int pide){
    string t, b;
    a->getSocket(pide)->setData("CHAT");
    a->getSocket(recibe)->setData("CHAT");
    a->send("CHAT\nREQUEST", pide);
    a->send("CHAT\nREQUEST", recibe);
    a->send("WAIT\nRESPONSE", pide);
    a->send(a->getSocket(recibe)->getData(0), recibe);
    while((t=a->recv<string>(recibe))!="" && (b=a->recv<string>(pide))!="/fin");
    if(!(t=="CHAT\nREJECT" || b=="/fin"))
        while(t!="/fin" && b!="/fin"){
            if((t=a->recv<string>(pide))!="") a->send(t, recibe);
            if((b=a->recv<string>(recibe))!="") a->send(b, pide);
        }
    a->getSocket(pide)->setData("NORMAL");
    a->getSocket(recibe)->setData("NORMAL");
    a->send("CHAT\nEND", pide);
    a->send("CHAT\nEND", recibe);
}

void func(server *a, bool *show){
    string t, t2;
    int n, contador=1;
    thread *th;
    while(true){
        n=0;
        t="";
        if(a->newClient(1)){
            a->send<int>(a->getClients()-1, a->getClients()-1);
            for(int i=0; i<a->getClients()-1; i++)
                 a->send<string>("<INFOSERVER: Se ha conectado un cliente con ID " + parseInt(contador) + ">\n", i);
            cout << "Se ha conectado un cliente con ID " + parseInt(contador) + "." << endl;
            a->getSocket(a->getClients()-1)->setData(parseInt(contador));
            a->getSocket(a->getClients()-1)->setData("NORMAL");
            ++contador;
        }
        for(int i=0; i<a->getClients(); i++){
            if(a->getSocket(i)->getData(1) == "NORMAL"){
                t=a->recv<string>(i);
                if(t=="NORMAL\nMSG"){
                    while((t=a->recv<string>(i))=="");{
                        t=a->getSocket(i)->getData(0) + ": " + t;
                        if(*show) cout << t << endl;
                        for(int j=0; j<a->getClients(); j++)
                            if(j!=i) a->send<string>(t, j);
                    }
                }else
                if(t=="CHAT\nASK"){
                    if((t=a->recv<string>(i))!="");{
                        t.erase(0,6);
                        if(t.size()<=parseInt(a->getClients()).size() && t.size()>0){
                            n=-1;
                            for(int k=0; k<a->getClients(); k++)
                                if(a->getSocket(k)->getData(0) == t) n = k;
                            if(n!=i && n>=0){
                                th = new thread(&chatFunc, a, n, i);
                            }
                        }
                    }
                }
            }else{
                a->recv<string>(i); a->recv<string>(i);
            }
        }
    }
}

int main(){//DATA 0 = ID; DATA 1 = MODO;
    int puerto = 27015;
    server a(puerto);
    string comando;
    bool show = false;
    thread th(&func, &a, &show);
    th.detach();
    cout << "Servidor establecido en el puerto " << puerto << "." << endl;
    while(comando!="exit"){
        getline(cin, comando);
        if(comando == "/show") show = ~show;
        else for(int i=0; i<a.getClients(); i++) a.send<string>("SERVER: " + comando, i);
    }
    return 0;
}
