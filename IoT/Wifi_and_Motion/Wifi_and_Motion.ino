#include <ThingSpeak.h>

#include <ESP8266WiFi.h>
#define pir D0
#define led LED_BUILTIN
int pirState = LOW;
int val = 0;

// www.arduinesp.com
//
// Plot DTH11 data on thingspeak.com using an ESP8266
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com


const char* ssid = "iPhone van Sem";
const char* password = "blabla123";

WiFiClient client;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
  Serial.begin(9600);
  delay(10);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {
  val = digitalRead(pir);
    if (val == HIGH) {
      digitalWrite(led, HIGH);
        if (pirState == LOW) {
          Serial.println("Motion detected!");
          pirState = HIGH;
        }
    } else {
      digitalWrite(led, LOW);
        if (pirState == HIGH) {
          Serial.println("Motion ended");
          pirState = LOW;
        }
    }
}


