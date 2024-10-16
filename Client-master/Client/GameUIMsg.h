#pragma once
//////////////////////////////////////////////////////////////////////////
// 同UI交互信息管理
//////////////////////////////////////////////////////////////////////////
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "cs_net.h"
#include "ItemDefine.h"
#include "GameDefineTolua.h"
#include "GameStoreManager.h"

#pragma pack(1)
//tolua_begin

#define	MAX_CLI_LINKNAME 64

// 聊天消息

// 物品超链接
struct tagItemLink_ToLua
{
	int m_nItemId;
	int m_nItemDur;
	int InstVal1;                        	/*  Ver.1322  */
	int InstVal2;                        	/*  Ver.1322  */
	int InstVal3;                        	/*  Ver.1322  */
	int Flag;                            	/*  Ver.1324  */
};
// 装备超链接
struct tagArmLink_ToLua
{
	tagRoleArm_ToLua m_arm;
};

// 任务超链接
struct tagMissionLink_ToLua
{
	int m_MissionId;
	int	m_nRecycleID;
};

// 位置坐标链接
struct tagPostionLink_ToLua
{
	int m_nMapId;
	int m_nX;
	int m_nY;
};

struct tagSecondarySkillLink_ToLua
{
	short SkillType;                       	/*   技能类型: 采矿采药等等 Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	short SkillLevel;                      	/*   技能等级 */
	int SkillVal;                        	/*   技能熟练度 */
	char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
};

struct tagPetLink_ToLua
{
 	tagLong_ToLua	PetWid;
	char			PetName[NAME_LEN];
	char			OwnerName[ROLE_NAME_LEN]; 
};

struct tagStoreLink_ToLua
{
	int StoreID;                        
};

struct tagSetLink
{
	int				m_nLinkType;
	char			m_szLinkName[MAX_CLI_LINKNAME];
	char			m_szLinkName2ClientRich[MAX_CLI_LINKNAME];
	char			m_szLinkName2ServerRich[MAX_CLI_LINKNAME];
	tagItemLink_ToLua		m_itemLink;
	tagArmLink_ToLua		m_armLink;
	tagMissionLink_ToLua	m_missionLink;
	tagPostionLink_ToLua	m_postionLink;
	tagSecondarySkillLink_ToLua m_SecondarySkillLink;
 	tagPetLink_ToLua		m_PetLink;
	tagStoreLink_ToLua		m_StoreLink;
};

struct tagChatMsg
{
	char Type;                    
	char Speaker[ROLE_NAME_LEN];            
	char Content[CLT_MSG_LEN];  
	bool isVip;
	int	 IsCluster;
	int WorldID;
};

// 聊天系统消息
struct tagSysMsg
{
	char Type;                            	                         	
	char Times;					/*   公告显示次数，0不限制 */                 	
	char Interval;              /*   公告显示间隔 */     
	char Msg[SYS_MSG_LEN];               
};

struct tagChatReq
{
	char Type;                            	/*    Bind Macrosgroup:CHAT_TYPE,*/
	char Res;                            
	unsigned short LinkLen;                        
	char ToName[NAME_LEN];               
	char Content[CLT_MSG_LEN];           
	char LinkContent[MAX_LINK];          
	short ListType;                        	/*  Ver.275 消耗道具的背包栏位 Bind Macrosgroup:LIST_TYPE,*/
	unsigned short GridIdx;                         	/*  Ver.275 消耗道具的栏位格子 */
	unsigned char OnlineType;                      	/*  Ver.578 接收者的在线类型 Bind Macrosgroup:ONLINE_TYPE,*/
	bool isVip;
};

// 临时机械快捷栏
struct tagShortCut
{
	int		m_nNum;
	int		m_nType;
	int		m_Id;
	int		m_Lv;
	int		m_nFittingIndex;
	char	m_Des[MAX_PATH];
};

// 快捷栏
struct tagTmpShortCut
{
	int		m_nNum;
	int		m_nType;
	int		m_Id;
	int		m_Lv;
	int		m_nFittingIndex;
};

// 冷却时间
struct tagCoolTime
{
	int	  num;
	float starttime;
	float endtime;
};

// 区域地图信息
struct tagMaxMapInfo	// 此为描述当前美术资源给的部分纹理的信息
{
	int			nPlayerX;		// 此为角色相对于当前显示地图的X偏移
	int			nPlayerY;		// 此为角色相对于当前显示地图的Y偏移
	float		fZoom;			// 此为当前显示的地图相对整张图的缩放值 	
	int			nMapPosStartX;	// 此为当前显示的地图的左上角相对于美术给的完整地图纹理且拉伸为1024宽度的左上角的X偏移
	int			nMapPosStartY;	// 此为当前显示的地图的左上角相对于美术给的完整地图纹理且拉伸为1024高度的的左上角的Y偏移
	int			nMapPosWidth;	// 此为当前显示的地图在美术给的完整纹理上对应的截取部分纹理的宽度,而不是实际游戏场景的宽度,也不是m_ViewWidth
	int			nMapPosHeight;	// 此为当前显示的地图在美术给的完整纹理上对应的截取部分纹理的高度,而不是实际游戏场景的高度,也不是m_ViewHeight
	float		fAngle;			// 此为主角色偏转的角度
	float		fCamAngle;		// 此为主角色对应的照相机的偏转的角度
	int			mapId;			// 此为主角色当前所在地图的ID
};

struct tagFittingUIInfo
{
	int m_nFittingId;
	int m_nFittingLv;
};

struct tagSkillFitting
{
	unsigned int	 m_nFittingsNum;                    
	tagFittingUIInfo m_FittingList[MAX_ROLE_FITTINGS];
};

struct tagFittingSlot
{
	int m_nSkillId;
	int m_nFittingIndex;
	int m_nSlot;
	int m_nFittingId;
};

struct tagPowerCollect
{
	float fTotleTime;
	float fStartTime;
	int   times;
};

struct tagNpcDialog
{
	int		m_nNpcId;
	bool	m_bNext;
	char	m_szAnswer[DLG_STR_LEN];
};

enum
{
	NPC_DIALOG_LAST_OPT,
	NPC_DIALOG_NORMAL_OPT,
	NPC_DIALOG_LEAVE_OPT,
};

struct tagNpcDlgOneOpt
{
	char Opt[DLG_OPT_LEN];  
	int  m_nType;
};

struct tagNpcOpt
{
	int				m_nNpcId;
	char			m_szSentence[DLG_STR_LEN];  
	int				m_optTal;
	tagNpcDlgOneOpt	m_OneOpt[MAX_DLG_OPT]; 
	int				m_lastOptNum;
	int				m_lastOpt[MAX_DLG_OPT];
};

struct tagNpcLastDialog
{
	int		m_nNpcId;
	char	DlgLast[DLG_STR_LEN];   
};

struct tagNpcCommitDialog
{
	int		m_nNpcId;
	int		CommitNum;
	char	Answer[DLG_STR_LEN];   
};

// 任务对话
struct tagQuestTalk
{
	int		m_nNpcId;
	int		m_nQuestId;
	char	DlgLast[TASK_DIALOG_DESC_LEN];
	int		m_nRecycleID;
};

struct tagNpcId
{
	int m_nNpcId;
};

struct tagMidTipsMsg
{
	int  m_nMsgType;
};

struct tagReviveMsg
{
	char		 m_szRoleName[NAME_LEN];
	int			 m_nPlayerId;
	int			 m_nSkillId;
	unsigned int DropMoney;                       	/*  Ver.917 掉了多少钱 */
	unsigned int ReviveMoney;                     	/*  Ver.917 能恢复多少钱 */
	unsigned int DropExp;                         	/*  Ver.917 掉了多少exp */
	unsigned int ReviveExp;                       	/*  Ver.917 能恢复多少exp */

};

struct tagAddFriendMsg
{
	char	m_szReqName[NAME_LEN];
};

struct tagPworldMsg
{
	int		PworldID;
	int		PworldIdx;
	char	PworldName[RES_NAME_LEN];
	int		MatchType;
	int		MinPlayerNum;
};

struct tagSecondarySkill
{
	int		skillType;
	int		skillId;
};

struct tagGameActorMsg
{
	int		type;
	int		id;
};

struct tagGPRSMsg
{
	unsigned char Head;                            	/*  Ver.406  */
	unsigned char Hair;                            	/*  Ver.406  */
	unsigned char Face;                            	/*  Ver.406  */
	unsigned char Career;                          	/*  Ver.406  */
	int Level;                           	/*  Ver.406  */
	int Map;                            
	int X;                              
	int Y;                              
	int Team;                           
	int GPRSEnd;                        
	int Gender;                          	/*  Ver.472  */
	char RoleName[ROLE_NAME_LEN];        
	char ClanName[ROLE_NAME_LEN];    
};

struct tagDieDropItem_ToLua
{
	int ItemID;                         
	int Num;                            
};

struct tagDieDrop_ToLua
{
	int Count;                          
	int Money;                          
	int Exp;                            
	int ArmEndure;                       	/*   装备耐久度损失百分比 */
	tagDieDropItem_ToLua Items[DIE_DROP_ITEM_NUM];       
};
struct tagFengYinArm_ToLua
{
	int Num;                            
	int Items[MAX_FENGYIN_ARM];         
};

struct tagCSReviveInfo_ToLua
{
	int ReviveNum;                       	/*   原地复活每日次数 */
	int PerfectReviveNum;                	/*   完美复活次数 */
	int DayReviveNum;                    	/*  Ver.1676  */
	int DayPerfectReviveNum;             	/*  Ver.1676  */
	int PerfectReviveMoney;              	/*  Ver.1967  */
};


struct tagDaoBaInfo_ToLua
{
	unsigned int ClanID;                         
	unsigned int End;                            
	char Name[NAME_LEN];                 
	tdr_ulonglong ClanWID;                        
};

struct tagDieSvr_ToLua
{
	int End;                             	/*  Ver.455  */
	char KillerName[ROLE_NAME_LEN];      
	tagDieDrop_ToLua DieDrop;
	int AutoReviveTime;                  	/*  Ver.505 倒计时多少秒之后自动发送复活协议(只显示倒计时面板) */
	tagFengYinArm_ToLua FengYinArm;                      	/*  Ver.969  */
	tagCSReviveInfo_ToLua Revive;                          	/*  Ver.1632  */
	int NiXi;                            	/*  Ver.1821  */
	tagDaoBaInfo_ToLua DaoBaInfo;                       	/*  Ver.1930  */
};

// 修改角色名字
struct tagChangeRoleNameMsg
{
	char szOldName[ROLE_NAME_LEN];
	char szNewName[ROLE_NAME_LEN];
};

// struct tagBaseAttrInfo_ToLua
// {
// 	unsigned short ResultID;                       
// 	short Mul;                            
// };

struct tagRecastItemsDetail_ToLua
{
	unsigned short BaseAttrNum;                    
	tagBaseAttrInfo_ToLua BaseInfo[MAX_ARM_BASE_ATTR];    
};

struct tagRecastInfo_ToLua
{
	tdr_ulonglong WID;                             	/*   物品唯一标识 */
	unsigned short RecaseNum;                      
	tagRecastItemsDetail_ToLua RecastItemsDetail[MAX_ARM_RECAST_BATCH]; 	/*   十次装备重铸批量操作详细信息 */
};

struct tagRuneReset_ToLua
{
	int RuneType;                        	/*   RUNE_TYPE符文类型 1红 2蓝 3绿 */
	int RuneExID;                        	/*   重置获得的属性ID */
};

struct tagCowFinalRangeMember_ToLua
{
	short RecordID;                        	/*   进入排行榜的流水号,辅助排名 */
	short Time;                            	/*   比赛用时单位秒 */
	int Range;                           	/*   排名 */
	char Name[ROLE_NAME_LEN];               /*   角色名 */
	tdr_ulonglong RoleID;                   /*   角色ID */
	int Level;                           	/*   等级 */
	int Career;                          	/*   职业 */
	int TakeAward;                       	/*   是否已领取奖励 0-未领取 1-已领取 */
};

struct tagCowFinalRange_ToLua
{
	int MinRangeSec;                     	/*  Ver.2175 最末排名秒数 */
	int MinRangeIdx;                     	/*  Ver.2175 最末排名对应索引 */
	int RangeTime;                       	/*   排行榜生成时间 */
	int Num;                             	/*   排行榜数量 */
	tagCowFinalRangeMember_ToLua Members[MAX_COW_FINAL_RANGE];    	/*   成员列表 */
};

struct tagCSActionCowFinalRange_ToLua
{
	int OpType;                          	/*   操作类型|0打开ui 1更新信息 */
	tagCowFinalRange_ToLua CowFinalRange;                   	/*   排行榜信息 */
};

struct tagCSBaoXiangAwardInfo_ToLua
{
	int Succ;                           
	tdr_ulonglong ItemWID;                        
	int ItemID;                         
	int BaoXiangItemID;                  	/*   继续开哪个宝箱的ID */
	int Num;                             	/*   2153 */
};

struct tagBoxOneCardInfo_ToLua
{
	int Select;                         
	int GridIdx;                         	/*   位置 */
	int ItemID;                         
	int Num;                            
};

struct tagCSCardBoxInfo_ToLua
{
	int ID;                              	/*   箱子ID */
	int CostCount;                       	/*   花了几次钱 */
	int SelectCount;                     	/*   抽了几次 */
	int Wash;                            	/*   洗牌了几次 */
	int CardNum;                        
	tagBoxOneCardInfo_ToLua CardInfo[MAX_CARD_BOX_CARD_NUM];
};

struct tagCSCardBoxSelectInfo_ToLua
{
	int ID;                              	/*   箱子ID */
	int Count;                           	/*   第几次 */
	tagBoxOneCardInfo_ToLua SelectInfo;                      	/*   先翻开抽到的 */
	int CardNum;                        
	tagBoxOneCardInfo_ToLua CardInfo[MAX_CARD_BOX_CARD_NUM];
};

/* 航海大亨排行榜成员信息 */
struct tagHangHaiRangeMember_ToLua
{
	short UseTime;                         	/*   比赛用时单位秒 */
	int Range;                           	/*   排名 */
	char PlayerName[ROLE_NAME_LEN];       	/*   玩家名 */
	tdr_ulonglong RoleID;                          	/*   角色ID */
	unsigned char Level;                           	/*   等级 */
	char Career;                          	/*   职业 Bind Macrosgroup:CAREER_TYPE,*/
	int TakeAward;                       	/*   是否已领取奖励 */
};

/* 航海大亨排行榜 */
struct tagHangHaiRange_ToLua
{
	int MinRangeSec;                     	/*  Ver.2175 最末排名秒数 */
	int MinRangeIdx;                     	/*  Ver.2175 最末排名对应索引 */
	int RangeTime;                       	/*   排行榜生成时间 */
	int Num;                             	/*   排行榜成员数量 */
	tagHangHaiRangeMember_ToLua Members[MAX_HANG_HAI_RANGE];     	/*   成员列表 */
};

struct tagCSHangHaiRange_ToLua
{
	int OpType;                          	/*   操作类型|0打开ui 1更新信息 */
	tagHangHaiRange_ToLua HangHaiRange;                    	/*   排行榜信息 */
};

struct tagStorePageOne_ToLua
{
	int ID;                             
	char StoreName[NAME_LEN];            
	char RoleName[NAME_LEN];              	/*  Ver.2121  */
};

struct tagStorePageSvr_ToLua
{
	int Page;                            	/*   第几页,0开始 */
	int PageCount;                       	/*   一共几页 */
	int Num;                            
	tagStorePageOne_ToLua PageOne[STORE_PAGE_MAX];        
};

struct tagStoreOpenUI_ToLua
{
	tagStore_ToLua Store;                           	/*   自己的商铺信息,StoreID为0表示没有商铺 */
	tagStorePageSvr_ToLua StorePageSvr;                    	/*   当前页信息 */
	int NpcID;                          
};

struct tagStoreOne_ToLua
{
	tagStore_ToLua Store;                          
	int Self;                            	/*   1-自己的商铺 */
};

struct tagStoreGridChg_ToLua
{
	int ID;                             
	int Max;                            
};

struct tagStoreMoneyChg_ToLua
{
	int ID;                             
	int Money;                          
};

struct tagStoreItemChg_Tolua
{
	int ID;                             
	tagStoreGrid_ToLua StoreGrid;                      
};

struct tagStoreCloseSvr_Tolua
{
	int ID;                             
};

struct tagStoreOpenSvr_ToLua
{
	int Succ;                            	/*   0-成功 */
	int ID;                             
	char StoreName[NAME_LEN];            
};

union tagSvrStoreData_ToLua
{
	tagStoreOne_ToLua One;                             	/* STORE_SVR_OP_ONE_INFO,   */
	tagStorePageSvr_ToLua Page;                            	/* STORE_SVR_OP_PAGE,   */
	tagStoreGridChg_ToLua GridChg;                         	/* STORE_SVR_OP_GRID_CHG,   */
	tagStoreMoneyChg_ToLua MoneyChg;                        	/* STORE_SVR_OP_MONEY_CHG,   */
	tagStoreItemChg_Tolua ItemChg;                         	/* STORE_SVR_OP_ITEM_CHG,   */
	tagStoreCloseSvr_Tolua Close;                           	/* STORE_SVR_OP_CLOSE,   */
	tagStoreOpenSvr_ToLua Open;                            	/* STORE_SVR_OP_OPEN,   */
	tagStoreOpenUI_ToLua OpenUI;                          	/* STORE_SVR_OP_OPEN_UI, Ver.2124  */
};

/* 跑商道具右边的ui */
struct tagPaoShangRightUi_ToLua
{
	int RightItemID;                    
	int UserSellPrice;                   	/*   现在用户卖出的价格 */
	int HaveItemNum;                     	/*   用户拥有该物品的数量 */
	int SellTotalPrice;                  	/*   卖出的总价 */
	int CostPrice;                       	/*   成本价 */
};

/* 跑商道具左边的ui */
struct tagPaoShangLeftUi_ToLua
{
	int LeftItemID;                     
	int UserBuyPrice;                    	/*   现在用户购买需要的价格 */
	int BuyRemainNum;                    	/*   还剩多少数量 */
};

struct tagCSPaoShangUIInfo_ToLua
{
	int iNpcPaoShangID;                  	/*  Ver.2208 npc跑商商人的memid */
	int YinPiao;                         	/*   银票余额 */
	int JiaoPiao;                        	/*   交票金额 */
	int RightNum;                        	/*   右边ui道具数量 */
	tagPaoShangRightUi_ToLua PaoShangRightUi[MAX_PAOSHANG_DAOJU_NUM]; 	/*   右边道具列表 */
	int LeftNum;                         	/*   左边ui道具数量 */
	tagPaoShangLeftUi_ToLua PaoShangLeftUi[MAX_PAOSHANG_NPC_SHELL_ITEM_NUM]; 	/*   左边道具列表 */
};

struct tagHomeMon_ToLua
{
	int ID;                             
	unsigned char Lvl;                            
	int Exp;                            
	int Baoshi;                         
	int Qingjie;                        
	int Happy;                          
	int Jiankang;                       
	int GetTime;                        
};

struct tagHomeJiajuOne_ToLua
{
	int MonID;                          
	int Dir;                            
	tagResPos_ToLua Pos;                            
};

struct tagHomeHouse_ToLua
{
	unsigned char Num;                            
	tagHomeJiajuOne_ToLua One[HOME_MAX_JIAJU];            
};

struct tagHomeFarmOne_ToLua
{
	int Idx;                             	/*   第几块菜地 */
	int MonID;                           	/*   种植的monID */
	int ChengshuTime;                    	/*   植物成熟时间 */
	int MonMemID;                       
	int Stat;                            	/*   开垦状态 */
};

struct tagHomeLiuyanOne_ToLua
{
	char Name[NAME_LEN];                 
	char Text[HOME_MAX_LIUYAN_TEXT];     
	int Time;                           
};

struct tagHomeLiuyan_ToLua
{
	unsigned char Num;                            
	tagHomeLiuyanOne_ToLua One[HOME_MAX_LIUYAN];           
};

struct tagHomeSiol_ToLua
{
	int Lvl;                            
	int Exp;                            
	int Num;                             	/*   土地数 */
	tagHomeFarmOne_ToLua One[FARM_MAX_SOIL];             
};

struct tagHomeInfo_ToLua
{
	char Name[NAME_LEN];                  	/*   家园名字 */
	unsigned char Stat;                            	/*   HOME_STAT */
	int Sale;                            	/*   出售价格,大于0表示出售 */
	int PworldinstID;                   
	tagHomeLiuyan_ToLua Liuyan;                          	/*   留言 */
	tagHomeMon_ToLua Pet;                             	/*   宠物 */
	tagHomeHouse_ToLua House;                           	/*   住宅 */
	tagHomeSiol_ToLua Siol;                            	/*   菜地 */
	unsigned int LastEnterTime;                   	/*  Ver.2530  */
};

struct tagHome_ToLua
{
	unsigned int HomeID;                         
	tdr_ulonglong RoleID;                         
	char RoleName[NAME_LEN];             
	char Lvl;                            
	tagHomeInfo_ToLua Info;                           
	unsigned int CreateTime;                     
};

struct tagHomeSvrPage_ToLua
{
	char CountPage;                       	/*   总页数 */
	char Page;                            	/*   第几页 */
	int CountHome;                       	/*  Ver.2531  */
	char Num;                             	/*   当前页有多少个 */
	tagHome_ToLua Homes[HOME_PAGE_MAX];           
};

struct tagHomeSvrSingle_ToLua
{
	tagHome_ToLua Home;                           
};

struct tagHomeSvrFarm_ToLua
{
	tagHomeSiol_ToLua Farm;                           
};

struct tagHomeSvrHouse_ToLua
{
	tagHomeHouse_ToLua House;                          
};

struct tagHomeSvrDel_ToLua
{
	int ID;                             
};

struct tagHomeSvrPet_ToLua
{
	tagHomeMon_ToLua Pet;                            
};

struct tagHomeSvrNameChg_ToLua
{
	unsigned int ID;                             
	char Name[NAME_LEN];                  	/*   新名字 */
};

struct tagHomeSvrStatChg_ToLua
{
	unsigned int ID;                             
	int Stat;                           
};

struct tagHomeSvrLiuyanChg_ToLua
{
	unsigned int ID;                             
	tagHomeLiuyan_ToLua Liuyan;                         
};

struct tagHomeSvrSaleChg_ToLua
{
	unsigned int ID;                             
	int Sale;                           
};

struct tagHomeSvrBuy_ToLua
{
	unsigned int ID;                             
	char Name[NAME_LEN];                  	/*   新主人名字 */
};

union tagUIMsgBody
{
	tagSetLink			ChatSetLink;
	tagSysMsg 			SysMsg;
	tagChatReq			ChatReq;
	tagChatMsg			ChatMsg;
	tagShortCut			ShortCut;
	tagCoolTime			CoolTime;
	tagMaxMapInfo		MaxMapInfo;
	tagSkillFitting		SkillFitting;
	tagFittingSlot		FittingSlot;
	tagPowerCollect		PowerCollect;
	tagNpcDialog		NpcDialog;
	tagNpcOpt			NpcOpt;
	tagNpcLastDialog	NpcLastDialog;
	tagNpcCommitDialog	NpcCommitDialog;
	tagNpcId            NpcId;
	tagMidTipsMsg		MidTipsMsg;
	tagQuestTalk		QuestTalk;
	tagReviveMsg		ReviveInfo;
	tagAddFriendMsg		AddFriendInfo;
	tagPworldMsg		PworldInfo;
	tagSecondarySkill	SecondarySkill;
	tagGameActorMsg		Who;
	tagGPRSMsg			GPRSData;
	tagDieSvr_ToLua		DropData;
	tagChangeRoleNameMsg	ChangeNameData;
	tagRecastInfo_ToLua		RecastInfo;
	tagRuneReset_ToLua		RuneResetData;
	tagCSActionCowFinalRange_ToLua	CowFinalRangeData;
	tagCSBaoXiangAwardInfo_ToLua	BaoXiangAwardInfo;
	tagCSCardBoxInfo_ToLua			CardBoxInfo;
	tagCSCardBoxSelectInfo_ToLua	CardBoxSelectInfo;
	tagCSHangHaiRange_ToLua			HangHaiRangData;
	tagSvrStoreData_ToLua			StoreData;
	tagCSPaoShangUIInfo_ToLua		PaoShangData;
};

enum 
{
	DRAG_NONE = 1000,
	DRAG_BAG,
	DRAG_SHORTCUT,
	DRAG_SKILL,
	DRAG_WEARFORM,
	DRAG_RIDE,				// 从骑乘面板拖出的物品
	DRAG_PETARM,
	DRAG_PETFITTING,
	DRAG_SHOP,
	DRAG_SAVEITEM,
	DRAG_AWARDITEM,
	DRAG_TRADE,
	DRAG_MAIL,
	DRAG_STALL,
	DRAG_ARMPROCE,
	DRAG_GETMAILITEM,
	DRAG_STALLBUY,
	DRAG_TRUMPSKILL,
	DRAG_MARKET,			//商场临时背包
	DRAG_ENCHANT,			//附魔
	DRAG_PETOPERATION,
	DRAG_ARMBIND,			//绑定
	DRAG_ARMUNBIND,			//解绑
	DRAG_ARMSTARMOVE,		//品质转移
	DRAG_CONTAINERRESOLVE,	//背包分解
	DRAG_GEMCHARGE,			//宝石充能
	DRAG_CLAN_CITY,			//地图会标
	DRAG_ARM_RESOLVE,		//装备重铸
	DRAG_ARM_ADVRESOLVE,	//高级重铸
	DRAG_ARM_WASH,			//装备洗练
	DRAG_SPAR_SORB,			//晶石吸收
	DRAG_PET_AUTO_ITEM,		//宠物自动喂养
	DRAG_AUTO_ASSIST,		//自动辅助
	DRAG_SKILL_RUNE_COMBINE,	//符文合成
	DRAG_RIDE_MOVE,			//坐骑升级
	DRAG_ARM_LEVEL,			// 装备升级
	DRAG_SPAR_EPURATE,		//晶石提炼
	DRAG_ARM_ATTR_MOVE_SRC,	//装备属性转移
	DRAG_ARM_ATTR_MOVE_DST,	//装备属性转移
	DRAG_ARM_ATTR_MOVE_USE,	//装备属性转移
	DRAG_ARM_ATTR_RESET,	//装备属性重铸
	DRAG_ARM_STEP_UP,		//装备升阶
	DRAG_ARM_BASE_RESET,	//装备基础属性重置
	DRAG_ARM_SKILL_ACTIVE,	//装备属性激活
	DRAG_CLAN_PET_FEED,		//宠物寄养喂养
};

struct tagUIGV
{
	int		DragFrom;
};
//tolua_end
#pragma pack()

class GameUIMsg//tolua_export
	: public Ogre::Singleton<GameUIMsg>
{//tolua_export
public:
	GameUIMsg(void);
	~GameUIMsg(void);
	
	static GameUIMsg*		Create();
	//tolua_begin
	void					setUIMsg( const tagUIMsgBody& body );
	const tagUIMsgBody*		getUIMsg()const;	
	tagUIGV*				getUIGV() 
	{ 
		return &m_UIGV;
	}
	//tolua_end
private:
	tagUIMsgBody	m_UIMsg;
	tagUIGV			m_UIGV;

};//tolua_export
