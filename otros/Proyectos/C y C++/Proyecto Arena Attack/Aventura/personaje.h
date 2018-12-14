#ifndef PERSONAJE_H
#define PERSONAJE_H


class personaje
{
    private:
        int _vida, _vidamax, _mana, _manamax, _nivel, _ataque, _habilidad; //El mana aumenta el dano de magia
    public:
        personaje(int, int);
        int vida();
        int vidamax();
        int mana();
        int manamax();
        int nivel();
        int ataque();
        int habilidad();
        int atacar(personaje*, int); //Si el int es 1, ataque normal. Sino, ataque magico
        int curar();                 //Retorna el dano o la vida curada
        void reponer();
        void regenerar();
        void aumentarvida(int);
        void aumentarmana(int);
        void aumentarataque(int);
        void quitarvida(int);
        void quitarmana(int);
        void subirnivel();
        void establecerIA(int);
        void establecerhabilidad(int);

};

#endif // PERSONAJE_H

