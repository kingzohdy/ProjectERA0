#pragma once
#include "OgreColourValue.h"
#include "OgreKeyFrameArray.h"
#include "OgreSceneRendererPostEffect.h"
#include <vector>


// CSelectPostEffectDlg dialog

class CSelectPostEffectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectPostEffectDlg)

public:
	CSelectPostEffectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPostEffectDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_POSTEFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPostEffect;
	float m_fStart;
	float m_fEnd;
	BOOL m_bNoStart;
	BOOL m_bNoEnd;
	Ogre::ColourValue m_fsColor;
	Ogre::KeyFrameArray<Ogre::Vector4> m_fsColorKeys;
	Ogre::KeyFrameArray<float> m_transpKeys;
	std::vector<Ogre::PostproPhaseType> m_customPE;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkPostNostart();
	afx_msg void OnBnClickedChkPostNoend();
	afx_msg void OnBnClickedBtnPostsetting();
	afx_msg void OnCbnSelchangeCmoPostPostlist();
};
