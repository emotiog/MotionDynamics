
// MotionDynamicsDlg.h : 헤더 파일
//

#pragma once

#include "Screen.h"
#include "afxwin.h"

// CMotionDynamicsDlg 대화 상자
class CMotionDynamicsDlg : public CDialogEx
{
// 생성입니다.
public:
	CMotionDynamicsDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTIONDYNAMICS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.m
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAsfOpen();

public:
	//CPoint	mCurPoint;	// 현재 위치
	//CPoint	mPrePoint;	// 직전 프레임 위치

// 2013. 07. 25
	// thread를 이용하여 static box에서 render
	// static - 클래스 변수 포인터로는
	// 컴파일 단계에서 생성되지 않기 떄문에
	// 함수를 static으로 만들어야 한다
public:
	
	static UINT ThreadRender(LPVOID param);
	CWinThread* mRenderThread;

	static UINT ThreadAnimator(LPVOID param);
	CWinThread* mAnimatorThread;

	bool	mAMCopen;
	//bool	mMouseCircle;


	afx_msg	void RadioCtrl(UINT ID);

	afx_msg void OnBnClickedAmcOpen();
	CScreen mCanvas;
	afx_msg void OnBnClickedAmcExit();
	CButton mRadioControl;
	afx_msg void OnBnClickedButtonDynamicsReset();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckBalance();
	CButton mConstraintForceCheck;
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit mEditScale;
	afx_msg void OnBnClickedButtonScale();
};
