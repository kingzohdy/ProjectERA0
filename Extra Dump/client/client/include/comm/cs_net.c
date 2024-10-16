/*
**  @file $RCSfile: cs_net.c,v $
**  general description of this module
**  $Id: cs_net.c,v 1.107 2013/08/28 03:16:57 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/08/28 03:16:57 $
**  @version $Revision: 1.107 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "cs_net.h"
#include "ResConv.h"
#include "tresloader.h"
#include "grav_rsa.h"
#include "md5.h"

#ifdef _WIN32
#include "tnet.h"
#endif

#include "resdb_meta.h"
#define UNUSED( var ) ((void)var)
 
#ifdef _WIN32
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define snprintf				_snprintf
#define STRNCPY(pszDst, pszSrc, iLen)					      \
	do								      \
{								      \
	strncpy(pszDst, pszSrc, (iLen)-1);			      \
	pszDst[(iLen)-1] = 0;					      \
}								      \
	while(0)

#define HTONS(x)				htons((x))
#define HTONL(x)				htonl((x))
#define NTOHS(x)				ntohs((x))
#define NTOHL(x)				ntohl((x))

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ws2_32.lib")


#ifndef ROBOT
	#ifdef _DEBUG
	#pragma comment(lib, "libtdr_dbms_xml_d.lib")
	#pragma comment(lib, "onet_d.lib")
	#else
	#pragma comment(lib, "libtdr_dbms_xml.lib")
	#pragma comment(lib, "onet.lib")
	#endif
#endif

#else
#include <linux/tcp.h>
#endif 


extern RSA_PUBLIC_KEY * IDRSAPublicKey;

static NetCltAuthInfo g_stNetCltAuthInfo;

static char gs_szCharList[]	=	"ABCDEFGHIJKMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz23456789";

#define MAX_ERR_STR 2048
unsigned char sKey[]={\
    0xd6, 0x02, 0x08, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xd0, 0xca, 0x01, 0x00};

struct tagTCltapiHandle
{
	int s;
	int iErr;
	int iSelfVersion;
	int iPeerVersion;
	LPTDRMETA pstRecvMeta;
	LPTDRMETA pstSendMeta;
	char* pszRecvBuff;
	int iRecvBuff;
	int iOff;
	int iData;
	char* pszSendBuff;
	int iSendBuff;
	char szErrString[MAX_ERR_STR];
};

typedef struct tagCSAtkChg
{
	ITEMDEF *pstItemDef;
	int iArmType;
	
	//冰
	unsigned short unIceBaseArmAtk; 
	short nIceAtkChgMul;
	int iIceAtkChgAdd;
	int iIceDecDef;

	//火
	unsigned short unFireBaseArmAtk; 
	short nFireAtkChgMul;
	int  iFireAtkChgAdd;
	int iFireDecDef;

	//雷
	unsigned short unThunderBaseArmAtk; 
	short nThunderAtkChgMul;
	int    iThunderAtkChgAdd;
	int iThunderDecDef;

	//光
	unsigned short unLightBaseArmAtk; 
	short nLightAtkChgMul;
	int iLightAtkChgAdd;
	int iLightDecDef;

	//暗
	unsigned short unNightBaseArmAtk; 
	short nNightAtkChgMul;
	int iNightAtkChgAdd;
	int iNightDecDef;

	//物理
	unsigned short unPhsicBaseArmAtk; 
	short nPhsicAtkChgMul;
	int iPhsicAtkChgAdd;
	int iPhsicDecDef;

	 //弹药
	unsigned short unBaseAmmAtk;

	//武器等级修正
	short nArmLevChgMul;

	//破坏乘修正
	short nDamageMul;
	unsigned short unDamageAdd;
	
	//攻击伤害修正
	ATKERHURTCHG stAtkerHurtChg;
}CSAtkChg;

struct tagCSHealChg
{
	int iHPMaxBase;
	short nHPMaxMul;
	short nHPMaxAdd;

	int  iMPMaxBase;
	short nMPMaxMul;
	short nMPMaxAdd;

	short nLPMaxBase;
	short nLPMaxMul;
	short nLPMaxAdd;

	short nVimMaxBase;
	short nVimMaxMul;
	short nVimMaxAdd;

	short nSpdMoveRestore; 	/*移动力回复速度*/

	short nSpdMoveMaxBase; 		/*最大移动力*/
	short nSPdMoveMaxMul;
	short nSPdMoveMaxAdd;

	short nSpdFlyBase; 		/*飞行速度*/
	short nSPdFlyMul;
	short nSPdFlyAdd;

	short nSpdRideBase; 		/*坐骑速度*/
	short nSPdRideMul;
	short nSPdRideAdd;

	short nBaseHealHPMul;
	short nHealHPMul;
	unsigned short unHealHPAdd;

	short nBaseHealMPMul;
	short nHealMPMul;
	unsigned short unHealMPAdd;

	unsigned short unArmHPAdd;
	unsigned short unArmMPAdd;

	unsigned short unNoFightRestoreHPAdd;
	unsigned short unNoFightRestoreMPAdd;

	short nSitHPMul;		
	short nSitMPMul;
	short nSitExpMul;
	short nSitVigorMul;

	short nXpAdd;
	short nXpMul;

	short nMaxXpAdd;

	short nArmExpMul;

	unsigned char ucBadStatusRate;
	unsigned char ucBadStatusDefRate;
	unsigned short unBadStatusDecTime;

	unsigned char ucTaskItemRate;
	unsigned char ucStuffItemRate;
	unsigned char ucEggRate;
	unsigned char ucBullRate;

	unsigned short unPetExpMul;
	unsigned short unSkillPonitMul;
};
typedef struct tagCSHealChg	CSHEALCHG;
struct tagCSMidAttrList
{
	unsigned int uiHPMax;
	unsigned int uiMPMax;
	unsigned int uiNlMax;
	unsigned int uiVimMax;
	unsigned int uiSpeMoveMax;   	// 最大移动力	

	unsigned int uiFight; 			//战斗力
	int iPetFight; 				//宠物提供的战斗力

	unsigned char bXianListAdd;
	unsigned char bShiListAdd;

	unsigned int uiSitRestoreHP;	// 打坐回复的HP
	unsigned int uiSitRestoreMP;	// 打坐回复的MP
	unsigned int uiSitAddExp;		// 打坐增加的EXP
	short iSitConsumeVigor;		// 打坐消耗的Vigor
	unsigned int uiVigorMax;	// 最大精力
	unsigned int uiDayExpMax;		// 每天获取经验上限

	unsigned int uiLevelUpAddMp;	// 等级对应增加mp
	unsigned int uiLevelUpAddHp;	// 等级对应增加hp

	unsigned int uiXpMax; 			// 	最大xp值
	
	short nRes;
};
typedef struct tagCSMidAttrList		CSMIDATTRLIST;

typedef struct tagCSDefChg
{
	//冰
	unsigned short unIceDefBaseAdd;
	short nIceDefChgMul;
	int iIceDefChgAdd;

	//火
	unsigned short unFireDefBaseAdd;
	short nFireDefChgMul;
	int iFireDefChgAdd;

	//雷
	unsigned short unThunderDefBaseAdd;
	short nThunderDefChgMul;
	int iThunderDefChgAdd;

	//光
	unsigned short unLightDefBaseAdd;
	short nLightDefChgMul;
	int iLightDefChgAdd;

	//暗
	unsigned short unNightDefBaseAdd;
	short nNightDefChgMul;
	int iNightDefChgAdd;

	//物理
	unsigned short unPhsicDefBaseAdd;
	short nPhsicDefChgMul;
	int iPhsicDefChgAdd;

	int iDefIceTmp;
	int iDefFireTmp;
	int iDefThunderTmp;
	int iDefLightTmp;
	int iDefNightTmp;
	int iDefPhsicTmp;
}CSDefChg;


typedef struct tagCSHitChg
{
	unsigned short unHitBaseAdd;
	short nHitChgMul;
	int iHitChgAdd;
	
	int iHitTmp;
}CSHitChg;


typedef struct tagCSFleeChg
{
	unsigned short unFleeBaseAdd;
	short nFleeChgMul;
	int iFleeChgAdd;
	
	short nAtkeeBreakPrepareMul; //抵抗被攻击打断吟唱或集气修正
	short nRes;

	int iFleeTmp;
}CSFleeChg;

typedef struct tagCSHitRateChg
{
	short nAtkerHitRateMul;
	short nAtkerHitRateAdd;

	short nAtkeeHitRateMul;
	short nAtkeeHitRateAdd;

	short nHeavyHitAdd; //爆击概率
	short nHeavyHitVal; //爆击数值
}CSHitRateChg;

typedef struct tagCSMonChg
{
	short nFleeChgMul;
	unsigned short unFleeChgAdd;
	short nAtkeeHurtMul;
	unsigned short unAtkeeHurtAdd;

	short nAtkeeMagicDefMul;
	unsigned short nAtkeeMagicDefAdd;
	short nAtkeeIceDefMul;
	unsigned short nAtkeeIceDefAdd;
	short nAtkeePhsicDefMul;
	unsigned short nAtkeePhsicDefAdd;	
}CSMonChg;

typedef struct tagCSDynAttrList
{
    CSMIDATTRLIST stMidAttrList;
    CSHEALCHG stHealChg;
	CSAtkChg astAtkChg[3];
	CSDefChg stDefChg;
	CSHitChg stHitChg;
	CSFleeChg stFleeChg;
	CSHitRateChg stHitRateChg;
	ATKEEHURTCHG stAtkeeHurtChg; //抵抗伤害修正
	CSMonChg stMonChg;
	//ARMSKILL astArmSkill[MAX_ARM_SKILL]; //角色装备 可增加的技能数量	
	
}CSDynAttrList;

int re_confuse(unsigned char *pszLibC, int iLibSize)
{
	int i;

	for (i=0; i<iLibSize; i++)
	{
		*(pszLibC+i) ^= sKey[i%16];
	}

	return 0;
}

int tnet_connect2(const char* a_pszUri, int a_iTimeout, int iSendBuff, int iRecvBuff)
{	
	TNETOPT stOpt;
	TNETADDR stAddr;
	const TNETPROTODEF* pstProto;
	int s;

	if( -1==tnet_get_opt(&stOpt, a_pszUri) )
		return -1;


	pstProto	=	tnet_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return -1;

	s	=	tnet_open_by_proto(&stOpt, pstProto);

	if( -1==s )
		return -1;

	tnet_set_nonblock(s, 1);
	tnet_set_sendbuff(s, iSendBuff);
	tnet_set_recvbuff(s, iRecvBuff);

	switch(pstProto->iID)
	{
	case TNET_ID_TCP:
	case TNET_ID_UDP:
		if( tnet_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;
		
		if( tsocket_connect(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn), a_iTimeout)<0 )
			break;
		else
			return s;

		break;

#ifndef WIN32
	case TNET_ID_UNIX:
		if( tnet_str2unix(stOpt.szAddr, &stAddr.stUn) < 0 )
			break;
		
		if( tsocket_connect(s, (struct sockaddr*)&stAddr.stUn, sizeof(stAddr.stUn), a_iTimeout)<0 )
			break;
		else
			return s;

		break;
#endif /* WIN32 */

	default:
		break;
	}

	tnet_close(s);

	return -1;
}


int tnet_set_nonagle(int socket)
{
	int iFlag = 1;
	return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&iFlag, sizeof(iFlag) );
}

int res_mem_to_bin(LPTDRMETA pstMeta, char *pData, int iUnit, int iCount, char *pszBinPath)
{
	char szBinFile[1024];
	char *pch;
	int iLen;
	FILE *fp;
	TRESHEAD stResHead;
	int iWrite;
	int i;
	
	STRNCPY(szBinFile, pszBinPath, sizeof(szBinFile));
	pch = strrchr(szBinFile, '.');
	if (NULL != pch)
	{
		*pch = '\0';
	}
	iLen = (int)strlen(szBinFile);
	if ((iLen + strlen(".bin")) >= sizeof(szBinFile))
	{
		printf("生成保存资源信息的bin文件名失败\n");
		return -1;
	}
	pch = &szBinFile[iLen];
	STRNCPY(pch, ".bin", (sizeof(szBinFile) - iLen));

	fp = fopen(szBinFile, "wb");
	if (NULL == fp)
	{
		printf("以‘写’的方式打开文件(%s)失败\n", szBinFile);
		return -1;
	}

	memset(&stResHead, 0, sizeof(stResHead));
	stResHead.iCount = iCount;
	stResHead.iUnit = tdr_get_meta_size(pstMeta);
	stResHead.iMagic = TRES_FILE_MAGIC;
	stResHead.iVersion = TDR_METALIB_RESCONV_VERSION;
	stResHead.iResVersion = tdr_get_meta_current_version(pstMeta);
	iWrite = fwrite(&stResHead, sizeof(stResHead), 1, fp);
	if (iWrite != 1)
	{
		printf("向资源文件(%s)中写信息失败\n",szBinFile);
		return -1;
	}

	for (i=0; i<iCount; i++)
	{
		
		iWrite = fwrite(pData+iUnit*i, stResHead.iUnit, 1, fp);
		if (iWrite != 1)
		{
			printf("向资源文件(%s)中写信息失败\n",szBinFile);
			return  -1;
		}
	}

	fclose(fp);
	
	return 0;
}

int trl_load_mem(INOUT char** ppvBuff, OUT int *piBuff, OUT int *piUnit, IN const char* pszMem, IN int iMem,
			IN const char *pszMetalibHash, IN int iFlag )
{
	TRESHEAD stHead;
	char* pszData;
	int iData;
	int iPoint = 0;

	if ((NULL == piBuff) || (NULL == piUnit)||(NULL == ppvBuff))
	{
		return RL_ERROR_INVALID_PARAM;
	}
	
	if (iMem < (int)sizeof(stHead))
	{
		return RL_ERROR_INVALID_PARAM;
	}

	memcpy(&stHead, pszMem, sizeof(stHead));
	iPoint += sizeof(stHead);
	
	if (0 >= stHead.iCount)
	{
		return 0;
	}
	iData	=	stHead.iCount * stHead.iUnit + sizeof(stHead);
	if (iMem < iData)
	{
		return RL_ERROR_INVALID_PARAM;
	}
	
	if( TRES_FILE_MAGIC!=stHead.iMagic )
		return -1;

	if( TDR_METALIB_RESCONV_VERSION!=stHead.iVersion )
		return -1;

	if( stHead.iUnit<=0 )
		return -1;

	if( stHead.iCount<0 )
		return -1;

	if( (int)(sizeof(TRESHEAD) + (stHead.iUnit) * (stHead.iCount)) != iData )
		return -1;
	

	/*检验元数据描述库hash值*/
	if ((NULL != pszMetalibHash) && !(iFlag & RL_FLAG_INGORE_DIFF_HASH))
	{
		if (strcmp(pszMetalibHash, stHead.szMetalibHash))
		{
			return RL_ERROR_DIFF_HASH;
		}
	}

	if( piBuff )
		*piBuff	=	stHead.iCount * stHead.iUnit;

	if( piUnit )
		*piUnit	=	stHead.iUnit;

	pszData	=	(char*) calloc(1, stHead.iCount*stHead.iUnit);
	if( NULL==pszData )
	{
		return -1;
	}

	memcpy(pszData, pszMem+iPoint, stHead.iCount*stHead.iUnit);
	
	*ppvBuff	=	pszData;
	
	return stHead.iCount;
}

int load_res_confuse_bin_mem(unsigned char *pszBin, int bin_size, char *pAddr, int iUnitSize, int iUnitMax, int *piCount)
{
	int iCount;
	char *pszDataBuff = NULL;
	int iBuffLen;
	int iUnit;
	int i;

	if (0 > re_confuse(pszBin, bin_size))
	{
		return -1;
	}

	iCount = trl_load_mem(&pszDataBuff, &iBuffLen, &iUnit, (char *)pszBin,(int)bin_size,NULL, RL_FLAG_INGORE_DIFF_HASH);

	if (0 > iCount ||iUnit > iUnitSize || iCount > iUnitMax)
	{
		printf("trl_load_mem fail\n");
		return -1;
	}

	for (i=0; i<iCount; i++)
	{
		memcpy(pAddr+i*iUnitSize, pszDataBuff+iUnit*i, iUnit);
		memset(pAddr+i*iUnitSize + iUnit, 0, iUnitSize - iUnit);
	}

	*piCount = iCount;
	free(pszDataBuff);

	return 0;
}

int load_res_confuse_bin(char *pszBinPath, char *pAddr, int iUnitSize, int iUnitMax, int *piCount)
{
	struct stat stStat;
	unsigned char *pszBin;
	FILE *fp;
	int iCount;
	char *pszDataBuff = NULL;
	int iBuffLen;
	int iUnit;
	int i;

	if (0 != stat(pszBinPath, &stStat))
	{
		return -1;
	}
	pszBin = malloc(stStat.st_size);
	if(pszBin == NULL)
	{
		return -1;
	}
	
	fp = fopen(pszBinPath, "rb");
	if (NULL == fp)
	{
		return 0;
	}
	fread(pszBin, stStat.st_size, 1, fp);
	fclose(fp);
	
	if (0 > re_confuse(pszBin, stStat.st_size))
	{
		return -1;
	}

	iCount = trl_load_mem(&pszDataBuff, &iBuffLen, &iUnit, (char *)pszBin,(int)stStat.st_size,NULL, RL_FLAG_INGORE_DIFF_HASH);

	if (0 > iCount ||iUnit > iUnitSize || iCount > iUnitMax)
	{
		printf("trl_load_mem fail\n");
		return -1;
	}
	
	for (i=0; i<iCount; i++)
	{
		memcpy(pAddr+i*iUnitSize, pszDataBuff+iUnit*i, iUnit);
		memset(pAddr+i*iUnitSize + iUnit, 0, iUnitSize - iUnit);
	}

	*piCount = iCount;
	free(pszDataBuff);
	free(pszBin);
	
	return 0;
}

LPTDRMETALIB meta_file_lib(char *pszMetaPath)
{
	unsigned char *pszLibC;
	struct stat stStat;
	FILE *fp;

	if (0 != stat(pszMetaPath, &stStat))
	{
		return NULL;
	}

	fp = fopen(pszMetaPath, "rb");
	if (NULL == fp)
	{
		return NULL;
	}

	pszLibC = malloc(stStat.st_size);
	if (NULL == pszLibC)
	{
		fclose(fp);
		return NULL;
	}

	fread(pszLibC, stStat.st_size, 1, fp);
	fclose(fp);

	if (0 > re_confuse(pszLibC, stStat.st_size))
	{
		free(pszLibC);
		return NULL;
	}

	return (LPTDRMETALIB)pszLibC;
}

int meta_file_lib_free(LPTDRMETALIB pstLib)
{
	if (pstLib)
		free(pstLib);
	
	return 0;
}

int net_han_create(char *pszMetaPath, LPNETHANDLE *ppNetHandle)
{
	int iRet = 0;
	NETHANDLE *pstNetHandle = NULL;

	pstNetHandle = (NETHANDLE *)malloc(sizeof(NETHANDLE));
	if (!pstNetHandle)
	{
		return -1;
	}
	memset(pstNetHandle, 0, sizeof(*pstNetHandle));

	pstNetHandle->iConfuse = 1;
	pstNetHandle->pstCSProtoLib = meta_file_lib(pszMetaPath);
	if (NULL == pstNetHandle->pstCSProtoLib)
	{
		free(pstNetHandle);
		return -1;
	}

	pstNetHandle->pstCSProtoPkgMeta = tdr_get_meta_by_name(pstNetHandle->pstCSProtoLib, "CSPkg");
	if (!pstNetHandle->pstCSProtoPkgMeta)
	{
		iRet = -1;
	}

	if (0 > iRet)
	{
		meta_file_lib_free(pstNetHandle->pstCSProtoLib);
		free(pstNetHandle);
	}
	else
	{
		*ppNetHandle = pstNetHandle;
	}
	
	return iRet;
}

int net_han_create_c(unsigned char *pszLibC, LPNETHANDLE *ppNetHandle)
{
	int iRet = 0;
	NETHANDLE *pstNetHandle = NULL;

	pstNetHandle = (NETHANDLE *)malloc(sizeof(NETHANDLE));
	if (!pstNetHandle)
	{
		return -1;
	}
	memset(pstNetHandle, 0, sizeof(*pstNetHandle));

	pstNetHandle->iConfuse = 0;
	pstNetHandle->pstCSProtoLib = (LPTDRMETALIB)pszLibC;
	
	if (0 == iRet)
	{
		pstNetHandle->pstCSProtoPkgMeta = tdr_get_meta_by_name(pstNetHandle->pstCSProtoLib, "CSPkg");
		if (!pstNetHandle->pstCSProtoPkgMeta)
		{
			iRet = -1;
		}
	}

	if (0 > iRet)
	{
		free(pstNetHandle);
	}
	else
	{
		*ppNetHandle = pstNetHandle;
	}
	
	return iRet;
}

int net_han_destroy(LPNETHANDLE *ppNetHandle)
{
	LPNETHANDLE pstNetHandle;

	pstNetHandle = *ppNetHandle;

	if (pstNetHandle->iConfuse)
	{
		meta_file_lib_free(pstNetHandle->pstCSProtoLib);
	}
	
	free(*ppNetHandle);
	*ppNetHandle = NULL;
	
	return 0;
}

int net_cs_unpack(NETHANDLE  *pstNetHandler, char *pszBuf, int iBuf, CSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;
	TDRDATA stNet;
	
		
	stNet.pszBuff = pszBuf;
	stNet.iBuff = iBuf;
	stHost.pszBuff = (char *)pstPkg;
	stHost.iBuff = sizeof(CSPKG);

	return tdr_ntoh(pstNetHandler->pstCSProtoPkgMeta, &stHost, &stNet, iVersion);
}

int net_cs_pack(NETHANDLE  *pstNetHandler, TDRDATA *pstNet, CSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;

	pstPkg->Head.Ver =TDR_METALIB_PROTO_CS_VERSION;

	stHost.iBuff = sizeof(CSPKG);
	stHost.pszBuff = (char *)pstPkg;

	return tdr_hton(pstNetHandler->pstCSProtoPkgMeta, pstNet, &stHost, iVersion);
}

/********************************************************************************/
int tcltapi_make_connect(const char* a_pszUri, int a_iTimeout)
{
	return tnet_connect(a_pszUri, a_iTimeout);
}

int tcltapi_new(HTCLTAPI* a_phClt)
{
	HTCLTAPI hClt;

	if( !a_phClt )
		return -1;

	hClt	=	(HTCLTAPI) calloc(1, sizeof(TCLTAPIHANDLE));

	if( !hClt )
		return -1;
	
	hClt->s		=	-1;
	*a_phClt	=	hClt;

	return 0;
}

int tcltapi_free(HTCLTAPI* a_phClt)
{
	if( !a_phClt )
		return -1;

	if( *a_phClt )
	{
		if( -1!=(*a_phClt)->s )
		{
			tnet_close((*a_phClt)->s);
			(*a_phClt)->s	=	-1;
		}

		if( (*a_phClt)->pszSendBuff )
		{
		         free( (*a_phClt)->pszSendBuff  );
		       (*a_phClt)->pszSendBuff  = NULL;     
		}

		if( (*a_phClt)->pszRecvBuff)
		{
		         free( (*a_phClt)->pszRecvBuff  );
		         (*a_phClt)->pszRecvBuff  = NULL;     
		}


		free(*a_phClt);

		*a_phClt	=	NULL;
	}

	return 0;
}

int tcltapi_detach(HTCLTAPI a_hClt)
{
	int s;

	s		=	a_hClt->s;

	a_hClt->s	=	-1;

	return s;
}

void tcltapi_attach(HTCLTAPI a_hClt, int a_s)
{
	if( -1!=a_hClt->s )
	{
		tnet_close(a_hClt->s);

		a_hClt->s	=	-1;
	}

	a_hClt->s	=	a_s;
}

int tcltapi_open(const char* a_pszUri, int a_iTimeout, HTCLTAPI* a_phClt)
{
	int s=-1;

	s	=	tnet_connect(a_pszUri, a_iTimeout);

	if( -1==s )
		return -1;

	if( -1==tcltapi_new(a_phClt) )
	{
		tnet_close(s);

		return -1;
	}

	tcltapi_attach(*a_phClt, s);

	//tnet_set_nonagle(s);
	//tnet_set_sendbuff(s, 40000);
	//tnet_set_recvbuff(s, 204800);
	
	return 0;
}

int tcltapi_set_pdu_meta(HTCLTAPI a_hClt, LPTDRMETA a_pstRecvMeta, LPTDRMETA a_pstSendMeta)
{
	int iSendBuff;
	int iRecvBuff;

	assert(a_pstRecvMeta && a_pstSendMeta);

	a_hClt->pstRecvMeta	=	a_pstRecvMeta;
	a_hClt->pstSendMeta	=	a_pstSendMeta;

	iSendBuff	=	tdr_get_meta_size(a_pstSendMeta);
	iRecvBuff	=	tdr_get_meta_size(a_pstRecvMeta);

	if( iSendBuff>a_hClt->iSendBuff )
	{
		if( a_hClt->pszSendBuff )
		{
			free(a_hClt->pszSendBuff);
			a_hClt->pszSendBuff	=	NULL;
			a_hClt->iSendBuff	=	0;
		}

		a_hClt->pszSendBuff	=	(char*)calloc(1, iSendBuff);

		if( a_hClt->pszSendBuff )
			a_hClt->iSendBuff	=	iSendBuff;
	}

	if( iRecvBuff>a_hClt->iRecvBuff )
	{
		if( a_hClt->pszRecvBuff )
		{
			free(a_hClt->pszRecvBuff);
			a_hClt->pszRecvBuff	=	NULL;
			a_hClt->iRecvBuff	=	0;
		}

		a_hClt->pszRecvBuff	=	(char*)calloc(1, iRecvBuff);

		if( a_hClt->pszRecvBuff )
			a_hClt->iRecvBuff	=	iRecvBuff;
	}

	if( a_hClt->pszSendBuff && a_hClt->pszRecvBuff )
		return 0;
	else
		return -1;
}

void tcltapi_set_version(HTCLTAPI a_hClt, int a_iSelfVersion, int a_iPeerVersion)
{
	a_hClt->iSelfVersion	=	a_iSelfVersion;
	a_hClt->iPeerVersion	=	a_iPeerVersion;
}


int tcltapi_send_buff(HTCLTAPI a_hClt, const char* a_pszBuff, int a_iBuff, int a_iTimeout)
{
	return tnet_sendall(a_hClt->s, a_pszBuff, a_iBuff, a_iTimeout);
}

int tcltapi_recv_buff(HTCLTAPI a_hClt, char* a_pszBuff, int a_iBuff, int a_iTimeout)
{
	return tnet_recv(a_hClt->s, a_pszBuff, a_iBuff, a_iTimeout);
}

int tcltapi_send_msg(HTCLTAPI a_hClt, const char* a_pszMsg, int a_iMsg, int a_iTimeout)
{
	int iSend;
	int iRet;
	TDRDATA stHost;
	TDRDATA stNet;

	stHost.pszBuff	=	(char*) a_pszMsg;
	stHost.iBuff	=	a_iMsg;

	stNet.pszBuff	=	a_hClt->pszSendBuff;
	stNet.iBuff	=	a_hClt->iSendBuff;

	if( tdr_hton(a_hClt->pstSendMeta, &stNet, &stHost, a_hClt->iPeerVersion)<0 )
		return -1;

	iSend	=	tcltapi_send_buff(a_hClt, stNet.pszBuff, stNet.iBuff, a_iTimeout);
	
	if( iSend==stNet.iBuff )
	{
		iRet	=	0;
	}
	else
	{
		iRet	=	-1;
	}

	return iRet;
}

int tcltapi_get_err(HTCLTAPI a_hClt)
{
	if(a_hClt)
	{
		return a_hClt->iErr;
	}
	
	return TCLTAPI_ERR_NET;
}

int tcltapi_get_msg(HTCLTAPI a_hClt, char* a_pszMsg, int* a_piMsg)
{
	TDRDATA stHost;
	TDRDATA stNet;
	int iRet;

	if (0 >= a_hClt->iData)
		return -1;
	
	stHost.pszBuff	=	a_pszMsg;
	stHost.iBuff	=	*(a_piMsg);

	stNet.pszBuff	=	a_hClt->pszRecvBuff + a_hClt->iOff;
	stNet.iBuff	=	a_hClt->iData;


	iRet	=	tdr_ntoh(a_hClt->pstRecvMeta, &stHost, &stNet, a_hClt->iPeerVersion);

	if( 0==iRet )
	{
		a_hClt->iOff	+=	stNet.iBuff;
		a_hClt->iData	-=	stNet.iBuff;

		*a_piMsg	=	stNet.iBuff;

		return 0;
	}
	
	return -1;
}

int tcltapi_recv_msg(HTCLTAPI a_hClt, char* a_pszMsg, int* a_piMsg, int a_iTimeout)
{
	char* pszRecvBuff;
	int iRecvBuff;
	int iRecv;

	if(a_hClt->iErr )
		return -1;

	if( 0==tcltapi_get_msg(a_hClt, a_pszMsg, a_piMsg) )
	{
		return 1;
	}
	else if( a_hClt->iOff )
	{
		if( a_hClt->iData )
			memmove(a_hClt->pszRecvBuff, a_hClt->pszRecvBuff+a_hClt->iOff, a_hClt->iData);
			
		a_hClt->iOff	=	0;
	}

	pszRecvBuff	=	a_hClt->pszRecvBuff + a_hClt->iOff + a_hClt->iData;
	iRecvBuff	=	a_hClt->iRecvBuff - a_hClt->iOff - a_hClt->iData;

	if( iRecvBuff<=0 )
	{
		a_hClt->iErr	=	TCLTAPI_ERR_BUF;
		snprintf(a_hClt->szErrString, sizeof(a_hClt->szErrString), "XXXXX ERR BUFF, iRecvBuff=%d, iOff=%d, iData=%d\n", 
				a_hClt->iRecvBuff, a_hClt->iOff, a_hClt->iData);
		return -1;
	}
	
	iRecv	=	tnet_recv(a_hClt->s, pszRecvBuff, iRecvBuff, a_iTimeout);

	if( iRecv<0 )
	{
		a_hClt->iErr	=	TCLTAPI_ERR_NET;
		return -1;
	}
	else if( 0==iRecv )
	{
		return 0;
	}

	a_hClt->iData	+=	iRecv;
	
	
	if( 0==tcltapi_get_msg(a_hClt, a_pszMsg, a_piMsg) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int tcltapi_get_sock(HTCLTAPI a_hClt)
{
	return a_hClt->s;
}

int tcltapi_set_recvbuff(HTCLTAPI a_hClt, int a_iSize)
{
	return tnet_set_recvbuff(a_hClt->s, a_iSize);
}

int tcltapi_set_sendbuff(HTCLTAPI a_hClt, int a_iSize)
{
	return tnet_set_sendbuff(a_hClt->s, a_iSize);
}

char *tcltapi_get_errstring(HTCLTAPI a_hClt)
{
	return &a_hClt->szErrString[0];
}

int net_clt_han_create(char *pszMetaPath, char *pszConndIP,  unsigned short unPort, LPNETCLTHANDLE *ppNetCltHandle)
{
	NETCLTHANDLE *pstNetCltHandle = NULL;
	char szUri[128];
	int iRet = 0;
	
	pstNetCltHandle = (NETCLTHANDLE *)malloc(sizeof(NETCLTHANDLE));
	if (!pstNetCltHandle)
	{
		return -1;
	}

	memset(pstNetCltHandle, 0, sizeof(*pstNetCltHandle));
	
	if (0 > net_han_create(pszMetaPath, &pstNetCltHandle->pstNetHandle))
	{
		free(pstNetCltHandle);
		return -1;
	}


	tsocket_init(2,0);
	snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pszConndIP, unPort);
	if (0 > tcltapi_open(szUri, 6000, &pstNetCltHandle->hApi))
	{
		free(pstNetCltHandle);
		return -1;
	}

	if (0 > tcltapi_set_pdu_meta(pstNetCltHandle->hApi, pstNetCltHandle->pstNetHandle->pstCSProtoPkgMeta, 
								pstNetCltHandle->pstNetHandle->pstCSProtoPkgMeta))
	{
		iRet = -1;
	}

	tcltapi_set_version(pstNetCltHandle->hApi, TDR_METALIB_PROTO_CS_VERSION, 0);


	if (0 > iRet)
	{
		tcltapi_free(&pstNetCltHandle->hApi);
		free(pstNetCltHandle);
	}
	else
	{
		*ppNetCltHandle = pstNetCltHandle;
	}
	
	return iRet;
}

int net_clt_han_create_mem(unsigned char *pmetabuffer, char *pszConndIP,  unsigned short unPort, LPNETCLTHANDLE *ppNetCltHandle)
{
	NETCLTHANDLE *pstNetCltHandle = NULL;
	char szUri[128];
	int iRet = 0;

	pstNetCltHandle = (NETCLTHANDLE *)malloc(sizeof(NETCLTHANDLE));
	if (!pstNetCltHandle)
	{
		return -1;
	}

	memset(pstNetCltHandle, 0, sizeof(*pstNetCltHandle));

	if (0 > net_han_create_c(pmetabuffer, &pstNetCltHandle->pstNetHandle))
	{
		free(pstNetCltHandle);
		return -1;
	}


	tsocket_init(2,0);
	snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pszConndIP, unPort);
	if (0 > tcltapi_open(szUri, 6000, &pstNetCltHandle->hApi))
	{
		free(pstNetCltHandle);
		return -2;
	}

	if (0 > tcltapi_set_pdu_meta(pstNetCltHandle->hApi, pstNetCltHandle->pstNetHandle->pstCSProtoPkgMeta, 
		pstNetCltHandle->pstNetHandle->pstCSProtoPkgMeta))
	{
		iRet = -1;
	}

	tcltapi_set_version(pstNetCltHandle->hApi, TDR_METALIB_PROTO_CS_VERSION, 0);

	if (0 > iRet)
	{
		tcltapi_free(&pstNetCltHandle->hApi);
		free(pstNetCltHandle);
	}
	else
	{
		*ppNetCltHandle = pstNetCltHandle;
	}

	return iRet;
}

int net_clt_han_destroy(LPNETCLTHANDLE *ppNetCltHandle)
{
	LPNETCLTHANDLE pNetCltHandle = *ppNetCltHandle;

	if (NULL == pNetCltHandle)
	{
		return -1;
	}

	net_han_destroy(&pNetCltHandle->pstNetHandle);
	tcltapi_free(&pNetCltHandle->hApi);
	
	free(pNetCltHandle);
	
	*ppNetCltHandle = NULL;
	
	return 0;
}

int net_clt_recv(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, int iTimeout)
{
	int iRet;
	int iLen = sizeof(CSPKG);
	//TPDUHEAD stPduHead;
	//char sKey[TQQ_KEY_LEN];

	if (NULL == pNetCltHandle)
	{
		return -1;
	}
	
	iRet = tcltapi_recv_msg(pNetCltHandle->hApi, (char *)pstPkg, &iLen, iTimeout);
	if (1 == iRet)
	{
		if (0 < iLen && ACC_LOGIN_RES == pstPkg->Head.Cmd && LOGIN_SUCC == pstPkg->Body.AccLoginRes.Result)
		{
			tcltapi_set_version(pNetCltHandle->hApi, TDR_METALIB_PROTO_CS_VERSION, pstPkg->Body.AccLoginRes.SvrProtoVer);
		}
	}
	else if (0 > iRet)
	{
		pNetCltHandle->iErr = tcltapi_get_err(pNetCltHandle->hApi);
	}

	return iRet;
}

int net_clt_send(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, int iTimeout)
{
	if (NULL == pNetCltHandle)
	{
		return -1;
	}
	
	pstPkg->Head.Ver =TDR_METALIB_PROTO_CS_VERSION;
		
	return tcltapi_send_msg(pNetCltHandle->hApi, (char *)pstPkg, sizeof(CSPKG), iTimeout);
}

char * rand_str(char* pszBuff, int iLen)
{
	char *pszChars=gs_szCharList;

	int	i, iChars;

	iChars	=	sizeof(gs_szCharList) - 1;

	for ( i=0; i<iLen;i++)
	{	
		pszBuff[i]	=	pszChars[ (int)( (float)iChars*rand()/(RAND_MAX+1.0) ) ];
	}

	return pszBuff;
}

void auth_init_base(PDUBASE* pstBase)
{
	pstBase->Magic	=	PDU_MAGIC;
	pstBase->Version	=	PDU_VERSION;
	pstBase->Cmd		=	PDU_CMD_NONE;
	pstBase->HeadLen	=	(unsigned char)sizeof(PDUBASE);
	pstBase->BodyLen	=	0;
}

static char HEX_value[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

static char* Bin2Hex(unsigned char * pbin,int binlen, char* phex,int *hexlen)
{
	unsigned char  low,hi;
	int j=0,i=0;
	if (*hexlen < (binlen * 2+1))
	{
		return NULL;
	}
	while (i <= binlen )
	{
              low = pbin[i] & 0X0F;
              hi = pbin[i]>>4;
		phex[j++] = HEX_value[hi];
		phex[j++] = HEX_value[low];
		i++;
	}
	*hexlen = binlen * 2;
	phex[*hexlen]=0;
      return phex;
}

static char* Hex2Bin(char * phex,int hexlen, unsigned char* pbin,int * binlen)
{

	int j=0;
	int i=0;
	if (hexlen > (*binlen) * 2)
	{
		return NULL;
	}
	while (j < hexlen )
	{
		if (phex[j] < 'A')
		{
			pbin[i] = phex[j++] -'0';
		}
		else
		{
		       pbin[i] = phex[j++] -'A' + 10;
		}

		pbin[i] <<=4;
		
		if (phex[j] < 'A')
		{
			pbin[i] |= phex[j++] -'0';
		}
		else
		{
		       pbin[i] |= phex[j++] -'A' + 10;
		}
		i++;
	}
	*binlen=  hexlen / 2;
	
       return (char*) pbin;
}


int auth_send_msg(NETCLTAUTHHANDLE *pstNetCltAuthHandle, PDUHEAD* pstHead, const char* pszMsg, int iMsg, int iTimeout)
{
	TDRDATA stHost;
	TDRDATA stNet;
	PDUHEAD stHead;
	CSPKG *pstPkg = (CSPKG *)pszMsg;
	char* pszEnc=NULL;
	int iEnc=0;
	int iHead=0;
	int iPkg=0;
	int iRet=0;
	int iSend=0;

	if( NULL==pstHead )
	{
		auth_init_base(&stHead.Base);
		pstHead	=	&stHead;
	}

	switch (pstPkg->Head.Cmd)
	{
		case SKILL_ATK:
		case SKILL_HIT:	
		case MOVE_CLT:	
			pstHead->Base.Cmd = PDU_CMD_PLAIN;
			break;
		default:
			break;
	}

	stNet.pszBuff	=	pstNetCltAuthHandle->pszSendBuff;
	stNet.iBuff	=	pstNetCltAuthHandle->iSendBuff;

	stHost.pszBuff	=	(char*)pstHead;
	stHost.iBuff	=	(int)sizeof(*pstHead);

	iRet = tdr_hton(pstNetCltAuthHandle->pstMetaHead, &stNet, &stHost, PDU_VERSION);
	if( iRet<0 )
	{
              pstNetCltAuthHandle->iErr=AUTH_ERR_TDR;
		pstNetCltAuthHandle->iTdrErrorCode = iRet;
		return -2;
	}

	iHead		=	stNet.iBuff;
	((PDUBASE*)pstNetCltAuthHandle->pszSendBuff)->HeadLen	=	(unsigned char)iHead;

	if( iMsg>0 )
	{
		stHost.pszBuff	=	(char*)pszMsg;
		stHost.iBuff	=	iMsg;

		if (pstHead->Base.Cmd != PDU_CMD_PLAIN)
		{
			stNet.pszBuff	=	pstNetCltAuthHandle->pszSendBuff2;
			stNet.iBuff	=	pstNetCltAuthHandle->iSendBuff2 - iHead - 32;
		
			iRet = tdr_hton(pstNetCltAuthHandle->pstSendMeta, &stNet, &stHost, pstNetCltAuthHandle->iPeerVersion);
			if( iRet <0 )
			{
				pstNetCltAuthHandle->iErr=AUTH_ERR_TDR;
				pstNetCltAuthHandle->iTdrErrorCode = iRet;
				return iRet;
			}
		
			pszEnc	=	pstNetCltAuthHandle->pszSendBuff + iHead;
			iEnc	=	pstNetCltAuthHandle->iSendBuff - iHead;
			if (0 > taes_encrypt(pstNetCltAuthHandle->pszSendBuff2, stNet.iBuff, &pstNetCltAuthHandle->stAesGameKey,
								pszEnc, &iEnc))
			{
				pstNetCltAuthHandle->iErr=AUTH_ERR_SYS;
				pstNetCltAuthHandle->iTdrErrorCode = 0;
				return -1;
			}

			stNet.iBuff	=	iEnc;
		}
		else
		{
			stNet.pszBuff	=	pstNetCltAuthHandle->pszSendBuff + iHead;
			stNet.iBuff	=	pstNetCltAuthHandle->iSendBuff - iHead;
		
			iRet = tdr_hton(pstNetCltAuthHandle->pstSendMeta, &stNet, &stHost, pstNetCltAuthHandle->iPeerVersion);
			if( iRet <0 )
			{
				pstNetCltAuthHandle->iErr=AUTH_ERR_TDR;
				pstNetCltAuthHandle->iTdrErrorCode = iRet;
				return iRet;
			}
		}
	}
	else
	{
		stNet.iBuff	=	0;
	}

	((PDUBASE*)pstNetCltAuthHandle->pszSendBuff)->BodyLen	=	(int)HTONL((long)stNet.iBuff);

	iPkg	=	iHead + stNet.iBuff;
	
       iSend=tnet_sendall(pstNetCltAuthHandle->s, pstNetCltAuthHandle->pszSendBuff, iPkg, iTimeout);
	if( iPkg!= iSend)
	{
		pstNetCltAuthHandle->iErr	=	AUTH_ERR_NET;
		return -3;
	}
	else
	{
		return 0;
	}
}

int get_clt_auth_han_info(NetCltAuthInfo *pstNetCltAuthInfo)
{
	*pstNetCltAuthInfo = g_stNetCltAuthInfo;
	return 0;
}


int net_clt_acc_get_role(char* pstLibC,char *pszAuthConnIP, unsigned short unAuthConnPort,
	char *pszAccountLoginName, ACCGETROLESVR *pstRes)
{
	int iRet;
	LPNETCLTHANDLE pNetCltHandle = NULL;
	CSPKG *pstMsg2 = NULL;
	ACCGETROLECLT *pstGetAccRoleReq;
	RSA *pstRsa = NULL;
	ACCGETROLEINFO stAccGetRoleInfo;
	LPTDRMETA pstAccGetRoleInfoMeta;
	TDRDATA stHost, stNet;
	char szBuff[256];

	pstMsg2 = malloc(sizeof(CSPKG));
	if (NULL == pstMsg2)
	{
		return -1;
	}

	iRet = net_clt_han_create_mem((unsigned char *)pstLibC, pszAuthConnIP, unAuthConnPort, &pNetCltHandle);	
	if (0 > iRet)
	{
		return iRet;
	}

	pstMsg2->Head.Magic = PROTOCOL_MAGIC;
	pstMsg2->Head.Cmd = ACC_GET_ROLE_CLT;
	pstGetAccRoleReq = &pstMsg2->Body.AccGetRoleClt;
	
	STRNCPY(stAccGetRoleInfo.AccountName, pszAccountLoginName, sizeof(stAccGetRoleInfo.AccountName));
	stAccGetRoleInfo.Rand = rand();
	stAccGetRoleInfo.SigLen = g_stNetCltAuthInfo.AuthJC.SigLen;
	memcpy(stAccGetRoleInfo.Sig, g_stNetCltAuthInfo.AuthJC.Sig, sizeof(stAccGetRoleInfo.Sig));
	pstAccGetRoleInfoMeta = tdr_get_meta_by_name((LPTDRMETALIB)pstLibC, "AccGetRoleInfo");
	if (NULL == pstAccGetRoleInfoMeta)
	{
		iRet = -1;
		goto _error;
	}

	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	stHost.pszBuff = (char *)&stAccGetRoleInfo;
	stHost.iBuff = sizeof(stAccGetRoleInfo);
	if (0 > tdr_hton(pstAccGetRoleInfoMeta, &stNet, &stHost, 0) || stNet.iBuff > (int)sizeof(pstGetAccRoleReq->Enc))
	{
		iRet = -1;
		goto _error;
	}

	pstRsa = rsa_from_public_key(IDRSAPublicKey);
	if (NULL == pstRsa)
	{
		iRet = -1;
		goto _error;
	}
	
	if (0 > rsa_public_encrypt(pstRsa, stNet.iBuff, stNet.pszBuff, pstGetAccRoleReq->Enc))
	{
		iRet = -1;
		goto _error;
	}
	
	if (0 > net_clt_send(pNetCltHandle, pstMsg2, 3000))
	{
		iRet = -2;
		goto _error;
	}
	
	if (0 >= net_clt_recv(pNetCltHandle, pstMsg2, 3000))
	{
		iRet = -2;
		goto _error;
	}

	if (pstMsg2->Head.Cmd != ACC_GET_ROLE_SVR)
	{
		iRet = -2;
		goto _error;
	}

	if (pstRes)
	{
		memcpy(pstRes, &pstMsg2->Body.AccGetRoleSvr, sizeof(*pstRes));
	}

	if (pstMsg2)
	{
		free(pstMsg2);
	}

	return 0;

_error:
	if (pstRsa)
	{
		rsa_free(pstRsa);
	}

	if (pNetCltHandle)
	{
		net_clt_han_destroy(&pNetCltHandle);
	}

	if (pstMsg2)
	{
		free(pstMsg2);
	}
	
	return iRet;
}

int net_clt_acc_login(char *pszZoneConnIP, unsigned short unZoneConnPort, char *pszAccountLoginName, int iCltVersion,
						NETCLTAUTHHANDLE *pstNetCltAuthHandle, int iSigLen, char *szSigInfo,char *MacAddr,
						int iCltType,
						int iAuthFlag
						)
{
	char szUri[128];
	int iRet;
	CSPKG *pstPkg2 = NULL;
	CSACCLOGINREQ *pstAccLogin;
	PDUHEAD stPDUHead;
	PDUEXTAUTHINFO *pstAuthSigInfo;

	pstPkg2 = malloc(sizeof(CSPKG));
	if (NULL == pstPkg2)
	{
		return AUTH_ERR_SYS;
	}

	pstAccLogin = &pstPkg2->Body.AccLoginReq;
	memset(pstAccLogin,0,sizeof(CSACCLOGINREQ));
	
	snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pszZoneConnIP, unZoneConnPort);
	pstNetCltAuthHandle->s = tnet_connect(szUri, 3000);
	if (-1 == pstNetCltAuthHandle->s)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	//tnet_set_nonagle(pstNetCltAuthHandle->s);
	//tnet_set_sendbuff(pstNetCltAuthHandle->s, 20480);
	//tnet_set_recvbuff(pstNetCltAuthHandle->s, 204800);

	pstPkg2->Head.Cmd = ACC_LOGIN_REQ;
	pstPkg2->Head.Magic = PROTOCOL_MAGIC;
	pstPkg2->Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	pstAccLogin->Uin = pstNetCltAuthHandle->iUin;
	pstAccLogin->CltProtoVer = TDR_METALIB_PROTO_CS_VERSION;
	pstAccLogin->CltVer = iCltVersion;
	STRNCPY( pstAccLogin->AccountLoginName, pszAccountLoginName, 
			 	sizeof(pstAccLogin->AccountLoginName));	 
	if(NULL != MacAddr)
	{
		STRNCPY( pstAccLogin->MacAdd, MacAddr, 
			 	sizeof(pstAccLogin->MacAdd));
	}

	pstAccLogin->AuthFlag = iAuthFlag;
	pstAccLogin->CltType = iCltType;
	
	auth_init_base(&stPDUHead.Base);
	stPDUHead.Base.Cmd = PDU_CMD_AUTH;
	pstAuthSigInfo = &stPDUHead.Ext.AuthInfo;
	pstAuthSigInfo->AuthType = AUTH_JC;
	pstAuthSigInfo->AuthData.AuthJC.Uin = pstAccLogin->Uin;
	pstAuthSigInfo->AuthData.AuthJC.SigLen = iSigLen;

	if (pstAuthSigInfo->AuthData.AuthJC.SigLen > (int)sizeof(pstAuthSigInfo->AuthData.AuthJC.Sig))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}
	memcpy(pstAuthSigInfo->AuthData.AuthJC.Sig, szSigInfo, 
			pstAuthSigInfo->AuthData.AuthJC.SigLen);
	if (0 > auth_send_msg(pstNetCltAuthHandle, &stPDUHead, (char *)pstPkg2, sizeof(*pstPkg2), 500))
	{
		iRet = pstNetCltAuthHandle->iErr;
		goto _error;
	}

	//暂时保存
	g_stNetCltAuthInfo.stAesGameKey = pstNetCltAuthHandle->stAesGameKey;
	g_stNetCltAuthInfo.AuthJC = pstAuthSigInfo->AuthData.AuthJC;
	STRNCPY( g_stNetCltAuthInfo.AccountLoginName, pszAccountLoginName, 
			 	sizeof(g_stNetCltAuthInfo.AccountLoginName));

	if (pstPkg2)
	{
		free(pstPkg2);
	}
	
	return 0;
_error:
	if (pstPkg2)
	{
		free(pstPkg2);
	}

	//net_clt_han_auth_destroy(&pstNetCltAuthHandle);
	
	return iRet;
}

int net_clt_direct_conn_c(char *pstLibC, NetCltAuthInfo *pstNetCltAuthInfo, char *pszZoneConnIP, unsigned short unZoneConnPort, int iCltVersion, 
							LPNETCLTAUTHHANDLE *ppNetCltAuthHandle, char *MacAddr,	int iCltType)
{
	LPTDRMETALIB pstCSProtoLib;
	LPTDRMETA pstAuthMeta;
	int iSendBuff;
	int iRecvBuff;
	NETCLTAUTHHANDLE *pstNetCltAuthHandle = NULL;	
	int iRet = 0;

	pstCSProtoLib = (LPTDRMETALIB)pstLibC;

	pstNetCltAuthHandle = (NETCLTAUTHHANDLE *)malloc(sizeof(NETCLTAUTHHANDLE));
	if (!pstNetCltAuthHandle)
	{
		return AUTH_ERR_SYS;
	}

	memset(pstNetCltAuthHandle, 0, sizeof(*pstNetCltAuthHandle));
	pstNetCltAuthHandle->s = -1;
	pstNetCltAuthHandle->pstMetaBase = tdr_get_meta_by_name(pstCSProtoLib, "PDUBase");
	pstNetCltAuthHandle->pstMetaHead = tdr_get_meta_by_name(pstCSProtoLib, "PDUHead");
	pstNetCltAuthHandle->pstRecvMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstNetCltAuthHandle->pstSendMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstAuthMeta = tdr_get_meta_by_name(pstCSProtoLib, "AuthInfo");

	if (!(pstNetCltAuthHandle->pstMetaBase && pstNetCltAuthHandle->pstMetaHead &&
		pstNetCltAuthHandle->pstRecvMeta && pstNetCltAuthHandle->pstSendMeta && pstAuthMeta))
	{
		return AUTH_ERR_SYS;
	}

	iSendBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstSendMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;
	iRecvBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstRecvMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;

	pstNetCltAuthHandle->iSendBuff2 = pstNetCltAuthHandle->iSendBuff = iSendBuff;
	pstNetCltAuthHandle->iRecvBuff2 = pstNetCltAuthHandle->iRecvBuff = iRecvBuff;

	pstNetCltAuthHandle->pszRecvBuff = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszRecvBuff2 = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszSendBuff = (char*)calloc(1, iSendBuff);
	pstNetCltAuthHandle->pszSendBuff2 = (char*)calloc(1, iSendBuff);

	if (!(pstNetCltAuthHandle->pszRecvBuff && pstNetCltAuthHandle->pszRecvBuff2 &&
		pstNetCltAuthHandle->pszSendBuff && pstNetCltAuthHandle->pszSendBuff2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}
	
	pstNetCltAuthHandle->stAesGameKey = pstNetCltAuthInfo->stAesGameKey;
	pstNetCltAuthHandle->iUin = pstNetCltAuthInfo->AuthJC.Uin;

	tsocket_init(2,0);
	
	iRet = net_clt_acc_login(pszZoneConnIP, unZoneConnPort, pstNetCltAuthInfo->AccountLoginName, iCltVersion,
							pstNetCltAuthHandle, pstNetCltAuthInfo->AuthJC.SigLen, pstNetCltAuthInfo->AuthJC.Sig,MacAddr,
							iCltType, 0);
	if (0 > iRet)
	{
		goto _error;
	}
	
	*ppNetCltAuthHandle = pstNetCltAuthHandle;

	return 0;
_error:

	net_clt_han_auth_destroy(&pstNetCltAuthHandle);
	
	return iRet;
}




int net_clt_direct_conn(char *pszMetaPath, NetCltAuthInfo *pstNetCltAuthInfo, char *pszZoneConnIP, unsigned short unZoneConnPort, int iCltVersion, 
							LPNETCLTAUTHHANDLE *ppNetCltAuthHandle, char *MacAddr, int iCltType)
{
	int iRet;
	LPTDRMETALIB pstCSProtoLib;

	pstCSProtoLib = meta_file_lib(pszMetaPath);
	if (NULL == pstCSProtoLib)
	{
		return AUTH_ERR_SYS;
	}

	iRet = net_clt_direct_conn_c((char * )pstCSProtoLib, pstNetCltAuthInfo, pszZoneConnIP, unZoneConnPort, iCltVersion, 
							ppNetCltAuthHandle, MacAddr, iCltType);
	if (0 > iRet)
	{
		free(pstCSProtoLib);
		return iRet;
	}

	(*ppNetCltAuthHandle)->pstLib = (char *)pstCSProtoLib;

	return iRet;
}

int net_clt_auth_msg_create_by_web_sign(char *pstLibC,	
								char* szWebSign,
								int iCltType,
								int iAuthFlag,
								CSPKG *pstPkg)
{
	LPTDRMETALIB pstCSProtoLib;
	LPTDRMETA pstAuthMeta;
	//TDRDATA stHost, stNet;	
	CSAUTHCLT *pstAuthClt = &pstPkg->Body.AuthClt;
	//char szAuth[128];
	char RandK1[KEY_LEN]; 
	int iRet = 0;
	int iEncLen = 0;
	char szBin[576];

	memset(pstAuthClt,0,sizeof(CSAUTHCLT));
	
	pstCSProtoLib = (LPTDRMETALIB)pstLibC;

	pstAuthMeta = tdr_get_meta_by_name(pstCSProtoLib, "AuthInfo");

	if (!pstAuthMeta)
	{
		return -1;
	}

	pstPkg->Head.Cmd = AUTH_CLT;
	pstPkg->Head.Magic = PROTOCOL_MAGIC;
	pstPkg->Head.Ver = TDR_METALIB_PROTO_CS_VERSION;

	
	memset(RandK1,0, sizeof(RandK1));
	
	memset(pstAuthClt->Enc,0,sizeof(pstAuthClt->Enc));	
	//szWebSign必须全部是大写
	iEncLen = sizeof(szBin);
	
	memset(szBin,0,sizeof(szBin));
	
	Hex2Bin(szWebSign, strlen(szWebSign),(unsigned char*)szBin,&iEncLen);

	memcpy(RandK1,szBin,KEY_LEN);

	memcpy(pstAuthClt->RandK1, RandK1, sizeof(pstAuthClt->RandK1));

	memcpy(pstAuthClt->Enc,&szBin[KEY_LEN],iEncLen - KEY_LEN);
	
	pstAuthClt->CltType = iCltType;
	pstAuthClt->AuthFlag = iAuthFlag;
	iRet = 0;

	return iRet;
}


int net_clt_auth_msg_create(	LPTDRMETA pstAuthMeta,
								RSA *pstRsa,
								char *pszAccountLoginName, 
								char *pszPasswd,
								char *Birthday,
								int iWebUin,
								tdr_ip_t IP,
								char *CertID,
								char *RealName,
								char *eMail,
								char *Mobile,
								int iCltType,
								int iAuthFlag,
								CSPKG *pstPkg,
								PASSWDCARDAUTHINFO *pstPasswdCard)
{

	AUTHINFO stAuthInfo;
	TDRDATA stHost, stNet;
	
	CSAUTHCLT *pstAuthClt = &pstPkg->Body.AuthClt;

	char szAuth[256];
	//RSA *pstRsa = NULL;
	char RandK1[KEY_LEN];  

	int iRet = 0;
	char szMd5[16];


	memset(pstAuthClt,0,sizeof(CSAUTHCLT));
	memset(&stAuthInfo,0,sizeof(stAuthInfo));
	

	Md5HashBuffer(szMd5, pszPasswd, strlen(pszPasswd));
	Md5HashBuffer(szMd5, szMd5, 16);

	pstRsa = rsa_from_public_key(IDRSAPublicKey);

	pstPkg->Head.Cmd = AUTH_CLT;
	pstPkg->Head.Magic = PROTOCOL_MAGIC;
	pstPkg->Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	rand_str(RandK1, sizeof(RandK1));
	memcpy(pstAuthClt->RandK1, RandK1, sizeof(pstAuthClt->RandK1));
	memcpy(stAuthInfo.RandK1, RandK1, sizeof(pstAuthClt->RandK1));
	STRNCPY(stAuthInfo.AccountLoginName, pszAccountLoginName, sizeof(stAuthInfo.AccountLoginName));
	STRNCPY(stAuthInfo.AccountLoginPasswd, pszPasswd, sizeof(stAuthInfo.AccountLoginPasswd));

	stAuthInfo.WebUin = iWebUin;
	stAuthInfo.AuthInfoType = 0;
	
	
	if( NULL != Birthday )
	{
		STRNCPY(stAuthInfo.Birthday,Birthday,sizeof(stAuthInfo.Birthday));
	}
	
	if( NULL != CertID  )
	{
		STRNCPY(stAuthInfo.CertID,CertID,sizeof(stAuthInfo.CertID));
		pstAuthClt->ActiveFlag = ACTIVE_FLAG_REG;
	}
	if( NULL != RealName )
	{
		STRNCPY(stAuthInfo.RealName,RealName,sizeof(stAuthInfo.RealName));
	}
	if( NULL != eMail )
	{
		STRNCPY(stAuthInfo.eMail,eMail,sizeof(stAuthInfo.eMail));
	}
	if( NULL != Mobile)
	{
		STRNCPY(stAuthInfo.Mobile,Mobile,sizeof(stAuthInfo.Mobile));
	}

	
	
	
	stNet.pszBuff = szAuth;
	stNet.iBuff = sizeof(szAuth);
	stHost.pszBuff = (char *)&stAuthInfo;
	stHost.iBuff = sizeof(stAuthInfo);
	if (0 > tdr_hton(pstAuthMeta, &stNet, &stHost, 0) || stNet.iBuff > (int)sizeof(pstAuthClt->Enc))
	{
		iRet = -1;
		goto _error;
	}

	if (0 > rsa_public_encrypt(pstRsa, stNet.iBuff, stNet.pszBuff, pstAuthClt->Enc))
	{
		iRet = -1;
		goto _error;
	}
	pstAuthClt->CltType = iCltType;
	pstAuthClt->AuthFlag = iAuthFlag;
	pstAuthClt->IP = (int)IP;

	if ( NULL != pstPasswdCard)
	{
		pstAuthClt->PasswdCardAuthInfo = *pstPasswdCard;
	}
	

	
	iRet = 0;

_error:


	
	return iRet;
}

//通过web签名认证
int net_clt_auth_han_create_web_sign(char *pszMetaPath, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,								
								char *pszWebSign, int iCltVersion,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,								
								int iCltType,
								int iAuthFlag)
{
	LPTDRMETALIB pstCSProtoLib;
	int iRet = 0;
	
	pstCSProtoLib = meta_file_lib(pszMetaPath);
	if (NULL == pstCSProtoLib)
	{
		return AUTH_ERR_SYS;
	}
	
	iRet = net_clt_auth_han_create_web_sign_c((char *) pstCSProtoLib,pszAuthConndIP,
				unAuthPort,pszZoneConnIP,unZoneConnPort,pszWebSign,iCltVersion,
				ppNetCltAuthHandle,iCltType,iAuthFlag);

	if (pstCSProtoLib)
	{
		free(pstCSProtoLib);
		pstCSProtoLib = NULL;
	}

	return iRet;
}


//通过web签名认证
int net_clt_auth_han_create_web_sign_c(char *pstLibC, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,								
								char *pszWebSign, int iCltVersion,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,								
								int iCltType,
								int iAuthFlag)
{
	LPTDRMETALIB pstCSProtoLib;
	int iSocket;
	//AUTHINFO stAuthInfo;
	TDRDATA stHost, stNet;
	CSPKG *pstPkg2 = NULL;
	//CSAUTHCLT *pstAuthClt = &stPkg.Body.AuthClt;
	NETCLTAUTHHANDLE *pstNetCltAuthHandle = NULL;
	char szUri[128];
	//RSA *pstRsa = NULL;
	char RandK1[KEY_LEN];
	int iLen;
	int iRet = 0;
	char szMd5[16];
	char *pszEnc;
	int iEnc;
	char cResult;
	char szDecrypt[512];
	int iNameLen = 0;
	char szAccName[ACCOUNT_LOGIN_NAME_LEN];
	int iOffset = 0;
	int iAccNameLen = 0;

	srand(time(NULL));

	pstCSProtoLib = (LPTDRMETALIB)pstLibC;

	if (NULL == pstCSProtoLib)
	{
		return AUTH_ERR_SYS;
	}	 
	
	pstPkg2 = malloc(sizeof(CSPKG));
	if (NULL == pstPkg2)
	{
		return AUTH_ERR_SYS;
	}

	iRet = net_clt_auth_han_create_init(pstCSProtoLib,&pstNetCltAuthHandle);
	if (iRet < 0)
	{
		goto _error;
	}

	strupr(pszWebSign);
	
	iAuthFlag |= AUTH_FLAG_PLUGIN_SIGN;

	if (0 > net_clt_auth_msg_create_by_web_sign((char*)pstCSProtoLib,
				pszWebSign,
				iCltType,iAuthFlag, pstPkg2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	Md5HashBuffer(szMd5, pstPkg2->Body.AuthClt.Enc, sizeof(pstPkg2->Body.AuthClt.Enc));
	szMd5[0] = pstPkg2->Body.AuthClt.Enc[1];
	szMd5[1] = pstPkg2->Body.AuthClt.Enc[2];	
	Md5HashBuffer(szMd5, szMd5, sizeof(szMd5));

	memcpy(RandK1, pstPkg2->Body.AuthClt.RandK1, sizeof(RandK1));
	memcpy(g_stNetCltAuthInfo.szMd5,szMd5,sizeof(szMd5));	
	memcpy(&g_stNetCltAuthInfo.stAuthClt,&pstPkg2->Body.AuthClt,sizeof(CSAUTHCLT));
	g_stNetCltAuthInfo.iCltVer = iCltVersion;
	
	stNet.iBuff = pstNetCltAuthHandle->iSendBuff;
	stNet.pszBuff= pstNetCltAuthHandle->pszSendBuff;
	stHost.iBuff = sizeof(*pstPkg2);
	stHost.pszBuff = (char *)pstPkg2;
	if (0 > tdr_hton(pstNetCltAuthHandle->pstSendMeta, &stNet, &stHost, 0))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pszAuthConndIP, unAuthPort);
	iSocket	=	tnet_connect(szUri, 6000);
	if (-1 == iSocket)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	tnet_set_nonagle(iSocket);
	tnet_set_sendbuff(iSocket, 20480);
	tnet_set_recvbuff(iSocket, 204800);
		
	if (0 > tnet_sendall(iSocket, stNet.pszBuff, stNet.iBuff, 1000))
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	iLen	=	tnet_recv(iSocket, pstNetCltAuthHandle->pszRecvBuff, pstNetCltAuthHandle->iRecvBuff, 6000);
	if (iLen < 0)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	tnet_close(iSocket);
	
	stNet.iBuff = iLen;
	stNet.pszBuff= pstNetCltAuthHandle->pszRecvBuff;
	stHost.iBuff = sizeof(*pstPkg2);
	stHost.pszBuff = (char *)pstPkg2;
	if (0 > tdr_ntoh(pstNetCltAuthHandle->pstRecvMeta, &stHost, &stNet, 0))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	if (pstPkg2->Head.Cmd != AUTH_SVR)
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szMd5) ||
		0 > taes_setkey(&pstNetCltAuthHandle->stAesK1, (unsigned char *)RandK1))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	pszEnc = pstPkg2->Body.AuthSvr.Auth;
	iEnc = pstPkg2->Body.AuthSvr.AuthLen;
	iLen = sizeof(szDecrypt);
	if (0 > taes_decrypt(pszEnc, iEnc, &pstNetCltAuthHandle->stAesGameKey, szDecrypt, &iLen) &&
		0 > taes_decrypt(pszEnc, iEnc, &pstNetCltAuthHandle->stAesK1, szDecrypt, &iLen))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	cResult = szDecrypt[1];
	switch(cResult)
	{
	case 0:
		memset(&g_stNetCltAuthInfo.stAuthClt,0,sizeof(g_stNetCltAuthInfo.stAuthClt));
		memset(g_stNetCltAuthInfo.szMd5,0,sizeof(g_stNetCltAuthInfo.szMd5));
		memset(g_stNetCltAuthInfo.szWebSign,0,sizeof(g_stNetCltAuthInfo.szWebSign));
		break;
	
	default:
		memset(g_stNetCltAuthInfo.szWebSign,0,sizeof(g_stNetCltAuthInfo.szWebSign));
		memset(&g_stNetCltAuthInfo.stAuthClt,0,sizeof(g_stNetCltAuthInfo.stAuthClt));
		memset(g_stNetCltAuthInfo.szMd5,0,sizeof(g_stNetCltAuthInfo.szMd5));
		iRet = cResult;
		goto _error;
		break;
	}
	iOffset = 2;

	memcpy(&iNameLen,szDecrypt+iOffset, sizeof(int));
	
	iNameLen = NTOHL(iNameLen);

	if (0 > iNameLen)
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}
	
	iOffset += sizeof(int);
	
	iAccNameLen = iNameLen;
	if (iAccNameLen >= (int)sizeof(szAccName))
	{
		iAccNameLen = sizeof(szAccName);
	}
	
	STRNCPY(szAccName,szDecrypt + iOffset,iAccNameLen);	

	iOffset += iNameLen;

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szDecrypt+iOffset))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	iOffset += KEY_LEN;

	

	memcpy(&pstNetCltAuthHandle->iUin, szDecrypt+iOffset, sizeof(int));
	pstNetCltAuthHandle->iUin = NTOHL(pstNetCltAuthHandle->iUin);

	iOffset += sizeof(int);

	
	iRet = net_clt_acc_login(pszZoneConnIP, unZoneConnPort, szAccName, iCltVersion,
							pstNetCltAuthHandle, iLen - iOffset, 
							szDecrypt+iOffset,NULL,iCltType,iAuthFlag);
	if (0 > iRet)
	{
		goto _error;
	}

	*ppNetCltAuthHandle = pstNetCltAuthHandle;

	

	
	iRet = 0;

	if (pstPkg2)
	{
		free(pstPkg2);
	}
	return iRet;

_error:
	
	if (pstNetCltAuthHandle)
	{
		net_clt_han_auth_destroy(&pstNetCltAuthHandle);
	}
	
	return iRet;
}

int net_clt_auth_han_create_in(LPTDRMETALIB pstCSProtoLib,NETCLTAUTHHANDLE **ppNetCltAuthHandle)
{
	int iSendBuff;
	int iRecvBuff;
	LPTDRMETA pstAuthMeta;
	int iRet = 0;
	NETCLTAUTHHANDLE *pstNetCltAuthHandle = NULL;

	*ppNetCltAuthHandle = NULL;
	
	pstNetCltAuthHandle = (NETCLTAUTHHANDLE *)malloc(sizeof(NETCLTAUTHHANDLE));
	if (!pstNetCltAuthHandle)
	{
		return AUTH_ERR_SYS;
	}

	memset(pstNetCltAuthHandle, 0, sizeof(*pstNetCltAuthHandle));
	pstNetCltAuthHandle->s = -1;
	pstNetCltAuthHandle->pstMetaBase = tdr_get_meta_by_name(pstCSProtoLib, "PDUBase");
	pstNetCltAuthHandle->pstMetaHead = tdr_get_meta_by_name(pstCSProtoLib, "PDUHead");
	pstNetCltAuthHandle->pstRecvMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstNetCltAuthHandle->pstSendMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstAuthMeta = tdr_get_meta_by_name(pstCSProtoLib, "AuthInfo");

	if (!(pstNetCltAuthHandle->pstMetaBase && pstNetCltAuthHandle->pstMetaHead &&
		pstNetCltAuthHandle->pstRecvMeta && pstNetCltAuthHandle->pstSendMeta && pstAuthMeta))
	{
		return AUTH_ERR_SYS;
	}

	iSendBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstSendMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;
	iRecvBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstRecvMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;

	pstNetCltAuthHandle->iSendBuff2 = pstNetCltAuthHandle->iSendBuff = iSendBuff;
	pstNetCltAuthHandle->iRecvBuff2 = pstNetCltAuthHandle->iRecvBuff = iRecvBuff;

	pstNetCltAuthHandle->pszRecvBuff = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszRecvBuff2 = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszSendBuff = (char*)calloc(1, iSendBuff);
	pstNetCltAuthHandle->pszSendBuff2 = (char*)calloc(1, iSendBuff);

	if (!(pstNetCltAuthHandle->pszRecvBuff && pstNetCltAuthHandle->pszRecvBuff2 &&
		pstNetCltAuthHandle->pszSendBuff && pstNetCltAuthHandle->pszSendBuff2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}
	
	pstNetCltAuthHandle->pstAuthMeta = pstAuthMeta;

	*ppNetCltAuthHandle = pstNetCltAuthHandle;

	return iRet;

_error:
	return iRet;
}



int net_clt_auth_han_create_init(LPTDRMETALIB pstCSProtoLib,
							NETCLTAUTHHANDLE **ppNetCltAuthHandle )
{
	int iRet ;
	
	memset(&g_stNetCltAuthInfo, 0, sizeof(g_stNetCltAuthInfo));
	g_stNetCltAuthInfo.iSocket = -1;

	iRet = net_clt_auth_han_create_in(pstCSProtoLib,ppNetCltAuthHandle);
	if (iRet < 0)
	{
		return iRet;
	}

	tsocket_init(2,0);
	
	return 0;
}

int net_clt_auth_conn(char *pszAuthConndIP,  unsigned short unAuthPort,int* piSocket)
{
	char szUri[128];
	
	snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pszAuthConndIP, unAuthPort);
	*piSocket	=	tnet_connect2(szUri, 6000,20000,30000);
	
	if (-1 == *piSocket)
	{
		return AUTH_ERR_NET;
	}

/*
	tnet_set_nonagle(*piSocket);
	tnet_set_sendbuff(*piSocket, 20480);
	tnet_set_recvbuff(*piSocket, 204800);

*/
	return 0;
}

int net_clt_auth_info_assign(char* pszAccountLoginName,char* pszPasswd,CSPKG *pstPkg,int iCltVersion)
{
	char szMd5[16];


	Md5HashBuffer(szMd5, pszPasswd, strlen(pszPasswd));	
	Md5HashBuffer(szMd5, szMd5, 16);

	
	STRNCPY( g_stNetCltAuthInfo.AccountLoginName, pszAccountLoginName, 
			 	sizeof(g_stNetCltAuthInfo.AccountLoginName));
	memcpy(g_stNetCltAuthInfo.szMd5,szMd5,sizeof(szMd5));	
	memcpy(&g_stNetCltAuthInfo.stAuthClt,&pstPkg->Body.AuthClt,sizeof(CSAUTHCLT));
	g_stNetCltAuthInfo.iCltVer = iCltVersion;
	g_stNetCltAuthInfo.szWebSign[0] = 0;

	return 0;
}

int net_auth_send(NETCLTAUTHHANDLE *pstNetCltAuthHandle,CSPKG *pstPkg,int iSocket,int iMs)
{
	TDRDATA stHost, stNet;
	
	stNet.iBuff = pstNetCltAuthHandle->iSendBuff;
	stNet.pszBuff= pstNetCltAuthHandle->pszSendBuff;
	stHost.iBuff = sizeof(*pstPkg);
	stHost.pszBuff = (char *)pstPkg;
	if (0 > tdr_hton(pstNetCltAuthHandle->pstSendMeta, &stNet, &stHost, 0))
	{
		return AUTH_ERR_SYS;
	}
		
	if (0 > tnet_sendall(iSocket, stNet.pszBuff, stNet.iBuff, iMs))
	{
		return AUTH_ERR_NET;
	}
	
	return 0;
}

int net_auth_recv(NETCLTAUTHHANDLE *pstNetCltAuthHandle,CSPKG *pstPkg,int iSocket,
					int iMs,char * szMd5,char *RandK1,char *pszDecrypt,int* piDecLen)
{
	TDRDATA stHost, stNet;
	int iLen;
	int iLen2;
	char *pszEnc;
	int iEnc;

	iLen	=	tnet_recv(iSocket, pstNetCltAuthHandle->pszRecvBuff, pstNetCltAuthHandle->iRecvBuff, iMs);
	if (iLen < 0)
	{
		return AUTH_ERR_NET;
	}
	
	stNet.iBuff = iLen;
	stNet.pszBuff= pstNetCltAuthHandle->pszRecvBuff;
	stHost.iBuff = sizeof(*pstPkg);
	stHost.pszBuff = (char *)pstPkg;
	if (0 > tdr_ntoh(pstNetCltAuthHandle->pstRecvMeta, &stHost, &stNet, 0))
	{
		return AUTH_ERR_SYS;
	}

	if (pstPkg->Head.Cmd != AUTH_SVR)
	{
		return AUTH_ERR_SYS;
	}

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szMd5) ||
		0 > taes_setkey(&pstNetCltAuthHandle->stAesK1, (unsigned char *)RandK1))
	{
		return AUTH_ERR_SYS;
	}

	pszEnc = pstPkg->Body.AuthSvr.Auth;
	iEnc = pstPkg->Body.AuthSvr.AuthLen;
	iLen = *piDecLen;
	iLen2 = *piDecLen;
	if (0 > taes_decrypt(pszEnc, iEnc, &pstNetCltAuthHandle->stAesGameKey, pszDecrypt, &iLen) &&
		0 > taes_decrypt(pszEnc, iEnc, &pstNetCltAuthHandle->stAesK1, pszDecrypt, &iLen2))
	{
		return AUTH_ERR_SYS;
	}

	*piDecLen = iLen;
	
	return 0;
}



int net_clt_auth_han_create_c_in(char *pstLibC, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd, int iCltVersion,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,
								char *MacAddr,
								char *CertID,
								char *RealName,
								char *eMail,
								char *Mobile,
								int iCltType,
								int iAuthFlag,
								char* pszBirthday,
								int iWebUin,
								char* pszIP)
{
	LPTDRMETALIB pstCSProtoLib;
	//int iSocket;
	
	CSPKG *pstPkg2 = NULL;
	NETCLTAUTHHANDLE *pstNetCltAuthHandle = NULL;

	int iLen;
	int iRet = 0;
	char cResult;
	char szDecrypt[AUTH_LEN];
	int iNameLen;
	int iOffset;
	RSA *pstRsa = NULL;

	pstPkg2 = malloc(sizeof(CSPKG));
	if (NULL == pstPkg2)
	{
		return AUTH_ERR_SYS;
	}
	
	memset(&g_stNetCltAuthInfo, 0, sizeof(g_stNetCltAuthInfo));
	g_stNetCltAuthInfo.iSocket  = -1;
	
	pstCSProtoLib = (LPTDRMETALIB)pstLibC;

	iRet = net_clt_auth_han_create_init(pstCSProtoLib,&pstNetCltAuthHandle);
	if (iRet < 0)
	{
		goto _error;
	}

	pstRsa = rsa_from_public_key(IDRSAPublicKey);

	if (pszBirthday && iWebUin && pszIP)
	{
		iAuthFlag |= AUTH_FLAG_WEB_SIGN;
		if (0 > net_clt_auth_msg_create(pstNetCltAuthHandle->pstAuthMeta,
										pstRsa,
									pszAccountLoginName,
									pszPasswd,
									pszBirthday, // as birthday
									iWebUin, // as webuin
									inet_addr(pszIP),// as IP
									CertID,
									RealName,
									eMail,
									Mobile,
									iCltType,
									iAuthFlag,
									pstPkg2,
									NULL))
		{
			iRet = AUTH_ERR_SYS;
			goto _error;
		}
	}
	else
	{
		if (0 > net_clt_auth_msg_create(pstNetCltAuthHandle->pstAuthMeta,
										pstRsa,
										pszAccountLoginName,
										pszPasswd,	
										"",
										0,
										0,
										CertID,
										RealName,
										eMail,
										Mobile,
										iCltType,
										iAuthFlag,
										pstPkg2,
										NULL))
		{
			iRet = AUTH_ERR_SYS;
			goto _error;
		}
	}

	if (pstRsa)
	{
		rsa_free(pstRsa);
		pstRsa = NULL;
	}
	
	net_clt_auth_info_assign(pszAccountLoginName,pszPasswd,pstPkg2,iCltVersion);
	
	
	iRet = net_clt_auth_conn(pszAuthConndIP,unAuthPort,&g_stNetCltAuthInfo.iSocket);

	if (-1 == g_stNetCltAuthInfo.iSocket || iRet < 0)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}
	iRet = net_auth_send(pstNetCltAuthHandle,pstPkg2,g_stNetCltAuthInfo.iSocket,2000);

	if (0 > iRet)
	{
		goto _error;
	}

	iLen = sizeof(szDecrypt);
	iRet = net_auth_recv(pstNetCltAuthHandle, pstPkg2,g_stNetCltAuthInfo.iSocket,6000,
				g_stNetCltAuthInfo.szMd5, g_stNetCltAuthInfo.stAuthClt.RandK1,
				szDecrypt,&iLen);

	if (iRet < 0)
	{
		goto _error;
	}

	//tnet_close(iSocket);

	cResult = szDecrypt[1];
	switch(cResult)
	{
	case 0:
	case AUTH_IFM_ACCOUNT_REGISTER_SUCCES:
		memset(&g_stNetCltAuthInfo.stAuthClt,0,sizeof(g_stNetCltAuthInfo.stAuthClt));
		memset(g_stNetCltAuthInfo.szMd5,0,sizeof(g_stNetCltAuthInfo.szMd5));
		memset(g_stNetCltAuthInfo.szWebSign,0,sizeof(g_stNetCltAuthInfo.szWebSign));
		break;
	case AUTH_IFM_NEED_REGIST_CODE_ERROR:
		{
			/* 激活码处理流程 */
			STRNCPY( g_stNetCltAuthInfo.AccountLoginName, pszAccountLoginName, 
				 	sizeof(g_stNetCltAuthInfo.AccountLoginName));
			iRet = cResult;
			goto _error;
			break;
		}
	case AUTH_IFM_WEB_SIGN_SUCC:
		{
			int iHexLen;
			char *p = NULL;
			memset(g_stNetCltAuthInfo.szWebSign,0,sizeof(g_stNetCltAuthInfo.szWebSign));
			
			if (pstPkg2->Body.AuthSvr.AuthLen <= 2)
			{
				iRet = AUTH_ERR_SYS;
				goto _error;
			}

			iHexLen = sizeof(g_stNetCltAuthInfo.szWebSign);
			
			p = Bin2Hex((unsigned char*)(szDecrypt + 2),iLen - 2,
						g_stNetCltAuthInfo.szWebSign, &iHexLen);
			
			iRet = cResult;
			if (!p)
			{
				iRet = AUTH_ERR_SYS;
			}
			goto _error;
			break;
		}
	case AUTH_REASON_NEED_PASSWD_CARD_ERROR:
		/* 密保卡处理流程 */
		g_stNetCltAuthInfo.stPasswdCardAuthSvr = pstPkg2->Body.AuthSvr.PasswdCardAuthSvr;
		iRet = cResult;
				
		goto _error;
		break;
			
	default:
		memset(g_stNetCltAuthInfo.szWebSign,0,sizeof(g_stNetCltAuthInfo.szWebSign));
		memset(&g_stNetCltAuthInfo.stAuthClt,0,sizeof(g_stNetCltAuthInfo.stAuthClt));
		memset(g_stNetCltAuthInfo.szMd5,0,sizeof(g_stNetCltAuthInfo.szMd5));
		iRet = cResult;
		goto _error;
		break;
	}

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szDecrypt+2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	iOffset = 2;

	memcpy(&iNameLen,szDecrypt+iOffset, sizeof(int));
	
	iNameLen = NTOHL(iNameLen);

	if (0 > iNameLen)
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}
	
	iOffset += sizeof(int);
	
	iOffset += iNameLen;

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szDecrypt+iOffset))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	iOffset += KEY_LEN;

	

	memcpy(&pstNetCltAuthHandle->iUin, szDecrypt+iOffset, sizeof(int));
	pstNetCltAuthHandle->iUin = NTOHL(pstNetCltAuthHandle->iUin);

	iOffset += sizeof(int);

	iRet = net_clt_acc_login(pszZoneConnIP, unZoneConnPort, pszAccountLoginName, iCltVersion,
							pstNetCltAuthHandle, iLen - iOffset, 
							szDecrypt+iOffset,MacAddr,iCltType,iAuthFlag);
	if (0 > iRet)
	{
		goto _error;
	}
	
	*ppNetCltAuthHandle = pstNetCltAuthHandle;

	/*
	if (pstRsa)
	{
		rsa_free(pstRsa);
	}*/
	
	if (pstPkg2)
	{
		free(pstPkg2);
		pstPkg2 = NULL;
	}

	tnet_close(g_stNetCltAuthInfo.iSocket);

	return cResult;

_error:
	if (pstPkg2)
	{
		free(pstPkg2);
		pstPkg2 = NULL;
	}
	

	if (pstRsa)
	{
		rsa_free(pstRsa);
		pstRsa = NULL;
	}
	
	if (iRet == 0)
	{
		iRet = cResult;
	}
	
	if (pstNetCltAuthHandle)
	{
		net_clt_han_auth_destroy(&pstNetCltAuthHandle);
	}
	
	return iRet;
}

int net_clt_auth_han_create_c(char *pstLibC, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd, int iCltVersion,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,
								char *MacAddr,
								char *CertID,
								char *RealName,
								char *eMail,
								char *Mobile,
								int iCltType,
								int iAuthFlag)
{
	return net_clt_auth_han_create_c_in(pstLibC,pszAuthConndIP,unAuthPort,pszZoneConnIP,
						unZoneConnPort,pszAccountLoginName, pszPasswd,iCltVersion,
						ppNetCltAuthHandle,MacAddr,CertID,RealName,eMail,Mobile,
						iCltType,iAuthFlag,NULL,0,NULL);
}

int net_clt_auth_han_create(char *pszMetaPath, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char *pszAccountLoginName, char *pszPasswd, int iCltVersion,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,
								char *MacAddr,
								char *CertID,
								char *RealName,
								char *eMail,
								char *Mobile,
								int iCltType,
								int iAuthFlag)
{
	int iRet;
	LPTDRMETALIB pstCSProtoLib;

	srand(time(NULL));

	pstCSProtoLib = meta_file_lib(pszMetaPath);
	if (NULL == pstCSProtoLib)
	{
		return AUTH_ERR_SYS;
	}

	iRet = net_clt_auth_han_create_c((char *)pstCSProtoLib, pszAuthConndIP, unAuthPort, pszZoneConnIP,
									unZoneConnPort, pszAccountLoginName, pszPasswd, iCltVersion, ppNetCltAuthHandle,
									MacAddr,CertID,RealName,eMail,Mobile,iCltType,iAuthFlag);
	//微端获取web签名成功
	if (iRet == AUTH_IFM_WEB_SIGN_SUCC && iCltType == CLT_PC)
	{
		iAuthFlag |= AUTH_FLAG_PLUGIN_SIGN;
		iRet = net_clt_auth_han_create_web_sign(pszMetaPath, pszAuthConndIP,
									unAuthPort, pszZoneConnIP,
									unZoneConnPort, 
									g_stNetCltAuthInfo.szWebSign,
									iCltVersion, ppNetCltAuthHandle,									
									iCltType,iAuthFlag);
	}
	
	if (0 != iRet)
	{
		free(pstCSProtoLib);
		return iRet;
	}

	if (*ppNetCltAuthHandle)
	{
		(*ppNetCltAuthHandle)->pstLib = (char *)pstCSProtoLib;
	}

	return iRet;
}

int net_clt_auth_han_active_c(char *pstLibC, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char* szActiveCode,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,
								char *MacAddr, int iCltType,PASSWDCARDAUTHINFO *pstPasswdCard)
{
	LPTDRMETALIB pstCSProtoLib;
	LPTDRMETA pstAuthMeta;
	int iSendBuff;
	int iRecvBuff;
	int iSocket;
	//AUTHINFO stAuthInfo;
	TDRDATA stHost, stNet;
	CSPKG *pstPkg2 = NULL;
	CSAUTHCLT *pstAuthClt;
	NETCLTAUTHHANDLE *pstNetCltAuthHandle = NULL;
	char szUri[128];//, szAuth[128];
	char RandK1[KEY_LEN];  
	int iLen;
	int iLen2;
	int iRet = 0;
	char szMd5[16];
	char *pszEnc;
	int iEnc;
	char cResult;
	char szDecrypt[AUTH_LEN];
	int iNameLen;
	int iOffset;

	pstPkg2 = malloc(sizeof(CSPKG));
	if (NULL == pstPkg2)
	{
		return AUTH_ERR_SYS;
	}

	pstAuthClt = &pstPkg2->Body.AuthClt;

	//memset(&g_stNetCltAuthInfo, 0, sizeof(g_stNetCltAuthInfo));
	memset(pstAuthClt,0,sizeof(CSAUTHCLT));
	
	pstCSProtoLib = (LPTDRMETALIB)pstLibC;
	
	pstNetCltAuthHandle = (NETCLTAUTHHANDLE *)malloc(sizeof(NETCLTAUTHHANDLE));
	if (!pstNetCltAuthHandle)
	{
		return AUTH_ERR_SYS;
	}

	memset(pstNetCltAuthHandle, 0, sizeof(*pstNetCltAuthHandle));
	pstNetCltAuthHandle->s = -1;
	pstNetCltAuthHandle->pstMetaBase = tdr_get_meta_by_name(pstCSProtoLib, "PDUBase");
	pstNetCltAuthHandle->pstMetaHead = tdr_get_meta_by_name(pstCSProtoLib, "PDUHead");
	pstNetCltAuthHandle->pstRecvMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstNetCltAuthHandle->pstSendMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstAuthMeta = tdr_get_meta_by_name(pstCSProtoLib, "AuthInfo");

	if (!(pstNetCltAuthHandle->pstMetaBase && pstNetCltAuthHandle->pstMetaHead &&
		pstNetCltAuthHandle->pstRecvMeta && pstNetCltAuthHandle->pstSendMeta && pstAuthMeta))
	{
		return AUTH_ERR_SYS;
	}

	iSendBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstSendMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;
	iRecvBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstRecvMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;

	pstNetCltAuthHandle->iSendBuff2 = pstNetCltAuthHandle->iSendBuff = iSendBuff;
	pstNetCltAuthHandle->iRecvBuff2 = pstNetCltAuthHandle->iRecvBuff = iRecvBuff;

	pstNetCltAuthHandle->pszRecvBuff = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszRecvBuff2 = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszSendBuff = (char*)calloc(1, iSendBuff);
	pstNetCltAuthHandle->pszSendBuff2 = (char*)calloc(1, iSendBuff);

	if (!(pstNetCltAuthHandle->pszRecvBuff && pstNetCltAuthHandle->pszRecvBuff2 &&
		pstNetCltAuthHandle->pszSendBuff && pstNetCltAuthHandle->pszSendBuff2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	//Md5HashBuffer(szMd5, pszPasswd, strlen(pszPasswd));
	//Md5HashBuffer(szMd5, szMd5, 16);	
	//tsocket_init(2,0);

	pstPkg2->Head.Cmd = AUTH_CLT;
	pstPkg2->Head.Magic = PROTOCOL_MAGIC;
	pstPkg2->Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	/*rand_str(RandK1, sizeof(RandK1));
	
	memcpy(pstAuthClt->RandK1, RandK1, sizeof(pstAuthClt->RandK1));
	memcpy(stAuthInfo.RandK1, RandK1, sizeof(pstAuthClt->RandK1));
	STRNCPY(stAuthInfo.AccountLoginName, pszAccountLoginName, sizeof(stAuthInfo.AccountLoginName));
	STRNCPY(stAuthInfo.AccountLoginPasswd, pszPasswd, sizeof(stAuthInfo.AccountLoginPasswd));

	stNet.pszBuff = szAuth;
	stNet.iBuff = sizeof(szAuth);
	stHost.pszBuff = (char *)&stAuthInfo;
	stHost.iBuff = sizeof(stAuthInfo);
	if (0 > tdr_hton(pstAuthMeta, &stNet, &stHost, 0) || stNet.iBuff > (int)sizeof(pstAuthClt->Enc))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	if (0 > rsa_public_encrypt(pstRsa, stNet.iBuff, stNet.pszBuff, pstAuthClt->Enc))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}*/

	

	memcpy(RandK1, g_stNetCltAuthInfo.stAuthClt.RandK1,sizeof(RandK1));
	memcpy(szMd5,g_stNetCltAuthInfo.szMd5,sizeof(szMd5));
	memcpy(pstAuthClt,&g_stNetCltAuthInfo.stAuthClt,sizeof(CSAUTHCLT));
	//stPkg =  g_stNetCltAuthInfo.stPkg;
	pstAuthClt->ActiveFlag = ACTIVE_FLAG_ACTIVE_CODE;
	snprintf(pstAuthClt->ActiveCode,sizeof(pstAuthClt->ActiveCode),"%s",szActiveCode);   

	if (pstPasswdCard)
	{
		pstAuthClt->PasswdCardAuthInfo = *pstPasswdCard;
		pstAuthClt->AuthFlag |= AUTH_FLAG_PASSWD_CARD;
	}
	
	
	stNet.iBuff = pstNetCltAuthHandle->iSendBuff;
	stNet.pszBuff= pstNetCltAuthHandle->pszSendBuff;
	stHost.iBuff = sizeof(*pstPkg2);
	stHost.pszBuff = (char *)pstPkg2;
	if (0 > tdr_hton(pstNetCltAuthHandle->pstSendMeta, &stNet, &stHost, 0))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	
	if (-1 == g_stNetCltAuthInfo.iSocket)
	{
		snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pszAuthConndIP, unAuthPort);
		iSocket	=	tnet_connect2(szUri, 4000, 20000, 30000);
		
		if (-1 != iSocket)
		{
			tnet_set_nonagle(iSocket);
			tnet_set_sendbuff(iSocket, 20480);
			tnet_set_recvbuff(iSocket, 204800);
		}
	}
	else
	{
		iSocket = g_stNetCltAuthInfo.iSocket;		
	}	

	if (-1 == iSocket)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}
	else
	{
		g_stNetCltAuthInfo.iSocket = iSocket;
	}

	if (0 > tnet_sendall(g_stNetCltAuthInfo.iSocket, stNet.pszBuff, stNet.iBuff, 1000))
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	iLen	=	tnet_recv(g_stNetCltAuthInfo.iSocket, pstNetCltAuthHandle->pszRecvBuff, pstNetCltAuthHandle->iRecvBuff, 10000);
	if (iLen < 0)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	//tnet_close(g_stNetCltAuthInfo.iSocket);
	
	stNet.iBuff = iLen;
	stNet.pszBuff= pstNetCltAuthHandle->pszRecvBuff;
	stHost.iBuff = sizeof(*pstPkg2);
	stHost.pszBuff = (char *)pstPkg2;
	if (0 > tdr_ntoh(pstNetCltAuthHandle->pstRecvMeta, &stHost, &stNet, 0))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	if (pstPkg2->Head.Cmd != AUTH_SVR)
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szMd5) ||
		0 > taes_setkey(&pstNetCltAuthHandle->stAesK1, (unsigned char *)RandK1))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	pszEnc = pstPkg2->Body.AuthSvr.Auth;
	iEnc = pstPkg2->Body.AuthSvr.AuthLen;
	iLen = sizeof(szDecrypt);
	iLen2 = iLen;
	if (0 > taes_decrypt(pszEnc, iEnc, &pstNetCltAuthHandle->stAesGameKey, szDecrypt, &iLen) &&
		0 > taes_decrypt(pszEnc, iEnc, &pstNetCltAuthHandle->stAesK1, szDecrypt, &iLen2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	cResult = szDecrypt[1];
	switch(cResult)
	{
	case AUTH_IFM_ACCOUNT_REGISTER_SUCCES:
	case 0:
		break;
		
	case AUTH_REASON_NEED_PASSWD_CARD_ERROR:
		/* 密保卡处理流程 */
		g_stNetCltAuthInfo.stPasswdCardAuthSvr = pstPkg2->Body.AuthSvr.PasswdCardAuthSvr;
		iRet = cResult;
		goto _error;
		break;

	default:
		iRet = cResult;
		goto _error;
		break;
	}

	iOffset = 2;

	memcpy(&iNameLen,szDecrypt+iOffset, sizeof(int));
	
	iNameLen = NTOHL(iNameLen);

	if (0 > iNameLen)
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}
	
	iOffset += sizeof(int);
	
	iOffset += iNameLen;

	if (0 > taes_setkey(&pstNetCltAuthHandle->stAesGameKey, (unsigned char *)szDecrypt+iOffset))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}

	iOffset += KEY_LEN;	

	memcpy(&pstNetCltAuthHandle->iUin, szDecrypt+iOffset, sizeof(int));
	pstNetCltAuthHandle->iUin = NTOHL(pstNetCltAuthHandle->iUin);

	iOffset += sizeof(int);

	iRet = net_clt_acc_login(pszZoneConnIP, unZoneConnPort, g_stNetCltAuthInfo.AccountLoginName, g_stNetCltAuthInfo.iCltVer,
							pstNetCltAuthHandle, iLen - iOffset, 
							szDecrypt+iOffset,MacAddr,iCltType,0);
	if (0 > iRet)
	{
		goto _error;
	}
	
	*ppNetCltAuthHandle = pstNetCltAuthHandle;

	tnet_close(g_stNetCltAuthInfo.iSocket);
	g_stNetCltAuthInfo.iSocket = -1;

	memset(&g_stNetCltAuthInfo.stAuthClt,0,sizeof(g_stNetCltAuthInfo.stAuthClt));
	memset(g_stNetCltAuthInfo.szMd5,0,sizeof(g_stNetCltAuthInfo.szMd5));

	if(pstPkg2)
	{
		free(pstPkg2);
		pstPkg2 = NULL;
	}

	return cResult;

_error:
	if(pstPkg2)
	{
		free(pstPkg2);
		pstPkg2 = NULL;
	}

	
	net_clt_han_auth_destroy(&pstNetCltAuthHandle);
	
	if (iRet != AUTH_REASON_NEED_PASSWD_CARD_ERROR)
	{
		tnet_close(g_stNetCltAuthInfo.iSocket);
		memset(&g_stNetCltAuthInfo,0,sizeof(g_stNetCltAuthInfo));

		g_stNetCltAuthInfo.iSocket = -1;
		
	}
	return iRet;
	UNUSED(pszAuthConndIP);	//add by paraunused.pl
	UNUSED(unAuthPort);	//add by paraunused.pl
}

int net_clt_auth_heart_beat(char *pstLibC)
{
	LPTDRMETALIB pstCSProtoLib;
	LPTDRMETA pstAuthMeta;
	int iSendBuff;
	int iRecvBuff;

	TDRDATA stHost, stNet;
	CSPKG *pstPkg2 = NULL;
	CSHEARTBEATREQ *pstHeartBeatReq = NULL;

	int iRet = 0;

	NETCLTAUTHHANDLE *pstNetCltAuthHandle = NULL;

	pstCSProtoLib = (LPTDRMETALIB)pstLibC;

	pstPkg2 = malloc(sizeof(CSPKG));
	if (NULL == pstPkg2)
	{
		return AUTH_ERR_SYS;
	}

	pstHeartBeatReq = &pstPkg2->Body.HeartBeatReq;

	memset(pstHeartBeatReq,0,sizeof(*pstHeartBeatReq));
	

	
	pstNetCltAuthHandle = (NETCLTAUTHHANDLE *)malloc(sizeof(NETCLTAUTHHANDLE));
	if (!pstNetCltAuthHandle)
	{
		return AUTH_ERR_SYS;
	}

	memset(pstNetCltAuthHandle, 0, sizeof(*pstNetCltAuthHandle));
	pstNetCltAuthHandle->s = -1;
	pstNetCltAuthHandle->pstMetaBase = tdr_get_meta_by_name(pstCSProtoLib, "PDUBase");
	pstNetCltAuthHandle->pstMetaHead = tdr_get_meta_by_name(pstCSProtoLib, "PDUHead");
	pstNetCltAuthHandle->pstRecvMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstNetCltAuthHandle->pstSendMeta = tdr_get_meta_by_name(pstCSProtoLib, "CSPkg");
	pstAuthMeta = tdr_get_meta_by_name(pstCSProtoLib, "AuthInfo");

	if (!(pstNetCltAuthHandle->pstMetaBase && pstNetCltAuthHandle->pstMetaHead &&
		pstNetCltAuthHandle->pstRecvMeta && pstNetCltAuthHandle->pstSendMeta && pstAuthMeta))
	{
		return AUTH_ERR_SYS;
	}

	iSendBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstSendMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;
	iRecvBuff	=	tdr_get_meta_size(pstNetCltAuthHandle->pstRecvMeta) + tdr_get_meta_size(pstNetCltAuthHandle->pstMetaHead) + 32;

	pstNetCltAuthHandle->iSendBuff2 = pstNetCltAuthHandle->iSendBuff = iSendBuff;
	pstNetCltAuthHandle->iRecvBuff2 = pstNetCltAuthHandle->iRecvBuff = iRecvBuff;

	pstNetCltAuthHandle->pszRecvBuff = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszRecvBuff2 = (char*)calloc(1, iRecvBuff);
	pstNetCltAuthHandle->pszSendBuff = (char*)calloc(1, iSendBuff);
	pstNetCltAuthHandle->pszSendBuff2 = (char*)calloc(1, iSendBuff);

	if (!(pstNetCltAuthHandle->pszRecvBuff && pstNetCltAuthHandle->pszRecvBuff2 &&
		pstNetCltAuthHandle->pszSendBuff && pstNetCltAuthHandle->pszSendBuff2))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}


	pstPkg2->Head.Cmd = HEARTBEAT_REQ;
	pstPkg2->Head.Magic = PROTOCOL_MAGIC;
	pstPkg2->Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	
	stNet.iBuff = pstNetCltAuthHandle->iSendBuff;
	stNet.pszBuff= pstNetCltAuthHandle->pszSendBuff;
	stHost.iBuff = sizeof(*pstPkg2);
	stHost.pszBuff = (char *)pstPkg2;
	if (0 > tdr_hton(pstNetCltAuthHandle->pstSendMeta, &stNet, &stHost, 0))
	{
		iRet = AUTH_ERR_SYS;
		goto _error;
	}


	if (-1 == g_stNetCltAuthInfo.iSocket)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}


	if (0 > tnet_sendall(g_stNetCltAuthInfo.iSocket, stNet.pszBuff, stNet.iBuff, 1000))
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	//iLen	=	tnet_recv(g_stNetCltAuthInfo.iSocket, pstNetCltAuthHandle->pszRecvBuff, pstNetCltAuthHandle->iRecvBuff, 10000);


	if(pstPkg2)
	{
		free(pstPkg2);
	}


	return iRet;

_error:
	if(pstPkg2)
	{
		free(pstPkg2);
	}
	
	net_clt_han_auth_destroy(&pstNetCltAuthHandle);
	
	if (iRet != AUTH_REASON_NEED_PASSWD_CARD_ERROR)
	{
		tnet_close(g_stNetCltAuthInfo.iSocket);
		memset(&g_stNetCltAuthInfo,0,sizeof(g_stNetCltAuthInfo));
		g_stNetCltAuthInfo.iSocket = -1;

		
	}

	
	return iRet;

}

int net_clt_auth_heart_beat_web_sign(NETCLTAUTHHANDLE *pstNetCltAuthHandle,
								CSPKG *pstPkg,int iSocket)
{

	CSHEARTBEATREQ *pstHeartBeatReq = NULL;

	int iRet = 0;

	if (NULL == pstPkg)
	{
		return AUTH_ERR_SYS;
	}

	if (-1 == iSocket)
	{
		iRet = AUTH_ERR_NET;
		goto _error;
	}

	pstHeartBeatReq = &pstPkg->Body.HeartBeatReq;

	memset(pstHeartBeatReq,0,sizeof(*pstHeartBeatReq));

	//net_clt_auth_han_create_in(pstCSProtoLib, &pstNetCltAuthHandle);

	pstPkg->Head.Cmd = HEARTBEAT_REQ;
	pstPkg->Head.Magic = PROTOCOL_MAGIC;
	pstPkg->Head.Ver = TDR_METALIB_PROTO_CS_VERSION;	

	iRet = net_auth_send(pstNetCltAuthHandle,pstPkg,iSocket,1000);


	return iRet;

_error:

	
	return iRet;

}


/*  这个函数是输入激活码后调用的,用户密码被零时记录在内存 
 *  所有不要用户名和密码
 *  szActiveCode激活码
 */
int net_clt_auth_han_active(char *pszMetaPath, char *pszAuthConndIP,  unsigned short unAuthPort,
								char *pszZoneConnIP, unsigned short unZoneConnPort,
								char* szActiveCode,
								LPNETCLTAUTHHANDLE *ppNetCltAuthHandle,
								char *MacAddr, int iCltType,PASSWDCARDAUTHINFO *pstPasswdCard)
{
	int iRet;
	LPTDRMETALIB pstCSProtoLib;

	pstCSProtoLib = meta_file_lib(pszMetaPath);
	if (NULL == pstCSProtoLib)
	{
		return AUTH_ERR_SYS;
	}

	iRet = net_clt_auth_han_active_c((char *)pstCSProtoLib, pszAuthConndIP, unAuthPort, pszZoneConnIP,
									unZoneConnPort, 
									szActiveCode,ppNetCltAuthHandle,
									MacAddr, iCltType,pstPasswdCard);
	if (0 != iRet)
	{
		free(pstCSProtoLib);
		return iRet;
	}

	if (*ppNetCltAuthHandle)
	{
		(*ppNetCltAuthHandle)->pstLib = (char *)pstCSProtoLib;
	}

	return iRet;
}


int net_clt_auth_get_uin(NETCLTAUTHHANDLE *pNetCltAuthHandle)
{
	return pNetCltAuthHandle->iUin;
}

int auth_get_msg(NETCLTAUTHHANDLE *pNetCltAuthHandle, PDUHEAD* pstHead, char* pszMsg, int* piMsg)
{
	TDRDATA stHost;
	TDRDATA stNet;
	int iHead;
	int iBody;
	char* pszEnc;
	int iEnc;
	char* pszDec;
	int iDec;
	int iRet=0;
	CSPKG *pstPkg;

	if( pNetCltAuthHandle->iData<(int)sizeof(PDUBASE) )
	{
		iHead	=	(int) sizeof(PDUBASE);
		iBody	=	0;
	}
	else
	{
		iHead	=	((PDUBASE*)(pNetCltAuthHandle->pszRecvBuff + pNetCltAuthHandle->iOff))->HeadLen;
		iBody	=	((PDUBASE*)(pNetCltAuthHandle->pszRecvBuff + pNetCltAuthHandle->iOff))->BodyLen;

		iBody	=	NTOHL((long)iBody);
	}

	if( pNetCltAuthHandle->iData<iHead+iBody )
	{
		if( pNetCltAuthHandle->iOff )
		{
			if( pNetCltAuthHandle->iData )
				memmove(pNetCltAuthHandle->pszRecvBuff, pNetCltAuthHandle->pszRecvBuff+pNetCltAuthHandle->iOff, pNetCltAuthHandle->iData);
		
			pNetCltAuthHandle->iOff	=	0;
		}
		
             return 0;
	}

	stHost.pszBuff	=	(char*)pstHead;
	stHost.iBuff	=	(int)sizeof(*pstHead);

	stNet.pszBuff	=	pNetCltAuthHandle->pszRecvBuff + pNetCltAuthHandle->iOff;
	stNet.iBuff	=	iHead;

	iRet=tdr_ntoh(pNetCltAuthHandle->pstMetaHead, &stHost, &stNet, PDU_VERSION);

	if( iRet<0 )
	{
		pNetCltAuthHandle->iErr = AUTH_ERR_TDR;
		pNetCltAuthHandle->iTdrErrorCode = iRet;

		pNetCltAuthHandle->iOff	+=	(iHead + iBody);
		pNetCltAuthHandle->iData -=	(iHead + iBody);

		return iRet;
	}

	if( iBody )
	{
		stHost.pszBuff	=	pszMsg;
		stHost.iBuff	=	*(piMsg);

		if (pstHead->Base.Cmd == PDU_CMD_PLAIN)
		{
			stNet.pszBuff	=	pNetCltAuthHandle->pszRecvBuff + pNetCltAuthHandle->iOff + iHead;
			stNet.iBuff	=	iBody;

			pNetCltAuthHandle->iOff	+=	(iHead + iBody);
			pNetCltAuthHandle->iData -=	(iHead + iBody);
		}
		else
		{
			pszEnc	=	pNetCltAuthHandle->pszRecvBuff + pNetCltAuthHandle->iOff + iHead;	
			iEnc	=	iBody;
			pszDec	=	pNetCltAuthHandle->pszRecvBuff2;
			iDec	=	pNetCltAuthHandle->iRecvBuff2;

			pNetCltAuthHandle->iOff	+=	(iHead + iBody);
			pNetCltAuthHandle->iData -=	(iHead + iBody);

			if (0 > taes_decrypt(pszEnc, iEnc, &pNetCltAuthHandle->stAesGameKey, pszDec, &iDec))
			{
				pNetCltAuthHandle->iErr = AUTH_ERR_SYS;
				pNetCltAuthHandle->iTdrErrorCode = 0;
				return -1;
			}
			else
			{
				stNet.pszBuff	=	pszDec;
				stNet.iBuff	=	iDec;
			}
		}

		iRet = tdr_ntoh(pNetCltAuthHandle->pstRecvMeta, &stHost, &stNet, pNetCltAuthHandle->iPeerVersion);
		if (iRet < 0)
		{
			pNetCltAuthHandle->iErr=AUTH_ERR_TDR;
			pNetCltAuthHandle->iTdrErrorCode = iRet;
			
			return iRet;
		}
	}
	else
	{
		stHost.iBuff	=	0;

		pNetCltAuthHandle->iOff	+=	(iHead + iBody);
		pNetCltAuthHandle->iData -=	(iHead + iBody);
	}

	*piMsg	=	stHost.iBuff;

	if (stHost.iBuff > 0)
	{
		pstPkg = (CSPKG *)pszMsg;

		if (ACC_LOGIN_RES == pstPkg->Head.Cmd && LOGIN_SUCC == pstPkg->Body.AccLoginRes.Result)
		{
			pNetCltAuthHandle->iPeerVersion = pstPkg->Body.AccLoginRes.SvrProtoVer;
			pNetCltAuthHandle->iSelfVersion = TDR_METALIB_PROTO_CS_VERSION;
		}
	}
	
	switch (pstHead->Base.Cmd)
	{
	case PDU_CMD_QUEINFO: 
		pstPkg = (CSPKG *)pszMsg;
		pstPkg->Head.Cmd = QUE_INFO;
		pstPkg->Body.QueInfo.Max = pstHead->Ext.QueInfo.Max;
		pstPkg->Body.QueInfo.Pos = pstHead->Ext.QueInfo.Pos;
		*piMsg = 20;
		break;

	case PDU_CMD_STOP:
		pstPkg = (CSPKG *)pszMsg;
				
		if (pstHead->Ext.Stop.StopReason == PDU_STOP_CONNECTION_REFUSED)
		{
			pstPkg->Head.Cmd = ACC_LOGIN_RES;
			pstPkg->Body.AccLoginRes.Result = LOGIN_QUEUE_FULL;
			pstPkg->Body.AccLoginRes.SvrProtoVer = TDR_METALIB_PROTO_CS_VERSION;
			*piMsg = 16;
		}
		else if (pstHead->Ext.Stop.StopReason == PDU_STOP_CONNECTION_SAMEACC_QUE)
		{
			pstPkg->Head.Cmd = KICKOFF_INFO;
			strcpy(pstPkg->Body.KickoffInfo.Msg, "相同帐号在别处登录，你的帐号被顶了");
			*piMsg = 12 + sizeof(pstPkg->Body.KickoffInfo.Msg);
		}
		else
		{
			pstPkg->Head.Cmd = ACC_LOGIN_RES;
			pstPkg->Body.AccLoginRes.Result = LOGIN_SVRBUSY;
			pstPkg->Body.AccLoginRes.SvrProtoVer = TDR_METALIB_PROTO_CS_VERSION;
			*piMsg = 16;
		}
				
		break;

	default:
		break;
	}

	if (*piMsg > 0)
		return 1;
	else
		return 0;
}


int net_clt_auth_recv(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, int iTimeout)
{
	PDUHEAD stHead;
	char* pszRecvBuff;
	int iRecvBuff;
	int iRecv;
	int iRet=0;
	int iMsg;

	iMsg = sizeof(*pstPkg);
	iRet = auth_get_msg(pNetCltAuthHandle, &stHead, (char *)pstPkg, &iMsg);

	if( 0!=iRet )
	{
		return iRet;
	}

	pszRecvBuff	=	pNetCltAuthHandle->pszRecvBuff + pNetCltAuthHandle->iOff + pNetCltAuthHandle->iData;
	iRecvBuff	=	pNetCltAuthHandle->iRecvBuff - pNetCltAuthHandle->iOff - pNetCltAuthHandle->iData;

	if( iRecvBuff<=0 )
	{
		pNetCltAuthHandle->iErr	=	AUTH_ERR_SYS;
		return -1;
	}

	iRecv	=	tnet_recv(pNetCltAuthHandle->s, pszRecvBuff, iRecvBuff, iTimeout);

	if( iRecv<0 )		
	{
		pNetCltAuthHandle->iErr	=	AUTH_ERR_NET;
		return -1;
	}
	else if( 0==iRecv )
	{
		return 0;
	}

	pNetCltAuthHandle->iData	+=	iRecv;

	iMsg = sizeof(*pstPkg);
	iRet = auth_get_msg(pNetCltAuthHandle, &stHead, (char *)pstPkg, &iMsg);

	return iRet;
}

int net_clt_auth_send(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, int iTimeout)
{
	pstPkg->Head.Ver =TDR_METALIB_PROTO_CS_VERSION;
		
	return auth_send_msg(pNetCltAuthHandle, NULL, (char *)pstPkg, sizeof(*pstPkg), iTimeout);
}

int net_clt_auth_sprintf(NETCLTAUTHHANDLE *pNetCltAuthHandle, CSPKG *pstPkg, char *pBuff, int iBuff)
{
	TDRDATA stHost;
	TDRDATA stOut;

	pstPkg->Head.Ver =TDR_METALIB_PROTO_CS_VERSION;

	stHost.pszBuff = (char *)pstPkg;
	stHost.iBuff = sizeof(CSPKG);
	stOut.pszBuff = pBuff;
	stOut.iBuff = iBuff;
	
	tdr_sprintf(pNetCltAuthHandle->pstRecvMeta, &stOut, &stHost, pNetCltAuthHandle->iPeerVersion);

	if (stOut.iBuff < iBuff)
	{
		stOut.pszBuff[stOut.iBuff] = '\0';
	}
	else
	{
		stOut.pszBuff[iBuff -1] = '\0';
	}	
	
	return 0;
}

int net_clt_han_auth_destroy(LPNETCLTAUTHHANDLE *ppNetCltAuthHandle)
{
	LPNETCLTAUTHHANDLE pstNetCltAuthHandle;

	if (NULL == ppNetCltAuthHandle || NULL ==  *ppNetCltAuthHandle)
	{
		return 0;
	}

	pstNetCltAuthHandle = *ppNetCltAuthHandle;

	if (pstNetCltAuthHandle->pszRecvBuff)
	{
		free(pstNetCltAuthHandle->pszRecvBuff);
		pstNetCltAuthHandle->pszRecvBuff = NULL;
		pstNetCltAuthHandle->iRecvBuff = 0;
	}

	if (pstNetCltAuthHandle->pszRecvBuff2)
	{
		free(pstNetCltAuthHandle->pszRecvBuff2);
		pstNetCltAuthHandle->pszRecvBuff2 = NULL;
		pstNetCltAuthHandle->iRecvBuff2 = 0;
	}

	if (pstNetCltAuthHandle->pszSendBuff)
	{
		free(pstNetCltAuthHandle->pszSendBuff);
		pstNetCltAuthHandle->pszSendBuff = NULL;
		pstNetCltAuthHandle->iSendBuff = 0;
	}

	if (pstNetCltAuthHandle->pszSendBuff2)
	{
		free(pstNetCltAuthHandle->pszSendBuff2);
		pstNetCltAuthHandle->pszSendBuff2 = NULL;
		pstNetCltAuthHandle->iSendBuff2 = 0;
	}

	if (-1 != pstNetCltAuthHandle->s)
	{
		tnet_close(pstNetCltAuthHandle->s);
		pstNetCltAuthHandle->s = -1;
	}

	if (NULL != pstNetCltAuthHandle->pstLib)
	{
		free(pstNetCltAuthHandle->pstLib);
	}

	free(pstNetCltAuthHandle);
	*ppNetCltAuthHandle = NULL;
	
	return 0;
}

int net_clt_sprintf(NETCLTHANDLE *pNetCltHandle, CSPKG *pstPkg, char *pBuff, int iBuff)
{
	TDRDATA stHost;
	TDRDATA stOut;

	pstPkg->Head.Ver =TDR_METALIB_PROTO_CS_VERSION;

	stHost.pszBuff = (char *)pstPkg;
	stHost.iBuff = sizeof(CSPKG);
	stOut.pszBuff = pBuff;
	stOut.iBuff = iBuff;

	tdr_sprintf(pNetCltHandle->pstNetHandle->pstCSProtoPkgMeta, &stOut, &stHost, pNetCltHandle->hApi->iPeerVersion);
	
	if (stOut.iBuff < iBuff)
	{
		stOut.pszBuff[stOut.iBuff] = '\0';
	}
	else
	{
		stOut.pszBuff[iBuff -1] = '\0';
	}		
	
	return 0;
}
int wear_pos_rep(ITEMDEF *pstItemDef1, ITEMDEF *pstItemDef2)
{
	char sMap[WEAR_POS_MAX+1];

	memset(sMap, 0, sizeof(sMap));

	if (pstItemDef1->WearPos1 != WEAR_POS_RING)
	{
		if (pstItemDef1->WearPos1 != WEAR_POS_NO)
		{
			sMap[pstItemDef1->WearPos1] = 1;
		}
		if (pstItemDef1->WearPos2 != WEAR_POS_NO)
		{
			sMap[pstItemDef1->WearPos2] = 1;
		}
		if (pstItemDef1->WearPos3 != WEAR_POS_NO)
		{
			sMap[pstItemDef1->WearPos3] = 1;
		}
	}

	if (pstItemDef2->WearPos1 != WEAR_POS_RING)
	{
		if (sMap[pstItemDef2->WearPos1] || sMap[pstItemDef2->WearPos2] || sMap[pstItemDef2->WearPos3])
		{
			return 1;
		}
	}

	return 0;
}
//伤害修正都加到武器上去，抵抗修正都加到被攻击的玩家身上去
int attr_result(CSDynAttrList *pstDyn, RESULT *pstResult)
{
	int i;
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstDyn->stAtkeeHurtChg;
	
	switch(pstResult->ResultID)
	{
		case RESULT_ICE_ATK:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].unIceBaseArmAtk += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].nIceAtkChgMul += pstResult->ResultVal2;
					pstDyn->astAtkChg[i].iIceAtkChgAdd += pstResult->ResultVal3;
				}
			}
			
			break;
		case RESULT_FIRE_ATK:
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].unFireBaseArmAtk += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].nFireAtkChgMul += pstResult->ResultVal2;
					pstDyn->astAtkChg[i].iFireAtkChgAdd += pstResult->ResultVal3;
				}
			}
			
			break;
		case RESULT_THUNDER_ATK:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].unThunderBaseArmAtk += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].nThunderAtkChgMul += pstResult->ResultVal2;
					pstDyn->astAtkChg[i].iThunderAtkChgAdd += pstResult->ResultVal3;
				}
			}
			
			break;
		case RESULT_LIGHT_ATK:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].unLightBaseArmAtk += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].nLightAtkChgMul += pstResult->ResultVal2;
					pstDyn->astAtkChg[i].iLightAtkChgAdd += pstResult->ResultVal3;
				}
			}
			
			break;	
		case RESULT_NIGHT_ATK:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].unNightBaseArmAtk += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].nNightAtkChgMul += pstResult->ResultVal2;
					pstDyn->astAtkChg[i].iNightAtkChgAdd += pstResult->ResultVal3;
				}
			}
			
			break;
		case RESULT_PHSIC_ATK:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].unPhsicBaseArmAtk += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].nPhsicAtkChgMul += pstResult->ResultVal2;
					pstDyn->astAtkChg[i].iPhsicAtkChgAdd += pstResult->ResultVal3;
				}
			}
			
			break;
		
		case RESULT_HITRATE:
			pstDyn->stHitRateChg.nAtkerHitRateMul += pstResult->ResultVal1;
			pstDyn->stHitRateChg.nAtkerHitRateAdd += pstResult->ResultVal2;
			break;
		case RESULT_FLEE:
			pstDyn->stFleeChg.unFleeBaseAdd += pstResult->ResultVal1;
			pstDyn->stFleeChg.nFleeChgMul += pstResult->ResultVal2;
			pstDyn->stFleeChg.iFleeChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_ICE_DEF:
			pstDyn->stDefChg.unIceDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nIceDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iIceDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_FIRE_DEF:
			pstDyn->stDefChg.unFireDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nFireDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iFireDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_THUNDER_DEF:
			pstDyn->stDefChg.unThunderDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nThunderDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iThunderDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_LIGHT_DEF:
			pstDyn->stDefChg.unLightDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nLightDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iLightDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_NIGHT_DEF:
			pstDyn->stDefChg.unNightDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nNightDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iNightDefChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_PHSIC_DEF:
			pstDyn->stDefChg.unPhsicDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nPhsicDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iPhsicDefChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_DEF_ICEHURT:
			pstAtkeeHurtChg->AtkeeIceHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeIceHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_FIREHURT:
			pstAtkeeHurtChg->AtkeeFireHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeFireHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_THUNDERHURT:
			pstAtkeeHurtChg->AtkeeThunderHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeThunderHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_LIGHTHURT:
			pstAtkeeHurtChg->AtkeeLightHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeLightHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_NIGHTHURT:
			pstAtkeeHurtChg->AtkeeNightHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeNightHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_PHISICHURT:
			pstAtkeeHurtChg->AtkeePhsicHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeePhsicHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_DEF_MAGICHURT:
			pstAtkeeHurtChg->AtkeeMagicHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeMagicHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_ICEHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerIceHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerIceHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;
		case RESULT_FIREHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerFireHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerFireHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;	
		case RESULT_THUNDERHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerThunderHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerThunderHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;	
		case RESULT_LIGHTHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerLightHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerLightHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;		
		case RESULT_NIGHTHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerNightHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerNightHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;	
		case RESULT_PHISICHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerPhsicHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerPhsicHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;	
		case RESULT_MAGICHURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerMagicHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerMagicHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;	
		case RESULT_HEAVY_HIT:
			pstDyn->stHitRateChg.nHeavyHitAdd += pstResult->ResultVal1;
			break;
		case RESULT_HURT:
			
			{
				for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); i++)
				{
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerHurtMul += pstResult->ResultVal1;
					pstDyn->astAtkChg[i].stAtkerHurtChg.AtkerHurtAdd += pstResult->ResultVal2;
				}
			}
			
			break;

		case RESULT_DEF_HURT:
			pstAtkeeHurtChg->AtkeeHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeHurtAdd += pstResult->ResultVal2;
			break;

		case RESULT_MON_FLEE:
			pstDyn->stMonChg.nFleeChgMul += pstResult->ResultVal1;
			pstDyn->stMonChg.unFleeChgAdd += pstResult->ResultVal2;
			break;

		case RESULT_MON_DEF_HURT:
			pstDyn->stMonChg.nAtkeeHurtMul += pstResult->ResultVal1;
			pstDyn->stMonChg.unAtkeeHurtAdd += pstResult->ResultVal2;
			break;	
			
		default:
			break;
	}

	return 0;
}

/*
void dyn_tmp(CSDynAttrList *pstDyn)
{
	CSDefChg *pstDefChg;
	
	//命中
	pstDyn->stHitChg.iHitTmp = (NEW_HIT*1.0 + pstDyn->stHitChg.unHitBaseAdd)*(1+pstDyn->stHitChg.nHitChgMul/100.0) + 
								pstDyn->stHitChg.iHitChgAdd;

	//闪避
	pstDyn->stFleeChg.iFleeTmp = (NEW_FLEE*1.0 + pstDyn->stFleeChg.unFleeBaseAdd);

	//防御
	pstDefChg = &pstDyn->stDefChg;
	pstDefChg->iDefIceTmp = (NEW_DEF + pstDefChg->unIceDefBaseAdd);
	pstDefChg->iDefFireTmp = (NEW_DEF + pstDefChg->unFireDefBaseAdd);
	pstDefChg->iDefThunderTmp = (NEW_DEF + pstDefChg->unThunderDefBaseAdd);
	pstDefChg->iDefLightTmp = (NEW_DEF + pstDefChg->unLightDefBaseAdd);
	pstDefChg->iDefNightTmp = (NEW_DEF + pstDefChg->unNightDefBaseAdd);
	pstDefChg->iDefPhsicTmp = (NEW_DEF + pstDefChg->unPhsicDefBaseAdd);
		
	return;
}
*/

int cacl_skill_result(SKILLRESULT *pstResult, CSDynAttrList *pstDyn, CSAtkChg *pstAtkChg)
{

	switch (pstResult->SkillResultID)
	{
		case RESULT_ICE_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unIceBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nIceAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iIceAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_FIRE_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unFireBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nFireAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iFireAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_THUNDER_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unThunderBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nThunderAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iThunderAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_LIGHT_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unLightBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nLightAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iLightAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;	
		case RESULT_NIGHT_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unNightBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nNightAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iNightAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_PHSIC_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unPhsicBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nPhsicAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iPhsicAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_DEC_ICEDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iIceDecDef += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DEC_FIREDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iFireDecDef += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DEC_THUNDERDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iThunderDecDef += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DEC_LIGHTDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iLightDecDef += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DEC_NIGHTDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iNightDecDef += pstResult->SkillResultVal1;
			}
			break;	
			
		case RESULT_ICEHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd += pstResult->SkillResultVal2;
			}
			break;
		case RESULT_FIREHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerFireHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerFireHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_THUNDERHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerThunderHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerThunderHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_LIGHTHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerLightHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerLightHurtAdd += pstResult->SkillResultVal2;
			}
			break;		
		case RESULT_NIGHTHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerNightHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerNightHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_PHISICHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_MAGICHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerMagicHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerMagicHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_HURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal2;
			}
			break;
	
		default:
			break;
	}

	return 0;
	UNUSED(pstDyn);	//add by paraunused.pl
}

int cacl_skill_attr(SKILLDEF *pstSkillDef, CSDynAttrList *pstDyn, CSAtkChg *pstAtkChg)
{
	int i;
	SKILLRESULT *pstResult;
		
	for (i=0; i<MAX_SKILL_RESULT; i++)
	{
		pstResult = pstSkillDef->SkillResult+i;
		if (RESULT_NONE == pstResult->SkillResultID)
		{
			break;
		}

		cacl_skill_result(pstResult, pstDyn, pstAtkChg);
	}

	return 0;
}

int is_weapon(ITEMDEF *pstItemDef)
{
	if (pstItemDef->NumType >= 100 && pstItemDef->NumType <= 159)
	{
		return 1;
	}

	return 0;
}

static int item_id_num(ITEMDEF *pstItemDef)
{
	int iType;
	int iTypeNum;
	int iAmmType;
	char szItemID[16];

	memset(szItemID, 0, sizeof(szItemID));
	sprintf(szItemID, "%u", pstItemDef->ItemID);

	iAmmType = szItemID[3] - '0';

	//取前3 位
	szItemID[3] = 0;
	iTypeNum = atoi(szItemID);

	pstItemDef->NumType = iTypeNum;
	pstItemDef->AmmType = 0;

	if (pstItemDef->ItemID >= ITEM_NUM_ARM_MIN && pstItemDef->ItemID <= ITEM_NUM_ARM_MAX)
	{
		iType =  ITEM_TYPE_ARM;
	}
	else if (pstItemDef->ItemID >= ITEM_NUM_ITEM_MIN && pstItemDef->ItemID <= ITEM_NUM_ITEM_MAX)
	{
		if (pstItemDef->ItemID >= ITEM_NUM_AMM_MIN && pstItemDef->ItemID <= ITEM_NUM_AMM_MAX)
		{
			iType = ITEM_TYPE_AMM;
			pstItemDef->AmmType = iAmmType;
		}
		else if (pstItemDef->ItemID >= ITEM_NUM_STUFF_MIN && pstItemDef->ItemID <= ITEM_NUM_STUFF_MAX)
		{
			iType = ITEM_TYPE_STUFF;
		}
		else if (pstItemDef->ItemID >= ITEM_NUM_TASK_MIN && pstItemDef->ItemID <= ITEM_NUM_TASK_MAX)
		{
			iType = ITEM_TYPE_TASK;
		}
		else
		{
			iType = ITEM_TYPE_ITEM;
		}
	}
	else
	{
		iType = -1;
	}

	pstItemDef->ItemType = iType;

	return 0;
}

int get_arm_result_total(ITEMDEF *pstItemDef, RESULT *pstResult, int iResultID)
{
	int i;

	memset(pstResult, 0, sizeof(*pstResult));
	
	for (i=0; i<MAX_ITEM_RESULT; i++)	
	{
		if (0 ==pstItemDef->Result[i].ResultID)
		{
			//break;
			continue;
		}

		if (pstItemDef->Result[i].ResultID == iResultID)
		{
			pstResult->ResultVal1 += pstItemDef->Result[i].ResultVal1;
			pstResult->ResultVal2 += pstItemDef->Result[i].ResultVal2;
			pstResult->ResultVal3 += pstItemDef->Result[i].ResultVal3;
			pstResult->ResultVal4 += pstItemDef->Result[i].ResultVal4;
		}
	}

	return 0;
}

int add_attr_result(PlayerAtk *pstPlayerAtk,CSDynAttrList *pstDyn, ITEMDEF *pstItemDef, int iAddAttrID, ARMRESULT *pstResult, int iAllWeaponAtkChg)
{
	int i;
	RESULT stResult;
	ARMLEVELHURTDEF *pstArmLevelHurt;
	
	switch(iAddAttrID)
	{
		case ADD_ATTR_RESULT_ATK:
		case ADD_ATTR_RESULT_MIN_ATK:
		case ADD_ATTR_RESULT_ENERGE_ATK:
		case ADD_ATTR_RESULT_CANNON_ATK:
			if (iAllWeaponAtkChg)
			{
				for (i=0; i<MAX_ARM_ON; i++)
				{
					get_arm_result_total(pstItemDef, &stResult, RESULT_ICE_ATK);
					pstDyn->astAtkChg[i].unIceBaseArmAtk += stResult.ResultVal1*pstResult->ResultVal1/100.0;

					get_arm_result_total(pstItemDef, &stResult, RESULT_PHSIC_ATK);
					pstDyn->astAtkChg[i].unPhsicBaseArmAtk +=  stResult.ResultVal1*pstResult->ResultVal1/100.0;
				}
			}
			break;
		case ADD_ATTR_RESULT_HP:
		case ADD_ATTR_RESULT_MIN_HP:
			get_arm_result_total(pstItemDef, &stResult, RESULT_HPMAX);
			pstDyn->stHealChg.iHPMaxBase += stResult.ResultVal1*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_MP:
		case ADD_ATTR_RESULT_MIN_MP:
			get_arm_result_total(pstItemDef, &stResult, RESULT_MPMAX);
			pstDyn->stHealChg.iMPMaxBase += stResult.ResultVal1*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_DAMAGE:
			pstArmLevelHurt = pstPlayerAtk->pstArmLevelHurtDef;//->HurtMul;
			if (pstArmLevelHurt)
			{
				if (iAllWeaponAtkChg)
				{
					for (i=0; i<MAX_ARM_ON; i++)
					{
						pstDyn->astAtkChg[i].nArmLevChgMul += pstArmLevelHurt->HurtMul*pstResult->ResultVal1/100.0;
					}
				}
			}
			
			break;	
		case ADD_ATTR_RESULT_MIN_EXP:
		case ADD_ATTR_RESULT_EXP:
			pstDyn->stHealChg.nArmExpMul += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_PHSIC_DEF:
			get_arm_result_total(pstItemDef, &stResult, RESULT_PHSIC_DEF);
			pstDyn->stDefChg.unPhsicDefBaseAdd += stResult.ResultVal1*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_ICE_DEF:
			get_arm_result_total(pstItemDef, &stResult, RESULT_ICE_DEF);
			pstDyn->stDefChg.unIceDefBaseAdd += stResult.ResultVal1*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_ADD_DEF:
			get_arm_result_total(pstItemDef, &stResult, RESULT_PHSIC_DEF);
			pstDyn->stDefChg.unPhsicDefBaseAdd += stResult.ResultVal1*pstResult->ResultVal1/100.0;
			get_arm_result_total(pstItemDef, &stResult, RESULT_ICE_DEF);
			pstDyn->stDefChg.unIceDefBaseAdd += stResult.ResultVal1*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_DEF_PHISICHURT:
			get_arm_result_total(pstItemDef, &stResult, RESULT_DEF_PHISICHURT);
			pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += stResult.ResultVal2*pstResult->ResultVal1/100.0;
			break;	
		case ADD_ATTR_RESULT_DEF_ICEHURT:
			get_arm_result_total(pstItemDef, &stResult, RESULT_DEF_ICEHURT);
			pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += stResult.ResultVal2*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_DEF_ICE_PHISIC:
			get_arm_result_total(pstItemDef, &stResult, RESULT_DEF_PHISICHURT);
			pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += stResult.ResultVal2*pstResult->ResultVal1/100.0;
			get_arm_result_total(pstItemDef, &stResult, RESULT_DEF_ICEHURT);
			pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += stResult.ResultVal2*pstResult->ResultVal1/100.0;
			break;
		case ADD_ATTR_RESULT_FLEE:
			pstDyn->stFleeChg.unFleeBaseAdd += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_FIGHTVAL:
			pstDyn->stMidAttrList.uiFight += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_HPADD:
			get_arm_result_total(pstItemDef, &stResult, RESULT_HPMAX);
			pstDyn->stHealChg.unArmHPAdd += stResult.ResultVal1*pstResult->ResultVal1/10000.0;
			break;
		case ADD_ATTR_RESULT_MPADD:
			get_arm_result_total(pstItemDef, &stResult, RESULT_MPMAX);
			pstDyn->stHealChg.unArmMPAdd += stResult.ResultVal1*pstResult->ResultVal1/10000.0;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_HPADD:
			get_arm_result_total(pstItemDef, &stResult, RESULT_HPMAX);
			pstDyn->stHealChg.unNoFightRestoreHPAdd += stResult.ResultVal1*pstResult->ResultVal1/10000.0;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_MPADD:
			get_arm_result_total(pstItemDef, &stResult, RESULT_MPMAX);
			pstDyn->stHealChg.unNoFightRestoreMPAdd += stResult.ResultVal1*pstResult->ResultVal1/10000.0;
			break;
		case ADD_ATTR_RESULT_SKILL_LEVEL:
			//z_arm_skill_levelup(pstDyn, pstResult->ResultVal1, pstResult->ResultVal2);
			break;
		case ADD_ATTR_RESULT_BADSTATUS_RATE:
			pstDyn->stHealChg.ucBadStatusRate += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_BADSTATUS_DEF:
			pstDyn->stHealChg.ucBadStatusDefRate += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_BADSTATUS_DEC_TIME:
			pstDyn->stHealChg.unBadStatusDecTime += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_HEAVY_HIT:
			pstDyn->stHitRateChg.nHeavyHitAdd += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_HITRATE:
			pstDyn->stHitRateChg.nAtkerHitRateAdd += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_TASK_ITEM_RATE:
			if (pstDyn->stHealChg.ucTaskItemRate+pstResult->ResultVal1> 100)
			{
				pstDyn->stHealChg.ucTaskItemRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucTaskItemRate+= pstResult->ResultVal1;
			}
			break;
		case ADD_ATTR_RESULT_STUFF_ITEM_RATE:
			if (pstDyn->stHealChg.ucStuffItemRate+pstResult->ResultVal1> 100)
			{
				pstDyn->stHealChg.ucStuffItemRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucStuffItemRate+= pstResult->ResultVal1;
			}
			break;
		case ADD_ATTR_RESULT_EGG_RATE:
			if (pstDyn->stHealChg.ucEggRate+pstResult->ResultVal1> 100)
			{
				pstDyn->stHealChg.ucEggRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucEggRate+= pstResult->ResultVal1;
			}
			break;
		case ADD_ATTR_RESULT_ADD_BULL:
			if (pstDyn->stHealChg.ucBullRate+pstResult->ResultVal1> 100)
			{
				pstDyn->stHealChg.ucBullRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucBullRate+= pstResult->ResultVal1;
			}
			break;
		case ADD_ATTR_RESULT_ADD_PET_EXP:
			pstDyn->stHealChg.unPetExpMul += pstResult->ResultVal1;
			break;
		case ADD_ATTR_RESULT_ADD_SKILLPOINT:
			pstDyn->stHealChg.unSkillPonitMul += pstResult->ResultVal1;
			break;
		default:
			break;
	}

	return 0;
}

/*
//装备品质,战斗力计算
int  arm_star_fight(PlayerAtk *pstPlayerAtk, CSDynAttrList *pstDyn, ITEMDEF *pstItemDef)
{
	STARATTR *pstStarAttr = NULL;
	ARMGRADEDEF *pstArmGradeDef = NULL;
	
	//pstArmGradeDef = armgrade_def_find(pstEnv, pstItemDef->NumType, pstItemDef->ItemLevel);
	//if (!pstArmGradeDef)
	//{
	//	return -1;		
	//}
	int i;
	for(i=0; i<MAX_ARMGRADE;i++)
	{
		if (NULL == pstPlayerAtk->pastArmGradeDef[i])
		{
			break;
		}
		
		if( pstPlayerAtk->pastArmGradeDef[i]->ArmType  == pstItemDef->NumType && 
			pstPlayerAtk->pastArmGradeDef[i]->ArmLevel == pstItemDef->ItemLevel)
		{
			pstArmGradeDef = pstPlayerAtk->pastArmGradeDef[i];
			break;
		}
	}
	if (!pstArmGradeDef)
	{
		return -1;		
	}

	//战斗力
	//if (pstRoleArm->FightLv > 0)
	//{
	//	pstDyn->stMidAttrList.uiFight += pstArmGradeDef->FightVal[pstRoleArm->FightLv-1];
	//}	
	
	//品质计算只影响装备自身的属性提升
	if (pstPlayerAtk->StarLv > 0 && pstPlayerAtk->StarLv < MAX_ARM_STAR_LEVEL)
	{
		RESULT stResult;
		pstStarAttr = &pstArmGradeDef->ArmStar[pstPlayerAtk->StarLv-1];
 
 		//血量上限
		get_arm_result_total(pstItemDef, &stResult, RESULT_HPMAX);
		pstDyn->stHealChg.iHPMaxBase += stResult.ResultVal1*pstStarAttr->MaxHPMul/100.0;
		//MP 上限
		get_arm_result_total(pstItemDef, &stResult, RESULT_MPMAX);
		pstDyn->stHealChg.iMPMaxBase += stResult.ResultVal1*pstStarAttr->MaxMPMul/100.0;

		//冰抗
		get_arm_result_total(pstItemDef, &stResult, RESULT_DEF_ICEHURT);
		pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += stResult.ResultVal2*pstStarAttr->IceHurtDefMul/100.0;

		//物抗
		get_arm_result_total(pstItemDef, &stResult, RESULT_DEF_PHISICHURT);
		pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += stResult.ResultVal2*pstStarAttr->PhsicHurtDefMul/100.0;

		//只对怪物起作用冰抗
		get_arm_result_total(pstItemDef, &stResult, RESULT_MON_ICE_HURT);
		pstDyn->stMonChg.nAtkeeIceDefAdd += stResult.ResultVal2*pstStarAttr->IceHurtDefMul/100.0;

		//只对怪物起作用物抗
		get_arm_result_total(pstItemDef, &stResult, RESULT_MON_PHSIC_HURT);
		pstDyn->stMonChg.nAtkeePhsicDefAdd += stResult.ResultVal2*pstStarAttr->PhsicHurtDefMul/100.0;

		//物防
		get_arm_result_total(pstItemDef, &stResult, RESULT_PHSIC_DEF);
		pstDyn->stDefChg.unPhsicDefBaseAdd += stResult.ResultVal1*pstStarAttr->PhsicDefMul/100.0;

		//冰防
		get_arm_result_total(pstItemDef, &stResult, RESULT_ICE_DEF);
		pstDyn->stDefChg.unIceDefBaseAdd += stResult.ResultVal1*pstStarAttr->IceDefMul/100.0;

		//随时间恢复HP
		get_arm_result_total(pstItemDef, &stResult, RESULT_ARM_HPADD);
		pstDyn->stHealChg.unArmHPAdd += stResult.ResultVal1*pstStarAttr->HPRecoverMul/100.0;

		//随时间恢复MP
		get_arm_result_total(pstItemDef, &stResult, RESULT_ARM_MPADD);
		pstDyn->stHealChg.unArmMPAdd += stResult.ResultVal1*pstStarAttr->MPRecoverMul/100.0;

		//只对怪物起作用闪避
		get_arm_result_total(pstItemDef, &stResult, RESULT_MON_FLEE);
		pstDyn->stMonChg.unFleeChgAdd += stResult.ResultVal2*pstStarAttr->FleeMul/100.0;

		//闪避
		get_arm_result_total(pstItemDef, &stResult, RESULT_FLEE);
		pstDyn->stFleeChg.iFleeChgAdd += stResult.ResultVal1*pstStarAttr->FleeMul/100.0;

	}

	return 0;
}
*/
int cacl_player_dyn(PlayerAtk *pstPlayerAtk, CSDynAttrList *pstDyn, char *pszError)
{
	int i, j,z;
	int iRingNum = 0;
	ITEMDEF *pstItemDef;
	ITEMDEF *pstItemDef2;
	RESULT *pstResult;
	CSAtkChg *pstAtkChg;
	int iWeaponNum = 0;
	//int idxSuitCount = 0;
	//SuitCount astSuitCount[MAX_WEAR];
	ARMADDATTRDEF *pstArmAddAttr;

	//memset(&astSuitCount, 0, sizeof(astSuitCount));
	/* 玩家没装备不计算*/
	if (pstPlayerAtk->iArm < 0 || pstPlayerAtk->iArm > MAX_WEAR)
	{
		sprintf(pszError, "输入装备数量错误");
		return -1;
	}
    
	for (i=0; i<pstPlayerAtk->iArm; i++)
	{
		pstItemDef = pstPlayerAtk->pastArm[i];
		item_id_num(pstItemDef);
		//如果是戒指记录下有多少个
		if (WEAR_POS_RING == pstItemDef->WearPos1)
		{
			iRingNum++;
			if (iRingNum > 3)
			{
				sprintf(pszError, "输入装备戒指数量超过3");
				return -1;
			}
		}
		
		if (pstItemDef->WearPosNum <= 0)
		{
			sprintf(pszError, "%s 不是装备", pstItemDef->Name);
			return -1;
		}
        //判断下是否有相冲突的装备
		for (j=i+1; j<pstPlayerAtk->iArm; j++)
		{
			pstItemDef2 = pstPlayerAtk->pastArm[j];
			if (wear_pos_rep(pstItemDef, pstItemDef2))
			{
				sprintf(pszError, "装备%s 和装备%s冲突", pstItemDef->Name, pstItemDef2->Name);
				return -1;
			}
		}

		//如果是武器
		if (is_weapon(pstItemDef))
		{
		    if (iWeaponNum >= MAX_ARM_ON)
			{
				pstAtkChg = NULL;
			}
			else
			{
				pstAtkChg = &pstDyn->astAtkChg[iWeaponNum];
				pstAtkChg->pstItemDef = pstItemDef;
				if (NULL != pstPlayerAtk->pstArmLevelHurtDef)
				{
				    //把武器的伤害修正为对应的等级伤害 
					pstAtkChg->nArmLevChgMul = pstPlayerAtk->pstArmLevelHurtDef->HurtMul;
				}
				//赋值下装备的类型
				pstAtkChg->iArmType = pstItemDef->NumType;
				iWeaponNum++;
			}
		}
        //如果装备有 破坏力乘修正 和 破坏力加修正 
		if (0 != pstItemDef->DamageMul || 0 != pstItemDef->DamageAdd)
		{
			for (j=0; j<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0])); j++)
			{
			    //把装备做下处理，记录下这个数据
				pstDyn->astAtkChg[j].nDamageMul += pstItemDef->DamageMul;
				pstDyn->astAtkChg[j].unDamageAdd += pstItemDef->DamageAdd;
			}	
		}
        // 如果是套装先把套装记录下来，后面来算修正
        //if (0 < pstItemDef->SuitSetID)
		//{
			/* look for suit combination */
		//	for (j = 0; j < idxSuitCount; j++)
		//	{
		///		if (pstItemDef->SuitSetID == astSuitCount[j].iSuitID)
		//		{
		//			astSuitCount[j].iCount += pstItemDef->WearPosNum;
		//			break;
		//		}
		//	}
        //
		//	/* not found, unique */
		//	if (idxSuitCount == j)
		//	{
		//		astSuitCount[idxSuitCount].iSuitID = pstItemDef->SuitSetID;
		//		astSuitCount[idxSuitCount].iCount++;
		//		idxSuitCount++;
		//	}
		//}
		//装备的战斗力?
        //pstDyn->stMidAttrList.uiFight += pstItemDef->ArmFight;

		//如果装备对武器有影响，需要把reulst的修正加到所有的武器攻击修正上去
		for (j=0; j<MAX_ITEM_RESULT; j++)	
		{
			pstResult = &pstItemDef->Result[j];

			if (0 == pstResult->ResultID)
			{
				break;
			}

			attr_result(pstDyn, pstResult);
		}

        //装备关键字效果
		for (j=0; j<MAX_ARM_ADD_ATTR_RESULT; j++)	
		{
			if (0 == pstItemDef->AddAttr[j].ID)
			{
				break;
			}

			pstArmAddAttr = NULL;
			for(z=0;z<MAX_ARM_ADDATTR_DEF;z++)
			{
				if (NULL == pstPlayerAtk->pastArmAddAttrDef[z])
				{
					break;
				}
				
				 if( pstPlayerAtk->pastArmAddAttrDef[z]->AddAttrID == pstItemDef->AddAttr[j].ID)
				 {
				      pstArmAddAttr = pstPlayerAtk->pastArmAddAttrDef[z];
					  break;
				 }
			}
        
			if (!pstArmAddAttr)
			{
				continue;
			}

			if (pstItemDef->AddAttr[j].Idx == 0 || pstItemDef->AddAttr[j].Idx > MAX_ARM_ADD_ATTR_RESULT)
			{
				continue;
			}
			
			add_attr_result(pstPlayerAtk, pstDyn, 
					                   pstItemDef, 
									   pstItemDef->AddAttr[j].ID, 
									   &pstArmAddAttr->Result[pstItemDef->AddAttr[j].Idx-1], 1);
			
		}
        //装备品质,战斗力计算
		//arm_star_fight(pstPlayerAtk,pstDyn, pstItemDef);

		//装备宝石属性计算 不在这里计算了 
		//z_arm_gem(pstEnv, pstDyn, pstRoleArm, pstAtkChg);
		 
		
	}

    //套装属性 后续再算

	//dyn_tmp(pstDyn);
		
	return 0;
}

int cacl_mon_dyn(MonAtk *pstMonAtk, CSDynAttrList *pstDyn, char *pszError)
{
	CSAtkChg *pstAtkChg;
	MONSTERDEF *pstMonsterDef = pstMonAtk->pstMonDef;
		
	memset(pstDyn, 0, sizeof(*pstDyn));
	pstAtkChg = &pstDyn->astAtkChg[0];
	memcpy(&pstAtkChg->stAtkerHurtChg, &pstMonsterDef->AtkerHurtChg, sizeof(pstAtkChg->stAtkerHurtChg));
	
	pstAtkChg->unIceBaseArmAtk = pstMonsterDef->IceAtk;
	pstAtkChg->unFireBaseArmAtk = pstMonsterDef->FireAtk;
	pstAtkChg->unThunderBaseArmAtk = pstMonsterDef->ThunderAtk;
	pstAtkChg->unLightBaseArmAtk = pstMonsterDef->LightAtk;
	pstAtkChg->unNightBaseArmAtk = pstMonsterDef->NightAtk;
	pstAtkChg->unPhsicBaseArmAtk = pstMonsterDef->PhsicAtk;
	pstDyn->stDefChg.iDefIceTmp = pstMonsterDef->IceDef;
	pstDyn->stDefChg.iDefFireTmp = pstMonsterDef->FireDef;
	pstDyn->stDefChg.iDefThunderTmp = pstMonsterDef->ThunderDef;
	pstDyn->stDefChg.iDefLightTmp = pstMonsterDef->LightDef;
	pstDyn->stDefChg.iDefNightTmp = pstMonsterDef->NightDef;
	pstDyn->stDefChg.iDefPhsicTmp = pstMonsterDef->PhsicDef;
	pstDyn->stHitChg.iHitTmp = pstMonsterDef->Hit;
	pstDyn->stFleeChg.iFleeTmp = pstMonsterDef->Flee;
	memcpy(&pstDyn->stAtkeeHurtChg, &pstMonsterDef->AtkeeHurtChg, sizeof(pstDyn->stAtkeeHurtChg));
	pstDyn->stHitRateChg.nHeavyHitAdd = pstMonsterDef->HeavyHitRate;
	
	return 0;
	UNUSED(pszError);	//add by paraunused.pl
}

CSAtkChg *get_atkchg(SKILLDEF *pstSkillDef, CSDynAttrList *pstDyn)
{
	int i, j;
	CSAtkChg *pstAtkChg;
	ITEMDEF *pstItemDef;
	
	for (i=0; i<(int)(sizeof(pstDyn->astAtkChg)/sizeof(pstDyn->astAtkChg[0]));i++)
	{
		pstAtkChg = &pstDyn->astAtkChg[i];
		if (NULL == pstAtkChg->pstItemDef)
			break;

		for (j=0; j<MAX_SKILL_ARMTYPE; j++)
		{
			if (0 == pstSkillDef->ArmType[j].ArmType)
			{
				break;
			}

			pstItemDef = pstAtkChg->pstItemDef;
			
			if (pstItemDef->NumType == pstSkillDef->ArmType[j].ArmType)
			{
				return pstAtkChg;
			}
		}
	}

	if (ARM_ANY == pstSkillDef->ArmType[0].ArmType)
	{
		return &pstDyn->astAtkChg[0];
	}

	return NULL;
}

int cacl_hitrate(CSDynAttrList *pstAtkerDyn, CSDynAttrList *pstAtkeeDyn)
{
	int iHit, iFlee, iHitRate;
		
	iHit = pstAtkerDyn->stHitChg.iHitTmp;

	//防御方的闪避
	iFlee = pstAtkeeDyn->stFleeChg.iFleeTmp*(1+pstAtkeeDyn->stFleeChg.nFleeChgMul/100.0) + 
								pstAtkeeDyn->stFleeChg.iFleeChgAdd;

	//命中初值
	if (iFlee > 0)
	{
		iHitRate = iHit*100.0/iFlee;
	}
	else
	{
		iHitRate = 100;
	}

	iHitRate  = iHitRate * 
				(1+pstAtkerDyn->stHitRateChg.nAtkerHitRateMul/100.0-pstAtkeeDyn->stHitRateChg.nAtkeeHitRateMul/100.0) + 
				(pstAtkerDyn->stHitRateChg.nAtkerHitRateAdd - pstAtkeeDyn->stHitRateChg.nAtkeeHitRateAdd);

	return iHitRate;
}

int cacl_hit_hurt(CSDynAttrList *pstAtkerDyn, CSAtkChg *pstAtkChg, CSDynAttrList *pstAtkeeDyn, SKILLDEF *pstSkillDef, SKILLARMCHG *pstSkillArmChg,
					CaclOutput *pstOutput)
{
	CSDefChg *pstDefChg;
	int iHP, iHPTmp;
	int iSkillHurt;
	ATKERHURTCHG *pstAtkerHurtChg;
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstAtkeeDyn->stAtkeeHurtChg;
	int iHurtMin;
	int iHurtMax;
	int iHurtMul;
	int iDamage;
	int iAtk = 0;
	int iDef = 0;
	int iMagicCacl = 0;
	
	pstAtkerHurtChg = &pstAtkChg->stAtkerHurtChg;
	pstDefChg = &pstAtkeeDyn->stDefChg;
	iDamage = pstAtkChg->nArmLevChgMul*(1+pstAtkChg->nDamageMul/100.0)+pstAtkChg->unDamageAdd;

	//算最小伤害
	iHP = 0;
	if ((pstSkillDef->MainAttr == SKILL_ATTR_ICE || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->IceHurtMax> 0)
	{
		iMagicCacl = 1;
			
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->IceHurtMin;
			iHurtMax = pstSkillDef->IceHurtMax;
			iHurtMul = pstSkillDef->IceHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		iAtk = (pstAtkChg->unIceBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nIceAtkChgMul/100.0)+pstAtkChg->iIceAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefIceTmp*(1+pstDefChg->nIceDefChgMul/100.0)+pstDefChg->iIceDefChgAdd - pstAtkChg->iIceDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerIceHurtMul/100.0-pstAtkeeHurtChg->AtkeeIceHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerIceHurtAdd-pstAtkeeHurtChg->AtkeeIceHurtAdd);
	}
	
	if ((pstSkillDef->MainAttr == SKILL_ATTR_FIRE || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->FireHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_FIRE)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->FireHurtMin;
			iHurtMax = pstSkillDef->FireHurtMax;
			iHurtMul = pstSkillDef->FireHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin ;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		iAtk = (pstAtkChg->unFireBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nFireAtkChgMul/100.0)+pstAtkChg->iFireAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefFireTmp*(1+pstDefChg->nFireDefChgMul/100.0)+pstDefChg->iFireDefChgAdd - pstAtkChg->iFireDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerFireHurtMul/100.0-pstAtkeeHurtChg->AtkeeFireHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerFireHurtAdd-pstAtkeeHurtChg->AtkeeFireHurtAdd);
	}

	if ((pstSkillDef->MainAttr == SKILL_ATTR_THUNDER || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->ThunderHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_THUNDER)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->ThunderHurtMin;
			iHurtMax = pstSkillDef->ThunderHurtMax;
			iHurtMul = pstSkillDef->ThunderHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unThunderBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->ThunderHurtMul/100.0)*(1+pstAtkChg->nThunderAtkChgMul/100.0)+pstAtkChg->iThunderAtkChgAdd;
		iAtk = (pstAtkChg->unThunderBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nThunderAtkChgMul/100.0)+pstAtkChg->iThunderAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefThunderTmp*(1+pstDefChg->nThunderDefChgMul/100.0)+pstDefChg->iThunderDefChgAdd	- pstAtkChg->iThunderDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerThunderHurtMul/100.0-pstAtkeeHurtChg->AtkeeThunderHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerThunderHurtAdd-pstAtkeeHurtChg->AtkeeThunderHurtAdd);
	}
	
	if ((pstSkillDef->MainAttr == SKILL_ATTR_LIGHT || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->LightHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_LIGHT)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->LightHurtMin;
			iHurtMax = pstSkillDef->LightHurtMax;
			iHurtMul = pstSkillDef->LightHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin ;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unLightBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->LightHurtMul/100.0)*(1+pstAtkChg->nLightAtkChgMul/100.0)+pstAtkChg->iLightAtkChgAdd;
		iAtk = (pstAtkChg->unLightBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nLightAtkChgMul/100.0)+pstAtkChg->iLightAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefLightTmp*(1+pstDefChg->nLightDefChgMul/100.0)+pstDefChg->iLightDefChgAdd - pstAtkChg->iLightDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerLightHurtMul/100.0-pstAtkeeHurtChg->AtkeeLightHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerLightHurtAdd-pstAtkeeHurtChg->AtkeeLightHurtAdd);
	}

	if ((pstSkillDef->MainAttr == SKILL_ATTR_NIGHT || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->NightHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_NIGHT)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->NightHurtMin;
			iHurtMax = pstSkillDef->NightHurtMax;
			iHurtMul = pstSkillDef->NightHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unNightBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->NightHurtMul/100.0)*(1+pstAtkChg->nNightAtkChgMul/100.0)+pstAtkChg->iNightAtkChgAdd;
		iAtk = (pstAtkChg->unNightBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nNightAtkChgMul/100.0)+pstAtkChg->iNightAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefNightTmp*(1+pstDefChg->nNightDefChgMul/100.0)+pstDefChg->iNightDefChgAdd - pstAtkChg->iNightDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerNightHurtMul/100.0-pstAtkeeHurtChg->AtkeeNightHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerNightHurtAdd-pstAtkeeHurtChg->AtkeeNightHurtAdd);
	}

	//法术伤害处理
	if (iMagicCacl)
	{
		iHP = iHP*(1+pstAtkerHurtChg->AtkerMagicHurtMul/100.0-pstAtkeeHurtChg->AtkeeMagicHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerMagicHurtAdd-pstAtkeeHurtChg->AtkeeMagicHurtAdd;
	}
	
	if (pstSkillDef->MainAttr == SKILL_ATTR_PHSIC && pstSkillDef->PhsicHurtMax> 0)
	{
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->PhsicHurtMin;
			iHurtMax = pstSkillDef->PhsicHurtMax;
			iHurtMul = pstSkillDef->PhsicHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unPhsicBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->PhsicHurtMul/100.0)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;
		iAtk = (pstAtkChg->unPhsicBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefPhsicTmp*(1+pstDefChg->nPhsicDefChgMul/100.0)+pstDefChg->iPhsicDefChgAdd - pstAtkChg->iPhsicDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		//iHPTmp = iAtk*(1+pstAtkChg->nArmLevChgMul/100.0)/iDef;
		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHPTmp = (iHPTmp*(1+pstAtkerHurtChg->AtkerPhsicHurtMul/100.0-pstAtkeeHurtChg->AtkeePhsicHurtMul/100.0) + 
				 	pstAtkerHurtChg->AtkerPhsicHurtAdd-pstAtkeeHurtChg->AtkeePhsicHurtAdd);
	}
	else
	{
		iHPTmp = 0;
	}

	iHP = (iHP*1.0 + iHPTmp)*(1+pstAtkerHurtChg->AtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeHurtMul/100.0) + 
			pstAtkerHurtChg->AtkerHurtAdd-pstAtkeeHurtChg->AtkeeHurtAdd;

	if (iHP < 0)
	{
		iHP = 0;
	}

	pstOutput->iHurtMin = iHP;

	//计算最大伤害
	iHP = 0;
	if ((pstSkillDef->MainAttr == SKILL_ATTR_ICE || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->IceHurtMax> 0)
	{
		iMagicCacl = 1;
			
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->IceHurtMin;
			iHurtMax = pstSkillDef->IceHurtMax;
			iHurtMul = pstSkillDef->IceHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMax;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		iAtk = (pstAtkChg->unIceBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nIceAtkChgMul/100.0)+pstAtkChg->iIceAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefIceTmp*(1+pstDefChg->nIceDefChgMul/100.0)+pstDefChg->iIceDefChgAdd - pstAtkChg->iIceDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerIceHurtMul/100.0-pstAtkeeHurtChg->AtkeeIceHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerIceHurtAdd-pstAtkeeHurtChg->AtkeeIceHurtAdd);
	}
	
	if ((pstSkillDef->MainAttr == SKILL_ATTR_FIRE || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->FireHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_FIRE)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->FireHurtMin;
			iHurtMax = pstSkillDef->FireHurtMax;
			iHurtMul = pstSkillDef->FireHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMax;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		iAtk = (pstAtkChg->unFireBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nFireAtkChgMul/100.0)+pstAtkChg->iFireAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefFireTmp*(1+pstDefChg->nFireDefChgMul/100.0)+pstDefChg->iFireDefChgAdd - pstAtkChg->iFireDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerFireHurtMul/100.0-pstAtkeeHurtChg->AtkeeFireHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerFireHurtAdd-pstAtkeeHurtChg->AtkeeFireHurtAdd);
	}

	if ((pstSkillDef->MainAttr == SKILL_ATTR_THUNDER || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->ThunderHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_THUNDER)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->ThunderHurtMin;
			iHurtMax = pstSkillDef->ThunderHurtMax;
			iHurtMul = pstSkillDef->ThunderHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMax;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unThunderBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->ThunderHurtMul/100.0)*(1+pstAtkChg->nThunderAtkChgMul/100.0)+pstAtkChg->iThunderAtkChgAdd;
		iAtk = (pstAtkChg->unThunderBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nThunderAtkChgMul/100.0)+pstAtkChg->iThunderAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefThunderTmp*(1+pstDefChg->nThunderDefChgMul/100.0)+pstDefChg->iThunderDefChgAdd	- pstAtkChg->iThunderDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerThunderHurtMul/100.0-pstAtkeeHurtChg->AtkeeThunderHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerThunderHurtAdd-pstAtkeeHurtChg->AtkeeThunderHurtAdd);
	}
	
	if ((pstSkillDef->MainAttr == SKILL_ATTR_LIGHT || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->LightHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_LIGHT)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->LightHurtMin;
			iHurtMax = pstSkillDef->LightHurtMax;
			iHurtMul = pstSkillDef->LightHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMax;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unLightBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->LightHurtMul/100.0)*(1+pstAtkChg->nLightAtkChgMul/100.0)+pstAtkChg->iLightAtkChgAdd;
		iAtk = (pstAtkChg->unLightBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nLightAtkChgMul/100.0)+pstAtkChg->iLightAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefLightTmp*(1+pstDefChg->nLightDefChgMul/100.0)+pstDefChg->iLightDefChgAdd - pstAtkChg->iLightDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerLightHurtMul/100.0-pstAtkeeHurtChg->AtkeeLightHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerLightHurtAdd-pstAtkeeHurtChg->AtkeeLightHurtAdd);
	}

	if ((pstSkillDef->MainAttr == SKILL_ATTR_NIGHT || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->NightHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_NIGHT)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->NightHurtMin;
			iHurtMax = pstSkillDef->NightHurtMax;
			iHurtMul = pstSkillDef->NightHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMax;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unNightBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->NightHurtMul/100.0)*(1+pstAtkChg->nNightAtkChgMul/100.0)+pstAtkChg->iNightAtkChgAdd;
		iAtk = (pstAtkChg->unNightBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nNightAtkChgMul/100.0)+pstAtkChg->iNightAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefNightTmp*(1+pstDefChg->nNightDefChgMul/100.0)+pstDefChg->iNightDefChgAdd - pstAtkChg->iNightDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHP += (iHPTmp*(1+pstAtkerHurtChg->AtkerNightHurtMul/100.0-pstAtkeeHurtChg->AtkeeNightHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerNightHurtAdd-pstAtkeeHurtChg->AtkeeNightHurtAdd);
	}

	//法术伤害处理
	if (iMagicCacl)
	{
		iHP = iHP*(1+pstAtkerHurtChg->AtkerMagicHurtMul/100.0-pstAtkeeHurtChg->AtkeeMagicHurtMul/100.0) + 
				pstAtkerHurtChg->AtkerMagicHurtAdd-pstAtkeeHurtChg->AtkeeMagicHurtAdd;
	}
	
	if (pstSkillDef->MainAttr == SKILL_ATTR_PHSIC && pstSkillDef->PhsicHurtMax> 0)
	{
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->PhsicHurtMin;
			iHurtMax = pstSkillDef->PhsicHurtMax;
			iHurtMul = pstSkillDef->PhsicHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMax;
		}
		else
		{
			iSkillHurt = 0;
		}
				
		//iAtk = (pstAtkChg->unPhsicBaseArmAtk*(1+pstAtkChg->unBaseAmmAtk/100.0)+iSkillHurt)*(1+pstSkillDef->PhsicHurtMul/100.0)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;
		iAtk = (pstAtkChg->unPhsicBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;

		if (pstOutput->iContinueNum > 1)
		{
			iAtk = iAtk / pstOutput->iContinueNum;
		}
		
		iDef = pstDefChg->iDefPhsicTmp*(1+pstDefChg->nPhsicDefChgMul/100.0)+pstDefChg->iPhsicDefChgAdd - pstAtkChg->iPhsicDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		//iHPTmp = iAtk*(1+pstAtkChg->nArmLevChgMul/100.0)/iDef;
		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		iHPTmp = (iHPTmp*(1+pstAtkerHurtChg->AtkerPhsicHurtMul/100.0-pstAtkeeHurtChg->AtkeePhsicHurtMul/100.0) + 
				 	pstAtkerHurtChg->AtkerPhsicHurtAdd-pstAtkeeHurtChg->AtkeePhsicHurtAdd);
	}
	else
	{
		iHPTmp = 0;
	}

	iHP = (iHP*1.0 + iHPTmp)*(1+pstAtkerHurtChg->AtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeHurtMul/100.0) + 
			pstAtkerHurtChg->AtkerHurtAdd-pstAtkeeHurtChg->AtkeeHurtAdd;

	if (iHP < 0)
	{
		iHP = 0;
	}

	pstOutput->iHurtMax = iHP;
	
	return 0;
	UNUSED(pstAtkerDyn);	//add by paraunused.pl
}

int cacl_heavyreate(CSDynAttrList *pstAtkerDyn, CSDynAttrList *pstAtkeeDyn, SKILLDEF *pstSkillDef)
{
	int iHeavyHit = 0;
	
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
	}

	if (iHeavyHit > 100)
	{
		iHeavyHit = 100;
	}

	return iHeavyHit;
	UNUSED(pstAtkeeDyn);	//add by paraunused.pl
}


/**通过攻防双方的信息计算最后的伤害
*@param[in]  输入参数，攻击者和被攻击者信息
*@param[out]   输出最终的攻击数值
*@retval 0   succ
*@retval <0  fail  
*/
int calc_val(CaclInput *pstInput, CaclOutput *pstOutput)
{
	int i;
	CSDynAttrList stAtkerDyn;
	CSDynAttrList stAtkeeDyn;
	CSAtkChg *pstAtkChg;
	SKILLDEF *pstSkillDef;
	SKILLARMCHG *pstSkillArmChg; 
	
	memset(&stAtkerDyn, 0, sizeof(stAtkerDyn));
	memset(&stAtkeeDyn, 0, sizeof(stAtkeeDyn));
	pstSkillArmChg = NULL;

	if (OBJ_PLAYER == pstInput->stAtker.iObjType)
	{
		//算玩家的攻击数值
		if (0 > cacl_player_dyn(&pstInput->stAtker.stData.stPlayerAtk, &stAtkerDyn, pstOutput->szError))
		{
			return -1;
		}
        //以下算技能的攻击数值
		pstSkillDef = pstInput->stAtker.stData.stPlayerAtk.pstSkillDef;
		if (NULL == pstSkillDef)
		{
			sprintf(pstOutput->szError, "没有攻击技能");
			return -1;
		}

		pstAtkChg = get_atkchg(pstSkillDef, &stAtkerDyn);
		if (NULL == pstAtkChg)
		{
			sprintf(pstOutput->szError, "没有攻击技能所需要的武器");
			return -1;
		}

		for (i=0; i<MAX_SKILL_ARM_CHG; i++)
		{
			if (0 == pstSkillDef->SkillArmChg[i].ArmType)
			{
				break;
			}

			if (pstAtkChg->iArmType == pstSkillDef->SkillArmChg[i].ArmType)
			{
				pstSkillArmChg = &pstSkillDef->SkillArmChg[i];
			}
		}
	}
	else if (OBJ_MONSTER == pstInput->stAtker.iObjType)
	{
		if (0 > cacl_mon_dyn(&pstInput->stAtker.stData.stMonAtk, &stAtkerDyn, pstOutput->szError))
		{
			return -1;
		}

		pstSkillDef = pstInput->stAtker.stData.stMonAtk.pstSkillDef;
		if (NULL == pstSkillDef)
		{
			sprintf(pstOutput->szError, "没有攻击技能");
			return -1;
		}

		pstAtkChg = &stAtkerDyn.astAtkChg[0];
	}
	else
	{
		sprintf(pstOutput->szError, "Atker ObjType %d err", pstInput->stAtker.iObjType);
		return -1;
	}

	if (pstSkillArmChg)
	{
		pstOutput->iContinueNum = pstSkillArmChg->ContinueNum;
	}
	else
	{
		pstOutput->iContinueNum = pstSkillDef->ContinueNum;
	}

	if (0 >= pstOutput->iContinueNum)
	{
		pstOutput->iContinueNum = 1;
	}

	cacl_skill_attr(pstSkillDef, &stAtkerDyn, pstAtkChg);

	if (OBJ_PLAYER == pstInput->stAtkee.iObjType)
	{
		if (0 > cacl_player_dyn(&pstInput->stAtkee.stData.stPlayerAtk, &stAtkeeDyn, pstOutput->szError))
		{
			return -1;
		}

		if (OBJ_MONSTER == pstInput->stAtker.iObjType)
		{
			stAtkeeDyn.stAtkeeHurtChg.AtkeeHurtMul += stAtkeeDyn.stMonChg.nAtkeeHurtMul;
			stAtkeeDyn.stAtkeeHurtChg.AtkeeHurtAdd +=  stAtkeeDyn.stMonChg.unAtkeeHurtAdd;
			stAtkeeDyn.stFleeChg.nFleeChgMul += stAtkeeDyn.stMonChg.nFleeChgMul;
			stAtkeeDyn.stFleeChg.iFleeChgAdd += stAtkeeDyn.stMonChg.unFleeChgAdd;	
		}
	}
	else if (OBJ_MONSTER == pstInput->stAtkee.iObjType)
	{
		if (0 > cacl_mon_dyn(&pstInput->stAtkee.stData.stMonAtk, &stAtkeeDyn, pstOutput->szError))
		{
			return -1;
		}
	}
	else   
	{
		sprintf(pstOutput->szError, "Atkee ObjType %d err", pstInput->stAtkee.iObjType);
		return -1;
	}
 
	if (pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
	{
		pstOutput->iHitRate = cacl_hitrate(&stAtkerDyn, &stAtkeeDyn);
	}
	else
	{
		pstOutput->iHitRate = 100;
	}

	pstOutput->iHeavyRate = cacl_heavyreate(&stAtkerDyn, &stAtkeeDyn, pstSkillDef);
	
	cacl_hit_hurt(&stAtkerDyn, pstAtkChg, &stAtkeeDyn, pstSkillDef, pstSkillArmChg, pstOutput);	
	    
	pstOutput->szError[0] = 0;
	return 0;
}

unsigned int version_to_uint(char *pszVer)
{
	// 1.2.100   1--8bit  2--8bit  100--16bit
	unsigned int iVersion = 0;
	int iSegment = 0;
	char *pszTempVer = pszVer;
	int iCount = 0;

	while(pszTempVer)
	{
		iCount ++;
		iSegment = atoi(pszTempVer);
		if (iSegment < 0)
		{
			return -1;
		}
		if (iCount == 1)
		{
			if (iSegment > 255)
			{
				return -1;
			}

			iVersion += iSegment << 24;
		}
		else if(iCount == 2)
		{
			if (iSegment > 255)
			{
				return -1;
			}
			iVersion += iSegment << 16;
		}
		else if(iCount == 3)
		{
			if (iSegment > 65535)
			{
				return -1;
			}
			iVersion += iSegment;
		}
		else
			{return -1;}

		pszTempVer = strstr(pszTempVer, ".");
		if (!pszTempVer)
		{
			return iVersion;
		}
		pszTempVer ++;
	}
	return -1;
}

const char* uint_to_version(unsigned int uiVer)
{
	static char szVer[32];
	sprintf(szVer,"%d.%d.%d",(uiVer>>24)&0xFF ,(uiVer>>16)&0xFF, uiVer&0xFFFF);
	return szVer;
}

