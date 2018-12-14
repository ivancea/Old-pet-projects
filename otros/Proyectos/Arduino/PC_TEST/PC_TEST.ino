
const int IN_AN_PIN = A1;
const int IN_PIN = 7;
const int OUT_PIN = 8;

void send(uint16_t n){
  Serial.write((unsigned char*)&n,2);
}

int getDistance(int pinIn, int pinOut){
  int tiempo;
  digitalWrite(pinOut,LOW);
  delayMicroseconds(5);
  digitalWrite(pinOut, HIGH);
  delayMicroseconds(10);
  tiempo=pulseIn(pinIn, HIGH, 1024);
  return tiempo;
}

void setup() {
  pinMode(IN_AN_PIN,INPUT);
  //pinMode(IN_PIN,INPUT);
  //pinMode(OUT_PIN,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    Serial.read();
    //send(getDistance(IN_PIN,OUT_PIN));
    send(analogRead(IN_AN_PIN));
  }
}
