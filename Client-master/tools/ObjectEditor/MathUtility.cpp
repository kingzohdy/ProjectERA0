
#include "stdafx.h"
#include "MathUtility.h"
#include "EditorManager.h"

#include "OgreMatrix4.h"
#include "OgreQuaternion.h"
#include "OgreCamera.h"

C3DMath::C3DMath(void)
{
}

C3DMath::~C3DMath(void)
{
}

bool C3DMath::CheckTouchBox(Ogre::Matrix4 tm, Ogre::BoxBound box, bool ext, float scale, float x, float y, float *dept)
{
	Ogre::Camera* pCamera = CEnginAgent::GetInstance()->m_pCamera;
	Ogre::Matrix4 mProj = pCamera->getProjectMatrix();

	Ogre::Vector3 vViewPos;
	//float* row0 = mProj.getRow3(0);
	//float* row1 = mProj[1];
	vViewPos.x = x / mProj.getRow3(0).x;
	vViewPos.y = y / mProj.getRow3(1).y;
	vViewPos.z = 1.0f;


	Ogre::Matrix4 mViewInv = pCamera->getViewMatrix();
	mViewInv.inverse();
	vViewPos = vViewPos * mViewInv;

	Ogre::Vector3 vOrg(0, 0, 0);
	vOrg = vOrg * mViewInv;

	//float dir[] = {	1, 1, 1,   -1, 1, 1,   1,-1, 1,  -1,-1, 1,
	//	1, 1,-1,   -1, 1,-1,   1,-1,-1,  -1,-1,-1};
	float dir[] = {	2, 2, 2,   0, 2, 2,   2,0, 2,  0,0, 2,
		2, 2,0,   0, 2,0,   2,0,0,  0,0,0};
	//if(!ext)
	{
		for(int i = 0; i < 24; i++)
			dir[i] -= 1;
	}
	Ogre::Vector3 vdir[8];//scale = 1.0f;
	for(int i = 0; i < 8; i++)
	{
		vdir[i].x = dir[i*3] * box.getExtension().x;// + box.m_Center.x; // * scale;
		vdir[i].y = dir[i*3+1] * box.getExtension().y;// + box.m_Center.y;// * scale;
		vdir[i].z = dir[i*3+2] * box.getExtension().z;// + box.m_Center.z;// * scale;

		vdir[i] += box.getCenter();
		vdir[i] *= scale;
		vdir[i] = vdir[i] * tm;

	}

	Ogre::RenderLines* pDbg = CEnginAgent::GetInstance()->m_pLineRenderer;

	static int indexes[] = {0,1,2,  1,2,3,  4,5,6,  5,6,7,  0,2,4,  2,4,5, 1,3,6, 3,6,7, 0,1,4, 1,4,6, 2,3,5, 3,5,7};
	for(int i = 0; i < 12; i++)
	{
		float depth = 0.0f;

		//pDbg->RenderLine(vdir[indexes[i*3]], vdir[indexes[i*3+1]], Ogre::ColorQuad(255, 0, 0));
		//pDbg->RenderLine(vdir[indexes[i*3+1]], vdir[indexes[i*3+2]], Ogre::ColorQuad(255, 0, 0));
		//pDbg->RenderLine(vdir[indexes[i*3+2]], vdir[indexes[i*3]], Ogre::ColorQuad(255, 0, 0));

		bool result = C3DMath::Pick(vOrg, vViewPos-vOrg, 
			vdir[indexes[i*3]], vdir[indexes[i*3+1]], vdir[indexes[i*3+2]], 
			&depth);
		if (result) 
		{
			*dept = depth;
			return true;
		}
	}

	return false;
}

bool C3DMath::CheckTouchBox(Ogre::Vector3 centerpos, float x, float y, float range)
{
	Ogre::Camera* pCamera = CEnginAgent::GetInstance()->m_pCamera;
	Ogre::Matrix4 mProj = pCamera->getProjectMatrix();

	Ogre::Vector3 vViewPos;
	//float* row0 = mProj.getRow3(0);
	//float* row1 = mProj[1];
	vViewPos.x = x / mProj.getRow3(0).x;
	vViewPos.y = y / mProj.getRow3(1).y;
	vViewPos.z = 1.0f;


	Ogre::Matrix4 mViewInv = pCamera->getViewMatrix();
	mViewInv.inverse();
	vViewPos = vViewPos * mViewInv;

	Ogre::Vector3 vOrg(0, 0, 0);
	vOrg = vOrg * mViewInv;

	static float dir[] = {	1, 1, 1,   -1, 1, 1,   1,-1, 1,  -1,-1, 1,
		1, 1,-1,   -1, 1,-1,   1,-1,-1,  -1,-1,-1};
	Ogre::Matrix4 mWorld;// = m_pEmitterNode->GetWorldMatrix();
	mWorld.makeTranslateMatrix(centerpos);
	Ogre::Vector3 vdir[8];
	for(int i = 0; i < 8; i++)
	{
		vdir[i].x = dir[i*3] * range;
		vdir[i].y = dir[i*3+1] * range;
		vdir[i].z = dir[i*3+2] * range;
		vdir[i] = vdir[i] * mWorld;//m_mLastMatrix;
		//vdir[i] = dir[i] * m_fSize;
		//vdir[i] = dir[i] * mWorld;
	}

	static int indexes[] = {0,1,2,  1,2,3,  4,5,6,  5,6,7,  0,2,4,  2,4,5, 1,3,6, 3,6,7, 0,1,4, 1,4,6, 2,3,5, 3,5,7};
	for(int i = 0; i < 12; i++)
	{
		float depth = 0.0f;
		bool result = C3DMath::Pick(vOrg, vViewPos-vOrg, 
			vdir[indexes[i*3]], vdir[indexes[i*3+1]], vdir[indexes[i*3+2]], 
			&depth);
		if (result) 
		{
			return true;
		}
	}

	return false;
}

bool C3DMath::Pick(Ogre::Vector3 orig, Ogre::Vector3& dir, Ogre::Vector3 v0, Ogre::Vector3 v1, Ogre::Vector3 v2, float* depth)
{
	float u, v;
	Ogre::Vector3 edge1 = v1 - v0;	//D3DXVECTOR3 edge1 = v1 - v0;
	Ogre::Vector3 edge2 = v2 - v0;	//D3DXVECTOR3 edge2 = v2 - v0;

	Ogre::Vector3 pvec;//D3DXVECTOR3 pvec;
	pvec = Ogre::CrossProduct(dir, edge2);		//D3DXVec3Cross( &pvec, &dir, &edge2 );

	// 如果det为0，或接近于零则射线与三角面共面或平行，不相交
	//此处det就相当于上面的 ，
	float det = Ogre::DotProduct(edge1, pvec);	//FLOAT det = D3DXVec3Dot( &edge1, &pvec );

	Ogre::Vector3 tvec;	//D3DXVECTOR3 tvec;
	if( det > 0 )
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if( det < 0.0001f )
		return false;

	// 计算u并测试是否合法（在三角形内）
	u = Ogre::DotProduct(tvec, pvec);	// *u = D3DXVec3Dot( &tvec, &pvec );
	if( u < 0.0f || u > det )
		return false;

	// Prepare to test V parameter
	Ogre::Vector3 qvec;	//	D3DXVECTOR3 qvec;
	qvec = Ogre::CrossProduct(tvec, edge1);	//D3DXVec3Cross( &qvec, &tvec, &edge1 );

	//计算v并测试是否合法（在三角形内）
	v = Ogre::DotProduct(dir, qvec);		// *v = D3DXVec3Dot( &dir, &qvec );
	if( v < 0.0f || u + v > det )
		return false;

	/*计算t,并把t,u,v放缩为合法值（注意前面的t,v,u不同于算法描述中的相应量，乘了一个系数det）,注意：由于该步运算需要使用除法，所以放到最后来进行，避免不必要的运算，提高算法效率*/
	*depth = Ogre::DotProduct(edge2, qvec);	// *t = D3DXVec3Dot( &edge2, &qvec );
	float fInvDet = 1.0f / det;
	*depth *= fInvDet;
	return TRUE;
}

float C3DMath::QuaternionToAngleX(Ogre::Quaternion& quat)
{
	Ogre::Vector3 idnY(0.0f, 1.0f, 0.0f);

	//angle 1
	Ogre::Matrix4 matRot;
	quat.getMatrix(matRot);
	Ogre::Vector3 ang1 = idnY;
	matRot.transformNormal(ang1, idnY);

	//build quaternion
	Ogre::Quaternion qRef;
	Ogre::Vector3 qRefAix = quat.getAxisX();
	qRef.setAxisAngle(qRefAix, 0.0f);

	//angle 2
	Ogre::Vector3 ang2 = idnY;

	ang2 = Ogre::CrossProduct(ang1, Ogre::Vector3(0.0f, ang1.y, ang1.z));
	//qRef.getMatrix(matRot);
	//matRot.transformNormal(ang2, idnY);
	Ogre::Normalize(ang2);
	float cosAng = Ogre::DotProduct(ang2, ang1);

	return Ogre::Rad2Deg(acos(cosAng));
}

float C3DMath::QuaternionToAngleY(Ogre::Quaternion& quat)
{
	Ogre::Vector3 idnX(1.0f, 0.0f, 0.0f);

	//angle 1
	Ogre::Matrix4 matRot;
	quat.getMatrix(matRot);
	Ogre::Vector3 ang1 = idnX;
	matRot.transformNormal(ang1, idnX);

	//build quaternion
	Ogre::Quaternion qRef;
	Ogre::Vector3 qRefAix = quat.getAxisY();
	qRef.setAxisAngle(qRefAix, 0.0f);

	//angle 2
	Ogre::Vector3 ang2 = idnX;
	qRef.getMatrix(matRot);
	matRot.transformNormal(ang2, idnX);
	ang2 = Ogre::CrossProduct(ang1, Ogre::Vector3(ang1.x, 0.0f, ang1.z));
	Ogre::Normalize(ang2);
	float cosAng = Ogre::DotProduct(ang2, ang1);

	return Ogre::Rad2Deg(acos(cosAng));
}

float C3DMath::QuaternionToAngleZ(Ogre::Quaternion& quat)
{
	Ogre::Vector3 idnY(0.0f, 1.0f, 0.0f);

	//angle 1
	Ogre::Matrix4 matRot;
	quat.getMatrix(matRot);
	Ogre::Vector3 ang1 = idnY;
	matRot.transformNormal(ang1, idnY);

	//build quaternion
	Ogre::Quaternion qRef;
	Ogre::Vector3 qRefAix = quat.getAxisZ();
	qRef.setAxisAngle(qRefAix, 0.0f);

	//angle 2
	Ogre::Vector3 ang2 = idnY;
	qRef.getMatrix(matRot);
	matRot.transformNormal(ang2, idnY);
	ang2 = CrossProduct(ang1, Ogre::Vector3(ang1.x, ang1.y, 0.0f));
	Ogre::Normalize(ang2);
	float cosAng = Ogre::DotProduct(ang2, ang1);

	return Ogre::Rad2Deg(acos(cosAng));
}

Ogre::Quaternion C3DMath::XYZAngleToQuat(float x, float y, float z)
{
	Ogre::Matrix4 mx, my, mz, mat;
	mx.makeRotateX(x);
	my.makeRotateY(y);
	mz.makeRotateZ(z);

	mat = mz * my * mx;

	Ogre::Quaternion ret;
	ret.setMatrix(mat);
	return ret;
}

Ogre::Quaternion C3DMath::XYZAngleToQuat(Ogre::Vector3 angle)
{
	return XYZAngleToQuat(angle.x, angle.y, angle.z);
}

void C3DMath::QuaternionToEular(Ogre::Quaternion& quat, float* yaw, float* pitch, float* roll)
{
	Ogre::Matrix4 mat;
	quat.getMatrix(mat);


	float cg = mat._33;
	float sg = -mat._32;

	if(fabs(sg / cg) < 0.0001)
	{
		*yaw = 0;
	}
	if(cg >= 0.0001)
	{
		*yaw = Ogre::Rad2Deg(atan2(sg , cg));
	}
	else
	{
		if(sg > 0)
			*yaw = 180-Ogre::Rad2Deg(atan2(sg , -cg));
		else
			*yaw = -180-Ogre::Rad2Deg(atan2(sg , -cg));
	}



	// -----------Y---------------------------
	float tmp = sqrt(mat._32 * mat._32 + mat._33 * mat._33);
	*pitch = Ogre::Rad2Deg(asin(mat._31));
	float rowpitch = *pitch;
	bool bTransed = false;

	bool bNeedConvert = false;
	if(fabs(Ogre::Cos(*yaw)) >= 0.5)
	{
		if(Ogre::Cos(*yaw) * mat._33< 0)
			bNeedConvert = true;
	}
	else
	{
		if(-Ogre::Sin(*yaw) * mat._32 < 0)
			bNeedConvert = true;
	}
	if(bNeedConvert == true)
	{
		if(*pitch < 0)
			*pitch = -180 - *pitch;
		else
			*pitch = 180 - *pitch;
		bTransed = true;
		//Ogre::Rad2Deg(atan(mat._31 / tmp));
	}


	// ---------Z-------------

	float ca = mat._11;
	float sa = -mat._21;
	if(Ogre::Cos(*pitch) < 0)
	{
		ca = -ca;
		sa = -sa;
	}

	if(ca >= 0)
	{
		*roll = Ogre::Rad2Deg(atan2( sa , ca));
	}
	else
	{
		if(sa > 0)
			*roll = 180 - Ogre::Rad2Deg(atan2( sa , -ca));
		else
			*roll = -180 - Ogre::Rad2Deg(atan2( sa , -ca));
	}

	if(*roll > -0.001 && *roll < 0.001)
		*roll = 0.0f;

	if(*pitch > -0.001 && *pitch < 0.001)
		*pitch = 0.0f;

	if(*yaw > -0.001 && *yaw < 0.001)
		*yaw = 0.0f;

	//TRACE("x:%g  y:%g  z:%g  , mat_33:%g,  raw y: %g, Ogre::Cos(*yaw):%g, mat._11:%g\n", *yaw, *pitch, *roll, mat._33,  rowpitch, Ogre::Cos(*yaw), mat._11);
}

void  C3DMath::MatrixToAngleXYZ(Ogre::Matrix4& mat, float* x, float* y, float* z)
{
	*x = Ogre::Rad2Deg(atan2(-mat._32 , mat._33));
	*y = Ogre::Rad2Deg(asin(mat._31));//Ogre::Rad2Deg(atan(mat._31 / tmp));
	*z = Ogre::Rad2Deg(atan2( -mat._21 , mat._11));


	if(*z > -0.001 && *z < 0.001)
		*z = 0.0f;

	if(*y > -0.001 && *y < 0.001)
		*y = 0.0f;

	if(*x > -0.001 && *x < 0.001)
		*x = 0.0f;
}

//void C3DMath::QuaternionToEular(Ogre::Quaternion& quat, float* yaw, float* pitch, float* roll)
//{
//	// Extract sin(pitch)
//	float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
//	float sp = -2.0f * (y*z + w*x); 
//
//	if (fabs(sp) > 0.9999f) 
//	{
//		// Looking straight up or down
//
//		//if(roll)
//		//{
//		//	*roll = Ogre::Rad2Deg(1.570796f * sp);
//		//	if(*roll < 0.0001 && *roll > -0.0001) *roll = 0.0f;
//		//}
//
//		//// Compute heading, slam bank to zero
//		//if(yaw)
//		//{
//		//	*yaw = Ogre::Rad2Deg(atan2(-x*z - w*y, 0.5f - y*y - z*z));
//		//	if(*yaw < 0.0001 && *yaw > -0.0001) *yaw = 0.0f;
//		//}
//
//		//if(pitch) *pitch = 0.0f;
//
//		if(pitch)
//		{
//			*pitch = Ogre::Rad2Deg(1.570796f * sp);
//			if(*pitch < 0.0001 && *pitch > -0.0001) *pitch = 0.0f;
//		}
//
//		// Compute heading, slam bank to zero
//		if(yaw)
//		{
//			*yaw = Ogre::Rad2Deg(atan2(-x*z - w*y, 0.5f - y*y - z*z));
//			if(*yaw < 0.0001 && *yaw > -0.0001) *yaw = 0.0f;
//		}
//
//		if(roll) *roll = 0.0f;
//	} 
//	else 
//	{
//
//		//if(roll)
//		//{
//		//	*roll = Ogre::Rad2Deg(asin(sp));
//		//	if(*roll < 0.0001 && *roll > -0.0001) *roll = 0.0f;
//		//}
//		//if(yaw)
//		//{
//		//	*yaw = Ogre::Rad2Deg( atan2(x*z + w*y, 0.5f - x*x - y*y) );
//		//	if(*yaw < 0.0001 && *yaw > -0.0001) *yaw = 0.0f;
//		//}
//		//if(pitch)
//		//{
//		//	*pitch = Ogre::Rad2Deg( atan2(x*y + w*z, 0.5f - x*x - z*z));
//		//	if(*pitch < 0.0001 && *pitch > -0.0001) *pitch = 0.0f;
//		//}
//
//		if(pitch)
//		{
//			*pitch = Ogre::Rad2Deg(asin(sp));
//			if(*pitch < 0.0001 && *pitch > -0.0001) *pitch = 0.0f;
//		}
//		if(yaw)
//		{
//			*yaw = Ogre::Rad2Deg( atan2(x*z + w*y, 0.5f - x*x - y*y) );
//			if(*yaw < 0.0001 && *yaw > -0.0001) *yaw = 0.0f;
//		}
//		if(roll)
//		{
//			*roll = Ogre::Rad2Deg( atan2(x*y + w*z, 0.5f - x*x - z*z));
//			if(*roll < 0.0001 && *roll > -0.0001) *roll = 0.0f;
//		}
//	}
//}