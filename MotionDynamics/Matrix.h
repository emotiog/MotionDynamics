#pragma once

#include "Vector.h"
// matrix


namespace NAnimation
{
#pragma pack(push)
#pragma pack(1)

	template <typename T, unsigned int ROW, unsigned int COLUMN>
	class CMatrix
	{
	public:
		typedef	typename T		elementType;
		typedef unsigned int	sizeType;

		typedef CMatrix<T, ROW, COLUMN> matType;
		typedef CVector<T, COLUMN>		colVecType;
		typedef CVector<T, ROW>			rowVecType;

	public:
		union
		{
			elementType	elements[ROW][COLUMN];
			elementType element1D[ROW * COLUMN];
		};

	public:
		CMatrix(void)
		{
			const elementType	ZERO	= elementType();
			const elementType	ONE		= static_cast<elementType>(1);

			for(sizeType row = 0 ; row < ROW ; row++)
			{
				for(sizeType col = 0 ; col < COLUMN ; col ++)
				{
					if( row == col)
					{
						elements[row][col] = ONE;	
					}
					else
						elements[row][col] = ZERO;
				}
			}
		}

		CMatrix(const elementType* _pElements)
		{
			const sizeType	TOTAL	= ROW * COLUMN;

			for(sizeType iter = 0 ; iter < TOTAL ; iter ++)
			{
				element1D[iter ] = _pElements[iter];
			}
		}

		CMatrix(const matType& _rMat)
		{
			const sizeType TOTAL	= ROW * COLUMN;

			for (sizeType iter = 0 ; iter < TOTAL ; iter++)
			{
				element1D[ iter ] = _rMat.element1D[iter];
			}
		}

	public:
		matType& operator=(const matType& _rSrc)
		{
			if( this != &_rSrc)
			{
				const sizeType	TOTAL = ROW * COLUMN;

				for (sizeType iter = 0 ; iter < TOTAL ; iter ++)
				{
					element1D[ iter] = _rSrc.element1D[ iter];
				}
			}

			return (*this);
		}

		elementType& at(sizeType row, sizeType col)
		{
			if( row < ROW && col < COLUMN)
			{
				return elements[row][col];
			}
			else
			{
				throw std::out_of_range();
			}
		}

		const elementType& at(sizeType row, sizeType col) const
		{
			if( row < ROW && col < COLUMN)
			{
				return elements[row][col];
			}
			else
			{
				throw std::out_of_range();
			}
		}

		matType& operator *= (const elementType& _rC)
		{
			const sizeType TOTAL = ROW * COLUMN;

			for (sizeType i = 0; i < TOTAL; ++i)
			{
				element1D[i] *= _rC;
			}

			return (*this);
		}

		matType& operator/=(const elementType& _rC)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / _rC;

			return ((*this) *= inv_c);
		}

		matType& operator+=(const matType& _rMat)
		{
			const sizeType TOTAL = ROW * COLUMN;

			for (sizeType i = 0; i < TOTAL; ++i)
			{
				element1D[i] += _rMat.element1D[i];
			}

			return (*this);
		}

		matType& operator-=(const matType& _rMat)
		{
			const sizeType TOTAL = ROW*COLUMN;

			for (sizeType i = 0; i < TOTAL; ++i)
			{
				element1D[i] -= _rMat.element1D[i];
			}

			return (*this);
		}

		matType operator*(const elementType& _rValue) const
		{
			elementType elems[ROW*COLUMN];

			const sizeType TOTAL = ROW*COLUMN;

			for (sizeType i = 0; i < TOTAL; ++i)
			{
				elems[i] = element1D[i] * _rValue;
			}
			return matType(elems);
		}


		matType operator/(const elementType& _rValue) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / _rValue;

			return ((*this) * inv_c);
		}

		matType operator+(const matType& _rMat) const
		{
			elementType elems[ROW*COLUMN];

			const sizeType TOTAL = ROW*COLUMN;

			for (sizeType i = 0; i < TOTAL; ++i)
			{
				elems[i] = element1D[i] + _rMat.element1D[i];
			}

			return matType(elems);
		}

		//matType operator*(const CDyMatrix& _rMat) const
		//{
		//	elementType elems[ROW*COLUMN];


		//	return matType(elems);
		//}


		CMatrix<T, COLUMN, ROW>	transpose(void)
		{
			CMatrix<T, COLUMN, ROW> result;

			for(unsigned int row = 0 ; row < ROW ; row ++)
			{
				for(unsigned int col = 0 ; col < COLUMN ; col ++)
				{
					result.elements[col][row] = elements[row][col];
				}
			}

			return result;
		}

		matType operator-(const matType& _rMat) const
		{
			elementType elems[ROW*COLUMN];

			const sizeType TOTAL = ROW*COLUMN;
			for (sizeType i = 0; i < TOTAL; ++i)
			{
				elems[i] = element1D[i] - _rMat.element1D[i];
			}

			return matType(elems);
		}	


		template <unsigned int NEW_COLUMN>
		CMatrix<elementType, ROW, NEW_COLUMN> operator*(const CMatrix<elementType, COLUMN, NEW_COLUMN>& _rMat) const
		{
			elementType elems[ROW][NEW_COLUMN];

			for(sizeType r = 0 ; r < ROW ; r ++)
			{
				for(sizeType nc = 0 ; nc < NEW_COLUMN ; nc++)
				{
					elementType curVal	= elementType();

					for(sizeType nr = 0 ; nr < COLUMN ; ++nr)
					{
						curVal += elements[r][nr] * _rMat.elements[nr][nc];
					}

					elems[r][nc] = curVal;
				}
			}

			return CMatrix<T, ROW, NEW_COLUMN>((const elementType*)elems);
		}


		rowVecType operator*(const colVecType& _rVec) const
		{
			const elementType ZERO = elementType();

			elementType elems[ROW];

			for (sizeType r = 0 ; r < ROW ; r ++)
			{
				elems[r] = ZERO;

				for ( sizeType c = 0 ; c < COLUMN ; c ++)
				{
					elems[r] += elements[r][c] * _rVec[c];
				}
			}

			return rowVecType(elems);
		}

		friend matType operator*(const elementType& c, const matType& _rMat)
		{
			elementType elems[ROW * COLUMN ];

			const sizeType TOTAL = ROW * COLUMN;

			for (sizeType i = 0 ;i < TOTAL ; i ++)
			{
				elems[i] = _rMat.element1D[i] * c;
			}

			return matType(elems);
		}

		elementType&	operator () (const size_t& _row, const size_t& _col)
		{
			assert( _row < ROW && _col < COLUMN );
			return elements[_row][_col];
		}

		elementType	operator () (size_t _row, size_t _col) const
		{
			assert( _row < ROW && _col < COLUMN );
			return elements[_row][_col];
		}

		
		void zero(void)
		{
			const elementType zero = elementType();

			for(size_t row = 0; row < ROW ; row ++)
			{
				for(size_t col = 0; col < COLUMN ; col ++)
				{
					elements[row][col]	= zero;
				}
			}
		}

		CMatrix<T, ROW, COLUMN>	inverse(void)
		{
			assert(ROW == COLUMN);

			CMatrix<T, ROW, COLUMN> inv;

			for(size_t k_ = 0 ; k_ < ROW ; k_ ++)
			{
				int idex	= pivot(k_);

				assert( idex != -1);

				if(idex != 0)
				{// swap
					T rowPtr[COLUMN] = {T(0), };
					
					for(size_t col = 0 ; col < COLUMN ; col ++)
					{
						rowPtr[col ] = inv.elements[k_][col];
						inv.elements[k_][col]	= inv.elements[idex][col];
						inv.elements[idex][col]	= rowPtr[col ];
					}
				}

				T	a1	= elements[k_][k_];

				for(size_t j_ = 0 ; j_ < ROW ; j_ ++)
				{
					elements[k_][j_]		/= a1;
					inv.elements[k_][j_]	/= a1;
				}

				for(size_t i_ = 0 ; i_ < ROW ; i_ ++)
				{
					if( i_ != k_ )
					{
						T a2	= elements[i_][k_];

						for(size_t j_ = 0 ; j_ < ROW ; j_ ++)
						{
							elements[i_][j_]		-= a2 * elements[k_][j_];
							inv.elements[i_][j_]	-= a2 * inv.elements[k_][j_];
						}
					}
				}
			}

			return inv;
		}

		int pivot(size_t _row)
		{
			int k_ = static_cast<int>(_row);

			double amax	= -1;
			double temp	= 0.;

			for(size_t i_ = _row ; i_ < ROW ; i_ ++)
			{
				temp = abs(elements[i_][_row]);

				if(temp > amax && temp != 0.)
				{
					amax	= temp;
					k_		= i_;
				}

				if( elements[k_][_row]	== T(0))
					return -1;

				if( k_ != static_cast<int>(_row))
				{
					//T* rowPtr		= elements[k_];
					//elements[k_]	= elements[_row];
					//elements[_row]	= rowPtr;
					T rowPtr[COLUMN] = {T(0), };
					
					for(size_t col = 0 ; col < COLUMN ; col ++)
					{
						rowPtr[col ]		= elements[k_][col];
						elements[k_][col]	= elements[_row][col];
						elements[_row][col]	= rowPtr[col ];
					}

					return k_;
				}
			}
			return 0;
		}


		CMatrix<T, ROW, COLUMN> operator ! (void)
		{
			CMatrix<T, ROW, COLUMN> inv	= this->inverse();
			return inv;
		}
	};
	

	template <typename T>
	class CMatrix<T, 2, 2>
	{
	public:

		typedef typename T elementType;
		typedef unsigned int sizeType;
		typedef CMatrix<T, 2, 2> matType;
		typedef CVector<T, 2> vecType;

	public:
		union
		{
			elementType elements[2][2];
			elementType element1D[2*2];
			struct
			{
				elementType m11, m12;
				elementType m21, m22;
			};
		};

	public:
		CMatrix(void)
			: m11(static_cast<elementType>(1)), m12(elementType())
			, m21(elementType()), m22(static_cast<elementType>(1))
		{
		}

		CMatrix(
			const elementType& a11, const elementType& a12,
			const elementType& a21, const elementType& a22)
			: m11(a11), m12(a12)
			, m21(a21), m22(a22)
		{
		}

		CMatrix(const matType& _rMat)
			: m11(_rMat.m11), m12(_rMat.m12)
			, m21(_rMat.m21), m22(_rMat.m22)
		{
		}

		matType& operator=(const matType& _rMat)
		{
			if (this != &_rMat)
			{
				m11 = _rMat.m11; m12 = _rMat.m12;
				m21 = _rMat.m21; m22 = _rMat.m22;
			}
			return (*this);
		}

		elementType& at(sizeType row, sizeType column)
		{
			if (row < 2 && column < 2)
			{
				return elements[row][column];
			}
			else
			{
				assert(false);
				throw std::out_of_range();
			}
		}

		const elementType& at(sizeType row, sizeType column) const
		{
			if (row < 2 && column < 2)
			{
				return elements[row][column];
			}
			else
			{
				assert(false);
				throw std::out_of_range();
			}
		}

		matType& operator+=(const matType& _rMat)
		{
			m11 += _rMat.m11; m12 += _rMat.m12;
			m21 += _rMat.m21; m22 += _rMat.m22;
			return (*this);
		}

		matType& operator-=(const matType& _rMat)
		{
			m11 -= _rMat.m11; m12 -= _rMat.m12;
			m21 -= _rMat.m21; m22 -= _rMat.m22;
			return (*this);
		}

		matType& operator*=(const elementType& c)
		{
			m11 *= c; m12 *= c;
			m21 *= c; m22 *= c;
			return (*this);
		}

		matType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		matType operator+(const matType& _rMat) const
		{
			return matType(
				m11+_rMat.m11, m12+_rMat.m12,
				m21+_rMat.m21, m22+_rMat.m22);
		}

		matType operator-(const matType& _rMat) const
		{
			return matType(
				m11-_rMat.m11, m12-_rMat.m12,
				m21-_rMat.m21, m22-_rMat.m22);
		}

		matType operator*(const elementType& c) const
		{
			return matType(
				m11*c, m12*c,
				m21*c, m22*c);
		}

		matType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		matType operator+(void) const
		{
			return matType(
				+m11, +m12,
				+m21, +m22);
		}

		matType operator-(void) const
		{
			return matType(
				-m11, -m12,
				-m21, -m22);
		}

		matType getTransposeMatrix(void) const
		{
			return matType(
				m11, m21,
				m12, m22);
		}

		void transpose(void)
		{
			std::swap(m12, m21);
		}

		elementType GetDeterminant(void) const
		{
			// a b
			// c d
			// ad - bc

			// m11 m12
			// m21 m22
			// m11*m22 - m12*m21
			return (m11*m22 - m12*m21);
		}

		bool IsInvertible(void) const
		{
			return (abs(GetDeterminant()) > EPSILON);
		}

		void Identity(void)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType zero = elementType();

			m11 =  one; m12 = zero;
			m21 = zero; m22 =  one;
		}

		void Inverse(void)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType InvDeterminant = one / GetDeterminant();

			const elementType a11 =  m22*InvDeterminant;
			const elementType a12 = -m12*InvDeterminant;
			const elementType a21 = -m21*InvDeterminant;
			const elementType a22 =  m11*InvDeterminant;

			m11 = a11; m12 = a12;
			m21 = a21; m22 = a22;
		}

		matType getInverseMatrix(void) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType InvDeterminant = one / GetDeterminant(void);

			return matType(
				 m22*InvDeterminant, -m12*InvDeterminant,
				-m21*InvDeterminant,  m11*InvDeterminant);
		}

		matType& operator*=(const matType& _rMat)
		{
			const elementType a11 = m11*_rMat.m11 + m12*_rMat.m21;
			const elementType a12 = m11*_rMat.m12 + m12*_rMat.m22;
			const elementType a21 = m21*_rMat.m11 + m22*_rMat.m21;
			const elementType a22 = m21*_rMat.m12 + m22*_rMat.m22;

			m11 = a11; m12 = a12;
			m21 = a21; m22 = a22;

			return (*this);
		}

		matType operator*(const matType& _rMat) const
		{
			return matType(
				m11*_rMat.m11 + m12*_rMat.m21, m11*_rMat.m12 + m12*_rMat.m22,
				m21*_rMat.m11 + m22*_rMat.m21, m21*_rMat.m12 + m22*_rMat.m22);
		}

		vecType operator*(const vecType& _rMat) const
		{
			return vecType(
				m11*_rMat.x + m12*_rMat.y,
				m21*_rMat.x + m22*_rMat.y);
		}

		friend matType operator*(const elementType& c, const matType& _rMat)
		{
			return matType(
				c*_rMat.m11, c*_rMat.m12,
				c*_rMat.m21, c*_rMat.m22);
		}

		void Rotation(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 = c_value; m12 = -s_value;
			m21 = s_value; m22 =  c_value;
		}

		void UniformScaling(const elementType& scale)
		{
			const elementType zero = elementType();

			m11 = scale; m12 =  zero;
			m21 =  zero; m22 = scale;
		}

		void Scaling(
			const elementType& sx,
			const elementType& sy)
		{
			const elementType zero = elementType();

			m11 =   sx; m12 = zero;
			m21 = zero; m22 =   sy;
		}

		void Scaling(const vecType& vScale)
		{
			return this->Scaling(vScale.x, vScale.y);
		}
	};

	template <typename T>
	class CMatrix<T, 3, 3>
	{
	public:
		typedef typename T elementType;
		typedef unsigned int sizeType;
		typedef CMatrix<T, 3, 3> matType;
		typedef CVector<T, 2> vector2_type;
		typedef CVector<T, 3> vector3_type;

	public:
		union
		{
			elementType elements[3][3];
			elementType element1D[3*3];
			struct
			{
				elementType m11, m12, m13;
				elementType m21, m22, m23;
				elementType m31, m32, m33;
			};
		};

	public:
		CMatrix(void)
			: m11(static_cast<elementType>(1)), m12(elementType()), m13(elementType())
			, m21(elementType()), m22(static_cast<elementType>(1)), m23(elementType())
			, m31(elementType()), m32(elementType()), m33(static_cast<elementType>(1))
		{
		}

		CMatrix(
			const elementType& a11, const elementType& a12, const elementType& a13,
			const elementType& a21, const elementType& a22, const elementType& a23,
			const elementType& a31, const elementType& a32, const elementType& a33)
			: m11(a11), m12(a12), m13(a13)
			, m21(a21), m22(a22), m23(a23)
			, m31(a31), m32(a32), m33(a33)
		{
		}

		CMatrix(const matType& _rMat)
			: m11(_rMat.m11), m12(_rMat.m12), m13(_rMat.m13)
			, m21(_rMat.m21), m22(_rMat.m22), m23(_rMat.m23)
			, m31(_rMat.m31), m32(_rMat.m32), m33(_rMat.m33)
		{
		}

		matType& operator=(const matType& _rMat)
		{
			if (this != &_rMat)
			{
				m11 = _rMat.m11; m12 = _rMat.m12; m13 = _rMat.m13;
				m21 = _rMat.m21; m22 = _rMat.m22; m23 = _rMat.m23;
				m31 = _rMat.m31; m32 = _rMat.m32; m33 = _rMat.m33;
			}
			return (*this);
		}

		elementType& at(sizeType row, sizeType column)
		{
			if (row < 3 && column < 3)
			{
				return elements[row][column];
			}
			else
			{
				assert(false);
				throw std::out_of_range();
			}
		}

		const elementType& at(sizeType row, sizeType column) const
		{
			if (row < 3 && column < 3)
			{
				return elements[row][column];
			}
			else
			{
				assert(false);
				throw std::out_of_range();
			}
		}

		matType& operator+=(const matType& _rMat)
		{
			m11 += _rMat.m11; m12 += _rMat.m12; m13 += _rMat.m13;
			m21 += _rMat.m21; m22 += _rMat.m22; m23 += _rMat.m23;
			m31 += _rMat.m31; m32 += _rMat.m32; m33 += _rMat.m33;
			return (*this);
		}

		matType& operator-=(const matType& _rMat)
		{
			m11 -= _rMat.m11; m12 -= _rMat.m12; m13 -= _rMat.m13;
			m21 -= _rMat.m21; m22 -= _rMat.m22; m23 -= _rMat.m23;
			m31 -= _rMat.m31; m32 -= _rMat.m32; m33 -= _rMat.m33;
			return (*this);
		}

		matType& operator*=(const elementType& c)
		{
			m11 *= c; m12 *= c; m13 *= c;
			m21 *= c; m22 *= c; m23 *= c;
			m31 *= c; m32 *= c; m33 *= c;
			return (*this);
		}

		matType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		matType operator+(const matType& _rMat) const
		{
			return matType(
				m11+_rMat.m11, m12+_rMat.m12, m13+_rMat.m13,
				m21+_rMat.m21, m22+_rMat.m22, m23+_rMat.m23,
				m31+_rMat.m31, m32+_rMat.m32, m33+_rMat.m33);
		}

		matType operator-(const matType& _rMat) const
		{
			return matType(
				m11-_rMat.m11, m12-_rMat.m12, m13-_rMat.m13,
				m21-_rMat.m21, m22-_rMat.m22, m23-_rMat.m23,
				m31-_rMat.m31, m32-_rMat.m32, m33-_rMat.m33);
		}

		matType operator*(const elementType& c) const
		{
			return matType(
				m11*c, m12*c, m13*c,
				m21*c, m22*c, m23*c,
				m31*c, m32*c, m33*c);
		}

		matType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		matType operator+(void) const
		{
			return matType(
				+m11, +m12, +m13,
				+m21, +m22, +m23,
				+m31, +m32, +m33);
		}

		matType operator-(void) const
		{
			return matType(
				-m11, -m12, -m13,
				-m21, -m22, -m23,
				-m31, -m32, -m33);
		}

		matType getTransposeMatrix(void) const
		{
			return matType(
				m11, m21, m31,
				m12, m22, m32,
				m13, m23, m33);
		}

		void transpose(void)
		{
			std::swap(m12, m21);
			std::swap(m13, m31);
			std::swap(m23, m32);
		}

		elementType getDeterminant(void) const
		{
			//detA = 
			// a11a22a33 +
			// a21a32a13 +
			// a31a12a23 -
			// a11a32a23 -
			// a31a22a13 -
			// a21a12a33

			// a11(a33a22-a32a23)-a21(a33a12-a32a13)+a31(a23a12-a22a13)
			return
				(m11*m22*m33) +
				(m21*m32*m13) +
				(m31*m12*m23) -
				(m11*m32*m23) -
				(m31*m22*m13) -
				(m21*m12*m33);
		}

		bool IsInvertible(void) const
		{
			return (abs(GetDeterminant()) > EPSILON);
		}

		void Identity()
		{
			const elementType one = static_cast<elementType>(1);
			const elementType zero = elementType();

			m11 =  one; m12 = zero; m13 = zero;
			m21 = zero; m22 =  one; m23 = zero;
			m31 = zero; m32 = zero; m33 =  one;
		}

		void Inverse()
		{
			const elementType one = static_cast<elementType>(1);
			const elementType InvDeterminant = one / GetDeterminant();

			// a33a22-a32a23
			const elementType a11 = ((m22*m33) - (m23*m32)) * InvDeterminant;
			//-(a33a12-a32a13)
			const elementType a12 = ((m13*m32) - (m12*m33)) * InvDeterminant;
			// a23a12-a22a13
			const elementType a13 = ((m12*m23) - (m13*m22)) * InvDeterminant;

			//-(a33a21-a31a23)
			const elementType a21 = ((m23*m31) - (m21*m33)) * InvDeterminant;
			// a33a11-a31a13
			const elementType a22 = ((m11*m33) - (m13*m31)) * InvDeterminant;
			//-(a23a11-a21a13)
			const elementType a23 = ((m13*m21) - (m11*m23)) * InvDeterminant;

			// a32a21-a31a22
			const elementType a31 = ((m21*m32) - (m22*m31)) * InvDeterminant;
			//-(a32a11-a31a12)
			const elementType a32 = ((m12*m31) - (m11*m32)) * InvDeterminant;
			// a22a11-a21a12
			const elementType a33 = ((m11*m22) - (m12*m21)) * InvDeterminant;

			m11 = a11; m12 = a12; m13 = a13;
			m21 = a21; m22 = a22; m23 = a23;
			m31 = a31; m32 = a32; m33 = a33;
		}

		matType getInverseMatrix(void) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType InvDeterminant = one / GetDeterminant();

			return matType(
				((m22*m33) - (m23*m32)) * InvDeterminant,
				((m13*m32) - (m12*m33)) * InvDeterminant,
				((m12*m23) - (m13*m22)) * InvDeterminant,

				((m23*m31) - (m21*m33)) * InvDeterminant,
				((m11*m33) - (m13*m31)) * InvDeterminant,
				((m13*m21) - (m11*m23)) * InvDeterminant,
				
				((m21*m32) - (m22*m31)) * InvDeterminant,
				((m12*m31) - (m11*m32)) * InvDeterminant,
				((m11*m22) - (m12*m21)) * InvDeterminant);
		}

		matType& operator*=(const matType& _rMat)
		{
			elementType a11 = m11*_rMat.m11 + m12*_rMat.m21 + m13*_rMat.m31;
			elementType a12 = m11*_rMat.m12 + m12*_rMat.m22 + m13*_rMat.m32;
			elementType a13 = m11*_rMat.m13 + m12*_rMat.m23 + m13*_rMat.m33;

			elementType a21 = m21*_rMat.m11 + m22*_rMat.m21 + m23*_rMat.m31;
			elementType a22 = m21*_rMat.m12 + m22*_rMat.m22 + m23*_rMat.m32;
			elementType a23 = m21*_rMat.m13 + m22*_rMat.m23 + m23*_rMat.m33;

			elementType a31 = m31*_rMat.m11 + m32*_rMat.m21 + m33*_rMat.m31;
			elementType a32 = m31*_rMat.m12 + m32*_rMat.m22 + m33*_rMat.m32;
			elementType a33 = m31*_rMat.m13 + m32*_rMat.m23 + m33*_rMat.m33;

			m11 = a11; m12 = a12; m13 = a13;
			m21 = a21; m22 = a22; m23 = a23;
			m31 = a31; m32 = a32; m33 = a33;

			return (*this);
		}

		matType operator*(const matType& _rMat) const
		{
			return matType(
				m11*_rMat.m11 + m12*_rMat.m21 + m13*_rMat.m31,
				m11*_rMat.m12 + m12*_rMat.m22 + m13*_rMat.m32,
				m11*_rMat.m13 + m12*_rMat.m23 + m13*_rMat.m33,
				m21*_rMat.m11 + m22*_rMat.m21 + m23*_rMat.m31,
				m21*_rMat.m12 + m22*_rMat.m22 + m23*_rMat.m32,
				m21*_rMat.m13 + m22*_rMat.m23 + m23*_rMat.m33,
				m31*_rMat.m11 + m32*_rMat.m21 + m33*_rMat.m31,
				m31*_rMat.m12 + m32*_rMat.m22 + m33*_rMat.m32,
				m31*_rMat.m13 + m32*_rMat.m23 + m33*_rMat.m33);
		}

		vector3_type operator*(const vector3_type& _rMat) const
		{
			return vector3_type(
				m11*_rMat.x + m12*_rMat.y + m13*_rMat.z,
				m21*_rMat.x + m22*_rMat.y + m23*_rMat.z,
				m31*_rMat.x + m32*_rMat.y + m33*_rMat.z);
		}

		vector2_type operator*(const vector2_type& _rMat) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_z = one / (m31*_rMat.x + m32*_rMat.y + m33);

			return vector2_type(
				(m11*_rMat.x + m12*_rMat.y + m13) * inv_z,
				(m21*_rMat.x + m22*_rMat.y + m23) * inv_z);
		}

		friend matType operator*(const elementType& c, const matType& _rMat)
		{
			return matType(
				c*_rMat.m11, c*_rMat.m12, c*_rMat.m13,
				c*_rMat.m21, c*_rMat.m22, c*_rMat.m23,
				c*_rMat.m31, c*_rMat.m32, c*_rMat.m33);
		}

		void RotationAxisX(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 = o_value; m12 = z_value; m13 =  z_value;
			m21 = z_value; m22 = c_value; m23 = -s_value;
			m31 = z_value; m32 = s_value; m33 =  c_value;
		}

		void RotationAxisY(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 =  c_value; m12 = z_value; m13 = s_value;
			m21 =  z_value; m22 = o_value; m23 = z_value;
			m31 = -s_value; m32 = z_value; m33 = c_value;
		}

		void RotationAxisZ(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 = c_value; m12 = -s_value; m13 = z_value;
			m21 = s_value; m22 =  c_value; m23 = z_value;
			m31 = z_value; m32 =  z_value; m33 = o_value;
		}

		void RotationAxis(
			const elementType& ax,
			const elementType& ay,
			const elementType& az,
			const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);
			const elementType t_value = o_value - c_value;

			m11 = c_value + t_value*ax*ax;
			m12 = t_value*ax*ay - s_value*az;
			m13 = t_value*ax*az + s_value*ay;

			m21 = t_value*ax*ay + s_value*az;
			m22 = c_value + t_value*ay*ay;
			m23 = t_value*ay*az - s_value*ax;

			m31 = t_value*ax*az - s_value*ay;
			m32 = t_value*ay*az + s_value*ax;
			m33 = c_value + t_value*az*az;
		}

		void RotationAxis(
			const vector3_type& vAxis,
			const elementType& angle)
		{
			return this->RotationAxis(vAxis.x, vAxis.y, vAxis.z, angle);
		}

		void UniformScaling2x2(const elementType& scale)
		{
			const elementType zero = elementType();
			const elementType one = static_cast<elementType>(1);

			m11 = scale; m12 =  zero; m13 = zero;
			m21 =  zero; m22 = scale; m23 = zero;
			m31 =  zero; m32 =  zero; m33 =  one;
		}

		void UniformScaling3x3(const elementType& scale)
		{
			const elementType zero = elementType();

			m11 = scale; m12 =  zero; m13 =  zero;
			m21 =  zero; m22 = scale; m23 =  zero;
			m31 =  zero; m32 =  zero; m33 = scale;
		}

		void Scaling(
			const elementType& sx,
			const elementType& sy)
		{
			const elementType zero = elementType();
			const elementType one = static_cast<elementType>(1);

			m11 =   sx; m12 = zero; m13 = zero;
			m21 = zero; m22 =   sy; m23 = zero;
			m31 = zero; m32 = zero; m33 =  one;
		}

		void Scaling(const vector2_type& vScale)
		{
			return this->Scaling(vScale.x, vScale.y);
		}

		void Scaling(
			const elementType& sx,
			const elementType& sy,
			const elementType& sz)
		{
			const elementType zero = elementType();

			m11 =   sx; m12 = zero; m13 = zero;
			m21 = zero; m22 =   sy; m23 = zero;
			m31 = zero; m32 = zero; m33 =   sz;
		}

		void Scaling(const vector3_type& vScale)
		{
			return this->Scaling(vScale.x, vScale.y, vScale.z);
		}

		void Translation(
			const elementType& tx,
			const elementType& ty)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType zero = elementType();

			m11 =  one; m12 = zero; m13 =  tx;
			m21 = zero; m22 =  one; m23 =  ty;
			m31 = zero; m32 = zero; m33 = one;
		}

		void Translation(const vector2_type& vTrans)
		{
			return this->Translation(vTrans.x, vTrans.y);
		}
	};

	template <typename T>
	class CMatrix<T, 4, 4>
	{
	public:
		typedef typename T elementType;
		typedef unsigned int sizeType;
		typedef CMatrix<T, 4, 4> matType;
		typedef CVector<T, 3> vector3_type;
		typedef CVector<T, 4> vector4_type;

	public:
		union
		{
			elementType elements[4][4];
			elementType element1D[4*4];
			struct
			{
				elementType m11, m12, m13, m14;
				elementType m21, m22, m23, m24;
				elementType m31, m32, m33, m34;
				elementType m41, m42, m43, m44;
			};
		};

	public:
		CMatrix(void)
			: m11(static_cast<elementType>(1)), m12(elementType()), m13(elementType()), m14(elementType())
			, m21(elementType()), m22(static_cast<elementType>(1)), m23(elementType()), m24(elementType())
			, m31(elementType()), m32(elementType()), m33(static_cast<elementType>(1)), m34(elementType())
			, m41(elementType()), m42(elementType()), m43(elementType()), m44(static_cast<elementType>(1))
		{
		}

		CMatrix(
			const elementType& a11, const elementType& a12, const elementType& a13, const elementType& a14,
			const elementType& a21, const elementType& a22, const elementType& a23, const elementType& a24,
			const elementType& a31, const elementType& a32, const elementType& a33, const elementType& a34,
			const elementType& a41, const elementType& a42, const elementType& a43, const elementType& a44)
			: m11(a11), m12(a12), m13(a13), m14(a14)
			, m21(a21), m22(a22), m23(a23), m24(a24)
			, m31(a31), m32(a32), m33(a33), m34(a34)
			, m41(a41), m42(a42), m43(a43), m44(a44)
		{
		}

		CMatrix(const matType& _rMat)
			: m11(_rMat.m11), m12(_rMat.m12), m13(_rMat.m13), m14(_rMat.m14)
			, m21(_rMat.m21), m22(_rMat.m22), m23(_rMat.m23), m24(_rMat.m24)
			, m31(_rMat.m31), m32(_rMat.m32), m33(_rMat.m33), m34(_rMat.m34)
			, m41(_rMat.m41), m42(_rMat.m42), m43(_rMat.m43), m44(_rMat.m44)
		{
		}

		matType& operator=(const matType& _rMat)
		{
			if (this != &_rMat)
			{
				m11 = _rMat.m11; m12 = _rMat.m12; m13 = _rMat.m13; m14 = _rMat.m14;
				m21 = _rMat.m21; m22 = _rMat.m22; m23 = _rMat.m23; m24 = _rMat.m24;
				m31 = _rMat.m31; m32 = _rMat.m32; m33 = _rMat.m33; m34 = _rMat.m34;
				m41 = _rMat.m41; m42 = _rMat.m42; m43 = _rMat.m43; m44 = _rMat.m44;
			}
			return (*this);
		}

		elementType& at(sizeType row, sizeType column)
		{
			if (row < 4 && column < 4)
			{
				return elements[row][column];
			}
			else
			{
				assert(false);
				throw std::out_of_range("Matrix<T, 4, 4>::at");
			}
		}

		const elementType& at(sizeType row, sizeType column) const
		{
			if (row < 4 && column < 4)
			{
				return elements[row][column];
			}
			else
			{
				assert(false);
				throw std::out_of_range("Matrix<T, 4, 4>::at const");
			}
		}

		matType& operator+=(const matType& _rMat)
		{
			m11 += _rMat.m11; m12 += _rMat.m12; m13 += _rMat.m13; m14 += _rMat.m14;
			m21 += _rMat.m21; m22 += _rMat.m22; m23 += _rMat.m23; m24 += _rMat.m24;
			m31 += _rMat.m31; m32 += _rMat.m32; m33 += _rMat.m33; m34 += _rMat.m34;
			m41 += _rMat.m41; m42 += _rMat.m42; m43 += _rMat.m43; m44 += _rMat.m44;
			return (*this);
		}

		matType& operator-=(const matType& _rMat)
		{
			m11 -= _rMat.m11; m12 -= _rMat.m12; m13 -= _rMat.m13; m14 -= _rMat.m14;
			m21 -= _rMat.m21; m22 -= _rMat.m22; m23 -= _rMat.m23; m24 -= _rMat.m24;
			m31 -= _rMat.m31; m32 -= _rMat.m32; m33 -= _rMat.m33; m34 -= _rMat.m34;
			m41 -= _rMat.m41; m42 -= _rMat.m42; m43 -= _rMat.m43; m44 -= _rMat.m44;
			return (*this);
		}

		matType& operator*=(const elementType& c)
		{
			m11 *= c; m12 *= c; m13 *= c; m14 *= c;
			m21 *= c; m22 *= c; m23 *= c; m24 *= c;
			m31 *= c; m32 *= c; m33 *= c; m34 *= c;
			m41 *= c; m42 *= c; m43 *= c; m44 *= c;
			return (*this);
		}

		matType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		matType operator+(const matType& _rMat) const
		{
			return matType(
				m11+_rMat.m11, m12+_rMat.m12, m13+_rMat.m13, m14+_rMat.m14,
				m21+_rMat.m21, m22+_rMat.m22, m23+_rMat.m23, m24+_rMat.m24,
				m31+_rMat.m31, m32+_rMat.m32, m33+_rMat.m33, m34+_rMat.m34,
				m41+_rMat.m41, m42+_rMat.m42, m43+_rMat.m43, m44+_rMat.m44);
		}

		matType operator-(const matType& _rMat) const
		{
			return matType(
				m11-_rMat.m11, m12-_rMat.m12, m13-_rMat.m13, m14-_rMat.m14,
				m21-_rMat.m21, m22-_rMat.m22, m23-_rMat.m23, m24-_rMat.m24
				m31-_rMat.m31, m32-_rMat.m32, m33-_rMat.m33, m34-_rMat.m34,
				m41-_rMat.m41, m42-_rMat.m42, m43-_rMat.m43, m44-_rMat.m44);
		}

		matType operator*(const elementType& c) const
		{
			return matType(
				m11*c, m12*c, m13*c, m14*c,
				m21*c, m22*c, m23*c, m24*c,
				m31*c, m32*c, m33*c, m34*c,
				m41*c, m42*c, m43*c, m44*c);
		}

		matType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		matType operator-(void) const
		{
			return matType(
				-m11, -m12, -m13, -m14,
				-m21, -m22, -m23, -m24,
				-m31, -m32, -m33, -m34,
				-m41, -m42, -m43, -m44);
		}

		matType operator+(void) const
		{
			return matType(
				+m11, +m12, +m13, +m14,
				+m21, +m22, +m23, +m24,
				+m31, +m32, +m33, +m34,
				+m41, +m42, +m43, +m44);
		}

		void transpose(void)
		{
			std::swap(m12, m21);
			std::swap(m13, m31);
			std::swap(m14, m41);
			std::swap(m23, m32);
			std::swap(m24, m42);
			std::swap(m34, m43);
		}

		matType getTransposeMatrix(void) const
		{
			return matType(
				m11, m21, m31, m41,
				m12, m22, m32, m42,
				m13, m23, m33, m43,
				m14, m24, m34, m44);
		}

		void identity(void)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType zero = elementType();

			m11 =  one; m12 = zero; m13 = zero; m14 = zero;
			m21 = zero; m22 =  one; m23 = zero; m24 = zero;
			m31 = zero; m32 = zero; m33 =  one; m34 = zero;
			m41 = zero; m42 = zero; m43 = zero; m44 =  one;
		}

		elementType GetDeterminant(void) const
		{
			return
				(m11*m22*m33*m44) + (m11*m23*m34*m42) + (m11*m24*m32*m43) +
				(m12*m21*m34*m43) + (m12*m23*m31*m44) + (m12*m24*m33*m41) +
				(m13*m21*m32*m44) + (m13*m22*m34*m41) + (m13*m24*m31*m42) +
				(m14*m21*m33*m42) + (m14*m22*m31*m43) + (m14*m23*m32*m41) -
				(m11*m22*m34*m43) - (m11*m23*m32*m44) - (m11*m24*m33*m42) -
				(m12*m21*m33*m44) - (m12*m23*m34*m41) - (m12*m24*m31*m43) -
				(m13*m21*m34*m42) - (m13*m22*m31*m44) - (m13*m24*m32*m41) -
				(m14*m21*m32*m43) - (m14*m22*m33*m41) - (m14*m23*m31*m42);
		}

		bool IsInvertible(void) const
		{
			return (abs(GetDeterminant()) > EPSILON);
		}

		void Inverse()
		{
			const elementType InvDeterminant = static_cast<elementType>(1) / GetDeterminant();

			const elementType a11 = m22*m33*m44 + m23*m34*m42 + m24*m32*m43 - m22*m34*m43 - m23*m32*m44 - m24*m33*m42;
			const elementType a12 = m12*m34*m43 + m13*m32*m44 + m14*m33*m42 - m12*m33*m44 - m13*m34*m42 - m14*m32*m43;
			const elementType a13 = m12*m23*m44 + m13*m24*m42 + m14*m22*m43 - m12*m24*m43 - m13*m22*m44 - m14*m23*m42;
			const elementType a14 = m12*m24*m33 + m13*m22*m34 + m14*m23*m32 - m12*m23*m34 - m13*m24*m32 - m14*m22*m33;

			const elementType a21 = m21*m34*m43 + m23*m31*m44 + m24*m33*m41 - m21*m33*m44 - m23*m34*m41 - m24*m31*m43;
			const elementType a22 = m11*m33*m44 + m13*m34*m41 + m14*m31*m43 - m11*m34*m43 - m13*m31*m44 - m14*m33*m41;
			const elementType a23 = m11*m24*m43 + m13*m21*m44 + m14*m23*m41 - m11*m23*m44 - m13*m24*m41 - m14*m21*m43;
			const elementType a24 = m11*m23*m34 + m13*m24*m31 + m14*m21*m33 - m11*m24*m33 - m13*m21*m34 - m14*m23*m31;

			const elementType a31 = m21*m32*m44 + m22*m34*m41 + m24*m31*m42 - m21*m34*m42 - m22*m31*m44 - m24*m32*m41;
			const elementType a32 = m11*m34*m42 + m12*m31*m44 + m14*m32*m41 - m11*m32*m44 - m12*m34*m41 - m14*m31*m42;
			const elementType a33 = m11*m22*m44 + m12*m24*m41 + m14*m21*m42 - m11*m24*m42 - m12*m21*m44 - m14*m22*m41;
			const elementType a34 = m11*m24*m32 + m12*m21*m34 + m14*m22*m31 - m11*m22*m34 - m12*m24*m31 - m14*m21*m32;

			const elementType a41 = m21*m33*m42 + m22*m31*m43 + m23*m32*m41 - m21*m32*m43 - m22*m33*m41 - m23*m31*m42;
			const elementType a42 = m11*m32*m43 + m12*m33*m41 + m13*m31*m42 - m11*m33*m42 - m12*m31*m43 - m13*m32*m41;
			const elementType a43 = m11*m23*m42 + m12*m21*m43 + m13*m22*m41 - m11*m22*m43 - m12*m23*m41 - m13*m21*m42;
			const elementType a44 = m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 - m13*m22*m31;

			m11 = a11*InvDeterminant; m12 = a12*InvDeterminant; m13 = a13*InvDeterminant; m14 = a14*InvDeterminant;
			m21 = a21*InvDeterminant; m22 = a22*InvDeterminant; m23 = a23*InvDeterminant; m24 = a24*InvDeterminant;
			m31 = a31*InvDeterminant; m32 = a32*InvDeterminant; m33 = a33*InvDeterminant; m34 = a34*InvDeterminant;
			m41 = a41*InvDeterminant; m42 = a42*InvDeterminant; m43 = a43*InvDeterminant; m44 = a44*InvDeterminant;
		}

		matType getInverseMatrix(void) const
		{
			const elementType InvDeterminant = static_cast<elementType>(1) / GetDeterminant();

			return matType(
				(m22*m33*m44 + m23*m34*m42 + m24*m32*m43 - m22*m34*m43 - m23*m32*m44 - m24*m33*m42) * InvDeterminant,
				(m12*m34*m43 + m13*m32*m44 + m14*m33*m42 - m12*m33*m44 - m13*m34*m42 - m14*m32*m43) * InvDeterminant,
				(m12*m23*m44 + m13*m24*m42 + m14*m22*m43 - m12*m24*m43 - m13*m22*m44 - m14*m23*m42) * InvDeterminant,
				(m12*m24*m33 + m13*m22*m34 + m14*m23*m32 - m12*m23*m34 - m13*m24*m32 - m14*m22*m33) * InvDeterminant,

				(m21*m34*m43 + m23*m31*m44 + m24*m33*m41 - m21*m33*m44 - m23*m34*m41 - m24*m31*m43) * InvDeterminant,
				(m11*m33*m44 + m13*m34*m41 + m14*m31*m43 - m11*m34*m43 - m13*m31*m44 - m14*m33*m41) * InvDeterminant,
				(m11*m24*m43 + m13*m21*m44 + m14*m23*m41 - m11*m23*m44 - m13*m24*m41 - m14*m21*m43) * InvDeterminant,
				(m11*m23*m34 + m13*m24*m31 + m14*m21*m33 - m11*m24*m33 - m13*m21*m34 - m14*m23*m31) * InvDeterminant,
				
				(m21*m32*m44 + m22*m34*m41 + m24*m31*m42 - m21*m34*m42 - m22*m31*m44 - m24*m32*m41) * InvDeterminant,
				(m11*m34*m42 + m12*m31*m44 + m14*m32*m41 - m11*m32*m44 - m12*m34*m41 - m14*m31*m42) * InvDeterminant,
				(m11*m22*m44 + m12*m24*m41 + m14*m21*m42 - m11*m24*m42 - m12*m21*m44 - m14*m22*m41) * InvDeterminant,
				(m11*m24*m32 + m12*m21*m34 + m14*m22*m31 - m11*m22*m34 - m12*m24*m31 - m14*m21*m32) * InvDeterminant,
				
				(m21*m33*m42 + m22*m31*m43 + m23*m32*m41 - m21*m32*m43 - m22*m33*m41 - m23*m31*m42) * InvDeterminant,
				(m11*m32*m43 + m12*m33*m41 + m13*m31*m42 - m11*m33*m42 - m12*m31*m43 - m13*m32*m41) * InvDeterminant,
				(m11*m23*m42 + m12*m21*m43 + m13*m22*m41 - m11*m22*m43 - m12*m23*m41 - m13*m21*m42) * InvDeterminant,
				(m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 - m13*m22*m31) * InvDeterminant);
		}

		matType& operator*=(const matType& _rMat)
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

		matType operator*(const matType& _rMat) const
		{
			return matType(
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

		vector3_type operator*(const vector3_type& _rMat) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_w = one / (m41*_rMat.x + m42*_rMat.y + m43*_rMat.z + m44);
			return vector3_type(
				(m11*_rMat.x + m12*_rMat.y + m13*_rMat.z + m14) * inv_w,
				(m21*_rMat.x + m22*_rMat.y + m23*_rMat.z + m24) * inv_w,
				(m31*_rMat.x + m32*_rMat.y + m33*_rMat.z + m34) * inv_w);
		}

		vector4_type operator*(const vector4_type& _rMat) const
		{
			return vector4_type(
				m11*_rMat.x + m12*_rMat.y + m13*_rMat.z + m14*_rMat.w,
				m21*_rMat.x + m22*_rMat.y + m23*_rMat.z + m24*_rMat.w,
				m31*_rMat.x + m32*_rMat.y + m33*_rMat.z + m34*_rMat.w,
				m41*_rMat.x + m42*_rMat.y + m43*_rMat.z + m44*_rMat.w);
		}

		friend matType operator*(const elementType& c, const matType& _rMat)
		{
			return matType(
				c*_rMat.m11, c*_rMat.m12, c*_rMat.m13,
				c*_rMat.m21, c*_rMat.m22, c*_rMat.m23,
				c*_rMat.m31, c*_rMat.m32, c*_rMat.m33);
		}

		void RotationAxisX(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 = o_value; m12 = z_value; m13 =  z_value; m14 = z_value;
			m21 = z_value; m22 = c_value; m23 = -s_value; m24 = z_value;
			m31 = z_value; m32 = s_value; m33 =  c_value; m34 = z_value;
			m41 = z_value; m42 = z_value; m43 =  z_value; m44 = o_value;
		}

		void RotationAxisY(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 =  c_value; m12 = z_value; m13 = s_value; m14 = z_value;
			m21 =  z_value; m22 = o_value; m23 = z_value; m24 = z_value;
			m31 = -s_value; m32 = z_value; m33 = c_value; m34 = z_value;
			m41 =  z_value; m42 = z_value; m43 = z_value; m44 = o_value;
		}

		void RotationAxisZ(const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);

			m11 = c_value; m12 = -s_value; m13 = z_value; m14 = z_value;
			m21 = s_value; m22 =  c_value; m23 = z_value; m24 = z_value;
			m31 = z_value; m32 =  z_value; m33 = o_value; m34 = z_value;
			m41 = z_value; m42 =  z_value; m43 = z_value; m44 = o_value;
		}

		void RotationAxis(
			const elementType& ax,
			const elementType& ay,
			const elementType& az,
			const elementType& angle)
		{
			const elementType c_value = static_cast<elementType>(cos(angle));
			const elementType s_value = static_cast<elementType>(sin(angle));
			const elementType z_value = elementType();
			const elementType o_value = static_cast<elementType>(1);
			const elementType t_value = o_value - c_value;

			m11 = c_value + t_value*ax*ax;
			m12 = t_value*ax*ay - s_value*az;
			m13 = t_value*ax*az + s_value*ay;
			m14 = z_value;

			m21 = t_value*ax*ay + s_value*az;
			m22 = c_value + t_value*ay*ay;
			m23 = t_value*ay*az - s_value*ax;
			m24 = z_value;

			m31 = t_value*ax*az - s_value*ay;
			m32 = t_value*ay*az + s_value*ax;
			m33 = c_value + t_value*az*az;
			m34 = z_value;

			m41 = z_value;
			m42 = z_value;
			m43 = z_value;
			m44 = o_value;
		}

		void RotationAxis(
			const vector3_type& vAxis,
			const elementType& angle)
		{
			return this->RotationAxis(vAxis.x, vAxis.y, vAxis.z, angle);
		}

		void UniformScaling3x3(const elementType& scale)
		{
			const elementType zero = elementType();
			const elementType one = static_cast<elementType>(1);

			m11 = scale; m12 =  zero; m13 =  zero; m14 = zero;
			m21 =  zero; m22 = scale; m23 =  zero; m24 = zero;
			m31 =  zero; m32 =  zero; m33 = scale; m34 = zero;
			m41 =  zero; m42 =  zero; m43 =  zero; m44 =  one;
		}

		void UniformScaling4x4(const elementType& scale)
		{
			const elementType zero = elementType();
			const elementType one = static_cast<elementType>(1);

			m11 = scale; m12 =  zero; m13 =  zero; m14 =  zero;
			m21 =  zero; m22 = scale; m23 =  zero; m24 =  zero;
			m31 =  zero; m32 =  zero; m33 = scale; m34 =  zero;
			m41 =  zero; m42 =  zero; m43 =  zero; m44 = scale;
		}

		void Scaling(
			const elementType& sx,
			const elementType& sy,
			const elementType& sz)
		{
			const elementType zero = elementType();
			const elementType one = static_cast<elementType>(1);

			m11 =   sx; m12 = zero; m13 = zero; m14 = zero;
			m21 = zero; m22 =   sy; m23 = zero; m24 = zero;
			m31 = zero; m32 = zero; m33 =   sz; m34 = zero;
			m41 = zero; m42 = zero; m43 = zero; m44 =  one;
		}

		void Scaling(const vector3_type& vScale)
		{
			return this->Scaling(vScale.x, vScale.y, vScale.z);
		}

		void Scaling(
			const elementType& sx,
			const elementType& sy,
			const elementType& sz,
			const elementType& sw)
		{
			const elementType zero = elementType();
			const elementType one = static_cast<elementType>(1);

			m11 =   sx; m12 = zero; m13 = zero; m14 = zero;
			m21 = zero; m22 =   sy; m23 = zero; m24 = zero;
			m31 = zero; m32 = zero; m33 =   sz; m34 = zero;
			m41 = zero; m42 = zero; m43 = zero; m44 =   sw;
		}

		void Scaling(const vector4_type& vScale)
		{
			return this->Scaling(vScale.x, vScale.y, vScale.z, vScale.w);
		}

		void Translation(
			const elementType& tx,
			const elementType& ty,
			const elementType& tz)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType zero = elementType();

			m11 =  one; m12 = zero; m13 = zero; m14 =  tx;
			m21 = zero; m22 =  one; m23 = zero; m24 =  ty;
			m31 = zero; m32 = zero; m33 =  one; m34 =  tz;
			m41 = zero; m42 = zero; m43 = zero; m44 = one;
		}

		void Translation(const vector3_type& vTrans)
		{
			return this->Translation(vTrans.x, vTrans.y, vTrans.z);
		}

		void PerspectiveProjection(
			const elementType& fov_y,
			const elementType& aspect_ratio,
			const elementType& zn,
			const elementType& zf)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType half = one / static_cast<elementType>(2);
			const elementType zero = elementType();

			const elementType y_scale = one / static_cast<elementType>(tan(fov_y * half));
			const elementType x_scale = y_scale / aspect_ratio;
			const elementType prop_n = zf/(zf-zn);
			const elementType prop_f = -zn*zf/(zf-zn);

			m11 = x_scale; m12 =    zero; m13 =   zero; m14 =   zero;
			m21 =    zero; m22 = y_scale; m23 =   zero; m24 =   zero;
			m31 =    zero; m32 =    zero; m33 = prop_n; m34 = prop_f;
			m41 =    zero; m42 =    zero; m43 =    one; m44 =   zero;
		}
	};

	template<typename T, unsigned int ROW, unsigned int COLUMN> 
	CMatrix<T, COLUMN, ROW> operator ~ ( const CMatrix<T, ROW, COLUMN>& _rSrc)
	{// transpose
		CMatrix<T, COLUMN, ROW> temp;

		for(size_t _iter = 0 ; _iter < ROW ; _iter ++)
		{
			for(size_t _j = 0 ; _j < COLUMN ; _j ++)
			{
				temp.elements[_j][_iter]	= _rSrc.elements[_iter][_j];
			}
		}

		return temp;
	}
#pragma pack(pop)
}