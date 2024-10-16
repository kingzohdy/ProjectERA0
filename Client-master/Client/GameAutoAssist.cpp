#include "GameAutoAssist.h"
#include "GameClientOpt.h"
#include "GameActor.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "GameMaxMap.h"
#define  MAX_AUTO_ASSIST_RANG 1200
#define  MAX_MOVE_RANG		  2700

AutoAssist::AutoAssist(void) : m_nMapID(0),m_asBeginFlag(false)
{
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_AUTO_ASSIST, sizeof( tagAutoAssistData ) );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_VIP_AUTO_ASSIST, sizeof( tagVipAutoAssistData ) );
	m_CapturePet.clear();
}

AutoAssist::~AutoAssist(void)
{
  
}

void AutoAssist::setAutoAssistPickUpOpt(int index,bool chkBtnVal)
{
	m_assistData.m_bChkBtn[index] = chkBtnVal;

}

void AutoAssist::setAutoAssistBegin( bool isbegin)
{
	m_asBeginFlag = isbegin;
	if ( isbegin )
	{
		m_CapturePet.clear();
	}
	else
	{
		ResumePetSubType();
	}
}

bool AutoAssist::getAutoAssistBegin()
{
	return m_asBeginFlag;
}
void AutoAssist::saveAutoAssistOpt()
{
	GameClientOpt::getSingleton().setClientOpt(	SAVE_OPT_AUTO_ASSIST,(char*)& m_assistData );
}

tagAutoAssistData* AutoAssist::getAutoAssistOpt()
{
	return &m_assistData;
}

void AutoAssist::resetAssistInfo()
{	
	m_asBeginFlag =	false;
	// 辅助设置
	m_assistData.m_bChkBtn[0]	=	false;
	for (int i = 1;i < MAX_CHKBTN_NUM;i++)
	{
		m_assistData.m_bChkBtn[i]	= true;
	}
	m_assistData.m_hpSliderVal	= 75;
	m_assistData.m_mpSliderVal	= 50;
	m_assistData.m_hpId			= 0;
	m_assistData.m_mpId			= 0;
	for (int i = 0; i < MAX_AUTO_ASSIST_SKILL_NUM;i++)
	{
		m_assistData.m_nSkillId[i] = 0;
	}
}

void AutoAssist::resetVipAssist()
{
	// VIP高级辅助设置
	m_VipAssist.m_nHpIndex		= 1;
	m_VipAssist.m_nMpIndex		= 1;
	m_VipAssist.m_nInterval		= 30;
	m_VipAssist.m_nHPNum		= 50;
	m_VipAssist.m_nMPNum		= 50;
	for (int i = 0; i < MAX_VIP_CHKBTN_NUM;i++)
	{
		m_VipAssist.m_bVipChkBtn[i] = false;
	}
}

bool AutoAssist::targetCanAttack( int targetId )
{
	MainGamePlayer *	pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	if (pMainPlayer == NULL)
	{
		return false;
	}
	if( targetId == 200029 )
	{
		//黑暗物质
		return false;
	}
	ActorManager&	actorMgr	= ActorManager::getSingleton();
	GameActor*		pDesObj		= actorMgr.FindActor( targetId );
	if (pDesObj == NULL)
	{
		return false;
	}
	if(	pDesObj->getType() == GAT_MONSTER 
		&&(pDesObj->getSubType() == MON_SUB_TYPE_COLLECT 
			|| pDesObj->getSubType() == MON_SUB_TYPE_TRAP
			|| pDesObj->getSubType() == MON_SUB_TYPE_BUILD) )
	{
		return false;
	}
	if(pDesObj->isDead() || 
		pDesObj->hasOtherBooty() || 
		pDesObj->getType() == GAT_ITEM || 
		pDesObj->getType() == GAT_PET ||
		pDesObj->getType() == GAT_NPC)
	{
		pMainPlayer->SetSelectTarget( NULL );
		pMainPlayer->CmdMoveStop();
		return false;
	}

	return true;
}

void AutoAssist::saveNativePos()
{
	MainGamePlayer*		pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	m_nPos		= pMainPlayer->GetPosition();
	m_nMapID	= pMainPlayer->getMapId();
}

bool AutoAssist::isInfixedRangeAssist()
{
	MainGamePlayer *	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return false;
	}
	GameXYZ  targetPos	= pMainPlayer->GetPosition();
	targetPos.y			= m_nPos.y;
	float length		= m_nPos.subtract(targetPos).length();
	if ( length >= MAX_MOVE_RANG)
	{
		pMainPlayer->SetSelectTarget( NULL );
		return false;
	}
	if (length	<= MAX_AUTO_ASSIST_RANG )
	{
		return true;
		/*pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(m_nPos.x),
			(int)Ogre::WorldPos::Fix2Flt(m_nPos.z),0 );*/
	}
	else
		return false;
	
}

void AutoAssist::HandleAutoFind()
{
	MainGamePlayer *	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}
	if ( pMainPlayer->getAutoFind() )
	{
		pMainPlayer->CmdMoveStop();
	}
}

void AutoAssist::ResumePetSubType()
{
	if ( m_CapturePet.size() != 0 )
	{
		for ( std::vector<int>::iterator iter = m_CapturePet.begin();iter != m_CapturePet.end(); ++iter )
		{
			GameActor* ObjActor = ActorManager::getSingleton().FindActor( *iter );
			if ( ObjActor != NULL && !ObjActor->isDead() )
			{
				//	int nSubType = getMonsterSubtype( ObjActor->getResId() );
				ObjActor->setSubType();
			}
		}
		m_CapturePet.clear();
	}
	
}
void AutoAssist::GetNearestCaptureMonster()
{
	int radius = 15000;

	MainGamePlayer *	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}
	GameActor* pSrcActor	= ActorManager::getSingleton().FindActor( pMainPlayer->GetID() );
	GameXYZ		srcPos		= pMainPlayer->GetPosition();

	for( std::map<int, GameActor *>::iterator iter = ActorManager::getSingleton().getActorBegin(); 
		iter != ActorManager::getSingleton().getActorEnd(); ++iter )
	{
		if( pSrcActor == iter->second )
		{
			continue;
		}
		if( radius != -1 )
		{
			GameXYZ pos	= iter->second->GetPosition();
			int length	= (int)pos.subtract(srcPos).length();
			if (length > radius/10)
			{
				continue;
			}
		}
		if ( iter->second->getType() == GAT_MONSTER && 
			iter->second->getMonsterPhysicType() == 3 && 
			iter->second->getSubType() != MON_SUB_TYPE_HUMAN_FRIEND )
		{
			iter->second->setSubType( MON_SUB_TYPE_HUMAN_FRIEND );
			m_CapturePet.push_back( iter->first );
		}
	}
}

tagVipAutoAssistData* AutoAssist::getVIPAutoAssistOpt()
{
	return  &m_VipAssist;
}

void AutoAssist::saveVIPAssistOpt()
{
	GameClientOpt::getSingleton().setClientOpt(	SAVE_OPT_VIP_AUTO_ASSIST,(char*)& m_VipAssist );
}

bool AutoAssist::ReturnAssistPosition()
{
	MainGamePlayer *	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return false;
	}
	GameXYZ  targetPos	= pMainPlayer->GetPosition();
	int		 nCurMap	= pMainPlayer->getMapId();
	if ( m_nMapID == 0 )
	{
		return true;
	}
	GameMapShower	* pMapShower	= GameManager::getSingleton().getGameMapShower();
	int		nDestX = m_nPos.x / 10;
	int		nDestY = m_nPos.z / 10;
	if ( nCurMap != m_nMapID )
	{
		pMapShower->GetMaxMapContal()->linkMoveto( m_nMapID , nDestX, nDestY );
		return false;
	}
	targetPos.y			= m_nPos.y;
	float length		= m_nPos.subtract(targetPos).length();

	if (length	<= MAX_AUTO_ASSIST_RANG )
	{
		return true;	
	}
	else
	{
		pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(m_nPos.x),
			(int)Ogre::WorldPos::Fix2Flt(m_nPos.z),0 );
		return false;
	}
}

void AutoAssist::setAssistMapID( int nMapID )
{
	m_nMapID = nMapID;
}
