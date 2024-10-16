#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <string>

// CResBrowerDlg dialog

class CResBrowerDlg : public CDialog
{
	DECLARE_DYNAMIC(CResBrowerDlg)

public:
	CResBrowerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResBrowerDlg();

// Dialog Data
	enum { IDD = IDD_RES_BROWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void AddFile(CString StrPath, HTREEITEM faItem);
	void InitFileTree(CString rootpath);

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ResPreview;
	CTreeCtrl m_ResTree;
	CString m_RootPath;
	bool m_bInited;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedResTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckDrawRadarmap();
	CButton m_DrawRadarMap;
	CBCGPColorButton m_PreviewBGColor;
	afx_msg void OnBnClickedButtonPreviewBgcolor();
	afx_msg void OnBnClickedRadioResAll();
	afx_msg void OnBnClickedRadioResModel();
	afx_msg void OnBnClickedRadioResTiletex();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	static std::string m_SelectName; 
};
