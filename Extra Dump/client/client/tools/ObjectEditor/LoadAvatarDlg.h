#pragma once
#include "GameEngineTypes.h"


// CLoadAvatarDlg dialog

class CLoadAvatarDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadAvatarDlg)

public:
	CLoadAvatarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadAvatarDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOADAVATAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBCGPPropList m_wndPropList;

	CAvatarShowInfo* m_pShowInfo;	
	int m_attrib[MAX_AVATAR_FIELD];
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
