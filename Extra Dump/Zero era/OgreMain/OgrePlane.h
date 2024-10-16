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
// Original free version by:
// Magic Software, Inc.
// http://www.geometrictools.com/
// Copyright (c) 2000, All Rights Reserved

#ifndef __Plane_H__
#define __Plane_H__

#include "OgrePrerequisites.h"
#include "OgreVector3.h"
#include "OgreWorldPos.h"

namespace Ogre {

    /** Defines a plane in 3D space.
        @remarks
            A plane is defined in 3D space by the equation
            Ax + By + Cz + D = 0
        @par
            This equates to a vector (the normal of the plane, whose x, y
            and z components equate to the coefficients A, B and C
            respectively), and a constant (D) which is the distance along
            the normal you have to go to move the plane back to the origin.
     */
	class _OgreExport Plane
	{
	public:
		enum ESIDE_T
		{
			// Plane front & back must be 0 and 1 cuz I use ^= 1 to switch between them
			FRONT=0,
			BACK=1,
			INTERSECT,
			ON 
			// only used by routines that make sense to return either
			//  INTERSECTING or ON
			// most routines (like PointSide and BoxSide) only return INTERSECTING
			//	Segment and Polygon routines can return INTERSECTING or ON
			//  in which case PLANE_ON means coplanar-with.
		};

	public:
		Plane()
		{
			//assert(0);
		}

		Plane( const Vector3 &normal, const Vector3 &point )
		{
			setFromNormalAndPoint(normal,point);
		}

		Plane(const Vector3 & normal,const float offset) :m_normal(normal), m_offset(offset)
		{
			assert( isValid() );
		}

		// default operator = and copy constructor are fine

		void mirrorPoint( Vector3 &dest, const Vector3 &src );
		void mirrorVector( Vector3 &dest, const Vector3 &src );

		bool isValid() const
		{
			assert( IsFloatValid(m_offset) );
			return true;
		}

		///////////////////////////////////////////////////////
		// basic Set's
		void setFromPlaneParam( const float plane[4] )
		{
			float len = (float)Sqrt( plane[0]*plane[0] + plane[1]*plane[1] + plane[2]*plane[2] );
			m_normal.x = plane[0] / len;
			m_normal.y = plane[1] / len;
			m_normal.z = plane[2] / len;
			m_offset   = plane[3] / len;
		}

		void setFromNormalAndPoint(const Vector3 & normal,const Vector3 & point)
		{
			m_normal = normal;
			m_offset = - DotProduct( m_normal, point );

			assert(isValid());
		}

		float setFromThreePoints(const Vector3 & point1,const Vector3 & point2,const Vector3 & point3);

		///////////////////////////////////////////////////////
		// Point-Plane tests 

		float distanceToPoint(const Vector3 & point) const
		{
			assert(isValid());
			return DotProduct(m_normal, point) + m_offset;
		}

		ESIDE_T pointSideGE(const Vector3 & point) const
		{
			assert(isValid());
			const float d = distanceToPoint(point);
			return ( d >= 0.f ) ? FRONT : BACK;
		}

		ESIDE_T pointSideOrOn(const Vector3 & point,const float rOnTolerance = EPSILON) const
		{
			assert(isValid());
			const float d = distanceToPoint(point);
			if ( d > rOnTolerance )
				return FRONT;
			if ( d <-rOnTolerance )
				return BACK;
			return INTERSECT;
		}

		ESIDE_T boxSide(const BoxBound &box) const;
		ESIDE_T sphereSide(const SphereBound &sphere) const;
		ESIDE_T	boxSphereBoundSide(const BoxSphereBound &bound) const;

		///////////////////////////////////////////////////////
		// modifications :

		//need the inverse transpose of the matrix
		void transformIT( const Matrix4 & xform );
		void transform( const Matrix4 & xform );
		void normalize();

		void rotate( const Matrix4 &mat );

		void translate(const Vector3 & v)
		{
			assert(isValid());
			// slide the offset
			m_offset -= DotProduct( v, m_normal );
		}

		// moves the plane by Delta along its normal
		// if you do MoveForwards( DistanceToPoint( vert ) );
		// then you will be on vert
		void moveForwards(const float rDelta)
		{
			assert(isValid());
			m_offset -= rDelta;
		}

		void flipNormal(void)
		{
			assert(isValid());
			m_normal *= -1.0f;
			m_offset *= -1;
		}

		float solveForX( float y, float z ) const
		{
			if( m_normal.x ) return -(m_normal.y*y + m_normal.z*z + m_offset)/m_normal.x;
			else
			{
				assert(0);
				return 0;
			}
		}
		float solveForY( float x, float z ) const
		{
			if( m_normal.y ) return -(m_normal.x*x + m_normal.z*z + m_offset)/m_normal.y;
			else
			{
				assert(0);
				return 0;
			}
		}
		float solveForZ( float x, float y ) const
		{
			if( m_normal.z ) return -(m_normal.x*x + m_normal.y*y + m_offset)/m_normal.z;
			else
			{
				assert(0);
				return 0;
			}
		}
		///////////////////////////////////////////////////////
		// seg tests :
		bool segmentIntersec(Vector3& vOut, const Vector3&vP0, const Vector3& vP1) const
		{
			float fDis0 = distanceToPoint(vP0);
			float fDis1 = distanceToPoint(vP1);
			if( fDis0 * fDis1 >= 0 )
				return false;

			vOut = vP0 + (vP1 - vP0) * fDis0 / (fDis0 - fDis1);
			return true;
		}

		ESIDE_T segmentSide(const Vector3 & fm,const Vector3 & to, const float radius = 0.f) const
		{
			assert(isValid());

			const float dFm = distanceToPoint(fm);
			const float dTo = distanceToPoint(to);

			if ( fabsf(dFm) <= EPSILON && fabsf(dTo) <= EPSILON )
			{
				return ON;
			}
			else if ( dFm < -radius && dTo < -radius )
			{
				return BACK;
			}
			else if ( dFm >  radius && dTo >  radius )
			{
				return FRONT;
			}
			else
			{
				return INTERSECT;
			}
		}

		///////////////////////////////////////////////////////
		// Get members :

		const Vector4 & getVec4(void) const
		{ return *((Vector4 *)&m_normal); }

		const Vector3 & getNormal(void) const
		{ return m_normal; }

		const float getOffset() const
		{ return(m_offset); }



	protected:
		Vector3	m_normal;
		float m_offset;
	};

	class _OgreExport WorldPlane
	{
	public:
		void relativePlane( Plane &dest, const WorldPos &origin ) const;
		float distanceToPoint(const WorldPos & pt) const;
		void mirrorPoint( WorldPos &dest, const WorldPos &src ) const;
		void mirrorVector( Vector3 &dest, const Vector3 &src ) const;

	public:
		Vector3 m_Normal;
		WorldPos m_Point; //point on plane
	};

}

#endif
