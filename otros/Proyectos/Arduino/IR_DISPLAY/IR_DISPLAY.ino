#include <IRremote.h>

const int IR_PIN = 13;

enum{
   ZERO=B11111100, //0
   ONE=B01100000, //1
   TWO=B11011010, //2
   THREE=B11110010, //3
   FOUR=B01100110, //4
   FIVE=B10110110, //5
   SIX=B10111110, //6
   SEVEN=B11100000, //7
   EIGHT=B11111110, //8
   NINE=B11100110, //9
   VOID=B00000000, //shows nothing
   RIGHT=B00001110,
   LEFT=B01100010,
   MINUS=B00000010,
   E=B10011110,
   MUTE=B10010010
 };
/*FF629D mode
FFE21D mute
FF22DD start
FF02FD left
FFC23D right
FFE01F EQ
FFA857 minus
FF906F plus
FF9867 remolino
FFB04F u/sd
*/
const int segmentPins[8]= { 5,8,9,7,6,4,3,2 };

IRrecv irrecv(IR_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  for (int i=0; i < 8; i++){
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop() {
  if(irrecv.decode(&results)){
    if(results.value!=-1){
      Serial.println(results.value,HEX);
      switch(results.value){
      case 0xFF6897:show(ZERO);break;
      case 0xFF30CF:show(ONE);break;
      case 0xFF18E7:show(TWO);break;
      case 0xFF7A85:show(THREE);break;
      case 0xFF10EF:show(FOUR);break;
      case 0xFF38C7:show(FIVE);break;
      case 0xFF5AA5:show(SIX);break;
      case 0xFF42BD:show(SEVEN);break;
      case 0xFF4AB5:show(EIGHT);break;
      case 0xFF52AD:show(NINE);break;
      case 0xFFA25D:show(VOID);break;
      case 0xFF02FD:show(LEFT);break;
      case 0xFFC23D:show(RIGHT);break;
      case 0xFFE01F:show(E);break;
      case 0xFFA857:show(MINUS);break;
      case 0xFFE21D:show(MUTE);break;
      }
    }
    irrecv.resume();
  }
}

void show(int value){
  boolean isBitSet;
  for (int segment=0; segment < 8; segment++){
    isBitSet= bitRead(value, segment);
    digitalWrite(segmentPins[segment], isBitSet);
  }
}
