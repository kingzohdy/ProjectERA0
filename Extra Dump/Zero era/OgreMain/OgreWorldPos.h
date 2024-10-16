
#ifndef __WorldPos_H__
#define __WorldPos_H__

#include "OgrePrerequisites.h"
#include "OgreVector3.h"
#include "OgreArchive.h"

namespace Ogre
{
	typedef int WPOS_T;
	class _OgreExport WorldPos
	{
	public:
		enum{ UNIT = 10 }; //µ¥Î»: 10/cm
		static WorldPos m_Origin;

		WorldPos()
		{
		}

		WorldPos( WPOS_T _x, WPOS_T _y, WPOS_T _z ) : x(_x), y(_y), z(_z)
		{
		}

		WorldPos( const Vector3 &vec )
		{
			x = (WPOS_T)(vec.x * UNIT);
			y = (WPOS_T)(vec.y * UNIT);
			z = (WPOS_T)(vec.z * UNIT);
		}

		const WorldPos &operator=( const Vector3 &vec )
		{
			x = (WPOS_T)(vec.x * UNIT);
			y = (WPOS_T)(vec.y * UNIT);
			z = (WPOS_T)(vec.z * UNIT);

			return *this;
		}

		const WorldPos &operator+=( const Vector3 &vec )
		{
			x += (WPOS_T)(vec.x * UNIT);
			y += (WPOS_T)(vec.y * UNIT);
			z += (WPOS_T)(vec.z * UNIT);

			return *this;
		}

		const WorldPos &operator+=( const WorldPos &pos )
		{
			x += pos.x;
			y += pos.y;
			z += pos.z;

			return *this;
		}

		const WorldPos &operator-=( const Vector3 &vec )
		{
			x -= (WPOS_T)(vec.x * UNIT);
			y -= (WPOS_T)(vec.y * UNIT);
			z -= (WPOS_T)(vec.z * UNIT);

			return *this;
		}

		const WorldPos &operator-=( const WorldPos &pos )
		{
			x -= pos.x;
			y -= pos.y;
			z -= pos.z;

			return *this;
		}

		WorldPos operator+( const WorldPos &pos ) const
		{
			return WorldPos(x+pos.x, y+pos.y, z+pos.z);
		}

		WorldPos operator-( const WorldPos &pos ) const
		{
			return WorldPos(x-pos.x, y-pos.y, z-pos.z);
		}

		WorldPos operator+( const Vector3 &pos ) const
		{
			return WorldPos(x+(WPOS_T)(pos.x*UNIT), y+(WPOS_T)(pos.y*UNIT), z+(WPOS_T)(pos.z*UNIT));
		}

		WorldPos operator-( const Vector3 &pos ) const
		{
			return WorldPos(x-(WPOS_T)(pos.x*UNIT), y-(WPOS_T)(pos.y*UNIT), z-(WPOS_T)(pos.z*UNIT));
		}

		WorldPos operator*( int s ) const
		{
			return WorldPos((WPOS_T)(x*s), (WPOS_T)(y*s), (WPOS_T)(z*s));
		}

		WorldPos operator*( float s ) const
		{
			return WorldPos((WPOS_T)(x*s), (WPOS_T)(y*s), (WPOS_T)(z*s));
		}

		WorldPos operator/( int s ) const
		{
			return WorldPos((WPOS_T)(x/s), (WPOS_T)(y/s), (WPOS_T)(z/s));
		}

		bool operator==( const WorldPos &rhs ) const
		{
			return x==rhs.x && y==rhs.y && z==rhs.z;
		}

		bool operator!=( const WorldPos &rhs ) const
		{
			return x!=rhs.x || y!=rhs.y || z!=rhs.z;
		}

		void inverse()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		Vector3 normalize() const
		{
			double fx = double(x);
			double fy = double(y);
			double fz = double(z);
			double len = sqrt( fx*fx + fy*fy + fz*fz );

			return Vector3( float(fx/len), float(fy/len), float(fz/len) );
		}

		Vector3 toVector3() const
		{
			return subtract(m_Origin);
		}

		Vector3 subtract( const WorldPos &pos ) const
		{
			return Vector3( float(double(x-pos.x)/UNIT), float(double(y-pos.y)/UNIT), float(double(z-pos.z)/UNIT) );
		}

		static WPOS_T Flt2Fix( float x )
		{
			return (WPOS_T)(x*UNIT);
		}

		static float Fix2Flt( WPOS_T x )
		{
			return float(double(x)/UNIT);
		}

		friend Archive &operator<<(Archive &ar, WorldPos &pos)
		{
			ar.serializeRawType(pos);
			return ar;
		}

	public:
		WPOS_T x, y, z;
	};

	inline Vector3 WorldPosDiff( const WorldPos &p1, const WorldPos &p2 )
	{
		return p1.subtract(p2);
	}

	inline void WorldPosDiff( Vector3 &vec, const WorldPos &p1, const WorldPos &p2 )
	{
		vec = p1.subtract(p2);
	}
}

#endif