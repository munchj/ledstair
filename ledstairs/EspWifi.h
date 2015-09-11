#define BUFFER_SIZE 512
#define OKRN "OK\r\n"

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

private:
	bool wait_for_esp_response(int timeout, char* term = OKRN);
	bool read_till_eol();

public:
	void connect(char * ssid, char * password);
	void printStatus();
	bool isConnected();
};

