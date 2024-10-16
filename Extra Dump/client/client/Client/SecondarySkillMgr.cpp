#include "SecondarySkillMgr.h"
#include "GameActorManager.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "GameCollect.h"
#include "ShowActor.h"
#include "OgreScriptLuaVM.h"
#include "GameUIMsg.h"
#include "GameChatManager.h"
#include "GameQuest.h"

SecondarySkillMgr::SecondarySkillMgr()
{
	static_assert(sizeof(tagSecondarySkillDef_toLua) == sizeof(tagSecondarySkillDef));
	static_assert( sizeof(tagExtendedResult_ToLua) == sizeof(tagExtendedResult) );
	clearSecondarySkillData();
	INetworkManager::getSingleton().RegisterMsgHandler(SECONDARY_SKILL_SVR,this);
	INetworkManager::getSingleton().RegisterMsgHandler(EXTEND_ITEM_SVR,this);
}
SecondarySkillMgr::~SecondarySkillMgr()
{
	clearSecondarySkillData();
}

int SecondarySkillMgr::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid == SECONDARY_SKILL_SVR )
	{
		switch( msgbody.SecondarySkillSvr.SvrOp )
		{
		case SECONDARY_SKILL_SVR_INFO:
			{
				m_SecondaryData.SkillNum = msgbody.SecondarySkillSvr.SvrData.AllInfo.SkillNum;
				for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
				{
					m_SecondaryData.SkillInfo[i] = msgbody.SecondarySkillSvr.SvrData.AllInfo.SkillInfo[i];
				}
				GameManager::getSingleton().SendUIEvent("GE_SECONDARYSKILL_SKILL_CHG");
				GameQuest::getSingleton().updateNpcQuestTitle();
			}
			break;
		case SECONDARY_SKILL_SVR_RECIPE_CHG:
			{
				for( int i = 0 ; i < m_SecondaryData.SkillNum ; i ++ )
				{
					if( m_SecondaryData.SkillInfo[i].SkillType == msgbody.SecondarySkillSvr.SvrData.RecipeChg.SkillType )
					{
						bool bFound = false;
						tagUIMsgBody uiMsg;
						uiMsg.SecondarySkill.skillType = msgbody.SecondarySkillSvr.SvrData.RecipeChg.SkillType;
						uiMsg.SecondarySkill.skillId = msgbody.SecondarySkillSvr.SvrData.RecipeChg.RecipeID;
						if( !isLearnedRecipe( msgbody.SecondarySkillSvr.SvrData.RecipeChg.SkillType,msgbody.SecondarySkillSvr.SvrData.RecipeChg.RecipeID ) )
						{
							learnRecipe(msgbody.SecondarySkillSvr.SvrData.RecipeChg.SkillType,msgbody.SecondarySkillSvr.SvrData.RecipeChg.RecipeID);
							GameManager::getSingleton().SendUIEvent("GE_SECONDARYSKILL_LEARNED_RECIPE");
							const RECIPEDEF* pRecipeDef = DBDataManager::getSingleton().getRecipeDef(uiMsg.SecondarySkill.skillType,uiMsg.SecondarySkill.skillId);
							if( pRecipeDef != NULL )
							{
								char szBuf[256];
								sprintf(szBuf,"#cDCD27D你学会了%s",pRecipeDef->NameDesc );
								ChatManager::getSingleton().addMakeSucessMsg(0,szBuf);
							}
						}
						if( msgbody.SecondarySkillSvr.SvrData.RecipeChg.CoolDownTime > 0 )
						{
							for( int j = 0 ; j < m_SecondaryData.SkillInfo[i].CoolDownNum ; j++ )
							{
								if( m_SecondaryData.SkillInfo[i].CoolDownInfo[j].RecipeID == msgbody.SecondarySkillSvr.SvrData.RecipeChg.RecipeID )
								{
									bFound = true;
									m_SecondaryData.SkillInfo[i].CoolDownInfo[j].CoolDownTime = msgbody.SecondarySkillSvr.SvrData.RecipeChg.CoolDownTime;
									break;
								}
							}
							if( !bFound && m_SecondaryData.SkillInfo[i].CoolDownNum < MAX_SECONDARY_SKILL_COOLDOWN_RECIPE )
							{
								m_SecondaryData.SkillInfo[i].CoolDownInfo[m_SecondaryData.SkillInfo[i].CoolDownNum ].CoolDownTime = msgbody.SecondarySkillSvr.SvrData.RecipeChg.CoolDownTime;
								m_SecondaryData.SkillInfo[i].CoolDownInfo[m_SecondaryData.SkillInfo[i].CoolDownNum ].RecipeID = msgbody.SecondarySkillSvr.SvrData.RecipeChg.RecipeID;
								m_SecondaryData.SkillInfo[i].CoolDownNum ++;
							}
						}
						break;
						GameManager::getSingleton().SendUIEvent("GE_SECONDARYSKILL_RECIPECHG");
					}
				}
			}
			break;
		case SECONDARY_SKILL_SVR_DEL_SKILL:
			{
				for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
				{
					if( m_SecondaryData.SkillInfo[i].SkillType == msgbody.SecondarySkillSvr.SvrData.DelSkill.SkillType )
					{
						for( int j = i ; j < m_SecondaryData.SkillNum - 1; j++ )
						{
							m_SecondaryData.SkillInfo[j] = m_SecondaryData.SkillInfo[j+1];
						}
						m_SecondaryData.SkillNum --;
						break;
					}
				}
				GameQuest::getSingleton().updateNpcQuestTitle();
				GameManager::getSingleton().SendUIEvent("GE_SECONDARYSKILL_SKILL_DEL");
			}
			break;
		case SECONDARY_SKILL_SVR_SKILL_CHG:
			{
				bool bFound = false;
				for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
				{
					if( m_SecondaryData.SkillInfo[i].SkillType == msgbody.SecondarySkillSvr.SvrData.SkillChg.SkillType )
					{
						bFound = true;
						m_SecondaryData.SkillInfo[i].SkillLevel = msgbody.SecondarySkillSvr.SvrData.SkillChg.SkillLevel;
						m_SecondaryData.SkillInfo[i].SkillVal = msgbody.SecondarySkillSvr.SvrData.SkillChg.SkillVal;
						break;
					}
				}
				if( !bFound )
				{
					m_SecondaryData.SkillInfo[m_SecondaryData.SkillNum].SkillType = msgbody.SecondarySkillSvr.SvrData.SkillChg.SkillType;
					m_SecondaryData.SkillInfo[m_SecondaryData.SkillNum].SkillLevel = msgbody.SecondarySkillSvr.SvrData.SkillChg.SkillLevel;
					m_SecondaryData.SkillInfo[m_SecondaryData.SkillNum].SkillVal = msgbody.SecondarySkillSvr.SvrData.SkillChg.SkillVal;
					memset( m_SecondaryData.SkillInfo[m_SecondaryData.SkillNum ].RecipeInfo,0,sizeof( m_SecondaryData.SkillInfo[m_SecondaryData.SkillNum ].RecipeInfo ) );
					m_SecondaryData.SkillNum ++;
				}
				GameQuest::getSingleton().updateNpcQuestTitle();
				GameManager::getSingleton().SendUIEvent("GE_SECONDARYSKILL_SKILL_CHG");
			}
			break;
		case SECONDARY_SKILL_SVR_RECIPE_PRE:
			{
				const CSSECONDARYSKILLPRE& pre = msgbody.SecondarySkillSvr.SvrData.SkillPre;
				GameActor* pActor = ActorManager::getSingleton().FindActor( pre.MemID );
				if( pActor && pActor->getType() != GAT_MAINPLAYER )
				{
					tagRecipeDef_toLua* pDef = getRecipeDef(pre.SkillType,pre.RecipeID);
					if( pDef )
					{
						pActor->startSecondaryChant(pDef->MachiningTime*1000,pre.SkillType,pre.RecipeID);
					}
				}
			}
			break;
		case SECONDARY_SKILL_SVR_RECIPE_PRE_BRK:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( msgbody.SecondarySkillSvr.SvrData.SkillPreBrk.MemID );
				if( pActor != NULL )
				{
					pActor->onBreakChant( eSecondary_Chant );
				}
			}
			break;
		case SECONDARY_SKILL_SVR_RECIPE_SUCC:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor(msgbody.SecondarySkillSvr.SvrData.SuccInfo.MemID);
				if( pActor != NULL )
				{
					const CSSECONDARYSKILLSUCC& succInfo = msgbody.SecondarySkillSvr.SvrData.SuccInfo;
					if((succInfo.SkillType == CLIENT_SECONDRAY_SKILL_TYPE_MINING && succInfo.RecipeID < 100) ||
						(succInfo.SkillType == CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM && succInfo.RecipeID < 100))
					{
					}
					pActor->finishChant( eSecondary_Chant );
					if( pActor->getType() == GAT_MAINPLAYER )
					{
						GameMsgPrint::getSingleton().SendUIEvent("GE_SECONDARYSKILL_RECIPE_SUCC");
					}
					const RECIPEDEF* pRecipeDef = DBDataManager::getSingleton().getRecipeDef(succInfo.SkillType,succInfo.RecipeID);
					if( pRecipeDef != NULL )
					{
						if( (pRecipeDef->SkillType == CLIENT_SECONDRAY_SKILL_TYPE_MINING || pRecipeDef->SkillType ==CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM)
							&& pRecipeDef->RecipeID < 101 )
						{ 
							if( pActor->getType() != GAT_MAINPLAYER  )
							{
								break;
							}
						}
						char szBuf[256];
						sprintf(szBuf,"%s制造了@ITEMLINK@ X %d",pActor->getType() == GAT_MAINPLAYER?"你":pActor->getName(),succInfo.ItemNum);
						ChatManager::getSingleton().addMakeSucessMsg(succInfo.ItemID,szBuf);
					}
				}
			}
			break;
		default:
			break;
		}
	}
	//附魔相关
	if( msgid == EXTEND_ITEM_SVR )
	{
		if( msgbody.ExtendItemSvr.Succeeded )
		{
			//成功
			GameManager::getSingleton().SendUIEvent("GE_ENCHANT_SUCCEED");
			memcpy( &m_ExtendResult, &msgbody.ExtendItemSvr.Result,sizeof(tagExtendedResult_ToLua) );
			char buf[256];
			sprintf( buf,"UpdateEnchantFrameValue( %d )",msgbody.ExtendItemSvr.FromItemID );
			GameManager::getSingleton().getScriptVM()->callString(buf);

		}else{
			//失败
			memset( &m_ExtendResult, 0, sizeof(tagExtendedResult_ToLua) );
		}		 
	}
	return 0;
}
tagExtendedResult_ToLua* SecondarySkillMgr::getExtendResult()
{
	return &m_ExtendResult;
}

void SecondarySkillMgr::saveExtendResult( int nFromID, int nDestList,int nDestGrid )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL )
	{
		return;
	}
	Item& destItem = pMain->getContainer().getItem( nDestList,nDestGrid );
	if( destItem.getItemId() != 0  )
	{
		tagCSPkgBody msgbody;
		msgbody.ExtendItemSave.BeExtended.ListType	= nDestList;
		msgbody.ExtendItemSave.BeExtended.GridIdx	= nDestGrid;
		msgbody.ExtendItemSave.BeExtended.WID		= destItem.getItemWId(); 
		msgbody.ExtendItemSave.FromItemID			= nFromID;
		memcpy( &msgbody.ExtendItemSave.Result, getExtendResult(),sizeof(tagExtendedResult_ToLua ) );
		SendGameServerMsg(EXTEND_ITEM_SAVE,msgbody);
	}
}

void SecondarySkillMgr::requestLearnRecipt(int nType,int id)
{
	const RECIPEDEF* pRecipeDef = DBDataManager::getSingleton().getRecipeDef(nType,id);
	if( pRecipeDef != NULL )
	{
		tagCSPkgBody msgbody;
		msgbody.SecondarySkillClt.CltOp = SECONDARY_SKILL_CLT_LEARN_RECIPE;
		msgbody.SecondarySkillClt.CltData.LearnRecipe.RecipeID = id;
		msgbody.SecondarySkillClt.CltData.LearnRecipe.SkillType = nType;
		SendGameServerMsg(SECONDARY_SKILL_CLT,msgbody);
	}
}

void SecondarySkillMgr::requestUseSkillPre(int nType,int id)
{
	//对cd 的判断
	tagRecipeCoolDownInfo_toLua* coolInfo = getSkillCoolInfo(nType,id);
	if( coolInfo != NULL && coolInfo->CoolDownTime > 0 )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_SECONDARYSKILL_FAILED_COLDDOWN );
		return;
	}
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( !(pMain->getActState().GetState() == GSACT_STAND || pMain->getActState().GetState() == GSACT_MOVE) 
		|| pMain->m_Equip.isUsedTmp()|| pMain->isInMorph() ||pMain->getActState().getInState(GSACT_IN_USEDMACHINE)
		||pMain->getActState().getInState(GSACT_IN_CARRY) || !pMain->getBufStatus()->canUsedSpell() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_SECONDARYSKILL_WRONG_STATE);
		return;
	}/*else if( pMain->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_SECONDARYSKILL_FAILED_INFIGHT);
		return;
	}*/else if( pMain->isInSkillTetanic()
		||pMain->getActState().getInState(GSACT_IN_CAST) )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_SECONDARYSKILL_FAILED_INCASTING);
		return;
	}
	if( !pMain->canControl() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_SECONDARYSKILL_FAILED_CANNTCONTROL);
		return;
	}else if( pMain->isInRide() )
	{
		if( !pMain->m_Rider.isOwnRide() )
		{
			GameMsgPrint::getSingleton().showMidTip(MID_SECONDARYSKILL_WRONG_STATE);
			return;
		}
		pMain->m_Rider.requestRideDown();
	}
	if( !pMain->canStartChant( eSecondary_Chant ) )
	{
		return;
	}
	pMain->CmdMoveStop();
	const RECIPEDEF* pRecipeDef = DBDataManager::getSingleton().getRecipeDef(nType,id);
	if( pRecipeDef != NULL )
	{
		requestReciptPre(nType,id);
		pMain->startSecondaryChant(pRecipeDef->MachiningTime*1000,nType,id);
	}
}

void SecondarySkillMgr::requestReciptPre(int nType,int id)
{
	tagCSPkgBody msgbody;
	msgbody.SecondarySkillClt.CltOp = SECONDARY_SKILL_CLT_RECIPE_PRE;
	msgbody.SecondarySkillClt.CltData.RecipePre.SkillType = nType;
	msgbody.SecondarySkillClt.CltData.RecipePre.RecipeID = id;
	SendGameServerMsg(SECONDARY_SKILL_CLT,msgbody);
}

void SecondarySkillMgr::requestBreakReciptPre()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain->isInPre(eSecondary_Chant) )
	{
		pMain->requestBreakChant( eSecondary_Chant );
	}
}

void SecondarySkillMgr::requestReciptResolve(int nList,int gridIdx)
{
	tagCSPkgBody msgbody;
	msgbody.SecondarySkillClt.CltOp = SECONDARY_SKILL_CLT_RECIPE_RESOLVE;
	msgbody.SecondarySkillClt.CltData.Resolve.ListType = nList;
	msgbody.SecondarySkillClt.CltData.Resolve.GridIdx = gridIdx;
	SendGameServerMsg(SECONDARY_SKILL_CLT,msgbody);
}


void SecondarySkillMgr::requestDelSkill(int nSkillType)
{
	tagCSPkgBody msgbody;
	msgbody.SecondarySkillClt.CltOp = SECONDARY_SKILL_CLT_DEL_SKILL;
	msgbody.SecondarySkillClt.CltData.DelSkill.SkillType = nSkillType;
	SendGameServerMsg(SECONDARY_SKILL_CLT,msgbody);
}

bool SecondarySkillMgr::isLearnedRecipe( int nType,int id )
{
	if( id <0 || id > 1023 )
	{
		assert(0&&"id错误");
		return false;
	}
	for( int i = 0 ; i < m_SecondaryData.SkillNum ; i ++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == nType )
		{
			return (m_SecondaryData.SkillInfo[i].RecipeInfo[id/8] & 1<<id%8);
		}
	}
	return false;
}

int SecondarySkillMgr::getSecondarySkillNum()
{
	return m_SecondaryData.SkillNum;
}

tagSecondarySkillInfo_toLua* SecondarySkillMgr::getSecondarySkillInfo( int index )
{
	static_assert(sizeof(tagSecondarySkillInfo_toLua)==sizeof(tagSecondarySkillInfo));
	if( index < m_SecondaryData.SkillNum )
	{
		return (tagSecondarySkillInfo_toLua*)(&m_SecondaryData.SkillInfo[index]);
	}
	return NULL;
}

tagSecondarySkillInfo_toLua* SecondarySkillMgr::getSecondarySkillInfoByType( int type )
{
	static_assert(sizeof(tagSecondarySkillInfo_toLua)==sizeof(tagSecondarySkillInfo));
	for( int i = 0 ; i< m_SecondaryData.SkillNum; i++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == type )
		{
			return (tagSecondarySkillInfo_toLua*)(&m_SecondaryData.SkillInfo[i]);
		}
	}
	return NULL;
}

tagSecondarySkillDef_toLua* SecondarySkillMgr::getSecondarySkillData(int nType,int nLevel)
{
	static_assert(sizeof(tagSecondarySkillDef_toLua) == sizeof(tagSecondarySkillDef) );
	return (tagSecondarySkillDef_toLua*) DBDataManager::getSingleton().getSecondarySkillDef(nType,nLevel);
}
tagRecipeDef_toLua* SecondarySkillMgr::getRecipeDef(int nType,int id)
{
	static_assert(sizeof(tagRecipeDef_toLua) == sizeof(tagRecipeDef));
	return (tagRecipeDef_toLua*) DBDataManager::getSingleton().getRecipeDef(nType,id);
}

void SecondarySkillMgr::clearSecondarySkillData()
{
	memset(&m_SecondaryData,0,sizeof(tagSecondarySkillMiscInfo));
}

bool SecondarySkillMgr::startCollect(int nType,int nRecipeID)
{
	for( ACTOR_ITER iter = ActorManager::getSingleton().getActorBegin();
		iter != ActorManager::getSingleton().getActorEnd();iter ++)
	{
		GameActor* pActor = iter->second;
		if( pActor->getType() == GAT_MONSTER || pActor->getSubType() == MON_SUB_TYPE_COLLECT )
		{
			const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pActor->getResId() );
			if( monsterDef.Collect.CollectSecondarySkillType == nType &&
				monsterDef.Collect.CollectRecipeID == nRecipeID )
			{
				GameCollect::getSingleton().collect( pActor->GetID() );
				return true;
			}
		}
	}
	return false;
}

bool SecondarySkillMgr::canCollect(int nresId)
{
// 	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( nresId );
// 	if( monsterDef.Collect.CollectSecondarySkillType > 0  )
// 	{
// 		if( isLearnedRecipe( monsterDef.Collect.CollectSecondarySkillType,monsterDef.Collect.CollectRecipeID ))
// 		{
// 			return true;
// 		}
// 		return false;
// 	}
	return true;
}

bool SecondarySkillMgr::checkCollect(int nresId)
{
// 	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( nresId );
// 	if( monsterDef.Collect.CollectSecondarySkillType > 0  )
// 	{
// 		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
// 		if( !(pMain->getActState().GetState() == GSACT_STAND || pMain->getActState().GetState() == GSACT_MOVE) || pMain->m_Equip.isUsedTmp()
// 			|| pMain->isInMorph() || pMain->isInRide()  )
// 		{
// 			//GameMsgPrint::getSingleton().showMidTip(MID_SECONDARYSKILL_WRONG_STATE);
// 			//pMain->m_Rider.rideDown();
// 			return true;
// 		}
// 	}
 	return true;
}

bool SecondarySkillMgr::checkRequireItem(int nresId)
{
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( nresId );
	if( monsterDef.Collect.CollectSecondarySkillType > 0 )
	{
		char buf[256] = {0};
		if( !isLearnedRecipe( monsterDef.Collect.CollectSecondarySkillType,monsterDef.Collect.CollectRecipeID ) )
		{
			sprintf(buf,"SecondarySkillCollectLevel(%d,%d)",monsterDef.Collect.CollectSecondarySkillType,monsterDef.Collect.CollectRecipeID);
			GameManager::getSingleton().getScriptVM()->callString(buf);
			return false;
		}
		//判断是否有需要的装备
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for( int i = EQUIP_POS_RIGHTHAND; i<EQUIP_POS_END; i ++ )
		{
			if( pMain->m_Equip.getItem(i).getItemId() != 0  )
			{
				tagItemDef_ToLua* pItemDef = pMain->m_Equip.getItem(i).getItemDef();
				if( pItemDef != NULL )
				{
					for( int idx = 0 ; idx < MAX_ITEM_RESULT; idx ++ )
					{
						if( pItemDef->Result[idx].ResultID == RESULT_SECONDARY_SKILL_ITEM 
							&& pItemDef->Result[idx].ResultVal1 == monsterDef.Collect.CollectSecondarySkillType )
						{
							return true;
						}
					}
				}
			}
		}
		
		sprintf(buf,"SecondarySkillNoNeedItem(%d)",monsterDef.Collect.CollectSecondarySkillType);
		GameManager::getSingleton().getScriptVM()->callString(buf);
		return false;
	}
	return true;
}

bool SecondarySkillMgr::canLearnSkill(int nType,int nRecipeId)
{
	const RECIPEDEF* pRecipeDef = DBDataManager::getSingleton().getRecipeDef(nType,nRecipeId);
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pRecipeDef != NULL && pMain != NULL)
	{
		if( !isLearnedRecipe(pRecipeDef->SkillType,pRecipeDef->RecipeID)
			&& isLearnedSecondarySkill( nType,pRecipeDef->LearnSkillLevel) )
		{
			return true;
		}
	}
	return false;
}

void SecondarySkillMgr::learnRecipe(int nType,int nRecipeID)
{
	if( nRecipeID <0 || nRecipeID > 1023 )
	{
		assert(0&&"id错误");
		return;
	}
	for( int i = 0 ; i < m_SecondaryData.SkillNum ; i ++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == nType )
		{
			m_SecondaryData.SkillInfo[i].RecipeInfo[nRecipeID/8] |= 1<<nRecipeID%8;
			return;
		}
	}
}

void SecondarySkillMgr::endCollect()
{
	ActorManager::getSingleton().getMainPlayer()->breakChannel();
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain != NULL )
	{
		pMain->getActState().SetState(GSACT_STAND);
		pMain->onChangeStand();
	}
	GameManager::getSingleton().SendUIEvent("GE_SECONDARYSKILL_RECIPE_BREAK"); 
}

bool SecondarySkillMgr::isLearnedSecondarySkill( int nType )
{
	for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == nType )
		{
			return true;
		}
	}
	return false;
}

bool SecondarySkillMgr::isInSeondarySkillPre()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	return ( pMain->isInPre( eSecondary_Chant ) );
}

bool SecondarySkillMgr::isLearnedSecondarySkill( int nType,int nSkillLevel )
{
	for( int i = 0 ; i < m_SecondaryData.SkillNum; i ++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == nType && m_SecondaryData.SkillInfo[i].SkillLevel >= nSkillLevel )
		{
			return true;
		}
	}
	return false;
}

bool SecondarySkillMgr::hasSecondarySkill( int nType,int nSkillLevel )
{
	for( int i = 0 ; i < m_SecondaryData.SkillNum; i ++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == nType && m_SecondaryData.SkillInfo[i].SkillLevel == nSkillLevel )
		{
			return true;
// 			tagSecondarySkillDef_toLua* skillDef = getSecondarySkillData( nType, nSkillLevel);
// 			if( skillDef != NULL && m_SecondaryData.SkillInfo[i].SkillVal >= skillDef->LevelUpVal )
// 			{
// 				return true;
// 			}			
		}
	}
	return false;
}

bool SecondarySkillMgr::canLearnNextLevel( int nType,int nSkillLevel )
{
	for( int i = 0 ; i < m_SecondaryData.SkillNum; i ++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == nType && m_SecondaryData.SkillInfo[i].SkillLevel == nSkillLevel )
		{			
 			tagSecondarySkillDef_toLua* skillDef = getSecondarySkillData( nType, nSkillLevel);
 			if( skillDef != NULL && m_SecondaryData.SkillInfo[i].SkillVal >= skillDef->LevelUpVal )
 			{
 				return true;
 			}			
		}
	}
	return false;
}

bool SecondarySkillMgr::HasRecipeType1( int SkillType,int nType )
{
	for( int i = 0 ; i < MAX_RECIPE_NUM ; i++ )
	{
		if(isLearnedRecipe(SkillType,i) )
		{
			const RECIPEDEF* recipeDef = DBDataManager::getSingleton().getRecipeDef(SkillType,i);
			if( recipeDef->RecipeType1 == nType )
			{
				return true;
			}
		}
	}
	return false;
}

bool SecondarySkillMgr::LinkHasRecipeType1( int nLinkId,int nType )
{
	tagSecondarySkillLink_ToLua skillLink = ChatManager::getSingleton().getSecondarySkillLink(nLinkId);
	if( skillLink.SkillType != 0 )
	{
		for( int i = 0 ; i < MAX_RECIPE_NUM ; i++ )
		{
			if( skillLink.RecipeInfo[i/8] & 1<<(i%8) )
			{
				const RECIPEDEF* recipeDef = DBDataManager::getSingleton().getRecipeDef(skillLink.SkillType,i);
				if( recipeDef->RecipeType1 == nType )
				{
					return true;
				}
			}
		}
	}	
	return false;
}

bool SecondarySkillMgr::HasRecipeType2( int SkillType,int nType )
{
	for( int i = 0 ; i < MAX_RECIPE_NUM ; i++ )
	{
		if(isLearnedRecipe(SkillType,i) )
		{
			const RECIPEDEF* recipeDef = DBDataManager::getSingleton().getRecipeDef(SkillType,i);
			if( recipeDef->RecipeType2 == nType )
			{
				return true;
			}
		}
	}
	return false;
}

void SecondarySkillMgr::setSecondarySkillLink( int skillType )
{
	for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == skillType )
		{
			tagSecondarySkillLink_ToLua secondarySkillLink;
			secondarySkillLink.SkillType = skillType;
			secondarySkillLink.SkillLevel = m_SecondaryData.SkillInfo[i].SkillLevel;
			secondarySkillLink.SkillVal = m_SecondaryData.SkillInfo[i].SkillVal;
			memcpy(secondarySkillLink.RecipeInfo,m_SecondaryData.SkillInfo[i].RecipeInfo,sizeof(m_SecondaryData.SkillInfo[i].RecipeInfo));
			ChatManager::getSingleton().setSecondarySkillLink(secondarySkillLink);
			return;
		}
	}
}


bool SecondarySkillMgr::isLinkLearnedRecipe(int linkId,int recipeID)
{
	if( recipeID <0 || recipeID > 1023 )
	{
		assert(0&&"recipeID错误");
		return false;
	}
	tagSecondarySkillLink_ToLua skillLink = ChatManager::getSingleton().getSecondarySkillLink(linkId);
	if( skillLink.SkillType != 0 )
	{
		return skillLink.RecipeInfo[recipeID/8] & 1<<(recipeID%8);
	}
	return false;
}

int SecondarySkillMgr::getNonCommSecondaySkillNum()
{
	int nNum = 0;
	for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType != CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING )
		{
			nNum ++;
		}
	}
	return nNum;
}

// 使用附魔物品
void SecondarySkillMgr::requestUseEnchant( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL )
	{
		return;
	}
	Item& srcItem = pMain->getContainer().getItem( nSrcList,nSrcGrid );
	Item& destItem = pMain->getContainer().getItem( nDestList,nDestGrid );
	if( srcItem.getItemId() != 0 && destItem.getItemId() != 0  )
	{
		tagCSPkgBody msgbody;
		msgbody.ExtendItemClt.BeUsed.ListType= nSrcList ;
		msgbody.ExtendItemClt.BeUsed.GridIdx = nSrcGrid;
		msgbody.ExtendItemClt.BeUsed.WID = srcItem.getItemWId();
		msgbody.ExtendItemClt.BeExtended.ListType = nDestList ;
		msgbody.ExtendItemClt.BeExtended.GridIdx = nDestGrid;
		msgbody.ExtendItemClt.BeExtended.WID = destItem.getItemWId(); 
		SendGameServerMsg(EXTEND_ITEM_CLT,msgbody);
	}
}

tagRecipeCoolDownInfo_toLua* SecondarySkillMgr::getSkillCoolInfo( int skillType, int recipeId )
{
	for( int i = 0 ; i < m_SecondaryData.SkillNum ; i++ )
	{
		if( m_SecondaryData.SkillInfo[i].SkillType == skillType )
		{
			for( int j = 0 ; j < m_SecondaryData.SkillInfo[i].CoolDownNum ; j++ )
			{
				if( m_SecondaryData.SkillInfo[i].CoolDownInfo[j].RecipeID == recipeId )
				{
					return (tagRecipeCoolDownInfo_toLua*)( &m_SecondaryData.SkillInfo[i].CoolDownInfo[j] );
				}
			}
		}
	}
	return NULL;
}