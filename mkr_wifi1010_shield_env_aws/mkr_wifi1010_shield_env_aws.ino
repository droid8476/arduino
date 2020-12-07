#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <Arduino_MKRENV.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <Arduino_JSON.h>
#include <RTCZero.h>
#include "mkr_wifi1010_shield_env_aws.h"

const int SERIAL_RETRY = 3;
const int READ_SENSORS = 60000;
const int GREEN = 25;
const int RED = 26;
const int BLUE = 27;
const int GMT = 1;

RTCZero rtc;

//Enter your sensitive data in mkr_wifi1010_aws_test.h
const char ssid[]           = SECRET_SSID;
const char pass[]           = SECRET_PASS;
const char aws_endpoint[]   = AWS_ENDPOINT;
const char* certificate     = SECRET_CERTIFICATE;

const char mqttid[]      = "ARDUINO_MKR1010";  

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

unsigned long lastMillis = 0;

void setup() {
  int cont=0;
  
  // initialize digital pin LED_BUILTIN as an output.
  WiFiDrv::pinMode(GREEN, OUTPUT);
  WiFiDrv::pinMode(RED, OUTPUT);
  WiFiDrv::pinMode(BLUE, OUTPUT);

  blinkProgress(3);
  
  while(!Serial && cont<SERIAL_RETRY){
      Serial.begin(115200);
      blinkError(1);
      cont = cont +1;
  }
  blinkProgress(1);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    blinkError(3);        
    while(1);
  }
  blinkProgress(1);

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    blinkError(3);
    while (1);
  }
  blinkProgress(1);

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);
  blinkProgress(1);
  
  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);
  blinkProgress(1);

  rtc.begin();
  rtc.setEpoch(WiFi.getTime());
  
  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  // mqttClient.setId(mqttid);

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);
  blinkProgress(1);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    blinkProgress(3);
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    blinkProgress(3);
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  // read all the sensor values
  char date_telemetry[11];
  char time_telemetry[9];
  String sdate_telemetry;
  String stime_telemetry;
  String pos_latitude  = "41.731090";
  String pos_longitude = "1.821773";
  int pos_elevation    = 261;
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();

  rtc.setEpoch(getTime());
  sprintf(date_telemetry,"%.2d-%.2d-%.d",rtc.getDay(),rtc.getMonth(),rtc.getYear());
  sprintf(time_telemetry,"%.2d:%.2d:%.2d",(rtc.getHours() + GMT),rtc.getMinutes(),rtc.getSeconds());
  sdate_telemetry = date_telemetry;
  stime_telemetry = time_telemetry;
  // print each of the sensor values
  Serial.print("Date        = ");
  Serial.println(date_telemetry);
  Serial.print("Time        = ");
  Serial.println(time_telemetry);
  Serial.print("Latitude    = ");
  Serial.print(pos_latitude);
  Serial.println("°");
  Serial.print("Longitude    = ");
  Serial.print(pos_longitude);
  Serial.println("°");
  Serial.print("Elevation   = ");
  Serial.print(pos_elevation);
  Serial.println("m");
  Serial.print("Temperature = ");
  Serial.print(temperature,2);
  Serial.println("°C");
  Serial.print("Humidity    = ");
  Serial.print(humidity,2);
  Serial.println("%");
  Serial.print("Pressure    = ");
  Serial.print(pressure*10,1);
  Serial.println("kPa");
  Serial.print("Illuminance = ");
  Serial.print(illuminance,2);
  Serial.println("lx");
  Serial.print("UVA         = ");
  Serial.println(uva,2);
  Serial.print("UVB         = ");
  Serial.println(uvb,2);
  Serial.print("UV Index    = ");
  Serial.println(uvIndex,2);
  Serial.println();

  // Publist to AWS the sensors values
  publishMessage(date_telemetry,time_telemetry,pos_latitude,pos_longitude,pos_elevation,temperature,humidity,pressure,illuminance,uva,uvb,uvIndex);
  blinkOK(3);

  // wait to read sensors again
  waitReadValues();
}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");
  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    blinkError(3);
    delay(5000);
  }
  
  Serial.println();
  Serial.println("You're connected to the network");
  Serial.println();
  blinkOK(1);
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(aws_endpoint);
  Serial.println(" ");

  while (!mqttClient.connect(aws_endpoint, 8883)) {
    // failed, retry
    Serial.print(".");
    blinkError(1);
    delay(5000);
  }
  Serial.println();
  Serial.println("You're connected to the MQTT broker");
  Serial.println();
  blinkOK(1);

  // subscribe to a topic
  mqttClient.subscribe("arduino_mkr1010/incoming");
  blinkOK(1);
}

void publishMessage(String date_telemetry, String time_telemetry,String pos_latitude, String pos_longitude, int pos_elevation,float temperature, float humidity, float pressure, float illuminance, float uva, float uvb, float uvIndex) {
  Serial.println("Creating JSON");
  JSONVar myJSONobject;

  myJSONobject["Date"] = date_telemetry;
  myJSONobject["Time"] = time_telemetry;
  myJSONobject["Latitude"] = pos_latitude;
  myJSONobject["Longitude"] = pos_longitude;
  myJSONobject["Elevation"] = pos_elevation;
  myJSONobject["Temperature"] = (int)(temperature * 100 + 0.5) / 100.0;
  myJSONobject["Humidity"] = (int)(humidity * 100 + 0.5) / 100.0;
  myJSONobject["Pressure"] = (int)(pressure * 10 + 0.5) / 10.0;
  myJSONobject["Illuminance"] = (int)(illuminance * 100 + 0.5) / 100.0;
  myJSONobject["UVA"] = (int)(uva * 100 + 0.5) / 100.0;
  myJSONobject["UVB"] = (int)(uvb * 100 + 0.5) / 100.0;
  myJSONobject["UVIndex"] = (int)(uvIndex * 100 + 0.5) / 100.0;

  Serial.println("Publishing message");

  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage("arduino_mkr1010/outgoing");
  mqttClient.print(myJSONobject);
  mqttClient.endMessage();
}

void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }

  Serial.println();
  Serial.println();
}

void waitReadValues(){
  unsigned long time_now = millis();

  while(millis() < time_now + READ_SENSORS){
    if (WiFi.status() != WL_CONNECTED) {
      blinkError(1);
    }else{
      blinkOK(1);
    }
  }
}

void blinkError(int num){
  WiFiDrv::digitalWrite(RED, LOW);
  WiFiDrv::digitalWrite(GREEN, LOW);
  WiFiDrv::digitalWrite(BLUE, LOW);
  for(int i=0; i<num; i++)
  {
    WiFiDrv::digitalWrite(RED, HIGH);     
    delay(500);                           
    WiFiDrv::digitalWrite(RED, LOW);      
    delay(500);                           
  }
  WiFiDrv::digitalWrite(RED, HIGH);
 
}

void blinkOK(int num){
  WiFiDrv::digitalWrite(RED, LOW);
  WiFiDrv::digitalWrite(GREEN, LOW);
  WiFiDrv::digitalWrite(BLUE, LOW);
  for(int i=0; i<num; i++)
  {
    WiFiDrv::digitalWrite(GREEN, HIGH);    
    delay(500);                            
    WiFiDrv::digitalWrite(GREEN, LOW);     
    delay(500);                            
  }
  WiFiDrv::digitalWrite(GREEN, HIGH);
}

void blinkProgress(int num){
  WiFiDrv::digitalWrite(RED, LOW);
  WiFiDrv::digitalWrite(GREEN, LOW);
  WiFiDrv::digitalWrite(BLUE, LOW);
  for(int i=0; i<num; i++)
  {
    WiFiDrv::digitalWrite(BLUE, HIGH);      
    delay(500);                             
    WiFiDrv::digitalWrite(BLUE, LOW);       
    delay(500);                             
  }
}
