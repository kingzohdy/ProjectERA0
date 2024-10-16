#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "EditListCtrl.h"

// CNpcEditDlg dialog

class CNpcEditDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CNpcEditDlg)

public:
	CNpcEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNpcEditDlg();

	void InitNpcResData();
	void UpdateNpcData();
	void OnSelectNPC(int sel);
	void OnDeleteNPC(int sel);

// Dialog Data
	enum { IDD = IDD_NPC_EDIT };

	char m_IDFilter[64];
	char m_NameFilter[64];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickListNpcRes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListNpcInst(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckDisplayNpc();
	afx_msg void OnBnClickedCheckEditNpc();
	afx_msg void OnBnClickedButtonDelNpc();
	CBCGPListCtrl m_NpcResList;
	CEditListCtrl m_NpcInstList;
	CButton m_DispNpcCheck;
	CButton m_EditNpcCheck;
	afx_msg void OnLvnItemchangedListNpcRes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelAllnpc();
	afx_msg void OnBnClickedButtonSavenpcTodb();
	afx_msg void OnBnClickedButtonCopyjingyingTodb();
	afx_msg void OnBnClickedButtonLoadnpcFromdb();
	afx_msg void OnEnChangeEditNpcidFilter();
	afx_msg void OnEnChangeEditNpcnameFilter();
	afx_msg void OnLvnItemchangedListNpcInst(NMHDR *pNMHDR, LRESULT *pResult);
};
