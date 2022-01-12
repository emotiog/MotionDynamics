#ifndef __MOTION_RENDER_H_
#define	__MOTION_RENDER_H_

#include "define.h"
#include "Skeleton.h"
#include "Hierarchy.h"

#include "gl/glut.h"

namespace NAnimation
{
	class CMotionRender
	{
	public:
		CAnimation				mMotionSequence;	
		std::tstring			_mActiveSequenceName;

		double	m_dWidth;
		double	m_dHeight;

		//float	sMaxTime;		// ?

		CSkeleton*	mpBody;
		CHierarchy	mHierarchy;


		CMotionRender& operator=(const CMotionRender& _rSrc);

	public:
		CMotionRender(void);
		CMotionRender(CSkeleton* _pBody);
		CMotionRender(const CMotionRender& _rSrc);
		~CMotionRender(void);

		bool	connectSkeleton(CSkeleton* _pBody);

	public:
		void	activateSequence(const std::tstring& sSequenceName);
		void	setMinAnimationTime(float fTime);
		void	animatePaused(void);

	// openGL
	public:
		bool	InitOpenGL(void);
		void	render(void);

	protected:
		void	_initGL(void);

	};

}


#endif