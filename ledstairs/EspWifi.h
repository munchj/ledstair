#include <Constants.h>
#include <Wire.h>
#include <Stair.h>
#include <QueueList\QueueList.h>
#include <EspHttpMessage.h>


#define BUFFER_SIZE 1024
#define OKRN "OK\r\n"
#define ERRRN " ERROR\r\n"
#define MAX_CLIENTS 4

#ifndef ESPWIFI
#define ESPWIFI


enum WifiState {
	BEGINNING,
	SERVER_READY,
	WAITING_CONNECTION_SEQUENCE,
	WAITING_AT,
	AT_OK,
	AT_KO,
	WAITING_SET_MODE,
	SET_MODE_OK,
	SET_MODE_KO,
	WAITING_RESET,
	RESET_OK,
	RESET_KO,
	WAITING_CHECK_CONNECTED,
	CHECK_CONNECTED_OK,
	CHECK_CONNECTED_KO,
	WAITING_JOIN_AP,
	JOIN_AP_OK,
	JOIN_AP_KO,
	WAITING_ENABLE_MULTIPLE_CONNECTIONS,
	ENABLE_MULTIPLE_CONNECTIONS_OK,
	ENABLE_MULTIPLE_CONNECTIONS_KO,
	WAITING_SERVER_START,
	SERVER_START_OK,
	SERVER_START_KO,
	WAITING_SET_SERVER_TIMEOUT,
	SET_SERVER_TIMEOUT_OK,
	SET_SERVER_TIMEOUT_KO,
	WAITING_SEND_MESSAGE,
	SEND_MESSAGE_OK,
	SEND_MESSAGE_KO,
	INCOMING_MESSAGE,
	WAITING_STATUS,
	STATUS_OK,
	STATUS_KO,
	WAITING_MESSAGE_PROMPT,
	WAITING_MESSAGE_SENT,
	WAITING_SET_IP,
	SET_IP_OK
};



class EspWifi
{
public:
	EspWifi();
	~EspWifi();

private:
	char * _ssid;
	char * _password;
	int _port;


private:
	char _buffer[BUFFER_SIZE];
	bool _error;
	Stair * _stair;
	QueueList<EspHttpMessage> _incomingMessages;
	QueueList<EspHttpMessage> _outgoingMessages;

public:
	void setStair(Stair * stair) { _stair = stair; }

public:
	void tick();

public:
	char * getBuffer() { return _buffer; }

public:
	void send(String message, int channelId);



public:
	void connect(char * ssid, char * password, int port);


private:
	WifiState _state;
	long _timeout;
	void transitionTo(WifiState newState, long timeout = 0);
	bool timeout();
	bool find(char * term = OKRN);
	bool readLine();
	int _findIndex;
public:
	WifiState getState() { return _state; }
};

#endif
