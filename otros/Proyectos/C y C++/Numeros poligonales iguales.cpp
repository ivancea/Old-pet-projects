#include <iostream>

using namespace std;

int main () {
unsigned long long int nt=0, np=0, nh=0, t=1, p=1, h=1;
nt=(t*(t+1)/2);
np=(p*(3*p-1)/2);
nh=(h*(2*h-1));
for(;;){
if(nt == np && np == nh){
cout << "Numero: " << nt << endl << "Numeros(triangular, pentagonal, hexagonal): " << t << " " << p << " " << h << endl << endl;
//break;
}
if(nt<np && nt<nh){
t++;
}else if(np<nt && np<nh){
p++;
}else if(nh<=np && nh<=nt){
h++;
}else t++;
//cout << t << " " << p << " " << h << endl << nt << " " << np << " " << nh << endl << endl;
nt=(t*(t+1)/2);
np=(p*(3*p-1)/2);
nh=(h*(2*h-1));
}
system("pause");
}
//(t*(t+1)/2) == (p*(3*p-1)/2) && (t*(t+1)/2) == (h*(2*h-1))
//n(n+1)/2 triangular
//n(3n-1)/2	pentagonal
//n(2n-1) hexagonal
