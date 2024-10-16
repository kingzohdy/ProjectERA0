#pragma warning( disable : 4996 )

#include "AvatarManager.h"
#include "OgreLog.h"
#include "OgreResDB.h"
#include "OgreModel.h"
#include "OgreResourceManager.h"
#include "OgreEntity.h"
#include "OgreTexture.h"
#include "AvatarTypes.h"
#include "AvatarTexture.h"
#include "GameResDBTypes.h"
#include "GameDBData.h"
#include "Item.h"
#include "Equip_Common.h"
#include "OgreRoot.h"

enum
{
	AVATARTEX_NUM = 16
};

namespace Ogre
{
	static void ShowGeoset(const ITEMDISPLAYINFO* pItemDsp, size_t field, SkinShowBuilder* pskinshow, CItemTypeInfo* pTypeInfo)
	{
		if(pTypeInfo->IsFieldEnable(field) == false)
			return;

		const char *pname = NULL;
		switch(field)
		{
		case ItemDisplayDB::GeosetA: pname = pItemDsp->geoset_a; break;
		case ItemDisplayDB::GeosetB: pname = pItemDsp->geoset_b; break;
		case ItemDisplayDB::GeosetC: pname = pItemDsp->geoset_c; break;
		case ItemDisplayDB::GeosetD: pname = pItemDsp->geoset_d; break;
		case ItemDisplayDB::GeosetE: pname = pItemDsp->geoset_e; break;
		}

		if(pname && pname[0])
		{
			pskinshow->showSkin(pname);
		}
	}

	static void GetRealPath(std::string &retpath, CTextParser* pParser, std::string nameInField, std::string pathInTypeInfo)
	{
		pParser->SetParam("field", nameInField);
		retpath = pParser->ParseString(pathInTypeInfo);
		std::string suffix = pParser->GetParam("branch");
		char buffer[1024];
		char *pdot;
		if (suffix != "" && suffix != "0")
		{
			suffix = '_' + suffix;

			std::string str = retpath;
			{
				size_t pos = str.rfind(".");
				if( pos != str.npos )
				{
					str.replace(pos, str.size() - pos, suffix + str.substr(pos, str.size() - pos));
				}
			}
			strncpy(buffer, str.c_str(),1024);
			pdot = strrchr(buffer, '.');
			if(stricmp(pdot, ".omod") == 0)
			{
				strcpy(pdot, ".ent");
				if(ResourceManager::getSingleton().getFileAttrib(buffer) != 0)
				{
					retpath = buffer;
					return;
				}
			}
			if(ResourceManager::getSingleton().getFileAttrib(str.c_str()) != 0)
			{
				retpath = str;
				return;
			}
		}

		strncpy(buffer, retpath.c_str(),1024);
		pdot = strrchr(buffer, '.');
		if(stricmp(pdot, ".omod") == 0)
		{
			strcpy(pdot, ".ent");
			if(ResourceManager::getSingleton().getFileAttrib(buffer) != 0)
			{
				retpath = buffer;
			}
		}
	}


	static void AddItemTextureLayer(AvatarTexturePtr* ct, const ITEMDISPLAYINFO* pItemDsp, size_t field, int region, int layer, int gender, CTextParser* pParser, CItemTypeInfo* pTypeInfo)
	{
		if(!pTypeInfo->IsFieldEnable(field))
		{
			return;
		}

		std::string respath;
		const char* tmp = NULL;
		switch(field)
		{
		case ItemDisplayDB::TexA: tmp = pItemDsp->tex_a;	break;
		case ItemDisplayDB::TexB: tmp = pItemDsp->tex_b;	break;
		case ItemDisplayDB::TexC: tmp = pItemDsp->tex_c;	break;
		case ItemDisplayDB::TexD: tmp = pItemDsp->tex_d;	break;
		case ItemDisplayDB::TexE: tmp = pItemDsp->tex_e;	break;
		case ItemDisplayDB::TexF: tmp = pItemDsp->tex_f;	break;
		case ItemDisplayDB::TexG: tmp = pItemDsp->tex_g;	break;
		case ItemDisplayDB::TexH: tmp = pItemDsp->tex_h;	break;
		default:
			return;
		}

		if(tmp==NULL || tmp[0]==0)
		{
			return;
		}

		GetRealPath(respath, pParser, tmp, pTypeInfo->GetFieldPath(field));
		int texId = pTypeInfo->m_FieldEnable[field].iTexID;
		for(int i = 0; i < AVATARTEX_NUM; i++)
		{
			if(ct[i]->m_TextureID == texId)
			{
				ct[i]->addLayer(respath.data(), region, pTypeInfo->GetFieldTexLayer(field));
				break;
			}
		}
	}

	static std::string getSpecularName(std::string texName)
	{
		std::string strResult(texName);
		size_t pos = strResult.find(".dds");
		if( pos != strResult.npos )
		{
			strResult.replace(pos,strResult.npos,"_s.dds");
			if(ResourceManager::getSingleton().getFileAttrib(strResult.c_str()) != 0)
			{
				return strResult;
			}
		}
		return "";
	}

	static std::string getMaskName(std::string texName)
	{
		std::string strResult(texName);
		size_t pos = strResult.find(".dds");
		if( pos != strResult.npos )
		{
			strResult.replace(pos,strResult.npos,"_m.dds");
			if(ResourceManager::getSingleton().getFileAttrib(strResult.c_str()) != 0)
			{
				return strResult;
			}
		}
		return "";
	}

	AvatarManager::AvatarManager()
	{
	}

	AvatarManager::~AvatarManager()
	{
		OGRE_RELEASE(m_pDefaultSpecularTex);
		OGRE_RELEASE(m_pUVMaskTex);
		for (std::map<std::string, Resource *>::iterator i = m_modelResouces.begin(), j = m_modelResouces.end();
			i != j; ++i)
		{
			OGRE_RELEASE( i->second );
		}
		for (std::map<ResourceHandle, Entity *>::iterator i = m_hEntityLoads.begin();
			i != m_hEntityLoads.end(); ++i)
		{
			breakLoad(i->first);
		}
	}

	bool AvatarManager::init()
	{
		m_Config.LoadAvatarConfig();
		m_pDefaultSpecularTex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad("shaders\\illumspec.dds"));
		m_pUVMaskTex = static_cast<TextureData*>(ResourceManager::getSingleton().blockLoad("character\\mask.dds"));
		return true;
	}

	static Vector3 avatarStarColor[12] = 
	{
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 52/255.0f , 52/255.0f , 52/255.0f ),
		Vector3( 255/255.0f , 255/255.0f , 255/255.0f ),
		Vector3( 224/255.0f , 246/255.0f , 249/255.0f ),
		Vector3( 253/255.0f , 255/255.0f , 193/255.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
		Vector3( 1.0f , 1.0f , 1.0f ),
	};

	void AvatarManager::equipItem(Entity* pentity, int itemid, CEquipVarInfo* pPos, AvatarTexturePtr* pCT, int layer, SkinShowBuilder* pskinshow, CTextParser* pParser, int nAvatarStar, bool block/* = false*/)
	{
		const ITEMDISPLAYINFO &itemdsp = DBDataManager::getSingleton().getItemDisplayInfo(itemid);
		if( &itemdsp == NULL )
		{
			return;
		}

		CItemTypeInfo* pTypeInfo = m_Config.GetItemTypeInfoById(itemdsp.flags);
		if(pTypeInfo == NULL)
		{
			LOG_WARNING("cannot find item typeinfo: id=%d, type=%d", itemid, itemdsp.flags);
			return;
		}

		pParser->AddParam("field", "");	
		std::string respath;
		int bindclass = AVATAR_BIND_CLASS + pPos->GetVarId();

		//Model
		static const size_t modelmodes[] = {
			ItemDisplayDB::Model,
			ItemDisplayDB::Model2,
			ItemDisplayDB::Model3,
		};
		//skin
		static const size_t skinmodes[] = {
			ItemDisplayDB::Skin,
			ItemDisplayDB::Skin2,
			ItemDisplayDB::NoType,
		};
		static const char *modelpaths[3];
		modelpaths[0] = itemdsp.model;
		modelpaths[1] = itemdsp.model2;
		modelpaths[2] = itemdsp.model3;
		static const char *skinpaths[3];
		skinpaths[0] = itemdsp.skin;
		skinpaths[1] = itemdsp.skin2;
		skinpaths[2] = NULL;
		if (block)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				if(pTypeInfo->IsFieldEnable(modelmodes[i]) && modelpaths[i][0] && pPos->m_iAnchorPos[i] > 0)
				{
					GetRealPath(respath, pParser, modelpaths[i], pTypeInfo->GetFieldPath(modelmodes[i]));
					MovableObject *pobject = createObjectFromResource(respath.c_str());
					if (pobject)
					{
						std::string texpath;
						if (skinmodes[i] != ItemDisplayDB::NoType && skinpaths[i] && skinpaths[i][0])
						{
							GetRealPath(texpath, pParser, skinpaths[i], pTypeInfo->GetFieldPath(skinmodes[i]));
						}
						bindEquipItem(pentity, pobject, pPos->m_iAnchorPos[i], bindclass, nAvatarStar, texpath.c_str(), true);
					}
				}
			}
		}
		else
		{
			for (size_t i = 0; i < 3; ++i)
			{
				if(pTypeInfo->IsFieldEnable(modelmodes[i]) && modelpaths[i][0] && pPos->m_iAnchorPos[i] > 0)
				{
					GetRealPath(respath, pParser, modelpaths[i], pTypeInfo->GetFieldPath(modelmodes[i]));
					std::string texpath;
					if (skinmodes[i] != ItemDisplayDB::NoType && skinpaths[i] && skinpaths[i][0])
					{
						GetRealPath(texpath, pParser, skinpaths[i], pTypeInfo->GetFieldPath(skinmodes[i]));
					}
					asynBindEquipItem(pentity, pPos->m_iAnchorPos[i], bindclass, nAvatarStar, respath.c_str(), texpath.c_str());
				}
			}
		}

		ShowGeoset(&itemdsp, ItemDisplayDB::GeosetA, pskinshow, pTypeInfo);
		ShowGeoset(&itemdsp, ItemDisplayDB::GeosetB, pskinshow, pTypeInfo);
		ShowGeoset(&itemdsp, ItemDisplayDB::GeosetC, pskinshow, pTypeInfo);
		ShowGeoset(&itemdsp, ItemDisplayDB::GeosetD, pskinshow, pTypeInfo);
		ShowGeoset(&itemdsp, ItemDisplayDB::GeosetE, pskinshow, pTypeInfo);

		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexA, CR_TEX_A, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexB, CR_TEX_B, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexC, CR_TEX_C, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexD, CR_TEX_D, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexE, CR_TEX_E, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexF, CR_TEX_F, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexG, CR_TEX_G, layer, m_Gender, pParser, pTypeInfo);
		AddItemTextureLayer(pCT, &itemdsp, ItemDisplayDB::TexH, CR_TEX_H, layer, m_Gender, pParser, pTypeInfo);
		
		if( pTypeInfo->IsFieldEnable(ItemDisplayDB::TexA) )
		{
			if( nAvatarStar >=6 )
			{
				if( m_pUVMaskTex != NULL )
				{
					pentity->setLiuGuangTexture( m_pUVMaskTex );
					pentity->enableUVMask( true,false );
					pentity->setUVMaskColor( avatarStarColor[nAvatarStar - 1] );
				}
			}
		}

		// 临时对时装特效的处理
		int attrId = pPos->GetVarId();
		bool bFashion = false;
		const int FASHION_ATTRS[] = 
		{
			POS_ID_FASHION_BODY,
			POS_ID_FASHION_HEAD,
			POS_ID_FASHION_BACK,
			POS_ID_FASHION_HAND,
			POS_ID_FASHION_LEG,
			POS_ID_FASHION_SHOES
		};
		for( int i = 0 ; i < sizeof(FASHION_ATTRS)/sizeof(int) ; ++i )
		{
			if( attrId == FASHION_ATTRS[i] )
			{
				bFashion = true;
				break;
			}
		}
		if( nAvatarStar >= 3 || bFashion )
		{
			std::string visuals;
			std::string suffix = pParser->GetParam("branch");
			if (suffix == "1" && itemdsp.visuals1[0])
			{
				visuals = itemdsp.visuals1;
			}
			else if(itemdsp.visuals[0])
			{
				visuals = itemdsp.visuals;
			}
			if (!visuals.empty())
			{
				std::string gender = pParser->GetParam("gender");
				if (gender == "male")
				{
					char path[100];
					sprintf(path, "effect\\public\\%s_1.emo", visuals.c_str());
					if (ResourceManager::getSingleton().getFileAttrib(path) != 0)
					{
						visuals += "_1";
					}
				}
				pentity->playMotion(visuals.c_str(), false, AVATAR_VISUAL_CLASS);
			}

		}
	}

	Resource * AvatarManager::getModelResouces(const std::string &str)
	{
		std::map<std::string, Resource *>::iterator i = m_modelResouces.find(str);
		if (i == m_modelResouces.end())
		{
			if( ResourceManager::getSingleton().getFileAttrib(str.c_str()) != 0 )
			{
				m_modelResouces[str] = ResourceManager::getSingleton().blockLoad(str.c_str());
				i = m_modelResouces.find(str);
			}else{
				return NULL;
			}
			
		}
		return i->second;
	}

	Ogre::FixedString AvatarManager::getModelPath(const int *attrib, std::vector<Resource *> &animres)
	{
		CTextParser *parser = CreateTextParser(attrib);
		CModInfo& modInfo = m_Config.getModeInfo(IsHaveFashionEquip(attrib));
		std::string str = parser->ParseString(modInfo.m_strModPath);
		FixedString modelpath = str.c_str();
		getModelResouces(str);

		for(size_t i=0; i<modInfo.m_AnimFiles.size(); i++)
		{
			str = parser->ParseString(modInfo.m_AnimFiles[i]);
			Resource *pres = getModelResouces(str);
			if (pres)
			{
				animres.push_back(pres);
			}
		}

		delete parser;
		return modelpath;
	}

	void AvatarManager::ValidateAttrib( int* attrib )
	{
		if( IsHaveFashionEquip(attrib) )
		{
// 			attrib[POS_ID_MASK] = 0;
			attrib[POS_ID_CLOSES] = 0;
			attrib[POS_ID_HAT] = 0;
			attrib[POS_ID_BACK] = 0;
			attrib[POS_ID_SUIT] = 0;
			// 对肩甲特殊处理
 			if(!( attrib[POS_ID_FASHION_HAIR] & 2 ))
			{
				attrib[POS_ID_SHOULD] = 0;
			}
		}
	}

	void AvatarManager::updateEntityAttrib(Entity *pentity, int *attrib,const int *avatarStar, bool imm /*= false*/)
	{
		Model *pmodel = pentity->getMainModel();
		if (pmodel == NULL)
		{
			return;
		}
		ValidateAttrib(attrib);
		size_t i;

		CTextParserPtr pTextParser = CreateTextParser(attrib);
		CModInfo& modInfo = m_Config.getModeInfo(IsHaveFashionEquip(attrib));
		SkinShowBuilder skinshow(pmodel);

		//设置Geoset
		pmodel->showSkins(false);
		for(i = 0; i < modInfo.m_geosetInfo.size(); i++)
		{
			std::string skinName = pTextParser->ParseString(modInfo.m_geosetInfo[i].strShowSkin);
			skinshow.showSkin(skinName.c_str());
		}

		//模型自身贴图
		AvatarTexturePtr replace_tex[AVATARTEX_NUM];
		for (size_t i = 0; i < AVATARTEX_NUM; ++i)
		{
			replace_tex[i] = new AvatarTexture;
		}
		for(i = 0; i < modInfo.m_texInfo.size(); i++)
		{
			assert(i < AVATARTEX_NUM);
			CModInfo::TEX_INFO &texinfo = modInfo.m_texInfo[i];
			AvatarTexture &modTex = *(replace_tex[i]);

			modTex.init(texinfo.iArea, texinfo.width, texinfo.height);

			for(int j = 0; j < (int)texinfo.layers.size(); j++)
			{
				std::string texName = pTextParser->ParseString(texinfo.layers[j].strPath);
				int layer = texinfo.layers[j].iLayer;
				int pos = texinfo.layers[j].iPos;
				modTex.addLayer(texName.c_str(), pos, layer);
			}
		}

		//装备Item
		pentity->unbindRange(AVATAR_BIND_CLASS, AVATAR_BIND_CLASS+MAX_AVATAR_SLOT);
		std::map<Entity *, EntityEquipInfoList>::iterator iter = m_lstEquipInfo.find(pentity);
		if (iter != m_lstEquipInfo.end())
		{
			EntityEquipInfoList &lst = iter->second;
			for (EntityEquipInfoList::iterator i = lst.begin(), j = lst.end(); i != j;)
			{
				EntityEquipInfo &info = i->second;
				if (info.bindclass >= AVATAR_BIND_CLASS && info.bindclass <= AVATAR_BIND_CLASS + MAX_AVATAR_SLOT)
				{
					breakLoad(i->first);
					i = lst.erase(i);
				}
				else
				{
					++i;
				}
			}
			if (lst.empty())
			{
				m_lstEquipInfo.erase(iter);
			}
		}
		pentity->stopMotion(AVATAR_VISUAL_CLASS);
		pentity->enableUVMask(false,false);

		for(i = 0; i<m_Config.m_VarList.size(); i++)
		{
			CVarInfo* pVar = m_Config.m_VarList[i];
			if(pVar->GetVarType() == CVarInfo::EQUIPVAR)
			{
				CEquipVarInfo* pEqpVar = (CEquipVarInfo* )pVar;
				int nAttrID = pEqpVar->GetVarId();
				int itemid = attrib[nAttrID];
				if(itemid <= 0) continue;
				int layer = 10 + pEqpVar->GetVarId(); //装备贴图按前后顺序覆盖，为身体贴图预留10层
				int nStar = avatarStar[nAttrID];
				equipItem(pentity, itemid, pEqpVar, replace_tex, layer, &skinshow, pTextParser, nStar, imm);				
			}
		}

		if (imm)
		{
			for(i = 0; i < modInfo.m_texInfo.size(); i++)
			{
				TextureData * ptex = replace_tex[i]->compose();
				pmodel->setTextureByID(replace_tex[i]->m_TextureID, ptex);
				OGRE_RELEASE(ptex);

				ptex = replace_tex[i]->composeIllumSpecular();
				if(ptex == NULL)
				{
					ptex = m_pDefaultSpecularTex;
					ptex->addRef();
				}
				pmodel->setTextureByID(replace_tex[i]->m_TextureID + 10, ptex);
				ptex->release();

				ptex = replace_tex[i]->composeMask();
				if(ptex == NULL)
				{
					ptex = m_pDefaultSpecularTex;
					ptex->addRef();
				}
				pmodel->setTextureByID(replace_tex[i]->m_TextureID + 110, ptex);
				ptex->release();
			}
		}
		else
		{
			for(i = 0; i < modInfo.m_texInfo.size(); i++)
			{
				pmodel->setTextureByID(replace_tex[i]->m_TextureID, replace_tex[i]->asyncompose());

				FixedString path;

				if (replace_tex[i]->composeIllumSpecular(path))
				{
					pmodel->setTextureByID(replace_tex[i]->m_TextureID + 10, path);
				}
				else
				{
					pmodel->setTextureByID(replace_tex[i]->m_TextureID + 10, m_pDefaultSpecularTex);
				}

				if (replace_tex[i]->composeMask(path))
				{
					pmodel->setTextureByID(replace_tex[i]->m_TextureID + 110, path);
				}
				else
				{
					pmodel->setTextureByID(replace_tex[i]->m_TextureID + 110, m_pDefaultSpecularTex);
				}
			}
		}
		//pentity->playCurAnim();
	}

	void AvatarManager::cancelEntityAttrib(Entity *pentity)
	{
		std::map<Entity *, EntityEquipInfoList>::iterator i = m_lstEquipInfo.find(pentity);
		if (i != m_lstEquipInfo.end())
		{
			for (EntityEquipInfoList::iterator j = i->second.begin(), k = i->second.end(); j != k; ++j)
			{
				ResourceHandle handle = j->first;
				breakLoad(handle);
				std::map<ResourceHandle, Entity *>::iterator it = m_hEntityLoads.find(handle);
				if( it != m_hEntityLoads.end() )
				{
					m_hEntityLoads.erase(it);
				}
			}
			m_lstEquipInfo.erase(i);
		}
	}

	CTextParser* AvatarManager::CreateTextParser(const int *arrib)
	{
		CTextParser* pRet = new CTextParser();
		for(size_t i=0; i<m_Config.m_VarList.size(); i++)
		{
			std::string	paramName	= m_Config.m_VarList[i]->GetVarName();
			int	id					= m_Config.m_VarList[i]->GetVarId();
			std::string	paramValue	= m_Config.m_VarList[i]->GetValueText(arrib[id]);
			pRet->AddParam(paramName, paramValue);
		}

		return pRet;
	}

	int	 AvatarManager::GetVarIdByName(const char* szName)
	{
		CVarInfo* pVar = m_Config.FindVarByName(szName);
		if(pVar == NULL)
			return -1;

		return pVar->m_iId;
	}

	void AvatarManager::update(unsigned int dtick)
	{
	}
	
	static const int s_FashionPos[] = 
	{
		POS_ID_FASHION_BODY,
		POS_ID_FASHION_HEAD,
// 		POS_ID_FASHION_BACK,
		POS_ID_FASHION_HAND,
		POS_ID_FASHION_LEG,
		POS_ID_FASHION_SHOES
	};
	//是否有时装
	bool AvatarManager::IsHaveFashionEquip( const int* attrib )
	{
		for( int i = 0 ; i < sizeof(s_FashionPos)/sizeof(int) ; i++ )
		{
			if( attrib[ s_FashionPos[i] ] > 0 )
			{
				return true;
			}
		}
		if( attrib[POS_ID_FASHION_HAIR] & 1 )
		{
			return true;
		}
		return false;
	}

	void AvatarManager::bindEquipItem(Entity* pentity, MovableObject *pobject, int anchor, int bindclass, int star, const char *texpath, bool block)
	{
		if (texpath[0])
		{
			Model *pmodel = NULL;
			if (IS_KIND_OF(Model, pobject))
				pmodel = static_cast<Model *>(pobject);
			else if (IS_KIND_OF(Entity, pobject))
				pmodel = static_cast<Entity *>(pobject)->getMainModel();

			if(pmodel)
			{
				if (block)
				{
					TextureData *ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(texpath));
					pmodel->setTextureByID(1, ptex);
					if (ptex)
						ptex->release();
				}
				else
				{
					pmodel->setTextureByID(1, texpath);
				}
			}
		}
		pentity->bindObject(anchor, pobject, bindclass);
		if (star >= 6)
		{
			if (m_pUVMaskTex != NULL)
			{
				pobject->setLiuGuangTexture(m_pUVMaskTex);
				pobject->enableUVMask(true, true);
				if (IS_KIND_OF(Model, pobject))
				{
					((Model*)pobject)->setUVMaskColor(avatarStarColor[star - 1]);
				}
			}
		}
		if (!pentity->playBindAnim(bindclass, pentity->getCurAnimID()))
		{
			pentity->playBindAnim(bindclass, 10100);
		}
		pobject->update(0);
		pobject->release();
	}

	void AvatarManager::asynBindEquipItem(Entity* pentity, int anchor, int bindclass, int star, const char *respath, const char *texpath)
	{
		ResourceHandle hLoad = backgroundLoad(respath, ResourceManager::DP_MONSTER_NPC);
		m_hEntityLoads[hLoad] = pentity;
		EntityEquipInfo &info = m_lstEquipInfo[pentity][hLoad];
		info.texpath = texpath;
		info.bindclass = bindclass;
		info.anchor = anchor;
		info.star = star;
	}

	void AvatarManager::ResourceLoaded(Resource *rec, ResourceHandle h)
	{
		std::map<ResourceHandle, Entity *>::iterator i = m_hEntityLoads.find(h);
		if (i != m_hEntityLoads.end())
		{
			Entity *pentity = i->second;
			std::map<Entity *, EntityEquipInfoList>::iterator j = m_lstEquipInfo.find(pentity);
			if( j != m_lstEquipInfo.end() )
			{
				EntityEquipInfoList &lst = j->second;
				EntityEquipInfoList::iterator k = j->second.find(h);
				if(rec && k != lst.end())
				{
					MovableObject *pobject = createObjectFromResource(rec);
					if (pobject)
					{
						EntityEquipInfo &info = k->second;
						bindEquipItem(pentity, pobject, info.anchor, info.bindclass, info.star, info.texpath.c_str(), false);
					}
				}
				if( k != lst.end() )
				{
					lst.erase(k);
				}
				if (lst.empty())
				{
					m_lstEquipInfo.erase(j);
				}
			}
			m_hEntityLoads.erase(i);
		}
	}

	SkinShowBuilder::SkinShowBuilder(Model *pmodel)
	{
		m_pGroupInfo = &AvatarManager::getSingleton().m_Config.m_skinGroup;
		m_pModel = pmodel;
	}

	void SkinShowBuilder::showSkin(const FixedString &skinname)
	{
		std::vector<MeshInstance *>::iterator it = m_showSkin.begin();
		while(it!=m_showSkin.end())
		{
			MeshInstance *pmesh = *it;
			if(m_pGroupInfo->IsCovering(skinname, pmesh->getName()))
			{
				pmesh->show(false);
				it = m_showSkin.erase(it);
			}
			else it++;
		}

		MeshInstance *pmesh = m_pModel->findMesh(skinname);
		if(pmesh)
		{
			pmesh->show(true);
			m_showSkin.push_back(pmesh);
		}
	}
}