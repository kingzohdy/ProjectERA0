#include "GameSkillLearn.h"
#include "GameDBData.h"
#include "GameActor.h"
#include "GameQuest.h"
#include "GameSkill_Mgr.h"
#include "GameMainPlayer.h"

GameSkillLearn::GameSkillLearn(void)
{
	tagSkillLearnInfo Info;
	memset( &Info, 0, sizeof(tagSkillLearnInfo) );
	m_SkillLearnAll[SKILLLEARN_TYPE_PUBLIC] = Info;
	m_SkillLearnCan[SKILLLEARN_TYPE_PUBLIC] = Info;
	DBDataManager::getSingleton().loadSkillDef( m_AllSkillDef );
}

GameSkillLearn::~GameSkillLearn(void)
{
}

bool GameSkillLearn::isSkillAutoLearn(int skillId, int lv)
{
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(skillId, lv);
	return  SkillDef.Learn.Mode == SKILL_LEARN_AUTO;
}

bool GameSkillLearn::isSkillNeedBookLearn(int skillId, int lv)
{
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef(skillId, lv);
	return  SkillDef.Learn.Mode == SKILL_LEARN_BOOK;
}

// 技能是否可以学习
int GameSkillLearn::checkSkillLearn( int skillId, int lv )
{
	static_assert( sizeof( tagSkillDef ) == sizeof( tagSkillDef_ToLua ) );
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef( skillId, lv );
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return SKILLLEARN_FAIL_CAREER;
	}
	
	if( pMainPlayer->getLv() < SkillDef.Learn.LevelMin )
	{
		return SKILLLEARN_FAIL_LVMIN;
	}

	if( CAREER_NON != SkillDef.Learn.Career && pMainPlayer->getRace() != SkillDef.Learn.Career && (pMainPlayer->getRace() & 0x0F) != SkillDef.Learn.Career)
	{
		return SKILLLEARN_FAIL_CAREER;
	}

	if ( pMainPlayer->getExp() < SkillDef.Learn.Exp )
	{
		return SKILLlEARN_FAIL_EXP;
	}

	if ( SkillDef.Learn.Mode == SKILL_LEARN_AUTO ) // 加个判断技能是否是自动学习的
	{
		return SKILLLEARN_FAIL_NONEED;
	}

	//////////////////////////////////////////////////////////////////////////
	// TODO::还缺素质判断
	//////////////////////////////////////////////////////////////////////////

	/*if( *(unsigned int*)pMainPlayer->getAttriValule( ATTR_ID_SKILLPOINT ) < SkillDef.Learn.SkillPoint )
	{
		return SKILLLEARN_FAIL_SKILLPOINT;
	}*/
	unsigned int nMoneyVal = 0;
	if ( SkillDef.Learn.MoneyAttrID == ATTR_ID_MONEY )
	{
		nMoneyVal = pMainPlayer->getMoney();
	}
	else
	{
		nMoneyVal = pMainPlayer->getMoney() + pMainPlayer->getBull();
		const tagLevelUP& LevelUpDef = DBDataManager::getSingleton().getLevelUP( pMainPlayer->getLv() );
		if ( &LevelUpDef != NULL )
		{
			int num = LevelUpDef.DailyUseBull - pMainPlayer->getDailyUsedBull();
			if ( pMainPlayer->getBull() > num )
			{
				num = num > 0 ? num : 0;
				nMoneyVal = num + pMainPlayer->getMoney();
			}
		}
	}

	if( nMoneyVal < SkillDef.Learn.MoneyVal )
	{
		return SKILLLEARN_FAIL_MONEY;
	}
	
	if( !GameQuest::getSingleton().isQuestFini( SkillDef.Learn.TaskID ) && SkillDef.Learn.TaskID != 0 )
	{
		return SKILLLEARN_FAIL_QUEST;
	}
	
	tagSkillDyAttrib* pSkillDyAttrib = SkillManager::getSingleton().getSkillDyAttrib( skillId );
	if ( pSkillDyAttrib != NULL && pSkillDyAttrib->SkillLevel + 1 < lv )
	{
		return SKILLlEARN_FAIL_SKILLLV;
	}
	
	else
	{
		if( pSkillDyAttrib == NULL && lv > 1 )
		{
			return SKILLlEARN_FAIL_SKILLLV;
		}
	}

	return SKILLLEARN_SUCCED;
}

// 刷新技能学习列表
void GameSkillLearn::updateSkillLearnList()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return;
	}
	
	m_SkillLearnAll[SKILLLEARN_TYPE_PUBLIC].m_nNum = 0;
	m_SkillLearnCan[SKILLLEARN_TYPE_PUBLIC].m_nNum = 0;

	for( size_t i = 0; i < m_AllSkillDef.size(); i++ )
	{
		const tagSkillDef& skillDef = m_AllSkillDef[i];
		if (skillDef.SkillID >= TMP_SKILL_MIN && skillDef.SkillID <= TMP_SKILL_MAX)
		{
			continue;
		}
		if (CAREER_NON != skillDef.Learn.Career && pMainPlayer->getRace() != skillDef.Learn.Career && (pMainPlayer->getRace() & 0x0F) != skillDef.Learn.Career)
		{
			continue;
		}
		if (skillDef.Learn.Career == 0)
		{
			continue;// 不显示共同的可学习技能，以后若需修改再去掉，需同时修改数据库
		}
		
		tagSkillDyAttrib* pAttrib = SkillManager::getSingleton().getSkillDyAttrib( skillDef.SkillID );
		int nLevel = 0;
		if( pAttrib != NULL )
		{
			nLevel = pAttrib->SkillLevel;
		}
		if (nLevel + 1 != skillDef.Level)
		{
			continue;
		}
		
		if( skillDef.Learn.Mode == SKILL_LEARN_BOOK || skillDef.Learn.Mode == SKILL_LEARN_AUTO )
		{
			continue;
		}

		/*if( skillDef.ArmType[0].ArmType == ARM_ANY || skillDef.ArmType[0].ArmType == ARM_ANY_EX )
		{*/
			int nLast		= m_SkillLearnAll[SKILLLEARN_TYPE_PUBLIC].m_nNum++;
			tagOneSkillLearnInfo info;
			info.m_nLv		= skillDef.Level;
			info.m_nSkillId	= skillDef.SkillID;
			m_SkillLearnAll[SKILLLEARN_TYPE_PUBLIC].m_SkillInfo[nLast] = info;

			if( pMainPlayer->getLv() >= skillDef.Learn.LevelMin )
			{
				nLast = m_SkillLearnCan[SKILLLEARN_TYPE_PUBLIC].m_nNum++;
				m_SkillLearnCan[SKILLLEARN_TYPE_PUBLIC].m_SkillInfo[nLast] = info;
			}
		/*}*/
	}
}

// 获取全部技能数量
const tagSkillLearnInfo* GameSkillLearn::getAllSkill( int type )const
{
	std::map<int, tagSkillLearnInfo>::const_iterator iter = m_SkillLearnAll.find( type );
	if( iter != m_SkillLearnAll.end() )
	{
		return &iter->second;
	}
	return NULL;
}

// 获取可学
const tagSkillLearnInfo* GameSkillLearn::getCanLearnSkill( int type )const
{
	std::map<int, tagSkillLearnInfo>::const_iterator iter = m_SkillLearnCan.find( type );
	if( iter != m_SkillLearnCan.end() )
	{
		return &iter->second;
	}
	return NULL;
}

//获取所指定的技能
const tagOneSkillLearnInfo* GameSkillLearn::getSkillInfo( int nSkillType, int nIndex, bool isAll ) const
{
	std::map<int, tagSkillLearnInfo>::const_iterator iter;

	if ( isAll )
	{
		iter = m_SkillLearnAll.find( nSkillType );

		if( iter != m_SkillLearnAll.end( ) )
		{
			return &(iter->second.m_SkillInfo[nIndex]);
		}
	}
	else
	{
		iter = m_SkillLearnCan.find( nSkillType );

		if( iter != m_SkillLearnCan.end( ) )
		{
			return &(iter->second.m_SkillInfo[nIndex]);
		}
	}

	return NULL;
}

// 技能学习
void GameSkillLearn::skillLearn( int npcId, int skillId, int lv )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = SKILL_LEARN;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = skillId;
	msgbody.SkillLearn.Level = lv;
	SendGameServerMsg( LEARN_CLT, msgbody );
}

// 技能升级
void GameSkillLearn::skillLvUp( int npcId, int skillId, int lv )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = SKILL_LEVEL_UP;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = skillId;
	msgbody.SkillLearn.Level = lv;
	SendGameServerMsg( LEARN_CLT, msgbody );
}

// 素质学习
void GameSkillLearn::diathesisLearn( int npcId, int diathesisId, int lv )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = DIATHESIS_LEARN;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = diathesisId;
	msgbody.SkillLearn.Level = lv;
	SendGameServerMsg( LEARN_CLT, msgbody );
}

// 素质升级
void GameSkillLearn::diathesisLvUp( int npcId, int diathesisId, int lv )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = DIATHESIS_LEVEL_UP;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = diathesisId;
	msgbody.SkillLearn.Level = lv;
	SendGameServerMsg( LEARN_CLT, msgbody );
}

// 天赋学习
void GameSkillLearn::geniusLearn( int npcId, int geniusId, int lv )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = GENIUS_LEARN;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = geniusId;
	msgbody.SkillLearn.Level = lv;
	SendGameServerMsg( LEARN_CLT, msgbody );
}

// 天赋升级
void GameSkillLearn::geniusLvUp( int npcId, int geniusId, int lv )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = GENIUS_LEVEL_UP;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = geniusId;
	msgbody.SkillLearn.Level = lv;
	SendGameServerMsg( LEARN_CLT, msgbody );
}

// 天赋重置
void GameSkillLearn::geniusReset( int npcId )
{
	tagCSPkgBody msgbody;
	msgbody.SkillLearn.type	 = GENIUS_ALL_RESET;	
	msgbody.SkillLearn.NpcID = npcId;
	msgbody.SkillLearn.ID	 = 0;
	msgbody.SkillLearn.Level = 0;
	SendGameServerMsg( LEARN_CLT, msgbody );
}
