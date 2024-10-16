
#ifndef CS_NET_H
#define CS_NET_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _WIN32
#include "tdr/tdr.h"
#include "pal/pal.h"
#endif

#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "taes.h"

#ifndef ROBOT

#ifdef _WIN32
#include <Windows.h>
#include <sys\stat.h>
#include <io.h>
#include <fcntl.h>

struct tagTDRData
{
	char* pszBuff;          /**< ����DR���ݵĻ�����ָ��*/
	int iBuff;		/**< ����DR���ݵĻ�������С */
};

#define NTOHS(x)				ntohs((x))
typedef struct tagTDRMetaLib	*LPTDRMETALIB;
typedef struct tagTDRMeta	*LPTDRMETA;
typedef struct tagTDRData	TDRDATA;
#define INOUT 
#define OUT
#define IN
#endif //_WIN32

#else
#include "tdr/tdr.h"
#include "pal/pal.h"
#endif



#define TCLTAPI_ERR_NET			1
#define TCLTAPI_ERR_TDR			2
#define TCLTAPI_ERR_BUF			3


struct tagTCltapiHandle;
typedef struct tagTCltapiHandle 	TCLTAPIHANDLE;
typedef struct tagTCltapiHandle 	*HTCLTAPI;


struct tagNetHandle
{
	int iConfuse;
	LPTDRMETALIB pstCSProtoLib;
	LPTDRMETA pstCSProtoPkgMeta;
};

typedef struct  tagNetHandle		NETHANDLE;
typedef struct  tagNetHandle		*LPNETHANDLE;

struct tagNetCltHandle
{
	NETHANDLE *pstNetHandle;
	HTCLTAPI hApi;
	int iErr;
};

typedef struct  tagNetCltHandle		NETCLTHANDLE;
typedef struct  tagNetCltHandle		*LPNETCLTHANDLE;

struct tagNetCltAuthHandle
{
	int s;
	int iErr;
	int iTdrErrorCode;
	int iSelfVersion;
	int iPeerVersion;
	LPTDRMETA pstMetaBase;
	LPTDRMETA pstMetaHead;
	LPTDRMETA pstRecvMeta;
	LPTDRMETA pstSendMeta;
	char* pszRecvBuff;
	int iRecvBuff;
	char* pszRecvBuff2;
	int iRecvBuff2;
	int iOff;
	int iData;
	char* pszSendBuff;
	int iSendBuff;
	char* pszSendBuff2;
	int iSendBuff2;
	AES stAesGameKey;
	AES stAesK1;
	int iUin;
	char *pstLib;
};

typedef struct  tagNetCltAuthHandle		NETCLTAUTHHANDLE;
typedef struct  tagNetCltAuthHandle		*LPNETCLTAUTHHANDLE;

struct tagNetCltAuthInfo
{
	char AccountLoginName[ACCOUNT_LOGIN_NAME_LEN];
	AES stAesGameKey;
	AUTHJC AuthJC;
	CSPKG stPkg;/*�ڲ��ʱ����*/
	char szMd5[16];/*�ڲ��ʱ����*/
};
typedef struct tagNetCltAuthInfo	NetCltAuthInfo;

//��tdr �ļ��������紦����
int net_han_create(char *pszMetaPath, LPNETHANDLE *ppNetHandle);

//���ڴ�ӳ���ļ��������紦����
int net_han_create_c(unsigned char *pszLibC, LPNETHANDLE *ppNetHandle);

int net_han_destroy(LPNETHANDLE *ppNetHandle);
int net_cs_unpack(NETHANDLE  *pstNetHandler, char *pszBuf, int iBuf, CSPKG *pstPkg, int iVersion);
int net_cs_pack(NETHANDLE  *pstNetHandler, TDRDATA *pstNet, CSPKG *pstPkg, int iVersion);
int res_mem_to_bin(LPTDRMETA pstMeta, char *pData, int iUnit, int iCount, char *pszBinPath);
//int res_mem_to_bin(LPTDRMETA pstMeta, char *pData, int iUnit, int iCount, char *pszBinPath,int iType);
int re_confuse(unsigned char *pszLibC, int iLibSize);

/** @defgroup clt api ������
	* @{
*/
int net_clt_han_create(char *pszMetaPath, char *pszConndIP,  unsigned short unPort, LPNETCLTHANDLE *ppNetCltHandle);
int net_clt_han_create_mem(unsigned char *pmetabuffer, char *pszConndIP,  unsigned short unPort, LPNETCLTHANDLE *ppNetCltHandle);
int net_clt_han_destroy(LPNETCLTHANDLE *ppNetCltHandle);

/**recv clt msg
*@param[in]   pNetCltHandle        Client handle
*@param[in]   pstPkg               Pkg point
*@param[in]   iTimeout             recv timeout, unit ms   
*
*@retval 1   recv a msg
*@retval 0   not recv a msg
*@retval <0  error, error result in pNetCltHandle->iErr
*/
int net_clt_recv(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, int iTimeout);

/**send clt msg
*@param[in]   pNetCltHandle        Client handle
*@param[in]   pstPkg               Pkg point
*@param[in]   iTimeout             send timeout, unit ms   
*
*@retval 0   send succ
*@retval <0  send fail
*/
int net_clt_send(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, int iTimeout);

/**������Ϣ
*@param[in]   pNetCltHandle    Client handle
*@param[in]   pstPkg               Pkg pointer
*@param[out]   pBuff                 ���Buff
*@param[out]   iBuff                 ���Buff ��С 
*Ӧ����net_clt_recv ��net_clt_send ֮�����
*@retval 0   succ
*@retval <0  fail
*/
int net_clt_sprintf(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, char *pBuff, int iBuff);

/*
�������pAddr NULL����piCount ����bin�м�¼�ĸ���
*/
int load_res_confuse_bin(char *pszBinPath, char *pAddr, int iUnitSize, int iUnitMax, int *piCount);

/** @}*/ /*defgroup clt api ������*/

void auth_init_base(PDUBASE* pstBase);

/** @defgroup clt auth api ������
	* @{
*/

#define AUTH_ERR_SYS -1
#define AUTH_ERR_NET -2
#define AUTH_ERR_PASSWD -3
#define AUTH_ERR_TDR -4
#define AUTH_ERR_NOUSR -5

/**��¼��֤��������֤���
*@param[in]   pszMetaPath    proto_cs.meta��·��
*@param[in]   pszAuthConnIP      auth_conn�Ľ���ip
*@param[in]   unAuthPort      auth_conn�Ľ���˿�
*@param[in]   pszZoneConnIP      zone_conn�Ľ���ip
*@param[in]   unZoneConnPort      zone_conn�Ľ���˿�
*@param[in]   pszAccountLoginName    �ʺŵ�¼��
*@param[in]   pszPasswd    �ʺŵ�¼����

*@param[out]   ppNetCltAuthHandle      ��֤���ָ��

*@retval 0   succ
*@retval <0  fail  AUTH_ERR_NET �������
                   AUTH_ERR_SYS ϵͳ����
                   AUTH_ERR_PASSWD �������
                   AUTH_ERR_NOUSR �û�������
                   ����ο�proto_comm.h �Ķ��� tagAUTH_REASON_CODE
                   ǰ���ִ�����Ҫ��ip���ԣ������ֲ������Ե�Ҫ��ʾ
*/
int net_clt_auth_han_create(char *pszMetaPath, char *pszAuthConnIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

int net_clt_auth_han_create_c(char *pstLibC, char *pszAuthConnIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

/**���켤�����¼��֤��������֤���
*@param[in]   pszMetaPath    proto_cs.meta��·��
*@param[in]   pszAuthConnIP      auth_conn�Ľ���ip
*@param[in]   unAuthPort      auth_conn�Ľ���˿�
*@param[in]   pszZoneConnIP      zone_conn�Ľ���ip
*@param[in]   unZoneConnPort      zone_conn�Ľ���˿�
*@param[in]   szActiveCode      ���켤����

*@param[out]   ppNetCltAuthHandle      ��֤���ָ��

*@retval 0   succ
*@retval <0  fail  AUTH_ERR_NET �������
                   AUTH_ERR_SYS ϵͳ����
                   AUTH_ERR_PASSWD �������
                   AUTH_ERR_NOUSR �û�������
                   ����ο�proto_comm.h �Ķ��� tagAUTH_REASON_CODE
                   ǰ���ִ�����Ҫ��ip���ԣ������ֲ������Ե�Ҫ��ʾ
*/
int net_clt_auth_han_active(char *pszMetaPath, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char* szActiveCode,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

int net_clt_auth_han_active_c(char *pstLibC, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char* szActiveCode,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);
								
/**��ȡ��¼��Ϸ���û�����ǩ����Ϣ
*@param[out]  pstNetCltAuthInfo    �����¼��Ϸ���ʻ���֤ǩ����Ϣ

*@retval 0   succ
*/
int get_clt_auth_han_info(NetCltAuthInfo *pstNetCltAuthInfo);

/**�����л���¼��Ϸ��������֤���
*@param[in]   pszMetaPath    proto_cs.meta��·��
*@param[in]   pszZoneConnIP      zone_conn�Ľ���ip
*@param[in]   unZoneConnPort      zone_conn�Ľ���˿�

*@param[out]   ppNetCltAuthHandle      ��֤���ָ��

*Ӧ����get_clt_auth_han_info  ֮�����
*@retval 0   succ
*@retval <0  fail  AUTH_ERR_NET �������
                   AUTH_ERR_SYS ϵͳ����
                   AUTH_ERR_PASSWD �������
                   AUTH_ERR_NOUSR �û�������
                   ǰ���ִ�����Ҫ��ip���ԣ������ֲ������Ե�Ҫ��ʾ
*/
int net_clt_direct_conn(char *pszMetaPath, NetCltAuthInfo *pstNetCltAuthInfo, char *pszZoneConnIP, unsigned short unZoneConnPort, LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

/*ĳЩЭ�飬�����ʺ��˳�����Ҫ�ڲ�uin*/
int net_clt_auth_get_uin(NETCLTAUTHHANDLE *pNetCltAuthHandle);


/**recv clt auth msg
*@param[in]   pNetCltAuthHandle     ��֤���
*@param[in]   pstHead             ��֤Э������ͷ��ָ��
*@param[in]   pstPkg               Pkg ָ��
*@param[in]   iTimeout            recv timeout, unit ms   
*
*@retval 1   �յ���Ϣ
*@retval 0   not recv a msg
*@retval <0  error, error result in pNetCltHandle->iErr
*/
int net_clt_auth_recv(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, int iTimeout);

/**send clt auth msg
*@param[in]   pNetCltAuthHandle       ��֤���
*@param[in]   pstPkg               Pkg point
*@param[in]   iTimeout             send timeout, unit ms   
*
*@retval 0   send succ
*@retval <0  send fail
*/
int net_clt_auth_send(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, int iTimeout);

/**������Ϣ
*@param[in]   pNetCltAuthHandle    Client auth handle
*@param[in]   pstPkg               Pkg pointer
*@param[out]   pBuff                 ���Buff
*@param[out]   iBuff                 ���Buff ��С 
*Ӧ����net_clt_auth_recv ��net_clt_auth_send ֮�����
*@retval 0   succ
*@retval <0  fail
*/
int net_clt_auth_sprintf(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, char *pBuff, int iBuff);

int net_clt_han_auth_destroy(LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

/** @}*/ /*defgroup clt auth api ������*/

int trl_load_mem(INOUT char** ppvBuff, OUT int *piBuff, OUT int *piUnit, IN const char* pszMem, IN int iMem,
			IN const char *pszMetalibHash, IN int iFlag );

int load_res_confuse_bin(char *pszBinPath, char *pAddr, int iUnitSize, int iUnitMax, int *piCount);
int load_res_confuse_bin_mem(unsigned char *pszBin, int bin_size, char *pAddr, int iUnitSize, int iUnitMax, int *piCount);

unsigned int version_to_uint(char *pszVer);
#define MAX_ARM_ON  3  //ͬʱ��װ������������
#define MAX_ARM_ADDATTR_DEF 256
#define MAX_ARMGRADE 64
typedef struct tagPlayerAtk
{
	int iArm; //װ������
	ITEMDEF *pastArm[MAX_WEAR]; //װ��
	ARMLEVELHURTDEF *pstArmLevelHurtDef; //�����ȼ�����
	SKILLDEF *pstSkillDef; //����

	ARMADDATTRDEF *pastArmAddAttrDef[MAX_ARM_ADDATTR_DEF]; //�¼ӵĹؼ��ֱ�Ķ���,��Ҫ���ߴ���
	unsigned char StarLv;                          	//  Ver.74 Ʒ��(��)����, ���װ���Ǽ�12 
	ARMGRADEDEF *pastArmGradeDef[MAX_ARMGRADE];     // װ��������
}PlayerAtk;

typedef struct tagMonAtk
{
	MONSTERDEF *pstMonDef; //����
	SKILLDEF *pstSkillDef; //����
}MonAtk;

typedef union 
{
	PlayerAtk stPlayerAtk;
	MonAtk stMonAtk;
}CSAtkData;

typedef struct
{
	int iObjType; //OBJ_MONSTER, OBJ_PLAYER
	CSAtkData stData;
}CSAtkInfo;

typedef struct
{
	CSAtkInfo stAtker;
	CSAtkInfo stAtkee;
}CaclInput;

typedef struct
{
	char szError[256]; //����ԭ��
	int iHurtMin; //��С�˺�
	int iHurtMax; //����˺�
	int iHitRate; //������
	int iHeavyRate; //������
	int iContinueNum; //����Ƶ��
}CaclOutput;

//����ֵ0 �ɹ���-1 ����
int calc_val(CaclInput *pstInput, CaclOutput *pstOutput);

#ifdef __cplusplus
}
#endif


#endif /* CS_NET_H */
