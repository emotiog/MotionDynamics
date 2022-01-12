#include "StdAfx.h"
#include "TransformMatrix.h"


namespace NAnimation
{
	CTransformMatrix::CTransformMatrix(void)
	{
		identity();
	}


	CTransformMatrix::~CTransformMatrix(void)
	{
	}


	CTransformMatrix::CTransformMatrix(
			const double& a11, const double& a12, const double& a13, const double& a14,
			const double& a21, const double& a22, const double& a23, const double& a24,
			const double& a31, const double& a32, const double& a33, const double& a34,
			const double& a41, const double& a42, const double& a43, const double& a44)			
			: CMatrix(a11, a12, a13, a14,
						a21, a22, a23, a24,
						a31, a32, a33, a34,
						a41, a42, a43, a44)
	{
	}

	
	CTransformMatrix::CTransformMatrix(const CTransformMatrix& _rMat)
	{
		*this = _rMat;
	}


	CTransformMatrix::CTransformMatrix(const CVector<double, 3>& _rPos, const Quaternion<double>& _rQuat)
	{
		// 회전량 정의
		MakeRotationMatrix4x4dFromQuaternion(_rQuat);

		// 이동 값 정의
		m14	= _rPos.x;
		m24	= _rPos.y;
		m34 = _rPos.z;
		
		m41	= 0.f;									m42	= 0.f;
		m43	= 0.f;									m44	= 1.f;
	}


	CTransformMatrix::CTransformMatrix(double x, double y, double z, double phi, double theta, double psi)
	{
		double sphi	= sin(phi);		double cphi	= cos(phi);
		double stht	= sin(theta);	double ctht	= cos(theta);
		double spsi	= sin(psi);		double cpsi	= cos(psi);


		m11	= cpsi * ctht;							m12	= cpsi * stht * sphi - spsi * cphi;
		m13	= cpsi * stht * cphi + spsi * sphi;		m14	= x;

		m21	= spsi * ctht;							m22 = spsi * stht * sphi + cpsi * cphi;
		m23	= spsi * stht * cphi - cpsi * sphi;		m24	= y;

		m31	= -stht;								m32	= ctht * sphi;
		m33	= ctht * cphi;							m34	= z;

		m41	= 0.f;									m42	= 0.f;
		m43	= 0.f;									m44	= 1.f;
	}


	CTransformMatrix::CTransformMatrix(const CVector<double, 3>& _rPos, const CVector<double, 3>& _rRot)
	{
		double sphi	= sin(_rRot.x);		double cphi	= cos(_rRot.x);
		double stht	= sin(_rRot.y);		double ctht	= cos(_rRot.y);
		double spsi	= sin(_rRot.z);		double cpsi	= cos(_rRot.z);


		m11	= cpsi * ctht;							m12	= cpsi * stht * sphi - spsi * cphi;
		m13	= cpsi * stht * cphi + spsi * sphi;		m14	= _rPos.x;

		m21	= spsi * ctht;							m22 = spsi * stht * sphi + cpsi * cphi;
		m23	= spsi * stht * cphi - cpsi * sphi;		m24	= _rPos.y;

		m31	= -stht;								m32	= ctht * sphi;
		m33	= ctht * cphi;							m34	= _rPos.z;

		m41	= 0.f;									m42	= 0.f;
		m43	= 0.f;									m44	= 1.f;
	}


	void CTransformMatrix::zero(void)
	{
		m11	= 0.f;	m12	= 0.f;	m13	= 0.f;	m14	= 0.f;
		m21	= 0.f;	m22	= 0.f;	m23	= 0.f;	m24	= 0.f;
		m31	= 0.f;	m32	= 0.f;	m33	= 0.f;	m34	= 0.f;
		m41	= 0.f;	m42	= 0.f;	m43	= 0.f;	m44	= 0.f;
	}


	CMatrix<double, 3, 3> CTransformMatrix::getRotation(void)
	{
		CMatrix<double, 3, 3> R;


		for(int row = 0 ; row < 3 ; row ++)
		{
			for(int col = 0 ; col < 3 ; col ++)
			{
				R.elements[row][col]	= elements[row][col];
			}
		}

		return R;
	}

	// ---- get Rotation ---------------------------------------------
	//	a라는 world 좌표계에서의 z-axis 방향 단위 벡터에 값을 넣는 과정
	//	element의 어떤 축을 가져오는지를 가져오는 것
	CVector<double, 3>	CTransformMatrix::getRotation(int col)
	{
		assert( 0 <= col && col < 3);

		CVector<double, 3>	v;

		v.x	= elements[0][col];
		v.y	= elements[1][col];
		v.z	= elements[2][col];

		return v;
	}


	CVector<double, 3>	CTransformMatrix::setOrientation(void)
	{
		CVector<double, 3>	v;

		v.x	= atan2( m32, m33);
		v.y	= atan2( -m31, sqrt(m32 * m32 + m33 * m33) );
		v.z	= atan2( m21, m11);

		return v;
	}


	CVector<double, 3>	CTransformMatrix::getWorldPosition(void)
	{
		CVector<double, 3>	p;

		p.x	= m14;
		p.y	= m24;
		p.z = m34;

		return p;
	}


	CTransformMatrix CTransformMatrix::operator *(const CTransformMatrix& _rMat) const
	{ //연산자는 overloading이 안되게 c++에서 막아놨음 (그 외, friend, 생성자, 소멸자 는 안됨)
		return CTransformMatrix(
				m11*_rMat.m11 + m12*_rMat.m21 + m13*_rMat.m31 + m14*_rMat.m41,
				m11*_rMat.m12 + m12*_rMat.m22 + m13*_rMat.m32 + m14*_rMat.m42,
				m11*_rMat.m13 + m12*_rMat.m23 + m13*_rMat.m33 + m14*_rMat.m43,
				m11*_rMat.m14 + m12*_rMat.m24 + m13*_rMat.m34 + m14*_rMat.m44,

				m21*_rMat.m11 + m22*_rMat.m21 + m23*_rMat.m31 + m24*_rMat.m41,
				m21*_rMat.m12 + m22*_rMat.m22 + m23*_rMat.m32 + m24*_rMat.m42,
				m21*_rMat.m13 + m22*_rMat.m23 + m23*_rMat.m33 + m24*_rMat.m43,
				m21*_rMat.m14 + m22*_rMat.m24 + m23*_rMat.m34 + m24*_rMat.m44,

				m31*_rMat.m11 + m32*_rMat.m21 + m33*_rMat.m31 + m34*_rMat.m41,
				m31*_rMat.m12 + m32*_rMat.m22 + m33*_rMat.m32 + m34*_rMat.m42,
				m31*_rMat.m13 + m32*_rMat.m23 + m33*_rMat.m33 + m34*_rMat.m43,
				m31*_rMat.m14 + m32*_rMat.m24 + m33*_rMat.m34 + m34*_rMat.m44,

				m41*_rMat.m11 + m42*_rMat.m21 + m43*_rMat.m31 + m44*_rMat.m41,
				m41*_rMat.m12 + m42*_rMat.m22 + m43*_rMat.m32 + m44*_rMat.m42,
				m41*_rMat.m13 + m42*_rMat.m23 + m43*_rMat.m33 + m44*_rMat.m43,
				m41*_rMat.m14 + m42*_rMat.m24 + m43*_rMat.m34 + m44*_rMat.m44);
	}


	CTransformMatrix& CTransformMatrix::operator *= (const CTransformMatrix& _rMat)
	{
		elementType a11 = m11*_rMat.m11 + m12*_rMat.m21 + m13*_rMat.m31 + m14*_rMat.m41;
		elementType a12 = m11*_rMat.m12 + m12*_rMat.m22 + m13*_rMat.m32 + m14*_rMat.m42;
		elementType a13 = m11*_rMat.m13 + m12*_rMat.m23 + m13*_rMat.m33 + m14*_rMat.m43;
		elementType a14 = m11*_rMat.m14 + m12*_rMat.m24 + m13*_rMat.m34 + m14*_rMat.m44;

		elementType a21 = m21*_rMat.m11 + m22*_rMat.m21 + m23*_rMat.m31 + m24*_rMat.m41;
		elementType a22 = m21*_rMat.m12 + m22*_rMat.m22 + m23*_rMat.m32 + m24*_rMat.m42;
		elementType a23 = m21*_rMat.m13 + m22*_rMat.m23 + m23*_rMat.m33 + m24*_rMat.m43;
		elementType a24 = m21*_rMat.m14 + m22*_rMat.m24 + m23*_rMat.m34 + m24*_rMat.m44;

		elementType a31 = m31*_rMat.m11 + m32*_rMat.m21 + m33*_rMat.m31 + m34*_rMat.m41;
		elementType a32 = m31*_rMat.m12 + m32*_rMat.m22 + m33*_rMat.m32 + m34*_rMat.m42;
		elementType a33 = m31*_rMat.m13 + m32*_rMat.m23 + m33*_rMat.m33 + m34*_rMat.m43;
		elementType a34 = m31*_rMat.m14 + m32*_rMat.m24 + m33*_rMat.m34 + m34*_rMat.m44;

		elementType a41 = m41*_rMat.m11 + m42*_rMat.m21 + m43*_rMat.m31 + m44*_rMat.m41;
		elementType a42 = m41*_rMat.m12 + m42*_rMat.m22 + m43*_rMat.m32 + m44*_rMat.m42;
		elementType a43 = m41*_rMat.m13 + m42*_rMat.m23 + m43*_rMat.m33 + m44*_rMat.m43;
		elementType a44 = m41*_rMat.m14 + m42*_rMat.m24 + m43*_rMat.m34 + m44*_rMat.m44;

		m11 = a11; m12 = a12; m13 = a13; m14 = a14;
		m21 = a21; m22 = a22; m23 = a23; m24 = a24;
		m31 = a31; m32 = a32; m33 = a33; m34 = a34;
		m41 = a41; m42 = a42; m43 = a43; m44 = a44;

		return (*this);
	}


	CTransformMatrix& CTransformMatrix::operator = (const CMatrix<double, 4, 4>& _rMat)
	{
		m11	= _rMat.m11;	m12	= _rMat.m12;	m13	= _rMat.m13;	m14	= _rMat.m14;
		m21	= _rMat.m21;	m22	= _rMat.m22;	m23	= _rMat.m23;	m24	= _rMat.m24;
		m31	= _rMat.m31;	m32	= _rMat.m32;	m33	= _rMat.m33;	m34	= _rMat.m34;
		m41	= _rMat.m41;	m42	= _rMat.m42;	m43	= _rMat.m43;	m44	= _rMat.m44;

		return (*this);
	}


	CTransformMatrix& CTransformMatrix::operator = (const CTransformMatrix& _rMat)
	{
		m11	= _rMat.m11;	m12	= _rMat.m12;	m13	= _rMat.m13;	m14	= _rMat.m14;
		m21	= _rMat.m21;	m22	= _rMat.m22;	m23	= _rMat.m23;	m24	= _rMat.m24;
		m31	= _rMat.m31;	m32	= _rMat.m32;	m33	= _rMat.m33;	m34	= _rMat.m34;
		m41	= _rMat.m41;	m42	= _rMat.m42;	m43	= _rMat.m43;	m44	= _rMat.m44;

		return (*this);
	}



	CVector<double, 3> CTransformMatrix::operator * (const CVector<double, 3>& _rVec) const
	{

		const double	one	= static_cast<double>(1);
		const double inv_w	= one / (m41 * _rVec.x + m42 * _rVec.y + m43 * _rVec.z + m44);

		return CVector<double, 3>(
			(m11 * _rVec.x + m12 * _rVec.y + m13 * _rVec.z + m14) * inv_w,
			(m21 * _rVec.x + m22 * _rVec.y + m23 * _rVec.z + m24) * inv_w,
			(m31 * _rVec.x + m32 * _rVec.y + m33 * _rVec.z + m34) * inv_w);
	}


	CTransformMatrixDiffX::CTransformMatrixDiffX(double x, double y, double z, double phi, double theta, double psi)
	{
		m11	= 0.f;		m12	= 0.f;		m13	= 0.f;		m14	= 1.f;
		m21	= 0.f;		m22	= 0.f;		m23	= 0.f;		m24	= 0.f;
		m31	= 0.f;		m32	= 0.f;		m33	= 0.f;		m34	= 0.f;
		m41	= 0.f;		m42	= 0.f;		m43	= 0.f;		m44	= 0.f;
	}


	CTransformMatrixDiffY::CTransformMatrixDiffY(double x, double y, double z, double phi, double theta, double psi)
	{
		m11	= 0.f;		m12	= 0.f;		m13	= 0.f;		m14	= 0.f;
		m21	= 0.f;		m22	= 0.f;		m23	= 0.f;		m24	= 1.f;
		m31	= 0.f;		m32	= 0.f;		m33	= 0.f;		m34	= 0.f;
		m41	= 0.f;		m42	= 0.f;		m43	= 0.f;		m44	= 0.f;
	}


	CTransformMatrixDiffZ::CTransformMatrixDiffZ(double x, double y, double z, double phi, double theta, double psi)
	{
		m11	= 0.f;		m12	= 0.f;		m13	= 0.f;		m14	= 0.f;
		m21	= 0.f;		m22	= 0.f;		m23	= 0.f;		m24	= 0.f;
		m31	= 0.f;		m32	= 0.f;		m33	= 0.f;		m34	= 1.f;
		m41	= 0.f;		m42	= 0.f;		m43	= 0.f;		m44	= 0.f;
	}


	CTransformMatrixDiffPsi::CTransformMatrixDiffPsi(double x, double y, double z, double phi, double theta, double psi)
	{
		double sphi	= sin(phi);			double cphi	= cos(phi);
		double stht	= sin(theta);		double ctht	= cos(theta);
		double spsi	= sin(psi);			double cpsi	= cos(psi);

		m11	= -ctht *spsi;							m12	= -sphi *stht *spsi - cphi *cpsi;		
		m13	= -cphi *stht *spsi + sphi *cpsi;		m14	= 0.f;

		m21	= ctht *cpsi;							m22	= sphi *stht *cpsi - cphi *spsi;
		m23	= cphi *stht *cpsi + sphi *spsi;		m24	= 0.f;

		m31	= 0.f;		m32	= 0.f;		m33	= 0.f;		m34	= 0.f;
		m41	= 0.f;		m42	= 0.f;		m43	= 0.f;		m44	= 0.f;
	}


	CTransformMatrixDiffTheta::CTransformMatrixDiffTheta(double x, double y, double z, double phi, double theta, double psi)
	{
		double sphi	= sin(phi);			double cphi	= cos(phi);
		double stht	= sin(theta);		double ctht	= cos(theta);
		double spsi	= sin(psi);			double cpsi	= cos(psi);

		m11	= -stht *cpsi;							m12	= sphi *ctht *cpsi;
		m13	= cphi *ctht *cpsi;						m14	= 0.f;

		m21	= -stht *spsi;							m22	= sphi *ctht *spsi;
		m33	= cphi *ctht *spsi;						m24	= 0.f;

		m41	= 0.f;		m42	= 0.f;		m43	= 0.f;		m44	= 0.f;
	}


	CTransformMatrixDiffPhi::CTransformMatrixDiffPhi(double x, double y, double z, double phi, double theta, double psi)
	{
		double sphi	= sin(phi);			double cphi	= cos(phi);
		double stht	= sin(theta);		double ctht	= cos(theta);
		double spsi	= sin(psi);			double cpsi	= cos(psi);

		m11	= 0.f;									m12	= cphi *stht *cpsi + sphi *spsi;
		m13	= -sphi *stht *cpsi + cphi *spsi;		m14	= 0.f;

		m21	= 0.f;									m22	= cphi *stht *spsi - sphi *cpsi;
		m23	= -sphi *stht *spsi - cphi *cpsi;		m24	= 0.f;

		m41	= 0.f;		m42	= 0.f;		m43	= 0.f;		m44	= 0.f;
	}


	CTransformMatrixDiffQuaternion::CTransformMatrixDiffQuaternion(const CVector<double, 3>& _rDir, double _rTheta)
	{
		double fSin	= sin(_rTheta/2);
		double fCos	= cos(_rTheta/2);
		
		double fCos2_fSin2	= fCos * fCos - fSin * fSin;
		double doublefSinXfCos		= 2 * fSin * fCos;

		m11	= -doublefSinXfCos * (_rDir.y * _rDir.y + _rDir.z * _rDir.z);
		m12	=  _rDir.x * _rDir.y * doublefSinXfCos - _rDir.z * fCos2_fSin2;
		m13	=  _rDir.x * _rDir.z * doublefSinXfCos + _rDir.y * fCos2_fSin2;
		m14	= 0.f;

		m21	=  _rDir.x * _rDir.y * doublefSinXfCos + _rDir.z * fCos2_fSin2;
		m22	= -doublefSinXfCos * ( _rDir.x * _rDir.x + _rDir.z * _rDir.z);
		m23	=  _rDir.y * _rDir.z * doublefSinXfCos - _rDir.x * fCos2_fSin2;
		m24	= 0.f;

		m31 =  _rDir.x * _rDir.z * doublefSinXfCos - _rDir.y * fCos2_fSin2;
		m32	=  _rDir.y * _rDir.z * doublefSinXfCos - _rDir.x * fCos2_fSin2;
		m33	= -doublefSinXfCos * (_rDir.x * _rDir.x + _rDir.y * _rDir.y);
		m34	= 0.f;

		m41	= 0.f;
		m42	= 0.f;
		m43	= 0.f;
		m44	= 0.f;
	}
}