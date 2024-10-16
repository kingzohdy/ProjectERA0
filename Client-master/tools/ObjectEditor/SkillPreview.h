
#pragma once
#include "GameResDBTypes.h"
#include "ObjectEditorTypes.h"
#include "GameDBData.h"

#include "OgreVector3.h"

class CSkillPreview
{
public:
	CSkillPreview(void);
	~CSkillPreview(void);

	tagSkillInfo*		GetSkillInfo()		{ return &m_skillInfo;};
	SKILL_STATE		GetState()			{ return  m_currState;};


	void			Init();
	void			Destroy();

	void			PlaySkill();
	void			StopSkill();
	void			DoFrame(float dtime);
	void			UpdateTarget();
	void			ClearSkill();

	void			SaveToDb();
	bool			FindSkill(int id, tagSkillInfo* pSkill);
	bool			UpdateSkillDB();
	bool			AddToSkillDB();

	unsigned int	NewSkillID();
	bool			LoadSkill(int id);

	//

	bool			HasPrepare(SKILL_DISPLAY_TYPE type);
	bool			HasLaunch(SKILL_DISPLAY_TYPE type);
	bool			HasBehit(SKILL_DISPLAY_TYPE type);
	bool			HasFly(SKILL_DISPLAY_TYPE type);

	//
	void UpdateInteract(float dtime);
	bool OnMouseMove(float x, float y);
	bool OnLButtonDown(float x, float y);
	bool OnLButtonUp(float x, float y);

	void ShowCoord(bool bShow);
	bool IsCoordShowing()		{ return m_bCoord; };
	void SetTargetType(SKILL_TARGET_TYPE type);
	SKILL_TARGET_TYPE GetTargetType()		{ return m_targetType; };
	void EnableTarget(bool bEnable);
	bool IsEnableTarget()		{ return m_bEnableTarget; };

	tagSkillInfo		m_skillInfo;
	SKILL_STATE		m_currState;
	SKILL_TARGET_TYPE	m_targetType;

	Ogre::Entity*		m_pFlyEffect;
	Ogre::Entity*		m_pTargetModel;

	CCoord*				m_pCoord;
	COOR_ARROW			m_tranArr;
	bool				m_bCoord;
	bool				m_bEnableTarget;

	CString				m_strTargetName;
	CString				m_strLastTargetName;
	Ogre::Vector3		m_vTargetPos;
	float				m_fDir;
	float			m_fAccTime;
};
