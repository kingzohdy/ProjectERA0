#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameDBData.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1) 
struct tagSubJectSelf_ToLua
{
	char Succ;                            	/*   答对数 */
    char Star;                            	/*   灵感之星数量 */
    unsigned short Top;                             	/*   自己排名 */
	int Integ;                           	/*   自己积分 */
	int CurrIdx;                         	/*  Ver.1542 大于0表示当前题已经答过了,值为玩家答案索引|从1开始 */
};

struct tagSubJectTopOne_ToLua
{
	unsigned short Top;                            
	int Integ;                           	/*   积分 */
	char RoleName[ROLE_NAME_LEN_SAVE];    	/*   角色名 */
};

struct tagSubJectTop_ToLua
{
	int Num;                            
	tagSubJectTopOne_ToLua Tops[SUBJECT_MAX_TOP];          
};

struct tagSubJectPloy_ToLua
{
	char Already;                         	/*   服务器已经出题数,第几道题 */
	char Odd;                             	/*   服务器剩余出题数 */
	int ID;                              	/*   题库ID */
	int ReqEnd;                          	/*   邀请结束时间|大于0表示要显示开始比赛倒计时 */
	int ResEnd;                          	/*   答题结束时间 */
	int LockEnd;                         	/*   阅题结束时间 */
};

struct tagSubJectAward_ToLua
{
	int Top;                             	/*   排名奖励 */
	int Base;                            	/*   基础奖励 */
	int Add;                             	/*   额外奖励 */
};

struct tagSubJectFlush_ToLua
{
	tagSubJectSelf_ToLua Self;                            	/*   自己的信息 */
	tagSubJectTop_ToLua Top;                             	/*   前20名的信息 */
	tagSubJectPloy_ToLua Ploy;                            	/*   答题活动信息 */
	tagSubJectAward_ToLua Award;                           	/*   奖励信息|此结构任何一字段有值表示活动结束 */
};
struct tagSubJectCommit_ToLua
{
	int ID;                              	/*   题库ID,服务器用来验证 */
	int Idx;                             	/*   第几道题,服务器用来验证 */
	int KeyIdx;                          	/*   答案索引0-3  */
	int IsStar;                          	/*   是否使用灵感之星1-是 */
};
struct tagSubJectKey_ToLua
{
	char Str[MAX_SUBJECT_KEY_LEN];       
};

struct tagSubJectDef_ToLua
{
	unsigned int ID;                             
	char SubJect[MAX_SUBJECT_LEN];        				/*   题目 */
	tagSubJectKey_ToLua SubJectKey[MAX_SUBJECT_KEY];    /*   4个答案 */
	int Succ;                            				/*   正确答案索引|从1开始 */
	int Type;                            				/*  Ver.1534 类型 */
};
#pragma pack()
const tagSubJectDef_ToLua*  getSubjectDef(int id);
//tolua_end

class GameIntelliRaceMgr //tolua_export
	: public NetMsgHandler,public Ogre::Singleton<GameIntelliRaceMgr>
{//tolua_export
public:
	GameIntelliRaceMgr(void);
	~GameIntelliRaceMgr(void);
	//tolua_begin
	void IntelligenceRaceJoinAgree();
	void IntelliRaceAnswerCommit();
	void IntelliRaceAnswerQuit();
	tagSubJectFlush_ToLua* getSubJectFlushInfo();
	tagSubJectCommit_ToLua* getSubjectCommitInfo();
	
	//tolua_end
	
	tagSubJectCommit_ToLua	m_SubJectCommitInfo;

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
private:
	tagSubJectFlush		m_SubjectFlush;

};//tolua_export