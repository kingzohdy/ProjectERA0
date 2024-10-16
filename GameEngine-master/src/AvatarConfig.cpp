
#include "AvatarConfig.h"
#include "AvatarManager.h"

namespace Ogre
{
	const char* AvatarConfig::filename  = "db\\client\\avatarinfo.xml";
	const int CItemTypeInfo::fieldCount = 10;

	//----------class CVarInfo--------------------------------------

	bool CVarInfo::ReadFromNode(XMLNode pNode)
	{
		m_strName = pNode.attribToString("name");
		m_strDesc = pNode.attribToString("desc");
		m_iId	  = pNode.attribToInt("id");
		m_AffectModelPath = pNode.attribToBool("modelpath");
		return true;
	}


	// ---------class CEnumVarInfo----------------------------------- [11/3/2007]

	ENUM_ITEM*	CEnumVarInfo::GetEnumItemByName(std::string name)
	{
		for(int i = 0; i < (int)m_enums.size(); i++)
		{
			if(m_enums[i].strName == name)
				return &m_enums[i];
		}
		return NULL;
	}

	ENUM_ITEM*	CEnumVarInfo::GetEnumItemByValue(int iValue)
	{
		for(int i = 0; i < (int)m_enums.size(); i++)
		{
			if(m_enums[i].iValue == iValue)
				return &m_enums[i];
		}
		return NULL;
	}

	ENUM_ITEM*	CEnumVarInfo::GetEnumItemByDesc(std::string desc)
	{
		for(int i = 0; i < (int)m_enums.size(); i++)
		{
			if(m_enums[i].strDesc == desc)
				return &m_enums[i];
		}
		return NULL;
	}

	bool CEnumVarInfo::ReadFromNode(XMLNode pNode)
	{
		CVarInfo::ReadFromNode(pNode);

		XMLNode pChild = pNode.iterateChild();
		while(!pChild.isNull())
		{
			ENUM_ITEM enumItem;
			enumItem.iValue = pChild.attribToInt("value");
			enumItem.strDesc = pChild.attribToString("desc");
			enumItem.strName = pChild.attribToString("name");

			m_enums.push_back(enumItem);
			pChild = pNode.iterateChild(pChild);
		}

		return true;
	}

	// ---------class CEquipVarInfo--------------------------------- [11/5/2007]
	std::string CEquipVarInfo::GetValueText(int iValue) 
	{
		_itoa_s(iValue, tmp,256, 10);
		return tmp;
	}

	std::string CEquipVarInfo::GetValueDesc(int iValue) 
	{
		_itoa_s(iValue, tmp, 256, 10);
		return tmp;
	}

	int	CEquipVarInfo::GetValueByText(std::string text) 
	{
		return atoi(text.c_str()); 
	}

	int CEquipVarInfo::GetValueByDesc(std::string desc) 
	{
		return atoi(desc.c_str()); 
	}

	bool CEquipVarInfo::ReadFromNode(XMLNode pNode)
	{
		CVarInfo::ReadFromNode(pNode);
		m_iAnchorPos[0] = -1;
		m_iAnchorPos[1] = -1;
		m_iAnchorPos[2] = -1;

		XMLNode pChild = pNode.iterateChild();
		while(!pChild.isNull())
		{
			if(strcmp(pChild.getName(), "itemvalue") == 0)
			{
				int type = 0;
				if(pChild.attribToInt("itemtypeid", type))
					m_supportType.push_back(type);
			}
			else if(strcmp(pChild.getName(), "anchor") == 0)
			{
				if(pChild.hasAttrib("pos1"))
					pChild.attribToInt("pos1", m_iAnchorPos[0]);

				if(pChild.hasAttrib("pos2"))
					pChild.attribToInt("pos2", m_iAnchorPos[1]);

				if(pChild.hasAttrib("pos3"))
					pChild.attribToInt("pos3", m_iAnchorPos[2]);
			}

			pChild = pNode.iterateChild(pChild);
		}


		return true;
	}
	// ---------class CItemTypeInfo----------------------------------

	bool CItemTypeInfo::ReadFromNode(XMLNode pNode)
	{
		pNode.attribToInt("typeid", m_iTypeId);
		m_strName = pNode.attribToString("name");

		for(int i = 0; i < 30; i++)
		{
			m_FieldEnable[i].bEnable = false;
			m_FieldEnable[i].strPath = "";
			m_FieldEnable[i].iTexLayer = 0;
			m_FieldEnable[i].iTexID = 0;
		}

		XMLNode pChild = pNode.iterateChild();
		while(!pChild.isNull())
		{
			std::string fieldName = pChild.attribToString("field");
			int id = FieldNameToId(fieldName);
			if(id != -1)
			{
				m_FieldEnable[id].bEnable = true;
				if(pChild.hasAttrib("path"))
				{
					m_FieldEnable[id].strPath = pChild.attribToString("path");
				}
				if(pChild.hasAttrib("texlayer"))
				{
					pChild.attribToInt("texlayer", m_FieldEnable[id].iTexLayer);
				}
				if(pChild.hasAttrib("texid"))
				{
					pChild.attribToInt("texid", m_FieldEnable[id].iTexID);
				}
				else
				{
					m_FieldEnable[id].iTexLayer = 0;
					m_FieldEnable[id].iTexID = 0;
				}
			}

			pChild = pNode.iterateChild(pChild);
		}

		return true;
	}

	int CItemTypeInfo::GetFieldTexLayer(size_t index)
	{
		if(index >=0 && index < 30)
			return m_FieldEnable[index].iTexLayer;
		return 0;
	}

	bool CItemTypeInfo::IsFieldEnable(size_t index)
	{
		if(index >=0 && index < 30)
			return m_FieldEnable[index].bEnable;
		return false;
	}

	std::string CItemTypeInfo::GetFieldPath(size_t index)
	{
		if(index >=0 && index < 30)
			return m_FieldEnable[index].strPath;
		return "";
	}

	char* g_fieldName[] = 
	{
		"id",
		"model",
		"model2",
		"model3",
		"skin",
		"skin2",
		"icon",
		"geoset_a",
		"geoset_b",
		"geoset_c",
		"geoset_d",
		"geoset_e",
		"flags",
		"tex_a",//"TexArmUpper",
		"tex_b",//"TexArmLower",
		"tex_c",//"TexHands",
		"tex_d",//"TexChestUpper",
		"tex_e",//"TexChestLower",
		"tex_f",//"TexLegUpper",
		"tex_g",//"TexLegLower",
		"tex_h",//"TexFeet",
		"visuals"
	};
	int g_fieldCount = sizeof(g_fieldName)/sizeof(char *);
	int CItemTypeInfo::FieldNameToId(std::string fieldName)
	{
		for(int i = 0; i < g_fieldCount; i++)
		{
			if(fieldName == g_fieldName[i])
				return i;
		}
		return -1;
	}

	// --------class CSkinGroupInfo------------------------------------------------ [11/15/2007]
	void CSkinGroupInfo::ReadFromNode(XMLNode pNode)
	{
		m_groups.clear();

		XMLNode pChild = pNode.iterateChild();
		while(!pChild.isNull())
		{
			if(strcmp(pChild.getName(), "group") == 0)
			{
				GROUP g;
				pChild.attribToInt("id", g.id);
				pChild.attribToInt("default", g.defaultskin);
				pChild.attribToInt("texid", g.texAreaId);
				g.groupHeader = pChild.attribToString("header");

				if(pChild.hasAttrib("cover")) 
					pChild.attribToInt("cover", g.coverGroup);
				else
					g.coverGroup = -1;
				m_groups.push_back(g);
			}
			else if(strcmp(pChild.getName(), "singlecover") == 0)
			{
				SINGLE_COVER sc;
				sc.over = pChild.attribToString("over");
				sc.under = pChild.attribToString("under");
				m_singlecovers.push_back(sc);
			}

			pChild = pNode.iterateChild(pChild);
		}
	}

	CSkinGroupInfo::GROUP* CSkinGroupInfo::GetGroupById(int groupId)
	{
		for(int i = 0; i < (int) m_groups.size(); i++)
		{
			if(m_groups[i].id == groupId)
				return &m_groups[i];
		}
		return NULL;
	}

	CSkinGroupInfo::GROUP* CSkinGroupInfo::GetGroupBySkinName(const FixedString &skinname)
	{
		for(size_t i = 0; i < m_groups.size(); i++)
		{
			size_t headerlen = m_groups[i].groupHeader.length();
			if(m_groups[i].groupHeader == skinname.substr(0, headerlen))
				return &m_groups[i];
		}
		return NULL;
	}

	bool CSkinGroupInfo::IsCovering(const FixedString &over, const FixedString &under)
	{
		for(size_t i=0; i<m_singlecovers.size(); i++)
		{
			if(m_singlecovers[i].over == over && m_singlecovers[i].under == under)
				return true;
		}

		GROUP* pOverGroup = GetGroupBySkinName(over);
		GROUP* pUnderGroup = GetGroupBySkinName(under);

		if(pOverGroup == NULL || pUnderGroup == NULL)
			return false;

		if(pOverGroup == pUnderGroup)
			return true;

		if(pOverGroup->coverGroup == pUnderGroup->id)
			return true;

		return false;
	}

	// --------class CModInfo-------------------------------------------------------- [11/2/2007]
	void CModInfo::ReadFromNode(XMLNode pNode)
	{
		m_strModPath = pNode.attribToString("path");

		XMLNode pChild = pNode.iterateChild();
		while(!pChild.isNull())
		{
			if(strcmp(pChild.getName(), "tex") == 0)
			{
				TEX_INFO ti;
				ReadTexFromNode(pChild, ti);
				m_texInfo.push_back(ti);
			}
			else if(strcmp(pChild.getName(), "geoset") == 0)
			{
				GEOSET_INFO gi;
				gi.strShowSkin = pChild.attribToString("show");
				m_geosetInfo.push_back(gi);
			}
			else if(strcmp(pChild.getName(), "animfile") == 0)
			{
				std::string str = pChild.attribToString("path");
				m_AnimFiles.push_back(str);

				size_t pos = str.find( ".oanim" );
				if( pos != str.npos )
				{
					for( int i = 1 ; i <= 3 ; ++i )
					{
						std::string tmp(str);
						char buf[256];
						sprintf( buf, "%d.oanim",i );
						tmp.replace( pos,strlen(".oanim"),buf );
						m_AnimFiles.push_back( tmp );
					}
				}
			}

			pChild = pNode.iterateChild(pChild);
		}
	}

	void CModInfo::ReadTexFromNode(XMLNode pNode, TEX_INFO& texInfo)
	{
		texInfo.layers.clear();
		texInfo.width = texInfo.height = 0;

		pNode.attribToInt("texid", texInfo.iArea);
		pNode.attribToInt("width", texInfo.width);
		pNode.attribToInt("height", texInfo.height);

		XMLNode pChild = pNode.iterateChild();
		while(!pChild.isNull())
		{
			TEX_LAYER_INFO tli;
			pChild.attribToInt("layer", tli.iLayer);
			pChild.attribToInt("pos", tli.iPos);
			tli.strPath = pChild.attribToString("path");
			texInfo.layers.push_back(tli);

			pChild = pNode.iterateChild(pChild);
		}
	}

	void CModInfo::ReadGeosetFromNode(XMLNode pNode, GEOSET_INFO& geoInfo)
	{
		geoInfo.strShowSkin = pNode.attribToString("show");
	}

	//---------class AvatarConfig--------------------------------------

	AvatarConfig::AvatarConfig(void)
	{
	}

	AvatarConfig::~AvatarConfig(void)
	{
		for(int i = 0; i < (int) m_VarList.size(); i++)
		{
			delete(m_VarList[i]);
		}

		for(int i = 0; i < (int) m_itemTypes.size(); i++)
		{
			delete(m_itemTypes[i]);
		}
	}

	bool AvatarConfig::LoadVariant(XMLNode pNode)
	{
		m_VarList.clear();

		XMLNode pVarNode = pNode.iterateChild();
		while(!pVarNode.isNull())
		{
			if(strcmp(pVarNode.getName(), "var") == 0)
			{
				CVarInfo* pVar = NULL;

				if(strcmp(pVarNode.attribToString("vartype"), "enum") == 0)
					pVar = new CEnumVarInfo();
				else if(strcmp(pVarNode.attribToString("vartype"), "equip") == 0)
					pVar = new CEquipVarInfo();

				if(pVar != NULL)
				{
					pVar->ReadFromNode(pVarNode);
					m_VarList.push_back(pVar);
				}
			}
			else
			{
				assert(0 && "var node != var");
			}

			pVarNode = pNode.iterateChild(pVarNode);
		}

		return true;
	}

	bool AvatarConfig::LoadItemTypes(XMLNode pNode)
	{
		m_itemTypes.clear();

		XMLNode pTypeNode = pNode.iterateChild();
		while(!pTypeNode.isNull())
		{
			assert(strcmp(pTypeNode.getName(), "type") == 0);

			CItemTypeInfo* pTypeInfo = new CItemTypeInfo();
			if(pTypeInfo->ReadFromNode(pTypeNode) == true)
				m_itemTypes.push_back(pTypeInfo);

			pTypeNode = pNode.iterateChild(pTypeNode);
		}

		return true;
	}

	bool AvatarConfig::LoadModInfo(XMLNode pNode)
	{
		if( pNode.hasAttrib("name") && std::string("fashion")==pNode.attribToString("name") )
		{
			m_fashionModInfo.ReadFromNode(pNode);
		}else{
			m_modelInfo.ReadFromNode(pNode);
		}		
		return true;
	}

	bool AvatarConfig::LoadAvatarConfig()
	{
		XMLData configFile;
		if(configFile.loadFile(filename) == false)
			return false;

		XMLNode pRoot = configFile.getRootNode();
		if(strcmp(pRoot.getName(), "avatar") != 0)
			return false;

		XMLNode pConfig = pRoot.iterateChild();
		while(!pConfig.isNull())
		{
			if(strcmp(pConfig.getName(), "variant") == 0)
			{
				if(!LoadVariant(pConfig)) return false;
			}
			else if(strcmp(pConfig.getName(), "modinfo")  == 0)
			{
				if(! LoadModInfo(pConfig)) return false;
			}
			else if(strcmp(pConfig.getName(), "itemtypes")  == 0)
			{
				if(! LoadItemTypes(pConfig)) return false;
			}
			else if(strcmp(pConfig.getName(), "skingroup") == 0)
			{
				m_skinGroup.ReadFromNode(pConfig);
			}
			else if (stricmp(pConfig.getName(), "preload") == 0)
			{
				PreLoadFile(pConfig);
			}
			else
			{
				assert( 0 && "Unknown node in avatar config file at ROOT");
			}

			pConfig = pRoot.iterateChild(pConfig);
		}

		return true;
	}


	CVarInfo* AvatarConfig::FindVarByName(std::string strName)
	{
		for(int i = 0; i < (int) m_VarList.size(); i++)
		{
			if(m_VarList[i]->GetVarName() == strName)
				return m_VarList[i];
		}

		return NULL;
	}

	CVarInfo* AvatarConfig::FindVarById(int id)
	{
		for(int i = 0; i < (int) m_VarList.size(); i++)
		{
			if(m_VarList[i]->GetVarId() == id)
				return m_VarList[i];
		}
		return NULL;
	}

	CItemTypeInfo* AvatarConfig::GetItemTypeInfoById(int id)
	{
		for(int i = 0; i < (int) m_itemTypes.size(); i++)
		{
			if(m_itemTypes[i]->m_iTypeId == id)
				return m_itemTypes[i];
		}
		return NULL;
	}

	bool AvatarConfig::IsModVar(int id)
	{
		for(int i = 0; i < (int) m_VarList.size(); i++)
		{
			if(m_VarList[i]->GetVarId() == id)
				return m_VarList[i]->m_AffectModelPath;
		}
		return false;
	}

	bool AvatarConfig::IsNormalVar(int id)
	{
		return !IsModVar(id);
	}

	CModInfo& AvatarConfig::getModeInfo( bool bFashion )
	{
		if( bFashion )
		{
			return m_fashionModInfo;
		}else{
			return m_modelInfo;
		}
	}

	bool AvatarConfig::PreLoadFile(XMLNode pNode)
	{
		for (XMLNode pChild = pNode.iterateChild(); !pChild.isNull(); pChild = pNode.iterateChild(pChild))
		{
			if (stricmp(pChild.getName(), "path") == 0)
			{
				const char *path = pChild.getText();
				if (path)
				{
					AvatarManager::getSingleton().getModelResouces(path);
				}
				else
				{
					assert( 0 && "Empty node in avatar config file at PreLoad");
				}
			}
			else
			{
				assert( 0 && "Unknown node in avatar config file at PreLoad");
			}
		}
		return true;
	}

	// -----------class CTextParser-------------------------------------- [11/3/2007]

	void CTextParser::AddParam(std::string name, std::string value)
	{
		std::string newname = "$(";
		newname += name;
		newname += ")";
		m_params[newname] = value;
	}

	void CTextParser::SetParam(std::string name, std::string value)
	{
		std::string newname = "$(";
		newname += name;
		newname += ")";
		std::map<std::string, std::string>::iterator it = m_params.find(newname);
		if(it != m_params.end())
		{
			it->second = value;
		}
	}

	std::string CTextParser::GetParam(std::string name)
	{
		std::string newname = "$(";
		newname += name;
		newname += ")";
		std::map<std::string, std::string>::iterator it = m_params.find(newname);
		if(it == m_params.end())
			return "";

		return it->second;
	}

	std::string CTextParser::ParseString(std::string src)
	{
		char txtbuf[512];
		txtbuf[0] = 0;
		int parsed = 0;
		int read = 0;
		int srclen = (int)src.length();
		std::string ret = src;

		for(std::map<std::string, std::string>::iterator i = m_params.begin();
			i != m_params.end(); i++)
		{
			int namelen = (int)i->first.length();
			size_t pos = 0; 

			while( (pos = ret.find(i->first.c_str(), 0)) != std::string::npos )
			{
				ret = ret.replace(pos, namelen, i->second.c_str());
			}
		}

		return ret;
	}
}