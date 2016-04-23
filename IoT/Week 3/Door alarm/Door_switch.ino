#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// The sound played is from https://www.arduino.cc/en/tutorial/melody
// The code to trigger the door sensor is from http://www.instructables.com/id/Magnetic-Door-Sensor-and-Arduino/

const int switchPin = D1;
const int ledPin = D2;
int speakerPin = D3;
int length = 15; //het aantal noten
char notes[] = "ccggaagffeeddc "; //een spatie vertegenwoordigd een moment van rust
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
unsigned long currentMillis=0;
// Interval is how long we wait
// add const if this should never change
int interval=5000;
int interval2=30000;
// Tracks the time since last event fired
unsigned long openMillis=0;
boolean isOpen = false;
int playCount=0;
unsigned long currentAlarm=0;
unsigned long previousAlarm=0;

const char* ssid     = "iPhone van Sem";
const char* password = "bakkumpie321";
String host = "http://www.sembakkum.nl/IoT/";
String host2 = "http://www.sembakkum.nl/IoT/door.json";

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

HTTPClient http;

void sendValue(int value) {
  Serial.println(value);
  http.begin(host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String payload = "value=" + String(value);

  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.println("POST request gelukt");
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}

String getValue() {
  http.begin(host2);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String response = http.getString();
    Serial.print("Server waarde");
    Serial.println(response);
    delay(1000);
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject(response);
    String doorStatus = root["door"];
    Serial.println(doorStatus);
    return doorStatus;
  } else {
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}

void setup() {

  pinMode(switchPin, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, HIGH);
  
  digitalWrite(switchPin, HIGH);
  
  pinMode(speakerPin, OUTPUT);
  
  Serial.begin(9600);

  String postStr = "";
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP());

}

void loop() {

  currentMillis = millis();
  currentAlarm = millis();
  delay(1000);
  String doorStatus;
  doorStatus = getValue();
  Serial.print("I'm status in the loop: ");
  Serial.println(doorStatus);

  if(digitalRead(switchPin) == HIGH && !isOpen) {
    Serial.println("I'm in the loop");
    sendValue(1);
    isOpen = true;
    openMillis = currentMillis;
  }

  if(digitalRead(switchPin) == LOW && isOpen) {
    sendValue(0);
    isOpen = false;
    playCount=0;
    digitalWrite(ledPin, LOW);
  }

  Serial.println(doorStatus);

  if(currentMillis - openMillis >= interval && isOpen && doorStatus == String("open")){
    
     digitalWrite(ledPin, HIGH);
     playCount++;
     Serial.println(playCount);
     for (int i = 0; i < length; i++) {
      if(digitalRead(switchPin) == HIGH && playCount <=2) {
        if (notes[i] == ' ') {
          delay(beats[i] * tempo); // rest
        } else {
          playNote(notes[i], beats[i] * tempo);
        }
        // pause between notes
        delay(tempo / 2); 
      }
     }
  }

  if(playCount >=2 && doorStatus == String("open")){ 
    for (int i = 0; i < length; i++) {
      if(digitalRead(switchPin) == HIGH) {
        if (notes[i] == ' ') {
          delay(beats[i] * tempo); // rest
        } else {
          playNote(notes[i], beats[i] * tempo / 2);
        }
        // pause between notes
        delay(tempo / 2); 
      }
     }     
  }
}
