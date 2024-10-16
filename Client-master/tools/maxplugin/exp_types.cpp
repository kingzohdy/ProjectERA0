
#include "stdafx.h"
#include "exp_types.h"

using namespace Ogre;

namespace DKEXP
{
	DecompMatrix::DecompMatrix() : t(0.0f,0.0f,0.0f), q(0.0f,0.0f,0.0f,1.0f), u(0.0f,0.0f,0.0f,1.0f), k(1.0f,1.0f,1.0f), f(1.0f)
	{
	}

	DecompMatrix & DecompMatrix::operator =( const ::Matrix3 &src )
	{
		AffineParts partobj;
		decomp_affine( src, &partobj );

		t = ConvertPoint3(partobj.t);
		q = ConvertQuat(partobj.q);
		//u = ConvQuaternion(partobj.u);
		k = ConvertPoint3(partobj.k);
		f = partobj.f;

		if(f < 0)
		{
			k *= f;
			f = 1.0f;
		}

		return *this;
	}

	bool DecompMatrix::operator != ( const DecompMatrix &mat )
	{
		if( t==mat.t && q==mat.q && u==mat.u && k==mat.k && f==mat.f ) return false;
		return true;
	}

	Vector3 ConvertPoint3(const Point3 &p3)
	{
		return Vector3(p3.x, p3.y, p3.z);
	}

	Vector2 ConvertPoint2(const Point2 &p2)
	{
		return Vector2(p2.x, p2.y);
	}

	Quaternion ConvertQuat(const Quat q)
	{
		return Quaternion(q.x, q.y, q.z, q.w);
	}

	Matrix4 ConvertMatrix3( ::Matrix3 const &matMax)
	{	
		Matrix4 ret;
		Point3 row;

		row = matMax.GetRow(0);
		ret.setRow(0, row.x, row.y, row.z, 0);
		row = matMax.GetRow(1);
		ret.setRow(1, row.x, row.y, row.z, 0);
		row = matMax.GetRow(2);
		ret.setRow(2, row.x, row.y, row.z, 0);
		row = matMax.GetRow(3);
		ret.setRow(3, row.x, row.y, row.z, 1);

		return ret;
	}

	::Matrix3 Max2EngineMatrix()
	{
		static ::Matrix3 mat;
		static bool init = false;

		if(!init)
		{
			::Matrix3 tm = RotateXMatrix(-DegToRad(90.0f));
			::Matrix3 mirror;
			mirror.IdentityMatrix();
			mirror.SetRow(2, Point3(0.0f, 0.0f, -1.0f));

			mat = tm*mirror;
			init = true;
		}

		return mat;
	}

}