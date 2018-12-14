#include "C:\includes\ivancea.h"
#include <conio.h>

/** NUM_OF_FILES/FIRST_NAME/FIRST_SIZE/SECOND_NAME/SECOND_SIZE\.../FILE1FILE2FILE3...**/

int getFileSize(ifstream& file);
vector< pair<string,int> > parseData(string s);
string getFileName(string dir);
void verArchivos(ifstream& file);
void comprimir(vector< pair<string,ifstream*> >& v, ofstream& fName);
void descomprimir(ifstream& file);
void encode(string& t);
void decode(string& t);

int main(){
    system("title Compresor/Descompresor de archivos");

    char c=0;
    do{
        system("cls");
        c=0;
        cout << endl << "Que desea hacer?" << endl << "Comprimir (C) / Descomprimir (D) / Ver archivos comprimidos (V) / Salir (S)? ";
        while(c!='D'&&c!='d'&&c!='C'&&c!='c'&&c!='V'&&c!='v'&&c!='S'&&c!='s')
            c=getch();

        system("cls");

        if(c=='V'||c=='v'){ /// VER ARCHIVOS ////////////////////////////////////

            string fName;
            ifstream file;

            cout << "Nombre o ruta del archivo (*.icf): " << endl;
            getline(cin, fName);
            if(!equalIgnoreCase(fName.substr(fName.size()-4,4),".icf")) fName+=".icf";
            file.open(fName,ios::binary);
            if(!file){
                cout << "No se ha podido abrir el archivo." << endl;
                system("pause");
                continue;
            }

            system("cls");

            verArchivos(file);

        }else if(c=='D'||c=='d'){ /// DESCOMPRIMIR ///////////////////////////////

            string fName;
            ifstream file;

            cout << "Nombre o ruta del archivo (*.icf): " << endl;
            getline(cin, fName);
            if(fName.size()>=4) if(!equalIgnoreCase(fName.substr(fName.size()-4,4),".icf")) fName+=".icf";
            file.open(fName,ios::binary);
            if(!file){
                cout << "No se ha podido abrir el archivo '" << fName << "'." << endl;
                system("pause");
                continue;
            }
            system("cls");

            descomprimir(file);

        }else if(c=='C'||c=='c'){ /// COMPRIMIR ///////////////////////////////

            bool fail=false;
            vector< pair<string,ifstream*> > v;
            string t;
            int nFiles=0;
            cout << "Numero de archivos a comprimir: ";
            getline(cin, t);
            nFiles=parseString(t);
            if(nFiles==0){
                cout << endl << "Numero de archivos nulo o invalido." << endl;
                system("pause");
                continue;
            }
            v.resize(nFiles, pair<string,ifstream*>());
            for(int i=0; i<v.size(); i++)
                v[i].second = new ifstream();
            for(int i=0; i<v.size(); i++){
                cout << "Nombre del archivo " << i+1 << ": " << endl;
                getline(cin, t);
                v[i].first=t;
                v[i].second->open(t,ios::binary);
                if(!(*(v[i].second))){
                    fail=true;
                    break;
                }
            }
            if(fail){
                cout << endl << "No se puede abrir el archivo." << endl;
                cout << "Asegurese de que los archivos estan bien escritos y listos para ser abiertos." << endl;
                system("pause");
                for(int i=0; i<v.size(); i++)
                    delete v[i].second;
                continue;
            }
            string fName;
            cout << endl << "Nombre del nuevo archivo (*.icf): ";
            getline(cin, fName);
            if(!equalIgnoreCase(fName.substr(fName.size()-4,4),".icf")) fName+=".icf";
            ofstream file(fName,ios::binary|ios::trunc);
            if(!file){
                cout << endl << "No se ha podido crear el archivo." << endl;
                system("pause");
                continue;
            }
            comprimir(v,file);
            cout << endl << "Proceso terminado. Archivo '" << fName << "' creado." << endl;
            system("pause");
            for(int i=0; i<v.size(); i++)
                delete v[i].second;

        }
    }while(c!='S'&&c!='s');
    return 0;
}

void comprimir(vector< pair<string,ifstream*> >& v, ofstream& f){
    cout << "Comprimiendo..." << endl;
    string t;
    f << v.size();
    for(int i=0; i<v.size(); i++)
        f << '/' << v[i].first << '/' << getFileSize(*v[i].second);
    f << '\\';

    for(int i=0; i<v.size(); i++){
        while(!v[i].second->eof()){
            getline(*v[i].second,t);
            if(!v[i].second->eof())
                t += '\n';
            encode(t);
            f << t;
        }
    }
}

void descomprimir(ifstream& file){
    string t;
    int n=0;
    int fSize=getFileSize(file);
    getline(file,t,'\\');
    vector< pair<string,int> > v = parseData(t);
    for(int i=0; i<v.size(); i++)
        n+=v[i].second;
    n+=1+t.size();
    if(v.size()==0 || n!=fSize){
        cout << "Archivo corrupto o invalido." << endl;
        system("pause");
        return;
    }
    vector<ofstream*> files;
    files.resize(v.size());
    for(int i=0; i<files.size(); i++)
        files[i]=new ofstream();
    bool fail=false;
    for(int i=0; i<v.size(); i++){
        files[i]->open(v[i].first,ios::binary|ios::trunc);
        if(!files[i]){
            fail = true;
            break;
        }
    }
    if(fail){
        cout << endl << "No se puede abrir un archivo para escritura." << endl;
        cout << "Asegurese de que no hay archivos con el mismo nombre en el directorio." << endl;
        system("pause");
        for(int i=0; i<files.size(); i++)
            delete files[i];
        return;
    }

    cout << "Descomprimiendo..." << endl;

    t="";
    for(int i=0; i<files.size(); i++){
        int bytes=0;
        if(t.size()>0){
            *files[i] << t.substr(0,v[i].second);
            bytes+=t.size();
        }
        while(!file.eof() && bytes<v[i].second){
            getline(file,t);
            if(!file.eof())
                t += '\n';
            decode(t);
            *files[i] << t.substr(0,v[i].second-bytes);
            t.erase(0,v[i].second-bytes);
            bytes+=t.size();
        }
    }

    cout << "Proceso de descompresion terminado." << endl;
    for(int i=0; i<files.size(); i++)
        delete files[i];
    system("pause");
}

void verArchivos(ifstream& file){
    string t;
    int n=0;
    int fSize=getFileSize(file);
    getline(file,t,'\\');
    vector< pair<string,int> > v = parseData(t);
    for(int i=0; i<v.size(); i++)
        n+=v[i].second;
    n+=1+t.size();
    if(v.size()==0 || n!=fSize){
        cout << "Archivo corrupto o invalido." << endl;
        system("pause");
        return;
    }
    cout << "LISTA DE ARCHIVOS" << endl;
    cout << "-----------------" << endl;
    for(int i=0; i<v.size(); i++)
        cout << i << ") " << v[i].first << "  //  " << v[i].second << " bytes." << endl;
    cout << "-----------------" << endl;
    system("pause");
}

string getFileName(string dir){
    for(int i=dir.size()-1; i>=0; i++)
        if(dir[i]=='/'||dir[i]=='\\') return dir.substr(i+1,dir.size());
    return dir;
}

vector< pair<string,int> > parseData(string s){
    vector< pair<string,int> > t;
    vector<string> v = split(s,'/',2);
    if(v.size()<1) return t;
    int n = parseString(v[0]);
    v = split(s,'/',(n*2)+1);
    if(v.size()<(n*2)+1) return t;
    for(int i=0; i<n; i++)
        t.push_back(pair<string,int>(v[1+i*2],parseString(v[2+i*2])));
    return t;
}

int getFileSize(ifstream& file){
    streampos t=file.cur;
    int size=-1;
    if(file){
        file.seekg (0, file.end);
        size = file.tellg();
        file.seekg(t-1);
    }
    return size;
}

void encode(string& t){
    for(int i=0; i<t.size(); i++)
        t[i]+=2;
}

void decode(string& t){
    for(int i=0; i<t.size(); i++)
        t[i]-=2;
}
