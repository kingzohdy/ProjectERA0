#include "UseItemChant.h"
#include <Windows.h>

UseItemChant::UseItemChant( GameActor* pOwner, unsigned int uStartTime, unsigned int uChantTime, int nItemID, int TargetID /*= 0*/, int ListType /*= -1*/, int GridIdx /*= -1*/ )
:IChant_Base( pOwner, uStartTime,uChantTime )
{
	m_nItemID = nItemID;
	m_nTargetID = TargetID;
	m_nListType = ListType;
	m_nGridIdx = GridIdx;
	SetType( eUseItem_Chant );
}

UseItemChant::~UseItemChant()
{

}

bool UseItemChant::OnStart()
{
	GameActor* pTarget = NULL;
	if( m_nTargetID > 0 )
	{
		pTarget = ActorManager::getSingleton().FindActor( m_nTargetID );
		if( pTarget == NULL )
		{
			return false;
		}
	}
	if( GetOwner() )
	{
		if( GetOwner()->getType() == GAT_MAINPLAYER )
		{
			char szScript[256];
			sprintf( szScript, "BeginUseItem(%d,%d,%d)", GetChantTime(),m_nListType,m_nGridIdx );
			ActorManager::getSingleton().getScriptVM()->callString( szScript );
		}
		GetOwner()->m_Equip.hideWeapon();
		int nCanPlayChangeState	= GetOwner()->getActState().canChangeState( GSACT_COLLECTITEM );
		if (nCanPlayChangeState != STATE_UNCHANGE)
		{
			//GetOwner()->getActState().SetState(GSACT_COLLECTITEM);
		}
		if( nCanPlayChangeState == STATE_CHANGE )
		{
		 	const tagItemDef_ToLua* pItemDef = getItemDef( m_nItemID );
		 	if( pItemDef->ItemID == m_nItemID && pTarget != NULL && pItemDef->Result[0].ResultID == RESULT_CATCH_PET )
		 	{
				// 处理抓宠物
				Ogre::Vector3 pos = pTarget->GetPosition().toVector3();
				GetOwner()->SetFaceTo( int(pos.x), int(pos.z) );
				GetOwner()->playMotion( "10254_4" );
				pTarget->playMotion( "10254_5" );
				GetOwner()->getShowActor()->playAction( 10254, NULL, 0, 0 );				
			}else{
				int nActionID = 0;
				char motion[256] = {0};
				char targetMotion[256] = {0};
				ActorManager::getSingleton().getScriptVM()->callFunction( "getActionIDbyItemID","i>iss",
					m_nItemID,&nActionID,motion,targetMotion );
				if( nActionID == 0 )
				{
					// 10300
					GetOwner()->getShowActor()->playAction( 10300, NULL, 0, 0 );
				}else{
					GetOwner()->getShowActor()->playAction( nActionID, NULL, 0, 0 );
				}
				if( motion != NULL && strlen(motion) > 0 )
				{
					GetOwner()->playMotion(motion);
				}
				if( pTarget && targetMotion != NULL && strlen(targetMotion) > 0 )
				{
					pTarget->playMotion(targetMotion);
				}
			}
		}	 
	}else{
		return false;
	}
	return true;
}

bool UseItemChant::Update()
{
	unsigned int uCurTick = timeGetTime();
	
	if( m_nTargetID != 0 )
	{
		// 目标 不在视野内，打断
		GameActor *pActor = ActorManager::getSingleton().FindActor(m_nTargetID);
		if (pActor == NULL || ( pActor->getType() == GAT_MONSTER && pActor->isDead() ) )
		{
			return RequestBreak();
		}
	}
	
	return false;
}

bool UseItemChant::SendFinishRequest()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		((MainGamePlayer*)GetOwner())->getContainer().send_UsedExpendableItem( m_nListType, m_nGridIdx, m_nTargetID );
		m_bRequest = true;
	}
	return true;
}

bool UseItemChant::RequestBreak()
{
	if( GetOwner() && GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.PackageReq.Type	= PACKAGE_REQ_USE_PRE_BRK;

		PACKAGEUSEBRK& requireBreakUseItemPreData = msgbody.PackageReq.PackageReqData.PackageUseBrk;
		requireBreakUseItemPreData.ItemID = m_nItemID; 
		SendGameServerMsg( PACKAGE_REQ, msgbody );
		return true;
	}
	return false;
}

void UseItemChant::OnBreak()
{
	if( !GetOwner()->isInRide() && !GetOwner()->isOnMonsterMachine() )
	{
		GetOwner()->m_Equip.showWeapon();
		GetOwner()->playAction( GetOwner()->getActState().getActionId() );
	}	
	
 	GetOwner()->stopMotion( "10254_4" );	
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_BREAK_PRE_ACTION");
		char szBuf[256];
		sprintf(szBuf,"UnLockItem(%d,%d)",m_nListType,m_nGridIdx);
		ActorManager::getSingleton().getScriptVM()->callString( szBuf );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}
}
