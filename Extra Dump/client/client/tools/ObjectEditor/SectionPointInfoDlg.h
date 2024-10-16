#pragma once


// CSectionPointInfoDlg dialog

class CSectionPointInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CSectionPointInfoDlg)

public:
	CSectionPointInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSectionPointInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SECTIONINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	float m_fTexV;
	float m_fPosX;
	float m_fPosY;

};
