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
#ifndef __Ray_H_
#define __Ray_H_

#include "OgrePrerequisites.h"
#include "OgreVector3.h"
#include "OgreWorldPos.h"

namespace Ogre
{
	class _OgreExport Ray
	{
	public:
		Ray() : m_Range(MAX_FLOAT){}
		Ray(const Vector3 &origin, const Vector3 &dir, float range=MAX_FLOAT) : m_Origin(origin), m_Dir(dir), m_Range(range)
		{
		}

		bool intersectHorizon( float y, float *pt=NULL) const; //和水平面相交
		bool intersectPlane(const Plane *plane, float *pt=NULL) const;
		bool intersectBox(const BoxBound *pbox, float *pt=NULL) const;
		bool intersectSphere(const SphereBound *psphere, float *pt=NULL) const;
		bool intersectBoxSphere(const BoxSphereBound *pbound, float *pt=NULL ) const;
		// return <=0: no intersect
		bool intersectTriangle( const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, float *pt=NULL ) const;
		bool intersectQuad( const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, float *pt=NULL ) const;
		bool intersectCapsule(const Vector3 &bottom_center, const Vector3 &height, float radius) const;

	public:
		Vector3 m_Origin;
		Vector3 m_Dir;
		float   m_Range;
	};

	class _OgreExport WorldRay
	{
	public:
		WorldRay() : m_Range(MAX_FLOAT){}
		void getRelativeRay(Ray &ray, const WorldPos &pt) const;

	public:
		WorldPos m_Origin;
		Vector3 m_Dir;
		float m_Range;
	};
}
#endif
