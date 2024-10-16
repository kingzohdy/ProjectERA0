#include <Windows.h>
#include "GameSkill_Mgr.h"
#include "GameSkill_Base.h"
#include "GameDBData.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "OgreXMLData.h"
#include "OgreScriptLuaVM.h"
#include "OgreLog.h"
#include "GameActor.h"
#include "ServerTimer.h"
#include "GameMsgPrint.h"
#include "GameMap.h"
#include "GameMainPlayer.h"
#include "PworldLimitInfo.h"
// skill type
#include "GameSkill_Control.h"
#include "SkillGenera.h"
#include "SkillAttackP2P.h"
#include "SkillCircularAttack.h"
#include "SkillSelectAngle.h"
#include "SkillScan.h"
#include "SkillTrig.h"
#include "SkillVigor.h"
#include "SkillLine.h"

#include "GameSkill_Package.h"
#include "SkillSelectAttack.h"
#include "SkillRandom.h"
#include "SkillBlow.h"
#include "Skilllink.h"
#include "GameSkill_Hit.h"

#include "GameBufStatus.h"

const float AUTO_NEXT_CD = 0.5f;

std::map<int, FUN> SkillManager::m_SkillCreateFun;
std::map<int, tagSkillAttrib*> SkillManager::m_SkillDes;
std::map<int, ATTRIB_MAP> SkillManager::m_SkillAttribMap;

using namespace Ogre;

SkillManager::SkillManager() : m_SpellUIControl(NULL), m_skillReq(0), m_lockSkill(0), m_pNextSpellId(0), m_isInAttack(-1)
{
	INetworkManager::getSingleton().RegisterMsgHandler( SKILL_PREPARE_SVR,  this );
	INetworkManager::getSingleton().RegisterMsgHandler( SKILL_BRK_SVR,  this );
	INetworkManager::getSingleton().RegisterMsgHandler( SKILL_INFO,  this );

	// 初始化技能模板
	initSkill();
}

SkillManager::~SkillManager()
{
	clearSkill();
}

void SkillManager::registerSpell( int nId, FUN p, tagSkillAttrib* pDes )
{
	m_SkillCreateFun[nId]	= p;
	m_SkillDes[nId]			= pDes;
}

void SkillManager::registerSpellAttrib( int nId, ATTRIB_MAP& attrib_map )
{
	m_SkillAttribMap[nId] = attrib_map;
}

int SkillManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{	
	if( SKILL_PREPARE_SVR == msgid )
	{
		const CSSKILLPRES& Msg = msgbody.SkillPreS;
		ISpellControler *sc = getSpellControler(Msg.ID);
		MainGamePlayer* mainPlayer = ActorManager::getSingleton().getMainPlayer();
		//返回消息是主角 禁止
		if ( mainPlayer && mainPlayer->GetID() == Msg.ID )
		{
			return 0;
		}
		if (sc)
		{
			GameSpell* pSpell = sc->getSpell(Msg.RuneTransferedID == 0 ? Msg.SkillID : Msg.RuneTransferedID, Msg.SkillLevel, 0);
			GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
			if (pActor)
			{
				pSpell->setSelfPoint(pActor->GetPosition());
			}
			pSpell->setTargetPoint(msgbody.SkillPreS.AtkPos.X*10, msgbody.SkillPreS.AtkPos.Y*10);
	//		pSpell->setSingleTarget( Msg.AtkeeID );
			const tagSkillDef &SkillDef = pSpell->getSkillDef();
			if(SkillDef.PrepareType == PREPARE_TYPE_SING)
			{
				sc->onPrepare(pSpell);
			}
			else if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
			{
				sc->onCollect(pSpell);
			}
		}
	}

	if( SKILL_BRK_SVR == msgid )
	{
		const CSSKILLBRKS& Msg = msgbody.SkillBrkS;
		int skillId = 0;
		ISpellControler *sc = getSpellControler(Msg.ID);
		if (sc)
		{
			unsigned int LeftCdTime = 0;
			if (Msg.LeftCdTime > 0)
			{
				LeftCdTime = ((unsigned int)Msg.LeftCdTime);
			}
			GameSpell *pSpell = sc->getSpell(0, 0, Msg.SkillCltSeq);
			if (pSpell != NULL)
			{
				skillId = pSpell->getSpellId();
			}
			sc->onBreak(Msg.SkillSeq, Msg.SkillCltSeq, LeftCdTime);
		}
		if(ActorManager::getSingleton().FindActor(Msg.ID) == ActorManager::getSingleton().getMainPlayer())
		{
			if (skillId != 0)
			{
				ActorManager::getSingleton().getMainPlayer()->ListenSpellPackage(SkillManager::getSingleton().getRuneSkillId(skillId),
					false, 0, 0);
			}
			ServerTimer::getSingleton().updateServerBenchTime(Msg.BenchTimeMs);
		}
	}

	if (SKILL_INFO == msgid)
	{
		const CSSKILLINFO&		Msg			= msgbody.SkillInfo;
		const CSSKILLINFODATA&	skillinfo	= Msg.Data;
		if (Msg.Type == SKILL_INFO_DEL)
		{
			ActorManager::getSingleton().getScriptVM()->callString("OnSkillInfoDel()");
			delSkillDyAttrib(skillinfo.SkillDelID); 
		}
		else if (Msg.Type == SKILL_INFO_CHANGE)
		{
			const CSSKILLLIST& skilllist	= skillinfo.SkillChg;
			// skill list
			for (int i = 0; i < skilllist.SkillNum; i++)
			{
				tagSkillDyAttrib* pAttrib = new tagSkillDyAttrib;
				pAttrib->SkillLevel			= skilllist.Skills[i].SkillLevel;
				pAttrib->CollectNum			= skilllist.Skills[i].CollectNum;		
				pAttrib->CollectNumMax		= skilllist.Skills[i].CollectNumMax;
				pAttrib->RuneSlotEnabled	= skilllist.Skills[i].RuneSlotEnabled;
				pAttrib->RuneSlot			= skilllist.Skills[i].RuneSlot;
				pAttrib->RuneItemID			= skilllist.Skills[i].RuneItemID;
				pAttrib->RuneItemIDs.resize(skilllist.Skills[i].RuneNum);
				for (int n = 0; n < skilllist.Skills[i].RuneNum; ++n)
				{
					pAttrib->RuneItemIDs[n] = skilllist.Skills[i].RuneInfo[n].RuneItemID;
				}
				// todo:向服务器确认学习新技能的时候发的消息
				char buf[256];
				sprintf( buf, "SkillLearn( %d, %d )", skilllist.Skills[i].SkillID, skilllist.Skills[i].SkillLevel );
				ActorManager::getSingleton().getScriptVM()->callString( buf );

				bool bLearned = SkillManager::getSingleton().isSkillLearned(skilllist.Skills[i].SkillID);
				setSkillDyAttrib(skilllist.Skills[i].SkillID, pAttrib);
				setSkillCDTime(skilllist.Skills[i].SkillID, skilllist.Skills[i].CoolLeft * 100);
				if( !bLearned )
				{
					char buff[256];
					sprintf( buff,"setShortCutNewSkillEffect(%d,%d)",skilllist.Skills[i].SkillID,skilllist.Skills[i].SkillLevel);
					ActorManager::getSingleton().getScriptVM()->callString(buff);
				}
			}
		}
		else if (Msg.Type == SKILL_COOL_CLEAN)
		{
			if (skillinfo.SkillCool.SkillType == SKILL_COOL_ALL)
			{
				clearAllCD();
			}
			else if (skillinfo.SkillCool.SkillType == SKILL_COOL_ONE)
			{
				clearCD(skillinfo.SkillCool.SkillID);
			}
			else if (skillinfo.SkillCool.SkillType == SKILL_COOL_CAREER)
			{
				for (std::map<int, tagCDTime>::iterator i = m_SkillCD.begin();
					i != m_SkillCD.end(); ++i)
				{
					int id = i->first;
					if (id <= 5000 || (id >= 7000 && id <= 7003))
					{
						i->second.startTime = i->second.LastTime = 0;
						if (m_SpellUIControl != NULL)
						{
							const tagCDTime &cd = getCDTime(id);
							m_SpellUIControl->setCoolTime(id,
								cd.past(), cd.total());
						}
					}
				}
			}
		}
		// update UI
		GameMsgPrint::getSingleton().SendUIEvent( "GE_SKILL_LIST_UPDATE" );
	}
	return 0;
}

void SkillManager::initSkill()
{
	clearIntentSpell();
	SkillGenera::init();
	SkillAttackP2P::init();
	SkillSelectAngle::init();
	SkillSelectAttack::init();
	SkillSelectArrow::init();
	SkillScan::init();
	SkillTrig::init();
	SkillVigor::init();
	SkillLine::init();
	SkillRandom::init();
	//SkillFly::init();
}

GameSpellControler* SkillManager::getSpellControler() 
{
	return &(GameSpellControler::getSingleton());
}

ISpellControler* SkillManager::getSpellControler(int id)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (pActor == NULL)
	{
		assert(false);
		return NULL;
	}
	if (pActor == ActorManager::getSingleton().getMainPlayer())
	{
		return getSpellControler();
	}
	std::map<int, ISpellControler *>::iterator i = m_ActorSpellControl.find(id);
	if (i == m_ActorSpellControl.end())
	{
		m_ActorSpellControl[id] = new ActorSpellControler(id);
		i = m_ActorSpellControl.find(id);
	}
	return i->second;
}

int SkillManager::getSpellCollectNumUse( int skillid, int lv )
{
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef( skillid, lv );

	return SkillDef.CollectNumUse;
}

int SkillManager::getSpellCollectNumCur( int id )
{
	tagSkillDyAttrib* pDyAttrib = getSkillDyAttrib( id );
	if (pDyAttrib == NULL)
	{
		return 0;
	}
	return pDyAttrib->CollectNum;
}

tagSkillAttrib* SkillManager::getSpellDes( int skillid )
{
	return m_SkillDes[skillid];
}

const tagSkillDef_ToLua* SkillManager::getSpellStaticAtt( int skillid, int lv )
{
	static_assert( sizeof(tagSkillDef) == sizeof(tagSkillDef_ToLua) );
	return ((const tagSkillDef_ToLua *)&DBDataManager::getSingleton().GetSkillDef( skillid, lv ));
}

tagDiathesisDef_ToLua* SkillManager::getDiathesisDef( int id, int lv )
{
	static_assert( sizeof(tagDiathesisDef_ToLua) == sizeof(tagDiathesisDef) );

	const tagDiathesisDef* pDiathesisDef  = &DBDataManager::getSingleton().getDiathesisDef( id, lv );
	if( pDiathesisDef == NULL )
	{
		return NULL;
	}

	return (tagDiathesisDef_ToLua*)pDiathesisDef;
}

void SkillManager::setTmpSkill( int index, int skillId, int skillLv )
{
	m_nTmpSkillId[index].m_nSkillId = skillId;
	m_nTmpSkillId[index].m_nLv		= skillLv;
}

int SkillManager::getTmpSkillId( int index )
{
	return m_nTmpSkillId[index].m_nSkillId;
}

int SkillManager::getTmpSkillLv( int index )
{
	return m_nTmpSkillId[index].m_nLv;
}

// 确认法术是否足够
int SkillManager::checkMagic( int skillid )
{
	MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
	if( !pActor->getBufStatus()->canUsedSpell( skillid ) || !pActor->canControl() )
	{
		//TODO::提示炫晕等状态不可使用技能
		return MID_SHORTCUT_CANNOTUSESPELL_BUF;
	}

	if (pActor->getActState().GetState() == GSACT_DEAD)
	{
		return MID_SELF_DEAD;
	}

	if(getSkillDyAttrib( skillid ) == NULL)
	{
		return MID_SHORTCUT_ENERGENOTENOUGH;
	}
	int id = getRuneSkillId(skillid);
	if (!castMP(id))
	{
		return MID_SHORTCUT_MPNOTENOUGH;
	}
	if (!castXP(id))
	{
		return MID_XP_NOENOUGHXP;
	}
	if (!castCollect(id))
	{
		return MID_XP_NOENOUGHCOLLECT;
	}
	if( !castItem(id) )
	{
		return MID_NOTENOUGH_COST_ITEM;
	}

	return 0;
}

// 是否是无双技能
bool SkillManager::isXPSkill( int skillid )
{
	if( getSkillDyAttrib( skillid ) == NULL )
	{
		return false;
	}
	int nLv = getSpellLv( skillid );
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef( skillid, nLv );
	if( SkillDef.WushuangFlag == 0 )
	{
		return false;
	}

	return true;
}

// 获得当前职业学习等级的技能ID
int SkillManager::getLevelSkillId( int nRace, int nLv, int nIndex )
{
	int nSkillId = 0;
	const tagLevelUP LevelUpDef = DBDataManager::getSingleton().getLevelUP( nLv );
	switch( nRace )
	{
	case CAREER_WARRIOR:
		nSkillId = LevelUpDef.WarriorSkillList[nIndex];
		break;
	case CAREER_MAGICOR:
		nSkillId = LevelUpDef.MagicorSkillList[nIndex];
		break;
	case CAREER_GUNER:
		nSkillId = LevelUpDef.GunerSkillList[nIndex];
	    break;
	case CAREER_CHURCH:
		nSkillId = LevelUpDef.ChurchSkillList[nIndex];
	    break;
	}
	return nSkillId;
}

void SkillManager::update(unsigned int dtick)
{
	unsigned int now = timeGetTime();
	while (!attackArray.empty())
	{
		if (attackArray.front().first > now)
		{
			break;
		}
		GameSpellPtr p = attackArray.front().second;
		std::pop_heap(attackArray.begin(), attackArray.end(), PairFirstGreater());
		attackArray.pop_back();
		p->Hit();
	}
	while (!breakArray.empty())
	{
		if (breakArray.front().first > now)
		{
			break;
		}
		GameSpellPtr p = breakArray.front().second;
		std::pop_heap(breakArray.begin(), breakArray.end(), PairFirstGreater());
		breakArray.pop_back();
		p->Break();
	}
	execSpellStop();
	for (std::set<SkillHit *>::iterator i = hitArray.begin(); i != hitArray.end();)
	{
		if ((*i)->actived())
		{
			(*i)->update(dtick);
			++i;
		}
		else
		{
			i = hitArray.erase(i);
		}
	}
	if (!m_pIntentSpell && !getSpellControler()->getCurSpell())
	{
		MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if (pMainPlayer)
		{
			if (false && m_lockSkill)
			{
				useSpell(m_lockSkill);
			}
			else if (m_pNextSpell)
			{
				const tagSkillDef &SkillDef = m_pNextSpell->getSkillDef();
				if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
				{
					setNextSpell();
				}
				else if (!isSkillLearned(m_pNextSpell->getSpellId()))
				{
					const GameXYZ *ppos = m_pNextSpell->getTargetPoint();
					GameXYZ pos;
					if (ppos)
					{
						pos = *ppos;
					}
					else
					{
						pos = pMainPlayer->GetPosition();
					}
					if (pos.x == 0 && pos.z == 0)
					{
						setNextSpell(m_pNextSpellId, m_pNextSpell->getSingleTarget());
					}
					else
					{
						setNextSpell(m_pNextSpellId, m_pNextSpell->getSingleTarget(), pos.x, pos.z);
					}
				}
				else
				{
					if (m_nextTime <= now)
					{
						m_nextTime = now + 200;
						if ((pMainPlayer->getActState().GetState() == GSACT_STAND || pMainPlayer->getActState().GetState() == GSACT_MOVE) 
								&& !pMainPlayer->isInPre(eUseItem_Chant) && !pMainPlayer->isInPre(ePet_Chant))
						{
							int targetId = m_pNextSpell->getSingleTarget();
							if (targetId == 0 || targetId == pMainPlayer->GetSelectTarget())
							{
								int result = m_pNextSpell->check();
								if (result == GameSpell::RET_SUCCESS)
								{
									GameSpellPtr spell = m_pNextSpell;
									m_pNextSpell = NULL;
									onStart(spell);
								}
								else if (result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT)
								{
									const GameXYZ *pos = m_pNextSpell->getTargetPoint();
									if (pos == NULL || MOVE_SUCCESS != pMainPlayer->CmdMoveTo(pos->x / 10, pos->z / 10, 0))
									{
										setNextSpell();
									}
								}
								else if (result != GameSpell::RET_CD && result != GameSpell::RET_TETANIC)
								{
									setNextSpell();
								}
							}
							else
							{
								setNextSpell();
							}
						}
					}
				}
			}
			else if (m_isInAttack != -1)
			{
				if (pMainPlayer->GetSelectTarget() == 0)
				{
					m_isInAttack = -1;
				}
				else
				{
					if (m_isInAttack == 0 || m_isInAttack == pMainPlayer->GetSelectTarget())
					{
						GameActor *target = ActorManager::getSingleton().FindActor(pMainPlayer->GetSelectTarget());
						if (target && !target->isDead())
						{
							if (pMainPlayer->getActState().GetState() == GSACT_STAND)
							{
								normalAttack(getNormalAttackSkill());
							}
						}
						else
						{
							m_isInAttack = -1;
						}
					}
					else
					{
						m_isInAttack = -1;
					}
				}
			}
		}
	}
	getSpellControler()->update(dtick);
	for (std::map<int, ISpellControler *>::iterator i = m_ActorSpellControl.begin(); i != m_ActorSpellControl.end(); ++i)
	{
		i->second->update(dtick);
	}
}

void SkillManager::deleteActorSpell( int id )
{
	std::map<int, ISpellControler *>::iterator i = m_ActorSpellControl.find(id);
	if (i != m_ActorSpellControl.end())
	{
		delete i->second;
		m_ActorSpellControl.erase(i);
	}
}

void SkillManager::clearSkill()
{
	SkillGenera::destory();
	SkillAttackP2P::destory();
	SkillSelectAngle::destory();
	SkillSelectAttack::destory();
	SkillSelectArrow::destory();
	SkillScan::destory();
	SkillTrig::destory();
	SkillVigor::destory();
	SkillLine::destory();
	SkillRandom::destory();

	m_SkillDes.clear();
}

void SkillManager::reloadSkill()
{
	DBDataManager::getSingleton().reloadDB();
	clearSkill();
	initSkill();
}

GameSpell * SkillManager::createSpell(int id, ISpellControler *sc)
{
	std::map<int, FUN>::iterator iter = m_SkillCreateFun.find(id);
	if(iter == m_SkillCreateFun.end())
	{
		char str[256];
		sprintf(str, "缺少对应的技能模板:%d", id);
		::MessageBox(NULL, str, "错误", MB_OK);
		return NULL;
	}
	GameSpell *pSpell = m_SkillCreateFun[id]();
	pSpell->setSpellId(id);
	pSpell->setOwner(sc->getOwnerId());
	pSpell->setSpellController(sc);
	return pSpell;
}

GameSpell * SkillManager::getSpellSample(int id)
{
	std::map<int, GameSpellPtr>::iterator i = m_SpellSample.find(id);
	if (i == m_SpellSample.end())
	{
		m_SpellSample[id] = createSpell(id, getSpellControler());
		i = m_SpellSample.find(id);
	}
	i->second->setSpellLv(getSpellLv(id));
	return i->second;
}

int SkillManager::castInspect(int id)
{
	GameSpell *pSpell = getSpellSample(id);
	pSpell->setSpellLv(getSpellLv(id));
	int result = pSpell->inspect();
	if (result == GameSpell::RET_SUCCESS)
	{
		return pSpell->inspect(ActorManager::getSingleton().getMainPlayer()->GetSelectTarget());
	}
	return result;
}

bool SkillManager::castInMap(int id)
{
	return castInMap(id, getSpellLv(id));
}

bool SkillManager::castInMap(int id, int lv)
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return false;
	}
	GameMap *map = player->getCurMap();
	if (map == NULL)
	{
		return false;
	}
	const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(map->getRealId());
	if ((mapdef.CtrlFlag & MAP_CTRL_NO_SKILL) && !isTmpSkill(id))
	{
		return false;
	}
	return true;
}

bool SkillManager::castInPworld(int id)
{
	return castInPworld(id, getSpellLv(id));
}

bool SkillManager::castInPworld(int id, int lv)
{
	int pworldId = PworldLimitInfo::getSingleton().getCurPworldId();
	if (pworldId != 0)
	{
		const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
		if ((pworldDef->CtrlFlag & PWORLD_NO_SKILL) && !isTmpSkill(id))
		{
			return false;
		}
		const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
		if ((pworldDef->CtrlFlag & PWORLD_NO_WUSHUANG_SKILL) && SkillDef.WushuangFlag)
		{
			return false;
		}
	}
	return true;
}

bool SkillManager::castWeapon(int id)
{
	return castWeapon(id, getSpellLv(id));
}

bool SkillManager::castWeapon(int id, int lv)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if( SkillDef.ArmType[0].ArmType == ARM_ANY )
	{
		return true;
	}

	if (SkillDef.ArmType[0].ArmType == ARM_ANY_EX)
	{
		return ITEM_SLOTTYPE_NONE != pMainPlayer->m_Equip.getArmType( EQUIP_POS_RIGHTHAND );
	}
	else
	{
		for( int i = 0; i < MAX_SKILL_ARMTYPE; i++ )
		{
			if( SkillDef.ArmType[i].ArmType == 0 )
			{
				continue;
			}

			if( pMainPlayer->m_Equip.getArmType( EQUIP_POS_RIGHTHAND ) == SkillDef.ArmType[i].ArmType )
			{
				// 弹药检测
				int AmmType = pMainPlayer->m_Equip.getItem( EQUIP_POS_RIGHTHAND ).needAmm();
				if( AmmType != UNUSED_AMM && SkillDef.AmmUse > pMainPlayer->getContainer().getAmmCount( AmmType )  )
				{
					return false;
				}

				return true;
			}

			if( pMainPlayer->m_Equip.getArmType( EQUIP_POS_LEFT ) == SkillDef.ArmType[i].ArmType )
			{
				return true;
			}

			if( pMainPlayer->m_Equip.getArmType( EQUIP_POS_SHOULDER ) == SkillDef.ArmType[i].ArmType )
			{
				return true;
			}

			if( pMainPlayer->m_Equip.getArmType( EQUIP_POS_BACK ) == SkillDef.ArmType[i].ArmType )
			{
				return true;
			}
		}
	}
	return false;
}

bool SkillManager::castMP(int id)
{
	return castMP(id, getSpellLv(id));
}

bool SkillManager::castMP(int id, int lv)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer->getBufStatus()->checkBuf(STATUS_ID_NEXT_SKILL_NO_MP))
	{
		return true;
	}
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	return pMainPlayer->getMP() >= getSkillCostMp(id, lv);
}

bool SkillManager::castXP(int id)
{
	return castXP(id, getSpellLv(id));
}

bool SkillManager::castXP(int id, int lv)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	return pMainPlayer->getXP() >= getSkillCostXp(id, lv);
}

bool SkillManager::castCollect(int id)
{
	return castCollect(id, getSpellLv(id));
}

bool SkillManager::castCollect(int id, int lv)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	return pMainPlayer->getCollectSlotCurr() >= getSkillCostCollect(id, lv);
}

bool SkillManager::castEnergy(int id)
{
	return castEnergy(id, getSpellLv(id));
}

bool SkillManager::castEnergy(int id, int lv)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	return SkillDef.TmpEnergeUse == 0 || pMainPlayer->getEnerge() >= SkillDef.TmpEnergeUse;
}

bool SkillManager::castMachineEnergy(int id)
{
	return castMachineEnergy(id, getSpellLv(id));
}

bool SkillManager::castMachineEnergy(int id, int lv)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if (SkillDef.EnergyUse == 0)
	{
		return true;
	}
	if (!pMainPlayer->isOnMonsterMachine())
	{
		return false;
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(pMainPlayer->m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
	if (pActor == NULL)
	{
		return false;
	}
	return pActor->m_MonsterMachine.getEnergy() >= SkillDef.EnergyUse;
}

bool SkillManager::castMorph(int id)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer->m_bChangeSkillShortCut && !isTmpSkill(id))
	{
		return false;
	}
	return true;
}

bool SkillManager::castSkillCD(int id)
{
	return getSkillCDTime(getReserveSkillId(id)).remain() == 0;
}

bool SkillManager::castCommonCD(int id)
{
	return getCommonCDTime().remain() == 0;
}

bool SkillManager::castCD(int id)
{
	return castCommonCD(id) && castSkillCD(id);
}

const tagCDTime & SkillManager::getCDTime(int id)
{
	const static tagCDTime nonCD;
	if (isSkillPassive(id))
	{
		return m_SkillCD[id];
	}
	std::map<int, tagCDTime>::iterator iter = m_SkillCD.find(id);
	const tagSkillDef & SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	if( SkillDef.CommCool > 0 )
	{
		if( iter != m_SkillCD.end() )
		{
			if( iter->second.remain() >= m_PublicCD.remain() )
			{
				return iter->second;
			}
		}
		return m_PublicCD;
	}else{
		if( iter != m_SkillCD.end() )
		{
			return iter->second;
		}
		return nonCD;
	}
}

const tagCDTime & SkillManager::getSkillCDTime(int id)
{
	return m_SkillCD[id];
}

const tagCDTime & SkillManager::getCommonCDTime()
{
	return m_PublicCD;
}

void SkillManager::setCommonCDTime(int id)
{
	unsigned int now = timeGetTime();
	int nCommCool = getSkillCommCoolTime(id);
	if(nCommCool > 0)
	{
		m_PublicCD.startTime	= now;
		m_PublicCD.LastTime		= now + nCommCool * 100;
	}

	if (m_SpellUIControl != NULL)
	{
		for (std::map<int, tagSkillDyAttrib*>::iterator iter = m_SkillDyAttrib.begin();
			iter != m_SkillDyAttrib.end(); ++iter)
		{
			const tagCDTime &cd = getCDTime(iter->first);
			if (cd.remain() == getCommonCDTime().remain())
			{
				m_SpellUIControl->setCoolTime(iter->first, cd.past(), 
					cd.total());
			}
		}
	}
}

void SkillManager::resetSkillCDTime(int id)
{
	id = getReserveSkillId(id);
	clearCD(id);

	const tagSkillDef & SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	checkCD_resetTypeCD( SkillDef.SpeCoolType1, SkillDef.SpeCoolVal1 * 100);
	checkCD_resetTypeCD( SkillDef.SpeCoolType2, SkillDef.SpeCoolVal2 * 100);
	checkCD_resetTypeCD( SkillDef.SpeCoolType3, SkillDef.SpeCoolVal3 * 100);
	checkCD_resetTypeCD( SkillDef.SpeCoolType4, SkillDef.SpeCoolVal4 * 100);
}

void SkillManager::setSkillCDTime(int id)
{
	unsigned int now = timeGetTime();
	setSkillCDTime(id, getSkillSelfCoolTime(id) * 100);

	const tagSkillDef & SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	checkCD_setTypeCD( SkillDef.SpeCoolType1, SkillDef.SpeCoolVal1 * 100, now);
	checkCD_setTypeCD( SkillDef.SpeCoolType2, SkillDef.SpeCoolVal2 * 100, now);
	checkCD_setTypeCD( SkillDef.SpeCoolType3, SkillDef.SpeCoolVal3 * 100, now);
	checkCD_setTypeCD( SkillDef.SpeCoolType4, SkillDef.SpeCoolVal4 * 100, now);
}

void SkillManager::setSkillCDTime(int id, unsigned int left)
{
	unsigned int now = timeGetTime();
	unsigned int nSelfCool = getSkillSelfCoolTime(id);
	int nId = getReserveSkillId(id);
	if (left >= nSelfCool * 100)
	{
		m_SkillCD[nId].startTime = now;
		m_SkillCD[nId].LastTime = now + left;
	}
	else if (nSelfCool > 0)
	{
		m_SkillCD[nId].startTime = now + left - nSelfCool * 100;
		m_SkillCD[nId].LastTime = now + left;
	}

	if (m_SpellUIControl != NULL)
	{
		const tagCDTime &cd = getCDTime(nId);
		m_SpellUIControl->setCoolTime(nId,
			cd.past(), cd.total());
	}
}

unsigned int SkillManager::getSkillCommCoolTime(int id)
{
	return DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id)).CommCool;
}

unsigned int SkillManager::getSkillSelfCoolTime(int id)
{
	const tagSkillDef &skill = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	unsigned int selfcool = skill.SelfCool;
	if (skill.StatusChg.StatusID != 0 && skill.StatusChg.Cool != 0)
	{
		MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if (pMainPlayer)
		{
			if (pMainPlayer->getBufStatus()->checkBuf(skill.StatusChg.StatusID))
			{
				selfcool = skill.StatusChg.Cool;
			}
		}
	}
	return selfcool;
}

unsigned int SkillManager::getSkillPrepareTime(int id)
{
	return getSkillPrepareTime(id, getSpellLv(id));
}

unsigned int SkillManager::getSkillPrepareTime(int id, int lv)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if (SkillDef.PrepareType != PREPARE_TYPE_SING)
	{
		return 0;
	}
	return SkillDef.PrepareTime;
}

void SkillManager::checkCD_resetTypeCD(int t, unsigned int v)
{
	if(t == 0 || v == 0)
	{
		return;
	}
	for( std::map<int, tagSkillDyAttrib*>::iterator i = m_SkillDyAttrib.begin(); i != m_SkillDyAttrib.end(); ++i )
	{
		const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(i->first, i->second->SkillLevel);
		if(SkillDef.SelfSpeCoolType == t)
		{
			std::map<int, tagCDTime>::iterator iter = m_SkillCD.find(i->first);
			if(iter != m_SkillCD.end() && iter->second.total() == v)
			{
				clearCD(i->first);
			}
		}
	}
}

void SkillManager::checkCD_setTypeCD(int t, unsigned int v, unsigned int time)
{
	if(t == 0 || v == 0)
	{
		return;
	}
	for( std::map<int, tagSkillDyAttrib*>::iterator i = m_SkillDyAttrib.begin(); i != m_SkillDyAttrib.end(); ++i )
	{
		const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(i->first, i->second->SkillLevel);
		if(SkillDef.SelfSpeCoolType == t)
		{
			std::map<int, tagCDTime>::iterator iter = m_SkillCD.find(i->first);
			if(iter != m_SkillCD.end() && iter->second.remain() >= v)
			{
				continue;
			}
			m_SkillCD[i->first].startTime = time;
			m_SkillCD[i->first].LastTime = time + v;

			if (m_SpellUIControl != NULL)
			{
				const tagCDTime &cd = getCDTime(i->first);
				m_SpellUIControl->setCoolTime(i->first,
					cd.past(), cd.total());
			}
		}
	}
}

unsigned int SkillManager::getSkillCollectTime(int id)
{
	return getSkillCollectTime(id, getSpellLv(id));
}

unsigned int SkillManager::getSkillCollectTime(int id, int lv)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if (SkillDef.PrepareType != PREPARE_TYPE_COLLECT)
	{
		return 0;
	}
	return (unsigned int)ceil(SkillDef.JiqiTime / (float)getSkillCollectNum(id, lv));
}

int SkillManager::getSkillCollectNum(int id)
{
	return getSkillCollectNum(id, getSpellLv(id));
}

int SkillManager::getSkillCollectNum(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, lv).JiqiChgFlag == 1 ? 3 : 1;
}

const RESRANGE & SkillManager::getSkillMainRange(int id)
{
	return DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id)).MainRange;
}

unsigned int SkillManager::getSkillMaxTargets(int id)
{
	return DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id)).TargetNum;
}

unsigned int SkillManager::getSkillPilotTime(int id)
{
	return getSkillPilotTime(id, getSpellLv(id));
}

unsigned int SkillManager::getSkillPilotTime(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, lv).PilotTime;
}

int SkillManager::getSkillPilotNum(int id)
{
	return getSkillPilotNum(id, getSpellLv(id));
}

int SkillManager::getSkillPilotNum(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, lv).PilotNum;
}

int SkillManager::getSkillCostMp(int id)
{
	return getSkillCostMp(id, getSpellLv(id));
}

int SkillManager::getSkillCostMp(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, lv).MPUse;
}

int SkillManager::getSkillCostCollect(int id)
{
	return getSkillCostCollect(id, getSpellLv(id));
}

int SkillManager::getSkillCostCollect(int id, int lv)
{
	const tagSkillDef &skill = DBDataManager::getSingleton().GetSkillDef(id, lv);
	int collect = skill.CollectSkillUse;
	if (skill.StatusChg.StatusID != 0 && skill.StatusChg.CollectNum != 0)
	{
		MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if (pMainPlayer)
		{
			if (pMainPlayer->getBufStatus()->checkBuf(skill.StatusChg.StatusID))
			{
				collect = skill.StatusChg.CollectNum;
			}
		}
	}
	return collect;
}

int SkillManager::getSkillCostXp(int id)
{
	return getSkillCostXp(id, getSpellLv(id));
}

int SkillManager::getSkillCostXp(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, lv).UseXp;
}

ISpellUIControl* SkillManager::getSpellUIControl()
{
	return m_SpellUIControl;
}

void SkillManager::setSpellUIControl(ISpellUIControl* p)
{
	m_SpellUIControl = p; 
}

int SkillManager::getNextLearnedSkill(int id)
{
	std::map<int, tagSkillDyAttrib*>::iterator iter;
	if (id == 0)
	{
		iter = m_SkillDyAttrib.begin();
	}
	else
	{
		iter = m_SkillDyAttrib.find(id);
		if (iter != m_SkillDyAttrib.end())
		{
			++iter;
		}
	}

	if (iter == m_SkillDyAttrib.end())
	{
		return 0;
	}

	return iter->first;
}

void SkillManager::sendBreakMsg(GameSpell *pSpell)
{
	tagCSPkgBody msgbody;
	msgbody.SkillBrk.SkillID = pSpell->getSpellId();
	SendGameServerMsg(SKILL_BRK, msgbody);
	m_skillReq += 100;
}

void SkillManager::sendPrepareMsg(GameSpell *pSpell)
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagCSPkgBody msgbody;
	msgbody.SkillPre.SkillID = getReserveSkillId(pSpell->getSpellId());
	if( pMainPlayer != NULL )
	{
		pMainPlayer->trySendCacheMove(true);
		msgbody.SkillPre.x = pMainPlayer->GetPosition().x/10;
		msgbody.SkillPre.y = pMainPlayer->GetPosition().z/10;
	}
	msgbody.SkillPre.AtkPos.X = pSpell->getTargetPoint()->x / 10;
	msgbody.SkillPre.AtkPos.Y = pSpell->getTargetPoint()->z / 10;
	msgbody.SkillPre.SkillSeq		= ++m_skillReq;
	ServerTimer::getSingleton().getServerTime(msgbody.SkillPre.SvrTime.BenchTimeSec, msgbody.SkillPre.SvrTime.BenchTimeUSec);
	SendGameServerMsg(SKILL_PREPARE, msgbody);

	getSpellControler()->addSpell(pSpell, m_skillReq);
}

void SkillManager::sendAttackMsg(GameSpell *pSpell, int BulletId /*= 0*/)
{
	const tagSkillDef &SkillDef = pSpell->getSkillDef();
	if (SkillDef.PrepareType == PREPARE_TYPE_NONE && pSpell->getPilotNum() == 1)
	{
		m_skillReq++;
	}
	sendAttackMsg(pSpell, m_skillReq, BulletId);
}

void SkillManager::sendAttackMsg(GameSpell *pSpell, int skillReq, int BulletId)
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameMsgPrint::getSingleton().SendUIEvent( "GE_BEGIN_ATTACK_OTHER" );
	tagCSPkgBody msgbody;
	msgbody.SkillAtk.SkillID = getReserveSkillId(pSpell->getSpellId());
	if(pMainPlayer != NULL)
	{
		pMainPlayer->trySendCacheMove(true);
		msgbody.SkillAtk.RolePos.X = pSpell->getSelfPoint().x / 10;
		msgbody.SkillAtk.RolePos.Y = pSpell->getSelfPoint().z / 10;
	}

	msgbody.SkillAtk.TargetNum = (unsigned char)(pSpell->getTargetNum());
	for(size_t i = 0; i < pSpell->getTargetNum(); i++)
	{
		msgbody.SkillAtk.TargetIDs[i] = pSpell->getTarget(i);
	}
	msgbody.SkillAtk.AtkPos.X = pSpell->getTargetPoint()->x / 10;
	msgbody.SkillAtk.AtkPos.Y = pSpell->getTargetPoint()->z / 10;
	msgbody.SkillAtk.SkillSeq = skillReq;
	msgbody.SkillAtk.CollectPowerNum = pSpell->getCollectNum();
	msgbody.SkillAtk.MonID = BulletId;

	ServerTimer::getSingleton().getServerTime(msgbody.SkillAtk.SvrTime.BenchTimeSec, msgbody.SkillAtk.SvrTime.BenchTimeUSec);
	SendGameServerMsg(SKILL_ATK, msgbody);

	getSpellControler()->addSpell(pSpell, skillReq);
}

void SkillManager::sendHitMsg(GameSpell *pSpell, int BulletId /*= 0*/)
{
	const tagSkillDef &SkillDef = pSpell->getSkillDef();
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameMsgPrint::getSingleton().SendUIEvent( "GE_BEGIN_ATTACK_OTHER" );
	tagCSPkgBody msgbody;
	msgbody.SkillHit.SkillID = getReserveSkillId(pSpell->getSpellId());
	if(pMainPlayer != NULL)
	{
		msgbody.SkillHit.RolePos.X = pSpell->getSelfPoint().x / 10;
		msgbody.SkillHit.RolePos.Y = pSpell->getSelfPoint().z / 10;
	}

	msgbody.SkillHit.TargetNum = (unsigned char)(pSpell->getTargetNum());
	for(size_t i = 0; i < pSpell->getTargetNum(); i++)
	{
		msgbody.SkillHit.TargetIDs[i] = pSpell->getTarget(i);
	}
	msgbody.SkillHit.AtkPos.X = pSpell->getTargetPoint()->x / 10;
	msgbody.SkillHit.AtkPos.Y = pSpell->getTargetPoint()->z / 10;
	msgbody.SkillHit.SkillSeq = pSpell->getSpellReq();
	msgbody.SkillHit.CollectPowerNum = pSpell->getCollectNum();
	msgbody.SkillHit.MonID = BulletId;

	ServerTimer::getSingleton().getServerTime(msgbody.SkillHit.SvrTime.BenchTimeSec, msgbody.SkillHit.SvrTime.BenchTimeUSec);
	SendGameServerMsg(SKILL_HIT, msgbody);
}

GameSpell * SkillManager::getIntentSpell()
{
	return m_pIntentSpell;
}

GameSpell* SkillManager::getReadySpell()
{
	return m_pReadySpell;
}

void SkillManager::setReadySpell(GameSpell *pSpell/* = NULL*/)
{
	m_pReadySpell = pSpell;
}

GameSpell* SkillManager::getAngleSpell()
{
	return m_pAngleSpell;
}

void SkillManager::setAngleSpell(GameSpell *pSpell/* = NULL*/)
{
	m_pAngleSpell = pSpell;
}

GameSpell* SkillManager::getTurnSpell()
{
	return m_pTurnSpell;
}

void SkillManager::setTurnSpell(GameSpell *pSpell /*= NULL*/)
{
	m_pTurnSpell = pSpell;
}


int SkillManager::useSpell(int id)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return GameSpell::RET_UNKNOWN;
	}
	pMainPlayer->RefreshCmdTime();
	if (!isSkillLearned(id))
	{
		return GameSpell::RET_UNKNOWN;
	}
	clearIntentSpell();
	m_lockSkill = id;
	id = getInsteadSkillId(id);
	int nId = getRuneSkillId(id);
	int result = castInspect(nId);
	if (result == GameSpell::RET_SUCCESS || result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT)
	{
		GameSpellPtr pSpell = SkillManager::getSingleton().createSpell(nId, getSpellControler());
		pSpell->setSpellLv(getSpellLv(id));
		if (((GetKeyState(VK_LMENU) & 0xff00) || (GetKeyState(VK_RMENU) & 0xff00)) && canCastSelf(nId))
		{
			pSpell->setSingleTarget(pMainPlayer->GetID());
		}
		else
		{
			pSpell->setSingleTarget(pMainPlayer->GetSelectTarget());
		}
		if (!onStart(pSpell))
		{
			return GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_CD)
	{
		if (getSkillCDTime(id).remain() < AUTO_NEXT_CD)
		{
			setNextSpell(id);
			result = GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_TETANIC)
	{
		setNextSpell(id);
	}
	else if (result == GameSpell::RET_MPSHORT || result == GameSpell::RET_XPSHORT ||
		result == GameSpell::RET_COLLECTSHORT || result == GameSpell::RET_ENERGYSHORT ||
		result == GameSpell::RET_MACHINEENERGYSHORT)
	{
		unUseSpell();
	}
	return result;
}

int SkillManager::useSpell(int id, int target)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return GameSpell::RET_UNKNOWN;
	}
	pMainPlayer->RefreshCmdTime();
	if (!isSkillLearned(id))
	{
		return GameSpell::RET_UNKNOWN;
	}
	clearIntentSpell();
	m_lockSkill = id;
	id = getInsteadSkillId(id);
	int nId = getRuneSkillId(id);
	GameSpellPtr pSpell = SkillManager::getSingleton().createSpell(nId, getSpellControler());
	pSpell->setSpellLv(getSpellLv(id));
	pSpell->setSingleTarget(target);
	int result = pSpell->check();
	if (result == GameSpell::RET_SUCCESS || result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT)
	{
		if (!onStart(pSpell))
		{
			return GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_CD)
	{
		if (getSkillCDTime(id).remain() < AUTO_NEXT_CD)
		{
			setNextSpell(id, target);
			result = GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_TETANIC)
	{
		setNextSpell(id, target);
	}
	else if (result == GameSpell::RET_MPSHORT || result == GameSpell::RET_XPSHORT ||
		result == GameSpell::RET_COLLECTSHORT || result == GameSpell::RET_ENERGYSHORT ||
		result == GameSpell::RET_MACHINEENERGYSHORT)
	{
		unUseSpell();
	}
	return result;
}

int SkillManager::useSpell(int id, int x, int y)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return GameSpell::RET_UNKNOWN;
	}
	pMainPlayer->RefreshCmdTime();
	if (!isSkillLearned(id))
	{
		return GameSpell::RET_UNKNOWN;
	}
	clearIntentSpell();
	m_lockSkill = id;
	id = getInsteadSkillId(id);
	int nId = getRuneSkillId(id);
	GameSpellPtr pSpell = SkillManager::getSingleton().createSpell(nId, getSpellControler());
	pSpell->setSpellLv(getSpellLv(id));
	pSpell->setTargetPoint(x, y);
	int result = pSpell->check();
	if (result == GameSpell::RET_SUCCESS || result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT)
	{
		if (!onStart(pSpell))
		{
			return GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_CD)
	{
		if (getSkillCDTime(id).remain() < AUTO_NEXT_CD)
		{
			setNextSpell(id, x, y);
			result = GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_TETANIC)
	{
		setNextSpell(id, x, y);
	}
	else if (result == GameSpell::RET_MPSHORT || result == GameSpell::RET_XPSHORT ||
		result == GameSpell::RET_COLLECTSHORT || result == GameSpell::RET_ENERGYSHORT ||
		result == GameSpell::RET_MACHINEENERGYSHORT)
	{
		unUseSpell();
	}
	return result;
}

int SkillManager::useSpell(int id, int target, int x, int y)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return GameSpell::RET_UNKNOWN;
	}
	pMainPlayer->RefreshCmdTime();
	if (!isSkillLearned(id))
	{
		return GameSpell::RET_UNKNOWN;
	}
	clearIntentSpell();
	m_lockSkill = id;
	id = getInsteadSkillId(id);
	int nId = getRuneSkillId(id);
	GameSpellPtr pSpell = SkillManager::getSingleton().createSpell(nId, getSpellControler());
	pSpell->setSpellLv(getSpellLv(id));
	pSpell->setSingleTarget(target);
	pSpell->setTargetPoint(x, y);
	int result = pSpell->check();
	if (result == GameSpell::RET_SUCCESS || result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT)
	{
		if (!onStart(pSpell))
		{
			return GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_CD)
	{
		if (getSkillCDTime(id).remain() < AUTO_NEXT_CD)
		{
			setNextSpell(id, target, x, y);
			result = GameSpell::RET_UNKNOWN;
		}
	}
	else if (result == GameSpell::RET_TETANIC)
	{
		setNextSpell(id, target, x, y);
	}
	else if (result == GameSpell::RET_MPSHORT || result == GameSpell::RET_XPSHORT ||
		result == GameSpell::RET_COLLECTSHORT || result == GameSpell::RET_ENERGYSHORT ||
		result == GameSpell::RET_MACHINEENERGYSHORT)
	{
		unUseSpell();
	}
	return result;
}

void SkillManager::unUseSpell()
{
	m_lockSkill = 0;
	GameSpell *pSpell = getSpellControler()->getCurSpell();
	if (pSpell && pSpell->getState() == GameSpell::STATE_COLLECT)
	{
		pSpell->endCollect();
	}
}

void SkillManager::clearIntentSpell()
{
	if (m_pIntentSpell)
	{
		m_pIntentSpell = NULL;
		GameMsgPrint::getSingleton().hideSelectEffect();
	}
}

bool SkillManager::onStart(GameSpell* pSpell)
{
	execSpellStop();
	if (!pSpell->start())
	{
		return false;
	}
	if (pSpell->getState() == GameSpell::STATE_START)
	{
		m_pIntentSpell = pSpell;
		//setNextSpell();
	}
	else if (pSpell->getState() == GameSpell::STATE_CATCH)
	{
		if (ActorManager::getSingleton().getMainPlayer()->m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE
			&& pSpell->getSpellId() == 3223) // tag 暂时措施
		{
			ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_NONE, 0, pSpell->getSpellId());
		}
		setNextSpell(pSpell);
	}
	return true;
}

void SkillManager::onStop()
{
	clearIntentSpell();
	GameSpell *pSpell = getSpellControler()->getCurSpell();
	if (pSpell && !canMoveAttack(pSpell->getSpellId()))
	{
		pSpell->Break();
	}
	setNextSpell();
	m_isInAttack = -1;
}

bool SkillManager::normalAttack(int id)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return false;
	}
	if (getSpellControler()->getCurSpell())
	{
		return false;
	}
	if (!isTmpSkill(id) && pMainPlayer->m_Rider.isOnHorse())
	{
		return false;
	}
	if (!isSkillLearned(id))
	{
		return false;
	}
	clearIntentSpell();
	id = getInsteadSkillId(id);
	int nId = getRuneSkillId(id);
	int result = castInspect(nId);
	if (result == GameSpell::RET_SUCCESS || result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT)
	{
		GameSpell *pSpell = SkillManager::getSingleton().createSpell(nId, getSpellControler());
		pSpell->setSpellLv(getSpellLv(id));
		pSpell->setSingleTarget(pMainPlayer->GetSelectTarget());
		return onStart(pSpell);
	}
	else if (result == GameSpell::RET_MASKTARGET)
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(pMainPlayer->GetSelectTarget());
		if (pActor)
		{
			GameXYZ pos = pActor->GetPosition();
			if (MOVE_SUCCESS == pMainPlayer->CmdMoveTo(pos.x / 10, pos.z / 10))
			{
				setNextSpell(id, pMainPlayer->GetSelectTarget());
				return true;
			}
		}
	}
	else if (result == GameSpell::RET_CD)
	{
		setNextSpell(id, pMainPlayer->GetSelectTarget());
		return true;
	}
	return false;
}

void SkillManager::setSkillDyAttrib(int id, tagSkillDyAttrib *attrib)
{
	std::map<int, tagSkillDyAttrib*>::iterator iter = m_SkillDyAttrib.find( id );
	if( iter != m_SkillDyAttrib.end() )
	{
		*(iter->second) = *attrib;
		delete attrib;
		attrib = iter->second;
		GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
		if (pActor)
		{
			pActor->onUpdateSkill(id);
		}
	}
	else
	{
		m_SkillDyAttrib[id] = attrib;
		GameMsgPrint::getSingleton().newSkill(id, attrib->SkillLevel);
	}
	if (attrib->RuneSlotEnabled)
	{
		int skillid = 0;
		const tagSkillDef &skillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
		switch (attrib->RuneSlot)
		{
		case RUNE_RED:
			skillid = skillDef.RedRuneTransferedID;
			break;
		case RUNE_BLUE:
			skillid = skillDef.BlueRuneTransferedID;
			break;
		case RUNE_YELLOW:
			skillid = skillDef.YellowRuneTransferedID;
			break;
		}
		if (skillid != 0)
		{
			m_RuneSkill[id] = skillid;
			m_ReserveSkill[skillid] = id;
		}
		else
		{
			std::map<int, int>::iterator i = m_RuneSkill.find(id);
			if (i != m_RuneSkill.end())
			{
				std::map<int, int>::iterator j = m_ReserveSkill.find(i->second);
				if (j != m_ReserveSkill.end())
				{
					m_ReserveSkill.erase(j);
				}
				m_RuneSkill.erase(i);
			}
		}
	}
	else
	{
		std::map<int, int>::iterator i = m_RuneSkill.find(id);
		if (i != m_RuneSkill.end())
		{
			m_ReserveSkill.erase(m_ReserveSkill.find(i->second));
			m_RuneSkill.erase(i);
		}
	}
}

void SkillManager::reSetSkilList()
{
	for( std::map<int,tagSkillDyAttrib*>::iterator iter = m_SkillDyAttrib.begin(); iter != m_SkillDyAttrib.end(); )
	{
		int id = iter->first;
		int lv = iter->second->SkillLevel;
		delete iter->second;
		iter = m_SkillDyAttrib.erase(iter);	
		GameMsgPrint::getSingleton().forgetSkill(id, lv);
	}
	m_ReserveSkill.clear();
	m_RuneSkill.clear();
	m_SpellSample.clear();
}

tagSkillDyAttrib * SkillManager::getSkillDyAttrib(int id)
{
	std::map<int, tagSkillDyAttrib*>::iterator iter = m_SkillDyAttrib.find( id );
	if( iter != m_SkillDyAttrib.end() )
	{
		return iter->second;
	}
	return NULL;
}

void SkillManager::clearCD(int id)
{
	std::map<int, tagCDTime>::iterator i = m_SkillCD.find(id);
	if (i != m_SkillCD.end())
	{
		i->second.startTime = i->second.LastTime = 0;
		if (m_SpellUIControl != NULL)
		{
			const tagCDTime &cd = getCDTime(id);
			m_SpellUIControl->setCoolTime(id,
				cd.past(), cd.total());
		}
	}
}

void SkillManager::clearAllCD()
{
	if (m_SpellUIControl != NULL)
	{
		for (std::map<int, tagSkillDyAttrib*>::iterator iter = m_SkillDyAttrib.begin();
			iter != m_SkillDyAttrib.end(); ++iter)
		{
			m_SpellUIControl->setCoolTime(iter->first, m_PublicCD.past(), 
				m_PublicCD.total());
		}
	}
	m_SkillCD.clear();
}

void SkillManager::delSkillDyAttrib(int id)
{
	std::map<int, tagSkillDyAttrib*>::iterator iter_SA = m_SkillDyAttrib.find( id );
	if( iter_SA != m_SkillDyAttrib.end() )
	{
		int lv = iter_SA->second->SkillLevel;
		delete iter_SA->second;
		m_SkillDyAttrib.erase( iter_SA );
		GameMsgPrint::getSingleton().forgetSkill(id, lv);
		{
			std::map<int, int>::iterator i = m_RuneSkill.find(id);
			if (i != m_RuneSkill.end())
			{
				m_ReserveSkill.erase(m_ReserveSkill.find(i->second));
				m_RuneSkill.erase(i);
			}
		}
	}
}

int SkillManager::getSpellLv(int id)
{
	id = getReserveSkillId(id);
	// 改变技能等级buff判断 STATUS_RESULT_CHG_SKILL_LEVEL 
	MainGamePlayer*	mainplayer	= ActorManager::getSingleton().getMainPlayer();
	if( mainplayer != NULL)
	{
		GameBufStatus* pActorStatus = mainplayer->getBufStatus();
		GameBuf* pActorBuf = pActorStatus->getBufResult( STATUS_RESULT_CHG_SKILL_LEVEL );
		if( pActorBuf != NULL )
		{
			const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( pActorBuf->getBufId(), 
				pActorBuf->getBufLv() );

			for (int i = 0; i < MAX_STATUS_RESULT; i++)
			{
				if ( statusDef.Result[i].ResultID == STATUS_RESULT_CHG_SKILL_LEVEL && statusDef.Result[i].ResultVal1 == id )
				{
					return statusDef.Result[i].ResultVal2;
				}
			}
		}
	}
	
	const tagSkillDyAttrib *attrib = getSkillDyAttrib(id);
	if (attrib == NULL)
	{
		return 1;
	}
	return attrib->SkillLevel;
}

bool SkillManager::isTmpSkill(int id)
{
	return id ? m_TmpSkills.find(id) != m_TmpSkills.end() : true;
}

bool SkillManager::canMoveAttack(int id)
{
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	return SkillDef.MoveAttack == 1 || SkillDef.BeatBack == 1;
}

bool SkillManager::isNoPrepareSkill(int id)
{
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	return SkillDef.PrepareType  == PREPARE_TYPE_NONE;
}

bool SkillManager::canCastSelf(int id)
{
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	return (SkillDef.TargetType & SKILL_TARGET_SELF) != 0;
}

void SkillManager::addTmpSkill(int id)
{
	if (id)
	{
		m_TmpSkills.insert(id);
	}
}

void SkillManager::delTmpSkill(int id)
{
	std::set<int>::iterator i = m_TmpSkills.find(id);
	if (i != m_TmpSkills.end())
	{
		m_TmpSkills.erase(i);
	}
}

void SkillManager::reduceCD(int id, unsigned int reduceTime)
{
	std::map<int, tagCDTime>::iterator i = m_SkillCD.find(id);
	if (i != m_SkillCD.end() && i->second.remain() > 0)
	{
		unsigned int now = timeGetTime();
		int remain = i->second.remain();// 剩余冷却时间
		remain -= reduceTime;
		if (m_PublicCD.remain() > 0)
		{
			int publicRemain = m_PublicCD.remain();
			if (remain < publicRemain)
			{
				remain = publicRemain;
			}
		}
		remain = remain > 0 ? remain : 0;
		i->second.startTime = now + remain - i->second.total();
		i->second.LastTime = now + remain;
		if (m_SpellUIControl != NULL)
		{
			const tagCDTime &cd = getCDTime(id);
			m_SpellUIControl->setCoolTime(id,
				cd.past(), cd.total());
		}
	}
}

int SkillManager::getSkillDistance(int id)
{
	return getSkillDistance(id, getSpellLv(id));
}

int SkillManager::getSkillDistance(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, lv).Distance;
}

void SkillManager::setNextSpell(GameSpell *pSpell /*= NULL*/, int id /*= 0*/, bool immediately /*= true*/)
{
	m_pNextSpell = pSpell;
	m_pNextSpellId = id;
	if (immediately)
	{
		m_nextTime = 0;
	}
	else
	{
		m_nextTime = timeGetTime() + 1000;
	}
	if (id != 0 && id == getNormalAttackSkill())
	{
		if (pSpell == NULL)
		{
			m_isInAttack = 0;
		}
		else
		{
			m_isInAttack = pSpell->getSingleTarget();
		}
	}
}

void SkillManager::setNextSpell(int skillId, bool immediately /*= true*/)
{
	setNextSpell(skillId, ActorManager::getSingleton().getMainPlayer()->GetSelectTarget(), immediately);
}

void SkillManager::setNextSpell(int skillId, int targetId, bool immediately /*= true*/)
{
	GameSpell *pSpell = NULL;
	if (skillId == -1)
	{
		m_isInAttack = -1;
	}
	else if (skillId != 0)
	{
		int id = getInsteadSkillId(skillId);
		int nId = getRuneSkillId(id);
		pSpell = createSpell(nId, getSpellControler());
		pSpell->setSpellLv(getSpellLv(skillId));
		pSpell->setSingleTarget(targetId);
	}
	setNextSpell(pSpell, skillId, immediately);
}

void SkillManager::setNextSpell(int skillId, int x, int y, bool immediately /*= true*/)
{
	setNextSpell(skillId, ActorManager::getSingleton().getMainPlayer()->GetSelectTarget(), x, y, immediately);
}

void SkillManager::setNextSpell(int skillId, int targetId, int x, int y, bool immediately /*= true*/)
{
	GameSpell *pSpell = NULL;
	if (skillId == -1)
	{
		m_isInAttack = -1;
	}
	else if (skillId != 0)
	{
		int id = getInsteadSkillId(skillId);
		int nId = getRuneSkillId(id);
		pSpell = createSpell(nId, getSpellControler());
		pSpell->setSpellLv(getSpellLv(id));
		pSpell->setSingleTarget(targetId);
		pSpell->setTargetPoint(x, y);
	}
	setNextSpell(pSpell, skillId, immediately);
}

void SkillManager::spellAttack(GameSpell *p, unsigned int delay)
{
	attackArray.push_back(std::pair<unsigned int, GameSpellPtr>(timeGetTime() + delay, p));
	std::push_heap(attackArray.begin(), attackArray.end(), PairFirstGreater());
}

bool SkillManager::testResult(int result)
{
	static int ret2tips[][2] = {
		{GameSpell::RET_ENV, MID_MAP_NOSKILL},
		{GameSpell::RET_SAFEENV, MID_SAFE_ENV_NOSKILL},
		{GameSpell::RET_WEAPONSHORT, MID_SPELL_UNUSED_NONEEQUIP},
		{GameSpell::RET_CD, MID_XP_COOLDOWNTIME},
		{GameSpell::RET_MPSHORT, MID_SHORTCUT_MPNOTENOUGH},
		{GameSpell::RET_XPSHORT, MID_XP_NOENOUGHXP},
		{GameSpell::RET_COLLECTSHORT, MID_XP_NOENOUGHCOLLECT},
		{GameSpell::RET_ENERGYSHORT, MID_SHORTCUT_ENERGENOTENOUGH},
		{GameSpell::RET_MACHINEENERGYSHORT, MID_NOT_ENOUGH_ENERGY},
		{GameSpell::RET_INFIGHT, MID_INFIGHT},
		{GameSpell::RET_OUTFIGHT, MID_NOINFIGHT},
		{GameSpell::RET_BUFSTOP, MID_SHORTCUT_CANNOTUSESPELL_BUF},
		{GameSpell::RET_MOVESTOP, MID_SHORTCUT_CANNOTUSESPELL_MOVE},
		{GameSpell::RET_NOTARGET, MID_SPELL_NOTARGET},
		{GameSpell::RET_NOTTYPE, MID_SPELL_UNUSED_OBJ},
		{GameSpell::RET_NOTCAMP, MID_SPELL_UNUSED_CAMP},
		{GameSpell::RET_DEAD, MID_SPELL_UNUSED_DEAD},
		{GameSpell::RET_MASKTARGET, MID_SPELL_UNUSED_MASK_TARGET},
		{GameSpell::RET_MASKPOINT, MID_SPELL_UNUSED_MASK_POINT},
		{GameSpell::RET_DOOTHER, MID_SELF_BUSY},
		{GameSpell::RET_SPELLPRE, MID_SELF_SPELLPRE},
		{GameSpell::RET_SELFDEAD, MID_SELF_DEAD},
		{GameSpell::RET_NOITEM,MID_NOTENOUGH_COST_ITEM},
		{GameSpell::RET_WORLDCITYCONTROL,MID_NO_ATTACK_WORLD_CITY_CONTROL},
	};

	for (size_t i = 0; i < sizeof(ret2tips) / sizeof(int) / 2; i++)
	{
		if (result == ret2tips[i][0])
		{
			GameMsgPrint::getSingleton().showMidTip(EMID_MESSAGE(ret2tips[i][1]));
			break;
		}
	}
	return result == GameSpell::RET_SUCCESS || result == GameSpell::RET_OUTRANGE || result == GameSpell::RET_MASKTARGET || result == GameSpell::RET_MASKPOINT;
}

int SkillManager::useSpellOnce(int id)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
	{
		return GameSpell::RET_UNKNOWN;
	}
	int result = useSpell(id);
	unUseSpell();
	return result;
}

int SkillManager::useSpellOnce(int id, int target)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
	{
		return GameSpell::RET_UNKNOWN;
	}
	int result = useSpell(id, target);
	unUseSpell();
	return result;
}

int SkillManager::useSpellOnce(int id, int x, int y)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
	{
		return GameSpell::RET_UNKNOWN;
	}
	int result = useSpell(id, x, y);
	unUseSpell();
	return result;
}

int SkillManager::useSpellOnce(int id, int target, int x, int y)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
	{
		return GameSpell::RET_UNKNOWN;
	}
	int result = useSpell(id, target, x, y);
	unUseSpell();
	return result;
}

void SkillManager::setSpellCollectNumCur(int id, int nCollectNum)
{
	std::map<int, tagSkillDyAttrib*>::iterator iter = m_SkillDyAttrib.find( id );
	if( iter != m_SkillDyAttrib.end() )
	{
		(iter->second)->CollectNum = nCollectNum;
	}
}

bool SkillManager::isSkillLearned(int id)
{
	return getSkillDyAttrib(getReserveSkillId(id)) != NULL;
}

bool SkillManager::getSpellRuneEnable(int id)
{
	const tagSkillDyAttrib *attrib = getSkillDyAttrib(id);
	return attrib && attrib->RuneSlotEnabled;
}

char SkillManager::getSpellRuneType(int id)
{
	const tagSkillDyAttrib *attrib = getSkillDyAttrib(id);
	return attrib ? attrib->RuneSlot : 0;
}

int SkillManager::getSpellRuneItemId(int id)
{
	const tagSkillDyAttrib *attrib = getSkillDyAttrib(id);
	return attrib == NULL ? 0 : attrib->RuneItemID;
}

int SkillManager::getSpellRuneItemId(int id, int type)
{
	const tagSkillDyAttrib *attrib = getSkillDyAttrib(id);
	if (attrib != NULL)
	{
		for (size_t i = 0; i < attrib->RuneItemIDs.size(); ++i)
		{
			int itemid = attrib->RuneItemIDs[i];
			if (itemid != 0)
			{
				const tagItemDef &itemdef = DBDataManager::getSingleton().getItemDef(itemid);
				for (int j = 0; j < MAX_ITEM_RESULT; ++j)
				{
					if (itemdef.Result[j].ResultID == RESULT_RUNE && itemdef.Result[j].ResultVal2 == type)
					{
						return itemid;
					}
				}
			}
		}
	}
	return 0;
}

const char * SkillManager::getSpkillRuneDesc(int id, int level, int type)
{
	int skillid = 0;
	const tagSkillDef &skillDef = DBDataManager::getSingleton().GetSkillDef(id, level);
	switch (type)
	{
	case RUNE_RED:
		skillid = skillDef.RedRuneTransferedID;
		break;
	case RUNE_BLUE:
		skillid = skillDef.BlueRuneTransferedID;
		break;
	case RUNE_YELLOW:
		skillid = skillDef.YellowRuneTransferedID;
		break;
	}
	if (skillid == 0)
	{
		return "";
	}
	return DBDataManager::getSingleton().GetSkillDef(skillid, level).RuneDesc;
}

const char * SkillManager::getSpkillRuneDesc(int id, int type)
{
	return getSpkillRuneDesc(id, getSpellLv(id), type);
}

void SkillManager::spellStop(GameSpell *p, unsigned int delay)
{
	stopArray.push_back(std::pair<unsigned int, GameSpellPtr>(timeGetTime() + delay, p));
	std::push_heap(stopArray.begin(), stopArray.end(), PairFirstGreater());
}

void SkillManager::spellBreak(GameSpell *p, unsigned int delay)
{
	breakArray.push_back(std::pair<unsigned int, GameSpellPtr>(timeGetTime() + delay, p));
	std::push_heap(breakArray.begin(), breakArray.end(), PairFirstGreater());
}

int SkillManager::getInsteadSkillId(int id)
{
	std::map<int, std::pair<int, std::vector<int>>>::iterator i = m_InsteadSkills.find(id);
	if (i != m_InsteadSkills.end())
	{
		size_t n = i->second.second.size();
		if (n > 1)
		{
			int &ignore = i->second.first;
			int m = 1;
			for (size_t j = 0; j < n; ++j)
			{
				int k = i->second.second[j];
				if (k != ignore)
				{
					if (((unsigned int)rand()) % m == 0)
					{
						id = k;
					}
					++m;
				}
			}
			ignore = id;
		}
		else if (n > 0)
		{
			id = i->second.second[0];
		}
	}
	return id;
}

int SkillManager::getRuneSkillId(int id)
{
	std::map<int, int>::iterator i = m_RuneSkill.find(id);
	if (i != m_RuneSkill.end())
	{
		return i->second;
	}
	return id;
}

int SkillManager::getReserveSkillId(int id)
{
	std::map<int, int>::iterator i = m_ReserveSkill.find(id);
	if (i != m_ReserveSkill.end())
	{
		return i->second;
	}
	return id;
}

void SkillManager::addInsteadBind(int id, int repId)
{
	bool has = false;
	std::pair<int, std::vector<int>> &skills = m_InsteadSkills[id];
	for (std::vector<int>::iterator i = skills.second.begin(); i != skills.second.end(); ++i)
	{
		if ((*i) == repId)
		{
			has = true;
			break;
		}
	}
	if (!has)
	{
		skills.first = 0;
		skills.second.push_back(repId);
	}
}

void SkillManager::delInsteadBind(int id, int repId)
{
	std::pair<int, std::vector<int>> &skills = m_InsteadSkills[id];
	for (std::vector<int>::iterator i = skills.second.begin(); i != skills.second.end(); ++i)
	{
		if ((*i) == repId)
		{
			skills.second.erase(i);
			if (skills.second.empty())
			{
				m_InsteadSkills.erase(m_InsteadSkills.find(id));
			}
			break;
		}
	}
}

void SkillManager::execSpellStop()
{
	unsigned int now = timeGetTime();
	while (!stopArray.empty())
	{
		if (stopArray.front().first > now)
		{
			break;
		}
		GameSpellPtr p = stopArray.front().second;
		std::pop_heap(stopArray.begin(), stopArray.end(), PairFirstGreater());
		stopArray.pop_back();
		p->onStop();
	}
}

int SkillManager::getSpellWeaponType(int id, unsigned int idx)
{
	return getSpellWeaponType(id, getSpellLv(id), idx);
}

int SkillManager::getSpellWeaponType(int id, int lv, unsigned int idx)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	return SkillDef.ArmType[idx].ArmType;
}

int SkillManager::getNormalAttackSkill()
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return 0;
	}
	return getNormalAttackSkill(pMainPlayer->GetID());
}

int SkillManager::getNormalAttackSkill(int actorId)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(actorId);
	if (pActor == NULL)
	{
		return 0;
	}
	int nRace = pActor->getRace();
	return nLeftSkill[nRace & 0x0f];
}

bool SkillManager::isNormalSkill(int id)
{
	int lv = getSpellLv(id);
	return isNormalSkill(id, max(lv, 1));
}

bool SkillManager::isNormalSkill(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id)).SkillType == SKILL_TYPE_NORMAL;
}

bool SkillManager::isSkillPassive(int id)
{
	int lv = getSpellLv(id);
	return isSkillPassive(id, max(lv, 1));
}

bool SkillManager::isSkillPassive(int id, int lv)
{
	return DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id)).SkillType == SKILL_TYPE_FAIRY_BACKHIT;
}

GameSpell * SkillManager::getNextSpell()
{
	return m_pNextSpell;
}

int SkillManager::getSpkillRuneSkillId(int id, int type)
{
	int skillid = 0;
	const tagSkillDef &skillDef = DBDataManager::getSingleton().GetSkillDef(id, getSpellLv(id));
	switch (type)
	{
	case RUNE_RED:
		skillid = skillDef.RedRuneTransferedID;
		break;
	case RUNE_BLUE:
		skillid = skillDef.BlueRuneTransferedID;
		break;
	case RUNE_YELLOW:
		skillid = skillDef.YellowRuneTransferedID;
		break;
	}
	return skillid;
}

bool SkillManager::castPlayerLv(int id)
{
	return castPlayerLv(id, getSpellLv(id));
}

bool SkillManager::castPlayerLv(int id, int lv)
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return false;
	}
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	return !SkillManager::getSingleton().isTmpSkill(id) || SkillDef.Learn.LevelMin <= player->getLv();
}

void SkillManager::reset()
{
	hitArray.clear();
	attackArray.clear();
}

bool SkillManager::castItem(int id)
{
	return castItem(id, getSpellLv(id));
}

bool SkillManager::castItem(int id, int lv)
{
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if( SkillDef.UseItemID > 0 )
	{
		MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
		if (player == NULL)
		{
			return false;
		}
		if( player->getContainer().getItemCount( SkillDef.UseItemID ) > 0 )
		{
			return true;
		}
		return false;
	}
	return true;
}
