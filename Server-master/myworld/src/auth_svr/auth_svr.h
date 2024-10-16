#ifndef _AUTH_SVR_H
#define _AUTH_SVR_H

#include "../zone/zone_svr/role_db_meta.h"
#include "tconnapi.h"
#include "tormapi/torm_protocol.h"
#include "proto_comm.h"
#include "proto_cs.h"

#define ACTIVE_KEY_STATUS_FLAG_ASK 1 //ѯ���Ƿ�ʹ���˼�����
#define ACTIVE_KEY_STATUS_FLAG_YES 2 //�ʺ�ʹ���˼�����


struct tagauth_svrEnv;// AUTH_SVRENV;

#pragma pack(1)

// connd ��TCP����������
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


// TORM Callback �������
typedef struct tagSelectInfo
{
	ConnCtx stConnCtx;
	AUTHINFO stAuthInfo;
	char ActiveFlag;                      	/*  Ver.165 0��������Ч,1��ʾ��Ч */
	char ActiveCode[INFORMER_ACTIVECODE_LEN]; 	/*  Ver.165 ������ */
	int AuthFlag;                        	/*  Ver.1220 ��־λ AUTH_FLAG_QUICK */
	int Uin;								//uin
	int AuthMode;							//auth_svr ģʽ
	int AskActiveKeyStatusFlag;				//ѯ�ʼ�����״̬Flag,0��,1��
	ACCOUNTAUTHINFO stAccount;	
	int CltType;//�ͻ�������
	char SelectType;   				// 1--account���н�ɫѡ��
	PASSWDCARDAUTHINFO stPasswdCardAuthInfo; //�ܱ�����֤��Ϣ
	char UinType;   				// 0 account���ݿ���û�д��ʺ� 1�д��ʺ�
	int iIP;
	int iWebUin;
}SelectInfo;

typedef struct tagInsertInfo
{
	ConnCtx stConnCtx;
	ACCOUNTAUTHINFO stAccount;
	int AuthFlag;                        	/*  Ver.1220 ��־λ AUTH_FLAG_QUICK */
	int CltType;
	AUTHINFO stAuthInfo;
	int iWebUin;
}InsertInfo;

typedef struct tagUpdateInfo
{
	ConnCtx stConnCtx;
	char szLoginName[ACCOUNT_LOGIN_NAME_LEN];
	AUTHINFO stAuthInfo;
	int AuthFlag;                        	/*  Ver.1220 ��־λ AUTH_FLAG_QUICK */
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

