#pragma once
#include <vector>

// CNewKeyframeDlg dialog
struct KFINFO
{
	UINT tick;
};

enum NEWKFTYPE
{
	NKT_LERP,
	NKT_COPY
};

class CNewKeyframeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewKeyframeDlg)

public:
	CNewKeyframeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewKeyframeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEWKEYFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDesc;
	std::vector<KFINFO> m_kfInfo;

	afx_msg void OnBnClickedRdoLerp();
	afx_msg void OnBnClickedRdoCopy();
	CListCtrl m_list;
	NEWKFTYPE m_type;
	int m_iCopyFrom;

protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
