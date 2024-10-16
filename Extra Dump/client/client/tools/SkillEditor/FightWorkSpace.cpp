#include "StdAfx.h"
#include "FightWorkSpace.h"
#include "NotifyProperty.h"
#include "EnginAgent.h"
#include "GameDBData.h"
#include "resdb_meta.h"
#include "GameMap.h"
#include "GameActor.h"
#include "GameSkill_Common.h"
#include "GameSkill_Control.h"
#include "GameSkill_Mgr.h"
#include "Item.h"
#include "GameSkill_Package.h"

const int MAX_MAP = 100;
static int Map_ProcId[MAX_MAP][2] = {
	{ 2300, 1 },
	{ 2501, 1 },
	{ 3301, 1 },
	{ 1002, 2 },
};

int findProcId( int mapId )
{
	for( int i = 0; i < MAX_MAP; i++ )
	{
		if( Map_ProcId[i][0] == mapId ) return Map_ProcId[i][1];
	}
	return 0;
}

static std::string strPosName[EQUIP_POS_FASHION] = 
{
	"右手武器","头部","衣服","背部","左手武器","脸部","肩膀", "时装"
};
static std::string strCurPos = "";

BEGIN_MESSAGE_MAP(CFightWorkSpace, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
END_MESSAGE_MAP()

CFightWorkSpace::CFightWorkSpace(void)
	:	m_nEquipId(0), 
		m_nModelId(0),
		m_nMapPosX(0),
		m_nMapPosY(0),
		m_nMonsterId(0),
		m_nMonsterDyId(0),
		m_nMonsterTargetId(0),
		m_nMonsterUsedSkill(0)
{
	memset( m_skillId, 0, sizeof(m_skillId) );
}

CFightWorkSpace::~CFightWorkSpace(void)
{
}

int CFightWorkSpace::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_PROPLIST_FIGHT_PROP);
	
	m_wndPropList.EnableHeaderCtrl(FALSE);
	
	//////////////////////////////////////////////////////////////////////////
	CBCGPProp* pPos = new CBCGPProp("场景位置");
	UpdateDispProp( CREATE_PROP );
	
	pPos->AddSubItem(m_pPropMapId );
	pPos->AddSubItem(m_pPropPosX);
	pPos->AddSubItem(m_pPropPosY);

	m_pPropMapId->SetNotifyTarget( GetSafeHwnd() );
	m_pPropPosX->SetNotifyTarget( GetSafeHwnd() );
	m_pPropPosY->SetNotifyTarget( GetSafeHwnd() );
	
	//////////////////////////////////////////////////////////////////////////
	CBCGPProp* pEquipPos = new CBCGPProp("对象装备/变身");
	for( int i = 0; i < EQUIP_POS_FASHION; i++ )
	{
		m_pEquipPos->AddOption( strPosName[i].c_str() );
	}
	pEquipPos->AddSubItem(m_pEquipPos);
	pEquipPos->AddSubItem(m_pEquipId);
	pEquipPos->AddSubItem(m_pModelId);

	m_pModelId->SetNotifyTarget( GetSafeHwnd() );
	m_pEquipPos->SetNotifyTarget( GetSafeHwnd() );
	m_pEquipId->SetNotifyTarget( GetSafeHwnd() );
	//////////////////////////////////////////////////////////////////////////
	CBCGPProp* pMonserAdd = new CBCGPProp("种怪");
	pMonserAdd->AddSubItem(m_pMonsterId);
	m_pMonsterId->SetNotifyTarget( GetSafeHwnd() );
	//////////////////////////////////////////////////////////////////////////
	CBCGPProp* pAccelKeySetting = new CBCGPProp("技能快捷键设置");
	for( int i = 0; i < 12; i++ )
	{
		pAccelKeySetting->AddSubItem(m_pAccelKey[i]);
		m_pAccelKey[i]->SetNotifyTarget( GetSafeHwnd() );
	}
	//////////////////////////////////////////////////////////////////////////
	CBCGPProp* pMonseterAI= new CBCGPProp("怪物AI");
	pMonseterAI->AddSubItem(m_pMonsterDyId);
	pMonseterAI->AddSubItem(m_pMonsterAISelect);
	pMonseterAI->AddSubItem(m_pMonsterTargetId);
	pMonseterAI->AddSubItem(m_pMonsterUsedSkill);

	m_pMonsterAISelect->AddOption("不动");
	m_pMonsterAISelect->AddOption("指定目标");
	m_pMonsterAISelect->SetNotifyTarget( GetSafeHwnd() );
	//////////////////////////////////////////////////////////////////////////
	m_wndPropList.AddProperty(pPos);
	m_wndPropList.AddProperty(pEquipPos);
	m_wndPropList.AddProperty(pMonserAdd);
	m_wndPropList.AddProperty(pAccelKeySetting);
	m_wndPropList.AddProperty(pMonseterAI);

	m_nMapId = FRIST_MAP_ID;
	UpdateDispProp( DATA_TO_PROP );
	return 0;
}

void CFightWorkSpace::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top , SWP_NOZORDER);
}

void CFightWorkSpace::OnKillFocus(CWnd* pNewWnd)
{
	CBCGPDockingControlBar::OnKillFocus(pNewWnd);
	m_wndPropList.EndEditItem(TRUE);
	// TODO: Add your message handler code here
}

LRESULT CFightWorkSpace::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	UpdateDispProp( PROP_TO_DATA );
	
	if( CEnginAgent::GetInstance()->m_CurMapID != m_nMapId )
	{
		int nProcId = findProcId( m_nMapId );
		if( nProcId != 0 )
		{
			/*const tagProcEvent MapDef = DBDataManager::getSingleton().getProcEvent( m_nMapId, nProcId );
			CEnginAgent::GetInstance()->enterMap( m_nMapId, MapDef.FailMapPos.X, MapDef.FailMapPos.Y );
			m_nMapPosX = MapDef.FailMapPos.X;
			m_nMapPosY = MapDef.FailMapPos.Y;
			UpdateDispProp( DATA_TO_PROP );*/
		}else{
			CEnginAgent::GetInstance()->enterMap( m_nMapId, 0, 0 );
			m_nMapPosX = 0;
			m_nMapPosY = 0;
			UpdateDispProp( DATA_TO_PROP );
		}
	}	
	
	std::string tem = m_strEquipPos;
	if( strCurPos != tem )
	{
		strCurPos = tem;
		m_nEquipId = 0;
		UpdateDispProp( DATA_TO_PROP );
	}else{
		int Pos = 0;
		for( int i = 0; i < EQUIP_POS_FASHION; i++ )
		{
			if( strPosName[i] == tem )
			{
				Pos = i+1;
			}
		}
		if( Pos != 0 )
		{
			CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.m_Arm[Pos].m_ItemType = ITEM_SLOTTYPE_AMM;
			CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.m_Arm[Pos].m_bCanUsed = true;
			CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.m_Arm[Pos].m_ItemData.m_Arm.DefID = m_nEquipId;
			CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.changEquip( Pos, m_nEquipId );
			CEnginAgent::GetInstance()->m_pMainPlayer->getAvatar()->updateAttrib();
		}
	}
	
	if( m_nModelId != 0 )
	{
		CEnginAgent::GetInstance()->m_pMainPlayer->onMorph( m_nModelId );
		CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.clearAllEquip();
		m_nModelId = 0;
		UpdateDispProp( DATA_TO_PROP );
	}

	if( m_nMonsterId != 0 )
	{
		CEnginAgent::GetInstance()->createActor(m_nMonsterId);
		m_nMonsterId = 0;
		UpdateDispProp( DATA_TO_PROP );
	}
	
	if( m_nMonsterDyId != 0 && m_nMonsterTargetId != 0 )
	{
		if( m_nMonsterUsedSkill != 0 )
		{
			if( m_strAISelect == "指定目标" )
			{
				CSHITS Hit;
				Hit.SkillID		= m_nMonsterUsedSkill;
				Hit.Level		= 1;
				Hit.TotalHit	= 1;
				Hit.Hits[0].ID  = m_nMonsterTargetId;
				Hit.Hits[0].HitBackFlag = 0;
				Hit.Hits[0].DeadFlag	= 0;
				Hit.Hits[0].StatusFlag  = 0;

				GameSkill_Package* pPackage = new GameSkill_Package;
				pPackage->setTargetState( m_nMonsterDyId, Hit );

				GameSpell* pSpell = SkillManager::getSingleton().castSpell( m_nMonsterDyId, m_nMonsterUsedSkill, 1, false );
				if( pSpell == NULL ) return 0;

				pSpell->setSingleTarget( m_nMonsterTargetId );
				pSpell->Attack();
				pSpell->setSkillMsgPackage( pPackage );
			}

		}
	}
	return 0;
}

void CFightWorkSpace::UpdateDispProp(DATA_UPDATA_TYPE type)
{
	PropUpdate_int((CBCGPProp**)&m_pPropMapId, &m_nMapId, type, _T("地图ID"));
	PropUpdate_int((CBCGPProp**)&m_pPropPosX, &m_nMapPosX, type, _T("X"));
	PropUpdate_int((CBCGPProp**)&m_pPropPosY, &m_nMapPosY, type, _T("Y"));

	PropUpdate_cstring((CBCGPProp**)&m_pEquipPos, &m_strEquipPos, type, _T("装备位置"));
	PropUpdate_int((CBCGPProp**)&m_pEquipId, &m_nEquipId, type, _T("装备ID"));
	PropUpdate_int((CBCGPProp**)&m_pModelId, &m_nModelId, type, _T("模型ID"));
	
	PropUpdate_int((CBCGPProp**)&m_pMonsterId, &m_nMonsterId, type, _T("怪物ID"));
	
	PropUpdate_cstring((CBCGPProp**)&m_pSkillDyAttribProp, &m_strSkillName, type, _T("技能名"));
	
	for( int i = 0; i < 12; i++ )
	{
		char tem[25];
		sprintf( tem, "F%d", (i+1) );
		PropUpdate_int((CBCGPProp**)&m_pAccelKey[i], &m_skillId[i], type, _T(tem));
	}

	PropUpdate_int((CBCGPProp**)&m_pMonsterDyId, &m_nMonsterDyId, type, _T("选定怪物ID"));
	PropUpdate_cstring((CBCGPProp**)&m_pMonsterAISelect, &m_strAISelect, type, _T("怪物行为指定"));
	PropUpdate_int((CBCGPProp**)&m_pMonsterTargetId, &m_nMonsterTargetId, type, _T("怪物目标ID"));
	PropUpdate_int((CBCGPProp**)&m_pMonsterUsedSkill, &m_nMonsterUsedSkill, type, _T("怪物使用技能ID"));
}

void CFightWorkSpace::initSkillAttribPropValue()
{
	CBCGPProp* pSkillAttribProp = new CBCGPProp("服务器技能数据模拟");
	SKILLDYATTRIB_ITER iter = SkillManager::getSingleton().getSpellControler()->getDyAttrib_begin();
	for( ; iter != SkillManager::getSingleton().getSpellControler()->getDyAttrib_end(); ++iter )
	{	
		const tagSkillDef& skillDef = DBDataManager::getSingleton().GetSkillDef( iter->first, iter->second->SkillLevel );
		char tem[256];
		sprintf( tem, "%d %s", iter->first, skillDef.Name );
		m_pSkillDyAttribProp->AddOption( tem );
	}
	pSkillAttribProp->AddSubItem(m_pSkillDyAttribProp);
	
	m_wndPropList.AddProperty(pSkillAttribProp);
}

void CFightWorkSpace::initMonsterAIPropValue( int SelectId )
{
	m_nMonsterDyId = SelectId;
	UpdateDispProp( DATA_TO_PROP );
}