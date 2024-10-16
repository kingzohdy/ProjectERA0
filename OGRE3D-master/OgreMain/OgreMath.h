#ifndef __Math_H__
#define __Math_H__

#include "OgrePrerequisites.h"
#include <math.h>

namespace Ogre
{
	const float ONE_PI = 3.14159f;
	const float TWO_PI = ONE_PI*2;
	const float	HALF_PI = ONE_PI*0.5f;
	const float EPSILON = 0.00001f;
	const float RADS_PER_DEG = (0.01745329251994329576f);
	const float DEGS_PER_RAD = (57.2957795130823208767f);

	inline float Deg2Rad( float x )
	{
		return x*RADS_PER_DEG;
	}
	inline float Rad2Deg( float x )
	{
		return x*DEGS_PER_RAD;
	}

	inline float Sin( float x )
	{
		return (float)sin( Deg2Rad(x) );
	}

	inline float Cos( float x )
	{
		return (float)cos( Deg2Rad(x) );
	}

	float fastSin (float x);
	float fastCos (float x);

	inline float Tan( float x )
	{
		return (float)tan( Deg2Rad(x) );
	}

	inline float Cotan( float x )
	{
		return 1.0f/Tan(x);
	}

	inline float ASin( float x )
	{
		return Rad2Deg((float)asin(x));
	}

	inline float ACos( float x )
	{
		return Rad2Deg((float)acos(x));
	}

	inline float Sqrt( float x )
	{
		return (float)sqrt(x);
	}

	inline float Pow( float x, float y )
	{
		return (float)pow(x, y);
	}

	inline float Log2( float x )
	{
		const float LN_2 = 0.693147180559945f;
		return ::logf(x) / LN_2;
	}

	inline bool FltEqual( float x, float y )
	{
		float tmp = x - y;
		return ( tmp>-EPSILON && tmp<EPSILON );
	}

	inline bool IsFloatValid( float x )
	{
		assert( *(uint *)&x != 0xcdcdcdcd );
		assert( *(uint *)&x != 0xdddddddd );

		// this works because NAN always returns false on compares
		return (x>=-MAX_FLOAT && x<=MAX_FLOAT);
	}

	inline int RoundToFloor( float x )
	{
		return (int)x;
	}

	inline int RoundToNear( float x )
	{
		return (int)(x+0.5f);
	}

	inline int RoundToCeil( float x )
	{
		return (int)::ceilf(x);
	}

	template<class Type> inline void Swap(Type &a,Type &b)
	{
		Type c(a); a = b; b = c;
	}

	template<class Type> inline Type Clamp(const Type &x,const Type &lo,const Type &hi)
	{
		return ( ( x < lo ) ? lo : ( x > hi ) ? hi : x );
	}

	template<class Type> inline Type Min(const Type &a,const Type &b)
	{
		return ( (a) < (b) ? (a) : (b) );
	}

	template<class Type> inline Type Max(const Type &a,const Type &b)
	{
		return ( (a) > (b) ? (a) : (b) );
	}

	template<class Type> inline Type Abs(const Type &x)
	{
		return (x<0.0f) ? (-x) : x;
	}

	template<class Type> inline Type Sign(const Type &x)
	{
		return (x<0) ? Type(-1): (x>0?Type(1):0);
	}

	inline float Lerp( float v1, float v2, float t )
	{
		return v1+(v2-v1)*t;
	}

	template <class T, class U>
	T CubicInterp (const T& P0, const T& T0,
		const T& P1, const T& T1,
		const U& A)
	{
		const float A2 = A*A;
		const float A3 = A2*A;

		return (T)
			(((2*A3)-(3*A2)+1)*P0) + ((A3-(2*A2)+A)*T0) + ((A3-A2)*T1) + (((-2*A3)+(3*A2))*P1);	
	}

	inline bool  IsPowerOfTwo( int n )
	{
		return (((n & (n-1)) == 0) && (n != 0));
	}

	inline int  PrevIntLog2( unsigned x )
	{
		float fx;
		unsigned int rx;

		if( x<=1 ) return x - 1;

		fx = (float)(int)(x);
		rx = *(unsigned int*)(&fx);

		return (rx >> 23) - 127;
	}

	inline int  NextIntLog2( unsigned x )
	{
		float fx;
		unsigned int rx;

		if (x <= 1) return x - 1;

		fx = (float)(int)(x - 1);
		rx = *(unsigned int*)(&fx);

		return (rx >> 23) - 126;
	}

	inline unsigned int PrevPowerOfTwo( unsigned int x )
	{
		x |= x >> 16;
		x |= x >> 8;
		x |= x >> 4;
		x |= x >> 2;
		x |= x >> 1;

		return (x + 1) >> 1;
	}

	inline unsigned int NextPowerOfTwo( unsigned int x )
	{
		x -= 1;

		x |= x >> 16;
		x |= x >> 8;
		x |= x >> 4;
		x |= x >> 2;
		x |= x >> 1;

		return x + 1;
	}
}

#endif
