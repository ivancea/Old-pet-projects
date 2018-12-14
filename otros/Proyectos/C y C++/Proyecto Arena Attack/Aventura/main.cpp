#include <iostream>        //Malo es el nombre de el enemigo (que cambia cada ronda)
#include "personaje.h"     //Yo es el nombre del protagonista
#include <cstdlib>
#include <time.h>

//#ifdef WIN32 or WINDOWS or _WIN32 or _WIN64 or WIN64 or _WINDOWS //Portabilidad para Windows y Linux
#define borrar "cls"
//#else
//#define borrar "clear"
//#endif // _win32

using namespace std;

int dificultad=0, temp=0, espec=0; //Dificultad y valores temporales para elecciones switch y ataques
bool perder=0;

void turno(personaje*, personaje*);
void turnoIA(personaje*, personaje*);
void menu();
void mostrar(personaje*, personaje*);
void subirnivel(personaje*);
void acabar();
void pedir (int*); //Pido con "cin" y agarro errores

personaje yo(1, 0);

int main()
{
    srand (time(NULL));
    menu();
    yo.establecerhabilidad(temp);
    system(borrar);

    while(perder==0)
    {
        srand (time(NULL));
        temp=rand()%3;
        switch(temp)
        {
            case 1:temp=3;break;
            case 2:temp=5;break;
            default:temp=2;break;
        }
        if(dificultad==666) temp=60;
        if(yo.nivel()%20==0) temp=30;
        personaje * malo = new personaje((rand()%5==0?(rand()%3)+yo.nivel()-1:yo.nivel())+yo.nivel()*0.2 + yo.nivel()*0.1, temp);
        malo->establecerIA(dificultad);   // ^^^^^^ yo.nivel()*0.2 es para que cada 5 / 10 niveles sea mas nivel que tu
        yo.reponer();
        while(malo->vida()>0)
        {
            srand (time(NULL));
            yo.regenerar();
            mostrar(&yo,malo);
            turno(&yo,malo);    //Turno mío
            system(borrar);
            if(yo.vida()<=0)    //yo estoy muerto?
            {
                perder=1;
                break;
            }


            if(malo->vida()<=0) //el esta muerto?
            {
                break;
            }
            malo->regenerar();
            mostrar(&yo,malo);
            turnoIA(malo,&yo);  //Turno del malo IA
            system(borrar);
            if(yo.vida()<=0)    //yo estoy muerto?
            {
                perder=1;
                break;
            }
         }
         if(perder==0){
            mostrar(&yo, malo);
            cout << "Lo has derrotado!" << endl;
            system("pause");
            system(borrar);
            subirnivel(&yo);
         }
         system(borrar);
     }
     acabar();
     return 0;
}


void turno(personaje * atacante, personaje * defensor) //Se dan las opciones para combatir
{
    cout << "Que quieres hacer? (Ataque normal como predeterminado)" << endl;
    cout << "1) Ataque normal" << endl;
    if(atacante->mana()>=5+atacante->nivel()){
    cout << "2) Magia - Curarme   (" << 5+atacante->nivel() << " de mana)" << endl; //Tiene mana suficiente?
    if(atacante->mana()>=7+atacante->nivel()){
    cout << "3) Magia - Atacar   (" << 7+atacante->nivel() << " de mana)" << endl;
    }}
    pedir(&temp);
    if(atacante->mana()<7+atacante->nivel() && temp==3) temp=1;
    if(atacante->mana()<5+atacante->nivel() && temp==2) temp=1;
    switch(temp)   /** Se hace el daño o curacion **/
    {
    case 2:cout << "Te has curado " << atacante->curar() << "!" << endl;
    break;
    case 3:cout << "Has causado " << defensor->atacar(atacante, 2) << " de dano magico!" << endl;
    break;
    case 12368: if (defensor->habilidad()==60){cout << "No le puedes hacer eso al diablo..." << endl; atacante->quitarvida(atacante->vida()+1);}else {cout << "Le has robado el alma a tu adversario..." << endl; atacante->aumentarvida(defensor->vida());
    atacante->aumentarmana(defensor->mana()); atacante->aumentarataque(defensor->ataque()); defensor->quitarvida(defensor->vida()+10);}
    break; //Secreto
    default:cout << "Has causado " << defensor->atacar(atacante, 1) << " de dano normal!" << endl;
    break;
    }
    system("pause");

}

void turnoIA(personaje * atacante, personaje * defensor) //Se establece el dano o cura de la IA
{ /** La IA hace daño o curacion, segun tenga mana y vida **/
    if(atacante->vida()<=atacante->manamax()/1.2 && rand()%3==2 && atacante->mana()>=5+atacante->nivel())
    {
        cout << "Tu rival se ha curado " << atacante->curar() << "!" << endl;
    }else if((atacante->habilidad()%5==0 || rand()%2==1) && atacante->mana()>=7+atacante->nivel() && (atacante->habilidad()!=3 && atacante->habilidad()!=6))
    {
        cout << "Tu rival te ha causado " << defensor->atacar(atacante, 2) << " de dano magico!" << endl;
    }else
    {
        cout << "Tu rival te ha causado " << defensor->atacar(atacante, 1) << " de dano normal!" << endl;
    }
    system("pause");
}

void menu() //Pedir dificultad, presentacion.
{
system("title Arena Attack");
cout << "Bienvenido a Arena Attack" << endl << endl << "Elige la dificultad del juego(0-2, otro caracter sera \"Facil\"):" << endl;
cout << "0) Facil" << endl << "1) Medio" << endl << "2) Dificil" << endl;
pedir(&temp);
switch (temp) //Eleccion de la dificultad
{
    case 1:dificultad=1;break;
    case 2:dificultad=2;break;
    case 666:dificultad=666;break; //Secreto
    default:dificultad=0;break;
}
system(borrar);
cout << "Elige tu especializacion(0-2, otro caracter sera \"Gladiador\"):" << endl;
cout << "0) Gladiador - Regeneracion de vida" << endl << "1) Asesino - Probabilidad de ataques dobles" << endl << "2) Mago - Probabilidad de magias o curaciones dobles" << endl;
cout << endl << " Modos extra:" << endl << "3) Asesino sombrio - Mago y Asesino" << endl << "4) Guerrero - Gladiador y Asesino" << endl << "5) Archimago - Gladiador y mago" << endl;
cout << "6) Guerrero sombrio - Gladiador, Asesino y Mago" << endl;
pedir(&temp);
switch(temp)
{
case 1:temp=3;break;
case 2:temp=5;break;
case 3:temp=15;break;  //Multiplicando los valores de cada luchador, les doy las habilidades especiales
case 4:temp=6;break;
case 5:temp=10;break;
case 6:temp=30;break;
case 666:temp=60;break; //Secreto
default:temp=2;break;
}
system(borrar);
cout << "Ahora te enfrentaras a adversarios con ataques normales o con magia." << endl << "Iras subiendo niveles al matarlos." << endl << "Consigue ser el mas fuerte!" << endl;
system("pause");
}

void mostrar(personaje * a, personaje * b) //Mostrar las stats de los 2 luchadores cada ronda
{
    cout << "-- Tu -- ";
    if(a->habilidad()==2){cout << "Gladiador";}else if(a->habilidad()==3){cout << "Asesino";}else if(a->habilidad()==5){cout << "Mago";}
    else if(a->habilidad()==15){cout << "Asesino sombrio";}else if(a->habilidad()==6){cout << "Guerrero";}else if(a->habilidad()==10){cout << "Archimago";}else if(a->habilidad()==30){cout << "Guerrero sombrio";}
    else if(a->habilidad()==60){cout << "Dios";}
    cout << " --" << endl << "Tu nivel: " << a->nivel() << endl;
    cout << "  Tu vida: " << a->vida() << endl;
    cout << "  Tu mana: " << a->mana() << endl;
    cout << "  Tu ataque: " << a->ataque() << endl << endl;
    cout << "-- Tu adversario -- ";
    if(b->habilidad()==2){cout << "Gladiador";}else if(b->habilidad()==3){cout << "Asesino";}else if(b->habilidad()==5){cout << "Mago";}
    else if(b->habilidad()==15){cout << "Asesino sombrio";}else if(b->habilidad()==6){cout << "Guerrero";}else if(b->habilidad()==10){cout << "Archimago";}else if(b->habilidad()==30){cout << "Guerrero sombrio";}
    else if(b->habilidad()==60){cout << "Diablo";}
    cout << " --" << endl <<  "Su nivel: " << b->nivel() << endl;
    cout << "  Su vida: " << b->vida() << endl;
    cout << "  Su mana: " << b->mana() << endl;
    cout << "  Su ataque: " << b->ataque() << endl << endl;
}

void subirnivel(personaje * yo)  /**Muestra pantalla para elegir atributo a subir, y aumenta nivel**/
{
    cout << "Has subido de nivel!" << endl << "Puedes mejorar una de estas caracteristicas... (Vida por defecto)" << endl;
    cout << "1) Vida" << endl << "2) Mana y magia" << endl <<"3) Ataque normal" << endl;
    pedir(&temp);
    switch(temp)
    {
        case 2:yo->aumentarmana(3); break;
        case 3:yo->aumentarataque(3); break;
        default:yo->aumentarvida(7); break;
    }
    yo->subirnivel();
    cout << endl << "Has elegido " << temp << "! Tu estadistica ha subido!"<< endl;
    system("pause");
}

void acabar()    /**  Fin y creditos **/
{
system(borrar);
cout << "Has sido derrotado..." << endl;
cout << "Has llegado hasta el nivel " << yo.nivel() << "." << endl;
cout << "Vuelve a jugar siempre que quieras!" << endl << endl;
cout << "Copyright Ivancea / Ivancea96 / Zzerodes" << endl << endl;
cout << "Pulsa una tecla para salir de Arena Attack...";
system("pause > nul");
}

void pedir(int *var) /**Con esta función, evito problemas con "cin" al introducir una letra en vez de numero**/
{
    cin >> *var;
    if(cin.fail())
    {
       cin.clear();
        cin.ignore(10000, '\n');
        *var=0;
    }
}
