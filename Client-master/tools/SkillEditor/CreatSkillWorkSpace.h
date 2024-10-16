#pragma once
#include "GameEngineTypes.h"
#include "SkillTemplateConfig.h"
#include "TypeBind.h"
#include "GameSkill_Common.h"
#include "NotifyProperty.h"

const int MAX_PROPVALUE = 15;

class CCreatSkillWorkSpace: public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CCreatSkillWorkSpace)
public:
	CCreatSkillWorkSpace(void);
	~CCreatSkillWorkSpace(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);

	void UpdateDispProp(DATA_UPDATA_TYPE type);
	void UpdateDispSkillPropValue(DATA_UPDATA_TYPE type);
	void initSkillTemplate();

private:
	void* getAttrib( int skillId, const char* szName );
public:
	CBCGPPropList				m_wndPropList;
	CBCGPProp*					m_pSkillTemplate;
	std::vector<EachItem*>		m_CurSkillItem;

	ParseSTCfg					m_ParseSTCfg;
	CNotifiedProp*				m_pTemplateName;
	CString						m_strTemplateName;
	CString						m_strCurTemplateName;
	
	CNotifiedProp*				m_pSelectSkillId;
	int							m_nSelectSkillId;
	int							m_nCurSelectSkillId;
	CNotifiedProp*				m_pPropValue[MAX_PROPVALUE];
	CString						m_strPropStrValue[MAX_PROPVALUE];

	std::vector<std::string>	m_TemplateName;
};
