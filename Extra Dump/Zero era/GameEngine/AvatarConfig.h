
#ifndef __AvatarConfig_H__
#define __AvatarConfig_H__

#include "GameEngineTypes.h"
#include "OgreXMLData.h"
#include "OgreFixedString.h"
#include "SmartPtr.h"
#include <map>

namespace Ogre
{
	class CVarInfo
	{
	public:
		//CVarInfo(std::string varName);
		virtual ~CVarInfo() {};
		enum VAR_TYPE
		{
			ENUMVAR,
			EQUIPVAR

		};

		std::string		GetVarName()	{ return m_strName; };
		std::string		GetVarDesc()	{ return m_strDesc; };  
		int				GetVarId()		{ return m_iId; };	

		virtual	VAR_TYPE	GetVarType() = 0;
		virtual bool		ReadFromNode(XMLNode pNode);
		virtual std::string GetValueText(int iValue) = 0;
		virtual std::string GetValueDesc(int iValue) = 0;
		virtual int			GetValueByText(std::string text) = 0;
		virtual int			GetValueByDesc(std::string desc) = 0;

		std::string		m_strName;
		std::string		m_strDesc;
		int				m_iId;
		bool            m_AffectModelPath;
	};
	struct ENUM_ITEM
	{
		int iValue;
		std::string strName;
		std::string strDesc;
	};
	class CEnumVarInfo : public CVarInfo
	{
	public:
		virtual ~CEnumVarInfo() { m_enums.clear(); };
		ENUM_ITEM*	GetEnumItemByName(std::string name);
		ENUM_ITEM*	GetEnumItemByValue(int iValue);
		ENUM_ITEM*	GetEnumItemByDesc(std::string desc);
		virtual	VAR_TYPE	GetVarType() { return ENUMVAR; };


		virtual std::string GetValueText(int iValue) { 
			ENUM_ITEM* Ret =GetEnumItemByValue(iValue);
			if( Ret == NULL )
				return std::string("");
			return Ret->strName;
		};
		virtual std::string GetValueDesc(int iValue) { return GetEnumItemByValue(iValue)->strDesc; };

		virtual int			GetValueByText(std::string text) { return GetEnumItemByName(text)->iValue; };
		virtual int			GetValueByDesc(std::string desc) { return GetEnumItemByDesc(desc)->iValue; };

		bool	ReadFromNode(XMLNode pNode);

		std::vector<ENUM_ITEM> m_enums;
	};

	class CEquipVarInfo : public CVarInfo
	{
	public:
		virtual	VAR_TYPE	GetVarType() { return EQUIPVAR; };
		virtual std::string GetValueText(int iValue);
		virtual std::string GetValueDesc(int iValue);

		virtual int	GetValueByText(std::string text);
		virtual int	GetValueByDesc(std::string desc);

		bool	ReadFromNode(XMLNode pNode);

		std::vector<int> m_supportType;
		int m_iAnchorPos1;
		int m_iAnchorPos2;
		int m_iAnchorPos3;

		int m_iAnchorPos[3];

		char tmp[256];
		//int m_iRelateType
	};

	enum SKINGROUP_TYPE
	{
		SELECT_ONE
	};

	class CSkinGroupInfo
	{
	public:
		struct GROUP
		{
			int id;
			SKINGROUP_TYPE type;
			FixedString groupHeader;
			int coverGroup;
			int defaultskin;
			int texAreaId;
		};

		struct SINGLE_COVER
		{
			FixedString over;
			FixedString under;
		};

		GROUP* GetGroupById(int groupId);
		GROUP* GetGroupBySkinName(const FixedString &skinname);
		bool IsCovering(const FixedString &over, const FixedString &under);

		void ReadFromNode(XMLNode pNode);

		std::vector<GROUP> m_groups;
		std::vector<SINGLE_COVER> m_singlecovers;
	};

	class CModInfo
	{
	public:
		struct TEX_LAYER_INFO
		{
			int iLayer;
			int iPos;
			std::string strPath;
		};
		struct TEX_INFO
		{
			int iArea;
			std::vector<TEX_LAYER_INFO> layers;
			int width;
			int height;
		};
		struct GEOSET_INFO
		{
			std::string strShowSkin;
		};

		std::string					m_strModPath;
		std::vector<std::string>   m_AnimFiles;
		std::vector<TEX_INFO>		m_texInfo;
		std::vector<GEOSET_INFO>	m_geosetInfo;

		void ReadFromNode(XMLNode pNode);
		void ReadTexFromNode(XMLNode pNode, TEX_INFO& texInfo);
		void ReadGeosetFromNode(XMLNode pNode, GEOSET_INFO& geoInfo);
	};

	class CItemTypeInfo
	{
	public:
		struct FIELDINFO
		{
			bool bEnable;
			std::string strPath;
			int iTexLayer;
			int iTexID;
		};
		FIELDINFO		m_FieldEnable[30];		//就算表)展，也应该不会超过20列
		int				m_iTypeId;
		std::string		m_strName;

		bool ReadFromNode(XMLNode pNode);
		bool IsFieldEnable(size_t index);	
		std::string GetFieldPath(size_t index);
		int GetFieldTexLayer(size_t index);

		static int FieldNameToId(std::string fieldName);	
		const static int fieldCount;

	};

	class AvatarConfig
	{
	public:
		AvatarConfig();
		~AvatarConfig();

		bool LoadAvatarConfig();
		bool LoadVariant(XMLNode pNode);
		bool LoadItemTypes(XMLNode pNode);
		bool LoadModInfo(XMLNode pNode);
		bool PreLoadFile(XMLNode pNode);

		CVarInfo* FindVarByName(std::string strName);
		CVarInfo* FindVarById(int id);

		bool IsModVar(int id);
		bool IsNormalVar(int id);

		CItemTypeInfo*		GetItemTypeInfoById(int id);
		
		CModInfo&		getModeInfo( bool bFashion = false );
		CModInfo						m_modelInfo;
		CModInfo						m_fashionModInfo;
		std::vector<CVarInfo*>			m_VarList;
		std::vector<CItemTypeInfo*>		m_itemTypes;
		CSkinGroupInfo					m_skinGroup;
		const static char* filename;
	};


	class CTextParser : public AutoRelease<CTextParser>
	{
	public:
		std::map<std::string, std::string>	m_params;
		void AddParam(std::string name, std::string value);
		void SetParam(std::string name, std::string value);
		std::string GetParam(std::string name);
		std::string ParseString(std::string src);
	};
	typedef SmartPtr<CTextParser>		CTextParserPtr;
}

#endif