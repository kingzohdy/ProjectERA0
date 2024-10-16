
#ifndef __Skeleton_H__
#define __Skeleton_H__

#include "OgrePrerequisites.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreMatrix4.h"

namespace Ogre
{
	class _OgreExport BoneInstance
	{
	public:
		BoneInstance();
		void resetCache();
		void addBlendXform( float weight, const Vector3 &translate, const Quaternion &rotate, const Vector3 &scale );
		void calculateXform();
		int getParentID();

	public:
		BoneData *m_pBoneData;
		BoneInstance *m_pFather;

		Vector3     m_CacheTranslate;
		Quaternion  m_CacheRotate;
		Vector3     m_CacheScale;
		float       m_CacheWeightSum;
		int		m_CurAnimLayer;
		Matrix4 	m_WorldTM;
		bool        m_bCalcTM;
	};

	class _OgreExport SkeletonInstance
	{
	public:
		SkeletonInstance(SkeletonData *pdata);
		~SkeletonInstance();

		void applyAnimation(AnimPlayTrack *ptracks[], size_t count);

		size_t getNumBone()
		{
			return m_Bones.size();
		}

		const Matrix4 &getBoneTM(int boneid)
		{
			assert(boneid>=0 && boneid<(int)(m_Bones.size()));
			return m_Bones[boneid].m_WorldTM;
		}

		Quaternion getBoneRotation(int boneid);

		BoneInstance *getBoneInstance(int boneid)
		{
			assert(boneid>=0 && boneid<(int)(m_Bones.size()));
			return &m_Bones[boneid];
		}

		const FixedString &getBoneName(int boneid);

		int findBoneID(const FixedString &name);

		SkeletonData *getSkeletonData()
		{
			return m_pSkeletonData;
		}

		bool checkSize()
		{
			return m_Bones.size() > 200 ? false : true;
		}

	public:
		SkeletonData *m_pSkeletonData;
		std::vector<BoneInstance>m_Bones;
	};

}

#endif