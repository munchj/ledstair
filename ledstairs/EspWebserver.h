#include <Constants.h>
#include <EspWifi.h>
#include <Stair.h>

#ifndef ESPWEBSERVER
#define ESPWEBSERVER

class EspWebserver
{
public:
	EspWebserver();
	~EspWebserver();
private:
	EspWifi * _wifi;
	Stair * _stair;
public:
	void setWifi(EspWifi * wifi) { _wifi = wifi; }
	void setStair(Stair * stair) { _stair = stair; }
public:
	void tick();
private:
	void serveHomepage(int ch_id);
};

#endif
