
#ifndef __JOINT_ENUMERATE_H_
#define __JOINT_ENUMERATE_H_

#include "define.h"
#include "Character.h"

namespace NAnimation
{
	enum ENUM_PUSHED_DIRECTION
	{
		PUSHED_ROOT,
		PUSHED_LEFT,
		PUSHED_RIGHT
	};

	enum ENUM_ROTATION_TYPE
	{
		ROT_EULER_XYZ,
		ROT_EULER_XZY,
		ROT_EULER_YXZ,
		ROT_EULER_YZX,
		ROT_EULER_ZXY,
		ROT_EULER_ZYX,
		ROT_QUATERNION
	};

	enum ENUM_JOINT_TYPE
	{
		FIXED_JOINT,		// 고정된 조인트: 좌표를 이동하기위해 사용한다.
		REVOLUTE_JOINT,		// 회전 운동 조인트
		PRISMATIC_JOINT,	// 직선 운동 조인트
	};
	ENUM_ROTATION_TYPE GetEulerRotationTypeFromString(const std::tstring& sType);	
}


#endif