#include "SecondaryChant.h"

SecondaryChant::SecondaryChant( GameActor* pOwner, unsigned int uStartTime, unsigned int uChantTime, int nType, int nId )
:IChant_Base(pOwner,uStartTime,uChantTime)
{
	m_nType = nType;
	m_nId = nId;
	SetType( eSecondary_Chant );
}

SecondaryChant::~SecondaryChant()
{

}

bool SecondaryChant::OnStart()
{
	GetOwner()->getShowActor()->playAction(10300);
	GetOwner()->m_Equip.hideWeapon();
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		char buf[256];
		sprintf(buf,"startSecondarySkill(%d,%d,%d)",m_nType,m_nId,GetChantTime());
		ActorManager::getSingleton().getScriptVM()->callString(buf);
	}
	return true;
}

bool SecondaryChant::Update()
{
	return false;
}

bool SecondaryChant::SendFinishRequest()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.SecondarySkillClt.CltOp = SECONDARY_SKILL_CLT_RECIPE_BLACKSMITHING;
		msgbody.SecondarySkillClt.CltData.Blacksmithing.SkillType = m_nType;
		msgbody.SecondarySkillClt.CltData.Blacksmithing.RecipeID = m_nId;
		SendGameServerMsg(SECONDARY_SKILL_CLT,msgbody);
		m_bRequest = true;
	}
	return false;
}

bool SecondaryChant::RequestBreak()
{
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		memset(&msgbody,0,sizeof(tagCSPkgBody));
		msgbody.SecondarySkillClt.CltOp = SECONDARY_SKILL_CLT_RECIPE_PRE_BRK;
		SendGameServerMsg(SECONDARY_SKILL_CLT,msgbody);
		return true;
	}
	return false;
}

void SecondaryChant::OnBreak()
{
	GetOwner()->getShowActor()->playAction( GetOwner()->getActState().getActionId() );
	GetOwner()->m_Equip.showWeapon();
	//´¦ÀíÖ÷½Ç
	if( GetOwner()->getType() == GAT_MAINPLAYER )
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_SECONDARYSKILL_RECIPE_BREAK");
		GameMsgPrint::getSingleton().SendUIEvent("GE_BREAK_PRE_ACTION");
	}
}
