#include "PetChant.h"

PetChant::PetChant( GameActor* pOwner, unsigned int uStartTime, tdr_longlong wid )
:IChant_Base(pOwner,uStartTime,PET_CHANT_TIME)
{
 	m_Wid = wid;
	SetType(ePet_Chant);
}

PetChant::~PetChant()
{

}

bool PetChant::OnStart()
{
	GetOwner()->CmdMoveStop();
	GetOwner()->m_Equip.hideWeapon();
	GetOwner()->getShowActor()->playAction( 10300, NULL, 0, 0 );

	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		char script[256];
		sprintf( script, "PetCampaign(%d)", GetChantTime() );
		ActorManager::getSingleton().getScriptVM()->callString( script );
	}
	return true;
}

bool PetChant::Update()
{	
	return false;
}

bool PetChant::SendFinishRequest()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.PetClt.PetCltType					= PET_CLT_ACT;
		msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_CAMPAIGN;
		msgbody.PetClt.PetCltData.PetAct.PetWid		= m_Wid;
		SendGameServerMsg( PET_CLT, msgbody ); 
		m_bRequest = true;
	}
	return true;
}

bool PetChant::RequestBreak()
{
	if( GetOwner() && GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.PetClt.PetCltType					= PET_CLT_ACT;
		msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_CAMPAIGN_BRK;
		msgbody.PetClt.PetCltData.PetAct.PetWid		= m_Wid;
		SendGameServerMsg( PET_CLT, msgbody );
		return true;
	}
	return false;
}

void PetChant::OnBreak()
{
	GetOwner()->m_Equip.showWeapon();
	GetOwner()->playAction( GetOwner()->getActState().getActionId() );

	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_BREAK_PRE_ACTION");
	}
}
