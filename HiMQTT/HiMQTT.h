
// HiMQTT.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CHiMQTTApp:
// �� Ŭ������ ������ ���ؼ��� HiMQTT.cpp�� �����Ͻʽÿ�.
//

class CHiMQTTApp : public CWinApp
{
public:
	CHiMQTTApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CHiMQTTApp theApp;