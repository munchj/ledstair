#include "EspHttpMessage.h"



EspHttpMessage::EspHttpMessage()
{
	_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n";
}


EspHttpMessage::~EspHttpMessage()
{
}


String EspHttpMessage::getMessage() {
	String message = _header;
	message += "Content-Length:";
	message += (int)(_content.length());
	message += "\r\n\r\n";
	message += _content;
	return message;
}