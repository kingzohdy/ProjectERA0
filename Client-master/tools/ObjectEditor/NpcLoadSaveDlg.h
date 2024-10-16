#pragma once
#include "afxcmn.h"


// CNpcLoadSaveDlg dialog

class CNpcLoadSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CNpcLoadSaveDlg)

public:
	CNpcLoadSaveDlg(CWnd* pParent = NULL, bool load=true);   // standard constructor
	virtual ~CNpcLoadSaveDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NPCLOADSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void loadNpcList();
	virtual BOOL OnInitDialog();
	bool m_isLoad;
	int m_NpcID;
	CString m_NpcName;
	CString m_NpcDesc;
	float m_NpcScale;
	float m_NpcXparent;
	CListCtrl m_NpcList;
	afx_msg void OnLvnItemchangedListNpclist(NMHDR *pNMHDR, LRESULT *pResult);
	int m_NpcHeight;
	int m_NpcRadius;
};
