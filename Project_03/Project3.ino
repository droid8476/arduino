const int sensorPin = A0;
const int offset = 2;
const float baselineTemp = 28;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int pinNumber = 2; pinNumber <=4; pinNumber++){
    pinMode(pinNumber,OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0) * 5.0;
  float temp = (voltage - .5) * 100;
  
  Serial.print("Valor del sensor: ");
  Serial.print(sensorVal);
  Serial.print(", voltatge: ");
  Serial.print(voltage);
  Serial.print(", temperatura: ");
  Serial.print(temp);
  Serial.print("\n");


  if (temp <= baselineTemp - offset){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  } else if (temp >= baselineTemp + offset) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  } else {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }

  delay(2000);

}
