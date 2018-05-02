#pragma once

#include <mosquittopp.h>


#define MESSAGE_LOG_LOG (WM_APP + 1001)
#define MESSAGE_LOG_PUB (WM_APP + 1002)
#define MESSAGE_LOG_SUB (WM_APP + 1003)

class CMqTT : public mosqpp::mosquittopp
{
private:
	HWND m_hWnd;

public:
	CMqTT(HWND, const char *id, const char *host, int port = 1883, int keepalive = 120);
	~CMqTT();

	void on_error();
	void on_connect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_subcribe(int mid, int qos_count, const int *granted_qos);
	void on_disconnect(int rc);
	void on_publish(int mid);
	void on_unsubscribe(int mid);
	void on_log(int level, const char * str);

	bool send_message(const  char * _topic, const char *_payload, const int qos = 0, const bool retain = false);
	bool listen_message(const  char * _topic, const int qos = 2);
	bool listen_stop(const char * _topic);

};

