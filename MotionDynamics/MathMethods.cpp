#include "stdafx.h"
#include "MathMethods.h"

namespace NAnimation
{
	Quaternionf MakeQuaternionFromRotationMatrix(const Matrix4x4f& mat)
	{
		const float fTrace = mat.m11 + mat.m22 + mat.m33;
		if (fTrace > 0.0f)
		{
			const float S = sqrt(fTrace + 1.0f) * 2.0f;
			return Quaternionf(
				(mat.m32 - mat.m23) / S,
				(mat.m13 - mat.m31) / S,
				(mat.m21 - mat.m12) / S,
				(S * 0.25f));
		}
		else if ((mat.m11 > mat.m22) && (mat.m11 > mat.m33))
		{
			const float S = sqrt(1.0f + mat.m11 - mat.m22 - mat.m33) * 2.0f;
			return Quaternionf(
				(S * 0.25f),
				(mat.m12 + mat.m21) / S,
				(mat.m13 + mat.m31) / S,
				(mat.m32 - mat.m23) / S);
		}
		else if (mat.m22 > mat.m33)
		{
			const float S = sqrt(1.0f + mat.m22 - mat.m11 - mat.m33) * 2.0f;
			return Quaternionf(
				(mat.m12 + mat.m21) / S,
				(S * 0.25f),
				(mat.m23 + mat.m32) / S,
				(mat.m13 - mat.m31) / S);
		}
		else
		{
			const float S = sqrt(1.0f + mat.m33 - mat.m11 - mat.m22) * 2.0f;
			return Quaternionf(
				(mat.m13 + mat.m31) / S,
				(mat.m23 + mat.m32) / S,
				(S * 0.25f),
				(mat.m21 - mat.m12) / S);
		}
	}

	Quaterniond MakeQuaternionFromRotationMatrix(const Matrix4x4d& mat)
	{
		const double fTrace = mat.m11 + mat.m22 + mat.m33;
		if (fTrace > 0.0f)
		{
			const double S = sqrt(fTrace + 1.0f) * 2.0f;
			return Quaterniond(
				(mat.m32 - mat.m23) / S,
				(mat.m13 - mat.m31) / S,
				(mat.m21 - mat.m12) / S,
				(S * 0.25f));
		}
		else if ((mat.m11 > mat.m22) && (mat.m11 > mat.m33))
		{
			const double S = sqrt(1.0f + mat.m11 - mat.m22 - mat.m33) * 2.0f;
			return Quaterniond(
				(S * 0.25f),
				(mat.m12 + mat.m21) / S,
				(mat.m13 + mat.m31) / S,
				(mat.m32 - mat.m23) / S);
		}
		else if (mat.m22 > mat.m33)
		{
			const double S = sqrt(1.0f + mat.m22 - mat.m11 - mat.m33) * 2.0f;
			return Quaterniond(
				(mat.m12 + mat.m21) / S,
				(S * 0.25f),
				(mat.m23 + mat.m32) / S,
				(mat.m13 - mat.m31) / S);
		}
		else
		{
			const double S = sqrt(1.0f + mat.m33 - mat.m11 - mat.m22) * 2.0f;
			return Quaterniond(
				(mat.m13 + mat.m31) / S,
				(mat.m23 + mat.m32) / S,
				(S * 0.25f),
				(mat.m21 - mat.m12) / S);
		}
	}


	Quaterniond MakeQuaterniondFromEulerRadianAngles(
		const Vector3d& v,
		ENUM_ROTATION_TYPE eRotationOrder)
	{
		const double fAngleX = v.x;
		const double fAngleY = v.y;
		const double fAngleZ = v.z;

		Quaterniond qRotX = MakeQuaterniondFromAxisAndAngle(Vector3d(1.0, 0.0, 0.0), fAngleX);
		Quaterniond qRotY = MakeQuaterniondFromAxisAndAngle(Vector3d(0.0, 1.0, 0.0), fAngleY);
		Quaterniond qRotZ = MakeQuaterniondFromAxisAndAngle(Vector3d(0.0, 0.0, 1.0), fAngleZ);

		switch (eRotationOrder)
		{
		case ROT_EULER_XYZ:
			return qRotZ * qRotY * qRotX;

		case ROT_EULER_XZY:
			return qRotY * qRotZ * qRotX;

		case ROT_EULER_YXZ:
			return qRotZ * qRotX * qRotY;

		case ROT_EULER_YZX:
			return qRotX * qRotZ * qRotY;

		case ROT_EULER_ZXY:
			return qRotY * qRotX * qRotZ;

		case ROT_EULER_ZYX:
			return qRotX * qRotY * qRotZ;

		default:
			assert(false);
			return qRotZ * qRotY * qRotX;
		}
	}

	Quaternionf MakeQuaternionfFromEulerRadianAngles(
		const Vector3f& v,
		ENUM_ROTATION_TYPE eRotationOrder)
	{
		const float fAngleX = v.x;
		const float fAngleY = v.y;
		const float fAngleZ = v.z;

		Quaternionf qRotX = MakeQuaternionfFromAxisAndAngle(Vector3f(1.0f, 0.0f, 0.0f), fAngleX);
		Quaternionf qRotY = MakeQuaternionfFromAxisAndAngle(Vector3f(0.0f, 1.0f, 0.0f), fAngleY);
		Quaternionf qRotZ = MakeQuaternionfFromAxisAndAngle(Vector3f(0.0f, 0.0f, 1.0f), fAngleZ);

		switch (eRotationOrder)
		{
		case ROT_EULER_XYZ:
			return qRotZ * qRotY * qRotX;

		case ROT_EULER_XZY:
			return qRotY * qRotZ * qRotX;

		case ROT_EULER_YXZ:
			return qRotZ * qRotX * qRotY;

		case ROT_EULER_YZX:
			return qRotX * qRotZ * qRotY;

		case ROT_EULER_ZXY:
			return qRotY * qRotX * qRotZ;

		case ROT_EULER_ZYX:
			return qRotX * qRotY * qRotZ;

		default:
			assert(false);
			return qRotZ * qRotY * qRotX;
		}
	}

	Quaterniond MakeQuaterniondFromEulerDegreeAngles(
		const Vector3d& v,
		ENUM_ROTATION_TYPE eRotationOrder)
	{
		const double fAngleX = (v.x*PI_D) / 180.0;
		const double fAngleY = (v.y*PI_D) / 180.0;
		const double fAngleZ = (v.z*PI_D) / 180.0;

		Quaterniond qRotX = MakeQuaterniondFromAxisAndAngle(Vector3d(1.0, 0.0, 0.0), fAngleX);
		Quaterniond qRotY = MakeQuaterniondFromAxisAndAngle(Vector3d(0.0, 1.0, 0.0), fAngleY);
		Quaterniond qRotZ = MakeQuaterniondFromAxisAndAngle(Vector3d(0.0, 0.0, 1.0), fAngleZ);

		switch (eRotationOrder)
		{
		case ROT_EULER_XYZ:
			return qRotZ * qRotY * qRotX;

		case ROT_EULER_XZY:
			return qRotY * qRotZ * qRotX;

		case ROT_EULER_YXZ:
			return qRotZ * qRotX * qRotY;

		case ROT_EULER_YZX:
			return qRotX * qRotZ * qRotY;

		case ROT_EULER_ZXY:
			return qRotY * qRotX * qRotZ;

		case ROT_EULER_ZYX:
			return qRotX * qRotY * qRotZ;

		default:
			assert(false);
			return qRotZ * qRotY * qRotX;
		}
	}

	Quaterniond MakeQuaterniondFromAxisAndAngle(
		const Vector3d& vAxis,
		const double dAngle)
	{
		const double fCosValue = cos(dAngle * 0.5);
		const double fSinValue = sin(dAngle * 0.5);

		return Quaterniond(vAxis.x*fSinValue, vAxis.y*fSinValue, vAxis.z*fSinValue, fCosValue);
	}

	Quaternionf MakeQuaternionfFromAxisAndAngle(
		const Vector3f& vAxis,
		const float fAngle)
	{
		const float fCosValue = cos(fAngle * 0.5f);
		const float fSinValue = sin(fAngle * 0.5f);

		return Quaternionf(vAxis.x*fSinValue, vAxis.y*fSinValue, vAxis.z*fSinValue, fCosValue);
	}


	Quaternionf MakeQuaternionfFromEulerDegreeAngles(
		const Vector3f& v,
		ENUM_ROTATION_TYPE eRotationOrder)
	{
		const float fAngleX = (v.x*PI_F) / 180.0f;
		const float fAngleY = (v.y*PI_F) / 180.0f;
		const float fAngleZ = (v.z*PI_F) / 180.0f;

		Quaternionf qRotX = MakeQuaternionfFromAxisAndAngle(Vector3f(1.0f, 0.0f, 0.0f), fAngleX);
		Quaternionf qRotY = MakeQuaternionfFromAxisAndAngle(Vector3f(0.0f, 1.0f, 0.0f), fAngleY);
		Quaternionf qRotZ = MakeQuaternionfFromAxisAndAngle(Vector3f(0.0f, 0.0f, 1.0f), fAngleZ);

		switch (eRotationOrder)
		{
		case ROT_EULER_XYZ:
			return qRotZ * qRotY * qRotX;

		case ROT_EULER_XZY:
			return qRotY * qRotZ * qRotX;

		case ROT_EULER_YXZ:
			return qRotZ * qRotX * qRotY;

		case ROT_EULER_YZX:
			return qRotX * qRotZ * qRotY;

		case ROT_EULER_ZXY:
			return qRotY * qRotX * qRotZ;

		case ROT_EULER_ZYX:
			return qRotX * qRotY * qRotZ;

		default:
			assert(false);
			return qRotZ * qRotY * qRotX;
		}
	}

	Matrix4x4f MakeRotationMatrix4x4fFromQuaternion(const Quaternionf& q)
	{
		return Matrix4x4f(
			1.0f - 2.0f*(q.y*q.y) - 2.0f*(q.z*q.z),
			2.0f*(q.x*q.y) - 2.0f*(q.z*q.w),
			2.0f*(q.x*q.z) + 2.0f*(q.y*q.w),
			0.0f,
			2.0f*(q.x*q.y) + 2.0f*(q.z*q.w),
			1.0f - 2.0f*(q.x*q.x) - 2.0f*(q.z*q.z),
			2.0f*(q.y*q.z) - 2.0f*(q.x*q.w),
			0.0f,
			2.0f*(q.x*q.z) - 2.0f*(q.y*q.w),
			2.0f*(q.y*q.z) + 2.0f*(q.x*q.w),
			1.0f - 2.0f*(q.x*q.x) - 2.0f*(q.y*q.y),
			0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	};


	Matrix4x4d MakeRotationMatrix4x4dFromQuaternion(const Quaterniond& q)
	{
		return Matrix4x4d(
			1.0f - 2.0f*(q.y*q.y) - 2.0f*(q.z*q.z),
			2.0f*(q.x*q.y) - 2.0f*(q.z*q.w),
			2.0f*(q.x*q.z) + 2.0f*(q.y*q.w),
			0.0f,
			2.0f*(q.x*q.y) + 2.0f*(q.z*q.w),
			1.0f - 2.0f*(q.x*q.x) - 2.0f*(q.z*q.z),
			2.0f*(q.y*q.z) - 2.0f*(q.x*q.w),
			0.0f,
			2.0f*(q.x*q.z) - 2.0f*(q.y*q.w),
			2.0f*(q.y*q.z) + 2.0f*(q.x*q.w),
			1.0f - 2.0f*(q.x*q.x) - 2.0f*(q.y*q.y),
			0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	};


	bool isValidValue(const float fValue)
	{
		static const float INDETERMINED = sqrt(-1.0000001f);
		return (
			(false == std::numeric_limits<float>::has_infinity || fValue != std::numeric_limits<float>::infinity()) &&
			(false == std::numeric_limits<float>::has_quiet_NaN || fValue != std::numeric_limits<float>::quiet_NaN()) &&
			(false == std::numeric_limits<float>::has_signaling_NaN || fValue != std::numeric_limits<float>::signaling_NaN()) &&
			(*(unsigned int*)&fValue != *(unsigned int*)&INDETERMINED));
	}


	double sign(double a)
	{
		return (0. < a) ? 1. : ((a < 0) ? -1. : 0.);
	}



	void	MakeAxisAndAngleFromQuaternionf(const Quaternionf& q, CVector<float, 3>* axis, float fAngle)
	{
		Quaternionf result	= q;
		result.Normalize();

		float fHalfSin2	= 1- result.w * result.w;
		float fHalfSin	= sqrt( 1 - result.w * result.w );
		float fTheta	= 0.f;

		if(fHalfSin != 0.f)
		{
			result.x /= fHalfSin;
			result.y /= fHalfSin;
			result.z /= fHalfSin;

			axis->x = result.x;
			axis->y = result.y;
			axis->z = result.z;
		}
		if(result.w == 1)
			fAngle = fTheta = 0.f;

		// 각 구하기
		float	fSin	= fHalfSin * result.w;
		float	fCos	= result.w * result.w - fHalfSin * fHalfSin;

		// PI /2 ~ PI
		if(fSin > 0 && fCos < 0)
			fTheta = acos(fCos) + NAnimation::PI_F / 2;

		// -PI /2 ~ 0
		else if(fSin < 0 && fCos > 0)
			fTheta = - acos(fCos);

		// -PI ~ - PI /2
		else if(fSin < 0 && fCos < 0)
			fTheta = - acos(fCos) - PI_F / 2;

		// PI /2
		else if(fCos == 0)
		{
			if(fSin == 1)
				fTheta = PI_F / 2;
			else 
				fTheta = - PI_F / 2;
		}
		// 0 ~ PI /2 
		else// if(fSin > 0 && fCos > 0)
			fTheta = acos(fCos);

		fAngle = fTheta;
	}


	void	MakeAxisAndAngleFromQuaterniond(const Quaterniond& q, CVector<double, 3>* axis, double fAngle)
	{
		Quaterniond result	= q;
		result.Normalize();

		double fHalfSin2	= 1- result.w * result.w;
		double fHalfSin	= sqrt( 1 - result.w * result.w );
		double fTheta	= 0.f;

		if(fHalfSin != 0.f)
		{
			result.x /= fHalfSin;
			result.y /= fHalfSin;
			result.z /= fHalfSin;

			axis->x = result.x;
			axis->y = result.y;
			axis->z = result.z;
		}
		if(result.w == 1)
			fAngle = fTheta = 0.f;

		// 각 구하기
		double	fSin	= fHalfSin * result.w;
		double	fCos	= result.w * result.w - fHalfSin * fHalfSin;

		// PI /2 ~ PI
		if(fSin > 0 && fCos < 0)
			fTheta = acos(fCos) + NAnimation::PI_F / 2;

		// -PI /2 ~ 0
		else if(fSin < 0 && fCos > 0)
			fTheta = - acos(fCos);

		// -PI ~ - PI /2
		else if(fSin < 0 && fCos < 0)
			fTheta = - acos(fCos) - PI_F / 2;

		// PI /2
		else if(fCos == 0)
		{
			if(fSin == 1)
				fTheta = PI_F / 2;
			else 
				fTheta = - PI_F / 2;
		}
		// 0 ~ PI /2 
		else// if(fSin > 0 && fCos > 0)
			fTheta = acos(fCos);

		fAngle = fTheta;
	}
}
