// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "InfoPanel.h"
#include "EditPanel.h"
#include "WorldEditorView.h"
#include "TileTexLayerInfoDlg.h"
#include "ModelInfoDlg.h"
#include "FindDialog.h"

namespace TE
{
	class BeastManager;
}

class CMainFrame : public CBCGPFrameWnd
{
	
public: // 仅从序列化创建
	CMainFrame();
protected:
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	void SetCurOpenFilePath(const char *path);
	void SetRenderInfo(float fps, int num_dp, int num_tri);
	void OnIdle();
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	HWND GetResPreviewWnd();
	HWND GetRadarViewWnd();

public:  // 控件条嵌入成员
	CWorldEditorView m_wndView;
	CBCGPStatusBar  m_wndStatusBar;
	CBCGPToolBar    m_wndToolBar;
	CBCGPMenuBar    m_wndMenuBar;

	CInfoPanel m_InfoPanel;
	CEditPanel m_EditPanel;
	CTileTexLayerInfoDlg m_TexLayerInfoDlg;
	CModelInfoDlg m_ModelInfoDlg;
	CFindDialog m_FindDialog;

	TE::BeastManager *m_pBestManager;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnViewResbrower();
	afx_msg void OnViewEditbar();
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnEditModeSelect();
	afx_msg void OnEditModeMove();
	afx_msg void OnEditModeRotate();
	afx_msg void OnEditModeScale();
	afx_msg void OnEditModeAdd();
	afx_msg void OnUpdateViewResbrower(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewEditbar(CCmdUI *pCmdUI);
	afx_msg void OnExportMaskFile();
	afx_msg void OnCalNaviMap();
	afx_msg void OnCopyWalkToFly();
	afx_msg void OnCalStaticLight();
	afx_msg void OnCalDirLight();
	afx_msg void OnExportObjfile();
	afx_msg void OnPreviewCameraAnim();
	afx_msg void OnViewBackcolor();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFindmodel();
	afx_msg void OnExportObjterrain();
	afx_msg void OnExportObjterrainandwater();
//	afx_msg void OnBeastbake();
	afx_msg void OnShowBeastback();
	afx_msg void OnUpdateShowBeastback(CCmdUI *pCmdUI);
//	afx_msg void OnBeastbake();
	afx_msg void OnBeastbake();
	afx_msg void OnUsecamerarendertarget();
	afx_msg void OnUpdateUsecamerarendertarget(CCmdUI *pCmdUI);
	afx_msg void OnTerrainUseLightmap();
	afx_msg void OnUpdateTerrainUseLightmap(CCmdUI *pCmdUI);
};


