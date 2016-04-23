#Make your own door alarm.
## To keep anyone from forgetting to close it.

At the end of this readme you will have your very own door alarm which you can monitor through a server.

The outcome will look something like this. 

![Outcome](https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png "Door alarm")

### The hardware

Lets start with the hardware that is needed to complete this project. 

1. A buzzer or piezo, you can buy this [here.](http://www.hackerstore.nl/Artikel/230)
2. A set of jumper wires, you can buy those [here](http://www.hackerstore.nl/Artikel/39) and [here.](http://www.hackerstore.nl/Artikel/222)
3. An LED and resistors, you can buy those [here.](http://www.hackerstore.nl/Artikel/201)
4. An magnetic door switch, you can buy that [here.](http://www.hackerstore.nl/Artikel/299)
5. A breadboard, you can buy that [here.](http://www.hackerstore.nl/Artikel/29)
6. An NodeMCU, you can buy that [here.](http://www.hackerstore.nl/Artikel/806)

### The magnetic door switch (step 1). 

To connect the door switch to the NodeMCU and let a LED ligth up when the door is open I used the tutorial write [here](http://www.instructables.com/id/Magnetic-Door-Sensor-and-Arduino/?ALLSTEPS).

In this tutorial the switch is connected to an Arduino board, but we're connection it to an NodeMCU so the first thing we should change is the pin number our door switch and LED are connected to. In the image below I connected the door switch to pin (orange jumper wires) D1 and the LED to pin D2. We connect them to the D pins because these sensors give us digital in/output. The black jumper wires are connected to the ground. 

![Step 1](https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png "Turn on LED when door is open")

The following code lets us accomplish the first step. 

```javascript
const int switchPin = D1; // Here we say to which pin the door switch is connected.
const int ledPin = D2; // Here we say to which pin the LED is connected.

void setup() {

	pinMode(switchPin, INPUT_PULLUP);

	pinMode(ledPin, OUTPUT);

	digitalWrite(switchPin, HIGH);

}

void loop() {

	if(digitalRead(switchPin) == LOW){

		digitalWrite(ledPin, LOW);

	}

	else{

		digitalWrite(ledPin, HIGH);

	}

}
```

### The buzzer or piezo (step 2)

Connecting the buzzer or piezo is quite easy. Just look at the photo below and you can manage to succeed.

![Buzzer piezo](https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png "Buzzer, piezo connection")

I connected the Buzzer, piezo to pin D3 and to the ground of the breadboard. The next step is to let it produce some sounds for us and how we do that will be treated in step 3.

### Let the alarm go off when the door opens (step 3)

Now the point of this alarm is that it will go off after a certain time of the door being open. So we don't want it to go off immediately.
To accomplish this we can use the millis() function, instead of using a delay. A delay will delay the rest of the code from running that is declared after it. Millis() doesn't give us this problem, but first we want our alarm (Buzzer or piezo) to make a sound when the door is open. For the sound it will produce I used to code provided in this [link](https://www.arduino.cc/en/tutorial/melody). There is also an good explanation of how the code works. So if we merge our previous code with this code the result will look like the code below.

```javascript
const int switchPin = D1; // Here we say to which pin the door switch is connected.
const int ledPin = D2; // Here we say to which pin the LED is connected.
int speakerPin = D3; // Here we say the buzzer or piezo is connected to pin D3.

// The melody declarations.
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

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

void setup() {

	pinMode(switchPin, INPUT_PULLUP);

	pinMode(ledPin, OUTPUT);

	digitalWrite(switchPin, HIGH);

	pinMode(speakerPin, OUTPUT);

}

void loop() {

	if(digitalRead(switchPin) == LOW){

		digitalWrite(ledPin, LOW);

	}

	else{

		digitalWrite(ledPin, HIGH);

		for (int i = 0; i < length; i++) {
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
```

### millis() (step 4)

As mentioned in step 3 we want to let the alarm go off after a certain time the door is open. This can be done with millis(). The only thing we have to do is make a simple mathmetical calculation, but first let me tell how millis() works. Once millis() is declared it will start running a time in milliseconds. We can use this time to calute and trigger certain events in the code without delaying the code. How the code is built up right now will not quite work for us, so we've got to refactor it a little. The main thing we will be refactoring is the void loop(). The code will look like the one below.

```javascript
const int switchPin = D1; // Here we say to which pin the door switch is connected.
const int ledPin = D2; // Here we say to which pin the LED is connected.
int speakerPin = D3; // Here we say the buzzer or piezo is connected to pin D3.

// The melody declarations.
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

// Interval is how long we wait for the alarm to go off.
int interval=5000;

// Tracks the time since last event fired.
unsigned long currentMillis=0;
unsigned long openMillis=0;

// Check if the door is open.
boolean isOpen = false;

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

void setup() {

	pinMode(switchPin, INPUT_PULLUP);

	pinMode(ledPin, OUTPUT);

	digitalWrite(switchPin, HIGH);

	pinMode(speakerPin, OUTPUT);

}

void loop() {
	
	// Here we declare that the current time is going to be millis(), so this is the point that millis starts running.
	currentMillis = millis();

	if(digitalRead(switchPin) == HIGH && !isOpen) {
    	isOpen = true;
    	openMillis = currentMillis;
  }

  if(digitalRead(switchPin) == LOW && isOpen) {
    	isOpen = false;
    	digitalWrite(ledPin, LOW);
  }
  
	// If the door has been open longer than 5 sec. it is okay to set off the alarm. 
  	if(currentMillis - openMillis >= interval && isOpen){
    
     	digitalWrite(ledPin, HIGH);
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

}
```

So we set the currentMillis and the openMillis to 0. Once the code starts in the void loop() currentMillis becomes millis.
We also use a check "isOpen" that can be true or false. So what we do here is say if the door is open and the boolean isOpen is true, then set isOpen to true and set openMillis, that was declared to be zero, to currentMillis. At this point currentMillis is still running and openMillis just copies the time it was given from that point. So if the door is open and the boolean isOpen is true we can do our mathmetical calculation and that goes as following. If currentMillis - openMillis is bigger or the same as interval (which we have set to be 5000) and isOpen (which is been set to true) are all true you can let the alarm go off. If we want to wait longer for the alarm to go off we can just set the interval state to a bigger number, lets say 7000. Then the alarm will wait 7 seconds before going off.

We can eventually say that we want the sound the play normal twice and after the second time speed it up. To accomplish this we add some extra code we'va already written and add a few extra things. The code below show how this can be achieved. 

```javascript
const int switchPin = D1; // Here we say to which pin the door switch is connected.
const int ledPin = D2; // Here we say to which pin the LED is connected.
int speakerPin = D3; // Here we say the buzzer or piezo is connected to pin D3.

// The melody declarations.
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

// Interval is how long we wait for the alarm to go off.
int interval=5000;

// Tracks the time since last event fired.
unsigned long currentMillis=0;
unsigned long openMillis=0;

// Check if the door is open.
boolean isOpen = false;

// The amount of times the melody has been played.
int playCount=0;

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

void setup() {

	pinMode(switchPin, INPUT_PULLUP);

	pinMode(ledPin, OUTPUT);

	digitalWrite(switchPin, HIGH);

	pinMode(speakerPin, OUTPUT);

}

void loop() {
	
	// Here we declare that the current time is going to be millis(), so this is the point that millis starts running.
	currentMillis = millis();

	if(digitalRead(switchPin) == HIGH && !isOpen) {
    	isOpen = true;
    	openMillis = currentMillis;
  }

  if(digitalRead(switchPin) == LOW && isOpen) {
    	isOpen = false;
    	digitalWrite(ledPin, LOW);
    	playCount=0;
  }
  
	// If the door has been open longer than 5 sec. it is okay to set off the alarm. 
  	if(currentMillis - openMillis >= interval && isOpen){
    	
    	playCount++;
     	digitalWrite(ledPin, HIGH);
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

  if(playCount >=2){ 
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
```

So we declared a playCount=0; This keeps track how many times the sound played or alarm went off. Each time the door is closed playCount is set to 0, but if the door is open and the alarm starts playinh we increase playCount by 1. When playCount is 2 we can use the new piece of code to play the alarm faster. In that if statement we check if playCount is equal to or bigger than 2 if that is true we play the alarm sound but we play it twice as fast. As you can see we can do this by dividing temp by 2. 

### Connecting our NodeMCU to WiFi (step 5)

For this setup I made us of the following link [here.](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiClientBasic/WiFiClientBasic.ino).

To use this code you should include the ESP8266WiFi.h library. You can use your own hotspot or home WiFi to connect to with the following code.
```javascript
// Libraries used in the code
#include <ESP8266WiFi.h>

const int switchPin = D1; // Here we say to which pin the door switch is connected.
const int ledPin = D2; // Here we say to which pin the LED is connected.
int speakerPin = D3; // Here we say the buzzer or piezo is connected to pin D3.

// The melody declarations.
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

// Interval is how long we wait for the alarm to go off.
int interval=5000;

// Tracks the time since last event fired.
unsigned long currentMillis=0;
unsigned long openMillis=0;

// Check if the door is open.
boolean isOpen = false;

// The amount of times the melody has been played.
int playCount=0;

// Connection to WiFi.
const char* ssid     = "********"; // Fill in your own SSID.
const char* password = "********"; // Fill in your password.

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

void setup() {

	pinMode(switchPin, INPUT_PULLUP);

	pinMode(ledPin, OUTPUT);

	digitalWrite(switchPin, HIGH);

	pinMode(speakerPin, OUTPUT);

	Serial.begin(9600);
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
	
	// Here we declare that the current time is going to be millis(), so this is the point that millis starts running.
	currentMillis = millis();

	if(digitalRead(switchPin) == HIGH && !isOpen) {
    	isOpen = true;
    	openMillis = currentMillis;
  }

  if(digitalRead(switchPin) == LOW && isOpen) {
    	isOpen = false;
    	digitalWrite(ledPin, LOW);
    	playCount=0;
  }
  
	// If the door has been open longer than 5 sec. it is okay to set off the alarm. 
  	if(currentMillis - openMillis >= interval && isOpen){
    	
    	playCount++;
     	digitalWrite(ledPin, HIGH);
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

  if(playCount >=2){ 
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
```























