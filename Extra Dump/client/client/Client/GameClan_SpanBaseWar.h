#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDefineTolua.h"
#include "GameWizard.h"

#pragma pack(1)
//tolua_begin
struct tagRStrongOwner_ToLua
{
	int SvrID;                          
	int ClanID;                         
	int OccTime;                        
	char ClanName[NAME_LEN];             
};
struct tagRSaleOne_ToLua
{
	int SvrID;                          
	int ClanID;                         
	int Sale;                           
	char ClanName[NAME_LEN];             
};
struct tagRSalePlayerOne_ToLua
{
	int SvrID;                          
	int Sale;                           
	int ClanID;                         
	char RoleName[NAME_LEN];             
};
struct tagRStrongSale_ToLua
{
	int iSale;                           	/*   竞拍前两名 */
	tagRSaleOne_ToLua RSale[MAX_RSALE_TOP];           
	int iPlayer;                         	/*   竞拍者,要退钱的 */
	tagRSalePlayerOne_ToLua RPlayer[MAX_RSALE_NUM];         
};
struct tagSpanStrongMon_ToLua
{
	int Flag;                            	/*   1-表示是最大的资源怪 */
	int MemID;                          
	int Camp;                           /* 1-守方 2-攻方*/
	int X;                              
	int Y;                              
};
struct tagRStrongPworld_ToLua
{
	int InstID;                         
	int DefRes;                         
	int AtkRes;                         
	int MonNum;                         
	tagSpanStrongMon_ToLua Mon[MAX_SPAN_STRONG_RES_MON];   
	tdr_ulonglong PworldWid;                       	/*  Ver.2410  */
};
struct tagRStrongOne_ToLua
{
	char Stat;                           
	char IsOpen;                         
	int StrongID;                       
	tagRStrongOwner_ToLua Owner;                           	/*   据点防守者 */
	tagRStrongSale_ToLua Sale;                           
	tagRStrongPworld_ToLua Pworld;                         
	tagRSaleOne_ToLua Atk;                             	/*  Ver.2410 进攻方 */
};
struct tagResMon_ToLua
{
	int MonID;                          
	int DefMonID;                        	/*  Ver.1592  */
	int AtkMonID;                        	/*  Ver.1592  */
	tagResPos_ToLua Pos;                            
};

struct tagMonPos_ToLua
{
	int MonID;                          
	tagResPos_ToLua Pos;                            
};
struct tagGiftMoney_ToLua
{
	int Type;                           
	int Num;                            
};
struct tagStrongAward_ToLua
{
	int Exp;                             	/*  Ver.1868  */
	tagGiftItem_ToLua Item[MAX_STRONG_ITEM];          
	tagGiftMoney_ToLua Money[MAX_STRONG_MONEY];        
};
struct tagWorldAward_ToLua
{
	int Type;                            	/*   1-经验2-绑银 3-绑金 */
	int Num;                            
};
struct tagKillBuff_ToLua
{
	int KillNum;                        
	int BuffID;                         
	int BuffLvl;                        
	int BuffTime;                       
};

struct tagDieBuff_ToLua
{
	int DieNum;                         
	int BuffID;                         
	int BuffLvl;                        
	int BuffTime;                       
};
/* 战场据点定义表 */
struct tagSpanStrongDef_ToLua
{
	int StrongID;                        	/*   据点ID */
	char Lvl;                             	/*   据点等级 */
	int PworldID;                        	/*   对应的副本ID */
	int BaseUse;                         	/*   竞拍底价 */
	char Name[RES_NAME_LEN];             
	tagResMon_ToLua ResMon[MAX_RES_MON];             	/*   4个资源怪 */
	int ResLimit;                        	/*   资源上限 */
	tagMonPos_ToLua ReviveMonID[MAX_STRONG_REVIVE_MON]; 	/*   两个复活点怪0-防守方 1-进攻方 */
	tagStrongAward_ToLua WinAward;                       
	tagStrongAward_ToLua FailAward;            
	tagWorldAward_ToLua WorldAward[MAX_WORLD_AWARD_TYPE]; 	/*  Ver.2443  */
	tagKillBuff_ToLua KillBuff[MAX_STRONG_KILL_BUFF];  	/*  Ver.2457  */
	tagDieBuff_ToLua DieBuff;                         	/*  Ver.2457 逆袭buff */  
};
struct tagCsSpanStrongWarOne_ToLua
{
	char RoleName[NAME_LEN];             
	char Career;                         
	unsigned short ResGet;                         
	unsigned short CountKill;                      
	unsigned short MaxContinueKill;                
	unsigned short AssitNum;                       
	unsigned short CurrContinueKill;               
	int MapID;                          
	int Die;                            
	unsigned int EnterTime;                      
	char ClanName[NAME_LEN];             
	int Camp;                            	/*  Ver.2426   1-守方 2-攻方*/
};
struct tagCsSpanStrongPworld_ToLua
{
	int DefRes;                         
	int AtkRes;                         
	int DefSvrID;                       
	int AtkSvrID;                       
	char DefClanName[NAME_LEN];          
	char AtkClanName[NAME_LEN];          
	int MonNum;                         
	tagSpanStrongMon_ToLua Mon[MAX_SPAN_STRONG_RES_MON];   
};
//tolua_end
#pragma pack()

class GameClan_SpanBaseWar//tolua_export
	:public NetMsgHandler, public IGameSpanStrong, public Ogre::Singleton<GameClan_SpanBaseWar>
{//tolua_export
public:
	GameClan_SpanBaseWar(void);
	~GameClan_SpanBaseWar(void);
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	virtual void	objEnterView(GameActor *actor);

	//tolua_begin
	//c->s
	void SpanStrongSale( int nID, int nMoney );
	void SpanStrongEnter( int nID );
	void ReqSpanStrongInfo();
	void ReqSelectGroup( int nStrongID, int nGroupID );
	void ReqSpanStrongAward( int nStrongID );

	tagRStrongOne_ToLua getSpanStrongInfoByIndex( int nIdx );			//获取跨服据点信息
	tagSpanStrongDef_ToLua getSpanStrongDef( int ID );						//获取跨服据点定义

	tagCsSpanStrongWarOne_ToLua	getSpanStrongPlayerInfo( int nIdx );			//获取跨服战场里玩家信息
	int	getSpanStrongPlayerNum();																				//获取跨服战场里玩家数目
	int	getSpanStrongSelfContinueKill();																	//获取跨服战场自己的连杀数

	tagCsSpanStrongPworld_ToLua getSpanStrongWarPworldInfo();					//获取当前跨服战场信息

	int getSpanStrongOwnerExpVal();																			//获取占领打怪经验加成
	int getSpanStrongOwnerBindGold();																		//获取占领获得绑金数
	int getSpanStrongOwnerBull();																					//获取占领获得绑银数

	void loadSpanStrongPworldID();																				//load所有跨服据点的PworldID
	bool isInSpanStrongPworld();																				//是否在跨服战场中

	//tolua_end
private:
	int												m_nSpanPworldID[MAX_SPAN_STRONG_NUM];
	tagRStrongInfo						mAllStrongInfo;										//所有跨服据点信息
	tagCsSpanStrongWarAll			mWarAllPlayerInfo;									//跨服据点战争中，玩家战斗信息
	tagCsSpanStrongPworld		mSpanStrongPwolrd;								//当前正在争夺的跨服据点的攻防信息
	tagCsSpanStrongWarOne		mSpanStrongSelfInfo;								//跨服据点战争中，自己信息
	tagCsSpanStrongOcc				m_SpanStrongOccInfo;							//跨服据点及时通知的占领信息
};//tolua_export
