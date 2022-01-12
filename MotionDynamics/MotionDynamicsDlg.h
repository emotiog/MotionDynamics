
// MotionDynamicsDlg.h : ��� ����
//

#pragma once

#include "Screen.h"
#include "afxwin.h"

// CMotionDynamicsDlg ��ȭ ����
class CMotionDynamicsDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMotionDynamicsDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MOTIONDYNAMICS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.m
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAsfOpen();

public:
	//CPoint	mCurPoint;	// ���� ��ġ
	//CPoint	mPrePoint;	// ���� ������ ��ġ

// 2013. 07. 25
	// thread�� �̿��Ͽ� static box���� render
	// static - Ŭ���� ���� �����ͷδ�
	// ������ �ܰ迡�� �������� �ʱ� ������
	// �Լ��� static���� ������ �Ѵ�
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
