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
#ifndef __Vector3_H__
#define __Vector3_H__

#include "OgrePrerequisites.h"
#include "OgreMath.h"
#include "OgreArchive.h"

namespace Ogre
{

    /** Standard 3-dimensional vector.
        @remarks
            A direction in 3D space represented as distances along the 3
            orthoganal axes (x, y, z). Note that positions, directions and
            scaling factors can be represented by a vector, depending on how
            you interpret the values.
    */
	class _OgreExport Vector3
	{
	public:
		Vector3(){}
		Vector3(const Vector3 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}
		Vector3( float x1, float y1, float z1 ) : x(x1), y(y1), z(z1){}
		Vector3(float v) : x(v), y(v), z(v){}
		Vector3(float* v) : x(v[0]), y(v[1]), z(v[2]){}

		const Vector3 &operator =(const Vector3 &vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}
		void operator +=( const Vector3 &vec )
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}
		void operator -=( const Vector3 &vec )
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
		}

		void operator *=( float s )
		{
			x *= s;
			y *= s;
			z *= s;
		}

		void operator *=( const Vector3 &vec )
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
		}

		void operator /=( float s )
		{
			x /= s;
			y /= s;
			z /= s;
		}
		operator float *()
		{
			return &x;
		}
		operator const float *() const
		{
			return &x;
		}
		Vector3 operator +() const
		{
			return *this;
		}
		Vector3 operator -() const
		{
			return Vector3(-x, -y, -z);
		}
		bool operator ==( const Vector3 &vec ) const
		{
			return (x==vec.x && y==vec.y && z==vec.z);
		}
		void setElement( float x1, float y1, float z1 )
		{
			x = x1;
			y = y1;
			z = z1;
		}
		void setElement( float* p )
		{
			x = p[0];
			y = p[1];
			z = p[2];
		}
		float length() const
		{
			return (float)Sqrt(x*x+y*y+z*z);
		}
		float lengthSqr() const
		{
			return (x*x+y*y+z*z);
		}

		friend Archive &operator<<(Archive &ar, Vector3 &value)
		{
			ar.serializeRawType(value);
			return ar;
		}

		// 特定值
		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 UNIT_SCALE;

	public:
		float x, y, z;
	};

	_OgreExport void GetPerpendicular(Vector3 &v1, Vector3 &v2, const Vector3 &vec); //得到和他垂直的两个TVector

	inline Vector3 operator+( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
	}

	inline Vector3 operator+( const Vector3 &v1, const float val )
	{
		return Vector3(v1.x+val, v1.y+val, v1.z+val);
	}

	inline Vector3 operator-( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
	}

	inline Vector3 operator-( const Vector3 &v1, const float val )
	{
		return Vector3(v1.x-val, v1.y-val, v1.z-val);
	}

	inline Vector3 operator*( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
	}

	inline Vector3 operator*( const Vector3 &v1, const float s )
	{
		return Vector3(v1.x*s, v1.y*s, v1.z*s);
	}

	inline Vector3 operator/( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
	}

	inline Vector3 operator/( const Vector3 &v1, float s )
	{
		return Vector3(v1.x/s, v1.y/s, v1.z/s);
	}

	inline Vector3 Abs( const Vector3 &v )
	{
		return Vector3( Abs(v.x), Abs(v.y), Abs(v.z) );
	}

	inline Vector3 Maximize( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(Max(v1.x,v2.x), Max(v1.y,v2.y), Max(v1.z,v2.z)); 
	}

	inline Vector3 Minimize( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(Min(v1.x,v2.x), Min(v1.y,v2.y), Min(v1.z,v2.z));
	}

	inline const Vector3 &Normalize(Vector3 &v)
	{
		//assert( v.length() > 0 );
		if(v.length() > 0)
		{
			float scale = 1.0f/v.length();
			v.x *= scale;
			v.y *= scale;
			v.z *= scale;
		}
		else
		{
			v.x = 0.0f;
			v.y = 0.0f;
			v.z = 0.0f;
		}

		return v;
	}

	inline Vector3 GetNormalize( const Vector3 &v )
	{
		//assert( v.length() > 0 );
		if(v.length() == 0.0f)
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			float scale = 1.0f/v.length();
			return Vector3(v.x*scale, v.y*scale, v.z*scale);
		}
	}

	inline bool IsNormalized(const Vector3 &vec)
	{
		return Abs(vec.length()-1.0f) < EPSILON;
	}

	inline Vector3 Lerp( const Vector3 &v1, const Vector3 &v2, float t )
	{
		return Vector3(v1.x+(v2.x-v1.x)*t, v1.y+(v2.y-v1.y)*t,v1.z+(v2.z-v1.z)*t );
	}

	inline float DotProduct( const Vector3 &v1, const Vector3 &v2 )
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}

	inline Vector3 CrossProduct( const Vector3 &v1, const Vector3 &v2 )
	{
		return Vector3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
	}

	inline float Distance( const Vector3 &p1, const Vector3 &p2 )
	{
		return (float)sqrt( double((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z)) );
	}

	inline float DistanceSqr( const Vector3 &p1, const Vector3 &p2 )
	{
		return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z);
	}
}
#endif
