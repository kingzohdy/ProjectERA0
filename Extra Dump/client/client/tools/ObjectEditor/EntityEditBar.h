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
	CAnimationEditPane	m_wndAnim;			//�������
	CBoneEditPane		m_wndBone;			//������󶨵����
	CAnchorPane			m_wndAnchor;		
	CMontionEditPane	m_wndMotion;		//��Ч�������
	CItemEditPane		m_wndItem;			//װ���趨���

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnPaint();
};


