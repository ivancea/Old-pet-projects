const int pinOut1 = 7;
const int pinIn1 = 8;
const int pinOut2 = 12;
const int pinIn2 = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(pinIn1,INPUT);
  pinMode(pinOut1,OUTPUT);
  pinMode(pinIn2,INPUT);
  pinMode(pinOut2,OUTPUT);
  
  pinMode(10, OUTPUT);
}

void loop(){
  while(getDistance(pinIn1, pinOut1)>100);
  float n = micros();
  while(getDistance(pinIn2, pinOut2)>100);
  n = micros()-n;
  Serial.print(150000.0/n);
  Serial.println(" m/s");
  analogWrite(10, 100);
  delay(20);
  digitalWrite(10, LOW);
  delay(2000);
}

int getDistance(int pinIn, int pinOut){
  int tiempo;
  digitalWrite(pinOut,LOW);
  delayMicroseconds(5);
  digitalWrite(pinOut, HIGH);
  delayMicroseconds(10);
  tiempo=pulseIn(pinIn, HIGH);
  return tiempo*0.17;
}
