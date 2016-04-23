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






















