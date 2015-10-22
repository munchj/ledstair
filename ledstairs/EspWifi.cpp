#include "Arduino.h"
#include "EspWifi.h"

char * WifiStateName[] =
{
	"BEGINNING",
	"SERVER_READY",
	"WAITING_CONNECTION_SEQUENCE",
	"WAITING_AT",
	"AT_OK",
	"AT_KO",
	"WAITING_SET_MODE",
	"SET_MODE_OK",
	"SET_MODE_KO",
	"WAITING_RESET",
	"RESET_OK",
	"RESET_KO",
	"WAITING_CHECK_CONNECTED",
	"CHECK_CONNECTED_OK",
	"CHECK_CONNECTED_KO",
	"WAITING_JOIN_AP",
	"JOIN_AP_OK",
	"JOIN_AP_KO",
	"WAITING_ENABLE_MULTIPLE_CONNECTIONS",
	"ENABLE_MULTIPLE_CONNECTIONS_OK",
	"ENABLE_MULTIPLE_CONNECTIONS_KO",
	"WAITING_SERVER_START",
	"SERVER_START_OK",
	"SERVER_START_KO",
	"WAITING_SET_SERVER_TIMEOUT",
	"SET_SERVER_TIMEOUT_OK",
	"SET_SERVER_TIMEOUT_KO",
	"WAITING_SEND_MESSAGE",
	"SEND_MESSAGE_OK",
	"SEND_MESSAGE_KO",
	"INCOMING_MESSAGE",
	"WAITING_STATUS",
	"STATUS_OK",
	"STATUS_KO",
	"WAITING_MESSAGE_PROMPT",
	"WAITING_MESSAGE_SENT"
};

EspWifi::EspWifi()
{
	_error = false;
}


EspWifi::~EspWifi()
{

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

	_buffer[_findIndex] = 0;

	//if (found) {
		DBG.print(_buffer);
	//}
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
	case BEGINNING:
	{
		break;
	}
	case WAITING_CONNECTION_SEQUENCE:
	{
		ESP.println("AT+GMR");
		transitionTo(WAITING_AT, 1000);
		break;
	}
	case WAITING_AT:
	{
		if (timeout()) {
			transitionTo(AT_KO);
		}
		else if (find()) {
			transitionTo(AT_OK);
		}
		break;
	}
	case AT_OK:
	{
		ESP.println("AT+CWMODE=1");
		transitionTo(WAITING_SET_MODE, 1000);
		break;
	}
	case AT_KO:
	{
		break;
	}
	case WAITING_SET_MODE:
	{
		if (timeout()) {
			transitionTo(SET_MODE_KO);
		}
		else if (find()) {
			transitionTo(SET_MODE_OK);
		}
		break;
	}
	case SET_MODE_OK:
	{
		ESP.println("AT+RST");
		transitionTo(WAITING_RESET, 2000);
		break;
	}
	case SET_MODE_KO:
	{
		break;
	}
	case WAITING_RESET:
	{
		if (timeout()) {
			transitionTo(RESET_KO);
		}
		else if (find()) {
			transitionTo(RESET_OK);
		}
		break;
	}
	case RESET_OK:
	{
		//_esp->println("AT+CWJAP?");
		transitionTo(WAITING_CHECK_CONNECTED, 5000);
		break;
	}
	case RESET_KO:
	{
		break;
	}
	case WAITING_CHECK_CONNECTED:
	{
		if (timeout()) {
			transitionTo(CHECK_CONNECTED_KO);
		}
		else if (find()) {
			DBG.println(_buffer + 8);
			if (strncmp(_buffer + 8, _ssid, strlen(_ssid)))
			{
				transitionTo(CHECK_CONNECTED_OK);
			}
			else {
				transitionTo(CHECK_CONNECTED_KO);
			}
		}
		break;
	}
	case CHECK_CONNECTED_OK:
	{
		transitionTo(JOIN_AP_OK);
		break;
	}
	case CHECK_CONNECTED_KO:
	{
		ESP.print("AT+CWJAP=\"");
		ESP.print(_ssid);
		ESP.print("\",\"");
		ESP.print(_password);
		ESP.println("\"");
		transitionTo(WAITING_JOIN_AP, 10000);

		break;
	}
	case WAITING_JOIN_AP:
	{
		if (timeout()) {
			transitionTo(JOIN_AP_KO);
		}
		else if (find()) {
			transitionTo(JOIN_AP_OK);
		}
		break;
	}
	case JOIN_AP_OK:
	{
		ESP.println("AT+CIPMUX=1");
		transitionTo(WAITING_ENABLE_MULTIPLE_CONNECTIONS, 1000);
		break;
	}
	case JOIN_AP_KO:
	{
		//_esp->println("AT+CWJAP?");
		//find();
		break;
	}
	case WAITING_ENABLE_MULTIPLE_CONNECTIONS:
	{
		if (timeout()) {
			transitionTo(ENABLE_MULTIPLE_CONNECTIONS_KO);
		}
		else if (find()) {
			transitionTo(ENABLE_MULTIPLE_CONNECTIONS_OK);
		}
		break;
	}
	case ENABLE_MULTIPLE_CONNECTIONS_OK:
	{
		ESP.print("AT+CIPSERVER=1,"); // turn on TCP service
		ESP.println(_port);
		transitionTo(WAITING_SERVER_START, 1000);
		break;
	}
	case ENABLE_MULTIPLE_CONNECTIONS_KO:
	{
		break;
	}
	case WAITING_SERVER_START:
	{
		if (timeout()) {
			transitionTo(SERVER_START_KO);
		}
		else if (find()) {
			transitionTo(SERVER_START_OK);
		}
		break;
	}
	case SERVER_START_OK:
	{
		ESP.println("AT+CIPSTO=30");
		transitionTo(WAITING_SET_SERVER_TIMEOUT, 1000);
		break;
	}
	case SERVER_START_KO:
	{
		break;
	}
	case WAITING_SET_SERVER_TIMEOUT:
	{
		if (timeout()) {
			transitionTo(SET_SERVER_TIMEOUT_KO);
		}
		else if (find()) {
			transitionTo(SET_SERVER_TIMEOUT_OK);
		}
		break;
	}
	case SET_SERVER_TIMEOUT_OK:
	{
		ESP.println("AT+CIFSR");
		transitionTo(WAITING_STATUS, 5000);
		break;
	}
	case SET_SERVER_TIMEOUT_KO:
	{
		break;
	}
	case WAITING_SEND_MESSAGE:
	{
		break;
	}
	case SEND_MESSAGE_OK:
	{
		break;
	}
	case SEND_MESSAGE_KO:
	{
		break;
	}
	case INCOMING_MESSAGE:
	{
		break;
	}
	case WAITING_STATUS:
	{
		if (timeout()) {
			transitionTo(STATUS_KO);
		}
		else if (find()) {
			transitionTo(STATUS_OK);
		}
		break;
	}
	case STATUS_OK:
	{
		transitionTo(SERVER_READY);
		break;
	}
	case STATUS_KO:
	{
		transitionTo(SERVER_READY);
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
			ESP.print(message.getMessage());
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
			ESP.print("AT+CIPSEND=");
			ESP.print(message.getChannelId());
			ESP.print(",");
			ESP.println(message.getMessage().length());
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
						else if (strncmp(pb, "/spike/", 7) == 0) {
							int led, time;
							sscanf(pb + 7, "%d/%d", &led, &time);
							DBG.print("spike ");
							DBG.println(led);
							_stair->lightUp(led, 4095, time);
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
	transitionTo(WAITING_CONNECTION_SEQUENCE);
}


void EspWifi::send(String message, int channelId) {
	EspHttpMessage httpMessage;
	httpMessage.setContent(message);
	httpMessage.setChannelId(channelId);
	_outgoingMessages.push(httpMessage);
}

