
#pragma once
#include <string>
#include "OgrePrerequisites.h"
#include "GameEngineTypes.h"

class CAvatarShowInfo
{
public:
	CAvatarShowInfo();
	~CAvatarShowInfo();

	bool			LoadAvatarConfig();
	std::string		GetModelName(int* attr);

	int				GetParamIDByName(std::string name);
	int				GetParamIDByDesc(std::string desc);
	std::string		GetParamNameByDesc(std::string name);
	std::string		GetParamDescByName(std::string desc);
	std::string		GetParamDesccByID(int id);

	void			SetParamValue(std::string name, std::string value, bool* pNeedReload = NULL);
	void			SetParamValue(int name, std::string value, bool* pNeedReload = NULL);
	void			SetParamValue(std::string name, int value, bool* pNeedReload = NULL);
	void			SetParamValue(int name, int value, bool* pNeedReload = NULL);

	void			SetAllParams(int* attrib, bool bForceReload = false);
	bool			NeedReload();

	void			EnableAvatar(bool bEnable);
	bool			IsEnable();
	void			UpdateActor();


	Ogre::AvatarManager*		m_pAvatarShow;

	bool			m_bEnableAvatar;
	int m_curAttrib[MAX_AVATAR_FIELD];

	int m_lastUpdatedAttr[MAX_AVATAR_FIELD];
};
