
#ifndef __OGREANIMTRACKBONE_H__
#define __OGREANIMTRACKBONE_H__

#include "OgreResource.h"
#include "OgreKeyFrameArray.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

namespace Ogre
{
	class _OgreExport BoneTrack : public Resource
	{
		DECLARE_RTTI(BoneTrack)

	public:
		~BoneTrack(){}
		bool getValue( int iseq, uint tick, Vector3 &translate, Quaternion &rotate, Vector3 &scale, bool loop);

		virtual void _serialize(Archive &ar, int version);

	public:
		FixedString m_BoneName;
		int  m_BoneID;
		KeyFrameArray<Vector3>m_TranslateKeys;
		KeyFrameArray<Quaternion>m_RotateKeys;
		KeyFrameArray<Vector3>m_ScaleKeys;
	};
}

#endif