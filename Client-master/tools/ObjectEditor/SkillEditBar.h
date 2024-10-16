
#pragma once
#include "NormalSplitterWnd.h"
#include "ResourceDropTarget.h"

#include "OgrePrerequisites.h"

class CSkillBar : public CWnd
{
	DECLARE_DYNAMIC(CSkillBar)

public:
	CSkillBar();
	virtual ~CSkillBar();

	CBCGPToolBar	m_wndToolBar;
	CNormalSplitterWnd m_wndSplitter;
	CBCGPPropList	m_wndPropList;
	CBCGPPropList	m_wndPropListTarget;
	CResDropPropListTarget	m_dropTargetTarget;
	CResDropPropListTarget	m_dropTargetSkill;
	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;

	//Skill Param
	CNotifiedProp* m_pPropID;      
	CNotifiedProp* m_pPropSkillName;          
	CNotifiedProp* m_pPropPrepare;
	CNotifiedProp* m_pPropLaunch;
	CNotifiedProp* m_pPropBehit;
	CNotifiedProp* m_pPropFly;
	CNotifiedProp* m_pPropPrepareTime;
	CNotifiedProp* m_pPropHitTime;
	CNotifiedProp* m_pPropSrcAnchor;
	CNotifiedProp* m_pPropDstAnchor;
	CNotifiedProp* m_pPropSpeed;  

	//Target Param
	CNotifiedProp*	m_pPropTargetType;
	CNotifiedProp*	m_pPropTargetName;
	CNotifiedProp*	m_pPropTargetRot;
	CNotifiedProp*	m_pPropTargetPosX;
	CNotifiedProp*	m_pPropTargetPosY;
	CNotifiedProp*	m_pPropTargetPosZ;

	void UpdatePropList();
	void UpdatePropListData();

	void PropUpdate(DATA_UPDATA_TYPE type);

	void AddMotionsListFromDoc(CBCGPProp* pProp);
	void AddMotionsListFromModel(CBCGPProp* pProp, Ogre::Entity* pModel);
	void ClearSkillProp();

	SKILL_DISPLAY_TYPE m_displayType;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickPropBtn(WPARAM w, LPARAM l);

	afx_msg void OnPlay();
	afx_msg void OnShowCoord();
	afx_msg void OnUpdateShowCoord(CCmdUI* pCmdUI);
	afx_msg void OnSelfTarget();
	afx_msg void OnUpdateSelfTarget(CCmdUI* pCmdUI);
	afx_msg void OnEnableTarget();
	afx_msg void OnUpdateEnableTarget(CCmdUI* pCmdUI);
	afx_msg void OnStop();
	afx_msg void OnSave();
	afx_msg void OnLoadSkill();
	afx_msg void OnSkillTypeCmd();

	afx_msg void TestNotify(NMHDR*, LRESULT*);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


