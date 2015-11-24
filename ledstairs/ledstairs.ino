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

void setup() {
	DBG.begin(9600);
	ESP.begin(115200);

	DBG.println("[ledstair] starting up...");
	stair.begin();
	wifi.setStair(&stair);
	//wifi.connect(SSID, PASS, PORT);
	
	sensor1.begin(&stair);
	sensor2.begin(&stair);
	sensor3.begin(&stair);
	
	pinMode(A13, INPUT);
	
	pinMode(A7, INPUT_PULLUP);
	button.attach(A7);
	button.interval(10);
}


void loop() {
	sensor1.tick();
	sensor2.tick();
	sensor3.tick();
	//wifi.tick();
	stair.tick();
	
	button.update();
	if (button.fell()) {
		stair.nextMode();
	}
}
