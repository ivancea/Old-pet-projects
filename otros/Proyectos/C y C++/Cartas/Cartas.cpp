#include <iostream>
#include <ctime>
#include "C:/includes/ivancea.h"

using namespace std;

class conjunto;
class barajaEspanola;
class mano;

class conjunto{
private:
    int cantidad;
    int *cartas;

public:
    conjunto(){cantidad = 0; cartas = nullptr;}
    ~conjunto(){delete cartas;}
    int getCantidad(){return cantidad;}
    int getCarta(int n){if(n<cantidad && n>=0) return cartas[n]; else if(cantidad>0 && cartas != nullptr) return cartas[0]; else return 0;}
    bool contiene(int n){for(int i=0; i<cantidad; i++) if(cartas[i] == n) return true; return false;}
    bool vacio(){if(getCantidad()==0) return 1; else return 0;}
    virtual void agregar(int c){
        int *temp = new int[cantidad+1];
        for(int i=0; i<cantidad;i++)
            temp[i] = cartas[i];
        temp[cantidad] = c;
        delete cartas;
        cartas = temp;
        cantidad++;
    }
    virtual bool quitar(int c){
        bool buscar = 0;
        if(cartas == nullptr || cantidad == 0) return false;
        if(cantidad == 1){delete cartas; cartas = nullptr; cantidad = 0; return true;}
        for(int i=0; i<cantidad; i++) if(cartas[i] == c){buscar = 1; break;}
        if(!buscar) return false;
        int *temp = new int[cantidad-1];
        for(int i=0, j=0; i<cantidad;i++, j++){
            if(cartas[i] == c && buscar){
                buscar == false;
                --j;
            }else
                temp[j] = cartas[i];
        }
        --cantidad;
        delete cartas;
        cartas = temp;
        return true;
    }
    void agregar(conjunto *a){
        for(int i=0; i<a->getCantidad();i++){
            agregar(a->getCarta(i));
            a->quitar(a->getCarta(i));
        }
    }
};

class barajaEspanola:public conjunto{
private:
    pila baraja;

public:
    barajaEspanola(){
        for(int i=1;i<5;i++)
            for(int j=1;j<11;j++)
                agregar(i*100 + j);
            for(int i=0; i<getCantidad(); i++)
                baraja.push(getCarta(i));
    }

    void barajar(){
        if(getCantidad()<1) return;
        srand(time(NULL));
        while(baraja.pop());
        conjunto temp;
        for(int i=0; i<getCantidad(); i++)
            temp.agregar(getCarta(i));
        int alea = 0;
        while(temp.getCantidad()>0){
            alea = rand()%(temp.getCantidad());
            baraja.push(temp.getCarta(alea));
            temp.quitar(temp.getCarta(alea));
        }
    }

    int verCarta(){return baraja.getData();}

    bool retirar(){
        if(!conjunto::quitar(baraja.getData())) return false;
        if(!baraja.pop()) return false;
        return true;
    }

    void agregar(int c){
        conjunto::agregar(c);
        baraja.push(c);
    }

    bool quitar(int c){
        if(!conjunto::quitar(c)) return false;
        pila temp;
        bool quitado = false;
        int len = baraja.getLength();
        for(int i=0;i<len;i++){
            if(baraja.getData()!=c || quitado){
                temp.push(baraja.getData());
                baraja.pop();
            }else{
                quitado = true;
                baraja.pop();
            }
        }
        len = temp.getLength();
        for(int i=0;i<len; i++){
            baraja.push(temp.getData());
            temp.pop();
        }
        return true;
    }
    enum{
        AS_OROS = 101,
        DOS_OROS = 102,
        TRES_OROS,
        CUATRO_OROS,
        CINCO_OROS,
        SEIS_OROS,
        SIETE_OROS,
        SOTA_OROS,
        CABALLO_OROS,
        REY_OROS,

        AS_ESPADAS = 201,
        DOS_ESPADAS = 202,
        TRES_ESPADAS,
        CUATRO_ESPADAS,
        CINCO_ESPADAS,
        SEIS_ESPADAS,
        SIETE_ESPADAS,
        SOTA_ESPADAS,
        CABALLO_ESPADAS,
        REY_ESPADAS,

        AS_COPAS = 301,
        DOS_COPAS = 302,
        TRES_COPAS,
        CUATRO_COPAS,
        CINCO_COPAS,
        SEIS_COPAS,
        SIETE_COPAS,
        SOTA_COPAS,
        CABALLO_COPAS,
        REY_COPAS,

        AS_BASTOS = 401,
        DOS_BASTOS = 402,
        TRES_BASTOS,
        CUATRO_BASTOS,
        CINCO_BASTOS,
        SEIS_BASTOS,
        SIETE_BASTOS,
        SOTA_BASTOS,
        CABALLO_BASTOS,
        REY_BASTOS,
    };
    static string getName(int c){
        string temp = "";
        if(!((c>100 && c<111) || (c>200 && c<211) || (c>300 && c<311) || (c>400 && c<411))) return temp;
        switch(c%100){
            case 1:temp+="As"; break;
            case 2:temp+="Dos"; break;
            case 3:temp+="Tres"; break;
            case 4:temp+="Cuatro"; break;
            case 5:temp+="Cinco"; break;
            case 6:temp+="Seis"; break;
            case 7:temp+="Siete"; break;
            case 8:temp+="Sota"; break;
            case 9:temp+="Caballo"; break;
            case 10:temp+="Rey"; break;
            default:return "";
        }
        switch(c/100){
            case 1:temp+=" de Oros"; break;
            case 2:temp+=" de Espadas"; break;
            case 3:temp+=" de Copas"; break;
            case 4:temp+=" de Bastos"; break;
        }
        return temp;
    }

    static void coutConjunto(conjunto *a){
        for(int i=0;i<a->getCantidad();i++)
            cout << a->getCarta(i) << "  -  " << getName(a->getCarta(i)) << endl;
    }
};

class mano:public conjunto{
private:

public:

    bool coger(barajaEspanola *a){
        if(a->verCarta()==-1 || a->getCantidad()==0) return false;
        agregar(a->verCarta());
        a->retirar();
        return true;
    }
};

void turno(mano *a, conjunto *mesa, conjunto *puntos){
    conjunto elecciones;
    int eleccion = 0,  suma = 0;
    do{
    system("cls");
    cout << "Estas son tus cartas: " << endl << endl;
    barajaEspanola::coutConjunto(a);
    cout << endl << endl << endl << "Estas son las cartas que hay en la mesa: " << endl << endl;
    barajaEspanola::coutConjunto(mesa);
    cout << endl << endl << "Introduce el numero de la carta que quieres echar: " << endl;
    cin >> eleccion;
    }while(!a->contiene(eleccion));
    mesa->agregar(eleccion);
    a->quitar(eleccion);
    suma = eleccion%100;
    system("cls");
    cout << "Echaste: " << eleccion << " - " << barajaEspanola::getName(eleccion) << endl;
    elecciones.agregar(eleccion);
    do{
    cout << endl << endl << "Estas son las cartas que hay en la mesa: " << endl << endl;
    barajaEspanola::coutConjunto(mesa);
    cout << endl << endl << "La suma de la baza es " << suma << "." << endl;
    cout << "Elige con que cartas quieres coger la baza (pon un 0 para no coger baza): ";
    cin >> eleccion;
    if(eleccion==0){
        suma = 20;
        break;
    }
    if(mesa->contiene(eleccion) && !elecciones.contiene(eleccion)){
        elecciones.agregar(eleccion);
        suma += eleccion%100;
    }
    system("cls");
    }while(suma < 15);
    if(suma == 15){
        for(int i=0; i<elecciones.getCantidad();i++){
            mesa->quitar(elecciones.getCarta(i));
            puntos->agregar(elecciones.getCarta(i));
        }
        if(mesa->getCantidad()==0) puntos->agregar(1000);
    }
}

void turnoIA(mano *a, conjunto *mesa, conjunto *puntos){
    mesa->agregar(a->getCarta(0));
    a->quitar(a->getCarta(0));
}


int main(){
    system("title Escoba");
    barajaEspanola mazo;
    conjunto puntosa, puntosb;
    mano a, b;
    mano campo;

    mazo.barajar();
    for(int i=0; i<4; i++)
        campo.coger(&mazo);
    while(!mazo.vacio()){
        for(int i=0;i<3;i++){
            if(!a.coger(&mazo)) break;
            b.coger(&mazo);
        }
        while(!(a.vacio() && b.vacio())){
            turno(&a, &campo, &puntosa);
            turnoIA(&b, &campo, &puntosb);
        }
    }
    a.agregar(&campo);
    system("cls");
    int puntoa=0, puntob=0;
    int oros=0, sietes=0;
    for(int i=0;i<puntosa.getCantidad();i++){
        if(puntosa.getCarta(i)/100 == 1) oros++; //Calcular sietes y oros
        if(puntosa.getCarta(i)%100 == 7) sietes++;
        if(puntosa.getCarta(i)==1000) puntoa++; //Calcular escobas de A
    }
    for(int i=0; i<puntosb.getCantidad();i++) //Calcular escobas de B
        if(puntosb.getCarta(i)==1000) puntob++;
    if(oros>5) puntoa++; else if(oros<5) puntob++;
    if(sietes>2) puntoa++; else if(sietes<2) puntob++;
    if(puntosa.getCantidad()>20) puntoa++; else if(puntosa.getCantidad()<20) puntob++; //Calcular cantidad de cartas
    if(puntosa.contiene(107)) puntoa++; else puntob++; //Sumar siete velo
    cout << "Tus puntos son: " << puntoa << endl << "Los puntos de tu adversario son: " << puntob;
}
