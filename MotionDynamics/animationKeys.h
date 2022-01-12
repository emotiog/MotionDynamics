#ifndef __ANIMATION_KEY_H_
#define __ANIMATION_KEY_H_

#include "define.h"

namespace NAnimation
{
	class CPosKey
	{
	public:
		float		fTime;
		Vector3d	vPos;

	public:
		CPosKey(void) : fTime(0.f) {}
		
		CPosKey(const CPosKey& _rSrc)
			: fTime(_rSrc.fTime),
			vPos(_rSrc.vPos)
		{
		}

		
		CPosKey(CPosKey&& _rSrc)
		{
			(*this) = std::move(_rSrc);
		}

		CPosKey& operator=(const CPosKey& _rSrc)
		{
			fTime	= _rSrc.fTime;
			vPos	= _rSrc.vPos;

			return (*this);
		}

		CPosKey& operator=(CPosKey&& _rSrc)
		{
			fTime	= _rSrc.fTime;
			vPos	= _rSrc.vPos;

			_rSrc.clear();

			return (*this);
		}

		void clear(void)
		{
			fTime	= 0.f;
			vPos	= Vector3d(0.f, 0.f, 0.f);
		}
	};


	class CRotKey
	{
	public:
		float		fTime;
		Quaterniond	qRot;

	public:
		CRotKey(void): fTime(0.f) {}
		
		CRotKey(const CRotKey& _rSrc)
			: fTime(_rSrc.fTime),
		qRot(_rSrc.qRot)
		{
		}

		CRotKey(CRotKey&& _rSrc)
		{
			(*this) = std::move(_rSrc);
		}

		CRotKey& operator=(const CRotKey& _rSrc)
		{
			fTime	= _rSrc.fTime;
			qRot	= _rSrc.qRot;

			return (*this);
		}

		CRotKey& operator=(CRotKey&& _rSrc)
		{
			fTime	= _rSrc.fTime;
			qRot	= _rSrc.qRot;

			_rSrc.clear();

			return (*this);
		}

		void clear(void)
		{
			fTime	= 0.f;
			qRot	= Quaterniond();
		}
	};
	
	typedef std::vector< CRotKey > RotKeyArray;
	typedef std::vector< CPosKey > PosKeyArray;
}
#endif
