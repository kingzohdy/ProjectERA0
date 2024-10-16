#ifndef _AUTH_SVR_H
#define _AUTH_SVR_H

#include "../zone/zone_svr/role_db_meta.h"
#include "tconnapi.h"
#include "tormapi/torm_protocol.h"
#include "proto_comm.h"
#include "proto_cs.h"

#define ACTIVE_KEY_STATUS_FLAG_ASK 1 //询问是否使用了激活码
#define ACTIVE_KEY_STATUS_FLAG_YES 2 //帐号使用了激活码


struct tagauth_svrEnv;// AUTH_SVRENV;

#pragma pack(1)

// connd 的TCP连接上下文
typedef struct tagConnCtx
{
	char chVer; 
	char chCmd;
  char chExtraType;                    
  char chTimeStampType;                
  int iID;                            
  int iConnIdx;                       
  TFRAMEHEADDATA stExtraInfo;                    
  TTIMESTAMPDATA stTimeStamp;  
	int iCltProtoVer;
	int IP;
}ConnCtx;


// TORM Callback 相关数据
typedef struct tagSelectInfo
{
	ConnCtx stConnCtx;
	AUTHINFO stAuthInfo;
	char ActiveFlag;                      	/*  Ver.165 0激活码无效,1表示有效 */
	char ActiveCode[INFORMER_ACTIVECODE_LEN]; 	/*  Ver.165 激活码 */
	int AuthFlag;                        	/*  Ver.1220 标志位 AUTH_FLAG_QUICK */
	int Uin;								//uin
	int AuthMode;							//auth_svr 模式
	int AskActiveKeyStatusFlag;				//询问激活码状态Flag,0否,1是
	ACCOUNTAUTHINFO stAccount;	
	int CltType;//客户端类型
	char SelectType;   				// 1--account所有角色选择
	PASSWDCARDAUTHINFO stPasswdCardAuthInfo; //密保卡验证信息
	char UinType;   				// 0 account数据库中没有此帐号 1有此帐号
	int iIP;
	int iWebUin;
}SelectInfo;

typedef struct tagInsertInfo
{
	ConnCtx stConnCtx;
	ACCOUNTAUTHINFO stAccount;
	int AuthFlag;                        	/*  Ver.1220 标志位 AUTH_FLAG_QUICK */
	int CltType;
	AUTHINFO stAuthInfo;
	int iWebUin;
}InsertInfo;

typedef struct tagUpdateInfo
{
	ConnCtx stConnCtx;
	char szLoginName[ACCOUNT_LOGIN_NAME_LEN];
	AUTHINFO stAuthInfo;
	int AuthFlag;                        	/*  Ver.1220 标志位 AUTH_FLAG_QUICK */
}UpdateInfo;

typedef union tagTormCallbackContext
{
	InsertInfo stInsertInfo;
	SelectInfo stSelectInfo;
	UpdateInfo stUpdateInfo;
}TormCallbackContext;


typedef struct tagTormCallback
{
	int iMatchedCmd;
	TormCallbackContext stContext;
}TormCallback;

#pragma pack()


#endif // _AUTH_SVR_H

