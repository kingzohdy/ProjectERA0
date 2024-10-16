#pragma once
#include "afxwin.h"


// CMapViewInfoDlg dialog

class CMapViewInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapViewInfoDlg)

public:
	CMapViewInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapViewInfoDlg();

// Dialog Data
	enum { IDD = IDD_MAPVIEWINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_RadarView;
};
