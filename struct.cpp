#include "ivancea.h" // class exc
#include <exception>
#include <iostream>
#include <cstdlib>

using namespace std;

void* makeStruct(string type){ //int,string
    int count=0;
    for(int i=0; i<type.size(); i++)
        if(type.substr(i,3)=="int"){
            count+=4;
            i+=3;
        }else if(type.substr(i,6)=="string"){
            count+=4;
            i+=6;
        }
    void* mem = malloc(count);
    count=0;
    for(int i=0; i<type.size(); i++)
        if(type.substr(i,3)=="int"){
            *((int*)(mem+count))=0;
            count+=4;
            i+=3;
        }else if(type.substr(i,6)=="string"){ //Se guarda puntero a string
            *((string**)(mem+count))=new string();
            count+=4;
            i+=6;
        }
    return mem;
}

int getInt(string type, void* st, int index){
    int count=0, i=0;
    for(i=0; i<type.size(); i++)
        if(type.substr(i,3)=="int"){
            if(count==index) break;
            count++;
            i+=3;
        }else if(type.substr(i,6)=="string"){
            if(count==index) throw exc("Invalid index of struct (get): " + to_string(index));
            count++;
            i+=6;
        }
    return *((int*)(st+4*index));
}

string getString(string type, void* st, int index){
    int count=0, i=0;
    for(i=0; i<type.size(); i++)
        if(type.substr(i,3)=="int"){
            if(count==index) throw exc("Invalid index of struct (get): " + to_string(index));
            count++;
            i+=3;
        }else if(type.substr(i,6)=="string"){
            if(count==index) break;
            count++;
            i+=6;
        }
    return **((string**)(st+4*index));
}

void setInt(string type, void* st, int index, int value){
    int count=0, i=0;
    for(i=0; i<type.size(); i++)
        if(type.substr(i,3)=="int"){
            if(count==index) break;
            count++;
            i+=3;
        }else if(type.substr(i,6)=="string"){
            if(count==index) throw exc("Invalid index of struct (set): " + to_string(index));
            count++;
            i+=6;
        }
    *((int*)(st+4*index))=value;
}

void setString(string type, void* st, int index, string value){
    int count=0, i=0;
    for(i=0; i<type.size(); i++)
        if(type.substr(i,3)=="int"){
            if(count==index) throw exc("Invalid index of struct (set): " + to_string(index));
            count++;
            i+=3;
        }else if(type.substr(i,6)=="string"){
            if(count==index) break;
            count++;
            i+=6;
        }
    **((string**)(st+4*index))=value;
}

#define CONTACT "string,int,string" //Nombre,Numero,Mail
#define CONTACT_NAME 0
#define CONTACT_NUMBER 1
#define CONTACT_MAIL 2

int main(){
    void* a = makeStruct(CONTACT);
    try{ /// STRUCT_TYPE, VAR, FIELD
        setString(CONTACT,a,CONTACT_NAME,"Hola, que tal");
        cout << getString(CONTACT,a,CONTACT_NAME);
        getString(CONTACT,a,CONTACT_NUMBER); //EXCEPTION THROW
    }catch(exception &e){
        cout << endl << e.what() << endl;
    }
}
