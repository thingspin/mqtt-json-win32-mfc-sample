#include "stdafx.h"
#include "CMqTT.h"

CMqTT::CMqTT(HWND hWnd, const char *id, const char *host, int port, int keepalive) 
	: mosquittopp(id)
{
	m_hWnd = hWnd;

	mosqpp::lib_init();
}

void CMqTT::on_error(void)
{
	//TRACE("MQTT> ERROR\n");
}

void CMqTT::on_connect(int rc)
{
	PostMessage(m_hWnd, MESSAGE_LOG_LOG, (WPARAM)"on_connect", (LPARAM)rc);
}

void CMqTT::on_subcribe(int mid, int qos_count, const int *granted_qos)
{
	PostMessage(m_hWnd, MESSAGE_LOG_LOG, (WPARAM)"on_unsubscribe", (LPARAM)mid);
}

void CMqTT::on_unsubscribe(int mid)
{
	PostMessage(m_hWnd, MESSAGE_LOG_LOG, (WPARAM)"on_unsubscribe", (LPARAM)mid);
}

void CMqTT::on_message(const struct mosquitto_message *message)
{
	if (message->mid < 0) {
		return;
	}

	SendMessage(m_hWnd, MESSAGE_LOG_SUB, (WPARAM)message, (LPARAM)message->mid);

}

void CMqTT::on_disconnect(int rc)
{
	PostMessage(m_hWnd, MESSAGE_LOG_LOG, (WPARAM)"on_disconnect", (LPARAM)rc);
}

void CMqTT::on_publish(int mid)
{	
	PostMessage(m_hWnd, MESSAGE_LOG_LOG, (WPARAM)"on_publish", (LPARAM)mid);
}

void CMqTT::on_log(int level, const char* str)
{
	//SendMessage(m_hWnd, MESSAGE_LOG_LOG, (WPARAM)str, NULL);
}

bool CMqTT::send_message(const  char * _topic, const char *_payload, const int qos, const bool retain)
{
	int ret = publish(NULL, _topic, strlen(_payload), _payload, qos, retain);

	if (!ret) {
		SendMessage(m_hWnd, MESSAGE_LOG_PUB, (WPARAM)_topic, (LPARAM)_payload);
	}

	return (ret == MOSQ_ERR_SUCCESS);
}

bool CMqTT::listen_message(const char * _topic, const int qos)
{
	int ret = subscribe(NULL, _topic, qos);
	loop_start();

	return (ret == MOSQ_ERR_SUCCESS);
}

bool CMqTT::listen_stop(const char* _topic)
{
	int ret = unsubscribe(NULL, _topic);
	loop_stop();

	return (ret == MOSQ_ERR_SUCCESS);
}

CMqTT::~CMqTT()
{
	mosqpp::lib_cleanup();
}
