#pragma once

#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreSingleton.h"
#include "GameDefineToLua.h"

//tolua_begin
struct tagRoleTeacherInfo_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[ROLE_NAME_LEN_SAVE];   
	unsigned char Level;                          
	unsigned char Head;                           
	unsigned char Face;                           
	unsigned char Hair;                           
	char Gender;                         
	char Career;                         
	int JoinLevel;                      
	int JoinTime;                       
	int MeiLi;                          
	int Online;                         
	int Map;                            
	int TeamNum;              
	char ClanName[NAME_LEN];              	/*  Ver.2091 公会名字 */              
};
struct tagRoleStudent_ToLua
{
	tagRoleTeacherInfo_ToLua Teacher;                        
	int Exp;                             	/*   当前未领取的经验值 */
	int Val;                             	/*   领取后给导师增加的声望 */
	int AwardCount;                     
	int TotalCount;                     
};

struct tagRoleStudentInfo_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[ROLE_NAME_LEN_SAVE];   
	unsigned char Level;                          
	unsigned char Head;                           
	unsigned char Face;                           
	unsigned char Hair;                           
	char Gender;                         
	char Career;                         
	int JoinLevel;                      
	int DayTime;                        
	int JoinTime;                       
	int MeiLi;                          
	int Online;                         
	int Map;                            
	int TeamNum;                    
	char ClanName[NAME_LEN];              	/*  Ver.2091 公会名字 */
};
struct tagSchoolXinDeDef_ToLua
{
    int Level;                          
    int AddExp;                         
    int SchoolVal;                      
};
struct tagSchoolAddStudent_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[ROLE_NAME_LEN_SAVE];   
	tagLong_ToLua TeacherRoleID;                  
	char TeacherRoleName[ROLE_NAME_LEN_SAVE];
};
/* ROLE_SCHOOL_TYPE_TOLUA*/
enum tagROLE_SCHOOL_TYPE_TOLUA 
{
	ROLE_SCHOOL_TYPE_NONE_TOLUA = 0, 
	ROLE_SCHOOL_TYPE_TEACHER_TOLUA = 1, 
	ROLE_SCHOOL_TYPE_STUDENT_TOLUA = 2, 
};
#define MAX_ROLE_STUDENT_TOLUA MAX_ROLE_STUDENT
#define SCHOOL_DEL_MONEY_NUM_TOLUA SCHOOL_DEL_MONEY_NUM 	/* 解除师徒关系需要扣除的绑银数 */
//tolua_end

class GameSchool //tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameSchool>
{ //tolua_export
public:
	GameSchool();
	~GameSchool();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	void SetRegInfo( tagCSSvrRegInfo regInfo );
	//tolua_begin
	void InviteToSchool( const char* szName );				//邀请加入师门
	void AcceptToSchool( const char* szName );				//接受加入师门邀请
	void RejectToSchool( int nFlag, const char* szName );	//拒绝加入师门
	void RequestMemInfo( const char* szName );				//请求学生面板中某个学生的信息
	void DelSchool( int nHeight, int nLow, const char* szName );	//解除师徒关系
	void SchoolXinDe( int nHeight, int nLow, const char* szName );	//冲入心得
	void AwardSchoolXinDe( int nHeight, int nLow );			//领取心得

	void CharmRequest( const char charmFlag, const char charmType, const char* szAcceptName, int nNum );			//送花

	void RegSchool( const char* szMsg );					//登记时的留言

	int GetRegStudentsNum();					//获取当前登记在线的玩家数目
	const char* GetRegStudentName( int nIdx );			//根据索引获取当前登记在线的玩家名字
	int GetRegStudentLevel( int nIdx );					//根据索引获取当前登记在线的玩家等级
	int GetRegStudentRace( int nIdx );					//根据索引获取当前登记在线的玩家职业
	const char* GetRegStudentClanName( int nIdx );		//根据索引获取当前登记在线的玩家公会名字
	const char* GetRegStudentMsg( int nIdx );			//根据索引获取当前登记在线的玩家留言

	int GetSelfSchoolJobType(){ return m_SelfSchoolJobType;}
	int GetSelfStudentNum(){ return m_SelfStudentNum;}
	tagRoleStudentInfo_ToLua GetSelfStudentByIndex( int index );
	tagRoleStudentInfo_ToLua GetSelfStudent( const char* szName );
	tagRoleTeacherInfo_ToLua GetSelfTeacher();
	int GetSelfTeacherGainedCount();					//获取自己导师为自己存入的心得次数
	int GetSelfAwardTeacherGainedCount();				//获取自己领取导师心得次数
	int GetSelfCanAwardTeacherGainedExp();				//获取当前可以领取的经验值
	const tagSchoolXinDeDef_ToLua* GetSchoolXinDeDef( int nLevel );

	bool IsSelfSchool( const char* szName );			//玩家是否与自己有师生关系

	void ChangeSchoolName( const char* szOldName, const char* szNewName );
	//tolua_end
private:
	tagCSSvrRegInfo m_RegInfo;					//注册学生信息
	int	m_SelfSchoolJobType;					//当前师徒关系中的职务ROLE_SCHOOL_TYPE_TOLUA
	int	m_SelfStudentNum;						//自己学生数量
	tagRoleStudentInfo_ToLua		m_SelfStudents[MAX_ROLE_STUDENT];		//自己的学生信息
	tagRoleStudent_ToLua			m_SelfTeacher;							//自己的老师信息

	tagSchoolAddStudent_ToLua		m_SchoolAddStudent;						//成功加入师徒
}; //tolua_export