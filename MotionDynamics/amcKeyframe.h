#ifndef __AMC_KEY_FRAME_H_
#define __AMC_KEY_FRAME_H_

#include "define.h"

namespace NAnimation
{
	class CAMCkeyframe
	{
	public:
		unsigned int	mKeyIndex;
		double			mValues[7];

	public:
		CAMCkeyframe(void)
			: mKeyIndex(0xffffffff)
		{
			for(int it_ = 0 ; it_ < 7 ; it_ ++)
				mValues[it_] = 0.;
		}

		CAMCkeyframe(const CAMCkeyframe& _rSrc)
			:mKeyIndex(_rSrc.mKeyIndex)
		{
			for(int it_ = 0 ; it_ < 7 ; it_ ++)
				mValues[it_] = _rSrc.mValues[it_];
		}


		CAMCkeyframe& operator=(const CAMCkeyframe& _rSrc)
		{
			if(this == &_rSrc)
				return (*this);

			mKeyIndex	= _rSrc.mKeyIndex;

			for(int it_ = 0 ; it_ < 7 ; it_ ++)
				mValues[it_] = _rSrc.mValues[it_];

			return (*this);
		}


		CAMCkeyframe& operator=(CAMCkeyframe&& _rSrc)
		{
			if(this == &_rSrc)
				return (*this);

			mKeyIndex	= _rSrc.mKeyIndex;

			for(int it_ = 0 ; it_ < 7 ; it_ ++)
				mValues[it_] = _rSrc.mValues[it_];

			_rSrc.clear();

			return (*this);
		}

		void clear(void)
		{
			 mKeyIndex	= 0xffffffff;

			for(int it_ = 0 ; it_ < 7 ; it_ ++)
				mValues[it_] = 0.;
		}
	};
}
	typedef std::vector< NAnimation::CAMCkeyframe> AMCkeyframeArray;
	typedef std::unordered_map<std::tstring , AMCkeyframeArray > NameAMCkeyframeArray;
#endif