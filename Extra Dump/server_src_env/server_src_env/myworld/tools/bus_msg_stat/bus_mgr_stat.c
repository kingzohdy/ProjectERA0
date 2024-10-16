/** @file $RCSfile: bus_mgr_stat.c,v $
  general description of this module
  $Id: bus_mgr_stat.c,v 1.1 2013/01/17 08:45:57 kent Exp $
@author $Author: kent $
@date $Date: 2013/01/17 08:45:57 $
@version $Revision: 1.1 $
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/

#include "tdr/tdr.h"
#include "tbus/tbus.h"
#include "tbus_config_mng.h"
#include "tbus_kernel.h"
#include "tconnapi.h"

int stat_output_file(char *pszFile, int iChnIdx, int iUpDown, char *pszShm, int iBussID)
{
	int iRet;
	LPTBUSSHMGCIM pShmGCIM;
	LPTBUSSHMGCIMHEAD pstHead ;
	LPTBUSSHMCHANNELCNF pstShmChl;
	TBUSCHANNEL stChannel;
	CHANNELVAR *pstChanelVar;
	int iDataLen;
	char *pszTmp;
	FILE *file;
	int iMsg;
	TBUSHEAD stHead;
	int iHSeq = 0;
	CHANNELHEAD stChannelHead;

	iRet = tbus_initialize(pszShm, iBussID);
	if (TBUS_SUCCESS != iRet)
	{
		printf("tbus init error\n");	
		return -1;
	}
	
	iRet = tbus_get_gcimshm(&pShmGCIM, pszShm,  iBussID, 0, 0);
	if (0 > iRet)
	{
		return -1;
	}

	pstHead = &pShmGCIM->stHead;
	if ((0 >= iChnIdx) || (iChnIdx > (int)pstHead->dwUsedCnt))
	{
		return -1;
	}
	
	if (0 > iUpDown || iUpDown > 1)
	{
		return -1;
	}

	pstShmChl = &pShmGCIM->astChannels[iChnIdx-1];
	iRet = tbus_attach_channel(pstShmChl->astAddrs[0], pstShmChl, &stChannel, pstHead);		
	if (0 > iRet)
	{
		return -1;
	}

	pstChanelVar = &stChannel.pstHead->astQueueVar[iUpDown];

	pszTmp = malloc(pstChanelVar->dwSize+sizeof(*pstChanelVar)+sizeof(stChannelHead));
	if (NULL == pszTmp)
	{
		return -1;
	}

	memcpy(&stChannelHead,  stChannel.pstHead, sizeof(stChannelHead));
	
	memcpy(pszTmp, pstChanelVar, sizeof(*pstChanelVar));
	memcpy(pszTmp+sizeof(*pstChanelVar), &stChannelHead, sizeof(stChannelHead));
	memcpy(pszTmp+sizeof(*pstChanelVar)+sizeof(stChannelHead), stChannel.pszQueues[iUpDown], pstChanelVar->dwSize);

	iDataLen = pstChanelVar->dwTail - pstChanelVar->dwHead;
	if (iDataLen < 0) 
	{
		iDataLen += pstChanelVar->dwSize;
	}	

	if ( (0 < iDataLen) && 
		(TBUS_SUCCESS == tbus_get_pkghead(&stHead, stChannel.pszQueues[iUpDown], pstChanelVar->dwSize, (unsigned int*)&pstChanelVar->dwHead, pstChanelVar->dwTail)))
	{
		iHSeq = stHead.iSeq;
	}
	
	if (0 < iDataLen)
	{
		iMsg = pstChanelVar->iSeq - iHSeq;
	}else
	{
		iMsg = 0;
	}

	printf("msg num %u\n", iMsg);
	
	file = fopen(pszFile, "wb");
	if (NULL == file)
	{
		return -1;
	}

	fwrite(pszTmp, pstChanelVar->dwSize+sizeof(*pstChanelVar)+sizeof(stChannelHead), 1, file);
	
	return 0;
}

int stat_dump_one_msg(LPTDRMETA	pstMsgMeta, char *pszMsg, int iMsg, FILE *file1)
{
	TFRAMEHEAD stFrameHead;
	TDRDATA stHost;
	TDRDATA stNet;
	int iRet;
	char *pszData;
	int iData;
	char szHost[128000];
	
	stHost.pszBuff	=	&stFrameHead;
	stHost.iBuff	=	(int)sizeof(stFrameHead);

	stNet.pszBuff	=	pszMsg;
	stNet.iBuff	=	iMsg;
	iRet=TCONNAPI_FRAMEHEAD_NTOH(&stHost, &stNet, 0);
	if (0 > iRet)
	{
		return -1;
	}

	pszData = pszMsg + stNet.iBuff;
	iData = iMsg - stNet.iBuff;

	stHost.iBuff = sizeof(szHost);
	stHost.pszBuff = szHost;
	stNet.pszBuff = pszData;
	stNet.iBuff = iData;
	iRet = tdr_ntoh(pstMsgMeta, &stHost, &stNet, 0);
	if (0 > iRet)
	{
		return -1;
	}
	
	if (0 < stHost.iBuff)
	{
		if (stFrameHead.Cmd == TFRAMEHEAD_CMD_INPROC &&
			stFrameHead.CmdData.InProc.Count  > 1)
		{
			fprintf(file1, "group count %d\n", stFrameHead.CmdData.InProc.Count);
		}
		
		tdr_fprintf(pstMsgMeta, file1, &stHost, 0);
	}
	
	return 0;
}

int stat_do_dump(char *pShm, int BussID, char *pszInFile, char *pszLibFile, char *pszOutFile)
{
	char *pszTmp;
	LPTDRMETALIB pstLib;
	LPTDRMETA	pstMsgMeta;
	FILE *file, *file1;
	struct stat stStat;
	CHANNELVAR stChanelVar;
	CHANNELVAR *pstVar = &stChanelVar;
	int iHead, iTail, iSize, iData;
	TBUSHEAD stHead;
	int iPkg;
	char *pszPkg = NULL;
	CHANNELHEAD stChnHead;
	int iTotal = 0;
	
	if (0 > tdr_load_metalib(&pstLib, pszLibFile))
	{
		return -1;
	}

	tconnapi_initialize(pShm, BussID);

	pstMsgMeta = tdr_get_meta_by_name(pstLib, "CSPkg");
	if (NULL == pstMsgMeta)
	{
		return -1;
	}

	if (0 != stat(pszInFile, &stStat))
	{
		return -1;
	}

	pszTmp = malloc(stStat.st_size);
	if (NULL == pszTmp)
	{
		return -1;
	}

	file = fopen(pszInFile, "rb");
	if (NULL == file)
	{
		return -1;
	}

	fread(&stChanelVar, sizeof(stChanelVar), 1, file);
	fread(&stChnHead, sizeof(stChnHead), 1, file);
	fread(pszTmp, stStat.st_size-sizeof(stChanelVar)-sizeof(stChnHead), 1, file);
	fclose(file);

	iHead =pstVar->dwHead;
	iTail = pstVar->dwTail;
	iSize = pstVar->dwSize;
	if( iHead==iTail )
		return 0;

	iData = iTail - iHead;
	if (iData< 0) iData	+=	iSize;

	file1 = fopen(pszOutFile, "w");
	if (file1 == NULL)
	{
		return -1;
	}

	while( iData>0 )
	{
		if (TBUS_SUCCESS != tbus_get_pkghead(&stHead, pszTmp, iSize, (unsigned int *)&iHead, iTail))
		{
			break;
		}

		iPkg	=	stHead.bHeadLen + stHead.iBodyLen;
		TBUS_CALC_ALIGN_VALUE_BY_LEVEL(iPkg, stChnHead.dwAlignLevel);
		if( iData < iPkg )
			break;

		assert(0 <= iHead);
		pszPkg = pszTmp + iHead + stHead.bHeadLen;

		iTotal++;
		
		stat_dump_one_msg(pstMsgMeta, pszPkg, stHead.iBodyLen, file1);
	
		tbus_moveto_next_pkg(&stHead, &stChnHead, iSize, &iHead, iTail);
		iData	-=	iPkg;
	}

	printf ("total msg %d\n", iTotal);
	
	return 0;
}

int main ( int argc, char ** argv ) 
{
	int iWay; //0 只拷贝到文件 1 从文件读取打印2 统计
	
	iWay = atoi(argv[1]);

	switch(iWay)
	{
	case 0:
		stat_output_file(argv[2], atoi(argv[3]), atoi(argv[4]), argv[5], atoi(argv[6]));
		break;
	case 1:
		stat_do_dump(argv[2], atoi(argv[3]), argv[4], argv[5], argv[6]);
		break;
	case 2:
		break;
	default:
		break;
	}
	
	return 0;
}

