
#pragma once
#include "ObjectEditorTypes.h"

#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"

#include "ResourceDropTarget.h"

enum SBTYPE
{
	SBT_SRT,
	SBT_FLOAT,
	SBT_COLOR,
	SBT_REMANETSHADOW,
	SBT_OFFCOLOR,
	SBT_ICEFROZEN,
	SBT_FOCUSBLUR,
};

class CSRTBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CSRTBar)

public:
	CSRTBar();
	virtual ~CSRTBar();

	CResDropPropListTarget m_dropTarget;

	CNotifiedProp* m_pPropPosX;
	CNotifiedProp* m_pPropPosY;
	CNotifiedProp* m_pPropPosZ;

	CNotifiedProp* m_pPropYaw;
	CNotifiedProp* m_pPropPitch;
	CNotifiedProp* m_pPropRoll;

	CNotifiedProp* m_pPropResetRot;

	CNotifiedProp* m_pPropScaleX;
	CNotifiedProp* m_pPropScaleY;
	CNotifiedProp* m_pPropScaleZ;
	CNotifiedProp* m_pPropTransparent;

	CNotifiedProp* m_pPropFloatValue;
	CNotifiedColorProp* m_pPropColor;
	CNotifiedProp* m_pPropAlpha;

	CNotifiedProp *m_pPropFade;
	CNotifiedColorProp *m_pPropOffColor;
	CNotifiedProp *m_pPropOverlayBlendMode;
	CNotifiedProp *m_pPropOverlayTex;
	CNotifiedProp *m_pPropInterval;
	CNotifiedProp *m_pPropImages;

	CBCGPPropList	m_wndPropList;
	CBCGPEdit		m_wndTitle;

	CString			m_strObjName;

	Ogre::Vector3		m_pos;
	Ogre::Quaternion		m_rot;
	Ogre::Vector3		m_scale;
	Ogre::Vector3		m_rotAngle;
	float				m_trans;

	float				m_float;
	float				m_min;
	float				m_max;
	float				m_ratio;
	CString				m_strFloatName;
	Ogre::BlendMode     m_OverlayBlendMode;
	CString             m_OverlayTex;

	Ogre::ColourValue   m_offcolor;
	Ogre::ColourValue		m_color;
	SBTYPE				m_type;
	int m_ShadowInterval;
	int m_Images;

	bool m_bFade;

	CSRTObject*		m_pSrt;
	CFloatObject*	m_pFlt;
	CColorObject*	m_pClr;
	CRemanetShadowObject* m_pRemanetShadow;
	COffColorObject* m_pOffClr;			//偏色
	CIceFrozenObject* m_pIceFrozen;
	CFocusBlurObject* m_pFocusBlur;		//焦点模糊

	//焦点模糊参数
	CNotifiedProp*  m_pPropBlur;		//焦点模糊大小
	CNotifiedProp*	m_pPropXOff;		//焦点模糊X轴偏移
	CNotifiedProp*	m_pPropYOff;		//焦点模糊Y轴偏移
	CNotifiedProp*	m_pPropZOff;		//焦点模糊Z轴偏移
	float			m_nblur;
	float			m_nxoff;
	float			m_nyoff;
	float			m_nzoff;

	void			SetRemanetShadowObject(CRemanetShadowObject* pObj);
	CRemanetShadowObject* GetRemanetShadowObject() { return m_pRemanetShadow; }

	void			SetSRTObject(CSRTObject * pSrtObj);
	CSRTObject*		GetSRTObject() { return m_pSrt; };

	void			SetFloatObject(CFloatObject * pObj);
	CFloatObject*	GetFloatObject() { return m_pFlt; };

	void			SetColorObject(CColorObject * pObj);
	CColorObject*	GetColorObject() { return m_pClr; };

	void			SetOffColorObject(COffColorObject* pObj);
	COffColorObject* GetOffColorObject(){ return m_pOffClr; }

	void			SetIceFrozenObject(CIceFrozenObject* pObj);
	CIceFrozenObject* GetIceFrozenObject() { return m_pIceFrozen; }

	void			SetFocusBlurObject(CFocusBlurObject* pObj);
	CFocusBlurObject* GetFocusBlurObject() { return m_pFocusBlur; }

	void			ClearTargetObject();


	void CreatePropFloat();
	void CreatePropColor();
	void CreatePropRemanetShadow();
	void CreatePropOffColor();
	void CreatePropSRT();
	void CreatePropIceFrozen();
	void CratePropFocusBlur();

	void SaveCurrentProp(CString strChangeBy);
	void UpdateList();
	void DataUpdateSRT(DATA_UPDATA_TYPE type);
	void DataUpdateFloat(DATA_UPDATA_TYPE type);
	void DataUpdateColor(DATA_UPDATA_TYPE type);
	void DataUpdateRemanetShadow(DATA_UPDATA_TYPE type);
	void DataUpdateOffColor(DATA_UPDATA_TYPE type);
	void DataUpdateIceFrozen(DATA_UPDATA_TYPE type);
	void DataUpdateFocusBlur(DATA_UPDATA_TYPE type);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnClickBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


