int notes[] = {262,294,330,349};

void setup() {
  Serial.begin(9600);
}

void loop() {
  int keyVal = analogRead(A0);

  Serial.println(keyVal);

  if(keyVal >= 1020 && keyVal <= 1023){
    tone(8, notes[0]);
  } else if(keyVal >= 830 && keyVal <= 840){
    tone(8, notes[1]);
  } else if(keyVal >= 650 && keyVal <= 660){
    tone(8, notes[2]);
  } else if(keyVal >= 505 && keyVal <= 515){
    tone(8, notes[3]);
  } else {
    noTone(8);
  }
}
