/*
**  @file $RCSfile: zone_move_in.c,v $
**  general description of this module
**  $Id: zone_move_in.c,v 1.30 2013/04/07 01:35:14 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/04/07 01:35:14 $
**  @version $Revision: 1.30 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_move.h"
#include "zone_map.h"
#include "zone_act_status.h"
#include "zone_clt.h"

#define MAX_FIRST_ADJUST 16

extern ZONESVRENV gs_stEnv;

int z_search_area_this(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;

	memset(pstSArea, 0, sizeof(SearchArea));
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos);
	if (!pstArea) return -1;
	pstSArea->iNum = 1;
	pstSArea->pastArea[0] = pstArea;
	return 0;
}

int z_search_area_left(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.X -= SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_right(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.X += SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_up(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.Y += SCREEN_YGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_down(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.Y -= SCREEN_YGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_leftdown(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.X -= SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.Y -= SCREEN_YGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.X += SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_leftup(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.X -= SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.Y += SCREEN_YGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.X += SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_rightup(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.X += SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.Y += SCREEN_YGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.X -= SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}

int z_search_area_rightdown(MapIndex* pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea)
{
	AREADYN *pstArea;
	RESPOS stPos;

	if (z_search_area_this(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos, pstSArea) < 0) return -1;
	memcpy(&stPos, pstPos, sizeof(stPos));
	stPos.X += SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.Y -= SCREEN_YGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	stPos.X -= SCREEN_XGRID;
	pstArea = z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, &stPos);
	if (pstArea) pstSArea->pastArea[pstSArea->iNum++] = pstArea;
	return 0;
}


int z_init_search_func(SearchFunc *pstSearchFun)
{
	int i;

	for (i = 0; i < 64; i++) {
		switch (i) {
			case 27:
			case 28:
			case 29:
			case 35:
			case 36:
			case 37:
			case 43:
			case 44:
			case 45:
				pstSearchFun[i] = z_search_area_this;
				break;
			case 24:
			case 25:
			case 26:
			case 32:
			case 33:
			case 34:
			case 40:
			case 41:
			case 42:
				pstSearchFun[i] = z_search_area_left;
				break;
			case 30:
			case 31:
			case 38:
			case 39:
			case 46:
			case 47:
				pstSearchFun[i] = z_search_area_right;
				break;
			case 51:
			case 52:
			case 53:
			case 59:
			case 60:
			case 61:
				pstSearchFun[i] = z_search_area_up;
				break;
			case 3:
			case 4:
			case 5:
			case 11:
			case 12:
			case 13:
			case 19:
			case 20:
			case 21:
				pstSearchFun[i] = z_search_area_down;
				break;
			case 0:
			case 1:
			case 2:
			case 8:
			case 9:
			case 10:
			case 16:
			case 17:
			case 18:
				pstSearchFun[i] = z_search_area_leftdown;
				break;
			case 48:
			case 49:
			case 50:
			case 56:
			case 57:
			case 58:
				pstSearchFun[i] = z_search_area_leftup;
				break;
			case 54:
			case 55:
			case 62:
			case 63:
				pstSearchFun[i] = z_search_area_rightup;
				break;
			case 6:
			case 7:
			case 14:
			case 15:
			case 22:
			case 23:
				pstSearchFun[i] = z_search_area_rightdown;
				break;
		}
	}
	return 0;
}

int z_in_same_area(MapIndex *pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos1, RESPOS *pstPos2)
{
	AREADYN* pstArea1;
	AREADYN* pstArea2;

	pstArea1	=	z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos1);
	pstArea2	=	z_pos_area_dyn(pstMapIdx, pstAreaDynPool, pstMapInst, pstPos2);

	if( pstArea1 && pstArea1==pstArea2 )
		return 1;
	else
		return 0;
}

int z_ani_enter_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst)
{
	AREADYN *pstArea;

	if (!pstMapInst)
	{
		return -1;
	}
	
	if (!(pstArea = z_pos_area_dyn(pstMapIndex, pstAreaDynPool, pstMapInst, pstPos))) 
	{
		return -2;
	}
	if (pstArea->iAnimate >= MAX_AREA_ANIMATE) 
	{
		return -3;
	}
	if (binsert_int(&iID, pstArea->aiAnimate, &pstArea->iAnimate, sizeof(int), 1)) 
	{
		return 0;
	}

	return -4;
}

int z_ani_enter_area_test(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iAniNum, RESPOS *pstPos, ZONEMAPINST* pstMapInst)
{
	AREADYN *pstArea;

	if (!pstMapInst)
	{
		return -1;
	}
	
	if (!(pstArea = z_pos_area_dyn(pstMapIndex, pstAreaDynPool, pstMapInst, pstPos))) 
	{
		return -2;
	}
	
	if (pstArea->iAnimate + iAniNum > MAX_AREA_ANIMATE) 
	{
		return -3;
	}
	
	return 0;
}


int z_ani_leave_area_test(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst)
{
	int iEqu = 0;
	AREADYN *pstArea;

	if (!(pstArea = z_pos_area_dyn(pstMapIndex, pstAreaDynPool, pstMapInst, pstPos))) {
		return -2;
	}

	bsearch_int(&iID, &pstArea->aiAnimate, pstArea->iAnimate, sizeof(int), &iEqu);
	if (iEqu)
	{
		return 0;
	}
	/*if (bdelete_int(&iID, &pstArea->aiAnimate, &pstArea->iAnimate, sizeof(int)))
	{
		return 0;
	}*/

	return -4;
}

int z_ani_leave_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst)
{
	AREADYN *pstArea;

	if (!(pstArea = z_pos_area_dyn(pstMapIndex, pstAreaDynPool, pstMapInst, pstPos))) {
		return -2;
	}

	if (bdelete_int(&iID, &pstArea->aiAnimate, &pstArea->iAnimate, sizeof(int)))
	{
		return 0;
	}

	return -4;
}

int z_item_leave_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst)
{
AREADYN *pstArea;

	if (!(pstArea = z_pos_area_dyn(pstMapIndex, pstAreaDynPool, pstMapInst, pstPos))) {
		return -2;
	}
	
	if (bdelete_int(&iID, &pstArea->aiInanimate, &pstArea->iInanimate, sizeof(int))) {
		return 0;
	}

	return -4;
}

int z_item_enter_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst)
{
	AREADYN *pstArea;

	if (!pstMapInst)
	{
		return -1;
	}
	
	if (!(pstArea = z_pos_area_dyn(pstMapIndex, pstAreaDynPool, pstMapInst, pstPos))) 
	{
		return -2;
	}
	if (pstArea->iInanimate >= MAX_AREA_INANIMATE) 
	{
		return -3;
	}
	if (binsert_int(&iID, pstArea->aiInanimate, &pstArea->iInanimate, sizeof(int), 1)) 
	{
		return 0;
	}

	return -4;
}

Area *z_pos_area(MapObj *pstMap, MapIndex* pstMapIdx, RESPOS *pstPos)
{
	int iDx, iDy;
	
	if (pstMapIdx->iAreaOrigin < 0) return NULL;
	iDx = (pstPos->X) / AREA_XGRID;
	iDy = (pstPos->Y) / AREA_YGRID;
	if (iDx >= pstMapIdx->iXAreaNum) return NULL;
	if (iDy >= pstMapIdx->iYAreaNum) return NULL;
	
	return (Area *)pstMap->sArea[pstMapIdx->iAreaOrigin + iDy * pstMapIdx->iXAreaNum + iDx];
}

AREADYN *z_pos_area_dyn(MapIndex* pstMapIdx, TMEMPOOL *pstAreaPool,ZONEMAPINST* pstMapInst, RESPOS *pstPos)
{
	int iDx, iDy;
	int iAreaDyn;
	TMEMBLOCK *pstMemBlock;
	
	if(!pstMapIdx)
		return NULL;

	iDx = (pstPos->X) / AREA_XGRID;
	iDy = (pstPos->Y) / AREA_YGRID;
	if (iDx >= pstMapIdx->iXAreaNum) return NULL;
	if (iDy >= pstMapIdx->iYAreaNum) return NULL;

	iAreaDyn	=	iDy * pstMapIdx->iXAreaNum + iDx;

	if( iAreaDyn<0 || iAreaDyn>=pstMapInst->iAreaDyn )
		return NULL;

	/* index twice. */
	iAreaDyn	=	pstMapInst->aiAreaDyn[iAreaDyn];
	if( iAreaDyn<0)
		return NULL;

	pstMemBlock = TMEMPOOL_GET_PTR(pstAreaPool, iAreaDyn);
	if (!pstMemBlock->fValid)
		return NULL;

	return (AREADYN *)pstMemBlock->szData;
}

int z_adjust_time(struct timeval *pstCltTime, struct timeval *pstConnectTime, Player *pstPlayer)
{
	long long  llRet;
	
	if (pstPlayer->stOnline.stTimeAdjustCool.tv_sec > 0)
	{
		struct timeval stSub;
		
		TV_DIFF(stSub, pstPlayer->stOnline.stTimeAdjustCool, gs_stEnv.pstAppCtx->stCurr);
		TV_TO_MS(llRet, stSub);
		if (llRet > 0)
		{
			tlog_debug(gs_stEnv.pstLogCat, 0, 0, "adjust time %d player cool, sec %d, usec %d", 
						pstPlayer->stRoleData.Uin, pstPlayer->stOnline.stTimeAdjustCool.tv_sec,
						pstPlayer->stOnline.stTimeAdjustCool.tv_usec);
			return 0;
		}
	}

	llRet = z_compare_time(pstCltTime, pstConnectTime);

	switch(pstPlayer->stOnline.cCheckCltTimeFlag)
	{
		case 1: //开始对时
			pstPlayer->stOnline.llTimeErrAdjust = llRet;
			pstPlayer->stOnline.iTimeAdjustNum = 1;
			
			if (llabs(llRet) < 100)
			{
				pstPlayer->stOnline.cCheckAdust++;
			}
			else
			{
				pstPlayer->stOnline.cCheckAdust = 0;
			}
			
			if (pstPlayer->stOnline.cCheckAdust >= 3)
			{
				pstPlayer->stOnline.llTimeErrAdjust = 0;
				pstPlayer->stOnline.iTimeAdjustNum = 0;
				
				pstPlayer->stOnline.cCheckCltTimeFlag = 2; //对时结束
				pstPlayer->stOnline.cCheckAdust = 0;
				tlog_debug(gs_stEnv.pstLogCat, 0, 0, "adjust time %d player checkflag 1 to 2, bFirstAjustCount %d", pstPlayer->stOnline.bFirstAjustCount);
			}
			break;
		case 2:
			if (llabs(llRet) > 300)
			{
				pstPlayer->stOnline.cCheckAdust++;
			}
			else
			{
				pstPlayer->stOnline.cCheckAdust = 0;
			}

			if (pstPlayer->stOnline.cCheckAdust >= 5)
			{
				pstPlayer->stOnline.cCheckCltTimeFlag = 1; //继续开始对时
				pstPlayer->stOnline.cCheckAdust = 0;
				pstPlayer->stOnline.llTimeErrAdjust = 0;
				pstPlayer->stOnline.iTimeAdjustNum = 0;
				tlog_debug(gs_stEnv.pstLogCat, 0, 0, "adjust time %d player checkflag 2 to 1, llRet %lld, bFirstAjustCount %d", llRet, pstPlayer->stOnline.bFirstAjustCount);
			}
			break;
		
		default:
			return 0;
			break;
	}

	return 0;
}

int z_bench_time(Player *pstPlayer)
{
	int iRet;

	if (!pstPlayer)
		return 0;
	if (pstPlayer->stOnline.iTimeAdjustNum <= 0)
		return 0;

	iRet = pstPlayer->stOnline.llTimeErrAdjust/pstPlayer->stOnline.iTimeAdjustNum;
	pstPlayer->stOnline.llTimeErrAdjust = 0;
	pstPlayer->stOnline.iTimeAdjustNum = 0;
	
	TV_ADD_MS(pstPlayer->stOnline.stTimeAdjustCool, gs_stEnv.pstAppCtx->stCurr, 500);
	tlog_debug(gs_stEnv.pstLogCat, 0, 0, "adjust time set %d player cool, sec %d, usec %d bFirstAjustCount %d", 
						pstPlayer->stRoleData.Uin, pstPlayer->stOnline.stTimeAdjustCool.tv_sec,
						pstPlayer->stOnline.stTimeAdjustCool.tv_usec, pstPlayer->stOnline.bFirstAjustCount);
	
	if (pstPlayer->stOnline.bFirstAjustCount < MAX_FIRST_ADJUST) 
	{
		pstPlayer->stOnline.bFirstAjustCount++;
			
		if (pstPlayer->stOnline.stLastCltTime.tv_sec > 0)
		{
			TV_ADD_MS(pstPlayer->stOnline.stLastCltTime, pstPlayer->stOnline.stLastCltTime, iRet);
		}

		if (pstPlayer->stOnline.stLastCltMoveTime.tv_sec > 0)
		{
			TV_ADD_MS(pstPlayer->stOnline.stLastCltMoveTime, pstPlayer->stOnline.stLastCltMoveTime, iRet);
		}

		if (pstPlayer->stOnline.stLastCltStopTime.tv_sec > 0)
		{
			TV_ADD_MS(pstPlayer->stOnline.stLastCltStopTime, pstPlayer->stOnline.stLastCltStopTime, iRet);
		}
	}
	
	return iRet;
}

RESPOS* z_pos_search_pos(MapIndex *pstMapIdx, RESPOS *pstPos, RESPOS *pstSearchPos)
{
	unsigned int iSDistX, iSDistY;

	iSDistX = SCREEN_XGRID / 4;
	iSDistY = SCREEN_YGRID / 4;
	pstSearchPos->X = pstPos->X + iSDistX / 2;
	pstSearchPos->Y = pstPos->Y + iSDistY / 2;

	pstSearchPos->X = ((pstSearchPos->X) / iSDistX) * iSDistX;
	pstSearchPos->Y = ((pstSearchPos->Y) / iSDistY) * iSDistY;
	if (pstSearchPos->X < pstMapIdx->stEnd.X && pstSearchPos->Y < pstMapIdx->stEnd.Y)
	{
		return pstSearchPos;
	}
	
	memcpy(pstSearchPos, pstPos, sizeof(*pstSearchPos));
	pstSearchPos->X =((pstSearchPos->X) / iSDistX) * iSDistX;
	pstSearchPos->Y = ((pstSearchPos->Y) / iSDistY) * iSDistY;
	if (pstSearchPos->X < pstMapIdx->stEnd.X && pstSearchPos->Y < pstMapIdx->stEnd.Y)
	{
		return pstSearchPos;
	}
	
	return NULL;
}



int z_pos_search_area(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos,
							SearchArea *pstSArea)
{
	MapIndex* pstMapIdx;
	int iFuncIndex;

	pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
	{
		pstSArea->iNum	=	0;
		return 0;
	}

	iFuncIndex = (((pstPos->Y) % AREA_YGRID) / (AREA_YGRID / 8)) * 8 + ((pstPos->X) % AREA_XGRID) / (AREA_XGRID / 8);
	if (iFuncIndex < 0 || iFuncIndex >= 64) 
	{
		pstSArea->iNum = 0;
		return 0;
	}
	
	if (0 > pstEnv->astSearchFuns[iFuncIndex](pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos, pstSArea))
		return -1;

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


inline int z_pos_in_view(RESPOS *pstPos1, RESPOS *pstPos2)
{
	if (abs(pstPos1->X - pstPos2->X) > SCREEN_XGRID * 3 /4) return 0;
	if (abs(pstPos1->Y - pstPos2->Y) > SCREEN_YGRID * 3 /4) return 0;
	return 1;
}

inline double z_distance(RESPOS* pstPos1, RESPOS* pstPos2)
{
	int iDx, iDy;
	double dDist;

	iDx = abs(pstPos1->X - pstPos2->X);
	iDy = abs(pstPos1->Y - pstPos2->Y);
	if (iDx > 100000 || iDy > 100000) return 100000;
	dDist = hypot(iDx, iDy);
	return dDist;
}

inline int z_same_pos(RESPOS *pstPos1, RESPOS *pstPos2)
{
	if (pstPos1->X != pstPos2->X) return 0;
	if (pstPos1->Y != pstPos2->Y) return 0;
	return 1;
}

/*inline int z_compare_time(struct timeval *pstTime1, struct timeval *pstTime2)
{
	if (pstTime1->tv_sec < 0 || pstTime1->tv_usec < 0 ||
			pstTime2->tv_sec < 0 || pstTime2->tv_usec < 0 || pstTime1->tv_usec > 1000*1000 ||  pstTime2->tv_usec > 1000*1000)
		return -100000;

	return (pstTime2->tv_sec - pstTime1->tv_sec)*1000 + (pstTime2->tv_usec - pstTime1->tv_usec)/1000;
}*/
	
inline long long z_compare_time(struct timeval *pstTime1, struct timeval *pstTime2)
{
	long long llCmp = -100000;
	
	if (pstTime1->tv_sec < 0 || pstTime1->tv_usec < 0 ||
			pstTime2->tv_sec < 0 || pstTime2->tv_usec < 0 || pstTime1->tv_usec > 1000*1000 ||  pstTime2->tv_usec > 1000*1000)
		return llCmp;

	llCmp = ((long long)pstTime2->tv_sec - (long long)pstTime1->tv_sec)*1000 + ((long long)pstTime2->tv_usec - (long long)pstTime1->tv_usec)/1000;
	
	return llCmp;
}	

inline int z_pos_in_view_half(RESPOS *pstPos1, RESPOS *pstPos2)
{
	if (abs(pstPos1->X - pstPos2->X) > SCREEN_XGRID * 3 /8) return 0;
	if (abs(pstPos1->Y - pstPos2->Y) > SCREEN_YGRID * 3 /8) return 0;
	return 1;
}

inline int z_pos_in_view_half1(RESPOS *pstPos1, RESPOS *pstPos2)
{
	if (abs(pstPos1->X - pstPos2->X) > SCREEN_XGRID /2) return 0;
	if (abs(pstPos1->Y - pstPos2->Y) > SCREEN_YGRID /2) return 0;
	return 1;
}

inline int z_pos_in_view_half2(RESPOS *pstPos1, RESPOS *pstPos2)
{
	if (abs(pstPos1->X - pstPos2->X) > SCREEN_XGRID * 5 /8) return 0;
	if (abs(pstPos1->Y - pstPos2->Y) > SCREEN_YGRID * 5 /8) return 0;
	return 1;
}

inline int z_is_point_inseg( int x, int y, int x1, int y1, int x2, int y2 )
{
	int r, dist;

	if( x1 <= x2 )
	{
		if( x<x1 || x>x2 ) return 0;
	}
	else if( x<x2 || x>x1 ) return 0;

	if( y1 <= y2 )
	{
		if( y<y1 || y>y2 ) return 0;
	}
	else if( y<y2 || y>y1 ) return 0;

	r = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
	if( r == 0 ) return 1;
	dist = (x-x1)*(y2-y1) - (y-y1)*(x2-x1);
	dist = dist * dist / r;

	return dist <= 2 ? 1 : 0;
}

int z_point_on_recordpath(int iX, int iY, char cStart, char cPoints, RESPOS *pstTurnPoints, unsigned char bFuturePoint)
{
	int cA, cB;
	int i;

/*
	for (i = 0; i < cPoints; i++) {
		cA = (cStart + i) % RECORD_TURN;
		cB = (cStart + i + 1) % RECORD_TURN;
		if (z_is_point_inseg(iX, iY, pstTurnPoints[cA].X, pstTurnPoints[cA].Y, pstTurnPoints[cB].X, pstTurnPoints[cB].Y)) return i + 1;
	}
*/
       //todo: 在极小概率下有重复的点，这时应该借助bFuturePoint辅助判断是哪个点

	for (i=cPoints-1; i>=0; i--)
	{
		cA = (cStart + i) % RECORD_TURN;
		cB = (cStart + i + 1) % RECORD_TURN;
		if (z_is_point_inseg(iX, iY, pstTurnPoints[cA].X, pstTurnPoints[cA].Y, pstTurnPoints[cB].X, pstTurnPoints[cB].Y)) return i + 1;
	}

	return 0;
	UNUSED(bFuturePoint);	//add by paraunused.pl
}

int z_turn_point_dist(RESPOS *pstTurnPoints, char cStart, char cEnd)
{
	double iDist;
	int cNext;

	if (cStart == cEnd) return 0;
	iDist = 0;
	while (1) {
		cNext = (cStart + 1) % RECORD_TURN;
		iDist += z_distance(&pstTurnPoints[(int)cStart], &pstTurnPoints[cNext]);
		if (cNext == cEnd) break;
		cStart = (cStart + 1) % RECORD_TURN;
	}
	return iDist;
}

