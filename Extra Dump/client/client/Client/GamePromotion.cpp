#include "GamePromotion.h"
#include "ServerTimer.h"
#include "GameManager.h"
#include "GameUIMsg.h"
#include "OgreScriptLuaVM.h"
#include "NetMessage.h"
#include "GameMaxMap.h"
#include "GameQuest.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "BattleLimitInfo.h"
#include "PworldDifficulty.h"
#include "PworldLimitInfo.h"
#include "GameMainPlayer.h"
#include "GameBusyManager.h"
#include "ogrebloom.h"
#include <list>

GamePromotion::GamePromotion( void ) : m_nPinJiuBlurStartTime(0)
{
	m_mapActivityGuid[7] = true;  //欢乐嘉年华
	m_mapActivityGuid[23] = true; //黑暗来袭
	m_mapActivityGuid[30] = true; //魔影重重
	m_mapActivityGuid[32] = true; //牛仔很忙-决赛
	m_mapActivityGuid[42] = true; //光影之战
	m_mapActivityGuid[45] = true; //机械改造
	m_mapActivityGuid[48] = true; //末世荣耀殿堂
	//m_mapActivityGuid[54] = true; //生命在于运动
	m_mapActivityGuid[64] = true; //坦克大战
}

GamePromotion::~GamePromotion( void )
{
	m_vecAll.clear();
	m_vectPromotion.clear();
	m_vectClanPro.clear();
	m_mapPromotionTime.clear();
	m_vecDailyTask.clear();
	m_vecNeedShowDailyTask.clear();
	m_mapActivityGuid.clear();
}

GamePromotion* GamePromotion::create( const char* szFileName )
{
	GamePromotion* p = new GamePromotion;
	p->setPromotionTime();
	return p;
}

// 设置活动时间表
void GamePromotion::setPromotionTime()
{
	m_mapPromotionTime.clear();
	std::vector<tagActiveHelper*> activeHelper;
	DBDataManager::getSingleton().loadActiveHelper(activeHelper);
	for( int i = 0; i < (int)activeHelper.size() ; i++ )
	{
		tagPromotionDate time;
		time.m_nID			= activeHelper[i]->ActiveID;
		time.m_nFlag		= activeHelper[i]->Type;
		time.m_nIconID		= activeHelper[i]->ActiveIcon;
		strncpy( time.m_szName,activeHelper[i]->ActiveName,NAME_LEN );
		time.m_nType		= activeHelper[i]->ReCycleType;
		char* szEnd;
		time.m_nWeekFlag	= strtol( activeHelper[i]->WeekDay ,&szEnd,2);

		std::string strTime = activeHelper[i]->StartDate;
		strcpy(time.m_szStartDate,activeHelper[i]->StartDate);
		strcpy(time.m_szEndDate,activeHelper[i]->EndDate);

		int idx = 0;
		for( idx = 0 ; idx < MAX_ACTIVE_DAY_TIME ; idx++ )
		{
			if( strcmp(activeHelper[i]->ActiveDayTime[idx].StartTime,"-1")==0 )
			{
				idx --;
				break;
			}
			if( strlen(activeHelper[i]->ActiveDayTime[idx].StartTime) == 0 )
			{
				strcpy( time.m_Time[idx].m_szStartTime,"00:00" );
			}else{
				strcpy( time.m_Time[idx].m_szStartTime,activeHelper[i]->ActiveDayTime[idx].StartTime );
			}
			if( strlen(activeHelper[i]->ActiveDayTime[idx].EndTime) == 0 )
			{
				strcpy( time.m_Time[idx].m_szEndTime,"24:00" );
				break;
			}else{
				strcpy( time.m_Time[idx].m_szEndTime,activeHelper[i]->ActiveDayTime[idx].EndTime );
			}
			if( strlen(activeHelper[i]->ActiveDayTime[idx].StartTime) == 4 )
			{
				sprintf( time.m_Time[idx].m_szStartTime, "0%s", activeHelper[i]->ActiveDayTime[idx].StartTime );
			}
			if( strlen(activeHelper[i]->ActiveDayTime[idx].EndTime) == 4 )
			{
				sprintf( time.m_Time[idx].m_szEndTime, "0%s", activeHelper[i]->ActiveDayTime[idx].EndTime );
			}
		}
		time.m_nTimeCount = idx + 1;

		time.m_nLevel			= activeHelper[i]->LevelLimit;
		time.m_nNeedTeam		= activeHelper[i]->MemberType;
		time.m_nAutoTeamID		= activeHelper[i]->AutoTeamID;
		time.m_nRelateId		= activeHelper[i]->TaskPworldID;
		time.m_nMainRewardType	= activeHelper[i]->MainGainType;
		time.m_nMainRewardLevel = activeHelper[i]->MainGainStar;
		strcpy(time.m_szNpcName,activeHelper[i]->NpcInfo);
		strcpy(time.m_szDesc,activeHelper[i]->ActiveDesc);
		time.m_nRewardItemCount = 0;
		for( idx = 0; idx < MAX_ACTIVE_AWARD_ITEM; idx++  )
		{
			int itemid = activeHelper[i]->AwardItemID[idx];
			if( itemid == 0 )
			{
				continue;
			}
			time.m_RewardItem[idx] = itemid;
			time.m_nRewardItemCount ++;
		}

		time.m_nReCommand		= activeHelper[i]->Recommend;
		time.m_bShowInAutoTeam	= activeHelper[i]->HuoDongShow & 1?true:false;
		time.m_nTimeShowType	= activeHelper[i]->TimeShowType;

		time.TransMapID			= activeHelper[i]->TransMapID;	
		time.TransPos.X = activeHelper[i]->TransPos.X;
		time.TransPos.Y = activeHelper[i]->TransPos.Y;
		time.m_nBusyID	= activeHelper[i]->BusyID;
		time.m_nHuoDongShow		= activeHelper[i]->HuoDongShow;

		m_mapPromotionTime[time.m_nID] = time;
		m_vecAll.push_back(time.m_nID);
	}
	setPromotion();
	setClanPro();
	DBDataManager::getSingleton().loadWorldBossDef( m_vecWorldBossData );
}

void GamePromotion::setPromotion()
{
	m_vectPromotion.clear();
	std::map<int, tagPromotionDate>::iterator it = m_mapPromotionTime.begin();
	for( ; it != m_mapPromotionTime.end(); ++it )
	{
		tagPromotionDate& time = it->second;
		if( time.m_nFlag == PROMITION_FLAG_PROMITION && time.m_bShowInAutoTeam )
		{
			m_vectPromotion.push_back( it->first );
		}
	}
}

void GamePromotion::setClanPro()
{
	m_vectClanPro.clear();
	std::map<int, tagPromotionDate>::iterator it = m_mapPromotionTime.begin();
	for( ; it != m_mapPromotionTime.end(); ++it )
	{
		tagPromotionDate& time = it->second;
		if( time.m_nFlag == PROMITION_FLAG_CLAN )
		{
			m_vectClanPro.push_back( it->first );
		}
	}
}

tagPromotionDate* GamePromotion::getPromotionData( int nRow )
{
	std::map<int, tagPromotionDate>::iterator iter = m_mapPromotionTime.find( nRow );
	if ( iter == m_mapPromotionTime.end() )
	{
		return NULL;
	}
	return &(iter->second);
}

int GamePromotion::getAllNum()
{
	return (int)m_vecAll.size();
}

int GamePromotion::getPromotionNum()
{
	return (int)m_vectPromotion.size();
}

int GamePromotion::getClanProNum()
{
	return (int)m_vectClanPro.size();
}

int GamePromotion::getAllRow(int nIndex)
{
	return m_vecAll[nIndex];
}

int GamePromotion::getPromotionRow( int nIndex )
{
	return m_vectPromotion[nIndex];
}

int GamePromotion::getClanProRow( int nIndex )
{
	return m_vectClanPro[nIndex];
}

// 排序活动
int GamePromotion::sortPromition( int nProType )
{
	if( nProType == PROMOTION_ALL )
	{
		sortPromitionList( m_vecAll,1,1 ,nProType);
	}else if( nProType == PRMOTION_ACTIVE )
	{
		sortPromitionList( m_vectPromotion,1,1,nProType );
	}else if( nProType == PRMOTION_CLAN )
	{
		sortPromitionList( m_vectClanPro,1,1,nProType );
	}else if( nProType == PRMOTION_DAILY_TASK )
	{
		sortDailyGoalList();
	}else if( nProType == PRMOTION_BUSY )
	{
		sortPromitionList( m_vecAll,1,1,nProType );
	}
	return 0;
}

// 重置活动相关数据
void GamePromotion::resetProData()
{
	m_pWorldmap.clear();
	memset( &m_DailyInfo,0,sizeof(CSACTIONDAILYGOALINFO) );
	m_vecNeedShowDailyTask.clear();
	m_mapDynActiveInfo.clear();
}

// 获取当前服务器时间的天
struct tm* GamePromotion::getServerTimeTm()
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	time_t t = (time_t)sec;

	return localtime( &t );
}

bool CmpDailyTaskList( int lhs, int rhs )
{
	bool bLhsFinish = GameQuest::getSingleton().getQusetHaveFinishNumOneDay( lhs ) >= GameQuest::getSingleton().getQuestMaxCanAcceptNumOneDay( lhs );
	bool bRhsFinish = GameQuest::getSingleton().getQusetHaveFinishNumOneDay( rhs ) >= GameQuest::getSingleton().getQuestMaxCanAcceptNumOneDay( rhs );
	if( bLhsFinish && !bRhsFinish )
	{
		return false;
	}else if( !bLhsFinish && bRhsFinish )
	{
		return true;
	}

	int lhsRunState = GameQuest::getSingleton().getQuestRunState( lhs );
	int rhsRunState = GameQuest::getSingleton().getQuestRunState( rhs );
	int bLhsFlag = GameQuest::getSingleton().isQuestMust( lhs );
	int bRhsFlag = GameQuest::getSingleton().isQuestMust( rhs );
	if( lhsRunState == CLIENT_TASK_STAT_FINI && rhsRunState != CLIENT_TASK_STAT_FINI )
	{
		return true;
	}else if( lhsRunState != CLIENT_TASK_STAT_FINI && rhsRunState == CLIENT_TASK_STAT_FINI )
	{
		return false;
	}

	if( lhsRunState == CLIENT_TASK_STAT_NO && rhsRunState != CLIENT_TASK_STAT_NO )
	{
		return true;
	}else if( lhsRunState != CLIENT_TASK_STAT_NO && rhsRunState == CLIENT_TASK_STAT_NO )
	{
		return false;
	}else if( bLhsFlag && !bRhsFlag )
	{
		return true;
	}else if( !bLhsFlag && bRhsFlag )
	{
		return false;
	}else if( lhsRunState == CLIENT_TASK_STAT_FINI && rhsRunState != CLIENT_TASK_STAT_FINI )
	{
		return true;
	}
	return lhs > rhs ;
}

void GamePromotion::sortDailyGoalList()
{
	if( m_vecNeedShowDailyTask.size() <= 1 )
	{
		return;
	}
	sort( m_vecNeedShowDailyTask.begin(),m_vecNeedShowDailyTask.end(),CmpDailyTaskList );
}

// 活动信息数据排序
void GamePromotion::sortPromitionList( std::vector<int>& vectProData, int nType, int nFlag,int nProType )
{
	for( size_t i = 0; i < vectProData.size()-1; i++ )
	{
		if( vectProData.size() <= 1 )
		{
			break;
		}

		for( size_t j = i; j < vectProData.size(); j++ )
		{
			if( cmpMember( vectProData[i], vectProData[j], nType,nProType )*nFlag < 0 )
			{
				std::swap( vectProData[i], vectProData[j] );
			}
		}
	}
}

// 比较成员的大小
int GamePromotion::cmpMember( const int src, const int des, const int nType,int nProType )
{
	bool bSrcFinish = isFinishActive( src );
	bool bDesFinish = isFinishActive( des );
	if( bSrcFinish && !bDesFinish )
	{
		return -1;
	}else if( !bSrcFinish && bDesFinish )
	{
		return 1;
	}
	bool bSrcIsBeach = isBeachPromotion( src );
	bool bDesIsBeach = isBeachPromotion( des );
	if ( nProType == PRMOTION_BUSY )
	{
		if ( GameBusyManager::getSingleton().GetBeachTime() > 0 )
		{
			if( bSrcIsBeach && !bDesIsBeach )
			{
				return -1;
			}else if( !bSrcIsBeach && bDesIsBeach )
			{
				return 1;
			}
		}
		if ( ( m_mapPromotionTime[src].m_nBusyID == 0 && m_mapPromotionTime[des].m_nBusyID > 0 )
			|| ( m_mapPromotionTime[src].m_nBusyID > 0 && m_mapPromotionTime[des].m_nBusyID == 0 ) )
		{
			return m_mapPromotionTime[src].m_nBusyID - m_mapPromotionTime[des].m_nBusyID;
		}
	}
	int srcFlag = getCurrPromitionStartInfo(src)->startFlag;
	int desFlag = getCurrPromitionStartInfo(des)->startFlag;

	int srcCanJoin = getAutoTeamState( src );
	int desCanJoin = getAutoTeamState( des );

	bool bSrcCanJoin = ( srcCanJoin == PROMOTION_OK ||srcCanJoin == PROMOTION_NOT_NEED_AUTOTEAM || srcCanJoin == PROMOTION_SINGLE_LIMIT ) & (srcFlag == PROMOTION_RUNING) ;
	bool bDesCanJoin = ( desCanJoin == PROMOTION_OK ||desCanJoin == PROMOTION_NOT_NEED_AUTOTEAM || desCanJoin == PROMOTION_SINGLE_LIMIT ) & ( desFlag == PROMOTION_RUNING );
	if( bSrcCanJoin && !bDesCanJoin )
	{
		return 1;
	}else if( !bSrcCanJoin && bDesCanJoin )
	{
		return -1;
	}

	return m_mapPromotionTime[des].m_nLevel - m_mapPromotionTime[src].m_nLevel;

	return -1;
}

// 比较函数
int GamePromotion::comp_func(const void * v1, const void *v2)
{
	return *((int *)v1) - *((int *)v2);
}

//是否是战场
bool GamePromotion::isBattleActive( int nIndex )
{
	tagPromotionDate& promotion = m_mapPromotionTime[nIndex];
	if( promotion.m_nFlag == PROMITION_FLAG_PROMITION )
	{
		if( !(promotion.m_bShowInAutoTeam && promotion.m_nAutoTeamID != 0) )
		{
			const tagPworldDef_ToLua* pWorld = getPworldDef(promotion.m_nRelateId);
			if( pWorld != NULL && pWorld->BattleID > 0 )
			{
				return true;
			}
		}
	}
	return false;
}
// 判断是否在 设定期限内
bool GamePromotion::isInDatePeriod( int nIndex )
{
	struct tm* curTime = getServerTimeTm();
	tagPromotionDate& promotion = m_mapPromotionTime[nIndex];
	char buf[9];
	//todo here 对长度的判断
	strftime(buf, sizeof(buf), "%Y%m%d",curTime);
	if( strlen(promotion.m_szStartDate) == 8 && strlen(promotion.m_szEndDate) == 8 )
	{
		if( atoi( buf ) - atoi( promotion.m_szStartDate ) < 0 || (atoi(buf) - atoi(promotion.m_szEndDate)) > 0 )
		{
			return false;
		}
	}
	return true;
}
//判断是否已经开始
PromotionStartInfo* GamePromotion::getCurrPromitionStartInfo( int nIndex )
{
	static PromotionStartInfo startInfo;
	struct tm* curTime = getServerTimeTm();
	tagPromotionDate& promotion = m_mapPromotionTime[nIndex];
	char buf[9];
	//todo here 对长度的判断
	strftime(buf, sizeof(buf), "%Y%m%d",curTime);
	if( strlen(promotion.m_szStartDate) == 8 && strlen(promotion.m_szEndDate) == 8 )
	{
		if( atoi( buf ) - atoi( promotion.m_szStartDate ) < 0 || (atoi(buf) - atoi(promotion.m_szEndDate)) > 0 )
		{
			startInfo.startFlag = PROMOTION_NOTTODAY;
			startInfo.timeIdx = -1;
			return &startInfo;
		}
	}
	//如果是每周
	if( promotion.m_nType == PROMOTION_TYPE_WEEKLY )
	{
		if( !(promotion.m_nWeekFlag & (1 << (6 - curTime->tm_wday))) )
		{
			startInfo.startFlag = PROMOTION_NOTTODAY;
			startInfo.timeIdx = -1;
			return &startInfo;
		}
	}

	strftime( buf, sizeof(buf), "%H0%M", curTime);
	for( int i = 0 ; i < promotion.m_nTimeCount;i++ )
	{
		char szTmp[9];
		strcpy(szTmp,promotion.m_Time[i].m_szStartTime);
		szTmp[2] = '0';

		if( atoi(buf)-atoi(szTmp) < 0 )
		{
			startInfo.startFlag = PROMOTION_COMING;
			startInfo.timeIdx = i;
			return &startInfo;
		}
		strcpy(szTmp,promotion.m_Time[i].m_szEndTime);
		szTmp[2] = '0';
		if( atoi(buf)-atoi(szTmp) < 0 )
		{
			startInfo.startFlag = PROMOTION_RUNING;
			startInfo.timeIdx = i;
			return &startInfo;
		}
	}
	startInfo.startFlag = PROMOTION_PASSED;
	startInfo.timeIdx = -1;
	return &startInfo;
}

bool GamePromotion::isInTime( const char* szStartTime ,int min)
{
	int nStartHour,nStartMin;
	sscanf(szStartTime,"%d:%d",&nStartHour,&nStartMin);
	struct tm* curTime = getServerTimeTm();
	if( curTime->tm_hour*60 + curTime->tm_min + min < nStartHour*60 + nStartMin  )
	{
		return false;
	}	
	return true;
}

int GamePromotion::getRepeatCount( int nRow )
{
	tagPromotionDate& promotionInfo = m_mapPromotionTime[nRow];
	if( promotionInfo.m_nRelateId == 0 )
	{
		return 0;
	}
	if( promotionInfo.m_nFlag == PROMITION_FLAG_QUEST )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( promotionInfo.m_nRelateId );
		// 任务是否有组限制
		if( taskDef.LimitGroupID > 0 )
		{
			const tagTaskLimitGroup& taskLimitGroup = DBDataManager::getSingleton().getTaskLimitGroup( taskDef.LimitGroupID );
			return taskLimitGroup.GroupOneDay;
		}

		int nRecycleID = GameQuest::getSingleton().getFirstNotLvLimitRecycle( taskDef.ID );
		if( nRecycleID > 0 )
		{
			const tagTaskRecycleDef& questRecycleDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
			// 每天完成次数的限制
			return questRecycleDef.OneDayNum;
		}
		// 非循环任务没有次数限制
		if( taskDef.Redo == 0 )
		{
			return 1;
		}
		return taskDef.TaskGetLimit.NumOneDay;

	}else if ( promotionInfo.m_nFlag == PROMITION_FLAG_PROMITION )
	{
		if( promotionInfo.m_bShowInAutoTeam && promotionInfo.m_nAutoTeamID != 0 )
		{
			const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef( promotionInfo.m_nRelateId );
			if( diffPworldDef != NULL )
			{	
				int pworldId = 0;
				if( diffPworldDef->PworldInfo[1].PworldID == 0 )
				{
					pworldId = PworldDifficulty::getSingleton().getDifficultyPworldId( promotionInfo.m_nRelateId,0 );

				}else{
					pworldId = PworldDifficulty::getSingleton().getDifficultyPworldId( promotionInfo.m_nRelateId,MAX_DIFF_PWORLD_LIST-1 );
				}
				PworldLimitData& limitData = PworldLimitInfo::getSingleton().getPworldData(pworldId);
				return limitData.freeTimeTotal;
			}
		}else{
			//战场活动
			const tagPworldDef_ToLua* pWorld = getPworldDef(promotionInfo.m_nRelateId);
			if( pWorld != NULL )
			{
				if( pWorld->BattleID > 0 )
				{
					const tagBattleDef* pBattleDef = DBDataManager::getSingleton().getBattleDef( pWorld->BattleID );
					if( pBattleDef != NULL )
					{
						return pBattleDef->NumEveryDay;
					}
				}
				PworldLimitData& limitData = PworldLimitInfo::getSingleton().getPworldData( pWorld->PworldID );
				return limitData.freeTimeTotal;
			}
		}
	}else if ( promotionInfo.m_nFlag == PROMITION_FLAG_TASKGROUP )
	{
		const tagTaskLimitGroup& taskLimitGroup = DBDataManager::getSingleton().getTaskLimitGroup( promotionInfo.m_nRelateId );
		if ( &taskLimitGroup != NULL )
		{
			return taskLimitGroup.GroupOneDay;
		}
	}
	return 0;
}

int GamePromotion::getFinishCount( int nRow )
{
	tagPromotionDate& promotionInfo = m_mapPromotionTime[nRow];
	if( promotionInfo.m_nRelateId == 0 )
	{
		return 0;
	}
	if( promotionInfo.m_nFlag == PROMITION_FLAG_QUEST )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( promotionInfo.m_nRelateId );
		// 任务是否有组限制
		if( taskDef.LimitGroupID > 0 )
		{
			return GameQuest::getSingleton().getGroupQusetOneDayNum( taskDef.LimitGroupID );
		}
		// 任务是否环组任务
		int nRecycleID = GameQuest::getSingleton().getFirstNotLvLimitRecycle( taskDef.ID );
		if( nRecycleID > 0 )
		{
			// 每天完成次数的限制
			return GameQuest::getSingleton().getReCycleQusetOneDayNum( nRecycleID );
		}
		// 非循环任务没有次数限制
		if( taskDef.Redo == 0 )
		{
			return 0;
		}
		return GameQuest::getSingleton().getQusetHaveFinishNumOneDay( promotionInfo.m_nRelateId );

	}else if( promotionInfo.m_nFlag == PROMITION_FLAG_PROMITION )
	{
		if( promotionInfo.m_bShowInAutoTeam && promotionInfo.m_nAutoTeamID != 0 )
		{
			const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef( promotionInfo.m_nRelateId );
			
			if( diffPworldDef != NULL )
			{	
				int pworldId = 0;
				if( diffPworldDef->PworldInfo[1].PworldID == 0 )
				{
					pworldId = PworldDifficulty::getSingleton().getDifficultyPworldId( promotionInfo.m_nRelateId,0 );

				}else{
					pworldId = PworldDifficulty::getSingleton().getDifficultyPworldId( promotionInfo.m_nRelateId,MAX_DIFF_PWORLD_LIST-1 );
				}
				PworldLimitData& limitData = PworldLimitInfo::getSingleton().getPworldData(pworldId);
				return limitData.freeTimeUse;
			}
		}else{
			//战场活动
			const tagPworldDef_ToLua* pWorld = getPworldDef(promotionInfo.m_nRelateId);
			if( pWorld != NULL )
			{
				if( pWorld->BattleID > 0 )
				{
					BattleLimitData& data = BattleLimitInfo::getSingleton().getBattleData( pWorld->BattleID );
					return data.Use;
				}else{
					PworldLimitData& limitData = PworldLimitInfo::getSingleton().getPworldData( pWorld->PworldID );
					return limitData.freeTimeUse;
				}
			}
		}		
		std::map<int,int>::iterator it = m_pWorldmap.find( promotionInfo.m_nRelateId );
		if( it == m_pWorldmap.end() )
		{
			return 0;
		}
		return it->second;
	}
	else if ( promotionInfo.m_nFlag == PROMITION_FLAG_TASKGROUP )
	{
		return GameQuest::getSingleton().getGroupQusetOneDayNum( promotionInfo.m_nRelateId );
	}
	return 0;
}

int GamePromotion::getSerHour()
{
	struct tm* curTime = getServerTimeTm();
	return curTime->tm_hour;
}
int GamePromotion::getSerMin()
{
	struct tm* curTime = getServerTimeTm();
	return curTime->tm_min;
}

bool GamePromotion::isWeekOpen(int nRow,int nIndex)
{
	tagPromotionDate& data = m_mapPromotionTime[nRow];
	return ( data.m_nWeekFlag & (1<<(6 - nIndex)) );
}

bool GamePromotion::isWeekToday(int nIndex)
{
	struct tm* curTime = getServerTimeTm();
	return (curTime->tm_wday == nIndex);
}

int GamePromotion::getFirstOpenWeek( int nRow )
{
	struct tm* curTime = getServerTimeTm();
	for( int i = curTime->tm_wday ; i < curTime->tm_wday + 7 ; i++ )
	{
		int nIndex = i%7;
		if( isWeekOpen(nRow,nIndex) )
		{
			return nIndex;
		}
	}
	return 0;
}

AutoTeamData* GamePromotion::getAutoTeamData( int teamID )
{
	std::map<int, tagPromotionDate>::iterator it = m_mapPromotionTime.begin();
	for( ; it != m_mapPromotionTime.end();it ++ )
	{
		if( it->second.m_nAutoTeamID == teamID )
		{
			static AutoTeamData data;
			data.mapID = data.npcID = 0;
			std::string tmp( it->second.m_szNpcName );
			size_t start = tmp.find('[');
			if( start != tmp.npos )
			{
				tmp = tmp.substr(start);
				sscanf(tmp.c_str(),"[%d,%d]#n",&data.npcID,&data.mapID);
			}
			data.proType	= it->second.m_nFlag;
			data.relateID	= it->second.m_nRelateId;
			data.nRow		= it->first;
			return &data;
		}
	}
	return NULL;
}
//顺序;
int GamePromotion::getAutoTeamState( int nRowID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagPromotionDate& data = m_mapPromotionTime[nRowID];
	
	PromotionStartInfo* info = getCurrPromitionStartInfo(nRowID);
	if( info->startFlag == PROMOTION_COMING )
	{
		return PROMOTION_NOT_START;
	}

	if( info->startFlag == PROMOTION_NOTTODAY )
	{
		return PROMOTION_NOT_TODAY;
	}

	if ( info->startFlag == PROMOTION_HAVE_FINISH )
	{
		return PROMOTION_HAVE_FINISH;
	}

	if ( pMainPlayer->getLv() < data.m_nLevel )
	{
		return PROMOTION_LEVEL_LIMIT;
	}
	if( GetDynActiveInfo( nRowID ) < 0 )
	{
		if( getRepeatCount(nRowID) != 0 && getFinishCount(nRowID) >= getRepeatCount(nRowID) )
		{
			return PROMOTION_HAVE_FINISH;
		}
	}else{
		if( GetDynActiveInfo( nRowID ) == ACTIVE_STATUS_INIT )
		{
			return PROMOTION_NOT_TODAY;
		}else if( GetDynActiveInfo( nRowID ) == ACTIVE_STATUS_END )
		{
			return PROMOTION_HAVE_FINISH;
		}
	}
	if( !data.m_bShowInAutoTeam || data.m_nAutoTeamID <= 0 )
	{
		return PROMOTION_NOT_NEED_AUTOTEAM;
	}
	if( data.m_nNeedTeam == PROMOTION_SINGLE && pMainPlayer->isInTeam() )
	{
		return PROMOTION_SINGLE_LIMIT;
	}
	return PROMOTION_OK;
}

int GamePromotion::getAutoTeamStateByNoCountLimit( int nRowID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagPromotionDate& data = m_mapPromotionTime[nRowID];
	if( !data.m_bShowInAutoTeam || data.m_nAutoTeamID <= 0 )
	{
		return PROMOTION_NOT_NEED_AUTOTEAM;
	}
	if( data.m_nNeedTeam == PROMOTION_SINGLE && pMainPlayer->isInTeam() )
	{
		return PROMOTION_SINGLE_LIMIT;
	}

	PromotionStartInfo* info = getCurrPromitionStartInfo(nRowID);
	if( info->startFlag == PROMOTION_COMING )
	{
		return PROMOTION_NOT_START;
	}

	if( info->startFlag == PROMOTION_NOTTODAY )
	{
		return PROMOTION_NOT_TODAY;
	}

	if ( info->startFlag == PROMOTION_HAVE_FINISH )
	{
		return PROMOTION_HAVE_FINISH;
	}

	if ( pMainPlayer->getLv() < data.m_nLevel )
	{
		return PROMOTION_LEVEL_LIMIT;
	}

	return PROMOTION_OK;
}

void GamePromotion::setPworldLimit(int pworldid,int num)
{
	m_pWorldmap[ pworldid ] = num;
	GameManager::getSingleton().SendUIEvent("GE_PROMOTION_UPDATE");
}

void GamePromotion::promotionClearLimit()
{
	m_pWorldmap.clear();
	GameManager::getSingleton().SendUIEvent("GE_PROMOTION_UPDATE");
}

const char* GamePromotion::getDisPlayName( int nRow )
{
	std::map<int, tagPromotionDate>::iterator iter = m_mapPromotionTime.find( nRow );
	if ( iter != m_mapPromotionTime.end() )
	{
		if( iter->second.m_nFlag == PROMITION_FLAG_PROMITION 
			&& iter->second.m_bShowInAutoTeam && iter->second.m_nAutoTeamID != 0 )
		{
			const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef( iter->second.m_nRelateId );
			if( diffPworldDef != NULL )
			{
				if( diffPworldDef->PworldInfo[1].PworldID != 0 )
				{
					int pworldId = PworldDifficulty::getSingleton().getDifficultyPworldId( iter->second.m_nRelateId,MAX_DIFF_PWORLD_LIST-1 );
					const tagPworldDef_ToLua* pWorld = getPworldDef( pworldId );
					if( pWorld != NULL  )
					{
						return pWorld->Name;
					}
				}
			}
		}
		return iter->second.m_szName;
	}
	return "";
}

bool GamePromotion::isFinishActive( int nRow )
{
	if( GetDynActiveInfo(nRow) < 0 )
	{
		if( getRepeatCount( nRow ) > 0 )
		{
			return getFinishCount( nRow ) >= getRepeatCount( nRow );
		}		
	}else if( GetDynActiveInfo(nRow) == ACTIVE_STATUS_END )
	{
		return true;
	}
	return false;
}

void GamePromotion::addDailyGoalTask( int taskID )
{
	m_vecDailyTask.push_back( taskID );
}

void GamePromotion::SetDailyInfo( const CSACTIONDAILYGOALINFO& info )
{
	m_DailyInfo = info;
	ResetCanApplyTask();
	sortDailyGoalList();
	GameManager::getSingleton().SendUIEvent("GE_PROMOTION_DAILY_INFO_CHG");
}

void GamePromotion::SetDynActiveInfo( const CSACTIONDYNACTIVEINFO& info )
{
	m_mapDynActiveInfo[info.ActiveHelpID] = info;
	GameManager::getSingleton().SendUIEvent("GE_PROMOTION_UPDATE");
}

int GamePromotion::GetDynActiveInfo( int nRow )
{	
	std::map<int,CSACTIONDYNACTIVEINFO>::iterator it = m_mapDynActiveInfo.find( m_mapPromotionTime[nRow].m_nID );
	if( it != m_mapDynActiveInfo.end() )
	{
		return it->second.Stauts;
	}
	return -1;
}

void GamePromotion::ResetCanApplyTask()
{
	m_vecNeedShowDailyTask.clear();
	std::list<int> lstTmp;

	for( size_t i = 0 ; i < m_vecDailyTask.size() ; i++ )
	{
		const tagTaskDef* task = GameQuest::getSingleton().getTaskDef( m_vecDailyTask[i] );
		if( task != NULL && task->TaskGetLimit.GetLevelMin <= m_DailyInfo.MinLevel && task->TaskGetLimit.GetLevelMax >= m_DailyInfo.MinLevel )
		{
			lstTmp.push_back( m_vecDailyTask[i] );
		}
	}
	lstTmp.sort();

	for( int i = 0 ; i < GameQuest::getSingleton().getNumQusetRun(); i ++ )
	{
		int taskId = GameQuest::getSingleton().getQuestRunId( i );		
		const tagTaskDef* task = GameQuest::getSingleton().getTaskDef( taskId );
		if( task != NULL && task->Type == TASK_TYPE_DAILY_GOAL )
		{
			std::list<int>::iterator it = std::find( lstTmp.begin(),lstTmp.end(),taskId );
			if( it != lstTmp.end() )
			{
				lstTmp.erase( it );
			}
			lstTmp.push_front( taskId );
		}
	}
	TASKONE_ITER it = GameQuest::getSingleton().getTaskOnedayInfoBegin();
	for( ; it != GameQuest::getSingleton().getTaskOnedayInfoEnd(); it ++ )
	{
		int taskId = it->second.TaskID;
		const tagTaskDef* task = GameQuest::getSingleton().getTaskDef( taskId );
		if( task != NULL && task->Type == TASK_TYPE_DAILY_GOAL )
		{
			std::list<int>::iterator it = std::find( lstTmp.begin(),lstTmp.end(),taskId );
			if( it != lstTmp.end() )
			{
				lstTmp.erase( it );
			}
			lstTmp.push_front( taskId );
		}
	}
	m_vecNeedShowDailyTask.resize( lstTmp.size() );
	std::copy( lstTmp.begin(),lstTmp.end(),m_vecNeedShowDailyTask.begin() );
 	m_vecNeedShowDailyTask.resize(std::unique(m_vecNeedShowDailyTask.begin(),m_vecNeedShowDailyTask.end()) - m_vecNeedShowDailyTask.begin() );
	if( m_vecNeedShowDailyTask.size() > 8 )
	{
		m_vecNeedShowDailyTask.resize(8);
	}
}

int GamePromotion::GetDailyFiniNum()
{
	return m_DailyInfo.TaskNum;
}

int GamePromotion::GetDailyAwardFlag()
{
	return m_DailyInfo.TakeAwardFlag;
}

int GamePromotion::getDailyTaskNum()
{
	return (int) m_vecNeedShowDailyTask.size();
}

int GamePromotion::getDailyTaskID( int nIndex )
{
	return m_vecNeedShowDailyTask[nIndex];
}

void GamePromotion::RequestGetDailyAward( int nIndex )
{
	tagCSPkgBody msgbody;
	msgbody.ActionReq.ActionID = ACTION_TAKE_DAILY_GOAL_AWARD;
	msgbody.ActionReq.Data.TakeDailyGoalAward.TakeAward = nIndex;
	SendGameServerMsg(ACTION_REQ, msgbody);
}

tagDailyGoalAward_ToLua* GamePromotion::getDailyGoalAward( int id )
{
	static_assert( sizeof(tagDailyGoalAward_ToLua) == sizeof(tagDailyGoalAward) );
	return (tagDailyGoalAward_ToLua*) DBDataManager::getSingleton().getDailyGoalAward(id);
}

void GamePromotion::Update()
{
	MainGamePlayer *	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	//活动助手等级 15
	if ( pMainPlayer == NULL  )
	{
		return;
	}
	if ( pMainPlayer->isDead() || pMainPlayer->getLv() < 15 )
	{
		return;
	}
	std::map<int,bool>::iterator iter;
	for( iter = m_mapActivityGuid.begin();iter != m_mapActivityGuid.end(); ++iter )
	{	
		if ( iter->first == PROMOTION_DARK_COMING_ID && GameManager::getSingleton().GetLevelFeng() <= 59 )
		{
			continue;
		}
		PromotionStartInfo* info = getCurrPromitionStartInfo( iter->first );
		if ( iter->second && info->startFlag == PROMOTION_RUNING )
		{
			if ( pMainPlayer->getLv() <  getPromotionData( iter->first )->m_nLevel )
			{
				continue;
			}
			char buff[256];
			sprintf( buff,"SetSpecialActivityGuid( %d )",iter->first );
			GameManager::getSingleton().getScriptVM()->callString(buff);
			iter->second = false;
		}
		else if ( !iter->second && info->startFlag != PROMOTION_RUNING )
		{
			iter->second = true;
			char buff[256];
			sprintf( buff,"CloseSpecialActivityGuid( %d )",iter->first );
			GameManager::getSingleton().getScriptVM()->callString(buff);
			continue;
		}
	}
	if( m_nPinJiuBlurStartTime > 0 )
	{
		unsigned int nCurtime = timeGetTime();
		unsigned int nPassSec = ( nCurtime - m_nPinJiuBlurStartTime ) / 1000;
		if( nPassSec > 2 )
		{
			Ogre::BloomEffect::getSingleton().setGaussBlur( false );
			m_nPinJiuBlurStartTime = 0;
		}
	}
}

void GamePromotion::RequestTransportToNPC( int nActiveID )
{
	tagCSPkgBody msgbody;
	msgbody.ActiveClt.Cmd = ACTIVE_CLT_OP_TRANS;
	msgbody.ActiveClt.Data.Trans.ActiveID = nActiveID;
	SendGameServerMsg(ACTIVE_CLT, msgbody);
}

void GamePromotion::resetActivityGuidInfo()
{
	m_mapActivityGuid[7] = true;  //欢乐嘉年华
	m_mapActivityGuid[23] = true; //黑暗来袭
	m_mapActivityGuid[30] = true; //魔影重重
	m_mapActivityGuid[32] = true; //牛仔很忙-决赛
	m_mapActivityGuid[42] = true; //光影之战
	m_mapActivityGuid[45] = true; //机械改造
	m_mapActivityGuid[48] = true; //末世荣耀殿堂
	//m_mapActivityGuid[54] = true; //生命在于运动
	m_mapActivityGuid[64] = true; //坦克大战
}


const tagWorldBossDef_ToLua* GamePromotion::getWorldBossDefByIndex( int nIndex )
{
	assert( nIndex < (int)m_vecWorldBossData.size() );
	return (const tagWorldBossDef_ToLua *)(m_vecWorldBossData[nIndex]);
}

const tagWorldBossDef_ToLua* GamePromotion::getWorldBossDef( int id )
{
	assert( sizeof(tagWorldBossDef) == sizeof(tagWorldBossDef_ToLua) );
	const tagWorldBossDef* worldBossDef = DBDataManager::getSingleton().getWorldBossDef( id );
	if ( worldBossDef != NULL )
	{
		return (tagWorldBossDef_ToLua *)worldBossDef;
	}
	return NULL;
}

void GamePromotion::requestWorldBossInfo()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_WORLD_BOSS_INFO_REQ;
	SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
}

int GamePromotion::getDieFreshMonsterID( int id )
{
	std::vector<tagWorldBossDef*>::iterator iter = m_vecWorldBossData.begin();
	for ( ; iter != m_vecWorldBossData.end(); ++iter)
	{
		if ( (*iter)->DieFreshMonID == id )
		{
			return (*iter)->MonID;
		}
	}
	return 0;
}

int GamePromotion::GetWeekDay( int nTime )
{
	time_t localt	= (time_t)( nTime );
	struct tm *localTM	=  localtime( &localt );
	return localTM->tm_wday;
}

bool GamePromotion::isBeachPromotion( int nProId )
{
	return nProId == PROMOTION_BEACH_ID;
}

void GamePromotion::setPinJiuBlurTime( unsigned int tickTime )
{
	m_nPinJiuBlurStartTime = tickTime;
}