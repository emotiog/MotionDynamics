
// MotionDynamics.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMotionDynamicsApp:
// �� Ŭ������ ������ ���ؼ��� MotionDynamics.cpp�� �����Ͻʽÿ�.
//

class CMotionDynamicsApp : public CWinApp
{
public:
	CMotionDynamicsApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMotionDynamicsApp theApp;