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
// NOTE THAT THIS FILE IS BASED ON MATERIAL FROM:

// Magic Software, Inc.
// http://www.geometrictools.com/
// Copyright (c) 2000, All Rights Reserved
//
// Source code from Magic Software is supplied under the terms of a license
// agreement and may not be copied or disclosed except in accordance with the
// terms of that agreement.  The various license agreements may be found at
// the Magic Software web site.  This file is subject to the license
//
// FREE SOURCE CODE
// http://www.geometrictools.com/License/WildMagic3License.pdf

#ifndef __Quaternion_H__
#define __Quaternion_H__

#include "OgrePrerequisites.h"
#include "OgreMath.h"
#include "OgreArchive.h"

namespace Ogre {
	class _OgreExport Quaternion
	{
	public:
		Quaternion():x(0), y(0), z(0), w(1.0f){}
		Quaternion( float x1, float y1, float z1, float w1 ) : x(x1), y(y1), z(z1), w(w1){}
		Quaternion( const Vector3 &vForward, const Vector3 &vUp );

		Quaternion &operator =(const Quaternion &quat)
		{
			x = quat.x;
			y = quat.y;
			z = quat.z;
			w = quat.w;
			return *this;
		}
		void operator *=( float s )
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}
		float norm()
		{
			return x*x+y*y+z*z+w*w;
		}
		void identity()
		{
			x = y = z = 0;
			w = 1.0f;
		}
		void inverse()
		{
			x = -x;
			y = -y;
			z = -z;
		}
		void inverse( Quaternion &quat ) const
		{
			quat.x = -x;
			quat.y = -y;
			quat.z = -z;
			quat.w = w;
		}
		void setElement( float x1, float y1, float z1, float w1 )
		{
			x = x1;
			y = y1;
			z = z1;
			w = w1;
		}

		bool operator ==( const Quaternion &quat ) const;
		inline bool operator!= (const Quaternion& quat) const
		{
			return !operator==(quat);
		}
		float magnitude();
		void operator *=( const Quaternion &quat );
		void normalize();
		void setAxisAngle( const Vector3 &axis, float angle );
		void setAxisAngleX( float angle );
		void setAxisAngleY( float angle );
		void setAxisAngleZ( float angle );
		void rotate(const Vector3 &vAxis, float fAngle);
		void slerp( const Quaternion &q1, const Quaternion &q2, float t );
		void rotate( Vector3 &dest, const Vector3 &src ) const;
		void rotate( WorldPos &dest, const WorldPos &src ) const;
		void getMatrix( Matrix4 &mat ) const;
		void getMatrix( Matrix3 &mat ) const;
		Vector3 getAxisX() const;
		Vector3 getAxisY() const;
		Vector3 getAxisZ() const;
		void setMatrix( const Matrix4 &mat );
		void setRotateArc( const Vector3 &from, const Vector3 &to ); //to = from * (*this)
		void setEulerAngle( float yaw, float pitch, float roll );

		friend Archive &operator<<(Archive &ar, Quaternion &value)
		{
			ar.serializeRawType(value);
			return ar;
		}

		// ÌØ¶¨Öµ
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

	public:
		float x, y, z, w;
	};

	inline Quaternion operator *( const Quaternion &q, float s )
	{
		return Quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
	}

	inline Quaternion operator +( const Quaternion &q1, const Quaternion &q2 )
	{
		return Quaternion(q1.x+q2.x, q1.y+q2.y, q1.z+q2.z, q1.w+q2.w);
	}

	inline float DotProduct( const Quaternion &q1, const Quaternion &q2 )
	{
		return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	}

	inline Quaternion operator *(const Quaternion &q1, const Quaternion &q2 )
	{
		return Quaternion( q2.x*q1.w + q2.w*q1.x + q2.y*q1.z - q2.z*q1.y,
			q2.y*q1.w + q2.w*q1.y + q2.z*q1.x - q2.x*q1.z,
			q2.z*q1.w + q2.w*q1.z + q2.x*q1.y - q2.y*q1.x,
			q2.w*q1.w - q2.x*q1.x - q2.y*q1.y - q2.z*q1.z );
	}

	inline float DistanceSqr(const Quaternion &q1, const Quaternion &q2)
	{
		Quaternion inv_q2;
		q2.inverse(inv_q2);

		Quaternion diff = q1*inv_q2;
		return 1.0f-diff.w*diff.w;
	}

	inline float Distance(const Quaternion &q1, const Quaternion &q2)
	{
		return Sqrt(DistanceSqr(q1, q2));
	}
}

#endif 
