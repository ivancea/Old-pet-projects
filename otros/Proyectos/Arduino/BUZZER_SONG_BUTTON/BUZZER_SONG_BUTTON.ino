#include <EEPROM.h>

const int SOUND_PIN = 11;

enum notas{
    DO=1,
    DO_S=2,
    RE,
    RE_S,
    MI,
    FA,
    FA_S,
    SOL,
    SOL_S,
    LA,
    LA_S,
    SI
};

void note(int octava, int nota, int ms){
    if(octava<=0 || nota<0 ||ms<=0) return;
    double oct = octava, no = nota;
    /**37 through 32,767**/
    double n = 440.0 * exp(((oct-4)+(no-10)/12)*log(2));
    if(n<37 || n>32767) return;
    tone(SOUND_PIN, n, ms);
    delay(ms);
}

void tetris(int velocity=200){
    note(4, LA, velocity*2);
    note(4, MI, velocity*1);
    note(4, FA, velocity*1);

    note(4, SOL, velocity*2);
    note(4, FA, velocity*1);
    note(4, MI, velocity*1);

    note(4, RE, velocity*2);
    note(4, RE, velocity*1);
    note(4, FA, velocity*1);

    note(4, LA, velocity*2);
    note(4, SOL, velocity*1);
    note(4, FA, velocity*1);

    note(4, MI, velocity*2);
    note(4, MI, velocity*1);
    note(4, FA, velocity*1);

    note(4, SOL, velocity*2);
    note(4, LA, velocity*2);

    note(4, FA, velocity*2);
    note(4, RE, velocity*2);

    note(4, RE, velocity*4);

    delay(velocity);
    note(4, SOL, velocity*2);
    note(4, LA_S, velocity*1);

    /*****************************************************/

    note(5, RE, velocity*1);
    note(5, RE, velocity*1);
    note(5, DO, velocity*1);
    note(4, LA_S, velocity*1);

    note(4, LA, velocity*3);
    note(4, FA, velocity*1);

    note(4, LA, velocity*1);
    note(4, LA, velocity*1);
    note(4, SOL, velocity*1);
    note(4, FA, velocity*1);

    note(4, MI, velocity*2);
    note(4, MI, velocity*1);
    note(4, FA, velocity*1);

    note(4, SOL, velocity*2);
    note(4, LA, velocity*2);

    note(4, FA, velocity*2);
    note(4, RE, velocity*2);

    note(4, RE, velocity*4);
}

void himnoAleman(int velocity=200){
    note(4, FA, velocity*2);
    note(4, FA, velocity*2);
    note(4, MI, velocity*2);

    note(4, RE, velocity*6);
    note(4, FA, velocity*2);

    note(4, SI, velocity*3);
    note(4, LA, velocity*1);
    note(4, SOL, velocity*2);
    note(4, RE, velocity*2);

    note(4, FA, velocity*4);
    note(4, MI, velocity*4);

    /******************************/

    delay(velocity*2);
    note(4, MI, velocity*2);
    note(4, MI, velocity*2);
    note(4, RE, velocity*2);

    note(4, DO, velocity*6);
    note(4, FA, velocity*2);

    note(4, FA, velocity*3);
    note(4, FA, velocity*1);
    note(4, SOL, velocity*2);
    note(4, FA, velocity*2);

    note(4, RE, velocity*6);
    delay(velocity*2);

    for(int i=0; i<2; i++){

        delay(velocity*2);
        note(4, FA, velocity*2);
        note(4, RE, velocity*2);
        note(4, FA, velocity*2);

        note(4, SI, velocity*3);
        note(5, RE, velocity*1);
        note(5, RE, velocity*2);
        note(5, RE, velocity*2);

        note(5, DO, velocity*2);
        note(4, SI, velocity*2);
        note(4, LA, velocity*2);
        note(4, SI, velocity*2);

        note(5, MI, velocity*4);
        note(5, MI, velocity*4);

        /******************************/

        delay(velocity*2);
        note(5, DO, velocity*2);
        note(4, SI, velocity*2);
        note(4, SI, velocity*2);

        note(5, RE, velocity*6);
        note(4, SI, velocity*2);

        note(4, LA, velocity*3);
        note(4, LA, velocity*1);
        note(4, SOL, velocity*2);
        note(4, LA, velocity*2);

        note(4, SI, velocity*6);
        delay(velocity*2);
    }
}

int song = -1;

void setup() {
  pinMode(SOUND_PIN, OUTPUT);
  song = EEPROM.read(0);
}

void loop() {
  switch(song){
  case 0: tetris(250); break;
  case 1: himnoAleman(); break;
  }
  /*song = (song==0?1:0);
  EEPROM.write(0,song);*/
}
