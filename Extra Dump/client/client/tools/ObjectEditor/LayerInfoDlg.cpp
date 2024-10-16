// LayerInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "LayerInfoDlg.h"
#include "EffectManager.h"
#include "BindInfo.h"
#include "ObjectTypeHelper.h"
#include "EffectUnit.h"
#include "AnchorSet.h"
#include "ColorKeysDlg.h"

#include "OgreEntity.h"
#include "OgreModel.h"


// CLayerInfoDlg dialog

IMPLEMENT_DYNAMIC(CLayerInfoDlg, CDialog)

CLayerInfoDlg::CLayerInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerInfoDlg::IDD, pParent)
	, m_strAnchor(_T("0"))
	, m_fx(0)
	, m_fy(0)
	, m_fz(0)
	, m_fStart(0)
	, m_fEnd(2)
	, m_iSel(-1)
	, m_iAnim(-1)
	, m_bNoStart(FALSE)
	, m_bNoEnd(FALSE)
	, m_bOverwrite(FALSE)
	, m_iPriority(10)
	, m_strPostEffectName(_T(""))
{
	m_scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
}

CLayerInfoDlg::~CLayerInfoDlg()
{
}

void CLayerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EFFECT, m_wndList);
	DDX_CBString(pDX, IDC_CMB_MOO_ANCHOR, m_strAnchor);
	DDX_Text(pDX, IDC_EDIT_MOO_X, m_fx);
	DDX_Text(pDX, IDC_EDIT_MOO_Y, m_fy);
	DDX_Text(pDX, IDC_EDIT_MOO_Z, m_fz);
	DDX_Text(pDX, IDC_EDIT_MOO_START, m_fStart);
	DDX_Text(pDX, IDC_EDIT_MOO_END, m_fEnd);
	DDX_Control(pDX, IDC_COMBO_ANIM_ID, m_cmbAnim);
	DDX_Check(pDX, IDC_CHK_MOO_NOSTART, m_bNoStart);
	DDX_Check(pDX, IDC_CHK_MOO_NOEND, m_bNoEnd);
	DDX_Check(pDX, IDC_CHK_MOO_OVERWRITE, m_bOverwrite);
	DDX_Text(pDX, IDC_EDIT_MOO_PRIORITY, m_iPriority);
	DDX_CBString(pDX, IDC_CMP_POSTEFFECT, m_strPostEffectName);
}


BEGIN_MESSAGE_MAP(CLayerInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_RDO_EFFECT, OnBnClickedRdoEffect)
	ON_BN_CLICKED(IDC_RDO_ANIM, OnBnClickedRdoAnim)
	ON_BN_CLICKED(IDC_CHK_MOO_NOSTART, OnBnClickedChkMooNostart)
	ON_BN_CLICKED(IDC_CHK_MOO_NOEND, OnBnClickedChkMooNoend)
	ON_CBN_SELCHANGE(IDC_CMP_POSTEFFECT, OnCbnSelchangeCmpPosteffect)
	ON_BN_CLICKED(IDC_RDO_POST, OnBnClickedRdoPost)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EFFECT, OnLvnItemchangedListEffect)
	ON_BN_CLICKED(IDC_BTN_POSTSETTING, OnBnClickedBtnPostsetting)
END_MESSAGE_MAP()


// CLayerInfoDlg message handlers
BOOL CLayerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndList.InsertColumn(0, "特效名", LVCFMT_LEFT, 200);
	m_wndList.InsertColumn(1, "类型", LVCFMT_LEFT, 100);

	CCvtHelper cvt;
	int index = 0;
	for(int i = 0; i < (int)CEffectMgr::GetInstance()->m_Effects.size(); i++)
	{
		if( CEffectMgr::GetInstance()->m_Effects[i]->GetBindInfo()->bDynamic)
			continue;
		CString name = CEffectMgr::GetInstance()->m_Effects[i]->GetName();
		CString type = "其他";

		switch(CEffectMgr::GetInstance()->m_Effects[i]->GetType())
		{
		case EFFECT_PEMITTER:
			type = "粒子";
			break;
		case EFFECT_BILLBOARD:
			type = "公告板";
			break;
		case EFFECT_LIGHT:
			type = "光源";
			break;
		case EFFECT_RIBBON:
			type = "条带";
			break;
		case EFFECT_BEAM:
			type = "波带";
			break;
		case EFFECT_DECAL:
			type = "地形贴花";
			break;
		case EFFECT_VIRTUALNODE:
			type = "虚拟体";
			break;
		case EFFECT_GEOM:
			type = "几何体";
			break;
		}

		m_wndList.InsertItem(LVIF_TEXT, index, name.GetBuffer(), 0, 0, 0, 0);
		m_wndList.SetItem(index, 1, LVIF_TEXT, type.GetBuffer(), 0, 0, 0, 0);
		m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)i, 0);
		index++;
	}

	//添加新特效

	m_wndList.InsertItem(LVIF_TEXT, index, "新粒子", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_PEMITTER, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新光源", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_LIGHT, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新公告板", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_BILLBOARD, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新条带", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_RIBBON, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新波带", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_BEAM, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新声音", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_SOUND, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新贴花", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_DECAL, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "新几何体", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_GEOM, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));

	m_wndList.InsertItem(LVIF_TEXT, index, "虚拟体", 0, 0, 0, 0);
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)EFFECT_VIRTUALNODE, 0);
	m_wndList.SetCellColor(index++, 0, RGB(0, 0, 0), RGB(150, 150, 150));


	// ---------------------------
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	GetDlgItem(IDC_CMB_MOO_ANCHOR)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"0");

	for(int i = 0; i <(int) CEffectMgr::GetInstance()->m_anchorInfo->m_AnchorInfo.size(); i++)
	{
		int id = CEffectMgr::GetInstance()->m_anchorInfo->m_AnchorInfo[i].id;
		if(id == 0)
			continue;
		CString str;
		str.Format("%d", id);
		GetDlgItem(IDC_CMB_MOO_ANCHOR)->SendMessage(CB_ADDSTRING, 0, (LPARAM)str.GetBuffer() );
	}

	Ogre::Model *pmodel = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(pmodel)
	{
		std::vector<Ogre::SequenceDesc>seqarray;
		pmodel->getModelData()->getAllSequence(seqarray);
		for(size_t i=0; i<seqarray.size(); i++)
		{
			CString seqname = cvt.AnimIDToDesc(seqarray[i].id);
			m_cmbAnim.AddString(seqname.GetBuffer());
		}
	}
	else
	{
		GetDlgItem(IDC_RDO_ANIM)->EnableWindow(FALSE);
		m_cmbAnim.EnableWindow(FALSE);
	}

	std::vector<CString> effects;
	cvt.EnumPostEffect(effects);
	for(int i = 0; i < (int) effects.size(); i++)
		GetDlgItem(IDC_CMP_POSTEFFECT)->SendMessage(CB_ADDSTRING, 0, (LPARAM)(effects[i].GetBuffer()));
	GetDlgItem(IDC_CMP_POSTEFFECT)->SetWindowText(effects[0].GetBuffer());

	GetDlgItem(IDC_RDO_EFFECT)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	OnBnClickedRdoEffect();


	return TRUE; 
}

void CLayerInfoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	LVITEM item;
	item.iItem = m_wndList.GetSelectionMark();
	item.mask = LVIF_PARAM;
	m_wndList.GetItem(&item);

	//m_iSel = (int)item.lParam;

	m_iSel = m_wndList.GetSelectionMark();
	CCvtHelper cvt;
	UpdateData();
	m_rotate.identity();

	if(m_type == Ogre::MOT_ANIM)
	{
		CString anim;
		m_cmbAnim.GetWindowText(anim);
		//sscanf(anim.GetBuffer(), "%d", &m_iAnim);
		m_iAnim = cvt.DescToAnimID(anim);

	}
	if(m_type == Ogre::MOT_POSTEFFECT)
	{
		m_postEffect = cvt.StringToPostEffect(m_strPostEffectName);
	}

	if(m_type == Ogre::MOT_MODEL && m_iSel >= 0)
	{
		int iStaticEffectCount = 0;
		for(int i = 0; i <(int) CEffectMgr::GetInstance()->m_Effects.size(); i++)
		{
			if(CEffectMgr::GetInstance()->m_Effects[i]->GetBindInfo()->bDynamic == false)
			{

				if(m_iSel == iStaticEffectCount)
				{
					m_iSel = i;
					CDialog::OnOK();
					return;

				}
				iStaticEffectCount++;
			}
		}

		//if(m_iSel < iStaticEffectCount)
		//{
		//	CDialog::OnOK();
		//}
		//else
		//{
		m_iSel = -1;
		m_newType = (EFFECT_TYPE)item.lParam;
		CDialog::OnOK();
		//}
	}
	else if(m_type == Ogre::MOT_ANIM && m_iAnim >= 0)
	{
		CDialog::OnOK();
	}
	else if(m_type == Ogre::MOT_POSTEFFECT)
	{
		CDialog::OnOK();
	}
	else
	{
		MessageBox("没有任何特效被选择");
	}
}

void CLayerInfoDlg::OnBnClickedRdoEffect()
{
	GetDlgItem(IDC_LIST_EFFECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMB_MOO_ANCHOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MOO_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MOO_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MOO_Z)->EnableWindow(TRUE);

	m_cmbAnim.EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_PRIORITY)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHK_MOO_OVERWRITE)->EnableWindow(FALSE);

	GetDlgItem(IDC_CMP_POSTEFFECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_POSTSETTING)->EnableWindow(FALSE);

	m_type = Ogre::MOT_MODEL;
}

void CLayerInfoDlg::OnBnClickedRdoAnim()
{
	GetDlgItem(IDC_LIST_EFFECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMB_MOO_ANCHOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_Z)->EnableWindow(FALSE);

	m_cmbAnim.EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MOO_PRIORITY)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHK_MOO_OVERWRITE)->EnableWindow(TRUE);

	GetDlgItem(IDC_CMP_POSTEFFECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_POSTSETTING)->EnableWindow(FALSE);

	m_type = Ogre::MOT_ANIM;
}

void CLayerInfoDlg::OnBnClickedRdoPost()
{
	GetDlgItem(IDC_LIST_EFFECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMB_MOO_ANCHOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_Z)->EnableWindow(FALSE);

	m_cmbAnim.EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MOO_PRIORITY)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHK_MOO_OVERWRITE)->EnableWindow(FALSE);

	GetDlgItem(IDC_CMP_POSTEFFECT)->EnableWindow(TRUE);
	//OnCbnSelchangeCmpPosteffect();

	m_type = Ogre::MOT_POSTEFFECT;
}



void CLayerInfoDlg::OnBnClickedChkMooNostart()
{
	if(BST_CHECKED == GetDlgItem(IDC_CHK_MOO_NOSTART)->SendMessage(BM_GETCHECK))
	{
		GetDlgItem(IDC_EDIT_MOO_START)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_EDIT_MOO_START)->EnableWindow(TRUE);
	}

}

void CLayerInfoDlg::OnBnClickedChkMooNoend()
{
	if(BST_CHECKED == GetDlgItem(IDC_CHK_MOO_NOEND)->SendMessage(BM_GETCHECK))
	{
		GetDlgItem(IDC_EDIT_MOO_END)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_EDIT_MOO_END)->EnableWindow(TRUE);
	}

}

void CLayerInfoDlg::OnCbnSelchangeCmpPosteffect()
{
	CString effect;
	GetDlgItem(IDC_CMP_POSTEFFECT)->GetWindowText(effect);


	if((effect == "画面泛色" || effect == "角色透明") && m_type == Ogre::MOT_POSTEFFECT)
	{
		GetDlgItem(IDC_BTN_POSTSETTING)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_POSTSETTING)->EnableWindow(FALSE);
	}
}



void CLayerInfoDlg::OnLvnItemchangedListEffect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(((pNMLV->uOldState & LVIS_SELECTED) == 0)
		&& ((pNMLV->uNewState & LVIS_SELECTED) != 0))
	{
		int index = (int)pNMLV->iItem;
		if(index >= 0 && index < (int)CEffectMgr::GetInstance()->m_Effects.size())
		{
			CEffectUnit* pUnit = CEffectMgr::GetInstance()->m_Effects[index];

			m_fx = pUnit->GetBindInfo()->offset.x;
			m_fy = pUnit->GetBindInfo()->offset.y;
			m_fz = pUnit->GetBindInfo()->offset.z;

			m_strAnchor.Format("%d", pUnit->GetBindInfo()->boneid);

			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}

void CLayerInfoDlg::OnBnClickedBtnPostsetting()
{
	CString effect;
	GetDlgItem(IDC_CMP_POSTEFFECT)->GetWindowText(effect);

	if(effect == "画面泛色")
	{
		CColorKeysDlg dlg;
		dlg.m_color = m_fsColor;
		if(IDOK == dlg.DoModal())
		{
			m_fsColor = dlg.m_color;
		}
	}
}