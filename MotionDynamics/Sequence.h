#ifndef __SEQUENCE_H_
#define __SEQUENCE_H_

#include "define.h"
#include "animationKeys.h"
#include "amcKeyframe.h"
#include "JointEnumerate.h"
#include "MathMethods.h"

namespace NAnimation
{
	class CSequence
	{
	protected:
		static RotKeyArray		rotKeyBuffer;
		static unsigned int		rotKeyBufferSize;
		static unsigned int		rotKeyBufferCount;
		static PosKeyArray		posKeyBuffer;
		static unsigned int		posKeyBufferSize;
		static unsigned int		posKeyBufferCount;


	protected:
		RotKeyArray rotKeys_;
		PosKeyArray posKeys_;

		mutable int lastRotKeyIndex_;
		mutable int lastPosKeyIndex_;	// protected 이지만 조작하고 싶다.

		mutable float currentTime_;
		mutable Quaternionf currentRotation_;
		mutable Vector3f currentPosition_;

	public:
		CSequence(void);
		~CSequence(void);

		CSequence(const CSequence& rhs);
		CSequence(CSequence&& rhs);

		CSequence& operator=(const CSequence& rhs);
		CSequence& operator=(CSequence&& rhs);

	public:
		void swap(CSequence& rhs);

		bool createFromAmcKeyframes(const std::tstring _rAMCorder[7], 
			const unsigned int _rAMCcount,
			NAnimation::ENUM_ROTATION_TYPE _rRotOrder,
			const std::vector<CAMCkeyframe>& keyframes,
			float fFramesPerSecond);
		void destroy(void);

		void smoothing(void);

		bool isContainedPosKeys(void) const;
		bool isContainedRotKeys(void) const;

		unsigned int findInbetweenPosKeys(
						float fCurTime,
						const CPosKey*& pKeyFirst,
						const CPosKey*& pKeySecond,
						unsigned int nLastKeyIndex) const;

		unsigned int findInbetweenRotKeys(
						float fCurTime,
						const CRotKey*& pKeyFirst,
						const CRotKey*& pKeySecond,
						unsigned int nLastKeyIndex) const;

	public:
		float getAnimationStartTime(void) const;
		float getAnimationStopTime(void) const;

		unsigned int getPosKeyCount(void) const;
		unsigned int getRotKeyCount(void) const;
	};
}

#endif