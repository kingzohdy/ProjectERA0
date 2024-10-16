
#pragma once
#include "EffectUnitEditBar.h"

class CParticleBar : public CEffectEditBar, public CKeyFrameObject
{
	DECLARE_DYNAMIC(CParticleBar)

public:
	CParticleBar();
	virtual ~CParticleBar();

	virtual void SetEditTarget(CEffectUnit* pEffect);
	virtual CEffectUnit* GetEditTarget();
	virtual CKeyFrameObject* GetKeyFramObj() { return static_cast<CKeyFrameObject*>(this); };
	CParticle* m_pTarget;
	bool	m_bLockApp;

	CBCGPToolBar		m_wndToolBar;
	CNormalSplitterWnd	m_wndSplitter;
	CTreeCtrl			m_wndTree;
	CBCGPPropList		m_wndPropList;
	CResDropPropListTarget		m_dropTarget;
	CResDropTarget		m_dropTargetReplaceObj;

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;

	//CNotifiedProp* m_pPropOffsetX;
	//CNotifiedProp* m_pPropOffsetY;
	//CNotifiedProp* m_pPropOffsetZ;

	//CNotifiedProp* m_pPropYaw;
	//CNotifiedProp* m_pPropPitch;
	//CNotifiedProp* m_pPropRoll;

	CNotifiedProp*	m_pPropEmitterType;
	CNotifiedProp*	m_pPropParticleType;
	CNotifiedProp*	m_pPropFlag;
	CNotifiedProp*	m_pPropBlendType;
	CNotifiedProp*	m_pPropPos;
	CNotifiedProp*	m_pPropMaxParticle;
	CNotifiedProp*	m_pPropUVRotate;
	CNotifiedProp*	m_pPropUVScale;

	CNotifiedProp*	m_pPropSlowDown;
	CNotifiedProp*	m_pPropRotation;
	CNotifiedProp*	m_pPropTexRows;
	CNotifiedProp*	m_pPropTexCols;
	CNotifiedProp*	m_pPropTimeSlot;
	//CBCGPProp*	m_pPropColorKeys;
	CBCGPColorProp*	m_pPropColorKey1;
	CNotifiedProp*	m_pPropAlphaKey1;
	CBCGPColorProp*	m_pPropColorKey2;
	CNotifiedProp*	m_pPropAlphaKey2;
	CBCGPColorProp*	m_pPropColorKey3;
	CNotifiedProp*	m_pPropAlphaKey3;
	//CBCGPProp*	m_pPropSizeKeys;
	CNotifiedProp*	m_pPropSizeKey1;
	CNotifiedProp*	m_pPropSizeKey2;
	CNotifiedProp*	m_pPropSizeKey3;
	CNotifiedProp*  m_pPropAspectKey1;
	CNotifiedProp*  m_pPropAspectKey2;
	CNotifiedProp*  m_pPropAspectKey3;
	CNotifiedProp*	m_pPropTexture;
	CNotifiedProp*	m_pPropObject;
	CNotifiedProp*  m_pPropIsLocal;
	CNotifiedProp*  m_pPropCustomEmit;
	CNotifiedProp*	m_pPropRotStyle;
	CNotifiedProp*	m_pPropEmitOnce;
	//CNotifiedProp*  m_pPropScale;
	CNotifiedProp*  m_pPropRandomTile;
	CNotifiedProp*  m_pPropDistort;
	CNotifiedProp*  m_pPropAddressMode;
	CNotifiedProp*  m_pPropRandomRot;

	CNotifiedProp* m_pPropSpeed;
	CNotifiedProp* m_pPropSpeedDir;
	CNotifiedProp* m_pPropSpeedVar;
	CNotifiedProp* m_pPropSpread;
	CNotifiedProp* m_pPropSpreadOffset;
	CNotifiedProp* m_pPropLatitude;
	CNotifiedProp* m_pPropAccelerate;
	CNotifiedProp* m_pPropAccelerateDir;
	CNotifiedProp* m_pPropLifespan;
	CNotifiedProp* m_pPropRate;
	CNotifiedProp* m_pPropAreaLenth;
	CNotifiedProp* m_pPropAreaWidth;
	CNotifiedProp* m_pPropAreaDepth;
	CNotifiedProp* m_pPropDeacceleration;
	CNotifiedProp* m_pPropTick;
	CNotifiedProp* m_pPropSizeVar;
	CNotifiedProp* m_pPropLifeVar;
	CNotifiedProp* m_pPropRandomSpread;

	CNotifiedProp* m_pPropBindBone;

	CNotifiedProp* m_pPropColorBtn;
	CNotifiedProp* m_pPropAlphaBtn;
	CNotifiedProp* m_pPropSizeBtn;
	CNotifiedProp* m_pPropAspectBtn;


	CNotifiedProp* m_pPropCldOffsetX;
	CNotifiedProp* m_pPropCldOffsetY;
	CNotifiedProp* m_pPropCldOffsetZ;

	CNotifiedProp* m_pPropCldYaw;
	CNotifiedProp* m_pPropCldPitch;
	CNotifiedProp* m_pPropCldRoll;

	CNotifiedProp* m_pPropUpLevel;
	CNotifiedProp* m_pPropReverseUpLevel;
	CNotifiedProp* m_pPropCldWidth;
	CNotifiedProp* m_pPropCldLength;
	CNotifiedProp* m_pPropCldAtten;


	CNotifiedProp*	m_pPropMaskUVRotate;
	CNotifiedProp*	m_pPropMaskUVScale;
	CNotifiedProp*	m_pPropMaskUVAddressMode;
	CNotifiedProp*	m_pPropMaskRow;
	CNotifiedProp*	m_pPropMaskCol;
	CNotifiedProp*	m_pPropMaskTexture;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnNewFrame();
	afx_msg void OnDeleteFrame();
	afx_msg void OnNewCld();
	afx_msg void OnDelCld();
	afx_msg void OnApply();
	afx_msg void OnRemove();
	afx_msg void OnSave();
	afx_msg void OnVisible();
	afx_msg void OnBtnClick();
	afx_msg void OnBtnLockApply();
	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);
	afx_msg void OnBtnDelReplaceObj();

	afx_msg void OnUpdateVisible(CCmdUI* pUI);
	afx_msg void OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult);
	afx_msg LRESULT OnUpdatePropValue(WPARAM w, LPARAM l);
	afx_msg LRESULT OnClickPropBtn(WPARAM w, LPARAM l);
	afx_msg LRESULT OnDropRes(WPARAM w, LPARAM l);
	afx_msg void OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult);
protected:
	HTREEITEM m_hKeyFrame;
	HTREEITEM m_hDesc;
	HTREEITEM m_hBindBone;
	HTREEITEM m_hCollision;
	HTREEITEM m_hReplaceObj;

	void ChangePropList();
	void SaveCurrentProp(HTREEITEM hOldItem);
	void UpdateList();
	void UpdateTree();
	LPARAM GetItemParam(HTREEITEM hItem);
	void ClearProp();
	void DescUpdate(DATA_UPDATA_TYPE type);
	void KeyFrameUpdate(DATA_UPDATA_TYPE type, CParticle::KFItem* pItem);
	void CollisionFaceUpdate(DATA_UPDATA_TYPE type, Ogre::PECollisionFace* pItem);

	void OnDropResOnPropList(CResDropTarget::DROPNOTIFY* pdn);
	void OnDropResOnTree(CResDropTarget::DROPNOTIFY* pdn);

	void UpdateReplaceObject();

	void PropUpdate_emittertype(CBCGPProp** ppProp, Ogre::EmitterType* data, DATA_UPDATA_TYPE type, CString name);
	void PropUpdate_particletype(CBCGPProp** ppProp, Ogre::ParticleDrawType* data, DATA_UPDATA_TYPE type, CString name);
	void PropUpdate_acc(CBCGPProp** ppProp, Ogre::Vector3* data, DATA_UPDATA_TYPE type, CString name);
	void PropUpdate_rotstyle(CBCGPProp** ppProp, Ogre::ParticleDirType* data, DATA_UPDATA_TYPE type, CString name);
	//void DescUpdate(DATA_UPDATA_TYPE type);

	HTREEITEM m_hLastSel;
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);


	void UpdateKeyFrameWnd();
	void ReleaseKeyFrameWnd();


	//KeyFrameObject
	virtual void CopyKeyFrame(int from, int to);
	virtual int AddKeyFrame(UINT tick);
	virtual void DeleteKeyFrame(int index);
	virtual void ActiveKeyFrame(int index);
	virtual void SetKeyFramePos(int index, UINT tick);
	virtual int GetKeyFrameCount();
	virtual UINT GetKeyFrameTick(int index);
	virtual CString GetName();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


