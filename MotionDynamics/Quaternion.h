#ifndef __QUATERNION_H__
#define __QUATERNION_H__


#pragma warning(push)
#pragma warning(disable:4201)


namespace NAnimation
{
#pragma pack(push)
#pragma pack(1)
	template <typename T>
	class Quaternion
	{
	public:
		typedef typename	T	element_type;
		typedef unsigned	int	size_type;
		typedef Quaternion< T > my_type;

	public:
		union
		{
			struct { element_type x, y, z, w; };
			element_type elements[4];
		};

		Quaternion()
			: x(element_type())
			, y(element_type())
			, z(element_type())
			, w(static_cast<element_type>(1))
		{
		}

		Quaternion(
			const element_type& arg_x,
			const element_type& arg_y,
			const element_type& arg_z,
			const element_type& arg_w)
			: x(arg_x)
			, y(arg_y)
			, z(arg_z)
			, w(arg_w)
		{
		}

		Quaternion(const my_type& rhs)
			: x(rhs.x)
			, y(rhs.y)
			, z(rhs.z)
			, w(rhs.w)
		{
		}

		element_type& operator[](size_type index)
		{
			assert(index < 4);
			return elements[index];
		}

		const element_type& operator[](size_type index) const
		{
			assert(index < 4);
			return element_type[index];
		}

		element_type& at(size_type index)
		{
			if (index < 4)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("Quaternion<T>::at");
			}
		}

		const element_type& at(size_type index) const
		{
			if (index < 4)
				return elements[index];
			else
			{
				assert(false);
				throw std::out_of_range("Quaternion<T>::at const");
			}
		}

		my_type& operator=(const my_type& rhs)
		{
			if (this != &rhs)
			{
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
				w = rhs.w;
			}
			return (*this);
		}

		my_type& operator*=(const my_type& rhs)
		{
			element_type new_x = w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y;
			element_type new_y = w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x;
			element_type new_z = w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w;
			element_type new_w = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;

			x = new_x;
			y = new_y;
			z = new_z;
			w = new_w;

			return (*this);
		}

		my_type operator*(const my_type& rhs) const
		{
			return my_type(
				w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
				w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x,
				w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w,
				w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
		}

		my_type& operator+=(const my_type& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return (*this);
		}

		my_type operator+(const my_type& rhs) const
		{
			return my_type(
				x + rhs.x,
				y + rhs.y,
				z + rhs.z,
				w + rhs.w);
		}

		my_type& operator*=(const element_type& c)
		{
			x *= c;
			y *= c;
			z *= c;
			w *= c;
			return (*this);
		}

		my_type operator*(const element_type& c) const
		{
			return my_type(x*c, y*c, z*c, w*c);
		}

		my_type& operator/=(const element_type& c)
		{
			const element_type one = static_cast<element_type>(1);
			const element_type inv_c = one / c;
			return ((*this) *= inv_c);
		}

		my_type operator/(const element_type& c) const
		{
			const element_type one = static_cast<element_type>(1);
			const element_type inv_c = one / c;
			return ((*this) * inv_c);
		}

		friend my_type operator*(const element_type& c, const my_type& rhs)
		{
			return my_type(
				c*rhs.x,
				c*rhs.y,
				c*rhs.z,
				c*rhs.w);
		}

		element_type Dot(const my_type& rhs) const
		{
			return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w);
		}

		element_type SquaredLength() const
		{
			return (x*x + y*y + z*z + w*w);
		}

		element_type Length() const
		{
			return static_cast<element_type>(sqrt(SquaredLength()));
		}

		void Normalize()
		{
			const element_type one = static_cast<element_type>(1);
			const element_type InvLength = one / Length();

			x *= InvLength;
			y *= InvLength;
			z *= InvLength;
			w *= InvLength;
		}

		void Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		void Inverse()
		{
			const element_type sq_length = SquaredLength();
			Conjugate();
			(*this) /= sq_length;
		}

		my_type Slerp(const my_type& rhs, const element_type& t) const
		{
			const element_type one = static_cast<element_type>(1);
			const element_type half = static_cast<element_type>(1) / static_cast<element_type>(2);

			element_type cos_value = this->Dot(rhs);
			if (cos_value > one)
				cos_value = one;
			else if (cos_value < -one)
				cos_value = -one;

			const element_type theta = acos(cos_value);
			const element_type sin_value = sqrt(one - cos_value*cos_value);
			if (sin_value < 1e-5f)
			{
				return (*this)*half + rhs*half;
			}

			const element_type a = sin(theta * (one - t)) / sin_value;
			const element_type b = sin(theta * t) / sin_value;

			return (*this)*a + rhs*b;
		}
	};
#pragma pack(pop)
}


#pragma warning(pop)


#endif//__QUATERNION_H__