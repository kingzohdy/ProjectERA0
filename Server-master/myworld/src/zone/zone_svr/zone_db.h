
#ifndef ZONE_DB_H
#define ZONE_DB_H

#include "zone_svr.h"
#include "auth_svr.h"

#pragma pack(1)

enum tagDBCmds 
{
    DB_ROLE_LIST = 1 , 
    DB_ROLE_INSERT,		
    DB_CHECK_NAME,
    DB_ROLE_UPDATE,
    DB_ROLE_SELECT,
    DB_ROLE_DELETE,
    DB_BUDDY_ASYN_INIT,
    DB_BUDDY_CHG,
    DB_GLOBAL_UPDATE,
    DB_GLOBAL_SELECT,
    DB_CLAN_COUNT_SELECT,
    DB_CLAN_UPDATE,
    DB_CLAN_SELECT,
    DB_CLAN_DELETE,
    DB_MAIL_COUNT_SELECT,
    DB_MAIL_UPDATE,
    DB_MAIL_SELECT,
    DB_MAIL_DELETE,
    DB_MAIL_ROLE_COUNT_SELECT,
    DB_CHG_NAME_SELECT,
    DB_CHG_NAME_UP,
    DB_CHG_CLANNAME_SELECT,
    DB_ROLE_DELETE_RESPITE,
    DB_CS_CHG_NAME_UP_ROLEDATA,
    DB_UPDATE_ROLE_LIST,
    DB_SELECT_PLAYER_ROLE_LIST,
    DB_SELECT_COUNT_TRADE_SELL_RES,
    DB_SELECT_COUNT_TRADE_BUY_RES,
    DB_SELECT_TRADE_SELL_RES,
    DB_SELECT_TRADE_BUY_RES,
    DB_DELETE_TRADE_SELL_RES,
    DB_DELETE_TRADE_BUY_RES,  
    DB_INSERT_TRADE_SELL_RES,
    DB_INSERT_TRADE_BUY_RES,    
    DB_UPDATE_TRADE_SELL_RES,
    DB_UPDATE_TRADE_BUY_RES,
    DB_CS_DEL_ROLE_SELECT,
    DB_CS_DEL_ROLE_DELETE,
    DB_CS_DEL_ROLE_UPDATE,
    DB_SHADOW_ROLE_SELECT,
    DB_SELECT_COUNT_SHOPDEF_RES,
    DB_SELECT_COUNT_SHOPMSGDEF_RES,
    DB_SELECT_SHOPDEF_RES,
    DB_SELECT_SHOPMSGDEF_RES,
    DB_SELECT_COUNT_SHOPLIMITDEF_RES,
    DB_SELECT_SHOPLIMITDEF_RES,
    DB_UPDATE_SHOPLIMITDEF_RES,
    DB_RANGE_COUNT_SELECT,
    DB_RANGE_SELECT,
    DB_RANGE_DELETE,
    DB_RANGE_UPDATE,
    DB_ROLE_DROP_SELECT,
    DB_ROLE_DROP_UPDATE,
    DB_ROLE_DEL_NAMEID_RES,
    DB_SHADOW_SELECT_RES,
    DB_ROLEDETAIL_SELECT_RES,
    DB_ROLEDETAIL_UPDATE_RES,
    DB_SELECT_ACC_SAFEPWD,
    DB_ROLE_DELETE_REALLY,
    DB_ROLE_UPDATE_UNDEL,
    DB_ACC_SHARE_SELECT,
    DB_ACC_SHARE_INSERT,
    DB_UNFENGYIN_SELECT_NAMEID,
    DB_GMLIMIT_SELECT,
    DB_SELECT_VIP_BAR_DEF_RES,
	DB_SELECT_COUNT_VIP_BAR_DEF_RES,
	DB_SELECT_VIP_BAR_LEVEL_DEF_RES,
	DB_SELECT_COUNT_VIP_BAR_LEVEL_DEF_RES,
	DB_SELECT_VIP_BAR_NUM_DEF_RES,
	DB_SELECT_COUNT_VIP_BAR_NUM_DEF_RES,
	DB_SELECT_WATCH_RES,
	DB_SELECT_ACCOUNT_RES,
	DB_UPDATE_ACCOUNT_RES,
	DB_ROLE_SECOND_PWD_SELECT,
	DB_ROLE_BASE_SELECT,
	DB_ROLE_SYNC_RELATION_SELECT,
	DB_ROLE_SYNC_RELATION_UPDATE,
	DB_STORE_COUNT,
	DB_STORE_SELECT,
	DB_STORE_INSERT_RES,
	DB_STORE_DELETE_RES,
	DB_STORE_UPDATE_RES,
	DB_UPDATE_MOBILE_RES, 	
	DB_ROLE_MISC_INFO_SELECT,
	DB_ROLE_BUDDY_SELECT,
	DB_ROLE_SKILL_INFO_SELECT,

	DB_REGION_GLOBAL_SELECT,

	DB_HOME_COUNT,
	DB_HOME_SELECT,
	DB_HOME_INSERT_RES,
	DB_HOME_DELETE_RES,
	DB_HOME_UPDATE_RES,
};

enum OpType
{
	MAIL_OP = 1,
	CLAN_OP,
	FACTION_OP,
	RANGE_OP,
	NATION_OP,
	CO_OP,
};

typedef struct
{
	CSROLENEWREQ stNew;
	tdr_ulonglong ullGid;
}NewPlayerInfo;

typedef struct
{
	char szOwner[ROLE_NAME_LEN];
	int iMailIdx;
	int iMailVersion;
}ActiveMailboxInfo;

typedef struct
{
	int Cmd;                            
    	int ConnIdx;                        
    	TIPINFO IPInfo;
	int Uin;
	tdr_longlong RoleID;
	char szName[NAME_LEN] ;
	char szOldName[NAME_LEN] ;
}ChgRoleName;

typedef struct
{
	int Cmd;                            
    	int ConnIdx;                        
    	TIPINFO IPInfo;
	int Uin;
	tdr_ulonglong RoleID;
	char szName[NAME_LEN] ;
	
}CsDelRole;

typedef struct
{
	int iMainRange;
	int iSubType;
	int iIdx;
}RangeUpdate;

typedef union {
		ROLEPET stPet;
		ROLEGRID stGrid;
} DropObj;


typedef struct
{
	tdr_ulonglong ullRoleID;
	int iType; //4类型ROLE_DROP_TYPE {Grid: 0   Pet: 1}
	DropObj stData;
}DropObjUpdate;

typedef struct
{
	int iUin;
	tdr_ulonglong 	ullRoleID;
	unsigned int 	GodCoin;
	ROLEMEMBER 		RoleInfo;                        	/*   角色信息 */
	char szAccount[ACCOUNT_LOGIN_NAME_LEN+5];
}RoleDetailUpdate;

typedef struct
{
	tdr_ulonglong 	ullPetWid;
	tdr_ulonglong ullRoleID;
}WatchRole;

typedef struct
{
	int Uin;
	tdr_ulonglong ullRoleID;
}RoleBaseSelect;

typedef struct
{
	int iTotalVal;
}TeacherAdd;

typedef struct
{
	int iExp;
	int iVal;
}StudentAdd;

typedef union {
	TeacherAdd stTeacherAdd;
	StudentAdd stStudentAdd;
}SyncSelectUnion;

typedef struct
{
	int Uin;
	tdr_ulonglong ullRoleID;
	int iType; //0 TeacherAddVal, 1StudentAddExp, 2Del
	SyncSelectUnion stSelectInfo;
	char szNewName[32];
}RoleSyncRelationSelect;

typedef struct
{
	char szMaxBuf[1024];
}DBAsynCmdDataMaxBuff;

enum tagDBWeddingCmds
{
	DB_MISC_WEDDING_SET_NULL = 1 ,
	DB_WEDDING_REMOVE_RELATION,
	DB_WEDDING_REMOVE_FRIEND,
};

enum tagDBMiscInfoCmds 
{
	DB_MISC_INFO_DSN_UPDATE = 1 ,
	DB_MISC_INFO_WEDDINGINFO_UPDATE,
};

typedef struct
{
	tdr_ulonglong ullRoleID;
	unsigned int uiDsn;
}MiscInfoDsn;

typedef struct
{
	tdr_ulonglong ullRoleID;
	int type;
}MiscInfoWedding;

typedef union
{
	MiscInfoDsn stMiscInfoDsn;
	MiscInfoWedding stMiscInfoWedding;
}DBMiscInfoData;

typedef struct {
	int iCmd;
	DBMiscInfoData AsynMiscInfo;
}DBMiscInfo;

typedef struct
{
	tdr_ulonglong ullRoleID;
	tdr_ulonglong ullOtherRoleID;
	int type;
}DBRoleBuddy;

enum tagDBSkillInfoCmds 
{
	DB_SKILL_INFO_DELETE = 1 ,
};

typedef struct
{
	unsigned short SkillID;                        
}SkillInfoDeleteData;

typedef struct
{
	tdr_ulonglong ullRoleID;
	int iDeleteNum;
	SkillInfoDeleteData stSkillInfoDeleteData[6];
}SkillInfoDelete;

typedef union
{
	SkillInfoDelete stSkillInfoDelete;
}DBRoleSkillData;

typedef struct
{
	int iCmd;
	DBRoleSkillData AsynRoleSkill;
}DBRoleSkillInfo;

typedef union
{
	tdr_ulonglong ullRoleID ; //DB_ROLE_DELETE、DB_ROLE_DELETE_RESPITE
	NewPlayerInfo stNewPlayer; //DB_CHECK_NAME
	ROLELISTINFO stRoleListInfo; //DB_ROLE_INSERT
	DBBuddyChg stBuddyChg;//DB_BUDDY_CHG
	unsigned int nClanID; // DB_CLAN_DELETE or DB_CLAN_UPDATE
	ActiveMailboxInfo stActiveMailboxInfo; // DB_MAIL_XXXXX
	//char szName[NAME_LEN] ;// DB_CHG_NAME_UPDATE
	ChgRoleName stChgRoleName;
	int iUin;
	SHOPDEF stShopDef;
	CSSHOPPINGBUYREQ stShoppingBuyReq;
	TRADESELL stTradeSell;
	TRADEBUY stTradeBuy;
	CsDelRole stCsDelRole;
	RangeUpdate stRangeUpdate;
	DropObjUpdate stDropObjUpdate;
	RoleDetailUpdate stDetailUpdate;
	WatchRole stWatchRole;
	SelectInfo stSelectInfo;//DB_SELECT_ACCOUNT_RES
	RoleBaseSelect stRoleBaseSelect;
	RoleSyncRelationSelect stSyncRelationSelect;
	char szMaxBuf[1024];
	DBCOROUTINERPC stDBCoroutineRpc; //CO_OP
	DBMiscInfo stDBMiscInfo; //DB_ROLE_MISC_INFO_SELECT
	DBRoleBuddy stDBRoleBuddy; //DB_ROLE_BUDDY_SELECT
	DBRoleSkillInfo stDBRoleSkillInfo; //DB_ROLE_SKILL_INFO_SELECT
}DBAsynCmdData;

char szWatchDBAsynCmdData[sizeof(DBAsynCmdDataMaxBuff) - sizeof(DBAsynCmdData)];

typedef struct {
	int iCmd;
	DBAsynCmdData AsynCmd;
}DBAsyn;

typedef struct
{
	int iPlayerNum;
	int iMap;
	int DefIdx;
}BirthMapSort;

#pragma pack() 

#define NEW_VALLEGE_DISP  100
#define MAX_COOL_TIME_ERR_MUL 3 //最大冷却时间误差倍数

#define ERROR_FIND_UINCACHE -2			// 查不到uincache
#define ERROR_UINCACHE_FIND_ROLE -1	// uincahce中查不到角色

void z_init_torm_head(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKGHEAD *pstHead, int iCmd, char *pszMetaName, int  iUin, DBAsyn *pstDBAsyn);
int z_torm_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg, int iDstID);
int z_db_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen, int iMsgSrc);
int z_db_role_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_torm_exchg_ver(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_db_role_insert(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLENEWREQ *pstRoleNewReq);
int z_db_role_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLELOGINREQ *pstRoleLoginReq);
int z_db_role_del(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEDELREQ *pstRoleDelReq,int iHasGodCoin);
tdr_ulonglong z_get_gid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_db_role_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int OperType);
int z_send_rolelist(ZONESVRENV* pstEnv, Player *pstPlayer, UinCache *pstUinCache);
int z_role_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iCameraDir, int iDir);
int z_init_role(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iOperType);
int z_role_login_notify( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iCameraDir,short LoginSrc );
int z_db_role_logout_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_account_db_update_deltime(ZONESVRENV* pstEnv, Player *pstPlayer,
	                              				tdr_ulonglong ullRoleID, int iDelTime);
int z_account_db_del(ZONESVRENV* pstEnv, int iUin, tdr_ulonglong ullRoleID);
int z_account_db_insert(ZONESVRENV* pstEnv, Player *pstPlayer, 
	                      const ACCOUNTROLE *pstAccRole);
int z_account_db_update_name(ZONESVRENV* pstEnv, int iUin, 
	                           const ACCOUNTROLENAME *pstAccRoleName);
int z_account_db_update_archive(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_db_role_list_x(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iUin);
int z_del_role_res_cs(ZONESVRENV* pstEnv,DBAsyn*pstDbAsyn);
int z_db_del_time_update( ZONESVRENV * pstEnv,int iUin, const ROLEDELINFO* pstRoleDelInfo,int iCmd);

int z_role_drop_record(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, void *pstData);

int z_db_role_del_name_req(ZONESVRENV* pstEnv, Player *pstPlayer,  tdr_ulonglong ullRoleID);

int z_db_role_del_name_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstTormPkg, tdr_ulonglong ullRoleID );
int z_db_role_drop_update_name(ZONESVRENV* pstEnv, tdr_ulonglong ullWID, const char * pszName);
int cs_chg_pos_up_cache(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, CHGPOSUPCACHEREQ* pstReq);

int is_role_login_valid_pworldinst(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst);

int is_battle_runaway(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst);
int z_db_shadowroledata_up_name(ZONESVRENV* pstEnv, tdr_ulonglong ullWID, const char * pszName);

int z_db_get_roledetail_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEDELREQ *pstRoleDelReq);

int z_db_get_roledetail_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);

int z_db_set_roledetail_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMUPDATERES *pstUpdateRes, DBAsyn *pstDBAsyn);

int z_db_set_roledetail_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
				Player *pstPlayer, CHGROLEDETAIL *pstChgRoleDetail,unsigned int GodCoin,RoleDetailUpdate *pstDetailUpdate);

int z_check_uin_cache(ZONESVRENV* pstEnv,UinCache *pstUinCache,Player *pstPlayer);
int z_role_del_up_else(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID,int iCareer, char *pszName);
int player_undel(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
ROLELISTINFO *z_get_one_rolelistinfo(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID,int *piRet);
int set_show_flag(unsigned char *p, int iFlag);

int db_select_shadow_req(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID);
int z_db_update_acc_share_req(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_new_role_birth(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_db_acc_share_select(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player * pstPlayer);
int z_db_unfengyin_select_nameid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
									tdr_ulonglong ullGid,int iUin,tdr_ulonglong ullFind);

int z_role_del_respite(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEDELREQ *pstRoleDelReq);


//linzhongkai
int z_select_role_secondpwd(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_role_secondpwd_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);
int z_get_role_misc_info(ZONESVRENV* pstEnv,DBMiscInfo *pstDBMiscInfo,tdr_ulonglong ullRoleID);
int z_db_misc_info_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);
int z_db_misc_info_dsn_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBMiscInfo *pstDBMiscInfo);
int z_db_update_misc_info(ZONESVRENV* pstEnv, MISCROLEINFO *pstMiscRoleInfo);

int z_db_wedding_remove_friend(ZONESVRENV* pstEnv,ROLEBUDDY *pstRoleBuddy,tdr_ulonglong ullOtherRoleID);
int z_db_wedding_remove_relation(ZONESVRENV* pstEnv,ROLEBUDDY *pstRoleBuddy,tdr_ulonglong ullOtherRoleID);
int z_db_role_buddy_info_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);
int z_db_update_role_buddy_info(ZONESVRENV* pstEnv, ROLEDATABUDDY* pstRoleBuddy);
int z_get_role_buddy(ZONESVRENV* pstEnv,DBRoleBuddy *pstDBRoleBuddy,tdr_ulonglong ullRoleID);
int z_db_misc_info_wedding_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBMiscInfo *pstDBMiscInfo);
int z_get_role_skill_info(ZONESVRENV* pstEnv,DBRoleSkillInfo *pstDBRoleSkillInfo,tdr_ulonglong ullRoleID);
int z_db_update_skill_info(ZONESVRENV* pstEnv, ROLESKILLINFO *pstRoleSkillInfo);
int z_db_role_skill_info_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);
int z_db_skill_info_del(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,SkillInfoDelete *pstSkillInfoDelete);
int z_pre_create_award_money(ZONESVRENV* pstEnv, Player *pstPlayer);



#endif /* ZONE_DB_H */
