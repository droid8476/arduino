const int redLEDPin = 11;
const int greenLEDPin = 10;
const int blueLEDPin = 9;

const int redSensorPin = A0;
const int greenSensorPin = A1;
const int blueSensorPin = A2;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int redSensorValue = 0;
int greenSensorValue = 0;
int blueSensorValue = 0;


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  pinMode(redLEDPin,OUTPUT);
  pinMode(greenLEDPin,OUTPUT);
  pinMode(blueLEDPin,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  redSensorValue = analogRead(redSensorPin);
  delay(5);
  
  greenSensorValue = analogRead(greenSensorPin);
  delay(5);
  
  blueSensorValue = analogRead(blueSensorPin);
  delay(5);

  redValue = redSensorValue/4;
  greenValue = greenSensorValue/4;
  blueValue = blueSensorValue/4;

  digitalWrite(redLEDPin,redValue);
  digitalWrite(greenLEDPin,greenValue);
  digitalWrite(blueLEDPin,blueValue);

  Serial.print("RAW sensor values: \t Red:");
  Serial.print(redSensorValue);
  Serial.print("\t Green: ");
  Serial.print(greenSensorValue);
  Serial.print("\t Blue: ");
  Serial.print(blueSensorValue);
  Serial.print("\n");

  Serial.print("Mapped sensor values: \t Red:");
  Serial.print(redValue);
  Serial.print("\t Green: ");
  Serial.print(greenValue);
  Serial.print("\t Blue: ");
  Serial.print(blueValue);
  Serial.print("\n");
  
  delay(1000);
}
