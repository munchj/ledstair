#include "EspWebserver.h"


EspWebserver::EspWebserver(HardwareSerial * dbg)
{
	_dbg = dbg;
}


EspWebserver::~EspWebserver()
{
}


void EspWebserver::tick() {
	//int ch_id, packet_len;
	//char *pb;
	//if (_wifi->read_till_eol()) {
	//	if (strncmp(_wifi->getBuffer(), "+IPD,", 5) == 0) {
	//		_dbg->println("[EspWebserver] new request");
	//		sscanf(_wifi->getBuffer() + 5, "%d,%d", &ch_id, &packet_len);
	//		if (packet_len > 0) {
	//			pb = _wifi->getBuffer() + 5;
	//			while (*pb != ':') pb++;
	//			pb++;
	//			if (strncmp(pb, "GET", 3) == 0) {
	//				_dbg->print("[EspWebserver] GET ");
	//				pb += 4;
	//				if (strncmp(pb, "/favicon.ico", 12) == 0) {
	//					_dbg->println("favicon");
	//				}
	//				else if (strncmp(pb, "/spike/", 7) == 0) {
	//					int led, time;
	//					sscanf(pb + 7, "%d/%d", &led, &time);
	//					_dbg->print("spike ");
	//					_dbg->println(led);
	//					_stair->lightUp(led, 4095, time);
	//					_wifi->wait_for_esp_response(1000);
	//					serveHomepage(ch_id);
	//				}
	//				else if (strncmp(pb, "/", 1) == 0) {
	//					_dbg->println("/");
	//					_wifi->wait_for_esp_response(1000);
	//					serveHomepage(ch_id);
	//				}
	//			}
	//		}
	//	}
	//}
}

void EspWebserver::serveHomepage(int ch_id) {
	//String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";

	//String content = "OK";

	//header += "Content-Length:";
	//header += (int)(content.length());
	//header += "\r\n\r\n";

	//_wifi->send(ch_id, header + content);
}