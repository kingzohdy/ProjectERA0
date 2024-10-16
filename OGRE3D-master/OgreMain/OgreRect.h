
#ifndef __OgreRect_H__
#define __OgreRect_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	template<typename T>
	class TRect
	{
	public:
		TRect(){}
		TRect( T left, T top, T right, T bottom ) :m_Left(left), m_Top(top), m_Right(right), m_Bottom(bottom)
		{
		}

		void set(T left, T top, T right, T bottom)
		{
			m_Left = left, m_Top = top, m_Right = right, m_Bottom = bottom;
		}

		T getWidth() const{ return m_Right-m_Left; }
		T getHeight() const{ return m_Bottom-m_Top; }

		bool isEmpty() const
		{
			return m_Top == 0 && m_Bottom == 0 && m_Left == 0 && m_Right == 0;
		}
		
		template<typename RhsType>
		TRect<T>& operator=( const TRect<RhsType>& rhs )
		{
			m_Left		= rhs.m_Left;
			m_Top		= rhs.m_Top;
			m_Right		= rhs.m_Right;
			m_Bottom	= rhs.m_Bottom;
			return *this;
		}
		template<typename RhsType>
		TRect( RhsType left, RhsType top, RhsType right, RhsType bottom ) : m_Left(left), m_Top(top), 
																			m_Right(right), m_Bottom(bottom)
		{}

		template<typename RhsType>
		TRect( const TRect<RhsType>& rhs )
		{
			m_Left		= rhs.m_Left;
			m_Top		= rhs.m_Top;
			m_Right		= rhs.m_Right;
			m_Bottom	= rhs.m_Bottom;
		}

	public:
		T m_Left;
		T m_Top;
		T m_Right;
		T m_Bottom;
	};

	typedef TRect<int> RectInt;
	typedef TRect<float> RectFloat;

	template<typename T>
	class TBox
	{
	public:
		TBox(){}
		TBox( T left, T top, T front, T right, T bottom, T back ) :m_Left(left), m_Top(top), m_Front(front), m_Right(right), m_Bottom(bottom), m_Back(back)
		{
		}

		T getWidth() const{ return m_Right-m_Left; }
		T getHeight() const{ return m_Bottom-m_Top; }
		T getDepth() const{ return m_Back-m_Front; }

		bool contains(const TBox &def) const
		{
			return (def.m_Left >= m_Left && def.m_Top >= m_Top && def.m_Front >= m_Front &&
				def.m_Right <= m_Right && def.m_Bottom <= m_Bottom && def.m_Back <= m_Back);
		}

	public:
		T m_Left;
		T m_Top;
		T m_Front;
		T m_Right;
		T m_Bottom;
		T m_Back;
	};

	typedef TBox<int> BoxInt;
	typedef TBox<float> BoxFloat;

	template<typename T>
	class TVector2
	{
	public:
		TVector2(){}
		TVector2( T x1, T y1 ) : x(x1), y(y1){}
		TVector2(T v) : x(v), y(v){}

		const TVector2 &operator =(const TVector2 &vec)
		{
			x = vec.x;
			y = vec.y;
			return *this;
		}
		void operator +=( const TVector2 &vec )
		{
			x += vec.x;
			y += vec.y;
		}
		void operator -=( const TVector2 &vec )
		{
			x -= vec.x;
			y -= vec.y;
		}
		void operator *=( T s )
		{
			x *= s;
			y *= s;
		}
		void operator /=( T s )
		{
			x /= s;
			y /= s;
		}
		bool operator==(const TVector2 &vec)
		{
			return x==vec.x&&y==vec.y;
		}
		bool operator!=(const TVector2 &vec)
		{
			return x!=vec.x||y!=vec.y;
		}

		operator T *(){ return &x; }
		operator const T *() const{ return &x; }
		TVector2 operator +() const{ return *this; }
		TVector2 operator -() const{ return TVector2(-x,-y);}

		void SetElement( T x1, T y1 )
		{ 
			x = x1;
			y = y1;
		}
		T Length() const
		{
			return (T)sqrt(double(x*x) + double(y*y));
		}
		T LengthSqr() const
		{
			return x*x + y*y;
		}

	public:
		T x, y;
	};
	
	template<typename T>
	inline TVector2<T> operator+(const TVector2<T> &v1, const TVector2<T> &v2)
	{
		return TVector2<T>(v1.x+v2.x, v1.y+v2.y);
	}

	template<typename T>
	inline TVector2<T> operator-(const TVector2<T> &v1, const TVector2<T> &v2)
	{
		return TVector2<T>(v1.x-v2.x, v1.y-v2.y);
	}

	template<typename T>
	inline TVector2<T> operator*(const TVector2<T> &v1, const TVector2<T> &v2)
	{
		return TVector2<T>(v1.x*v2.x, v1.y*v2.y);
	}

	template<typename T>
	inline TVector2<T> operator/(const TVector2<T> &v1, const TVector2<T> &v2)
	{
		return TVector2<T>(v1.x/v2.x, v1.y/v2.y);
	}


	typedef TVector2<int> Point2D;
}

#endif