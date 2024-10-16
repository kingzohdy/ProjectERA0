
#include "AvatarEntity.h"
#include "OgreResourceManager.h"
#include "AvatarManager.h"
#include "OgreRoot.h"

namespace Ogre
{
	IMPLEMENT_RTTI(AvatarEntity, Entity, 100)
	AvatarManager *AvatarEntity::m_pAvatarMgr = NULL;

	AvatarEntity::AvatarEntity()
	{
		memset(m_AvatarAttrib, 0, sizeof(m_AvatarAttrib));
		memset(m_AvatarStar, 0, sizeof(m_AvatarStar));
	}

	AvatarEntity::~AvatarEntity()
	{
		if (AvatarManager::getSingletonPtr())
		{
			AvatarManager::getSingleton().cancelEntityAttrib(this);
		}
	}

	void AvatarEntity::updateAttrib(const int *attrib, const int* avatarStar)
	{
		std::vector<Resource *> animres;
		FixedString modelpath = m_pAvatarMgr->getModelPath(attrib, animres);
		if(modelpath != m_ModelPath)
		{
			load(modelpath, true);
			m_ModelPath = modelpath;

			if(getMainModel())
			{
				for(size_t i=0; i<animres.size(); i++)
				{
					getMainModel()->AddAnimation(animres[i]);
				}
			}
			playCurAnim();
		}

		memcpy(m_AvatarAttrib, attrib, sizeof(m_AvatarAttrib));
		memcpy(m_AvatarStar, avatarStar, sizeof(m_AvatarStar));

		m_pAvatarMgr->updateEntityAttrib(this, m_AvatarAttrib, m_AvatarStar);
	}

	void AvatarEntity::updateAttrib()
	{
		updateAttrib(m_AvatarAttrib, m_AvatarStar);
	}
	
	void AvatarEntity::updateAttrib(const int *attrib)
	{
		memset(m_AvatarStar, 0, sizeof(m_AvatarStar));
		updateAttrib(m_AvatarAttrib, m_AvatarStar);
	}
	
	void AvatarEntity::equipItem(int slot, int itemid,int itemStar)
	{
		assert(slot>=0 && slot<MAX_AVATAR_SLOT);
		m_AvatarAttrib[slot] = itemid;
		m_AvatarStar[slot] = itemStar;
	}

	int AvatarEntity::getAvatarAttrib(int index )
	{
		return m_AvatarAttrib[index];
	}

	void AvatarEntity::setAvatarAttrib(int index, int attr)
	{
		m_AvatarAttrib[index] = attr;
	}

}
