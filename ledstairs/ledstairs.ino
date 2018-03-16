#include <Constants.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include <EspWifi.h>
#include <Stair.h>
#include <MotionSensor.h>
#include <Bounce2.h>


Stair stair;
EspWifi wifi;
MotionSensor sensor1(A0);
MotionSensor sensor2(A1);
MotionSensor sensor3(A2);
Bounce button = Bounce();
int incomingByte = 0;
bool buttonChanged = false;
int currentButtonValue = HIGH;
int lastButtonValue = HIGH;
unsigned long buttonDownTime = 0;

void setup() {
	DBG.begin(9600);
	ESP.begin(115200);

	DBG.println("[ledstair] starting up...");
	//Initializing stair routine
	stair.begin();
	stair.setWifi(&wifi);

	//Initializing wifi routine
	wifi.setStair(&stair);
	wifi.connect(SSID, PASS, PORT);

	//Start sensors
	sensor1.begin(&stair);
	sensor2.begin(&stair);
	sensor3.begin(&stair);
	
	//Input pin for sound sensor
	pinMode(A13, INPUT);
	
	//Input pin for push button
	pinMode(A7, INPUT_PULLUP);
	button.attach(A7);
	button.interval(5);
}


void loop() {
	//Update sensors
	sensor1.tick();
	sensor2.tick();
	sensor3.tick();
	//Update wifi routine
	wifi.tick();
	//Update stairs routine
	stair.tick();
	//Check for button push
	buttonChanged = button.update();
	currentButtonValue = button.read();
	if (currentButtonValue != lastButtonValue) {
		if (currentButtonValue == HIGH) {
			unsigned long pressDuration = millis() - buttonDownTime;
			if (pressDuration < 400) {
				stair.nextMode();
			}
			else if (pressDuration < 2000) {
				stair.nextIntensity();
			}
			else {
				stair.setMode(DEFAULT_LIGHT_MODE);
			}
			
		}
		else {
			buttonDownTime = millis();
		}
		
	}
	lastButtonValue = currentButtonValue;


	//Read serial input
	if (DBG.available() > 0) {
		incomingByte = DBG.read();
		switch (incomingByte) {
		case 'n' :
			stair.nextMode();
			break;
		}
	}
}
