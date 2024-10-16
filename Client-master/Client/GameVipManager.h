#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "ItemDefine.h"


#define	 MAX_OPTIONBTN_NUM  7
//tolua_begin
enum 
{
	STAR_STONE_CTYSTAL	= 1,	//水晶徽章
	STAR_STONE_JADE		= 2,	//翡翠徽章
	STAR_STONE_FIRE		= 3,	//火焰徽章
	STAR_STONE_SILVER	= 4,	//银星徽章
	STAR_STONE_BLACK	= 5,	//黑钻徽章
};

#pragma pack(1)
struct tagStarStoneDef_ToLua
{
	int ID;                              	/*   钻ID, 取值1到31 */
	char Name[NAME_LEN];                  	/*   钻名字 */
	char StarDesc[SHORT_DESC_LEN];        	/*   钻描述 */
	int IconID;                          	/*   钻图标id */
	int StarFuncList[MAX_STONE_FUNC];    	/*   钻对应的功能id列表 */
	int OpenGodCoin[MAX_STONE_OPEN_CASE]; 	/*   三种时间开通方式 7天 30天 180天 */
};

struct tagStarFuncDef_ToLua
{
	int ID;                              	/*   钻功能id,定义 加功能请一定要和程序说下 */
	char StarFuncDesc[SHORT_DESC_LEN];    	/*   钻功能描述 */
	char OpenLink[NAME_LEN];              	/*   打开、查看等描述链接 */
	tagResult_ToLua StarResult;                     
};

enum tagSTAR_RESULT_TOLUA 
{
	STAR_RESULT_NONE_TOLUA = 0, 	/* 无效效果 */
	STAR_RESULT_PWORLD_CARD_TOLUA = 1, 	/* 副本翻牌 */
	STAR_RESULT_STASH_TOLUA = 2, 	/* 随身仓库 */
	STAR_RESULT_FUZHU_TOLUA = 3, 	/* 辅助功能 */
	STAR_RESULT_CANGBAOTU_TOLUA = 4, 	/* 藏宝图次数,val1表示多少次 */
	STAR_RESULT_RONGLU_TOLUA = 5, 	/* 熔炉次数,val1表示多少次 */
	STAR_RESULT_DADI_TOLUA = 6, 	/* 大地之心次数,val1表示多少次 */
	STAR_RESULT_SHANGJIN_TOLUA = 7, 	/* 赏金次数,val1表示多少次 */
	STAR_RESULT_SHANGSAN_TOLUA = 8, 	/* 赏善罚恶,val1表示多少次 */
	STAR_RESULT_SHATAN_EXP_TOLUA = 9, 	/* 沙滩经验,val1 30表示多百分之30 */
	STAR_RESULT_SHOP_TOLUA = 10, 	/* 随身商店,val1 NpcSellList中的SellListID */
	STAR_RESULT_PKVAL_TOLUA = 11, 	/* 杀气值减少,val1 100表示2倍速度,200表示3倍速度 */
	STAR_RESULT_TRANS_TOLUA = 12, 	/* 航空传送 */
	STAR_RESULT_BUSY_TOLUA = 13, 	/* 免费我很忙 */
	STAR_RESULT_DEC_PKVAL_TOLUA = 14, 	/* 任意地点使用减杀气值道具 */
	STAR_RESULT_STORE_TOLUA = 15, 	/* 随身商铺 */
};

#pragma pack()

struct tagAutoQuestData 
{
	int m_Num[MAX_OPTIONBTN_NUM];
	bool m_bChkBtn[MAX_OPTIONBTN_NUM];
};

struct tagAutoRecoveryData 
{
	int m_hpValue;
	int m_mpValue;
	int m_hpItemID;
	int m_mpItemID;
};
//tolua_end

class GameVipManager //tolua_export
	:public Ogre::Singleton<GameVipManager> , public NetMsgHandler
{//tolua_export
public:
	GameVipManager();
	~GameVipManager();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	const tagStarStoneDef_ToLua* getStarStoneDefByIdx( int nIdx );
	const tagStarStoneDef_ToLua* getStarStoneDef( int nID );
	const tagStarFuncDef_ToLua* getStarFuncDef( int nID );

	bool haveOpenVip();
	bool isEnableVip( int ID );
	int getVipEndTime( int nID );

	void requestOpenVip(  int nType , int nID );
	void openVipShop();

	int getCangBaoTuVipNum();
	int getBCTVipNum();
	int getShangShanNum();
	int getRongLuNum();
	int getShangJinNum();

	tagAutoQuestData* getAutoQuestOpt();
	void saveAutoQuestOpt();
	tagAutoRecoveryData* getAutoRecoveryOpt();
	void saveAutoRecoveryOpt();
	bool isOwnVipFunc( int nResultID );
	//tolua_end

	void reset();

private:
	void resetFuncData();
	int	 getStarFunResultVal1( int nResultID );

private:
	VIPDATA				m_vipData;
	bool				m_bRequestServer;
	std::vector<int>	m_vEnableFunc;
	tagAutoQuestData	m_autoQuestData;
	tagAutoRecoveryData m_autoRecoveryData;
};//tolua_export