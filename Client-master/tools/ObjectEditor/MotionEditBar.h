
#pragma once
#include "ObjectEditorTypes.h"
#include "NormalSplitterWnd.h"
#include "ResourceDropTarget.h"
#include "ExtListCtrl.h"
#include "TimeBar.h"
#include "ModelMotionInfo.h"

struct TIME_AIX_LEVEL
{
	float m_fUnitValue;
	int m_iExactInStep;
	int m_iStepPixes;
	int m_iSeperate;
	bool m_bShowGrid;
};

struct LayerCreateParam
{
	Ogre::MOTIONOBJTYPE_T	m_type;
	Ogre::POSTEFFECT_T		m_postEffect;
	float					m_fStart;
	float					m_fEnd;
	BOOL					m_bNoStart;
	BOOL					m_bNoEnd;

	LayerCreateParam() : m_type(Ogre::MOT_POSTEFFECT), m_postEffect(Ogre::PE_FSEFFECT), m_fStart(0), m_fEnd(2.0f), m_bNoStart(FALSE), m_bNoEnd(FALSE)
	{
	}
};
// CMontionEditPane

class CMontionEditPane : public CWnd
{
	DECLARE_DYNAMIC(CMontionEditPane)

public:
	CMontionEditPane();
	virtual ~CMontionEditPane();

protected:

	CBCGPToolBar	m_wndToolBar;
	CNormalSplitterWnd m_wndSplitter;	// 切割窗口 
	CListCtrlExt		m_wndList;

	//CTimeBarList	m_wndTimeBarList;
	CTimeAixes		m_wndTimeAixes;		

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;

	int	m_iCurrAixLevel;
	CResDropTarget		m_dropTarget;

	int m_iMontionSetLod; //要设置Lod的montion
	AIXES_LOD_SHOW_MODEL m_eShowLodState;  //TimeAxies中触发的H、M、L

	bool m_CreateLayerByDlg; //通过对话框来创建层， 否则通过下面的参数来创建
	LayerCreateParam m_CreateLayerParam;


	DECLARE_MESSAGE_MAP()

public:
	void Clear();
	void UpdateList();
	void UpdateTimeBarList(CModelMotionInfo::MOTION* pMotion);
	void UpdateTimeBarList();
	void DoFrame(float dtime);
	void SetTimeAixLevel(int level);

	void UpdateMontionLod(int level);
	void SortObjLayer(CModelMotionInfo::MOTION* pMotion); //将对象层按照：后期效果、动画层、特效层排序
	bool WhetherShowByVN(CObjectLayerInfo* pObjLayer); //根据父节点虚拟体，判断pObjLayer是否显示
	void SetSelect(CString strMotionName, bool bIsPublic, int layer);

	int ComputeRelativeLayer(int layer); //由于虚拟体的折叠导致layer可能存在偏差
	void ShareCodes();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNewMotion();
	afx_msg void OnDelMotion();
	afx_msg void OnAddEffect();
	afx_msg void OnDelEffect();
	afx_msg void OnPlayMotion();
	afx_msg void OnMergeKeyFrame();
	afx_msg void OnApartKeyFrame();
	afx_msg void OnSaveMon();		// 保存特效	
	afx_msg void OnImportMon();
	afx_msg void OnStopMotion();
	afx_msg void OnNewMotionFromBone();
	afx_msg void OnCopyLayer();
	afx_msg void OnRulerIn();
	afx_msg void OnRulerOut();
	afx_msg void OnSaveMonToLib();
	afx_msg void OnNewObjLayer(UINT nCmdId);
	afx_msg void OnMergeMotion();
	afx_msg void OnSaveObjAsMotion();
	afx_msg void OnSaveMonAsShowModel();
	afx_msg void OnNewCameraShake();
	afx_msg void OnNewFrozen();
	afx_msg void OnNewTransparent();

	afx_msg void OnNewEventObjLayer();

	afx_msg void OnShowPath();
	afx_msg void OnUpdateShowPath(CCmdUI* pCmdUI);

	afx_msg void OnLoopPlay();
	afx_msg void OnUpdateLoopPlay(CCmdUI* pCmdUI);

	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);

	afx_msg void OnObjLayerLodChange(NMHDR* pHDR, LRESULT* pResult); //ObjectLayer LOD
	afx_msg void OnSetMotionLodLevel(NMHDR* pHDR, LRESULT* pResult); //Motion LOD

	afx_msg void OnObjShowByLod(NMHDR* pHDR, LRESULT* pResult);
	//设置对象层的 LOD 水平
	afx_msg void OnSetHighLod();
	afx_msg void OnSetMiddleLod();
	afx_msg void OnSetLowLod();
	afx_msg void OnExitLodPopMenu();

	afx_msg void OnPasteLayers(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnCopyLayers(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnCutLayers(NMHDR* pHDR, LRESULT* pResult);

	afx_msg void OnTimeLayerSelected(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeLayerSizeChange(NMHDR* pHDR, LRESULT* pResult);

	afx_msg void OnTimeLayerPosChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeLayerActive(NMHDR* pHDR, LRESULT* pResult);	

	afx_msg void OnTimeAixesEndChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeAixesRedBoxChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeLayerClickOption(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeAixLayerPos(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeAixLayerRelate(NMHDR* pHDR, LRESULT* pResult);

	afx_msg void OnTimeLayerKeyDown(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnTimeAixesCtrlDrag(NMHDR* pHDR, LRESULT* pResult);


	afx_msg void OnMotionListItemChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnMotionListItemActive(NMHDR* pHDR, LRESULT* pResult);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnNewEffect();
	afx_msg void OnMenuNewpostCamerashake();
};


