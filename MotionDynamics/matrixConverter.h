

#ifndef __MATRIX_CONVERTER_H_
#define __MATRIX_CONVERTER_H_

#include "define.h"
#include "DynamicMatrix.h"
#include "Matrix.h"

#include <assert.h>

namespace NAnimation
{
	template<typename T, unsigned int ROW, unsigned int COLUMN> 
		CMatrix<T, ROW, COLUMN> convertToMatrix (const CDynamicMatrix<T>& _rSrc)
	{// CDynamicMatrix -> CMatrix
		assert(_rSrc->m.mRow == ROW && _rSrc->m.mCol == COLUMN);

		CMatrix<T, ROW, COLUMN> temp;
		for(size_t _i = 0 ; _i < ROW ; _i ++)
		{
			for(size_t _j = 0 ; _j < COLUMN ; _j ++)
			{
				temp.elements[_i][_j]	= _rSrc.m->m2D[_i][_j];
			}
		}

		return temp;
	}


	template<typename T, unsigned int ROW, unsigned int COLUMN> 
		CDynamicMatrix<T> convertToMatrix (const CMatrix<T, ROW, COLUMN>& _rSrc)
	{// CDynamicMatrix -> CMatrix
		CDynamicMatrix<T> temp(ROW, COLUMN);

		for(size_t _i = 0 ; _i < ROW ; _i ++)
		{
			for(size_t _j = 0 ; _j < COLUMN ; _j ++)
			{
				temp.m->m2D[_i][_j]	= _rSrc.elements[_i][_j];
			}
		}

		return temp;
	}


	template <class T>	CDynamicMatrix<T>
		rotationMatrix(T _phi, T _theta, T _psi)
	{
		T sphi	= sin(_phi);		T cphi	= cos(_phi);
		T stht	= sin(_theta);		T ctht	= cos(_theta);
		T spsi	= sin(_psi);		T cpsi	= cos(_psi);

		CDynamicMatrix<T> A(3,3);

		A(0,0) = cpsi *ctht;	A(0,1) = cpsi *stht *sphi -spsi *cphi;	A(0,2) = cpsi *stht *cphi +spsi *sphi;
		A(1,0) = spsi *ctht;	A(1,1) = spsi *stht *sphi +cpsi *cphi;	A(1,2) = spsi *stht *cphi -cpsi *sphi;
		A(2,0) = -stht;			A(2,1) = ctht *sphi;					A(2,2) = ctht *cphi;

		return A;
	}


	template <typename T>	CMatrix<T, 3, 3>
		rotationMatrix(T _phi, T _theta, T _psi)
	{
		T sphi	= sin(_phi);		T cphi	= cos(_phi);
		T stht	= sin(_theta);		T ctht	= cos(_theta);
		T spsi	= sin(_psi);		T cpsi	= cos(_psi);

		CMatrix<T, 3, 3> A;

		A.m11 = cpsi *ctht;		A.m12 = cpsi *stht *sphi -spsi *cphi;	A.m13 = cpsi *stht *cphi +spsi *sphi;
		A.m21 = spsi *ctht;		A.m22 = spsi *stht *sphi +cpsi *cphi;	A.m23 = spsi *stht *cphi -cpsi *sphi;
		A.m31 = -stht;			A.m32 = ctht *sphi;						A.m33 = ctht *cphi;

		return A;
	}


	template <typename T, unsigned int ROW, unsigned int COLUMN>	
	CDynamicMatrix<T> operator * (const CMatrix<T, ROW, COLUMN>& _rStaticMat,
									const CDynamicMatrix<T>& _rDynamicMat)
	{
		// 가로 크기 == 세로 크기
		assert(COLUMN == _rDynamicMat.m->mRow);

		CDynamicMatrix<T> result( ROW , _rDynamicMat.m->mCol);

		for(unsigned int row_ = 0 ; row_ < ROW ; row_ ++)
		{
			float factorSum = 0.f;

			for(unsigned int col_ = 0 ;  col_ < _rDynamicMat.m->mCol ; col_ ++)
			{
				factorSum	= 0.f;

				for(unsigned int k_ = 0 ; k_ < COLUMN ; k_ ++)
				{
					factorSum += _rStaticMat.elements[row_][k_] * _rDynamicMat.m[k_][col_];
				}

				result.m[row_][col_]	= factorSum;
			}
		}

		return result;
	}


	template <typename T, unsigned int ROW, unsigned int COLUMN>
	CDynamicMatrix<T> operator * (const CDynamicMatrix<T>& _rDynamicMat,
		const CMatrix<T, ROW, COLUMN>& _rStaticMat)
	{
		// 가로 크기 == 세로 크기
		assert(_rDynamicMat.m->mCol == ROW);

		CDynamicMatrix<T> result( _rDynamicMat.m->mRow , COLUMN);

		for(unsigned int row_ = 0 ; row_ < _rDynamicMat.m->mRow ; row_ ++)
		{
			float factorSum	= 0.f;
		
			for(unsigned int col_ = 0 ;col_ < COLUMN ; col_ ++)
			{
				factorSum = 0.f;

				for(unsigned int k_ = 0 ; k_ < ROW ; k_ ++)
					factorSum += _rDynamicMat.m->m2D[row_][k_] * _rStaticMat.elements[k_][col_];

				result.m->m2D[row_][col_]	= factorSum;
			}
		}

		return result;
	}


	template <typename T, int DIMENSION> CDynamicMatrix<T>
	operator * (const CVector<T, DIMENSION>& _rVec, const CDynamicMatrix<T>& _rMat)
	{
		assert(DIMENSION == _rMat.m->mRow);

		CDynamicMatrix<T> result(1 , _rMat.m->mCol);

		float factorSum = 0.f;
		
		for(unsigned int k_ = 0 ; k_ < _rMat.m->mCol ; k_ ++)
		{
			factorSum = 0.f;

			for(unsigned int col_ = 0 ; col_ < DIMENSION ; col_ ++)
				factorSum += _rVec.elements[col_] * _rMat.m->m2D[col_][k_];

			result.m->m2D[0][k_] = factorSum;
		}

		return result;
	}
};

#endif