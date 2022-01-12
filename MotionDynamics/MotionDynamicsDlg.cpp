
// MotionDynamicsDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MotionDynamics.h"
#include "MotionDynamicsDlg.h"
#include "afxdialogex.h"

#include "openGL.h"

#include "gl/glut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMotionDynamicsDlg ��ȭ ����




CMotionDynamicsDlg::CMotionDynamicsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotionDynamicsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mAMCopen	= false;
}

void CMotionDynamicsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_MOTION_VIEW, mCanvas);
	DDX_Control(pDX, IDC_RADIO_MOTION_ONLY, mRadioControl);
	DDX_Control(pDX, IDC_CHECK_BALANCE, mConstraintForceCheck);
	DDX_Control(pDX, IDC_EDIT_CHARACTER_EDIT, mEditScale);
}

BEGIN_MESSAGE_MAP(CMotionDynamicsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ASF_OPEN, &CMotionDynamicsDlg::OnBnClickedAsfOpen)
	ON_BN_CLICKED(IDC_AMC_OPEN, &CMotionDynamicsDlg::OnBnClickedAmcOpen)
	ON_BN_CLICKED(IDC_AMC_EXIT, &CMotionDynamicsDlg::OnBnClickedAmcExit)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MOTION_ONLY, IDC_RADIO_DYNAMICS, RadioCtrl)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMICS_RESET, &CMotionDynamicsDlg::OnBnClickedButtonDynamicsReset)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_BALANCE, &CMotionDynamicsDlg::OnBnClickedCheckBalance)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMotionDynamicsDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CMotionDynamicsDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMotionDynamicsDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SCALE, &CMotionDynamicsDlg::OnBnClickedButtonScale)
END_MESSAGE_MAP()


// CMotionDynamicsDlg �޽��� ó����

BOOL CMotionDynamicsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	
// 2013. 08. 13  --------------------------------------------------------------------------
	mRadioControl.SetCheck(BST_CHECKED);
	//mMouseCircle	= false;
	mCanvas.m_bExit	= false;
	mConstraintForceCheck.SetCheck(BST_CHECKED);
// ----------------------------------------------------------------------------------------


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMotionDynamicsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMotionDynamicsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		//if(mMouseCircle)
		//	dc.Ellipse(mCurPoint.x -10, mCurPoint.y -10, mCurPoint.x +10, mCurPoint.y +10);
	
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMotionDynamicsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




// 2013. 07. 25 --------------------------------------------------------------
void CMotionDynamicsDlg::OnBnClickedAsfOpen()
{
	CFileDialog dlg(TRUE, L"asf", L"*.asf");

	if(dlg.DoModal() != IDOK)
		return;

	CString filename = dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();

	std::tstring path(filename);
	mAMCopen = mCanvas.mLoader.loadAcclaimSkeletonFile(path);
	
	if( mAMCopen )
		MessageBox(L"1 model loaded success", L"Text", 0);
	else
		MessageBox(L"asf loading failure",L"Error", 0);
}


// 2013. 07. 25 --------------------------------------------------------------
UINT CMotionDynamicsDlg::ThreadRender(LPVOID _pParam)
{
	static LARGE_INTEGER prevTime, frequency;
	static LARGE_INTEGER curTime;
	
	// opengl ���� -> �׸� �׸���
	CScreen* data	= (CScreen*)_pParam;	
	
	data->createGL();
	CRect rect = data->GetWindowSize();

	while(!data->m_bExit)
	{
		::QueryPerformanceFrequency(&frequency);
		::QueryPerformanceCounter(&curTime);
		
		double	elapseTime = static_cast<double>(curTime.QuadPart - prevTime.QuadPart);
		elapseTime = elapseTime / (static_cast<double>(frequency.QuadPart));


		data->motionController(static_cast<float>(elapseTime));

		data->computeDynamics(&data->mTorque);
		
		//data->updateCharacterSize();
#ifdef _SKELETON
		data->standOnPlane();
#endif

		prevTime = curTime;

		data->render();
	}

	AfxEndThread(0, true);

	return 0;
}


UINT CMotionDynamicsDlg::ThreadAnimator(LPVOID param)
{
	CScreen* data	= (CScreen*)param;
	CVector<double, 15> torque;

	return 0;
}



// 2013. 07. 25 --------------------------------------------------------------
void CMotionDynamicsDlg::OnBnClickedAmcOpen()
{
	using namespace NAnimation;
	static	bool	bMotionLoad	= false;

	if( !mAMCopen)
	{
		MessageBox(L"asf loads needed",L"Error", 0);
		return;
	}

	CFileDialog dlg(TRUE, L"amc", L"*.amc");

	if(dlg.DoModal() != IDOK)
		return;

	CString filename = dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();

	std::tstring path(filename);

	if(bMotionLoad)
	{// clear �����ϱ� 
		MessageBox(L"Version 1.0 does not support re-load amc file, Please restart program");
		return;
	}

	if (!mCanvas.mLoader.loadAcclaimMotionFile(path))
	{
		MessageBox(L"amc loading failure",L"Error", 0);
		bMotionLoad	= false;
	}
	else
	{
		MessageBox(L"amc loaded successfully" ,L"Text", 0);
		bMotionLoad	= true;
	}

	//mLoader.arrangeHierarchy();
	mCanvas.activateSequence(path);
	mCanvas.setMinAnimationTime(0.1f);
	mCanvas.mRender.mpBody->mMotionSequence	= mCanvas.mRender.mMotionSequence;

	// ���ó�� ���̰� root�� thorax ���� �ø� -mass �� ����
	CJoint*	pThorax	= mCanvas.mLoader.mpBody->findByName(L"thorax");
	pThorax->setLocalJointPosition(pThorax->getLocalJointPosition() * 3.);
	//pThorax->setLength(pThorax->getLength() * 3.);
	pThorax->setMass( pThorax->getMass() * 1.3);

	mCanvas.mLoader.mpBody->setBodyPosition(Vector3d(0.f, 10.f, 0.f));

	// start rendering
	mCanvas.mAnimator.connectSkeleton(mCanvas.mRender.mpBody);
	mRenderThread	= AfxBeginThread(ThreadRender, &mCanvas);	
	
	//mAnimatorThread	= AfxBeginThread(ThreadAnimator, &mCanvas);

	mCanvas.SetFocus();
}


void CMotionDynamicsDlg::OnBnClickedAmcExit(void)
{
	int returnCall = AfxMessageBox(_T("Do you want to exit?"), MB_OKCANCEL);

	if(returnCall == IDOK)
	{
		mCanvas.m_bExit = true;
		SendMessage(WM_CLOSE,0,0);
	}
	else // IDCANCEL
		return;
}


void CMotionDynamicsDlg::RadioCtrl(UINT ID)
{
	UpdateData(TRUE);

	switch(ID)
	{
	case IDC_RADIO_MOTION_ONLY:
		mCanvas.m_bControl = false;
		break;
	case IDC_RADIO_DYNAMICS:
		mCanvas.m_bControl = true;
		break;
	}
}

void CMotionDynamicsDlg::OnBnClickedButtonDynamicsReset()
{
	mCanvas.mAnimator.reset();
	mCanvas.mTorque.zero();
}


void CMotionDynamicsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	static CPoint	previousPoint	= point;
	// JoyStick Control �κп��� Ŭ���Ͽ��� ���
	CRect	rt;

	if(nFlags == MK_LBUTTON)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_JOYSTICK_INTERFACE))->GetWindowRect(&rt);
		// â�� ũ�� �޾ƿ���

		// ���� ��ǥ -> �� ��ǥ
		ScreenToClient(&rt);

		if(rt.PtInRect(point) && !mCanvas.mRender.mpBody->emptyJoint())
		{
			//mMouseCircle	= true;
			//mCurPoint		= point;

			// velocity ���� �ٿ��� openGL�� ���� ĭ ���̷� 
			CVector<double, 3> vVel;
			vVel.x	= (point.x - previousPoint.x) * 0.00001;
			vVel.y	= (point.y - previousPoint.y) * 0.00001;
			vVel.z	= 0.;
#ifdef	VERSION_1
			mCanvas.mTorque[0]	= vVel.length();
#endif
			// �Է��ؼ� �����̴� ���� ��� �ۿ�
			mCanvas.inputModule(vVel);
			previousPoint	= point;
		}        
	}	

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMotionDynamicsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//mMouseCircle = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMotionDynamicsDlg::OnBnClickedCheckBalance()
{
	if(mConstraintForceCheck.GetCheck() == BST_CHECKED)
	{
		mCanvas.m_bConstraint	= true;
	}
	else
	{
		mCanvas.m_bConstraint	= false;
	}
}


void CMotionDynamicsDlg::OnBnClickedButtonPlay()
{
	mCanvas.eMotionCtrl	= PLAY;
}


void CMotionDynamicsDlg::OnBnClickedButtonPause()
{
	mCanvas.eMotionCtrl	= PAUSE;
}


void CMotionDynamicsDlg::OnBnClickedButtonStop()
{
	mCanvas.eMotionCtrl	= STOP;
}


BOOL CMotionDynamicsDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			OnBnClickedButtonScale();
			break;

		case VK_LEFT:
			mCanvas.mCamera.m_vCameraPos.z ++;
			pMsg->wParam = NULL;
			break;

		case VK_RIGHT:
			mCanvas.mCamera.m_vCameraPos.z --;
			pMsg->wParam = NULL;
			break;

		case VK_UP:
			mCanvas.mCamera.m_vCameraPos.y ++;
			pMsg->wParam = NULL;
			break;

		case VK_DOWN:
			mCanvas.mCamera.m_vCameraPos.y --;
			pMsg->wParam = NULL;
			break;

		//case VK_NUMPAD1:
		//	mCanvas.mCamera.m_vCameraPos.x --;
		//	break;

		//case VK_NUMPAD3:
		//	mCanvas.mCamera.m_vCameraPos.x ++;
		//	break;

		//case VK_NUMPAD4:
		//	mCanvas.mCamera.m_vCameraAxisX -= 0.1;
		//	break;

		//case VK_NUMPAD7:
		//	mCanvas.mCamera.m_vCameraAxisX += 0.1;
		//	break;

		//case VK_NUMPAD5:
		//	mCanvas.mCamera.m_vCameraAxisY -= 0.1;
		//	break;

		//case VK_NUMPAD8:
		//	mCanvas.mCamera.m_vCameraAxisY += 0.1;
		//	break;

		//case VK_NUMPAD6:
		//	mCanvas.mCamera.m_vCameraAxisZ -= 0.1;
		//	break;

		//case VK_NUMPAD9:
		//	mCanvas.mCamera.m_vCameraAxisZ += 0.1;
		//	break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMotionDynamicsDlg::OnBnClickedButtonScale()
{
	CString scale;
	GetDlgItemText(IDC_EDIT_CHARACTER_EDIT, scale);

	if(scale.IsEmpty())
		return;

#ifdef _UNICODE
	mCanvas.mScale	= _wtof(scale);
#else
	mCanvas.mScale	= atof(scale);
#endif

	mCanvas.mAnimator.mpBody->mScale.x	= mCanvas.mScale;
	mCanvas.mAnimator.mpBody->mScale.y	= mCanvas.mScale;
	mCanvas.mAnimator.mpBody->mScale.z	= mCanvas.mScale;
	//scale.Format(_T("%f"), mCanvas.mScale);
}
