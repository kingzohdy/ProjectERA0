#include "GameSkill_Package.h"
#include "GameActor.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMsgPrint.h"
#include "GameActor_RoleText.h"
#include "OgreResourceManager.h"
#include "ShowActor.h"
#include "GameSkill_Mgr.h"
#include "GameEffect_Mgr.h"
#include "OgreScriptLuaVM.h"
#include <Windows.h>
#include "GameSCTDamageManager.h"
#include "TerreneTraceEffect.h"
#include "proto_cs.h"
#include "GameDiathesis.h"
#include "GameDamageHealCounter.h"
#include "EntityEffect.h"
#include <algorithm>

using namespace Ogre;

bool isNormalSkill( int nSkillID, int nSkillLevel )
{
	return DBDataManager::getSingleton().GetSkillDef( nSkillID, nSkillLevel ).SkillType == SKILL_TYPE_NORMAL;
}

bool operator< ( const tagCSHurt& lhs, const tagCSHurt& rhs )
{
	return lhs.HurtType < rhs.HurtType;
}

GameSkill_Package::GameSkill_Package(void)
{
	m_uStartTime = timeGetTime();
	m_uStatusIdx = GameBufStatus::getBufStatusIdx();
}

GameSkill_Package::~GameSkill_Package(void)
{
}

bool GameSkill_Package::showDamageForAll()
{	
	if ( m_nAttackerID == 0 )
	{
		return false;
	}

	for( int i = 0; i < (int)m_HitValue.TotalHit; i++ )
	{
		if ( m_HitValue.Hits[i].ID == m_nAttackerID )
		{
			return true;
		}
	}

	return false;
}

// 执行技能包
void GameSkill_Package::doPackage()
{
	ActorManager *actorMgr = ActorManager::getSingletonPtr();
	if (actorMgr == NULL)
	{
		return;
	}
	GameActor *pAttackActor = actorMgr->FindActor(m_nAttackerID);
	if (pAttackActor && pAttackActor->getType() == GAT_MONSTER)
	{
		if (pAttackActor->getSubType() == MON_SUB_TYPE_BOSS && m_HitValue.TotalHit > 0)
		{
			GameDamageHealCounter::getSingleton().InsertBoss(m_nAttackerID, m_HitValue.Hits[0].ID);
		}
	}
	while (m_Hits.size() > 0)
	{
		doPackage(m_Hits.begin()->first);
	}
}

int GameSkill_Package::doPackage(int id)
{
	std::map<int, tagCSOneHit *>::iterator iter = m_Hits.find(id);
	if (iter == m_Hits.end())
	{
		return ACTION_HIT_NONE;
	}
	tagCSOneHit &Hits = *(iter->second);
	id = Hits.ID;
	m_Hits.erase(iter);
	int nResult = ACTION_HIT_NONE;
	MainGamePlayer* pMainPlayer		= ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return ACTION_HIT_NONE;
	}
	// pObj为受伤害的对象
	GameActor* pObj = ActorManager::getSingleton().FindActor(id);
	if(pObj == NULL) 
	{
		return ACTION_HIT_NONE;
	}
	if (m_hitbackIds.find(id) != m_hitbackIds.end())
	{
		GameSpell *pSpell = SkillManager::getSingleton().getSpellControler(id)->getCurSpell();
		if (pSpell)
		{
			pSpell->onHitBackPrepareTime();
		}
	}
	int totalDamage = 0;
	int totalHeal = 0;

	int subType = 0;
	if(pObj->getType() == GAT_MONSTER)
	{
		const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef(pObj->getResId());
		subType = monsterDef.SubType;
	}

	std::stable_sort(Hits.Hurts, Hits.Hurts +(int)Hits.TotalHurt);
	int nPreHurtType = HURT_TYPE_DMG;
	int nDamageIndex = 0;
	
	unsigned int uStartTime = timeGetTime();
	for (int i = 0; i < (int)Hits.TotalHurt; ++i)
	{
		if ((int)Hits.Hurts[i].HurtType == HURT_TYPE_DMG		||
			(int)Hits.Hurts[i].HurtType == HURT_TYPE_HEAVY_DMG	||
			(int)Hits.Hurts[i].HurtType == HURT_TYPE_HITDEAD)
		{
			totalDamage += Hits.Hurts[i].Value;
		}
		else if ((int)Hits.Hurts[i].HurtType == HURT_TYPE_HP)
		{
			totalHeal += Min(Hits.Hurts[i].Value, int(pObj->getMaxHP() - pObj->getHP()));
		}

		if (nPreHurtType != (int)Hits.Hurts[i].HurtType)
		{
			nPreHurtType = (int)Hits.Hurts[i].HurtType;
			nDamageIndex = 0;
		}

		SCTDamageManager&	sctMgr		= SCTDamageManager::getSingleton();
		sctMgr.setEntityAttr((int)Hits.Hurts[i].HurtType, Hits.Hurts[i].Value,
			Hits.ID);
		SCTDamageBase*	pSCTDamage	= NULL;
		if (Hits.ID == m_nAttackerID && m_nAttackerID != pMainPlayer->GetID())
		{
			pSCTDamage = sctMgr.getActiveSCT(pMainPlayer->GetID());
		}
		else
		{
			if (pMainPlayer->isRideMonster(pObj))
			{
				pSCTDamage = sctMgr.getPassiveSCT(pMainPlayer->GetID());
			}
			else
			{
				pSCTDamage	= sctMgr.getPassiveSCT(Hits.ID);
			}
		}

		pSCTDamage	= (pSCTDamage != NULL) ? pSCTDamage : sctMgr.getActiveSCT(m_nAttackerID);
		if (pSCTDamage != NULL)
		{
			pSCTDamage->calculateDamage(Hits.Hurts[i].Value, 
				m_HitValue.SkillID, m_HitValue.Level, 
				pObj,
				(int)Hits.Hurts[i].HurtType, 
				(int)Hits.TotalHurt, nDamageIndex);
			pSCTDamage->showFllyFont( uStartTime );
			++nDamageIndex;
		}

		pSCTDamage = NULL;
		if (m_nAttackerID != Hits.ID && (int)Hits.Hurts[i].HurtType == HURT_TYPE_HP)
		{
			pSCTDamage = sctMgr.getPassiveSCT(m_nAttackerID);
		}
		if (pSCTDamage != NULL)
		{
			pSCTDamage->calculateDamage(Hits.Hurts[i].Value, 
				m_HitValue.SkillID, m_HitValue.Level, 
				pObj,
				(int)Hits.Hurts[i].HurtType, 
				(int)Hits.TotalHurt, nDamageIndex);
			pSCTDamage->showFllyFont( uStartTime );
			++nDamageIndex;
		}

		if ((int)Hits.Hurts[i].HurtType == HURT_TYPE_DMG		||
			(int)Hits.Hurts[i].HurtType == HURT_TYPE_HEAVY_DMG	||
			(int)Hits.Hurts[i].HurtType == HURT_TYPE_HITDEAD)
		{
			//calSkillHitBack(pObj, Hits.Hurts[i].Value);
			GameDamageHealCounter::getSingleton().IncHurt(pObj->GetID(), Hits.Hurts[i].Value);
		}
		static const int pairs[][2] = {
			{HURT_TYPE_DMG, ACTION_HIT_HIT},
			{HURT_TYPE_HP, ACTION_HIT_HIT},
			{HURT_TYPE_MP, ACTION_HIT_HIT},
			{HURT_TYPE_SBSORB, ACTION_HIT_INVALID},
			{HURT_TYPE_NOEFFECT, ACTION_HIT_INVALID},
			{HURT_TYPE_MISS, ACTION_HIT_MISS},
			{HURT_TYPE_HEAVY_DMG, ACTION_HIT_HEAVY},
			{HURT_TYPE_HITDEAD, ACTION_HIT_HEAVY},
		};
		for (int n = 0; n < sizeof(pairs) / 2 / sizeof(int); ++n)
		{
			if ((int)Hits.Hurts[i].HurtType == pairs[n][0] && nResult < pairs[n][1])
			{
				nResult = pairs[n][1];
			}
		}
	}

	GameActor*	pAttackActor	= ActorManager::getSingleton().FindActor(m_nAttackerID);

	if (Hits.DeadFlag)
	{
		if (subType != MON_SUB_TYPE_TMP)
		{
			if (pObj == pMainPlayer && pObj->getActState().GetState() != GSACT_DEAD)
			{
				//GameMsgPrint::getSingleton().SendUIEvent("GE_MESSAGE_DEAD");
			}
			if (pObj->getActState().GetState() != GSACT_DEAD)
			{
				if (pObj->m_flagDead)
				{
					pObj->ChangeActState( GSACT_DEAD );
				}

				if (pAttackActor && pAttackActor != pObj)
				{
					pAttackActor->kill(pObj->GetID());
				}
			}
			if (pObj->m_flagDead)
			{
				pObj->reSetHp(0);
				pObj->setMP( 0 );
				GameMsgPrint::getSingleton().updateActorAttr(pObj);
			}

			// 如果受死亡的对象是怪物
			if (pAttackActor != NULL && pObj->getType() == GAT_MONSTER)
			{
				const tagMonsterSpeechDef* pMonsterSpeechDef = DBDataManager::getSingleton().getMonsterSpeechDef(pObj->getResId());
				if (pMonsterSpeechDef)
				{
					for (size_t i = 0; i < MAX_MON_ACT_SPEECH; ++i)
					{
						if (pMonsterSpeechDef->MonActSpeech[i].ConditionType == SPEECH_MON_DEAD &&
							(pMonsterSpeechDef->MonActSpeech[i].BroadcastType == BROADCAST_ONE_SELF ||
							pMonsterSpeechDef->MonActSpeech[i].BroadcastType == BROADCAST_MON_VIEW))
						{
							char szScript[256];
							sprintf(szScript, "SetMonsterDeadSpeeck(%d,%d,[===[%s]===],%d,%d)", 
								pObj->getResId(), pObj->GetID(), pAttackActor->getName(), pAttackActor->getRace(), pAttackActor->getGender());
							ActorManager::getSingleton().getScriptVM()->callString(szScript);
							break;
						}
					}
				}
			}
		}
		else
		{
			pObj->getActState().SetState(GSACT_DEAD);

			char szBuf[256];
			char szBuf2[256];
			sprintf(szBuf, "effect\\public\\%d_19.emo", pObj->getResId());
			sprintf(szBuf2, "%d_19", pObj->getResId()); 
			int res = Ogre::ResourceManager::getSingleton().getFileAttrib(szBuf);
			if (res != 0)
			{
				pObj->playMotion(szBuf2);
			}
			int actionId = pObj->getActState().getActionId();
			if (!pObj->getShowActor()->hasAnim(actionId))
			{
				pObj->getShowActor()->playAction(pObj->getActState().getActionId(true), NULL, 0, 0);
			}
			else
			{
				pObj->getShowActor()->playAction(actionId, NULL, 0, 0);
			}
		}
		nResult = ACTION_HIT_DEAD;
	}

	if (Hits.HitBackFlag)
	{
		tagSkillAttrib* pSkillAttr = SkillManager::getSingleton().getSpellDes(m_HitValue.SkillID);
		pObj->doBack(m_HitValue.SkillID,
			m_nAttackerID,
			Hits.HitBack.Pos.X,
			Hits.HitBack.Pos.Y,
			pSkillAttr->m_nHitBackTime,
			pSkillAttr->m_nHitBackType,
			pSkillAttr->m_nHitBackHight,
			pSkillAttr->m_nHitBackSpeed,
			pSkillAttr->m_strHitBackExtraInfo);
		if (pObj == pMainPlayer)
		{
			pMainPlayer->addMoveSeq(50);
		}
		if (pAttackActor != NULL && !pSkillAttr->m_strTraceEff.empty())
		{
			TerreneTraceEffect* effect = new TerreneTraceEffect(pSkillAttr->m_strTraceEff.c_str(), pSkillAttr->m_nTraceFadeTime, pSkillAttr->m_nTraceTotalTime, pSkillAttr->m_nTraceSpeed, 0, pObj->GetPosition(), pAttackActor->GetPosition());
			effect->start();
			EffectManage::getSingleton().cleanup(effect);
		}
	}

	if (Hits.TiShenFlag)
	{
		// 替身表现
		GameActor *pOwner = ActorManager::getSingleton().FindActor(pObj->getOwnerId());
		if (pOwner != NULL)
		{
			GameXYZ pos = pOwner->GetPosition();
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pObj->getResId());
			EntityEffect *effect = EffectManage::getSingleton().playEffect(pos.x / 10, pos.z / 10, pOwner->getAngle(), info.model, 1000);
			if (effect)
			{
				effect->setScale(2 * info.scale);
				effect->setTransparent(0.3);
				effect->playAction(10140);
			}
		}else if( pObj->getType() == GAT_PLAYER || pObj->getType() == GAT_MAINPLAYER ){
			for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
			{
				GameActor* pPet = pObj->m_Pet.getFightPet(i);
				if( pPet != NULL )
				{
					GameXYZ pos = pObj->GetPosition();
					const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pPet->getResId());
					EntityEffect *effect = EffectManage::getSingleton().playEffect(pos.x / 10, pos.z / 10, pObj->getAngle(), info.model, 1000);
					if (effect)
					{
						effect->setScale(2 * info.scale);
						effect->setTransparent(0.3);
						effect->playAction(10140);
					}
					break;
				}
			}
		}
	}

	GameDamageHealCounter::getSingleton().IncDamage(m_nAttackerID, totalDamage);
	GameDamageHealCounter::getSingleton().IncHeal(m_nAttackerID, totalHeal);

	return nResult;
}

// 设置对象
void GameSkill_Package::setTargetState(int id, const CSHITS& Package)
{
	m_HitValue		= Package;
	m_nAttackerID	= id;
	for (int i = 0; i < m_HitValue.TotalHit; ++i)
	{
		if (m_HitValue.Hits[i].ID != 0)
		{
			int nId = m_HitValue.Hits[i].ID;
			GameActor *pActor = ActorManager::getSingleton().FindActor(nId);
			if (pActor && pActor->getType() == GAT_PET)
			{
				if (m_HitValue.Hits[i].TiShenFlag != 0)
				{
					nId = pActor->getOwnerId();
				}
			}
			if (m_Hits.find(nId) == m_Hits.end())
			{
				m_Hits[nId] = &(m_HitValue.Hits[i]);
			}
		}
	}
}

// 刷新目标HP，MP
void GameSkill_Package::updateTargetValue()
{
	for( int i = 0; i < (int)m_HitValue.TotalHit; i++ )
	{	
		// pObj为受伤害的对象
		GameActor* pObj = ActorManager::getSingleton().FindActor( m_HitValue.Hits[i].ID );
		if( pObj == NULL ) 
		{
			continue;
		}
		GameSpell *pSpell = SkillManager::getSingleton().getSpellControler(m_HitValue.Hits[i].ID)->getCurSpell();
		if (pSpell && pSpell->isHitBackPrepareTime())
		{
			m_hitbackIds.insert(m_HitValue.Hits[i].ID);
		}

		for ( int j = 0; j < (int)m_HitValue.Hits[i].TotalHurt; j++ )
		{
			int nHurtType = (int)m_HitValue.Hits[i].Hurts[j].HurtType;
			// 血量改变
			if( nHurtType == HURT_TYPE_DMG		 || 
				nHurtType == HURT_TYPE_HEAVY_DMG || 
				nHurtType == HURT_TYPE_HITDEAD	 || 
				nHurtType == HURT_TYPE_HP ) 
			{
				pObj->setTmpHp( m_HitValue.Hits[i].HP );
			}
			else if (nHurtType == HURT_TYPE_MP)
			{
				int nMaxAddMP	= pObj->getMaxMP() - pObj->getMP();
				INT nCurAddMP	= m_HitValue.Hits[i].Hurts[j].Value;
				// TODO: 服务器发过来的数据没判断主角蓝是否已经满了
				pObj->setMP( pObj->getMP() + ( nCurAddMP > nMaxAddMP ? nMaxAddMP : nCurAddMP ) );
			}
		}
		if (m_HitValue.Hits[i].StatusFlag)
		{
			if (pObj->getType() == GAT_PET)
			{
				pObj->m_BufStatus.setBufStatus(m_HitValue.Hits[i].Status, m_uStatusIdx);
			}
			else
			{
				pObj->m_BufStatus.setBufStatus(m_HitValue.Hits[i].Status, m_uStartTime, m_uStatusIdx);
			}
		}
	}
}

// 计算技能吟唱回退打断
void GameSkill_Package::calSkillHitBack( GameActor* pObj, int nHitValue )
{
	bool isCollecting = false;
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pObj != pMainPlayer )
	{
		return;
	}

	GameSpell* pSpell = SkillManager::getSingleton().getSpellControler()->getCurPreSpell();
	if (pSpell == NULL)
	{
		isCollecting = true;
		pSpell = SkillManager::getSingleton().getSpellControler()->getCurCloSpell();
	}
	if ( pSpell == NULL )
	{
		return;
	}

	int		nPreTimeDesc = 20;
	// 检查天赋对吟唱打退时间的影响
	DiathesisManager&  diathesisMgr = pMainPlayer->getDiathesisManager();
	const tagDiathesisInfo_toLua& diathesisInfo = diathesisMgr.getDiathesisInfo();
	for ( uint i = 0; i < diathesisInfo.DiathesisNum; ++i )
	{
		const tagDiathesisData_toLua& diathesisData = diathesisInfo.DiathesisList[i];
		const tagDiathesisDef* pDiathesisDef  = &DBDataManager::getSingleton().getDiathesisDef( 
			diathesisData.DiathesisID, diathesisData.DiathesisLevel );
		if ( pDiathesisDef == NULL )
		{
			continue;
		}

		for ( int j = 0; j < MAX_DIATHESIS_RESULT; ++j )
		{
			const SKILLRESULT& skillResult = pDiathesisDef->DiathesisResult[j];
			if ( skillResult.SkillResultID == RESULT_DEC_PRE_TIME_RATE )
			{
				nPreTimeDesc -= skillResult.SkillResultVal1;
			}
		}
	}
	if (nPreTimeDesc < 0)
	{
		nPreTimeDesc = 0;
	}

	double backTime = 0;

	unsigned int now = timeGetTime();

	if (isCollecting)
	{
		if (pSpell->m_lastTime != 0)
		{
			unsigned int nColTime = SkillManager::getSingleton().getSkillCollectTime(pSpell->getSpellId()) * 100;

			unsigned int nCurColTime = now - pSpell->m_lastTime;
			if (nCurColTime * 100 / nColTime < nPreTimeDesc)
			{
				backTime = nCurColTime / 1000.0;
			}
			else
			{
				backTime = (nColTime / 1000.0) * (nPreTimeDesc / 100.0);
			}
			pSpell->m_lastTime += (unsigned int)(backTime * 1000);
			if (pSpell->m_lastTime > now)
			{
				pSpell->m_lastTime = now;
			}
		}
	}
	else
	{
		int nPreTime = SkillManager::getSingleton().getSkillPrepareTime(pSpell->getSpellId()) * 100;

		int nCurPreTime = now - pSpell->m_lastTime;

		if (nCurPreTime * 100 / nPreTime < nPreTimeDesc)
		{
			backTime = nCurPreTime / 1000.0;
		}
		else
		{
			backTime = (nPreTime / 1000.0) * (nPreTimeDesc / 100.0);
		}
		pSpell->m_lastTime += (unsigned int)(backTime * 1000);
		if (pSpell->m_lastTime > now)
		{
			pSpell->m_lastTime = now;
		}
	}

	if (backTime >= 0.001)
	{
		char szBuf[256];
		sprintf(szBuf, "skillPreBack(%.3f)", backTime);
		ActorManager::getSingleton().getScriptVM()->callString( szBuf );
	}
}

unsigned int GameSkill_Package::getTime()
{
	return m_uStartTime;
}
