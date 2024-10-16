#pragma once
#include "OgreVector3.h"
#include "TypeBind.h"
#include "Equip.h"
#include "GameSkill_Common.h"

class CNotifiedProp;

class CFightWorkSpace: public CBCGPDockingControlBar
{
public:
	CFightWorkSpace(void);
	~CFightWorkSpace(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);

	void UpdateDispProp(DATA_UPDATA_TYPE type);
	void initSkillAttribPropValue();

	void initMonsterAIPropValue( int SelectId );
public:
	CBCGPPropList	m_wndPropList;
	
	// 场景
	CNotifiedProp*	m_pPropMapId;
	CNotifiedProp*	m_pPropPosX;
	CNotifiedProp*  m_pPropPosY;
	int				m_nMapId;
	int				m_nMapPosX;
	int				m_nMapPosY;

	// 装备/变身
	CNotifiedProp*	m_pPlayerId;
	CNotifiedProp*	m_pModelId;
	CNotifiedProp*	m_pEquipPos;
	CNotifiedProp*  m_pEquipId;
	CString			m_strEquipPos;
	int				m_nEquipId;
	int				m_nModelId;

	// 种怪
	CNotifiedProp*	m_pMonsterId;
	int				m_nMonsterId;

	// 技能快捷键
	CNotifiedProp*	m_pAccelKey[12];
	int				m_skillId[12];

	// 技能服务器数值模拟
	tagSkillDyAttrib m_skillDyAttrib;
	CNotifiedProp*	 m_pSkillDyAttribProp;
	CString			 m_strSkillName;
	int				 m_nSelect;

	// 怪物AI模拟
	CNotifiedProp*	m_pMonsterDyId;
	CNotifiedProp*	m_pMonsterAISelect;
	CNotifiedProp*	m_pMonsterTargetId;
	CNotifiedProp*	m_pMonsterUsedSkill;
	int				m_nMonsterDyId;
	int				m_nMonsterTargetId;
	int				m_nMonsterUsedSkill;
	CString			m_strAISelect;

};	
