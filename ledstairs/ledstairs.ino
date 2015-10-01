#include <Constants.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include <EspWifi.h>
#include <Stair.h>


Stair stair;
EspWifi wifi;


void setup() {
	DBG.begin(9600);
	ESP.begin(115200);

	DBG.println("[ledstair] starting up...");

	stair.begin();
	wifi.setStair(&stair);
	wifi.connect(SSID, PASS, PORT);

	pinMode(A0, INPUT);
	pinMode(A1, INPUT);

	stair.flash();
}



void loop() {
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
