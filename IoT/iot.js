#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const int ledPin = D2;
const int inPin = D1;
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long timeR = 0;         // the last timeR the output pin was toggled
long debounce = 200;   // the debounce timeR, increase if the output flickers

//const char* ssid     = "iPhone van Rover";  
//const char* password = "zmofvvxzmg9tk";

const char* ssid      = "iPhone van Sem";
const char* password  = "newpass123";

const char* host     = "www.rovervannispen.nl"; // Your domain  
String path          = "/iot/light.json";  
String postStr = "";
String ledState = "";

void setup() {  
 pinMode(ledPin, OUTPUT);
 pinMode(inPin, INPUT);
 Serial.begin(115200);

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
 Serial.print("connecting to ");
 Serial.println(host);
 WiFiClient client;
 const int httpPort = 80;
 if (!client.connect(host, httpPort)) {
   Serial.println("connection failed");
   return;
 }

 client.print(String("GET ") + path + " HTTP/1.1\r\n" +
              "Host: " + host + "\r\n" + 
              "Connection: keep-alive\r\n\r\n");

// Reads if the button is pressed - HIGH or LOW
 reading = digitalRead(inPin);

 // read response
 String section="header";
 while(client.available()){
   String line = client.readStringUntil('\r');
   // Serial.print(line);
   // we'll parse the HTML body here
   if (section=="header") { // headers..
     Serial.print(".");
     if (line=="\n") { // skips the empty space at the beginning 
       section="json";
     }
   }
   else if (section=="json") {  // print the good stuff
     section="ignore";
     String result = line.substring(1);

     // Parse JSON
     int size = result.length() + 1;
     char json[size];
     result.toCharArray(json, size);
     StaticJsonBuffer<200> jsonBuffer;
     JsonObject& json_parsed = jsonBuffer.parseObject(json);
     if (!json_parsed.success())
     {
       Serial.println("parseObject() failed");
       return;
     }

     // Make the decision to turn off or on the LED
     if (strcmp(json_parsed["light"], "on") == 0) {
       state = HIGH; 
       Serial.println("Led on");
       ledState = "on";
     }
     else {
       state = LOW;
       Serial.println("Led off");
       ledState = "on";
     }
   }

   if (reading == HIGH && previous == LOW && millis() - timeR > debounce) {
      if (state == HIGH) {
        state = LOW;
        ledState = "on";
        postStr += "light=off";
        Serial.println("State = LOW");
        Serial.println(postStr);
      } else {
        state = HIGH;
        ledState = "off";
        postStr += "light=on";
        Serial.println("State = HIGH");
        Serial.println(postStr);
      }
      timeR = millis();
      Serial.println("connecting...");
      client.println("POST /iot/index.php HTTP/1.1");
 
      // EDIT: 'Host' to match your domain
      client.println("Host: www.rovervannispen.nl");
      client.println("User-Agent: Arduino/1.0");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded;");
      client.print("Content-Length: ");
      client.println(postStr.length());
      client.println();
      client.println(postStr);
      Serial.println("End of post");
      delay(2000);
 
      while(client.available()) {
        char c = client.read();
        Serial.print(c);
      }    
    }
    
    digitalWrite(ledPin, state);
    previous = reading;
     
   }
   delay(5000); // wait for server to respond
   
   
   
 Serial.print("closing connection. ");
}