#pragma once
#include <vector>
#include "afxwin.h"

struct ANCHOR_OF_OBJ
{
	CString strObjName;
	CObjectLayerInfo* pObj;
	std::vector<int> anchors;
};

// CBindObjectInfoDlg dialog

class CBindObjectInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBindObjectInfoDlg)

public:
	CBindObjectInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBindObjectInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BINDOBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//CString m_strAnchor;
	float m_fx;
	float m_fy;
	float m_fz;
	float m_fStart;
	float m_fEnd;
	int m_iAnchor;
	virtual BOOL OnInitDialog();
	CString m_strName;
	CComboBox m_cmbParent;
	CComboBox m_cmbAnchor;
	int m_SortIndex;
	CObjectLayerInfo* m_pCurrObj;
	std::vector<ANCHOR_OF_OBJ> m_AnchorInfo;

	CObjectLayerInfo* m_pSelectedParent; //作为返回选择用；


	void UpdateAnchorList(int iSelParent, int iSelAnchor = 0);


	afx_msg void OnCbnSelchangeCmbMooAnchor();
	afx_msg void OnCbnSelchangeCmbMooParent();
protected:
	virtual void OnOK();
public:
	CComboBox m_PosInterpolCombo;
	CEdit m_AnchorEdit;
	afx_msg void OnEnChangeEditBindobjAnchor();
};
