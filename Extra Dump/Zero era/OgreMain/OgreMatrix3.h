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
#ifndef __Matrix3_H__
#define __Matrix3_H__

#include "OgrePrerequisites.h"

#include "OgreVector3.h"

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

namespace Ogre
{
    /** A 3x3 matrix which can represent rotations around axes.
        @note
            <b>All the code is adapted from the Wild Magic 0.2 Matrix
            library (http://www.geometrictools.com/).</b>
        @par
            The coordinate system is assumed to be <b>right-handed</b>.
    */
	class _OgreExport Matrix3
	{
	public:
		Vector3 getRow3( size_t i ) const
		{
			return Vector3(m[i][0], m[i][1], m[i][2]);
		}
		void identity()
		{
			_11=1.0f, _12=0.0f, _13=0.0f;
			_21=0.0f, _22=1.0f, _23=0.0f;
			_31=0.0f, _32=0.0f, _33=1.0f;
		}
		void setRow( size_t i, float x, float y, float z)
		{
			m[i][0] = x;
			m[i][1] = y;
			m[i][2] = z;
		}

		Vector3 getColumn(size_t i) const
		{
			return Vector3(m[0][i], m[1][i], m[2][i]);
		}

		float det() const;
		void inverse( Matrix3 &output ) const;
		void inverse();
		void makeScaleMatrix(const Vector3 &scale);
		void makeScaleMatrix(float s)
		{
			makeScaleMatrix(Vector3(s, s, s));
		}

		void transform(Vector3 &output, const Vector3 &input) const;
		void transform(WorldPos &output, const WorldPos &input) const;

	public:
		union
		{
			float m[3][3];
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
		};
	};

	inline Vector3 operator *(const Vector3 &vec, const Matrix3 &mat)
	{
		return Vector3( vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31,
			vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32,
			vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33 );
	}

	inline Matrix3 operator *( const Matrix3 &m1, const Matrix3 &m2 )
	{
		Matrix3 retvalue;
		for( int i=0; i<3; i++ )
		{
			for( int j=0; j<3; j++ )
			{
				retvalue.m[i][j] = m1.m[i][0]*m2.m[0][j] + m1.m[i][1]*m2.m[1][j] + m1.m[i][2]*m2.m[2][j];
			}
		}
		return retvalue;
	}
}

#endif
