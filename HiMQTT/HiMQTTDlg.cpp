//=====================================================================================
// SAMPLE IMPLEMENTATION FOR MQTT & JSON FOR MFC APPLICATION
// by lonycell@gmail.com
//=====================================================================================
#include "stdafx.h"
#include "HiMQTT.h"
#include "HiMQTTDlg.h"
#include "afxdialogex.h"

#include "CMqTT.h"
#include <json/json.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CHiMQTTDlg::CHiMQTTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIMQTT_DIALOG, pParent)
	, m_bConnected(false)
	, m_brocker_ip(_T("localhost"))
	, m_brocker_port(1883)
	, m_enable_pub(TRUE)
	, m_enable_sub(TRUE)
	, m_interval_secs(1)
	, m_topic_pub(_T("KR/INSPECT"))
	, m_topic_sub(_T("KR/INSPECT/#"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHiMQTTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSGS_PUB, m_cListPub);
	DDX_Control(pDX, IDC_LIST_MSGS_SUB, m_cListSub);
	DDX_Text(pDX, IDC_EDIT_BROCKER_IP, m_brocker_ip);
	DDX_Text(pDX, IDC_EDIT_BROCKER_PORT, m_brocker_port);
	DDV_MinMaxInt(pDX, m_brocker_port, 1024, 65535);
	DDX_Check(pDX, IDC_CHECK_ENABLE_PUB, m_enable_pub);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SUB, m_enable_sub);
	DDX_Text(pDX, IDC_EDIT_INTERVAL_PUB, m_interval_secs);
	DDV_MinMaxInt(pDX, m_interval_secs, 1, 60);
	DDX_Text(pDX, IDC_EDIT_TOPIC_PUB, m_topic_pub);
	DDV_MaxChars(pDX, m_topic_pub, 128);
	DDX_Text(pDX, IDC_EDIT_TOPIC_SUB, m_topic_sub);
	DDV_MaxChars(pDX, m_topic_sub, 128);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_button_connect);
	DDX_Control(pDX, IDC_CHECK_ENABLE_PUB, m_ctrl_EnablePub);
	DDX_Control(pDX, IDC_CHECK_ENABLE_SUB, m_ctrl_EnableSub);
	DDX_Control(pDX, IDC_LIST_LOG, m_cListLog);
}

BEGIN_MESSAGE_MAP(CHiMQTTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CHiMQTTDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_PUB, &CHiMQTTDlg::OnBnClickedButtonClearPub)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SUB, &CHiMQTTDlg::OnBnClickedButtonClearSub)
	ON_MESSAGE(MESSAGE_LOG_LOG, &CHiMQTTDlg::OnLog)
	ON_MESSAGE(MESSAGE_LOG_PUB, &CHiMQTTDlg::OnPubLog)
	ON_MESSAGE(MESSAGE_LOG_SUB, &CHiMQTTDlg::OnSubLog)
END_MESSAGE_MAP()

BOOL CHiMQTTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	USES_CONVERSION;	
	m_cListPub.InsertColumn(0, _T("#"), LVCFMT_LEFT, 50, -1);
	m_cListPub.InsertColumn(1, _T("Topic"), LVCFMT_LEFT, 200, -1);
	m_cListPub.InsertColumn(2, _T("Payload"), LVCFMT_LEFT, 600, -1);

	m_cListSub.InsertColumn(0, _T("#"), LVCFMT_LEFT, 50, -1);
	m_cListSub.InsertColumn(1, _T("Topic"), LVCFMT_LEFT, 200, -1);
	m_cListSub.InsertColumn(2, _T("Payload"), LVCFMT_LEFT, 600, -1);

	return TRUE;
}

void CHiMQTTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CHiMQTTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CHiMQTTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHiMQTTDlg::OnBnClickedButtonConnect()
{
	USES_CONVERSION;

	UpdateData();

	if (!m_bConnected) {

		if (m_PUB != NULL) {
			delete m_PUB;
			m_PUB = NULL;
		}
		m_PUB = new CMqTT(m_hWnd, "CID-0", T2A((LPCTSTR)m_brocker_ip), m_brocker_port);

		if (!m_PUB->connect(T2A(m_brocker_ip))) {
			m_bConnected = true;
			this->m_button_connect.SetWindowText(_T("끊기"));
			this->m_ctrl_EnablePub.EnableWindow(FALSE);

			Sleep(500);

			if (m_enable_pub) {
				SetTimer(TIMER_PUBLISH, m_interval_secs*1000, NULL);
				m_PUB->loop_start();
			}
			else {
				m_PUB->loop_stop();
				KillTimer(TIMER_PUBLISH);
			}
		}
		else {
			m_bConnected = false;
			this->m_button_connect.SetWindowText(_T("연결"));
			this->m_ctrl_EnablePub.EnableWindow(TRUE);
		}

		if (m_SUB != NULL) {
			delete m_SUB;
			m_SUB = NULL;
		}
		m_SUB = new CMqTT(m_hWnd, "CID-1", T2A((LPCTSTR)m_brocker_ip), m_brocker_port);

		if (!m_SUB->connect(T2A(m_brocker_ip))) {

			this->m_ctrl_EnableSub.EnableWindow(FALSE);

			if (this->m_enable_sub) {
				m_SUB->listen_message(T2A((LPCTSTR)m_topic_sub));
			}
		}
	}
	else {
		if (!m_PUB->disconnect()) {
			m_bConnected = false;
			this->m_button_connect.SetWindowText(_T("연결"));
			this->m_ctrl_EnablePub.EnableWindow(TRUE);
			KillTimer(TIMER_PUBLISH);

			if (m_PUB != NULL) {
				delete m_PUB;
				m_PUB = NULL;
			}
		}

		if (!m_SUB->disconnect()) {
			this->m_ctrl_EnableSub.EnableWindow(TRUE);

			USES_CONVERSION;
			if (this->m_enable_sub) {
				m_SUB->unsubscribe(NULL, T2A((LPCTSTR)m_topic_sub));
			}

			if (m_SUB != NULL) {
				delete m_SUB;
				m_SUB = NULL;
			}
		}
	}
}

void CHiMQTTDlg::OnBnClickedButtonClearPub()
{
	m_cListPub.DeleteAllItems();
}


void CHiMQTTDlg::OnBnClickedButtonClearSub()
{
	m_cListSub.DeleteAllItems();
}

//=========================================================================================
void CHiMQTTDlg::OnTimer(UINT nIDEvent)
{
	USES_CONVERSION;

	if (!m_enable_pub) {
		KillTimer(TIMER_PUBLISH);
		return;
	}

	if (nIDEvent != TIMER_PUBLISH) {
		return;
	}

	// string payload sample.
	//m_mqtt->send_message( T2A((LPCTSTR)m_topic_pub), "Hello I am client.", 0, false);

	// json payload sample.
	Json::Value inspect;
	inspect["tid"] = "M0001";
	inspect["machine"] = "A01";
	inspect["channel"] = 1;
	inspect["pass"] = true;

	Json::Value details;
	for (int i = 0; i < 10; i++) {
		Json::Value r;
		r["iid"] = i;
		r["val"] = i;
		r["pass"] = i % 3 ? true : false;
		details.append(r);
	}
	inspect["detail"] = details;

	CString topic;
	topic.Format(_T("%s/%s"), m_topic_pub, A2T(inspect["tid"].asCString()));

	Json::FastWriter w;
	m_PUB->send_message(T2A(topic), w.write(inspect).c_str(), 0, false);
}

afx_msg LRESULT CHiMQTTDlg::OnLog(WPARAM w, LPARAM l)
{
	const char* str = (const char*)w;

	USES_CONVERSION;
	m_cListLog.InsertString(0, A2T(str));

	return 0;
}

LRESULT CHiMQTTDlg::OnPubLog(WPARAM w, LPARAM l)
{
	const char* topic = (const char*)w;
	const char* payload = (const char*)l;

	USES_CONVERSION;
	
	CString s;
	int n = m_cListPub.GetItemCount();

	s.Format(_T("%d"), n);
	int i = m_cListPub.InsertItem(0, s);

	m_cListPub.SetItemText(i, 1, A2T(topic));
	m_cListPub.SetItemText(i, 2, A2T(payload));

	return 0;
}

LRESULT CHiMQTTDlg::OnSubLog(WPARAM w, LPARAM l)
{
	const struct mosquitto_message *m = (const struct mosquitto_message *)w;

	USES_CONVERSION;

	CString s;

	int n = m_cListSub.GetItemCount();

	s.Format(_T("%d"), n);
	int i = m_cListSub.InsertItem(0, s);

	m_cListSub.SetItemText(i, 1, A2T(m->topic));

	CString payload((LPCSTR)(m->payload), m->payloadlen);
	m_cListSub.SetItemText(i, 2, (LPCTSTR)payload.AllocSysString());

/*
	// TEST: parse json payload for DEMO topic(KR/INSPEC in this app)
	Json::Value val;
	Json::Reader reader;
	std::string json(T2A(payload.AllocSysString()));
	bool ok = reader.parse(json, val);

	if (ok) {
		CString tid = A2T(val["tid"].asCString());
		CString machine = A2T(val["machine"].asCString());
		int channel = val["channel"].asInt();
		bool pass = val["pass"].asBool();

		CString log;
		log.Format(
			_T("<<PARSED>> [ tid: %s, machine: %s, channel:%d, pass:%d ]"),
			tid, machine, channel, pass
		);

		this->m_cListLog.InsertString(0, log);
	}
	else {
		SendMessage(MESSAGE_LOG_LOG, (WPARAM)"parse to json is failed.", NULL);
	}
*/

	return S_OK;
}
//==================================================================================END