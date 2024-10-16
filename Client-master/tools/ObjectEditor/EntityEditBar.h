#pragma once
#include "FixedTabWnd.h"
#include "AnimationEditBar.h"
#include "BoneEditBar.h"
#include "AnchorEditBar.h"
#include "MotionEditBar.h"
#include "ItemEditBar.h"

class CModelEditDlg : public CBCGPDockingControlBar
{
	DECLARE_DYNCREATE(CModelEditDlg)

public:
	CModelEditDlg();
	virtual ~CModelEditDlg();

protected:
	CFixedTabWnd		m_wndTab;
public:
	CAnimationEditPane	m_wndAnim;			//动作面板
	CBoneEditPane		m_wndBone;			//骨骼与绑定点面板
	CAnchorPane			m_wndAnchor;		
	CMontionEditPane	m_wndMotion;		//特效动画面板
	CItemEditPane		m_wndItem;			//装备设定面板

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnPaint();
};


