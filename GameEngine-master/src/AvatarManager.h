
#ifndef __AvatarManager_H__
#define __AvatarManager_H__

#include "AvatarConfig.h"
#include "OgreSingleton.h"
#include "AvatarTexture.h"

namespace Ogre
{
	class SkinShowBuilder;

	class AvatarManager : public Singleton<AvatarManager>, public LoadWrap
	{
	public:
		AvatarManager();
		~AvatarManager();

		bool init();

		void updateEntityAttrib(Entity *pentity, int *attrib,const int *avatarStar, bool imm = false);
		void cancelEntityAttrib(Entity *pentity);
		Resource *	getModelResouces(const std::string &str);
		FixedString getModelPath(const int *attrib, std::vector<Resource *> &animres);

		int	 GetVarIdByName(const char* szName);
		CTextParser* CreateTextParser(const int *arrib);
		void update(unsigned int dtick);
		bool IsHaveFashionEquip(const int* attrib);
	private:
		void equipItem(Entity* pentity, int itemid, CEquipVarInfo* pPos, AvatarTexturePtr* pCT, int layer, SkinShowBuilder* pSkinView, CTextParser* pParser,int nAvatarStar, bool block = false);
		void bindEquipItem(Entity* pentity, MovableObject *pobject, int anchor, int bindclass, int star, const char *texpath, bool block);
		void asynBindEquipItem(Entity* pentity, int anchor, int bindclass, int star, const char *respath, const char *texpath);
		void ValidateAttrib( int* attrib );
		void ResourceLoaded(Resource *rec, ResourceHandle h);

	public:
		int m_Race;
		int m_Gender;
		int m_Geosets[32]; //Ñ¡ÔñµÄgeoset

		AvatarConfig		m_Config;
		TextureData *m_pDefaultSpecularTex;
		TextureData *m_pUVMaskTex;

		std::map<std::string, Resource *>	m_modelResouces;

		struct EntityEquipInfo
		{
			std::string		texpath;
			int				bindclass;
			int				anchor;
			int				star;
		};
		typedef std::map<ResourceHandle, EntityEquipInfo> EntityEquipInfoList;
		std::map<Entity *, EntityEquipInfoList>	m_lstEquipInfo;
		std::map<ResourceHandle, Entity *> m_hEntityLoads;
	};

	class SkinShowBuilder
	{
	public:
		SkinShowBuilder(Model *pmodel);
		void showSkin(const FixedString &skinname);

	private:
		CSkinGroupInfo* m_pGroupInfo;
		Model *m_pModel;
		std::vector<MeshInstance *>m_showSkin;
	};
}

#endif