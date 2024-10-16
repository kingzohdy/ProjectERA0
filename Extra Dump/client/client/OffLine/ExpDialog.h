#pragma once

#include "RenderScene.h"
#include "afxwin.h"

class Ogre::AvatarEntity;

// CExpDialog dialog

class CExpDialog : public CDialog
{
	DECLARE_DYNAMIC(CExpDialog)

public:
	CExpDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExpDialog();

// Dialog Data
	enum { IDD = IDD_EXPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	float m_fRotYaw;
	CComboBox m_RoleList;

	CString	 m_cstrModelPath;
	int		 m_AvatarAttrib[32];
	Ogre::AvatarEntity*	m_pEntity;

	void	UpdateAvatar();

public:
	virtual BOOL OnInitDialog();
	CWnd m_expRenderWnd;
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT onNetNotify(WPARAM,LPARAM);
	afx_msg void OnCbnSelchangeComboRolelist();
	afx_msg void OnBnClickedButton3();
	CString m_cstrAttr;
};
