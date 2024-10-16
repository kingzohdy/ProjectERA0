
#ifndef __AvatarEntity_H__
#define __AvatarEntity_H__

#include "GameEngineTypes.h"
#include "AvatarTypes.h"
#include "OgreFixedString.h"
#include "OgreEntity.h"

namespace Ogre
{
	class AvatarManager;
	class AvatarEntity : public Entity
	{
		DECLARE_RTTI(AvatarEntity)

	public:
		AvatarEntity();
		~AvatarEntity();

		void updateAttrib(const int *attrib, const int *avatarStar);
 		void updateAttrib(const int *attrib);
		void updateAttrib();
		void equipItem(int slot, int itemid,int itemStar);
		int  getAvatarAttrib(int index );
		void  setAvatarAttrib(int index, int attr );

	public:
		static AvatarManager* m_pAvatarMgr;

		std::vector<FixedString>m_AvatarMotions;//因为avatar产生的motion
	private:
		int m_AvatarAttrib[MAX_AVATAR_SLOT];
		int m_AvatarStar[MAX_AVATAR_SLOT];	//装备星级

		FixedString m_ModelPath;
	};


}

#endif