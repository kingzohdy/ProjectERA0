#include <time.h>
#include <dirreport.h>
#include "tdr/tdr_types.h"
#include "comm_misc.h"

#define DIR_REPORT_ADDR "udp://192.168.0.77:8567?reuse=1"
#define REPORT_SIZE(num1,num2) (((num1)/(num2))+(((num1)%(num2))?1:0))

typedef struct tagDirReportEnv
{
	int iSocket;
	SSHANDLE *pstSSHan; 
}DIRREPORTENV;

int g_iSec = 0;
int g_iSeconds = 0;

SSREPORTDATAALL g_stRePortAll[REPORT_SIZE(MAX_WORLD_NUM, REPORT_MAX_WORLD)];

extern unsigned char g_szMetalib_proto_ss[];

int init_report_interface(int *pHandle)
{
	DIRREPORTENV *pstEnv = NULL;

	pstEnv = (DIRREPORTENV *)malloc(sizeof(DIRREPORTENV));
	if(pstEnv == NULL)
	{
		return -1;
	}
	
	pstEnv->iSocket = -1;
	pstEnv->pstSSHan = NULL;
	
	pstEnv->iSocket = tnet_connect(DIR_REPORT_ADDR, 0);
	if (0 > pstEnv->iSocket)
	{
		printf("tnet_connect is fial\n");
		destory_reprot_interface((int)pstEnv);
		return -1;
	}

	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		printf("ss_han_create is fial\n");
		destory_reprot_interface((int)pstEnv);
		return -1;
	}
	
	*pHandle = (int)pstEnv;
	return 0;
}

void destory_reprot_interface(int iHandle)
{
	DIRREPORTENV *pstEnv = (DIRREPORTENV *)iHandle;

	if(pstEnv == NULL)
	{	
		return;
	}
	
	if(pstEnv->iSocket != -1)
	{
		tnet_close(pstEnv->iSocket);
	}

	if(pstEnv->pstSSHan != NULL)
	{
		free(pstEnv->pstSSHan);
		pstEnv->pstSSHan = NULL;
	}
	
	free(pstEnv);
}

static int send_report(int iHandle, char *szData,int iTimeOut,int iCmd)
{
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	DIRREPORTENV *pstEnv = (DIRREPORTENV *)iHandle;

	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	stSSPkg.Head.Magic = PROTOCOL_MAGIC;
	stSSPkg.Head.HeadLen = sizeof(stSSPkg.Head);

	if (iCmd == RECORD_DIR_INFO)
	{
		stSSPkg.Head.Cmd = RECORD_DIR_INFO;
		memcpy(&stSSPkg.Body.RePortDataAll, szData, sizeof(SSREPORTDATAALL));
	}
	else if(iCmd == DIR_INFO_HOUR)
	{
		stSSPkg.Head.Cmd = DIR_INFO_HOUR;
		memcpy(&stSSPkg.Body.DirInfoHour, szData, sizeof(stSSPkg.Body.DirInfoHour));
	}
	else
	{
		return -1;
	}

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) <0 )
	{
		printf("ss_pack is fail\n");
		return -1;
	}

	if(tnet_send(pstEnv->iSocket, stNet.pszBuff, stNet.iBuff, iTimeOut) < 0)
	{
		printf("tnet_send is fail\n");
		return -1;
	}
	
	return 0;
}


static REPORTDATA* find_world_of_reportdata(int iWorld, int *pI)
{
	int i,j;
	SSREPORTDATAALL * pstReport;
	int iNum = REPORT_SIZE(MAX_WORLD_NUM, REPORT_MAX_WORLD);

	// iWorld 是否存在  存在就找到相应的位置
	for(i=0; i<iNum; i++)
	{
		pstReport = &g_stRePortAll[i];
		for(j=0; j<pstReport->WorldNum; j++)
		{
			if (pstReport->RePort[j].DataNum > 0 && pstReport->RePort[j].WorldID == iWorld)
			{
				*pI = i;
				return &pstReport->RePort[j];
			}
		}
	}

	//iWorld 不存在   
	for(i=0; i<iNum; i++)
	{
		if(g_stRePortAll[i].WorldNum < REPORT_MAX_WORLD)
		{
			*pI = i;
			return &g_stRePortAll[i].RePort[g_stRePortAll[i].WorldNum ++];
		}
	}

	return NULL;
}

int record_data(int iworldID, int iOnlineNum)
{
	struct tm stTm;
	int  iSeconds;
	REPORTDATA *pstData;
	
	int i;
	int iHour;
	int iIdx = -1;
	
	iSeconds = time((time_t*)NULL);
	localtime_r((time_t*)&iSeconds, &stTm);
	iHour = stTm.tm_hour;

	if(g_iSec == 0)
	{
		g_iSec = iSeconds;
		memset( &g_stRePortAll, 0, sizeof(g_stRePortAll));
		for (i=0; i<REPORT_SIZE(MAX_WORLD_NUM, REPORT_MAX_WORLD); i++)
		{
			g_stRePortAll[i].time = iSeconds;
		}
	}

	if(g_iSeconds == 0)
	{
		g_iSeconds = iSeconds;
	}
	
	pstData = find_world_of_reportdata(iworldID, &iIdx);
	if(iIdx == -1 || pstData == NULL)
	{
		return -1;
	}
	
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
		if (pstData->WorldID <= 0)
		{
			pstData->WorldID = iworldID;
		}
		
		pstData->DirHourData[pstData->DataNum].Hour= iHour;
		pstData->DirHourData[pstData->DataNum].PlayerNum = iOnlineNum;
		pstData->DataNum ++;
	}
	
	return 0;

}

int is_same_hour(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;
	localtime_r(&t1,&curr1);
	localtime_r(&t2,&curr2);

	if(curr1.tm_year == curr2.tm_year && curr1.tm_yday == curr2.tm_yday && curr1.tm_hour == curr2.tm_hour)
	{
		return 1;
	}
	return 0;
}

int is_same_min(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;
	localtime_r(&t1,&curr1);
	localtime_r(&t2,&curr2);

	if(curr1.tm_year == curr2.tm_year && curr1.tm_yday == curr2.tm_yday 
		&& curr1.tm_hour == curr2.tm_hour && curr1.tm_min == curr2.tm_min)
	{
		return 1;
	}
	return 0;
}

static int hourdata_find(HOURDATA *pstHourData, int iDataNum, int iHour, int *pI)
{
	int i;
	HOURDATA *pstData;
	
	*pI  = -1;
	for (i=0; i<iDataNum; i++)
	{
		pstData = pstHourData+i;
		if(pstData->Hour == iHour)
		{
			*pI = i;
			break;
		}
	}

	return *pI;
}

static int send_report_of_hour(int iSeconds, int iHandle, int iTimeOut)
{
	SSDIRINFOHOUR stDirInfoHour;
	struct tm stTm;
	int i,j;
	SSREPORTDATAALL *pstAll;
	REPORTDATA *pstRePortData;
	int iIdx;

	if (is_same_hour(iSeconds, g_iSeconds) == 0)
	{
		memset(&stDirInfoHour, 0, sizeof(SSDIRINFOHOUR));
		localtime_r((time_t*)&g_iSeconds, &stTm);
		stDirInfoHour.Hour = stTm.tm_hour;
		stDirInfoHour.Time = g_iSeconds;
		
		for (i=0; i<REPORT_SIZE(MAX_WORLD_NUM, REPORT_MAX_WORLD); i++)
		{
			pstAll = &g_stRePortAll[i];
			for(j=0; j<pstAll->WorldNum; j++)
			{
				pstRePortData = &pstAll->RePort[j];
				if(hourdata_find(pstRePortData->DirHourData, pstRePortData->DataNum, stDirInfoHour.Hour, &iIdx) < 0)
				{
					continue;
				}
				stDirInfoHour.WorldInfoHour[stDirInfoHour.WorldNum].WorldID = pstRePortData->WorldID;
				stDirInfoHour.WorldInfoHour[stDirInfoHour.WorldNum].OnLineNum = pstRePortData->DirHourData[iIdx].PlayerNum;
				stDirInfoHour.WorldNum ++;
			}
		}

		if(stDirInfoHour.WorldNum > 0)
		{
			if (send_report(iHandle, (char *)&stDirInfoHour, iTimeOut, DIR_INFO_HOUR) < 0)
			{
				//destory_reprot_interface(iHandle);
				return -1;
			}

			g_iSeconds = iSeconds;
		}
		
	}

	return 0;
}

static int send_report_of_day(int iSeconds, int iHandle, int iTimeOut)
{
	int i;
	SSREPORTDATAALL *pstAll;

	if (IsSameDay (iSeconds, g_iSec) == 0)
	{
		for (i=0; i<REPORT_SIZE(MAX_WORLD_NUM, REPORT_MAX_WORLD); i++)
		{
			if(g_stRePortAll[i].WorldNum > 0)
			{
				if (send_report(iHandle, (char *)&g_stRePortAll[i], iTimeOut, RECORD_DIR_INFO) < 0)
				{
					//destory_reprot_interface(iHandle);
					return -1;
				}
			}
		}
		
		memset( &g_stRePortAll, 0, sizeof(g_stRePortAll));
		g_iSec = iSeconds;
		
		for (i=0; i<REPORT_SIZE(MAX_WORLD_NUM, REPORT_MAX_WORLD); i++)
		{
			pstAll = &g_stRePortAll[i];
			pstAll->time = iSeconds;
		}
	}

	return 0;
}

int report_data(int iHandle,int iTimeOut)
{
	int  iSeconds = time((time_t*)NULL);

	if ( send_report_of_hour(iSeconds, iHandle, iTimeOut) < 0)
	{
		return -1;
	}

	if ( send_report_of_day(iSeconds, iHandle, iTimeOut) < 0)
	{
		return -1;
	}
	
	return 0;
}
