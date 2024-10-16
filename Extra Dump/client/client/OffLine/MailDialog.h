#pragma once


// CMailDialog dialog

class CMailDialog : public CDialog
{
	DECLARE_DYNAMIC(CMailDialog)

public:
	CMailDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMailDialog();

// Dialog Data
	enum { IDD = IDD_MAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
