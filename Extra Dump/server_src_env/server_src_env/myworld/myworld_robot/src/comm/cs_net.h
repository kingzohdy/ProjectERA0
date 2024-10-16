
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
	char* pszBuff;          /**< 保存DR数据的缓冲区指针*/
	int iBuff;		/**< 保存DR数据的缓冲区大小 */
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
	CSPKG stPkg;/*内测得时候用*/
	char szMd5[16];/*内测得时候用*/
};
typedef struct tagNetCltAuthInfo	NetCltAuthInfo;

//从tdr 文件生成网络处理句柄
int net_han_create(char *pszMetaPath, LPNETHANDLE *ppNetHandle);

//从内存映射文件生成网络处理句柄
int net_han_create_c(unsigned char *pszLibC, LPNETHANDLE *ppNetHandle);

int net_han_destroy(LPNETHANDLE *ppNetHandle);
int net_cs_unpack(NETHANDLE  *pstNetHandler, char *pszBuf, int iBuf, CSPKG *pstPkg, int iVersion);
int net_cs_pack(NETHANDLE  *pstNetHandler, TDRDATA *pstNet, CSPKG *pstPkg, int iVersion);
int res_mem_to_bin(LPTDRMETA pstMeta, char *pData, int iUnit, int iCount, char *pszBinPath);
//int res_mem_to_bin(LPTDRMETA pstMeta, char *pData, int iUnit, int iCount, char *pszBinPath,int iType);
int re_confuse(unsigned char *pszLibC, int iLibSize);

/** @defgroup clt api 处理函数
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

/**解析消息
*@param[in]   pNetCltHandle    Client handle
*@param[in]   pstPkg               Pkg pointer
*@param[out]   pBuff                 输出Buff
*@param[out]   iBuff                 输出Buff 大小 
*应该在net_clt_recv 和net_clt_send 之后调用
*@retval 0   succ
*@retval <0  fail
*/
int net_clt_sprintf(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, char *pBuff, int iBuff);

/*
如果输入pAddr NULL，则piCount 返回bin中记录的个数
*/
int load_res_confuse_bin(char *pszBinPath, char *pAddr, int iUnitSize, int iUnitMax, int *piCount);

/** @}*/ /*defgroup clt api 处理函数*/

void auth_init_base(PDUBASE* pstBase);

/** @defgroup clt auth api 处理函数
	* @{
*/

#define AUTH_ERR_SYS -1
#define AUTH_ERR_NET -2
#define AUTH_ERR_PASSWD -3
#define AUTH_ERR_TDR -4
#define AUTH_ERR_NOUSR -5

/**登录认证并创建认证句柄
*@param[in]   pszMetaPath    proto_cs.meta的路径
*@param[in]   pszAuthConnIP      auth_conn的接入ip
*@param[in]   unAuthPort      auth_conn的接入端口
*@param[in]   pszZoneConnIP      zone_conn的接入ip
*@param[in]   unZoneConnPort      zone_conn的接入端口
*@param[in]   pszAccountLoginName    帐号登录名
*@param[in]   pszPasswd    帐号登录密码

*@param[out]   ppNetCltAuthHandle      认证句柄指针

*@retval 0   succ
*@retval <0  fail  AUTH_ERR_NET 网络错误
                   AUTH_ERR_SYS 系统错误
                   AUTH_ERR_PASSWD 密码错误
                   AUTH_ERR_NOUSR 用户不存在
                   具体参考proto_comm.h 的定义 tagAUTH_REASON_CODE
                   前两种错误需要换ip重试，后两种不需重试但要提示
*/
int net_clt_auth_han_create(char *pszMetaPath, char *pszAuthConnIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

int net_clt_auth_han_create_c(char *pstLibC, char *pszAuthConnIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

/**反响激活码登录认证并创建认证句柄
*@param[in]   pszMetaPath    proto_cs.meta的路径
*@param[in]   pszAuthConnIP      auth_conn的接入ip
*@param[in]   unAuthPort      auth_conn的接入端口
*@param[in]   pszZoneConnIP      zone_conn的接入ip
*@param[in]   unZoneConnPort      zone_conn的接入端口
*@param[in]   szActiveCode      反响激活码

*@param[out]   ppNetCltAuthHandle      认证句柄指针

*@retval 0   succ
*@retval <0  fail  AUTH_ERR_NET 网络错误
                   AUTH_ERR_SYS 系统错误
                   AUTH_ERR_PASSWD 密码错误
                   AUTH_ERR_NOUSR 用户不存在
                   具体参考proto_comm.h 的定义 tagAUTH_REASON_CODE
                   前两种错误需要换ip重试，后两种不需重试但要提示
*/
int net_clt_auth_han_active(char *pszMetaPath, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char* szActiveCode,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

int net_clt_auth_han_active_c(char *pstLibC, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char* szActiveCode,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);
								
/**获取登录游戏的用户认真签名信息
*@param[out]  pstNetCltAuthInfo    最近登录游戏的帐户认证签名信息

*@retval 0   succ
*/
int get_clt_auth_han_info(NetCltAuthInfo *pstNetCltAuthInfo);

/**快速切换登录游戏并创建认证句柄
*@param[in]   pszMetaPath    proto_cs.meta的路径
*@param[in]   pszZoneConnIP      zone_conn的接入ip
*@param[in]   unZoneConnPort      zone_conn的接入端口

*@param[out]   ppNetCltAuthHandle      认证句柄指针

*应该在get_clt_auth_han_info  之后调用
*@retval 0   succ
*@retval <0  fail  AUTH_ERR_NET 网络错误
                   AUTH_ERR_SYS 系统错误
                   AUTH_ERR_PASSWD 密码错误
                   AUTH_ERR_NOUSR 用户不存在
                   前两种错误需要换ip重试，后两种不需重试但要提示
*/
int net_clt_direct_conn(char *pszMetaPath, NetCltAuthInfo *pstNetCltAuthInfo, char *pszZoneConnIP, unsigned short unZoneConnPort, LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

/*某些协议，例如帐号退出等需要内部uin*/
int net_clt_auth_get_uin(NETCLTAUTHHANDLE *pNetCltAuthHandle);


/**recv clt auth msg
*@param[in]   pNetCltAuthHandle     认证句柄
*@param[in]   pstHead             认证协议数据头部指针
*@param[in]   pstPkg               Pkg 指针
*@param[in]   iTimeout            recv timeout, unit ms   
*
*@retval 1   收到消息
*@retval 0   not recv a msg
*@retval <0  error, error result in pNetCltHandle->iErr
*/
int net_clt_auth_recv(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, int iTimeout);

/**send clt auth msg
*@param[in]   pNetCltAuthHandle       认证句柄
*@param[in]   pstPkg               Pkg point
*@param[in]   iTimeout             send timeout, unit ms   
*
*@retval 0   send succ
*@retval <0  send fail
*/
int net_clt_auth_send(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, int iTimeout);

/**解析消息
*@param[in]   pNetCltAuthHandle    Client auth handle
*@param[in]   pstPkg               Pkg pointer
*@param[out]   pBuff                 输出Buff
*@param[out]   iBuff                 输出Buff 大小 
*应该在net_clt_auth_recv 和net_clt_auth_send 之后调用
*@retval 0   succ
*@retval <0  fail
*/
int net_clt_auth_sprintf(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, char *pBuff, int iBuff);

int net_clt_han_auth_destroy(LPNETCLTAUTHHANDLE *ppNetCltAuthHandle);

/** @}*/ /*defgroup clt auth api 处理函数*/

int trl_load_mem(INOUT char** ppvBuff, OUT int *piBuff, OUT int *piUnit, IN const char* pszMem, IN int iMem,
			IN const char *pszMetalibHash, IN int iFlag );

int load_res_confuse_bin(char *pszBinPath, char *pAddr, int iUnitSize, int iUnitMax, int *piCount);
int load_res_confuse_bin_mem(unsigned char *pszBin, int bin_size, char *pAddr, int iUnitSize, int iUnitMax, int *piCount);

unsigned int version_to_uint(char *pszVer);
#define MAX_ARM_ON  3  //同时能装备的武器数量
#define MAX_ARM_ADDATTR_DEF 256
#define MAX_ARMGRADE 64
typedef struct tagPlayerAtk
{
	int iArm; //装备数量
	ITEMDEF *pastArm[MAX_WEAR]; //装备
	ARMLEVELHURTDEF *pstArmLevelHurtDef; //武器等级修正
	SKILLDEF *pstSkillDef; //技能

	ARMADDATTRDEF *pastArmAddAttrDef[MAX_ARM_ADDATTR_DEF]; //新加的关键字表的定义,需要工具传入
	unsigned char StarLv;                          	//  Ver.74 品质(星)级别, 最高装备星级12 
	ARMGRADEDEF *pastArmGradeDef[MAX_ARMGRADE];     // 装备提升表
}PlayerAtk;

typedef struct tagMonAtk
{
	MONSTERDEF *pstMonDef; //怪物
	SKILLDEF *pstSkillDef; //技能
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
	char szError[256]; //错误原因
	int iHurtMin; //最小伤害
	int iHurtMax; //最大伤害
	int iHitRate; //命中率
	int iHeavyRate; //爆击率
	int iContinueNum; //攻击频数
}CaclOutput;

//返回值0 成功，-1 错误
int calc_val(CaclInput *pstInput, CaclOutput *pstOutput);

#ifdef __cplusplus
}
#endif


#endif /* CS_NET_H */
