// RibbonSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "RibbonSectionDlg.h"


// CRibbonSectionDlg dialog

IMPLEMENT_DYNAMIC(CRibbonSectionDlg, CDialog)

CRibbonSectionDlg::CRibbonSectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRibbonSectionDlg::IDD, pParent)
{

}

CRibbonSectionDlg::~CRibbonSectionDlg()
{
}

void CRibbonSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRibbonSectionDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRibbonSectionDlg message handlers
BOOL CRibbonSectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	GetClientRect(rect);

	for(int i = 0; i < (int)m_vectors.size(); i++)
	{
		VERTEX_INFO vi;
		vi.pos = m_vectors[i];
		vi.selected = false;
		vi.texV = m_texv[i];
		m_wndDrawPanel.m_Vertexes.push_back(vi);
	}

	LINE_INFO li;
	for(int i = 0; i < (int)m_indexes.size(); i++)
	{
		if(m_indexes[i] == -1)
		{
			if(li.lineVertex.size() > 0)
				m_wndDrawPanel.m_Lines.push_back(li);
			li.lineVertex.clear();
		}
		else
		{
			li.lineVertex.push_back(m_indexes[i]);
		}
	}
	if(li.lineVertex.size() > 0)
	{
		m_wndDrawPanel.m_Lines.push_back(li);
	}



	m_wndDrawPanel.Create(NULL, "", WS_CHILD, CRect(0, 0, rect.Width(), rect.Height()), this, 11);
	m_wndDrawPanel.ShowWindow(SW_SHOW);




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRibbonSectionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);
	if(m_wndDrawPanel.GetSafeHwnd())
		m_wndDrawPanel.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
}

void CRibbonSectionDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	int res = MessageBox("保存编辑结果吗?","提示", MB_YESNOCANCEL);
	if(IDYES == res)
	{
		OnOK();
	}
	else if(IDNO == res)
	{
		CDialog::OnClose();
	}
}

void CRibbonSectionDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndDrawPanel.m_Vertexes.size() >= 40)
	{
		MessageBox("不能超过40个点");
		return;
	}
	m_vectors.clear();
	m_texv.clear();
	m_indexes.clear();

	for(int i = 0; i < (int)m_wndDrawPanel.m_Vertexes.size(); i++)
	{
		m_vectors.push_back(m_wndDrawPanel.m_Vertexes[i].pos);
		m_texv.push_back(m_wndDrawPanel.m_Vertexes[i].texV);
	}

	for(int i = 0; i < (int)m_wndDrawPanel.m_Lines.size(); i++)
	{
		LINE_INFO& li = m_wndDrawPanel.m_Lines[i];
		bool bHasDot = false;
		for(int k = 0; k < (int)li.lineVertex.size(); k++)
		{
			bHasDot = true;
			m_indexes.push_back(li.lineVertex[k]);
		}

		if(bHasDot == true && i != (int)m_wndDrawPanel.m_Lines.size() - 1)
		{
			m_indexes.push_back(-1);
		}

	}

	CDialog::OnOK();
}