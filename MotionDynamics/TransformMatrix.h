#ifndef __TRANSFORM_MATRIX_H_
#define __TRANSFORM_MATRIX_H_


#include "matrix.h"
#include "Quaternion.h"
#include <math.h>
#include "MathMethods.h"

namespace NAnimation
{
	class CTransformMatrix : public CMatrix<double, 4, 4>
	{
	public:
		CTransformMatrix(void);
		~CTransformMatrix(void);

		CTransformMatrix(
			const double& a11, const double& a12, const double& a13, const double& a14,
			const double& a21, const double& a22, const double& a23, const double& a24,
			const double& a31, const double& a32, const double& a33, const double& a34,
			const double& a41, const double& a42, const double& a43, const double& a44);

		CTransformMatrix(const CTransformMatrix& _rMat);
		CTransformMatrix(double x, double y, double z, double phi, double theta, double psi);
		CTransformMatrix(const CVector<double, 3>& _rPos, const CVector<double, 3>& _rRot);

		CTransformMatrix(const CVector<double, 3>& _rPos, const Quaternion<double>& _rQuater);

	public:
		void	zero(void);

		CMatrix<double, 3, 3>	getRotation(void);
		CVector<double, 3>		getRotation(int col);
		CVector<double, 3>		setOrientation(void);
		CVector<double, 3>		getWorldPosition(void);

	public:
		CTransformMatrix	operator *(const CTransformMatrix& _rSrc) const;
		CTransformMatrix&	operator *=(const CTransformMatrix& _rMat);
		CTransformMatrix&	operator =(const CMatrix<double, 4,4>& _rMat);
		CTransformMatrix&	operator = (const CTransformMatrix& _rMat);
		CVector<double, 3>	operator *(const CVector<double, 3>& _rVec) const;
	};


	class CTransformMatrixDiffX : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffX(void)	{};
		~CTransformMatrixDiffX(void){};

		CTransformMatrixDiffX(double x, double y, double z, double phi, double theta, double psi);
	};


	class CTransformMatrixDiffY : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffY(void) {};
		~CTransformMatrixDiffY(void){};

		CTransformMatrixDiffY(double x, double y, double z, double phi, double theta, double psi);
	};


	class CTransformMatrixDiffZ : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffZ(void) {};
		~CTransformMatrixDiffZ(void){};

		CTransformMatrixDiffZ(double x, double y, double z, double phi, double theta, double psi);
	};


	class CTransformMatrixDiffPsi : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffPsi(void) {};
		~CTransformMatrixDiffPsi(void){};

		CTransformMatrixDiffPsi(double x, double y, double z, double phi, double theta, double psi);
	};


	class CTransformMatrixDiffTheta : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffTheta(void) {};
		~CTransformMatrixDiffTheta(void){};

		CTransformMatrixDiffTheta(double x, double y, double z, double phi, double theta, double psi);
	};


	class CTransformMatrixDiffPhi : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffPhi(void) {};
		~CTransformMatrixDiffPhi(void){};

		CTransformMatrixDiffPhi(double x, double y, double z, double phi, double theta, double psi);
	};


	class CTransformMatrixDiffQuaternion : public CTransformMatrix
	{
	public:
		CTransformMatrixDiffQuaternion(void) {};
		~CTransformMatrixDiffQuaternion(void) {};

		CTransformMatrixDiffQuaternion(const CVector<double, 3>& _rDir, double _rTheta);
	};
};


#endif