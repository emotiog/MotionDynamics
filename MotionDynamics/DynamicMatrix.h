#pragma once

#include <vector>

#include "vector.h"

namespace NAnimation
{
	template<class T>
	class CDynamicMatrix 
	{
	public:
		class CBaseMatrix
		{
		public:
			T	**m2D;
			T	*m1D;	// 메모리 할 당을 두 번만 하도록 하기 위해서 추가함
			
			size_t	mRow, mCol;
			size_t	mRowSize, mColSize;
			
			unsigned int mRefCnt;

		public:
			CBaseMatrix(size_t row, size_t col, T** v)
			{
				mRow		= row;
				mRowSize	= row;
				mCol		= col;
				mColSize	= col;
				
				mRefCnt		= 1;

				m2D			= new T* [row];
				m1D			= new T [row*col];
				
				size_t rowlen	= col * sizeof(T);

				for (size_t iter=0; iter < row; iter++)
				{
					// m2D[i] = new T [col];
					m2D[iter] = &m1D[iter *col];
					
					if (v) 
						memcpy( m2D[iter], v[iter], rowlen);
				}
			};


			~CBaseMatrix(void)
			{
				destroy();
			};


			void destroy(void)
			{
				if(m2D != NULL)
				{
					delete[] m2D;
					m2D		= NULL;
				}

				if(m1D != NULL)
				{
					delete[] m1D;
					m1D		= NULL;
				}
			}
		};

		CBaseMatrix	* m;

	public:
		CDynamicMatrix( const CDynamicMatrix<T>& _m);
		CDynamicMatrix( size_t _row = 1, size_t _col = 1);


		~CDynamicMatrix(void);

		void clone(void);


	public:
		CDynamicMatrix<T>& operator = (const CDynamicMatrix<T>& _rSrc);
		
		T&	operator() (size_t _row, size_t _col);
		T	operator() (size_t _row, size_t _col) const;

		CDynamicMatrix<T>& operator += (const CDynamicMatrix<T>& _rSrc);
		CDynamicMatrix<T>& operator -= (const CDynamicMatrix<T>& _rSrc);

		CDynamicMatrix<T>& operator *= (const T& c);
		CDynamicMatrix<T>& operator *= (const CDynamicMatrix<T>& _rSrc);

		CDynamicMatrix<T>& operator /= (const T& c);
		CDynamicMatrix<T>& operator ^= (const size_t& pow);

		CDynamicMatrix<T>& operator - (void);

	public:
		void realloc( size_t _row, size_t _col);
		void extendSize( size_t _row, size_t _col);

		CDynamicMatrix<T>	inverse(void);

		void	identity(void);
		void	identity(const size_t& pow);

		void	null(void);
		void	null(const size_t& _row, const size_t& _col);

		int		pivot(size_t pow);
		
		T		determinent(void) const;

		T		normalise(void);

		T		condition(void);

		T		cofact(size_t _row, size_t _col);
		
		CDynamicMatrix<T>&	adjacement(void);

		CDynamicMatrix<T>	getSubMatrix(unsigned int _row, unsigned int _col,
			unsigned int _newRow, unsigned int _newCol);

		size_t	rowNo(void)	const	{ return m->mRow;	};
		size_t	colNo(void)	const	{ return m->mCol;	};

	public:
		bool	isSquareMatrixisSingular(void);
		bool	isIdentity(void);
		bool	isDiagonal(void);
		bool	isScalar(void);
		bool	isSymmetric(void);
		bool	isUpperTriangular(void);
		bool	isLowerTriangular(void);

	};


	template <class T> inline
		CDynamicMatrix<T>::CDynamicMatrix( const CDynamicMatrix<T>& _rM)
	{
		m	= _rM.m;
		m->mRefCnt ++;
	}


	template <class T> inline
		CDynamicMatrix<T>::CDynamicMatrix(size_t _row, size_t _col)
	{
		//	vector에서 m을 먼저 초기화 하고 실행하지를 못하겠다.
		//if( m != NULL)
		//	return;

		m	= new CBaseMatrix( _row, _col, 0 );
		for(unsigned int i_ = 0 ; i_ < _row ; i_ ++)
		{
			for(unsigned int j_ = 0 ; j_ < _col ; j_ ++)
				m->m2D[i_][j_]	= T(0);
		}
	}



	// Clone -------------------------------------------------------------
	//	detech one matrix and make new one
	//	internal copy constructor
	template <class T> inline void
		CDynamicMatrix<T>::clone(void)
	{
		m->mRefCnt--;
		m	= new CBaseMatrix(m->mRow, m->mCol, m->m2D);
	}


	template <class T> inline
		CDynamicMatrix<T>::~CDynamicMatrix(void)
	{
		if(m == NULL)
			return;

		// resize 할 때 RefCnt가 0보다 클 경우 자료 보존을 해야 한다.
		if( -- m->mRefCnt == 0)
		{
			delete m;
			m	= NULL;
		}
	}


	// = ------------------------------------------------------------------
	// assignment operator
	//	reference count 가 1 일 땐 포인터가 parameter를 인용하면 된다.
	//	1 이상 일 때는 값의 copy가 일어나야 한다.

	//	추후 이 부분에 추가해야 한다.
	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator = (const CDynamicMatrix<T>& _rSrc)
	{
		_rSrc.m->mRefCnt ++;

		if( -- m->mRefCnt == 0)
		{
			delete m;
			m = NULL;
		}

		m	= _rSrc.m;

		return *this;
	}

	
	template <class T> inline void
		CDynamicMatrix<T>::realloc ( size_t _row, size_t _col)
	{
		if( _row == m->mRowSize &&
			_col == m->mColSize )
		{
			m->mRow	= m->mRowSize;
			m->mCol	= m->mColSize;

			return;
		}

		CBaseMatrix *vm	= new CBaseMatrix(_row, _col, NULL);
		
		size_t	colSize	= min(m->mCol, _col) * sizeof(T);
		size_t	minRow	= min(m->mRow, _row);

		for( size_t iter_ = 0 ; iter_ < minRow ; iter_ ++)
		{
			memcpy( vm->m2D[iter_], m->m2D[iter_], colSize);
		}

		if( -- m->mRefCnt == 0)
		{
			delete m;
			m	= NULL;
		}

		m	= vm;

		return;
	}


	//	public method for resizing matrix
	template <class T> inline void
		CDynamicMatrix<T>::extendSize( size_t _row, size_t _col)
	{
		size_t	i_	= 0;
		size_t	j_	= 0;

		size_t	oldRow	= m->mRow;
		size_t	oldCol	= m->mCol;

		if( _row != m->mRowSize || _col != m->mRowSize )
			realloc( _row, _col );

		for( i_	= oldRow ; i_ < _row ; i_ ++)
		{
			for( j_ = 0 ; j_ < _col ; j_ ++)
			{
				m->m2D[i_][j_]	= T(0);
			}
		}

		for( i_ = 0 ; i_  < _row ; i_ ++)
		{
			for( j_ = oldCol ; j_ < _col ; j_ ++)
			{
				m->m2D[i_][j_]	= T(0);
			}
		}

		return;
	}


	// subscript operator to get/set individual elements
	template <class T> inline T&
		CDynamicMatrix<T>::operator() (size_t _row, size_t _col)
	{
		// out of range
		//assert( _row >= m->mRow || _col >= m->mCol);
		assert( _row < m->mRow || _col < m->mCol);

		if( m->mRefCnt > 1 )
			clone();

		return m->m2D[_row][_col];
	}


	// subscript operator to get/set individual elements
	template <class T> inline T
		CDynamicMatrix<T>::operator() (size_t _row, size_t _col) const
	{
		// out of range
		//assert( _row >= m->mRow || _col >= m->mCol);
		assert( _row < m->mRow || _col < m->mCol);

		return m->m2D[_row][_col];
	}


	// logical equal-to operator
	template <class T> inline bool
		operator == (const CDynamicMatrix<T>& m1, const CDynamicMatrix<T>& m2)
	{
		if(m1.rowNo() != m2.rowNo() || m1.colNo() != m2.colNo() )
			return false;

		for(size_t i_ = 0 ; i_ < m1.rowNo() ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m1.colNo() ; j_ ++)
			{
				if(m1(i_, j_) != m2(i_, j_))
					return false;
			}
		}

		return true;
	}


	// logical no-equal to operator
	template <class T> inline bool
		operator != (const CDynamicMatrix<T>& m1, const CDynamicMatrix<T>& m2)
	{
		return (m1 == m2) ? false : true;
	}


	// combined addition and assignment operator
	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator += (const CDynamicMatrix<T>& _rSrc)
	{
		//assert( m->mRow != _rSrc.m->mRow || m->mCol != _rSrc.m->mCol );
		assert( m->mRow == _rSrc.m->mRow && m->mCol == _rSrc.m->mCol );

		if(m->mRefCnt > 1)
			clone();

		for(size_t i_ = 0 ; i_ < _rSrc.m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < _rSrc.m->mCol ; j_ ++)
			{
				m->m2D[i_][j_] += _rSrc.m->m2D[i_][j_];
			}
		}

		return *this;
	}


	// combined subtraction and assignment operator 
	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator -= (const CDynamicMatrix<T>& _rSrc)
	{
		assert( m->mRow == _rSrc.m->mRow && m->mCol == _rSrc.m->mCol);

		if(m->mRefCnt > 1 ) 
			clone();

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_]	-= _rSrc.m->m2D[i_][j_];
			}
		}

		return *this;
	}


	// multiply and assigment operator
	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator *= (const T& c)
	{
		if(m->mRefCnt > 1)
			clone();

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_] += c;
			}
		}

		return *this;
	}

	// multiply matrix
	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator *= (const CDynamicMatrix<T>& _rSrc)
	{
		assert( m->mCol == _rSrc.m->mRow);

		CDynamicMatrix<T> temp(m->mRow, _rSrc.m->mCol);

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < _rSrc.m->mCol ; j_ ++)
			{
				temp.m->m2D[i_][j_] = T(0);

				for(size_t k_ = 0 ; k_ < m->mCol ; k_ ++)
				{
					temp.m->m2D[i_][j_] += m->m2D[i_][k_] * _rSrc.m->m2D[k_][j_];
				}
			}
		}

		*this = temp;

		return *this;
	}


	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator /= (const T& c)
	{
		if(m->mRefCnt > 1) 
			clone();

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_] /= c;
			}
		}

		return *this;
	}


	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator ^= (const size_t& pow)
	{
		CDynamicMatrix<T> temp(*this);

		for(size_t i_ = 2 ; i_ <= pow ; i_ ++)
			*this = *this * temp;

		return *this;
	}


	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::operator - (void)
	{
		CDynamicMatrix<T> temp(m->mRow , m->mCol);

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				temp.m->m2D[i_][j_] = - _m->m2D[i_][j_];
			}
		}

		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator + (const CDynamicMatrix<T>& m1, const CDynamicMatrix<T>& m2)
	{
		CDynamicMatrix<T> temp = m1;
		temp += m2;
		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator - (const CDynamicMatrix<T>& m1, const CDynamicMatrix<T>& m2)
	{
		CDynamicMatrix<T> temp = m1;
		temp -= m2;
		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator * (const CDynamicMatrix<T>& m1, const T& c)
	{
		CDynamicMatrix<T> temp = m1;
		temp *= c;
		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator * (const T& c, const CDynamicMatrix<T>& m1)
	{
		return ( m1 * c);
	}


	template <class T> inline CDynamicMatrix<T>
		operator * (const CDynamicMatrix<T>& m1, const CDynamicMatrix<T>& m2)
	{
		CDynamicMatrix<T> temp = m1;
		temp *= m2;
		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator / (const CDynamicMatrix<T>& m1, const T& c)
	{
		return (m1 * (T(1) / c));
	}


	template <class T> inline CDynamicMatrix<T>
		operator / (const T& c, const CDynamicMatrix<T>& m1)
	{
		return (!m1 * c);
	}

	template <class T> inline CDynamicMatrix<T>
		operator / (const CDynamicMatrix<T>& m1, const CDynamicMatrix<T>& m2)
	{
		return (m1 + !m2);
	}


	template <class T> inline CDynamicMatrix<T>
		operator ^ (const CDynamicMatrix<T>& m1, const size_t& pow)
	{
		CDynamicMatrix<T> temp = m1;
		temp ^= pow;
		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator ~ (const CDynamicMatrix<T>& m1)
	{
		CDynamicMatrix<T> temp(m1.colNo(), m1.rowNo());

		for(size_t i_ = 0 ; i_ < m1.rowNo() ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m1.colNo() ; j_ ++)
			{
				T x = m1(i_, j_);
				temp(j_ , i_ ) = x;
			}
		}

		return temp;
	}


	template <class T> inline CDynamicMatrix<T>
		operator ! (const CDynamicMatrix<T>& m1)
		{
			CDynamicMatrix<T> temp	= m1;	// 만드는 과정에서 0이 되어버린다???
			return temp.inverse();
		}


	template <class T> inline CDynamicMatrix<T>
		CDynamicMatrix<T>::inverse(void)
	{
		assert(m->mRow == m->mCol);

		CDynamicMatrix<T> temp(m->mRow, m->mCol);
		

		// 현재 값을 이용해서 값이 바뀌는 것을 막기 위해
		// 새로운 배열로 복제
		if(m->mRefCnt > 1)
			clone();

		temp.identity();

		for(size_t k_ = 0 ; k_ < m->mRow ; k_ ++)
		{
			int index = pivot(k_);

			assert(index != -1);
			
			if(index != 0)
			{
				T* powPtr			= temp.m->m2D[k_];
				temp.m->m2D[k_]		= temp.m->m2D[index];
				temp.m->m2D[index]	= powPtr;
			}

			T	a1	= m->m2D[k_][k_];

			for(size_t j_ = 0 ; j_ < m->mRow ; j_ ++)
			{
				m->m2D[k_][j_]		/= a1;
				temp.m->m2D[k_][j_]	/= a1;
			}

			for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
			{
				if( i_ != k_ )
				{
					T a2	= m->m2D[i_][k_];

					for(size_t j_ = 0 ; j_ < m->mRow ; j_ ++)
					{
						m->m2D[i_][j_]		-= a2 * m->m2D[k_][j_];
						temp.m->m2D[i_][j_]	-= a2 * temp.m->m2D[k_][j_];
					}
				}
			}
		}

		return temp;
	}


	template <class T> inline void
		CDynamicMatrix<T>::null(const size_t& _row, const size_t& _col)
	{
		if(_row != m->mRow || _col != m->mCol)
		{
			realloc( _row, _col);
		}

		if(m->mRefCnt > 1)
			clone();

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_]	= T(0);
			}
		}

		return;
	}


	template <class T> inline void
		CDynamicMatrix<T>::null(void)
	{
		if(m->mRefCnt > 1)
			clone();

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_]	= T(0);
			}
		}

		return;
	}


	template <class T> inline void
		CDynamicMatrix<T>::identity(const size_t& _row)
	{
		if( _row != m->mRow || _row != m->mCol)
			realloc( _row , _row);

		if(m->mRefCnt > 1)
			clone();

		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_]	= i == j ? T(1) : T(0);
			}
		}

		return;
	}


	template <class T> inline void
		CDynamicMatrix<T>::identity(void)
	{
		if(m->mRefCnt > 1)
			clone();

		size_t row = min(m->mRow , m->mCol);

		m->mRow = m->mCol = row;
	
		for(size_t i_ = 0 ; i_ < m->mRow ; i_ ++)
		{
			for(size_t j_ = 0 ; j_ < m->mCol ; j_ ++)
			{
				m->m2D[i_][j_]	= i_ == j_ ? T(1) : T(0);
			}
		}
	}


	template <class T> inline int
		CDynamicMatrix<T>::pivot(size_t _row)
	{
		int k_ = static_cast<int>(_row);

		double amax = -1;
		double temp = 0.;

		for(size_t i_ = _row ; i_ < m->mRow ; i_ ++)
		{
			temp = abs(m->m2D[i_][_row]);

			if( temp > amax && temp != 0.)
			{
				amax	= temp;
				k_		= i_;
			}

			if( m->m2D[k_][_row] == T(0))
				return -1;

			if( k_ != static_cast<int>(_row))
			{
				T* rowPtr		= m->m2D[k_];
				m->m2D[k_]		= m->m2D[_row];
				m->m2D[_row]	= rowPtr;

				return k_;
			}
		}
		return 0;
	}


	template <class T> inline T
		CDynamicMatrix<T>::determinent(void) const
	{
		assert(m->mRow == m->mCol);

		CDynamicMatrix<T> temp(*this);

		if(temp.m->mRefCnt > 1) 
			clone();

		T determinent_value	= T(1);
		T piv	= T(1);

		for( size_t k_ = 0 ; k_ < m->mRow ; k_ ++)
		{
			int index = temp.pivot(k_);

			if(index == -1)
				return 0;

			if(index != 0)
				determinent_value = -determinent_value;

			determinent_value	= determinent_value * temp.m->m2D[k_][k_];

			for(size_t i_ = k_ +1 ; i_ < m->mRow ; i_ ++)
			{
				piv = temp.m->m2D[i_][k_] / temp.m->m2D[k_][k_];

				for(size_t j_ = k_ +1 ; j_ < m->mRow ; j_ ++)
				{
					temp.m->m2D[i_][j_]	-= piv * temp.m->m2D[k_][j_];
				}
			}
		}

		return determinent_value;
	}



	template <class T> inline T
		CDynamicMatrix<T>::normalise(void)
	{
		T retValue	= T(0);

		for(size_t i = 0 ; i < m->mRow ; i ++)
		{
			for(size_t j = 0 ; j < m->mCol ; j ++)
			{
				retValue	+= m->m2D[i_][j_] * m->m2D[i_][j_];
			}
		}

		retValue	= sqrt( retValue);

		return retValue;
	}


	// calculate the condition number of a matrix
	template <class T> inline T
		CDynamicMatrix<T>::condition(void)
	{
		CDynamicMatrix<T>	inv = !(*this);
		return (normalise() * inv.normalise());
	}


	// calculate the cofactor of a matrix for a given element
	template <class T> inline T
		CDynamicMatrix<T>::cofact(size_t _row, size_t _col)
	{
		// square matrix
		assert(m->mRow == m->mCol);

		// check index range
		assert(_row < m->mRow && _col < m->mCol);

		CDynamicMatrix<T>& temp(m->mRow -1, m->mCol -1);

		for(size_t i = 0 , size_t i1 = 0 ; i < m->mRow ; i ++)
		{
			if( i == _row)
				continue;

			for(size_t j = 0, size_t j1 = 0 ; j < m->mCol ; j ++)
			{
				if(j == col)
					continue;

				temp.m->m2D[i1][j1]	= m->m2D[i][j];
				j1 ++;
			}
			i1 ++;
		}

		T cof = temp.determinent();

		if((_col + _row) %2 == 1)
			cof = -cof;

		return cof;
	}


	template <class T> inline CDynamicMatrix<T>&
		CDynamicMatrix<T>::adjacement(void)
	{
		assert(m->mRow == m->mCol);

		CDynamicMatrix<T> temp(m->mRow, m->mCol);

		for(size_t i = 0 ; i < m->mRow ; i ++)
		{
			for(size_t j = 0 ; j < m->mCol ; j ++)
			{
				temp.m->m2D[i][j]	= cofact(i,j);
			}
		}

		return temp;
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isSquareMatrixisSingular(void)
	{
		if(m->mRow != m->mCol)
			return false;

		return (determinent() == T(0));
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isDiagonal(void)
	{
		if(m->mRow != m->mCol)
			return false;

		for(size_t i = 0 ; i < m->mRow ; i ++)
		{
			for(size_t j = 0 ; j < m->mCol ; j ++)
			{
				if(i != j && m->m2D[i][j] != T(0))
					return false
			}
		}

		return true;
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isScalar(void)
	{
		if(!isDiagonal())
			return false;

		T v = m->m2D[0][0];

		for(size_t i = 1; i < m->mRow ; i ++)
		{
			if(m->m2D[i][i] != v)
				return false;
		}

		return true;
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isIdentity(void)
	{
		if(isScalar() && m->m2D[0][0] == T(1))
			return true;
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isSymmetric(void)
	{
		if(m->mRow != m->mCol)
			return false;

		for(size_t i = 0 ; i < m->mRow ; i ++)
		{
			for(size_t j = 0 ; j < m->mCol ; j ++)
			{
				if(m->m2D[i][j] != m->m2D[j][i])
					return false;
			}
		}

		return true;
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isUpperTriangular(void)
	{
		if( m->mRow != m->mCol)
			return false;
		
		for(size_t i = 1 ; i < m->mRow ; i ++)
		{
			for(size_t j = 0 ; j < i -1 ; j ++)
			{
				if(m->m2D[i][j] != T(0))
					return false;
			}
		}

		return true;
	}


	template <class T> inline bool
		CDynamicMatrix<T>::isLowerTriangular(void)
	{
		if( m->mRow != m->mCol)
			return false;
		
		for(size_t j = 1 ; j < m->mCol ; j ++)
		{
			for(size_t i = 0 ; i < j -1 ; i ++)
			{
				if(m->m2D[i][j] != T(0))
					return false;
			}
		}

		return true;
	}


	template <class T> inline CDynamicMatrix<T>
		CDynamicMatrix<T>::getSubMatrix(unsigned int _row, unsigned int _col,
			unsigned int _newRow, unsigned int _newCol)
	{
		assert( _row + _newRow <= m->mRowSize);
		assert( _col + _newCol <= m->mColSize);

		CDynamicMatrix<T> &m	= *this;
		CDynamicMatrix<T>	n(_newRow, _newCol);

		for(unsigned int r_ = 0 ; r_ < _newRow ; ++ r_)
		{
			for(unsigned int c_ = 0 ; c_ < _newCol ; c_ ++)
			{
				n(r_, c_)	= m(_row + r_ , _col + c_);
			}
		}

		return n;
	}



	template<class T>
	class CDynamicVector : public CDynamicMatrix<T>
	{
	public:
		//CDynamicVector (void) : CDynamicMatrix<T>(3, 1)
		//{		};

		CDynamicVector (const CDynamicVector<T>& _rSrc)
			: CDynamicMatrix<T>(_rSrc)	{};

		CDynamicVector (const CDynamicMatrix<T>& _rSrc) 
			: CDynamicMatrix<T>(_rSrc)	{};

		CDynamicVector (size_t size	= 3)
			: CDynamicMatrix<T>(size, 1) {};

	public:
		size_t	size(void)	const	{ return	rowNo() * colNo();	 };
		void	resize(size_t size)	{ extendSize(size, 1);	};

		void	zero(void)	{ null();	};

		T	operator[] (size_t i_) const
		{
			assert(colNo() == 1);

			return (*this)(i_, 0);
		}

		T& operator[] (size_t i_)
		{
			assert(colNo() == 1);

			return (*this)(i_, 0);
		}

		CDynamicVector<T> operator = (const CDynamicVector<T>& m1)
		{
			if( this != &m1)
			{
				CDynamicMatrix<T>::operator = (m1);
			}

			return *this;
		}


	public:
		T dotProduct(const CDynamicVector<T>& _rSrc)
		{
			assert(_rSrc.size() == this->size());

			T	result	= T(0);

			for(unsigned int iter = 0 ; iter < _rSrc.size() ; ++ iter)
			{
				result += this[iter] * _rSrc[iter];
			}

			return result;
		}


		CDynamicVector<T> crossProduct(const CDynamicVector<T>& _v)
		{
			assert(this->size() == 3 && _v.size() == 3);

			CDynamicVector<T> vector(3);

			vector[0]	= ((*this)[1] * _v[2]) - ((*this)[2] * _v[1]);
			vector[1]	= ((*this)[2] * _v[0]) - ((*this)[0] * _v[2]);
			vector[2]	= ((*this)[0] * _v[1]) - ((*this)[1] * _v[0]); 

			return vector;
		}


		T	length(void)
		{
			T s = T(0);

			for(auto i_ = 0 ; i_ < (int)size(); ++ i_)
			{
				s += this[ i_] * this[ i_];
			}

			return sqrt(s);
		}


		void	setVector( T _x , T _y , T _z)
		{
			if(size() != 3)
				resize(3);

			this[0]	= _x;
			this[1]	= _y;
			this[2]	= _z;
		}
	};
};