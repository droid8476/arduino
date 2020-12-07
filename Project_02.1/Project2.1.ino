
int switchstate = 0;
int i = 0;

void setup() {
  // put your setup code here, to run once:
 
pinMode(2, INPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  
  switchstate = digitalRead(2);
  if (switchstate == LOW){
    digitalWrite(3, HIGH);
    delay(5000);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    delay(5000);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(5000);
  }else{
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);

    for (i=0; i<5;i++){
      delay(250);
      digitalWrite(4, LOW);
      delay(250);
      digitalWrite(4, HIGH);
    }

    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);  
    
  }
}
