
#include <LiquidCrystal.h>
#include <IRremote.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
IRrecv irrecv(9);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.begin(16, 2);
  lcd.print("Code: ");
  pinMode(7, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value != -1) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(results.value, 16);
      Serial.println(results.value, 16);
    } else {
      digitalWrite(7, HIGH);
      delay(50);
      digitalWrite(7, LOW);
    }
    irrecv.resume();
  }
}

