#pragma once
//////////////////////////////////////////////////////////////////////////
// ͬUI������Ϣ����
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

// ������Ϣ

// ��Ʒ������
struct tagItemLink_ToLua
{
	int m_nItemId;
	int m_nItemDur;
	int InstVal1;                        	/*  Ver.1322  */
	int InstVal2;                        	/*  Ver.1322  */
	int InstVal3;                        	/*  Ver.1322  */
	int Flag;                            	/*  Ver.1324  */
};
// װ��������
struct tagArmLink_ToLua
{
	tagRoleArm_ToLua m_arm;
};

// ��������
struct tagMissionLink_ToLua
{
	int m_MissionId;
	int	m_nRecycleID;
};

// λ����������
struct tagPostionLink_ToLua
{
	int m_nMapId;
	int m_nX;
	int m_nY;
};

struct tagSecondarySkillLink_ToLua
{
	short SkillType;                       	/*   ��������: �ɿ��ҩ�ȵ� Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	short SkillLevel;                      	/*   ���ܵȼ� */
	int SkillVal;                        	/*   ���������� */
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

// ����ϵͳ��Ϣ
struct tagSysMsg
{
	char Type;                            	                         	
	char Times;					/*   ������ʾ������0������ */                 	
	char Interval;              /*   ������ʾ��� */     
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
	short ListType;                        	/*  Ver.275 ���ĵ��ߵı�����λ Bind Macrosgroup:LIST_TYPE,*/
	unsigned short GridIdx;                         	/*  Ver.275 ���ĵ��ߵ���λ���� */
	unsigned char OnlineType;                      	/*  Ver.578 �����ߵ��������� Bind Macrosgroup:ONLINE_TYPE,*/
	bool isVip;
};

// ��ʱ��е�����
struct tagShortCut
{
	int		m_nNum;
	int		m_nType;
	int		m_Id;
	int		m_Lv;
	int		m_nFittingIndex;
	char	m_Des[MAX_PATH];
};

// �����
struct tagTmpShortCut
{
	int		m_nNum;
	int		m_nType;
	int		m_Id;
	int		m_Lv;
	int		m_nFittingIndex;
};

// ��ȴʱ��
struct tagCoolTime
{
	int	  num;
	float starttime;
	float endtime;
};

// �����ͼ��Ϣ
struct tagMaxMapInfo	// ��Ϊ������ǰ������Դ���Ĳ����������Ϣ
{
	int			nPlayerX;		// ��Ϊ��ɫ����ڵ�ǰ��ʾ��ͼ��Xƫ��
	int			nPlayerY;		// ��Ϊ��ɫ����ڵ�ǰ��ʾ��ͼ��Yƫ��
	float		fZoom;			// ��Ϊ��ǰ��ʾ�ĵ�ͼ�������ͼ������ֵ 	
	int			nMapPosStartX;	// ��Ϊ��ǰ��ʾ�ĵ�ͼ�����Ͻ��������������������ͼ����������Ϊ1024��ȵ����Ͻǵ�Xƫ��
	int			nMapPosStartY;	// ��Ϊ��ǰ��ʾ�ĵ�ͼ�����Ͻ��������������������ͼ����������Ϊ1024�߶ȵĵ����Ͻǵ�Yƫ��
	int			nMapPosWidth;	// ��Ϊ��ǰ��ʾ�ĵ�ͼ�������������������϶�Ӧ�Ľ�ȡ��������Ŀ��,������ʵ����Ϸ�����Ŀ��,Ҳ����m_ViewWidth
	int			nMapPosHeight;	// ��Ϊ��ǰ��ʾ�ĵ�ͼ�������������������϶�Ӧ�Ľ�ȡ��������ĸ߶�,������ʵ����Ϸ�����ĸ߶�,Ҳ����m_ViewHeight
	float		fAngle;			// ��Ϊ����ɫƫת�ĽǶ�
	float		fCamAngle;		// ��Ϊ����ɫ��Ӧ���������ƫת�ĽǶ�
	int			mapId;			// ��Ϊ����ɫ��ǰ���ڵ�ͼ��ID
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

// ����Ի�
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
	unsigned int DropMoney;                       	/*  Ver.917 ���˶���Ǯ */
	unsigned int ReviveMoney;                     	/*  Ver.917 �ָܻ�����Ǯ */
	unsigned int DropExp;                         	/*  Ver.917 ���˶���exp */
	unsigned int ReviveExp;                       	/*  Ver.917 �ָܻ�����exp */

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
	int ArmEndure;                       	/*   װ���;ö���ʧ�ٷֱ� */
	tagDieDropItem_ToLua Items[DIE_DROP_ITEM_NUM];       
};
struct tagFengYinArm_ToLua
{
	int Num;                            
	int Items[MAX_FENGYIN_ARM];         
};

struct tagCSReviveInfo_ToLua
{
	int ReviveNum;                       	/*   ԭ�ظ���ÿ�մ��� */
	int PerfectReviveNum;                	/*   ����������� */
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
	int AutoReviveTime;                  	/*  Ver.505 ����ʱ������֮���Զ����͸���Э��(ֻ��ʾ����ʱ���) */
	tagFengYinArm_ToLua FengYinArm;                      	/*  Ver.969  */
	tagCSReviveInfo_ToLua Revive;                          	/*  Ver.1632  */
	int NiXi;                            	/*  Ver.1821  */
	tagDaoBaInfo_ToLua DaoBaInfo;                       	/*  Ver.1930  */
};

// �޸Ľ�ɫ����
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
	tdr_ulonglong WID;                             	/*   ��ƷΨһ��ʶ */
	unsigned short RecaseNum;                      
	tagRecastItemsDetail_ToLua RecastItemsDetail[MAX_ARM_RECAST_BATCH]; 	/*   ʮ��װ����������������ϸ��Ϣ */
};

struct tagRuneReset_ToLua
{
	int RuneType;                        	/*   RUNE_TYPE�������� 1�� 2�� 3�� */
	int RuneExID;                        	/*   ���û�õ�����ID */
};

struct tagCowFinalRangeMember_ToLua
{
	short RecordID;                        	/*   �������а����ˮ��,�������� */
	short Time;                            	/*   ������ʱ��λ�� */
	int Range;                           	/*   ���� */
	char Name[ROLE_NAME_LEN];               /*   ��ɫ�� */
	tdr_ulonglong RoleID;                   /*   ��ɫID */
	int Level;                           	/*   �ȼ� */
	int Career;                          	/*   ְҵ */
	int TakeAward;                       	/*   �Ƿ�����ȡ���� 0-δ��ȡ 1-����ȡ */
};

struct tagCowFinalRange_ToLua
{
	int MinRangeSec;                     	/*  Ver.2175 ��ĩ�������� */
	int MinRangeIdx;                     	/*  Ver.2175 ��ĩ������Ӧ���� */
	int RangeTime;                       	/*   ���а�����ʱ�� */
	int Num;                             	/*   ���а����� */
	tagCowFinalRangeMember_ToLua Members[MAX_COW_FINAL_RANGE];    	/*   ��Ա�б� */
};

struct tagCSActionCowFinalRange_ToLua
{
	int OpType;                          	/*   ��������|0��ui 1������Ϣ */
	tagCowFinalRange_ToLua CowFinalRange;                   	/*   ���а���Ϣ */
};

struct tagCSBaoXiangAwardInfo_ToLua
{
	int Succ;                           
	tdr_ulonglong ItemWID;                        
	int ItemID;                         
	int BaoXiangItemID;                  	/*   �������ĸ������ID */
	int Num;                             	/*   2153 */
};

struct tagBoxOneCardInfo_ToLua
{
	int Select;                         
	int GridIdx;                         	/*   λ�� */
	int ItemID;                         
	int Num;                            
};

struct tagCSCardBoxInfo_ToLua
{
	int ID;                              	/*   ����ID */
	int CostCount;                       	/*   ���˼���Ǯ */
	int SelectCount;                     	/*   ���˼��� */
	int Wash;                            	/*   ϴ���˼��� */
	int CardNum;                        
	tagBoxOneCardInfo_ToLua CardInfo[MAX_CARD_BOX_CARD_NUM];
};

struct tagCSCardBoxSelectInfo_ToLua
{
	int ID;                              	/*   ����ID */
	int Count;                           	/*   �ڼ��� */
	tagBoxOneCardInfo_ToLua SelectInfo;                      	/*   �ȷ����鵽�� */
	int CardNum;                        
	tagBoxOneCardInfo_ToLua CardInfo[MAX_CARD_BOX_CARD_NUM];
};

/* ����������а��Ա��Ϣ */
struct tagHangHaiRangeMember_ToLua
{
	short UseTime;                         	/*   ������ʱ��λ�� */
	int Range;                           	/*   ���� */
	char PlayerName[ROLE_NAME_LEN];       	/*   ����� */
	tdr_ulonglong RoleID;                          	/*   ��ɫID */
	unsigned char Level;                           	/*   �ȼ� */
	char Career;                          	/*   ְҵ Bind Macrosgroup:CAREER_TYPE,*/
	int TakeAward;                       	/*   �Ƿ�����ȡ���� */
};

/* ����������а� */
struct tagHangHaiRange_ToLua
{
	int MinRangeSec;                     	/*  Ver.2175 ��ĩ�������� */
	int MinRangeIdx;                     	/*  Ver.2175 ��ĩ������Ӧ���� */
	int RangeTime;                       	/*   ���а�����ʱ�� */
	int Num;                             	/*   ���а��Ա���� */
	tagHangHaiRangeMember_ToLua Members[MAX_HANG_HAI_RANGE];     	/*   ��Ա�б� */
};

struct tagCSHangHaiRange_ToLua
{
	int OpType;                          	/*   ��������|0��ui 1������Ϣ */
	tagHangHaiRange_ToLua HangHaiRange;                    	/*   ���а���Ϣ */
};

struct tagStorePageOne_ToLua
{
	int ID;                             
	char StoreName[NAME_LEN];            
	char RoleName[NAME_LEN];              	/*  Ver.2121  */
};

struct tagStorePageSvr_ToLua
{
	int Page;                            	/*   �ڼ�ҳ,0��ʼ */
	int PageCount;                       	/*   һ����ҳ */
	int Num;                            
	tagStorePageOne_ToLua PageOne[STORE_PAGE_MAX];        
};

struct tagStoreOpenUI_ToLua
{
	tagStore_ToLua Store;                           	/*   �Լ���������Ϣ,StoreIDΪ0��ʾû������ */
	tagStorePageSvr_ToLua StorePageSvr;                    	/*   ��ǰҳ��Ϣ */
	int NpcID;                          
};

struct tagStoreOne_ToLua
{
	tagStore_ToLua Store;                          
	int Self;                            	/*   1-�Լ������� */
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
	int Succ;                            	/*   0-�ɹ� */
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

/* ���̵����ұߵ�ui */
struct tagPaoShangRightUi_ToLua
{
	int RightItemID;                    
	int UserSellPrice;                   	/*   �����û������ļ۸� */
	int HaveItemNum;                     	/*   �û�ӵ�и���Ʒ������ */
	int SellTotalPrice;                  	/*   �������ܼ� */
	int CostPrice;                       	/*   �ɱ��� */
};

/* ���̵�����ߵ�ui */
struct tagPaoShangLeftUi_ToLua
{
	int LeftItemID;                     
	int UserBuyPrice;                    	/*   �����û�������Ҫ�ļ۸� */
	int BuyRemainNum;                    	/*   ��ʣ�������� */
};

struct tagCSPaoShangUIInfo_ToLua
{
	int iNpcPaoShangID;                  	/*  Ver.2208 npc�������˵�memid */
	int YinPiao;                         	/*   ��Ʊ��� */
	int JiaoPiao;                        	/*   ��Ʊ��� */
	int RightNum;                        	/*   �ұ�ui�������� */
	tagPaoShangRightUi_ToLua PaoShangRightUi[MAX_PAOSHANG_DAOJU_NUM]; 	/*   �ұߵ����б� */
	int LeftNum;                         	/*   ���ui�������� */
	tagPaoShangLeftUi_ToLua PaoShangLeftUi[MAX_PAOSHANG_NPC_SHELL_ITEM_NUM]; 	/*   ��ߵ����б� */
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
	int Idx;                             	/*   �ڼ���˵� */
	int MonID;                           	/*   ��ֲ��monID */
	int ChengshuTime;                    	/*   ֲ�����ʱ�� */
	int MonMemID;                       
	int Stat;                            	/*   ����״̬ */
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
	int Num;                             	/*   ������ */
	tagHomeFarmOne_ToLua One[FARM_MAX_SOIL];             
};

struct tagHomeInfo_ToLua
{
	char Name[NAME_LEN];                  	/*   ��԰���� */
	unsigned char Stat;                            	/*   HOME_STAT */
	int Sale;                            	/*   ���ۼ۸�,����0��ʾ���� */
	int PworldinstID;                   
	tagHomeLiuyan_ToLua Liuyan;                          	/*   ���� */
	tagHomeMon_ToLua Pet;                             	/*   ���� */
	tagHomeHouse_ToLua House;                           	/*   סլ */
	tagHomeSiol_ToLua Siol;                            	/*   �˵� */
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
	char CountPage;                       	/*   ��ҳ�� */
	char Page;                            	/*   �ڼ�ҳ */
	int CountHome;                       	/*  Ver.2531  */
	char Num;                             	/*   ��ǰҳ�ж��ٸ� */
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
	char Name[NAME_LEN];                  	/*   ������ */
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
	char Name[NAME_LEN];                  	/*   ���������� */
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
	DRAG_RIDE,				// ���������ϳ�����Ʒ
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
	DRAG_MARKET,			//�̳���ʱ����
	DRAG_ENCHANT,			//��ħ
	DRAG_PETOPERATION,
	DRAG_ARMBIND,			//��
	DRAG_ARMUNBIND,			//���
	DRAG_ARMSTARMOVE,		//Ʒ��ת��
	DRAG_CONTAINERRESOLVE,	//�����ֽ�
	DRAG_GEMCHARGE,			//��ʯ����
	DRAG_CLAN_CITY,			//��ͼ���
	DRAG_ARM_RESOLVE,		//װ������
	DRAG_ARM_ADVRESOLVE,	//�߼�����
	DRAG_ARM_WASH,			//װ��ϴ��
	DRAG_SPAR_SORB,			//��ʯ����
	DRAG_PET_AUTO_ITEM,		//�����Զ�ι��
	DRAG_AUTO_ASSIST,		//�Զ�����
	DRAG_SKILL_RUNE_COMBINE,	//���ĺϳ�
	DRAG_RIDE_MOVE,			//��������
	DRAG_ARM_LEVEL,			// װ������
	DRAG_SPAR_EPURATE,		//��ʯ����
	DRAG_ARM_ATTR_MOVE_SRC,	//װ������ת��
	DRAG_ARM_ATTR_MOVE_DST,	//װ������ת��
	DRAG_ARM_ATTR_MOVE_USE,	//װ������ת��
	DRAG_ARM_ATTR_RESET,	//װ����������
	DRAG_ARM_STEP_UP,		//װ������
	DRAG_ARM_BASE_RESET,	//װ��������������
	DRAG_ARM_SKILL_ACTIVE,	//װ�����Լ���
	DRAG_CLAN_PET_FEED,		//�������ι��
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
