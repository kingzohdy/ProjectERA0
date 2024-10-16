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
#ifndef __Vector4_H__
#define __Vector4_H__

#include "OgrePrerequisites.h"
#include "OgreVector3.h"

namespace Ogre
{

    /** 4-dimensional homogenous vector.
    */
	class Vector4
	{
	public:
		Vector4(){}
		Vector4( float x1, float y1, float z1, float w1 ) : x(x1), y(y1), z(z1), w(w1){}
		Vector4(const Vector3 &vec3, float w1) : x(vec3.x), y(vec3.y), z(vec3.z), w(w1){}
		Vector4(float v) : x(v), y(v), z(v), w(v){}

		const Vector4 &operator =(const Vector4 &vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
			return *this;
		}
		void operator +=( const Vector4 &vec )
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			w += vec.w;
		}
		void operator -=( const Vector4 &vec )
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;
		}
		void operator *=( float s )
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}
		void operator /=( float s )
		{
			x /= s;
			y /= s;
			z /= s;
			w /= s;
		}
		operator float *()
		{
			return &x;
		}
		operator const float *() const
		{
			return &x;
		}
		Vector4 operator +() const
		{
			return *this;
		}
		Vector4 operator -() const
		{
			return Vector4(-x, -y, -z, -w);
		}
		void setElement( float x1, float y1, float z1, float w1 )
		{
			x = x1;
			y = y1;
			z = z1;
			w = w1;
		}

		void setElement(const Vector3 &vec, float w1)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = w1;
		}

		float length() const
		{
			return sqrt(x*x+y*y+z*z+w*w);
		}
		float lengthSqr() const
		{
			return x*x+y*y+z*z+w*w;
		}

		friend Archive &operator<<(Archive &ar, Vector4 &value)
		{
			ar.serializeRawType(value);
			return ar;
		}

	public:
		float x, y, z, w;
	};

	_OgreExport void  Normalize(Vector4 &vec);
	_OgreExport bool  IsNormalized(const Vector4 &vec);

	inline Vector4 operator+( const Vector4 &v1, const Vector4 &v2 )
	{
		return Vector4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w);
	}

	inline Vector4 operator-( const Vector4 &v1, const Vector4 &v2 )
	{
		return Vector4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
	}

	inline Vector4 operator*( const Vector4 &v1, const Vector4 &v2 )
	{
		return Vector4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
	}

	inline Vector4 operator*( const Vector4 &v1, const float s )
	{
		return Vector4(v1.x*s, v1.y*s, v1.z*s, v1.w*s);
	}

	inline Vector4 operator/( const Vector4 &v1, const Vector4 &v2 )
	{
		return Vector4(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w);
	}

	inline float DotProduct( const Vector4 &v1, const Vector4 &v2 )
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
	}
	inline Vector4 Lerp( const Vector4 &v1, const Vector4 &v2, float t )
	{
		return Vector4(v1.x+(v2.x-v1.x)*t, v1.y+(v2.y-v1.y)*t,v1.z+(v2.z-v1.z)*t,v1.w+(v2.w-v1.w)*t );
	}

	inline float Distance( const Vector4 &p1, const Vector4 &p2 )
	{
		return (float)sqrt( double((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z) + (p1.w-p2.w)*(p1.w-p2.w)) );
	}

	inline float DistanceSqr( const Vector4 &p1, const Vector4 &p2 )
	{
		return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z) + (p1.w-p2.w)*(p1.w-p2.w);
	}
}

#endif
