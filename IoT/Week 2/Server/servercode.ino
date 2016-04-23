#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "iPhone van Sem";
const char* password = "newpass123";

const char* host     = "www.rovervannispen.nl"; // Your domain
String path          = "/iot/light.json";
const int pin        = D2;
const int LDR_PIN = A0;
String postStr = "";

void setup() {
    pinMode(pin, OUTPUT);
    pinMode(pin, HIGH);
     pinMode(LDR_PIN, INPUT);
    Serial.begin(115200);
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
  String section="header";
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

    delay(500); // wait for server to respond

    // read response
   
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
                digitalWrite(pin, HIGH);
                Serial.println("LED ON");

                int LDRReading = analogRead(LDR_PIN);
                Serial.println(LDRReading);

    postStr = "value=" + String(LDRReading);
    Serial.println(postStr);
    
    Serial.println("connecting...");
    client.println("POST /iot/fromarduino_sem.php HTTP/1.1");

    // EDIT: 'Host' to match your domain
    client.println("Host: www.rovervannispen.nl");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(postStr.length());
    client.println();
    client.println(postStr);
    




    while(client.available()) {
        char c = client.read();
        Serial.print(c);
    }
    delay(5000);
                
            }
            else {
                digitalWrite(pin, LOW);
                Serial.println("led off");
            }

        }
    
    }
    Serial.print("closing connection. ");
}
