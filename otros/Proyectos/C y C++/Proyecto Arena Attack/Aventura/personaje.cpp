#include "personaje.h"
#include <cstdlib>
#include <time.h>

personaje::personaje(int level, int spec)
{
    _habilidad=spec;
    _nivel=level;
    _vidamax=50+level*10;
    _vida=_vidamax;
    _manamax=level*5;
    _mana=_manamax;
    _ataque=10+level*5;
}

int personaje::atacar(personaje * golpeador, int tipo)
{
    int dano;
    if(tipo==1)
    {
        dano=rand()%(golpeador->nivel()+7)+golpeador->ataque()-ataque()/2.5;
        if(dano<golpeador->ataque()/5) dano=golpeador->ataque()/5;
        if(golpeador->habilidad()%3==0 && (rand()%4==0)) dano*=2;
    }else{
        dano=rand()%(golpeador->nivel()+7)+golpeador->manamax()-manamax()/2.5;
        if(dano<golpeador->manamax()/5) dano=golpeador->manamax()/5;
        if(golpeador->habilidad()%5==0 && (rand()%4==0)) dano*=2;
        golpeador->quitarmana(7+golpeador->nivel());
    }
    _vida=_vida-dano;
    if(_vida<0) _vida=0;
    return dano;
}

int personaje::curar()
{
    int cura=(manamax()/1.2);
    if(_habilidad%5==0 && (rand()%4)==0) cura*=2;
    _vida+=cura;
    if(_vida>_vidamax) _vida=_vidamax;
    quitarmana(5+nivel());
    return cura;
}

void personaje::regenerar()
{
    if(_habilidad%2==0) _vida+=3+_nivel;
    if(_vida>_vidamax) _vida=_vidamax;
    _mana+=_nivel;
    if(_mana>_manamax) _mana=_manamax;
}

void personaje::aumentarvida(int a)
{
    _vidamax+=a;
}

void personaje::aumentarmana(int a)
{
    _manamax+=a;
}

void personaje::aumentarataque(int a)
{
    _ataque+=a;
}

void personaje::establecerIA(int dificultad) //Pone las stats de la IA según la dificultad
{
    srand(time(NULL));
    _vidamax=50 + _nivel*10 + dificultad - 1 + rand()%4;
    _vida=_vidamax;
    _manamax=_nivel*5 + dificultad + rand()%4;
    _mana=_manamax;
    _ataque=10 + _nivel*5 + dificultad - 1 + rand()%4;
}

void personaje::quitarvida(int menos)
{
    _vida-=menos;
    if(_vida<0) _vida=0;
}

void personaje::quitarmana(int menos)
{
    _mana-=menos;
    if(_mana<0) _mana=0;
}

void personaje::reponer()
{
    _vida=_vidamax;
    _mana=_manamax;
}

void personaje::subirnivel()
{
    _nivel+=1;
    _vidamax+=10;
    _vida=_vidamax;
    _manamax+=6;
    _mana=_manamax;
    _ataque+=5;
}

void personaje::establecerhabilidad(int espec)
{
    _habilidad=espec;
    if(espec==60) // Modo Dios (al escribir 666 en eleccion de gladiador)
    {
        _vida+=666;
        _vidamax+=666;
        _mana+=666;
        _manamax+=666;
        _ataque+=666;
    }
}

int personaje::vida(){return _vida;}
int personaje::vidamax(){return _vidamax;}
int personaje::mana(){return _mana;}
int personaje::manamax(){return _manamax;}
int personaje::nivel(){return _nivel;}
int personaje::ataque(){return _ataque;}
int personaje::habilidad(){return _habilidad;}
