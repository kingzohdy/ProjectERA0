
#ifndef __SkeletonData_H__
#define __SkeletonData_H__

#include "OgreResource.h"
#include "OgreMatrix4.h"
#include <map>

namespace Ogre
{
	class _OgreExport BoneData : public Resource
	{
		DECLARE_RTTI(BoneData)

	public:
		BoneData()
		{
		}

	public:
		FixedString m_Name;
		FixedString m_UserData;
		int         m_ID;
		int         m_FatherID;

		Matrix4		m_OriginTM;

	private:
		virtual void _serialize(Archive &ar, int version);
	};

	class _OgreExport SkeletonData : public Resource
	{
		DECLARE_RTTI(SkeletonData)

	public:
		SkeletonData();

		int findBoneID(const FixedString &name);

		size_t getNumBone(){ return m_BoneTable.size(); }
		BoneData *getIthBone(size_t i){ return m_BoneTable[i]; }
		void addBone(BoneData *pbone);

	protected:
		std::vector<BoneData *>m_BoneTable;
		std::map<FixedString, int>m_BoneNameID;

	protected:
		~SkeletonData();
	private:
		virtual void _serialize(Archive &ar, int version);
	};
}

#endif