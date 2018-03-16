#include "Arduino.h"
#include "EspWifi.h"

char * WifiStateName[] =
{
	"SERVER_READY",
	"CHECKING_ESP_FIRMWARE",
	"WAITING_AT",
	"WAITING_SET_MODE",
	"WAITING_RESET",
	"WAITING_CHECK_CONNECTED",
	"WAITING_JOIN_AP",
	"WAITING_ENABLE_MULTIPLE_CONNECTIONS",
	"WAITING_SERVER_START",
	"WAITING_SET_SERVER_TIMEOUT",
	"WAITING_STATUS",
	"WAITING_MESSAGE_PROMPT",
	"WAITING_MESSAGE_SENT",
	"RETRY"
};

EspWifi::EspWifi()
{
	_error = false;
}


EspWifi::~EspWifi()
{

}

void ESPPrint(char * command) {
	DBG.print(">>>> ");
	DBG.print(command);
	ESP.print(command);
}
void ESPPrintLN(char * command) {
	DBG.print(">>>> ");
	DBG.println(command);
	ESP.println(command);
}
void ESPPrint(String command) {
	DBG.print(">>>> ");
	DBG.print(command);
	ESP.print(command);
}
void ESPPrintLN(String command) {
	DBG.print(">>>> ");
	DBG.println(command);
	ESP.println(command);
}
void ESPPrint(int command) {
	DBG.print(">>>> ");
	DBG.print(command);
	ESP.print(command);
}
void ESPPrintLN(int command) {
	DBG.print(">>>> ");
	DBG.println(command);
	ESP.println(command);
}


void EspWifi::transitionTo(WifiState newState, long timeout) {
	_state = newState;
	_timeout = millis() + timeout;
	DBG.print("[EspWifi][transitionTo] ");
	DBG.println(WifiStateName[newState]);
	_findIndex = 0;
}

bool EspWifi::timeout() {
	return millis() > _timeout;
}

bool EspWifi::find(char * term) {
	int len = strlen(term);
	bool found = false;
	bool success = false;
	if (ESP.available()) {
		_buffer[_findIndex++] = ESP.read();
		if (_findIndex >= len) {
			if (strncmp(_buffer + _findIndex - len, term, len) == 0) {
				found = true;
				success = true;
			}
			else if (strncmp(_buffer + _findIndex - len, ERRRN, len) == 0) {
				found = true;
				success = false;
			}
		}
	}
	else
	{
		//DBG.print(".");
	}

	_buffer[_findIndex] = 0;

	if (found) {
		DBG.print(_buffer);
	}
	return success;
}

bool EspWifi::readLine() {
	if (ESP.available()) {
		_buffer[_findIndex++] = ESP.read();
		if (_findIndex == BUFFER_SIZE)  _findIndex = 0;
		if (_findIndex > 1 && _buffer[_findIndex - 2] == 13 && _buffer[_findIndex - 1] == 10) {
			_buffer[_findIndex] = 0;
			_findIndex = 0;
			DBG.print(_buffer);
			return true;
		}
	}
	return false;
}

void EspWifi::tick() {
	switch (_state)
	{
	case RETRY:
	{
		//Initial/Retry state
		DBG.print("[ledstair] resetting ESP ...");
		analogWrite(8, 0);
		delay(10000);
		analogWrite(8, 170);
		DBG.println("done");
		delay(5000);

		//Check ESP firmware
		ESPPrintLN("AT+GMR");
		transitionTo(WAITING_AT, 5000);
		break;
	}
	case CHECKING_ESP_FIRMWARE:
	{
		if (timeout()) {
			//ESP did not reply, restart the wifi sequence from the start
			transitionTo(RETRY);
		}
		else if (find(")\r\n")) {
			//ESP replied, switching to station mode
			//1 = Station mode, 2 = AP mode, 3 = Station + AP dual mode
			ESPPrintLN("AT+CWMODE=1");
			transitionTo(WAITING_SET_MODE, 5000);
		}
		break;
	}
	case WAITING_SET_MODE:
	{
		if (timeout()) {
			//ESP did not reply, restart the wifi sequence from the start
			transitionTo(RETRY);
		}
		else if (find()) {
			//ESP set in station mode, connect to the wifi 
			ESPPrint("AT+CWJAP=\"");
			ESPPrint(_ssid);
			ESPPrint("\",\"");
			ESPPrint(_password);
			ESPPrintLN("\"");
			transitionTo(WAITING_JOIN_AP, 60000);
		}
		break;
	}
	case WAITING_JOIN_AP:
	{
		if (timeout()) {
			//ESP did not reply, restart the wifi sequence from the start
			transitionTo(RETRY);
		}
		else if (find("WIFI CONNECTED\r\n")) {
			//WIFI connected, enabled multiple connections
			ESPPrintLN("AT+CIPMUX=1");
			transitionTo(WAITING_ENABLE_MULTIPLE_CONNECTIONS, 60000);
		}
		break;
	}
	case WAITING_ENABLE_MULTIPLE_CONNECTIONS:
	{
		if (timeout()) {
			//ESP did not reply, restart the wifi sequence from the start
			transitionTo(RETRY);
		}
		else if (find()) {
			//Multiple connections ok, turn on TCP Service
			ESPPrint("AT+CIPSERVER=1,"); // turn on TCP service
			ESPPrintLN(_port);
			transitionTo(WAITING_SERVER_START, 1000);
		}
		break;
	}
	case WAITING_SERVER_START:
	{
		if (timeout()) {
			//ESP did not reply, restart the wifi sequence from the start
			transitionTo(RETRY);
		}
		else if (find()) {
			//Server started, set the timeout
			ESPPrintLN("AT+CIPSTO=30");
			transitionTo(WAITING_SET_SERVER_TIMEOUT, 1000);
		}
		break;
	}
	case WAITING_SET_SERVER_TIMEOUT:
	{
		if (timeout()) {
			//ESP did not reply, restart the wifi sequence from the start
			transitionTo(RETRY);
		}
		else if (find()) {
			//timeout set, checking status
			_stair->nextMode();
			ESPPrintLN("AT+CIFSR");
			transitionTo(WAITING_STATUS, 8000);
		}
		break;
	}
	case WAITING_STATUS:
	{
		if (timeout()) {
			transitionTo(RETRY);
		}
		else if (find()) {
			transitionTo(SERVER_READY);
		}
		break;
	}
	case WAITING_MESSAGE_PROMPT:
	{
		if (timeout()) {
			DBG.println("Failed to get prompt, removing message from queue anyways");
			EspHttpMessage message = _outgoingMessages.pop();
			transitionTo(SERVER_READY);
		}
		else if (find("> ")) {
			EspHttpMessage message = _outgoingMessages.pop();
			ESPPrint(message.getMessage());
			transitionTo(WAITING_MESSAGE_SENT, 2000);
		}
		break;
	}
	case WAITING_MESSAGE_SENT:
	{
		if (timeout()) {
			transitionTo(SERVER_READY);
		}
		else if (find()) {
			transitionTo(SERVER_READY);
		}
		break;
	}
	case SERVER_READY:
	{
		if (!_outgoingMessages.isEmpty()) {
			EspHttpMessage message = _outgoingMessages.peek();
			ESPPrint("AT+CIPSEND=");
			ESPPrint(message.getChannelId());
			ESPPrint(",");
			ESPPrintLN(message.getMessage().length());
			transitionTo(WAITING_MESSAGE_PROMPT, 2000);
		}
		else if (readLine())
		{
			int ch_id, packet_len;
			char *pb;
			if (strncmp(getBuffer(), "+IPD,", 5) == 0)
			{
				DBG.println("RECEIVED MESSAGE");
				sscanf(getBuffer() + 5, "%d,%d", &ch_id, &packet_len);
				if (packet_len > 0)
				{
					pb = getBuffer() + 5;
					while (*pb != ':') pb++;
					pb++;
					if (strncmp(pb, "GET", 3) == 0) {
						DBG.print("[EspWebserver] GET ");
						pb += 4;
						if (strncmp(pb, "/favicon.ico", 12) == 0) {
							DBG.println("favicon");
						}
						else if (strncmp(pb, "/nextmode/", 10) == 0) {
							int led, time;

							DBG.print("nextMode ");

							_stair->nextMode();
							send(OKRN, ch_id);
						}
						else if (strncmp(pb, "/spike/", 7) == 0) {
							int led, time;
							sscanf(pb + 7, "%d/%d", &led, &time);
							DBG.print("spike ");
							DBG.println(led);
							_stair->lightUp(led, 4095, time);
							send(OKRN, ch_id);
						}
						else if (strncmp(pb, "/setintensity/", 14) == 0) {
							int intensity;
							sscanf(pb + 14, "%d", &intensity);
							DBG.print("intensity ");
							DBG.println(intensity);
							_stair->setIntensity(intensity);
							send(OKRN, ch_id);
						}
						else if (strncmp(pb, "/setmode/", 9) == 0) {
							int mode;
							sscanf(pb + 9, "%d", &mode);
							DBG.print("mode ");
							DBG.println(mode);
							_stair->setMode(mode);
							send(OKRN, ch_id);
						}
						else if (strncmp(pb, "/", 1) == 0) {
							DBG.println("/");
							send(OKRN, ch_id);
						}
					}
				}
			}
			else if (strncmp(getBuffer() + 2, "CONNECT FAIL", 12) == 0)
			{
				DBG.println("Detected connect fail");
			}
			else if (strncmp(getBuffer() + 2, "CONNECT", 7) == 0)
			{
				DBG.println("Detected connect");
			}
		}

		break;
	}
	default:
	{

	}

	}
}



void EspWifi::connect(char * ssid, char * password, int port) {
	DBG.print("[EspWifi] connecting to ");
	DBG.println(ssid);
	_ssid = ssid;
	_password = password;
	_port = port;
	transitionTo(RETRY);
}


void EspWifi::send(String message, int channelId) {
	EspHttpMessage httpMessage;
	httpMessage.setContent(message);
	httpMessage.setChannelId(channelId);
	_outgoingMessages.push(httpMessage);
}

