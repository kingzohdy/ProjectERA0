#pragma once
#include <vector>
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
//const int NONE_AUTO_TEAM = -1;
bool isRoleInKuaFu( const char* pszName );
enum 
{
	CLT_AUTO_TEAM_LIMIT_NONE		= 0,						/* 没有任何限制 */
	CLT_AUTO_TEAM_LIMIT_EASY		= AUTO_TEAM_LIMIT_EASY,		/* 副本难度简单 */
	CLT_AUTO_TEAM_LIMIT_NORM		= AUTO_TEAM_LIMIT_NORM,		/* 副本难度普通 */
	CLT_AUTO_TEAM_LIMIT_DIFF		= AUTO_TEAM_LIMIT_DIFF,		/* 副本难度困难 */
	CLIENT_AUTO_TEAM_LIMIT_NIGHTMARE= AUTO_TEAM_LIMIT_NIGHTMARE,/* 副本难度专家 */
	CLT_AUTO_TEAM_LIMIT_WARRIOR		= AUTO_TEAM_LIMIT_WARRIOR, 	/* 需要战士职业 */
	CLT_AUTO_TEAM_LIMIT_MAGICOR		= AUTO_TEAM_LIMIT_MAGICOR, 	/* 需要法师职业 */
	CLT_AUTO_TEAM_LIMIT_GUNER		= AUTO_TEAM_LIMIT_GUNER, 	/* 需要枪手职业 */
	CLT_AUTO_TEAM_LIMIT_CHUNCH		= AUTO_TEAM_LIMIT_CHUNCH, 	/* 需要祭司职业 */
	CLT_AUTO_TEAM_LIMIT_LEADER		= AUTO_TEAM_LIMIT_LEADER, 	/* 需要当队长 */
};

enum 
{
	CLT_PRE_MEMBER_NOTDECIDE= 0,	// 没表态
	CLT_PRE_MEMBER_AGREE	= 1,	// 同意
	CLT_PRE_MEMBER_REJECT,			// 拒绝

	CLT_AUTO_TEAM_STATE_IN,			// 已经处于自动组队
	CLT_AUTO_TEAM_STATE_PREPARE,	// 处于自动组队的准备状态
	CLT_AUTO_TEAM_STATE_NONE,		// 不处于自动组队的相关状态
};

enum
{
	CLT_AUTO_TEAM_TASK = 0,		//  任务
	CLT_AUTO_TEAM_ACTIVE = 1,	//  活动
	CLT_AUTO_TEAM_CLAN = 2, 	/* 公会活动 */
};

enum
{
	CLT_CAREER_WARRIOR	= 1,	// 战士
	CLT_CAREER_MAGICOR	= 2,	// 法师
	CLT_CAREER_GUNER	= 3,	// 枪手
	CLT_CAREER_CHURCH	= 4,	// 牧师
};

enum
{
	CLT_PWORLD_SIMPLE		= DIFF_PWORLD_GRADE_D,
	CLT_PWORLD_NORMAL		= DIFF_PWORLD_GRADE_C,
	CLT_PWORLD_HARD			= DIFF_PWORLD_GRADE_B,
	CLT_PWORLD_PROFESSIONAL	=DIFF_PWORLD_GRADE_A,
	CLT_PWORLD_END			= DIFF_PWORLD_GRADE_A + 1,
};

#pragma pack( 1 )
struct tagAutoTeamApplyIDInfo_ToLua
{
	unsigned short LimitFlag;                       	/*   副本难度限制条件 */
	unsigned short AutoTeamID;                     
};

struct tagAutoTeamApplyInfo_ToLua
{
	short LimitFlag;                       	/*   职业和队长限制条件 */
	short AutoTeamIDNum;                  
	tagAutoTeamApplyIDInfo_ToLua AutoTeamIDInfos[AUTO_TEAM_ID_MAX];
};

struct tagCSAutoTeamPreMemInfo_ToLua
{
	char RoleName[NAME_LEN];             
	unsigned char Gender;                         
	unsigned char Career;                         
	unsigned char Head;                           
	unsigned char Face;                           
	unsigned char Hair;                           
	unsigned char Level;                          
	unsigned char CreatedTeamMem;                  	/*  Ver.307 0 非已建队伍成员，1 已建队伍成员 */
	unsigned char AgreeStat;                       	/*   0 未同意，1 已同意 */
	tagAutoTeamApplyInfo_ToLua AutoTeamApplyInfo;  
};
#pragma pack()

struct tagAutoTeamPreMem
{
	//bool	m_isAnyInTeam;
	std::vector<tagCSAutoTeamPreMemInfo_ToLua>	m_preMemInfo;
};

enum
{
	CLIENT_ENTER_PWORLD_SAFE_STATUSID = ENTER_PWORLD_SAFE_STATUSID,
};

//tolua_end

class AutoTeamManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<AutoTeamManager>
{//tolua_export
public:
	AutoTeamManager();
	void releaseAutoTeamInfo();
	void ValidateMemberName( char* szRoleName, int nMaxNameLen );
	const char* getValidateMemberName( const char* pszRoleName );
	//tolua_begin
	
	// 取消所有原来选择的副本
	void cancelAllSelectPWorld();
	// 添加所选择的副本
	void addSelectPWorld( int nAutoTeamID, int nSimpleFlg, int nNormalFlg, int nHardFlg, int nProfessionalFlag );
	// 所要求的额外条件（如：职业要求，是否想当队长）
	
	// 获得所有限制条件
	short getAllCareerAndCapRequest();
	// 取消所有职业和队长要求
	void cancelAllCareerAndCapRequest();
	// 添加职业和队长要求
	void addCareerAndCapRequest( int nCarrer, bool bAdd );

	// 请求系统匹配自动组队，即通过点击 寻求组队 按钮发起的操作
	void requestAutoTeam();
	// 拒绝完成创建
	void disagreeCreate();
	// 同意完成创建
	void agreeCreate();

	// 拒绝自动组队
	void disagreeAutoTeam();
	// 同意自动组队
	int getMainPlayerAgreeState();
	bool canAgreeAutoTeam();
	void agreeAutoTeam();

	// 发起完成创建请求
	void requestFinishCreate();
	
	bool canTranToPWorld();
	// 传送进自动组队所选择的副本
	void tranToPWorld();
	// 取消传送进副本
	void cancelTranToPWorld();
	
	// 获取自动组队中的人数
	int getAutoTeamMemberNum();
	// 查询自动组队中的成员
	tagCSAutoTeamPreMemInfo_ToLua* getAutoTeamMember( int nIndex );
	// 获取预创建队伍的成员数量
	int getPreMemberNum();
	// 查询预创建队伍的成员
	tagCSAutoTeamPreMemInfo_ToLua* getPreMemberByIndex( int nIndex );
	// 查询预创建队伍的成员
	tagCSAutoTeamPreMemInfo_ToLua* getPreMember( const char* pszName );
	// 获得自动组队ID 
	int getAutoTeamID();
	// 是否自己相当队长
	bool isSelfWantToBeCaptain();
	// 预创建队伍中是否有人加入队伍
	//bool isPreMemberInTeam();
	// 获取副本难度
	int getPworldDiffulty()
	{
		return m_nPworldDiffulty;
	}

	// 请求清除安全状态
	void cleanSafeStatus( int nStatusID = ENTER_PWORLD_SAFE_STATUSID );
	//tolua_end
	
	// 增量更新预创建队伍的成员
	void incrUpdatePreMember( const CSAUTOTEAMPREADD& addMemberInfo );
	// 全量更新预创建队伍的成员
	void totalUpdatePreMember( const CSAUTOTEAMPREADD& addMemberInfo );
	// 添加新加入预创建队伍的成员
	void addPreMember( const CSAUTOTEAMPREMEMINFO& newPreMemberInfo );
	// 减少加入预创建队伍的成员
	void delPreMember( const char* pszName );
	// 改变预创建队伍成员的准备状态
	void changePreMemberState( const char* pszName, unsigned char nState );
	// 更新自动组队队伍中的成员信息
	void updateAutoTeamMember( const CSAUTOTEAMSUCC& allMemberInfo );

	virtual int  HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

private:
	void	OnAutoTeamSucc( const CSAUTOTEAMSUCC& autoTeamSuccData );

private:
	int						m_nAutoTeamState;
	int						m_nAutoTeamID;
	AUTOTEAMAPPLYINFO		m_selfAutoTeamApplyInfo;	// 自己所请求的任务或者副本信息
	tagAutoTeamPreMem		m_preMemberInfo;
	int						m_nPworldDiffulty;			// 难度级别
};//tolua_export