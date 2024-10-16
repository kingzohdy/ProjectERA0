#pragma once
#include "afxwin.h"


// CPetDialog dialog

class CPetDialog : public CDialog
{
	DECLARE_DYNAMIC(CPetDialog)

public:
	CPetDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPetDialog();

// Dialog Data
	enum { IDD = IDD_PET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CWnd m_PetRenderWnd;
};
