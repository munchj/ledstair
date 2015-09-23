#include <WString.h>

#ifndef ESP_HTTP_MESSAGE
#define ESP_HTTP_MESSAGE

enum MessageState {
	NONE,
	RECEIVED,
	WAITING_SEND,
	WAITING_PROMPT,
	SENT
};

class EspHttpMessage
{
public:
	EspHttpMessage();
	~EspHttpMessage();

private:
	int _channelId;
	String _header;
	String _content;
	MessageState _state;

public:
	
	MessageState getState() { return _state; }
	String getMessage();
	int getChannelId() { return _channelId; }

public:
	void setState(MessageState state) { _state = state; }
	void setContent(String content) { _content = content; }
	void setChannelId(int channelId) { _channelId = channelId; }
};

#endif
