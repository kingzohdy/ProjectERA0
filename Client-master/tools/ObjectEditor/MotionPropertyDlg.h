#pragma once
#include "OgreEntityMotionData.h"


// CMotionPropertyDlg dialog

class CMotionPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CMotionPropertyDlg)

public:
	CMotionPropertyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotionPropertyDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTIONPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAnim;
	float m_fTime;
	virtual BOOL OnInitDialog();
	CString m_strMotionName;

	Ogre::MOTIONLOOP_T m_loop;
protected:
	virtual void OnOK();
public:
	BOOL m_bPublic;
	afx_msg void OnBnClickedOk();
};
