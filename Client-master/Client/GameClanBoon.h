#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
enum tagBOON_ADD_TYPE_ToLua 
{
	BOON_ADD_TYPE_BINGDAO_DEF_SUCC_TOLUA = 1, 	/* 冰岛防守成功 */
	BOON_ADD_TYPE_BINGDAO_DEF_FAIL_TOLUA = 2, 	/* 冰岛防守失败 */
	BOON_ADD_TYPE_BINGDAO_ATK_SUCC_TOLUA = 3, 	/* 冰岛进攻成功 */
	BOON_ADD_TYPE_XUEYUAN_DEF_SUCC_TOLUA = 4, 	/* 雪原防守成功 */
	BOON_ADD_TYPE_XUEYUAN_DEF_FAIL_TOLUA = 5, 	/* 雪原防守失败 */
	BOON_ADD_TYPE_XUEYUAN_ATK_SUCC_TOLUA = 6, 	/* 雪原进攻成功 */
	BOON_ADD_TYPE_SHAMO_DEF_SUCC_TOLUA = 7, 	/* 沙漠防守成功 */
	BOON_ADD_TYPE_SHAMO_DEF_FAIL_TOLUA = 8, 	/* 沙漠防守失败 */
	BOON_ADD_TYPE_SHAMO_ATK_SUCC_TOLUA = 9, 	/* 沙漠进攻成功 */
	BOON_ADD_TYPE_LUORI_DEF_SUCC_TOLUA = 10, 	/* 落日防守成功 */
	BOON_ADD_TYPE_LUORI_DEF_FAIL_TOLUA = 11, 	/* 落日防守失败 */
	BOON_ADD_TYPE_LUORI_ATK_SUCC_TOLUA = 12, 	/* 落日进攻成功 */
	BOON_ADD_TYPE_YULIN_DEF_SUCC_TOLUA = 13, 	/* 雨林防守成功 */
	BOON_ADD_TYPE_YULIN_DEF_FAIL_TOLUA = 14, 	/* 雨林防守失败 */
	BOON_ADD_TYPE_YULIN_ATK_SUCC_TOLUA = 15, 	/* 雨林进攻成功 */
	BOON_ADD_TYPE_MINGSHA_DEF_SUCC_TOLUA = 16, 	/* 鸣沙防守成功 */
	BOON_ADD_TYPE_MINGSHA_DEF_FAIL_TOLUA = 17, 	/* 鸣沙防守失败 */
	BOON_ADD_TYPE_MINGSHA_ATK_SUCC_TOLUA = 18, 	/* 鸣沙进攻成功 */
	BOON_ADD_TYPE_SHIJIAN_DEF_SUCC_TOLUA = 19, 	/* 时间城防守成功 */
	BOON_ADD_TYPE_SHIJIAN_DEF_FAIL_TOLUA = 20, 	/* 时间城防守失败 */
	BOON_ADD_TYPE_SHIJIAN_ATK_SUCC_TOLUA = 21, 	/* 时间城进攻成功 */
};

#define CLAN_BOON_LEVEL_TOLUA	CLAN_BOON_LEVEL
#define CLAN_TECH_NUM_TOLUA		CLAN_TECH_NUM
#define CLAN_ONLINE_AVE_TOLUA	CLAN_ONLINE_AVE
#define CLAN_GOLD_NUM1_TOLUA	CLAN_GOLD_NUM1
#define CLAN_GOLD_NUM2_TOLUA	CLAN_GOLD_NUM2
#define CLAN_BOON_BINDGOLD_TOLUA	CLAN_BOON_BINDGOLD
#define CLAN_BOON_GOLD_TOLUA	CLAN_BOON_GOLD
#define CLAN_CONTRI_NUM_TOLUA	CLAN_CONTRI_NUM
#define BOON_ADD_EVENT_MAX_TOLUA	BOON_ADD_EVENT_MAX
#define BOON_GET_MAX_TOLUA		BOON_GET_MAX


#pragma pack(1)
struct tagBoonAddEvent_ToLua
{
	unsigned char ID;                              	/*   取值为BOON_ADD_TYPE */
	char Per;                             	/*   加成百分比 */
	unsigned int Time;                            	/*   时间 */
};

struct tagBoonCond_ToLua
{
	short Ave;                             	/*   周平局在线|12:00-24:00 */
	short AveNum;                          	/*   采集了几个点 */
	unsigned int AveTime;                         	/*   采集时间 */
	int Tech;                            	/*   周科技点 */
	int Contri;                          	/*   周贡献度 */
	int Gold;                            	/*   周金币提取 */
	int EventNum;                       
	tagBoonAddEvent_ToLua Event[BOON_ADD_EVENT_MAX];       	/*   福利加成事件 */
	unsigned int ClearTime;                      
};

struct tagBoonAssignPos_ToLua
{
	char Name[RES_NAME_LEN];             
	unsigned char Pos;                             	/*   职位 */
	int Money;                           	/*   数量 */
	int Flag;                            	/*   0-未领取 */
};

struct tagBoonAssign_ToLua
{
	int BindGlod;                        	/*   上周可分配金券 */
	int Glod;                            	/*   上周金币 */
	tagBoonAssignPos_ToLua AssignGlod;                      	/*   会长金币 */
	int Office;                         
	tagBoonAssignPos_ToLua AssignBindGlod[BOON_GET_MAX];    	/*   金券分配细节 */
};

struct tagClanBoon_ToLua
{
	tagBoonCond_ToLua Cond;                           
	tagBoonAssign_ToLua Assign;                         
};
#pragma pack()
//tolua_end

class GameClanBoon//tolua_export
	:public Ogre::Singleton<GameClanBoon> , public NetMsgHandler
{//tolua_export
public:
	GameClanBoon();
	~GameClanBoon(){};
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	//tolua_begin
	//领取工资
	void requestClanBoonGet( int money, int flag );
	//分配工资
	void requestClanBoonAssign();
	//获取所有工资信息
	tagClanBoon_ToLua& getClanBoonData() { return m_ClanBoonInfo; }
	//获取分配信息
	tagBoonAssign_ToLua& getBoonAssignData() { return m_AssignInfo; }
	//设置分配信息
	void resetAssignData();
	void setBoonAssignDataByIndex( int idx, char * szName, int pos, int money );
	//获取领取情况
	int getBoonDataIndexByPos( int pos );
	//tolua_end
private:
	tagClanBoon_ToLua m_ClanBoonInfo;
	tagBoonAssign_ToLua m_AssignInfo;
};//tolua_export