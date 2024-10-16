#include "GameClanManager.h"
#include "GameExchange.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include <algorithm>

// 创建公会
void GameClanManager::clan_Opt_Create( const char* szClanName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_CREATE;
	strncpy( msgbody.ClanClt.ClanCltData.ClanCreateClanName, szClanName, strlen( szClanName ) + 1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 添加一个成员
void GameClanManager::clan_Opt_Invite( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_INVITE_JOIN_CLAN;
	strncpy( msgbody.ClanClt.ClanCltData.ClanInviteName, szName, strlen( szName ) + 1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 升级公会
void GameClanManager::clan_Opt_Upgrade( )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_UPGRADE;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 查看当前公会城市
void GameClanManager::clan_Opt_SeeClanCity()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_CITY_INFO;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 删除一个成员
void GameClanManager::clan_Opt_DelMember( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_DEL_MEMBER;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanDelMemberGID.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanDelMemberGID.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 退出公会
void GameClanManager::clan_Opt_Exit()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_EXIT_CLAN;
	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// void GameClanManager::clan_Opt_SetJobAndTitle()
// {
	//TODO HERE
// 	tagCSPkgBody msgbody;
// 	msgbody.ClanClt.Operation = CLAN_OP_SET_JOB_AND_TITLE;
// 	msgbody.ClanClt.ClanCltData.ClanSetJobAndTitle.Position = ;
// 	msgbody.ClanClt.ClanCltData.ClanSetJobAndTitle.RoleID = ;
// 	msgbody.ClanClt.ClanCltData.ClanSetJobAndTitle.Title = ;
// }

// 提升成员阶级
void GameClanManager::clan_Opt_UpPostion( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_UP_POSITION;	//TODO HERE 这里需要修改
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanUpPosRoleID.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanUpPosRoleID.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 降低成员阶级
void GameClanManager::clan_Opt_DownPostion( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_DOWN_POSITION;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanDownPosRoleID.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanDownPosRoleID.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 解散公会
void GameClanManager::clan_Opt_Disban()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_DISBAND;
	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 编辑公会
void GameClanManager::clan_Opt_EditBulletin( const char* szText )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_EDIT_BULLETIN;
	strncpy( msgbody.ClanClt.ClanCltData.ClanBulletinText, szText, strlen( szText )+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 编辑宣言
void GameClanManager::clan_Opt_EditManifesto( const char* szText )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_EDIT_MANIFESTO;
	strncpy( msgbody.ClanClt.ClanCltData.ClanManifestoText, szText, strlen( szText )+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 让位
void GameClanManager::clan_DemiseJob( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_DEMISE_JOB;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanDemiseJobRoleID.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanDemiseJobRoleID.RoleName, szName, strlen( szName )+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 申请入会
void GameClanManager::clan_Opt_ApplyJoin( int nHeight, int nLow, int nId, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_APPLY_JOIN_CLAN;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanJoinID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanJoinID.Name, szName, strlen( szName )+1 );
	msgbody.ClanClt.ClanCltData.ClanJoinID.Id = nId;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 入会审查  vote:0表示反对，1表示同意
void GameClanManager::clan_Opt_CensorJoin( int nHeight, int nLow, const char* szName, int nVote )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_CENSOR_JOIN_CLAN;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanCensorJoinClan.ID.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanCensorJoinClan.ID.RoleName, szName, strlen( szName )+1 );
	msgbody.ClanClt.ClanCltData.ClanCensorJoinClan.Vote = nVote;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 设置昵称
void GameClanManager::clan_Opt_SetNickName( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SET_MEMBER_NICKNAME;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanMemberNickname.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanMemberNickname.Nickname, szName, strlen( szName )+1 );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 查看入会请求列表
void GameClanManager::clan_Opt_LookJoinList()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_APPLY_JOIN_LIST;
	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 邀请加入同盟
void GameClanManager::clan_Opt_InviteJoinUnion( int nHeight, int nLow, int Id, const char* szClanName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_INVITE_JOIN_UNION;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanJoinUnionCustomID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanJoinUnionCustomID.Name, szClanName, strlen( szClanName )+1 );
	msgbody.ClanClt.ClanCltData.ClanJoinUnionCustomID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 公会申请加入同盟
void GameClanManager::clan_Opt_ApplyJoinUnion( int nHeight, int nLow, int Id, const char* szClanName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_APPLY_JOIN_UNION;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanJoinUnionHostID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanJoinUnionHostID.Name, szClanName, strlen( szClanName )+1 );
	msgbody.ClanClt.ClanCltData.ClanJoinUnionHostID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 退出同盟
void GameClanManager::clan_Opt_ExitUnion( int nHeight, int nLow, int Id, const char* szClanName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_EXIT_UNION;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanExitUnionID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanExitUnionID.Name, szClanName, strlen( szClanName )+1 );
	msgbody.ClanClt.ClanCltData.ClanExitUnionID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 添加敌对公会
void GameClanManager::clan_Opt_ApplyJoinEnemy( int nHeight, int nLow, int Id, const char* szClanName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_APPLY_JOIN_ENEMY;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanJoinEnemyHostID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanJoinEnemyHostID.Name, szClanName, strlen( szClanName )+1 );
	msgbody.ClanClt.ClanCltData.ClanJoinEnemyHostID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 删除敌对公会
void GameClanManager::clan_Opt_ExitEnemy( int nHeight, int nLow, int Id, const char* szClanName )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_EXIT_ENEMY;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanExitEnemyID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanExitEnemyID.Name, szClanName, strlen( szClanName )+1 );
	msgbody.ClanClt.ClanCltData.ClanExitEnemyID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 查看公会的外交状态
void GameClanManager::clan_Opt_LookDiplomatism()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_DIPLOMATISM_STATUS;
	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 回复入会邀请 ask 0 不同意 1同意
void GameClanManager::clan_Opt_AnswerJoinClan( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_ANSWER_JOIN_CLAN;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanAnswerJoinClan.ID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanAnswerJoinClan.ID.Name, szClanName, strlen( szClanName )+1 );
	strncpy( msgbody.ClanClt.ClanCltData.ClanAnswerJoinClan.HostName, szHostName, strlen( szHostName )+1 );
	msgbody.ClanClt.ClanCltData.ClanAnswerJoinClan.ID.Id = Id;
	msgbody.ClanClt.ClanCltData.ClanAnswerJoinClan.ACK	 = nAsk;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 回复入盟邀请 szHostName 请求人的名字，ask 0 不同意 1同意
void GameClanManager::clan_Opt_AnserJoinUnion( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_ANSWER_JOIN_UNION;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanAnswerJoinUnio.ID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanAnswerJoinUnio.ID.Name, szClanName, strlen( szClanName )+1 );
	strncpy( msgbody.ClanClt.ClanCltData.ClanAnswerJoinUnio.HostName, szHostName, strlen( szHostName )+1 );
	msgbody.ClanClt.ClanCltData.ClanAnswerJoinUnio.ID.Id = Id;
	msgbody.ClanClt.ClanCltData.ClanAnswerJoinUnio.ACK	 = nAsk;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 回复敌对请求 szHostName 请求人的名字，ask 0 不同意 1同意
void GameClanManager::clan_Opt_AnswerJoinEnemy( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_ANSWER_JOIN_ENEMY;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanAnswerJoinEnemy.ID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanAnswerJoinEnemy.ID.Name, szClanName, strlen( szClanName )+1 );
	strncpy( msgbody.ClanClt.ClanCltData.ClanAnswerJoinEnemy.HostName, szHostName, strlen( szHostName )+1 );
	msgbody.ClanClt.ClanCltData.ClanAnswerJoinEnemy.ID.Id = Id;
	msgbody.ClanClt.ClanCltData.ClanAnswerJoinEnemy.ACK	 = nAsk;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 搜索公会 nKeyId 为32位的公会ID
void GameClanManager::clan_Opt_SearchClan( int nKeyId )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SEARCH;
	msgbody.ClanClt.ClanCltData.ClanKeyID = nKeyId;
	SendGameServerMsg( CLAN_CLT, msgbody );
	m_QueryClanInfoList.clear();
}

// 用ID来搜索公会
void GameClanManager::clan_Opt_SearchClanByID( int nKeyId )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_FIND_REQ;
	msgbody.ClanClt.ClanCltData.ClanFindReq.ByWhat = FIND_CLAN_BY_ID;
	msgbody.ClanClt.ClanCltData.ClanFindReq.Key.ID = nKeyId;
	SendGameServerMsg( CLAN_CLT, msgbody );
	m_QueryClanInfoList.clear();
}

// 用名字来搜索公会
void GameClanManager::clan_Opt_SearchClanByName( const char* pszName )
{
	m_QueryClanInfoList.clear();
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( strstr( m_otherClanVec[i].m_ClanInfo.m_szClanName,pszName ) != NULL )
		{
			m_QueryClanInfoList.push_back( m_otherClanVec[i] );
		}
	}
	std::stable_sort( m_QueryClanInfoList.begin(),m_QueryClanInfoList.end(),otherClanCompare );
	GameManager::getSingleton().getScriptVM()->callString( "updataSearchClan()" );
}

void GameClanManager::clan_Opt_SearchClanByWID( tdr_ulonglong wid )
{
	m_QueryClanInfoList.clear();
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( memcmp( &m_otherClanVec[i].m_ClanInfo.m_clanLongId, &wid, sizeof(tdr_ulonglong) ) == 0 )
		{
			m_QueryClanInfoList.push_back( m_otherClanVec[i] );
		}
	}
	std::stable_sort( m_QueryClanInfoList.begin(),m_QueryClanInfoList.end(),otherClanCompare );
	GameManager::getSingleton().getScriptVM()->callString( "updataSearchClan()" );
}

const char* GameClanManager::clanGetClanNameByWid( tdr_ulonglong wid)
{
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( memcmp( &m_otherClanVec[i].m_ClanInfo.m_clanLongId, &wid, sizeof(tdr_ulonglong) ) == 0 )
		{
			return m_otherClanVec[i].m_ClanInfo.m_szClanName;
		}
	}
	return "";
}
const char* GameClanManager::clanGetClanNameByID( int clanID )
{
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( m_otherClanVec[i].m_ClanInfo.m_uClanId == clanID )
		{
			return m_otherClanVec[i].m_ClanInfo.m_szClanName;
		}
	}
	return "";
}
int GameClanManager::GetClanMemNumByWid( tdr_ulonglong wid )
{
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( memcmp( &m_otherClanVec[i].m_ClanInfo.m_clanLongId, &wid, sizeof(tdr_ulonglong) ) == 0 )
		{
			return m_otherClanVec[i].m_ClanInfo.m_nClanMemNum;
		}
	}
	return 0;
}

int GameClanManager::GetClanLVByWid( tdr_ulonglong wid )
{
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( memcmp( &m_otherClanVec[i].m_ClanInfo.m_clanLongId, &wid, sizeof(tdr_ulonglong) ) == 0 )
		{
			return m_otherClanVec[i].m_ClanInfo.m_nClanLevel;
		}
	}
	return 0;
}
const char* GameClanManager::GetClanCaptionNameByWid( tdr_ulonglong wid )
{
	for( size_t i = 0 ; i < m_otherClanVec.size(); i ++ )
	{
		if( memcmp( &m_otherClanVec[i].m_ClanInfo.m_clanLongId, &wid, sizeof(tdr_ulonglong) ) == 0 )
		{
			return m_otherClanVec[i].m_ClanInfo.m_szCaptionName;
		}
	}
	return "";
}
// 审查申请入盟的公会
void GameClanManager::clan_Opt_CensorJoinUnion( int nHeight, int nLow, int Id, const char* szName, int nVote )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_CENSOR_JOIN_UNION;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanCensorJoinUnion.ID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanCensorJoinUnion.ID.Name, szName, strlen( szName )+1 );
	msgbody.ClanClt.ClanCltData.ClanCensorJoinUnion.Vote = nVote;
	msgbody.ClanClt.ClanCltData.ClanCensorJoinUnion.ID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 审查申请敌对的公会
void GameClanManager::clan_Opt_CensorJoinEnemy( int nHeight, int nLow, int Id, const char* szName, int nVote )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_CENSOR_JOIN_ENEMY;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanCensorJoinEnemy.ID.GID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.ClanClt.ClanCltData.ClanCensorJoinEnemy.ID.Name, szName, strlen( szName )+1 );
	msgbody.ClanClt.ClanCltData.ClanCensorJoinEnemy.Vote = nVote;
	msgbody.ClanClt.ClanCltData.ClanCensorJoinEnemy.ID.Id = Id;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 查看所有成员信息快照
void GameClanManager::clan_Opt_GetMemberSnapshot()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_MEMBER_SNAPSHOT;
	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 要求查看公会的一般信息
void GameClanManager::clan_Opt_GetClanInfo()
{
// 	tagCSPkgBody msgbody;
// 	msgbody.ClanClt.Operation = CLAN_OP_INFO;
// 	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
// 	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 查看公会的宣言
void GameClanManager::clan_Opt_GetManifesto()
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_MANIFESTO;
	memset( &msgbody.ClanClt.ClanCltData, 0, sizeof(msgbody.ClanClt.ClanCltData) );
	SendGameServerMsg( CLAN_CLT, msgbody );
}

// 查看服务器一页的公会
void GameClanManager::clan_Opt_GetPageClan( int nPage )
{
}

void GameClanManager::clan_Opt_Research( int nCat,int nLevel )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_RESEARCH;
	msgbody.ClanClt.ClanCltData.ClanResearch.Category = nCat;
	msgbody.ClanClt.ClanCltData.ClanResearch.Level = nLevel;
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_CancleResearch(int nIndex)
{
	if( nIndex < m_ClanAdvace.Tech.TechResearchNum )
	{
		tagCSPkgBody msgbody;
		msgbody.ClanClt.Operation = CLAN_OP_CANCEL_RESEARCH;
		msgbody.ClanClt.ClanCltData.ClanCancelResearch.ProgressPos = nIndex;
		SendGameServerMsg(CLAN_CLT,msgbody);
	}
}

void GameClanManager::clan_Opt_Make(int nItemID,int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_MAKE;
	msgbody.ClanClt.ClanCltData.ClanMake.Category = CLAN_MAKE_FOR_ARM;
	msgbody.ClanClt.ClanCltData.ClanMake.ItemOrMachineID = nItemID;
	msgbody.ClanClt.ClanCltData.ClanMake.Num = nNum;
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_CancleMake(int nItemID )
{
	for( int i = 0 ; i < m_ClanAdvace.Produce.ArmMakingNum ; i++ )
	{
		if( m_ClanAdvace.Produce.ArmMakingProgress[i].ID == nItemID )
		{
			tagCSPkgBody msgbody;
			msgbody.ClanClt.Operation = CLAN_OP_CANCEL_MAKING;
			msgbody.ClanClt.ClanCltData.ClanCancelMaking.Category = CLAN_MAKE_FOR_ARM;
			msgbody.ClanClt.ClanCltData.ClanCancelMaking.ProgressPos = i;
			SendGameServerMsg(CLAN_CLT,msgbody);
			break;
		}
	}
}

void GameClanManager::clan_Opt_Machine_Grab(int nPos)
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_GRAB_MACHINE;
	msgbody.ClanClt.ClanCltData.ClanGrabMachine.StorageGridPos = nPos;
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_MachineDestroy( int nPos,int nCat )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_STORAGE_GRID_DESTROY;
	msgbody.ClanClt.ClanCltData.ClanStorageGridDestroy.GridIdentifier.GridPos = nPos;
	msgbody.ClanClt.ClanCltData.ClanStorageGridDestroy.GridIdentifier.Category = nCat;
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_Machine_Lock( int nPos ,int nCat, int nState)
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_STORAGE_GRID_LOCK_SWITCH;
	msgbody.ClanClt.ClanCltData.ClanStorageGridLockSwitch.LockState = nState;
	msgbody.ClanClt.ClanCltData.ClanStorageGridLockSwitch.GridIdentifier.GridPos = nPos;
	msgbody.ClanClt.ClanCltData.ClanStorageGridLockSwitch.GridIdentifier.Category = nCat;
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_Answer_Exit_Enemy( bool bAgree )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_ANSWER_EXIT_ENEMY;
	memcpy(&msgbody.ClanClt.ClanCltData.ClanAnswerExitEnemy.ID,&m_EnemyID,sizeof(CLANID));
	strcpy(msgbody.ClanClt.ClanCltData.ClanAnswerExitEnemy.HostName,m_szEnemyHostName);
	msgbody.ClanClt.ClanCltData.ClanAnswerExitEnemy.ACK = bAgree;
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_Set_Job_And_Title(int pos,int nLowID,int nHightID,char* szTitle)
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SET_JOB_AND_TITLE;
	msgbody.ClanClt.ClanCltData.ClanSetJobAndTitle.Position = pos;
	tagLong_ToLua roleID;
	roleID.nLow = nLowID;
	roleID.nHeight = nHightID;
	memcpy(&msgbody.ClanClt.ClanCltData.ClanSetJobAndTitle,&roleID,sizeof(tagLong_ToLua));
	strcpy(msgbody.ClanClt.ClanCltData.ClanSetJobAndTitle.Title,szTitle);
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::clan_Opt_Skill_Speed_Up(int nCat,int nTime)
{
	for( int i = 0 ; i < m_ClanAdvace.Tech.TechResearchNum ; i++ )
	{
		if( m_ClanAdvace.Tech.TechResearchProgress[i].Category == nCat )
		{
	 		tagCSPkgBody msgbody;
	 		msgbody.ClanClt.Operation = CLAN_OP_RESEARCH_SPEEDUP;
	 		msgbody.ClanClt.ClanCltData.ClanResearchSpeedup.ProgressPos = i;
	 		msgbody.ClanClt.ClanCltData.ClanResearchSpeedup.DecreasingTime = nTime*60;
	 		SendGameServerMsg(CLAN_CLT,msgbody);
			return;
		}
	}
}

void GameClanManager::clan_Opt_Product_Speed_Up(int nID,int nTime)
{
	for( int i = 0 ; i < m_ClanAdvace.Produce.ArmMakingNum ; i++ )
	{
		if( m_ClanAdvace.Produce.ArmMakingProgress[i].ID == nID )
		{
			tagCSPkgBody msgbody;
			msgbody.ClanClt.Operation = CLAN_OP_MAKING_SPEEDUP;
			msgbody.ClanClt.ClanCltData.ClanMakingSpeedup.Category = CLAN_MAKE_FOR_ARM;
			msgbody.ClanClt.ClanCltData.ClanMakingSpeedup.ProgressPos = i;
			msgbody.ClanClt.ClanCltData.ClanMakingSpeedup.DecreasingTime = nTime*60;
			SendGameServerMsg( CLAN_CLT,msgbody );
			break;
		}
	}
}
void GameClanManager::clan_Opt_SetClanColor(unsigned int nClanID,unsigned int nClanColor)
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation=CLAN_OP_SET_COLOR;
	msgbody.ClanClt.ClanCltData.ClanSetColor.Set.ClanID = nClanID;
	msgbody.ClanClt.ClanCltData.ClanSetColor.Set.Color = nClanColor;
	SendGameServerMsg( CLAN_CLT,msgbody );
}
void GameClanManager::clan_Opt_PworldAward()
{
	tagCSPkgBody msgbody;
	msgbody.ActiveGameClt.Type = ACTIVE_GAME_CLT_TYPE_AWARD;
	msgbody.ActiveGameClt.Data.Award.UIType = ACTIVE_UI_TYPE_CLAN_PWORLD;
	SendGameServerMsg( ACTIVE_GAME_CLT,msgbody );
}

void GameClanManager::clan_Opt_EnterCity(int clanId)
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE );
		return;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD );
		return;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_STALL );
		return;
	}
	if ( pMainplayer->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ENTER_CLANISLAND_IN_FIGHT );
		return;
	}
	if( pMainplayer->isInRide() && !pMainplayer->m_Rider.isOwnRide() )
	{
		pMainplayer->m_Rider.requestRideDown();
	}
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_ENTER_CLAN_CITY;
	msgbody.PworldClt.CltData.EnterClanCity.ClanID = clanId;
	msgbody.PworldClt.CltData.EnterClanCity.ClanSpyTransDefID = 0;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

void GameClanManager::clan_Opt_EnterClanPworldIsland()
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE );
		return;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD );
		return;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_STALL );
		return;
	}
	if( pMainplayer->isOnMonsterMachine() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_ENTER_CLANISLAND_IN_MACHINE);
		return;
	}
	if ( pMainplayer->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ENTER_CLANISLAND_IN_FIGHT );
		return;
	}
	if( pMainplayer->isInRide() && !pMainplayer->m_Rider.isOwnRide() )
	{
		pMainplayer->m_Rider.requestRideDown();
	}
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_CLAN_EVERYDAY_PWORLD_ENTER;
	SendGameServerMsg(PLAYER_MISC_CLT,msgbody);
}
//传送到公会城市情报官处
void GameClanManager::Clan_Opt_GoToClanIntellingenceNPC()
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE );
		return;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD );
		return;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_STALL );
		return;
	}
	if( pMainplayer->isOnMonsterMachine() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_ENTER_CLANISLAND_IN_MACHINE);
		return;
	}
	if ( pMainplayer->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ENTER_CLANISLAND_IN_FIGHT );
		return;
	}
	if( pMainplayer->isInRide() && !pMainplayer->m_Rider.isOwnRide() )
	{
		pMainplayer->m_Rider.requestRideDown();
	}
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_ENTER_CLAN_CITY;
	msgbody.PworldClt.CltData.EnterClanCity.ClanSpyTransDefID = SPY_TASK_NPC_TRANS_DEFID;
	msgbody.PworldClt.CltData.EnterClanCity.ClanID = pMainplayer->getClanID();
	SendGameServerMsg(PWORLD_CLT,msgbody);
}
//传送到指定公会的指定位置
void GameClanManager::Clan_Opt_GoToClanCityPos( int ClanID, int PosID )
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE );
		return;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD );
		return;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_STALL );
		return;
	}
	if( pMainplayer->isOnMonsterMachine() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_ENTER_CLANISLAND_IN_MACHINE);
		return;
	}
	if ( pMainplayer->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ENTER_CLANISLAND_IN_FIGHT );
		return;
	}
	if( pMainplayer->isInRide() && !pMainplayer->m_Rider.isOwnRide() )
	{
		pMainplayer->m_Rider.requestRideDown();
	}
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_ENTER_CLAN_CITY;
	msgbody.PworldClt.CltData.EnterClanCity.ClanSpyTransDefID = PosID;
	msgbody.PworldClt.CltData.EnterClanCity.ClanID = ClanID;
	SendGameServerMsg(PWORLD_CLT,msgbody);
}
void GameClanManager::clan_Opt_SetMapPos( int nMapId, int nX, int nY, int nType )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SET_MAP_POS;
	msgbody.ClanClt.ClanCltData.ClanSetMapPos.MapID = nMapId;
	msgbody.ClanClt.ClanCltData.ClanSetMapPos.Pos.X = nX;
	msgbody.ClanClt.ClanCltData.ClanSetMapPos.Pos.Y = nY;
	msgbody.ClanClt.ClanCltData.ClanSetMapPos.Type = nType;
	SendGameServerMsg( CLAN_CLT,msgbody );
}
void GameClanManager::Clan_Opt_EnterClanMachinePworld( int ClanID )
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE );
		return;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD );
		return;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTO_TEAM_FAIL_TRAN_BY_STALL );
		return;
	}
	if( pMainplayer->isOnMonsterMachine() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_ENTER_CLANISLAND_IN_MACHINE);
		return;
	}
	if ( pMainplayer->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ENTER_CLANISLAND_IN_FIGHT );
		return;
	}
	if( pMainplayer->isInRide() && !pMainplayer->m_Rider.isOwnRide() )
	{
		pMainplayer->m_Rider.requestRideDown();
	}
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_CLAN_MACHINE_PWORLD_ENTER;
	msgbody.PlayerMiscClt.PlayerMiscCltData.ClanMachinePworldEnter.ClanID = ClanID;
	SendGameServerMsg(PLAYER_MISC_CLT,msgbody);
}

void GameClanManager::Clan_Opt_SkillResearch( int id, int level )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SKILL_RESEARCH;
	msgbody.ClanClt.ClanCltData.ClanSkillResearch.SkillID = id;
	msgbody.ClanClt.ClanCltData.ClanSkillResearch.Level = level;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

void GameClanManager::Clan_Opt_SkillLearn( int id, int level )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SKILL_LEARN;
	msgbody.ClanClt.ClanCltData.ClanSkillResearch.SkillID = id;
	msgbody.ClanClt.ClanCltData.ClanSkillResearch.Level = level;
	SendGameServerMsg( CLAN_CLT, msgbody );
}
void GameClanManager::Clan_Opt_AskGetResInfo( int nPage )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_VIEW_GETRES_INFO;
	msgbody.ClanClt.ClanCltData.ViewGetResClt.Page = nPage;
	SendGameServerMsg( CLAN_CLT, msgbody );
}
void GameClanManager::ClearClanViewGetRes()
{
	memset( &m_ClanViewGetRes, 0, sizeof(tagClanViewGetRes_ToLua) );
}

bool GameClanManager::isInClanGroup()
{
	MainGamePlayer *pMain = ActorManager::getSingleton().getMainPlayer();
	if ( pMain->isInClanGroup() )
	{
		return true;
	}
	return false;
}