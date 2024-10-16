
#include "stdafx.h"
#include "AvatarShowInfo.h"
#include "AvatarConfig.h"
#include "AvatarManager.h"
#include "ObjectTypeHelper.h"
#include "EffectManager.h"

using namespace Ogre;

CAvatarShowInfo::CAvatarShowInfo(void)
{
	for(int i = 0; i < MAX_AVATAR_FIELD; i++)
		m_curAttrib[i] = 0;
	for(int i = 0; i < MAX_AVATAR_FIELD; i++)
		m_lastUpdatedAttr[i] = 0;
	m_bEnableAvatar = false;
}

CAvatarShowInfo::~CAvatarShowInfo(void)
{
}

int CAvatarShowInfo::GetParamIDByName(std::string name)
{
	Ogre::AvatarConfig* pReader = &m_pAvatarShow->m_Config;
	for(int i = 0; i < (int) pReader->m_VarList.size(); i++)
	{
		if(name == pReader->m_VarList[i]->m_strName)
			return pReader->m_VarList[i]->m_iId;
	}

	return -1;
}

int CAvatarShowInfo::GetParamIDByDesc(std::string desc)
{
	Ogre::AvatarConfig* pReader = &m_pAvatarShow->m_Config;
	for(int i = 0; i < (int) pReader->m_VarList.size(); i++)
	{
		if(desc == pReader->m_VarList[i]->m_strDesc)
			return pReader->m_VarList[i]->m_iId;
	}

	return -1;
}

std::string CAvatarShowInfo::GetParamNameByDesc(std::string name)
{
	return "";
}

std::string CAvatarShowInfo::GetParamDescByName(std::string desc)
{
	return "";
}

std::string CAvatarShowInfo::GetParamDesccByID(int id)
{
	return "";
}

void CAvatarShowInfo::SetParamValue(std::string name, std::string value, bool* pNeedReload)
{
	//int id = GetParamIDByName(name);
	//if(id != -1)
	//{
	//	CVarInfo* pVar = m_pAvatarShow->m_Config.
	//}
}

void CAvatarShowInfo::SetParamValue(int id, std::string value, bool* pNeedReload)
{
	CCvtHelper cvt;

	CVarInfo* pVar = m_pAvatarShow->m_Config.FindVarById(id);
	if(!pVar)
		return;

	std::string utfstr = cvt.GbToUtf8(value.c_str());

	int val = pVar->GetValueByDesc(utfstr);

	SetParamValue(id, val, pNeedReload);
}

void CAvatarShowInfo::SetParamValue(std::string name, int value, bool* pNeedReload)
{
}

void CAvatarShowInfo::UpdateActor()
{
	int avatarStar[MAX_AVATAR_FIELD];
	memset(avatarStar,0,sizeof(avatarStar));
	m_pAvatarShow->updateEntityAttrib(CEffectMgr::GetInstance()->GetModelActor(), m_curAttrib,avatarStar, true);

}

void CAvatarShowInfo::SetParamValue(int id, int value, bool* pNeedReload)
{
	memcpy(m_lastUpdatedAttr, m_curAttrib, sizeof(int) * MAX_AVATAR_FIELD);

	m_curAttrib[id] = value;
	int avatarStar[MAX_AVATAR_FIELD];
	memset(avatarStar,0,sizeof(avatarStar));
	m_pAvatarShow->updateEntityAttrib(CEffectMgr::GetInstance()->GetModelActor(), m_curAttrib,avatarStar, true);
}

void CAvatarShowInfo::SetAllParams(int* attrib,  bool bForceReload)
{
	bool bReload = false;
	memcpy(m_lastUpdatedAttr, m_curAttrib, sizeof(int) * MAX_AVATAR_FIELD);

	for(int i = 0; i < MAX_AVATAR_FIELD; i++)
	{
		if(attrib[i] != m_curAttrib[i])
		{
			if(m_pAvatarShow->m_Config.IsModVar(i))
				bReload = true;
			m_curAttrib[i] = attrib[i];
		}
	}

	if(bReload || bForceReload)
	{
		CEffectMgr::GetInstance()->LoadModelFromAvatar(attrib);
	}
}


void CAvatarShowInfo::EnableAvatar(bool bEnable)
{
	if(bEnable == m_bEnableAvatar)
		return ;
	m_bEnableAvatar = bEnable;
}

bool CAvatarShowInfo::IsEnable()
{
	return m_bEnableAvatar;
}
