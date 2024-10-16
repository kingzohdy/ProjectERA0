#include "GameActor_MonsterMachine.h"
#include "GameActorManager.h"
#include "NetMessage.h"
#include "GameActor.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "ShowActor.h"
#include "GameSkill_Mgr.h"
#include "GameMsgPrint.h"
#include "GameMainPlayer.h"
#include "ServerTimer.h"
#include "GameActor_PVP.h"
#include "OgreMath.h"
#include "OgreScriptLuaVM.h"

#include <Windows.h>

MonsterMachine::MonsterMachine()
{
	m_nOwnerID						= 0;
	m_monsterInfo.m_nMonsterMemID	= 0;
	m_FromItemWID					= 0;
	m_playerInfo.m_bBind			= false;
}

MonsterMachine::~MonsterMachine()
{

}

tagMonMachineDef_ToLua* MonsterMachine::getMonMachineDef( int nMonsterID )
{
	static_assert(sizeof(tagMonMachineDef) == sizeof(tagMonMachineDef_ToLua));
	return (tagMonMachineDef_ToLua*)DBDataManager::getSingleton().getMonMachineDef(nMonsterID);
}

void MonsterMachine::setOwner( int nOwnerID )
{
	m_nOwnerID = nOwnerID;
}

void MonsterMachine::clearRideMachinInfo()
{
	m_monsterInfo.m_nMonsterDefID	= 0;
	m_monsterInfo.m_nMonsterMemID	= 0;
	m_monsterInfo.m_nCollcetTime	= 0;
	m_monsterInfo.m_nCollcetVal		= 0;
	memset(m_monsterInfo.m_nOre, 0, sizeof(m_monsterInfo.m_nOre));
	m_monsterInfo.m_nMaxOre			= 0;
	m_monsterInfo.m_nEnergy			= 0;
	m_monsterInfo.m_nMaxEnergy		= 0;
	m_monsterInfo.m_nMaxPassenger	= 0;
	memset( &m_monsterInfo.m_OwnerClanWID, 0, sizeof( m_monsterInfo.m_OwnerClanWID ) );
	m_monsterInfo.m_OwnerType		= 0;
	m_playerInfo.m_bBind			= false;
	m_playerInfo.m_nMonsterDefID	= 0;
	SetMonsterMachineID( 0 );
	m_playerInfo.m_nSitIdx			= 0;
	m_nOwnerID						= 0;
	m_monsterInfo.m_nCollectType	= 0;
}

void MonsterMachine::SetMonsterMachineID( int nMachineActorID )
{
	m_playerInfo.m_nMonsterMemID = nMachineActorID;
	if ( m_nOwnerID == 0 )
	{
		return;
	}
	
	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor* pOwner		= actorMgr.FindActor( m_nOwnerID );
	RoleText& actorTitleMgr = pOwner->getRoleText();
	actorTitleMgr.BuildTitle();
}

bool MonsterMachine::isInMonsterMachine()
{
	return m_playerInfo.m_bBind;
}

bool MonsterMachine::canAttackTarget( int nId )
{
	return true;
}

bool MonsterMachine::cancelSkillSelectData()
{
	return false;
}

bool MonsterMachine::setCollectSelectData( int angle )
{
	return false;
}

void MonsterMachine::endCollectSkill()
{
}

void MonsterMachine::useMonsterSkill( int skillID,int skillLv )
{
}

void MonsterMachine::trySendAttackMsg()
{
}

void MonsterMachine::rideOnMonsterMachine( const CSROLEMACHINEINFO& rideOnData )
{
	if ( rideOnData.MachineID == 0 )
	{
		return;
	}

	SetMonsterMachineID( rideOnData.MachineID );
	m_playerInfo.m_nMonsterDefID = rideOnData.MachineDefIdx;
	//TODO HERE 上车的座位	
	m_playerInfo.m_nSitIdx		 = 0;
	GameActor* pMonster = ActorManager::getSingleton().FindActor( rideOnData.MachineID );
	if( pMonster != NULL )
	{
		pMonster->m_MonsterMachine.setOwner( pMonster->GetID() );
		//找下owner有没有在乘客列表，有则不用强制将主驾驶设置为当前owner
		bool findPassenger = false;
		for( int i = 0 ; i < pMonster->m_MonsterMachine.m_monsterInfo.m_nMaxPassenger ; i++ )
		{
			if(pMonster->m_MonsterMachine.m_monsterInfo.m_nPassengerID[i] == m_nOwnerID)
			{
				findPassenger = true;
			}
		}
		if ( !findPassenger )
		{
			pMonster->m_MonsterMachine.m_monsterInfo.m_nPassengerID[0] = m_nOwnerID;
		}
	}
	bindMachineAndOwner();
}

//怪物机械进入视野的时候
void MonsterMachine::setMachineRideData( const MONMACHINEINFO& machineInfo )
{
	GameActor* pOwner = ActorManager::getSingleton().FindActor( m_nOwnerID );
	if( pOwner != NULL && pOwner->getType() == GAT_MONSTER )
	{

		m_monsterInfo.m_nMonsterDefID = pOwner->getResId();
		m_monsterInfo.m_nMonsterMemID = pOwner->GetID();
		m_monsterInfo.m_nCollcetTime = machineInfo.CollcetTime;
		m_monsterInfo.m_nCollcetVal = machineInfo.CollcetVal;
		m_monsterInfo.m_nEnergy = machineInfo.Energy;
		m_monsterInfo.m_nMaxEnergy = machineInfo.MaxEnergy;
		for (size_t i = 0; i < MAX_MON_MACHINE_ORE; ++i)
		{
			m_monsterInfo.m_nOre[i] = machineInfo.Ores[i].Ore;
		}
		m_monsterInfo.m_nMaxOre = machineInfo.MaxPackOre;
		m_monsterInfo.m_nMaxPassenger = machineInfo.MaxPassenger;
		m_monsterInfo.m_OwnerType = machineInfo.OwnerType;
		if( machineInfo.OwnerType == MACHINE_OWNER_CLAN || machineInfo.OwnerType == MACHINE_OWNER_SELF || machineInfo.OwnerType == MACHINE_OWNER_SPOUSE )
		{
			m_monsterInfo.m_OwnerClanWID = machineInfo.OwnerID;
			if( pOwner->m_DartCarType == 0 )
			{
				pOwner->setName( machineInfo.OwnerName );
			}else{
				char buf[128];
				sprintf(buf,"%s的%s",machineInfo.OwnerName,pOwner->getName());
				pOwner->setName( buf );
			}
		}

		for( int i = 0 ; i < m_monsterInfo.m_nMaxPassenger ; i++ )
		{
			m_monsterInfo.m_nPassengerID[i] = machineInfo.PassengerID[i];
			GameActor* pActor = ActorManager::getSingleton().FindActor( machineInfo.PassengerID[i] );
			if( pActor != NULL )
			{
				pActor->m_MonsterMachine.rideOnMonsterMachine( m_monsterInfo );
			}
		}

		const tagMonMachineDef* pDef = DBDataManager::getSingleton().getMonMachineDef( pOwner->getResId() );
		if ( pDef != NULL )
		{
			m_monsterInfo.m_nCollectType = pDef->CollectType;
		}
	}
}

int MonsterMachine::getTotalOre()
{
	int num = 0;
	for (size_t i = 0; i < MAX_MON_MACHINE_ORE; ++i)
	{
		num = m_monsterInfo.m_nOre[i] + num;
	}
	return num;
}

// 上矿车
void MonsterMachine::rideOnMonsterMachine( const CSACTIONRIDEONMACHINE& rideOnData )
{
	SetMonsterMachineID( rideOnData.MachineID );
	m_playerInfo.m_nMonsterDefID = rideOnData.MachineDefID;
	m_playerInfo.m_nSitIdx		 = rideOnData.SeatIdx;

	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor* pMonseter	= actorMgr.FindActor( m_playerInfo.m_nMonsterMemID );
	if( pMonseter != NULL )
	{
		if( rideOnData.SeatIdx < pMonseter->m_MonsterMachine.m_monsterInfo.m_nMaxPassenger )
		{
			pMonseter->m_MonsterMachine.m_monsterInfo.m_nPassengerID[rideOnData.SeatIdx] = m_nOwnerID;
		}else{
			assert(0);
		}
	}
	const RESPOS& standPos	= rideOnData.RolePos;
	GameActor* pOwner = actorMgr.FindActor( m_nOwnerID );
	if( pOwner != NULL )
	{
		pOwner->SetPosition( standPos.X, standPos.Y );
	}
	bindMachineAndOwner();
}

void MonsterMachine::rideOnMonsterMachine( tagMachineMonsterInfo& monsterInfo )
{
	SetMonsterMachineID( monsterInfo.m_nMonsterMemID );
	m_playerInfo.m_nMonsterDefID = monsterInfo.m_nMonsterDefID;
	bindMachineAndOwner();
}

// 下矿车
void MonsterMachine::downMonsterMachine( const CSACTIONLEAVEMACHINE& rideOnMachineInfo )
{
	const RESPOS& standPos = rideOnMachineInfo.RolePos;
	unbindMachineAndOwner( standPos );
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_playerInfo.m_nMonsterMemID );
	if( pMonster != NULL )
	{
		for( int i = 0 ; i < pMonster->m_MonsterMachine.m_monsterInfo.m_nMaxPassenger; i ++ )
		{
			if(pMonster->m_MonsterMachine.m_monsterInfo.m_nPassengerID[i] == m_nOwnerID )
			{
				pMonster->m_MonsterMachine.m_monsterInfo.m_nPassengerID[i] = 0;
				break;
			}
		}
	}
	GameActor* pOwner = ActorManager::getSingleton().FindActor( m_nOwnerID );
	clearRideMachinInfo();	
	if( pOwner && (pOwner->getType() == GAT_PLAYER || pOwner->getType() == GAT_MAINPLAYER) )
	{
		pOwner->ChangeActState( GSACT_STAND );
	}	
}

void MonsterMachine::ChangePassengerInfo( const CSACTIONMACHINEPASSENGERINFO& info )
{
	m_monsterInfo.m_nMaxPassenger = info.MaxPassenger;
	for( int i = 0 ; i < info.MaxPassenger ; i ++ )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( info.Passengers[i].PassengerID );
		if( pActor != NULL )
		{
			if( pActor->getType() == GAT_MAINPLAYER && m_monsterInfo.m_nPassengerID[i] != info.Passengers[i].PassengerID )
			{
				pActor->m_MonsterMachine.m_playerInfo.m_nSitIdx = i;
				pActor->m_MonsterMachine.m_playerInfo.m_nMonsterDefID = m_monsterInfo.m_nMonsterDefID;
				pActor->m_MonsterMachine.SetMonsterMachineID( m_monsterInfo.m_nMonsterMemID );
				pActor->m_MonsterMachine.setMachineSkill();
				GameMsgPrint::getSingleton().SendUIEvent("GE_TANK_SIT_CHANGE");
			}
			pActor->m_MonsterMachine.rideOnMonsterMachine( m_monsterInfo );
		}
		m_monsterInfo.m_nPassengerID[i] = info.Passengers[i].PassengerID;
	}
	m_PassengerInfo = info;
	GameMsgPrint::getSingleton().SendUIEvent("GE_PASSENGER_CHANGE");
}

// 绑定怪物与玩家
void MonsterMachine::bindMachineAndOwner()
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	GameActor* pPlayer = actorMgr.FindActor( m_nOwnerID ) ;
	GameActor* pMonseter = actorMgr.FindActor( m_playerInfo.m_nMonsterMemID );
	if( pPlayer == NULL || pMonseter == NULL )
	{
		return;
	}

	pPlayer->hide(true);

	if( m_playerInfo.m_bBind )
	{
		return;
	}
	const tagMonMachineDef* pDef = DBDataManager::getSingleton().getMonMachineDef( m_playerInfo.m_nMonsterDefID );
	if( pDef == NULL )
	{
		return;
	}

	MONSTERDEF monsterDef = DBDataManager::getSingleton().getMonsterDef( m_playerInfo.m_nMonsterDefID );
	const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( m_playerInfo.m_nMonsterDefID );

	pPlayer->getActState().setInState( GSACT_IN_MORPH, true );
	if (npcDisplayInfo.race == MONSTER_RIDE)
	{
		pMonseter->getShowActor()->bindActor(200 + m_playerInfo.m_nSitIdx, pPlayer->getShowActor());
		if (npcDisplayInfo.rideact == 0)
		{
			pPlayer->playAction( 10111, NULL, 0, 0 );
		}
		else
		{
			pPlayer->playAction( npcDisplayInfo.rideact, NULL, 0, 0 );
		}
	}

	if( pPlayer->getType() == GAT_MAINPLAYER )
	{
		setMachineSkill();
		((MainGamePlayer*)pPlayer)->m_bChangeSkillShortCut = true;
		GameMsgPrint::getSingleton().SendUIEvent("GE_CLAN_BINDMACHINE");
	}
	m_playerInfo.m_bBind = true;
}

void MonsterMachine::setMachineSkill()
{
	MONSTERDEF monsterDef = DBDataManager::getSingleton().getMonsterDef( m_playerInfo.m_nMonsterDefID );
	tagMonMachineDef_ToLua* monMachineDef = getMonMachineDef();
	if( monMachineDef == NULL )
	{
		return;
	}
	if( monMachineDef->PassengerNum == 1 )
	{
		for( int i = 0 ; i < MAX_NORM_SKILL; ++ i )
		{
			int skillId = monsterDef.AINormalSkills[m_playerInfo.m_nSitIdx*2+i].SkillID;
			int skillLv = monsterDef.AINormalSkills[m_playerInfo.m_nSitIdx*2+i].SkillLevel;
			SkillManager::getSingleton().setTmpSkill( i,skillId,skillLv );
			SkillManager::getSingleton().addTmpSkill( skillId );
		}
	}else{
		int idx = m_playerInfo.m_nSitIdx/2;
		switch(idx)
		{
		case 0:
			{				
				for( int i = 0 ; i < 2 ; i++ )
				{
					int skillId = monsterDef.AINormalSkills[m_playerInfo.m_nSitIdx*2+i].SkillID;
					int skillLv = monsterDef.AINormalSkills[m_playerInfo.m_nSitIdx*2+i].SkillLevel;
					SkillManager::getSingleton().setTmpSkill( i,skillId,skillLv );
					SkillManager::getSingleton().addTmpSkill( skillId );				
				}
			}
			break;
		case 1:
			{	
				for( int i = 0 ; i < 2 ; i++ )
				{
					int skillId = monsterDef.AIAtkerSkills[(m_playerInfo.m_nSitIdx-2)*2+i].SkillID;
					int skillLv = monsterDef.AIAtkerSkills[(m_playerInfo.m_nSitIdx-2)*2+i].SkillLevel;
					SkillManager::getSingleton().setTmpSkill( i,skillId,skillLv );
					SkillManager::getSingleton().addTmpSkill( skillId );				
				}
			}
			break;
		case 2:
			{	
				for( int i = 0 ; i < 2 ; i++ )
				{
					int skillId = monsterDef.AIAtkeeSkills[(m_playerInfo.m_nSitIdx-4)*2+i].SkillID;
					int skillLv = monsterDef.AIAtkeeSkills[(m_playerInfo.m_nSitIdx-4)*2+i].SkillLevel;
					SkillManager::getSingleton().setTmpSkill( i,skillId,skillLv );
					SkillManager::getSingleton().addTmpSkill( skillId );				
				}
			}
			break;
		}
	}
}

// 解除怪物与玩家的绑定
void MonsterMachine::unbindMachineAndOwner( const RESPOS& standPos )
{
 	ActorManager& actorMgr		= ActorManager::getSingleton();
  	GameActor* pMonster		= actorMgr.FindActor( m_playerInfo.m_nMonsterMemID );
	GameActor* pOwner			= actorMgr.FindActor( m_nOwnerID );
	if( pOwner == NULL /*|| pMonseter == NULL*/ )
	{
		return;
	}
	if (pMonster != NULL)
	{
		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( m_playerInfo.m_nMonsterDefID );
		if (npcDisplayInfo.race == MONSTER_RIDE)
		{
			pMonster->getShowActor()->unbindActor(pOwner->getShowActor());
		}
	}

	if( pOwner && (pOwner->getType() == GAT_MAINPLAYER || pOwner->getType() == GAT_PLAYER) )
	{
		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( m_playerInfo.m_nMonsterDefID );
			
		pOwner->hide(false);
		pOwner->getActState().setInState( GSACT_IN_MORPH,false );

		pOwner->ChangeActState( GSACT_STAND );
		pOwner->SetPosition( standPos.X, standPos.Y );
		pOwner->getShowActor()->setFaceTo(pOwner->getAngle());
	}

	if( pOwner->getType() == GAT_MAINPLAYER )
	{
		SkillManager::getSingleton().delTmpSkill(SkillManager::getSingleton().getTmpSkillId(0));
		SkillManager::getSingleton().delTmpSkill(SkillManager::getSingleton().getTmpSkillId(1));
		SkillManager::getSingleton().setTmpSkill( 0, 0, 0 );
		SkillManager::getSingleton().setTmpSkill( 1, 0, 0 );
		((MainGamePlayer*)pOwner)->m_bChangeSkillShortCut = false;
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CLAN_UNBINDMACHINE" );
	}
	m_playerInfo.m_bBind = false;
	if( pMonster != NULL )
	{
		pMonster->m_MonsterMachine.onPassengeLeave( m_nOwnerID );
	}
}

int MonsterMachine::getOre(int i)
{
	if (i > 0 && i <= MAX_MON_MACHINE_ORE)
	{
		return m_monsterInfo.m_nOre[i - 1];
	}
	return 0;
}

int MonsterMachine::getOreId(int i)
{
	if (i > 0 && i <= MAX_MON_MACHINE_ORE)
	{
		const tagMonMachineDef *monMachineDef = DBDataManager::getSingleton().getMonMachineDef(m_monsterInfo.m_nMonsterDefID);
		if (monMachineDef == NULL)
		{
			return 0;
		}
		return monMachineDef->Ore[i - 1].OreItemID;
	}
	return 0;
}

int MonsterMachine::getMaxOre()
{
	return m_monsterInfo.m_nMaxOre;
}
int MonsterMachine::getEnergy()
{
	return m_monsterInfo.m_nEnergy;
}
int MonsterMachine::getMaxEnergy()
{
	return m_monsterInfo.m_nMaxEnergy;
}

bool MonsterMachine::canMove()
{
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_playerInfo.m_nMonsterMemID );
	if( pMonster != NULL )
	{
		// 不是0号位置，无法移动
		if( !pMonster->m_MonsterMachine.isMainPlayerDriver() )
		{
			return false;
		}
		//巡逻路径大于0，不可移动
		if( pMonster->m_nPatrolID > 0 )
		{
			return false;
		}
// 		if( pMonster->m_MonsterMachine.getEnergy() > 0 )
// 		{
// 			return true;
// 		}else{
// 			GameMsgPrint::getSingleton().showMidTip(MID_NOT_ENOUGH_ENERGY);
// 			return false;
// 		}
	}
	return true;
}

bool MonsterMachine::hasPatrolID()
{
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_playerInfo.m_nMonsterMemID );
	if( pMonster != NULL )
	{
		//巡逻路径大于0，不可移动
		if( pMonster->m_nPatrolID > 0 )
		{
			return true;
		}
	}
	return false;
}

MonsterMachine* MonsterMachine::getPlayerMonsterMachine()
{
	if( m_playerInfo.m_nMonsterMemID >0  )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( m_playerInfo.m_nMonsterMemID );
		if(pActor != NULL )
		{
			return pActor->getMonsterMachine();
		}
	}
	return NULL;
}

void MonsterMachine::setAttriValule( int attriId, int nValue )
{
	int type = -1;
	int value = 0;
	switch (attriId)
	{
	case MON_MACHINE_ATTR_ENERGY:
		{
			type = 0;
			value = nValue - m_monsterInfo.m_nEnergy;
			m_monsterInfo.m_nEnergy = nValue;
		}
		break;
	case MON_MACHINE_ATTR_ORE1:
		{
			type = 1;
			value = nValue - m_monsterInfo.m_nOre[0];
			m_monsterInfo.m_nOre[0] = nValue;
		}
		break;
	case MON_MACHINE_ATTR_ORE2:
		{
			type = 2;
			value = nValue - m_monsterInfo.m_nOre[1];
			m_monsterInfo.m_nOre[1] = nValue;
		}
		break;
	case MON_MACHINE_ATTR_ORE3:
		{
			type = 3;
			value = nValue - m_monsterInfo.m_nOre[2];
			m_monsterInfo.m_nOre[2] = nValue;
		}
		break;
	case MON_MACHINE_ATTR_ORE4:
		{
			type = 4;
			value = nValue - m_monsterInfo.m_nOre[3];
			m_monsterInfo.m_nOre[3] = nValue;
		}
		break;
	case MON_MACHINE_ATTR_ORE5:
		{
			type = 5;
			value = nValue - m_monsterInfo.m_nOre[4];
			m_monsterInfo.m_nOre[4] = nValue;
		}
		break;
	}

	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer->getMonsterMachine()->m_playerInfo.m_nMonsterMemID == m_monsterInfo.m_nMonsterMemID)
	{
		if (type != -1 && value != 0)
		{
			char script[100];
			sprintf(script, "OnMonsterMachineAttribChg(%d, %d)", type, value);
			ActorManager::getSingleton().getScriptVM()->callString(script);
		}
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MONSTER_ATTR_CHANGE" );
	}
}

tagMonMachineDef_ToLua* MonsterMachine::getMonMachineDef()
{
	return getMonMachineDef( m_playerInfo.m_nMonsterDefID ) ;
}

bool MonsterMachine::canRideOn()
{
	for( int i = 0 ; i < m_monsterInfo.m_nMaxPassenger ; i++ )
	{
		if( m_monsterInfo.m_nPassengerID[i] == 0 )
		{
			return true;
		}
	}
	return false;
}

int MonsterMachine::getFirstEmptyIdx()
{
	for( int i = 0 ; i < m_monsterInfo.m_nMaxPassenger ; i++ )
	{
		if( m_monsterInfo.m_nPassengerID[i] == 0 )
		{
			return i;
		}
	}
	return -1;
}

void MonsterMachine::Update()
{
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_nOwnerID );
	if( pMonster == NULL )
	{
		return;
	}
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	for( int i = 0 ; i < m_monsterInfo.m_nMaxPassenger ; i++ )
	{
		if( i == 0 && m_monsterInfo.m_nPassengerID[i] == pMain->GetID() && pMonster->m_nPatrolID <= 0 )
		{
			GameXYZ pos = pMain->GetPosition();
			pMonster->m_pMapMover->StopOnPoint( pos.x/10, pos.z/10 );
			pMonster->SetPositionTM( pos.x,pos.y, pos.z );
			pMonster->SetFaceTo( pMain->getAngle(),true );
			continue;
		}
		GameActor* pActor = ActorManager::getSingleton().FindActor( m_monsterInfo.m_nPassengerID[i] );
		if( pActor != NULL )
		{
			GameXYZ pos = pMonster->GetPosition();
			pActor->m_pMapMover->StopOnPoint( pos.x/10,pos.z/10 );
			pActor->SetPositionTM( pos.x,pos.y,pos.z );
			pActor->SetFaceTo( pMonster->getAngle(),true );
		}
	}
}

bool MonsterMachine::isSameCamp()
{
	if( m_monsterInfo.m_nMonsterMemID > 0  )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		if( m_monsterInfo.m_OwnerType == MACHINE_OWNER_SPOUSE )
		{
			if( pMain->m_loverID == m_monsterInfo.m_OwnerClanWID || pMain->getRoleWId() == m_monsterInfo.m_OwnerClanWID )
			{
				return true;
			}
		}else if( m_monsterInfo.m_OwnerType == MACHINE_OWNER_CLAN )
		{
			if( pMain->getClanWID() == m_monsterInfo.m_OwnerClanWID )
			{
				return true;
			}

		}else if( m_monsterInfo.m_OwnerType == MACHINE_OWNER_SELF )
		{
			if( pMain->getRoleWId() == m_monsterInfo.m_OwnerClanWID )
			{
				return true;
			}else if( !canChangeSide() && GameActor_PVP::getSingleton().isSameTeam(m_monsterInfo.m_OwnerClanWID) )
			{
				return true;
			}
		}else if( m_monsterInfo.m_OwnerType == MACHINE_OWNER_NONE )
		{
			return true;
		}
	}
	return false;
}

int MonsterMachine::getMonsterEnergy()
{
	if( m_playerInfo.m_nMonsterMemID > 0 )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( m_playerInfo.m_nMonsterMemID );
		if( pActor != NULL )
		{
			return pActor->m_MonsterMachine.getEnergy();
		}
	}
	return 0;
}

bool MonsterMachine::isMainPlayerDriver()
{
	if( m_monsterInfo.m_nMonsterMemID > 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		if( pMain->GetID() == m_monsterInfo.m_nPassengerID[0] )
		{
			return true;
		}
	}
	return false;
}

void MonsterMachine::onPassengeLeave( int nActorId )
{
	for( int i=0;i<m_monsterInfo.m_nMaxPassenger;i++ )
	{
		if( m_monsterInfo.m_nPassengerID[i] == nActorId )
		{
			m_monsterInfo.m_nPassengerID[i] = 0;
		}
	}
}

tagCSActionMachinePassengerInfo_ToLua* MonsterMachine::getMachinePassgerInfo()
{
	static_assert( sizeof(tagCSActionMachinePassengerInfo_ToLua) == sizeof(tagCSActionMachinePassengerInfo) );
	return (tagCSActionMachinePassengerInfo_ToLua*)&m_PassengerInfo;
}

bool MonsterMachine::isPlayerOnMachine( GameActor* pActor )
{
	if( pActor == NULL )
	{
		return false;
	}
	if( m_monsterInfo.m_nMonsterMemID == 0 )
	{
		return false;
	}
	return ( m_monsterInfo.m_nMonsterMemID == pActor->getMonsterMachine()->m_playerInfo.m_nMonsterMemID );
}

float MonsterMachine::getLeavePrepareTime()
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	Item *item = pMainPlayer->getContainer().getItem(m_FromItemWID);
	if (item == NULL)
	{
		return 0;
	}
	return m_monsterInfo.m_OwnerType == MACHINE_OWNER_NONE ? 0 : MACHINE_LEAVE_PRE_TIME;
}

bool MonsterMachine::canChangeSide()
{
	return m_FromItemWID == 0;
}

bool MonsterMachine::isInCollectSkill()
{
	return false;
}

int MonsterMachine::getPassenger(int i)
{
	if( m_monsterInfo.m_nMonsterMemID > 0 )
	{
		return m_monsterInfo.m_nPassengerID[i];
	}
	return 0;
}

int MonsterMachine::getPassengerNum()
{
	if( m_monsterInfo.m_nMonsterMemID > 0 )
	{
		return m_monsterInfo.m_nMaxPassenger;
	}
	return 0;
}
