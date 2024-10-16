#include "GameIntelliRaceMgr.h"

GameIntelliRaceMgr::GameIntelliRaceMgr(void)
{
	memset(&m_SubJectCommitInfo, 0, sizeof(tagSubJectCommit_ToLua));
	INetworkManager::getSingleton().RegisterMsgHandler( SUBJECT_SVR, this );
}
GameIntelliRaceMgr::~GameIntelliRaceMgr(void)
{
	
}
int GameIntelliRaceMgr::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == SUBJECT_SVR )
	{
		const CSSUBJECTSVR& subjectSvr = msgbody.SubJectSvr.Data;
		//MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		switch ( msgbody.SubJectSvr.Cmd)
		{
		case SUBJECT_SVR_OP_INVITE:
			{
				int startTime = subjectSvr.Invite.StartTime;
				int isRaceEnd = subjectSvr.Invite.Invite;
				char szBuf[256];
				//显示竞赛提示框
				sprintf( szBuf,"RaceInvitationShow(%d ,%d)",startTime,isRaceEnd);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}	
			break;
		case SUBJECT_SVR_OP_FLUSH:
			{
				m_SubjectFlush = subjectSvr.Flush;
				char szBuf[256];
				//更新竞赛面板信息
				sprintf( szBuf, "UpdateRaceUIInfo()");
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}	
			break;
		}
	}
	return 0;
}

void GameIntelliRaceMgr::IntelligenceRaceJoinAgree()
{
	tagCSPkgBody msgbody;
	msgbody.SubJectClt.Cmd = SUBJECT_CLT_OP_JOIN;
	SendGameServerMsg( SUBJECT_CLT, msgbody );
}
void GameIntelliRaceMgr::IntelliRaceAnswerCommit()
{
	tagCSPkgBody msgbody;
	msgbody.SubJectClt.Cmd = SUBJECT_CLT_OP_COMMIT;
	msgbody.SubJectClt.Data.Commit.ID		= m_SubJectCommitInfo.ID;
	msgbody.SubJectClt.Data.Commit.Idx		= m_SubJectCommitInfo.Idx;
	msgbody.SubJectClt.Data.Commit.IsStar	= m_SubJectCommitInfo.IsStar;
	msgbody.SubJectClt.Data.Commit.KeyIdx	= m_SubJectCommitInfo.KeyIdx;
	SendGameServerMsg( SUBJECT_CLT, msgbody );
}
void GameIntelliRaceMgr::IntelliRaceAnswerQuit()
{
	tagCSPkgBody msgbody;
	msgbody.SubJectClt.Cmd = SUBJECT_CLT_OP_EXIT;
	SendGameServerMsg( SUBJECT_CLT, msgbody );
}
tagSubJectCommit_ToLua* GameIntelliRaceMgr::getSubjectCommitInfo()
{
	return &m_SubJectCommitInfo;
}
tagSubJectFlush_ToLua* GameIntelliRaceMgr::getSubJectFlushInfo()
{
	static_assert( sizeof(tagSubJectFlush_ToLua) == sizeof(tagSubJectFlush) );		
	
	return (tagSubJectFlush_ToLua*)&m_SubjectFlush;
}
const tagSubJectDef_ToLua* getSubjectDef( int id )
{
	static_assert( sizeof(tagSubJectDef) == sizeof(tagSubJectDef_ToLua) );
	const tagSubJectDef* pSubjectDef =  DBDataManager::getSingleton().getSubjectDef( id ) ;
	//tagSubJectDef_ToLua* pLuaSubjectDef = (tagSubJectDef_ToLua*)( pSubjectDef );
	return (const tagSubJectDef_ToLua*)( pSubjectDef );
}
