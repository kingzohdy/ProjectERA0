#include <string.h>
#include <string>
#include "AutoTeam.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GamePromotion.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMsgPrint.h"
#include "GameExchange.h"
using namespace std;

const int NONE_AUTO_TEAM = -1;
const char* BATTLE_NAME_SYMBOL = "$";

bool isRoleInKuaFu( const char* pszName )
{
	assert( pszName != NULL );
	const char* pszResult	= strpbrk( pszName, BATTLE_NAME_SYMBOL );
	return pszResult != NULL;
}

AutoTeamManager::AutoTeamManager()
{
	releaseAutoTeamInfo();
	INetworkManager::getSingleton().RegisterMsgHandler( AUTOTEAM_SVR, this );
}

void AutoTeamManager::releaseAutoTeamInfo()
{
	m_selfAutoTeamApplyInfo.AutoTeamIDNum	= 0;
	m_selfAutoTeamApplyInfo.LimitFlag		= 0;
	m_preMemberInfo.m_preMemInfo.clear();
	m_nAutoTeamID		= NONE_AUTO_TEAM;
	m_nAutoTeamState	= CLT_AUTO_TEAM_STATE_NONE;
}

// 取消所有原来选择的副本
void AutoTeamManager::cancelAllSelectPWorld()
{
	m_selfAutoTeamApplyInfo.AutoTeamIDNum = 0;
}

void AutoTeamManager::addSelectPWorld( int nAutoTeamID, int nSimpleFlg, int nNormalFlg, int nHardFlg, int nProfessionalFlag )
{
	assert( m_selfAutoTeamApplyInfo.AutoTeamIDNum < AUTO_TEAM_ID_MAX );
	if ( m_selfAutoTeamApplyInfo.AutoTeamIDNum >= AUTO_TEAM_ID_MAX )
	{
		return;
	}

	int nSelNum = m_selfAutoTeamApplyInfo.AutoTeamIDNum;
	AUTOTEAMAPPLYIDINFO& applyIDInfo = m_selfAutoTeamApplyInfo.AutoTeamIDInfos[nSelNum];
	applyIDInfo.AutoTeamID	= nAutoTeamID;
	applyIDInfo.LimitFlag	= ( nSimpleFlg | nNormalFlg | nHardFlg | nProfessionalFlag );
	if ( applyIDInfo.LimitFlag == 0 )
	{
		applyIDInfo.LimitFlag = AUTO_TEAM_LIMIT_EASY;
	}
	++m_selfAutoTeamApplyInfo.AutoTeamIDNum;
}

// 获得所有限制条件
short AutoTeamManager::getAllCareerAndCapRequest()
{
	return m_selfAutoTeamApplyInfo.LimitFlag;
}

// 取消所有职业要求
void AutoTeamManager::cancelAllCareerAndCapRequest()
{
	m_selfAutoTeamApplyInfo.LimitFlag = 0;
}

// 添加职业要求
void AutoTeamManager::addCareerAndCapRequest( int nCarrer, bool bAdd )
{
	if ( bAdd )
	{
		m_selfAutoTeamApplyInfo.LimitFlag |= nCarrer;
	}
	else
	{
		m_selfAutoTeamApplyInfo.LimitFlag &= ~nCarrer;
	}
}

// 请求系统匹配自动组队，即通过点击 寻求组队 按钮发起的操作
void AutoTeamManager::requestAutoTeam()
{
	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_APPLY;

	AUTOTEAMAPPLYINFO& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.Apply;
	autoTeamCltUIData = m_selfAutoTeamApplyInfo;
	m_nAutoTeamState = CLT_AUTO_TEAM_STATE_PREPARE;
	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
}

// 拒绝完成创建
void AutoTeamManager::disagreeCreate()
{
	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;

	AUTOTEAMCLTUI& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.UIOp;
	autoTeamCltUIData.UIOp = AUTOTEAM_UI_CREATE_REJECT;

	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
}

// 同意完成创建
void AutoTeamManager::agreeCreate()
{
	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;

	AUTOTEAMCLTUI& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.UIOp;
	autoTeamCltUIData.UIOp = AUTOTEAM_UI_CREATE_AGREE;

	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
}

// 拒绝自动组队
void AutoTeamManager::disagreeAutoTeam()
{
	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;

	AUTOTEAMCLTUI& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.UIOp;
	autoTeamCltUIData.UIOp = AUTOTEAM_UI_REJECT;

	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
	m_preMemberInfo.m_preMemInfo.clear();
}

int AutoTeamManager::getMainPlayerAgreeState()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	for ( int i = 0, size = getPreMemberNum(); i < size; ++i )
	{
		tagCSAutoTeamPreMemInfo_ToLua* pMember = getPreMemberByIndex( i );
		if ( CompareRoleName( pMember->RoleName, pMainPlayer->getName() ) ) 
		{
			return pMember->AgreeStat;
		}
	}

	assert( false );
	return CLT_PRE_MEMBER_NOTDECIDE;
}

// 同意自动组队
bool AutoTeamManager::canAgreeAutoTeam()
{
	if ( getMainPlayerAgreeState() == CLT_PRE_MEMBER_AGREE )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_AGREE_READY );
		return false;
	}

	return true;
}

void AutoTeamManager::agreeAutoTeam()
{
	/*if ( !canAgreeAutoTeam() )
	{
	return;
	}*/

	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;

	AUTOTEAMCLTUI& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.UIOp;
	autoTeamCltUIData.UIOp = AUTOTEAM_UI_AGREE;

	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
}

// 发起完成创建请求
void AutoTeamManager::requestFinishCreate()
{
	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;

	AUTOTEAMCLTUI& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.UIOp;
	autoTeamCltUIData.UIOp = AUTOTEAM_UI_CREATE;

	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
}

bool AutoTeamManager::canTranToPWorld()
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE );
		return false;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD );
		return false;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_STALL );
		return false;
	}

	AutoTeamData* pAutoTeamData = GamePromotion::getSingleton().getAutoTeamData( getAutoTeamID() );
	if ( pAutoTeamData == NULL )
	{
		//return false;
		return true;
	}

	PromotionStartInfo* info = GamePromotion::getSingleton().getCurrPromitionStartInfo( pAutoTeamData->nRow );
	if ( info->startFlag != PROMOTION_RUNING )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TIME_OUT );
		return false;
	}

	return true;
}

// 传送进自动组队所选择的副本
void AutoTeamManager::tranToPWorld()
{	
	AutoTeamData* pAutoTeamData = GamePromotion::getSingleton().getAutoTeamData( getAutoTeamID() );
	
	if ( pAutoTeamData != NULL && pAutoTeamData->proType != CLT_AUTO_TEAM_ACTIVE )
	{
		return;
	}

	/*if ( !canTranToPWorld() )
	{
	return;
	}*/

	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_TRANS;

	AUTOTEAMCLTTRANS& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.Trans;
	if ( pAutoTeamData == NULL )
	{
		autoTeamCltUIData.PworldID = 0;
	}
	else
	{
		autoTeamCltUIData.PworldID = pAutoTeamData->relateID;
	}
	
	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
	GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_REQUEST_TRAN_TO_PWORLD" );
}

// 取消传送进副本
void AutoTeamManager::cancelTranToPWorld()
{
	tagCSPkgBody msgbody;
	msgbody.AutoTeamClt.AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;

	AUTOTEAMCLTUI& autoTeamCltUIData = msgbody.AutoTeamClt.AutoTeamCltData.UIOp;
	autoTeamCltUIData.UIOp = AUTOTEAM_UI_EXIT_TRANS;

	SendGameServerMsg( AUTOTEAM_CLT, msgbody );
}

// 获取自动组队中的人数
int AutoTeamManager::getAutoTeamMemberNum()
{
	return (int)m_preMemberInfo.m_preMemInfo.size();
}

// 查询自动组队中的成员
tagCSAutoTeamPreMemInfo_ToLua* AutoTeamManager::getAutoTeamMember( int nIndex )
{
	assert( nIndex < (int)m_preMemberInfo.m_preMemInfo.size() );
	return &( m_preMemberInfo.m_preMemInfo[nIndex] );
}

// 获取预创建队伍的成员数量
int AutoTeamManager::getPreMemberNum()
{
	return (int)m_preMemberInfo.m_preMemInfo.size();
}

// 查询预创建队伍的成员
tagCSAutoTeamPreMemInfo_ToLua* AutoTeamManager::getPreMemberByIndex( int nIndex )
{
	assert( nIndex < (int)m_preMemberInfo.m_preMemInfo.size() );
	return &( m_preMemberInfo.m_preMemInfo[nIndex] );
}

// 查询预创建队伍的成员
tagCSAutoTeamPreMemInfo_ToLua* AutoTeamManager::getPreMember( const char* pszName )
{
	for ( int i = 0, size = (int)m_preMemberInfo.m_preMemInfo.size(); i < size; ++i )
	{
		tagCSAutoTeamPreMemInfo_ToLua& preMemInfo = m_preMemberInfo.m_preMemInfo[i];
		if ( CompareRoleName( pszName, preMemInfo.RoleName ) )
		{
			return &preMemInfo;
		}
	}

	return NULL;
}

// 获得自动组队ID 
int AutoTeamManager::getAutoTeamID()
{
	return m_nAutoTeamID;
}

// 是否自己相当队长
bool AutoTeamManager::isSelfWantToBeCaptain()
{
	bool bResult = ( m_selfAutoTeamApplyInfo.LimitFlag & CLT_AUTO_TEAM_LIMIT_LEADER ) != 0;
	return ( m_selfAutoTeamApplyInfo.LimitFlag & CLT_AUTO_TEAM_LIMIT_LEADER ) != 0;
}

void AutoTeamManager::cleanSafeStatus( int nStatusID )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_CLEAN_SAFETIME_STATUSID_CLT;

	CSPLAYERMISCCLTDATA& cleanSafeStatusData	= msgbody.PlayerMiscClt.PlayerMiscCltData;
	cleanSafeStatusData.CleanStatusID			= nStatusID;

	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

// 预创建队伍中是否有人加入队伍
//bool AutoTeamManager::isPreMemberInTeam()
//{
//	return m_preMemberInfo.m_isAnyInTeam;
//}

// 增量更新预创建队伍的成员
void AutoTeamManager::incrUpdatePreMember( const CSAUTOTEAMPREADD& addMemberInfo )
{
	for ( int i = 0; i < addMemberInfo.Num; ++i )
	{
		addPreMember( addMemberInfo.PreMemInfos[i] );
	}
}

// 全量更新预创建队伍的成员
void AutoTeamManager::totalUpdatePreMember( const CSAUTOTEAMPREADD& addMemberInfo )
{
	m_preMemberInfo.m_preMemInfo.clear();
	for ( int i = 0; i < addMemberInfo.Num; ++i )
	{
		addPreMember( addMemberInfo.PreMemInfos[i] );
	}
}

// 添加新加入预创建队伍的成员
void AutoTeamManager::addPreMember( const CSAUTOTEAMPREMEMINFO& newPreMemberInfo )
{
	static_assert( sizeof( CSAUTOTEAMPREMEMINFO ) == sizeof( tagCSAutoTeamPreMemInfo_ToLua ) );
	//assert( m_preMemberInfo.m_nNum < MAX_TEAM_MEMBER );
	tagCSAutoTeamPreMemInfo_ToLua* preMemInfo = getPreMember( newPreMemberInfo.RoleName );
	if ( preMemInfo != NULL )
	{
		preMemInfo->AgreeStat		= newPreMemberInfo.AgreeStat;
		preMemInfo->Career			= newPreMemberInfo.Career;
		preMemInfo->Face			= newPreMemberInfo.Face;
		preMemInfo->Gender			= newPreMemberInfo.Gender;
		preMemInfo->Hair			= newPreMemberInfo.Hair;
		preMemInfo->Head			= newPreMemberInfo.Head;
		preMemInfo->Level			= newPreMemberInfo.Level;
	}
	else
	{
		const tagCSAutoTeamPreMemInfo_ToLua& preMemInfo = reinterpret_cast<const tagCSAutoTeamPreMemInfo_ToLua&>( 
			newPreMemberInfo );
		m_preMemberInfo.m_preMemInfo.push_back( preMemInfo );
		tagCSAutoTeamPreMemInfo_ToLua& oneMemberInfo = m_preMemberInfo.m_preMemInfo.back();
		ValidateMemberName( oneMemberInfo.RoleName, NAME_LEN );
	}
}

void AutoTeamManager::ValidateMemberName( char* szRoleName, int nMaxNameLen )
{
	/*const char* pszResult = strpbrk( szRoleName, BATTLE_NAME_SYMBOL );
	if ( pszResult == NULL )
	{
	return;
	}
	++pszResult;
	strncpy( szRoleName, pszResult, nMaxNameLen );*/
}

const char* AutoTeamManager::getValidateMemberName( const char* pszRoleName )
{
	/*static char s_roleName[NAME_LEN] = { 0 };
	strncpy( s_roleName, pszRoleName, NAME_LEN );
	ValidateMemberName( s_roleName, NAME_LEN );
	return s_roleName;*/

	return pszRoleName;
}

// 减少加入预创建队伍的成员
void AutoTeamManager::delPreMember( const char* pszName )
{
	for ( std::vector<tagCSAutoTeamPreMemInfo_ToLua>::iterator iter = m_preMemberInfo.m_preMemInfo.begin(), 
		end = m_preMemberInfo.m_preMemInfo.end(); iter != end; ++iter )
	{
		if ( CompareRoleName( iter->RoleName, pszName ) )
		{
			m_preMemberInfo.m_preMemInfo.erase( iter );
			break;
		}
	}

}

// 改变预创建队伍成员的准备状态
void AutoTeamManager::changePreMemberState( const char* pszName, unsigned char nState )
{
	tagCSAutoTeamPreMemInfo_ToLua* preMemInfo = getPreMember( pszName );
	if ( preMemInfo != NULL )
	{
		preMemInfo->AgreeStat = nState;
	}
}

// 更新自动组队队伍中的成员信息
void AutoTeamManager::updateAutoTeamMember( const CSAUTOTEAMSUCC& allMemberInfo )
{
	m_preMemberInfo.m_preMemInfo.clear();
	for ( int i = 0; i < allMemberInfo.Num; ++i )
	{
		const tagCSAutoTeamPreMemInfo_ToLua& preMemInfo = reinterpret_cast<const tagCSAutoTeamPreMemInfo_ToLua&>( 
			allMemberInfo.PreMemInfos[i] );
		m_preMemberInfo.m_preMemInfo.push_back( preMemInfo );

		tagCSAutoTeamPreMemInfo_ToLua& oneMemberInfo = m_preMemberInfo.m_preMemInfo.back();
		ValidateMemberName( oneMemberInfo.RoleName, NAME_LEN );
	}
}

int AutoTeamManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid != AUTOTEAM_SVR )
	{
		return 0;
	}

	const CSAUTOTEAMSVR& autoTeamSvr = msgbody.AutoTeamSvr;
	switch ( autoTeamSvr.AutoTeamSvrOp )
	{
	case AUTOTEAM_SVR_OP_CLEAN:
		{
			const CSAUTOTEAMCLEAN& autoTeamCleanData = autoTeamSvr.AutoTeamSvrData.AutoTeamClean;
			releaseAutoTeamInfo();
			GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_RESET" );
		}
		break;

	case AUTOTEAM_SVR_OP_PRE_ADD:	/* 预创建队伍队员加入 */
		{
			GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_PRE_MEM_PRE_ADD" );
			const CSAUTOTEAMPREADD& teamPreAddInfo = autoTeamSvr.AutoTeamSvrData.PreAdd;
			if ( teamPreAddInfo.Replace == 0 )
			{
				incrUpdatePreMember( teamPreAddInfo );
			}
			else
			{
				totalUpdatePreMember( teamPreAddInfo );
			}

			m_nAutoTeamID		= teamPreAddInfo.AutoTeamID;
			m_nAutoTeamState	= CLT_AUTO_TEAM_STATE_PREPARE;
			//m_preMemberInfo.m_isAnyInTeam = ( teamPreAddInfo.CreatedTeam == 1 );
			GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_PRE_MEM_ADD" );
		}
		break;

	case AUTOTEAM_SVR_OP_PRE_DEC:	/* 预创建队伍队员减少 */		
		{
			const CSAUTOTEAMPREDEC& teamPreDesInfo = autoTeamSvr.AutoTeamSvrData.PreDec;
			delPreMember( teamPreDesInfo.RoleName );
			GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_PRE_MEM_DEC" );
		}
		break;

	case AUTOTEAM_SVR_OP_PRE_AGREE:	/* 预创建队伍队员同意 */		
		{
			const CSAUTOTEAMPREAGREE& teamPreAgreeInfo = autoTeamSvr.AutoTeamSvrData.PreAgree;
			const char* pszRoleName = getValidateMemberName( teamPreAgreeInfo.RoleName );
			changePreMemberState( pszRoleName, CLT_PRE_MEMBER_AGREE );
			GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_PRE_MEM_STATE_CHG" );
		}
		break;

	case AUTOTEAM_SVR_OP_CANCEL:	/* 取消自动组队 */		
		{
			const CSAUTOTEAMCANCELSVR& teamPreCancelInfo = autoTeamSvr.AutoTeamSvrData.CancelSvr;
			m_preMemberInfo.m_preMemInfo.clear();
			m_nAutoTeamID		= NONE_AUTO_TEAM;
			m_nAutoTeamState	= CLT_AUTO_TEAM_STATE_NONE;
			char buffer[256];
			sprintf( buffer, "CancelAutoTeam(%d)", teamPreCancelInfo.CancelFlag );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;

	case AUTOTEAM_SVR_OP_SUCC:	/* 自动组队成功 */		
		{
			const CSAUTOTEAMSUCC& autoTeamSuccData = autoTeamSvr.AutoTeamSvrData.Succ;
			OnAutoTeamSucc( autoTeamSuccData );
		}
		break;

	case AUTOTEAM_SVR_OP_CREATE:	/* 完成组建通知 */		
		{
			const CSAUTOTEAMCREATESVR& teamPreAgreeInfo = autoTeamSvr.AutoTeamSvrData.Create;

			char buffer[256];
			const char* pszRoleName = getValidateMemberName( teamPreAgreeInfo.RoleName );
			sprintf( buffer, "InviteFinishingCreate(\"%s\")", pszRoleName );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;

	case AUTOTEAM_SVR_OP_CREATE_CANCEL:	/* 结束完成组建 */		
		{
			const CSAUTOTEAMCREATECANCELSVR& teamPreCancelInfo = autoTeamSvr.AutoTeamSvrData.CreateCancel;
			char buffer[256];
			const char* pszRoleName = getValidateMemberName( teamPreCancelInfo.RoleName );
			sprintf( buffer, "RefuseFinishingCreate(\"%s\")", pszRoleName );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;

	case AUTOTEAM_SVR_OP_INFO_SYNC:	/* 成功自动组队后,同步的自动组队相关信息 */
		{
			const CSAUTOTEAMENDSYNC& autoTeamEndSync = autoTeamSvr.AutoTeamSvrData.AutoTeamEndSync;
			m_nAutoTeamID		= autoTeamEndSync.AutoTeamID;
			m_nPworldDiffulty	= autoTeamEndSync.PworldGrade;
			if ( m_nAutoTeamID == 0 )
			{
				m_preMemberInfo.m_preMemInfo.clear();
			}
			GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_SYNC" );
		}
		break;

	default:
		break;
	}

	return 0;
}

/*  Ver.893 0表示正常流程 1表示tick */
enum
{
	SUCCESS_EVENT	= 0,
	TICK_EVENT		= 1,
};

void AutoTeamManager::OnAutoTeamSucc( const CSAUTOTEAMSUCC& autoTeamSuccData )
{
	updateAutoTeamMember( autoTeamSuccData );
	m_nAutoTeamID		= autoTeamSuccData.AutoTeamID;
	m_nAutoTeamState	= CLT_AUTO_TEAM_STATE_IN;
	m_nPworldDiffulty	= autoTeamSuccData.PworldGrade;
	if ( autoTeamSuccData.OperType == SUCCESS_EVENT )
	{
		// 删除上一次所请求成功的所有数据
		cancelAllSelectPWorld();
		cancelAllCareerAndCapRequest();
		GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_SUCCESS" );
	}
	else
	{
		GameManager::getSingleton().SendUIEvent( "GE_AUTO_TEAM_SYNC_SUCC" );
	}
}