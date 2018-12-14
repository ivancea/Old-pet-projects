#include <iostream>
int i=63, f[11]={29,7,0,3,-79,87,-8,3,-6,-8,-67}, c=0;
int main () {
    i+=9;
    for(int k=f[2]-1;i!=34;k++,i+=f[k]){
        std::cout << (char)i;
        c = (i*5!=165)? c:c+=1;
        i = c<3? i:i+=1;
        k=(bool)c?k=1:k;
    }
    system("pause > nul");
}
