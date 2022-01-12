#include "stdafx.h"
#include "JointEnumerate.h"


namespace NAnimation
{
	ENUM_ROTATION_TYPE GetEulerRotationTypeFromString(const std::tstring& sType)
	{
		std::tstring sConverted = sType;
		std::transform(sConverted.begin(), sConverted.end(), sConverted.begin(), ToUpper());

		if (_T("XYZ") == sConverted)
			return ROT_EULER_XYZ;
		else if (_T("XZY") == sConverted)
			return ROT_EULER_XZY;
		else if (_T("YXZ") == sConverted)
			return ROT_EULER_YXZ;
		else if (_T("YZX") == sConverted)
			return ROT_EULER_YZX;
		else if (_T("ZXY") == sConverted)
			return ROT_EULER_ZXY;
		else if (_T("ZYX") == sConverted)
			return ROT_EULER_ZYX;
		else
		{
			assert(false);
			return ROT_EULER_XYZ;
		}
	}
}