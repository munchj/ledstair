#include <Wire.h>
#include <Stair.h>
#define BUFFER_SIZE 512
#define OKRN "OK\r\n"

#ifndef ESPWIFI
#define ESPWIFI

class HardwareSerial;

class EspWifi
{
public:
	EspWifi(HardwareSerial * dbg, HardwareSerial * esp);
	~EspWifi();

private:
	char * _ssid;
	char * _password;

private:
	HardwareSerial* _esp;
	HardwareSerial* _dbg;
	char _buffer[BUFFER_SIZE];
	bool _error;
	Stair * _stair;

public:
	void setStair(Stair * stair) { _stair = stair; }

public:
	bool wait_for_esp_response(int timeout, char* term = OKRN);
	bool read_till_eol();

public:
	void tick();

public:
	char * getBuffer() { return _buffer; }

public:
	void send(int ch_id, String message);
	void startServer(int port);
	

public:
	void connect(char * ssid, char * password);
	void printStatus();
	bool isConnected();
};

#endif
