#include <IRremote.h>

const int IR_PIN = 4;

const int LED[3]={8,9,10};
bool on[3]={false,false,false};

IRrecv irrecv(IR_PIN);
decode_results results;

enum{
  ON_OFF=0xFFA25D,
  MODE=0xFF629D,
  MUTE=0xFFE21D
};

int CON[2]={LOW,HIGH};

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  for(int i=0; i<3; i++)
    pinMode(LED[i],OUTPUT);
}

void loop() {
  if(irrecv.decode(&results)){
    if(results.value!=-1){
      Serial.println(results.value,HEX);
      switch(results.value){
      case ON_OFF:
        if(on[0])
          digitalWrite(LED[0],LOW);
        else
          digitalWrite(LED[0],HIGH);
        on[0] = !on[0];
        break;
      case MODE:
        if(on[1])
          digitalWrite(LED[1],LOW);
        else
          digitalWrite(LED[1],HIGH);
        on[1] = !on[1];
        break;
      case MUTE:
        if(on[2])
          digitalWrite(LED[2],LOW);
        else
          digitalWrite(LED[2],HIGH);
        on[2] = !on[2];
        break;
      }
    }
    irrecv.resume();
  }
}
