// FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "TerrainEdit.h"
#include "FindDialog.h"
#include "TerrainTile.h"
#include "ResBrowerDlg.h"
#include <string>

using namespace TE;
using namespace std;

// CFindDialog dialog

IMPLEMENT_DYNAMIC(CFindDialog, CDialog)

CFindDialog::CFindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDialog::IDD, pParent)
	, m_FindName(_T(""))
{

}

CFindDialog::~CFindDialog()
{
}

void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESNAME, m_FindName);
	DDX_Control(pDX, IDC_MODELLIST, m_ModeList);
}


BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
	ON_BN_CLICKED(IDC_FIND, &CFindDialog::OnBnClickedFind)
	ON_LBN_SELCHANGE(IDC_MODELLIST, &CFindDialog::OnLbnSelchangeModellist)
	ON_BN_CLICKED(IDC_PASTE, &CFindDialog::OnBnClickedPaste)
END_MESSAGE_MAP()


// CFindDialog message handlers

void CFindDialog::OnBnClickedFind()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_ModeList.ResetContent();

	for (int i=0; 
		i<(int)(g_TerrainEdit.GetEditData()->modelData.modelSet.size()); i++)
	{
		std::string modelName = 
			std::string(g_TerrainEdit.GetEditData()->modelData.modelSet[i]->GetModelData()->name);

		std::string findName = std::string(m_FindName.GetBuffer());
		
		if (modelName == findName)
		{
			int index = m_ModeList.AddString(modelName.c_str());
			m_ModeList.SetItemData(index, (DWORD_PTR)g_TerrainEdit.GetEditData()->modelData.modelSet[i]);
		}
	}
}

void CFindDialog::OnLbnSelchangeModellist()
{
	// TODO: Add your control notification handler code here
	int index = m_ModeList.GetCurSel();

	TerrModel *model = (TerrModel *)m_ModeList.GetItemData(index);

	if (model)
		g_TerrainEdit.setSelectModel(model);
}

void CFindDialog::OnBnClickedPaste()
{
	// TODO: Add your control notification handler code here
	m_FindName = CString(CResBrowerDlg::m_SelectName.c_str());

	UpdateData(0);
}
