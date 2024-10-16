#pragma once
#include "SectionDrawPanel.h"

// CRibbonSectionDlg dialog

class CRibbonSectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CRibbonSectionDlg)

public:
	CRibbonSectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRibbonSectionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RIBBONSECTION };
	std::vector<Ogre::Vector2> m_vectors;
	std::vector<float> m_texv;
	std::vector<int> m_indexes;

	CDrawPanel		m_wndDrawPanel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
protected:
	virtual void OnOK();
};
