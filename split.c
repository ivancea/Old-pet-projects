#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int split(char* str, unsigned int strLength, char*** ret, char c){
    char** temp;
    unsigned int size=1, i, j, cont=0;
    for(i=0;i<strLength; i++)
        if(str[i]==c && i+1<strLength && str[i+1]!=c)
            ++size;
    temp = (char**)malloc(size*sizeof(char*));
    for(i=0, j=0; cont<size && j<strLength; j++){
        if(j==strLength-1 && str[j]!=c){
            ++j;
            temp[cont] = (char*)malloc((j-i+1)*sizeof(char));
            memcpy(temp[cont], str+i, j-i);
            temp[cont][j-i] = '\0';
            break;
        }else if(str[j]==c && j-i>1){
            temp[cont] = (char*)malloc((j-i+1)*sizeof(char));
            memcpy(temp[cont], str+i, j-i);
            temp[cont][j-i] = '\0';
            ++cont;
            i=j+1;
        }else if(str[j]==c && j==i){
            i=j+1;
        }
    }
    *ret = temp;
    return size;
}

unsigned int splitStr(char* str, char*** ret, char c){
    return split(str,strlen(str),ret,c);
}

void freeSplit(char** mem, unsigned int size){
    int i=0;
    for(; i<size; i++)
        free(mem[i]);
    free(mem);
}

int main(){
    char** fin;
    unsigned int i, size=splitStr("Hola, mi nombre    es Ivancea :D", &fin, ' ');
    for(i=0; i<size; i++){
        puts(fin[i]);
    }
    freeSplit(fin, size);
    return 0;
}
