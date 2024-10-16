#pragma once
#include "ExtListCtrl.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"
#include "OgreEntityMotionElement.h"

// CLayerInfoDlg dialog

class CLayerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerInfoDlg)

public:
	CLayerInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLayerInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LAYERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CListCtrlExt m_wndList;
	CString m_strAnchor;
	float					m_fx;
	float					m_fy;
	float					m_fz;
	float					m_fStart;
	float					m_fEnd;
	int						m_iSel;
	int						m_iAnim;
	Ogre::Vector3			m_scale;
	Ogre::Quaternion			m_rotate;

	Ogre::MOTIONOBJTYPE_T	m_type;
	Ogre::POSTEFFECT_T		m_postEffect;
	Ogre::ColourValue			m_fsColor;
	BOOL					m_bOverwrite;
	int						m_iPriority;
	CString					m_strPostEffectName;
	BOOL					m_bNoStart;
	BOOL					m_bNoEnd;
	CComboBox				m_cmbAnim;

	EFFECT_TYPE				m_newType;

protected:
	virtual void OnOK();
public:

	afx_msg void OnBnClickedRdoEffect();
	afx_msg void OnBnClickedRdoAnim();

	afx_msg void OnBnClickedChkMooNostart();
	afx_msg void OnBnClickedChkMooNoend();

	afx_msg void OnCbnSelchangeCmpPosteffect();
	afx_msg void OnBnClickedRdoPost();
	afx_msg void OnLvnItemchangedListEffect(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnPostsetting();
};
