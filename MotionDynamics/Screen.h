#pragma once


// CScreen
#include "define.h"
#include "MotionLoader.h"
#include "MotionRender.h"
#include "MotionAnimator.h"
#include "Limits.h"


using namespace NAnimation;


enum MOTION_CONTROLLER{PLAY, PAUSE, STOP};

class CScreen : public CStatic
{
	DECLARE_DYNAMIC(CScreen)

public:
	CScreen(void);
	virtual ~CScreen(void);

public:
	CDC*	_mpCDC;
	HGLRC	_mhOGLRC;

public:
	bool				m_bControl;
	MOTION_CONTROLLER	eMotionCtrl;
	CVector<double, 15>	mTorque;

public:
	struct 
	{
		Vector3f			m_vCameraPos;
		Vector3f			m_vCameraAxisX;
		Vector3f			m_vCameraAxisY;
		Vector3f			m_vCameraAxisZ;

		float	m_fFarDistance;
		float	m_fNearDistance;
		float	m_fFieldofView;
	} mCamera;

	class CAnimation
	{
	public:
		float _curAnimationTime;
		float _minAnimationTime;
		float _maxAnimationTime;

		CAnimation(void)
		{
			_curAnimationTime = 0.f;
			_minAnimationTime = 0.f;
			_maxAnimationTime = 0.f;
		};

		void reset(void)
		{
			_curAnimationTime = 0.f;
			_minAnimationTime = 0.f;
			_maxAnimationTime = 0.f;
		};
	};

	std::tstring			_mActiveSequenceName;

	double	m_dWidth;
	double	m_dHeight;

public:
	CSkeleton		mBody;
	CMotionLoader	mLoader;
	CMotionRender	mRender;
	CMotionAnimator	mAnimator;

	void	drawModel(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	CRect GetWindowSize(void);

private:
	BOOL	SetupPixelFormat(HDC hdc);
	void	_initGL(void);
	void	_releaseGL(void);

	Matrix4x4f	_setViewportMatrix(void);

public:
	void	activateSequence(const std::tstring& sSequenceName);
	void	setMinAnimationTime(float fTime);
	void	computeDynamics(CVector<double, 15>* rTorque);

	void	motionController(float elapseTime);

public:
	void	render(void);
	int		createGL(void);

public:
	bool	m_bConstraint;
	bool	m_bExit;

	double	mScale;
	void	updateCharacterSize(void);

	void	standOnPlane(void);

public:
	void	inputModule(const CVector<double, 3>& _rVel);

protected:
	void	_invJacobian(CJoint* _pTarget, const CVector<double, 3>& _rForce);
};


