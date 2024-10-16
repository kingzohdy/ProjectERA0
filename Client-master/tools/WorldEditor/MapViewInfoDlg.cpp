// MapViewInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "MapViewInfoDlg.h"


// CMapViewInfoDlg dialog

IMPLEMENT_DYNAMIC(CMapViewInfoDlg, CDialog)

CMapViewInfoDlg::CMapViewInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapViewInfoDlg::IDD, pParent)
{

}

CMapViewInfoDlg::~CMapViewInfoDlg()
{
}

void CMapViewInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADARVIEW, m_RadarView);
}


BEGIN_MESSAGE_MAP(CMapViewInfoDlg, CDialog)
END_MESSAGE_MAP()


// CMapViewInfoDlg message handlers
