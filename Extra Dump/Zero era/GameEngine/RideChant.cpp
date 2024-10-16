#include "RideChant.h"

RideChant::RideChant( GameActor* pOwner, unsigned int uStartTime, unsigned int rideId, int nLevel )
: IChant_Base(pOwner,uStartTime,RIDE_CHANT_TIME)
{
	m_uRideID = rideId;
	m_nRideLv = nLevel;
	SetType(eRide_Chant);
}

RideChant::~RideChant()
{

}

bool RideChant::OnStart()
{
	GetOwner()->m_Equip.hideWeapon();
	GetOwner()->getShowActor()->playAction( 10300, NULL, 0, 0 );
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		char szScript[256];
		sprintf( szScript, "StartRide(%d)",GetChantTime() );
		ActorManager::getSingleton().getScriptVM()->callString(szScript);
	}
	return true;
}

bool RideChant::Update()
{
	return false;
}

bool RideChant::SendFinishRequest()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.RideOn.RideItemID = m_uRideID;
		msgbody.RideOn.RidePos.X	= GetOwner()->GetPosition().x/10;
		msgbody.RideOn.RidePos.Y	= GetOwner()->GetPosition().z/10;
		ServerTimer::getSingleton().getServerTime(msgbody.RideOn.SvrTime.BenchTimeSec, 
			msgbody.RideOn.SvrTime.BenchTimeUSec);
		SendGameServerMsg( RIDE_ON, msgbody ); 
		m_bRequest = true;
	}
	return true;
}

bool RideChant::RequestBreak()
{
	if( GetOwner() && GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.RideBrk.RideItemID = m_uRideID;
		SendGameServerMsg( RIDE_BRK, msgbody );
		return true;
	}
	return false;
}

void RideChant::OnBreak()
{
	if( GetOwner()->m_Equip.m_bHideWeap )
	{
		GetOwner()->m_Equip.showWeapon();
	}
	GetOwner()->playAction( GetOwner()->getActState().getActionId() );	
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_BREAK_PRE_ACTION");		
	}
}
