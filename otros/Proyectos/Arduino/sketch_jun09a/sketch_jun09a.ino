const int DIR = 12,
          BRAKE = 9,
          PWM = 3;
          
bool isDir=false;

void setup() {
  Serial.begin(9600);
  pinMode(DIR, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(PWM, OUTPUT);
  analogWrite(PWM, 0);
}

void loop() {
  if(Serial.available()>0){
    int n = Serial.parseInt();
    analogWrite(PWM, 0);
    digitalWrite(BRAKE, HIGH);
    digitalWrite(DIR,(isDir?LOW:HIGH));
    isDir = !isDir;
    delay(25);
    digitalWrite(BRAKE,LOW);
    setMotorSpeed(n);
  }else delay(10);  
}

void setMotorSpeed(byte n){
  if(n<60)
    n=0;
  analogWrite(PWM, n);
}
