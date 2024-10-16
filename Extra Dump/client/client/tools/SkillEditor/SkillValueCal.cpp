// SkillValueCal.cpp : implementation file
//

#include "stdafx.h"
#include "SkillEditor.h"
#include "SkillValueCal.h"
#include "GameDBData.h"

// SkillValueCal dialog

IMPLEMENT_DYNAMIC(SkillValueCal, CDialog)

SkillValueCal::SkillValueCal(CWnd* pParent /*=NULL*/)
	: CDialog(SkillValueCal::IDD, pParent), m_nAttackerAddEquip(0)
	, m_nHitAddEquip(0), m_nAtkAmmCount(0), m_nHitAmmCount(0), m_nAttackerLv(1)
	, m_nAttackMonsterId(0), m_nHitMonsterId(0), m_nAttackSkillId(0), m_nAttackSkillLv(1)
{
	ZeroMemory( &m_Input, sizeof(CaclInput) );
	ZeroMemory( &m_OutPut, sizeof(CaclOutput) );
}

SkillValueCal::~SkillValueCal()
{
}

void SkillValueCal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT2, m_nAttackerAddEquip);
	DDX_Text(pDX, IDC_EDIT5, m_nHitAddEquip);
	DDX_Text(pDX, IDC_EDIT7, m_nAttackerLv);
	DDX_Text(pDX, IDC_EDIT3, m_nAttackMonsterId);
	DDX_Text(pDX, IDC_EDIT6, m_nHitMonsterId);
	DDX_Text(pDX, IDC_EDIT4, m_nAttackSkillId);
	DDX_Text(pDX, IDC_EDIT1, m_nAttackSkillLv);
}


BEGIN_MESSAGE_MAP(SkillValueCal, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedAttackPlayer)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedAttackMonster)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedHitPlayer)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedHitMonster)
	ON_BN_CLICKED(IDC_BUTTON1, &SkillValueCal::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SkillValueCal::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &SkillValueCal::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &SkillValueCal::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &SkillValueCal::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &SkillValueCal::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &SkillValueCal::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &SkillValueCal::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON4, &SkillValueCal::OnBnClickedButton4)
END_MESSAGE_MAP()

void SkillValueCal::OnBnClickedAttackPlayer()
{
	GetDlgItem( IDC_EDIT2 )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON1 )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON2 )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON3 )->EnableWindow( TRUE );
	GetDlgItem( IDC_LIST4 )->EnableWindow( TRUE );
	GetDlgItem( IDC_EDIT7 )->EnableWindow( TRUE );

	GetDlgItem( IDC_EDIT3 )->EnableWindow( FALSE );
	m_Input.stAtker.iObjType = OBJ_PLAYER;

	if( m_HurtDef.empty() )
	{
		DBDataManager::getSingleton().loadArmHurtDef( m_HurtDef );
	}
}

void SkillValueCal::OnBnClickedAttackMonster()
{
	GetDlgItem( IDC_EDIT2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON3 )->EnableWindow( FALSE );
	GetDlgItem( IDC_LIST4 )->EnableWindow( FALSE );
	GetDlgItem( IDC_EDIT7 )->EnableWindow( FALSE );

	GetDlgItem( IDC_EDIT3 )->EnableWindow( TRUE );
	m_Input.stAtker.iObjType = OBJ_MONSTER;

	if( m_HurtDef.empty() )
	{
		DBDataManager::getSingleton().loadArmHurtDef( m_HurtDef );
	}
}

void SkillValueCal::OnBnClickedHitPlayer()
{
	GetDlgItem( IDC_EDIT5 )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON5 )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON6 )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON7 )->EnableWindow( TRUE );
	GetDlgItem( IDC_LIST5 )->EnableWindow( TRUE );

	GetDlgItem( IDC_EDIT6 )->EnableWindow( FALSE );
	m_Input.stAtkee.iObjType = OBJ_PLAYER;

	if( m_HurtDef.empty() )
	{
		DBDataManager::getSingleton().loadArmHurtDef( m_HurtDef );
	}
}

void SkillValueCal::OnBnClickedHitMonster()
{
	GetDlgItem( IDC_EDIT5 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON5 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON6 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON7 )->EnableWindow( FALSE );
	GetDlgItem( IDC_LIST5 )->EnableWindow( FALSE );

	GetDlgItem( IDC_EDIT6 )->EnableWindow( TRUE );
	m_Input.stAtkee.iObjType = OBJ_MONSTER;

	if( m_HurtDef.empty() )
	{
		DBDataManager::getSingleton().loadArmHurtDef( m_HurtDef );
	}
}


// SkillValueCal message handlers
// 添加装备
void SkillValueCal::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nAttackerAddEquip == 0 ) return;
	if( m_nAtkAmmCount >= MAX_WEAR ) return;

	if( m_ItemDefMap.find( m_nAttackerAddEquip ) != m_ItemDefMap.end() )
	{
		m_Input.stAtker.stData.stPlayerAtk.pastArm[m_nAtkAmmCount] = &m_ItemDefMap[m_nAttackerAddEquip];
	}else{
		m_ItemDefMap[m_nAttackerAddEquip] = DBDataManager::getSingleton().getItemDef( m_nAttackerAddEquip );
		m_Input.stAtker.stData.stPlayerAtk.pastArm[m_nAtkAmmCount] = &m_ItemDefMap[m_nAttackerAddEquip];
		
	}
	
	m_nAtkAmmCount++;
	((CListBox*)GetDlgItem( IDC_LIST4 ))->ResetContent();
	for( int i = 0; i < m_nAtkAmmCount; i++ )
	{
		ITEMDEF* pItemDef = m_Input.stAtker.stData.stPlayerAtk.pastArm[i];
		char buf[256];
		sprintf( buf, "ID:%d 名称:%s", pItemDef->ItemID, pItemDef->Name );
		((CListBox*)GetDlgItem( IDC_LIST4 ))->AddString( buf );
	}
}

void SkillValueCal::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( m_nAttackerAddEquip == 0 ) return;

	for( int i = 0; i < m_nAtkAmmCount; i++ )
	{
		ITEMDEF* pItemDef = m_Input.stAtker.stData.stPlayerAtk.pastArm[i];
		if( m_nAttackerAddEquip == pItemDef->ItemID )
		{
			m_nAtkAmmCount--;
			m_Input.stAtker.stData.stPlayerAtk.pastArm[i] = NULL;
			for( int j = i; j < MAX_WEAR; j++ )
			{
				if( j +1 < MAX_WEAR )
				{
					std::swap( m_Input.stAtker.stData.stPlayerAtk.pastArm[j], 
						m_Input.stAtker.stData.stPlayerAtk.pastArm[j+1]);
				}
			}

			if( m_ItemDefMap[m_nAttackerAddEquip].ItemID/100000 < 20 )
			{
				int arm_sub_type = m_ItemDefMap[m_nAttackerAddEquip].ItemID/10000;
				if( arm_sub_type == m_HurtDef[i].ArmType )
				{
					m_Input.stAtker.stData.stPlayerAtk.pstArmLevelHurtDef = NULL;
				}
			}
		}
	}
	
	((CListBox*)GetDlgItem( IDC_LIST4 ))->ResetContent();
	for( int i = 0; i < m_nAtkAmmCount; i++ )
	{
		ITEMDEF* pItemDef = m_Input.stAtker.stData.stPlayerAtk.pastArm[i];
		if( pItemDef != NULL )
		{
			char buf[256];
			sprintf( buf, "ID:%d 名称:%s", pItemDef->ItemID, pItemDef->Name );
			((CListBox*)GetDlgItem( IDC_LIST4 ))->AddString( buf );
		}
	}
}

void SkillValueCal::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	for( int i = 0; i < m_nAtkAmmCount; i++ )
	{
		m_Input.stAtker.stData.stPlayerAtk.pastArm[i] = NULL;
	}
	m_Input.stAtker.stData.stPlayerAtk.pstArmLevelHurtDef = NULL;
	m_nAtkAmmCount = 0;
	((CListBox*)GetDlgItem( IDC_LIST4 ))->ResetContent();
}

void SkillValueCal::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	
	if( m_nHitAddEquip == 0 ) return;
	if( m_nHitAmmCount >= MAX_WEAR ) return;

	if( m_ItemDefMap.find( m_nHitAddEquip ) != m_ItemDefMap.end() )
	{
		m_Input.stAtkee.stData.stPlayerAtk.pastArm[m_nHitAmmCount] = &m_ItemDefMap[m_nHitAddEquip];
	}else{
		m_ItemDefMap[m_nHitAddEquip] = DBDataManager::getSingleton().getItemDef( m_nHitAddEquip );
		m_Input.stAtkee.stData.stPlayerAtk.pastArm[m_nHitAmmCount] = &m_ItemDefMap[m_nHitAddEquip];
	}

	m_nHitAmmCount++;
	((CListBox*)GetDlgItem( IDC_LIST5 ))->ResetContent();
	for( int i = 0; i < m_nHitAmmCount; i++ )
	{
		ITEMDEF* pItemDef = m_Input.stAtkee.stData.stPlayerAtk.pastArm[i];
		char buf[256];
		sprintf( buf, "ID:%d 名称:%s", pItemDef->ItemID, pItemDef->Name );
		((CListBox*)GetDlgItem( IDC_LIST5 ))->AddString( buf );
	}
}

void SkillValueCal::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( m_nHitAddEquip == 0 ) return;

	for( int i = 0; i < m_nHitAmmCount; i++ )
	{
		ITEMDEF* pItemDef = m_Input.stAtkee.stData.stPlayerAtk.pastArm[i];
		if( m_nHitAddEquip == pItemDef->ItemID )
		{
			m_nHitAmmCount--;
			m_Input.stAtkee.stData.stPlayerAtk.pastArm[i] = NULL;
			for( int j = i; j < MAX_WEAR; j++ )
			{
				if( j +1 < MAX_WEAR )
				{
					std::swap( m_Input.stAtkee.stData.stPlayerAtk.pastArm[j], 
						m_Input.stAtkee.stData.stPlayerAtk.pastArm[j+1]);
				}
			}
		}
	}

	((CListBox*)GetDlgItem( IDC_LIST5 ))->ResetContent();
	for( int i = 0; i < m_nHitAmmCount; i++ )
	{
		ITEMDEF* pItemDef = m_Input.stAtkee.stData.stPlayerAtk.pastArm[i];
		if( pItemDef != NULL )
		{
			char buf[256];
			sprintf( buf, "ID:%d 名称:%s", pItemDef->ItemID, pItemDef->Name );
			((CListBox*)GetDlgItem( IDC_LIST5 ))->AddString( buf );
		}
	}
}

void SkillValueCal::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	for( int i = 0; i < m_nHitAmmCount; i++ )
	{
		m_Input.stAtkee.stData.stPlayerAtk.pastArm[i] = NULL;
	}
	m_nHitAmmCount = 0;
	((CListBox*)GetDlgItem( IDC_LIST5 ))->ResetContent();
}

void SkillValueCal::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( m_nAttackMonsterId == 0 ) return;

	const MONSTERDEF& monsterDef= DBDataManager::getSingleton().getMonsterDef( m_nAttackMonsterId );
	char buf[256];
	((CListBox*)GetDlgItem( IDC_LIST2 ))->ResetContent();
	sprintf( buf, "怪物ID:%d  名称:%s", monsterDef.MonsterID, monsterDef.Name );
	((CListBox*)GetDlgItem( IDC_LIST2 ))->AddString( buf );
	sprintf( buf, "最大血量:%d", monsterDef.HP );
	((CListBox*)GetDlgItem( IDC_LIST2 ))->AddString( buf );
}

void SkillValueCal::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	if( m_nHitMonsterId == 0 ) return;

	const MONSTERDEF& monsterDef= DBDataManager::getSingleton().getMonsterDef( m_nHitMonsterId );
	char buf[256];
	((CListBox*)GetDlgItem( IDC_LIST1 ))->ResetContent();
	sprintf( buf, "怪物ID:%d  名称:%s", monsterDef.MonsterID, monsterDef.Name );
	((CListBox*)GetDlgItem( IDC_LIST1 ))->AddString( buf );
	sprintf( buf, "最大血量:%d", monsterDef.HP );
	((CListBox*)GetDlgItem( IDC_LIST1 ))->AddString( buf );
}

void SkillValueCal::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( OBJ_UNKNOW == m_Input.stAtker.iObjType )
	{
		::MessageBox(NULL, "请选择攻击方", "错误", MB_OK);
		return;
	}
	
	if( OBJ_UNKNOW == m_Input.stAtker.iObjType)
	{
		::MessageBox(NULL, "请选择受击方", "错误", MB_OK);
		return;
	}

	if( m_nAttackSkillId == 0 )
	{
		::MessageBox(NULL, "请输入攻击技能", "错误", MB_OK);
		return;
	}
	
	const SKILLDEF &skillDef = DBDataManager::getSingleton().GetSkillDef( m_nAttackSkillId, m_nAttackSkillLv );
	if( m_Input.stAtker.iObjType == OBJ_PLAYER )
	{
		m_Input.stAtker.stData.stPlayerAtk.iArm			= m_nAtkAmmCount;
		m_Input.stAtker.stData.stPlayerAtk.pstSkillDef	= &skillDef;
		
		for( int j = 0; j < m_nAtkAmmCount; j++ )
		{
			if( m_Input.stAtker.stData.stPlayerAtk.pastArm[j]->ItemID/100000 < 20 )
			{
				for( int i = 0; i < m_HurtDef.size(); i++ )
				{
					if( m_Input.stAtker.stData.stPlayerAtk.pastArm[j] != NULL )
					{ 
						int arm_sub_type = m_Input.stAtker.stData.stPlayerAtk.pastArm[j]->ItemID/10000;
						if( arm_sub_type == m_HurtDef[i].ArmType && m_nAttackerLv == m_HurtDef[i].Level )
						{
							m_Input.stAtker.stData.stPlayerAtk.pstArmLevelHurtDef = &m_HurtDef[i];
						}
					}
				}
			}
		}
	}else if( m_Input.stAtker.iObjType == OBJ_MONSTER )
	{
		if( m_nAttackMonsterId == 0 ) 
		{
			::MessageBox(NULL, "请输入攻击怪物ID", "错误", MB_OK);
			return;
		}
		
		MONSTERDEF monsterDef = DBDataManager::getSingleton().getMonsterDef( m_nAttackMonsterId ); 
		m_Input.stAtker.stData.stMonAtk.pstMonDef	= &monsterDef;
		m_Input.stAtker.stData.stMonAtk.pstSkillDef	= &skillDef;
	}
	
	if( m_Input.stAtkee.iObjType == OBJ_PLAYER )
	{
		m_Input.stAtkee.stData.stPlayerAtk.iArm			= m_nHitAmmCount;
	}else if( m_Input.stAtkee.iObjType == OBJ_MONSTER )
	{
		if( m_nHitMonsterId == 0 ) 
		{
			::MessageBox(NULL, "请输入受击怪物ID", "错误", MB_OK);
			return;
		}
		MONSTERDEF monsterDef= DBDataManager::getSingleton().getMonsterDef( m_nHitMonsterId ); 
		m_Input.stAtkee.stData.stMonAtk.pstMonDef	= &monsterDef;
	}

	int nReslut = calc_val( &m_Input, &m_OutPut );
	char buf[256];
	((CListBox*)GetDlgItem( IDC_LIST3 ))->ResetContent();
	if( nReslut == 0 )
	{
		sprintf( buf, "最小伤害:%d", m_OutPut.iHurtMin );
		((CListBox*)GetDlgItem( IDC_LIST3 ))->AddString( buf );
		sprintf( buf, "最大伤害:%d", m_OutPut.iHurtMax );
		((CListBox*)GetDlgItem( IDC_LIST3 ))->AddString( buf );
		sprintf( buf, "命中率:%d", m_OutPut.iHitRate );
		((CListBox*)GetDlgItem( IDC_LIST3 ))->AddString( buf );
		sprintf( buf, "爆击率:%d", m_OutPut.iHeavyRate );
		((CListBox*)GetDlgItem( IDC_LIST3 ))->AddString( buf );
		sprintf( buf, "攻击频数:%d", m_OutPut.iContinueNum );
		((CListBox*)GetDlgItem( IDC_LIST3 ))->AddString( buf );
	}else{
		sprintf( buf, "输入错误:%s", m_OutPut.szError );
		((CListBox*)GetDlgItem( IDC_LIST3 ))->AddString( buf );
	}
}
