//Included libraries
#include <EIoTCloudRestApi.h>
#include <EIoTCloudRestApiConfig.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

//Define pins and leds
int pir = D0;
int ldr = A0;
int led = LED_BUILTIN;
int pirState = LOW;
int val = 0;
int light = 0;

//Define client
const char* ssid = "*************";
const char* password = "*************";
WiFiClient client;

//IoT keys for parameters
EIoTCloudRestApi eiotcloud;
#define AP_SSID "******"
#define AP_PASSWORD "******"
#define INSTANCE_ID_A "*************"
#define INSTANCE_ID_B "*************"

//ThingSpeak ID and API key
unsigned long CHANNEL_ID  = ******;
const char * API_KEY = "*************";

void setup() {
  
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
  Serial.begin(9600);
  eiotcloud.begin();
  ThingSpeak.begin(client);
  
}

void loop() {
  delay(1000);
  
  light = analogRead(ldr);
  Serial.println(light);
  eiotcloud.sendParameter(INSTANCE_ID_B, light);
  ThingSpeak.writeField(CHANNEL_ID, 2, light, API_KEY);

  val = digitalRead(pir); 
    if (val == HIGH){
      if (pirState == LOW){
        digitalWrite(led, HIGH);
        Serial.println("Motion detected!");
        ThingSpeak.writeField(CHANNEL_ID, 1, val, API_KEY);
        eiotcloud.sendParameter(INSTANCE_ID_A, 1);
        pirState = HIGH;
      }
    } else {
      if (pirState == HIGH) {
        digitalWrite(led, LOW);
        Serial.println("Motion ended");
        ThingSpeak.writeField(CHANNEL_ID, 1, val, API_KEY);
        eiotcloud.sendParameter(INSTANCE_ID_A, 0);
        pirState = LOW;
      }
    }
}
