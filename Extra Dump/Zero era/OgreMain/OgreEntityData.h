
#ifndef __FXModelData_H__
#define __FXModelData_H__

#include "OgreKeyFrameArray.h"
#include "OgreModelData.h"
#include "OgreEntityMotionData.h"

namespace Ogre
{
	class _OgreExport EntityData : public Resource
	{
		DECLARE_RTTI(EntityData)
	public:
		/*
		struct BINDOBJ_T
		{
			int anchorid;
			Vector3 offset; //not used when motion obj
			Quaternion rotate;
			Resource *pres;
			BIND_ANIM_MODE animmode;
			std::string animname;
			Vector3 scale;
		};*/

	public:
		EntityData();

	public:
		Resource *m_pMainResource;
		std::vector<EntityMotionData *>m_Motions;
		std::vector<EntityMotionData *>m_AddtionAnims;

	protected:
		virtual ~EntityData();
		virtual void _serialize(Archive &ar, int version);
	};
}

#endif