#ifndef ESP_CLIENT
#define ESP_CLIENT

#include <QueueList\QueueList.h>
#include <EspHttpMessage.h>

class EspClient
{
public:
	EspClient();
	~EspClient();

public:
	QueueList<EspHttpMessage> _incomingQueue;
	QueueList<EspHttpMessage> _outgoingQueue;
};

#endif