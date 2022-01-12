// Screen.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MotionDynamics.h"
#include "Screen.h"
#include <gl/GLU.h>
#include <limits>
// CScreen

IMPLEMENT_DYNAMIC(CScreen, CStatic)

CScreen::CScreen() :
	mBody(),
	mLoader(&mBody),
	mRender(&mBody),
	m_bControl(false),
	m_bConstraint(true),
	eMotionCtrl(PLAY)
{
	_mpCDC		= NULL;
	_mhOGLRC	= NULL;
	
	using namespace NAnimation;

	mCamera.m_vCameraAxisX	= Vector3f(-1.0f, 0.f, 0.f);
	mCamera.m_vCameraAxisY	= Vector3f(0.0f, 0.981627f, 0.190809f);
	mCamera.m_vCameraAxisZ	= Vector3f(0.0f, 0.190809f, -0.981627f);
	mCamera.m_vCameraPos	= Vector3f(0.0f, 5.0f, -10.0f);
	mCamera.m_fFieldofView	= 60.f;
	mCamera.m_fNearDistance	= 0.1f;
	mCamera.m_fFarDistance	= 500.f;

	mScale	= 10.;
}

CScreen::~CScreen()
{
	if( _mpCDC != NULL)
	{
		//ReleaseDC(_mpCDC);	// 근데 얘 때문에 자꾸 문제가 생기긴 함..
		_mpCDC	= NULL;
	}
}


BEGIN_MESSAGE_MAP(CScreen, CStatic)
END_MESSAGE_MAP()



// 2013. 07. 25
CRect CScreen::GetWindowSize(void)
{
	CRect rect;
	this->GetWindowRect(&rect);

	ScreenToClient(&rect);

	m_dWidth	= rect.right;
	m_dHeight	= rect.bottom;

	glViewport(0, 0, m_dWidth, m_dHeight);
	
	gluPerspective(
		mCamera.m_fFieldofView,
		static_cast<double>(m_dWidth) / static_cast<double>(m_dHeight),
		mCamera.m_fNearDistance,
		mCamera.m_fFarDistance);

	return rect;
}


void CScreen::_releaseGL(void) {}


BOOL CScreen::SetupPixelFormat(HDC hdc)
{
	// 픽셀 코드를 설정합니다. 
	// 픽셀 포맷은 직접 그릴 객체의 대상, 즉 윈도우나 비트맵에 대한 컬러 비트의 구조에 대한 속성을 지정하는 것을 의미합니다. 
	// PIXELFORMATDESCRIPTOR 구조체는 DC에 의해 사용되는 픽셀 포맷을 지정하며, 각 윈도우는 각자의 픽셀 포맷을 가지고 있습니다. 
	// 여기서 PIXELFORMATDESCRIPTOR 구조체에는 크기, 버전 넘버, 속성 플래그, 컬러 비트 수, 어큐뮬레이터(accumulate)나 
	// 깊이(depth) 버퍼, 스텐슬(stencil), 보조(auxiliary) 버퍼 등의 지정, 메인 레이어(layer)타입등의 정보를 지정할 수 있죠. 
	// 더블 버퍼링은 전면 버퍼(Screen Buffer)와 이면 버퍼(Off-Screen Buffer, 후면 버퍼라고도 한다.)를 이용하는데, 
	// 이면 버퍼에 먼저 렌더링한 후 전면 버퍼와 교체하는 방식을 통해 보다 빠른 애니메이션을 구현할 수 있습니다. 

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |		    // support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
    };
    
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if(pixelFormat == 0) { 
		// 에러 -> DC hdc를 잃어오지 못한다거나.. 그래서 문제인가?
		::AfxMessageBox(L"ChoosePixelFormat failed."); 
		 return FALSE;
    } 

	BOOL ret = SetPixelFormat(hdc, pixelFormat, &pfd);
	if(ret == FALSE) { 
		::AfxMessageBox(L"SetPixelFormat failed."); 
		return FALSE; 
	} 
	return TRUE; 
}


int CScreen::createGL(void)
{
	// DC에 대한 픽셀 포맷을 설정한 후 이와 연관된 RC를 생성합니다. 
	// RC는 모든 OpenGL 명령어들이 통과해야 하는 일종의 포트로 생각할 수 있습니다. 
	// RC는 DC에 지정된 것과 같은 픽셀 포맷을 지니는데 RC와 DC는 서로 같은 것이 아니며, 
	// DC는 GDI를 위한 정보를 포함하는 반면 RC는 OpenGL을 위한 정보를 담고 있습니다. 

	// 먼저 디바이스 컨텍스트를 얻는다. 
	_mpCDC = GetDC ();

	if (!SetupPixelFormat(_mpCDC->m_hDC)) {
		return -1;
	}
	// 렌더링 컨텍스트를 얻는다 
	_mhOGLRC = wglCreateContext(_mpCDC->m_hDC);
	if (!_mhOGLRC) {
		::AfxMessageBox(L"wglCreateContext failed"); 
		return -1; 
	} 
	// 현재의 렌더링 컨텍스트를 만든다. 
	if(!wglMakeCurrent(_mpCDC->m_hDC, _mhOGLRC)) { 
		::AfxMessageBox(L"wglMakeCurrent failed"); 
		return -1; 
	} 
	
	_initGL ();

	return 0;
}


void CScreen::_initGL(void)
{
	// 컬러버퍼와 깊이버퍼를 초기화
	COLORREF color = ::GetSysColor(COLOR_3DFACE);
	glClearColor((float)GetRValue(color)/255.0f,(float)GetGValue(color)/255.0f,(float)GetBValue(color)/255.0f, 1.0);
	glClearDepth(1.);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// 조명 설정
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	float AmbientColor[]    = { 0.0f, 0.1f, 0.2f, 0.0f };         
	float DiffuseColor[]    = { 0.5f, 0.5f, 0.5f, 0.0f };         
	float SpecularColor[]   = { 0.5f, 0.5f, 0.5f, 0.0f };        
	float Position[]        = { 100.0f, 100.0f, -400.0f, 1.0f };  

	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientColor);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularColor);
	glLightfv(GL_LIGHT1, GL_POSITION, Position);

	// 재질의 속성 설정
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}


void CScreen::render(void)
{
	wglMakeCurrent(_mpCDC->GetSafeHdc(),_mhOGLRC);

	// 그림 그리기
	glMatrixMode(GL_MODELVIEW);    // Select The Modelview Matrix
	glLoadIdentity();              // Reset The Modelview Matrix

	// 그림 지우기
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		mCamera.m_fFieldofView,
		static_cast<double>(m_dWidth) / static_cast<double>(m_dHeight),
		mCamera.m_fNearDistance,
		mCamera.m_fFarDistance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(_setViewportMatrix().element1D);


	glColor3f(0.f, 0.f, 0.f);
	glBegin(GL_POLYGON);
	glVertex3f(-100.0, -1.0f, -10.0);
	glVertex3f( 100.0, -1.0f, -10.0);
	glVertex3f( 100.0, -1.0f,  100.0);
	glVertex3f(-100.0, -1.0f,  100.0);
	glEnd();

	glDisable(GL_DEPTH_TEST);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	
	for (int z = -100; z < 100; ++z)
	{
		for (int x = -100; x < 100; ++x)
		{
			glVertex3f(x,   -1.0, z);
			glVertex3f(x+1, -1.0, z);

			glVertex3f(x, -1.0, z+1);
			glVertex3f(x, -1.0, z);
		}
	}

	glEnd();

	//glBegin(GL_POINT);
	//glColor3f(1.f, 1.f, 0.f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glEnd();

	glEnable(GL_DEPTH_TEST);

	// model render
	drawModel();

	glFinish();
	SwapBuffers(_mpCDC->GetSafeHdc());
}


Matrix4x4f CScreen::_setViewportMatrix(void)
{
	using namespace NAnimation;

	Matrix4x4f m;

	m.m11 = mCamera.m_vCameraAxisX.x; 
	m.m12 = mCamera.m_vCameraAxisX.y; 
	m.m13 = mCamera.m_vCameraAxisX.z; 
	m.m14 = -mCamera.m_vCameraPos.dotProduct(mCamera.m_vCameraAxisX);

	m.m21 = mCamera.m_vCameraAxisY.x; 
	m.m22 = mCamera.m_vCameraAxisY.y; 
	m.m23 = mCamera.m_vCameraAxisY.z; 
	m.m24 = -mCamera.m_vCameraPos.dotProduct(mCamera.m_vCameraAxisY);

	m.m31 = mCamera.m_vCameraAxisZ.x; 
	m.m32 = mCamera.m_vCameraAxisZ.y;
	m.m33 = mCamera.m_vCameraAxisZ.z; 
	m.m34 = -mCamera.m_vCameraPos.dotProduct(mCamera.m_vCameraAxisZ);

	m.m41 = 0.0f; 
	m.m42 = 0.0f; 
	m.m43 = 0.0f; 
	m.m44 = 1.0f;
	
	m.transpose();

	return m;
}



void CScreen::drawModel(void)
{
	mRender.render();
}


void CScreen::computeDynamics(CVector<double, 15>* _pUsersExForce)
{
	static CVector<double, 15>			_vPushAccel;
	static const CVector<double, 15>	ZERO;
	CVector<double, 15>					_vBalanceForce;

	if(!m_bControl)
		return;

	if(!mAnimator.m_bTagged)
		mAnimator.setReferenceBody();


	//if(mAnimator.mpBody->isCollapse())
	//{
	//	for(int i = 0 ; i < 15 ; i ++)
	//	{
	//		mAnimator.mpBody->getIndex(i)->mDynamics.q	= 0.;
	//		mAnimator.mpBody->getIndex(i)->mDynamics.dq	= 0.;
	//		mAnimator.mpBody->getIndex(i)->mDynamics.ddq= 0.;
	//	}
	//}
	
	// 발동 조건 - 각 조인트의 가속도가 허용 이상 값이 측정되었을 때
	// 발동 조건을 수정해야 합니다. <- 
	//if(fabs(mAnimator.mpBody->findByName(L"lfemur")->mDynamics.ddq) > 0.01 ||
	//	fabs(mAnimator.mpBody->findByName(L"rfemur")->mDynamics.ddq) > 0.01)
	//{
	//	eMotionCtrl = PAUSE;

	//	_vBalanceForce -= mAnimator.inverseDynamicsStatic();

	//	// 가중치 조절을 위한 parameter
	//	mAnimator.jointsDistance(&mAnimator.mDistanceForce);
	//	
	//	double totalForce = 0.;

	//	for(int iter = 0 ; iter < 15 ; iter ++)
	//		totalForce += mAnimator.mDistanceForce[iter];

	//	//if(totalForce > 0.1)
	//	//	for(int i = 0 ; i < 15 ; i ++)
	//	//		_vBalanceForce *= (1.4 + mAnimator.mDistanceForce[i]);
	//}
	//else
	//{
	//	_vBalanceForce	= ZERO;
	//	//eMotionCtrl		= PLAY;
	//}

	_vPushAccel = mAnimator.forwardDynamicsStatic(_vBalanceForce + *_pUsersExForce); 
	
	
	if(m_bConstraint)
	{
		mAnimator.mConstraintAccel	= mAnimator.forwardDynamicsStatic(ZERO);
	}
	else
	{
		mAnimator.mConstraintAccel	= ZERO;
		//mAnimator.mpBody->findByName(L"root")->mTransformMatrix.m24 -= 4.905 * 0.01;
	}
	_vPushAccel -= mAnimator.mConstraintAccel;

	mAnimator.applyStatic15x1(_vPushAccel, 0.01f);
	
#ifdef _SKELETON
	mAnimator.mpBody->adaptDynamicsForce();
#else
	mAnimator.mpBody->animateSkeletonStatic15();
#endif

	*_pUsersExForce +=_vBalanceForce;
}


void CScreen::motionController(float elapseTime)
{
	switch(eMotionCtrl)
	{
	case PLAY:
		mAnimator.mpBody->mMotionSequence._curAnimationTime	+= elapseTime;	
		mAnimator.mpBody->mMotionSequence.recycle();	

		mAnimator.mpBody->m_ePushedDir	= PUSHED_ROOT;
#ifdef _SKELETON
		mRender.mpBody->adaptDynamicsForce();
#else
		mRender.mpBody->animateSkeletonStatic15();
#endif
		break;
	case STOP:	
		mRender.mpBody->mMotionSequence._curAnimationTime = 0.;
		break;
	case PAUSE:	
		break;
	}
}


void CScreen::updateCharacterSize(void)
{
	assert(mAnimator.mpBody != NULL);

	int size = mAnimator.mpBody->getJointSize();

	CJoint* curJoint = NULL;
	CVector<double, 3> pos;
	for(int i_ = 0 ; i_ < size ; i_ ++)
	{
		curJoint	= mAnimator.mpBody->getIndex(i_);
		pos	= curJoint->getLocalJointDirection() * curJoint->getLength() * 2.54 / 100.0 * mScale;

		curJoint->setLocalJointPosition(pos);
	}
}


void CScreen::standOnPlane(void)
{
	if(!m_bConstraint)
		return;

	CJoint*	lFoot	= mAnimator.mpBody->findByName(L"ltibia");
	CJoint*	rFoot	= mAnimator.mpBody->findByName(L"rtibia");

	double LfootPos_y	= 
		lFoot->mDynamics.mWorldPos.y;

	double RfootPos_y	= 
		rFoot->mDynamics.mWorldPos.y;

	double closerFoot	= 0.;
	// dynamics에서는 발이 중심이라 발을 옮겨야 해!

	switch(mAnimator.mpBody->m_ePushedDir)
	{
	case PUSHED_ROOT:
		closerFoot	= LfootPos_y < RfootPos_y ? LfootPos_y : RfootPos_y;
		
		mAnimator.mpBody->setBodyPosition(
			mAnimator.mpBody->getBodyPosition() 
			- CVector<double, 3>(0., closerFoot, 0.));
		break;

	case PUSHED_LEFT:
		lFoot->mTransformMatrix.m24	= 0.;
		break;

	case PUSHED_RIGHT:
		rFoot->mTransformMatrix.m24	= 0.;
		break;
	}
}

	
void CScreen::activateSequence(const std::tstring& sSequenceName)
{
	mRender.activateSequence(sSequenceName);
}


void CScreen::setMinAnimationTime(float fMinTime)
{
	mRender.setMinAnimationTime(fMinTime);
}


void CScreen::_invJacobian(CJoint* _pTarget, const CVector<double, 3>& _rForce)
{
	const unsigned int size	= 15;

	//CJoint* terNode	= mBody.findByName(L"ltibia");
	CVector<double, 3>			vAxis	= CVector<double, 3>(0, 0, 1);	
	CVector<double, 3>			vLink;
	CVector<double, 3>			vLinkRoot, vTargetLink;
	CVector<double, 3>			vEntry;
	CVector<double, 6>			dForce;

	CMatrix<double, 6, size>	Jmat;
	CMatrix<double, size, 6>	invJmat, tpJmat;

#ifdef _SKELETON
	unsigned int LterSize	= _pTarget->mHierarchyByCenter.size();
	CJoint* _pEnd	= _pTarget->mHierarchyByCenter[0];	// 머리

	for(unsigned int i_ = 0 ; i_ < LterSize ; i_ ++)
	{
		vLink	= _pTarget->mHierarchyByCenter[i_]->mDynamics.mWorldPos;
		
#else
	unsigned int LterSize	= mAnimator.mpBody->getJointSize();
	CJoint* _pEnd	= mAnimator.mpBody->getIndex(LterSize-1);	// 머리

	for(unsigned int i_ = 0 ; i_ < LterSize ; i_ ++)
	{
		vLink	= _pTarget->mDynamics.mWorldPos;
	
#endif
		vLinkRoot	= _pEnd->mDynamics.mWorldPos - vLink;

		vTargetLink	= (_rForce + _pEnd->mDynamics.mWorldPos) - vLink;

		vLinkRoot.crossProduct(vTargetLink);
		vAxis.normalise();

		vEntry	= vLinkRoot.crossProduct(vAxis);

		Jmat.elements[0][i_]	= vEntry.x;
		Jmat.elements[1][i_]	= vEntry.y;
		Jmat.elements[2][i_]	= vEntry.z;
		Jmat.elements[3][i_]	= vAxis.x;
		Jmat.elements[4][i_]	= vAxis.y;
		Jmat.elements[5][i_]	= vAxis.z;
	}


	double dRamda	= 0.001;

	CMatrix<double, 6, 6> I;

	tpJmat	= ~Jmat;
	invJmat = tpJmat * !(Jmat * tpJmat + dRamda * dRamda * I);

	dForce.elements[0]	= _rForce.x; 
	dForce.elements[1]	= _rForce.y; 
	dForce.elements[2]	= _rForce.z; 
	dForce.elements[3]	= 0.;
	dForce.elements[4]	= 0.;
	dForce.elements[5]	= 0.;

	for(unsigned iRow = 0; iRow < LterSize ; ++iRow)
	{
#ifdef _SKELETON
		CJoint*	pushedJoint	= _pTarget->mHierarchyByCenter[iRow];
#endif
		CJoint* pushedJoint	= mAnimator.mpBody->getIndex(iRow);
		if(pushedJoint == _pEnd)
			continue;

		int	influencedRow	= pushedJoint->getIndex();
		for(unsigned int iColumn = 0; iColumn < 6; ++iColumn)
		{
		//	_pTarget->mHierarchyByCenter[iRow]->mDynamics.dq += 
		//	invJmat.elements[iRow][iColumn] * dForce.elements[iColumn];
			this->mTorque.elements[influencedRow]	+=
				invJmat.elements[iRow][iColumn] * dForce.elements[iColumn];
		}
	}

}


void CScreen::inputModule(const CVector<double, 3>& _rVel)
{
	if(!m_bControl)
		return;

	// Static 버젼 관전 15개
	const unsigned int size		= 15;
	CVector<double, size> Jroot;
	
	CVector<double, 3>	exForce	= _rVel;
	//exForce.normalise();

	CJoint*	root	= mBody.findByName(L"root");
	CVector<double, 3>		vTarget = root->mDynamics.mWorldPos + _rVel;

	vTarget.z	= 0.;

	CVector<double, 3>		vEntry;
	CVector<double, 3>		vLink;
	CVector<double, 3>		vLinkRoot;
	CVector<double, 3>		vTargetLink;

	CVector<double, 3>			vAxis;
	CMatrix<double, 6, size>	Jmat;
	CMatrix<double, size, 6>	invJmat, tpJmat;
	CVector<double, 6>			dForce;


	// 1. root는 그 힘을 받은 방향 만큼 translate
	//root->mTransformMatrix.m14	+= exForce.x;
	//root->mTransformMatrix.m24	+= exForce.y;
	//root->mTransformMatrix.m34	+= exForce.z;

	if(_rVel.x > 0)
	{
		_invJacobian(mBody.findByName(L"ltibia"), _rVel);
		mBody.m_ePushedDir	= PUSHED_RIGHT;
	}
	else
	{
		_invJacobian(mBody.findByName(L"rtibia"), _rVel);
		mBody.m_ePushedDir	= PUSHED_LEFT;
	}
}