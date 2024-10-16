
#ifndef __Bounding_H__
#define __Bounding_H__

#include "OgrePrerequisites.h"
#include "OgreVector3.h"
#include "OgreArchive.h"

namespace Ogre
{
	class _OgreExport BoxBound
	{
	public:
		BoxBound() : m_isValid(false){}
		BoxBound(const Vector3 &minpos, const Vector3 &maxpos) : m_MinPos(minpos), m_MaxPos(maxpos), m_isValid(true){}
		BoxBound(const BoxBound &rhs) : m_MinPos(rhs.m_MinPos), m_MaxPos(rhs.m_MaxPos), m_isValid(rhs.m_isValid){}

		BoxBound& operator+=(const Vector3 &Other)
		{
			if( m_isValid )
			{
#if ASM_x86
				__asm
				{
						mov		eax,[Other]
						mov		ecx,[this]

						movss	xmm3,[eax]Vector3.x
						movss	xmm4,[eax]Vector3.y
						movss	xmm5,[eax]Vector3.z

						movss	xmm0,[ecx]BoxBound.m_MinPos.x
						movss	xmm1,[ecx]BoxBound.m_MinPos.y
						movss	xmm2,[ecx]BoxBound.m_MinPos.z
						minss	xmm0,xmm3
						minss	xmm1,xmm4
						minss	xmm2,xmm5
						movss	[ecx]BoxBound.m_MinPos.x,xmm0
						movss	[ecx]BoxBound.m_MinPos.y,xmm1
						movss	[ecx]BoxBound.m_MinPos.z,xmm2

						movss	xmm0,[ecx]BoxBound.m_MaxPos.x
						movss	xmm1,[ecx]BoxBound.m_MaxPos.y
						movss	xmm2,[ecx]BoxBound.m_MaxPos.z
						maxss	xmm0,xmm3
						maxss	xmm1,xmm4
						maxss	xmm2,xmm5
						movss	[ecx]BoxBound.m_MaxPos.x,xmm0
						movss	[ecx]BoxBound.m_MaxPos.y,xmm1
						movss	[ecx]BoxBound.m_MaxPos.z,xmm2
				}
#else
				m_MinPos.x = Min( m_MinPos.x, Other.x );
				m_MinPos.y = Min( m_MinPos.y, Other.y );
				m_MinPos.z = Min( m_MinPos.z, Other.z );

				m_MaxPos.x = Max( m_MaxPos.x, Other.x );
				m_MaxPos.y = Max( m_MaxPos.y, Other.y );
				m_MaxPos.z = Max( m_MaxPos.z, Other.z );
#endif
			}
			else
			{
				m_MinPos = m_MaxPos = Other;
				m_isValid = true;
			}
			return *this;
		}

		BoxBound& operator+=( const BoxBound& Other )
		{
			if( m_isValid && Other.m_isValid )
			{
#if ASM_X86
				__asm
				{
						mov		eax,[Other]
						mov		ecx,[this]

						movss	xmm0,[ecx]BoxBound.m_MinPos.x
						movss	xmm1,[ecx]BoxBound.m_MinPos.y
						movss	xmm2,[ecx]BoxBound.m_MinPos.z
						minss	xmm0,[eax]BoxBound.m_MinPos.x
						minss	xmm1,[eax]BoxBound.m_MinPos.y
						minss	xmm2,[eax]BoxBound.m_MinPos.z
						movss	[ecx]BoxBound.m_MinPos.x,xmm0
						movss	[ecx]BoxBound.m_MinPos.y,xmm1
						movss	[ecx]BoxBound.m_MinPos.z,xmm2

						movss	xmm0,[ecx]BoxBound.m_MaxPos.x
						movss	xmm1,[ecx]BoxBound.m_MaxPos.y
						movss	xmm2,[ecx]BoxBound.m_MaxPos.z
						maxss	xmm0,[eax]BoxBound.m_MaxPos.x
						maxss	xmm1,[eax]BoxBound.m_MaxPos.y
						maxss	xmm2,[eax]BoxBound.m_MaxPos.z
						movss	[ecx]BoxBound.m_MaxPos.x,xmm0
						movss	[ecx]BoxBound.m_MaxPos.y,xmm1
						movss	[ecx]BoxBound.m_MaxPos.z,xmm2
				}
#else
				m_MinPos.x = Min( m_MinPos.x, Other.m_MinPos.x );
				m_MinPos.y = Min( m_MinPos.y, Other.m_MinPos.y );
				m_MinPos.z = Min( m_MinPos.z, Other.m_MinPos.z );

				m_MaxPos.x = Max( m_MaxPos.x, Other.m_MaxPos.x );
				m_MaxPos.y = Max( m_MaxPos.y, Other.m_MaxPos.y );
				m_MaxPos.z = Max( m_MaxPos.z, Other.m_MaxPos.z );
#endif
			}
			else if( Other.m_isValid )
			{
				*this = Other;
			}
			return *this;
		}

		void setRange( const Vector3 &minpt, const Vector3 &maxpt )
		{
			m_MinPos = minpt;
			m_MaxPos = maxpt;
			m_isValid = true;
		}

		void setCenterExtension(const Vector3 &center, const Vector3 &exten)
		{
			m_MinPos = center-exten;
			m_MaxPos = center+exten;
			m_isValid = true;
		}

		BoxBound transformBy( const Matrix4& M ) const;
		BoxBound transformProjectBy( const Matrix4& ProjM ) const;
		BoxBound expandBy( float w ) const
		{
			return BoxBound( m_MinPos - Vector3(w,w,w), m_MaxPos + Vector3(w,w,w) );
		}

		/**
		* compute the bound volume according to input vertex data
		* pvert[0] = x, pvert[1] = y,  pvert[2] = z
		* vertsize ---- in bytes
		*/
		void setVertexBuffer( const float *vert, size_t vertsize, size_t num );
		Vector3 getCenter() const{ return (m_MinPos + m_MaxPos)*0.5f; }
		Vector3 getExtension() const{ return (m_MaxPos - m_MinPos)*0.5f; }
		const Vector3 &getMinPos() const{ return m_MinPos; }
		const Vector3 &getMaxPos() const{ return m_MaxPos; }

		float getRadiusInDirection( const Vector3 &dir ) const
		{
			Vector3 extent = getExtension();
			return Abs(dir.x*extent.x) + Abs(dir.y*extent.y) + Abs(dir.z*extent.z);
		}

		bool isPointIn(const Vector3& vPos) const
		{
			return (vPos.x>m_MinPos.x && vPos.x<m_MaxPos.x 
				    &&vPos.y>m_MinPos.y && vPos.y<m_MaxPos.y
					&&vPos.z>m_MinPos.z && vPos.z<m_MaxPos.z );
		}

		bool isPointInXZ(const Vector3 &vPos) const
		{
			return (vPos.x>m_MinPos.x && vPos.x<m_MaxPos.x 
				&&vPos.z>m_MinPos.z && vPos.z<m_MaxPos.z );
		}

		bool intersectBoxBound(const BoxBound& other) const
		{
			if( m_MinPos.x > other.m_MaxPos.x || other.m_MinPos.x > m_MaxPos.x )
				return false;
			if( m_MinPos.y > other.m_MaxPos.y || other.m_MinPos.y > m_MaxPos.y )
				return false;
			if( m_MinPos.z > other.m_MaxPos.z || other.m_MinPos.z > m_MaxPos.z )
				return false;
			return true;
		}

		// 检测给定的box是否完全包含在当前BoxBound之内
		bool isBoxBoundIn(const BoxBound& box)
		{
			if( box.m_MaxPos.x > m_MaxPos.x ||
				box.m_MinPos.x < m_MinPos.x )
				return false;
			if( box.m_MaxPos.y > m_MaxPos.y ||
				box.m_MinPos.y < m_MinPos.y )
				return false;
			if( box.m_MaxPos.z > m_MaxPos.z ||
				box.m_MinPos.z < m_MinPos.z )
				return false;

			return true;
		}

		// 检测给定的box是否完全包含在当前BoxBound之外
		bool isBoxBoundOut(const BoxBound& box) const
		{
			Vector3 vDis = Abs(box.getCenter() - getCenter());
			Vector3 vRadius = box.getExtension() + getExtension();

			if( vDis.x > vRadius.x || vDis.y > vRadius.y || vDis.z > vRadius.z )
				return true;

			return false;
		}

		friend Archive & operator<<(Archive &ar, BoxBound &box)
		{
			return ar << box.m_MinPos << box.m_MaxPos << box.m_isValid;
		}

	public:
		Vector3 m_MinPos;
		Vector3 m_MaxPos;
		bool m_isValid;
	};

	class _OgreExport SphereBound
	{
	public:
		SphereBound(){}

		/**
		* compute the bound volume according to input vertex data
		* pvert[0] = x, pvert[1] = y,  pvert[2] = z
		* vertsize ---- in bytes
		*/
		void setVertexBuffer( const float *vert, size_t vertsize, size_t num );
		const Vector3 &getCenter() const{ return m_Center; }
		float   getRadius() const{ return m_Radius; }

		bool isPointIn(const Vector3& vPos) const
		{
			return ( (vPos - m_Center).lengthSqr() < m_Radius * m_Radius );
		}

		friend Archive & operator<<(Archive &ar, SphereBound &sphere)
		{
			ar.serializeRawType(sphere.m_Center);
			ar.serializeRawType(sphere.m_Radius);
			return ar;
		}

	public:
		Vector3 m_Center;
		float m_Radius;
	};

	class _OgreExport BoxSphereBound
	{
	public:
		BoxSphereBound()
		{
		}

		BoxSphereBound(const BoxBound &box)
		{
			fromBoxBound(box);
		}

		void reset()
		{
			m_Center = m_Extent = Vector3(0,0,0);
			m_Radius = 0;
		}

		void fromBoxBound(const BoxBound &box)
		{
			m_Center = box.getCenter();
			m_Extent = box.getExtension();

			m_Radius = m_Extent.length();
		}

		SphereBound getSphere()const
		{
			SphereBound sphere;
			sphere.m_Center = m_Center;
			sphere.m_Radius = m_Radius;
			return sphere;
		}

		BoxBound getBox() const
		{
			BoxBound box;
			box.m_MinPos = m_Center - m_Extent;
			box.m_MaxPos = m_Center + m_Extent;
			box.m_isValid = true;
			return box;
		}

		BoxSphereBound transformBy(const Matrix4 &tm) const;
		
		friend BoxSphereBound legacyUnion(const BoxSphereBound& A,const BoxSphereBound& B)
		{
			BoxBound box;
			box += (A.m_Center - A.m_Extent);
			box += (A.m_Center + A.m_Extent);
			box += (B.m_Center - B.m_Extent);
			box += (B.m_Center + B.m_Extent);

			// Build a bounding sphere from the bounding box's origin and the radii of A and B.
			BoxSphereBound	Result(box);
			Result.m_Radius = Min(Result.m_Radius, Max((A.m_Center - Result.m_Center).length() + A.m_Radius, (B.m_Center - Result.m_Center).length()));

			return Result;
		}

		Vector3 getMin() const
		{
			return m_Center-m_Extent;
		}

		Vector3 getMax() const
		{
			return m_Center+m_Extent;
		}

		const Vector3 &getCenter() const
		{
			return m_Center;
		}

		const Vector3 &getExtension() const
		{
			return m_Extent;
		}

		void fromBox(const Vector3 &minpt, const Vector3 &maxpt)
		{
			m_Center = (minpt + maxpt)/2.0f;
			m_Extent = (maxpt - minpt)/2.0f;
			m_Radius = m_Extent.length();
		}

		bool intersect(const BoxSphereBound &other) const
		{
			BoxBound box1 = getBox();
			BoxBound box2 = other.getBox();
			return box1.intersectBoxBound(box2);
		}

		friend Archive & operator<<(Archive &ar, BoxSphereBound &boxsphere)
		{
			ar.serializeRawType(boxsphere.m_Center);
			ar.serializeRawType(boxsphere.m_Extent);
			ar.serializeRawType(boxsphere.m_Radius);
			return ar;
		}

	public:
		Vector3 m_Center;
		Vector3 m_Extent;
		float m_Radius;
	};
}

#endif