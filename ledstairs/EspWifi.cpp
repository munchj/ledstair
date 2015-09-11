#include "Arduino.h"
#include "EspWifi.h"
#include "HardwareSerial.h"

EspWifi::EspWifi(HardwareSerial * dbg, HardwareSerial * esp)
{
	_dbg = dbg;
	_esp = esp;
	_error = false;
}


EspWifi::~EspWifi()
{
	_esp = NULL;
	_dbg = NULL;

}


bool EspWifi::wait_for_esp_response(int timeout, char* term) {
	unsigned long t = millis();
	bool found = false;
	int i = 0;
	int len = strlen(term);
	// wait for at most timeout milliseconds
	// or if OK\r\n is found
	while (millis() < t + timeout) {
		if (_esp->available()) {
			_buffer[i++] = _esp->read();
			if (i >= len) {
				if (strncmp(_buffer + i - len, term, len) == 0) {
					found = true;
					break;
				}
			}
		}
	}
	_buffer[i] = 0;
	_dbg->print(_buffer);
	return found;

}
bool EspWifi::read_till_eol() {
	static int i = 0;
	if (_esp->available()) {
		_buffer[i++] = _esp->read();
		if (i == BUFFER_SIZE)  i = 0;
		if (i > 1 && _buffer[i - 2] == 13 && _buffer[i - 1] == 10) {
			_buffer[i] = 0;
			i = 0;
			_dbg->print(_buffer);
			return true;
		}
	}
	return false;

}

void EspWifi::printStatus() {
	// print device IP address
	_dbg->print("device ip addr:");
	_esp->println("AT+CIFSR");
	wait_for_esp_response(1000);
}

bool EspWifi::isConnected() {
	return !_error;
}

void EspWifi::connect(char * ssid, char * password) {
	_ssid = ssid;
	_password = password;

	_esp->println("AT");
	if (wait_for_esp_response(1000)) {
		_dbg->println("ESP board properly connected");
		// set mode 1 (client)
		_esp->println("AT+CWMODE=1");
		wait_for_esp_response(1000);

		// reset WiFi module
		_esp->print("AT+RST\r\n");
		wait_for_esp_response(1500);
		delay(3000);

		// join AP
		_esp->print("AT+CWJAP=\"");
		_esp->print(_ssid);
		_esp->print("\",\"");
		_esp->print(_password);
		_esp->println("\"");
		// this may take a while, so wait for 5 seconds
		if (wait_for_esp_response(10000)) {
			_dbg->println("Successfuly connected to Wifi");
		}
		else {
			_dbg->println("Error conntecting to Wifi...");
			_error = true;
		}


	}

	//// start server
	//_esp->println("AT+CIPMUX=1");
	//wait_for_esp_response(1000);

	//_esp->print("AT+CIPSERVER=1,"); // turn on TCP service
	//_esp->println(PORT);
	//wait_for_esp_response(1000);


	//_esp->println("AT+CIPSTO=30");
	//wait_for_esp_response(1000);
}


