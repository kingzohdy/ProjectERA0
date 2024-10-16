// ModelInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "ModelInfoDlg.h"
#include "TerrainEdit.h"
#include "EditorModel.h"

using namespace TE;
// CModelInfoDlg dialog

IMPLEMENT_DYNAMIC(CModelInfoDlg, CDialog)

static char *ModelTypeNames[] = 
{
	"大型物件",
	"小型物件",
	"细节物件",
	"植被",
	"草层",
	"拼接模型",
	"拼接贴图",
	"纹理",
	"拼接水",
	"物理模型",
	"全局物件"
};

static int ModelTypeName2Index(const char *pname)
{
	for(int i=0; i<TYPE_MODEL_NUM; i++)
	{
		if(strcmp(ModelTypeNames[i], pname) == 0)
		{
			return i;
		}
	}
	assert(0);
	return -1;
}

CModelInfoDlg::CModelInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModelInfoDlg::IDD, pParent)
{
	m_bDataInit = false;
}

CModelInfoDlg::~CModelInfoDlg()
{
}

void CModelInfoDlg::InitData()
{
	m_bDataInit = false;
	if(!g_TerrainEdit.getSelectModel())
	{
		return;
	}
	TE::TerrModelData *pmodeldata = g_TerrainEdit.getSelectModel()->GetModelData();

	Ogre::Vector3 pos = pmodeldata->pos;
	SetDlgItemInt(IDC_EDIT_MODELINFO_X, pos.x);
	SetDlgItemInt(IDC_EDIT_MODELINFO_Y, pos.y);
	SetDlgItemInt(IDC_EDIT_MODELINFO_Z, pos.z);

	Ogre::Vector3 dir = pmodeldata->dir;
	m_PitchSpin.SetPos((int)dir.x);
	m_YawSpin.SetPos((int)dir.y);
	m_RollSpin.SetPos((int)dir.z);

	float scale = pmodeldata->scale;
	m_ScaleSpin.SetPos((int)(scale*100));

	SetDlgItemText(IDC_EDIT_MODELINFO_PATH, pmodeldata->name);

	int sel = m_TypeList.FindString(-1, ModelTypeNames[pmodeldata->cType]);
	m_TypeList.SetCurSel(sel);

	m_AnimList.ResetContent();
	if(pmodeldata->pModel)
	{
		std::vector<Ogre::ACTION_INFO> actinfo;
		pmodeldata->pModel->GetShowModel()->getActionList(actinfo);

		for(size_t i=0; i<actinfo.size(); i++)
		{
			m_AnimList.AddString((const char *)actinfo[i].name);
		}
	}
	m_AnimList.SetCurSel(0);

	m_ReflectedBtn.SetCheck(pmodeldata->reflected?1:0);
	m_RefractedBtn.SetCheck(pmodeldata->refracted?1:0);
	m_TransableBtn.SetCheck(pmodeldata->transable?1:0);

	m_bDataInit = true;
}

void CModelInfoDlg::SavePosData()
{
	if(!m_bDataInit) 
		return;

	Ogre::Vector3 pos, dir;
	pos.x = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_X);
	pos.y = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_Y);
	pos.z = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_Z);

	dir.x = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_PITCH);
	dir.y = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_YAW);
	dir.z = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_ROLL);

	float scale = (float)GetDlgItemInt(IDC_EDIT_MODELINFO_SCALE);

	g_TerrainEdit.ModifySelectModelPos(pos);
	g_TerrainEdit.ModifySelectModelDir(dir);
	g_TerrainEdit.ModifySelectModelScale(scale);
	g_TerrainEdit.UpdateProcessData();
}



void CModelInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_MODELINFO_PITCH, m_PitchSpin);
	DDX_Control(pDX, IDC_SPIN_MODELINFO_YAW, m_YawSpin);
	DDX_Control(pDX, IDC_SPIN_MODELINFO_ROLL, m_RollSpin);
	DDX_Control(pDX, IDC_SPIN_MODELINFO_SCALE, m_ScaleSpin);
	DDX_Control(pDX, IDC_COMBO_MODELINFO_TYPE, m_TypeList);
	DDX_Control(pDX, IDC_COMBO_MODELINFO_ANIM, m_AnimList);
	DDX_Control(pDX, IDC_EDIT_MODELINFO_PATH, m_PathEdit);
	DDX_Control(pDX, IDC_CHECK_REFLECTED, m_ReflectedBtn);
	DDX_Control(pDX, IDC_CHECK_REFRACTED, m_RefractedBtn);
	DDX_Control(pDX, IDC_CHECK_TRANSABLE, m_TransableBtn);
}


BEGIN_MESSAGE_MAP(CModelInfoDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_X, &CModelInfoDlg::OnEnChangeEditModelinfoX)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_Y, &CModelInfoDlg::OnEnChangeEditModelinfoY)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_Z, &CModelInfoDlg::OnEnChangeEditModelinfoZ)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_PITCH, &CModelInfoDlg::OnEnChangeEditModelinfoPitch)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_YAW, &CModelInfoDlg::OnEnChangeEditModelinfoYaw)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_ROLL, &CModelInfoDlg::OnEnChangeEditModelinfoRoll)
	ON_EN_CHANGE(IDC_EDIT_MODELINFO_SCALE, &CModelInfoDlg::OnEnChangeEditModelinfoScale)
	ON_CBN_SELCHANGE(IDC_COMBO_MODELINFO_TYPE, &CModelInfoDlg::OnCbnSelchangeComboModelinfoType)
	ON_CBN_SELCHANGE(IDC_COMBO_MODELINFO_ANIM, &CModelInfoDlg::OnCbnSelchangeComboModelinfoAnim)
	ON_BN_CLICKED(IDC_CHECK_REFLECTED, &CModelInfoDlg::OnBnClickedCheckReflected)
	ON_BN_CLICKED(IDC_CHECK_REFRACTED, &CModelInfoDlg::OnBnClickedCheckRefracted)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MODELINFO_PITCH, &CModelInfoDlg::OnDeltaposSpinModelinfoPitch)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MODELINFO_YAW, &CModelInfoDlg::OnDeltaposSpinModelinfoYaw)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_TRANSABLE, &CModelInfoDlg::OnBnClickedCheckTransable)
END_MESSAGE_MAP()


// CModelInfoDlg message handlers

void CModelInfoDlg::OnEnChangeEditModelinfoX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnEnChangeEditModelinfoY()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnEnChangeEditModelinfoZ()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnEnChangeEditModelinfoPitch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnEnChangeEditModelinfoYaw()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnEnChangeEditModelinfoRoll()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnEnChangeEditModelinfoScale()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SavePosData();
}

void CModelInfoDlg::OnCbnSelchangeComboModelinfoType()
{
	// TODO: Add your control notification handler code here
	if (g_TerrainEdit.getSelectModel())
	{	
		CString str;
		m_TypeList.GetLBText(m_TypeList.GetCurSel(), str);
		g_TerrainEdit.getSelectModel()->GetModelData()->cType = ModelTypeName2Index(str.GetBuffer());
	}
}

void CModelInfoDlg::OnCbnSelchangeComboModelinfoAnim()
{
	// TODO: Add your control notification handler code here
	CString str;
	m_AnimList.GetLBText(m_AnimList.GetCurSel(), str);

	TerrModel *pselmodel = g_TerrainEdit.getSelectModel();
	if(pselmodel)
	{
		pselmodel->GetModelData()->pModel->GetShowModel()->stopMotion(m_CurAnimName.GetBuffer());
		pselmodel->GetModelData()->pModel->GetShowModel()->playMotion(str.GetBuffer());
		m_CurAnimName = str;
	}
}

BOOL CModelInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_TypeList.AddString(ModelTypeNames[TYPE_BIG_MODEL]);
	m_TypeList.AddString(ModelTypeNames[TYPE_SMALL_MODEL]);
	m_TypeList.AddString(ModelTypeNames[TYPE_DETAIL_MODEL]);
	m_TypeList.AddString(ModelTypeNames[TYPE_PLANT_MODEL]);
	m_TypeList.AddString(ModelTypeNames[TYPE_GLOBAL_MODEL]);

	m_PitchSpin.SetRange(0, 360);
	m_YawSpin.SetRange(0, 360);
	m_RollSpin.SetRange(0, 360);
	m_ScaleSpin.SetRange(1, 1000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModelInfoDlg::OnBnClickedCheckReflected()
{
	// TODO: Add your control notification handler code here
	TerrModel *pselmodel = g_TerrainEdit.getSelectModel();
	if(pselmodel)
	{
		pselmodel->GetModelData()->reflected = m_ReflectedBtn.GetCheck()!=0;
		pselmodel->ResetMat();
	}
}

void CModelInfoDlg::OnBnClickedCheckRefracted()
{
	// TODO: Add your control notification handler code here
	TerrModel *pselmodel = g_TerrainEdit.getSelectModel();
	if(pselmodel)
	{
		pselmodel->GetModelData()->refracted = m_RefractedBtn.GetCheck()!=0;
		pselmodel->ResetMat();
	}
}

void CModelInfoDlg::OnDeltaposSpinModelinfoPitch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CModelInfoDlg::OnDeltaposSpinModelinfoYaw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CModelInfoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	g_TerrainEdit.m_bShowModelInfoDlg = false;

	CDialog::OnClose();
}

void CModelInfoDlg::OnBnClickedCheckTransable()
{
	// TODO: Add your control notification handler code here
	TerrModel *pselmodel = g_TerrainEdit.getSelectModel();
	if(pselmodel)
	{
		pselmodel->GetModelData()->transable = m_TransableBtn.GetCheck()!=0;
		pselmodel->ResetMat();
	}
}
