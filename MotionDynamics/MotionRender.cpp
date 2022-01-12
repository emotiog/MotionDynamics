#include "stdafx.h"
#include "MotionRender.h"

namespace NAnimation
{
	CMotionRender::CMotionRender(void) :
		mMotionSequence(),
		m_dWidth(0),
		m_dHeight(0),
		_mActiveSequenceName()
	{
		mpBody = NULL;
	}


	CMotionRender::~CMotionRender(void)
	{
	}


	CMotionRender::CMotionRender(CSkeleton* _pBody) :
		m_dWidth(0),
		m_dHeight(0),
		_mActiveSequenceName()
	{
		mpBody = _pBody;
	}


	CMotionRender::CMotionRender(const CMotionRender& _rSrc)
	{
		(*this)	= _rSrc;
	}
	

	CMotionRender& CMotionRender::operator=(const CMotionRender& _rSrc)
	{
		mMotionSequence				= _rSrc.mMotionSequence;
		_mActiveSequenceName	= _rSrc._mActiveSequenceName;
		m_dWidth				= _rSrc.m_dWidth;
		m_dHeight				= _rSrc.m_dHeight;
		
		mpBody					= _rSrc.mpBody;		
		mMotionSequence				= _rSrc.mMotionSequence;

		mHierarchy				= _rSrc.mHierarchy;
		return (*this);
	}


	bool	CMotionRender::connectSkeleton(CSkeleton* _pBody)
	{
		if(_pBody == NULL)
			return false;

		if(mpBody != NULL)
			return false;	// 하나가 reset되면 연결된 _pBody도 reset됨
			//mpBody->reset();

		mpBody	= _pBody;

		return true;
	}


	void CMotionRender::activateSequence(const std::tstring& sSequenceName)
	{
		if(sSequenceName == _mActiveSequenceName)
			return;

		if(mpBody->emptyJoint())
		{
			mMotionSequence.reset();
			return;
		}
		else
		{
			mMotionSequence._minAnimationTime = std::numeric_limits<float>::max();
			mMotionSequence._maxAnimationTime = std::numeric_limits<float>::lowest();

			for(unsigned int i  = 0 ; i < mpBody->getJointSize() ; ++ i )
			{
				CJoint* joint = mpBody->findByIndex(i);
				assert(joint != NULL);
				joint->activeSequence(sSequenceName);

				float fStartTime	= joint->getAnimationStartTime();
				float fStopTime		= joint->getAnimationStopTime();

				if(fStartTime < mMotionSequence._minAnimationTime)
					mMotionSequence._minAnimationTime = fStartTime;
				if(fStopTime > mMotionSequence._maxAnimationTime)
					mMotionSequence._maxAnimationTime = fStopTime;
			}

			mMotionSequence._curAnimationTime = mMotionSequence._minAnimationTime;

			_mActiveSequenceName = sSequenceName;
		}
	}

	void CMotionRender::setMinAnimationTime(float fMinTime)
	{
		mMotionSequence._minAnimationTime = fMinTime;
		if (mMotionSequence._curAnimationTime < mMotionSequence._minAnimationTime)
			mMotionSequence._curAnimationTime = mMotionSequence._minAnimationTime;

		if (mMotionSequence._maxAnimationTime < mMotionSequence._minAnimationTime)
			mMotionSequence._maxAnimationTime = mMotionSequence._minAnimationTime;
	}


	void CMotionRender::animatePaused(void)
	{
		mpBody->mMotionSequence._curAnimationTime = 0.;
	}


	void CMotionRender::render(void)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		mpBody->renderSkeleton();
	}
}