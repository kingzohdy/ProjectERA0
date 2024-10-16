/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __Vector2_H__
#define __Vector2_H__


#include "OgrePrerequisites.h"
#include "OgreMath.h"
#include "OgreArchive.h"

namespace Ogre
{

    /** Standard 2-dimensional vector.
        @remarks
            A direction in 2D space represented as distances along the 2
            orthoganal axes (x, y). Note that positions, directions and
            scaling factors can be represented by a vector, depending on how
            you interpret the values.
    */
	class _OgreExport Vector2
	{
	public:
		Vector2(){}
		Vector2( float x1, float y1 ) : x(x1), y(y1){}
		Vector2(float v) : x(v), y(v){}

		const Vector2 &operator =(const Vector2 &vec)
		{
			x = vec.x;
			y = vec.y;
			return *this;
		}
		void operator +=( const Vector2 &vec )
		{
			x += vec.x;
			y += vec.y;
		}
		void operator -=( const Vector2 &vec )
		{
			x -= vec.x;
			y -= vec.y;
		}
		void operator *=( float s )
		{
			x *= s;
			y *= s;
		}
		void operator /=( float s )
		{
			x /= s;
			y /= s;
		}
		bool operator==(const Vector2 &vec)
		{
			return x==vec.x&&y==vec.y;
		}
		bool operator!=(const Vector2 &vec)
		{
			return x!=vec.x||y!=vec.y;
		}

		operator float *(){ return &x; }
		operator const float *() const{ return &x; }
		Vector2 operator +() const{ return *this; }
		Vector2 operator -() const{ return Vector2(-x,-y);}

		void setElement( float x1, float y1 )
		{ 
			x = x1;
			y = y1;
		}
		float length() const
		{
			return (float)sqrt(x*x + y*y);
		}
		float lengthSqr() const
		{
			return x*x + y*y;
		}

		friend Archive &operator<<(Archive &ar, Vector2 &value)
		{
			ar.serializeRawType(value);
			return ar;
		}

	public:
		float x, y;
	};

	_OgreExport void  Normalize(Vector2 &vec);
	_OgreExport bool  IsNormalized(const Vector2 &vec);

	//inline functions:

	inline Vector2 operator+( const Vector2 &v1, const Vector2 &v2 )
	{
		return Vector2(v1.x+v2.x, v1.y+v2.y);
	}

	inline Vector2 operator-( const Vector2 &v1, const Vector2 &v2 )
	{
		return Vector2(v1.x-v2.x, v1.y-v2.y);
	}

	inline Vector2 operator*( const Vector2 &v1, const Vector2 &v2 )
	{
		return Vector2(v1.x*v2.x, v1.y*v2.y);
	}

	inline Vector2 operator/( const Vector2 &v1, const Vector2 &v2 )
	{
		return Vector2(v1.x/v2.x, v1.y/v2.y);
	}

	inline Vector2 operator*( const Vector2 &v1, float s )
	{
		return Vector2(v1.x*s, v1.y*s);
	}

	inline Vector2 operator/( const Vector2 &v1, float s )
	{
		return Vector2(v1.x/s, v1.y/s);
	}

	inline Vector2 Abs( const Vector2 &v )
	{
		return Vector2( Abs(v.x), Abs(v.y) );
	}

	inline Vector2 Maximize( const Vector2 &v1, const Vector2 &v2 )
	{
		return Vector2(Max(v1.x,v2.x), Max(v1.y,v2.y)); 
	}

	inline Vector2 Minimize( const Vector2 &v1, const Vector2 &v2 )
	{
		return Vector2(Min(v1.x,v2.x), Min(v1.y,v2.y));
	}

	inline Vector2 Lerp( const Vector2 &v1, const Vector2 &v2, float t )
	{
		return Vector2(v1.x+(v2.x-v1.x)*t, v1.y+(v2.y-v1.y)*t);
	}

	inline float DotProduct( const Vector2 &v1, const Vector2 &v2 )
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	inline float Distance( const Vector2 &p1, const Vector2 &p2 )
	{
		return (float)sqrt( double((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)) );
	}

	inline float DistanceSqr( const Vector2 &p1, const Vector2 &p2 )
	{
		return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y);
	}
}
#endif
