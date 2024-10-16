
#include "stdafx.h"
#include "ObjectTypeHelper_Avatar.h"

#include "AvatarConfig.h"
#include "AvatarManager.h"

using namespace Ogre;

unsigned int CAvatarCvtHelper::RaceNameToInt(LPTSTR raceName)
{
	/*	Ogre::AvatarConfig* pAvaReader =&( CEnginAgent::GetInstance()->m_pShowEngine->GetAvatarLoader()->m_reader);
	pAvaReader->m*/
	return 0;
}

char* CAvatarCvtHelper::IntToRaceName(unsigned int raceid)
{
	return "";
}

unsigned int CAvatarCvtHelper::GenderToInt(LPTSTR gender)
{
	return 0;
}

char* CAvatarCvtHelper::IntToGender(unsigned int gender)
{
	return "";
}

unsigned int CAvatarCvtHelper::StringToItemQuality(LPTSTR quality)
{
	CString strQulaty = quality;
	if(strQulaty == _T("粗糙"))		return 0;
	if(strQulaty == _T("普通"))		return 1;
	if(strQulaty == _T("优秀"))		return 2;
	if(strQulaty == _T("精良"))		return 3;
	if(strQulaty == _T("史诗"))		return 4;
	if(strQulaty == _T("传说"))		return 5;
	return 0;
}

char* CAvatarCvtHelper::ItemQualityToString(unsigned int quality)
{
	char* tmp = new char[100];
	tmp[0] = '\0';
	switch(quality)
	{
	case 0:		strcpy(tmp, "粗糙");	break;
	case 1:		strcpy(tmp, "普通");	break;
	case 2:		strcpy(tmp, "优秀");	break;
	case 3:		strcpy(tmp, "精良");	break;
	case 4:		strcpy(tmp, "史诗");	break;
	case 5:		strcpy(tmp, "传说");	break;
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

void CAvatarCvtHelper::EnumQuality(std::vector<char*>& arrQuality)
{
	arrQuality.clear();
	arrQuality.push_back("粗糙");
	arrQuality.push_back("普通");
	arrQuality.push_back("优秀");
	arrQuality.push_back("精良");
	arrQuality.push_back("史诗");
	arrQuality.push_back("传说");
}


COLORREF CAvatarCvtHelper::ItemQualityToColor(unsigned int quality)
{
	switch(quality)
	{
	case 0:		return RGB(127, 127, 127);
	case 1:		return RGB(255, 255, 255);
	case 2:		return RGB( 50, 230,  50);
	case 3:		return RGB( 50,  50, 230);
	case 4:		return RGB(230,  50, 230);
	}
	return RGB(0, 0, 0);
}


int CAvatarCvtHelper::StringToItemType(LPTSTR itemtype)
{
	Ogre::AvatarConfig* pAvaReader =&(Ogre::AvatarManager::getSingleton().m_Config);

	for(int i = 0; i < (int) pAvaReader->m_itemTypes.size(); i++)
	{
		if(pAvaReader->m_itemTypes[i]->m_strName == GbToUtf8(itemtype))
			return pAvaReader->m_itemTypes[i]->m_iTypeId;
	}

	return -1;
}

char* CAvatarCvtHelper::ItemTypeToString(int itemtype)
{
	Ogre::AvatarConfig* pAvaReader =&(Ogre::AvatarManager::getSingleton().m_Config);

	for(int i = 0; i < (int) pAvaReader->m_itemTypes.size(); i++)
	{
		if(pAvaReader->m_itemTypes[i]->m_iTypeId == itemtype)
			return Utf8ToGb((char*)pAvaReader->m_itemTypes[i]->m_strName.data());
	}

	return NULL;
}

void CAvatarCvtHelper::EnumItemType(std::vector<char*>& arrItemType)
{
	arrItemType.clear();
	if(Ogre::AvatarManager::getSingletonPtr() == NULL) return;

	Ogre::AvatarConfig* pAvaReader =&(Ogre::AvatarManager::getSingleton().m_Config);

	for(int i = 0; i < (int) pAvaReader->m_itemTypes.size(); i++)
	{
		arrItemType.push_back((char*)pAvaReader->m_itemTypes[i]->m_strName.data());
	}
}


int CAvatarCvtHelper::StringToSlot(LPTSTR slot)
{
	return 0;
}

int CAvatarCvtHelper::StringToSlot_en(LPTSTR slot)
{
	return 0;
}

char* CAvatarCvtHelper::SlotToString(int slot)
{
	return "";
}

void CAvatarCvtHelper::EnumSlot(std::vector<char*>& arrSlot)
{
}

bool CAvatarCvtHelper::TypeFitSlot(int type, int slot)
{
	Ogre::AvatarConfig* pAvaReader =&(Ogre::AvatarManager::getSingleton().m_Config);

	CVarInfo* pVar = pAvaReader->FindVarById(slot);
	if(pVar == NULL || pVar->GetVarType() != CVarInfo::EQUIPVAR)
		return false;

	CEquipVarInfo* pEquVar = (CEquipVarInfo*) pVar;

	for(int i = 0; i < (int) pEquVar->m_supportType.size(); i++)
	{
		if(pEquVar->m_supportType[i] == type)
			return true;
	}

	return false;
}

int CAvatarCvtHelper::ItemDspNameToId(LPTSTR dspname)
{
	//
	//pItemDB->
	int id = -1;
	int res = sscanf(dspname, "%d", &id);
	if(res > 0)
		return id;
	res = sscanf(dspname, "(%d)", &id);
	if(res > 0)
		return id;

	return -1;
}

char* CAvatarCvtHelper::IdToItemDspNameAsPtr(int id)
{
	assert(0);
	/*
 ItemDB * pItemDB = CAvatarShow::ms_pItemDB;
 const Item_T* pItem = pItemDB->FindItem(id);
 if(pItem == NULL)
 return NULL;

 char* tmp = new char[256];
 const char* name = pItemDB->GetFieldString((size_t)pItem->pname);
 if(name != NULL)
 {
 sprintf(tmp, "(%d) %s", id, name);
 }
 else
 {
 sprintf(tmp, "(%d)", id);
 }

 m_StrBufs.push_back(tmp);
 return tmp;
 */
	return NULL;
}

CString CAvatarCvtHelper::IdToItemDspName(int id)
{
	assert(0);
	/*
	Ogre::ResTable *ptable = Ogre::AvatarManager::getSingleton().m_pItemTable;
	Item_T 
	ptable->FindRecord()

	const Item_T* pItem = pItemDB->FindItem(id);
	if(pItem == NULL)
	return NULL;

	CString ret = "";
	const char* name = pItemDB->GetFieldString((size_t)pItem->pname);
	if(name != NULL)
	{
	ret.Format("(%d) %s", id, name);
	}
	else
	{
	ret.Format("(%d)", id);
	}
	return ret;
	*/
	return "";
}