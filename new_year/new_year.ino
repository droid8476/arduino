
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

const int switchPin = 6;
const int nextPin = A0;
const int tonePin = 6;
const int note = 294;

const int bells = 12;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(2,0);
  lcd.print("LET THERE BE");
  lcd.setCursor(5,1);
  lcd.print("CHANGE");
}

void loop() {

  if(analogRead(nextPin) > 1){
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("BELL");
    for(int i=1;i<=bells;i++){
      if(i<10){
        lcd.setCursor(8,1);
      }else{
        lcd.setCursor(7,1);
      }
      lcd.print(i);
      tone(tonePin, note);
      delay(1000);
      noTone(tonePin);
      delay(1000);
    }
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("BYE BYE 2020");
    lcd.setCursor(0,1);
    lcd.print("HAPPY NEW YEAR!!");
  }
}
