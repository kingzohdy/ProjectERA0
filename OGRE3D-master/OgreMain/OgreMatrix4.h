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
#ifndef __Matrix4__
#define __Matrix4__

// Precompiler options
#include "OgrePrerequisites.h"

#include "OgreVector3.h"
#include "OgreMatrix3.h"
#include "OgreVector4.h"

namespace Ogre
{
    /** Class encapsulating a standard 4x4 homogenous matrix.
        @remarks
            OGRE uses column vectors when applying matrix multiplications,
            This means a vector is represented as a single column, 4-row
            matrix. This has the effect that the tranformations implemented
            by the matrices happens right-to-left e.g. if vector V is to be
            transformed by M1 then M2 then M3, the calculation would be
            M3 * M2 * M1 * V. The order that matrices are concatenated is
            vital since matrix multiplication is not cummatative, i.e. you
            can get a different result if you concatenate in the wrong order.
        @par
            The use of column vectors and right-to-left ordering is the
            standard in most mathematical texts, and id the same as used in
            OpenGL. It is, however, the opposite of Direct3D, which has
            inexplicably chosen to differ from the accepted standard and uses
            row vectors and left-to-right matrix multiplication.
        @par
            OGRE deals with the differences between D3D and OpenGL etc.
            internally when operating through different render systems. OGRE
            users only need to conform to standard maths conventions, i.e.
            right-to-left matrix multiplication, (OGRE transposes matrices it
            passes to D3D to compensate).
        @par
            The generic form M * V which shows the layout of the matrix 
            entries is shown below:
            <pre>
                [ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
                | m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
                | m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
                [ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
            </pre>
    */
	class _OgreExport Matrix4
	{
	public:
		Matrix4();
		Matrix4( const Matrix4 &mat );
		Matrix4( const float data[] );

		const Matrix4 &operator =(const Matrix4 &mat);
		void operator +=( const Matrix4 &mat );
		void operator -=( const Matrix4 &mat );
		void operator *=( float s );
		void operator /=( float s );
		operator float *();
		void operator *=( const Matrix4 &mat );
		const float *operator[]( size_t irow ) const
		{
			return &m[irow][0];
		}
		float *operator[]( size_t irow )
		{
			return &m[irow][0];
		}

		float det() const;
		void identity();
		void makeTranslateMatrix( const Vector3 &pos );
		void makeScaleMatrix( const Vector3 &scale );
		void makeScaleMatrix(float s)
		{
			makeScaleMatrix(Vector3(s, s, s));
		}

		void makeRotateMatrix( const Quaternion &quat );
		void makeRotateMatrix( const Vector3 &axis, float angle );
		void makeRotateX( float angle );
		void makeRotateY( float angle );
		void makeRotateZ( float angle );

		/**
		* 得到新空间的点转换到原来空间所需要的矩阵
		* \param xaxis,yaxis,zaxis 新空间在原来空间中的三个轴:必须正交
		*/
		void makeRotateMatrix( const Vector3 &xaxis, const Vector3 &yaxis, const Vector3 &zaxis );

		void makeRFTMatrix( const Quaternion &quat, float flipscale, const Vector3 &translate );
		void makeSRTMatrix(const Vector3 &scale, const Quaternion &rot, const Vector3 &translate);
		void makeViewMatrix( const Vector3 &eyeloc, const Vector3 &goalloc, const Vector3 &upvec );
		void makeViewMatrix( const Vector3 &eyeloc, const Quaternion &quat );
		void makeOrthoMatrix( float w, float h, float zn, float zf );
		void makePerspectiveMatrix( float fovy, float aspect, float zn, float zf );
		void makeReflectMatrix( const Vector4 &plane ); //plane = (a, b, c, d)

		bool isOrthonormal() const;

		void inverse( Matrix4 &output ) const;
		void quickInverse( Matrix4 &output ) const; //for simple matrix only
		Matrix4& inverse();
		void quickInverse(); //for simple matrix only
		void transpose();
		void setRow( size_t i, float x, float y, float z, float w )
		{
			m[i][0] = x;
			m[i][1] = y;
			m[i][2] = z;
			m[i][3] = w;
		}
		void setColumn( size_t i, float x, float y, float z, float w )
		{
			m[0][i] = x;
			m[1][i] = y;
			m[2][i] = z;
			m[3][i] = w;
		}
		Vector3 getRow3( size_t i ) const
		{
			return Vector3(m[i][0], m[i][1], m[i][2]);
		}
		Vector3 getColumn3( size_t i ) const
		{
			return Vector3(m[0][i], m[1][i], m[2][i]);
		}
		Vector4 getRow4( size_t i ) const
		{
			return Vector4(m[i][0], m[i][1], m[i][2], m[i][3]);
		}
		Vector4 getColumn4( size_t i ) const
		{
			return Vector4(m[0][i], m[1][i], m[2][i], m[3][i]);
		}
		void setTranslate( float x, float y, float z )
		{
			setRow( 3, x, y, z, 1.0f );
		}
		void setTranslate( const Vector3 &pos )
		{
			setTranslate( pos.x, pos.y, pos.z );
		}
		Vector3 getTranslate() const
		{
			return getRow3(3);
		};
		void getScale(Matrix4& mat) const;

		void getMatrix3( Matrix3 &mat ) const;

		void transformVec4( Vector4 &dest, const Vector4 &src ) const;
		void transformCoord( Vector3 &dest, const Vector3 &src ) const;
		void transformNormal( Vector3 &dest, const Vector3 &src ) const;
		void apply4x4(Vector3 &dest, const Vector3 &src) const;

		Vector3 transformCoord(const Vector3 &pos) const;

	public:
		static const Matrix4 Iden;
		static const Matrix4 Zero;

	public:
		union
		{
			float m[4][4];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
		};
	};


	inline Matrix4 operator *( const Matrix4 &m1, const Matrix4 &m2 )
	{
		Matrix4 retvalue;
		for( int i=0; i<4; i++ )
		{
			for( int j=0; j<4; j++ )
			{
				retvalue.m[i][j] = m1.m[i][0]*m2.m[0][j] + m1.m[i][1]*m2.m[1][j] + m1.m[i][2]*m2.m[2][j] + m1.m[i][3]*m2.m[3][j];
			}
		}
		return retvalue;
	}

	inline Vector3 operator *( const Vector3 &vec, const Matrix4 &mat )
	{
		return Vector3( vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31 + mat._41,
			vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32 + mat._42,
			vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33 + mat._43 );
	}

	inline Vector4 operator *( const Vector4 &vec, const Matrix4 &mat )
	{
		return Vector4( vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31 + vec.w*mat._41,
			vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32 + vec.w*mat._42,
			vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33 + vec.w*mat._43,
			vec.x*mat._14 + vec.y*mat._24 + vec.z*mat._34 + vec.w*mat._44 );
	}

	inline Vector3 Matrix4::transformCoord(const Vector3 &vec) const
	{
		return Vector3( vec.x*_11 + vec.y*_21 + vec.z*_31 + _41,
			vec.x*_12 + vec.y*_22 + vec.z*_32 + _42,
			vec.x*_13 + vec.y*_23 + vec.z*_33 + _43 );
	}

	inline Matrix4 Lerp(const Matrix4 &m1, const Matrix4 &m2, float t)
	{
		Matrix4 ret;
		ret._11 = Lerp(m1._11, m2._11, t);
		ret._12 = Lerp(m1._12, m2._12, t);
		ret._13 = Lerp(m1._13, m2._13, t);
		ret._14 = 0;

		ret._21 = Lerp(m1._21, m2._21, t);
		ret._22 = Lerp(m1._22, m2._22, t);
		ret._23 = Lerp(m1._23, m2._23, t);
		ret._24 = 0;

		ret._31 = Lerp(m1._31, m2._31, t);
		ret._32 = Lerp(m1._32, m2._32, t);
		ret._33 = Lerp(m1._33, m2._33, t);
		ret._34 = 0;

		ret._41 = Lerp(m1._41, m2._41, t);
		ret._42 = Lerp(m1._42, m2._42, t);
		ret._43 = Lerp(m1._43, m2._43, t);
		ret._44 = 1.0f;

		return ret;
	}

	_OgreExport Matrix4 Invert( const Matrix4 &tm );
}

#endif
