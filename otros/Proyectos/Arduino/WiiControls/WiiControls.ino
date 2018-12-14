//#define WIICHUCK

#include <Wire.h>

#ifdef WIICHUCK
#include <WiiChuck.h>
  WiiChuck wii = WiiChuck();
#else
#include <WiiClassic.h>
  WiiClassic wii = WiiClassic();
#endif


void setup() {
  Serial.begin(9600);
  wii.begin();
  wii.update();
  pinMode(INPUT, A0);
}

void writeInt(int n){
  Serial.write((char*)&n, 2);
}

#ifdef WIICHUCK

void writeWii(){  /** CHUCK **/
  writeInt(wii.readRoll());
  writeInt(wii.readPitch());
  writeInt(wii.readAccelX());
  writeInt(wii.readAccelY());
  writeInt(wii.readAccelZ());
  writeInt(wii.readJoyX());
  writeInt(wii.readJoyY());
  writeInt(wii.PressedC);
  writeInt(wii.PressedZ);
}

#else

void writeWii(){ /** CLASSIC **/
  writeInt(wii.leftShoulderPressed());
  writeInt(wii.rightShoulderPressed());
  writeInt(wii.leftShouldPressure());
  writeInt(wii.rightShouldPressure());
  writeInt(wii.lzPressed());
  writeInt(wii.rzPressed());
  writeInt(wii.upDPressed());
  writeInt(wii.rightDPressed());
  writeInt(wii.downDPressed());
  writeInt(wii.leftDPressed());
  writeInt(wii.xPressed());
  writeInt(wii.yPressed());
  writeInt(wii.aPressed());
  writeInt(wii.bPressed());
  writeInt(wii.selectPressed());
  writeInt(wii.homePressed());
  writeInt(wii.startPressed());
  writeInt(wii.leftStickX());
  writeInt(wii.leftStickY());
  writeInt(wii.rightStickX());
  writeInt(wii.rightStickY());
}

#endif

void loop() {
  while(!Serial.available())
    delay(1);
  Serial.read();
  
  wii.update();
  writeWii();
  writeInt(analogRead(A0));
}
