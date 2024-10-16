#include "GameSchool.h"
#include "ClanInfo.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameDBData.h"

GameSchool::GameSchool()
{
	INetworkManager::getSingleton().RegisterMsgHandler( SCHOOL_SVR, this );
	memset( &m_RegInfo, 0, sizeof( m_RegInfo ) );
	m_SelfSchoolJobType = 0;
	m_SelfStudentNum = 0;
	memset( &m_SelfStudents, 0, sizeof( m_SelfStudents ) );	
	memset( &m_SelfTeacher, 0, sizeof( m_SelfTeacher ) );
}

GameSchool::~GameSchool()
{
}

int GameSchool::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == SCHOOL_SVR )
	{
		if ( msgbody.SchoolSvr.Type == SCHOOL_SVR_ALL_INFO )
		{
			m_SelfSchoolJobType = msgbody.SchoolSvr.Data.AllInfo.JobType;
			if ( msgbody.SchoolSvr.Data.AllInfo.JobType == ROLE_SCHOOL_TYPE_NONE )
			{
				memset( &m_SelfStudents, 0, sizeof( m_SelfStudents ) );	
				memset( &m_SelfTeacher, 0, sizeof( m_SelfTeacher ) );
			}
			else if ( msgbody.SchoolSvr.Data.AllInfo.JobType == ROLE_SCHOOL_TYPE_TEACHER )
			{
				m_SelfStudentNum = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Num;
				for ( int i = 0; i < msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Num; i++ )
				{
					strncpy( m_SelfStudents[i].RoleName, msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].RoleName, sizeof(msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].RoleName)+1 );
					strncpy( m_SelfStudents[i].ClanName, msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].ClanName, sizeof(msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].ClanName)+1 );
					m_SelfStudents[i].Career = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Career;
					m_SelfStudents[i].DayTime = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].DayTime;
					m_SelfStudents[i].Face = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Face;
					m_SelfStudents[i].Gender = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Gender;
					m_SelfStudents[i].Hair = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Hair;
					m_SelfStudents[i].Head = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Head;
					m_SelfStudents[i].JoinLevel = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].JoinLevel;
					m_SelfStudents[i].JoinTime = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].JoinTime;
					m_SelfStudents[i].Level = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Level;
					m_SelfStudents[i].Map = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Map;
					m_SelfStudents[i].MeiLi = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].MeiLi;
					m_SelfStudents[i].Online = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].Online;
					m_SelfStudents[i].TeamNum = msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].TeamNum;
					memcpy( &m_SelfStudents[i].RoleID,&msgbody.SchoolSvr.Data.AllInfo.Data.Teacher.Students[i].RoleID, sizeof(tdr_ulonglong) );
				}
				memset( &m_SelfTeacher, 0, sizeof( m_SelfTeacher ) );
			}
			else if ( msgbody.SchoolSvr.Data.AllInfo.JobType == ROLE_SCHOOL_TYPE_STUDENT )
			{
				strncpy( m_SelfTeacher.Teacher.RoleName, msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.RoleName, sizeof(msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.RoleName)+1 );
				strncpy( m_SelfTeacher.Teacher.ClanName, msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.ClanName, sizeof(msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.ClanName)+1 );
				m_SelfTeacher.AwardCount = msgbody.SchoolSvr.Data.AllInfo.Data.Student.AwardCount;
				m_SelfTeacher.TotalCount = msgbody.SchoolSvr.Data.AllInfo.Data.Student.TotalCount;
				m_SelfTeacher.Exp = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Exp;
				m_SelfTeacher.Val = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Val;
				m_SelfTeacher.Teacher.Career = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Career;
				m_SelfTeacher.Teacher.Face = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Face;
				m_SelfTeacher.Teacher.Gender = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Gender;
				m_SelfTeacher.Teacher.Hair = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Hair;
				m_SelfTeacher.Teacher.Head = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Head;
				m_SelfTeacher.Teacher.JoinLevel = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.JoinLevel;
				m_SelfTeacher.Teacher.JoinTime = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.JoinTime;
				m_SelfTeacher.Teacher.Level = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Level;
				m_SelfTeacher.Teacher.Map = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Map;
				m_SelfTeacher.Teacher.MeiLi = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.MeiLi;
				m_SelfTeacher.Teacher.Online = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.Online;
				m_SelfTeacher.Teacher.TeamNum = msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.TeamNum;
				memcpy( &m_SelfTeacher.Teacher.RoleID,&msgbody.SchoolSvr.Data.AllInfo.Data.Student.Teacher.RoleID, sizeof(tdr_ulonglong) );

				memset( &m_SelfStudents, 0, sizeof( m_SelfStudents ) );	
			}

			GameManager::getSingleton().getScriptVM()->callString( "UpdateSchoolRelationShip()" );
		}
		else if ( msgbody.SchoolSvr.Type == SCHOOL_SVR_INVITE_UI )
		{
			GameManager::getSingleton().getScriptVM()->callString( "OpenSchoolInviteFrame()" );
		}
		else if ( msgbody.SchoolSvr.Type == SCHOOL_SVR_JOIN_UI )
		{
			char buffer[256];
			sprintf( buffer, "OpenSchoolJoinFrame( \"%s\" )", msgbody.SchoolSvr.Data.JoinUI.RoleName );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		else if ( msgbody.SchoolSvr.Type == SCHOOL_SVR_MEM_INFO )
		{
			if ( msgbody.SchoolSvr.Data.MemInfo.Type == SCHOOL_MEM_TYPE_TEACHER )
			{
				strncpy( m_SelfTeacher.Teacher.RoleName, msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.RoleName, sizeof(msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.RoleName)+1 );
				strncpy( m_SelfTeacher.Teacher.ClanName, msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.ClanName, sizeof(msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.ClanName)+1 );
				m_SelfTeacher.Teacher.Career = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Career;
				m_SelfTeacher.Teacher.Face = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Face;
				m_SelfTeacher.Teacher.Gender = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Gender;
				m_SelfTeacher.Teacher.Hair = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Hair;
				m_SelfTeacher.Teacher.Head = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Head;
				m_SelfTeacher.Teacher.JoinLevel = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.JoinLevel;
				m_SelfTeacher.Teacher.JoinTime = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.JoinTime;
				m_SelfTeacher.Teacher.Level = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Level;
				m_SelfTeacher.Teacher.Map = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Map;
				m_SelfTeacher.Teacher.MeiLi = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.MeiLi;
				m_SelfTeacher.Teacher.Online = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.Online;
				m_SelfTeacher.Teacher.TeamNum = msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.TeamNum;
				memcpy( &m_SelfTeacher.Teacher.RoleID,&msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.RoleID, sizeof(tdr_ulonglong) );
				char buffer[256];
				sprintf( buffer, "OpenSchoolInfo( \"%s\", %d, %d )", msgbody.SchoolSvr.Data.MemInfo.Data.Teacher.RoleName, m_SelfTeacher.Teacher.RoleID.nLow, m_SelfTeacher.Teacher.RoleID.nHeight );
				GameManager::getSingleton().getScriptVM()->callString( buffer );

			}
			else if ( msgbody.SchoolSvr.Data.MemInfo.Type == SCHOOL_MEM_TYPE_STUDENT )
			{
				for ( int i = 0; i< m_SelfStudentNum; i++ )
				{
					if ( strcmp( m_SelfStudents[i].RoleName, msgbody.SchoolSvr.Data.MemInfo.Data.Student.RoleName ) == 0 )
					{
						strncpy( m_SelfStudents[i].RoleName, msgbody.SchoolSvr.Data.MemInfo.Data.Student.RoleName, sizeof(msgbody.SchoolSvr.Data.MemInfo.Data.Student.RoleName)+1 );
						strncpy( m_SelfStudents[i].ClanName, msgbody.SchoolSvr.Data.MemInfo.Data.Student.ClanName, sizeof(msgbody.SchoolSvr.Data.MemInfo.Data.Student.ClanName)+1 );
						m_SelfStudents[i].Career = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Career;
						m_SelfStudents[i].DayTime = msgbody.SchoolSvr.Data.MemInfo.Data.Student.DayTime;
						m_SelfStudents[i].Face = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Face;
						m_SelfStudents[i].Gender = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Gender;
						m_SelfStudents[i].Hair = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Hair;
						m_SelfStudents[i].Head = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Head;
						m_SelfStudents[i].JoinLevel = msgbody.SchoolSvr.Data.MemInfo.Data.Student.JoinLevel;
						m_SelfStudents[i].JoinTime = msgbody.SchoolSvr.Data.MemInfo.Data.Student.JoinTime;
						m_SelfStudents[i].Level = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Level;
						m_SelfStudents[i].Map = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Map;
						m_SelfStudents[i].MeiLi = msgbody.SchoolSvr.Data.MemInfo.Data.Student.MeiLi;
						m_SelfStudents[i].Online = msgbody.SchoolSvr.Data.MemInfo.Data.Student.Online;
						m_SelfStudents[i].TeamNum = msgbody.SchoolSvr.Data.MemInfo.Data.Student.TeamNum;
						memcpy( &m_SelfStudents[i].RoleID,&msgbody.SchoolSvr.Data.MemInfo.Data.Student.RoleID, sizeof(tdr_ulonglong) );
						char buffer[256];
						sprintf( buffer, "OpenSchoolInfo( \"%s\", %d, %d )", msgbody.SchoolSvr.Data.MemInfo.Data.Student.RoleName, m_SelfStudents[i].RoleID.nLow, m_SelfStudents[i].RoleID.nHeight );
						GameManager::getSingleton().getScriptVM()->callString( buffer );
					}
				}
			}
		}
		else if ( msgbody.SchoolSvr.Type == SCHOOL_SVR_ONLINE_UPDATE )
		{
			if (  GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_STUDENT )
			{
				m_SelfTeacher.Teacher.Online = msgbody.SchoolSvr.Data.Online.Online;
				char buffer[256];
				if ( msgbody.SchoolSvr.Data.Online.Online == 0 )
				{
					sprintf( buffer, "ShowMidTips( \"您的导师：%s下线\" )", msgbody.SchoolSvr.Data.Online.RoleName );
				}else
				{
					sprintf( buffer, "ShowMidTips( \"您的导师：%s上线\" )", msgbody.SchoolSvr.Data.Online.RoleName );
				}
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			else if ( GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER )
			{
				for( int i = 0; i < m_SelfStudentNum; i++ )
				{
					if ( strcmp( m_SelfStudents[i].RoleName, msgbody.SchoolSvr.Data.Online.RoleName ) == 0 )
					{ 
						m_SelfStudents[i].Online = msgbody.SchoolSvr.Data.Online.Online;
						char buffer[256];
						if ( msgbody.SchoolSvr.Data.Online.Online == 0 )
						{
							sprintf( buffer, "ShowMidTips( \"您的学生：%s下线\" )", msgbody.SchoolSvr.Data.Online.RoleName );
						}else 
						{
							sprintf( buffer, "ShowMidTips( \"您的学生：%s上线\" )", msgbody.SchoolSvr.Data.Online.RoleName );
						}
						GameManager::getSingleton().getScriptVM()->callString( buffer );
					}
				}
			}
			GameManager::getSingleton().getScriptVM()->callString( "UpdateSchoolRelationShip()" );
		}else if ( msgbody.SchoolSvr.Type ==  SCHOOL_SVR_ADD_STUDENT_ACTION )
		{
			memcpy( &m_SchoolAddStudent.RoleID,&msgbody.SchoolSvr.Data.AddStudent.RoleID, sizeof(tdr_ulonglong) ); 
			memcpy( &m_SchoolAddStudent.TeacherRoleID,&msgbody.SchoolSvr.Data.AddStudent.TeacherRoleID, sizeof(tdr_ulonglong) );
			strncpy( m_SchoolAddStudent.RoleName, msgbody.SchoolSvr.Data.AddStudent.RoleName, sizeof(msgbody.SchoolSvr.Data.AddStudent.RoleName)+1 );
			strncpy( m_SchoolAddStudent.TeacherRoleName, msgbody.SchoolSvr.Data.AddStudent.TeacherRoleName, sizeof(msgbody.SchoolSvr.Data.AddStudent.TeacherRoleName)+1 );
			char buffer[256];
			sprintf( buffer, "SchoolAddStudent( \"%s\",\"%s\" )", msgbody.SchoolSvr.Data.Online.RoleName, msgbody.SchoolSvr.Data.AddStudent.TeacherRoleName );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
	}
	return 0;
}

void GameSchool::InviteToSchool( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_INVITE;
	strncpy( msgbody.SchoolClt.Data.Invite.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::AcceptToSchool( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_ACCEPT;
	strncpy( msgbody.SchoolClt.Data.Accept.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::RejectToSchool( int nFlag, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_REJECT;
	msgbody.SchoolClt.Data.Reject.Flag = 0;
	strncpy( msgbody.SchoolClt.Data.Reject.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::DelSchool( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_DEL;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.SchoolClt.Data.Del.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.SchoolClt.Data.Del.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::RequestMemInfo( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_MEM_INFO_REQ;
	strncpy( msgbody.SchoolClt.Data.MemInfoReq.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::SchoolXinDe( int nHeight, int nLow, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_XINDE;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.SchoolClt.Data.XinDe.RoleID, &ID, sizeof(tdr_ulonglong) );
	strncpy( msgbody.SchoolClt.Data.XinDe.RoleName, szName, strlen(szName)+1 );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::AwardSchoolXinDe( int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	msgbody.SchoolClt.Type = SCHOOL_CLT_AWARD_XINDE;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	memcpy( &msgbody.SchoolClt.Data.AwardXinDe.RoleID, &ID, sizeof(tdr_ulonglong) );
	SendGameServerMsg( SCHOOL_CLT, msgbody );
}

void GameSchool::CharmRequest( const char charmFlag, const char charmType, const char* szAcceptName, int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_CHARM_REQ;
	msgbody.PlayerMiscClt.PlayerMiscCltData.CharmReq.Flag = charmFlag;
	msgbody.PlayerMiscClt.PlayerMiscCltData.CharmReq.Type = charmType;
	strncpy( msgbody.PlayerMiscClt.PlayerMiscCltData.CharmReq.Accept, szAcceptName, strlen(szAcceptName)+1 );
	msgbody.PlayerMiscClt.PlayerMiscCltData.CharmReq.Num = nNum;
	SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
}

void GameSchool::RegSchool( const char* szMsg )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_REG_REQ;
	strncpy( msgbody.PlayerMiscClt.PlayerMiscCltData.RegReq.Msg, szMsg, strlen(szMsg)+1 );
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameSchool::SetRegInfo( tagCSSvrRegInfo regInfo )
{
	m_RegInfo = regInfo;
}

int GameSchool::GetRegStudentsNum()
{
	return m_RegInfo.Num;
}

const char* GameSchool::GetRegStudentName( int nIdx )
{
	return m_RegInfo.RegInfoOne[nIdx].RegOne.Name;
}

int GameSchool::GetRegStudentLevel( int nIdx )
{
	return m_RegInfo.RegInfoOne[nIdx].Lvl;
}

int GameSchool::GetRegStudentRace( int nIdx )
{
	return m_RegInfo.RegInfoOne[nIdx].Career;
}

const char* GameSchool::GetRegStudentClanName( int nIdx )
{
	tdr_ulonglong clanWID = m_RegInfo.RegInfoOne[nIdx].ClanWID;
	return ClanInfo::getSingleton().getClanName( clanWID );
}

const char* GameSchool::GetRegStudentMsg( int nIdx )
{
	return m_RegInfo.RegInfoOne[nIdx].RegOne.Msg;
}

tagRoleStudentInfo_ToLua GameSchool::GetSelfStudentByIndex( int index )
{
	return m_SelfStudents[index];
}

tagRoleStudentInfo_ToLua GameSchool::GetSelfStudent( const char* szName )
{
	tagRoleStudentInfo_ToLua roleInfo;
	memset( &roleInfo, 0, sizeof(roleInfo) );
	for ( int i = 0; i < MAX_ROLE_STUDENT; i++ )
	{
		if ( strcmp(m_SelfStudents[i].RoleName , szName) == 0 )
		{
			roleInfo = m_SelfStudents[i];
		}
	}
	return roleInfo;
}

tagRoleTeacherInfo_ToLua GameSchool::GetSelfTeacher()
{
	return m_SelfTeacher.Teacher;
}

bool GameSchool::IsSelfSchool( const char* szName )
{
	if ( strcmp( m_SelfTeacher.Teacher.RoleName, szName ) == 0 )
	{
		return true;
	}
	else
	{
		for( int i = 0; i < m_SelfStudentNum; i++ )
		{
			if ( strcmp( m_SelfStudents[i].RoleName, szName ) == 0 )
			{
				return true;
			}
		} 
		return false;
	}
}

void GameSchool::ChangeSchoolName( const char* szOldName, const char* szNewName )
{
	if ( strcmp( m_SelfTeacher.Teacher.RoleName, szOldName ) == 0 )
	{
		strcpy( m_SelfTeacher.Teacher.RoleName, szNewName );
	}
	else
	{
		for( int i = 0; i < m_SelfStudentNum; i++ )
		{
			if ( strcmp( m_SelfStudents[i].RoleName, szOldName ) == 0 )
			{
				strcpy( m_SelfStudents[i].RoleName, szNewName );
			}
		} 
	}
}

int GameSchool::GetSelfTeacherGainedCount()
{
	return m_SelfTeacher.TotalCount;
}

int GameSchool::GetSelfAwardTeacherGainedCount()
{
	return m_SelfTeacher.AwardCount;
}

int GameSchool::GetSelfCanAwardTeacherGainedExp()
{
	return m_SelfTeacher.Exp;
}
const tagSchoolXinDeDef_ToLua* GameSchool::GetSchoolXinDeDef( int nLevel )
{
	return (const tagSchoolXinDeDef_ToLua*) DBDataManager::getSingleton().getSchoolXinDeDef( nLevel );
}