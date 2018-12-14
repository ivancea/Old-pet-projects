#include <arduino.h>

enum id{
    PINMODE=0,

    DIGITALWRITE,
    ANALOGWRITE,

    DIGITALREAD,
    ANALOGREAD,

    TONE,
    NOTONE
};

enum pinModes{
    PM_OUTPUT=0,
    PM_INPUT=1
};

enum digital{
    DM_LOW=0,
    DM_HIGH=1
};

void setup(){
    Serial.begin(9600);
    while(!Serial);
}

// Used Variables
    uint8_t id;
    uint8_t pin;
    uint8_t value;
    uint8_t mode;
    uint16_t analogValue;
    uint32_t time;
    uint32_t frequency;

void loop(){
    while(Serial.available()<1);
    
    char id = Serial.read();
    
    switch(id){
    case PINMODE:
        while(Serial.available()<2);
        pin = Serial.read();
        mode = Serial.read();
        pinMode(pin, (mode==PM_OUTPUT?OUTPUT:INPUT));
        break;
        
    case DIGITALWRITE:
        while(Serial.available()<2);
        pin = Serial.read();
        value = Serial.read();
        digitalWrite(pin, (value==DM_LOW?LOW:HIGH));
        break;
        
    case ANALOGWRITE:
        while(Serial.available()<2);
        pin = Serial.read();
        value = Serial.read();
        digitalWrite(pin, value);
        break;
        
    case DIGITALREAD:
        while(Serial.available()<1);
        pin = Serial.read();
        value = digitalRead(pin);
        Serial.write(value==LOW?DM_LOW:DM_HIGH);
        break;
        
    case ANALOGREAD:
        while(Serial.available()<1);
        pin = Serial.read();
        analogValue = analogRead(pin);
        Serial.write((uint8_t*)&analogValue, sizeof(analogValue));
        break;
        
    case TONE:
        while(Serial.available()<9);
        pin = Serial.read();
        Serial.readBytes((char*)&frequency,sizeof(frequency));
        Serial.readBytes((char*)&time,sizeof(time));
        if(time==0)
            tone(pin,frequency);
        else
            tone(pin,frequency,time);
        break;
        
    case NOTONE:
        while(Serial.available()<1);
        pin = Serial.read();
        noTone(pin);
        break;
    }
}
