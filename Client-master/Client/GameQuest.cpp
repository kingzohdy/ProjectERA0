#include "GameQuest.h"
#include "GameDBData.h"
#include "resdb_meta.h"
#include <algorithm>
#include "GameManager.h"
#include "GameUIMsg.h" 
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "GameNpc.h"
#include "OgreEntity.h"
#include "GameActionManager.h"
#include "GamePlayManager.h"
#include "GameWizard.h"
#include "GameSkill_Mgr.h"
#include "Item.h"
#include "Container.h"
#include "OgreScriptLuaVM.h"
#include "ServerTimer.h"
#include "GameMaxMap.h"
#include "GameTrust.h"
#include "GameClientOpt.h"
#include "GameDesignation.h"
#include "GameRespact.h "
#include <time.h>
#include <bitset>
#include "SecondarySkillMgr.h"
#include "OgreLog.h"
#include "GameCollect.h"
#include "GamePromotion.h"
#include "GameClanManager.h"
#include "GameVipManager.h"

const int MAX_TRACE_SIZE = 32;
const std::string strQuestEffect[] = {
	"9507_16",	// 可领取(!)
	"9508_16",	// 可提交(?)
	"9509_16",	// 不可提交(?)
	"9514_16",	// 循环任务可领取(!)	
	"9515_16",	// 不可接任务(!)
	"9549_16",	// 福利
};
const std::string strQuestType[] = {
	"新手",		// CLIENT_TASK_TYPE_NEW 新手任务
	"主线",		// CLIENT_TASK_TYPE_MAIN 主线任务
	"支线",		// CLIENT_TASK_TYPE_BRANCH 支线任务
	"教学",		// CLIENT_TASK_TYPE_BUSSINESS 经商任务
	"活动",		// CLIENT_TASK_TYPE_ACT 活动任务
	"日常",		// CLIENT_TASK_TYPE_REP 重复任务
	"黄金",		// CLIENT_TASK_TYPE_GOLD 黄金任务
	"副本",		// CLIENT_TASK_TYPE_PWORLD 副本任务
	"探索",		// CLIENT_TASK_TYPE_DISCOVER 探索任务
	"护送",	    // CLIENT_TASK_TYPE_SINGLE_CONVOY 单人护送任务
	"护送",	    // CLIENT_TASK_TYPE_TEAM_CONVOY 队伍护送任务
	"公会",     // CLIENT_TASK_TYPE_CLAN 公会任务 
	"活动",		// CLIENT_TASK_TYPE_TRUST 委托任务
	"转职",		// CLIENT_TASK_TYPE_CHG_CAREER 转职任务
	"坐骑",		// CLIENT_TASK_TYPE_RIDE 坐骑任务
	"精灵",		// CLIENT_TASK_TYPE_FAIRY 精灵任务
	"科技",		// CLIENT_TASK_TYPE_LIFE 生活任务
	"环组",		// CLIENT_TASK_TYPE_CHG_RECYCLE 环组任务
	"今日目标",		// CLIENT_TASK_TYPE_DAILY_GOAL 今日目标任务
	"赏金",		//CLIENT_TASK_TYPE_SJ_MON, 	/* 赏金剿灭令 */
	"赏金",	//CLIENT_TASK_TYPE_SJ_ITEM, 	/* 赏金收集令令 */
	"赏金",		//CLIENT_TASK_TYPE_SJ_BOSS, 	/* 赏金击杀令 */
	"商运",		//CLIENT_TASK_TYPE_DART_CAR		/* 商运镖车任务 */
	"称号",		//CLIENT_TASK_TYPE_DESIGNATION			/* 称号任务 */
	"宝鉴",		//CLIENT_TASK_TYPE_RONGLU, 	/* 熔炉任务 */
	"协助",		//CLIENT_TASK_TYPE_XIEZHU,	/* 协助任务 */	
	"战场",		//CLIENT_TASK_TYPE_BATTLE,	/* 战场任务 */
	"跑商",		//CLIENT_TASK_TYPE_PAOSHANG,	/* 跑商任务 */
};
const std::string STR_ID_PARSE_FORMAT			= "#%s%d#";
const std::string STR_GET_NUM_PARSE_FORMAT		= "%d/%d";
const std::string STR_FINISH_COLOR				= "#cFEBC42";
const std::string STR_KILL_NUM_STANDARD_FORMAT	= "#cFEBC42%d/%d#n";
const std::string STR_TASK_PROGRESS				= "#Progress#";

bool operator < ( const CSTASKRUNONE& lhs, const CSTASKRUNONE& rhs)
{
	return lhs.TaskID < rhs.TaskID;
}

// 设置已完成任务项的颜色
void setFinishColor( char *pszAllText, const char* szConvertedIDText )
{
	std::string strAllText( pszAllText );
	size_t nPos = strAllText.find( szConvertedIDText );
	if ( nPos != -1)
	{
		std::string strSubMonsterDesc = strAllText.substr( 0, nPos );
		size_t nFiniPos = strSubMonsterDesc.rfind( "#r" ) ;
		if ( nFiniPos != -1 )
		{
			nFiniPos += 2;
		}else{
			nFiniPos = strSubMonsterDesc.rfind("\n");
			nFiniPos = nFiniPos + 1;
		}

		strAllText.insert( nFiniPos, STR_FINISH_COLOR );
		assert( (strAllText.length()+1) <= SHORT_DESC_LEN );
		strncpy( pszAllText, strAllText.c_str(), strAllText.length()+1 );
	}
}

void strFindAndReplace( char *pszText, const char* szConvertedIDText, const char* szConvertedKillNumText )
{
	std::string strAllText( pszText );

	size_t nPos = strAllText.find( szConvertedIDText );
	if ( nPos != -1)
	{
		strAllText.replace( nPos, strlen( szConvertedIDText ), szConvertedKillNumText );
		strncpy( pszText, strAllText.c_str(), strAllText.length() + 1 );
	}
}

// 解析怪物击杀数目
void parseMonsterKillMon( int monsterId, int num, int maxnum, char* szConvertedDataDefDes )
{
	// 解析格式#Monster怪物ID#
	char szConvertedIDText[DLG_INPUT_LEN];
	char szConvertedKillNumText[DLG_INPUT_LEN];
	// STR_ID_PARSE_FORMAT = "#%s%d#";
	sprintf( szConvertedIDText, STR_ID_PARSE_FORMAT.c_str(),"Monster", monsterId );
	// STR_GET_NUM_PARSE_FORMAT = "%d/%d";
	sprintf( szConvertedKillNumText, STR_GET_NUM_PARSE_FORMAT.c_str(), num, maxnum );

	if ( maxnum != 0 && num == maxnum )
	{
		sprintf( szConvertedKillNumText, STR_KILL_NUM_STANDARD_FORMAT.c_str(), num, maxnum );
		setFinishColor( szConvertedDataDefDes, szConvertedIDText );
	}

	strFindAndReplace( szConvertedDataDefDes, szConvertedIDText, szConvertedKillNumText );
}

// 解析格式任务进度
void parseTaskProgress( int stat, int num, int maxnum, char* szDes )
{
	char szNumText[DLG_INPUT_LEN] = "";

	if (stat != TASK_STAT_FAIL)
	{
		if( maxnum != 0 && num == maxnum )
		{
			sprintf( szNumText, STR_KILL_NUM_STANDARD_FORMAT.c_str(), num, maxnum );
			setFinishColor( szDes, STR_TASK_PROGRESS.c_str() );
		}
		else
		{
			sprintf( szNumText, STR_GET_NUM_PARSE_FORMAT.c_str(), num, maxnum );
		}
	}

	strFindAndReplace( szDes, STR_TASK_PROGRESS.c_str(), szNumText );
}

// 解析物品收集数目
void parseItemGet( int itemId, int num, int maxnum, char* szDes )
{
	/*
	解析格式#Item物品ID#
	*/
	if ( num > maxnum ) num = maxnum;

	char szIDText[DLG_INPUT_LEN];
	char szNumText[DLG_INPUT_LEN];
	sprintf( szIDText, STR_ID_PARSE_FORMAT.c_str(),"Item", itemId );
	sprintf( szNumText, STR_GET_NUM_PARSE_FORMAT.c_str(), num, maxnum );

	if( maxnum != 0 && num == maxnum )
	{
		sprintf( szNumText, STR_KILL_NUM_STANDARD_FORMAT.c_str(), num, maxnum );
		setFinishColor( szDes, szIDText );
	}

	strFindAndReplace( szDes, szIDText, szNumText );
}

// 解析任务完成度
void paresQusetPerson( int questState, char* szDes )
{
	/*
	解析格式#Mission#
	*/
	char szConvertedIDText[DLG_INPUT_LEN];
	char szNumText[DLG_INPUT_LEN];
	sprintf( szConvertedIDText, "#%s#", "Mission" );

	if ( questState == CLIENT_TASK_STAT_FINI )
	{
		sprintf( szNumText, STR_GET_NUM_PARSE_FORMAT.c_str(), 1, 1 );
	}
	else
	{
		sprintf( szNumText, STR_GET_NUM_PARSE_FORMAT.c_str(), 0, 1 );
	}

	strFindAndReplace( szDes, szConvertedIDText, szNumText );
}

//解析出位置
std::string getQuestNpcPos(std::string szTips)
{
	int i=0;
	while ( i < szTips.length() - 1 )
	{
		if ( szTips[i] == '@' && szTips[i+1] == '@')
		{
			int nposStart = i+2;
			int nposEnd;
			nposEnd = szTips.find("#n", nposStart);
			szTips = szTips.substr( nposStart, nposEnd - nposStart);
			return szTips;
		}
		i++;
	}
	return "";
}

GameQuest::GameQuest(void):m_nTrustArea( 0 ),m_bTraceAll(true), m_ulTraceData( 0 )
{
	INetworkManager::getSingleton().RegisterMsgHandler( TASK_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( SJ_TASK_SVR, this );
	// TODO:策划改数据库临时注释
	DBDataManager::getSingleton().loadTaskDef( m_TaskDef ); 
	loadRecycleTaskPreTaskFixIndex();
	setTrustAreaType( MAP_TYPE_ALL );
	setTrustArea( MAP_AREA_BIRTH );

	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_TASKTRACE, sizeof( m_ulTraceData ) );
	GameMsgPrint::getSingleton().registGameMsgHandler( this );
}

GameQuest::~GameQuest(void)
{
}


// 获取任务
void GameQuest::getQuest()
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType							= TASK_INFO;
	msgbody.TaskClt.TaskCltData.GetTaskInfo			= 0;
	SendGameServerMsg( TASK_CLT, msgbody );
}

// 获取任务
void GameQuest::acceptQuest( unsigned int npcId, int questId )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType							= TASK_ACCEPT;
	msgbody.TaskClt.TaskCltData.TaskAccept.NpcID	= npcId;
	msgbody.TaskClt.TaskCltData.TaskAccept.TaskID	= questId;
	SendGameServerMsg( TASK_CLT, msgbody );
}

// 角色领取奖励
void GameQuest::awardQuest( unsigned int npcId, int questId, int selectId )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType									= TASK_AWARD;
	msgbody.TaskClt.TaskCltData.GetTaskAward.NpcID			= npcId;
	msgbody.TaskClt.TaskCltData.GetTaskAward.TaskID			= questId;
	msgbody.TaskClt.TaskCltData.GetTaskAward.SelectItemID	= selectId;
	SendGameServerMsg( TASK_CLT, msgbody );
}

// 角色放弃任务
void GameQuest::dropQuest( int questId )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType						= TASK_DROP;
	msgbody.TaskClt.TaskCltData.TaskDrop.TaskID	= questId;
	SendGameServerMsg( TASK_CLT, msgbody );
}

// 杀怪或者收集完成后结束任务
void GameQuest::finishQuest( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if(	&taskDef != NULL && ( taskDef.AutoFini != 0 || taskDef.FiniTaskArea.MapID > 0 ) )
	{
		tagCSPkgBody msgbody;
		msgbody.TaskClt.OpType						= TASK_FINI;
		msgbody.TaskClt.TaskCltData.TaskFini.TaskID	= questId;
		SendGameServerMsg( TASK_CLT, msgbody );
	}
}

//活动助手完成任务
void GameQuest::finishActiveQuest( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.Type == TASK_TYPE_DAILY_GOAL )
	{
		tagCSPkgBody msgbody;
		msgbody.TaskClt.OpType						= TASK_AWARD;
		msgbody.TaskClt.TaskCltData.GetTaskAward.TaskID = questId;
		msgbody.TaskClt.TaskCltData.GetTaskAward.SelectItemID = 0;
		msgbody.TaskClt.TaskCltData.GetTaskAward.NpcID = 0;
		SendGameServerMsg( TASK_CLT, msgbody );
	}
}

// 共享任务
void GameQuest::shareQuest( int questId )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType						= TASK_SHARE;
	msgbody.TaskClt.TaskCltData.TaskFini.TaskID	= questId;
	SendGameServerMsg( TASK_CLT, msgbody );
}

// 拒绝任务
void GameQuest::refuseQuest( int questId )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType						= TASK_REFUSE;
	msgbody.TaskClt.TaskCltData.TaskFini.TaskID	= questId;
	SendGameServerMsg( TASK_CLT, msgbody );
}

void GameQuest::OnCommitTask( const CSTASKRUNONE& oneChgTaskData )
{
	const tagTaskDef& taskDef	= DBDataManager::getSingleton().getTaskDef( oneChgTaskData.TaskID );

	// 任务完成
	delQuestRun( oneChgTaskData.TaskID );
	// 添加环任务的完成次数
	if( &taskDef != NULL )
	{
		if ( isRecycleTask( taskDef )  )
		{
			updateRecycleIdxByCommit( taskDef, oneChgTaskData.TaskCycleIdx, oneChgTaskData.CycleID );
			updateRecycleOneDayInfoByCommit( taskDef, oneChgTaskData.TaskCycleIdx, oneChgTaskData.CycleID );
		}
		else
		{
			//addQusetNumOneDay( oneChgTaskData.TaskID );
		}
	}
}

void GameQuest::OnDropTask( const CSTASKRUNONE& oneChgTaskData )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( oneChgTaskData.TaskID );
	if ( taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_ITEM || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
	{
		//赏金任务组计数添加，无论提交或者放弃都计数
		addQusetNumOneDay( oneChgTaskData.TaskID );
		AddGroupQuestWeekNum( oneChgTaskData.TaskID );
	}

	// 任务放弃
	delQuestRun( oneChgTaskData.TaskID );
	// 添加环任务的完成次数
	if ( &taskDef == NULL || !isBelongToRecycleTask( taskDef, oneChgTaskData.CycleID ) )
	{
		return;
	}

	updateRecycleIdxByDrop( taskDef, oneChgTaskData.TaskCycleIdx, oneChgTaskData.CycleID );
	updateRecycleOneDayInfoByDrop( taskDef, oneChgTaskData.CycleID );
}

void GameQuest::OnUpdateTaskState( const CSTASKRUNONE& oneChgTaskData )
{
	CSTASKRUNONE* pQuest = getQuestRun( oneChgTaskData.TaskID );
	if ( oneChgTaskData.TaskStat == TASK_STAT_FAIL )
	{
		if( pQuest == NULL )
		{
			return;
		}
		SetConvoyInfo( false, oneChgTaskData.TaskID );		
	}
	else if ( oneChgTaskData.TaskStat == TASK_STAT_RUN )
	{
		if ( pQuest != NULL )
		{
			showKillMonsterInfo( oneChgTaskData, *pQuest );
		}
		SetConvoyInfo( true, oneChgTaskData.TaskID );
	}

	if( pQuest != NULL )
	{
		// 若该变化的任务是已经接过的，则更新任务数据
		*pQuest = oneChgTaskData;
	}
	else
	{
		// 获取任务
		//PrintQuestState( oneChgTaskData.TaskID, oneChgTaskData.TaskStat, "TASK_CHG--460" );
		m_QuestRuns.push_back( oneChgTaskData );
		std::sort( m_QuestRuns.begin(), m_QuestRuns.end() );
		
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		m_PreTime[oneChgTaskData.TaskID] = timeGetTime();
		if( m_bTraceAll && getQuestType( oneChgTaskData.TaskID ) != TASK_TYPE_DAILY_GOAL )
		{
			addQuestTrace( oneChgTaskData.TaskID );
		}

		const tagTaskDef& taskDef	= DBDataManager::getSingleton().getTaskDef( oneChgTaskData.TaskID );
		assert( &taskDef != NULL );
		if ( &taskDef == NULL )
		{
			return;
		}

		Container& containerMgr		= pMainPlayer->getContainer();
		for ( int i = 0; i < MAX_TASK_ITEM; i++ )
		{
			int nCurHaveItemCount = containerMgr.getItemCount( taskDef.GetItem[i].ItemID );
			m_PreItemCount[taskDef.GetItem[i].ItemID] = nCurHaveItemCount;
		}
		
		if ( isBelongToRecycleTask( taskDef, oneChgTaskData.CycleID ) )
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( oneChgTaskData.CycleID );
			m_QuestRecycleIdx[oneChgTaskData.CycleID] = oneChgTaskData.TaskCycleIdx;
			tagCSTaskRecycleInfo info;
			info.TaskRecycleID = oneChgTaskData.CycleID;
			info.TaskID = oneChgTaskData.TaskID;
			info.TaskCycleIdx = oneChgTaskData.TaskCycleIdx;
			info.TaskRecycleGetTime = getServerTime().sec;
			m_TaskRecycleInfo[oneChgTaskData.CycleID] = info;
		}
	}
}

void GameQuest::OnUpdateCollectTaskState( CSTASKRUNONE* pOneRunQuest )
{
	const tagTaskDef& taskDef	= DBDataManager::getSingleton().getTaskDef( pOneRunQuest->TaskID );
	// 判断击杀怪物和物品获取是否完成
	if ( taskDef.KillMon[0].Num == 0 && taskDef.GetItem[0].ItemID == 0 )
	{
		return;
	}
	
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	bool bFiniItem = true;
	if( taskDef.GetItem[0].ItemID != 0 )
	{
		Container& containerMgr = pMainPlayer->getContainer();
		for( int i = 0; i < MAX_TASK_ITEM; i++ )
		{
			const TASKGETITEM& oneTaskGetItem = taskDef.GetItem[i];
			if ( oneTaskGetItem.Num == 0 )
			{
				continue;
			}
			
			int nCurHaveItemCount = containerMgr.getItemCount( oneTaskGetItem.ItemID );
			if ( nCurHaveItemCount < (int)oneTaskGetItem.Num )
			{
				bFiniItem = false;
				break;
			}
		}
	}

	bool bFiniKillMon = true;
	if( taskDef.KillMon[0].Num > 0 )
	{
		const TASKMONTRACE& taskMonTraceInfo =  pOneRunQuest->TaskMonTrace;
		for ( int i = 0; i < taskMonTraceInfo.TaskMonTraceNum; ++i )
		{
			const TASKMONTRACEONE& taskMonTraceOne =taskMonTraceInfo.TaskKillMons[i];
			if ( taskMonTraceOne.MaxNum == 0 )
			{
				continue;
			}

			if( taskMonTraceOne.Num < taskMonTraceOne.MaxNum )
			{
				bFiniKillMon = false;
				break;
			}
		}
	}

	if( bFiniKillMon && bFiniItem )
	{
		if( isQuestTimeLimit( pOneRunQuest->TaskID )  )
		{
			if( pOneRunQuest->TaskEndTime > 0 )
			{
				pOneRunQuest->TaskStat = CLIENT_TASK_STAT_FINI;
			}
			else
			{
				pOneRunQuest->TaskStat = CLIENT_TASK_STAT_FAIL;
			}
		}
	}
}

void GameQuest::OnTaskChg( const CSTASKRUNONE& oneChgTaskData )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return;
	}

	if ( oneChgTaskData.TaskStat == TASK_STAT_COMMIT )
	{
		OnCommitTask( oneChgTaskData );
	}
	else if( oneChgTaskData.TaskStat == TASK_STAT_DROP )
	{
		OnDropTask( oneChgTaskData );
	}
	else
	{
		OnUpdateTaskState( oneChgTaskData );
	}

	CSTASKRUNONE* pOneRunQuest = getQuestRun( oneChgTaskData.TaskID );
	if( pOneRunQuest != NULL )
	{
		OnUpdateCollectTaskState( pOneRunQuest );
		if( pOneRunQuest->TaskStat == CLIENT_TASK_STAT_FINI )
		{
			const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( pOneRunQuest->TaskID );
			// 只有非探索任务才需要向服务器发TASK_FINI请求 探索任务已经在update发过
			if( taskDef.FiniTaskArea.MapID <= 0 )
			{
				finishQuest( pOneRunQuest->TaskID );
			}

			//公会情报任务在update里也执行了一遍，这边又处理了一遍,提示重复,
			//这种类型的任务是不是要特别处理?
			if( pOneRunQuest->TaskID != 3681 && pOneRunQuest->TaskID != 3682 )
			{
				char szBuf[256];
				sprintf( szBuf, "QusetFinish(\"%s\")", taskDef.Name );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
		}
	}

	updateNpcQuestTitle();
	GamePromotion::getSingleton().ResetCanApplyTask();

	GameWizard::getSingleton().updateQuestNpc( oneChgTaskData.TaskStat, oneChgTaskData.TaskID, oneChgTaskData.CycleID );
	m_chgTaskInfo.m_nTaskID		= oneChgTaskData.TaskID;
	m_chgTaskInfo.m_nTaskState	= oneChgTaskData.TaskStat;
	GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
}

int GameQuest::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{

	if( msgid == TASK_SVR )
	{
		const CSTASKSVR& questSeverMsg = msgbody.TaskSvr;

		switch( questSeverMsg.OpType )
		{
		case TASK_INFO:
			{
				m_QuestRuns.clear();
				m_timeAccumQuestRuns.clear();
				m_QuestTrace.clear();
				m_TrustOneDayInfo.clear();
				m_FiniQuest.clear();
				m_PreTime.clear();
				m_RecycleOneDayInfo.clear();
				m_GroupOneDayInfo.clear();
				m_GroupWeekInfo.clear();
				m_QuestRecycleIdx.clear();
				m_mapDiscoverInfo.clear();
				m_TaskRecycleInfo.clear();
				m_FinishedDynamicGetTasks.clear();
				std::bitset<MAX_TRACE_SIZE> tracedata = m_ulTraceData;
				const CSTASKINFO& allTaskData = questSeverMsg.TaskSvrData.GetTaskInfo;
				for( int i = 0; i < allTaskData.TaskRunNum; i++ )
				{
					const CSTASKRUNONE& oneRunTaskData = allTaskData.TaskRuns[i];
					
					// 加入任务列表
					//PrintQuestState( oneRunTaskData.TaskID, oneRunTaskData.TaskStat, "TASK_INFO--321" );
					m_QuestRuns.push_back( oneRunTaskData );

					// 加入任务追踪列表
					if( !tracedata[MAX_TRACE_SIZE -1] )
					{
						m_QuestTrace.push_back( oneRunTaskData.TaskID );
						m_bTraceAll	= true;
					}
					// 设置任务的开始时间
					m_PreTime[oneRunTaskData.TaskID] = timeGetTime();
				}
				std::sort( m_QuestRuns.begin(), m_QuestRuns.end() );
				
				//updateNpcQuestTitle();
				for( int i = 0; i < allTaskData.TaskRunNum; i++ )
				{
					const CSTASKRUNONE& oneRunTaskData = allTaskData.TaskRuns[i];
					GameWizard::getSingleton().updateQuestNpc( oneRunTaskData.TaskStat, oneRunTaskData.TaskID, oneRunTaskData.CycleID );
					SetConvoyInfo( true, allTaskData.TaskRuns[i].TaskID );
				}

				for( int i = 0; i < allTaskData.TaskRecycleNum; i++ )
				{
					int nTaskRecycleId = allTaskData.TaskRecycleInfo[i].TaskRecycleID;
					m_TaskRecycleInfo[nTaskRecycleId] = allTaskData.TaskRecycleInfo[i];
					// 设置任务的当前进行的环
					m_QuestRecycleIdx[nTaskRecycleId] = allTaskData.TaskRecycleInfo[i].TaskCycleIdx;
				}

				// 获取任务一天完成次数
				for( int i = 0; i < allTaskData.TaskOneDayNum; i++ )
				{
					int nTaskId = allTaskData.TaskOneDayInfo[i].TaskID;
					m_TrustOneDayInfo[nTaskId] = allTaskData.TaskOneDayInfo[i];
				}

				// 获取环组任务的进行的环信息
				for( int i = 0; i < allTaskData.TaskCycleOneDay.TaskCycleNum; i++ )
				{
					int nCycleId = allTaskData.TaskCycleOneDay.TaskCycleOneDayInfo[i].TaskCycleID;
					m_RecycleOneDayInfo[nCycleId] = allTaskData.TaskCycleOneDay.TaskCycleOneDayInfo[i];
				}

				// TODO:获取组任务的完成次数
				for( int i = 0; i< allTaskData.GroupOneDay.GroupNum; i++ )
				{
					int nGroupId = allTaskData.GroupOneDay.GroupOneDayInfo[i].GroupID;
					m_GroupOneDayInfo[nGroupId] = allTaskData.GroupOneDay.GroupOneDayInfo[i];
				}
				// 获取组任务完成次数
				for( int i = 0; i< allTaskData.GroupWeek.GroupNum; i++ )
				{
					int nGroupId = allTaskData.GroupWeek.GroupWeekInfo[i].GroupID;
					m_GroupWeekInfo[nGroupId] = allTaskData.GroupWeek.GroupWeekInfo[i];
				}

				// 获取已完成任务列表，委托也包含在里面
				for( int i = 0; i < allTaskData.TaskFiniNum; i++ )
				{
					m_FiniQuest.push_back( allTaskData.TaskFinis[i] );
				}

				const HIDETASKINFO&  hideTaskInfo = allTaskData.HideTaskInfo;
				setDynamicGetTask( hideTaskInfo.HideTaskID, hideTaskInfo.HideTaskNum );

				if( tracedata[MAX_TRACE_SIZE -1] )
				{
					loadTaskTraceOpt();
				}

				updateAllTaskState( false );
				update(0);
				// 刷新委托信息
				reSetTrust( 0 );
				showCollectItemInfo( false );
				updateNpcQuestTitle();
				updateCanAcceptQuest();
				GamePromotion::getSingleton().ResetCanApplyTask();
				GameManager::getSingleton().SendUIEvent( "GE_QUEST_GETQUESTINFO" );
			}
			break;
		case TASK_CHG:
			{
				OnTaskChg( questSeverMsg.TaskSvrData.GetTaskChg );
			}
			break;
		case TASK_DIALOG:
			{
				int questId						= questSeverMsg.TaskSvrData.TaskDialog.TaskID;
				const CSTASKDIALOG& questDialog = questSeverMsg.TaskSvrData.TaskDialog;
				const tagTaskDef&	taskDef		= DBDataManager::getSingleton().getTaskDef( questId );
				tagUIMsgBody Msg;
				tagQuestTalk& questTalkInfo		= Msg.QuestTalk;
				switch( questDialog.Type)
				{
				case TASK_DIALOG_NOFINI_TIP:
					{
						questTalkInfo.m_nNpcId	    = questDialog.NpcID;
						questTalkInfo.m_nQuestId	= questId;
						questTalkInfo.m_nRecycleID	= questDialog.CycleID;
						strncpy( questTalkInfo.DlgLast, taskDef.TaskNoFiniDialog, sizeof(questTalkInfo.DlgLast) );
						GameUIMsg::getSingleton().setUIMsg( Msg );
						GameManager::getSingleton().SendUIEvent( "GE_QUEST_DIALOG_NOFINI" );
					}
					break;
				case TASK_DIALOG_ACCEPT:
					{
						questTalkInfo.m_nNpcId	    = questDialog.NpcID;
						questTalkInfo.m_nQuestId	= questDialog.TaskID;
						questTalkInfo.m_nRecycleID	= questDialog.CycleID;
						strncpy( questTalkInfo.DlgLast, taskDef.TaskGetDialog, sizeof(questTalkInfo.DlgLast) );
						GameUIMsg::getSingleton().setUIMsg( Msg );
						GameManager::getSingleton().SendUIEvent( "GE_QUEST_DIALOG_ACCEPT" );
					}
					break;
				case TASK_DIALOG_AWARD:
					{
						questTalkInfo.m_nNpcId		= questDialog.NpcID;
						questTalkInfo.m_nQuestId	= questId;
						questTalkInfo.m_nRecycleID	= questDialog.CycleID;
						strncpy( questTalkInfo.DlgLast, taskDef.TaskFiniDialog, sizeof(questTalkInfo.DlgLast) );
						GameUIMsg::getSingleton().setUIMsg( Msg );
						GameManager::getSingleton().SendUIEvent( "GE_QUEST_DIALOG_AWARD" );
					}
					break;
				case TASK_DIALOG_ACCEPT_TIP:
					{
						questTalkInfo.m_nNpcId		= questDialog.NpcID;
						questTalkInfo.m_nQuestId	= questDialog.TaskID;
						questTalkInfo.m_nRecycleID	= questDialog.CycleID;
						GameUIMsg::getSingleton().setUIMsg( Msg );
						GameManager::getSingleton().SendUIEvent( "GE_QUEST_DIALOG_ACCEPTTIP" );
					}
					break;
				}
			}
			break;

		case TASK_TIMER:
			{
				const CSTASKTIMER& taskTimer = questSeverMsg.TaskSvrData.TaskTimer;
				tagTimeAccumQuestInfo* pTimeAccumQuest = getTimeAccumQuest( taskTimer.TaskID );
				if ( pTimeAccumQuest != NULL )
				{
					pTimeAccumQuest->m_nMaxTime		= taskTimer.Time;
					pTimeAccumQuest->m_nPassTime	= 0;
					pTimeAccumQuest->m_nTimeMode	= taskTimer.TimerMode;
				}
				else
				{
					tagTimeAccumQuestInfo timeAccumQuest;
					timeAccumQuest.m_nMaxTime	= taskTimer.Time;
					timeAccumQuest.m_nTaskID	= taskTimer.TaskID;
					timeAccumQuest.m_nTimeMode	= taskTimer.TimerMode;
					timeAccumQuest.m_nPassTime	= 0;
					m_timeAccumQuestRuns.push_back( timeAccumQuest );
				}
				m_PreTime[taskTimer.TaskID] = timeGetTime();
				
				GameManager::getSingleton().SendUIEvent( "GE_QUEST_TIMECHANGE" );
			}
			break;
		case TASK_ITEM_RECYCLE_UI:
			{
				const CSTASKITEMRECYCLEUI& recycleInfo = questSeverMsg.TaskSvrData.ItemRecycleUI;
				char szBuf[256];
				sprintf( szBuf, "ShowRecycleTaskByItem(%d,%d,%d,%d)", recycleInfo.GridIdx, recycleInfo.ListType, recycleInfo.TaskID, recycleInfo.TaskRecycleID );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;

		default:
			break;
		}
	}
	else if ( msgid == SJ_TASK_SVR )
	{
		const CSSJTASKSVR& sjSeverMsg = msgbody.SJTaskSvr;
		switch(sjSeverMsg.Type)
		{
		case SJ_TASK_CLT_TYPE_OPEN:
			{
				const SJTASKONEINFO& sjTaskInfo = sjSeverMsg.Data.TaskUIInfo.SJTaskInfo;
				char szBuf[256];
				sprintf( szBuf, "ShowBountyHunter(%d,%d,%d,%d"")", sjTaskInfo.Num, sjTaskInfo.SJID, sjTaskInfo.Star, sjTaskInfo.TaskID );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case SJ_TASK_SVR_TYPE_CLOSE:
			{
				const CSSJTASKCLOSE& sjTaskClose = sjSeverMsg.Data.TaskClose;
				GameManager::getSingleton().getScriptVM()->callString( "HideBountyHunter()" );
			}
			break;
		case SJ_TASK_SVR_ITEM_CHG:
			{
				int num = sjSeverMsg.Data.TaskItem.Num;
				char szBuf[256];
				sprintf( szBuf, "SetHasGetSJTaskItemNum(%d)", num );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

// 获取任务名称
const char* GameQuest::getQuestName( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.Name;
}

// 获取道具奖励类型
int GameQuest::getAwardType( int questId, int nRecycleID, int nMaxIdx )
{
	const tagTaskDef& taskDef	= DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD& taskAward	= taskDef.TaskAward; 
	if( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		if( pQuest != NULL )
		{
			
			return taskRecyDef.TaskRecycles[pQuest->TaskCycleIdx].TaskRecycleAward.ItemAwardType;
		}
		else
		{
			return taskRecyDef.TaskRecycles[nMaxIdx-1].TaskRecycleAward.ItemAwardType; 
		}
	}
	return taskAward.ItemAwardType;
}

// 获取可接任务所在场景
const char* GameQuest::getQusetScene( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.Scene;
}

// 获取任务类型
int GameQuest::getQuestType( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.Type;
}


//获取已接受赏金任务个数
int GameQuest::getSJTaskNum()
{
	int countNum = 0;
	for ( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(), end = m_QuestRuns.end(); iter != end; ++iter )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( iter->TaskID );
		if ( taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_ITEM || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
		{
			countNum++;
		}
	}
	return countNum;
}

// 获取任务难度
int GameQuest::getQuestDifficult( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.Difficult;
}

// 获取任务星级
int GameQuest::getQuestStar( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	for ( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(), end = m_QuestRuns.end(); iter != end; ++iter )
	{
		if ( taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_ITEM || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
		{
			return iter->SJTaskStar;
		}
	}
	return taskDef.StarLevel;
}

// 获取任务最低等级要求
int GameQuest::getQuestLevelMin( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskGetLimit.GetLevelMin;
}

// 任务描述
const char* GameQuest::getQuestDes( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskDesc;
}

// 任务对话
const char* GameQuest::getQuestDialog( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskGetDialog;
}

// 任务完成对话
const char* GameQuest::getQuestFiniDialog( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskFiniDialog;
}

// 获取当前任务的环数
int GameQuest::getTaskRecycleIdx( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( nRecycleID == 0 )
	{
		return 0;
	}

	std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
	if ( iter == m_QuestRecycleIdx.end() )
	{
		return 1;
	}

	return iter->second + 1;
}
int GameQuest::getTaskRecycleIdx( int nRecycleID )
{
	if ( nRecycleID == 0 )
	{
		return 0;
	}

	std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
	if ( iter == m_QuestRecycleIdx.end() )
	{
		return 1;
	}

	return iter->second + 1;
}
// 是否显示环组次数文字
bool GameQuest::isShowRecycleDesc( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( nRecycleID == 0 )
	{
		return false;
	}

	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if ( taskRecyDef.CycType == CLIENT_TASK_RECYCLE_NO_SHOW )
	{
		return false;
	}

	return true;
}

// 获取环组任务描述
const std::string GameQuest::getRecycleDes( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( nRecycleID == 0 )
	{
		return "";
	}

	std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
	int nRecycleIndex = 1;
	if ( iter != m_QuestRecycleIdx.end() )
	{
		nRecycleIndex = iter->second + 1;
	}
	
	int nTaskIndex = 1;
	CSTASKRUNONE* pOneRunTask = getQuestRun( questId );
	if ( pOneRunTask != NULL )
	{
		nTaskIndex = pOneRunTask->TaskCycleIdx + 1;
	}

	int nCycleNum = getReCycleQusetOneDayNum( nRecycleID ) + 1;
	char szDesc[256];
	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if ( taskRecyDef.CycType == CLIENT_TASK_RECYCLE_NO_SHOW )
	{
		return "";
	}

	sprintf( szDesc, "#c0181f9环组任务：#n \n【%s】 当前正在进行第%d环第%d个任务 \n", taskRecyDef.Name, nCycleNum, nRecycleIndex );
	std::string strDesc( szDesc, strlen( szDesc ) + 1 );
	return strDesc;
}


// 获取任务一天可以完成的次数
int GameQuest::getQuestMaxCanAcceptNumOneDay( int questId )
{
	const tagTaskDef& taskDef = *( getTaskDef( questId ) );
	return taskDef.TaskGetLimit.NumOneDay;
}

//获取任务地图id
unsigned int GameQuest::getQuestMapID( int questId )
{
	const tagTaskDef& taskDef = *( getTaskDef( questId ) );
	if ( taskDef.TaskFiniTraceDesc == NULL )
	{
		return 0;
	}
	return taskDef.TrustMapID;
}

// 获取任务完成次数
int GameQuest::getQusetHaveFinishNumOneDay( int questId )
{
	std::map<int, TASKONEDAYINFO>::iterator iter = m_TrustOneDayInfo.find( questId );
	if( iter != m_TrustOneDayInfo.end() )
	{
		return iter->second.OneDayNum;
	}

	return 0;
}

// 获取环组任务完成次数
int GameQuest::getReCycleQusetOneDayNum( int nCycleId )
{
	std::map<int, TASKCYCLEONEDAYINFO>::iterator iter = m_RecycleOneDayInfo.find( nCycleId );
	if( iter != m_RecycleOneDayInfo.end() )
	{
		return iter->second.OneDayNum;
	}

	return 0;
}

// 获取组任务完成次数
int GameQuest::getGroupQusetOneDayNum( int nGroupId )
{
	std::map<int, GROUPONEDAYINFO>::iterator iter = m_GroupOneDayInfo.find( nGroupId );
	if( iter != m_GroupOneDayInfo.end() )
	{
		const GROUPONEDAYINFO& oneGroupInfo = iter->second;
		return oneGroupInfo.OneDayNum;
	}

	return 0;
}

//获取组任务每周完成次数
int GameQuest::getGroupQuestWeekNum( int nGroupId )
{
	std::map<int, TASKGROUPWEEKINFO>::iterator iter = m_GroupWeekInfo.find( nGroupId );
	if( iter != m_GroupWeekInfo.end() )
	{
		const TASKGROUPWEEKINFO& oneGroupWeekInfo = iter->second;
		return oneGroupWeekInfo.WeekNum;
	}

	return 0;
}

// 添加任务完成次数
void GameQuest::addQusetNumOneDay( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	std::map<int, TASKONEDAYINFO>::iterator iter = m_TrustOneDayInfo.find( questId );
	if( iter != m_TrustOneDayInfo.end() )
	{
		iter->second.OneDayNum++;
	}
	else
	{
		if( taskDef.Redo != 0 && taskDef.TaskGetLimit.NumOneDay > 0 )
		{
			TASKONEDAYINFO oneDayInfo;
			oneDayInfo.TaskID		= questId;
			oneDayInfo.OneDayNum	= 1;
			// 获取服务器时间
			int sec, usec;
			ServerTimer::getSingleton().getServerTime( sec, usec );
			oneDayInfo.Time = sec;

			m_TrustOneDayInfo[questId] = oneDayInfo;
		}
	}

	if( taskDef.LimitGroupID > 0 )
	{
		std::map<int, GROUPONEDAYINFO>::iterator groupiter = m_GroupOneDayInfo.find( taskDef.LimitGroupID );
		if( groupiter != m_GroupOneDayInfo.end() )
		{
			GROUPONEDAYINFO& oneGroupInfo = groupiter->second;
			oneGroupInfo.OneDayNum++;
		}else{
			GROUPONEDAYINFO oneDayGroupInfo;
			oneDayGroupInfo.GroupID		= taskDef.LimitGroupID;
			oneDayGroupInfo.OneDayNum	= 1;

			// 获取服务器时间
			int sec, usec;
			ServerTimer::getSingleton().getServerTime( sec, usec );
			oneDayGroupInfo.Time = sec;
			m_GroupOneDayInfo[taskDef.LimitGroupID] = oneDayGroupInfo;
		}
	}
}

// 添加组任务每周完成次数
void GameQuest::AddGroupQuestWeekNum( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );

	if( taskDef.LimitGroupID > 0 )
	{
		std::map<int, TASKGROUPWEEKINFO>::iterator groupiter = m_GroupWeekInfo.find( taskDef.LimitGroupID );
		if( groupiter != m_GroupWeekInfo.end() )
		{
			TASKGROUPWEEKINFO& oneGroupWeekInfo = groupiter->second;
			oneGroupWeekInfo.WeekNum++;
		}else{
			TASKGROUPWEEKINFO oneWeekGroupInfo;
			oneWeekGroupInfo.GroupID		= taskDef.LimitGroupID;
			oneWeekGroupInfo.WeekNum		= 1;

			// 获取服务器时间
			int sec, usec;
			ServerTimer::getSingleton().getServerTime( sec, usec );
			oneWeekGroupInfo.Time = sec;
			m_GroupWeekInfo[taskDef.LimitGroupID] = oneWeekGroupInfo;
		}
	}
}

// 是否是环组任务
bool GameQuest::isRecycleTask( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return isRecycleTask( taskDef );
}

bool GameQuest::isRecycleTask( const tagTaskDef& taskDef )
{
	if ( &taskDef == NULL )
	{
		return false;
	}
	
	for ( int i = 0; i < MAX_TASK_TASKCYCLE; ++i )
	{
		if ( taskDef.TaskRecycleIDs[i] > 0 )
		{
			return true;
		}
	}

	return false;
}

// 当放弃任务的时候更新环组任务当前的环数
void GameQuest::updateRecycleIdxByDrop( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID )
{
	if ( &taskDef == NULL || !isBelongToRecycleTask( taskDef, nRecycleID ) )
	{
		return;
	}

	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );

	if( !taskRecyDef.OtherDayFlag )
	{
		std::map<int,CSTASKRECYCLEINFO>::iterator it = m_TaskRecycleInfo.find( nRecycleID );
		if( it != m_TaskRecycleInfo.end() )
		{
			if( !isSameDay( it->second.TaskRecycleGetTime ) )
			{
				m_QuestRecycleIdx[nRecycleID] = 0;
				m_TaskRecycleInfo.erase( it );
				return;
			}
		}
	}

	m_QuestRecycleIdx[nRecycleID] = nRecycleIdx;	

	if ( taskRecyDef.DropFlag == 1 )
	{
		m_QuestRecycleIdx[nRecycleID] = 0;
	}
}

// 当放弃任务的时候更新环组任务一天已经做的轮数
void GameQuest::updateRecycleOneDayInfoByDrop( const tagTaskDef& taskDef, int nRecycleID )
{
	if ( !isBelongToRecycleTask( taskDef, nRecycleID ) )
	{
		return;
	}
	
	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if ( &taskRecyDef == NULL || taskRecyDef.DropFlag == 0 )
	{
		return;
	}

	std::map<int,CSTASKRECYCLEINFO>::iterator it = m_TaskRecycleInfo.find( nRecycleID );
	if( it != m_TaskRecycleInfo.end() )
	{
		if( taskRecyDef.OtherDayFlag == 0 && !isSameDay( it->second.TaskRecycleGetTime ) )
		{
			return;
		}
	}

	std::map<int, TASKCYCLEONEDAYINFO>::iterator cycleiter = m_RecycleOneDayInfo.find( nRecycleID );
	if( cycleiter != m_RecycleOneDayInfo.end() )
	{
		cycleiter->second.OneDayNum++;
	}
	else
	{
		TASKCYCLEONEDAYINFO oneDayCycleInfo;
		oneDayCycleInfo.TaskCycleID = nRecycleID;
		oneDayCycleInfo.OneDayNum	= 1;

		oneDayCycleInfo.Time = getServerTime().sec;
		m_RecycleOneDayInfo[nRecycleID] = oneDayCycleInfo;
	}
}

// 当提交任务的时候更新环组任务当前的环数
void GameQuest::updateRecycleIdxByCommit( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID )
{
	if ( &taskDef == NULL || !isBelongToRecycleTask( taskDef, nRecycleID ) )
	{
		return;
	}

	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if( !taskRecyDef.OtherDayFlag )
	{
		std::map<int,CSTASKRECYCLEINFO>::iterator it = m_TaskRecycleInfo.find( nRecycleID );
		if( it != m_TaskRecycleInfo.end() )
		{
			if( taskRecyDef.OtherDayFlag == 0 && !isSameDay( it->second.TaskRecycleGetTime ) )
			{
				m_QuestRecycleIdx[nRecycleID] = 0;
				return;
			}
		}
	}
	m_QuestRecycleIdx[nRecycleID] = nRecycleIdx;
}

// 当提交任务的时候更新环组任务一天已经做的轮数
void GameQuest::updateRecycleOneDayInfoByCommit( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID )
{
	if ( !isBelongToRecycleTask( taskDef, nRecycleID ) )
	{
		return;
	}

	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if ( nRecycleIdx > 0 )
	{
		return;
	}
	std::map<int,CSTASKRECYCLEINFO>::iterator it = m_TaskRecycleInfo.find( nRecycleID );
	if( it != m_TaskRecycleInfo.end() )
	{
		if( taskRecyDef.OtherDayFlag == 0 && !isSameDay( it->second.TaskRecycleGetTime ) )
		{
			return;
		}
	}

	std::map<int, TASKCYCLEONEDAYINFO>::iterator cycleiter = m_RecycleOneDayInfo.find( nRecycleID );
	if( cycleiter != m_RecycleOneDayInfo.end() )
	{
		cycleiter->second.OneDayNum++;
	}
	else
	{
		TASKCYCLEONEDAYINFO oneDayCycleInfo;
		oneDayCycleInfo.TaskCycleID = nRecycleID;
		oneDayCycleInfo.OneDayNum	= 1;

		oneDayCycleInfo.Time = getServerTime().sec;

		m_RecycleOneDayInfo[nRecycleID] = oneDayCycleInfo;
	}
}

// 获取任务完成时间
int GameQuest::getQusetTime( int questId )
{
	std::map<int, TASKONEDAYINFO>::iterator iter = m_TrustOneDayInfo.find( questId );
	if( iter != m_TrustOneDayInfo.end() )
	{
		return iter->second.Time;
	}

	return 0;
}

// 00:00后重置当所有委托完成次数
void GameQuest::resetQusetNumOneDay()
{
	for( std::map<int, TASKONEDAYINFO>::iterator iter = m_TrustOneDayInfo.begin(); iter != m_TrustOneDayInfo.end(); ++iter )
	{
		iter->second.OneDayNum = 0;
	}
	for( std::map<int, TASKCYCLEONEDAYINFO>::iterator iter = m_RecycleOneDayInfo.begin();iter != m_RecycleOneDayInfo.end();iter ++ )
	{
		iter->second.OneDayNum = 0;
	}
	for( std::map<int, GROUPONEDAYINFO>::iterator iter = m_GroupOneDayInfo.begin();iter != m_GroupOneDayInfo.end();iter ++ )
	{
		iter->second.OneDayNum = 0;
	}
}

// 任务是否达到完成次数限制
bool GameQuest::isQuestHaveFinishOneDayMaxNum( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	// 非循环任务没有次数限制
	if( taskDef.Redo == 0 )
	{
		return false;
	}

	int nQuestMaxCanAcceptNum	= getQuestMaxCanAcceptNumOneDay( questId );
	int nQusetHaveFinishNum		= getQusetHaveFinishNumOneDay( questId );
	if( nQuestMaxCanAcceptNum <= 0 || nQusetHaveFinishNum < nQuestMaxCanAcceptNum )
	{
		return false;
	}

	return true;
}

// 重新组织委托任务数据
void GameQuest::reSetTrust( int nMapId )
{
	m_TrustMap.clear();

	for( std::vector<tagTaskDef*>::iterator iter = m_TaskDef.begin(); iter != m_TaskDef.end(); ++iter )
	{
		const tagTaskDef& oneTaskDef = *(*iter);
		// 若非委托任务
		if ( oneTaskDef.TrustFlag == 0 )
		{
			continue;
		}

		// 选定所有的区域，无需筛选
		if ( isAllTrustSelect() )
		{
			m_TrustMap.insert( std::make_pair( oneTaskDef.TaskGetLimit.GetLevelMin, oneTaskDef.ID ) );
			continue;
		}

		const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( oneTaskDef.TrustMapID ); 
		// 若选定某个区域的所有地图
		if ( nMapId == 0 )
		{
			if ( ( ( m_nTrustArea % 1000 ) & mapDef.MapArea ) != 0 )
			{
				m_TrustMap.insert( std::make_pair( oneTaskDef.TaskGetLimit.GetLevelMin, oneTaskDef.ID ) );
			}
			continue;
		} 

		// 若不符合地图大洲要求,将其过滤掉
		if ( ( ( m_nTrustArea % 1000 ) & mapDef.MapArea ) == 0 )
		{
			continue;
		}

		if ( oneTaskDef.TrustMapID == nMapId )
		{
			m_TrustMap.insert( std::make_pair( oneTaskDef.TaskGetLimit.GetLevelMin, oneTaskDef.ID ) );
		}

	}
}

// 设置委托任务区域
void GameQuest::setTrustArea( int nArea )
{
	int nTrustArea = m_nTrustArea%1000;
	if( nTrustArea & nArea )
	{	
		nTrustArea ^= nArea;
	}else{
		nTrustArea |= nArea;
	}
	m_nTrustArea = m_nTrustArea/1000*1000 + nTrustArea;
}

// 选择一个委托任务区域
void GameQuest::clearTrustArea( )
{
	m_nTrustArea = 0;
}

// 设置委托任务区域类型 1为副本 2 为野外
void GameQuest::setTrustAreaType( int nType )
{
	m_nTrustArea = nType*1000 + m_nTrustArea%1000;
}

// 是否是委托任务区域
bool GameQuest::IsTrustArea( int nArea )
{
	return (( m_nTrustArea % 1000 ) & nArea)==0?false:true;
}

// 是否是委托任务区域类型
bool GameQuest::IsTrustAreaType( int nType )
{
	int nAreaType = ( m_nTrustArea/1000 ) & nType; 
	if(  nAreaType == nType )
	{
		return true;
	}
	return false;
}

// 获取指定地图的委托任务数量
int	GameQuest::getMapTustNum( int nMapId )
{
	int nCount = 0;
	for ( std::vector<tagTaskDef*>::iterator iter = m_TaskDef.begin(), end = m_TaskDef.end(); iter != end; ++iter )
	{
		const tagTaskDef& oneTaskDef = *(*iter);
		if ( oneTaskDef.TrustMapID == nMapId )
		{
			++nCount;
		}
	}

	return nCount;
}


// 是否是委托任务
bool GameQuest::IsTrustQuest( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.TrustFlag )
	{
		return true;
	}
	return false;
}

// 获取委托任务数据
int GameQuest::getTrustPageInfo( int nPage )
{
	int nNum = 0;
	for( std::multimap<int, int>::iterator iter = m_TrustMap.begin(); iter != m_TrustMap.end(); ++iter )
	{
		if( nPage == nNum )
		{
			return iter->second;
		}
		nNum++;
	}

	return 0;
}

// 获取委托任务等级
int GameQuest::getQuestLvLimit( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskGetLimit.GetLevelMin;
}

// 获取任务需求总物品
int GameQuest::getQuestNeedItemNum( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	int nCount = 0;
	for( int i = 0; i < MAX_TASK_ITEM; i++ )
	{
		if( taskDef.GetItem[i].ItemID != 0 )
		{
			nCount++;
		}
	}
	return nCount;
}

// 获取任务需求物品
int GameQuest::getQuestNeedItem( int questId, int index )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.GetItem[index].ItemID;
}

// 获取任务需求物品单个数量
int GameQuest::getQuestNeedItemCount( int questId, int index )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.GetItem[index].Num;
}

// 获取委托任务数量
int GameQuest::getNumTrust()
{
	return (int)m_TrustMap.size();
}

void GameQuest::selectAllTrustTask( bool bSelect )
{
	m_bSelectAll = bSelect;
}

bool GameQuest::isAllTrustSelect()
{
	return m_bSelectAll;
}

// 获取任务奖励物品数量
int GameQuest::getQuestItemNum( int questId, int nRecycleID, bool bFinalAward )
{
	const tagTaskDef& taskDef	= DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD&  taskAward = taskDef.TaskAward; 
	int nCount = 0;
	
	if( nRecycleID > 0 )
	{
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		CSTASKRUNONE* pQuest	= getQuestRun( questId );
		int nRecycleIdx			= 0;
		// 若不是最后一环
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if( bFinalAward )
		{
			//nRecycleIdx = taskRecyDef.CycleNum-1;
			for( int i = 0; i < MAX_TASK_AWARD_ITEM; i++ )
			{
				if( taskRecyDef.FnshAwd.Items[i].ID != 0 )
				{
					nCount++;
				}
			}
			return nCount;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second/* + 1*/;
			}
		}

		for( int i = 0; i < MAX_TASK_AWARD_ITEM; i++ )
		{
			if( taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Items[i].ID != 0 )
			{
				nCount++;
			}
		}
		return nCount;
	}

	for( int i = 0; i < MAX_TASK_AWARD_ITEM; i++ )
	{
		const TASKITEMAWARD& taskAwardItem = taskAward.Items[i];
		if( taskAwardItem.ID != 0 )
		{
			nCount++;
		}
	}
	return nCount;
}

// 获取任务奖励物品
// bFinalAward为true 返回的是环组任务最终奖励的物品ID
int GameQuest::getQuestItem( int questId, int nRecycleID, int index, bool bFinalAward )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	if( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		if( taskRecyDef.CtrlFlag & CYC_CTRL_FLAG_LAST && getReCycleQusetOneDayNum(nRecycleID) > 0 )
		{
			return 0;
		}
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if( bFinalAward )
		{
			return taskRecyDef.FnshAwd.Items[index].ID;
			//nRecycleIdx = taskRecyDef.CycleNum-1;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second/* + 1*/;
			}
		}
		return taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Items[index].ID;
	}

	return taskAward.Items[index].ID;
}

// 获取环组任务的总环数
int GameQuest::getTaskCycleNum( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( isRecycleTask( taskDef ) )
	{
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		if ( &taskRecyDef == NULL )
		{
			return 0;
		}
		return taskRecyDef.CycleNum;
	}
	return 0;
}

// 获取任务奖励物品所需要职业
int GameQuest::getQuestItemCareer( int questId, int nRecycleID, int index )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	TASKAWARD taskAward = taskDef.TaskAward; 
	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		if( pQuest != NULL )
		{
			return 0;
		}
	}

	//对赏金任务处理
	if( taskAward.ItemAwardType == 4 )
	{
		const CSTASKRUNONE* one = getQuestRun(questId);
		if( one != NULL && one->SJTaskStar + 1 == taskAward.Items[index].Career )
		{
			return 0;
		}else{
			return -1;
		}
	}
	return taskAward.Items[index].Career;
}

bool GameQuest::isQuestItemCareer(int questId, int nRecycleID, int index, int race)
{
	int must = getQuestItemCareer(questId, nRecycleID, index);
	return must == 0 || race == must || (race & 0x0f) == must;
}

// 获取任务奖励物品数量
int GameQuest::getQuestItemCount( int questId, int nRecycleID, int index, bool bFinalAward )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	if( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if( bFinalAward )
		{
			return taskRecyDef.FnshAwd.Items[index].Num;//nRecycleIdx = taskRecyDef.CycleNum-1;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second/* + 1*/;
			}
		}
		return taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Items[index].Num;
	}

	return taskAward.Items[index].Num;
}

// 获取任务奖励给玩家的公会个人贡献度
int GameQuest::getQuestClanContribute( int questId, int nRecycleID, bool bFinalAward )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return NULL;
	}
	 
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if( bFinalAward )
		{
			return taskRecyDef.FnshAwd.Contri; // TODO: 目前环组任务只填了个贡献度
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second;
			}
		}
		
		return taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Contri;
	}
	
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	return taskAward.ClanOwnedContribute;
}

// 获取公会安定度奖励
int GameQuest::getQuestClanAnDing( int questId, bool bFinalAward )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( &taskDef == NULL )
	{
		return 0;
	}
	return taskDef.TaskAward.ClanAnDing;	
}

int GameQuest::getQuestClanResourceId(int questId, unsigned int index)
{
	if (index >= MAX_TASK_CLAN_RESOURCE)
	{
		return -1;
	}
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef(questId);
	if ( &taskDef == NULL )
	{
		return -1;
	}
	return taskDef.TaskAward.ClanResource[index].ResourceID;
}

int GameQuest::getQuestClanResourceNum(int questId, unsigned int index)
{
	if (index >= MAX_TASK_CLAN_RESOURCE)
	{
		return 0;
	}
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( &taskDef == NULL )
	{
		return 0;
	}
	return taskDef.TaskAward.ClanResource[index].Num;
}

// 获得当前环至少接取的任务数量
int GameQuest::getRecycleLeastTaskNum( int nRecycleID )
{
	const tagTaskRecycleDef& taskRecyDef	= DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	int nMaxCycleNum = taskRecyDef.CycleNum;
	if ( taskRecyDef.HideAccType == 0 )
	{
		return nMaxCycleNum;
	}
	
	const int MAX_RATE = 10000;
	const TASKRECYCLEENTRY& lastRecycleEntry = taskRecyDef.TaskRecycles[nMaxCycleNum-1];
	if ( lastRecycleEntry.Rate < MAX_RATE )
	{
		return nMaxCycleNum - 1;
	}
	else
	{
		return nMaxCycleNum;
	}
}

// 获得任务的当前环数
int GameQuest::getQuestRecycleIndex( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( &taskDef == NULL || nRecycleID == 0 )
	{
		return -1;
	}

	return getReCycleQusetOneDayNum( nRecycleID );
}

const CYCAWDRATE* GameQuest::getRecycleAwardInfo( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( nRecycleID == 0 )
	{
		return NULL; 
	}

	int nCycleNum = getReCycleQusetOneDayNum( nRecycleID );
	++nCycleNum;

	const tagTaskRecycleDef& taskRecyDef	= DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	for ( int i = 0; i < MAX_CYC_AWD_RATE; ++i )
	{
		const CYCAWDRATE& awardRateInfo = taskRecyDef.AwdRate[i];
		if ( nCycleNum >= awardRateInfo.StartCyc && nCycleNum <= awardRateInfo.EndCyc )
		{
			return &awardRateInfo;
		}
	}

	return NULL;
}

// 获取某环的奖励
int GameQuest::getRecycleTaskAwardRate( int questId, int nRecycleID )
{
	const CYCAWDRATE* pQuestRecycleAwardRate = getRecycleAwardInfo( questId, nRecycleID );
	if ( pQuestRecycleAwardRate == NULL )
	{
		return 0;
	}

	return pQuestRecycleAwardRate->CycAwdRate;
}

// 获取公会建设度奖励
int GameQuest::getQuestClanBuildingValue( int questId, int nRecycleID, bool bFinalAward )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return NULL;
	}

	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	int nClanBuildingValue = taskAward.ClanBuildingValue;
	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second;
			}
		}

		for ( int i = 0; i < MAX_CYC_AWD_RATE; ++i )
		{
			const CYCAWDRATE& awardRateInfo = taskRecyDef.AwdRate[i];
			if ( nRecycleIdx >= awardRateInfo.StartCyc && nRecycleIdx <= awardRateInfo.EndCyc )
			{
				nClanBuildingValue = nClanBuildingValue * awardRateInfo.TaskAwdRate / 100.0;
			}
		}
	}
	
	return nClanBuildingValue;
}

// 获取公会资金奖励
int GameQuest::getQuestClanMoney( int questId, int nRecycleID, bool bFinalAward )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return 0;
	}
	
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( &taskDef == NULL )
	{
		return 0;
	}
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	int nClanMoney = taskAward.ClanMoney;

	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if ( bFinalAward )
		{
			//return (tagTaskMoney_toLua*)&taskRecyDef.FnshAwd.Money;
			return nClanMoney;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second/* + 1*/;
			}
		}
		nClanMoney += taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.ClanMoney;
		const CYCAWDRATE* pCycAwardRate = getRecycleAwardInfo( questId, nRecycleID );
		if ( pCycAwardRate == 0 )
		{
			return nClanMoney;
		}
		nClanMoney = nClanMoney * ( pCycAwardRate->TaskAwdRate / 100.0 );
	}

	return nClanMoney;
}

// 获取公会科技点奖励
int GameQuest::getQuestClanTechPoint( int questId, bool bFinalAward )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return NULL;
	}

	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( &taskDef == NULL )
	{
		return 0;
	}

	const TASKAWARD& taskAward = taskDef.TaskAward; 
	return taskAward.ClanTechPoint;
}

// 是否是动态奖励
bool GameQuest::isDynamicAwardMoneyAndExp( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( taskDef.Type == TASK_TYPE_DYN /*|| taskDef.Type == TASK_TYPE_SINGLE_CONVOY || 
			taskDef.Type == TASK_TYPE_TEAM_CONVOY*/ || taskDef.Type == TASK_TYPE_DAILY_GOAL 
			|| taskDef.Type == TASK_TYPE_DART_CAR  || taskDef.Type == TASK_TYPE_BATTLE)
	{
		return true;
	}

	if( nRecycleID == 0 )
	{
		return false;
	}

	const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if ( taskRecyDef.CycType == CLIENT_TASK_RECYCLE_NO_SHOW )
	{
		return true;
	}

	if ( taskRecyDef.NoDynAward == 0 )
	{
		return true;
	}

	return false;
}

// 获取任务奖励金钱
tagTaskMoney_toLua* GameQuest::getQuestMoney( int questId, int nRecycleID, bool bFinalAward )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return NULL;
	}

	const tagTaskDef& taskDef		= DBDataManager::getSingleton().getTaskDef( questId );
	const tagLevelUP& LevelUpDef	= DBDataManager::getSingleton().getLevelUP( pMainPlayer->getLv() );
	static tagTaskMoney_toLua TaskMoney;

	if ( taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_ITEM || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagSJTaskDef_tolua*  sjTaskdef = getSJTaskDef( taskDef.SJID );
		int SJStar = 0;
		if ( pQuest != NULL )
		{
			SJStar = pQuest->SJTaskStar;
		}
		if ( SJStar > MAX_SJ_TASK_STAR || SJStar < 0 )
		{
			SJStar = 0;
		}
		int money = sjTaskdef->StarRate[SJStar].MoneyMul;
		int nType = taskDef.TaskAward.Money.Type;
		TaskMoney.Val = (unsigned int)money;
		TaskMoney.Type = (unsigned int)nType;
		return (tagTaskMoney_toLua*)&TaskMoney;
	}

	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if ( bFinalAward )
		{
			return (tagTaskMoney_toLua*)&taskRecyDef.FnshAwd.Money;
			//nRecycleIdx = taskRecyDef.CycleNum-1;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second/* + 1*/;
			}
		}

		if ( isDynamicAwardMoneyAndExp( questId, nRecycleID ) )
		{
			TaskMoney.Type	= taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Money.Type;
			const TASKRECYCLEENTRY& oneRecycleEntry = taskRecyDef.TaskRecycles[nRecycleIdx];
			TaskMoney.Val	= (int)( ( oneRecycleEntry.TaskRecycleAward.Money.MoneyRate ) * 
										( LevelUpDef.ContributeMoney / 10000.0 ) );
		}	
		else
		{
			TaskMoney.Type	= taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Money.Type;
			TaskMoney.Val	= (int)( taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Money.MoneyRate );
		}
		
		return (tagTaskMoney_toLua*)&TaskMoney;
	}
	else
	{
		if ( isDynamicAwardMoneyAndExp( questId, nRecycleID ) )
		{
			TaskMoney.Type	= taskDef.TaskAward.Money.Type;
			TaskMoney.Val	= ( unsigned int )taskDef.TaskAward.Money.Val * 
												LevelUpDef.ContributeMoney / 10000.0f;
			return (tagTaskMoney_toLua*)&TaskMoney;
		}
		else
		{
			return (tagTaskMoney_toLua*)&taskDef.TaskAward.Money;
		}
	}	
}

// 获取任务奖励经验
unsigned int GameQuest::getQuestExp( int questId, int nRecycleID, bool bFinalAward )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return 0;
	}

	const tagLevelUP& LevelUpDef	= DBDataManager::getSingleton().getLevelUP( pMainPlayer->getLv() );
	const tagTaskDef& taskDef		= DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD&  taskAward		= taskDef.TaskAward; 

	if ( taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_ITEM || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagSJTaskDef_tolua*  sjTaskdef = getSJTaskDef( taskDef.SJID );
		int SJStar = 0;
		if( pQuest != NULL )
		{
			SJStar = pQuest->SJTaskStar;
		}
		if ( SJStar > MAX_SJ_TASK_STAR || SJStar < 0 )
		{
			SJStar = 0;
		}
		unsigned int EXp = sjTaskdef->StarRate[SJStar].ExpMul;
		return EXp;
	}

	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		int nRecycleIdx = 0;
		if( !bFinalAward && pQuest != NULL )
		{
			nRecycleIdx = pQuest->TaskCycleIdx;
		}
		else if( bFinalAward )
		{
			//nRecycleIdx = taskRecyDef.CycleNum-1;
			return (unsigned int)taskRecyDef.FnshAwd.Exp.ExpRate;
		}
		else
		{
			std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleID );
			if( iter!= m_QuestRecycleIdx.end() )
			{
				nRecycleIdx = iter->second/* + 1*/;
			}
		}

		if ( isDynamicAwardMoneyAndExp( questId, nRecycleID ) )
		{
			return (unsigned int)( taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Exp.ExpRate * 
							( LevelUpDef.ContributeExp/10000.0 ));
		}
		else
		{
			return taskRecyDef.TaskRecycles[nRecycleIdx].TaskRecycleAward.Exp.ExpRate;
		}
	}
	{
		if ( isDynamicAwardMoneyAndExp( questId, nRecycleID ) )
		{
			return (unsigned int)( taskAward.Exp / 10000.0f *  LevelUpDef.ContributeExp  );
		}
		else
		{
			return taskAward.Exp;
		}
	}
	
}

// 获取任务奖励称号
const char* GameQuest::getAwardDesignation( int questId, int nRecycleID, bool bFinalAward )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.TaskAward.Designation != 0 )
	{
		return GameDesignation::getSingleton().getDesignationById( taskDef.TaskAward.Designation )->Name;
	}

	if( nRecycleID != 0 && bFinalAward )
	{
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		if( taskRecyDef.FnshDesignation != 0 )
		{
			return GameDesignation::getSingleton().getDesignationById( taskRecyDef.FnshDesignation )->Name;
		}
	}

	return "";
}

const char* GameQuest::getAwardSecondarySkillName(int questId)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.TaskAward.SecondarySkillType != 0 )
	{
		int nSecondarySkillLevel = taskDef.TaskAward.SecondarySkillLevel != 0 ? taskDef.TaskAward.SecondarySkillLevel : 1;
		return DBDataManager::getSingleton().getSecondarySkillDef( taskDef.TaskAward.SecondarySkillType,nSecondarySkillLevel )->NameDesc;
	}
	return "";
}

int GameQuest::getAwardSecondarySkillVal( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskAward.SecondarySkillVal;
}

void GameQuest::PrintQuestState( int nQuestID, int nQuestState, const char* pszSrcServeInfo )
{
	if ( nQuestID != 3642 )
	{
		return;
	}
	LOG_PRIVATE( "服务器消息 = %s, 任务ID = %d, 状态 = %d", pszSrcServeInfo, nQuestID, nQuestState );
}

// 获取任务奖励声望名
const char* GameQuest::getAwardRespactName( int questId, int nRecycleID, bool bFinalAward )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.TaskAward.Respact.ID != 0 )
	{
		return GameRespact::getSingleton().getRespactPartyDef( taskDef.TaskAward.Respact.ID )->PartyName;
	}
	if( nRecycleID != 0 && bFinalAward )
	{
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		if( taskRecyDef.FnshRespact.ID  != 0 )
		{
			return GameRespact::getSingleton().getRespactPartyDef( taskRecyDef.FnshRespact.ID )->PartyName;
		}
	}
	return "";
}

// 获取任务奖励声望值
unsigned int GameQuest::getAwardRespactValue( int questId, int nRecycleID, bool bFinalAward )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.TaskAward.Respact.ID != 0 )
	{
		return taskDef.TaskAward.Respact.Value;
	}

	if( nRecycleID != 0 && bFinalAward )
	{
		const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		if( taskRecyDef.FnshRespact.ID  != 0 )
		{
			return taskRecyDef.FnshRespact.Value;
		}
	}
	return 0;
}

// 获得任务奖励关键字属性
int GameQuest::getQuestFixedAttr( int questId, int nRecycleID, int index )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	if ( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		if( pQuest != NULL )
		{
			const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
			return taskRecyDef.TaskRecycles[pQuest->TaskCycleIdx].TaskRecycleAward.Items[index].ID;
		}
	}
	return taskAward.Items[index].FixedAttr;
}

// 获得任务奖励的装备绑定及孔属性
int GameQuest::getQuestBindingAndSlot( int questId, int nRecycleID, int index )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	const TASKAWARD& taskAward = taskDef.TaskAward; 
	if( nRecycleID > 0 )
	{
		CSTASKRUNONE* pQuest = getQuestRun( questId );
		if( pQuest != NULL )
		{
			const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
			return taskRecyDef.TaskRecycles[pQuest->TaskCycleIdx].TaskRecycleAward.Items[index].ID;
		}
	}
	return taskAward.Items[index].BindingAndSlot;
}

// 获取已接取任务数量
int GameQuest::getNumQusetRun()
{
	return (int)m_QuestRuns.size();
}

// 获取已接任务
int GameQuest::getQuestRunId( int index )
{
	assert( index < (int)m_QuestRuns.size() );
	return m_QuestRuns[index].TaskID;
}

// 该任务是否已经接取
bool GameQuest::isQuestRun( int id )
{
	for ( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(), end = m_QuestRuns.end(); iter != end; ++iter )
	{
		if ( iter->TaskID == id )
		{
			return true;
		}
	}

	return false;
}

// 删除累积计时任务
void GameQuest::delTimeAccumQuest( int questId )
{
	for ( std::vector<tagTimeAccumQuestInfo>::iterator iter = m_timeAccumQuestRuns.begin(), end = m_timeAccumQuestRuns.end();
		iter != end; ++iter )
	{
		if ( questId == iter->m_nTaskID )
		{
			m_timeAccumQuestRuns.erase( iter );
			return;
		}
	}
}	

// 删除已接取任务
void GameQuest::delQuestRun( int questId )
{
	for( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(); iter != m_QuestRuns.end(); ++iter )
	{
		if( iter->TaskID == questId )
		{
			iter = m_QuestRuns.erase( iter );
			delTimeAccumQuest( questId );
			delQuestTrace( questId );
			GameManager::getSingleton().SendUIEvent( "GE_QUEST_DELQUESTRUN" );
			break;
		}
	}
	SetConvoyInfo( false, questId );
}

// 获取已接取任务所属环组ID
int GameQuest::getQuestRunRecycleID( int questId )
{
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	if( pQuest != NULL )
	{
		return pQuest->CycleID;
	}

	return 0;
}

// 获取任务完成情况
int GameQuest::getQuestRunState( int questId )
{
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	if( pQuest != NULL )
	{
		return pQuest->TaskStat;
	}

	return CLIENT_TASK_STAT_NO;
}

// 获取已接任务要求描述
const char* GameQuest::getQuestRunReqDesc( int questId )
{
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	if( pQuest != NULL )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
		return taskDef.TaskReqDesc;
	}
	assert(0);
	return "";
}

// 获取任务完成后追踪描述
const char* GameQuest::getQuestFiniTraceDes( int questId )
{
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	if( pQuest != NULL )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
		return taskDef.TaskFiniTraceDesc;
	}
	assert(0);
	return "";
}

// 获取任务完成后追踪描述位置
std::string GameQuest::getQuestFiniTraceDespos( int questId )
{
	std::string pszTippos = getQuestFiniTraceDes( questId );
	return getQuestNpcPos( pszTippos );
}

//获取赏金未接任务目标描述
const char* GameQuest::getSJQuestTraceNoFiniTip( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( taskDef.TaskFiniTraceDesc == NULL )
	{
		return "";
	}
	return taskDef.TaskTraceNoFiniTip;
}
// 获取任务未完成任务追踪提示
// (注意：在任务查询面板中就算完成也是用该描述信息)
const char* GameQuest::getQuestNoFiniTraceDes( int questId )
{
	static char szTaskFiniTraceDesc[TASK_TRACE_NOFINI_DESC_LEN];
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	pQuest->TaskMonTrace.TaskMonTraceNum;
	if ( pQuest == NULL )
	{
		assert(0);
		return "";
	}

	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	strncpy( szTaskFiniTraceDesc, taskDef.TaskTraceNoFiniTip, sizeof(szTaskFiniTraceDesc) );
	if ( taskDef.KillMon[0].Num != 0 )
	{
		// 需要击杀怪物
		for( int i = 0; i < pQuest->TaskMonTrace.TaskMonTraceNum; i++ )
		//for( int i = 0; taskDef.KillMon[i].MonID1 != 0; i++ )
		{
			const TASKMONTRACEONE& oneKillMonRequest = pQuest->TaskMonTrace.TaskKillMons[i];
			parseMonsterKillMon(	oneKillMonRequest.MonID, 
			//parseMonsterKillMon(	taskDef.KillMon[i].MonID1, 
				oneKillMonRequest.Num,
				oneKillMonRequest.MaxNum, 
				szTaskFiniTraceDesc );
		}
	}

	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if ( pMainPlayer != NULL )
	{
		for( int i = 0; i < MAX_TASK_ITEM; i++ )
		{
			const TASKGETITEM& oneTaskItemRequest = taskDef.GetItem[i];
			if ( oneTaskItemRequest.ItemID == 0 )
			{
				continue;
			}
			
			Container& containerMgr = pMainPlayer->getContainer();
			parseItemGet(	oneTaskItemRequest.ItemID, 
							containerMgr.getItemCount( oneTaskItemRequest.ItemID ), 
							//oneTaskItemRequest.Num, szTaskFiniTraceDesc );
							pQuest->TaskItemTrace.TaskGetItems[i].MaxNum , szTaskFiniTraceDesc );
		}
	}

	if( taskDef.MaxGenericProgress != 0 )
	{
		parseTaskProgress(	pQuest->TaskStat, pQuest->TaskGenericProgress.Progress, 
			taskDef.MaxGenericProgress, 
			szTaskFiniTraceDesc );
	}

	paresQusetPerson( pQuest->TaskStat, szTaskFiniTraceDesc );

	return szTaskFiniTraceDesc;
}

// 获取任务未完成任务追踪提示
// (注意：在任务查询面板中就算完成也是用该描述信息)
std::string GameQuest::getQuestNoFiniTraceDespos( int questId )
{
	std::string pszTippos;
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	if ( pQuest == NULL )
	{
		assert(0);
		return "";
	}

	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	pszTippos = taskDef.TaskTraceNoFiniTip;
	return getQuestNpcPos(pszTippos);
}

// 刷新可接任务
void GameQuest::updateCanAcceptQuest()
{
	m_CanAcceptQuest.clear();
	for( size_t i = 0; i < m_TaskDef.size(); i++ )
	{
		int nOneTaskID = m_TaskDef[i]->ID;
		// 任务是否已经完成
		if( isQuestFini( nOneTaskID ) )
		{
			continue;
		}

		// 是否已接任务
		if( isHaveAcceptQuest( nOneTaskID ) )
		{
			continue;
		}

		if( canAcceptQuest( nOneTaskID ) != CLINT_CAN_ACCEPT_TASK )
		{
			continue;
		}

		m_CanAcceptQuest.push_back( nOneTaskID );
	}
	GameManager::getSingleton().SendUIEvent( "GE_QUEST_UPDATECANACCEPT" );
}

// 获取可接任务数量
int GameQuest::getNumCanAcceptQuest()
{
	return (int)m_CanAcceptQuest.size();
}

// 获取可接任务任务
int GameQuest::getCanAcceptQuestId( int index )
{
	assert( index < (int)m_CanAcceptQuest.size() );
	return m_CanAcceptQuest[index];
}

// 获取可接任务要求描述
const char* GameQuest::getCanAcceptQuestReqDesc( int questId )
{

	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();

	std::vector<int>::iterator iter = std::find( m_CanAcceptQuest.begin(), m_CanAcceptQuest.end(), questId );
	if( iter != m_CanAcceptQuest.end() )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
		return taskDef.TaskCanAcceptReq;
	}
	assert(0);
	return "";
}

// 获取可接任务要求描述位置
std::string GameQuest::getCanAcceptQuestReqDescpos( int questId )
{
	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();

	std::vector<int>::iterator iter = std::find( m_CanAcceptQuest.begin(), m_CanAcceptQuest.end(), questId );
	if( iter != m_CanAcceptQuest.end() )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
		std::string psztemp=taskDef.TaskCanAcceptReq;
		return getQuestNpcPos(psztemp);
	}
	assert(0);
	return "";
}

const char* GameQuest::getQuestReqDesc( int questId )
{
	const tagTaskDef* pTaskDef = &DBDataManager::getSingleton().getTaskDef( questId );
	if( pTaskDef != NULL )
	{
		return pTaskDef->TaskCanAcceptReq;
	}
	return "";
}

// 获取任务追踪数量
int  GameQuest::getNumQuestTrace()
{
	return (int)m_QuestTrace.size();
}

// 获取任务追踪
int GameQuest::getQuestTrace( int index )
{
	assert( index < (int)m_QuestTrace.size() );
	return m_QuestTrace[index];
}

// 获取任务追踪Tips提示
const char* GameQuest::getQuestRunTip( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.TaskTraceTip;
}

// 获取任务追踪对象的坐标
std::string GameQuest::getQuestRunTippos( int questId )
{
	std::string szTips=getQuestRunTip(questId);
	return getQuestNpcPos(szTips);
}

// 添加任务追踪
void GameQuest::addQuestTrace( int questId )
{
	std::vector<int>::iterator iter = std::find( m_QuestTrace.begin(), m_QuestTrace.end(), questId );
	if( iter == m_QuestTrace.end() )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
		if( taskDef.TrustFlag )
		{
			CSTASKRUNONE taskrunone;
			taskrunone.TaskID = questId;
			taskrunone.TaskStat = GameTrust::getSingleton().trust_TrustIsComplete( questId );
			taskrunone.TaskCycleIdx = 0;
			taskrunone.TaskEndTime = 0;
			//PrintQuestState( taskrunone.TaskID, taskrunone.TaskStat, "addQuestTrace--2125" );
			m_QuestRuns.push_back( taskrunone );
			std::sort( m_QuestRuns.begin(), m_QuestRuns.end() );
		}
		m_QuestTrace.push_back( questId );
		saveTaskTraceOpt();
		GameManager::getSingleton().SendUIEvent( "GE_QUEST_ADDQUESTTRACE" );
	}else{
		assert(0);
	}
}

// 删除任务追踪
void GameQuest::delQuestTrace( int questId )
{
	std::vector<int>::iterator iter = std::find( m_QuestTrace.begin(), m_QuestTrace.end(), questId );
	if( iter != m_QuestTrace.end() )
	{
		m_QuestTrace.erase( iter );
		saveTaskTraceOpt();
	}

	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.TrustFlag )
	{
		for( std::vector<CSTASKRUNONE>::iterator taskiter = m_QuestRuns.begin(); taskiter != m_QuestRuns.end(); ++taskiter )
		{
			if ( taskiter->TaskID == questId )
			{
				m_QuestRuns.erase( taskiter );
				break;
			}
		}
	}
	GameManager::getSingleton().SendUIEvent( "GE_QUEST_DELQUESTTRACE" );
}

// 检测任务是否被追踪
bool GameQuest::isQuestTrace( int questId  )
{
	return std::find( m_QuestTrace.begin(), m_QuestTrace.end(), questId ) != m_QuestTrace.end();
}

// 任务是否可以接取 0 不可接 1 可接 2 下一等级可接
int GameQuest::canAcceptQuest( int nQuestId )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return CLINT_CAN_NOT_ACCEPT_TASK;
	}
	int nLv			= pMainPlayer->getLv();
	int nGender		= pMainPlayer->getGender();
	int nRace		= pMainPlayer->getRace();
	
	// TODO: 看这个关键步骤是否有影响
	const tagTaskDef& taskDef = *( getTaskDef( nQuestId ) );
	//公会等级限制
	if( taskDef.ClanLevelLimit > GameClanManager::getSingleton().getClanViewer().m_clanInfo.m_nClanLevel )
	{
		return CLINT_CAN_NOT_ACCEPT_TASK;
	}
	
	// 若是动态接取任务且还不能接
	if ( !isFixGetTask( taskDef.ID ) )
	{
		if ( canGetDynamicTask( taskDef.ID ) )
		{
			return CLINT_CAN_ACCEPT_TASK;
		}
		else
		{
			return CLINT_CAN_NOT_ACCEPT_TASK;
		}
	}

	if( taskDef.TaskGetLimit.Gender != 0 && taskDef.TaskGetLimit.Gender != nGender )
	{
		return CLINT_CAN_NOT_ACCEPT_TASK;
	}

	if( taskDef.TaskGetLimit.Career != 0 && 
		taskDef.TaskGetLimit.Career != nRace && 
		( taskDef.TaskGetLimit.Career != (nRace & 0x0f) ) )
	{
		return CLINT_CAN_NOT_ACCEPT_TASK;
	}
	
	int nPreTaskID = taskDef.TaskGetLimit.PreTaskID;
	if ( nPreTaskID > 0 && !isQuestFini( nPreTaskID ) )
	{
		if ( taskDef.Type == CLIENT_TASK_TYPE_REP )
		{
			if ( !isQuestHaveFiniEver( nPreTaskID ) && !isQuestFini( nPreTaskID ) )
			{
				return CLINT_CAN_NOT_ACCEPT_TASK;
			}
		}
		else
		{
			if ( !isQuestFini( nPreTaskID ) )
			{
				return CLINT_CAN_NOT_ACCEPT_TASK;
			}
		}
	}

	if ( !pMainPlayer->isInClan() && taskDef.Type == CLIENT_TASK_TYPE_CLAN )
	{
		return CLINT_CAN_NOT_ACCEPT_TASK;
	}

	// 是否有完成次数限制
	if ( isQuestHaveFinishOneDayMaxNum( nQuestId ) )
	{
		return CLINT_CAN_NOT_ACCEPT_TASK;
	}

	// 任务是否有组限制
	if( taskDef.LimitGroupID > 0 )
	{
		const tagTaskLimitGroup& taskLimitGroup = DBDataManager::getSingleton().getTaskLimitGroup( taskDef.LimitGroupID );
		int num = 0;
		if ( GameVipManager::getSingleton().isEnableVip( STAR_STONE_JADE ) )
		{
			if ( taskDef.Type == CLIENT_TASK_TYPE_GOLD )
			{
				num = GameVipManager::getSingleton().getShangShanNum();
			}
			else if ( taskDef.Type == CLIENT_TASK_TYPE_RONGLU )
			{
				num = GameVipManager::getSingleton().getRongLuNum();
			}
		}
		if( getGroupQusetOneDayNum( taskDef.LimitGroupID ) >= taskLimitGroup.GroupOneDay + num )
		{
			return CLINT_CAN_NOT_ACCEPT_TASK;
		}

		for( size_t i = 0; i < m_QuestRuns.size(); i++ )
		{
			if( taskDef.IsGroupMutex == 0 )
			{
				break;
			}

			const tagTaskDef& runTaskDef = DBDataManager::getSingleton().getTaskDef( m_QuestRuns[i].TaskID );
			if( runTaskDef.IsGroupMutex && runTaskDef.LimitGroupID == taskDef.LimitGroupID )
			{
				return CLINT_CAN_NOT_ACCEPT_TASK;
			}
		}
	}

	if ( isRecycleTask( taskDef ) )
	{
		// TODO
		if ( !canAcceptRecycleTask( taskDef ) )
		{
			return CLINT_CAN_NOT_ACCEPT_TASK;
		}
	}

	if( taskDef.TaskGetLimit.SecondarySkillType != 0 )
	{
		if( taskDef.TaskAward.SecondarySkillLevel == 0 )
		{
			if( !SecondarySkillMgr::getSingleton().hasSecondarySkill( taskDef.TaskGetLimit.SecondarySkillType,1) )
			{
				return CLINT_CAN_NOT_ACCEPT_TASK;
			}
		}else{
			if( !SecondarySkillMgr::getSingleton().canLearnNextLevel( taskDef.TaskGetLimit.SecondarySkillType,taskDef.TaskGetLimit.SecondarySkillLevel)
				|| SecondarySkillMgr::getSingleton().isLearnedSecondarySkill( taskDef.TaskAward.SecondarySkillType,taskDef.TaskAward.SecondarySkillLevel))
			{
				return CLINT_CAN_NOT_ACCEPT_TASK;
			}
		}
	}else if( taskDef.TaskAward.SecondarySkillType != 0 && taskDef.TaskAward.SecondarySkillLevel == 1 )
	{
		if ( SecondarySkillMgr::getSingleton().getNonCommSecondaySkillNum() >= 2 
			|| SecondarySkillMgr::getSingleton().isLearnedSecondarySkill( taskDef.TaskAward.SecondarySkillType,
			taskDef.TaskAward.SecondarySkillLevel ) )
		{
			return CLINT_CAN_NOT_ACCEPT_TASK;
		}
	}


	if ((taskDef.TaskGetLimit.GetLevelMin != 0 && nLv < taskDef.TaskGetLimit.GetLevelMin) || 
		(taskDef.TaskGetLimit.GetLevelMax != 0 && nLv > taskDef.TaskGetLimit.GetLevelMax) )
	{
		// 差一级企划要求特殊显示
		if( taskDef.TaskGetLimit.GetLevelMin != 0  && 
			nLv < taskDef.TaskGetLimit.GetLevelMin &&
			taskDef.TaskGetLimit.GetLevelMin <= nLv + 4)  
		{
			return CLINT_CAN_NOT_ACCEPT_TASK_BY_LITTLE_LV;
		}
		else
		{
			return CLINT_CAN_NOT_ACCEPT_TASK;
		}
	}

	return CLINT_CAN_ACCEPT_TASK;
}

// 环组任务是否可接取
bool GameQuest::canAcceptRecycleQuest( int nPayerLv, int nRecycleId, int npcId  )
{
	const tagTaskRecycleDef& taskRecycleDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleId );
	// 环组任务组和类型限制
	if( &taskRecycleDef != NULL
		&& !isLimitByLv( taskRecycleDef, nPayerLv )
		&& !isLimitByGroup( taskRecycleDef ) 
		&& !isLimitByMutex( taskRecycleDef ) 
		&& !isLimitByOneDay( taskRecycleDef )
		&& !isLimitByClan( taskRecycleDef )
		&& !isLimitByRecycleIdx( nRecycleId, npcId )
		&& !isLimitByPreTaskID( taskRecycleDef )
		)
	{
		return true;
	}

	return false;
}

// 任务采集物是否可以采集
bool GameQuest::canCollectQuest(int nMonResId )
{
	if (!IsCollectHasItem(nMonResId))
	{
		return false;
	}
	const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef( nMonResId );
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	if ( MonsterDef.Collect.TaskID[0] != 0 )
	{
		bool bFind = false;
		for ( int i = 0; i < MON_COLLECT_TASK_NUM; i++ )
		{
			for( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(); iter != m_QuestRuns.end(); ++iter )
			{
				if( iter->TaskID != MonsterDef.Collect.TaskID[i] )
				{				
					continue;
				}

				const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( iter->TaskID );
				// 判断采集是否完成
				if( MonsterDef.Booty.Items[0].ItemID != 0 )
				{
					for( int i = 0; i < MAX_TASK_ITEM; i++ )
					{
						if( taskDef.GetItem[i].ItemID != 0 
							&& MonsterDef.Booty.Items[0].ItemID == taskDef.GetItem[i].ItemID
							&& pMainPlayer->getContainer().getItemCount( taskDef.GetItem[i].ItemID ) >= (int)taskDef.GetItem[i].Num )
						{
							return false;
						}
					}
				}

				if ( iter->TaskStat == CLIENT_TASK_STAT_FINI )
				{
					continue;
				}
				bFind = true;
			}
		}
		if( !bFind )
		{
			return false;
		}
	}

	if ( MonsterDef.Collect.CollectArm > 0 )
	{
		bool bFind = false;
		for( int i = 0; i < EQUIP_POS_END; i++ )
		{	
			Item&  itemCollectArm = pMainPlayer->getEquip().getItem(i);	
			if( itemCollectArm.getItemId() == 0 )
			{
				continue;
			}

			if ( itemCollectArm.getItemId() == MonsterDef.Collect.CollectArm )
			{
				if( itemCollectArm.getItemDur() > 0 && itemCollectArm.getItemDur() < MonsterDef.Collect.CollectArmDur )
				{
					continue;
				}
				bFind = true;
			}			
		}
		if( !bFind )
		{
			return false;
		}
	}

	if ( MonsterDef.Collect.CollectSkillID > 0)
	{
		if ( SkillManager::getSingleton().getSpellLv(MonsterDef.Collect.CollectSkillID) < MonsterDef.Level )
		{
			return false;
		}
	}

	if ( MonsterDef.Collect.CollectItem > 0 )
	{
		int nCollectItemDur = MonsterDef.Collect.CollectItemDur;
		if( nCollectItemDur == 0 )
		{
			nCollectItemDur = 1;
		}
		if( pMainPlayer->getContainer().getItemCount( MonsterDef.Collect.CollectItem ) < nCollectItemDur )
		{
			return false;
		}
	}
	return true;
}

// 显示怪物提示
void GameQuest::showKillMonsterInfo( const CSTASKRUNONE& newOneTaskData, const CSTASKRUNONE& oldOneTaskData )
{
	const TASKMONTRACE& newMonTrace = newOneTaskData.TaskMonTrace;
	const TASKMONTRACE& oldMonTrace = oldOneTaskData.TaskMonTrace;

	for ( int newIndex = 0, newSize = newMonTrace.TaskMonTraceNum; newIndex < newSize; ++newIndex )
	{
		const TASKMONTRACEONE& newOneMonTraceInfo = newMonTrace.TaskKillMons[newIndex];
		for ( int oldIndex = 0, oldSize = oldMonTrace.TaskMonTraceNum; oldIndex < oldSize; ++oldIndex )
		{
			const TASKMONTRACEONE& oldOneMonTraceInfo = oldMonTrace.TaskKillMons[oldIndex];
			if ( oldOneMonTraceInfo.MonID == newOneMonTraceInfo.MonID && oldOneMonTraceInfo.Num < newOneMonTraceInfo.Num )
			{
				char szBuf[256];
				sprintf( szBuf, "AddGameMiddleTips(\"[%s] %d/%d\")", getMonsterName( newOneMonTraceInfo.MonID ), 
							newOneMonTraceInfo.Num, newOneMonTraceInfo.MaxNum );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
				break;
			}
		}
	}
}

// 显示物品提示
void GameQuest::showCollectItemInfo( bool bShow )
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	for( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(); iter != m_QuestRuns.end(); ++iter )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( iter->TaskID );
		if( taskDef.GetItem[0].ItemID == 0 )
		{
			continue;
		}
		for( int i = 0; i < MAX_TASK_ITEM; i++ )
		{
			if( taskDef.GetItem[i].Num == 0 )
			{
				continue;
			}
			int nItemCount = pMainPlayer->getContainer().getItemCount( taskDef.GetItem[i].ItemID );
			if( nItemCount != 0 )
			{
				std::map<int, int>::iterator iterItemCount = m_PreItemCount.find( taskDef.GetItem[i].ItemID );
				if( iterItemCount == m_PreItemCount.end() || 
					( iterItemCount->second < nItemCount && nItemCount <= (int)taskDef.GetItem[i].Num ) )
				{
					if( bShow )
					{
						// 显示物品信息
						tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( taskDef.GetItem[i].ItemID );
						char szBuf[256];
						sprintf( szBuf, "AddGameMiddleTips(\"获得[%s] %d/%d\")", itemDef.Name, nItemCount, taskDef.GetItem[i].Num );
						GameManager::getSingleton().getScriptVM()->callString( szBuf );
					}
					m_PreItemCount[taskDef.GetItem[i].ItemID] = nItemCount;
				}
			}
		}	
	}
}

// 获取任务的超链接
bool GameQuest::getMissionLink( int nQuestId, int nRecycleID )
{
	if( nQuestId > 0 && ( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) ) )
	{
		GameMsgPrint::getSingleton().setMissionLink( nQuestId, nRecycleID );
		return true;
	}
	return false;	
}

// 检查是否是需要累积计时的任务
bool GameQuest::isTimeAccumQuest( int nTaskID )
{
	for ( std::vector<tagTimeAccumQuestInfo>::iterator iter = m_timeAccumQuestRuns.begin(), end = m_timeAccumQuestRuns.end();
		iter != end; ++iter )
	{
		if ( nTaskID == iter->m_nTaskID )
		{
			return true;
		}
	}

	return false;
}

// 任务是否限时任务
bool GameQuest::isQuestTimeLimit( int nQuestId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( nQuestId );
	if( taskDef.TaskTimer != 0 )
	{
		return true;
	}
	return false;
}

// 获取任务的剩余时间
unsigned int GameQuest::getTaskEndTime( int nQuestId )
{
	CSTASKRUNONE* pQuset = getQuestRun( nQuestId );
	if ( pQuset == NULL )
	{
		return 0;
	}
	unsigned int nTime = pQuset->TaskEndTime;
	return nTime;
}	

void GameQuest::updateOneDayQuest()
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	time_t sert			= (time_t)sec;
	struct tm *serTM	=  localtime( &sert );
	int nServeDay		= serTM->tm_mday;
	int nServeMon		= serTM->tm_mon;
	for( std::map<int, TASKONEDAYINFO>::iterator iter = m_TrustOneDayInfo.begin(); iter != m_TrustOneDayInfo.end(); ++iter )
	{
		time_t localt	= (time_t)(iter->second.Time);
		struct tm *localTM	=  localtime( &localt );

		if( nServeDay != localTM->tm_mday || nServeMon != localTM->tm_mon )
		{
			iter->second.OneDayNum = 0;
		}
	}

	for( std::map<int, TASKCYCLEONEDAYINFO>::iterator cycleiter = m_RecycleOneDayInfo.begin(); cycleiter != m_RecycleOneDayInfo.end(); ++cycleiter )
	{
		time_t localt	= (time_t)(cycleiter->second.Time);
		struct tm *localTM	=  localtime( &localt );

		if( nServeDay != localTM->tm_mday || nServeMon != localTM->tm_mon )
		{
			cycleiter->second.OneDayNum = 0;
		}
	}

	for( std::map<int, GROUPONEDAYINFO>::iterator groupiter = m_GroupOneDayInfo.begin(); groupiter != m_GroupOneDayInfo.end(); ++groupiter )
	{
		time_t localt	= (time_t)(groupiter->second.Time);
		struct tm *localTM	=  localtime( &localt );

		if( nServeDay != localTM->tm_mday || nServeMon != localTM->tm_mon )
		{
			groupiter->second.OneDayNum = 0;
		}
	}
}

void GameQuest::updateCollectActors()
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	GameActor* pActor = NULL;
	for ( int i = 0, size = actorMgr.getMonsterCount(); i < size; ++i )
	{
		pActor = actorMgr.getMonster( i );
		if( pActor->getType() != GAT_MONSTER || pActor->getSubType() != MON_SUB_TYPE_COLLECT ||
			pActor->m_actorCollectInfo.m_nRace != 0 )
		{
			continue;
		}

		if( SecondarySkillMgr::getSingleton().canCollect(pActor->getResId()) 
			&& canCollectQuest( pActor->getResId() ) && GameCollect::getSingleton().canCollect(pActor) && GameCollect::getSingleton().limitMachineCollectSimple(pActor->getResId()))
		{
			pActor->getShowActor()->getCurEntity()->playMotion( "9510_16", false, 9510 );
			pActor->enableCollect( true );
		}
		else
		{
			pActor->getShowActor()->getCurEntity()->stopMotion( 9510 );
			pActor->enableCollect( false );
		}
	}
}

void GameQuest::update( unsigned int dtick )
{
	updateCollectActors();

	updateAllTaskState( true );

// 	updateOneDayQuest();
}

void GameQuest::npcEnterView( int id )
{	
	MainGamePlayer* mainplayer = ActorManager::getSingleton().getMainPlayer();
	GameNPC* pNpc = (GameNPC*)ActorManager::getSingleton().FindActor( id );	
	if( pNpc != NULL )
	{
		if ( mainplayer && mainplayer->isInPVPMap() ) {
			if ( pNpc->getCampIdx() > 0 && mainplayer && mainplayer->getCampIdx() != pNpc->getCampIdx() ) {
				return;
			}
		}
		int nResID = pNpc->getResId();
		int nShow = getNpcQuestShow( nResID );
		if( nShow != -2 && nShow != pNpc->m_nQusetShow )
		{
			if( nShow != -1 )
			{
				pNpc->getShowActor()->getCurEntity()->stopMotion( 10000 );
				pNpc->getShowActor()->getCurEntity()->playMotion( strQuestEffect[nShow].c_str(), false, 10000 );
			}else{
				pNpc->getShowActor()->getCurEntity()->stopMotion( 10000 );
			}			
		}
		//处理进入公会地图时公会情报npc标识的显示
		if( pNpc->getMapId() == CLAN_CITY_MAP_ID && nResID == CLAN_INTELLINGENCE_NPC_ID )
		{
			MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			pNpc->getShowActor()->getCurEntity()->stopMotion( 9541 );
			if ( pMainPlayer && pMainPlayer->getClanID() == GameClanManager::getSingleton().getCurClanCityId() )
			{
				pNpc->getShowActor()->getCurEntity()->playMotion( "9541_16", false, 9541 );
			}
		}
	}	
}

// 到达指定区域自动完成任务
void GameQuest::updateAutoFiniQuest( CSTASKRUNONE& questRun, const tagTaskDef& taskDef )
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL || taskDef.FiniTaskArea.MapID == 0 )
	{
		return;
	}

	bool	bIsInArea = false;
	int		nPassTime = 0;
	std::map<int, tagDiscoverInfo>::iterator iter = m_mapDiscoverInfo.find( questRun.TaskID );
	if ( iter != m_mapDiscoverInfo.end() )
	{
		if( !isInArea( iter->second, taskDef, questRun.TaskID, questRun.TaskStat ) )
		{
			iter->second.m_nInAreaTime	= 0;
			iter->second.m_bFiniTask	= false;
		}
	}
	else
	{
		tagDiscoverInfo  discoverInfo;
		discoverInfo.m_bFiniTask	= false;
		discoverInfo.m_nInAreaTime	= 0;
		m_mapDiscoverInfo[questRun.TaskID]	= discoverInfo;
	}
}

CSTASKRUNONE* GameQuest::getQuestRun( int questId )
{
	for( size_t i = 0; i < m_QuestRuns.size(); i++ )
	{
		if( questId == m_QuestRuns[i].TaskID )
		{
			return &m_QuestRuns[i];
		}
	}
	return NULL;
}

// 通过环组ID获取已接取任务
CSTASKRUNONE* GameQuest::getQuestRunByRecycle( int recycleId )
{
	for( size_t i = 0; i < m_QuestRuns.size(); i++ )
	{
		const CSTASKRUNONE& oneRunQuest = m_QuestRuns[i];
		if( recycleId == oneRunQuest.CycleID )
		{
			return &m_QuestRuns[i];
		}
	}
	return NULL;
}

bool GameQuest::canAddFiniQuest( int questId )
{
	/*const tagTaskDef* pTaskDef = getTaskDefFromClientTaskDefTable( questId );
	if ( pTaskDef == NULL )
	{
		return false;
	}

	if ( pTaskDef->Flag )
	{
		return true;
	}

	if ( pTaskDef->Redo > 0 )
	{
		return false;
	}*/

	return true;
}

// 清空任务列表
void GameQuest::clearQuest()
{
	m_QuestRuns.clear();
	m_CanAcceptQuest.clear();
	m_QuestTrace.clear();
	m_timeAccumQuestRuns.clear();
	m_FinishedDynamicGetTasks.clear();
	m_ulTraceData = 0;
}

void GameQuest::saveTaskTraceOpt()
{
	std::bitset<MAX_TRACE_SIZE> tracedata;
	tracedata.reset();
	for( size_t i = 0; i < m_QuestRuns.size(); i++ )
	{
		for( size_t j = 0; j < m_QuestTrace.size(); j++ )
		{
			if( m_QuestRuns[i].TaskID == m_QuestTrace[j] )
			{
				assert( i < (MAX_TRACE_SIZE-2));
				tracedata[i] = true;
			}
		}
	}
	tracedata[MAX_TRACE_SIZE-2] = m_bTraceAll;
	tracedata[MAX_TRACE_SIZE-1] = true;
	m_ulTraceData				= tracedata.to_ulong();
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_TASKTRACE, (char*)&m_ulTraceData );
	
	m_QuestTrace.clear();
	for( size_t i = 0; i < m_QuestRuns.size(); i++ )
	{
		if( tracedata[i] )
		{
			m_QuestTrace.push_back( m_QuestRuns[i].TaskID );
		}
	}
}

void GameQuest::loadTaskTraceOpt()
{
	std::bitset<MAX_TRACE_SIZE> tracedata;
	tracedata = m_ulTraceData;
	for( size_t i = 0; i < m_QuestRuns.size(); i++ )
	{
		if( tracedata[i] )
		{
			m_QuestTrace.push_back( m_QuestRuns[i].TaskID );
		}
	}
	m_bTraceAll = tracedata[MAX_TRACE_SIZE-2];
}

void GameQuest::updateNpcQuestTitle( int nMapID, int nRealMapID )
{
	GameMapShower*	pMapShower	= GameManager::getSingleton().getGameMapShower();
	if ( nRealMapID >= GAME_PWORLD_MAPID )
	{
		for ( int i = 0, size = pMapShower->getMapNpcCount( nRealMapID ); i < size; ++i )
		{
			const tagMapActorInfo& oneMapNpcInfo = pMapShower->getMapNpcInfo( nRealMapID, i );
			int nNpcId = oneMapNpcInfo.m_nId;
			showQuestTitle( nNpcId );
		}
	}

	for( int i = 0, size = pMapShower->getMapNpcCount( nMapID ); i < size; ++i )
	{
		const tagMapActorInfo& oneMapNpcInfo = pMapShower->getMapNpcInfo( nMapID, i );
		int nNpcId = oneMapNpcInfo.m_nId;
		showQuestTitle( nNpcId );
	}
}

// 更新当前地图NPC头顶任务标志
void GameQuest::updateNpcQuestTitle()
{
	GameMapShower*	pMapShower	= GameManager::getSingleton().getGameMapShower();
	GameMapControl* pMapControl = pMapShower->GetMinMapContal();

	int nRealMapID	= pMapShower->getRealMapId();
	int nMapID		= pMapControl->getMapID();

	updateNpcQuestTitle( nMapID, nRealMapID );
}

// 获取需要累积计时的任务
tagTimeAccumQuestInfo* GameQuest::getTimeAccumQuest( int questId )
{
	for ( std::vector<tagTimeAccumQuestInfo>::iterator iter = m_timeAccumQuestRuns.begin(), end = m_timeAccumQuestRuns.end();
		iter != end; ++iter )
	{
		if ( questId == iter->m_nTaskID )
		{
			return &(*iter);
		}
	}

	return NULL;
}

tagOneTaskInfo& GameQuest::getChgTaskInfo()
{
	return m_chgTaskInfo;
}

const tagTaskDef* GameQuest::getTaskDef( int questId )
{
	for ( std::vector<tagTaskDef*>::iterator iter = m_TaskDef.begin(), end = m_TaskDef.end(); iter != end; ++iter )
	{
		const tagTaskDef* pOneTaskDef = *iter;
		if ( pOneTaskDef->ID == questId )
		{
			return pOneTaskDef;
		}
	}

	return NULL;
}

// 添加一个已完成任务
void GameQuest::addQuestFini( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( IsTrustQuest( questId ) )
	{
		GameManager::getSingleton().SendUIEvent( "GE_TRUST_CHANGE" );
	}

	m_FiniQuest.push_back(questId);
	m_chgTaskInfo.m_nTaskID		= questId;
	m_chgTaskInfo.m_nTaskState	= CLIENT_TASK_STAT_FINI;
	GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
	GameQuest::getSingleton().updateNpcQuestTitle();
	addQusetNumOneDay( questId );
	AddGroupQuestWeekNum( questId );
	GamePromotion::getSingleton().ResetCanApplyTask();
}

// 是否已经接取了任务
bool GameQuest::isHaveAcceptQuest( int questId )
{
	return getQuestRun( questId ) != NULL;
}

// 是否曾经完成过任务
bool GameQuest::isQuestHaveFiniEver( int questId )
{
	return std::find( m_FiniQuest.begin(), m_FiniQuest.end(), questId ) != m_FiniQuest.end();
}

// 任务是否已经做过
bool GameQuest::isQuestFini( int questId )
{
	for( size_t i = 0; i < m_FiniQuest.size(); i++ )
	{
		if( m_FiniQuest[i] == questId )
		{
			const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );

			if(	&taskDef != NULL && taskDef.Redo != 0 && !isQuestHaveFinishOneDayMaxNum( questId ) )
			{
				return false;
			}

			return true;
		}
	}
	
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if ( &taskDef == NULL )
	{
		return false;
	}

	//如果是生活技能相关的任务的话，直接返回false
	if( taskDef.TaskAward.SecondarySkillType != 0 || taskDef.TaskGetLimit.SecondarySkillType != 0 )
	{
		return false;
	}
	
	if ( taskDef.Redo == 0 )
	{
		return false;
	}
	
	for ( int i = 0; i < MAX_TASK_TASKCYCLE; ++i )
	{
		int nOneRecycleID = taskDef.TaskRecycleIDs[i];
		if ( nOneRecycleID == 0 )
		{
			continue;
		}

		if (  isQuestLimitNumOneDay( questId, nOneRecycleID ) && 
				getQuestOneDayFiniNum( questId, nOneRecycleID ) >= getQuestOneDayLimitNum( questId, nOneRecycleID ) )
		{
			return true;
		}
	}

	return false;
}

// 设置任务是否全部追踪
void GameQuest::setQuestTraceAll( bool bTraceAll )
{
	m_bTraceAll = bTraceAll;
}


// 获得任务类型名称
const char* GameQuest::getQuestTypeName( int nType )
{
	assert( nType < CLIENT_TASK_TYPE_END );
	if ( nType >= CLIENT_TASK_TYPE_END )
	{
		return "";
	}

	return strQuestType[nType].c_str();
}

// 获取运镖任务所需要的压金
tagTaskMoney_toLua* GameQuest::getConvoyQuestMoney( int questId )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
		const tagLevelUP LevelUpDef = DBDataManager::getSingleton().getLevelUP( pMainPlayer->getLv() );
		if( taskDef.Type == CLIENT_TASK_TYPE_SINGLE_CONVOY || taskDef.Type == CLIENT_TASK_TYPE_TEAM_CONVOY || taskDef.Type == CLIENT_TASK_TYPE_DART_CAR )
		{
			static tagTaskMoney_toLua TaskMoney;
			TaskMoney.Type	= taskDef.ConsumeMoney.Type;
			TaskMoney.Val	= taskDef.ConsumeMoney.Val;
			return (tagTaskMoney_toLua*)&TaskMoney;
		}
	}
	return NULL;
}

// 获取任务一天完成的次数
const int GameQuest::getQuestOneDayFiniNum( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	// 非循环任务没有次数限制
	if( taskDef.Redo == 0 )
	{
		return -1;
	}

	// 任务是否有组限制
	if( taskDef.LimitGroupID > 0 )
	{
		return getGroupQusetOneDayNum( taskDef.LimitGroupID );
	}

	// 任务是否环组任务
	if( nRecycleID > 0 )
	{
		// 每天完成次数的限制
		return getReCycleQusetOneDayNum( nRecycleID );
	}

	return getQusetHaveFinishNumOneDay( questId );
}

// 判断该任务是否是不限制次数的
bool GameQuest::isQuestLimitNumOneDay( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.Redo == 0 )
	{
		return true;
	}

	// 任务是否有组限制
	if( taskDef.LimitGroupID > 0 )
	{
		/*const tagTaskLimitGroup& taskLimitGroup = DBDataManager::getSingleton().getTaskLimitGroup( taskDef.LimitGroupID );
		return taskLimitGroup.GroupOneDay;*/

		return true;
	}

	if( nRecycleID > 0 )
	{
		const tagTaskRecycleDef& questRecycleDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		// 每天完成次数的限制
		return questRecycleDef.OneDayNum != 0;
	}

	return taskDef.TaskGetLimit.NumOneDay != 0;
}

// 获得任务一天完成的限制次数
const int GameQuest::getQuestOneDayLimitNum( int questId, int nRecycleID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	// 非循环任务没有次数限制
	if( taskDef.Redo == 0 )
	{
		return 0;
	}

	// 任务是否有组限制
	if( taskDef.LimitGroupID > 0 )
	{
		const tagTaskLimitGroup& taskLimitGroup = DBDataManager::getSingleton().getTaskLimitGroup( taskDef.LimitGroupID );
		return taskLimitGroup.GroupOneDay;
	}

	if ( nRecycleID > 0 )
	{
		const tagTaskRecycleDef& questRecycleDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
		// 每天完成次数的限制
		return questRecycleDef.OneDayNum;
	}
	
	return taskDef.TaskGetLimit.NumOneDay;
}

void GameQuest::SetConvoyInfo( bool bGetNewTask, int nQuestId )
{
	static int s_convoyType[] = { CLIENT_TASK_TYPE_SINGLE_CONVOY, CLIENT_TASK_TYPE_TEAM_CONVOY, CLIENT_TASK_TYPE_DART_CAR };
	int* pEnd = s_convoyType + sizeof( s_convoyType ) / sizeof( int );
	const TASKDEF& taskDef = DBDataManager::getSingleton().getTaskDef( nQuestId );
	if ( std::find( s_convoyType, pEnd, taskDef.Type ) != pEnd )
	{
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		pMainPlayer->SetConvoy( bGetNewTask, nQuestId );
	}
}

bool GameQuest::canGetDynamicTask( int nQuestID )
{
	if ( m_FinishedDynamicGetTasks.empty() )
	{
		return false;
	}

	return std::find( m_FinishedDynamicGetTasks.begin(), m_FinishedDynamicGetTasks.end(), nQuestID ) != m_FinishedDynamicGetTasks.end();
}

void GameQuest::addDynamicGetTask( const int* pQuest, int nNum )
{
	for ( int i = 0; i < nNum; ++i )
	{
		int nQuestID = pQuest[i];
		if ( canGetDynamicTask( nQuestID ) )
		{
			continue;
		}

		m_FinishedDynamicGetTasks.push_back( nQuestID );
	}
}

void GameQuest::setDynamicGetTask( const int* pQuest, int nNum )
{
	assert( pQuest != NULL );

	m_FinishedDynamicGetTasks.clear();
	for ( int i = 0; i < nNum; ++i )
	{
		m_FinishedDynamicGetTasks.push_back( pQuest[i] );
	}
}

// 任务是否是共享任务
bool GameQuest::isShareQuest( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if(	taskDef.CanShare != 0 )
	{
		return true;
	}
	return false;
}

bool GameQuest::getQuestCanAccept( int questId )
{
	for( size_t i = 0; i < m_CanAcceptQuest.size(); i++ )
	{
		if( questId == m_CanAcceptQuest[i] )
		{
			return true;
		}
	}
	return false;
}
int GameQuest::GetNpcQuestID( int npcID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return 0 ;
	}
	tagNpcQusetInfo& npcQuestInfo = getNpcQuestInfo( npcID );
	for( size_t i = 0; i < npcQuestInfo.m_HaveAcceptQuestId.size(); i++ )
	{
		int nAcceptQuestID = npcQuestInfo.m_HaveAcceptQuestId[i];
		// 若可以接的任务已经完成，则无需查看
		if( isQuestFini( nAcceptQuestID ) )
		{
			continue;
		}

		CSTASKRUNONE* pRunQuest = GameQuest::getSingleton().getQuestRun( nAcceptQuestID );
		// 若已经接了该任务，则不存在该任务是否可以接的可能性了
		if( pRunQuest != NULL )
		{
			return nAcceptQuestID;
		}

		// 若该NPC身上可以接取的任务还没接
		if ( GameQuest::getSingleton().getQuestCanAccept( nAcceptQuestID ) )
		{
			return nAcceptQuestID;
		}
	}
	return 0;
}
// 显示头顶任务提示
void GameQuest::showQuestTitle( int npcId )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return;
	}
	int nLv		= pMainPlayer->getLv();
	int nGender	= pMainPlayer->getGender();
	int nShow	= CLIENT_TASK_STAT_NO;
	tagNpcQusetInfo& npcQuestInfo = getNpcQuestInfo( npcId );
	// 对赏金任务的处理
	npcQuestInfo.m_HaveFiniQuestId;
	for( size_t i = 0 ; i < m_QuestRuns.size() ; i ++ )
	{
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( m_QuestRuns[i].TaskID );
		if ( taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_ITEM || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
		{
			if( taskDef.CommitNpcID == npcId &&
				std::find( npcQuestInfo.m_HaveFiniQuestId.begin(), npcQuestInfo.m_HaveFiniQuestId.end(), m_QuestRuns[i].TaskID ) == npcQuestInfo.m_HaveFiniQuestId.end() )
			{
				npcQuestInfo.m_HaveFiniQuestId.push_back( m_QuestRuns[i].TaskID );
			}
		}
	}
	// 优先级从低到高
	// 1.检查可以接受的任务
	for( size_t i = 0; i < npcQuestInfo.m_HaveAcceptQuestId.size(); i++ )
	{
		int nHaveAcceptQuestId = npcQuestInfo.m_HaveAcceptQuestId[i];
		if ( nHaveAcceptQuestId == CLAN_SPY_TASK_ID || nHaveAcceptQuestId == CLAN_ANTI_SPY_TASK_ID )
		{
			if ( pMainPlayer->getClanID() != GameClanManager::getSingleton().getCurClanCityId() )
			{
				continue;
			}
		}
		// 若可以接的任务已经完成，则无需查看
		if( isQuestFini( nHaveAcceptQuestId ) )
		{
			continue;
		}

		CSTASKRUNONE* pQuset = GameQuest::getSingleton().getQuestRun( nHaveAcceptQuestId );
		// 若已经接了该任务，则不存在该任务是否可以接的可能性了
		if( pQuset != NULL )
		{
			continue;
		}

		// 若该NPC身上可以接取的任务还没接
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( nHaveAcceptQuestId );
		int nRes = GameQuest::getSingleton().canAcceptQuest( nHaveAcceptQuestId );
		// 若是该NPC没有任何任务状态，
		if( nShow == CLIENT_TASK_STAT_NO && nRes == CLINT_CAN_NOT_ACCEPT_TASK_BY_LITTLE_LV )
		{
			/*if( taskDef.Redo ==0 )
			{
				nShow = 4;
			}*/
			nShow = 4;
		}
		// 若该NPC没有处于完成任务的状态且该任务可以接
		else if( nShow != CLIENT_TASK_STAT_FINI && nRes == CLINT_CAN_ACCEPT_TASK )
		{
			// 若该任务可以接，且不是环组任务，则将NPC的任务状态置为可以接取的状态
			if ( taskDef.Redo ==0 )
			{
				nShow = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT;
			}
			// 若该任务是循环任务，且之前
			else if ( nShow != CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT  )
			{
				nShow = CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT;
			}
		}
	}

	for( size_t i = 0; i < npcQuestInfo.m_HaveAcceptDynamicQuestId.size(); i++ )
	{
		int nHaveAcceptDynamicQuestId = npcQuestInfo.m_HaveAcceptDynamicQuestId[i];
		// 若可以接的任务已经完成，则无需查看
		if( isQuestFini( nHaveAcceptDynamicQuestId ) )
		{
			continue;
		}

		CSTASKRUNONE* pQuset = GameQuest::getSingleton().getQuestRun( nHaveAcceptDynamicQuestId );
		// 若已经接了该任务，则不存在该任务是否可以接的可能性了
		if( pQuset != NULL )
		{
			continue;
		}

		// 若该NPC身上可以接取的任务还没接
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( nHaveAcceptDynamicQuestId );
		int nRes = GameQuest::getSingleton().canAcceptQuest( nHaveAcceptDynamicQuestId );
		// 若是该NPC没有任何任务状态，
		if( nShow != CLIENT_TASK_STAT_FINI && nRes == CLINT_CAN_ACCEPT_TASK )
		{
			// 若该任务可以接，且不是环组任务，则将NPC的任务状态置为可以接取的状态
			if ( taskDef.Redo ==0 )
			{
				nShow = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT;
			}
			// 若该任务是循环任务，且之前
			else if ( nShow != CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT  )
			{
				nShow = CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT;
			}
		}
	}
	
	// 2.检查可以提交的任务
	for( size_t i = 0; i < npcQuestInfo.m_HaveFiniQuestId.size(); i++ )
	{
		int nHaveFiniQuestId = npcQuestInfo.m_HaveFiniQuestId[i];
		CSTASKRUNONE* pQuset = GameQuest::getSingleton().getQuestRun( nHaveFiniQuestId );
		if ( nHaveFiniQuestId == CLAN_SPY_TASK_ID || nHaveFiniQuestId == CLAN_ANTI_SPY_TASK_ID )
		{
			if ( pMainPlayer->getClanID() != GameClanManager::getSingleton().getCurClanCityId() )
			{
				continue;
			}
		}
		// 若该任务已经提交，则无法将NPC置为任务可以提交的状态
		if( isQuestFini( nHaveFiniQuestId ) )
		{
			continue;
		}
		
		// 若该任务主角还没接取，则不存在可以提交的可能性了
		if ( pQuset == NULL )
		{
			continue;
		}

		// 若该任务已经完成，可以提交了，则将NPC的任务状态置为可以提交任务的状态
		if( pQuset->TaskStat == CLIENT_TASK_STAT_FINI ) 
		{
			nShow = CLIENT_TASK_STAT_FINI; 
		}
		else
		{
			// 若该任务既不是完成状态，也不是可以提交状态，则将任务置为失败状态
			if( nShow != CLIENT_TASK_STAT_FINI && nShow != CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT && 
				nShow != CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT )
			{
				nShow = CLIENT_TASK_STAT_FAIL;
			}
		}
	}
	
	// 3.检查可以接受的日常任务
	for( size_t i = 0; i < npcQuestInfo.m_HaveAcceptRecycle.size(); i++ )
	{
		const CYCLEGRADE& oneCycleGrade = npcQuestInfo.m_HaveAcceptRecycle[i];
		CSTASKRUNONE* pQuset = GameQuest::getSingleton().getQuestRunByRecycle( oneCycleGrade.TaskCycleID );
		// 若已经接了该循环任务，则不存在该循环任务是否可以接的可能性了
		if ( pQuset != NULL )
		{
			continue;
		}
		
		if( nShow != CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT && nShow != CLIENT_TASK_STAT_FINI && 
			canAcceptRecycleQuest( nLv, oneCycleGrade.TaskCycleID, npcId ) )
		{
			nShow = CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT;
		}
	}
	
	// 4.检查可以提交的日常任务
	for( size_t i = 0; i < npcQuestInfo.m_HaveFiniRecycle.size(); i++ )
	{
		const CYCLEGRADE& oneCycleGrade = npcQuestInfo.m_HaveFiniRecycle[i];
		int nRecycleId = npcQuestInfo.m_HaveFiniRecycle[i].TaskCycleID;
		
		CSTASKRUNONE* pQuset = GameQuest::getSingleton().getQuestRunByRecycle( oneCycleGrade.TaskCycleID );
		// 若该循环任务主角还没接取，则不存在可以提交的可能性了
		if( pQuset == NULL )
		{
			continue;
		}

		if( ( pQuset->TaskCycleIdx + 1 ) < oneCycleGrade.From
				|| ( pQuset->TaskCycleIdx + 1 ) > oneCycleGrade.To  )
		{
			continue;
		}

		if( pQuset->TaskStat == CLIENT_TASK_STAT_FINI ) 
		{
			nShow = CLIENT_TASK_STAT_FINI; 
		}
		else
		{
			if( nShow != CLIENT_TASK_STAT_FINI && nShow != CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT && 
				nShow != CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT )
			{
				nShow = CLIENT_TASK_STAT_FAIL; 
			}
		}
	}

	int nId		  = ActorManager::getSingleton().getActorIdByResId( npcId );
	GameNPC* pNpc = (GameNPC*)ActorManager::getSingleton().FindActor( nId );
	// 对公会福利npc处理
	if( CLAN_GIFT_NPC_ID == npcId && GameWizard::getSingleton().isCanGetClanGift() )
	{
		nShow = 5;
	}
	else if( CLAN_GIFT_NPC_ID == npcId && !GameWizard::getSingleton().isCanGetClanGift() )
	{
		nShow = CLIENT_TASK_STAT_NO;
	}
	if( pNpc != NULL )
	{
		if( nShow != npcQuestInfo.m_nQuestShow )
		{
			if( nShow != CLIENT_TASK_STAT_NO )
			{
				pNpc->getShowActor()->getCurEntity()->stopMotion( 10000 );
				pNpc->getShowActor()->getCurEntity()->playMotion( strQuestEffect[nShow].c_str(), false, 10000 );
			}
			else
			{
				pNpc->getShowActor()->getCurEntity()->stopMotion( 10000 );
			}
		}
		pNpc->m_nQusetShow = nShow;
	}

	npcQuestInfo.m_nQuestShow = nShow;
}

// 是否在指定区域
bool GameQuest::isInArea( tagDiscoverInfo& discoverInfo, const tagTaskDef& taskDef, int nTaskId, int nTaskState )
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	GameMapShower*	pMapShower = GameManager::getSingleton().getGameMapShower();
	int mapid = pMapShower->GetMinMapContal()->getMapID();
	const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( mapid );
	if( mapDef.ReferMapID > 0 && mapDef.MapLine > 0 )
	{
		mapid = mapDef.ReferMapID;
	}
	if( taskDef.FiniTaskArea.MapID == mapid )
	{
		float fDifPosX = abs( taskDef.FiniTaskArea.Pos.X - pMainPlayer->GetPosition().x/10.0f );
		float fDifPosY = abs( taskDef.FiniTaskArea.Pos.Y - pMainPlayer->GetPosition().z/10.0f );
		float nDisSquare =  sqrt( fDifPosX*fDifPosX + fDifPosY*fDifPosY );
		if( (int)taskDef.FiniTaskArea.Radius >= ceil( nDisSquare ) 
			&& nTaskState != CLIENT_TASK_STAT_FINI && !discoverInfo.m_bFiniTask)
		{
			if( discoverInfo.m_nInAreaTime == 0 )
			{
				discoverInfo.m_nInAreaTime = (int)timeGetTime();
			}
			int nPassTime = ( (int)timeGetTime() - discoverInfo.m_nInAreaTime ) / 1000;
			// TODO::将时间更换成服务器的宏 TAKS_AREA_STOP_TIME
			if( nPassTime >= TAKS_AREA_STOP_TIME )
			{
				finishQuest( nTaskId );
				discoverInfo.m_bFiniTask = true;
			}
			return true;
		}
	}
	return false;
}

// 检查是否满足累积计时的任务
bool GameQuest::isSatisfyTimeAccumQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef )
{
	tagTimeAccumQuestInfo* pTimeAccumQuest = getTimeAccumQuest( runQuest.TaskID );
	if ( pTimeAccumQuest == NULL )
	{
		return true;
	}

	return pTimeAccumQuest->m_nPassTime >= pTimeAccumQuest->m_nMaxTime;
}

// 更新需要累积计时的任务
void GameQuest::updateTimeAccumQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef )
{
	tagTimeAccumQuestInfo* pTimeAccumQuest = getTimeAccumQuest( runQuest.TaskID );
	if ( pTimeAccumQuest == NULL || pTimeAccumQuest->m_nPassTime >= pTimeAccumQuest->m_nMaxTime )
	{
		return;
	}

	std::map<int, int>::iterator pretimeiter = m_PreTime.find( runQuest.TaskID );
	
	if( pretimeiter != m_PreTime.end() )
	{
		int nCurTime	= (int)timeGetTime();
		int nPassTime	= ( nCurTime - pretimeiter->second ) / 1000;
		if ( nPassTime == 0 )
		{
			return;
		}
		
		// pTimeAccumQuest->m_nTimeMode == 0 正计时
		// pTimeAccumQuest->m_nTimeMode == 1   倒计时
		pTimeAccumQuest->m_nPassTime	+= nPassTime;
		pretimeiter->second				= nCurTime;
		// TODO: 正计时任务不会要求打怪或者收集物品
		GameManager::getSingleton().SendUIEvent( "GE_QUEST_TIMECHANGE" );
		//if ( pTimeAccumQuest->m_nPassTime >= pTimeAccumQuest->m_nMaxTime /*&& 
		//		isSatisfyItemOrMonsterQuest( runQuest, taskDef )*/ )
		//{
		//	runQuest.TaskStat = CLIENT_TASK_STAT_FINI;
		//	finishQuest( runQuest.TaskID );
		//	GameQuest::getSingleton().updateNpcQuestTitle();
		//	GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
		//}
		//else
		//{	
		//	GameManager::getSingleton().SendUIEvent( "GE_QUEST_TIMECHANGE" );
		//}
	}
}

// 更新倒计时任务
void GameQuest::updateTimeCountDownQuest( CSTASKRUNONE& runQuest )
{
	std::map<int, int>::iterator pretimeiter = m_PreTime.find( runQuest.TaskID );
	if( pretimeiter != m_PreTime.end() )
	{
		int nCurTime = (int)timeGetTime();
		int nPassTime	= ( nCurTime - pretimeiter->second ) / 1000;
		if( nPassTime > 0 )
		{
			runQuest.TaskEndTime	=  runQuest.TaskEndTime - nPassTime;
			pretimeiter->second		= nCurTime;
			GameManager::getSingleton().SendUIEvent( "GE_QUEST_TIMECHANGE" );
		}
	}
}

// 检查是否满足要收集物品或打怪
bool GameQuest::isSatisfyItemOrMonsterQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef )
{
	if ( taskDef.GetItem[0].ItemID == 0 && taskDef.KillMon[0].Num == 0 )
	{
		return true;
	}
	
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	bool bFiniItem = true;
	if( taskDef.GetItem[0].ItemID != 0 )
	{
		for( int i = 0; i < MAX_TASK_ITEM; i++ )
		{
			if( taskDef.GetItem[i].Num != 0 && 
				pMainPlayer->getContainer().getItemCount( taskDef.GetItem[i].ItemID ) < (int)taskDef.GetItem[i].Num )
			{
				bFiniItem = false;
				break;
			}
		}
	}

	bool bFiniMon = true;
	// 同时需要判断杀怪是否完成
	if( taskDef.KillMon[0].Num != 0 )
	{
		// 需要击杀怪物
		for( int i = 0; i < runQuest.TaskMonTrace.TaskMonTraceNum; i++ )
		{
			if( runQuest.TaskMonTrace.TaskKillMons[i].Num < runQuest.TaskMonTrace.TaskKillMons[i].MaxNum && 
				runQuest.TaskMonTrace.TaskKillMons[i].MaxNum != 0 )
			{
				bFiniMon = false;
				break;
			}
		}
	}

	return bFiniMon && bFiniItem;
}

// 更新需要道具或者打怪的任务
void GameQuest::updateItemOrMonsterQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef, bool bShowMsg )
{
	// TODO: 经策划确认以后不会加同时满足这2个情况的任务
	/*if ( !isSatisfyTimeAccumQuest( runQuest.TaskID ) )
	{
		return;
	}*/

	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}

	bool bFiniItem = true;
	if( taskDef.GetItem[0].ItemID != 0 )
	{
		for( int i = 0; i < MAX_TASK_ITEM; i++ )
		{
			if( taskDef.GetItem[i].ItemID != 0 && 
				pMainPlayer->getContainer().getItemCount( taskDef.GetItem[i].ItemID ) < runQuest.TaskItemTrace.TaskGetItems[i].MaxNum )
			{
				bFiniItem = false;
				break;
			}
		}
	}

	if (taskDef.Type == CLIENT_TASK_TYPE_SJ_MON)
	{
		int my =0;
	}
	if (taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS)
	{
		int my =0;
	}
	bool bFiniMon = true;
	// 同时需要判断杀怪是否完成
	if( taskDef.KillMon[0].Num != 0 )
	{
		// 需要击杀怪物
		for( int i = 0; i < runQuest.TaskMonTrace.TaskMonTraceNum; i++ )
		{
			const TASKMONTRACEONE& taskMonTraceOne = runQuest.TaskMonTrace.TaskKillMons[i];
			if( taskMonTraceOne.Num < taskMonTraceOne.MaxNum && taskMonTraceOne.MaxNum != 0 )
			{
				bFiniMon = false;
				break;
			}
		}
	}

	if( bFiniMon && bFiniItem ) 
	{
		if ( runQuest.TaskStat == CLIENT_TASK_STAT_FINI ) 
		{
			return;
		}

		runQuest.TaskStat = CLIENT_TASK_STAT_FINI;
		if( isQuestTimeLimit( runQuest.TaskID )  )
		{
			if( runQuest.TaskEndTime <= 0 )
			{
				runQuest.TaskStat = CLIENT_TASK_STAT_FAIL;
				return;
			}
		}
		
		m_chgTaskInfo.m_nTaskID		= runQuest.TaskID;
		m_chgTaskInfo.m_nTaskState	= runQuest.TaskStat;
		GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
		GameQuest::getSingleton().updateNpcQuestTitle();
		if( bShowMsg )
		{
			char szBuf[256];
			sprintf( szBuf, "QusetFinish(\"%s\", %d)", taskDef.Name, taskDef.ID );
			GameManager::getSingleton().getScriptVM()->callString( szBuf );
		}
		// 记录任务结束
		finishQuest( runQuest.TaskID );
	}
	else
	{
		if ( runQuest.TaskStat == CLIENT_TASK_STAT_FINI ) 
		{
			//PrintQuestState( runQuest.TaskID, runQuest.TaskStat, "updateItemOrMonsterQuest--3509" );

			runQuest.TaskStat = CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT;
			m_chgTaskInfo.m_nTaskID		= runQuest.TaskID;
			m_chgTaskInfo.m_nTaskState	= runQuest.TaskStat;
			GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
			GameQuest::getSingleton().updateNpcQuestTitle();
		}
	}	
}

// 登陆时设置任务状态
void GameQuest::updateAllTaskState( bool bShowMsg )
{
	for ( std::vector<CSTASKRUNONE>::iterator iter = m_QuestRuns.begin(), end = m_QuestRuns.end(); iter != end; ++iter )
	{
		if ( iter->TaskEndTime > 0 && iter->TaskStat != CLIENT_TASK_STAT_FINI )
		{
			updateTimeCountDownQuest( *iter );
		}
		
		const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( iter->TaskID );
		if( taskDef.GetItem[0].ItemID != 0 || taskDef.KillMon[0].Num != 0 || 
			taskDef.Type == CLIENT_TASK_TYPE_SJ_MON || taskDef.Type == CLIENT_TASK_TYPE_SJ_BOSS )
		{
			updateItemOrMonsterQuest( *iter, taskDef, bShowMsg );
		}	
		
		if ( taskDef.FiniTaskArea.MapID > 0 )
		{
			updateAutoFiniQuest( *iter, taskDef );
		}

		if ( isTimeAccumQuest( iter->TaskID ) )
		{
			updateTimeAccumQuest( *iter, taskDef );
		}
	}
}

int	GameQuest::getFirstNotLvLimitRecycle( int questId )
{
	const tagTaskDef& taskDef	= DBDataManager::getSingleton().getTaskDef( questId );
	ActorManager& actorMgr		= ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer = actorMgr.getMainPlayer();
	int nLv						= pMainPlayer->getLv();

	for ( int i = 0; i < MAX_TASK_TASKCYCLE; ++i )
	{
		int nOneRecycleID = taskDef.TaskRecycleIDs[i];
		if ( nOneRecycleID == 0 )
		{
			continue;
		}
		const tagTaskRecycleDef&	taskRecycleDef  = DBDataManager::getSingleton().getTaskRecycleDef( nOneRecycleID );
		if ( !isLimitByLv( taskRecycleDef, nLv ) )
		{
			return nOneRecycleID;
		}
	}
	return 0;
}

// 环组任务接取等级限制
bool GameQuest::isLimitByLv( const tagTaskRecycleDef& taskRecycleDef, int nPlayerLv )
{
	if( nPlayerLv > taskRecycleDef.MaxLevel || nPlayerLv < taskRecycleDef.MinLevel )
	{
		return true;
	}
	return false;
}

// 环组任务组限制
bool GameQuest::isLimitByGroup( const tagTaskRecycleDef& taskRecycleDef )
{
	if( taskRecycleDef.GroupLimit != 0 )
	{
		int nGroupOnedayNum = 0;
		std::map<int, TASKCYCLEONEDAYINFO>::iterator onedayiter = m_RecycleOneDayInfo.begin();
		for(; onedayiter != m_RecycleOneDayInfo.end(); onedayiter++)
		{
			const tagTaskRecycleDef& recycleDef = DBDataManager::getSingleton().getTaskRecycleDef( onedayiter->second.TaskCycleID );
			if( &recycleDef != NULL 
				&& recycleDef.GroupLimit == taskRecycleDef.GroupLimit )
			{
				nGroupOnedayNum +=onedayiter->second.OneDayNum;
			}
		}
		if( nGroupOnedayNum >= taskRecycleDef.GroupLimit )
		{
			return true;
		}
	}
	return false;
}

// 环组任务类型限制
bool GameQuest::isLimitByMutex( const tagTaskRecycleDef& taskRecycleDef )
{
	if( taskRecycleDef.IsGroupMutex != 0 )
	{
		std::map<int, TASKCYCLEONEDAYINFO>::iterator onedayiter = m_RecycleOneDayInfo.begin();
		for(; onedayiter != m_RecycleOneDayInfo.end(); onedayiter++)
		{
			const tagTaskRecycleDef& recycleDef = DBDataManager::getSingleton().getTaskRecycleDef( onedayiter->second.TaskCycleID );
			if( &recycleDef != NULL 
				&& recycleDef.IsGroupMutex != 0 
				&& recycleDef.GroupID == taskRecycleDef.GroupID
				&& recycleDef.ID != taskRecycleDef.ID )
			{
				return true;
			}
		}
	}
	return false;
}

// 环组任务每天完成次数的限制
bool GameQuest::isLimitByOneDay( const tagTaskRecycleDef& taskRecycleDef )
{
	if( getReCycleQusetOneDayNum( taskRecycleDef.ID ) >= taskRecycleDef.OneDayNum )
	{
		return true;
	}
	return false;
}

// 环组任务环限制
bool GameQuest::isLimitByRecycleIdx( int nRecycleId, int nNpcId  )
{
	int nRecycleIndex = 0;
	std::map<int, int>::iterator iter = m_QuestRecycleIdx.find( nRecycleId );
	if( iter!= m_QuestRecycleIdx.end() )
	{
		nRecycleIndex = iter->second/* + 1*/;
	}

	const tagNpcQusetInfo& npcinfo = getNpcQuestInfo( nNpcId );
	for( size_t i = 0; i < npcinfo.m_HaveAcceptRecycle.size(); i++ )
	{
		if( npcinfo.m_HaveAcceptRecycle[i].TaskCycleID == 0 )
		{
			return true;
		}

		if( nRecycleId == npcinfo.m_HaveAcceptRecycle[i].TaskCycleID
			&& nRecycleIndex >= npcinfo.m_HaveAcceptRecycle[i].From-1
			&& nRecycleIndex <= npcinfo.m_HaveAcceptRecycle[i].To-1 )
		{
			return false;
		}
	}	

	return true;
}

// 环组任务是否受公会限制
bool GameQuest::isLimitByClan( const tagTaskRecycleDef& taskRecycleDef )
{
	if ( taskRecycleDef.CycType != CLIENT_TASK_RECYCLE_CLAN )
	{
		return false;
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	return !pMainPlayer->isInClan();
}
//环组任务是否受前置任务限制
bool GameQuest::isLimitByPreTaskID( const tagTaskRecycleDef& taskRecycleDef )
{
	if ( taskRecycleDef.PreTaskID > 0  )
	{
		return !isQuestFini( taskRecycleDef.PreTaskID );
	}
	return false;
}

bool GameQuest::isFixedSequenceRecycleTask( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return isFixedSequenceRecycleTask( taskDef );
}

bool GameQuest::isFixedSequenceRecycleTask( const tagTaskDef& taskDef )
{
	if ( &taskDef == NULL )
	{
		return false;
	}

	return isRecycleTask( taskDef ) && taskDef.Type == CLIENT_TASK_TYPE_CHG_RECYCLE;
}

bool GameQuest::canAcceptRecycleTask( int nRecycleID )
{
	if ( nRecycleID == 0 )
	{
		return false;
	}
	
	ActorManager& actorMgr		= ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer = actorMgr.getMainPlayer();
	const tagTaskRecycleDef& taskRecycleDef = DBDataManager::getSingleton().getTaskRecycleDef( nRecycleID );
	if ( &taskRecycleDef == NULL )
	{
		return false;
	}

	if ( isLimitByLv( taskRecycleDef, pMainPlayer->getLv() ) )
	{
		return false;
	}

	if ( isLimitByGroup( taskRecycleDef ) )
	{
		return false;
	}

	if ( isLimitByMutex( taskRecycleDef ) )
	{
		return false;
	}

	if ( isLimitByOneDay( taskRecycleDef ) )
	{
		return false;
	}
	if ( isLimitByPreTaskID( taskRecycleDef ) )
	{
		return false;
	}

	return true;
}

int	 GameQuest::getFirstCanAcceptRecycleTaskID( const tagTaskDef& taskDef )
{
	if ( !isRecycleTask( taskDef ) )
	{
		return 0;
	}

	for ( int i = 0; i < MAX_TASK_TASKCYCLE; ++i )
	{
		int nOneRecycleID = taskDef.TaskRecycleIDs[i];
		if ( !canAcceptRecycleTask( nOneRecycleID ) )
		{
			continue;
		}

		bool isExactRecycleIdx = isFixedSequenceRecycleTask( taskDef ) && 
				taskDef.Flag == ( getTaskRecycleIdx( taskDef.ID, nOneRecycleID ) - 1 );
		if ( !isExactRecycleIdx )
		{
			continue;
		}

		return nOneRecycleID;
	}

	return 0;
}

bool GameQuest::canAcceptRecycleTask( const tagTaskDef& taskDef )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return false;
	}
	
	int nCanAcceptRecycleID = getFirstCanAcceptRecycleTaskID( taskDef );
	return nCanAcceptRecycleID != 0;
}

bool GameQuest::isBelongToRecycleTask( const tagTaskDef& taskDef, int nRecycleID )
{
	if ( nRecycleID == 0 )
	{
		return false;
	}

	for ( int i = 0; i < MAX_TASK_TASKCYCLE; ++i )
	{
		if ( nRecycleID == taskDef.TaskRecycleIDs[i] )
		{
			return true;
		}
	}

	return false;
}

void GameQuest::loadRecycleTaskPreTaskFixIndex()
{
	for ( int nTaskIndex = 0, size = (int)m_TaskDef.size(); nTaskIndex < size; ++nTaskIndex )
	{
		tagTaskDef& oneTaskDef = *( m_TaskDef[nTaskIndex] );
		if( oneTaskDef.Type == TASK_TYPE_DAILY_GOAL )
		{
			GamePromotion::getSingleton().addDailyGoalTask( oneTaskDef.ID );
			continue;
		}
		oneTaskDef.Flag = -1;
		if ( !isFixedSequenceRecycleTask( oneTaskDef ) )
		{
			continue;
		}

		for ( int nRecycleTaskIndex = 0; nRecycleTaskIndex < MAX_TASK_TASKCYCLE; ++nRecycleTaskIndex )
		{
			int nOneRecycleID = oneTaskDef.TaskRecycleIDs[nRecycleTaskIndex];
			if ( nOneRecycleID == 0 )
			{
				continue;
			}
			const tagTaskRecycleDef& taskRecyDef = DBDataManager::getSingleton().getTaskRecycleDef( nOneRecycleID );
			for ( int nRecycleIndex = 0; nRecycleIndex < MAX_TASK_RECYCLE; ++nRecycleIndex )
			{
				const TASKRECYCLEENTRY& oneRecycleTaskInfo = taskRecyDef.TaskRecycles[nRecycleIndex];
				if ( oneRecycleTaskInfo.TaskID == 0 )
				{
					break;
				}

				if ( oneRecycleTaskInfo.TaskID == oneTaskDef.ID )
				{
					oneTaskDef.Flag = nRecycleIndex;
					break;
				}
			}
		}
	}
}

int GameQuest::getQuestMaxGenericProgress(int questId)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.MaxGenericProgress;
}

int GameQuest::getQuestGenericProgress(int questId)
{
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	if ( pQuest == NULL )
	{
		return 0;
	}
	return pQuest->TaskGenericProgress.Progress;
}


bool GameQuest::IsCollectHasItem(int nMonResId )
{
	const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef(nMonResId);
	bool isNeed = false;
	for (size_t i = 0; i < MAX_MON_COLLECT_PACKAGE_ITEM; ++i)
	{
		if (MonsterDef.Collect.CollectPackgeItem[i] != 0)
		{
			isNeed = true;
			break;
		}
	}
	if (!isNeed)
	{
		return true;
	}
	MainGamePlayer* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return false;
	}
	for (size_t i = 0; i < MAX_MON_COLLECT_PACKAGE_ITEM; ++i)
	{
		if (MonsterDef.Collect.CollectPackgeItem[i] != 0)
		{
			if (pMainPlayer->getContainer().getItemCount(MonsterDef.Collect.CollectPackgeItem[i]) > 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool GameQuest::IsCollectNeedItem(int nMonResId )
{
	const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef( nMonResId );
	return  MonsterDef.Collect.TaskID[0] == 1748;
}

bool GameQuest::IsNeedCollectItem( int questId ,int index )
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return   false;
	}
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( taskDef.GetItem[index].Num != 0 && 
		pMainPlayer->getContainer().getItemCount( taskDef.GetItem[index].ItemID ) < (int)taskDef.GetItem[index].Num )
	{
		return true;
	}
	return false;
}

bool GameQuest::IsNeedMonster(int questId)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	 if ( taskDef.KillMon[0].Num != 0 )
	 {
		 return true;
	 }
	 return false;
}

//获得任务所需要的怪物ID
int GameQuest::getQuestMonsterId(int questId)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	TASKMONTRACE MonTrace = pQuest->TaskMonTrace;
	for (int i = 0; i< MAX_TASK_MON; i++)
	{
		if ( taskDef.KillMon[i].MonID1 != 0 )
		{
			for (int j = 0 ; j < MonTrace.TaskMonTraceNum;j++ )
			{
				if ( taskDef.KillMon[i].MonID1 == MonTrace.TaskKillMons[j].MonID && MonTrace.TaskKillMons[j].Num < MonTrace.TaskKillMons[j].MaxNum)
				{
					return taskDef.KillMon[i].MonID1;
				}
			}
		}
	}
	
	return -1;
}

bool GameQuest::checkQuestMonsterKilled( int questId, int index )
{
	assert( index < MAX_TASK_MON );
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	CSTASKRUNONE* pQuest = getQuestRun( questId );
	TASKMONTRACE MonTrace = pQuest->TaskMonTrace;
	if ( taskDef.KillMon[index].MonID1 != 0 && taskDef.KillMon[index].MonID1 == MonTrace.TaskKillMons[index].MonID 
		&& MonTrace.TaskKillMons[index].Num >= MonTrace.TaskKillMons[index].MaxNum )
	{
		return true;
	}
	return false;
}

//获得任务所需要的物品ID
int GameQuest::getQuestItemId(int questId)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return -1;
	}
	for( int i = 0; i < MAX_TASK_ITEM; i++ )
	{
		if( taskDef.GetItem[i].Num != 0 && 
			pMainPlayer->getContainer().getItemCount( taskDef.GetItem[i].ItemID ) < (int)taskDef.GetItem[i].Num )
		{
			return taskDef.GetItem[i].ItemID;
		}
	}

	return -1;
}
//获得任务特殊标记处的OBJ
int	 GameQuest::getQuestSpecialFiniObjVal(int questId,int index)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.SpecialTask[index].FiniObjVal;
}

//获得任务特殊标记处的OBJConditionVal
int	 GameQuest::getQuestSpecialFiniConditionVal(int questId,int index)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return taskDef.SpecialTask[index].FiniConditionVal;
}

int GameQuest::getFirstCanAcceptRecycleTaskID( int questId )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	return getFirstCanAcceptRecycleTaskID( taskDef );
}

void GameQuest::AutoQuest()
{
	
}

TASKONE_ITER GameQuest::getTaskOnedayInfoBegin()
{
	return m_TrustOneDayInfo.begin();
}

TASKONE_ITER GameQuest::getTaskOnedayInfoEnd()
{
	return m_TrustOneDayInfo.end();
}
void GameQuest::UseBountyHunterItem( int itemID )
{
	tagCSPkgBody msgbody;
	msgbody.SJTaskClt.Type = SJ_TASK_CLT_TYPE_OPEN;
	msgbody.SJTaskClt.Data.Open.ItemID = itemID;
	SendGameServerMsg( SJ_TASK_CLT, msgbody );
}
void GameQuest::RefurbishBountyHunterTask( int sjID )
{
	tagCSPkgBody msgbody;
	msgbody.SJTaskClt.Type = SJ_TASK_CLT_TYPE_REFRESH_TASK;
	msgbody.SJTaskClt.Data.RefreshTask.SJID = sjID;
	SendGameServerMsg( SJ_TASK_CLT, msgbody );
}
void GameQuest::RefurbishBountyHunterStar( int sjID, int nflag )
{
	tagCSPkgBody msgbody;
	msgbody.SJTaskClt.Type = SJ_TASK_CLT_TYPE_REFRESH_STAR;
	msgbody.SJTaskClt.Data.RefreshStar.RefreshFlag = nflag;
	msgbody.SJTaskClt.Data.RefreshStar.SJID = sjID;
	SendGameServerMsg( SJ_TASK_CLT, msgbody );
}
void GameQuest::AcceptBountyHunterTask( int itemID, int SJID, int TaskID )
{
	tagCSPkgBody msgbody;
	msgbody.SJTaskClt.Type = SJ_TASK_CLT_TYPE_ACCPET;
	msgbody.SJTaskClt.Data.Accpet.ItemID = itemID;
	msgbody.SJTaskClt.Data.Accpet.SJID = SJID;
	msgbody.SJTaskClt.Data.Accpet.TaskID = TaskID;
	SendGameServerMsg( SJ_TASK_CLT, msgbody );
}
const tagSJTaskDef_tolua* GameQuest::getSJTaskDef( int SJID )
{
	static_assert( sizeof(tagSJTaskDef_tolua) == sizeof(tagSJTaskDef) );
	return (tagSJTaskDef_tolua*)DBDataManager::getSingleton().getSJTaskDef( SJID );
}

bool GameQuest::isQuestMust(int questId)
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( questId );
	if( &taskDef != NULL)
	{
		return (taskDef.TaskFlag == TASK_NEED_DO);
	}
	return false;
}

void GameQuest::RequestRecycleTaskByItem( int nListType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType = TASK_ITEM_RECYCLE_OPEN;
	msgbody.TaskClt.TaskCltData.ItemRecycleOpen.ListType = nListType;
	msgbody.TaskClt.TaskCltData.ItemRecycleOpen.GridIdx	= nGridIdx;
	SendGameServerMsg( TASK_CLT, msgbody );
}

void GameQuest::AcceptRecycleTaskByItem( int nListType, int nGridIdx, int nTaskID )
{
	tagCSPkgBody msgbody;
	msgbody.TaskClt.OpType = TASK_ITEM_RECYCLE_ACCEPT;
	msgbody.TaskClt.TaskCltData.ItemRecycleAccept.ListType = nListType;
	msgbody.TaskClt.TaskCltData.ItemRecycleAccept.GridIdx = nGridIdx;
	msgbody.TaskClt.TaskCltData.ItemRecycleAccept.TaskID = nTaskID;
	SendGameServerMsg( TASK_CLT, msgbody );
}