
#include <WiFiNINA.h> // change to #include <WiFi101.h> for MKR1000
#include "wifi_secret.h"

const int baudrate = 115200;

//Enter your sensitive data in wifi_secret.h
const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

void setup() {
  Serial.begin(baudrate);
  while (!Serial);

  Serial.println("Serial port state: OPEN");
  Serial.print("Serial port baud rate: ");
  Serial.println(baudrate);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}
