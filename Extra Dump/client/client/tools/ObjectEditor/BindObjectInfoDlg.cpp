// BindObjectInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "BindObjectInfoDlg.h"
#include "EffectManager.h"
#include "AnchorSet.h"
#include "ModelMotionInfo.h"
#include "EffectLayerInfo.h"


// CBindObjectInfoDlg dialog

IMPLEMENT_DYNAMIC(CBindObjectInfoDlg, CDialog)

CBindObjectInfoDlg::CBindObjectInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBindObjectInfoDlg::IDD, pParent)
	, m_fx(0)
	, m_fy(0)
	, m_fz(0)
	, m_fStart(0)
	, m_fEnd(0)
	, m_strName(_T(""))
	, m_pCurrObj(NULL)
	, m_pSelectedParent(NULL)
	, m_SortIndex(0)
{

}

CBindObjectInfoDlg::~CBindObjectInfoDlg()
{
}

void CBindObjectInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MOO_X, m_fx);
	DDX_Text(pDX, IDC_EDIT_MOO_Y, m_fy);
	DDX_Text(pDX, IDC_EDIT_MOO_Z, m_fz);
	DDX_Text(pDX, IDC_EDIT_MOO_START, m_fStart);
	DDX_Text(pDX, IDC_EDIT_MOO_END, m_fEnd);
	DDX_Text(pDX, IDC_EDIT_MOO_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MOO_SORTINDEX, m_SortIndex);
	DDX_Control(pDX, IDC_CMB_MOO_PARENT, m_cmbParent);
	DDX_Control(pDX, IDC_CMB_MOO_ANCHOR, m_cmbAnchor);
	DDX_Control(pDX, IDC_CMB_MOO_POSINTERPOL, m_PosInterpolCombo);
	DDX_Control(pDX, IDC_EDIT_BINDOBJ_ANCHOR, m_AnchorEdit);
}


BEGIN_MESSAGE_MAP(CBindObjectInfoDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CMB_MOO_ANCHOR, OnCbnSelchangeCmbMooAnchor)
	ON_CBN_SELCHANGE(IDC_CMB_MOO_PARENT, OnCbnSelchangeCmbMooParent)
	ON_EN_CHANGE(IDC_EDIT_BINDOBJ_ANCHOR, &CBindObjectInfoDlg::OnEnChangeEditBindobjAnchor)
END_MESSAGE_MAP()


// CBindObjectInfoDlg message handlers
BOOL CBindObjectInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//GetDlgItem(IDC_CMB_MOO_ANCHOR)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"0");

	ANCHOR_OF_OBJ def;
	def.anchors.push_back(0);
	def.strObjName = "无";
	for(int i = 0; i <(int) CEffectMgr::GetInstance()->m_anchorInfo->m_AnchorInfo.size(); i++)
	{
		int id = CEffectMgr::GetInstance()->m_anchorInfo->m_AnchorInfo[i].id;
		if(id == 0)
			continue;
		def.anchors.push_back(id);
		//CString str;
		//str.Format("%d", id);
		//GetDlgItem(IDC_CMB_MOO_ANCHOR)->SendMessage(CB_ADDSTRING, 0, (LPARAM)str.GetBuffer() );
	}
	m_AnchorInfo.push_back(def);

	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->m_modelMotion;
	CModelMotionInfo::MOTION* pCurrMotion = pMotionInfo->FindMotionOfObject(m_pCurrObj);
	for(int i = 0; i < (int)pCurrMotion->objs.size(); i++)
	{
		ANCHOR_OF_OBJ objanchor;

		CObjectLayerInfo* pObjParent = pCurrMotion->objs[i];
		if(pObjParent->ReceiveChild(m_pCurrObj))
		{
			objanchor.strObjName = pObjParent->GetDisplayName();
			objanchor.pObj = pObjParent;
			pObjParent->GetAnchorList(objanchor.anchors);

			m_AnchorInfo.push_back(objanchor);
		}
	}

	m_PosInterpolCombo.AddString("线性插值");
	m_PosInterpolCombo.AddString("曲线插值");
	if(m_pCurrObj->m_PosInterpolType == Ogre::INTERPOL_LINEAR) m_PosInterpolCombo.SetCurSel(0);
	else m_PosInterpolCombo.SetCurSel(1);

	int iSel = 0;
	for(int i = 0; i < (int) m_AnchorInfo.size(); i++)
	{
		m_cmbParent.InsertString(i, m_AnchorInfo[i].strObjName);
		if(m_AnchorInfo[i].pObj == m_pCurrObj->getParent())
			iSel = i;
	}

	m_cmbParent.SetCurSel(iSel);

	UpdateAnchorList(iSel, m_iAnchor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBindObjectInfoDlg::UpdateAnchorList(int iSelParent, int iSelAnchor)
{
	while(m_cmbAnchor.GetCount() > 0)
		m_cmbAnchor.DeleteString(0);

	int iSel = 0;
	for(int i = 0; i < (int)m_AnchorInfo[iSelParent].anchors.size(); i++)
	{
		int id = m_AnchorInfo[iSelParent].anchors[i];
		CString str;
		str.Format("%d", id);
		m_cmbAnchor.InsertString(i, str.GetBuffer());
		if(id == iSelAnchor)
			iSel = i;
	}

	m_cmbAnchor.SetCurSel(iSel);

	char buffer[256];
	sprintf(buffer, "%d", iSelAnchor);
	m_AnchorEdit.SetWindowText(buffer);
}



void CBindObjectInfoDlg::OnCbnSelchangeCmbMooAnchor()
{
	int iSel = m_cmbAnchor.GetCurSel();
	if(iSel < 0)
		return;

	CString selText;
	m_cmbAnchor.GetWindowText(selText);

	int id = 0;
	sscanf(selText.GetBuffer(), "%d", &id);

	m_iAnchor = id;
	char buffer[256];
	sprintf(buffer, "%d", id);
	m_AnchorEdit.SetWindowText(buffer);
}

void CBindObjectInfoDlg::OnCbnSelchangeCmbMooParent()
{
	int iSel = m_cmbParent.GetCurSel();
	UpdateAnchorList(iSel, m_iAnchor);
}

void CBindObjectInfoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	int iSel = m_cmbParent.GetCurSel();
	m_pSelectedParent = NULL;

	if(iSel > 0)
	{
		m_pSelectedParent = m_AnchorInfo[iSel].pObj;
	}

	if(m_PosInterpolCombo.GetCurSel() == 0) m_pCurrObj->m_PosInterpolType = Ogre::INTERPOL_LINEAR;
	else m_pCurrObj->m_PosInterpolType = Ogre::INTERPOL_HERMITE;

	CDialog::OnOK();
}
void CBindObjectInfoDlg::OnEnChangeEditBindobjAnchor()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString buffer;
	m_AnchorEdit.GetWindowText(buffer);

	int id = 0;
	sscanf(buffer.GetBuffer(), "%d", &id);

	m_iAnchor = id;
}
