#pragma once

#undef	max

namespace NAnimation
{
#pragma pack(push)
#pragma pack(1)
	
	template <typename T, unsigned int DIMENSION>
	class CVector
	{
	public:
		typedef typename T				elementType;
		typedef typename int			sizeType;
		typedef CVector<T, DIMENSION>	vecType;

	public:
		elementType	elements[DIMENSION];

	public:
		CVector(void)
		{
			for(sizeType iter = 0 ; iter < DIMENSION ; iter++)
			{
				elements[ iter] = elementType();
			}
		}
		~CVector(void){};

		CVector(const elementType* _pElements)
		{
			assert(!IsBadReadPtr(_pElements, sizeof(elementType) * DIMENSION) );

			for( sizeType iter = 0 ; iter < DIMENSION ; iter ++)
			{
				elements[ iter] = _pElements[ iter];
			}
		}

		CVector(const elementType& _rSrc)
		{
			for(sizeType iter = 0 ; iter < DIMENSION ; iter ++)
			{
				elements[ iter ] = _rSrc;
			}
		}

		CVector(const vecType& _rVec)
		{
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elements[i] = _rVec.elements[i];
			}
		}

		const elementType& at(sizeType index) const
		{
			if(index < DIMENSION)
			{
				return elements[ index];
			}
			else
			{
				throw std::out_of_range("vector<T, DIMENSION>::at const");
			}
		}

		elementType& at(sizeType index)
		{
			if(index < DIMENSION)
			{
				return elements[ index];
			}
			else
			{
				throw std::out_of_range("vector<T, DIMENSION>::at const");
			}
		}

	public:
		vecType& operator=(const vecType& _rSrc)
		{
			if( this != &_rSrc)
			{
				for(sizeType iter = 0 ; iter < DIMENSION ; iter ++)
					elements[ iter ] = _rSrc.elements[ iter ];
			}
				return (*this);
		}

		vecType& operator=(sizeType value)
		{
			for(sizeType iter = 0 ; iter < DIMENSION ; iter ++)
				elements[ iter ] = value;

			return (*this);
		}
	
		T& operator[](sizeType index)
		{// 수정 필요-> 참조가 아니라 값 복사가 일어남
			assert(index < DIMENSION);
			return this->elements[index];
		}

		const T& operator[](sizeType index) const
		{
			assert(index < DIMENSION);
			return elements[index];
		}

		vecType& operator+=(const vecType& _rVec)
		{
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elements[i] += _rVec.elements[i];
			}
			return (*this);
		}

		vecType& operator-=(const vecType& _rVec)
		{
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elements[i] -= _rVec.elements[i];
			}
			return (*this);
		}

		vecType& operator*=(const elementType& c)
		{
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elements[i] *= c;
			}
			return (*this);
		}

		vecType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		vecType operator+(const vecType& _rVec) const
		{
			elementType elems[DIMENSION];
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elems[i] = elements[i] + _rVec.elements[i];
			}
			return vecType(elems);
		}

		vecType operator-(const vecType& _rVec) const
		{
			elementType elems[DIMENSION];
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elems[i] = elements[i] - _rVec.elements[i];
			}
			return vecType(elems);
		}

		vecType operator*(const elementType& c) const
		{
			elementType elems[DIMENSION];
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elems[i] = elements[i] * c;
			}
			return vecType(elems);
		}

		vecType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		vecType operator+(void) const
		{
			elementType elems[DIMENSION];
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elems[i] = +elements[i];
			}
			return vecType(elems);
		}

		vecType operator-(void) const
		{
			elementType elems[DIMENSION];
			for (sizeType i = 0; i < DIMENSION; ++i)
			{
				elems[i] = -elements[i];
			}
			return vecType(elems);
		}
		
		bool operator ==(const vecType& _rSrc)
		{
			for(sizeType iter_ = 0 ; iter_ < DIMENSION ; ++ iter_)
			{
				if(elements[ iter_] != _rSrc.elements[ iter_] )
					return false;
			}

			return true;
		}

		bool operator != (const vecType& _rSrc)
		{
			for(sizeType iter_ = 0 ; iter_ < DIMENSION ; ++ iter_)
			{
				if(elements[ iter_] == _rSrc.elements[ iter_] )
					return false;
			}

			return true;
		}


		void multiply(const vecType& _rVec)
		{
			for( sizeType iter = 0 ; iter < DIMENSION ; ++ iter)
				elements[ iter] *= _rVec.elements[ iter];
		}

		elementType squared(void) const
		{
			elementType sqLngth = elementType();
			for( sizeType iter = 0 ;iter < DIMENSION ; ++ iter)
			{
				sqLngth += elements[ iter ] * elements[ iter ];
			}

			return sqLngth;
		}

		elementType length(void) const
		{
			return static_cast<elementType>(sqrt(sqared()));
		}

		void normalise(void)
		{
			const elementType one		= static_cast<elementType>(1);
			const elementType invLngth	= one / length();

			for( sizeType iter = 0 ; iter < DIMENSION ; iter ++)
			{
				elements[ iter] = (elements[ iter] * invLngth);
			}
		}

		vecType normaliseVec(void)
		{
			vecType	normalised;
			const elementType one = static_cast<elementType>(1);

			if(length() == T(0))
				return *this;

			const elementType InvLength = one / length();

			for(size_t it_ = 0 ; it_ < DIMENSION ; it_ ++)
			{
				elements[it_]				*= InvLength;
				normalised.elements[it_]	= elements[it_];
			}

			return normalised;
		}


		elementType dotProduct(const vecType& _rVec) const
		{
			elementType dot	= elementType();

			for( sizeType iter = 0 ; iter < DIMENSION ; iter ++)
			{
				dot += (elements[ iter] * _rVec.elements[ iter ]);
			}

			return dot;
		}

		vecType crossProduct(const vecType& _rVec) const
		{
			elementType outer[DIMENSION];
			sizeType idx1, idx2;
			for (sizeType iter = 0; iter < DIMENSION; ++iter)
			{
				idx1 = (iter +1) % DIMENSION;
				idx2 = (iter +2) % DIMENSION;

				outer[iter] =
					elements[idx1]*_rVec.elements[idx2] -
					elements[idx2]*_rVec.elements[idx1];
			}
			return vecType(outer);

		}

		elementType&	operator [] (const size_t x)
		{
			assert(x < DIMENSION);

			return elements[ x];
		}


		void zero(void)
		{
			for(size_t i_ = 0 ; i_ < DIMENSION ; i_ ++)
			{
				elements[i_]	= T(0);
			}
		}
	};

//#pragma warning(push)
//#pragma warning(disable:4201)

	template <typename T>
	class CVector<T, 2>
	{
	public:
		typedef typename T elementType;
		typedef unsigned int sizeType;
		typedef CVector<T, 2> vecType;

	public:
		union
		{
			struct { elementType x, y; };
			struct { elementType u, v; };
			elementType elements[2];
		};

		CVector(void)
			: x(elementType()), y(elementType())
		{
		}

		CVector(const elementType* arg_elements)
		{
			assert(!IsBadReadPtr(arg_elements, sizeof(elementType) * 2));
			x = arg_elements[0];
			y = arg_elements[1];
		}

		CVector(const elementType& c)
			: x(c), y(c)
		{
		}

		CVector(const elementType& arg_x, const elementType& arg_y)
			: x(arg_x), y(arg_y)
		{
		}

		CVector(const vecType& _rVec)
			: x(_rVec.x), y(_rVec.y)
		{
		}

		elementType& operator[](sizeType index)
		{
			assert(index < 2);
			return elements[index];
		}

		const elementType& operator[](sizeType index) const
		{
			assert(index < 2);
			return elements[index];
		}

		elementType& at(sizeType index)
		{
			if (index < 2)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("CVector<T, 2>::at");
			}
		}

		const elementType& at(sizeType index) const
		{
			if (index < 2)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("CVector<T, 2>::at const");
			}
		}

		vecType& operator=(const vecType& _rVec)
		{
			if (this != &_rVec)
			{
				x = _rVec.x;
				y = _rVec.y;
			}
			return (*this);
		}

		vecType& operator=(sizeType value)
		{
			for(sizeType iter = 0 ; iter < DIMENSION ; iter ++)
				elements[ iter ] = value;

			return (*this);
		}

		vecType& operator+=(const vecType& _rVec)
		{
			x += _rVec.x;
			y += _rVec.y;
			return (*this);
		}

		vecType& operator-=(const vecType& _rVec)
		{
			x -= _rVec.x;
			y -= _rVec.y;
			return (*this);
		}

		vecType& operator*=(const elementType& c)
		{
			x *= c;
			y *= c;
			return (*this);
		}

		vecType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		vecType operator+(const vecType& _rVec) const
		{
			return vecType(x+_rVec.x, y+_rVec.y);
		}

		vecType operator-(const vecType& _rVec) const
		{
			return vecType(x-_rVec.x, y-_rVec.y);
		}

		vecType operator*(const elementType& c) const
		{
			return vecType(x*c, y*c);
		}

		vecType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		vecType operator+(void) const
		{
			return vecType(+x, +y);
		}

		vecType operator-(void) const
		{
			return vecType(-x, -y);
		}

		void multiply(const vecType& _rVec)
		{
			x *= _rVec.x;
			y *= _rVec.y;
		}

		void Maximize(void)
		{
			x = std::numeric_limits<elementType>::max();
			y = std::numeric_limits<elementType>::max();
		}

		void Minimize(void)
		{
			x = -std::numeric_limits<elementType>::max();
			y = -std::numeric_limits<elementType>::max();
		}

		elementType squared(void) const
		{
			return ((x*x) + (y*y));
		}

		elementType length(void) const
		{
			return static_cast<elementType>(sqrt(squared()));
		}

		void Normalize()
		{
			const elementType one = static_cast<elementType>(1);
			const elementType InvLength = one / Length();

			x *= InvLength;
			y *= InvLength;
		}

		elementType Dot(const vecType& _rVec) const
		{
			return ((x*_rVec.x) + (y*_rVec.y));
		}
	};

	template <typename T>
	class CVector<T, 3>
	{
	public:
		typedef typename T elementType;
		typedef unsigned int sizeType;
		typedef CVector<T, 3> vecType;

	public:
		union
		{
			struct { elementType x, y, z; };
			struct { elementType u, v, w; };
			elementType elements[3];
		};

		CVector(void)
			: x(elementType())
			, y(elementType())
			, z(elementType())
		{
		}

		CVector(const elementType* arg_elements)
		{
			assert(!IsBadReadPtr(arg_elements, sizeof(elementType) * 3));
			x = arg_elements[0];
			y = arg_elements[1];
			z = arg_elements[2];
		}

		CVector(const elementType& c)
			: x(c), y(c), z(c)
		{
		}

		CVector(
			const elementType& arg_x,
			const elementType& arg_y,
			const elementType& arg_z)
			: x(arg_x), y(arg_y), z(arg_z)
		{
		}

		CVector(const vecType& _rVec)
			: x(_rVec.x), y(_rVec.y), z(_rVec.z)
		{
		}

		elementType& operator[](sizeType index)
		{
			assert(index < 3);
			return elements[index];
		}

		const elementType& operator[](sizeType index) const
		{
			assert(index < 3);
			return elements[index];
		}

		elementType& at(sizeType index)
		{
			if (index < 3)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("CVector<T, 3>::at");
			}
		}

		const elementType& at(sizeType index) const
		{
			if (index < 3)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("CVector<T, 3>::at const");
			}
		}

		vecType& operator=(const vecType& _rVec)
		{
			if (this != &_rVec)
			{
				x = _rVec.x;
				y = _rVec.y;
				z = _rVec.z;
			}
			return (*this);
		}

		vecType& operator=(sizeType value)
		{
			for(sizeType iter = 0 ; iter < 3 ; iter ++)
				elements[ iter ] = value;

			return (*this);
		}

		vecType& operator+=(const vecType& _rVec)
		{
			x += _rVec.x;
			y += _rVec.y;
			z += _rVec.z;
			return (*this);
		}

		vecType& operator-=(const vecType& _rVec)
		{
			x -= _rVec.x;
			y -= _rVec.y;
			z -= _rVec.z;
			return (*this);
		}

		vecType& operator*=(const elementType& c)
		{
			x *= c;
			y *= c;
			z *= c;
			return (*this);
		}

		vecType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		vecType operator+(const vecType& _rVec) const
		{
			return vecType(x+_rVec.x, y+_rVec.y, z+_rVec.z);
		}

		vecType operator-(const vecType& _rVec) const
		{
			return vecType(x-_rVec.x, y-_rVec.y, z-_rVec.z);
		}

		vecType operator*(const elementType& c) const
		{
			return vecType(x*c, y*c, z*c);
		}

		vecType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		vecType operator+(void) const
		{
			return vecType(+x, +y, +z);
		}

		vecType operator-(void) const
		{
			return vecType(-x, -y, -z);
		}

		void multiply(const vecType& _rVec)
		{
			x *= _rVec.x;
			y *= _rVec.y;
			z *= _rVec.z;
		}

		void Maximize(void)
		{
			x = std::numeric_limits<elementType>::max();
			y = std::numeric_limits<elementType>::max();
			z = std::numeric_limits<elementType>::max();
		}

		void Minimize(void)
		{
			x = -std::numeric_limits<elementType>::max();
			y = -std::numeric_limits<elementType>::max();
			z = -std::numeric_limits<elementType>::max();
		}

		elementType squared(void) const
		{
			return ((x*x) + (y*y) + (z*z));
		}

		elementType length(void) const
		{
			return static_cast<elementType>(sqrt(squared()));
		}

		void normalise(void)
		{
			const elementType one		= static_cast<elementType>(1);
			const elementType slength	= length();

			if(slength == T(0))
				return;

			const elementType InvLength = one / slength;

			x *= InvLength;
			y *= InvLength;
			z *= InvLength;
		}

		vecType normaliseVec(void)
		{
			vecType	normalised;
			const elementType one = static_cast<elementType>(1);

			if(length() == T(0))
				return *this;

			const elementType InvLength = one / length();

			x *= InvLength;
			y *= InvLength;
			z *= InvLength;

			normalised.x	= x;
			normalised.y	= y;
			normalised.z	= z;

			return normalised;
		}


		elementType dotProduct(const vecType& _rVec) const
		{
			return ((x*_rVec.x) + (y*_rVec.y) + (z*_rVec.z));
		}

		vecType crossProduct(const vecType& _rVec) const
		{
			return vecType(
				y*_rVec.z - z*_rVec.y,
				z*_rVec.x - x*_rVec.z,
				x*_rVec.y - y*_rVec.x);
		}

		
		void zero(void)
		{
			x	= T(0);
			y	= T(0);
			z	= T(0);
		}
	};

	template <typename T>
	class CVector<T, 4>
	{
	public:
		typedef typename T elementType;
		typedef unsigned int sizeType;
		typedef CVector<T, 4> vecType;

	public:
		union
		{
			struct { elementType x, y, z, w; };
			elementType elements[4];
		};

		CVector(void)
			: x(elementType())
			, y(elementType())
			, z(elementType())
			, w(elementType())
		{
		}

		CVector(const elementType* arg_elements)
		{
			assert(!IsBadReadPtr(arg_elements, sizeof(elementType) * 4));
			x = arg_elements[0];
			y = arg_elements[1];
			z = arg_elements[2];
			w = arg_elements[3];
		}

		CVector(const elementType& c)
			: x(c), y(c), z(c), w(c)
		{
		}

		CVector(
			const elementType& arg_x,
			const elementType& arg_y,
			const elementType& arg_z,
			const elementType& arg_w)
			: x(arg_x), y(arg_y), z(arg_z), w(arg_w)
		{
		}

		CVector(const vecType& _rVec)
			: x(_rVec.x), y(_rVec.y), z(_rVec.z), w(_rVec.w)
		{
		}

		elementType& operator[](sizeType index)
		{
			assert(index < 4);
			return elements[index];
		}

		const elementType& operator[](sizeType index) const
		{
			assert(index < 4);
			return elements[index];
		}

		elementType& at(sizeType index)
		{
			if (index < 4)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("CVector<T, 4>::at");
			}
		}

		const elementType& at(sizeType index) const
		{
			if (index < 4)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("CVector<T, 4>::at const");
			}
		}

		vecType& operator=(const vecType& _rVec)
		{
			if (this != &_rVec)
			{
				x = _rVec.x;
				y = _rVec.y;
				z = _rVec.z;
				w = _rVec.w;
			}
			return (*this);
		}

		vecType& operator=(sizeType value)
		{
			for(sizeType iter = 0 ; iter < DIMENSION ; iter ++)
				elements[ iter ] = value;

			return (*this);
		}

		vecType& operator+=(const vecType& _rVec)
		{
			x += _rVec.x;
			y += _rVec.y;
			z += _rVec.z;
			w += _rVec.w;
			return (*this);
		}

		vecType& operator-=(const vecType& _rVec)
		{
			x -= _rVec.x;
			y -= _rVec.y;
			z -= _rVec.z;
			w -= _rVec.w;
			return (*this);
		}

		vecType& operator*=(const elementType& c)
		{
			x *= c;
			y *= c;
			z *= c;
			w *= c;
			return (*this);
		}

		vecType& operator/=(const elementType& c)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) *= inv_c);
		}

		vecType operator+(const vecType& _rVec) const
		{
			return vecType(x+_rVec.x, y+_rVec.y, z+_rVec.z, w+_rVec.w);
		}

		vecType operator-(const vecType& _rVec) const
		{
			return vecType(x-_rVec.x, y-_rVec.y, z-_rVec.z, w-_rVec.w);
		}

		vecType operator*(const elementType& c) const
		{
			return vecType(x*c, y*c, z*c, w*c);
		}

		vecType operator/(const elementType& c) const
		{
			const elementType one = static_cast<elementType>(1);
			const elementType inv_c = one / c;
			return ((*this) * inv_c);
		}

		vecType operator+(void) const
		{
			return vecType(+x, +y, +z, +w);
		}

		vecType operator-(void) const
		{
			return vecType(-x, -y, -z, -w);
		}

		void multiply(const vecType& _rVec)
		{
			x *= _rVec.x;
			y *= _rVec.y;
			z *= _rVec.z;
			w *= _rVec.w;
		}

		void Maximize(void)
		{
			const elementType maximum = std::numeric_limits<elementType>::max();
			w = z = y = x = maximum;
		}

		void Minimize(void)
		{
			const elementType minimum = -std::numeric_limits<elementType>::max();
			w = z = y = x = minimum;
		}

		elementType squared(void) const
		{
			return ((x*x) + (y*y) + (z*z) + (w*w));
		}

		elementType length(void) const
		{
			return static_cast<elementType>(sqrt(squared()));
		}

		void normalise(void)
		{
			const elementType one = static_cast<elementType>(1);
			const elementType InvLength = one / Length();

			x *= InvLength;
			y *= InvLength;
			z *= InvLength;
			w *= InvLength;
		}

		elementType dotProduct(const vecType& _rVec) const
		{
			return ((x*_rVec.x) + (y*_rVec.y) + (z*_rVec.z) + (w*_rVec.w));
		}
	};

#pragma pack(pop)
#pragma warning(pop)
};



//#endif//__VECTOR_H__
//#pragma pack(pop)

