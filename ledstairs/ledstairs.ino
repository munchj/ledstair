#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EspWifi.h>
#include <EspWebserver.h>
#include <Stair.h>

#define DBG Serial
#define ESP Serial1

#define SSID "SlackersParadise"
#define PASS "irrigas adjutrici!& elementis*1337"
#define PORT 8080


Stair stair;
EspWifi wifi(&DBG, &ESP);
EspWebserver webserver(&DBG);


void setup() {
	DBG.begin(9600);
	ESP.begin(115200);



	DBG.println("[ledstair] starting up...");

	stair.begin();
	wifi.setStair(&stair);
	webserver.setStair(&stair);
	wifi.connect(SSID, PASS);
	webserver.begin(&wifi, PORT);
	wifi.printStatus();
	
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);

	stair.flash();
}



void loop() {
	if (wifi.isConnected()) {
		stair.tick();
		webserver.tick();
	}
	else {
		//lightAll(4095);
		//delay(50);
		//lightAll(0);
		//delay(50);
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

