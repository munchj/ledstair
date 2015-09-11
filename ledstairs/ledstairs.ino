#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EspWifi.h>


#define DBG Serial
#define ESP Serial1

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
EspWifi wifi = EspWifi(&DBG, &ESP);


#define SSID "SlackersParadise"
#define PASS "irrigas adjutrici!& elementis*1337"
#define PORT 8080



void setup() {
	DBG.begin(9600);
	ESP.begin(115200);

	DBG.println("[ledstair] starting up...");

	wifi.connect(SSID, PASS);



	pwm.begin();
	pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

	uint8_t twbrbackup = TWBR;
	TWBR = 12; // upgrade to 400KHz!

	for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++) {
		pwm.setPWM(pwmnum, 0, 0);
	}

	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
}



void loop() {
	if (wifi.isConnected()) {


	}
	else {
		lightAll(4095);
		delay(50);
		lightAll(0);
		delay(50);
	}

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

	//int ch_id, packet_len;
	//char *pb;
	//if (read_till_eol()) {
	//	if (strncmp(buffer, "+IPD,", 5) == 0) {
	//		// request: +IPD,ch,len:data
	//		sscanf(buffer + 5, "%d,%d", &ch_id, &packet_len);
	//		if (packet_len > 0) {
	//			// read serial until packet_len character received
	//			// start from :
	//			pb = buffer + 5;
	//			while (*pb != ':') pb++;
	//			pb++;
	//			if (strncmp(pb, "GET /favicon.ico", 16) == 0) {
	//			}
	//			else if (strncmp(pb, "GET /", 5) == 0) {
	//				wait_for_esp_response(1000);
	//				//DBG.println("-> serve homepage");
	//				//serve_homepage(ch_id);
	//				DBG.println("-> light sequence");
	//				serve_reply(ch_id);
	//				if (b) { lightAll(0); }
	//				else { lightAll(4000); }
	//				b = !b;
	//			}
	//		}
	//	}
	//}


	//delay(5000);
}



//void serve_reply(int ch_id) {
//	String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";
//
//	String content = "OK";
//	// output the value of each analog input pin
//
//
//	header += "Content-Length:";
//	header += (int)(content.length());
//	header += "\r\n\r\n";
//	ESP.print("AT+CIPSEND=");
//	ESP.print(ch_id);
//	ESP.print(",");
//	ESP.println(header.length() + content.length());
//	if (wait_for_esp_response(2000, "> ")) {
//		ESP.print(header);
//		ESP.print(content);
//	}
//	else {
//		ESP.print("AT+CIPCLOSE=");
//		ESP.println(ch_id);
//	}
//}
//
//
//void serve_homepage(int ch_id) {
//	String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";
//
//	String content = "";
//	// output the value of each analog input pin
//	for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
//		int sensorReading = analogRead(analogChannel);
//		content += "analog input ";
//		content += analogChannel;
//		content += " is ";
//		content += sensorReading;
//		content += "<br />\n";
//	}
//
//	header += "Content-Length:";
//	header += (int)(content.length());
//	header += "\r\n\r\n";
//	ESP.print("AT+CIPSEND=");
//	ESP.print(ch_id);
//	ESP.print(",");
//	ESP.println(header.length() + content.length());
//	if (wait_for_esp_response(2000, "> ")) {
//		ESP.print(header);
//		ESP.print(content);
//	}
//	else {
//		ESP.print("AT+CIPCLOSE=");
//		ESP.println(ch_id);
//	}
//}

int pwmMap[16] = { 0, 2, 4, 6, 8, 10, 12 ,14, 15, 13, 11, 9, 7, 5, 3, 1 };
void setMappedPWM(uint16_t ledPin, int16_t value)
{
	pwm.setPWM(pwmMap[ledPin], 0, value);
}

void lightFromTop(uint8_t ledPin)
{
	for (int8_t pwmnum = 15; pwmnum >= ledPin; pwmnum--) {
		lightSpikeUp(pwmnum);
		lightSpikeDown(pwmnum);
	}
	lightSpikeUp(ledPin);
}

void lightSpikeDown(uint8_t ledPin)
{
	for (int16_t i = 4096; i >= 0; i -= 64) {
		setMappedPWM(ledPin, i);
	}
}

void lightSpikeUp(uint8_t ledPin) {
	for (uint16_t i = 0; i < 4096; i += 64) {
		setMappedPWM(ledPin, i);
	}
}

void smoothDown() {
	for (int16_t i = 4096; i >= 0; i -= 1) {
		for (int8_t ledPin = 15; ledPin >= 0; ledPin--) {
			setMappedPWM(ledPin, i);
		}
	}
}

void lightAll(int16_t value) {

	for (int8_t ledPin = 15; ledPin >= 0; ledPin--) {
		setMappedPWM(ledPin, value);
	}

}

void lightSequence1()
{
	for (int8_t ledPin = 0; ledPin < 16; ledPin++) {
		lightFromTop(ledPin);
	}
	delay(5000);
	smoothDown();

}