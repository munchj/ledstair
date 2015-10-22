#include <Constants.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include <EspWifi.h>
#include <Stair.h>
#include <MotionSensor.h>


Stair stair;
EspWifi wifi;
MotionSensor sensor1(A0);
MotionSensor sensor2(A0);

void setup() {
	DBG.begin(9600);
	ESP.begin(115200);

	DBG.println("[ledstair] starting up...");

	stair.begin();
	wifi.setStair(&stair);
	wifi.connect(SSID, PASS, PORT);
	
	sensor1.begin(&stair);
	sensor2.begin(&stair);

	//stair.flash();
}



void loop() {
	sensor1.tick();
	sensor2.tick();
	wifi.tick();
	stair.tick();
	

	//for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum += 2) {
	//	for (uint16_t i = 0; i<4096; i += 8) {
	//		pwm.setPWM(pwmnum, 0, i);
	//		delay(1);
	//	}
	//}
	//for (uint8_t pwmnum = 1; pwmnum < 16; pwmnum += 2) {
	//	for (uint16_t i = 0; i<4096; i += 8) {
	//		pwm.setPWM(pwmnum, 0, i);
	//		delay(1);
	//	}
	//}
	//for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum += 2) {
	//	for (int16_t i = 4096; i >= 0; i -= 8) {
	//		pwm.setPWM(pwmnum, 0, i);
	//		delay(1);
	//	}
	//}
	//for (uint8_t pwmnum = 1; pwmnum < 16; pwmnum += 2) {
	//	for (int16_t i = 4096; i >= 0; i -= 8) {
	//		pwm.setPWM(pwmnum, 0, i);
	//		delay(1);
	//	}
	//}


}
