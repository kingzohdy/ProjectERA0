// NewItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "NewItemDlg.h"
#include "ObjectTypeHelper_Avatar.h"
#include "DBManager.h"
#include "GameResDBTypes.h"

#include "OgreResDB.h"


// CNewItemDlg dialog

IMPLEMENT_DYNAMIC(CNewItemDlg, CDialog)

CNewItemDlg::CNewItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewItemDlg::IDD, pParent)
	, m_iItemID(0)
	, m_ItemName("")
{

}

CNewItemDlg::~CNewItemDlg()
{
}

void CNewItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEWITEM_ID, m_iItemID);
	DDX_Text(pDX, IDC_EDIT_NEWITEM_NAME, m_ItemName);
}


BEGIN_MESSAGE_MAP(CNewItemDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CNewItemDlg message handlers
void CNewItemDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CNewItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Ogre::ResTable *pitemtab = CDataBase::GetInstance()->GetTable("ItemDef");
	if(pitemtab)
	{
		ITEMDEF item;
		item.ItemID = m_iItemID;
		if(pitemtab->FindRecord(&item, 1, &item) > 0)
		{
			m_ItemName = item.Name;
			UpdateData(FALSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}