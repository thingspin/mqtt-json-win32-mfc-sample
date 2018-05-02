//=====================================================================================
// SAMPLE IMPLEMENTATION FOR MQTT & JSON FOR MFC APPLICATION
// by lonycell@gmail.com
//=====================================================================================
#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CMqTT;

#define TIMER_PUBLISH (WM_APP + 1000)

class CHiMQTTDlg : public CDialogEx
{
public:
	CHiMQTTDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIMQTT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	bool m_bConnected;

protected:
	HICON m_hIcon;
	CMqTT * m_PUB;
	CMqTT * m_SUB;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnLog(WPARAM w, LPARAM l);
	afx_msg LRESULT OnPubLog(WPARAM w, LPARAM l);
	afx_msg LRESULT OnSubLog(WPARAM w, LPARAM l);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();

	CListCtrl m_cListPub;
	CListCtrl m_cListSub;
	CString m_brocker_ip;
	int m_brocker_port;
	BOOL m_enable_pub;
	BOOL m_enable_sub;
	int m_interval_secs;
	CString m_topic_pub;
	CString m_topic_sub;
	CButton m_button_connect;
	afx_msg void OnBnClickedButtonClearPub();
	afx_msg void OnBnClickedButtonClearSub();
	CButton m_ctrl_EnablePub;
	CButton m_ctrl_EnableSub;
	CListBox m_cListLog;
};

//==================================================================================END
