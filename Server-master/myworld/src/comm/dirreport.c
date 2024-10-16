#include <time.h>
#include <dirreport.h>
#include "tdr/tdr_types.h"
#include "comm_misc.h"
#include "taes.h"
#include "grav_rsa.h"

#define UNUSED( var ) ((void)var)
#define REPORT_KEY "hello_ljy"

typedef struct tagDirReportEnv
{
	SSHANDLE *pstSSHan; 
	AES stAesKey;
	RSA *pstRsa;
}DIRREPORTENV;

static SSREPORTDATAALL g_stRePortAll;

extern RSA_PRIVATE_KEY * IDRSAPrivateKey;

extern unsigned char g_szMetalib_proto_ss[];

int init_report_interface(unsigned long int *pHandle, int iSec)
{
	DIRREPORTENV *pstEnv = NULL;

	pstEnv = (DIRREPORTENV *)malloc(sizeof(DIRREPORTENV));
	if(pstEnv == NULL)
	{
		return -1;
	}
	
	pstEnv->pstSSHan = NULL;
	
	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		printf("ss_han_create is fial\n");
		destory_reprot_interface((unsigned long int)pstEnv);
		return -1;
	}

	pstEnv->pstRsa = rsa_from_private_key(IDRSAPrivateKey);
	if (NULL == pstEnv->pstRsa)
	{
		return -1;
	}
	
	*pHandle = (unsigned long int)pstEnv;

	memset(&g_stRePortAll, 0, sizeof(g_stRePortAll));
	
	return 0;
	UNUSED(iSec);	//add by paraunused.pl
}

void destory_reprot_interface(unsigned long int iHandle)
{
	DIRREPORTENV *pstEnv = (DIRREPORTENV *)iHandle;

	if(pstEnv == NULL)
	{	
		return;
	}
	
	if(pstEnv->pstSSHan != NULL)
	{
		free(pstEnv->pstSSHan);
		pstEnv->pstSSHan = NULL;
	}
	
	free(pstEnv);

	return;
}

static REPORTDATA* find_world_of_reportdata(int iRegion, int iWorld)
{
	int i;
	REPORTDATA *pstReportData;
	SSREPORTDATAALL * pstReport = &g_stRePortAll;

	for (i=0; i<pstReport->WorldNum; i++)
	{
		if (pstReport->RePort[i].DataNum > 0 && pstReport->RePort[i].WorldID == iWorld &&
				pstReport->RePort[i].RegionID == iRegion)
		{
			return &pstReport->RePort[i];
		}
	}

	// 没找到就分配一下
	if (pstReport->WorldNum >= REPORT_MAX_WORLD)
	{
		return NULL;
	}

	pstReportData = &pstReport->RePort[pstReport->WorldNum];
	pstReport->WorldNum++;

	memset(pstReportData, 0, sizeof(*pstReportData));
	pstReportData->RegionID = iRegion;
	pstReportData->WorldID = iWorld;
	
	return pstReportData;	
}


int record_data(int iRegionID, int iworldID, int iOnlineNum)
{
	int i;
	struct tm stTm;
	int  iSeconds;
	int iHour;
	REPORTDATA *pstData;
	
	pstData = find_world_of_reportdata(iRegionID, iworldID);
	if(pstData == NULL)
	{
		return -1;
	}

	iSeconds = time((time_t*)NULL);
	localtime_r((time_t*)&iSeconds, &stTm);
	iHour = stTm.tm_hour;
	
	//  查看此时间段有没有记录
	for(i=0; i<pstData->DataNum; i++)
	{
		if (pstData->DirHourData[i].Hour == iHour )
		{
			if (pstData->DirHourData[i].PlayerNum < iOnlineNum)
			{
				pstData->DirHourData[i].PlayerNum = iOnlineNum;
			}
			
			break;
		}
	}

	// 没有记录
	if(i == pstData->DataNum)
	{
		if (pstData->DataNum >= MAX_DATA_NUM)
		{
			return -1;
		}
		
		pstData->DirHourData[pstData->DataNum].Hour= iHour;
		pstData->DirHourData[pstData->DataNum].PlayerNum = iOnlineNum;
		pstData->DataNum ++;
	}
	
	return 0;

}

int record_tick()
{
	static time_t tSecond = 0;
	time_t tNow;

	if (tSecond == 0)
	{
		tSecond = time(NULL);
	}

	tNow = time(NULL);

	if (IsSameDay (tSecond, tNow) == 0)
	{
		tSecond = tNow;
		memset(&g_stRePortAll, 0, sizeof(g_stRePortAll));
	}
	
	return 0;
}

int report_check(unsigned long int iHandle, int iNum, char *pBuff)
{
	char szDec[RSA_ENC_LEN];
	char szKey[sizeof(REPORT_KEY) + 1];
	DIRREPORTENV *pstEnv = (DIRREPORTENV *)iHandle;
	
	if (0 > rsa_private_decrypt(pstEnv->pstRsa, iNum, pBuff, szDec))
	{
		return -1;
	}

	memcpy(szKey, szDec, sizeof(REPORT_KEY));
	szKey[sizeof(REPORT_KEY)] = 0;

	if (0 == strcmp(REPORT_KEY, szKey))
	{
		if (0 > taes_setkey(&pstEnv->stAesKey, (unsigned char *)&szDec[sizeof(REPORT_KEY)]))
		{
			return -1;
		}
		
		return 0;
	}
	else
	{
		return -1;
	}
}

int report_get(unsigned long int iHandle, int *piNum, char *pBuff, int iMax)
{
	DIRREPORTENV *pstEnv = (DIRREPORTENV *)iHandle;
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];

	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	stSSPkg.Head.Magic = PROTOCOL_MAGIC;
	stSSPkg.Head.HeadLen = sizeof(stSSPkg.Head);

	stSSPkg.Head.Cmd = RECORD_DIR_INFO;

	g_stRePortAll.time = time(NULL);
	memcpy(&stSSPkg.Body.RePortDataAll, &g_stRePortAll, sizeof(SSREPORTDATAALL));

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) <0 )
	{
		return -1;
	}

	*piNum = iMax;
	if (0 > taes_encrypt(stNet.pszBuff, stNet.iBuff, &pstEnv->stAesKey, pBuff, piNum))
	{
		return -1;
	}
	
	return 0;
}
	
