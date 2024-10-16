#pragma once
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>
#include <vector>

//tolua_begin
#pragma pack(1)
struct tagRongluGrade_ToLua
{
	int MapID;                           	/*   任务地图 */
	int KillNum;                         	/*   杀怪数 */
	int MonIDs[RONGLU_KILL_MON_MAX];     	/*   哪些怪 */
	int IconIDs[RONGLU_ICONID_MAX];      	/*   奖励物品 */
	int MapID1;                          	/*  Ver.1793 任务地图2 */
};

struct tagRongluDef_ToLua
{
	int ItemID;                         
	int Rate;                            	/*   万分率 */
	char Pinzhi;                          	/*   0-白色 1-绿 2-蓝 3-紫 4-金 */
	tagRongluGrade_ToLua Grade[RONGLU_GRADE_MAX];        
};
#pragma pack()
//tolua_end

class MagicSmelterManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<MagicSmelterManager>
{//tolua_export
public:
	MagicSmelterManager(void);
	~MagicSmelterManager(void);
	//tolua_begin
	//开始熔炉重铸
	void requestRecastSmelter( int nGridIdx );
	//滚动结束
	void endRecastSmelter( int nGridIdx, int id );
	void breakRecastSmelter();
	//获取熔炉表对象
	tagRongluDef_ToLua* getRongluDef( int id );
	//根据品质获得熔炉id
	int getRongluIdByPinZhi( int nIndex );
	//最终获得物品的id
	int getRongluRecastEndID() { return m_nResultID; }
	//获得熔炉对应等级段
	int getRongluLv() { return m_nGradeLv; }
	//播放熔炉重铸音效
	void playRongluRecastSound();
	//重置数据
	void initRongluInfo();
	//tolua_end
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
private:
	int m_nSmelterID;
	int m_nResultID;
	int m_nGradeLv;
	std::vector<tagRongluDef*> m_vecRongluData;
};//tolua_export