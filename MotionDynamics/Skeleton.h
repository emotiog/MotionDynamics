#ifndef __SKELETON_H_
#define __SKELETON_H_

#include "Joint.h"
#include "define.h"

namespace NAnimation
{
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

		void recycle(void)
		{
			if(_curAnimationTime > _maxAnimationTime)
				_curAnimationTime = _minAnimationTime;
		}

		CAnimation& operator=(const CAnimation& _ani)
		{
			_curAnimationTime	= _ani._curAnimationTime;
			_minAnimationTime	= _ani._minAnimationTime;
			_maxAnimationTime	= _ani._maxAnimationTime;

			return (*this);
		}
	};

	class CSkeleton
	{
	public:
		typedef std::vector<CJoint> JointArray;

	protected:
		std::wstring		_mName;
		std::vector<CJoint>	_mJoint;

		Vector3d			_mBodyPosition;
		Quaterniond			_mBodyDirection;

	public:
		double				mBodyMass;
		CAnimation			mMotionSequence;
		CVector<double, 3>	mScale;


	public:
		void attachJoint(CJoint* _pJoint);
		void reset(void);

	public:
		CJoint*	findByIndex(unsigned int _idx);
		CJoint*	findByName(const std::tstring& _rStr);
		CJoint* getIndex(unsigned int _idx);

	public:
		CSkeleton(void);
		CSkeleton(const CSkeleton& _rSrc);
		~CSkeleton(void);

	public:
		bool	connectByHierarchy(const std::map<std::tstring, TStringArray>& hierarchy);
		void	resetIndex(void);

	public:
		const Vector3d&		getBodyPosition(void) const;
		const Quaterniond&	getBodyDirection(void) const;

		void	setBodyPosition(float _x, float _y, float _z);
		void	setBodyPosition(const Vector3d& _rPos = Vector3d(0., 0., 0.));

	public:
		void	erase(unsigned int _ith);
		void	removeJoint(unsigned int _ith);

	public:
		bool	isParent(unsigned int _childIndex = 0);
		bool	emptyJoint(void);
		void	resize(size_t size);
		unsigned int	getJointSize(void) const;

	public:
		CSkeleton&	operator=(const CSkeleton& _rSrc);
		void		operator=(const std::vector<CJoint>& _rSrc);

	public:
		void animateSkeleton(float fElapseTime);
		void renderSkeleton(void);

	public:	// 관절이 15개인 프로그램에 적합 + Dynamics 조합
		void	animateSkeletonStatic15(void);
		void	adaptDynamicsForce(void);

	public:
		void insertKeyframe(const std::tstring& sAmcFile,
				unsigned int nKeyframeIndex, std::vector< CAMCkeyframe >& _rMotionSequences);
		void smoothingSequence(void);

	public:
		void	centeralHierarchy(void);
		
	public:
		ENUM_PUSHED_DIRECTION		m_ePushedDir;	// 0 - none, 1 - left & 2 - right

	protected:
		void	_updateDynamicsMaterials(CJoint* _pEndEf, CJoint* _pRoot);
		void	_updateDynamicsMaterials_Rev(CJoint* _pEndEf, CJoint* _pRoot);

	public:
		bool	isCollapse(void);
	};
}

#endif