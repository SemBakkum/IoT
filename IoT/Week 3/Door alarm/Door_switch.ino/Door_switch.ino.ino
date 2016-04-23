// Libraries we need to make a connection to WiFi, decode JSON and use HTTP.
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// The sound played is from https://www.arduino.cc/en/tutorial/melody
// The code to trigger the door sensor is from http://www.instructables.com/id/Magnetic-Door-Sensor-and-Arduino/

// Connection to the pins used on the esp.
const int switchPin = D1;
const int ledPin = D2;
int speakerPin = D3;


int length = 15; // Number of notes.
char notes[] = "ccggaagffeeddc "; // A space represents a rest.
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300; // The speed at which the melody is played.

// Interval is how long we wait.
int interval=5000;

// Tracks the time since last event fired
unsigned long currentMillis=0;
unsigned long openMillis=0;

// Check if the door is open.
boolean isOpen = false;

// The amount of times the melody has been played.
int playCount=0;

// Connection to WiFi.
const char* ssid     = "iPhone van Sem";
const char* password = "bakkumpie321";

// The hosts we're using for the GET and the POST request.
String host = "http://www.sembakkum.nl/IoT/";
String host2 = "http://www.sembakkum.nl/IoT/door.json";

// Functions to play the melody.
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

// Function to POST the door value to the server, this can be 0(door closed) or 1(door open).
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

// Function to GET the door value from the server, this is a JSON format so this is where we use the JSON library.
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
  
// If the door has been open longer than 5 sec. it is okay to set of the alarm. 
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
