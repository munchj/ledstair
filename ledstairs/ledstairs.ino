#include <Constants.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include <EspWifi.h>
#include <Stair.h>
#include <MotionSensor.h>
#include <Bounce2.h>
#include <MemoryFree.h>

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
	

	delay(2000);
	DBG.print("[ledstair] resetting ESP ...");
	analogWrite(8, 0);
	delay(10000);
	analogWrite(8, 170);
	DBG.println("done");
	stair.begin();
	stair.setWifi(&wifi);
	wifi.setStair(&stair);



	wifi.connect(SSID, PASS, PORT);
	
	sensor1.begin(&stair);
	sensor2.begin(&stair);
	sensor3.begin(&stair);
	
	pinMode(A13, INPUT);
	
	pinMode(A7, INPUT_PULLUP);
	button.attach(A7);
	button.interval(5);
}


void loop() {
	sensor1.tick();
	sensor2.tick();
	sensor3.tick();
	wifi.tick();
	stair.tick();
	
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


	if (DBG.available() > 0) {
		incomingByte = DBG.read();
		switch (incomingByte) {
		case 'n' :
			stair.nextMode();
			break;
		}
	}
	//DBG.print("Free memory:");
	//DBG.println(freeMemory());
}
