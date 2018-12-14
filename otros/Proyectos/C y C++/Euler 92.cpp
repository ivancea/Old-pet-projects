#include <iostream>
#include <math.h>

using namespace std;

unsigned long long int cantidad=0, contador=0, contadorx=0, temp=0, num=0;
double porcentaje=0.0;

int main () {
do{
cout << "Calcular una cadena numerica (sumar los cuadrados de las cifras de un numero)." << endl << "Numeros a calcular: ";
cin >> cantidad;
system("cls");
}while(cantidad<1);
for(unsigned long long int i=1;i<=cantidad;i++)
 {
   temp=i;
   while(temp!=1 && temp!=89)
   {
     num=temp;
    /* for(int u=1;u<1000000000;u++) 
      {
        if((int)(temp/pow((double)10,(double)u))<=0){ tamanio=u; break;}
      }
     temp=0;
     for(int p=0;p<tamanio;p++)
      {
        k=num/pow((double)10,(double)(1+p));
       // h=p!=0?(num-k*pow((double)10,(double)(1+p)))/pow((double)10,(double)p):num-(k*pow((double)10,(double)(1+p)));
       h=(num-k*pow((double)10,(double)(1+p)))/pow((double)10,(double)p);
        temp+=h*h;
      }*/
      temp=0;
      while(num)
       {
         temp+=(num%10)*(num%10);
         num/=10;
       }
   }
   if(temp==89) contador++;
   else contadorx++;
 }
 porcentaje=(100*contador)/cantidad;
 cout << "En una cadena de " << cantidad << " numeros hay..." << endl;
 cout << "Hay " << contador << " que acaban en 89." << endl;
 cout << "Hay " << contadorx << " que acaban en 1." << endl;
 cout << "EL " << porcentaje << "% acaban en 89." << endl;
 cout << "El " << (100-porcentaje) << "% acaban en 1." << endl;
 system("pause");
 return 0;
}
