#pragma once


// CCreateMapDlg dialog

class CCreateMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateMapDlg)

public:
	CCreateMapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateMapDlg();

// Dialog Data
	enum { IDD = IDD_CREATE_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_MapName;
	int m_MapSizeX;
	int m_MapSizeZ;
	int m_MapID;
	int m_GridSizeX;
	int m_GridSizeZ;
	int m_MapCoordX;
	int m_MapCoordZ;
	int m_MapHeight;
	int m_MapMaxHeight;
	int m_MapTexRepeat;
	CString m_DefaultTexture;
	afx_msg void OnBnClickedButtonDefaultTexture();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
