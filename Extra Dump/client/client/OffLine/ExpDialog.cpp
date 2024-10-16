// ExpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OffLine.h"
#include "ExpDialog.h"
#include "OffLineDlg.h"
#include "MudNetMgr.h"
#include "AvatarEntity.h"
#include "AvatarManager.h"
#include "OgreResourceManager.h"
#include "Equip_Common.h"
using namespace Ogre;


// CExpDialog dialog

IMPLEMENT_DYNAMIC(CExpDialog, CDialog)

CExpDialog::CExpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExpDialog::IDD, pParent)
	, m_cstrAttr(_T(""))
{
	m_fRotYaw = 0.0f;
	m_pEntity = NULL;
}

CExpDialog::~CExpDialog()
{

}

void CExpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACTORSHOW, m_expRenderWnd);
	DDX_Control(pDX, IDC_COMBO_ROLELIST, m_RoleList);
	DDX_Text(pDX, IDC_EDIT_ROLEATTR, m_cstrAttr);
}


BEGIN_MESSAGE_MAP(CExpDialog, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_LEFT, &CExpDialog::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &CExpDialog::OnBnClickedRight)
	ON_WM_CREATE()
	ON_MESSAGE(WM_NET_NOTIFY,&CExpDialog::onNetNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_ROLELIST, &CExpDialog::OnCbnSelchangeComboRolelist)
	ON_BN_CLICKED(IDC_BUTTON3, &CExpDialog::OnBnClickedButton3)
END_MESSAGE_MAP()


// CExpDialog message handlers

void CExpDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

BOOL CExpDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExpDialog::OnBnClickedLeft()
{
	// TODO: Add your control notification handler code here
	if (((COffLineDlg*)GetParent())->m_pExpRenderUnit == 0)
		return;

	m_fRotYaw += 2.5f;

	if (m_pEntity)
	{
		m_pEntity->setRotation(m_fRotYaw, 0.0f, 0.0f);
	}
}

void CExpDialog::OnBnClickedRight()
{
	// TODO: Add your control notification handler code here
	if (((COffLineDlg*)GetParent())->m_pExpRenderUnit == 0)
		return;

	m_fRotYaw -= 2.5f;

	if (m_pEntity)
	{
		m_pEntity->setRotation(m_fRotYaw, 0.0f, 0.0f);
	}
}

int CExpDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	MudNetMgr::getSingleton().addMessageHwnd(GetSafeHwnd());
	AvatarEntity::m_pAvatarMgr = new AvatarManager;
	AvatarEntity::m_pAvatarMgr->init();

	return 0;
}

LRESULT CExpDialog::onNetNotify(WPARAM wParam,LPARAM lParam)
{
	switch( wParam )
	{
	case eRoleList:
		{
			int nNum = MudNetMgr::getSingleton().getRoleNum();
			m_RoleList.ResetContent();
			int nLastLoginTime = 0;
			int nLastLoginIdx = -1;
			for( int i = 0 ; i < nNum ; i++ )
			{
				ROLEINFO& info = MudNetMgr::getSingleton().getRoleInfo(i);
				m_RoleList.AddString( info.RoleName );
				m_RoleList.SetItemDataPtr(i,&info.RoleID);
				if( info.LastLogin >= (unsigned int)nLastLoginTime )
				{
					nLastLoginIdx = i;
					nLastLoginTime = info.LastLogin;
				}
			}
			if( nLastLoginIdx >= 0 )
			{
				m_RoleList.SetCurSel(nLastLoginIdx);
				OnCbnSelchangeComboRolelist();
			}
			GetDlgItem(IDC_BUTTON3)->SetWindowText("登陆");
		}
		break;
	case eRoleLogin:
		{
			CSMUDROLEDETAIL& detail = MudNetMgr::getSingleton().getRoleDetail();
			m_cstrAttr.Empty();
			CString msg;
			//TODO HERE 目前还没有属性发过来。
			for( int i = 0 ; i < detail.PlayerAttrVal.AttrNum ; i++ )
			{
				msg.Format( "%s%d\n",
					MudNetMgr::getSingleton().getAttrName(detail.PlayerAttrVal.AttrList[i].AttrID),
					detail.PlayerAttrVal.AttrList[i].Value );
				m_cstrAttr.Append(msg);
			}
			GetDlgItem(IDC_BUTTON3)->SetWindowText("登出");
			UpdateData(FALSE);
		}
		break;
	case eError:
		{
			if( lParam == eErrorAlreadyLogin )
			{
				AfxMessageBox("该角色已经登陆了。");
			}
		}
		break;
	default:
		break;
	}
	return 1L;
}

void CExpDialog::OnCbnSelchangeComboRolelist()
{
	int nIndex = m_RoleList.GetCurSel();
	memset(m_AvatarAttrib,0,sizeof(m_AvatarAttrib));
	if( nIndex >= 0 )
	{
		ROLEINFO& info = MudNetMgr::getSingleton().getRoleInfo(nIndex);

		for( int i = 0; i < info.Avatar.Num; i++ )
		{
			m_AvatarAttrib[EquipPos_2_PosId[info.Avatar.Arms[i].GridIdx]] = info.Avatar.Arms[i].ArmDefID;
		}
		m_AvatarAttrib[0] = info.Career;
		m_AvatarAttrib[1] = info.Gender-1;
		m_AvatarAttrib[2] = info.Hair;
		m_AvatarAttrib[3] = info.Face;
		m_AvatarAttrib[13] = info.Head;
	}
	UpdateAvatar();
}

void CExpDialog::UpdateAvatar()
{
	RenderUnit* pUint = ((COffLineDlg*)GetParent())->m_pExpRenderUnit;
	if( pUint == NULL )
	{
		return;
	}
	if( m_pEntity != NULL )
	{
		m_pEntity->detachFromScene();
		delete m_pEntity;
		m_pEntity = NULL;
	}
	m_pEntity = new AvatarEntity;
	m_pEntity->updateAttrib(m_AvatarAttrib);
	m_pEntity->attachToScene( pUint->m_pScene );
	Ogre::Vector3  scale;
	scale.x = scale.y = scale.z = 0.8f;
	m_pEntity->setScale(scale);
	m_pEntity->setRotation(m_fRotYaw, 0.0f, 0.0f);
	m_pEntity->playAnim(10100);
	m_pEntity->update(0);
}
void CExpDialog::OnBnClickedButton3()
{
	if( MudNetMgr::getSingleton().getLoginRoleID() != -1 )
	{
		MudNetMgr::getSingleton().requestRoleLogout();
	}else{
		int nIndex = m_RoleList.GetCurSel();
		if( nIndex < 0 )
		{
			AfxMessageBox("请选择登陆角色！");
			return;
		}
		MudNetMgr::getSingleton().requestEnterGameWorld( *(tdr_longlong*)m_RoleList.GetItemDataPtr(nIndex) );
	}
}
