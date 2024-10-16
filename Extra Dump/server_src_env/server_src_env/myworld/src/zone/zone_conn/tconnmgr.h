/*
**  @file $RCSfile: tconnmgr.h,v $
**  general description of this module
**  $Id: tconnmgr.h,v 1.9 2012/02/08 09:11:49 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2012/02/08 09:11:49 $
**  @version $Revision: 1.9 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCONNMGR_H
#define TCONNMGR_H

#include "tdr/tdr.h"
#include "tapp/tapp.h"
#include "tlog/tlog.h"
#include "zone_conn_conf_desc.h"
#include "tconnpool.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "op_log_meta.h"
#include "proto_ss.h"
#include "comm/shtable.h"

#include "taes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tagPDULenTDRParserInst	PDULENTDRPARSERINST;
typedef struct tagPDULenTDRParserInst	*LPPDULENTDRPARSERINST;

typedef struct tagPDULenAuthParserInst	PDULENAUTHPARSERINST;
typedef struct tagPDULenAuthParserInst	*LPPDULENAUTHPARSERINST;

typedef union tagPDULenParserInst	PDULENPARSERINST;
typedef union tagPDULenParserInst	*LPPDULENPARSERINST;

typedef struct tagConfInst		CONFINST;
typedef struct tagConfInst		*LPCONFINST;

typedef struct tagTConnThread	TCONNTHREAD;
typedef struct tagTConnThread	*LPTCONNTHREAD;


typedef int (*PFNTCONND_GET_PKGLEN)(TCONNINST* pstInst, LPTCONNTHREAD pstThread);


struct tagTDRInst
{
	char szName[TCONND_NAME_LEN];
	LPTDRMETALIB pstLib;
};

typedef struct tagTDRInst		TDRINST;
typedef struct tagTDRInst		*LPTDRINST;

struct tagTDRInstList
{
	int iCount;
	TDRINST astInsts[TCONND_MAX_TDR];
};

typedef struct tagTDRInstList	TDRINSTLIST;
typedef struct tagTDRInstList	*LPTDRINSTLIST;


/* 使用TDR方法来分析数据包的长度 */
struct tagPDULenTDRParserInst
{
	LPTDRMETA pstPkg;

	int iPkgLenNOff;	/*记录PDU长度成员相对PDU总结构的偏移*/
	int iPkgLenUnitSize;	/*记录PDU长度成员的存储空间*/
	int iHeadLenNOff;	/*记录PDU头部长度成员相对PDU总结构的偏移*/
	int iHeadLenUnitSize;	/*记录PDU头部长度成员的存储空间*/
	int iBodyLenNOff;	/*记录PDU消息体长度成员相对PDU总结构的偏移*/
	int iBodyLenUnitSize;	/*记录PDU消息体长度成员的存储空间*/
};

/* 使用AUTH方法来分析数据包的长度 */
struct tagPDULenAuthParserInst
{
	LPTDRMETA pstMetaHead;
	LPTDRMETA pstMetaGameSig;
	
	int iMaxPkgLen;
	int iHeadLenNOff;
	int iHeadLenUnitSize;
	int iBodyLenNOff;
	int iBodyLenUnitSize;
};

/* 分析协议数据单元(PDU)长度信息的数据结构，用于将数据流分解成应用定义的数据通信消息 */
union tagPDULenParserInst
{
	PDULENTDRPARSERINST stTDRParser;                     	/* PDULENPARSERID_BY_TDR,  使用TDR方法进行分析 */
	PDULENAUTHPARSERINST stAuthParser;
};

struct tagPDUInst
{
	char szName[TCONND_NAME_LEN];
	int iUnit;
	int iUpUnit;
	int iDownUnit;
	int iMinLen;
	int iLenParsertype;                  	/*  Bind Macrosgroup:PDULenParserID,*/
	PDULENPARSERINST stLenParser;                     	/*   分析协议数据单元(PDU)长度信息的成员 */
	PFNTCONND_GET_PKGLEN pfnGetPkgLen; /*分析数据包长度的回调函数*/
	PDU* pstPDU;
};

typedef struct tagPDUInst		PDUINST;
typedef struct tagPDUInst		*LPPDUINST;

struct tagPDUInstList
{
	int iCount;
	PDUINST astInsts[TCONND_MAX_PDU];
};

typedef struct tagPDUInstList	PDUINSTLIST;
typedef struct tagPDUInstList	*LPPDUINSTLIST;

struct tagTransInst
{
	char szName[TCONND_NAME_LEN];
	int iPDULoc;
	int iLisCount;
	int aiLisLoc[TCONND_MAX_NETTRANS];
	int iSerCount;
	int aiSerLoc[TCONND_MAX_NETTRANS];

	int iWaitQueueHead;
	int iWaitQueueTail;
	//unsigned int uiTokenAlloc;
	//unsigned int uiTokenPass;

	int iPkgPermit;
	int iPkgPermitLow;
	int iPkgPermitHigh;
	int iPkgMaxSpeed;

	int iBytePermit;
	int iBytePermitLow;
	int iBytePermitHigh;
	int iByteMaxSpeed;

	int iRecvCheckInterval;

	CONNSTATINFO stConnInfo;

	time_t tQueStartLimit;
};

typedef struct tagTransInst		TRANSINST;
typedef struct tagTransInst		*LPTRANSINST;

struct tagTransInstList
{
	int iCount;
	TRANSINST astInsts[TCONND_MAX_NETTRANS];
};

typedef struct tagTransInstList	TRANSINSTLIST;
typedef struct tagTransInstList	*LPTRANSINSTLIST;

struct tagLisInst
{
	char szName[TCONND_NAME_LEN];
	int iRef;
	LISTENER* pstListener;
};

typedef struct tagLisInst		LISINST;
typedef struct tagLisInst		*LPLISINST;

struct tagLisInstList
{
	int iCount;
	LISINST astInsts[TCONND_MAX_NETTRANS];
};

typedef struct tagLisInstList	LISINSTLIST;
typedef struct tagLisInstList	*LPLISINSTLIST;


struct tagSerInst
{
	char szName[TCONND_NAME_LEN];
	int iDst;
	int iRef;
	int iRes;
	SERIALIZER * pstSerializer;
};

typedef struct tagSerInst		SERINST;
typedef struct tagSerInst		*LPSERINST;

struct tagSerInstList
{
	int iCount;
	SERINST astInsts[TCONND_MAX_NETTRANS];
};

typedef struct tagSerInstList	SERINSTLIST;
typedef struct tagSerInstList	*LPSERINSTLIST;

struct tagConfInst
{
	TDRINSTLIST stTDRInstList;
	PDUINSTLIST stPDUInstList;
	LISINSTLIST stLisInstList;
	SERINSTLIST stSerInstList;
	TRANSINSTLIST stTransInstList;
};


struct tagTConnThread
{
	int iID;
	int iIsValid;
	int iIsExit;
	int epfd;

	pthread_t tid;
	CONFINST* pstConfInst;
	TCONNPOOL* pstPool;
	int iTickCount;

       int iUpUnit;
	int iDownUnit;
	int iPkgUnit;
	int iPoolUnit;
	TAPPCTX *pstAppCtx;
	void *pstEnv;
	int iScanPos;
	
	char* pszSendBuff;
	int iSendBuff;

	char* pszRecvBuff;
	int iRecvBuff;
	char* pszMsgBuff;
	int iMsgBuff;

	int iLastSrc;
	int iLastDst;
	TFRAMEHEAD stFrameHead;
	PDUHEAD stPDUHead;

	PDUINST* pstPDUInst;
	int iScanHerz;

	TLOGCATEGORYINST* pstLog;

	int iMsRecv;	/* the milli-second usecd to receive up message. */
	int iMsRecvPerLoop;
	int iRecvSlices;  /* time slices to receive up message*/
	int iWaitToSend; /*byte wait to send in down channel   */

	AES stAESCtx;
	//LPSHTABLE pstShtUinQue;
};


int tconnd_fini_tdrinstlist(TDRINSTLIST* pstTDRInstList);
int tconnd_init_tdrinstlist(TDRINSTLIST* pstTDRInstList, TDRLIST* pstTDRList);
int tconnd_find_lib(TDRINSTLIST* pstTDRInstList, const char* pszName, LPTDRMETALIB* ppstLib);
int tconnd_find_meta(TDRINSTLIST* pstTDRInstList, const char* pszName, LPTDRMETALIB pstPrefer, LPTDRMETA* ppstFind);
int tconnd_init_pduinst(TDRINSTLIST* pstTDRInstList, PDUINST* pstPDUInst, PDU* pstPDU);
int tconnd_init_pduinstlist(TDRINSTLIST* pstTDRInstList, PDUINSTLIST* pstPDUInstList, PDULIST* pstPDUList);
int tconnd_init_lisinstlist(LISINSTLIST* pstLisInstList, LISTENERLIST* pstListenerList);
int tconnd_fini_lisinstlist(LISINSTLIST* pstLisInstList);
int tconnd_init_serinstlist(SERINSTLIST* pstSerInstList, SERIALIZERLIST* pstSerializerList);
int tconnd_fini_serinstlist(SERINSTLIST* pstSerInstList);
int tconnd_init_transinstlist(TRANSINSTLIST* pstTransInstList, ZONE_CONNCONF* pstConnd, LISINSTLIST* pstLisInstList, SERINSTLIST* pstSerInstList);
int tconnd_fini_confinst(CONFINST* pstConfInst);
int tconnd_init_confinst(CONFINST* pstConfInst, ZONE_CONNCONF* pstConnd);
int tconnd_init_tconndrun(ZONE_CONNRUN_STATUS   *pstRun, ZONE_CONNCONF* pstConnd);


#ifdef __cplusplus
}
#endif

#endif /* TCONNMGR_H */
