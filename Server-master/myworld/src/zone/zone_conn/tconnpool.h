/*
**  @file $RCSfile: tconnpool.h,v $
**  general description of this module
**  $Id: tconnpool.h,v 1.7 2013/01/04 04:21:53 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/01/04 04:21:53 $
**  @version $Revision: 1.7 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCONNPOOL_H
#define TCONNPOOL_H

#include "pal/pal.h"
#include "comm/tmempool.h"
#include "taes.h"
#include "proto_comm.h"
#include "proto_cs.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tagTConnInst		TCONNINST;
typedef struct tagTConnInst		*LPTCONNINST;


typedef struct tagShadowRouting
{
	int fEnabled;     // Ӱ��·���Ƿ���Ч0:����Ч;����:��Ч
	int iReplacedIdx; // ���滻��Serializer����
	int iRealBusID;   // ��ʵ��BusID
}ShadowRouting;

struct tagTConnInst
{
	short fListen;
	short fStream;
	int iIdx;
	int iID;

	int iAuthPass;
	int iAuthType;
	int iUin;
	int iUinFlag;
	char BirthDay[BIRTHDAY_LEN];
	int iAccSilenceEnd;

	AES stGameKey;

	int s;
	int iLisLoc;
	int iTransLoc;
	int iUseTimeStamp;
	// Ӱ��·��: 
	// ��Ӱ��·����Ч��ʱ�򣬴�������յ����ݽ��ᷢ��Ӱ��·��
	// �����ᱻ���͵�ԭ���趨��Serialzier��
	ShadowRouting stShadowRouting;

	int iBuff;
	int iOff;
	int iData;
	int fWaitFirstPkg;

	int iHeadLen;
	int iPkgLen;

	int iRecvMsg;
	int iRecvByte;

	time_t tLastRecvMsgCheck;
	struct sockaddr stAddr;
	time_t tCreate;
	time_t tLastRecv;
	
	//unsigned int uiQueueToken;
	int iIsInQueue;
	int iQueuePrev;
	int iQueueNext;

	time_t tInQueue;
	time_t tOutQueue;
	int iSendFirstFrame;
	int iSendFirstPkg;

	int iNeedFree;
	int iNotify;

	//int iShortCountBuff;

	char szBuff[1];
};

typedef TMEMPOOL				TCONNPOOL;
typedef TMEMPOOL				*LPTCONNPOOL;

#define tconnd_init_pool(ppstPool, iMax, iUnit)	tmempool_new(ppstPool, iMax, iUnit)
#define tconnd_fini_pool(ppstPool)				tmempool_destroy(ppstPool)
#define tconnd_get_inst(pstPool, iIdx)			(TCONNINST*)tmempool_get(pstPool, iIdx)
#define tconnd_alloc_inst(pstPool)				tmempool_alloc(pstPool)
#define tconnd_free_inst(pstPool, iIdx)			tmempool_free(pstPool, iIdx)
#define tconnd_find_used_first(pstPool, piIdx)    tmempool_find_used_first(pstPool, piIdx) 
#define tconnd_find_used_next(pstPool, piIdx)    tmempool_find_used_next(pstPool, piIdx) 

#ifdef __cplusplus
}
#endif

#endif /* TCONNPOOL_H */

