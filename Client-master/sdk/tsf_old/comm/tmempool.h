/*
**  @file $RCSfile: tmempool.h,v $
**  general description of this module
**  $Id: tmempool.h,v 1.7 2007/12/24 06:43:41 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2007/12/24 06:43:41 $
**  @version $Revision: 1.7 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TMEMPOOL_H
#define TMEMPOOL_H

#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define TMEMPOOL_MAGIC			0x4324
#define TMEMPOOL_BUILD			0x01

#define TMEMPOOL_FIFO			0x00
#define TMEMPOOL_LIFO			0x01

struct tagTMemBlock
{
	int fValid;
	int iIdx;
	int iNext;
	int iCount;
	char szData[1];
};

typedef struct tagTMemBlock		TMEMBLOCK;
typedef struct tagTMemBlock		*LPTMEMBLOCK;

#define TMEMBLOCK_IS_VALID(p)	((p)->fValid)
#define TMEMBLOCK_GET_DATA(p)	((p)->szData)

struct tagTMemPool
{
	int iMagic;
	int iBuild;
	int iMax;
	int iUnit;

	int iUsed;
	int iRealUnit;
	int iSize;
	int iRealSize;

	int iLastIdx;
	int iInited;
	int iIsCalloc;
	int iMethod;

	int iFreeHead;
	int iFreeTail;
	int iStart;
	int iEnd;

	char szRes[64];

	char szBlocks[1];
};

typedef struct tagTMemPool		TMEMPOOL;
typedef struct tagTMemPool		*LPTMEMPOOL;


#define TMEMPOOL_GET_PTR(pool, idx)	(TMEMBLOCK*)((pool)->szBlocks + (pool)->iRealUnit*((idx) % (pool)->iMax) )
#define TMEMPOOL_GET_CAP(pool)		((pool)->iMax)

#define TMEMPOOL_GET_BLOCK_IDX(blk)	((blk)->iIdx)

#define TMEMPOOL_MIN_SIZE()			( offsetof(TMEMPOOL, szBlocks) )

#define TMEMPOOL_ROUND(unit, round)	( ( ( (unit) + (round) - 1 ) / (round) ) * (round) )

#define TMEMPOOL_ROUND_UNIT(unit)	TMEMPOOL_ROUND(unit, 256)

#define TMEMPOOL_ROUND_POOL(unit)	TMEMPOOL_ROUND(unit, 1024)

#define TMEMPOOL_REAL_UNIT(unit)	TMEMPOOL_ROUND_UNIT( ( (unit) + offsetof(TMEMBLOCK, szData) ) )

#define TMEMPOOL_CALC(max, unit)	( offsetof(TMEMPOOL, szBlocks) + (max)*TMEMPOOL_REAL_UNIT(unit) )


int tmempool_init(TMEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, int iSize);

int tmempool_new(TMEMPOOL** ppstPool, int iMax, int iUnit);

int tmempool_attach(TMEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, int iSize);

int tmempool_destroy(TMEMPOOL** ppstPool);

int tmempool_set_method(TMEMPOOL* pstPool, int iMethod);

void* tmempool_get(TMEMPOOL* pstPool, int iIdx);

int tmempool_alloc(TMEMPOOL* pstPool);

int tmempool_free(TMEMPOOL* pstPool, int iIdx);

int tmempool_fix(TMEMPOOL* pstPool);

int tmempool_find_used_first(TMEMPOOL* pstPool, int* piPos);
int tmempool_find_used_next(TMEMPOOL* pstPool, int* piPos);

#ifdef __cplusplus
}
#endif

#endif /* TMEMPOOL_H */
