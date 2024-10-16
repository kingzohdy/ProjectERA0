#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "EditListCtrl.h"


// CAreaEditDlg dialog
class CEnvironmentEditDlg;
class CProcEditDlg;

class CAreaEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CAreaEditDlg)

public:
	CAreaEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAreaEditDlg();

	void UpdateAreaData();
	void SelectArea(int i);

// Dialog Data
	enum { IDD = IDD_AREA_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void addArea(int type);
public:
	CEnvironmentEditDlg *m_pEnvEditDlg;
	CProcEditDlg *m_pProcEditDlg;

	CButton m_DisplayAreaCheck;
	CEditListCtrl m_AreaList;
	afx_msg void OnBnClickedButtonAddarea();
	afx_msg void OnBnClickedButtonDelarea();
	afx_msg void OnBnClickedButtonLoadarea();
	afx_msg void OnBnClickedButtonSavearea();
	afx_msg void OnBnClickedButtonSelectarea();
	afx_msg void OnBnClickedCheckDisplayarea();
	afx_msg void OnNMClickListArea(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddareaProc();
	afx_msg void OnBnClickedButtonSaveprocTodb();
	afx_msg void OnBnClickedButtonCopyarea();
	afx_msg void OnBnClickedButtonPastearea();
	afx_msg void OnLvnItemchangedListArea(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchangedListArea(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditListArea(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadprocFromdb();
};
