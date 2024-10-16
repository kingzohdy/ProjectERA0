#pragma once

#include "RollupCtrl.h"
#include "HeightEditDlg.h"
#include "TerrainLayerDlg.h"
#include "WaterEditDlg.h"
#include "ModelEditDlg.h"
#include "LightEditDlg.h"
#include "AreaEditDlg.h"
#include "EnvironmentEditDlg.h"
#include "ProcEditDlg.h"
#include "NpcEditDlg.h"


// CEditPanel

class CEditPanel : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CEditPanel)

public:
	CEditPanel();
	virtual ~CEditPanel();

public:
	CBCGPTabWnd	m_wndTabs;
	CHeightEditDlg m_HeightEditDlg;
	CTerrainLayerDlg m_TerrainLayerDlg;
	CWaterEditDlg m_WaterEditDlg;
	CModelEditDlg m_ModelEditDlg;
	CLightEditDlg m_LightEditDlg;
	CAreaEditDlg m_AreaEditDlg;
	CEnvironmentEditDlg m_EnvEditDlg;
	CProcEditDlg m_ProcEditDlg;
	CRollupCtrl m_AreaEnvRollup;
	CNpcEditDlg m_NpcEditDlg;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


