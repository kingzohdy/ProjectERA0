
#pragma once
#include "OgreVector3.h"

class C3DMath
{
public:
	C3DMath(void);
	~C3DMath(void);

	static bool Pick(Ogre::Vector3 orig, Ogre::Vector3& dir, Ogre::Vector3 v0, Ogre::Vector3 v1, Ogre::Vector3 v2, float* depth);
	static bool CheckTouchBox(Ogre::Vector3 centerpos, float x, float y, float range);
	static bool C3DMath::CheckTouchBox(Ogre::Matrix4 tm, Ogre::BoxBound box, bool ext,float scale, float x, float y, float *dept);
	static void QuaternionToEular(Ogre::Quaternion& quat, float* yaw, float* pitch, float* roll);
	static void MatrixToAngleXYZ(Ogre::Matrix4& quat, float* x, float* y, float* z);

	static float C3DMath::QuaternionToAngleX(Ogre::Quaternion& quat);
	static float C3DMath::QuaternionToAngleY(Ogre::Quaternion& quat);
	static float C3DMath::QuaternionToAngleZ(Ogre::Quaternion& quat);

	static Ogre::Quaternion XYZAngleToQuat(float x, float y, float z);
	static Ogre::Quaternion XYZAngleToQuat(Ogre::Vector3 angle);
};
