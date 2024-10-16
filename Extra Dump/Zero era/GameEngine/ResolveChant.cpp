#include "ResolveChant.h"

ResolveChant::ResolveChant( GameActor* pOwner, unsigned int uStartTime, int nListType, int nGridIdx, unsigned long long Wid, int nType )
:IChant_Base(pOwner,uStartTime,RESOLVE_CHANT_TIME)
{
	m_nListType = nListType;
	m_nGridIdx = nGridIdx;
	m_Wid = Wid;
	m_nType = nType;
	SetType( eResolve_Chant );
}

ResolveChant::~ResolveChant()
{

}

bool ResolveChant::Update()
{
	return false;
}

bool ResolveChant::SendFinishRequest()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.ArmClt.ArmCltData.ArmFenJie.ArmData.ArmIdxData.ArmListType = m_nListType;
		msgbody.ArmClt.ArmCltData.ArmFenJie.ArmData.ArmIdxData.ArmGridIdx =	m_nGridIdx;
		msgbody.ArmClt.ArmCltData.ArmFenJie.ArmData.WId = m_Wid;
		msgbody.ArmClt.ArmCltData.ArmFenJie.Type = m_nType;
		msgbody.ArmClt.ArmCltType = ARM_FENJIE;
		SendGameServerMsg( ARM_CLT,msgbody);
		m_bRequest = true;
	}
	return false;
}

bool ResolveChant::OnStart()
{
	GetOwner()->m_Equip.hideWeapon();
	GetOwner()->getShowActor()->stopCurAction();
	GetOwner()->getShowActor()->playAction(10300);
	GetOwner()->playMotion("10300",10300);
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		char szBuf[256];
		sprintf(szBuf,"StartContainerResolve(%d)",GetChantTime());
		ActorManager::getSingleton().getScriptVM()->callString( szBuf );
	}
	return true;
}

bool ResolveChant::RequestBreak()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.ArmClt.ArmCltType = ARM_FENJIE_PRE_BRK;
		msgbody.ArmClt.ArmCltData.ArmFenJiePreBrk = 0;
		SendGameServerMsg( ARM_CLT,msgbody );
		ActorManager::getSingleton().getScriptVM()->callString("BreakContainerResolve()");
		return true;
	}
	return false;
}

void ResolveChant::OnBreak()
{
	if( GetOwner()->getActState().GetState() == GSACT_STAND )
	{
		GetOwner()->getShowActor()->playAction( GetOwner()->getActState().getActionId() );
		GetOwner()->m_Equip.showWeapon();
		GetOwner()->stopMotion("10300");
	}
}
