#include "TrumpChant.h"

TrumpChant::TrumpChant( GameActor* pOwner, unsigned int uStartTime, int nHeighId, int nLowId, int nGridIdx )
: IChant_Base(pOwner,uStartTime,TRUMP_CHANT_TIME)
{
	m_nHeighId = nHeighId;
	m_nLowId = nLowId;
	m_nGridIdx = nGridIdx;
	SetType( eTrump_Chant );
}

TrumpChant::~TrumpChant()
{

}

bool TrumpChant::OnStart()
{
	char script[256];
	sprintf( script, "TrumpCampaign(%d)", GetChantTime() );
	ActorManager::getSingleton().getScriptVM()->callString( script );
	return true;
}

bool TrumpChant::Update()
{
	return false;
}

bool TrumpChant::SendFinishRequest()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.FairyClt.OpType   = OP_CAMP;
		tagTrumpId trumpId;
		trumpId.nHeight	    = m_nHeighId;
		trumpId.nLow		= m_nLowId;
		msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
		msgbody.FairyClt.Grididx    = m_nGridIdx;
		SendGameServerMsg( FAIRY_CLT, msgbody );
		m_bRequest = true;
	}
	return true;
}

bool TrumpChant::RequestBreak()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.FairyClt.OpType   = OP_CAMP_BRK;

		tagTrumpId trumpId;
		trumpId.nHeight	= m_nHeighId;
		trumpId.nLow	= m_nLowId;

		msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
		msgbody.FairyClt.Grididx    = m_nGridIdx;
		SendGameServerMsg( FAIRY_CLT, msgbody );
		return true;
	}
	return false;
}

void TrumpChant::OnBreak()
{
	ActorManager::getSingleton().getScriptVM()->callString("EndTrumpPre()");
}
