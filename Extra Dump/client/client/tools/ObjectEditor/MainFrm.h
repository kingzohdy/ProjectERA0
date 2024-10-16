// MainFrm.h : CMainFrame 类的接口
//


#pragma once

#include "ChildView.h"
#include "SRTEditBar.h"
#include "EntityEditBar.h"
#include "ObjectEditorTypes.h"
#include "EffectUnitListBar.h"
#include "KeyframeEditBar.h"
#include "ResourceBrowseBar.h"
#include "AvatarShowBar.h"
#include "ItemBrowserBar.h"
#include "BoundingEditBar.h"
#include "MotionBrowserBar.h"
#include <vector>

#include "OgreModel.h"
#include "OgreModelData.h"

#include <Windows.h>
#include "Mmsystem.h"
#pragma comment(lib, "winmm.lib")

using namespace Ogre;

class CMainFrame : public CBCGPFrameWnd
{
	
public:
	CMainFrame();

	void DoFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)
	DECLARE_MESSAGE_MAP()

// 属性
public:
	

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void RefreshScene();
	void NewFile();
	void SaveScene();
	void LoadScene(CString name);
	void UpdateEffectList(DWORD mask);
	CEffectEditBar* GetEffectBarByEffect(CEffectUnit* pEffect);

	void ClearEffectBars();
	void ClearModelPrivateEffectBars();
	void UpdateStateText();
	void SetCurOpenFilePath(const char *path);

public:  // 控件条嵌入成员
	CChildView    m_wndView;				// 视图
	CBCGPStatusBar  m_wndStatusBar;
	CBCGPToolBar    m_wndToolBar;
	CBCGPMenuBar	m_wndMenuBar;

	CModelEditDlg	m_EntityEditDlg;		//角色编辑面板(停靠用)
	CEditBar		m_EffectListBar;		//特效编辑
	//CSRTBar			m_SRTBar;				
	CSRTBar			m_wndSrtBar;			//关键帧常规参数面板(旋转 缩放 平移)
	CBrowserBar		m_wndBrowser1;			//资源浏览器
	CAvatarShowBar	m_wndAvatar;			//
	CItemViewerBar	m_wndItemViewer;		//物品窗口

	CBoudingBar		m_wndBoundBar;
	//CMotionBrowserBar m_wndMotionBrowser;	//特效动作信息窗口
	CKeyFrameWnd	m_wndKeyFrame;


	CResDropTarget	m_resDropTarget;
	bool			m_bIsTrans;
	bool			m_bTmpTag;


	std::vector<CEffectEditBar*> m_EffectBars;

	Model			*m_pBKModel;
	CString			m_StrTitle;
	CString			m_OldPath;

// 生成的消息映射函数
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/);
	afx_msg void OnDestroy();
	afx_msg void OnNewFile();
	afx_msg void OnNewEffect();
	void ClearBKModel();
	void BKModelUpdate();

	LRESULT CMainFrame::OnSave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEffectDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApply(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBgcolor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetSpeed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEffectListSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGotoPage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnActorLoaded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddNewEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReqestLoadActor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReqestLoadAvatar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReqestLoadEffect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientCreated(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinishEditItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateAnchorItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateSkillBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAvatarUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRequestKfWnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReleaseKfWnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectMotion(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCurrMotion(WPARAM wParam, LPARAM lParam);


	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLoadEffect();
	afx_msg void OnSaveAsNPC();

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnMnuEditview();
	afx_msg void OnUpdateMnuEditview(CCmdUI *pCmdUI);
	afx_msg void OnMnuModelview();
	afx_msg void OnUpdateMnuModelview(CCmdUI *pCmdUI);

	afx_msg void OnMnuResview();
	afx_msg void OnUpdateMnuResview(CCmdUI *pCmdUI);

	afx_msg void OnMnuAvaview();
	afx_msg void OnUpdateMnuAvaview(CCmdUI *pCmdUI);
	afx_msg void OnMnuItemview();
	afx_msg void OnUpdateMnuItemview(CCmdUI *pCmdUI);

	afx_msg void OnMnuSpeed(UINT id);
	afx_msg void OnUpdateMnuSpeed(CCmdUI* pCmd);
	afx_msg void OnBtnResetView();
	afx_msg void OnLstChangeSpeed();
	afx_msg void OnLstChangeColor();
	afx_msg void OnClose();
	afx_msg void OnFramelimite();
	afx_msg void OnMnuFrameLimite(UINT id);
	afx_msg void OnUpdateMnuFrameLimite(CCmdUI* pUI);




	afx_msg void OnMnuOpenSa();
	afx_msg void OnMnuSaveSa();
	afx_msg void OnBtnSetBackLight();

	afx_msg void OnUndo();
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnRedo();
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);
	afx_msg void OnAccRedo();
	afx_msg void OnAccUndo();
	afx_msg void OnMnuSaveSm();
	afx_msg void OnMnuOpenSm();
	afx_msg void OnLoadBGMesh();
	afx_msg void OnPaint();

	afx_msg void OnShowBouding();
	afx_msg void OnUpdateShowBouding(CCmdUI* pUI);

	afx_msg void OnBtnIncCoord();
	afx_msg void OnBtnDecCoord();

	afx_msg void OnBtnShowGrount();
	afx_msg void OnUpdateBtnShowGround(CCmdUI* pUI);

	afx_msg void OnBtnCoodPos();
	afx_msg void OnUpdateBtnCoodPos(CCmdUI* pUI);

	afx_msg void OnBtnCoodRot();
	afx_msg void OnUpdateBtnCoodRot(CCmdUI* pUI);

	afx_msg void OnBtnCoodScale();
	afx_msg void OnUpdateBtnCoodScale(CCmdUI* pUI);

	afx_msg void OnSetDistort();
	afx_msg void OnUpdateSetDistort(CCmdUI* pUI);

	afx_msg void OnBtnChangeRef();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMnuSrtprop();
	afx_msg void OnUpdateMnuSrtprop(CCmdUI *pCmdUI);

	afx_msg void OnBtnAABB();
	afx_msg void OnUpdateBtnAABB(CCmdUI *pCmdUI);

	afx_msg void OnBackgroundOther();
	afx_msg void OnBackgroundNone();
	afx_msg void OnBtnProj();
	afx_msg void OnUpdateBtnProj(CCmdUI *pCmdUI);
	afx_msg void OnBtnTopview();
	afx_msg void OnBtnOrtho();
	afx_msg void OnUpdateBtnOrtho(CCmdUI *pCmdUI);
	afx_msg void OnBtnLeftview();
	afx_msg void OnBtnFontview();
	afx_msg void OnFortyFiveview();

	afx_msg void OnBtnGlow();
	afx_msg void OnUpdateBtnGlow(CCmdUI *pCmdUI);

	afx_msg void OnActorEditMode(); //编辑模式，可以调整角色的位置和视角
	afx_msg void OnUpdateActorEditMode(CCmdUI* pCmdUI);

	afx_msg void OnHideMainActor();
	afx_msg void OnUpdateHideMainActor(CCmdUI* pCmdUI);
	//在prop中修改了技能的快捷方式
	afx_msg LRESULT OnAddEfxToMotion(WPARAM wParam, LPARAM lParam); //将emo(entity motion)作为Composite层
	afx_msg void OnBackgroundStreet(CCmdUI *pCmdUI);
};


