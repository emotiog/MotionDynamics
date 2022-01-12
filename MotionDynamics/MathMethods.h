#ifndef __MATH_METHODS_H_
#define __MATH_METHODS_H_

#include "define.h"
#include "JointEnumerate.h"
#include "Constants.h"

namespace NAnimation
{

	Quaterniond MakeQuaterniondFromEulerRadianAngles(const Vector3d& v, ENUM_ROTATION_TYPE eRotationOrder);
	Quaternionf MakeQuaternionfFromEulerRadianAngles(const Vector3f& v, ENUM_ROTATION_TYPE eRotationOrder);
	Quaterniond MakeQuaterniondFromEulerDegreeAngles(const Vector3d& v, ENUM_ROTATION_TYPE eRotationOrder);
	Quaternionf MakeQuaternionfFromEulerDegreeAngles(const Vector3f& v, ENUM_ROTATION_TYPE eRotationOrder);

	Quaternionf MakeQuaternionFromRotationMatrix(const Matrix4x4f& Matrix);
	Quaterniond MakeQuaternionFromRotationMatrix(const Matrix4x4d& Matrix);

	Quaterniond MakeQuaterniondFromAxisAndAngle(const Vector3d& vAxis, double dAngle);
	Quaternionf MakeQuaternionfFromAxisAndAngle(const Vector3f& vAxis, float fAngle);

	Matrix4x4f	MakeRotationMatrix4x4fFromQuaternion(const Quaternionf& q);
	Matrix4x4d	MakeRotationMatrix4x4dFromQuaternion(const Quaterniond& q);

	bool	isValidValue(const float fValue);

	// + or -
	double	sign(double a);

	void	MakeAxisAndAngleFromQuaternionf(const Quaternionf& q, CVector<float, 3>* axis, float fAngle);
	void	MakeAxisAndAngleFromQuaterniond(const Quaterniond& q, CVector<double, 3>* axis, double fAngle);

}

#endif