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