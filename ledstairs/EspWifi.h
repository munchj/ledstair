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
	SERVER_READY,
	CHECKING_ESP_FIRMWARE,
	WAITING_AT,
	WAITING_SET_MODE,
	WAITING_RESET,
	WAITING_CHECK_CONNECTED,
	WAITING_JOIN_AP,
	WAITING_ENABLE_MULTIPLE_CONNECTIONS,
	WAITING_SERVER_START,
	WAITING_SET_SERVER_TIMEOUT,
	WAITING_STATUS,
	WAITING_MESSAGE_PROMPT,
	WAITING_MESSAGE_SENT,
	RETRY
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
