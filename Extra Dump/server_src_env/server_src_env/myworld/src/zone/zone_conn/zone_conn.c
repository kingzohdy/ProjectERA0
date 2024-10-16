/*
**  @file RCSfile
**  general description of this module
**  Id
**  @author Author
**  @date Date
**  @version Revision
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "zone_conn_conf_desc.h"
#include "version.h"
#include "comm_misc.h"
#include "tconnapi.h"
#include "tconnmgr.h"
#include "tconnpool.h"
#include "tbus/tbus.h"
#include "cs_net.h"
#include "entity.h"

#define TIME_SLICE_COUNT                10
#define PAK_PER_SLICE                   300
#define MAX_EVENTS		                8192
#define TCONND_WAIT_TIME               3 
//#define SHORT_COUNT_BUFF             4000

#define UNUSED(param) (void)param

static time_t gs_tNow	=	0;

extern const unsigned char sig_key[];

#ifdef WIN32
	#if defined (LITTLEENDIAN) && LITTLEENDIAN >0 
		#define TDR_OS_LITTLEENDIAN

		#if defined (TDR_OS_BIGENDIAN)
		#undef TDR_OS_BIGENDIAN
		#endif
	#else
		#define TDR_OS_BIGENDIAN
		#if defined (TOS_LITTLEENDIAN)
		#undef TDR_OS_LITTLEENDIAN
		#endif
	#endif

#else /* WIN32 */
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define TDR_OS_LITTLEENDIAN

		#if defined (TDR_OS_BIGENDIAN)
		#undef TDR_OS_BIGENDIAN
		#endif
	#else
		#define TDR_OS_BIGENDIAN
		#if defined (TDR_OS_LITTLEENDIAN)
		#undef TDR_OS_LITTLEENDIAN
		#endif
	#endif
#endif

#if defined(WIN32) &&  _MSC_VER < 1300
#define tdr_os_swap64(x) \
	((((x) & (tdr_ulonglong)0xff00000000000000) >> 56)                                   \
	| (((x) & (tdr_ulonglong)0x00ff000000000000) >> 40)                                 \
	| (((x) & (tdr_ulonglong)0x0000ff0000000000) >> 24)                                 \
	| (((x) & (tdr_ulonglong)0x000000ff00000000) >> 8)                                  \
	| (((x) & (tdr_ulonglong)0x00000000ff000000) << 8)                                  \
	| (((x) & (tdr_ulonglong)0x0000000000ff0000) << 24)                                 \
	| (((x) & (tdr_ulonglong)0x000000000000ff00) << 40)                                 \
	| (((x) & (tdr_ulonglong)0x00000000000000ff) << 56))
#else
#define tdr_os_swap64(x) \
	((((x) & (tdr_ulonglong)0xff00000000000000LL) >> 56)                                   \
	| (((x) & (tdr_ulonglong)0x00ff000000000000LL) >> 40)                                 \
	| (((x) & (tdr_ulonglong)0x0000ff0000000000LL) >> 24)                                 \
	| (((x) & (tdr_ulonglong)0x000000ff00000000LL) >> 8)                                  \
	| (((x) & (tdr_ulonglong)0x00000000ff000000) << 8)                                  \
	| (((x) & (tdr_ulonglong)0x0000000000ff0000) << 24)                                 \
	| (((x) & (tdr_ulonglong)0x000000000000ff00) << 40)                                 \
	| (((x) & (tdr_ulonglong)0x00000000000000ff) << 56))

#endif


#define tdr_os_swap32(x) \
	((((x) & 0xff000000) >> 24)                                  \
	| (((x) & 0x00ff0000) >> 8)                                 \
	| (((x) & 0x0000ff00) << 8)                                 \
	| (((x) & 0x000000ff) << 24))                                  
	
#define tdr_os_swap16(x) \
	((((x) & 0xff00) >> 8)                                  \
	| (((x) & 0x00ff) << 8))                                 
              

#ifdef TDR_OS_LITTLEENDIAN
#define tdr_ntohq(x)                    tdr_os_swap64(x)
#define tdr_htonq(x)                    tdr_os_swap64(x)
#define tdr_ntohl_(x)                    tdr_os_swap32(x)
#define tdr_htonl_(x)                    tdr_os_swap32(x)
#define tdr_ntohs_(x)                    tdr_os_swap16(x)
#define tdr_htons_(x)                    tdr_os_swap16(x)
#else
#define tdr_ntohq(x)                    (x)
#define tdr_htonq(x)                    (x)
#define tdr_ntohl_(x)                   (x)
#define tdr_htonl_(x)                   (x)
#define tdr_ntohs_(x)                   (x)
#define tdr_htons_(x)                   (x)
#endif


#define TDR_GET_INT_NET(i, iSize, p)		switch(iSize)		      \
{									      \
	case 2:								      \
		i = (int)ntohs(*(unsigned short*)(p));			      \
		break;							      \
	case 4:								      \
		i = (int)ntohl(*(unsigned long*)(p));			      \
		break;							      \
	case 8:									\
		i = tdr_ntohq(*(tdr_ulonglong*)p);	\
		break;												\
	default:							      \
		i = (int)*(unsigned char*)(p);				      \
}

typedef struct tagUinQue 
{
	int  iUin;	 
	TCONNINST* pstInst;
}UinQue;

struct tagzone_connEnv
{
	ZONE_CONNCONF *pstConf;
	ZONE_CONNRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	CONFINST stConfInst;
	TCONNTHREAD stThread;
};

typedef struct tagzone_connEnv		ZONE_CONNENV;
typedef struct tagzone_connEnv		*LZONE_CONNENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_zone_conn[];

ZONE_CONNENV gs_stEnv;
static ZONE_CONNRUN_STATUS gs_stStat;

int tconnd_scan(TCONNTHREAD* pstThread, int iIsShutdown);

/*
int send_short_count(TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	int iRet = 0;
	int iSend = 0;

	if (pstInst->iShortCountBuff <= 0)
	{
		return 0;
	}

	if( pstInst->fStream )
	{
		iSend	= send(pstInst->s, pstInst->szBuff+pstInst->iBuff, pstInst->iShortCountBuff, 0);
	}
	
	if (iSend > 0)
	{
		pstInst->iShortCountBuff -= iSend;
		if (pstInst->iShortCountBuff > 0)
		{
			memmove(pstInst->szBuff+pstInst->iBuff, pstInst->szBuff+pstInst->iBuff+iSend, pstInst->iShortCountBuff);
		}
	}
	else if (iSend < 0)
	{
		if (EAGAIN == errno)
		{
			return 0;
		}
	}
	else
	{
		iRet = -1;
	}
	
	return iRet;
}

int store_short_count(TCONNINST* pstInst, char *pszBuff, int iBuff)
{
	int iRet = 0;

	if (pstInst->iShortCountBuff + iBuff > SHORT_COUNT_BUFF)
	{
		return -1;
	}

	memcpy(pstInst->szBuff+pstInst->iBuff+pstInst->iShortCountBuff, pszBuff, iBuff);

	pstInst->iShortCountBuff += iBuff;
	
	return iRet;
}*/

int UinQueCmp( const void * pv1 , const void *pv2 )
{
	UinQue *pstUinQueData1 = (UinQue *)pv1;
	UinQue *pstUinQueData2 = (UinQue *)pv2;

	return pstUinQueData1->iUin - pstUinQueData2->iUin;
}

unsigned int UinQueCode(const void* pvData)
{
	UinQue *pstUinShtData = (UinQue *)pvData;
	
	return (unsigned int)pstUinShtData->iUin;
}

static inline void init_base(PDUBASE* pstBase)
{
	pstBase->Magic	=	PDU_MAGIC;
	pstBase->Version	=	PDU_VERSION;
	pstBase->Cmd		=	PDU_CMD_NONE;
	pstBase->HeadLen	=	(unsigned char)sizeof(PDUBASE);
	pstBase->BodyLen	=	0;
}

int tconnd_get_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread)
{
	TRANSINST* pstTransInst;
	PDUINST* pstPDUInst;
	char* pszBuff;
	LPPDULENTDRPARSERINST pstTDRParser; 
	char *pszDataBase;

	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	pstPDUInst	=	pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;
	pstTDRParser = &pstPDUInst->stLenParser.stTDRParser;

	pszDataBase = pstInst->szBuff + pstInst->iOff; 
	if( 0 < pstTDRParser->iPkgLenUnitSize )
	{
		if( pstInst->iData < (pstTDRParser->iPkgLenNOff + pstTDRParser->iPkgLenUnitSize))
		{
			return -1;
		}

		pszBuff	=	pszDataBase + pstTDRParser->iPkgLenNOff;

		TDR_GET_INT_NET(pstInst->iPkgLen, pstTDRParser->iPkgLenUnitSize, pszBuff);

	}
	else
	{
		if( pstInst->iData < (pstTDRParser->iHeadLenNOff + pstTDRParser->iHeadLenUnitSize))
		{
			return -1;
		}

		pszBuff		=	pszDataBase + pstTDRParser->iHeadLenNOff;

		TDR_GET_INT_NET(pstInst->iHeadLen, pstTDRParser->iHeadLenUnitSize, pszBuff);

		if( pstInst->iData < (pstTDRParser->iBodyLenNOff + pstTDRParser->iBodyLenUnitSize))
		{
			return -1;
		}

		pszBuff		=	pszDataBase + pstTDRParser->iBodyLenNOff;

		TDR_GET_INT_NET(pstInst->iPkgLen, pstTDRParser->iBodyLenUnitSize, pszBuff);

		pstInst->iPkgLen	+=	pstInst->iHeadLen;
	}

	if( pstInst->iPkgLen<=0 || pstInst->iPkgLen > pstPDUInst->iUnit)
	{
		pstInst->iNeedFree	=	STOP_REASON_BADPKG;
		pstInst->iNotify	=	1;
		return -1;
	}
	else
	{
		return 0;
	}
}

int tconnd_get_thttp_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread)
{
	TRANSINST* pstTransInst;
	PDUINST* pstPDUInst;
	char *pszDataBase;
	int iMaxLen;
	int i;

	assert(NULL != pstInst);
	assert(NULL != pstThread);
	assert(0 <= pstInst->iPkgLen);

	if (0 >= pstInst->iData)
	{
		return -1;
	}
	
	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	pstPDUInst	=	pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;

	
	/*分析数据流，以'\0'字节进行分包*/
	iMaxLen = pstInst->iData;
	if (iMaxLen > pstPDUInst->iUpUnit)
	{
		iMaxLen = pstPDUInst->iUpUnit;
	}	
	pszDataBase = pstInst->szBuff + pstInst->iOff;
	for (i = pstInst->iPkgLen; i < iMaxLen; i++)
	{
	   if ('\0' == pszDataBase[i])
	   {
		   break;
	   }
	}/*for (i = pstInst->iPkgLen; i < iMaxLen; i++)*/
	if (i >= iMaxLen)
	{
		pstInst->iPkgLen = i;
		return -1;
	}
	else
	{
		pstInst->iPkgLen = i + 1;
	}
	
	
	if ( pstInst->iPkgLen > pstPDUInst->iUpUnit) 
	{
		/*上行包超过最大长度限制，则直接断开连接*/
		pstInst->iNeedFree	=	STOP_REASON_BADPKG;
		pstInst->iNotify	=	1;
		return -1;
	}

	return 0;
}

int tconnd_get_none_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread)
{
	(void)pstThread;
	
	if (0 >= pstInst->iData)
	{
		return -1;
	}
	else
	{
		pstInst->iPkgLen	=	pstInst->iData;
		return 0;
	}
}

int tconnd_get_auth_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread)
{
	TRANSINST* pstTransInst;
	PDUINST* pstPDUInst;
	char* pszBuff;
	LPPDULENAUTHPARSERINST pstAuthParser; 
	char *pszDataBase;

	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	pstPDUInst	=	pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;
	pstAuthParser = &pstPDUInst->stLenParser.stAuthParser;

	pszDataBase = pstInst->szBuff + pstInst->iOff; 

	if( pstInst->iData < (pstAuthParser->iHeadLenNOff + pstAuthParser->iHeadLenUnitSize))
	{
		return -1;
	}

	pszBuff		=	pszDataBase + pstAuthParser->iHeadLenNOff;

	TDR_GET_INT_NET(pstInst->iHeadLen, pstAuthParser->iHeadLenUnitSize, pszBuff);

	if( pstInst->iData < (pstAuthParser->iBodyLenNOff + pstAuthParser->iBodyLenUnitSize))
	{
		return -1;
	}

	pszBuff		=	pszDataBase + pstAuthParser->iBodyLenNOff;

	TDR_GET_INT_NET(pstInst->iPkgLen, pstAuthParser->iBodyLenUnitSize, pszBuff);

	pstInst->iPkgLen	+=	pstInst->iHeadLen;

	if( pstInst->iPkgLen<=0 || pstInst->iPkgLen > pstPDUInst->iUpUnit)
	{
		pstInst->iNeedFree	=	STOP_REASON_BADPKG;
		pstInst->iNotify	=	1;
		return -1;
	}
	else
	{
		return 0;
	}
}

static int tconnd_low_stop_conn(TCONNTHREAD* pstThread, int iSocket,
                                PDUINST *pstPDUInst, int iReason)
{
	PDUHEAD stHead;
	TDRDATA stNet, stHost;

	init_base(&stHead.Base);
	stHead.Base.Cmd	= PDU_CMD_STOP;
	stHead.Ext.Stop.StopReason = iReason;

	stHost.pszBuff = (char*)&stHead;
	stHost.iBuff = (int)sizeof(stHead);
	stNet.pszBuff = pstThread->pszSendBuff;
	stNet.iBuff	= pstThread->iSendBuff;

	if(tdr_hton(pstPDUInst->stLenParser.stAuthParser.pstMetaHead, &stNet,
	            &stHost, PDU_VERSION)<0 )
	{
	    tlog_error(pstThread->pstLog, 0,0,
	               "notify connection ignroed, tdr_ntoh failed");
	}

	tnet_set_nonblock(iSocket, 1);
	tnet_send(iSocket, stNet.pszBuff, stNet.iBuff, 0);
	return 0;
}

int show_sock_opt(int s)
{
 	int iBuff;
 	socklen_t iSize;
 	
 	iSize	=	sizeof(iBuff);
 	if (0 > getsockopt(s, SOL_SOCKET, SO_RCVBUF, (void*)&iBuff, &iSize))
 	{
 		return -1;
 	}
 	printf("recv buff %d\n", iBuff);
 	
 	iSize	=	sizeof(iBuff);
 	if (0 > getsockopt(s, SOL_SOCKET, SO_SNDBUF, (void*)&iBuff, &iSize))
 	{
 		return -1;
 	}
 	printf("send buff %d\n", iBuff);
 	
 	return 0;
}

int tconnd_add_conn(TCONNTHREAD* pstThread, int s, int iTransLoc, int iLisLoc, int iIsListen, struct sockaddr* pstAddr)
{
	ZONE_CONNCONF* pstConnd;
	ZONE_CONNRUN_STATUS* pstRun;
	CONFINST* pstConfInst;
	LISTENER* pstListener;
	TRANSINST* pstTransInst;
	NETTRANS* pstNetTrans;
	TCONNINST* pstInst;
	LISTENERRUN* pstLisRun;
	int iNeedFree = 0;


	int iRet;
	int iType;
	epoll_event_t e;

	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;
	pstConfInst	=	pstThread->pstConfInst;
	pstRun		=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstRunStat;

	pstTransInst	=	pstConfInst->stTransInstList.astInsts + iTransLoc;

	pstListener	=	pstConnd->ListenerList.Listeners + iLisLoc;
	pstNetTrans	=	pstConnd->NetTransList.NetTrans + iTransLoc;

	pstLisRun	=	pstRun->ListenerRunList.Listeners + iLisLoc;

	if( -1==s )
	{
		if( iIsListen )
		{
			tlog_error(pstThread->pstLog, 0, 0, "listen on %s failed.\n", pstListener->Url);
		}
		else
		{
			tlog_error(pstThread->pstLog, 0, 0, "accept from epoll failed.errorstring=%s\n",strerror(errno));
		}

		return -1;
	}

	if(pstNetTrans->ConnLimit.MaxConn &&
	   pstTransInst->stConnInfo.Total >= pstNetTrans->ConnLimit.MaxConn)
	{
		tconnd_low_stop_conn(pstThread, s, pstConfInst->stPDUInstList.astInsts +
	                         pstTransInst->iPDULoc, PDU_STOP_CONNECTION_REFUSED);

		tlog_debug(pstThread->pstLog, 0, 0,
		           "nettrans \'%s\' reach max conn limit %d. \n",
		           pstNetTrans->Name, pstNetTrans->ConnLimit.MaxConn);
		iNeedFree = STOP_REASON_LIMIT;
	}

	iType	=	SOCK_STREAM;

	tsocket_get_type(s, &iType);
	
	iRet	=	tnet_set_nonblock(s, 1);
	if( iRet<0 )
	{
		tnet_close(s);
		tlog_error(pstThread->pstLog, 0, 0, "set socket to non-block failed. \n");
		return -1;
	}

	if(iIsListen ) //accept的socket会继承
	{
		if( pstListener->SendBuff>0 )
		{
			tnet_set_sendbuff(s, pstListener->SendBuff);
		}
		if( pstListener->RecvBuff>0 )
		{
			tnet_set_recvbuff(s, pstListener->RecvBuff);
		}
	}

	//show_sock_opt(s);
	
	e.events	=	EPOLLIN;
	e.data.fd	=	tconnd_alloc_inst(pstThread->pstPool);
	
	if(e.data.fd<0)
	{
		tnet_close(s);
		tlog_error(pstThread->pstLog, 0, 0, "alloc conn instance for listen socket failed.\n");
		return -1;
	}

	(&e.data.fd)[1]	=	s;
	
	pstInst		=	tconnd_get_inst(pstThread->pstPool, e.data.fd);

	memset(pstInst, 0, offsetof(TCONNINST, szBuff));

	pstInst->s		=	s;
	pstInst->iIdx		=	e.data.fd;
	pstInst->iTransLoc	=	iTransLoc;
	pstInst->iLisLoc	=	iLisLoc;
	pstInst->fListen	=	iIsListen;
	pstInst->iNeedFree  =   iNeedFree;

	// 影子路由默认关闭:
	pstInst->stShadowRouting.fEnabled = 0;
	pstInst->stShadowRouting.iRealBusID = 0;
	pstInst->stShadowRouting.iReplacedIdx = 0;

	if( SOCK_STREAM==iType )
	{
		pstInst->fStream	=	1;
	}
	else
	{
		pstInst->fStream	=	0;
	}

	if( !iIsListen && SOCK_STREAM==iType )
	{
		pstInst->fWaitFirstPkg	=	1;
	}
	else
	{
		pstInst->fWaitFirstPkg	=	0;
	}

	pstInst->iID		=	-1;

	pstInst->tCreate	=	gs_tNow;
	pstInst->tLastRecv	=	gs_tNow;
			
	pstInst->iQueuePrev	=	-1;
	pstInst->iQueueNext	=	-1;

	if( pstAddr )
	{
		memcpy(&pstInst->stAddr, pstAddr, sizeof(pstInst->stAddr));
	}
	else
	{
		memset(&pstInst->stAddr, 0, sizeof(pstInst->stAddr));
	}

	pstInst->iBuff  	=	pstThread->iPkgUnit;
	pstInst->iUseTimeStamp	=	pstNetTrans->UseTimeStamp;

	if(!iNeedFree && -1 == epoll_ctl(pstThread->epfd, EPOLL_CTL_ADD, s, &e))
	{
		pstLisRun->ConnStat.EpollFail++;

		tnet_close(s);
		tconnd_free_inst(pstThread->pstPool, e.data.fd);

		tlog_error(pstThread->pstLog, 0, 0, "add listen socket to epoll-fd failed.\n");

		return -1;
	}

	if (!iIsListen)
	{
		pstTransInst->stConnInfo.Total++;
		pstTransInst->stConnInfo.Idle++;

		tlog_debug(pstThread->pstLog, 0, 0, "Add one socket(%d) to connInst(%d), peer info(%s:%d)",
			pstInst->s, pstInst->iIdx, inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr), 
				NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port));
	}

	return 0;
}



int tconnd_init_thread(TAPPCTX* pstAppCtx, ZONE_CONNENV* pstEnv)
{
	int i;
	int j;
	int s;
	int iRet;
	int iLimit;
	int iUpUnit = 0;
	int iBuff = 0;
	ZONE_CONNCONF* pstConnd;
	TCONNPOOL* pstPool;
	CONFINST* pstConfInst;
	NETTRANSLIST* pstNetTransList;
	LISTENERLIST* pstListenerList;
	NETTRANS* pstNetTrans;
	TRANSINST* pstTransInst;
	SERIALIZERLIST* pstSerializerList;
	LISTENER* pstListener;
	PDUINST* pstPDUInst;
	TCONNTHREAD* pstThread;
	int iShtUinQue = 0;

	pstThread	=	&pstEnv->stThread;
	pstThread->pstAppCtx	=	pstAppCtx;
	pstThread->pstEnv = pstEnv;
	pstThread->pstConfInst	=	&pstEnv->stConfInst;

	if (pstAppCtx->iTimer > 500)
	{
		printf("tick timer=%d, but recommend config < 500\n", pstAppCtx->iTimer);
		return -1;
	}	

	pstThread->iScanHerz	=	0;
	if( pstAppCtx->iTimer )
	{
		pstThread->iScanHerz	=	1000/pstAppCtx->iTimer;
	}

	if( pstThread->iScanHerz<=0 )
	{
       	pstThread->iScanHerz = 1;
	}

	pstThread->iRecvSlices = TIME_SLICE_COUNT/2;
	pstThread->iMsRecvPerLoop = (pstThread->iRecvSlices*pstThread->pstAppCtx->iTimer)/TIME_SLICE_COUNT;
	pstThread->iWaitToSend=0;
	pstThread->iLastSrc	=	TBUS_ADDR_ALL;

	pstThread->pstLog	=	pstEnv->pstLogCat;
	
	pstConfInst		=	&pstEnv->stConfInst;
	pstConnd		=	pstEnv->pstConf;

	pstNetTransList	=	&pstConnd->NetTransList;
	pstListenerList	=	&pstConnd->ListenerList;
	pstSerializerList=	&pstConnd->SerializerList;

	iLimit	=	pstConnd->MaxFD;

	tlog_debug_dr(pstThread->pstLog, 0, 0, (LPTDRMETA)pstAppCtx->stConfData.iMeta,
					pstConnd, sizeof(*pstConnd), 0);
	if( iLimit<=0 )
	{
		iLimit	=	0;

		for(i=0; i<pstConnd->NetTransList.Count; i++)
		{
			pstNetTrans	=	&pstNetTransList->NetTrans[i];
			iLimit		+=	pstNetTrans->ConnLimit.MaxConn;
		}
	}

	if( iLimit<=0 )
	{
		tlog_error(pstThread->pstLog, 0, 0, "the max-conn is %d, <=0.\n", iLimit);
		return -1;
	}
	
	if (0 > set_rlimit(iLimit))
	{
		printf("set_rlimit %d error\n", iLimit);
		tlog_error(pstThread->pstLog, 0, 0, "set_rlimit %d error", iLimit);
		return -1;
	}

	if (0 > taes_setkey(&pstThread->stAESCtx, sig_key))
	{
		tlog_error(pstThread->pstLog, 0, 0, "taes_setkey error");
		return -1;
	}

	for(i=0; i<pstConnd->NetTransList.Count; i++)
	{
		pstPDUInst	=	pstConfInst->stPDUInstList.astInsts + pstConfInst->stTransInstList.astInsts[i].iPDULoc;

		if( iUpUnit < pstPDUInst->iUpUnit )
		{
		    iUpUnit	=	pstPDUInst->iUpUnit;
		}
		if( iBuff < pstPDUInst->iUnit)
		{
            iBuff =  pstPDUInst->iUnit;
		}

		if (pstPDUInst->pstPDU->LenParser.AuthParser.AuthType == AUTH_JC)
		{
			iShtUinQue = 1;
		}
	}

	if( iBuff<=0 )
	{
		tlog_error(pstThread->pstLog, 0, 0, "the max pdu unit size is %d, <=0.\n", iBuff);
		return -1;
	}

	pstThread->iPkgUnit	=	iUpUnit;
	pstThread->iPoolUnit	=	pstThread->iPkgUnit + (int) offsetof(TCONNINST, szBuff); // + SHORT_COUNT_BUFF;

	iRet	=	tconnd_init_pool(&pstThread->pstPool, iLimit, pstThread->iPoolUnit);

	if( iRet<0 )
	{
		tlog_error(pstThread->pstLog, 0, 0, "tconnd_init_pool failed, max=%d, unit=%d.\n", iLimit, pstThread->iPoolUnit);
		return -1;
	}

/*
	if (iShtUinQue)
	{
		pstThread->pstShtUinQue = sht_create(iLimit, iLimit, sizeof(UinQue), NULL);
		if (NULL == pstThread->pstShtUinQue)
		{
			tlog_error(pstThread->pstLog, 0, 0, "sht_create fail");
			return -1;
		}
	}
*/

	pstPool	=	pstThread->pstPool;

	pstThread->epfd	=	epoll_create(iLimit);
	if( -1==pstThread->epfd )
	{
		tlog_error(pstThread->pstLog, 0, 0, "create epoll-fd[MAXFD=%d] failed.errorstring=%s\n", iLimit,strerror(errno));

		return -1;
	}

	for(i=0; i<pstNetTransList->Count; i++)
	{
		pstTransInst	=	&pstConfInst->stTransInstList.astInsts[i];

		for(j=0; j<pstTransInst->iLisCount; j++)
		{
			pstListener	=	pstListenerList->Listeners + pstTransInst->aiLisLoc[j];
	
			s	=	tnet_listen(pstListener->Url, pstListener->Backlog);

			if( tconnd_add_conn(pstThread, s, i, pstTransInst->aiLisLoc[j], 1, NULL)<0 )
			{
                            tlog_error(pstThread->pstLog, 0, 0, "tconnd_add_conn failed.\n");
				return -1;
			}
		}
	}


	pstThread->iSendBuff	=	iBuff + sizeof(TFRAMEHEAD) + sizeof(PDUHEAD);
	pstThread->pszSendBuff	=	malloc(pstThread->iSendBuff);
	pstThread->iRecvBuff	=	iBuff + sizeof(TFRAMEHEAD) + sizeof(PDUHEAD);
	pstThread->pszRecvBuff	=	malloc(pstThread->iRecvBuff);

	pstThread->iMsgBuff	=	iBuff + sizeof(TFRAMEHEAD) + sizeof(PDUHEAD);
	pstThread->pszMsgBuff	=	malloc(pstThread->iMsgBuff);

	if( !pstThread->pszSendBuff || !pstThread->pszRecvBuff || !pstThread->pszMsgBuff )
	{
		tlog_error(pstThread->pstLog, 0, 0, "alloc thread buffer size %d failed.\n", pstThread->iSendBuff);
		return -1;
	}

	return 0;
}


int zone_conn_init(TAPPCTX *pstAppCtx, ZONE_CONNENV *pstEnv)
{
	int iRet;
	SERINSTLIST* pstSerInstList;
	int i;
	SERINST* pstSerInst;
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	pstEnv->pstConf = (ZONE_CONNCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (ZONE_CONNRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}

	pstAppCtx->iEpollWait = 0;
	iRet = tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID);
	if (0 != iRet)
	{
		printf("tconnapi_initialize failed by GCIMkey %s and businessid %d, iRet: 0x%x\n",
			pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID, iRet);
		return iRet;
	}

	if( tconnd_init_confinst(&pstEnv->stConfInst, pstEnv->pstConf)<0 )
	{
		printf("error: init config failed.\n");
		return -1;
	}

	tconnd_init_tconndrun(pstEnv->pstRunStat, pstEnv->pstConf);

	
	pstSerInstList	=	&pstEnv->stConfInst.stSerInstList;
	for( i=0; i<pstSerInstList->iCount; i++ )
	{
		pstSerInst	=	pstSerInstList->astInsts + i;
	
		if( tbus_connect(pstAppCtx->iBus, pstSerInst->iDst)<0 )
		{
			printf("error: tbus_connect to Serializer<URL:%s> failed.\n", 
					tbus_addr_ntoa(pstSerInst->iDst));
			return -1;
		}
	}
	
	printf("conn start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "conn start");

	return tconnd_init_thread(pstAppCtx, pstEnv);
}

int tconnd_do_auth_jc(PDUHEAD* pstHead, TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	TDRDATA stHost;
	TDRDATA stNet;
	AUTHJC *pstAuthInfo;
	SIGINFO stSigInfo;
	PDULENAUTHPARSERINST* pstAuthParserInst;
	char szDec[AUTH_LEN+32];
	int iLen;
	
	pstAuthInfo = &pstHead->Ext.AuthInfo.AuthData.AuthJC;
       pstAuthParserInst	=	&pstThread->pstPDUInst->stLenParser.stAuthParser;

	iLen = sizeof(szDec);
	if (0 > taes_decrypt(pstAuthInfo->Sig, pstAuthInfo->SigLen, &pstThread->stAESCtx, szDec, &iLen))
	{
		tlog_error(pstThread->pstLog, 0, 0, "auth decrypt fail");
		return -1;
	}

	stHost.pszBuff = (char *)&stSigInfo;
	stHost.iBuff = sizeof(stSigInfo);
	stNet.pszBuff = szDec;
	stNet.iBuff = iLen;
	if (0 > tdr_ntoh(pstAuthParserInst->pstMetaGameSig, &stHost, &stNet, 0))
	{
		tlog_error(pstThread->pstLog, 0, 0, "auth tdr_ntoh fail");
		return -1;
	}

	if (pstAuthInfo->Uin != stSigInfo.Uin)
	{
		tlog_error(pstThread->pstLog, 0, 0, "auth uin %d but sig uin %d", pstAuthInfo->Uin, stSigInfo.Uin);
		return -1;
	}

	if (0 > taes_setkey(&pstInst->stGameKey, (unsigned char *)stSigInfo.GameKey))
	{
		return -1;
	}
	
	pstInst->iUinFlag = stSigInfo.UinFlag;
	pstInst->iUin = stSigInfo.Uin;
	pstInst->iAccSilenceEnd = stSigInfo.AccSilenceEnd;
	STRNCPY(pstInst->BirthDay, stSigInfo.BirthDay, sizeof(pstInst->BirthDay));

	tlog_debug(pstThread->pstLog, 0, 0,"Auth Success for Uin = %d \n", pstInst->iUin);

	/*
	{
		UinQue *pstUinQue, stUinQue;

		stUinQue.iUin = pstInst->iUin;
		stUinQue.pstInst = pstInst;
			
		pstUinQue = (UinQue *)sht_find(pstThread->pstShtUinQue, &stUinQue, UinQueCmp, UinQueCode);
		if (pstUinQue)
		{
			TRANSINST *pstTransInst1	=	pstThread->pstConfInst->stTransInstList.astInsts + pstUinQue->pstInst->iTransLoc;
			PDUINST *pstPDUInst1	=	pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst1->iPDULoc;

			tconnd_low_stop_conn(pstThread, pstUinQue->pstInst->s, pstPDUInst1, PDU_STOP_CONNECTION_SAMEACC_QUE);
			pstUinQue->pstInst->iNeedFree = STOP_REASON_LIMIT;
			pstUinQue->pstInst->iNotify = 0;
		}
		
		pstUinQue = (UinQue *)sht_insert_multi(pstThread->pstShtUinQue, &stUinQue, UinQueCode);
		if (pstUinQue)
		{
			memcpy(pstUinQue, &stUinQue, sizeof(*pstUinQue));
		}
	}*/

	return 0;
}


int tconnd_do_auth(PDUHEAD* pstHead, TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
     PDULENAUTHPARSER* pstAuthParser=NULL;
     PDUEXTAUTHINFO* pstPDUAuth=NULL;
     int  iRet = 0;

     pstPDUAuth	=	&pstHead->Ext.AuthInfo;
     pstAuthParser	=	&pstThread->pstPDUInst->pstPDU->LenParser.AuthParser;

     pstInst->iAuthType	 = pstAuthParser->AuthType;
	 
     if( PDU_CMD_AUTH!=pstHead->Base.Cmd )
     {
		return -1;
     }

     if( pstHead->Ext.AuthInfo.AuthType!=pstAuthParser->AuthType )
     {
              tlog_error(pstThread->pstLog, 0, 0,"AuthType do not match configuration NetAuth =%d,LocalAuth =%d\n",pstHead->Ext.AuthInfo.AuthType,pstAuthParser->AuthType);
		return -1;
     }

     switch( pstAuthParser->AuthType )
     {
          case AUTH_JC:	
			iRet = tconnd_do_auth_jc(pstHead,pstThread,pstInst);
	   	       if(iRet!=0)
	   	       {
                           tlog_error(pstThread->pstLog, 0, 0,"tconnd Auth QQUnified Sig failed!");
			      return iRet;
			}
			break;
           default:
		   	tlog_error(pstThread->pstLog, 0, 0,"AuthType uncorrect:%d\n",pstAuthParser->AuthType );
		   	return -1;
     }

    tlog_debug(pstThread->pstLog, 0, 0,"Auth success,connection pass\n");
    pstInst->iAuthPass =1;
  
    return 0;
}


int tconnd_pkg2msg(TDRDATA* pstMsg, TDRDATA* pstPkg, TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	PDUHEAD stHead;
	TDRDATA stHost;
	TDRDATA stNet;
	PDUINST* pstPDUInst;
	PDULENAUTHPARSER* pstAuthParser;
	PDULENAUTHPARSERINST* pstAuthParserInst;
	char* pszEnc;
	int iEnc;
	int iRet=0;

	pstPDUInst	=	pstThread->pstPDUInst;

	if (PDULENPARSERID_BY_AUTH != pstPDUInst->iLenParsertype)
	{
		if( !pstInst->iAuthPass )
		{
			pstInst->iAuthPass	=	1;
			return 0;
		}

		if( pstInst->iIsInQueue )
		{
	             tlog_info(pstThread->pstLog, 0, 0,"client %s is in queue, process recv data after queue up \n",
						inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr));
			return 1;
		}
		else
		{
			pstMsg->pszBuff	=	pstPkg->pszBuff;
			pstMsg->iBuff	=	pstPkg->iBuff;
			return 0;
		}
	}

	pstAuthParser	=	&pstPDUInst->pstPDU->LenParser.AuthParser;
	pstAuthParserInst	=	&pstPDUInst->stLenParser.stAuthParser;

	stHost.pszBuff	=	(char*)&stHead;
	stHost.iBuff	=	(int)sizeof(stHead);

	stNet.pszBuff	=	pstPkg->pszBuff;
	stNet.iBuff	=	pstPkg->iBuff;
	iRet = tdr_ntoh(pstAuthParserInst->pstMetaHead, &stHost, &stNet, PDU_VERSION);
	if( iRet<0 )
	{
              tlog_error(pstThread->pstLog, 0, 0,"tdr_ntoh auth metahead failed,iRet=%d,error=%s",iRet,tdr_error_string(iRet));
		return iRet;
	}

	if( !pstInst->iAuthPass )
	{
	       switch( stHead.Base.Cmd)
	       {
                   case PDU_CMD_AUTH:
				iRet=tconnd_do_auth(&stHead, pstThread, pstInst);
				if( iRet!=0 )
				{
                                 tlog_error(pstThread->pstLog, 0, 0,"tconnd Process Auth Msg failed\n");
				     return iRet;
				}
				return 0;
		     default:
			 	tlog_error(pstThread->pstLog, 0, 0,"Auth connection failed:CMDiD=%d \n",stHead.Base.Cmd );
				return -1;
		}
	}

	if( pstInst->iIsInQueue )
	{
	      tlog_info(pstThread->pstLog, 0, 0,"client %s is in queue, process recv data after queue up \n",
					inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr));
		return 1;
	}

	if (stHead.Base.Cmd == PDU_CMD_PLAIN)
	{
		pstMsg->pszBuff	=	pstPkg->pszBuff + stNet.iBuff;
		pstMsg->iBuff	=	pstPkg->iBuff - stNet.iBuff;
		
		return 0;
	}

	pszEnc		=	pstPkg->pszBuff + stNet.iBuff;
	iEnc		=	pstPkg->iBuff - stNet.iBuff;

	if( iEnc > 0)
	{
		iRet = taes_decrypt(pszEnc, iEnc, &pstInst->stGameKey, pstMsg->pszBuff, &pstMsg->iBuff);
		
		if( 0 >  iRet)
		{
                     tlog_error(pstThread->pstLog, 0, 0,"decrypt logic msg failed, msg len(%d) ret(%d)", iEnc, iRet);
			return -1;
		}

		if (pstMsg->iBuff < 0 || pstMsg->iBuff > iEnc)
		{
			tlog_error(pstThread->pstLog, 0, 0,"decrypt logic msg failed, msg len(%d), buff %d ", iEnc, pstMsg->iBuff);
			return -1;
		}
	}
	else if (iEnc == 0)
	{
		pstMsg->iBuff	=	0;
	}
	else
	{
		tlog_error(pstThread->pstLog, 0, 0,"decrypt logic msg failed, msg len(%d) \n", iEnc);
		return -1;
	}

	return 0;
}


int tconnd_push_queue(TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	TRANSINST* pstTransInst;
	TCONNINST* pstPrev;

	
    pstInst->iIsInQueue	=	1;
	
	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	//pstTransInst->uiTokenAlloc++;
	//pstInst->uiQueueToken	=	pstTransInst->uiTokenAlloc;
	pstInst->tInQueue = gs_tNow;

	pstTransInst->stConnInfo.Idle--;
	pstTransInst->stConnInfo.Wait++;

	if( -1==pstTransInst->iWaitQueueTail )
	{
		pstInst->iQueuePrev	=	-1;
		pstInst->iQueueNext	=	-1;

		pstTransInst->iWaitQueueHead	=	pstInst->iIdx;
		pstTransInst->iWaitQueueTail	=	pstInst->iIdx;
	}
	else
	{
		pstInst->iQueuePrev	=	pstTransInst->iWaitQueueTail;
		pstInst->iQueueNext	=	-1;

		pstPrev	=	tconnd_get_inst(pstThread->pstPool, pstTransInst->iWaitQueueTail);

		if( pstPrev )
		{
			pstPrev->iQueueNext	=	pstInst->iIdx;
		}

		pstTransInst->iWaitQueueTail	=	pstInst->iIdx;
	}

	return 0;
}

int tconnd_make_frame(TDRDATA* pstFrame, TDRDATA* pstMsg, TFRAMEHEAD* pstFrameHead)
{
	TDRDATA stHost;
	TDRDATA stNet;

	/* do process the msg. */
	stHost.pszBuff	=	(char*) pstFrameHead;
	stHost.iBuff	=	(int)sizeof(*pstFrameHead);

	stNet.pszBuff	=	pstFrame->pszBuff;
	stNet.iBuff	=	pstFrame->iBuff;


	if( TCONNAPI_FRAMEHEAD_HTON(&stNet,&stHost,  0)<0 )
	{
		return -1;
	}

	if( stNet.iBuff + pstMsg->iBuff > pstFrame->iBuff )
	{
		return -1;
       }

	pstFrame->iBuff	=	stNet.iBuff + pstMsg->iBuff;

	if( pstMsg->iBuff )
	{
		memcpy(stNet.pszBuff+stNet.iBuff, pstMsg->pszBuff, pstMsg->iBuff);
	}

	return 0;
}


int tconnd_msg2frame(TDRDATA* pstFrame, TDRDATA* pstMsg, TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	struct timeval stCurr;
	TFRAMEHEAD stHead;
	TIPINFO* pstIPInfo;
	int iRet=0;

	/* do process the msg. */

	memset(&stHead, 0, offsetof(TFRAMEHEAD, ID));

	stHead.ID	=	pstInst->iID;
	stHead.ConnIdx	=	pstInst->iIdx;

	if( pstInst->fWaitFirstPkg )
	{
		pstInst->fWaitFirstPkg	=	0;
		
        	stHead.Ver	=	0;
		stHead.ExtraType=	TFRAMEHEAD_EXTRA_IP;
		pstIPInfo	=	&stHead.ExtraInfo.IPInfo;

		pstIPInfo->family	=	pstInst->stAddr.sa_family;
		if( AF_INET==pstInst->stAddr.sa_family )
		{
			pstIPInfo->port=	((struct sockaddr_in*)(&pstInst->stAddr))->sin_port;
			pstIPInfo->ip	=	((struct sockaddr_in*)(&pstInst->stAddr))->sin_addr.s_addr;
		}
		else
		{
			pstIPInfo->port=	0;
			pstIPInfo->ip	=	0;
		}

	 	{      
		  //Auth Msg
			stHead.Cmd	=	TFRAMEHEAD_CMD_START;
			stHead.CmdData.Start.AuthType	=	AUTH_JC;   
			stHead.CmdData.Start.AuthData.AuthJC.Uin = pstInst->iUin;
			stHead.CmdData.Start.AuthData.AuthJC.UinFlag = pstInst->iUinFlag;
			stHead.CmdData.Start.AuthData.AuthJC.AccSilenceEnd = pstInst->iAccSilenceEnd;
			STRNCPY(stHead.CmdData.Start.AuthData.AuthJC.BirthDay, pstInst->BirthDay, 
			        sizeof(stHead.CmdData.Start.AuthData.AuthJC.BirthDay));
		}
	}
	else
	{
		stHead.Cmd	=	TFRAMEHEAD_CMD_INPROC;
		memset(&stHead.CmdData.InProc, 0, offsetof(TFRAMECMDINPROC, Idents));
	
		if( -1==stHead.ID || !pstInst->fStream )
		{
			stHead.ExtraType=	TFRAMEHEAD_EXTRA_IP;

			pstIPInfo	=	&stHead.ExtraInfo.IPInfo;
			pstIPInfo->family	=	pstInst->stAddr.sa_family;

			if( AF_INET==pstInst->stAddr.sa_family )
			{
				pstIPInfo->port=	((struct sockaddr_in*)(&pstInst->stAddr))->sin_port;
				pstIPInfo->ip	=	((struct sockaddr_in*)(&pstInst->stAddr))->sin_addr.s_addr;
			}
			else
			{
				pstIPInfo->port=	0;
				pstIPInfo->ip	=	0;
			}
		}
		else
		{
			stHead.ExtraType=	0;
		}
	}

	if( pstInst->iUseTimeStamp )
	{
		stHead.TimeStampType	=	TFRAMEHEAD_TIMESTAMP_TIMEVAL;

		gettimeofday(&stCurr, NULL);
		stHead.TimeStamp.TimeVal.sec	=	(int)stCurr.tv_sec;
		stHead.TimeStamp.TimeVal.usec	=	(int)stCurr.tv_usec;
	}
	else
	{
		stHead.TimeStampType	=	0;
	}
	
       iRet = tconnd_make_frame(pstFrame, pstMsg, &stHead);
	if(iRet)
	{
          tlog_error(pstThread->pstLog, 0, 0,"tconnd_make_frame failed\n");
	      return -1;
	}
	
	return 0;
}

int tconnd_sendto_serinst(TCONNTHREAD* pstThread, TCONNINST* pstInst, TDRDATA* pstFrame)
{
	int i;
	int iSrc;
	int iDst;
	CONFINST* pstConfInst;
	ZONE_CONNRUN_STATUS* pstRun;
	SERINSTLIST* pstSerInstList;
	TRANSINST* pstTransInst;
	NETTRANSRUN* pstTransRun;
	SERINST* pstSerInst;
	SERIALIZERRUN* pstSerRun;
	int iRet = 0;
	int iFrame;

	iFrame		=	pstFrame->iBuff;

	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;

	pstConfInst	=	pstThread->pstConfInst;
	pstRun		=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstRunStat;

	pstTransRun	=	pstRun->NetTransRunList.NetTrans + pstInst->iTransLoc;

	pstTransRun->TransStat.RecvPkgSucc++;
	pstTransRun->TransStat.RecvByteSucc	+=	iFrame;

	pstSerInstList	=	&pstConfInst->stSerInstList;

	/* send to all serializer */
	for(i=0; i<pstTransInst->iSerCount; i++)
	{
		pstSerInst	=	pstSerInstList->astInsts + pstTransInst->aiSerLoc[i];
		pstSerRun	=	pstRun->SerializerRunList.Serializers + pstTransInst->aiSerLoc[i];

		iSrc		=	0;

		if(pstInst->stShadowRouting.fEnabled 
			/*&& i == pstInst->stShadowRouting.iReplacedIdx*/)
		{
			iDst = pstInst->stShadowRouting.iRealBusID; // 影子路由生效
		}
		else
		{
			iDst = pstSerInst->iDst;
		}

		iRet	=	tbus_send( pstThread->pstAppCtx->iBus, &iSrc, &iDst, pstFrame->pszBuff, pstFrame->iBuff, 0);

		if( iRet<0 )
		{
			pstSerRun->TransStat.SendPkgFail++;
			pstSerRun->TransStat.SendByteFail	+=	iFrame;
			tlog_error(pstThread->pstLog, 0, 0,"Send package to tbus failed:des=%s :datelen =%d, SendFirstFrame=%d\n",
						tbus_addr_ntoa((TBUSADDR) iDst), pstFrame->iBuff, pstInst->iSendFirstFrame);	

			if( !pstInst->iSendFirstFrame )
			{
				pstInst->iNeedFree = STOP_REASON_AUTHFAIL;
				return -1;
			}
		}
		else
		{
			pstSerRun->TransStat.SendPkgSucc++;
			pstSerRun->TransStat.SendByteSucc	+=	iFrame;
            tlog_debug(pstThread->pstLog, 0, 0,"Send package to tbus success:desc=%s:datelen =%d\n",tbus_addr_ntoa((TBUSADDR) iDst),pstFrame->iBuff);

			
			if( !pstInst->iSendFirstFrame )
			{
				  pstInst->iSendFirstFrame	=	1;
			}
		}
	}

	return 0;
}


int tconnd_recv_pkg(TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	int iCount;
	CONFINST* pstConfInst;
	ZONE_CONNCONF* pstConnd;
	ZONE_CONNRUN_STATUS* pstRun;
	NETTRANS* pstNetTrans;
	TRANSINST* pstTransInst;
	LISTENERRUN* pstLisRun;
	TDRDATA stPkg;
	TDRDATA stMsg;
	TDRDATA stFrame;
	PFNTCONND_GET_PKGLEN pfnGetPkgLen;
	int iRet = 0;

	iCount	=	0;

	pstTransInst	= pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	pstThread->pstPDUInst	= pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;

	pfnGetPkgLen 	= pstThread->pstPDUInst->pfnGetPkgLen;
	
	if( !pstInst->iPkgLen )
	{
		iRet = (*pfnGetPkgLen)(pstInst, pstThread);
	}

	if( 0 != iRet)
	{
		return iRet;
	}

	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;
	pstConfInst	=	pstThread->pstConfInst;
	pstRun		=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstRunStat;

	pstNetTrans	= 	pstConnd->NetTransList.NetTrans + pstInst->iTransLoc;

	while( pstInst->iData >= pstInst->iPkgLen )
	{
		// stpkg--接收到的一段buff
		stPkg.pszBuff	=	pstInst->szBuff + pstInst->iOff;
		stPkg.iBuff	=	pstInst->iPkgLen;

		// stMsg---除开PDU的buff
		stMsg.pszBuff	=	pstThread->pszMsgBuff;
		stMsg.iBuff	=	pstThread->iMsgBuff;

		// stFrame ---FrameHead+stMsg 发给zone_svr
		stFrame.pszBuff	=	pstThread->pszRecvBuff;
		stFrame.iBuff	=	pstThread->iRecvBuff;

		iRet		=	0;
		iCount++;

		if( !pstInst->iAuthPass )
		{
			tconnd_pkg2msg(&stMsg, &stPkg, pstThread, pstInst);

			//wait for synack msg
			/*
			if( (!pstInst->iAuthPass)&&(pstInst->iSynSent) )
			{
                 	      pstInst->iData	-=	pstInst->iPkgLen;
		             pstInst->iOff	+=	pstInst->iPkgLen;
		             pstInst->iPkgLen	=	0;     
			      break;
			}*/
			
			//create connection fail,close
			if( !pstInst->iAuthPass )
			{
                		pstInst->iNeedFree	=	STOP_REASON_AUTHFAIL;
             			pstInst->iNotify	=	0;
				tlog_error(pstThread->pstLog, 0, 0," auth failed!, close connection:ip =%s,Port =%d\n",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)(&pstInst->stAddr))->sin_port));
				break;
			}
	
			if ((0 == pstNetTrans->UseWaitQueue && 0 == pstNetTrans->ConnLimit.Speed) || ((UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN) & pstInst->iUinFlag))
			{
				pstTransInst->stConnInfo.Idle--;
				pstTransInst->stConnInfo.Active++;
				iRet = tconnd_pkg2msg(&stMsg, &stPkg,pstThread,pstInst);
				if( 0!= iRet )
				{
	                            tlog_error(pstThread->pstLog, 0, 0," tconnd_pkg2msg failed!iRet =%d\n",iRet);
				}
				else
				{
	                         iRet = tconnd_msg2frame(&stFrame, &stMsg, pstThread, pstInst);
	   			    if(  0 != iRet )
	   			    {
	   		                  tlog_error(pstThread->pstLog, 0, 0,"tconnd_msg2frame failed\n");
	   			    }
				}
			}
			else
			{
				tlog_debug(pstThread->pstLog, 0, 0,"push client to queue,IP = %s,Port=%d\n",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)(&pstInst->stAddr))->sin_port));
				tconnd_push_queue(pstThread, pstInst);
				break;
			}
		}
		else
		{
			iRet	=	tconnd_pkg2msg(&stMsg, &stPkg, pstThread, pstInst);
			if(0 != iRet)
			{
	                   if( iRet > 0 )
	                   {
	                       //iRet = 1 process msg after queue up
	                       break;
			      }
			      tlog_error(pstThread->pstLog, 0, 0,"tconnd_pkg2msg failed\n");
			}
			else
			{
                  		iRet	=	tconnd_msg2frame(&stFrame, &stMsg, pstThread, pstInst);
			      if( 0 != iRet)
			      {
                        		tlog_error(pstThread->pstLog, 0, 0,"tconnd_msg2frame failed\n");
			      }
			}
			   
		}

		if( 0==iRet )
		{
			pstLisRun	=	pstRun->ListenerRunList.Listeners + pstInst->iLisLoc;
			
			if( (0==pstTransInst->iPkgPermit || pstInst->iRecvMsg <= pstTransInst->iPkgPermit) &&
			    (0==pstTransInst->iBytePermit ||  pstInst->iRecvByte <= pstTransInst->iBytePermit) )
			{
				iRet	=	tconnd_sendto_serinst(pstThread, pstInst, &stFrame);
			}
			else
			{
				unsigned short ushCmd=0;
				pstLisRun->TransStat.RecvPkgDrop++;
				pstLisRun->TransStat.RecvByteDrop	+=	stPkg.iBuff;
				memcpy(&ushCmd,stMsg.pszBuff+2,2);
				ushCmd = ntohs(ushCmd);

				tlog_error(pstThread->pstLog, 0, 0,"limit!, pkg drop:ip =%s,Port =%d, RecvMsg=%d, RecvByte=%d,Cmd=%d,PkgPermit=%d,BytePermit=%d",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
							NTOHS(((struct sockaddr_in*)(&pstInst->stAddr))->sin_port),
							pstInst->iRecvMsg, pstInst->iRecvByte,ushCmd,
							pstTransInst->iPkgPermit,pstTransInst->iBytePermit);
				
				/*tlog_error(pstThread->pstLog, 0, 0,"limit!, close connection:ip =%s,Port =%d, RecvMsg=%d, RecvByte=%d,Cmd=%d",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
							NTOHS(((struct sockaddr_in*)(&pstInst->stAddr))->sin_port),
							pstInst->iRecvMsg, pstInst->iRecvByte,ushCmd);*/

				/* 先观察，不断开
				pstInst->iNeedFree	=	STOP_REASON_LIMIT;
             			pstInst->iNotify	=	1;
				
				return iCount;*/
			}

			pstLisRun->TransStat.RecvPkgSucc++;
			pstLisRun->TransStat.RecvByteSucc	+=	stPkg.iBuff;

			if( gs_tNow > pstInst->tLastRecvMsgCheck + pstTransInst->iRecvCheckInterval )
			{
				pstInst->tLastRecvMsgCheck	=	gs_tNow;
				pstInst->iRecvMsg	=	0;
				pstInst->iRecvByte	=	0;
			}
			else
			{
				pstInst->iRecvMsg++;
				pstInst->iRecvByte	+=	stPkg.iBuff;
			}

			pstInst->iData	-=	pstInst->iPkgLen;
			pstInst->iOff	+=	pstInst->iPkgLen;
			pstInst->iPkgLen	=	0;

			iRet = (*pfnGetPkgLen)(pstInst, pstThread);
		}
		else
		{
			pstInst->iData =	0;
			pstInst->iOff	 =	0;
			pstInst->iPkgLen	=	0;
		}

		if(0 != iRet)
		{
			break;
		}

	}/*while( pstInst->iData>=pstInst->iPkgLen )*/

	if( pstInst->iOff )
	{
		if( pstInst->iData )
		{
			memmove(pstInst->szBuff, pstInst->szBuff+pstInst->iOff, pstInst->iData);
		}

		pstInst->iOff	=	0;
	}

	return iCount;
}


int tconnd_proc_recv(TCONNTHREAD* pstThread)
{
	int s;
	struct sockaddr sa;
	int iLen;
	struct epoll_event events[MAX_EVENTS];
	int iEvents;
	int i;
	TCONNPOOL* pstPool;
	ZONE_CONNCONF* pstConnd;
	TCONNINST* pstInst;
	TRANSINST* pstTransInst;
	PDUINST* pstPDUInst;
	LISTENER* pstListener;
	CONFINST* pstConfInst;
	NETTRANS* pstNetTrans;
	struct timeval stStart;
	struct timeval stEnd;
	struct timeval stSub;
	int iRecvPackage=0;
	int iTotalRecv=0;

	pstThread->iMsRecv = 0;   
	iEvents	=	epoll_wait(pstThread->epfd, events, MAX_EVENTS, pstThread->pstAppCtx->iEpollWait);

	if( iEvents<=0 )
	{
	       if(0 != iEvents )
	       {
             		tlog_error(pstThread->pstLog, 0, 0,"epoll_wait error errostring=%s\n", strerror(errno)); 
		}	  
		pstThread->pstAppCtx->iEpollWait = 0;
		return -1;
	}
	
	gettimeofday(&stStart, NULL);

	pstPool		=	pstThread->pstPool;
	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;
	pstConfInst	=	pstThread->pstConfInst;
	
	for(i=0; i<iEvents; i++)
	{
		pstInst	=	tconnd_get_inst(pstPool, events[i].data.fd);

		/* bad packet. */
		if( !pstInst )
		{
			epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, (&events[i].data.fd)[1], events+i);
			tlog_error(pstThread->pstLog, 0, 0,"pstInst not find, idx=%d\n", events[i].data.fd);
			continue;
		}

		pstInst->tLastRecv	=	gs_tNow;

		pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
		pstPDUInst	=	pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;
		pstListener	=	pstConnd->ListenerList.Listeners + pstInst->iLisLoc;
		pstNetTrans	=	pstConnd->NetTransList.NetTrans + pstInst->iTransLoc;

		if( pstInst->fListen && pstInst->fStream )
		{
			iLen	=	(int) sizeof(sa);

			s	=	epoll_accept(pstThread->epfd, pstInst->s, events+i, &sa, &iLen);

			tconnd_add_conn(pstThread, s, pstInst->iTransLoc, pstInst->iLisLoc, 0, &sa);
		}
		else
		{
			int iAddrLen;
			iAddrLen    =   (int) sizeof(pstInst->stAddr);

			if( pstInst->fStream )
			{
				iLen    =   epoll_recv(pstThread->epfd, pstInst->s, events+i, pstInst->szBuff+pstInst->iData, pstInst->iBuff - pstInst->iData);
				if( iLen<=0 )
				{		
					if( iLen<0 )
					{
						pstInst->iNeedFree	=	STOP_REASON_NETWORK;
						tlog_info(pstThread->pstLog, 0, 0, "client  %s:%d exit unnormal or connection fail !need to free!errorstring =%s\n",
					 	                                                           inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
					 	                                                           NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
					 	                                                           strerror(errno));

					}
					else
					{
						pstInst->iNeedFree	=	STOP_REASON_CLOSE;
						tlog_debug(pstThread->pstLog, 0, 0, "tconnd recv close package from %s:%d !\n",
					 	                                                           inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
					 	                                                           NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port));
					}

					epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, events+i); 

					pstInst->iNotify	=	1;
					
					continue;
				}
			}
			else
			{
				pstInst->iData  =   0;
				iLen    =   epoll_recvfrom(pstThread->epfd, pstInst->s, events+i, pstInst->szBuff+pstInst->iData, pstInst->iBuff - pstInst->iData, &pstInst->stAddr, &iAddrLen);
				if( iLen<=0 ) 
				{
				    continue;
				}
			}

			pstInst->iData	+=	iLen;

			if( pstInst->iPkgLen && pstInst->iData<pstInst->iPkgLen )
			{
				continue;
			}
			
            		tlog_debug(pstThread->pstLog, 0, 0, "tconnd recv package from %s:%d;datelen =%d\n",
					 	inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
					 	NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
					 	pstInst->iData);
					 
			iRecvPackage = tconnd_recv_pkg(pstThread, pstInst);

			if( pstInst->iNeedFree )
			{
				epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, events+i);
			}

			if(iRecvPackage > 0)
			{
                  		iTotalRecv+=iRecvPackage;
			}
			
			if( iTotalRecv >= PAK_PER_SLICE )
			{
	                    //check recv time every Package_Per_Slice package received
	                   iTotalRecv = iTotalRecv - PAK_PER_SLICE;
	                   gettimeofday(&stEnd, NULL);
	       	     TV_DIFF(stSub, stEnd, stStart);
		            TV_TO_MS(pstThread->iMsRecv, stSub);	
			     if( pstThread->iMsRecv > pstThread->iMsRecvPerLoop)
			     {
                     		return 0;
			     }
			}

		}
	}

	gettimeofday(&stEnd, NULL);
       TV_DIFF(stSub, stEnd, stStart);
	TV_TO_MS(pstThread->iMsRecv, stSub);	

	return 0;
}

int tconnd_sendto_serinst_by_bus(TCONNTHREAD* pstThread, TFRAMEHEAD* pstHead, TDRDATA* pstMsg, int iDst)
{
	int iRet;
	int iSrc;
	char szPkg[sizeof(TFRAMEHEAD)];
	TDRDATA stHost;
	TDRDATA stNet;
	struct iovec astIOVec[2];

	stHost.pszBuff	=	(char*)pstHead;
	stHost.iBuff	=	(int)sizeof(*pstHead);

	stNet.pszBuff	=	(char*)szPkg;
	stNet.iBuff	=	(int)sizeof(szPkg);

	iRet	=	TCONNAPI_FRAMEHEAD_HTON(&stNet, &stHost, 0);

	if( iRet<0 )
		return -1;

	iSrc	=	0;

	if( pstMsg && pstMsg->iBuff )
	{
		astIOVec[0].iov_base	=	(void*)stNet.pszBuff;
		astIOVec[0].iov_len	=	(size_t)stNet.iBuff;
		astIOVec[1].iov_base	=	(void*)pstMsg->pszBuff;
		astIOVec[1].iov_len	=	(size_t)pstMsg->iBuff;
		iRet	=	tbus_sendv( pstThread->pstAppCtx->iBus, &iSrc, &iDst, astIOVec, 2, 0);
	}
	else
	{
        //后期的预占路由功能      需要
		iRet = tbus_backward(pstThread->pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0);
	}

	return iRet;
}

int tconnd_set_routing(TCONNTHREAD *pstThread, TFRAMEHEAD *pstFrameHead,
                       TCONNINST *pstInst)
{
	/*
	int i = 0;
	const CONFINST *pstConfInst = pstThread->pstConfInst;
	const TRANSINST *pstTransInst = pstConfInst->stTransInstList.astInsts +
	                                pstInst->iTransLoc;
	char ucEntity = 0;

	
	GET_FUNC_ENTITY(ucEntity, pstFrameHead->CmdData.SetRouting.BattleBusID);
	
	tlog_debug(pstThread->pstLog, 0, 0, "request set routing: [%s] -> [%d] ",
	           tbus_addr_ntoa(pstFrameHead->CmdData.SetRouting.ZoneBusID),
	           ucEntity);
	tlog_debug(pstThread->pstLog, 0, 0,
		           "shadow routing: src = %d, dst = %d, real_bus_id = %d",
		           pstFrameHead->CmdData.SetRouting.ZoneBusID,
		           pstFrameHead->CmdData.SetRouting.BattleBusID,
		           pstThread->iLastSrc);

	*/
	//if(pstInst->stShadowRouting.fEnabled)
	if( pstFrameHead->CmdData.SetRouting.BusID == 0 )
	{
		// 如果原来的Bus ID就是被替换的Bus ID,说明路由已恢复到初始状态了
		/*int iDst = pstConfInst->stSerInstList.astInsts[
                                    pstInst->stShadowRouting.iReplacedIdx].iDst;
		if(iDst != pstFrameHead->CmdData.SetRouting.ZoneBusID)
		{
			tlog_error(pstThread->pstLog, 0, 0,
			           "reset routing fail: bus[%s] is not replaced",
			           tbus_addr_ntoa(pstFrameHead->CmdData.SetRouting.BusID));
			iRet = -1;
			goto final;
		}*/

		// 关闭影子路由
		pstInst->stShadowRouting.fEnabled = 0;
		pstInst->stShadowRouting.iRealBusID = 0;
		pstInst->stShadowRouting.iReplacedIdx = 0;

		tlog_debug(pstThread->pstLog, 0, 0, "shadow routing off: src = %d",
		           	pstThread->iLastSrc);
	}
	else
	{
		/*if(pstFrameHead->CmdData.SetRouting.BusID == pstThread->iLastSrc)
		{
			// 现在不能自己set_routing自己了
			iRet = -1;
			goto final;
		}

		for(i = 0; i < pstTransInst->iSerCount; ++i)
		{
			if(pstFrameHead->CmdData.SetRouting.BusID ==
			   pstConfInst->stSerInstList.astInsts[pstTransInst->aiSerLoc[i]].iDst)
			{
				break;
			}
		}
		if(i >= pstTransInst->iSerCount)
		{
			tlog_error(pstThread->pstLog, 0, 0,
			           "request set routing fail: bus[%s] is not exist in trans",
			           tbus_addr_ntoa(pstFrameHead->CmdData.SetRouting.ZoneBusID));
			return -1;     
		}
		*/
		

		// 打开影子路由
		pstInst->stShadowRouting.fEnabled = 1;
		//pstInst->stShadowRouting.iRealBusID = pstThread->iLastSrc;
		pstInst->stShadowRouting.iRealBusID = pstFrameHead->CmdData.SetRouting.BusID;
		//pstInst->stShadowRouting.iReplacedIdx = i; // 记录原先的Bus

		/*
		tlog_debug(pstThread->pstLog, 0, 0,
		           "shadow routing on: src = %d, dst = %d, real_bus_id = %d",
		           pstFrameHead->CmdData.SetRouting.ZoneBusID,
		           pstFrameHead->CmdData.SetRouting.BattleBusID,
		           pstThread->iLastSrc);
		*/
	}

	pstInst->iID = pstFrameHead->CmdData.SetRouting.ID;
/*
final:
	{
		TFRAMEHEAD stRes;
		stRes.Ver = 0;
		stRes.Cmd = TFRAMEHEAD_CMD_SETROUTING;
		stRes.ID = pstFrameHead->ID;
		stRes.ConnIdx = pstFrameHead->ConnIdx;
		stRes.ExtraType = 0;
		stRes.TimeStampType	= 0;
	 	stRes.CmdData.SetRouting = pstFrameHead->CmdData.SetRouting;
	 	stRes.CmdData.SetRouting.ZoneBusID = pstFrameHead->CmdData.SetRouting.ZoneBusID;
		stRes.CmdData.SetRouting.BattleBusID = pstFrameHead->CmdData.SetRouting.BattleBusID;
		if(iRet<0)
		{
	 		stRes.CmdData.SetRouting.ID = iRet;
		}
		else
		{
			pstFrameHead->CmdData.SetRouting.ID=0;
		}
		tconnd_sendto_serinst_by_bus(pstThread, &stRes, NULL,
		                             pstThread->iLastSrc);
	}
	*/
	return 0;
}

static int tconnd_set_limits(TCONNTHREAD* pstThread,
                             const TFRAMEHEAD *pstFrameHead)
{
	ZONE_CONNCONF *pstConnd = ((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;

	int i = pstConnd->NetTransList.Count;
	while(i--)
	{
		NETTRANS *pstNetTrans = pstConnd->NetTransList.NetTrans + i;
		pstNetTrans->QueueNewWorld = pstFrameHead->CmdData.SetLimits.Enabled;
	}

    tlog_info(pstThread->pstLog, 0, 0, "set queue new world enalbed: %d",
              pstFrameHead->CmdData.SetLimits.Enabled);
	return 0;
}

int tconnd_frame2msg(TDRDATA* pstMsg, TDRDATA* pstFrame, TCONNTHREAD* pstThread, TCONNINST** ppstInst)
{
	TCONNINST* pstInst = NULL;
	TFRAMEHEAD* pstFrameHead;

	TDRDATA stHost;
	TDRDATA stNet;
	int iRet=0;

	pstFrameHead	=	&pstThread->stFrameHead;

	stHost.pszBuff	=	(char*)pstFrameHead;
	stHost.iBuff	=	(int)sizeof(*pstFrameHead);

	stNet.pszBuff	=	pstFrame->pszBuff;
	stNet.iBuff	=	pstFrame->iBuff;
	iRet=TCONNAPI_FRAMEHEAD_NTOH(&stHost, &stNet, 0);

	if( iRet<0 )
	{
        tlog_error(pstThread->pstLog, 0, 0,
          "tconnd_frame2msg:TCONNAPI_FRAMEHEAD_NTOH failed,iRet=%d,error=%s!\n",
                   iRet,tdr_error_string(iRet));
		return -1;
	}

	if(pstFrameHead->Cmd == TFRAMEHEAD_CMD_SETLIMITS)
	{
		tconnd_set_limits(pstThread, pstFrameHead);
		return -1;
	}

	pstMsg->pszBuff	=	pstFrame->pszBuff + stNet.iBuff;
	pstMsg->iBuff	=	pstFrame->iBuff - stNet.iBuff;

	/*对于组播包,取其中一个存活的连接作为后面统计连接*/
	if( TFRAMEHEAD_CMD_INPROC==pstFrameHead->Cmd && BROADCAST_FLAG_GROUP == pstFrameHead->CmdData.InProc.Flag )
	{
	
		int i=0;
		while( i <  pstFrameHead->CmdData.InProc.Count )
		{
		    pstInst = tconnd_get_inst(pstThread->pstPool, pstFrameHead->CmdData.InProc.Idents[i].ConnIdx);		  
		    if( pstInst )             
		    {
		       *ppstInst	=	pstInst;
		      return 0;
		   }		   
		    i++;
		}

		return -1;
	}
	else
	{
        pstInst		=	tconnd_get_inst(pstThread->pstPool, pstFrameHead->ConnIdx);

	}
		
       
	if( !pstInst )
	{
		TMEMBLOCK *pstTmemBlock;

		if (pstFrameHead->ConnIdx < 0)
		{
			tlog_error(pstThread->pstLog, 0, 0, "ConnIdx %d", pstFrameHead->ConnIdx);
			return -1;
		}

		if (-1 == pstFrameHead->ID)
		{
			return -1;
		}
		
		pstTmemBlock = TMEMPOOL_GET_PTR(pstThread->pstPool, pstFrameHead->ConnIdx);
		pstInst = (TCONNINST*)pstTmemBlock->szData;
		if (pstInst->iID == pstFrameHead->ID && pstInst->tLastRecv+2>gs_tNow)
		{
			//之前通知过zone_svr
			return -1; 
		}
		
		TFRAMEHEAD stFrameHead;
		stFrameHead.Ver	=	0;
		stFrameHead.Cmd	=	TFRAMEHEAD_CMD_STOP;

		stFrameHead.ID		=	pstFrameHead->ID;
		stFrameHead.ConnIdx	=	pstFrameHead->ConnIdx;
		stFrameHead.ExtraType	=	0;
		stFrameHead.CmdData.Stop.Reason	=	STOP_REASON_LOST;
		stFrameHead.TimeStampType	=	0;
		
        tlog_error(pstThread->pstLog, 0, 0, "ConnIdx doesnot exsit,idx=%d, frame cmd=%d\n",pstFrameHead->ConnIdx, pstFrameHead->Cmd);

		if (TFRAMEHEAD_CMD_STOP != pstFrameHead->Cmd)
		{
			tconnd_sendto_serinst_by_bus(pstThread, &stFrameHead, NULL, pstThread->iLastSrc);	
		}
		return -1;
	}

	*ppstInst	=	pstInst;

	if( pstInst->fStream && pstInst->fListen )		
	{
        tlog_error(pstThread->pstLog, 0, 0, "ConnIdx canot be listen instance ,idx=%d\n",pstFrameHead->ConnIdx);
		return -1;
	}

	if( pstInst->fStream )
	{
		if( -1!=pstFrameHead->ID )
		{
			switch( pstFrameHead->Cmd )
			{
			case TFRAMEHEAD_CMD_START:
				pstInst->iID = pstFrameHead->ID;
				break;
			case TFRAMEHEAD_CMD_SETROUTING:
				if(tconnd_set_routing(pstThread, pstFrameHead, pstInst) < 0)
				{
					return -1;
				}
				break;
        	default:
        		// 防止组播发不出去:
        		// 如果影子路由已开启,就不要检查逻辑ID是否相等了,因为两个进程之间
        		// 的逻辑ID可能是不相等的,而这种情况只有在影子路由开启的时候才会
        		// 出现.
				/*
				if(!pstInst->stShadowRouting.fEnabled &&
				   pstInst->iID != pstFrameHead->ID)
				{
					tlog_error(pstThread->pstLog, 0,0, 
						"ID in framehead not equals to id in connect instance, %d,%d,ConnIdx=%d,cmd=%d",
						pstInst->iID,pstFrameHead->ID,pstFrameHead->ConnIdx,pstFrameHead->PkgHeadCmd);
					return -1;
				}
				*/

				//不在检查MEMID
			break;
			}
	    }
	    else
	    {
             if( pstInst->stAddr.sa_family!=pstFrameHead->ExtraInfo.IPInfo.family ||
		          ((struct sockaddr_in*)(&pstInst->stAddr))->sin_port!=pstFrameHead->ExtraInfo.IPInfo.port ||
		          ((struct sockaddr_in*)(&pstInst->stAddr))->sin_addr.s_addr!=pstFrameHead->ExtraInfo.IPInfo.ip )
		     {
                tlog_error(pstThread->pstLog, 0,0, "addr info in framehead not equals to addr in connect instance");
			    return -1;
		     }					
		}
	}
	else
	{
		((struct sockaddr_in*)(&pstInst->stAddr))->sin_family	= pstFrameHead->ExtraInfo.IPInfo.family;
		((struct sockaddr_in*)(&pstInst->stAddr))->sin_port	= pstFrameHead->ExtraInfo.IPInfo.port;
		((struct sockaddr_in*)(&pstInst->stAddr))->sin_addr.s_addr = pstFrameHead->ExtraInfo.IPInfo.ip;
	}
 
	return 0;
}

int tconnd_msg2pkg(TDRDATA* pstPkg, TDRDATA* pstMsg, TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	TDRDATA stHost;
	TDRDATA stNet;
	int iRet;
	char* pszEnc;
	int iEnc;
	
	if (PDULENPARSERID_BY_AUTH != pstThread->pstPDUInst->iLenParsertype)
	{
		pstPkg->pszBuff	=	pstMsg->pszBuff;
		pstPkg->iBuff	=	pstMsg->iBuff;
		return 0;
	}

	auth_init_base(&pstThread->stPDUHead.Base);

	switch(pstThread->stFrameHead.Cmd)
	{
		case TFRAMEHEAD_CMD_INPROC:
			if (pstThread->stFrameHead.CmdData.InProc.NoEnc)
			{
				pstThread->stPDUHead.Base.Cmd = PDU_CMD_PLAIN;
			}
			break;
		default:
			break;
	}

	stHost.pszBuff	=	(char*)&pstThread->stPDUHead;
	stHost.iBuff	=	(int)sizeof(pstThread->stPDUHead);

	stNet.pszBuff	=	pstPkg->pszBuff;
	stNet.iBuff	=	pstPkg->iBuff;

	iRet=tdr_hton(pstThread->pstPDUInst->stLenParser.stAuthParser.pstMetaHead, &stNet, &stHost, PDU_VERSION);
	if( iRet<0 )
	{
              tlog_error(pstThread->pstLog, 0, 0,"tdr_hton PDU Head failed,iRet=%d,error=%s\n",iRet,tdr_error_string(iRet));
		return iRet;
	}

	((PDUBASE*)pstPkg->pszBuff)->HeadLen	=	(unsigned char)stNet.iBuff;

	if (PDU_CMD_PLAIN == pstThread->stPDUHead.Base.Cmd)
	{
		iEnc	=	pstMsg->iBuff;

		if( pstPkg->iBuff - stNet.iBuff < pstMsg->iBuff )
		{
                     tlog_error (pstThread->pstLog, 0, 0, "tconnd_msg2pkg:Send buf not enough for connection");
			return -1;
		}

		if( pstMsg->iBuff )
		{
			memcpy(pstPkg->pszBuff+stNet.iBuff,  pstMsg->pszBuff, pstMsg->iBuff);
		}
	}
	else
	{
		if( pstMsg->iBuff )
		{
			pszEnc		=	pstPkg->pszBuff + stNet.iBuff;
			iEnc		=	pstPkg->iBuff - stNet.iBuff;

			if (0 > taes_encrypt(pstMsg->pszBuff, pstMsg->iBuff, &pstInst->stGameKey, pszEnc, &iEnc))
			{
				tlog_error(pstThread->pstLog, 0, 0, "encrpt error");
				return -1;
			}
		}
		else
		{
			iEnc		=	0;
		}
	}
	
	((PDUBASE*)pstPkg->pszBuff)->BodyLen	=	HTONL((long)iEnc);

	pstPkg->iBuff	=	stNet.iBuff + iEnc;

	return 0;
}

int show_short_count(TCONNTHREAD* pstThread, TCONNINST* pstInst, int iBuff, int iSend)
{
 	int iRecvBuff;
	int iSendBuff;
 	socklen_t iSize;
 	
 	iSize	=	sizeof(iRecvBuff);
 	if (0 > getsockopt(pstInst->s, SOL_SOCKET, SO_RCVBUF, (void*)&iRecvBuff, &iSize))
 	{
 		return -1;
 	}
 	 	
 	iSize	=	sizeof(iSendBuff);
 	if (0 > getsockopt(pstInst->s, SOL_SOCKET, SO_SNDBUF, (void*)&iSendBuff, &iSize))
 	{
 		return -1;
 	}
 	
	tlog_error(pstThread->pstLog, 0, 0,"short count send package to client failed:des=%s:%d :datelen =%d,send=%d, recvbuff=%d, sendbuff=%d, idle=%d",
						inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
						iBuff, iSend, iRecvBuff, iSendBuff,
						pstThread->pstAppCtx->stCurr.tv_sec-pstInst->tLastRecv);
 	
 	return 0;
}

int tconnd_sendto_lisinst(TCONNTHREAD* pstThread, TCONNINST* pstInst, TDRDATA* pstPkg)
{
	ZONE_CONNCONF* pstConnd;
	ZONE_CONNRUN_STATUS* pstRun;
	CONFINST* pstConfInst;
	NETTRANS* pstNetTrans;
	TRANSINST* pstTransInst;
//	SERINST* pstSerInst;
	NETTRANSRUN* pstTransRun;
	LISTENERRUN* pstLisRun;
//	SERIALIZERRUN* pstSerRun;
	struct epoll_event event;

	int iSend = 0;
	int iRet = 0;

	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;
	pstRun		=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstRunStat;
	pstConfInst	=	pstThread->pstConfInst;
	
	pstNetTrans	=	pstConnd->NetTransList.NetTrans + pstInst->iTransLoc;
	pstTransInst	=	pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	pstTransRun	=	pstRun->NetTransRunList.NetTrans + pstInst->iTransLoc;
	pstLisRun	=	pstRun->ListenerRunList.Listeners + pstInst->iLisLoc;

	if( pstInst->iNeedFree )
	{
		tlog_debug(pstThread->pstLog, 0, 0, "conn(%s:%d) need to free,reason=%d, so cannot send data",
					inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr), 
					NTOHS(((struct sockaddr_in*)&pstInst->stAddr)-> sin_port),
					pstInst->iNeedFree);
	       pstTransRun->TransStat.SendPkgFail++;
		pstTransRun->TransStat.SendPkgFail += pstPkg->iBuff;  
		pstLisRun->TransStat.SendPkgFail++;
		pstLisRun->TransStat.SendByteFail += pstPkg->iBuff;
		return -1;
	}

	if( !pstInst->iSendFirstPkg )
	{
		pstInst->iSendFirstPkg	=	1;
	}

/*
	if (pstInst->iShortCountBuff > 0)
	{
		if (0 > send_short_count(pstThread, pstInst))
		{
			iRet	=	-1;
		}
		else if (pstInst->iShortCountBuff > 0)
		{
			if (0 > store_short_count(pstInst, pstPkg->pszBuff, pstPkg->iBuff))
			{
				iRet	=	-1;
			}
			else
			{
				return 0;
			}
		}
	}

	if (0 > iRet )
	{
		goto _error;
	}
*/

	if( pstInst->fStream )
	{
		iSend	= send(pstInst->s, pstPkg->pszBuff, pstPkg->iBuff, 0);
	}
	else
	{
		iSend	= sendto(pstInst->s, pstPkg->pszBuff, pstPkg->iBuff, 0, &pstInst->stAddr, sizeof(pstInst->stAddr));
	}

	if( iSend==pstPkg->iBuff )
	{
		pstLisRun->TransStat.SendPkgSucc++;
		pstLisRun->TransStat.SendByteSucc +=	iSend;
		pstTransRun->TransStat.SendPkgSucc++;
		pstTransRun->TransStat.SendByteSucc +=	iSend;
		tlog_debug(pstThread->pstLog, 0, 0,"send package to client success:des=%s:%d :datelen =%d\n",inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),iSend);
	}
	else
	{
		pstLisRun->TransStat.SendPkgFail++;
		pstLisRun->TransStat.SendByteFail +=	pstPkg->iBuff;
		pstTransRun->TransStat.SendPkgFail++;
		pstTransRun->TransStat.SendByteFail +=	pstPkg->iBuff;
	}
	
	if( iSend!=pstPkg->iBuff )
	{
		iRet = -1;
		/*
		if (iSend <= 0)
		{
			if (errno == EAGAIN && pstThread->pstAppCtx->stCurr.tv_sec-pstInst->tLastRecv < 65)
			{
				if (0 > store_short_count(pstInst, pstPkg->pszBuff, pstPkg->iBuff))
				{
					iRet	=	-1;
				}
			}
			else
			{
				iRet	=	-1;
			}
		}
		else //不足计数short count 的情况
		{
			show_short_count(pstThread, pstInst, pstPkg->iBuff, iSend);
			
			if (pstThread->pstAppCtx->stCurr.tv_sec-pstInst->tLastRecv < 65)
			{
				iRet = store_short_count(pstInst, pstPkg->pszBuff+iSend, pstPkg->iBuff-iSend);
			}
			else
			{
				iRet = -1;
			}
		}*/
	}

//_error:
	if (0 > iRet)
	{
		/*tlog_error(pstThread->pstLog, 0, 0,"send short count to client failed:des=%s:%d :datelen =%d,send=%d, store buf %d, err=%s",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
							pstPkg->iBuff, iSend, pstInst->iShortCountBuff, strerror(errno));*/

		tlog_error(pstThread->pstLog, 0, 0,"send to client failed:des=%s:%d :datelen =%d,send=%d, err=%s",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
							pstPkg->iBuff, iSend, strerror(errno));
		
		pstInst->iNeedFree	=	STOP_REASON_NETWORK;
		pstInst->iNotify	=	1;
		epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, &event);
	}

	return iRet;
}



int tconnd_send_frame(TCONNTHREAD* pstThread, TDRDATA* pstFrame)
{
	int i;
	int iTotal;
	int iRet = -1;
	CONFINST* pstConfInst;
	TCONNINST* pstInst;
	TCONNINST* pstInstNow;
	ZONE_CONNCONF* pstConnd;
	ZONE_CONNRUN_STATUS* pstRun;
	TRANSINST* pstTransInst;
	LPPDUINST pstPDUInst;
	SERIALIZERRUN* pstSerRun;
//	TFRAMECMDINPROC stInProc;
	TFRAMECMDINPROC* pstInProcNow;

	TDRDATA stMsg;
	TDRDATA stPkg;
	struct epoll_event event;

	if( tconnd_frame2msg(&stMsg, pstFrame, pstThread, &pstInst)<0 )
	{
		//tlog_error(pstThread->pstLog, 0,0, "tconnd_send_frame:failed to convert frame to msg");
		return -1;
	}
	
   	pstConfInst	=	pstThread->pstConfInst;
	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;
	pstRun		=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstRunStat;

	pstTransInst	=	pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;
	pstPDUInst	= 	pstThread->pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;

	pstThread->pstPDUInst	=	pstPDUInst;

	for(i = 0; i < pstTransInst->iSerCount; i++)
	{
		if( pstThread->iLastSrc==pstConfInst->stSerInstList.astInsts[pstTransInst->aiSerLoc[i]].iDst )
		{
			pstSerRun	=	pstRun->SerializerRunList.Serializers + pstTransInst->aiSerLoc[i];
	
			pstSerRun->TransStat.RecvPkgSucc++;
			pstSerRun->TransStat.RecvByteSucc	+=	pstFrame->iBuff;
		}
	}

	iTotal	=	0;

	if (TFRAMEHEAD_CMD_INPROC==pstThread->stFrameHead.Cmd && pstThread->stFrameHead.CmdData.InProc.Flag)
	{
		//tlog_debug(pstThread->pstLog,0,0, "tconnd recv broadcast pkg, flag=%d", pstThread->stFrameHead.CmdData.InProc.Flag);
		
		if (BROADCAST_FLAG_GROUP ==  pstThread->stFrameHead.CmdData.InProc.Flag) //group broadcast
		{
			pstInProcNow	=	&pstThread->stFrameHead.CmdData.InProc;
			
			for(i=0; i<pstInProcNow->Count; i++)
			{
				stPkg.pszBuff	=	pstThread->pszSendBuff;
				stPkg.iBuff =	pstThread->iSendBuff;
			
				pstInstNow	=	tconnd_get_inst(pstThread->pstPool, pstInProcNow->Idents[i].ConnIdx );

				if(!pstInstNow)
				{
					unsigned short ushCmd;
					memcpy(&ushCmd,stMsg.pszBuff+2,2);
					ushCmd = ntohs(ushCmd);
					tlog_error(pstThread->pstLog, 0, 0,
					           "Can not find instance:Idx=%d, cmd=%d",
					           pstInProcNow->Idents[i].ConnIdx, ushCmd);
					continue;
				}

        		// 防止组播发不出去:
        		// 如果影子路由已开启,就不要检查逻辑ID是否相等了,因为两个进程之间
        		// 的逻辑ID可能是不相等的,而这种情况只有在影子路由开启的时候才会
        		// 出现.
				/*
				if(!pstInstNow->stShadowRouting.fEnabled &&
				   pstInstNow->iID != pstInProcNow->Idents[i].ID )
				{
					tlog_error(pstThread->pstLog, 0, 0,
					           "iID in frame Invalid:iID=%d\n",
					           pstInProcNow->Idents[i].ID);
					continue;
				}*/
			
				if( tconnd_msg2pkg(&stPkg, &stMsg, pstThread, pstInstNow)<0 )
				{
					tlog_error(pstThread->pstLog, 0, 0,"tconnd_msg2pkg failed\n");
					continue;
				}
				
				iRet	=	tconnd_sendto_lisinst(pstThread, pstInstNow, &stPkg);	
			
				if( 0==iRet )
				{
					 iTotal++;
				}
			}
		}
		else //broadcast all
		{
			int iIdx;
			int iPos;
			
			if (tconnd_find_used_first(pstThread->pstPool, &iPos)<0)
			{
				tlog_error(pstThread->pstLog, 0, 0, "broadcast all no valid pos");
				return -1;
			}

			while((iIdx=tconnd_find_used_next(pstThread->pstPool, &iPos))>=0)
			{
				pstInstNow	=	tconnd_get_inst(pstThread->pstPool, iIdx);
				if (pstInstNow && !pstInstNow->fListen && pstInstNow->iLisLoc == pstInst->iLisLoc)
				{
					stPkg.pszBuff	=	pstThread->pszSendBuff;
					stPkg.iBuff =	pstThread->iSendBuff;
					if( tconnd_msg2pkg(&stPkg, &stMsg, pstThread, pstInstNow)<0 )
					{
						tlog_error(pstThread->pstLog, 0, 0,"tconnd_msg2pkg failed\n");
						continue;
					}
					
					iRet	=	tconnd_sendto_lisinst(pstThread, pstInstNow, &stPkg);	
				
					if( 0==iRet )
					{
						 iTotal++;
					}
				}
			}
		}

		return iTotal;
	}

	if( ( TFRAMEHEAD_CMD_START == pstThread->stFrameHead.Cmd )
		 &&( iRet == 0 )
		 &&( pstInst->iSendFirstPkg == 1))
	{
         //tconnd_change_sessionkey(pstThread, pstInst);
         ;
	}
		
	stPkg.pszBuff	=	pstThread->pszSendBuff;
	stPkg.iBuff	=	pstThread->iSendBuff;
	if( tconnd_msg2pkg(&stPkg, &stMsg, pstThread, pstInst)<0 )
	{
        tlog_error(pstThread->pstLog, 0, 0,"tconnd_msg2pkg failed\n");
		return -1;
	}

	iRet	=	tconnd_sendto_lisinst(pstThread, pstInst, &stPkg);	

	if( 0==iRet )
	{
	     iTotal++;
	}

	if( TFRAMEHEAD_CMD_STOP==pstThread->stFrameHead.Cmd )
	{
	      pstInst->iNeedFree	=	STOP_REASON_CLOSE;
	      pstInst->iNotify	=	0;
	      epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, &event);

	      //加快关闭速度
	      tnet_close(pstInst->s);
	      
	      tlog_info(pstThread->pstLog, 0,0, "close the connection , id=%d:client(%s:%d:%d), iReason:%d",
					pstInst->iID, inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr), 
					NTOHS(((struct sockaddr_in*)&pstInst->stAddr)-> sin_port),pstInst->s, STOP_REASON_CLOSE);	  

	      pstInst->s	=	-1;		
	}

	return iTotal;
}


int tconnd_proc_send(TCONNTHREAD* pstThread)
{
	int i=0;
	TDRDATA stFrame;
	struct timeval stStart;
	struct timeval stEnd;
	struct timeval stSub;
    int iMaxSend = pstThread->pstAppCtx->iTimer-pstThread->iMsRecv-1;
	int iMsSend=0;
	int iRet=0;
   
	gettimeofday(&stStart, NULL);

	while(1)
	{
		i++;

		/* time based broken. */
		if(i > PAK_PER_SLICE)
		{
			i = i - PAK_PER_SLICE;
			gettimeofday(&stEnd, NULL);
			TV_DIFF(stSub, stEnd, stStart);
			TV_TO_MS(iMsSend, stSub);

			if( iMsSend >= iMaxSend )
			{
				break;
			}
		}

		
		pstThread->iLastDst	=	0;
		pstThread->iLastSrc	=	TBUS_ADDR_ALL;

		iRet=tbus_peek_msg(pstThread->pstAppCtx->iBus, &pstThread->iLastSrc, &pstThread->iLastDst, (const char **)&stFrame.pszBuff, (size_t *)&stFrame.iBuff, 0);
		if( 0 != iRet)
		{
            break;
		}
             //tlog_debug(pstThread->pstLog, 0, 0,"Recv package from tbus:src=%s,datelen=%d\n",inet_ntoa(*(struct in_addr *)&pstThread->iLastSrc),stFrame.iBuff);
		tconnd_send_frame(pstThread, &stFrame);

		/*删除peek msg*/
		iRet=tbus_delete_msg(pstThread->pstAppCtx->iBus, pstThread->iLastSrc, pstThread->iLastDst);
		if( 0 != iRet)
		{
              tlog_error(pstThread->pstLog, 0, 0,"delete package from tbus error:src=%s,des=%s\n", tbus_addr_ntoa((TBUSADDR) pstThread->iLastSrc),tbus_addr_ntoa((TBUSADDR) pstThread->iLastDst));
		}
		
	}

	if( 1==i)
	{
        //recv no package
        return -1;
	}

	return 0;
}


int zone_conn_proc(TAPPCTX *pstAppCtx, ZONE_CONNENV *pstEnv)
{
	int       iRecvOnWork=0;
	int       iSendOnWork=0;
	
	gs_tNow = pstAppCtx->stCurr.tv_sec;

	iRecvOnWork = tconnd_proc_recv(&pstEnv->stThread);

	iSendOnWork = tconnd_proc_send(&pstEnv->stThread);

	if( iRecvOnWork!=0 && iSendOnWork!=0 )
	{
        return -1;
	}

	return 0;
}

int zone_conn_reload(TAPPCTX *pstAppCtx, ZONE_CONNENV *pstEnv)
{
	int i, j;
	int iFind;
	ZONE_CONNCONF   *pstPreConf;
	ZONE_CONNCONF   *pstNowConf;
	CONFINST* pstConfInst;
	NETTRANS *pstNowNetTrans;
	NETTRANS *pstPreNetTrans;
	TRANSINST *pstTransInst;
	
	pstPreConf = (ZONE_CONNCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	pstNowConf = pstEnv->pstConf;

	//允许临时调整ConnLimit 和TransLimit
	pstConfInst = &pstEnv->stConfInst;
	for(i=0; i<pstNowConf->NetTransList.Count; i++)
	{
		pstNowNetTrans = &pstNowConf->NetTransList.NetTrans[i];
		pstTransInst = &pstConfInst->stTransInstList.astInsts[i];

		iFind = 0;
		for (j=0; j<pstPreConf->NetTransList.Count; j++)
		{
			pstPreNetTrans = &pstPreConf->NetTransList.NetTrans[j];
			if (0 == strcmp(pstNowNetTrans->Name, pstPreNetTrans->Name))
			{
				iFind = 1;
				break;
			}
		}

		if (iFind)
		{
			pstNowNetTrans->UseWaitQueue = pstPreNetTrans->UseWaitQueue;
			pstNowNetTrans->QueueNotifyInterval = pstPreNetTrans->QueueNotifyInterval;

			// 强制打开新服状态,但不会强制关闭新服状态
			if(pstPreNetTrans->QueueNewWorld)
			{
				pstNowNetTrans->QueueNewWorld = pstPreNetTrans->QueueNewWorld;
			}
			
			pstNowNetTrans->QueueLimitStart = pstPreNetTrans->QueueLimitStart;
			pstNowNetTrans->QueueLimitLast = pstPreNetTrans->QueueLimitLast;
		
			if (pstNowNetTrans->ConnLimit.Permit != pstPreNetTrans->ConnLimit.Permit && 
				pstPreNetTrans->ConnLimit.Permit < pstNowNetTrans->ConnLimit.MaxConn)
			{
				pstNowNetTrans->ConnLimit.Permit = pstPreNetTrans->ConnLimit.Permit;
			}

			if (pstNowNetTrans->ConnLimit.Speed != pstPreNetTrans->ConnLimit.Speed)
			{
				pstNowNetTrans->ConnLimit.Speed = pstPreNetTrans->ConnLimit.Speed;
			}

			if (pstNowNetTrans->TransLimit.PkgSpeed != pstPreNetTrans->TransLimit.PkgSpeed )
			{
				pstNowNetTrans->TransLimit.PkgSpeed = pstPreNetTrans->TransLimit.PkgSpeed;
				
				pstTransInst->iPkgMaxSpeed	=	pstNowNetTrans->TransLimit.PkgSpeed;;
				pstTransInst->iPkgPermit	=	pstTransInst->iPkgMaxSpeed*pstTransInst->iRecvCheckInterval;
				pstTransInst->iPkgPermitLow	=	pstTransInst->iPkgPermit/2;
				pstTransInst->iPkgPermitHigh	=	pstTransInst->iPkgPermit;
			}

			if (pstNowNetTrans->TransLimit.ByteSpeed != pstPreNetTrans->TransLimit.ByteSpeed )
			{
				pstNowNetTrans->TransLimit.ByteSpeed = pstPreNetTrans->TransLimit.ByteSpeed;

				pstTransInst->iByteMaxSpeed	=	pstNowNetTrans->TransLimit.ByteSpeed;
				pstTransInst->iBytePermit	=	pstTransInst->iByteMaxSpeed*pstTransInst->iRecvCheckInterval;
				pstTransInst->iBytePermitLow	=	pstTransInst->iBytePermit/2;
				pstTransInst->iBytePermitHigh	=	pstTransInst->iBytePermit;
			}
		}
	}
	
	printf("zone_conn reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "zone_conn reload");

	return 0;
}

int zone_conn_fini(TAPPCTX *pstAppCtx, ZONE_CONNENV *pstEnv)
{
	TCONNTHREAD* pstThread;

	UNUSED(pstAppCtx);
	pstThread = &pstEnv->stThread;
	
	tconnd_scan(pstThread, 1);
	
	if( -1!=pstThread->epfd )
	{
		epoll_destroy(pstThread->epfd);
		pstThread->epfd	=	-1;
	}

	if( pstThread->pszSendBuff )
	{
		free(pstThread->pszSendBuff);
		pstThread->pszSendBuff	=	NULL;
	}

	if( pstThread->pszRecvBuff )
	{
		free(pstThread->pszRecvBuff);
		pstThread->pszRecvBuff	=	NULL;
	}

	if( pstThread->pszMsgBuff )
	{
		free(pstThread->pszMsgBuff);
		pstThread->pszMsgBuff	=	NULL;
	}

/*
	if (pstThread->pstShtUinQue)
	{
		sht_destroy(&pstThread->pstShtUinQue);
	}
*/

	tconnapi_fini();

	
	printf("conn finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "conn finish");
	
	return 0;
}

int tconnd_adjust_recvtime(TCONNTHREAD* pstThread)
{
    int       iRecvInChannel = 0;
    int       iSendInChannel = 0;  
    TBUSADDR  iLocalAddr;
	TBUSADDR iPeerAddr;  
    int iRet=0;

	iLocalAddr = pstThread->pstAppCtx->iId;
	iPeerAddr = pstThread->iLastSrc;
	
    if( iPeerAddr > 0 )
    {
        iRet = tbus_get_channel_flux(pstThread->pstAppCtx->iBus, iLocalAddr,iPeerAddr, (int *)&iRecvInChannel, (int *)&iSendInChannel);
		  
	     if( 0 != iRet )
	     {
	        tlog_error(pstThread->pstLog, 0, 0, "tconnd_adjust_recvtime:fail to retrive channle data\n");
		    return -1;        
	     }
    }
	
    if( iRecvInChannel > pstThread->iWaitToSend)
    {
        /*上行允许时间减少一个时间片*/
	    pstThread->iRecvSlices--;
	    /* 至少不能少低于一个时间片  */
	    if( pstThread->iRecvSlices < 1 )
	    {
            pstThread->iRecvSlices = 1;      
	    }
    }
    else if( iRecvInChannel < pstThread->iWaitToSend)
    {
        pstThread->iRecvSlices++;
	      
	    if( pstThread->iRecvSlices > TIME_SLICE_COUNT/2 )
	    {
            pstThread->iRecvSlices= TIME_SLICE_COUNT/2;
	    }
    }

	pstThread->iMsRecvPerLoop = (pstThread->iRecvSlices*pstThread->pstAppCtx->iTimer)/TIME_SLICE_COUNT;
	  
    pstThread->iWaitToSend = iRecvInChannel;

	return 0;
}

int tconnd_notify_serinst_close(TCONNTHREAD* pstThread, TCONNINST* pstInst, int iReason)
{
	int iRet;
	ZONE_CONNCONF* pstConnd;
	TFRAMEHEAD stFrameHead;
	char szPkg[sizeof(TFRAMEHEAD)];
	struct timeval stCurr;
	TDRDATA stHost;
	TDRDATA stNet;

	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;

	stFrameHead.Ver	=	0;
	stFrameHead.Cmd	=	TFRAMEHEAD_CMD_STOP;

	stFrameHead.ID		=	pstInst->iID;
	stFrameHead.ConnIdx=	pstInst->iIdx;

	stFrameHead.ExtraType	=	0;

	if( pstInst->iNeedFree )
	{
		stFrameHead.CmdData.Stop.Reason	=	pstInst->iNeedFree;
	}
	else
	{
		stFrameHead.CmdData.Stop.Reason	=	iReason;
	}

	if( pstInst->iUseTimeStamp )
	{
		stFrameHead.TimeStampType	=	TFRAMEHEAD_TIMESTAMP_TIMEVAL;

		gettimeofday(&stCurr, NULL);
			
		stFrameHead.TimeStamp.TimeVal.sec	=	(int)stCurr.tv_sec;
		stFrameHead.TimeStamp.TimeVal.usec	=	(int)stCurr.tv_usec;
	}
	else
	{
		stFrameHead.TimeStampType	=	0;
	}

	stHost.pszBuff	=	(char*)&stFrameHead;
	stHost.iBuff	=	(int)sizeof(stFrameHead);

	stNet.pszBuff	=	(char*)szPkg;
	stNet.iBuff		=	(int)sizeof(szPkg);

	iRet	=	TCONNAPI_FRAMEHEAD_HTON(&stNet, &stHost, 0);

	if( iRet<0 )
	{
		return -1;
	}

	iRet	=	tconnd_sendto_serinst(pstThread, pstInst, &stNet);

	return iRet;
}

int tconnd_pop_queue(TCONNTHREAD* pstThread, TCONNINST* pstInst)
{
	TRANSINST* pstTransInst;
	TCONNINST* pstPrev;
	TCONNINST* pstNext;

	pstInst->iIsInQueue	=	0;
	pstInst->tOutQueue = gs_tNow;

	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;

	pstTransInst->stConnInfo.Active++;
	pstTransInst->stConnInfo.Wait--;

	//pstTransInst->uiTokenPass	=	pstInst->uiQueueToken;

	if( pstTransInst->iWaitQueueHead == pstInst->iIdx )
	{
		pstTransInst->iWaitQueueHead	=	pstInst->iQueueNext;
	}

	if( pstTransInst->iWaitQueueTail == pstInst->iIdx )
	{
		pstTransInst->iWaitQueueTail	=	pstInst->iQueuePrev;
	}

	if( -1!=pstInst->iQueuePrev )
	{
		pstPrev	=	tconnd_get_inst( pstThread->pstPool, pstInst->iQueuePrev );

		if( pstPrev )
		{
			pstPrev->iQueueNext	=	pstInst->iQueueNext;
		}
	}

	if( -1!=pstInst->iQueueNext )
	{
		pstNext	=	tconnd_get_inst( pstThread->pstPool, pstInst->iQueueNext );

		if( pstNext )
		{
			pstNext->iQueuePrev	=	pstInst->iQueuePrev;
		}
	}

	return 0;
}


int tconnd_safe_close(TCONNTHREAD* pstThread, int iIdx, int iReason, int iIsNotify)
{
	int iRet;
	TCONNINST* pstInst;
	TRANSINST* pstTransInst;

	pstInst	=	tconnd_get_inst( pstThread->pstPool, iIdx);

	if( !pstInst )
	{
		return -1;
	}

	if( pstInst->s>=0 )
	{
		tnet_close(pstInst->s);
		tlog_info(pstThread->pstLog, 0,0, "close the connection with Idx=%d, id=%d:client(%s:%d:%d), iReason:%d",
					iIdx, pstInst->iID, inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr), 
					NTOHS(((struct sockaddr_in*)&pstInst->stAddr)-> sin_port),pstInst->s, iReason);

		pstInst->s	=	-1;
	}

	pstTransInst	=	pstThread->pstConfInst->stTransInstList.astInsts + pstInst->iTransLoc;

	if( -1!=pstInst->iID && pstInst->iSendFirstFrame && iIsNotify )
	{
		iRet	=	tconnd_notify_serinst_close(pstThread, pstInst, iReason);
	}

	if( pstInst->iIsInQueue )
	{
		tconnd_pop_queue(pstThread, pstInst);
	}

	if( pstInst->iAuthPass )
	{
		pstTransInst->stConnInfo.Active--;

		/*
		if (pstInst->iAuthType == AUTH_JC)
		{
			UinQue stUinQue;

			stUinQue.iUin = pstInst->iUin;
			sht_remove(pstThread->pstShtUinQue, &stUinQue, UinQueCmp, UinQueCode);
		}*/
	}
	else
	{
		pstTransInst->stConnInfo.Idle--;
	}

	pstInst->tLastRecv = gs_tNow;
	tconnd_free_inst(pstThread->pstPool, iIdx);

	pstTransInst->stConnInfo.Total--;

	return 0;
}



int tconnd_scan_idle(TCONNTHREAD* pstThread, int iIsShutdown)
{
	CONFINST* pstConfInst;
	ZONE_CONNCONF* pstConnd;
	int iMax;
	int i;
	TMEMBLOCK* pstBlock;
	TCONNINST* pstInst;
	LISTENERLIST* pstListenerList;
	LISTENER* pstListener;
	struct epoll_event event;
	int iIdle;

	pstConfInst	=	pstThread->pstConfInst;
	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;

	iMax	=	TMEMPOOL_GET_CAP(pstThread->pstPool);

	if( !iIsShutdown )
	{
		iMax	/=	pstThread->iScanHerz;
	}

	pstListenerList	=	&pstConnd->ListenerList;

	for(i=0; i<iMax; i++)
	{
		pstBlock	=	TMEMPOOL_GET_PTR(pstThread->pstPool, pstThread->iScanPos);

		pstThread->iScanPos++;

		pstThread->iScanPos	=	pstThread->iScanPos % (TMEMPOOL_GET_CAP(pstThread->pstPool));

		if( !pstBlock->fValid )
		{
			continue;
		}

		pstInst		=	(TCONNINST*)TMEMBLOCK_GET_DATA(pstBlock);

		if( pstInst->fListen && !iIsShutdown )
		{
			continue;
		}

		if( iIsShutdown )
		{
			epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, &event);
      			tconnd_safe_close(pstThread, TMEMPOOL_GET_BLOCK_IDX(pstBlock), STOP_REASON_SHUTDOWN, 1);
		}
		else if( pstInst->iNeedFree )
		{
			tconnd_safe_close(pstThread, TMEMPOOL_GET_BLOCK_IDX(pstBlock), pstInst->iNeedFree, pstInst->iNotify);
		}
		else
		{
			int fNoAuth = 0;

			pstListener	=	pstListenerList->Listeners + pstInst->iLisLoc;
			if( !pstListener->MaxIdle && !pstListener->MaxNoAuthIdle)
			{
				continue;
			}

			if (pstInst->iAuthPass)
			{
				fNoAuth = 0;
				iIdle = pstListener->MaxIdle;
			}
			else
			{
				fNoAuth = 1;
				iIdle = pstListener->MaxNoAuthIdle;
			}

			if(0 == iIdle || (gs_tNow - pstInst->tLastRecv) <= iIdle)
			{
				/*
				if (pstInst->iShortCountBuff > 0)
				{
					if (0 > send_short_count(pstThread, pstInst))
					{
						tlog_error(pstThread->pstLog, 0, 0,"send short count to client failed:des=%s:%d , store buf %d err=%s",
							inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
							pstInst->iShortCountBuff, strerror(errno));
						
						epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, &event);
						tconnd_safe_close(pstThread, TMEMPOOL_GET_BLOCK_IDX(pstBlock), STOP_REASON_NETWORK, 1);
					}
				}*/
				
				continue;
			}

			tlog_info(gs_stEnv.pstLogCat, 0, 0,
			          fNoAuth ?
			          "connection will be close, Reason=NoAuth, CheckIdle=%d" :
			          "connection will be close, Reason=NoActive, CheckIdle=%d",
			          iIdle);

			/* begin to close the socket. */
			epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, &event);
			tconnd_safe_close(pstThread, TMEMPOOL_GET_BLOCK_IDX(pstBlock), STOP_REASON_IDLE, 1);
		}
	}

	return 0;
}

int tconnd_active_waitqueue(TCONNTHREAD* pstThread)
{
	CONFINST* pstConfInst;
	ZONE_CONNCONF* pstConnd;
	ZONE_CONNRUN_STATUS* pstRun;
	TMEMBLOCK* pstBlock;
	NETTRANSLIST* pstNetTransList;
	NETTRANS* pstNetTrans;
	TRANSINSTLIST* pstTransInstList;
	TRANSINST* pstTransInst;
	NETTRANSRUN* pstTransRun;
	TCONNINST* pstInst;
	int i;
	int iMax;
	int iPos;
	int iCount;
	int iPermit;

	pstConfInst	=	pstThread->pstConfInst;
	pstConnd	=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstConf;
	pstRun		=	((ZONE_CONNENV *)(pstThread->pstEnv))->pstRunStat;

	pstTransInstList=	&pstConfInst->stTransInstList;
	pstNetTransList	=	&pstConnd->NetTransList;

	for(i=0; i<pstTransInstList->iCount; i++)
	{
		pstTransInst	=	pstTransInstList->astInsts + i;
		pstNetTrans	=	pstNetTransList->NetTrans + i;
		pstTransRun	=	pstRun->NetTransRunList.NetTrans + i;

		memcpy(&pstTransRun->ConnInfo, &pstTransInst->stConnInfo, sizeof(pstTransRun->ConnInfo));

		if( 0==pstNetTrans->ConnLimit.Permit)
		{
			iPermit	=	pstNetTrans->ConnLimit.MaxConn;
		}
		else
		{
			iPermit	=	pstNetTrans->ConnLimit.Permit;
		}

		if( 0==iPermit )
		{
			iPermit	=	pstConnd->MaxFD;
		}

		if (pstNetTrans->QueueNewWorld && pstNetTrans->QueueLimitLast > 0 && iPermit > pstNetTrans->QueueLimitStart)
		{
			int iPermitTmp;
			
			if (pstTransInst->stConnInfo.Active > pstNetTrans->QueueLimitStart && pstTransInst->stConnInfo.Active < iPermit)
			{
				if (0 == pstTransInst->tQueStartLimit)
				{
					pstTransInst->tQueStartLimit = gs_tNow;
				}
				
				iPermitTmp = pstNetTrans->QueueLimitStart + 
						(gs_tNow - pstTransInst->tQueStartLimit)*(iPermit - pstNetTrans->QueueLimitStart)/pstNetTrans->QueueLimitLast;

				if (iPermitTmp < iPermit)
				{
					iPermit = iPermitTmp;
				}
			}
		}

		iMax	=	pstNetTrans->ConnLimit.Speed/(1000/pstThread->pstAppCtx->iTimer);
		if (0 >= iMax && pstNetTrans->ConnLimit.Speed != 0)
		{
			iMax =1 ;
		}
		// max=0表示不限制
		if ((0 >= iMax) || ( iMax + pstTransInst->stConnInfo.Active>iPermit ))
		{
			iMax	=	iPermit - pstTransInst->stConnInfo.Active;
		}

		iPos	=	pstTransInst->iWaitQueueHead;
		iCount	=	0;
		while(-1!=iPos && iCount<iMax)
		{
			pstBlock	=	TMEMPOOL_GET_PTR(pstThread->pstPool, iPos);

			pstInst		=	(TCONNINST*)TMEMBLOCK_GET_DATA(pstBlock);

			iPos		=	pstInst->iQueueNext;

			iCount++;

			tlog_debug(pstThread->pstLog, 0,0, "conn(s:%d, idx:%d) is active, its auth status:%d",
				pstInst->s, pstInst->iIdx, pstInst->iAuthPass);
			tconnd_pop_queue(pstThread, pstInst);
			tconnd_recv_pkg(pstThread, pstInst);
		}
	}

	return 0;
}

int tconnd_notify_waitqueue(TCONNTHREAD* pstThread)
{
	PDUHEAD stHead;
	CONFINST* pstConfInst;
	ZONE_CONNCONF* pstConnd;
	TMEMBLOCK* pstBlock;
	NETTRANSLIST* pstNetTransList;
	NETTRANS* pstNetTrans;
	TRANSINSTLIST* pstTransInstList;
	TRANSINST* pstTransInst;
	PDUINST* pstPDUInst;
	TCONNINST* pstInst;
	TDRDATA stNet;
	TDRDATA stHost;
	int i;
	int iMax;
	int iPos;
	int iCount;
	int iPosCount;

	pstConfInst	=	pstThread->pstConfInst;
	pstConnd	=	(ZONE_CONNCONF*)pstThread->pstAppCtx->stConfData.pszBuff;

	iMax		=	TMEMPOOL_GET_CAP(pstThread->pstPool);
	iCount		=	0;

	init_base(&stHead.Base);

	stHead.Base.Cmd	=	PDU_CMD_QUEINFO;

	pstTransInstList	=	&pstConfInst->stTransInstList;
	pstNetTransList		=	&pstConnd->NetTransList;

	for(i=0; i<pstTransInstList->iCount; i++)
	{
		pstTransInst	=	pstTransInstList->astInsts + i;
		pstNetTrans	=	pstNetTransList->NetTrans + i;

		if( !pstNetTrans->UseWaitQueue )
		{
			continue;
		}

		pstPDUInst	=	pstConfInst->stPDUInstList.astInsts + pstTransInst->iPDULoc;

		iPos	=	pstTransInst->iWaitQueueHead;

		iPosCount = 0;
		stHead.Ext.QueInfo.Max = pstTransInst->stConnInfo.Wait;
		while(-1!=iPos && iCount<iMax)
		{
			pstBlock	=	TMEMPOOL_GET_PTR(pstThread->pstPool, iPos);

			pstInst		=	(TCONNINST*)TMEMBLOCK_GET_DATA(pstBlock);

			iPos		=	pstInst->iQueueNext;

			iPosCount++;
			iCount++;

			if( gs_tNow - pstInst->tLastRecv < pstNetTrans->QueueNotifyInterval )
			{
				continue;
			}

			pstInst->tLastRecv	=	gs_tNow;

			if( PDULENPARSERID_BY_AUTH!=pstPDUInst->iLenParsertype )
			{
				continue;
			}
			
			//stHead.Ext.QueInfo.Max	= (int) (pstTransInst->uiTokenAlloc - pstTransInst->uiTokenPass);
			//stHead.Ext.QueInfo.Pos	= (int) (pstInst->uiQueueToken - pstTransInst->uiTokenPass);
			stHead.Ext.QueInfo.Pos = iPosCount;

			stHost.pszBuff	=	(char*)&stHead;
			stHost.iBuff	=	(int)sizeof(stHead);
			stNet.pszBuff	=	pstThread->pszSendBuff;
			stNet.iBuff	=	pstThread->iSendBuff;

			if(tdr_hton(pstPDUInst->stLenParser.stAuthParser.pstMetaHead, &stNet, &stHost, PDU_VERSION)<0 )
			{
                            tlog_error(pstThread->pstLog, 0,0, "notify conn(idx:%d) ignroed,tdr_ntoh failed",pstInst->iIdx);
				continue;
			} 
			tconnd_sendto_lisinst(pstThread, pstInst, &stNet);

		}
	}

	return 0;
}

int tconnd_scan(TCONNTHREAD* pstThread, int iIsShutdown)
{
	tconnd_scan_idle(pstThread, iIsShutdown);

	if( !iIsShutdown )
	{
		tconnd_active_waitqueue(pstThread);
		tconnd_notify_waitqueue(pstThread);
	}

	return 0;
}


int zone_conn_tick(TAPPCTX *pstAppCtx, ZONE_CONNENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	gs_tNow	=	pstAppCtx->stCurr.tv_sec;

	tconnd_adjust_recvtime(&pstEnv->stThread);
	tconnd_scan(&pstEnv->stThread, 0);
	return 0;
}

int zone_conn_stop(TAPPCTX *pstAppCtx, ZONE_CONNENV *pstEnv)
{
	int iReady=0;

	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	iReady = 1;
	if (iReady)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/*onidle:修改epoll wait 时间并sleep一会*/
int zone_conn_idle(TAPPCTX* pstAppCtx, ZONE_CONNENV* pstEnv)
{
	UNUSED(pstEnv);

	pstAppCtx->iEpollWait = TCONND_WAIT_TIME;
	usleep(pstAppCtx->iIdleSleep*1000);
	return 0;
}


int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)zone_conn_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_zone_conn;
	gs_stAppCtx.stConfData.pszMetaName = "zone_connconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);

	gs_stAppCtx.iIdleCount = 20;
	gs_stAppCtx.iIdleSleep = 2; 

	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	if (gs_stAppCtx.iTimer < 50)
	{
		gs_stAppCtx.iTimer = 50;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)zone_conn_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)zone_conn_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)zone_conn_tick;
	gs_stAppCtx.pfnIdle  =   (PFNTAPPFUNC)zone_conn_idle;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)zone_conn_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)zone_conn_stop;

	gs_tNow = time(NULL);
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


