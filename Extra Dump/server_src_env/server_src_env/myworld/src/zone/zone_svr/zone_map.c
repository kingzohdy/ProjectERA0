/*
**  @file $RCSfile: zone_map.c,v $
**  general description of this module
**  $Id: zone_map.c,v 1.1024 2014/06/30 08:56:59 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/30 08:56:59 $
**  @version $Revision: 1.1024 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_map.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_clt.h"
#include "zone_act_status.h"
#include "zone_npc.h"
#include "zone_move.h"
#include "zone_script.h"
#include "zone_ai.h"
#include "zone_skill.h"
#include "zone_team.h"
#include "zone_db.h"
#include "comm_misc.h"
#include "zone_script.h"
#include "zone_tmp.h"
#include "zone_package.h"
#include "zone_status.h"
#include "zone_combat.h"
#include "zone_player.h"
#include "zone_oplog.h"
#include "zone_attr.h"
#include "zone_clan.h"
#include "zone_pet.h"
#include "zone_fairy.h"
#include "tconnapi.h"
#include "zone_range.h"
#include "zone_autoteam.h"
#include "zone_machine.h"
#include "zone_mail.h"
#include "zone_misc.h"
#include "comm_misc.h"
#include "zone_machine_view.h"
#include "zone_battle.h"
#include "zone_diffpworld.h"
#include "zone_spansvr.h"
#include "zone_collect.h"
#include "zone_tmp.h"
#include "zone_city.h"
#include "zone_clan_city.h"
#include "zone_clan_construction.h"
#include "zone_unreal.h"
#include "zone_collect.h"
#include "zone_designation.h"
#include "zone_clan_advance.h"
#include "zone_strongpoint.h"
#include "zone_autopworld.h"
#include "zone_span.h"
#include "zone_boss.h"
#include "zone_sparta.h"
#include "zone_pworld.h"
#include "zone_vip.h"
#include "zone_err.h"
#include "zone_losttotem.h"
#include "zone_zh_name.h"
#include "zone_clan_pet.h"
#include "zone_dianfeng.h"
#include "zone_ploy.h"
#include "zone_zh_name.h"
#include "zone_span_strong.h"
#include "zone_home.h"
#include "zone_shikong.h"

extern int g_dartcar_allow;
extern int g_iDesigVar;
#define MSK_STEP 5
int g_iTimeLimit;

int g_iCheckSpanMap = 0;


#define IS_THROUGH_CHECK_LINESEG_MIN_LEN 50
#define PWORLD_NOPLAYER_TIME 180
#define MAP_AREA_PROTECTED_TIME 15
//进入副本后2分钟，不管有没有操作，都会加次数
#define PWORLD_ADD_LIMIT_TIME 120

#define DYN_MSK_BITS 2 //行走阻挡和魔法阻挡

//以下宏只对DYN_MSK_BITS 为2 4 8时有效
#define SET_DYN_MSK_BIT(Idx, Msk,Bit) (((char *)Msk)[(Idx)*DYN_MSK_BITS/8] |= ((Bit)<<(((Idx)*DYN_MSK_BITS)%8)))
#define CLEAR_DYN_MSK_BIT(Idx, Msk,Bit) (((char *)Msk)[(Idx)*DYN_MSK_BITS/8] &= (~((Bit)<<(((Idx)*DYN_MSK_BITS)%8))))
#define IS_DYN_MSK_BIT_SET(Idx, Msk,Bit) (((char *)Msk)[(Idx)*DYN_MSK_BITS/8]&((Bit)<<(((Idx)*DYN_MSK_BITS)%8)))

int g_iCreatePloyPworldPlayerNum = CREATE_PLOY_PWORLD_PLAYER_NUM;

int g_iMapPlayerNormalNum = MAP_PLAYER_NUM_NORMAL;
int g_iMapPlayerBusyNum = MAP_PLAYER_NUM_BUSY;

#define BCT_RAFFLE_LAYER 15

int player_leave_map_action(ZONESVRENV* pstEnv, Player *pstPlayer);

extern ScriptEnv stSEnv;
extern int g_team_follow_flag;

int g_RanPosDirCurr = 0;
int g_RanPosFaceCurr = 0;
int g_RanPosFaceIdx = 0;

typedef struct {
	RESPOS s_StartPt;
	RESPOS s_dPt;
	RESPOS s_CurGrid;
} GridPoint_t;

/*static RESPOS s_pos[] =
{
	{  1, 	1 },
	{ -1, 	1 },
	{  1,	-1},
	{ -1,	-1 },
};*/

int z_calc_mapidx(MapIndex *pstIdx)
{
	int iXDist, iYDist;

	if (pstIdx->stEnd.X <= 0 || pstIdx->stEnd.Y <= 0) return -1;
	iXDist = pstIdx->stEnd.X;
	iYDist = pstIdx->stEnd.Y;
	pstIdx->iXAreaNum = iXDist / AREA_XGRID;
	pstIdx->iYAreaNum = iYDist / AREA_YGRID;
	if (iXDist % AREA_XGRID) pstIdx->iXAreaNum++;
	if (iYDist % AREA_YGRID) pstIdx->iYAreaNum++;
	
	return 0;
}


int z_clear_all_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Npc *pstNpc;
	
	for (i=0; i<MAX_NPC; i++)
	{
		pstNpc = tmempool_get_bypos(pstEnv->pstNpcPool, i);
		if (!pstNpc)
		{
			continue;
		}

		z_monster_leave_map(pstAppCtx, pstEnv, &pstNpc->stMon);

		z_free_monster(pstAppCtx, pstEnv, &pstNpc->stMon);
	}
		
	return 0;
}

int z_clear_all_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Monster *pstMon;
	
	for (i=0; i<MAX_MONSTER; i++)
	{
		pstMon = tmempool_get_bypos(pstEnv->pstMonsterPool, i);
		if (!pstMon)
		{
			continue;
		}

		//机械回收
		if(is_mon_machine(pstMon))
		{
			mon_machine_back(pstEnv, pstMon);
			continue;
		}

		//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
		if (MONSTER_DIE != pstMon->cStat || pstMon->tLeaveMap > 0)
		{
			z_monster_leave_map(pstAppCtx, pstEnv, pstMon);
		}
		
		z_free_monster(pstAppCtx, pstEnv, pstMon);
	}
		
	return 0;
}



int z_load_msk_fromfile(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMskFile, MapIndex *pstMapIdx, TMAPMASK* pstMask)
{	
	int iMaskSize;
	int fAlloc;
	int iTotalArea=0;
	MapObj *pstMapObj = pstEnv->pstMapObj;
	UNUSED(pstAppCtx);
	
	//从文件装载msk, 修改pstMapIdx 相关属性
	if( pstMapIdx->iMaxMaskSize<=0)
	{
		iMaskSize = MAX_MASK_DATA - pstMapObj->iMask;
		pstMask = (TMAPMASK *)(pstMapObj->sMask + pstMapObj->iMask);
		fAlloc	=	1;
	}
	else
	{
		pstMask = (TMAPMASK *)(pstMapObj->sMask + pstMapIdx->iMaskOrigin);
		iMaskSize = pstMapIdx->iMaxMaskSize;
		fAlloc	=	0;
		iTotalArea = pstMapIdx->iXAreaNum * pstMapIdx->iYAreaNum;
	}

	if (!LoadMaskFile(pstMask, &iMaskSize, pszMskFile))
	{
		return -1;
	}

	pstMask->m_iSize = pstMask->m_lWidthMasks * pstMask->m_lHeightMasks;
	pstMapIdx->stEnd.X = pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	pstMapIdx->stEnd.Y = pstMask->m_lHeightMasks * pstMask->m_lMaskPixelHeight;

	if (0 > z_calc_mapidx(pstMapIdx))
	{
		return -1;
	}

	if( fAlloc )
	{
		pstMapIdx->iMaskOrigin	=	pstMapObj->iMask;
		pstMapIdx->iMaxMaskSize	=	iMaskSize;

		iTotalArea = pstMapIdx->iXAreaNum * pstMapIdx->iYAreaNum;
		pstMapIdx->iAreaOrigin = pstMapObj->iArea;

		if( pstMapObj->iArea + iTotalArea > MAX_AREA)
		{
			return -1;
		}
		
		pstMapObj->iArea		+=	iTotalArea;

		pstMapObj->iMask	+=	pstMapIdx->iMaxMaskSize;
	}
	else
	{
		if (iTotalArea != pstMapIdx->iXAreaNum * pstMapIdx->iYAreaNum)
		{
			return -1;
		}
	}

	if (AREA_XGRID*AREA_YGRID*DYN_MSK_BITS/(pstMask->m_lMaskPixelWidth*pstMask->m_lMaskPixelHeight*8) != DYN_MSK_DATA)
	{
		printf("dyn msk data err\n");
		exit(-1);
	}

	return 0;
}

MapIndex *z_get_refer_mapidx(ZONESVRENV* pstEnv, unsigned int iReferID)
{
	int i;
	MapIndex *pstMapIdx;
	MapObj *pstMapObj = pstEnv->pstMapObj;

	for (i=0; i<pstMapObj->iMapIndex; i++)
	{
		pstMapIdx = (MapIndex *)pstMapObj->sMapIdx[i];
		if (iReferID == pstMapIdx->stMapDef.MapID)
		{
			return pstMapIdx;
		}
	}

	return NULL;
}

int z_set_refer_mapidx(ZONESVRENV* pstEnv, MapIndex *pstMapIdxRefer, MapIndex *pstMapIdx)
{
	int iTotalArea;
	MapObj *pstMapObj = pstEnv->pstMapObj;

	iTotalArea = pstMapIdxRefer->iXAreaNum * pstMapIdxRefer->iYAreaNum;
	
	pstMapIdxRefer->iXAreaNum = pstMapIdx->iXAreaNum;
	pstMapIdxRefer->iYAreaNum = pstMapIdx->iYAreaNum;

	if (pstMapIdxRefer->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		pstMapIdxRefer->iAreaOrigin = pstMapIdx->iAreaOrigin;
	}
	else
	{
		if (pstMapIdxRefer->iAreaOrigin)
		{
			if (iTotalArea != pstMapIdxRefer->iXAreaNum * pstMapIdxRefer->iYAreaNum)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d area err", pstMapIdxRefer->stMapDef.MapID);
				return -1;
			}
		}
		else
		{
			iTotalArea = pstMapIdxRefer->iXAreaNum * pstMapIdxRefer->iYAreaNum;
			
			pstMapIdxRefer->iAreaOrigin = pstMapObj->iArea;

			if( pstMapObj->iArea + iTotalArea > MAX_AREA)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d area err", pstMapIdxRefer->stMapDef.MapID);
				return -1;
			}
			
			pstMapObj->iArea		+=	iTotalArea;
		}
	}
			
	pstMapIdxRefer->iMaxMaskSize = pstMapIdx->iMaxMaskSize;
	pstMapIdxRefer->iMaskOrigin = pstMapIdx->iMaskOrigin;
	pstMapIdxRefer->stEnd = pstMapIdx->stEnd;
	pstMapIdxRefer->iMaxProcSize = pstMapIdx->iMaxProcSize;
	pstMapIdxRefer->tMask = pstMapIdx->tMask;
	
	return 0;
}

int z_load_msk_proc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i, j;
	char szMskFile[64];
	TMAPMASK* pstMask;
	MapIndex *pstMapIdx;
	MapIndex *pstMapIdxRefer;
	int iRet;
	Area *pstArea;
	int iAreaIdx;
	int iTotalArea;
	int iIdx;
	PROCEVENT *pstProcEvt;
	MapObj *pstMapObj = pstEnv->pstMapObj;


	for (i=0; i<pstMapObj->iMapIndex; i++)
	{
		pstMapIdx = (MapIndex *)pstMapObj->sMapIdx[i];
		pstMask = (TMAPMASK *)(pstMapObj->sMask + pstMapObj->iMask);

		pstMapIdxRefer = NULL;
		if (pstMapIdx->stMapDef.ReferMapID > 0)
		{
			pstMapIdxRefer = pstMapIdx;
				
			snprintf(szMskFile, sizeof(szMskFile), "%s/%d.msk", RES_DIR, pstMapIdx->stMapDef.ReferMapID);

			pstMapIdx = z_get_refer_mapidx(pstEnv, pstMapIdx->stMapDef.ReferMapID);
			if (NULL == pstMapIdx)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "%d z_get_refer_mapidx %d fail", pstMapIdxRefer->stMapDef.MapID,
					pstMapIdxRefer->stMapDef.ReferMapID);
				continue;
			}
		}
		else
		{
			snprintf(szMskFile, sizeof(szMskFile), "%s/%d.msk", RES_DIR, pstMapIdx->stMapDef.MapID);
		}
		iRet = cmp_modify_time(szMskFile, &pstMapIdx->tMask);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "load msk from file %s fail", szMskFile);
			continue;
			//exit(-1);
		}
		else if (0 == iRet)
		{
			if (pstMapIdxRefer)
			{
				z_set_refer_mapidx(pstEnv, pstMapIdxRefer, pstMapIdx);
			}
			
			continue;
		}

		if (0 > z_load_msk_fromfile(pstAppCtx, pstEnv, szMskFile, pstMapIdx, pstMask))
		{
			pstMapIdx->tMask = 0;
			pstMapIdx->iMaxMaskSize = 0;
			tlog_error(pstEnv->pstLogCat, 0, 0, "load msk from file %s fail", szMskFile);
			printf("load msk from file %s fail\n", szMskFile);

			continue;

			//exit(-1);
		}
		
		if (pstMapIdxRefer)
		{
			z_set_refer_mapidx(pstEnv, pstMapIdxRefer, pstMapIdx);
		}
	}

	printf("Area=%d, Mask=%d\n", pstMapObj->iArea, pstMapObj->iMask);

	memset(pstMapObj->sArea, 0, sizeof(pstMapObj->sArea));
	
	for (i=0; i<pstMapObj->iProcDef; i++)
	{
		pstProcEvt = (PROCEVENT *)pstMapObj->sProcDef[i];
		pstMapIdx = z_mappos_index(pstMapObj, &iIdx, pstProcEvt->MapID);
		if (NULL == pstMapIdx)
		{
			printf("proc %d map %d error\n", pstProcEvt->ProcID, pstProcEvt->MapID);
			tlog_error(pstEnv->pstLogCat, 0, 0, "proc %d map %d error\n", pstProcEvt->ProcID, pstProcEvt->MapID);
			continue;
			//return -1;
		}
		
		iTotalArea = pstMapIdx->iXAreaNum * pstMapIdx->iYAreaNum;

		for (j=0; j<(int)pstProcEvt->AreaNum; j++)
		{
			iAreaIdx = pstProcEvt->AreaIndexList[j];
			if (iAreaIdx >= iTotalArea)
			{
				printf("%d map total area %d, proc event %d area idx %d\n", pstMapIdx->stMapDef.MapID, iTotalArea,
						pstProcEvt->ProcID, iAreaIdx);
				tlog_error(pstEnv->pstLogCat, 0, 0, "%d map total area %d, proc event %d area idx %d\n", pstMapIdx->stMapDef.MapID, iTotalArea,
						pstProcEvt->ProcID, iAreaIdx);
				continue;
				//return -1;
			}

			pstArea = (Area *)pstMapObj->sArea[pstMapIdx->iAreaOrigin + iAreaIdx];
			if(pstArea->stProc.iTotal >= MAX_AREA_PROC)
			{
				continue;
			}
			
			pstArea->stProc.aiIndex[pstArea->stProc.iTotal] = i;
			pstArea->stProc.iTotal++;	
		}
	}

	return 0;
}

MapIndex* z_mappos_index(MapObj *pstMap, int *piMapPos, int iMapIndex)
{
	MapIndex *pstMapIdx, stIdx;
	int iEqu,iPos;

	iMapIndex	=	GET_MAPID(iMapIndex);

	if (piMapPos && *piMapPos >= 0 && *piMapPos < pstMap->iMapIndex) {
		pstMapIdx = (MapIndex *)&pstMap->sMapIdx[*piMapPos];
		if ((int)pstMapIdx->stMapDef.MapID == iMapIndex)
		{
			return pstMapIdx;
		}
	}
	stIdx.stMapDef.MapID= iMapIndex;
	iPos = bsearch_int(&stIdx, pstMap->sMapIdx, pstMap->iMapIndex, MAP_INDEX_DATA, &iEqu);
	if (!iEqu) return NULL;

	if (piMapPos)
	{
		*piMapPos = iPos;
	}
	
	return (MapIndex *)&pstMap->sMapIdx[iPos];
}

ZONEPWORLDINST* z_find_pworld_inst(MapObj* pstMap, int iPworldInstID)
{
	int iIndex;
	ZONEPWORLDINST* pstPworldInst;
	TMEMBLOCK *pstMemBlock;

	iIndex	=	iPworldInstID & 0xffff;

	if( iIndex > MAX_PWORLD_INST || iIndex < 1 )
		return NULL;

	pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(((TMEMPOOL *)pstMap->sPworldInst), iIndex-1);
	if (0 == pstMemBlock->fValid)
	{
		return NULL;
	}
	
	pstPworldInst	=	(ZONEPWORLDINST*)pstMemBlock->szData;

	if( !pstPworldInst->iPworldInstID )
		return NULL;

	if( pstPworldInst->iPworldInstID == iPworldInstID )
		return pstPworldInst;
	else
		return NULL;
}

MONDYNATTRDEF *z_find_mondyn(ZONESVRENV* pstEnv, unsigned int uiLevel)
{
	return (MONDYNATTRDEF *)z_find_def(pstEnv->pstZoneObj->sMonDynAttrDef, uiLevel, 
										MON_DYN_ATTR_DATA, pstEnv->pstZoneObj->iMonDynAttrDef, NULL);
}

Monster* z_new_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef)
{
	
	Monster *pstMonster;
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	UNUSED(pstAppCtx);
		
	if (!z_valid_pos(pstEnv->pstMapObj, pstMon->iInitMap, &pstMon->iInitMapPos, &pstMon->stInitPos)) 
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d in %d map pos(%d,%d) error", 
					pstMon->iDefIdx, pstMon->iInitMap, pstMon->stInitPos.X, pstMon->stInitPos.Y);
		return NULL;
	}

	if (0 > z_mon_def_data(pstEnv, pstMon, pstMonDef))
	{
		return NULL;
	}

	iIndex = tmempool_alloc(pstEnv->pstMonsterPool);
	if (0 > iIndex)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_alloc monster %d in %d map error",
					pstMon->iDefIdx, pstMon->iInitMap);
		return NULL;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
	pstMonster = (Monster *)pstMemBlock->szData;
	memcpy(pstMonster, pstMon, sizeof(*pstMonster));

	pstMonster->iID         = 0;
	pstMonster->cType       = OBJ_MONSTER;
	pstMonster->iMIndex     = iIndex;
	pstMonster->iLinkedPrev = -1;
	pstMonster->iLinkedNext = -1;

	/*
	if(pstMonster->bSubType == MON_SUB_TYPE_JIN || pstMonster->bSubType == MON_SUB_TYPE_BOSS)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	"monster free: DefID %d, Map %d",
			pstMonster->iDefIdx, pstMonster->stMap.iID);
	}
	*/
	
	return pstMonster;
}

ZONEMAPINST*  z_find_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEIDX *pstIdx)
{
	
	ZONEMAPINST *pstFind;
	ZONEIDX *pstInstIdx;
	TMEMBLOCK *pstMemBlock;
	int iEqu;
	int iIndex;
	MapObj *pstMap = pstEnv->pstMapObj;

	if( pstIdx->iPos >= 0 && pstIdx->iPos < MAX_MAP_INST)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstMapInstPool, pstIdx->iPos);
		if (pstMemBlock->fValid)
		{
			pstFind = (ZONEMAPINST *)pstMemBlock->szData;
			if (pstIdx->iID == pstFind->iMapInstID)
			{
				return pstFind;
			}
		}
	}

	iIndex = bsearch_int(pstIdx, pstMap->astMapInstIdx, pstMap->iMapInst, sizeof(pstMap->astMapInstIdx[0]), &iEqu);
	if (!iEqu)
	{
		return NULL;
	}

	pstInstIdx = &pstMap->astMapInstIdx[iIndex];

	if( pstInstIdx->iPos < 0 || pstInstIdx->iPos >= MAX_MAP_INST )
	{
		return NULL;
	}

	pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstMapInstPool, pstInstIdx->iPos);
	if (!pstMemBlock->fValid)
	{
		return NULL;
	}
	pstFind	=	(ZONEMAPINST *)pstMemBlock->szData;

	if( pstFind->iMapInstID == pstInstIdx->iID )
	{
		pstIdx->iPos = pstInstIdx->iPos;
		return pstFind;
	}
	else
	{
		return NULL;
	}

	UNUSED(pstAppCtx);
}

void dyn_msk_LineInGrid(const RESPOS *pt1, const RESPOS *pt2, GridPoint_t *pstGP)
{
	int dx = pstGP->s_dPt.X;
	int y1 = pt1->Y * dx;
	int y2 = pt2->Y * dx;
	
	if( dx > 0 )
	{
		int y = pstGP->s_dPt.Y*(pt2->X - pstGP->s_StartPt.X) + pstGP->s_StartPt.Y*dx;
		if( y == y2 )
		{
			pstGP->s_CurGrid.X++;
			pstGP->s_CurGrid.Y++;
			return;
		}
		if( y>=y1 && y<y2 )
		{
			pstGP->s_CurGrid.X++;
			return;
		}
	}
	else if( dx < 0 )
	{
		int y = pstGP->s_dPt.Y*(pt1->X - pstGP->s_StartPt.X) + pstGP->s_StartPt.Y*dx;
		if( y == y2 ) {
			pstGP->s_CurGrid.Y++;
			return;
		}
		if( y>y2 && y<=y1 )
		{
			pstGP->s_CurGrid.X--;
			return;
		}
	}

	if( pstGP->s_dPt.Y > 0 ) pstGP->s_CurGrid.Y++;
	else pstGP->s_CurGrid.Y--;
}

int is_dynmsk_set(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, TMAPMASK *pstMask,
					RESPOS *pstPos, int iMskBit)
{
	AREADYN* pstArea;
	char *pstDynMskMem;
	int X; //相对坐标
	int Y;
	int i,j;

	pstArea	=	z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos);
	if (NULL == pstArea)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "dyn fail:map %d x %d y %d", 
					pstMapInst->iMapInstID, pstPos->X, pstPos->Y);
		return 1;
	}

	if (pstArea->iDynMskIdx < 0)
	{
		return 0;
	}
	else
	{
		pstDynMskMem = tmempool_get(pstEnv->pstDynMskPool, pstArea->iDynMskIdx);
		if (NULL == pstDynMskMem)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "dyn msk mempool get fail");
			return -1;
		}
	}

	X = pstPos->X%AREA_XGRID;
	Y = pstPos->Y%AREA_YGRID;

	i = X/pstMask->m_lMaskPixelWidth;
	j = Y/pstMask->m_lMaskPixelHeight;

	return IS_DYN_MSK_BIT_SET((AREA_XGRID/pstMask->m_lMaskPixelWidth)*j+i, pstDynMskMem, iMskBit);
}


int dyn_msk_check(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, TMAPMASK* pstMask, int lPixelX1, int lPixelY1, 
						int lPixelX2, int lPixelY2, int iMiscType,int iBit)
{
	int maxx, maxy;
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	RESPOS pt1, pt2; 
	GridPoint_t stGP;
	MapIndex *pstMapIdx;
	RESPOS stTmp;

	if ( NULL == pstMapInst || 
		 (iMiscType&MON_MISC_TYPE_IGNORE_DYNMSK) )
	{
		return 1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return 1;
	}

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

	if( lPixelX1<0 || lPixelX1>=maxx ||
	    lPixelY1<0 || lPixelY1>=maxy ||
	    lPixelX2<0 || lPixelX2>=maxx ||
	    lPixelY2<0 || lPixelY2>=maxy )
	{
		return 0;
	}

	stGP.s_StartPt.X = lPixelX1;
	stGP.s_StartPt.Y = lPixelY1;
	stGP.s_dPt.X = lPixelX2 - lPixelX1;
	stGP.s_dPt.Y = lPixelY2 - lPixelY1;

	grid_sx = lPixelX1/pstMask->m_lMaskPixelWidth;
	grid_sy = lPixelY1/pstMask->m_lMaskPixelHeight;
	grid_ex = lPixelX2/pstMask->m_lMaskPixelWidth;
	grid_ey = lPixelY2/pstMask->m_lMaskPixelHeight;

	stGP.s_CurGrid.X = grid_sx;
	stGP.s_CurGrid.Y = grid_sy;

	if (stGP.s_CurGrid.X == grid_ex && stGP.s_CurGrid.Y == grid_ey)
	{
		stTmp.X = stGP.s_CurGrid.X * pstMask->m_lMaskPixelWidth;
		stTmp.Y = stGP.s_CurGrid.Y * pstMask->m_lMaskPixelHeight;
		if (is_dynmsk_set(pstEnv, pstMapIdx, pstMapInst, pstMask, &stTmp, iBit))
		{
			return 0;
		}

		return 1;
	}

	while( stGP.s_CurGrid.X!=grid_ex || stGP.s_CurGrid.Y!=grid_ey )
	{
		pt1.X = stGP.s_CurGrid.X * pstMask->m_lMaskPixelWidth;
		pt1.Y = stGP.s_CurGrid.Y * pstMask->m_lMaskPixelHeight;
		pt2.X = pt1.X + pstMask->m_lMaskPixelWidth;
		pt2.Y = pt1.Y + pstMask->m_lMaskPixelHeight;

		dyn_msk_LineInGrid(&pt1, &pt2, &stGP);

		stTmp.X = stGP.s_CurGrid.X * pstMask->m_lMaskPixelWidth;
		stTmp.Y = stGP.s_CurGrid.Y * pstMask->m_lMaskPixelHeight;
		if (is_dynmsk_set(pstEnv, pstMapIdx, pstMapInst, pstMask, &stTmp, iBit))
		{
			return 0;
		}
	}
	
	return 1;
}

int z_mon_dyn_mask_set_check(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, Monster* pstMon, MONSTERDEF *pstMonDef)
{
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstMapIdx);	//add by paraunused.pl
	UNUSED(pstMapInst);	//add by paraunused.pl
	UNUSED(pstMon);	//add by paraunused.pl
	UNUSED(pstMonDef);	//add by paraunused.pl
}

int dyn_msk_find_valid_pos(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST *pstMapInst, TMAPMASK* pstMask, 
						RESPOS *pstPos1, RESPOS *pstPos2, RESPOS *pstPos)
{
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	RESPOS pt1, pt2; 
	GridPoint_t stGP;
	RESPOS stTmp;
	
	*pstPos = *pstPos1;

	if (NULL == pstMapInst || NULL == pstMapIdx)
	{
		return 0;
	}

	stGP.s_StartPt.X = pstPos1->X;
	stGP.s_StartPt.Y = pstPos1->Y;
	stGP.s_dPt.X = pstPos2->X - pstPos1->X;
	stGP.s_dPt.Y = pstPos2->Y - pstPos1->Y;

	grid_sx = pstPos1->X/pstMask->m_lMaskPixelWidth;
	grid_sy = pstPos1->Y/pstMask->m_lMaskPixelHeight;
	grid_ex = pstPos2->X/pstMask->m_lMaskPixelWidth;
	grid_ey = pstPos2->Y/pstMask->m_lMaskPixelHeight;

	stGP.s_CurGrid.X = grid_sx;
	stGP.s_CurGrid.Y = grid_sy;

	while( stGP.s_CurGrid.X!=grid_ex || stGP.s_CurGrid.Y!=grid_ey )
	{
		pt1.X = stGP.s_CurGrid.X * pstMask->m_lMaskPixelWidth;
		pt1.Y = stGP.s_CurGrid.Y * pstMask->m_lMaskPixelHeight;
		pt2.X = pt1.X + pstMask->m_lMaskPixelWidth;
		pt2.Y = pt1.Y + pstMask->m_lMaskPixelHeight;

		dyn_msk_LineInGrid(&pt1, &pt2, &stGP);

		stTmp.X = stGP.s_CurGrid.X * pstMask->m_lMaskPixelWidth;
		stTmp.Y = stGP.s_CurGrid.Y * pstMask->m_lMaskPixelHeight;
		if (is_dynmsk_set(pstEnv, pstMapIdx, pstMapInst, pstMask, &stTmp, DYN_MSK_STOP))
		{
			return 0;
		}

		*pstPos = pt1;
	}
	
	return 0;
}

int dyn_msk_advise_point(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_BLOCKMSK_POINT;
	pstActionRes->Data.BlockMskPoint.Point = *pstPos;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int dyn_msk_check_player_path(ZONESVRENV* pstEnv,Monster* pstMon, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, TMAPMASK *pstMask, Player *pstPlayer)
{
	int i, iCurrPoint;
	RESPOS stPos, stPos1;
	int iOldFuturePoint;
		
	if (pstPlayer->stOnline.cMoving)  z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);

	//当前位置检查
	if (!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,
						pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, 0,DYN_MSK_STOP))
	{
		int iMaxStep;
		int maxx,maxy;

		maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	       maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

		iMaxStep = pstMask->m_lWidthMasks > pstMask->m_lHeightMasks ? pstMask->m_lWidthMasks:pstMask->m_lHeightMasks;
		iMaxStep = iMaxStep/MSK_STEP + 1;
		stPos = pstPlayer->stRoleData.Pos;
		for (i=1; i<iMaxStep; i++)
		{
			//四方向尝试
			stPos1.X = pstPlayer->stRoleData.Pos.X + i*pstMask->m_lMaskPixelWidth*MSK_STEP;
			stPos1.Y = pstPlayer->stRoleData.Pos.Y;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, 0,DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}

			stPos1.X = pstPlayer->stRoleData.Pos.X - i*pstMask->m_lMaskPixelWidth*MSK_STEP;
			stPos1.Y = pstPlayer->stRoleData.Pos.Y;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y,0, DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}

			stPos1.X = pstPlayer->stRoleData.Pos.X;
			stPos1.Y = pstPlayer->stRoleData.Pos.Y + i*pstMask->m_lMaskPixelHeight*MSK_STEP;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, 0,DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}

			stPos1.X = pstPlayer->stRoleData.Pos.X;
			stPos1.Y = pstPlayer->stRoleData.Pos.Y -  i*pstMask->m_lMaskPixelHeight*MSK_STEP;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, 0,DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}
		}

		if (i<iMaxStep)
		{
			ZoneAni stAni;
			RESPOS stInter;
			int interNum = 0;
			int    iTargetDynMskMonID =0;
				
			memset(&stAni,0,sizeof(stAni));
			stAni.iType = OBJ_PLAYER;
			stAni.stObj.pstPlayer= pstPlayer;

			if(pstMon->bSubType == MON_SUB_TYPE_TOTEM)
			{
				iTargetDynMskMonID = mon_press_target_dynmsk_pos(pstEnv->pstAppCtx, pstEnv, pstMapInst, 
										&stAni, &pstPlayer->stRoleData.Pos, &stPos, &stInter,&interNum,1);
				if( interNum > 1 || 
					(iTargetDynMskMonID>0 && iTargetDynMskMonID != pstMon->iID) )
				{
					//中间有其他阻挡怪
					stPos = pstPlayer->stRoleData.Pos;
				}
			}
									
			stPos1 = pstPlayer->stRoleData.Pos;
			pstPlayer->stRoleData.Pos = stPos;
			SET_PLAYER_NOMOVE(pstPlayer);
			z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
			z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos1, 0);
		}

		return 0;
	}

	if (0 == pstPlayer->stOnline.cMoving || 0 == z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos))
	{
		return 0;
	}

	stPos = pstPlayer->stRoleData.Pos;
	iOldFuturePoint = pstPlayer->stOnline.cFuturePoints;
	for (i = 0; i < pstPlayer->stOnline.cFuturePoints; i++) 
	{
		iCurrPoint = (pstPlayer->stOnline.cCurrPoint + i) % RECORD_TURN;
		if (!dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos.X, stPos.Y, 
			pstPlayer->stOnline.stTurnPoints[iCurrPoint].X, pstPlayer->stOnline.stTurnPoints[iCurrPoint].Y,
			0,DYN_MSK_STOP))
		{
			dyn_msk_find_valid_pos(pstEnv, pstMapIdx, pstMapInst, pstMask, &stPos, &pstPlayer->stOnline.stTurnPoints[iCurrPoint], &stPos1);

			pstPlayer->stOnline.stTurnPoints[iCurrPoint] = stPos1;
			pstPlayer->stOnline.cFuturePoints = i+1;
			pstPlayer->stOnline.cLastPathPoints -= (iOldFuturePoint - pstPlayer->stOnline.cFuturePoints);

			dyn_msk_advise_point(pstEnv, pstPlayer, &stPos1);
			break;
		}

		stPos = pstPlayer->stOnline.stTurnPoints[iCurrPoint];
	}

	return 0;
}

int dyn_msk_check_mon_path(ZONESVRENV* pstEnv, Monster *pstMaskMon, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, TMAPMASK *pstMask, Monster *pstMon)
{
	int i;
	RESPOS stPos, stPos1;
	ZoneAni stAni;
	RESPOS stInter;
	int interNum = 0;
	int iTargetDynMskMonID =0;
		
	if (pstMon->cMoving)  z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);

	if (!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y,
						pstMon->stCurrPos.X, pstMon->stCurrPos.Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP))
	{
		int iMaxStep;
		int maxx,maxy;

		maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	       maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

		iMaxStep = pstMask->m_lWidthMasks > pstMask->m_lHeightMasks ? pstMask->m_lWidthMasks:pstMask->m_lHeightMasks;
		stPos = pstMon->stCurrPos;
		for (i=1; i<iMaxStep; i++)
		{
			//四方向尝试
			stPos1.X = pstMon->stCurrPos.X + i*pstMask->m_lMaskPixelWidth;
			stPos1.Y = pstMon->stCurrPos.Y;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}

			stPos1.X = pstMon->stCurrPos.X - i*pstMask->m_lMaskPixelWidth;
			stPos1.Y = pstMon->stCurrPos.Y;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}

			stPos1.X = pstMon->stCurrPos.X;
			stPos1.Y = pstMon->stCurrPos.Y + i*pstMask->m_lMaskPixelHeight;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}

			stPos1.X = pstMon->stCurrPos.X;
			stPos1.Y = pstMon->stCurrPos.Y -  i*pstMask->m_lMaskPixelHeight;
			if (stPos1.X>=0 && stPos1.X <maxx && stPos1.Y >=0 && stPos1.Y < maxy && 0 == IsPixelStop(pstMask, stPos1.X, stPos1.Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, stPos1.X, stPos1.Y, stPos1.X, stPos1.Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
				IsPixelLineThrough(pstMask, stPos.X, stPos.Y, stPos1.X, stPos1.Y, DYN_MSK_STOP))
			{
				stPos = stPos1;
				break;
			}
		}

		if (i<iMaxStep)
		{			
			memset(&stAni,0,sizeof(stAni));
			stAni.iType = OBJ_MONSTER;
			stAni.stObj.pstMon= pstMon;

			if(pstMon->bSubType == MON_SUB_TYPE_TOTEM)
			{
				iTargetDynMskMonID = mon_press_target_dynmsk_pos(pstEnv->pstAppCtx, pstEnv, pstMapInst, 
							&stAni, &pstMon->stCurrPos, &stPos, &stInter,&interNum,1);
				if( interNum > 1 || 
					(iTargetDynMskMonID>0 && iTargetDynMskMonID != pstMon->iID)  )
				{
					//中间有其他阻挡怪
					stPos = pstMon->stCurrPos;
				}
			}
			stPos1 = pstMon->stCurrPos;
			pstMon->stCurrPos = stPos;
			pstMon->cMoving = 0;
			z_mon_stop(pstEnv, pstMon);
			z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMon->iID, &stPos1, 0);
		}
		
		return 0;
	}

	if (0 == pstMon->cMoving || 0 == z_pos_in_view(&pstMaskMon->stCurrPos, &pstMon->stCurrPos))
	{
		return 0;
	}

	interNum = 0;
	memset(&stInter,0,sizeof(stInter));
	memset(&stAni,0,sizeof(stAni));
	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon= pstMon;

	iTargetDynMskMonID = mon_press_target_dynmsk_pos(pstEnv->pstAppCtx, pstEnv, pstMapInst, 
						&stAni, &pstMon->stCurrPos, &pstMon->stNextPos, &stInter,&interNum,0);
	if( iTargetDynMskMonID > 0 )
	{
		pstMon->stNextPos = stInter;
		z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_WALK);

		pstMon->stAtkTargets.iTargetCur = iTargetDynMskMonID;
		mon_press_add_by_memid(pstEnv, pstMon, pstMon->stAtkTargets.iTargetCur,10000000 );
	}
	
	return 0;
}

int dyn_msk_check_map_path(ZONESVRENV* pstEnv,Monster* pstMskMon, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, TMAPMASK *pstMask)
{
	int i, k, iDyn;
	int iID, iIndex;
	AREADYN *pstAreaDyn;
	Player *pstPlayer;
	Monster *pstMon;
	char cType;
	TMEMBLOCK *pstMemBlock;

	for (i=pstMapInst->iAreaDyn-1; i>=0; i--)
	{
		iDyn	=	pstMapInst->aiAreaDyn[i];

		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);
			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstPlayer	=	(Player *)pstMemBlock->szData;

					if (	player_is_check_dyn_mask(pstPlayer))
					{
						dyn_msk_check_player_path(pstEnv,pstMskMon, pstMapInst, pstMapIdx, pstMask, pstPlayer);
					}
					break;
				case OBJ_MONSTER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstMon	=	(Monster *)pstMemBlock->szData;
					if (0 == (pstMon->cMonMiscFlag & MON_MISC_FLAG_NO_DYNBAK))
					{
						dyn_msk_check_mon_path(pstEnv, pstMskMon, pstMapInst, pstMapIdx, pstMask, pstMon);
					}
					break;
				default:
					break;
			}
		}
	}

	return 0;
}

int dynmsk_set(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, TMAPMASK *pstMask,
					RESPOS *pstPos, int iMskBit, int iFlag)
{
	AREADYN* pstArea;
	char *pstDynMskMem;
	int X; //相对坐标
	int Y;
	int i,j;

	pstArea	=	z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos);
	if (NULL == pstArea)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "dyn fail:map %d x %d y %d", 
					pstMapInst->iMapInstID, pstPos->X, pstPos->Y);
		return -1;
	}

	if (pstArea->iDynMskIdx < 0)
	{
		pstArea->iDynMskIdx = tmempool_alloc(pstEnv->pstDynMskPool);
		if (0 > pstArea->iDynMskIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "dyn msk mempool alloc fail");
			return -1;
		}
		pstDynMskMem = tmempool_get(pstEnv->pstDynMskPool, pstArea->iDynMskIdx);
		if (NULL == pstDynMskMem)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "dyn msk mempool alloc fail");
			return -1;
		}

		memset(pstDynMskMem, 0, DYN_MSK_DATA);
	}
	else
	{
		pstDynMskMem = tmempool_get(pstEnv->pstDynMskPool, pstArea->iDynMskIdx);
		if (NULL == pstDynMskMem)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "dyn msk mempool get fail");
			return -1;
		}
	}

	X = pstPos->X%AREA_XGRID;
	Y = pstPos->Y%AREA_YGRID;

	i = X/pstMask->m_lMaskPixelWidth;
	j = Y/pstMask->m_lMaskPixelHeight;

	if (iFlag)
	{
		SET_DYN_MSK_BIT((AREA_XGRID/pstMask->m_lMaskPixelWidth)*j+i, pstDynMskMem, iMskBit);
	}
	else
	{
		CLEAR_DYN_MSK_BIT((AREA_XGRID/pstMask->m_lMaskPixelWidth)*j+i, pstDynMskMem, iMskBit);
	}
	
	return 0;
}

int z_mon_dyn_mask_set(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, Monster* pstMon, MONSTERDEF *pstMonDef, int iFlag)
{
	TMAPMASK *pstMask;
	int iStartX, iStartY, iEndX, iEndY;
	int i,j;
	RESPOS polygon[4];
	
	memset(polygon,0,sizeof(RESPOS)*4);
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	
	//第一个顶点
	if(pstMonDef->DynMaskX > 0 )
	{
		polygon[0].X = pstMon->stCurrPos.X - (int)pstMonDef->DynMaskX;
	}
	else
	{
		polygon[0].X = pstMon->stCurrPos.X - (int)pstMonDef->Radius;
	}
	if (0 > polygon[0].X)
	{
		polygon[0].X = 0;
	}
	if (polygon[0].X > pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks )
	{
		polygon[0].X = pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks;
	}
	
	if(pstMonDef->DynMaskY > 0 )
	{
		polygon[0].Y = pstMon->stCurrPos.Y - (int)pstMonDef->DynMaskY;
	}
	else
	{
		polygon[0].Y= pstMon->stCurrPos.Y - (int)pstMonDef->Radius;
	}
	if (0 > polygon[0].Y )
	{
		polygon[0].Y = 0;
	}
	if (polygon[0].Y > pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks)
	{
		polygon[0].Y = pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks;
	}

	//第二个顶点
	if(pstMonDef->DynMaskX > 0 )
	{
		polygon[1].X = pstMon->stCurrPos.X + (int)pstMonDef->DynMaskX;
	}
	else
	{
		polygon[1].X = pstMon->stCurrPos.X + (int)pstMonDef->Radius;
	}
	if (0 > polygon[1].X)
	{
		polygon[1].X = 0;
	}
	if (polygon[1].X > pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks )
	{
		polygon[1].X = pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks;
	}
	
	if(pstMonDef->DynMaskY > 0 )
	{
		polygon[1].Y = pstMon->stCurrPos.Y - (int)pstMonDef->DynMaskY;
	}
	else
	{
		polygon[1].Y= pstMon->stCurrPos.Y - (int)pstMonDef->Radius;
	}
	if (0 > polygon[1].Y )
	{
		polygon[1].Y = 0;
	}
	if (polygon[1].Y > pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks)
	{
		polygon[1].Y = pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks;
	}
	
	//第三个顶点
	if(pstMonDef->DynMaskX > 0 )
	{
		polygon[2].X = pstMon->stCurrPos.X + (int)pstMonDef->DynMaskX;
	}
	else
	{
		polygon[2].X = pstMon->stCurrPos.X + (int)pstMonDef->Radius;
	}
	if (0 > polygon[2].X)
	{
		polygon[2].X = 0;
	}
	if (polygon[2].X > pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks )
	{
		polygon[2].X = pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks;
	}
	
	if(pstMonDef->DynMaskY > 0 )
	{
		polygon[2].Y = pstMon->stCurrPos.Y + (int)pstMonDef->DynMaskY;
	}
	else
	{
		polygon[2].Y= pstMon->stCurrPos.Y + (int)pstMonDef->Radius;
	}
	if (0 > polygon[2].Y )
	{
		polygon[2].Y = 0;
	}
	if (polygon[2].Y > pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks)
	{
		polygon[2].Y = pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks;
	}
	//第四个顶点
	if(pstMonDef->DynMaskX > 0 )
	{
		polygon[3].X = pstMon->stCurrPos.X - (int)pstMonDef->DynMaskX;
	}
	else
	{
		polygon[3].X = pstMon->stCurrPos.X - (int)pstMonDef->Radius;
	}
	if (0 > polygon[3].X)
	{
		polygon[3].X = 0;
	}
	if (polygon[3].X > pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks )
	{
		polygon[3].X = pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks;
	}
	
	if(pstMonDef->DynMaskY > 0 )
	{
		polygon[3].Y = pstMon->stCurrPos.Y + (int)pstMonDef->DynMaskY;
	}
	else
	{
		polygon[3].Y= pstMon->stCurrPos.Y + (int)pstMonDef->Radius;
	}
	if (0 > polygon[3].Y )
	{
		polygon[3].Y = 0;
	}
	if (polygon[3].Y > pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks)
	{
		polygon[3].Y = pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks;
	}

	//把四个点旋转下
	for(i=0;i<4;i++)
	{
		RESPOS ret_p;
		int iAngle;
		RESPOS p;
		
		iAngle = pstMon->bDirection;
		iAngle = -iAngle*2;
		p.X = polygon[i].X;
		p.Y = polygon[i].Y;
		
		ret_p = z_rotate_pos(pstMon->stCurrPos,iAngle,p);
		polygon[i].X = ret_p.X;
		if(polygon[i].X < 0)
		{
			polygon[i].X = 0;
		}
		if (polygon[i].X > pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks)
		{
			polygon[i].X = pstMask->m_lMaskPixelWidth*pstMask->m_lWidthMasks;
		}
		polygon[i].Y = ret_p.Y;
		if(polygon[i].Y < 0)
		{
			polygon[i].Y = 0;
		}
		if (polygon[i].Y > pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks)
		{
			polygon[i].Y = pstMask->m_lMaskPixelHeight*pstMask->m_lHeightMasks;
		}
	}

	//把大矩形取出来
	iStartX = polygon[0].X;
	iStartY = polygon[0].Y;
	iEndX = polygon[0].X;
	iEndY = polygon[0].Y;
	for(i=0;i<4;i++)
	{
		if(polygon[i].X < iStartX)
		{
			iStartX = polygon[i].X;
		}
	
		if(polygon[i].Y < iStartY)
		{
			iStartY = polygon[i].Y;
		}
		if(polygon[i].X > iEndX)
		{
			iEndX = polygon[i].X;
		}
		if(polygon[i].Y > iEndY)
		{
			iEndY = polygon[i].Y;
		}
		
	}
	iStartX /= pstMask->m_lMaskPixelWidth;
	iStartY /= pstMask->m_lMaskPixelHeight;
	iEndX 	/= pstMask->m_lMaskPixelWidth;
	iEndY 	/= pstMask->m_lMaskPixelHeight;

	//把四方型缩小个像素位
	/*for(i=0;i<4;i++)
	{
		polygon[i].X /= pstMask->m_lMaskPixelWidth;
		polygon[i].Y /= pstMask->m_lMaskPixelHeight;
	}*/
	
	
	//看遮挡点是否在这个区域里
	for (i=iStartX; i<=iEndX; i++)
	{
		for (j=iStartY; j<=iEndY; j++)
		{
			RESPOS q;
			q.X = i * pstMask->m_lMaskPixelWidth;
			q.Y = j * pstMask->m_lMaskPixelHeight;
			if(!InsideConvexPolygon(4, polygon, q))
			{
				continue;
			}
		
			dynmsk_set(pstEnv, pstMapIdx, pstMapInst, pstMask, &q, pstMonDef->DynMask, iFlag);
		}
	}

	if (iFlag == 1)
	{
		dyn_msk_check_map_path(pstEnv,pstMon, pstMapInst, pstMapIdx, pstMask);
	}
	
	return 0;
}

int z_refresh_area_dynmsk(ZONESVRENV* pstEnv, MapIndex 	*pstMapIdx, ZONEMAPINST* pstMapInst, Monster *pstMon)
{
	SearchArea 	stSArea_curr;
	AREADYN 	*pstArea = NULL;
	int  		i = 0;
	int  		j = 0;
	Monster 	*pstMonArea = NULL;
	MONSTERDEF  *pstMonDef;
	
	memset(&stSArea_curr,0,sizeof(stSArea_curr));

	if ( !(pstMon->cMonMiscFlag & MON_MISC_FLAG_DYN) )
	{
		return -1;
	}

	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapInst->iMapInstID != pstMon->iInitMap)
	{
		return -1;
	}

	if (pstMapInst->iDynMskMonCount < 32)
	{
		for (i=0; i<pstMapInst->iDynMskMonCount ; i++)
		{
			pstMonArea = z_id_monster2(pstEnv->pstAppCtx, pstEnv,  pstMapInst->aiDynMskMonMemID[i]);
			if (NULL == pstMonArea)	
			{
				continue;
			}
			
		       if ( !(pstMonArea->cMonMiscFlag & MON_MISC_FLAG_DYN) ||
				pstMon == pstMonArea ||
				pstMon->iInitMap != pstMonArea->iInitMap ||
				0 == z_pos_in_view(&pstMon->stCurrPos, &pstMonArea->stCurrPos))
			{
				continue;
			}
			
			pstMonDef = z_get_mon_def(pstEnv,  pstMonArea);
			if(!pstMonDef)
			{
				continue;
			}
			
			z_mon_dyn_mask_set(pstEnv, pstMapIdx, pstMapInst, pstMonArea, pstMonDef, 2);
		
		}
		
		return 0;
	}
	
	if (0 > z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, &stSArea_curr))
	{
		return -1;
	}
	
	for (i = 0; i < stSArea_curr.iNum; i++) 
	{
		pstArea = stSArea_curr.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
		{
			pstMonArea = z_id_monster2(pstEnv->pstAppCtx, pstEnv,  pstArea->aiAnimate[j]);
			if (NULL == pstMonArea)	
			{
				continue;
			}
			
		       if ( !(pstMonArea->cMonMiscFlag & MON_MISC_FLAG_DYN) ||
				pstMon == pstMonArea ||
				pstMon->iInitMap != pstMonArea->iInitMap ||
				0 == z_pos_in_view(&pstMon->stCurrPos, &pstMonArea->stCurrPos))
			{
				continue;
			}
			
			pstMonDef = z_get_mon_def(pstEnv,  pstMonArea);
			if(!pstMonDef)
			{
				continue;
			}
			
			z_mon_dyn_mask_set(pstEnv, pstMapIdx, pstMapInst, pstMonArea, pstMonDef, 2);
		
		}	
       }
	
	return 0;
}

int z_monster_entermap(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, int iMapIndex, RESPOS* pstPos)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	RESPOS stSearchPos;
   	int iRet;
	ZONEPWORLDINST *pstPworldInst;
	MONSTERDEF *pstMonDef;
	
	stIdx.iID = iMapIndex;
	stIdx.iPos=	pstMon->stMap.iPos;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}

	if( !(pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx)) )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst error map=%d", iMapIndex);
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstPworldInst && pstPworldInst->bOver && 
		!(pstMon->cMonMiscFlag & MON_MISC_FLAG_IGNORE_PWORLD_OVER))
	{
		//不在打印错误日志
		return -4;
	}

	pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_mappos_index error map=%d", pstMapInst->iMapInstID);
		return -1;
	}

	pstMon->stMap.iID	=	stIdx.iID;
	pstMon->stMap.iPos	=	stIdx.iPos;

	memcpy(&pstMon->stCurrPos, pstPos, sizeof(pstMon->stCurrPos));
	memcpy(&pstMon->stNextPos, pstPos, sizeof(pstMon->stNextPos));   
	pstMon->cMoving = 0;

	if (pstMonDef->DynMask && pstMonDef->MoveType == MOVE_TYPE_NO && 0 > z_mon_dyn_mask_set_check(pstEnv, pstMapIdx, pstMapInst, pstMon, pstMonDef))
	{
		return -1;
	}

	if ((iRet = z_ani_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, pstMon->iID, &pstMon->stCurrPos, pstMapInst)) < 0) 
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d %d aistat %d z_ani_enter_area ret %d, mon x= %d y=%d", pstMon->iID, pstMon->iDefIdx, pstMon->bAiStat, iRet, pstMon->stCurrPos.X, pstMon->stCurrPos.Y);
		return iRet;
	}

	if (z_pos_search_pos(pstMapIdx, &pstMon->stCurrPos, &stSearchPos)) {
		z_mon_search_view(pstAppCtx, pstEnv,  pstMapInst, &stSearchPos, pstMon);
	}

	if ((pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD) || pstMapInst->bBroadcast)
	{
		CSMONSTER *pstCSMon;
		CSPKG stPkg;
		CSPKG *pstPkg = &stPkg;
		CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;

		pstEnterView->ID = pstMon->iID;
		pstEnterView->x = pstMon->stCurrPos.X;
		pstEnterView->y = pstMon->stCurrPos.Y;
		pstEnterView->Dir = pstMon->bDirection;
		pstEnterView->Type = pstMon->cType;

		if (pstMon->cType == OBJ_MONSTER)
		{
			pstCSMon = &pstEnterView->Object.Monster;
			z_make_mon_view(pstEnv->pstAppCtx, pstEnv, pstCSMon, pstMon);
		}
		else
		{
			pstCSMon = &pstEnterView->Object.Monster;
			z_make_mon_view(pstEnv->pstAppCtx, pstEnv, pstCSMon, pstMon);
		}
		
		Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);

		if (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD)
		{
			z_sendpkg_mon_area_view(pstAppCtx, pstEnv, pstMon, &stPkg);
		}
		else if (pstPworldInst)
		{
			pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, pstPkg);
		}
	}
	
	if (pstMon->cMonMiscFlag & MON_MISC_FLAG_DYN)
	{
		map_dynmsk_mon_memid_add(pstEnv, pstMapInst, pstMon->iID);
		z_mon_dyn_mask_set(pstEnv, pstMapIdx, pstMapInst, pstMon, pstMonDef, 1);
	}

	if (pstMon->cType == OBJ_MONSTER && 
		MON_SUB_TYPE_TMP > pstMonDef->SubType &&
		pstMonDef->SubType != MON_SUB_TYPE_TOTEM)
	{
		pstMapInst->iMonsterCount++;
		
		/*tlog_debug(pstEnv->pstLogCat, 0, 0, "mon %d %d refresh, map %d mon num %d", pstMon->iID, pstMonDef->MonsterID,
						pstMapInst->iMapInstID, pstMapInst->iMonsterCount);*/
	}

	if(is_mon_big_oil(pstMon->iDefIdx) ||
		is_mon_big_ore(pstMon->iDefIdx))
	{
		global_world_ore_add(pstEnv, pstMon);
	}

	unreal_up_boss_info(pstEnv, pstMon, 1);

   /*
	tlog_debug(pstEnv->pstLogCat, 0, 0, "monster id %d index %d enter map %d map inst %d currpos(%d, %d), initpos(%d,%d)", pstMon->iID, pstMon->iDefIdx, iMapIndex, 
				pstMapInst->iMapInstID, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstMon->stInitPos.X, pstMon->stInitPos.Y);*/

	return 0;
}

int z_safe_monster_entermap(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, int iMap, RESPOS* pstPos)
{
	int iRet;

	iRet	=	z_monster_entermap(pstAppCtx, pstEnv, pstMon, iMap, pstPos);

	switch(iRet)
	{
		case 0:
			break;
		case -1:
		case -2:
			tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d %d aistat %d enter map  %d error %d", 
							pstMon->iID, pstMon->iDefIdx, pstMon->bAiStat, iMap, iRet);
			z_free_monster(pstAppCtx, pstEnv, pstMon);
			break;
		case -3:
			z_id_clean(pstEnv, pstMon->iID);
			pstMon->iID	=	-3;

			pstMon->cStat = MONSTER_DIE;
			pstMon->tRefresh += 30;
			break;
		default:
			z_free_monster(pstAppCtx, pstEnv, pstMon);
			break;
	}

	return iRet;
}

int z_in_new_pworld_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, MapIndex* pstMapIndex,
									ZONEMAPINST* pstMapInst, MAPMONSTER *pstMapMonster)
{		
	Monster stMonster;
	Monster* pstFind;
	int iRet = 0;
	MONSTERDEF *pstMonDef;
	UNUSED(pstMapIndex);

	memset(&stMonster, 0, sizeof(stMonster));
		
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = pstMapMonster->MonsterID;
	stMonster.iDefPos = -1;
	stMonster.bDirection=	pstMapMonster->Dir;
	
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;
	
	memcpy(&stMonster.stInitPos, &pstMapMonster->Pos, sizeof(stMonster.stInitPos));
	memcpy(&stMonster.stCurrPos, &pstMapMonster->Pos, sizeof(stMonster.stCurrPos));

	stMonster.tRefresh	=	pstAppCtx->stCurr.tv_sec;
	
	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (!pstMonDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "map monster %d no def", stMonster.iDefIdx);
		return -1;
	}

	pstFind =	z_new_monster(pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstFind )
		return -1;

	if (pstMonDef->NotRefresh)
	{
		pstFind->cRefresh = 0;
	}
	else
	{
		pstFind->cRefresh = 1;
	}

	//巡逻路径
	pstFind->iPatrolID = pstMapMonster->PatrolID;

	mon_set_camp(pstFind, pstMapMonster->Camp);

	pstFind->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return -1;
	}
	pstFind->cFirstBorn = 0;

	if (pstMapMonster->PatrolID > 0)
	{
		pstFind->bMoveType = MOVE_TYPE_PATROL;
	}
	
	
	return iRet;
}

Npc* z_new_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Npc* pstNpc)
{
	Npc *pstNpcTmp;
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	MONSTERDEF *pstMonDef;
	UNUSED(pstAppCtx);
	
	iIndex = tmempool_alloc(pstEnv->pstNpcPool);
	if (0 > iIndex)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_alloc npc %d in %d map error",
					pstNpc->stMon.iDefIdx, pstNpc->stMon.iInitMap);
		return NULL;
	}

	pstMonDef = z_get_mon_def(pstEnv, &pstNpc->stMon);
	if (NULL == pstMonDef)
	{
		return NULL;
	}

	if (0 > z_mon_def_data(pstEnv, &pstNpc->stMon, pstMonDef))
	{
		return NULL;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
	pstNpcTmp = (Npc *)pstMemBlock->szData;
	memcpy(pstNpcTmp, pstNpc, sizeof(*pstNpcTmp));

	pstNpcTmp->stMon.iID		=	0;
	pstNpcTmp->stMon.iMIndex = iIndex;
	
	return pstNpcTmp;
}

char *trim(char *str)
{
      char *ibuf, *obuf;

      if (str)
      {
            for (ibuf = obuf = str; *ibuf; )
            {
                  while (*ibuf && (isspace (*ibuf)))
                        ibuf++;
                  if (*ibuf && (obuf != str))
                        *(obuf++) = ' ';
                  while (*ibuf && (!isspace (*ibuf)))
                        *(obuf++) = *(ibuf++);
            }
            *obuf = 0;
      }
      return (str);
}

int z_in_new_pworld_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, MapIndex* pstMapIndex,
								ZONEMAPINST* pstMapInst, MAPNPC *pstMapNpc,int iCampIdx)
{
	Npc stNpc;
	Npc* pstFind;
	//int iRet;
	MONSTERDEF *pstMonDef;
	ScriptIndex *pstScriptInd;
	int iCamp = 0;
	//ZONEPWORLDINST *pstPworldInst;
	memset(&stNpc, 0, sizeof(stNpc));

	stNpc.stMon.iDefIdx = pstMapNpc->MonsterID;
	stNpc.stMon.stMap.iID = stNpc.stMon.iInitMap = pstMapInst->iMapInstID;
	stNpc.stMon.iInitMapPos = pstMapInst->iMapPos;
	stNpc.stMon.stMap.iPos = pstMapInst->iIndex;
	stNpc.stMon.cType = OBJ_NPC;
	stNpc.stMon.cStat = MONSTER_DIE;
	stNpc.stMon.bDirection = pstMapNpc->Dir;
	stNpc.stMon.tRefresh = pstAppCtx->stCurr.tv_sec;
	memcpy(&stNpc.stMon.stInitPos, &pstMapNpc->Pos, sizeof(stNpc.stMon.stInitPos));
	memcpy(&stNpc.stMon.stCurrPos, &pstMapNpc->Pos, sizeof(stNpc.stMon.stCurrPos));
	stNpc.stMon.cRefresh = 0;

	iCamp = iCampIdx;
	if(pstMapNpc->SpanMapCamp > 0)
	{
		iCamp = pstMapNpc->SpanMapCamp;
	}
	mon_set_camp(&stNpc.stMon, iCamp);
	//mon_set_camp(&stNpc.stMon, iCampIdx);

	//pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstMapNpc->PkMode == 2)
	{
		mon_set_pkinfo(&stNpc.stMon, MON_PK_TYPE_PANISH, 0);
	}
	/*else if (pstMapNpc->PkMode == 1 && pstPworldInst && pstPworldInst->ullClanGID>0)
	{
		mon_set_pkinfo(&stNpc.stMon, MON_PK_TYPE_CLAN, pstPworldInst->ullClanGID);
	}*/

	STRNCPY(stNpc.szScriptName, pstMapNpc->Script, sizeof(stNpc.szScriptName));
	trim(stNpc.szScriptName);

	if (stNpc.szScriptName[0] != 0)
	{
		pstScriptInd = get_script_ind(stNpc.szScriptName);
		if (!pstScriptInd)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "npc %d script %s cant be available", pstMapNpc->MonsterID, pstMapNpc->Script);
			return -1;
		}
		memcpy(&stNpc.stScriptIdx, pstScriptInd, sizeof(stNpc.stScriptIdx));
	}

	pstFind	=	z_new_npc(pstAppCtx, pstEnv, &stNpc);
	if( !pstFind )
		return -1;

	pstMonDef = z_get_mon_def(pstEnv, &pstFind->stMon);
	if (!pstMonDef)
	{
		tmempool_free(pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		tlog_error(pstEnv->pstLogCat, 0, 0, "map npc %d no monster def", pstFind->stMon.iDefIdx);
		return -1;
	}

	if (pstMonDef->NotRefresh)
	{
		pstFind->stMon.cRefresh = 0;
	}
	else
	{
		pstFind->stMon.cRefresh = 1;
	}

	pstFind->stMon.unRadius = pstMonDef->Radius;
	pstFind->stMon.iPatrolID = pstMapNpc->PatrolID;

	pstFind->stMon.cFirstBorn = 1;
	if (0 > z_refresh_mon(pstAppCtx, pstEnv, (Monster *)pstFind,NULL))
	{
		tmempool_free(pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		return -1;
	}
	
	pstFind->stMon.cFirstBorn = 0;

	if(pstFind->stMon.iPatrolID > 0)
	{
		pstFind->stMon.bMoveType = MOVE_TYPE_PATROL;
	}

	pstFind->nTransListID = pstMapNpc->TransListID;
	pstFind->nSellListID = pstMapNpc->SellListID;

	z_check_npc_script(pstEnv, pstFind);
		
	return pstFind->stMon.iID;
	UNUSED(pstMapIndex);	//add by paraunused.pl
}

int z_do_call_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,MapIndex* pstMapIndex,
								ZONEMAPINST* pstMapInst, MAPNPC *pstMapNpc, int iLifeTime)
{	
	Npc stNpc;
	Npc* pstFind;
	Npc* pstNpc;
	int iRet = 0;
	MONSTERDEF *pstMonDef;
	ScriptIndex *pstScriptInd;
	UNUSED(pstMapIndex);


	if (pstPlayer->stOnline.iCallNpcMemID)
	{
		pstNpc = z_id_npc(pstAppCtx, pstEnv, pstPlayer->stOnline.iCallNpcMemID);
		if (pstNpc)
		{
			z_monster_leave_map(pstAppCtx, pstEnv, &pstNpc->stMon);
			z_free_monster(pstAppCtx, pstEnv, &pstNpc->stMon);
		}
	}

	memset(&stNpc, 0, sizeof(stNpc));

	stNpc.stMon.iDefIdx = pstMapNpc->MonsterID;
	stNpc.stMon.stMap.iID = stNpc.stMon.iInitMap = pstMapInst->iMapInstID;
	stNpc.stMon.iInitMapPos = pstMapInst->iMapPos;
	stNpc.stMon.stMap.iPos = pstMapInst->iIndex;
	stNpc.stMon.cType = OBJ_NPC;
	stNpc.stMon.cStat = MONSTER_DIE;
	stNpc.stMon.bDirection = pstMapNpc->Dir;
	stNpc.stMon.tRefresh = pstAppCtx->stCurr.tv_sec;
	memcpy(&stNpc.stMon.stInitPos, &pstMapNpc->Pos, sizeof(stNpc.stMon.stInitPos));
	memcpy(&stNpc.stMon.stCurrPos, &pstMapNpc->Pos, sizeof(stNpc.stMon.stCurrPos));
	memcpy(stNpc.stMon.szOwner, pstPlayer->stRoleData.RoleName, sizeof(stNpc.stMon.szOwner));
	stNpc.stMon.cRefresh = 1;
	stNpc.stMon.iOwnerID = pstPlayer->iMemID;
		
	STRNCPY(stNpc.szScriptName, pstMapNpc->Script, sizeof(stNpc.szScriptName));
	trim(stNpc.szScriptName);

	if (stNpc.szScriptName[0] != 0)
	{
		pstScriptInd = get_script_ind(stNpc.szScriptName);
		if (!pstScriptInd)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "npc %d script %s cant be available", pstMapNpc->MonsterID, pstMapNpc->Script);
			return -1;
		}
		memcpy(&stNpc.stScriptIdx, pstScriptInd, sizeof(stNpc.stScriptIdx));
	}

	pstFind	=	z_new_npc(pstAppCtx, pstEnv, &stNpc);
	if( !pstFind )
		return -1;

	pstMonDef = z_get_mon_def(pstEnv, &pstFind->stMon);
	if (!pstMonDef)
	{
		tmempool_free(pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		tlog_error(pstEnv->pstLogCat, 0, 0, "map npc %d no monster def", pstFind->stMon.iDefIdx);
		return -1;
	}

	pstFind->stMon.unRadius = pstMonDef->Radius;

	pstFind->stMon.cFirstBorn = 1;
	pstFind->stMon.tLifeEnd = pstAppCtx->stCurr.tv_sec + iLifeTime;
	if (0 > z_refresh_mon(pstAppCtx, pstEnv, (Monster *)pstFind,NULL))
	{
		tmempool_free(pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		return -1;
	}
	pstFind->stMon.cFirstBorn = 0;
	//pstFind->stMon.iOwnerID = pstPlayer->iMemID;

	pstFind->nTransListID = pstMapNpc->TransListID;
	pstFind->nSellListID = pstMapNpc->SellListID;
	
	pstPlayer->stOnline.iCallNpcMemID = pstFind->stMon.iID;
	z_check_npc_script(pstEnv, pstFind);
		
	return iRet;
}

int z_player_call_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iNpcDefID, int iNpcLifeTime)
{
	MAPNPC stMapNpc;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcDefID;
	stMapNpc.Dir = RAND1(180);
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIndex, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 50, &stMapNpc.Pos);

	//STRNCPY(stMapNpc.Script, "zccangku.mac", sizeof(stMapNpc.Script));

	// 以后改成npcid.mac   备忘
	snprintf(stMapNpc.Script, sizeof(stMapNpc.Script), "%d.mac", iNpcDefID);

	if (0 > z_do_call_npc(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstMapIndex, pstMapInst, &stMapNpc, iNpcLifeTime))
	{
		return -1;
	}
	return 0;
}

int z_new_map_tmp_item(ZONESVRENV* pstEnv, MapIndex* pstMapIndex, ZONEMAPINST* pstMapInst,MAPTMPITEMDEF* pstTmpItemDef)
{
	DropItem* pstItem = NULL;
	RESPOS stPos;
	ITEMDEF* pstItemDef = z_find_itemdef(pstEnv,pstTmpItemDef->TmpItemID);
	ZONEIDX stIdx;

	if (!pstItemDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tmp item MapID=%d ListIndex=%d ItemID=%d",pstTmpItemDef->MapID,pstTmpItemDef->ListIndex,pstTmpItemDef->TmpItemID);
		return -1;
	}

	stPos.X = pstTmpItemDef->X;
	stPos.Y = pstTmpItemDef->Y;
	stIdx.iID = pstMapInst->iMapInstID;
	stIdx.iPos = pstMapInst->iMapPos;
	
	tmp_item_create(pstEnv,NULL, pstTmpItemDef->TmpItemID, &stIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, pstItemDef->ItemTmp.RefreshTime, 
						pstItemDef->ItemTmp.StayTime, pstTmpItemDef->Dir,0,0,&pstItem);
	if (!pstItem)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tmp item MapID=%d ListIndex=%d ItemID=%d",pstTmpItemDef->MapID,pstTmpItemDef->ListIndex,pstTmpItemDef->TmpItemID);
		return -1;
	}

	pstItem->iTmpItemFlag |= TMP_ITEM_FLAG_MAP_REFRESH;

	return 0;
	UNUSED(pstMapIndex);	//add by paraunused.pl
}

int get_map_rand_mon_memid(ZONESVRENV* pstEnv,ZONEMAPINST* pstMapInst)
{
	int i = 0;
	int iRand = RAND1(pstMapInst->iMonsterCount);
	int iMemID = 0;
	int iCount = 0;
	
	for (i = 0; i < pstMapInst->iAreaDyn; i++) 
	{
		int iIndex	=	pstMapInst->aiAreaDyn[i];
		int k = 0;
		AREADYN* pstArea;

		if( iIndex<0)
		{
			continue;
		}

		pstArea	= (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}
		
		for (k = 0; k < pstArea->iAnimate; k++) 
		{
			Monster* pstMonster = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[k]);
			MONSTERDEF* pstMonDef = NULL;
			
			if(NULL == pstMonster || MONSTER_DIE == pstMonster->cStat ||
				is_city_build_mon(pstMonster) || pstMonster->iPKType != MON_PK_TYPE_NONE)
			{
				continue;
			}
			
			pstMonDef = z_get_mon_def(pstEnv,pstMonster);
			if (!pstMonDef ||
				(pstMonDef->MoveType != MOVE_TYPE_RANDOM))
			{
				continue;
			}
			

			iMemID = pstMonster->iID;
			
			if (iCount == iRand)
			{
				break;
			}
			iCount++;
		}
	}


	return iMemID;
}

MAPMONSTER * get_rand_map_mon_def(ZONESVRENV* pstEnv, int iMapID,int iExceptSafeArea)
{
	MapIndex* pstMapIndex1;
	MapIndex* pstMapIndexRefer = NULL;
	MapObj* pstMap;
	int i;

	MAPMONSTER *pstMapMonster;

	int iMapPos = 0;

	pstMap	=	pstEnv->pstMapObj;

	pstMapIndex1	=	z_mappos_index(pstMap, &iMapPos, iMapID);
	if( !pstMapIndex1 )
		return NULL;

	if (pstMapIndex1->stMapDef.ReferMapID > 0 && pstMapIndex1->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		pstMapIndexRefer = z_mappos_index(pstMap, &pstMapIndex1->stMapDef.ReferMapIDPos, pstMapIndex1->stMapDef.ReferMapID);
		if (NULL == pstMapIndexRefer)
		{
			return NULL;
		}
	}
	else
	{
		pstMapIndexRefer = pstMapIndex1;
	}

	if ( pstMapIndexRefer->iMonsterCount < 1)
	{
		return NULL;
	}

	i = RAND1(pstMapIndexRefer->iMonsterCount) ;

	pstMapMonster = (MAPMONSTER *)pstMap->sMapMonsterDef[i + pstMapIndexRefer->iMonsterStart];
	

	if (iExceptSafeArea && 
		(pstMapMonster->MonsterID == 233011 || 
		pstMapMonster->MonsterID == 226011 ||
		pstMapMonster->MonsterID == 253005 ||
		pstMapMonster->MonsterID == 205030 ||
		//上面安全区的,下面安全区周围的
		pstMapMonster->MonsterID == 235002 ||
		pstMapMonster->MonsterID == 235003 ||
		pstMapMonster->MonsterID == 241004 ||
		pstMapMonster->MonsterID == 233019 ||
		pstMapMonster->MonsterID == 261019 ||
		pstMapMonster->MonsterID == 235014 ||
		pstMapMonster->MonsterID == 204102 ||
		pstMapMonster->MonsterID == 250004 ||
		pstMapMonster->MonsterID == 238000 ||
		pstMapMonster->MonsterID == 293092 ||
		pstMapMonster->MonsterID == 293145 ||
		pstMapMonster->MonsterID == 293146 ||
		pstMapMonster->MonsterID == 293147 ||
		pstMapMonster->MonsterID == 293201
		))
	{
		int k = 0;
		pstMapMonster = NULL;
		for(k = 0; k < pstMapIndexRefer->iMonsterCount; k++)
		{
			int iIdx =( k + i) % pstMapIndexRefer->iMonsterCount + pstMapIndexRefer->iMonsterStart;
			
			pstMapMonster = (MAPMONSTER *)pstMap->sMapMonsterDef[iIdx];
			if (pstMapMonster->MonsterID != 233011 &&
				pstMapMonster->MonsterID != 226011 &&
				pstMapMonster->MonsterID != 253005 &&
				pstMapMonster->MonsterID != 205030 &&
				//上面安全区的,下面安全区周围的
				pstMapMonster->MonsterID != 235002 &&
				pstMapMonster->MonsterID != 235003 &&
				pstMapMonster->MonsterID != 241004 &&
				pstMapMonster->MonsterID != 233019 &&
				pstMapMonster->MonsterID != 261019 &&
				pstMapMonster->MonsterID != 235014 &&
				pstMapMonster->MonsterID != 204102 &&
				pstMapMonster->MonsterID != 250004 &&
				pstMapMonster->MonsterID != 238000 &&
				pstMapMonster->MonsterID != 293092 &&
				pstMapMonster->MonsterID != 293145 &&
				pstMapMonster->MonsterID != 293146 &&
				pstMapMonster->MonsterID != 293147 &&
				pstMapMonster->MonsterID != 293201)
			{
				break;
			}
			
		}
	}

	return pstMapMonster;
}


int z_in_refresh_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, PWORLDSELECT *pstSelect)
{
	MapIndex* pstMapIndex1;
	MapIndex* pstMapIndexRefer = NULL;
	MapObj* pstMap;
	int i;
	int iRet;
	MAPMONSTER *pstMapMonster;
	MAPNPC *pstMapNpc;
	///Npc *pstNpc;
	MAPTMPITEMDEF* pstTmpItemDef = NULL;

	pstMap	=	pstEnv->pstMapObj;

	pstMapIndex1	=	z_mappos_index(pstMap, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if( !pstMapIndex1 )
		return -1;

	if (pstMapIndex1->stMapDef.ReferMapID > 0 && pstMapIndex1->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		pstMapIndexRefer = z_mappos_index(pstMap, &pstMapIndex1->stMapDef.ReferMapIDPos, pstMapIndex1->stMapDef.ReferMapID);
		if (NULL == pstMapIndexRefer)
		{
			return -1;
		}
	}
	else
	{
		pstMapIndexRefer = pstMapIndex1;
	}

	for(i=0; i<pstMapIndexRefer->iMonsterCount; i++)
	{
		if( i+pstMapIndexRefer->iMonsterStart >= pstMap->iMapMonsterDef )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "monster index=%d",  i+pstMapIndexRefer->iMonsterStart);
			continue;
		}

		pstMapMonster = (MAPMONSTER *)pstMap->sMapMonsterDef[i+pstMapIndexRefer->iMonsterStart];

		if (PWORLD_LEVEL_NORMAL == pstSelect->PworldDifficulty && pstMapMonster->MonsterID > PWORLD_LEVEL_MON_BASE)
		{
			continue;
		}

		if (PWORLD_LEVEL_JINYING == pstSelect->PworldDifficulty && pstMapMonster->MonsterID < PWORLD_LEVEL_MON_BASE)
		{
			continue;
		}

		if (z_in_new_pworld_monster(pstAppCtx, pstEnv, pstMapIndex1, pstMapInst, pstMapMonster) == 0)
		{

		}
		
	}

	for(i=0; i<pstMapIndexRefer->iNpcCount; i++)
	{
		if( i+pstMapIndexRefer->iNpcStart >= pstMap->iMapNpcDef )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "npc index=%d", i+pstMapIndexRefer->iNpcStart);
			continue;
		}

		pstMapNpc = (MAPNPC *)pstMap->sMapNpcDef[i+pstMapIndexRefer->iNpcStart];

		iRet = z_in_new_pworld_npc(pstAppCtx, pstEnv, pstMapIndex1, pstMapInst, pstMapNpc,0);

		//野外地图种的NPC 都会刷新
		/*
		pstNpc = z_id_npc(pstAppCtx, pstEnv, iRet);
		if(pstNpc && pstMapIndexRefer->stMapDef.MapID < PWORLD_MAPID_BASE) 
		{
			pstNpc->stMon.cRefresh = 1;
		}*/
	}

	//刷新矿资源
	map_refresh_ore( pstEnv, pstMapIndexRefer, pstMapInst);

	//刷新临时物品
	for(i=0; i<pstMapIndexRefer->iTmpItemCount; i++)
	{
		if( i+pstMapIndexRefer->iTmpItemStart >= pstMap->iMapMonsterDef )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmp item index=%d",  i+pstMapIndexRefer->iTmpItemStart);
			continue;
		}

		pstTmpItemDef = (MAPTMPITEMDEF *)pstMap->sMapTmpItemDef[i+pstMapIndexRefer->iTmpItemStart];

		z_new_map_tmp_item(pstEnv,pstMapIndex1, pstMapInst,pstTmpItemDef);
	}
	

	return 0;
}

int z_new_map_areadyn_check(MapIndex* pstMapIndex, int iAreaNum, int iXAreaFromNum, int iYAreaFromNum,
									int iXAreatoNum, int iYAreatoNum)
{
	int iX, iY;

	iX =	iAreaNum % pstMapIndex->iXAreaNum;
	iY =	iAreaNum / pstMapIndex->iXAreaNum;

	if (iXAreaFromNum == 0 
		&& iYAreaFromNum == 0
		&& iXAreatoNum == 0
		&& iYAreatoNum == 0)
	{
		return 0;
	}

	if (iX >= iXAreaFromNum && iX <= iXAreatoNum + 1
		&& iY >= iYAreaFromNum && iY<= iYAreatoNum + 1)
	{
		return 0;
	}

	return -1;
	
}

int z_in_new_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iPworldInstID, PWORLDSELECT *pstSelect,/*int iLevel*/ZONEIDX *pstIdx)
{
	MapObj* pstMap;
	MapIndex* pstMapIndex;
	ZONEPWORLDINST* pstPworldInst;
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
	int iIndex;
	int iRealIndex;
	int i;
	int iXAreaFromNum = 0, iYAreaFromNum = 0;
	int iXAreaToNum = 0, iYAreaToNum = 0;
	ZONEIDX stIdx;
	TMEMBLOCK *pstMemBlock;
	char szScript[32];
	ScriptIndex *pstScriptInd;
	ZONEPWORLD *pstZonePworld;
	
	pstMap	=	pstEnv->pstMapObj;
	
	pstMapIndex =	z_mappos_index(pstMap, &pstIdx->iPos, pstIdx->iID);
	if( !pstMapIndex )
		return -1;

	//只有跨服野外才会加载此地图
	if((pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP) && (!is_battle_pvp_svr(pstEnv)))
		return 0;
	
	pstPworldInst	=	z_find_pworld_inst(pstMap, iPworldInstID);
	
	if( !pstPworldInst && iPworldInstID )
		return -1;
	
	if( pstPworldInst && pstPworldInst->iMapInst >= MAX_PWORLD_MAP )
		return -1;
	
	if( pstMapIndex->iXAreaNum*pstMapIndex->iYAreaNum > MAX_MAP_AREA )
		return -1;

	iIndex = tmempool_alloc(pstEnv->pstMapInstPool);
	if (0 > iIndex)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mempool alloc map inst fail");
		return -1;	
	}

	iRealIndex = iIndex;
	iIndex = iIndex % pstEnv->pstMapInstPool->iMax;
	pstMemBlock = (TMEMBLOCK *)TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstMapInstPool, iIndex);
	
	pstMapInst	=	(ZONEMAPINST*)pstMemBlock->szData;

	pstMapInst->iRealIndex = iRealIndex;
	pstMapInst->iIndex = iIndex;
	pstMapInst->nEffectID =	pstMapIndex->stMapDef.EffectID;
	pstMapInst->iMapPos 		=	pstIdx->iPos;
	pstMapInst->iMapInstID		=	(iPworldInstID & 0xffff)<<16 | (pstIdx->iID & 0xffff);
	pstMapInst->iMapArea		=	pstMapIndex->iXAreaNum*pstMapIndex->iYAreaNum;
	pstMapInst->bBroadcast = 0;
	pstMapInst->iDynMskMonCount = 0;
	memset(pstMapInst->aiDynMskMonMemID,0,sizeof(pstMapInst->aiDynMskMonMemID));
	//暂时只支持副本中的地图广播
	if (pstMapIndex->stMapDef.Broadcast > 0 && pstPworldInst)
	{
		pstMapInst->bBroadcast     =  pstMapIndex->stMapDef.Broadcast;
	}

	memset(pstMapInst->aiDynReviveCamp, 0, sizeof(pstMapInst->aiDynReviveCamp));

	/*-------jerry modify--------*/
	pstMapInst->szScriptVar[0] = 0;
	pstMapInst->stScriptIdx.cScriptType = -1;
	pstMapInst->cInstScriptFlag = 0;

	pstMapInst->iOre = 0;
	memset(pstMapInst->tRefreshOreTime, 0, sizeof(pstMapInst->tRefreshOreTime));
	
	snprintf(szScript, sizeof(szScript),"map%d.mac", pstMapIndex->stMapDef.MapID);
	szScript[sizeof(szScript) - 1] = 0;
	pstScriptInd = get_script_ind(szScript);
	if (pstScriptInd)
	{
		pstMapInst->stScriptIdx = *pstScriptInd;
		if (0 == check_script_proc(szScript, pstScriptInd, MAP_INST_SCRIPT_PROC_TIME))
		{
			pstMapInst->cInstScriptFlag |= MAP_INST_SCRIPT_PROC_TIME_FLAG;
		}

		if (0 == check_script_proc(szScript, pstScriptInd, MAP_INST_SCRIPT_PROC_MON_DIE))
		{
			pstMapInst->cInstScriptFlag |= MAP_INST_SCRIPT_PROC_MON_DIE_MAP;
		}
	}
	/*-------modify end--------*/
	
	if( pstPworldInst )
	{
		pstPworldInst->astMapInstIdx[pstPworldInst->iMapInst].iID	=	pstMapInst->iMapInstID;
		pstPworldInst->astMapInstIdx[pstPworldInst->iMapInst].iPos	=	iIndex;
		pstPworldInst->iMapInst++;
	
		pstMapInst->iPworldInstID	=	pstPworldInst->iPworldInstID;
	}
	else
		pstMapInst->iPworldInstID	=	0;
	
	stIdx.iID	=	pstMapInst->iMapInstID;
	stIdx.iPos= iIndex;
	
	binsert_int(&stIdx, pstMap->astMapInstIdx, &pstMap->iMapInst, sizeof(pstMap->astMapInstIdx[0]), 1);
	
	pstMapInst->iAreaDyn		=	0;
	pstMapInst->iMonsterCount	= 0;
	pstMapInst->iPlayerCount = 0;

	//计算地图实际占用区域
	iXAreaFromNum = pstMapIndex->stMapDef.ReferAreaFrom.X/AREA_XGRID;
	iYAreaFromNum = pstMapIndex->stMapDef.ReferAreaFrom.Y/AREA_YGRID;

	iXAreaToNum = pstMapIndex->stMapDef.ReferAreaTo.X/AREA_XGRID;
	iYAreaToNum = pstMapIndex->stMapDef.ReferAreaTo.Y/AREA_YGRID;
	
	for(i=0; i<pstMapInst->iMapArea; i++)
	{
		iIndex = -1;
		
		if (0 == z_new_map_areadyn_check(pstMapIndex, i, 
											iXAreaFromNum, iYAreaFromNum, 
											iXAreaToNum, iYAreaToNum))
		{
			iIndex = tmempool_alloc(pstEnv->pstAreaDynPool);
			if (0 > iIndex)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "mempool alloc area dyn fail");
				return -1;	
			}
			
			pstMemBlock = (TMEMBLOCK *)TMEMPOOL_GET_PTR(pstEnv->pstAreaDynPool, iIndex);
			pstAreaDyn	=	(AREADYN*) pstMemBlock->szData;
			
			pstAreaDyn->iMapInstID	=	pstMapInst->iMapInstID;
			pstAreaDyn->iAnimate	=	0;
			pstAreaDyn->iInanimate	=	0;
			pstAreaDyn->x			=	i % pstMapIndex->iXAreaNum;
			pstAreaDyn->y			=	i / pstMapIndex->iXAreaNum;
			pstAreaDyn->iDynMskIdx = -1;
		}
		pstMapInst->aiAreaDyn[pstMapInst->iAreaDyn] = iIndex;
		pstMapInst->iAreaDyn++;
	}
	
	tlog_debug(pstEnv->pstLogCat, 0, 0,  "InternalNewMapInst pworldinst:%d map:%d success", iPworldInstID, pstIdx->iID);
	
	z_in_refresh_map_inst(pstAppCtx, pstEnv, pstMapInst,pstSelect);

	if (pstPworldInst && pstPworldInst->stPworldIdx.iID == UNREAL_PWORLD_ID)
	{
		unreal_refresh_map_inst(pstEnv,pstPworldInst,pstMapInst);
	}
	
	if (pstPworldInst)
	{
		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (pstZonePworld && pstZonePworld->stPworldDef.StrongMapID > 0)
		{
			if (strong_is_normal(pstZonePworld))
			{
				strong_refresh_map_inst(pstEnv,pstPworldInst,pstMapInst,pstZonePworld->stPworldDef.StrongMapID);
			}
			else
			{
				span_strong_refresh_map_inst(pstEnv,pstPworldInst,pstMapInst,pstZonePworld->stPworldDef.StrongMapID);
			}
		}
	}
	return 0;
}

int z_new_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iMapIndex)
{
	ZONEIDX stIdx;
	PWORLDSELECT stSelect;

	memset(&stSelect, 0, sizeof(stSelect));
	stSelect.PworldDifficulty = PWORLD_LEVEL_NORMAL;
	stIdx.iID	=	iMapIndex;
	stIdx.iPos	=	-1;

	return z_in_new_map_inst(pstAppCtx, pstEnv, 0, &stSelect, &stIdx);
}

ZONEPWORLD* z_find_pworld(MapObj* pstMap, ZONEIDX* pstIdx)
{
	ZONEPWORLD stKey;
	ZONEPWORLD* pstFind;
	int iEqu;
	int iIndex;

	/*根据数组索引位置获取副本信息*/
	if( pstIdx->iPos >= 0 && pstIdx->iPos < pstMap->iPworld )
	{
		pstFind	= (ZONEPWORLD*)pstMap->sPworld[pstIdx->iPos];

		if( pstIdx->iID == (int)pstFind->stPworldDef.PworldID)
		{
			return pstFind;
		}
	}

	/*根据副本定义ID获取副本信息*/
	stKey.stPworldDef.PworldID = pstIdx->iID;
	iIndex = bsearch_int(&stKey, pstMap->sPworld, pstMap->iPworld, PWORLD_DATA, &iEqu);
	if (!iEqu)
	{
		return NULL;
	}
	pstFind	= (ZONEPWORLD*)pstMap->sPworld[iIndex];
	pstIdx->iPos = iIndex;

	return pstFind;
}

int z_mon_home_leave_view(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	CSPKG stPkg;
	MONSTERDEF *pstMonDef;
	CSPKG *pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;
	int iDel;

	iDel = bdelete_int(&pstMon->iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int));
	
	if( !iDel)
	{
		return -1;
	}

	if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
	{
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (pstMonDef)
		{
			pstLeaveView->ID = pstMon->iID;
			pstLeaveView->HomeFlag = 1;
			pstLeaveView->HomeData.Pos = pstMon->stInitPos;
			pstLeaveView->HomeData.Speed = pstMonDef->HomeMoveSpd;
			if (0 == pstLeaveView->HomeData.Speed)
			{
				pstLeaveView->HomeData.Speed = pstMonDef->FightMoveSpd;
			}

			Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);
			z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
		}
	}

	return 0;
}

int newnpc_add(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Npc *pstNpc)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstNpc->stMon.iID;
	pstActionRes->ActionID = ACTION_OBJ_ENTER_MAP;

	pstActionRes->Data.ObjEnterMap.Type = OBJ_NPC;
	pstActionRes->Data.ObjEnterMap.DefID = pstNpc->stMon.iDefIdx;
	pstActionRes->Data.ObjEnterMap.x = pstNpc->stMon.stCurrPos.X;
	pstActionRes->Data.ObjEnterMap.y = pstNpc->stMon.stCurrPos.Y;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);

	if (pstMapInst->iNewNpc >= MAX_NEW_NPC)
	{
		memmove(&pstMapInst->aiNewNpc[0], &pstMapInst->aiNewNpc[1], 
			(MAX_NEW_NPC-1)*sizeof(pstMapInst->aiNewNpc[0]));
		pstMapInst->aiNewNpc[MAX_NEW_NPC-1] = pstNpc->stMon.iID;
	}
	else
	{
		pstMapInst->aiNewNpc[pstMapInst->iNewNpc++] = pstNpc->stMon.iID;
	}

	return 0;
	UNUSED(pstEnv);
}

int newnpc_del(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMon)
{
	int i;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	for (i=pstMapInst->iNewNpc-1; i>=0; i--)
	{
		if (pstMapInst->aiNewNpc[i] == pstMon->iID)
		{
			pstActionRes->ID = pstMon->iID;
			pstActionRes->ActionID = ACTION_OBJ_LEAVE_MAP;

			pstActionRes->Data.ObjLeaveMap.Type = OBJ_NPC;
			pstActionRes->Data.ObjLeaveMap.DefID = pstMon->iDefIdx;
			
			Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
			map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);

			pstMapInst->iNewNpc--;
			if (i != pstMapInst->iNewNpc)
			{
				pstMapInst->aiNewNpc[i] = pstMapInst->aiNewNpc[pstMapInst->iNewNpc];
			}

			break;
		}
	}
	
	return 0;
}

int map_dynmsk_mon_memid_add(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, int iMemID)
{
	int i = 0;
	
	for (i=pstMapInst->iDynMskMonCount-1; i>=0; i--)
	{
		if (pstMapInst->aiDynMskMonMemID[i] == iMemID)
		{
			return 0;
		}
	}

	if( pstMapInst->iDynMskMonCount >= MAX_DYN_MSK_MON ||
		pstMapInst->iDynMskMonCount < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,  "map_dynmsk_mon_memid_add fail count:%d", pstMapInst->iDynMskMonCount);
	
		return -1;
	}

	if( pstMapInst->iDynMskMonCount < 0 )
	{
		if(pstMapInst->iDynMskMonCount < 0)
		{
			pstMapInst->iDynMskMonCount = 0;
		}
	}
	
	pstMapInst->aiDynMskMonMemID[pstMapInst->iDynMskMonCount++] = iMemID;
	
	return 0;
	UNUSED(pstEnv);
}

int map_dynmsk_mon_memid_del(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, int iMemID)
{
	int i=0;

	for (i=pstMapInst->iDynMskMonCount-1; i>=0; i--)
	{
		if (pstMapInst->aiDynMskMonMemID[i] == iMemID)
		{
			pstMapInst->iDynMskMonCount--;
			if (i != pstMapInst->iDynMskMonCount)
			{
				pstMapInst->aiDynMskMonMemID[i] = pstMapInst->aiDynMskMonMemID[pstMapInst->iDynMskMonCount];
			}
			break;
		}
	}
	
	return 0;
	UNUSED(pstEnv);
}

int mon_area_view_del(ZONESVRENV* pstEnv, Monster *pstMon, ZONEMAPINST* pstMapInst)
{
	int i, j, iIndex;
	char cType;
	Player *pstVPlayer;
	Monster *pstVMon;
	TMEMBLOCK *pstMemBlock;
	SearchArea stSArea;
	AREADYN *pstArea;

	if (z_pos_search_area(pstEnv->pstAppCtx, pstEnv,  pstMapInst, &pstMon->stCurrPos, &stSArea) < 0) return -1;

	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
             {
			iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			switch(cType)
			{
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						continue;
					}
					
					pstVPlayer = (Player *)pstMemBlock->szData;

					bdelete_int(&pstMon->iID, &pstVPlayer->stOnline.aiVAnimate, &pstVPlayer->stOnline.iVAnimate, sizeof(int));
					break;
				case OBJ_MONSTER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						continue;
					}
					
					pstVMon = (Monster *)pstMemBlock->szData;
					if (is_mon_machine(pstVMon))
					{
						bdelete_int(&pstMon->iID, &pstVMon->aiVAnimate, &pstVMon->iVPlayer, sizeof(int));
					}
					break;
			}
		}
       }
	
	return 0;
}

int z_monster_leave_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i, iIndex;
	char cType;
	Player *pstVPlayer;
	ZONEMAPINST* pstMapInst;
	TMEMBLOCK *pstMemBlock;
	MapIndex *pstMapIdx;
	int iRet;
	int iID;
	MONSTERDEF *pstMonDef;
	Monster *pstVMon;

	//tlog_error(pstEnv->pstLogCat, 0, 0, "z_monster_leave_map mon = %d", pstMon->iID);		
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}

	if (!(pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap))) return -1;
	pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	if (pstMapInst->bBroadcast || (pstMon->cMonMiscFlag & (MON_MISC_FLAG_MAPBROAD|MON_MISC_FLAG_IMPVIEW)))
	{
		CSPKG stPkg;
		CSPKG *pstPkg = &stPkg;
		CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;
		
		pstLeaveView->ID = pstMon->iID;

		/*
		if (AI_STAT_HOME == pstMon->bAiStat)
		{
			pstLeaveView->HomeFlag = 1;
			pstLeaveView->HomeData.Pos = pstMon->stInitPos;
			pstLeaveView->HomeData.Speed = pstMonDef->HomeMoveSpd;
			if (0 == pstLeaveView->HomeData.Speed)
			{
				pstLeaveView->HomeData.Speed = pstMonDef->FightMoveSpd;
			}			
		}
		else
		*/

		pstLeaveView->HomeFlag = 0;
		
		Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);

		z_sendpkg_mon_area_view(pstAppCtx, pstEnv, pstMon, &stPkg);

		mon_area_view_del(pstEnv, pstMon, pstMapInst);
	}
	else
	{
		for (i=pstMon->iVPlayer-1; i>=0; i--)
		{
			iID = pstMon->aiVPlayer[i];
			iIndex = z_id_indextype(pstAppCtx, pstEnv, iID , &cType);
			if (iIndex < 0) continue;

			if (is_mon_machine(pstMon))
			{
				obj_leave_machine_view(pstEnv, pstMon, pstMapInst, iID, cType);
				/*
				switch (cType) {
					case OBJ_NPC:
					case OBJ_MONSTER:
						if (cType == OBJ_NPC) 
						{
							pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
						}
						else
						{
							pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
						}
						if (!pstMemBlock->fValid)
						{
							break;
						}

						pstVMon = (Monster *)pstMemBlock->szData;

						if (OBJ_MONSTER == cType)
						{
							mon_press_delete_mon(pstEnv, pstVMon, pstMon);
						}
											
						bdelete_int(&pstMon->iID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int));

						obj_leave_machine_view(pstEnv, pstMon, pstMapInst, iID, cType);
						break;
					case OBJ_PLAYER:
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
						if (!pstMemBlock->fValid)
						{
							break;
						}
						
						pstVPlayer = (Player *)pstMemBlock->szData;
						z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 1, pstMon->iID, OBJ_PLAYER, 0);
						break;
				}
				*/
			}
			else if (cType == OBJ_PLAYER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (pstMemBlock->fValid)
				{
					pstVPlayer = (Player *)pstMemBlock->szData;

					/*
					if (AI_STAT_HOME == pstMon->bAiStat)
					{
						z_mon_home_leave_view(pstEnv, pstVPlayer, pstMon);
					}
					else
					*/
					
					z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 1, pstMon->iID, OBJ_MONSTER, 1);
				}
			}
			else if(cType == OBJ_MONSTER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}

				pstVMon = (Monster *)pstMemBlock->szData;
				if(is_mon_machine(pstVMon))
				{
					obj_leave_machine_view(pstEnv, pstVMon, pstMapInst, pstMon->iID, OBJ_MONSTER);
				}
			}
		}
	}
	
	pstMon->iVPlayer = 0;

	if (0 > (iRet = z_ani_leave_area(pstMapIdx, pstEnv->pstAreaDynPool, pstMon->iID, &pstMon->stCurrPos, pstMapInst)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d %d z_ani_leave_area ret %d, mon map=%d, pos=(%d,%d), nextpos=(%d,%d), ai stat=%d", 
					pstMon->iID, pstMon->iDefIdx, iRet, pstMon->stMap.iID, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, 
					pstMon->stNextPos.X, pstMon->stNextPos.Y, pstMon->bAiStat);
		return -1;
	}

	if (pstMon->cMonMiscFlag & MON_MISC_FLAG_DYN)
	{
		map_dynmsk_mon_memid_del(pstEnv, pstMapInst, pstMon->iID);
		z_mon_dyn_mask_set(pstEnv, pstMapIdx, pstMapInst, pstMon, pstMonDef, 0);
		z_refresh_area_dynmsk(pstEnv, pstMapIdx, pstMapInst, pstMon);
	}

	if (pstMon->cType == OBJ_MONSTER &&
		MON_SUB_TYPE_TMP > pstMon->bSubType && 
		pstMon->bSubType != MON_SUB_TYPE_TOTEM)
	{
		pstMapInst->iMonsterCount--;
		if (pstMapInst->iMonsterCount < 0)
		{
			pstMapInst->iMonsterCount = 0;
		}
	}

	unreal_up_boss_info(pstEnv, pstMon, 0);

	map_inst_ore_del(pstMapInst, pstMon);

	newnpc_del(pstEnv, pstMapInst, pstMon);

	losttotem_leave_map(pstEnv, pstMon);
	
	//tlog_debug(pstEnv->pstLogCat, 0, 0, "monster %d leave map inst %d", pstMon->iDefIdx, pstMapInst->iMapInstID);

	return 0;
}

int z_item_leave_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, DropItem *pstDItem)
{
	int i, j, iIndex;
	SearchArea stSArea;
	AREADYN *pstArea;
	char cType;
	ZONEMAPINST* pstMapInst;
	Player *pstVPlayer;
	int iRet;
	TMEMBLOCK *pstMemBlock;
	MapIndex *pstMapIndex;

	tlog_debug(pstEnv->pstLogCat, 0, 0, "item %d %d map %d pos (%d,%d) leave map", pstDItem->iID, 
				pstDItem->iDefIdx, pstDItem->stMap.iID, pstDItem->stPos.X, pstDItem->stPos.Y);

	if (!(pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstDItem->stMap)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "item leave map z_find_map_inst null");
		return -1;
	}
	
	pstMapIndex	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIndex)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0 , "item leave map z_mappos_index null");
		return -2;
	}
	
	if (z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, &pstDItem->stPos, &stSArea) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "item leave map z_pos_search_area fail");
		return -3;
	}

	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstArea->aiAnimate[j], &cType);
			if (iIndex < 0 ) continue ;
	            switch ( cType )
	            {
	                case OBJ_PLAYER:
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
						if (pstMemBlock->fValid)
						{
							pstVPlayer = (Player *)pstMemBlock->szData;
							z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 0, pstDItem->iID, OBJ_ITEM, 1);
						}
						break ;
	                default :
	                    break ;
	     	     }
		}
	}
	
	if ( 0 > (iRet = z_item_leave_area(pstMapIndex, pstEnv->pstAreaDynPool,  pstDItem->iID, &pstDItem->stPos, pstMapInst)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_item_leave_area ret %d", iRet);
	}

	z_id_clean(pstEnv, pstDItem->iID);
	pstDItem->cStat = DROP_ITEM_NOSTAY;

	if (0 == pstDItem->unRefreshTime)
	{
		if (0 > tmempool_free(pstEnv->pstItemPool, pstDItem->iMIndex))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_free fail");
		}
	}
	else if (pstDItem->unRefreshTime > 0)
	{
		pstDItem->iLeaveMapTime = pstAppCtx->stCurr.tv_sec;
	}
		
	return 0;
}

int z_item_clean(ZONESVRENV* pstEnv, DropItem *pstDItem)
{
	if (0 > z_item_leave_map(pstEnv->pstAppCtx, pstEnv, pstDItem))
	{
		z_id_clean(pstEnv, pstDItem->iID);
		tmempool_free(pstEnv->pstItemPool, pstDItem->iMIndex);	
	}
	else
	{
		if (0 != pstDItem->unRefreshTime)
		{
			tmempool_free(pstEnv->pstItemPool, pstDItem->iMIndex);		
		}
	}

	return 0;
}

int z_item_enter_map(ZONESVRENV * pstEnv, DropItem *pstDropItem)
{
	int iRet;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	RESPOS stPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstDropItem->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -2;
	}

	iRet = z_item_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, pstDropItem->iID, &pstDropItem->stPos, pstMapInst);

	if (0  > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_item_enter_area err %d", iRet);
		return iRet;
	}

	if (z_pos_search_pos(pstMapIdx, &pstDropItem->stPos, &stPos))
	{
		z_item_search_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPos, pstDropItem);
	}

	return 0;
}

// 退出副本控制
int player_pworld_exit_ctrl(ZONESVRENV* pstEnv, Player *pstPlayer,ZONEPWORLD *pstExitZonePworld)
{
	if (pstExitZonePworld )
	{
		if (pstExitZonePworld->stPworldDef.CtrlFlag & PWORLD_CLEAN_STATUS )
		{
			// 还原状态
			z_status_online(pstEnv->pstAppCtx, pstEnv, pstPlayer , 1);
			z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer );
			z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}

		if (pstExitZonePworld->stPworldDef.CtrlFlag & PWORLD_EXIT_CLEAN_XP)
		{
			fairy_camp_clean_xp(pstEnv, pstPlayer);
		}
	}
	return 0;
}

// 尽量不要使用此函数, 没有对ipos赋值,效率低--jerry
ZONEPWORLD *z_find_pworld2(ZONESVRENV* pstEnv, int iMap)
{
	if(iMap < PWORLD_MAPID_BASE)
	{	
		return NULL;
	}
	else
	{
		ZONEIDX stIdx;
		stIdx.iID = GET_PWORLDID(iMap);
		stIdx.iPos = -1;
		return  z_find_pworld(pstEnv->pstMapObj, &stIdx);
	}
}


ZONEPWORLDINST * z_find_pworldinst2(ZONESVRENV* pstEnv, int iMap)
{
	if(iMap < PWORLD_MAPID_BASE)
	{	
		return NULL;
	}
	else
	{
		ZONEMAPINST* pstMapInst;
		ZONEIDX stIdx;

		stIdx.iID = iMap;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapInst)
			return NULL;

		return z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	}
}

// 
int player_home(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos)
{
	int iRet;
	if(get_pkval_type(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal) == PK_VAL_TYPE_BLACK)
	{
		pstPos->X = BLACK_NAME_REVIVE_POS_X;
		pstPos->Y = BLACK_NAME_REVIVE_POS_Y;	
		iMap = BLACK_NAME_REVIVE_MAP;
		g_iDesigVar = 2;
	}
	
	if (	pstPlayer->stRoleData.Map>= PWORLD_MAPID_BASE &&
		GET_PWORLDID(pstPlayer->stRoleData.Map) == BCT_PWORLD_ID)
	{
		bct_exit_info(pstEnv, pstPlayer);
		memset (&pstPlayer->stRoleData.MiscInfo.BCTInfo, 0, sizeof(pstPlayer->stRoleData.MiscInfo.BCTInfo));
	}

	iRet = z_player_goto(pstEnv, pstPlayer, iMap, pstPos);
	g_iDesigVar = 0;
	return iRet;
}

int player_pworld_exit_chgmap(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RESPOS stExitPos;
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstZonePworld = NULL;
	int iExitMap;
	MapIndex *pstMapIdx;
	
	stExitPos.X = pstPlayer->stRoleData.ExitPos_x;
	stExitPos.Y = pstPlayer->stRoleData.ExitPos_y;
	iExitMap = pstPlayer->stRoleData.ExitMap;

	// 从幻境boss地图进入 返回时间城
	if (is_unreal_boss_map(pstEnv, GET_MAPID(pstPlayer->stRoleData.ExitMap)))
	{
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, BASE_CITY_MAPID);
		if (pstMapIdx)
		{
			iExitMap = BASE_CITY_MAPID;
			stExitPos = pstMapIdx->stMapDef.RevivePs[0];
		}
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, GET_MAPID(pstPlayer->stRoleData.ExitMap));
	if (pstMapIdx && pstMapIdx->stMapDef.CtrlFlag&MAP_CTRL_LOGOUT_TO_REVIVE_POINT)
	{
		iExitMap = pstMapIdx->stMapDef.ReviveMap;
		stExitPos = pstMapIdx->stMapDef.RevivePs[0];
	}

	if(CLAN_CITY_MAP_ID != GET_MAPID(pstPlayer->stRoleData.Map))
	{
		z_player_del_wushuang_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (pstZonePworldInst)
	{
		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
		if (!pstZonePworld)
		{
			return 0;
		}
		
		if ((pstPlayer->stOnline.State & CS_STAT_DEAD) &&
			is_die_exit_pworld_revive(pstZonePworld))
		{	
			player_revive_svr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}

		if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA)
		{
			int iHPAdd, iMPAdd;
			
			player_status_clean_debuff(pstEnv, pstPlayer);

			iHPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
			iMPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;

			player_revive_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, iHPAdd, iMPAdd);
		}

		strong_exit_clear_buff(pstEnv, pstPlayer, pstZonePworld);
		span_strong_exit_clear_buff(pstEnv, pstPlayer, pstZonePworld);

	}

	player_weixing_clear(pstEnv, pstPlayer);

	g_team_follow_flag = 3;
	g_iTimeLimit = 1;
	z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, iExitMap, &stExitPos, RAND1(180) , 0);
	g_team_follow_flag = 0;
	g_iTimeLimit = 0;
	if (!pstZonePworld) 
		return 0;

	player_pworld_exit_ctrl(pstEnv, pstPlayer,pstZonePworld);
	return 0;
}


// iCleanRoleFlag =  1  清除角色
int z_in_destroy_areadyn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, 
							ZONEPWORLDINST *pstPworldInst, AREADYN* pstAreaDyn, int iCleanRoleFlag)
{
	Monster* pstMon;
	Npc* pstNpc;
	DropItem* pstDropItem;
	Player* pstPlayer;
	int iID;
	char cType;
	int iIndex;
	AREADYN stAreaDyn;
	UNUSED(pstPworld);

	memcpy(&stAreaDyn, pstAreaDyn, sizeof(stAreaDyn));

	while(stAreaDyn.iAnimate>0)
	{
		iID	=	stAreaDyn.aiAnimate[stAreaDyn.iAnimate-1];

		iIndex	=	z_id_indextype(pstAppCtx, pstEnv, iID, &cType);

		if( iIndex<0 )
		{
			stAreaDyn.iAnimate--;
			tlog_error(pstEnv->pstLogCat, 0, 0, "animal id=%d iIndex=%d", iID, iIndex);
			continue;
		}

		switch(cType)
		{
		case OBJ_MONSTER:
			pstMon	=	(Monster *)tmempool_get(pstEnv->pstMonsterPool, iIndex);
			if (pstMon)
			{
				if(is_mon_machine(pstMon))
				{
					mon_machine_back(pstEnv, pstMon);
				}
				else
				{
					z_free_monster(pstAppCtx, pstEnv, pstMon);
				}
			}
			else
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "monster iIndex=%d", iIndex);	
			}
			break;
		case OBJ_NPC:
			pstNpc = (Npc *)tmempool_get(pstEnv->pstNpcPool, iIndex);
			if (pstNpc)
			{
				z_free_monster(pstAppCtx, pstEnv, &pstNpc->stMon);
			}
			else
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "npc iIndex=%d", iIndex);	
			}
			break;
		case OBJ_PLAYER:
			if (pstPworldInst && iCleanRoleFlag)
			{
				pstPlayer	=	(Player*) tmempool_get(pstEnv->pstPlayerPool, iIndex);
				if (pstPlayer)
				{
					  player_pworld_exit_chgmap(pstEnv, pstPlayer);
				}
				else
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "player iIndex=%d", iIndex);	
				}
			}
			break;
		default:
			break;
		}

		stAreaDyn.iAnimate--;
	}

	while(stAreaDyn.iInanimate>0)
	{
		iID	=	stAreaDyn.aiInanimate[stAreaDyn.iInanimate-1];

		iIndex	=	z_id_indextype(pstAppCtx, pstEnv, iID, &cType);

		if( iIndex<0 )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inanimal id=%d iIndex=%d", iID, iIndex);
			stAreaDyn.iInanimate--;
			continue;
		}

		switch(cType)
		{
		case OBJ_ITEM:
			pstDropItem = (DropItem *)tmempool_get(pstEnv->pstItemPool, iIndex);
			if (pstDropItem)
			{
				z_item_clean(pstEnv, pstDropItem);
			}
			else
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "item iIndex=%d", iIndex);	
			}
			break;
		default:
			break;
		}

		stAreaDyn.iInanimate--;
	}

	return 0;
}

int z_in_destroy_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, ZONEPWORLDINST *pstPworldInst, ZONEIDX *pstMapInstIdx)
{
	MapObj* pstMap;
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
	int iIndex;
	ZONEIDX stIdx;

	pstMap	=	pstEnv->pstMapObj;

	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, pstMapInstIdx);
	if( !pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstMapInstIdx->iID);
		return -1;
	}

	while(pstMapInst->iAreaDyn>0)
	{
		iIndex	=	pstMapInst->aiAreaDyn[pstMapInst->iAreaDyn-1];

		if( iIndex<0)
		{
			pstMapInst->iAreaDyn--;
			continue;
		}

		pstAreaDyn	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_get %d area dyn fail", iIndex);
			pstMapInst->iAreaDyn--;
			continue;
		}
		
		if (pstAreaDyn->iDynMskIdx >= 0)
		{
			tmempool_free(pstEnv->pstDynMskPool, pstAreaDyn->iDynMskIdx);
		}

		z_in_destroy_areadyn(pstAppCtx, pstEnv,  pstPworld, pstPworldInst, pstAreaDyn, 1);
		tmempool_free(pstEnv->pstAreaDynPool, iIndex);

		pstMapInst->iAreaDyn--;
	}

	stIdx.iID	=	pstMapInstIdx->iID;
	stIdx.iPos	=	-1;
	bdelete_int(&stIdx, pstMap->astMapInstIdx, &pstMap->iMapInst, sizeof(pstMap->astMapInstIdx[0]));

	tmempool_free(pstEnv->pstMapInstPool, pstMapInst->iRealIndex);

	return 0;
}

int z_clear_map_mon(ZONESVRENV* pstEnv, ZONEIDX *pstMapInstIdx)
{
	int i, k, iID;
	MapObj* pstMap;
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
	Monster *pstMon;
	
	pstMap	=	pstEnv->pstMapObj;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, pstMapInstIdx);
	if( !pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstMapInstIdx->iID);
		return -1;
	}

	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		if (0 > pstMapInst->aiAreaDyn[i])
		{
			continue;
		}
		
		pstAreaDyn	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, pstMapInst->aiAreaDyn[i]);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_get %d area dyn fail", pstMapInst->aiAreaDyn[i]);
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
			if (NULL == pstMon)
			{
				continue;
			}

			if (MON_SUB_TYPE_TMP > pstMon->bSubType)
			{
				//死亡的怪物要有个尸体表现时间
				if(MONSTER_DIE == pstMon->cStat )
				{
					continue;
				}

				//机械回收
				if(is_mon_machine(pstMon))
				{
					mon_machine_back(pstEnv, pstMon);
					continue;
				}
				
				//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
				if (MONSTER_DIE != pstMon->cStat || pstMon->tLeaveMap > 0)
				{
					z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
				}
				
				z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
			}
		}
	}

	return 0;
}


int up_limit_pworld(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, LIMITPWORLD *pstLimitPworld, int iUseItem)
{
	//memset(pstLimitPworld, 0, sizeof(*pstLimitPworld));
	pstLimitPworld->PworldID = pstPworldInst->stPworldIdx.iID;
	pstLimitPworld->Level = pstPworldInst->stPworldselect.PworldDifficulty;
	pstLimitPworld->PworldWID = pstPworldInst->ullPworldWID;
	pstLimitPworld->InstTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	if (!iUseItem)
	{
		pstLimitPworld->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstLimitPworld->Num = 1;
	}
	else
	{
		pstLimitPworld->ItemTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstLimitPworld->ItemNum = 1;
		pstLimitPworld->InstTime = 0;
	}

	pstLimitPworld->WeekNum = 0;
	pstLimitPworld->WeekTime = 0;
	
	{
		ZONEPWORLD* pstZonePworld = NULL;
		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	
		if(pstZonePworld && pstZonePworld->stPworldDef.NumEveryWeek)
		{
			pstLimitPworld->WeekNum = 1;
			pstLimitPworld->WeekTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		}
	}
	
	return 0;
}

int player_limit_time_pworld_check(ZONESVRENV* pstEnv, Player *pstPlayer, PWORLDDEF *pstPworldDef, tdr_ulonglong ullPworldWid)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	for (i=0; i<pstMiscInfo->LimitTimePworldNum; i++)
	{
		if(pstMiscInfo->LimitTimePworldInfo[i].PworldID == (int)pstPworldDef->PworldID &&
			pstMiscInfo->LimitTimePworldInfo[i].PworldWid != ullPworldWid)
		{
			if(pstMiscInfo->LimitTimePworldInfo[i].EndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				return -1;
			}
		}
	}

	return 0;
}

void player_insert_limit_time_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	int iIdx;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	iIdx = -1;
	for (i=0; i<pstMiscInfo->LimitTimePworldNum; i++)
	{
		if(pstMiscInfo->LimitTimePworldInfo[i].PworldID == pstPworldInst->stPworldIdx.iID)
		{
			if(pstMiscInfo->LimitTimePworldInfo[i].PworldWid != pstPworldInst->ullPworldWID)
			{
				pstMiscInfo->LimitTimePworldInfo[i].PworldWid = pstPworldInst->ullPworldWID;
				pstMiscInfo->LimitTimePworldInfo[i].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstZonePworld->stPworldDef.RedoLimitTime;
			}

			return ;
		}

		if(pstMiscInfo->LimitTimePworldInfo[i].EndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			iIdx = i;
			continue;
		}
	}

	if (iIdx >= 0)
	{
		pstMiscInfo->LimitTimePworldInfo[iIdx].PworldID = pstPworldInst->stPworldIdx.iID;
		pstMiscInfo->LimitTimePworldInfo[iIdx].PworldWid = pstPworldInst->ullPworldWID;
		pstMiscInfo->LimitTimePworldInfo[iIdx].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstZonePworld->stPworldDef.RedoLimitTime;
	}
	else
	{
		if (pstMiscInfo->LimitTimePworldNum < MAX_LIMIT_TIME_PWORLD)
		{
			pstMiscInfo->LimitTimePworldInfo[pstMiscInfo->LimitTimePworldNum].PworldID = pstPworldInst->stPworldIdx.iID;
			pstMiscInfo->LimitTimePworldInfo[pstMiscInfo->LimitTimePworldNum].PworldWid = pstPworldInst->ullPworldWID;
			pstMiscInfo->LimitTimePworldInfo[pstMiscInfo->LimitTimePworldNum].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstZonePworld->stPworldDef.RedoLimitTime;
			pstMiscInfo->LimitTimePworldNum++;
		}
	}

	return ;
}
// 0加 1减
void player_insert_limit_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstPworldInst,int AddFlag)
{
	int i;
	int iIdx;
	CSPKG stPkg;
	CSPWORLDLIMITINFO *pstPworldLimitInfo = &stPkg.Body.PworldLimitInfo;
	LIMITPWORLD *pstLimitPworld = NULL;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	LIMITPWORLD *pstChg = &pstPworldLimitInfo->PworldLimitInfoData.Chg;
	int iUseItem = pstPlayer->stOnline.stBctOnline.iUseItemEnterPworld;
	ZONEPWORLD *pstZonePworld;

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstZonePworld || 
		(0 == pstZonePworld->stPworldDef.NumEveryDay && 
		 0 == pstZonePworld->stPworldDef.ItemEnterNum &&
		 0 == pstZonePworld->stPworldDef.NumEveryWeek))
	{
		return;
	}

	iIdx = -1;

	if(AddFlag == 0)
	{
		//如果是活动副本,一进入就加次数
		if(PWORLD_TYPE_PLOY == pstPworldInst->iType)
		{
			pstPworldInst->chLimitFlag = 1;
		}

		player_pworld_offmul_chg(pstEnv, pstPlayer, pstZonePworld->stPworldDef.OffExpMul);
	}
	else
	{
		player_pworld_offmul_chg(pstEnv, pstPlayer, -pstZonePworld->stPworldDef.OffExpMul);
	}

	for (i=0; i<pstMiscInfo->LimitPworldNum; i++)
	{
		if ((int)pstMiscInfo->LimitPworld[i].PworldID == pstPworldInst->stPworldIdx.iID)
		{
			if (!iUseItem)
			{
				if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time))
				{
					//WID != 才算再次创建
					if( (AddFlag==0) && (pstMiscInfo->LimitPworld[i].PworldWID != pstPworldInst->ullPworldWID) )
					{
						pstMiscInfo->LimitPworld[i].Num++;
						pstMiscInfo->LimitPworld[i].PworldWID = pstPworldInst->ullPworldWID;
						pstMiscInfo->LimitPworld[i].InstTime = pstEnv->pstAppCtx->stCurr.tv_sec;
						
						if (pstMiscInfo->LimitPworld[i].WeekTime)
						{
							pstMiscInfo->LimitPworld[i].WeekNum++;
						}
						
						if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime) == 0)
						{
							pstMiscInfo->LimitPworld[i].ItemNum = 0;
							pstMiscInfo->LimitPworld[i].ItemTime= 0;
						}
						pstLimitPworld = &pstMiscInfo->LimitPworld[i];
					}
					else if( (AddFlag==1) && (pstMiscInfo->LimitPworld[i].PworldWID == pstPworldInst->ullPworldWID) )
					{
						// 把次数减回来
						if(pstMiscInfo->LimitPworld[i].Num > 0)
						{
							pstMiscInfo->LimitPworld[i].Num--;
						}
						pstLimitPworld = &pstMiscInfo->LimitPworld[i];
						pstLimitPworld->PworldWID = 0;//清理下这个
						
						if (pstMiscInfo->LimitPworld[i].WeekNum > 0)
						{
							pstMiscInfo->LimitPworld[i].WeekNum--;
						}
					}

					goto _end;
				}
				else if(0 < pstMiscInfo->LimitPworld[i].WeekTime &&
					IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].WeekTime))
				{
					if(AddFlag == 0 )
					{
						pstMiscInfo->LimitPworld[i].WeekNum++;

						pstLimitPworld = &pstMiscInfo->LimitPworld[i];
					}
					else if(AddFlag == 1)
					{
						// 把次数减回来						
						pstLimitPworld = &pstMiscInfo->LimitPworld[i];
						if (pstMiscInfo->LimitPworld[i].WeekNum > 0)
						{
							pstMiscInfo->LimitPworld[i].WeekNum--;
						}
					}

					goto _end;
				}
				else
				{
					iIdx = i;
					break;
				}
			}
			else
			{
				if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime))
				{
					//WID != 才算再次创建
					if(pstMiscInfo->LimitPworld[i].PworldWID != pstPworldInst->ullPworldWID)
					{
						pstMiscInfo->LimitPworld[i].ItemNum++;
						pstMiscInfo->LimitPworld[i].PworldWID = pstPworldInst->ullPworldWID;
						pstMiscInfo->LimitPworld[i].InstTime = 0;
						
						if (pstMiscInfo->LimitPworld[i].WeekTime)
						{
							pstMiscInfo->LimitPworld[i].WeekNum++;
						}
						
						if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time) == 0)
						{
							pstMiscInfo->LimitPworld[i].Num = 0;
							pstMiscInfo->LimitPworld[i].Time= 0;
						}
						pstLimitPworld = &pstMiscInfo->LimitPworld[i];
					}


					goto _end;
				}
				else if(0 < pstMiscInfo->LimitPworld[i].WeekTime &&
					IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].WeekTime))
				{
					pstMiscInfo->LimitPworld[i].WeekNum++;

					pstLimitPworld = &pstMiscInfo->LimitPworld[i];
					goto _end;
				}
				else
				{
					iIdx = i;
					break;
				}
			}
		}

		// 随便找一个可以替换的
		if ((0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time)) && 
			(0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime)))
		{
			if (0 == pstMiscInfo->LimitPworld[i].WeekTime ||
				0 == IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].WeekTime))
			{
				iIdx = i;
			}
			
			continue;
		}
	}

	if(AddFlag == 1)
	{
		//这个流程不走了
		goto _end;
	}

	if (iIdx >= 0)
	{
		up_limit_pworld(pstEnv,pstPworldInst,&pstMiscInfo->LimitPworld[iIdx], iUseItem);
		pstLimitPworld = &pstMiscInfo->LimitPworld[iIdx];	
		goto _end;
	}
	else
	{
		if (pstMiscInfo->LimitPworldNum < MAX_LIMIT_PWORLD_NUM)
		{
			iIdx = pstMiscInfo->LimitPworldNum;

			memset(&pstMiscInfo->LimitPworld[iIdx], 0, sizeof(pstMiscInfo->LimitPworld[0]));
			
			up_limit_pworld(pstEnv,pstPworldInst,&pstMiscInfo->LimitPworld[iIdx], iUseItem);
			pstMiscInfo->LimitPworldNum++;
			pstLimitPworld = &pstMiscInfo->LimitPworld[iIdx];
			goto _end;
		}
		else
		{
			memmove(&pstMiscInfo->LimitPworld[0], &pstMiscInfo->LimitPworld[1],  
					(MAX_LIMIT_PWORLD_NUM-1) * sizeof(pstMiscInfo->LimitPworld[0]));

			memset(&pstMiscInfo->LimitPworld[MAX_LIMIT_PWORLD_NUM-1], 0, sizeof(pstMiscInfo->LimitPworld[0]));
			
			up_limit_pworld(pstEnv,pstPworldInst,&pstMiscInfo->LimitPworld[MAX_LIMIT_PWORLD_NUM-1], iUseItem);
			pstLimitPworld = &pstMiscInfo->LimitPworld[MAX_LIMIT_PWORLD_NUM-1];
			goto _end;
		}
	}

_end:
	if (pstLimitPworld)
	{
		pstPworldLimitInfo->Type = PWORLD_LIMIT_TYPE_CHG;
		memcpy(pstChg, pstLimitPworld, sizeof(*pstChg));

		Z_CSHEAD_INIT(&stPkg.Head, PWORLD_LIMIT_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return ;
}

int pworld_limit_info_get(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPkg;
	CSPWORLDLIMITINFO *pstPworldLimitInfo = &stPkg.Body.PworldLimitInfo;
	CSPWORLDLIMITINFODATAALL *pstAll = &pstPworldLimitInfo->PworldLimitInfoData.All;
	int iRet1,iRet2;
	int iRet3 = 0;
	
	/*for (i=pstMiscInfo->LimitPworldNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time))
		{
			pstMiscInfo->LimitPworldNum--;
			if (i != pstMiscInfo->LimitPworldNum)
			{
				memmove(&pstMiscInfo->LimitPworld[i], &pstMiscInfo->LimitPworld[i+1], 
							(pstMiscInfo->LimitPworldNum - i) * sizeof(pstMiscInfo->LimitPworld[0]));
			}
		}
	}*/

	for (i=pstMiscInfo->LimitPworldNum-1; i>=0; i--)
	{
		iRet1 = IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time);
		iRet2 = IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime);
		iRet3 = (pstMiscInfo->LimitPworld[i].WeekTime ? IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].WeekTime):0);
		if (!iRet1 && !iRet2 && !iRet3)
		{
			pstMiscInfo->LimitPworldNum--;
			if (i != pstMiscInfo->LimitPworldNum)
			{
				memmove(&pstMiscInfo->LimitPworld[i], &pstMiscInfo->LimitPworld[i+1], 
							(pstMiscInfo->LimitPworldNum - i) * sizeof(pstMiscInfo->LimitPworld[0]));
			}
			continue;	
		}

		if (iRet1 == 0)
		{
			pstMiscInfo->LimitPworld[i].Num = 0;
			pstMiscInfo->LimitPworld[i].Time = 0;
		}

		if (iRet2 == 0)
		{
			pstMiscInfo->LimitPworld[i].ItemNum = 0;
			pstMiscInfo->LimitPworld[i].ItemTime = 0;
		}

		if (iRet3 == 0)
		{
			pstMiscInfo->LimitPworld[i].WeekNum= 0;
			pstMiscInfo->LimitPworld[i].WeekTime = 0;
		}		

		
		
		/*if ((iRet1 = IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time)) == 0 &&
			(iRet2 = IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime)) == 0 &&
			(iRet3 = pstMiscInfo->LimitPworld[i].WeekTime ? IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].WeekTime):0) == 0)
		{
			pstMiscInfo->LimitPworldNum--;
			if (i != pstMiscInfo->LimitPworldNum)
			{
				memmove(&pstMiscInfo->LimitPworld[i], &pstMiscInfo->LimitPworld[i+1], 
							(pstMiscInfo->LimitPworldNum - i) * sizeof(pstMiscInfo->LimitPworld[0]));
			}
			continue;
		}

		if (iRet1 == 0)
		{
			pstMiscInfo->LimitPworld[i].Num = 0;
			pstMiscInfo->LimitPworld[i].Time = 0;
		}

		if (iRet2 == 0)
		{
			pstMiscInfo->LimitPworld[i].ItemNum = 0;
			pstMiscInfo->LimitPworld[i].ItemTime = 0;
		}

		if (iRet3 == 0)
		{
			pstMiscInfo->LimitPworld[i].WeekNum= 0;
			pstMiscInfo->LimitPworld[i].WeekTime = 0;
		}	*/	
	}

	pstPworldLimitInfo->Type = PWORLD_LIMIT_TYPE_ALL;
	pstAll->LimitPworldNum = pstMiscInfo->LimitPworldNum;
	memcpy(&pstAll->LimitPworld[0], &pstMiscInfo->LimitPworld[0], 
			pstAll->LimitPworldNum*sizeof(pstAll->LimitPworld[0]));

	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_LIMIT_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int z_player_pworld_destroy(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD* pstPworld, ZONEPWORLDINST *pstPworldInst)
{
	/*
	char szMsg[256];

	if(pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && pstPworld->stPworldDef.Type != PWORLD_TYPE_SINGLE)
	{
		snprintf(szMsg, sizeof(szMsg), "%s 副本已关闭", pstPworld->stPworldDef.Name);
		szMsg[sizeof(szMsg) - 1] = 0;
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
	}
	*/
	
	pstPlayer->stRoleData.MiscInfo.PworldWID = 0;
	pstPlayer->stRoleData.MiscInfo.PworldInstID = 0;

	
	
	return 0;
	UNUSED(pstPworldInst);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPworld);	//add by paraunused.pl
}

int z_team_pworld_destroy(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLD* pstPworld, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	Player *pstPlayer;
	
	pstZoneTeam->iPworldInstID = 0;
	pstZoneTeam->ullPworldWID = 0;

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer && pstPlayer->stRoleData.MiscInfo.PworldWID == pstPworldInst->ullPworldWID)
		{
			z_player_pworld_destroy(pstEnv, pstPlayer, pstPworld, pstPworldInst);
		}
	}

	return 0;
}

int z_in_destroy_pworld_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	Player *pstPlayer;
	ZONEPWORLD* pstPworld;
	MapObj *pstMap = pstEnv->pstMapObj;

	pstPworld	=	z_find_pworld(pstMap, &pstPworldInst->stPworldIdx);
	if( !pstPworld )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_in_destroy_pworld_inst: Pworld %d defid %d not find", 
					pstPworldInst->iPworldInstID, pstPworldInst->stPworldIdx.iID);
		return -1;
	}

	if (pstPworld->stPworldDef.StrongMapID > 0)
	{
		if (strong_is_normal( pstPworld))
		{
			strong_del_pworld_info(pstEnv,pstPworld->stPworldDef.StrongMapID);
		}
		else
		{
			
		}
		
	}

	if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_END_MAP))
	{
		char szName[128];
			
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = NULL;
		stSEnv.pstPworldInst = pstPworldInst;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = NULL;

		snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworld->stPworldDef.PworldID);
		szName[sizeof(szName) - 1] = 0;
		
		z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_END);
	}

	/*由于广播副本的原因，角色要提前处理*/
	for (i=pstPworldInst->iPlayer-1; i>=0; i--)
	{
		pstPlayer = z_id_player(pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstPlayer)
		{
			if ((pstPlayer->stOnline.State & CS_STAT_DEAD) &&
				is_die_exit_pworld_revive(pstPworld))
			{	
				player_revive_svr(pstAppCtx, pstEnv, pstPlayer);
			}
			
			player_pworld_exit_chgmap(pstEnv, pstPlayer);
		}
	}

	/* release the resouce used(monster, npc, item). */
	while(pstPworldInst->iMapInst>0)
	{
		z_in_destroy_map_inst(pstAppCtx, pstEnv, pstPworld, pstPworldInst, pstPworldInst->astMapInstIdx + pstPworldInst->iMapInst - 1);
		pstPworldInst->iMapInst--;
	}

	if (PWORLD_TYPE_TEAM == pstPworldInst->iType)
	{
		ZoneTeam *pstZoneTeam = NULL;
		TMEMBLOCK *pstMemBlock;

		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstPworldInst->stPworldTypeData.stTeam.iTeamIdx);
		if (pstMemBlock->fValid)
		{
			pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;
			if (pstZoneTeam->stTeamInfo.TeamID == pstPworldInst->stPworldTypeData.stTeam.ullTeamID)
			{
				z_team_pworld_destroy(pstEnv, pstZoneTeam, pstPworld, pstPworldInst);
			}
		}
		if( pstZoneTeam != NULL)
		{
			team_del_pworld_inst(pstEnv, pstZoneTeam, pstPworldInst->stPworldIdx.iID);
		}

		// 幻境副本销毁删除链表
		if (pstPworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
		{
			unreal_info_delete( pstEnv,pstPworldInst->stUnreal.iCurrLayer,pstPworldInst->iPworldInstID, 1);
		}
	}
	else if (PWORLD_TYPE_SINGLE == pstPworldInst->iType)
	{
		pstPlayer = z_id_player(pstAppCtx, pstEnv, pstPworldInst->stPworldTypeData.stSingle.iID);
		if (pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus && 
			pstPlayer->stRoleData.RoleID == pstPworldInst->stPworldTypeData.stSingle.ullRoleID)
		{
			z_player_pworld_destroy(pstEnv, pstPlayer, pstPworld, pstPworldInst);
		}
	}
	else if (PWORLD_TYPE_PLOY== pstPworldInst->iType)	
	{
		int iPworldInstID;
		ZONEPWORLDINST *pstTmpPworldInst;

		// 战场没办法处理等待队列 ,放在tick里面
		if (	pstPworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT ||
			pstPworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT ||
			is_normal_battle_pworld(pstPworld))
		{
			z_clear_pworld_wait_queue(pstEnv, pstPworldInst->iWaitQueueIdx, pstPworldInst->iPworldInstID);
		}

		if(pstPworld->stPworldDef.PolyType == PWORLD_POLY_SHIKONG)
		{
			shikong_clear_wait_queue(pstEnv,pstPworldInst->iShiKongWaitQueueIdx,pstPworldInst->iPworldInstID);
		}
		
		iPworldInstID = pstPworld->iPworldInstHead;
		while(iPworldInstID>0)
		{
			if (iPworldInstID == pstPworldInst->iPworldInstID)
			{
				break;
			}

			pstTmpPworldInst = z_find_pworld_inst(pstMap, iPworldInstID);
			if(pstTmpPworldInst == NULL)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "PLOY Pworld %d error", iPworldInstID);
				break;
			}

			iPworldInstID = pstTmpPworldInst->iNextPworldInstID;
			if (pstTmpPworldInst->iNextPworldInstID == pstPworldInst->iPworldInstID)
			{
				pstTmpPworldInst->iNextPworldInstID = pstPworldInst->iNextPworldInstID;
			}
		}

		if (pstPworld->iPworldInstHead == pstPworldInst->iPworldInstID)
		{
			pstPworld->iPworldInstHead = pstPworldInst->iNextPworldInstID;
		}

		if (pstPworld->iPworldInstNum >0)
		{
			pstPworld->iPworldInstNum--;
		}
	}
	else if(PWORLD_TYPE_CLAN== pstPworldInst->iType)
	{
		ZoneClan *pstClan = z_id_clan(pstEnv, pstPworldInst->stPworldTypeData.stClan.iID);
		if(pstClan && pstClan->stZoneClanInfo.ullEveryDayPworldWID == pstPworldInst->ullPworldWID)
		{
			pstClan->stZoneClanInfo.ullEveryDayPworldWID = 0;
			pstClan->stZoneClanInfo.iEveryDayPworldInstID = 0;
			pstClan->stZoneClanInfo.iEveryDayPworldVal = 0;

			clan_everyday_pworld_close_ui(pstEnv, pstClan);

			player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
					ZONEERR_CLANBROAD266);
		}

		if(pstClan && pstClan->stZoneClanInfo.ullMachinePworldWID == pstPworldInst->ullPworldWID)
		{
			pstClan->stZoneClanInfo.ullMachinePworldWID = 0;
			pstClan->stZoneClanInfo.iMachinePworldInstID = 0;
			pstClan->stZoneClanInfo.iMachinePworldVal = 0;

			clan_active_ui_info_clan_machine_pworld(pstEnv, pstClan,ACTIVE_GAME_SVR_TYPE_UI_CLOSE, 
													pstPworldInst, 0);
		}
	}
	else
	{

	}

	if (pstPworldInst->iPlayer > 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pworld %d inst %d Player Num=%d after destroy", pstPworldInst->stPworldIdx.iID,
					pstPworldInst->iPworldInstID, pstPworldInst->iPlayer);
	}

	z_pworld_time_oplog(pstEnv, pstPworld->stPworldDef.PworldID,pstEnv->pstAppCtx->stCurr.tv_sec-pstPworldInst->tStart);
	
	tmempool_free(pstEnv->pstPworldInstPool, pstPworldInst->iRealIndex);

	return 0;
}

ZONEPWORLDINST* z_in_new_pworld_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, PWORLDSELECT *pstPworldSelect, int iPlayerMeanLevel, int iTimeout)
{
	int i;
	int iIndex;
	int iRealIndex;
	int iPworldInstID;
	ZONEPWORLDINST* pstPworldInst;
	MONDYNATTRDEF *pstMonDynAttr = NULL;
	TMEMBLOCK *pstMemBlock;
	MapObj *pstMap = pstEnv->pstMapObj;
	int fFail=0;

	/* check if the res is enough. 
	    todo: hold res*/
	if( pstEnv->pstMapInstPool->iUsed + pstPworld->iMap > MAX_MAP_INST ||
		pstEnv->pstItemPool->iUsed + MIN_PWORLD_ITEM > MAX_ITEM ||
		pstEnv->pstMonsterPool->iUsed + pstPworld->stPworldDef.MonNum > MAX_MONSTER ||
		pstEnv->pstNpcPool->iUsed + pstPworld->stPworldDef.NPCNum > MAX_NPC ||
		pstEnv->pstAreaDynPool->iUsed + 64 > MAX_AREA_DYN)
	{
		char szBuff[256];
		
		tlog_error(pstEnv->pstLogCat, 0, 0, "new %d pworld limit:map inst used=%d, item used=%d, monster used=%d, npc used=%d, Area used=%d",
					pstPworld->stPworldDef.PworldID, pstEnv->pstMapInstPool->iUsed, pstEnv->pstItemPool->iUsed,
					pstEnv->pstMonsterPool->iUsed, pstEnv->pstNpcPool->iUsed, pstEnv->pstAreaDynPool->iUsed);	
		
		if(!(pstEnv->iWarningFlag & ENV_NEW_PWORLD_WARNING))
		{
			pstEnv->iWarningFlag |= ENV_NEW_PWORLD_WARNING;
			snprintf(szBuff, CCH(szBuff), "new %d pworld limit:map inst used=%d, item used=%d, monster used=%d, npc used=%d, Area used=%d", 
					pstPworld->stPworldDef.PworldID, pstEnv->pstMapInstPool->iUsed, pstEnv->pstItemPool->iUsed,
					pstEnv->pstMonsterPool->iUsed, pstEnv->pstNpcPool->iUsed, pstEnv->pstAreaDynPool->iUsed);	
			z_game_notify(pstEnv, szBuff, CCH(szBuff));
		}
		
		return NULL;
	}

	//副本创建数量限制
	if (pstPworld->stPworldDef.PworldNum && 
		pstPworld->iPworldInstNum >= pstPworld->stPworldDef.PworldNum)
	{
		return NULL;
	}

	//动态等级取封印等级
	if (2 == pstPworld->stPworldDef.DynType)
	{
		iPlayerMeanLevel = get_curr_max_level(pstEnv);
	}

	if (pstPworld->stPworldDef.DynType)
	{
		pstMonDynAttr = z_find_mondyn(pstEnv, iPlayerMeanLevel);
		if (NULL == pstMonDynAttr)
		{
			return NULL;
		}
	}

	iIndex = tmempool_alloc(pstEnv->pstPworldInstPool);
	if (0 > iIndex)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_alloc %d pworld inst fail", pstPworld->stPworldDef.PworldID);
		return NULL;
	}

	iRealIndex = iIndex;
	iIndex = iIndex % pstEnv->pstPworldInstPool->iMax;
	pstMemBlock = (TMEMBLOCK *)TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPworldInstPool, iIndex);
	pstPworldInst	=	(ZONEPWORLDINST*)pstMemBlock->szData;
	memset(pstPworldInst, 0, sizeof(*pstPworldInst));

	pstPworldInst->iRealIndex = iRealIndex;
	pstPworldInst->iType = -1;
	pstPworldInst->ullPworldWID = z_get_gid(pstAppCtx, pstEnv);
	pstPworldInst->bPlayerMeanLevel = iPlayerMeanLevel;
	pstPworldInst->DynType = pstPworld->stPworldDef.DynType;
	pstPworldInst->stUnreal.iCurrLayer = pstPworldSelect->Layer;
	memcpy(&pstPworldInst->stPworldselect, pstPworldSelect, sizeof(pstPworldInst->stPworldselect));
	
	if (pstMonDynAttr)
	{	
		pstPworldInst->TmpLevel = pstMonDynAttr->TmpLevel;
	}

	pstMap->iLastID++;
	if( pstMap->iLastID<=0 )
	{
		pstMap->iLastID	=	1;
	}
	iPworldInstID	=	((pstMap->iLastID % MAX_PWORLD_INSTID)<<16) | (iIndex+1);

	
	pstPworldInst->iPworldInstID	=	iPworldInstID;
    	pstPworldInst->iPlayer = 0;

	pstPworldInst->stPworldIdx.iID	=	pstPworld->stPworldDef.PworldID;
	pstPworldInst->stPworldIdx.iPos	=	(long int)((char *)pstPworld - (char *)pstMap->sPworld)/PWORLD_DATA;

	pstPworldInst->tStart = pstAppCtx->stCurr.tv_sec;
	if (iTimeout > 0)
	{
		pstPworldInst->tEnd =	pstAppCtx->stCurr.tv_sec + iTimeout;
	}
	else
	{
		pstPworldInst->tEnd =	0;
	}

	if (pstPworld->stPworldDef.Type == PWORLD_TYPE_HOME)
	{
		pstPworldInst->iType = PWORLD_TYPE_HOME;
	}

	// 创建幻境副本实例时更新链表
	if (pstPworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
	{
		unreal_info_up(pstEnv, pstPworldInst, pstPworldInst->stUnreal.iCurrLayer,UNREAL_ENTER_NPC);
		unreal_cacl_down_time(pstEnv, pstPworldInst);
	}

	if (	pstPworld->stPworldDef.Type == PWORLD_TYPE_PLOY 	&& 
		pstPworld->stPworldDef.CampNum > 1)	
	{
		if(	pstPworld->stPworldDef.CampNum > MAX_CAMP_NUM)
		{
			z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			return NULL;
		}
	}
	pstPworldInst->stPworldCamp.iCampNum = pstPworld->stPworldDef.CampNum;

	pstPworldInst->iWaitQueueIdx = -1;
	if(	pstPworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT||
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT ||
		is_normal_battle_pworld(pstPworld))
	{
		int iQueueIdx;
		WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;

		if ( pstPworld->stPworldDef.CampNum < 1)
		{
			//todo:log
			z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			return NULL;
		}
		
		//找个队列
		iQueueIdx = z_get_free_wait_queue_idx(pstEnv);
		if(0 > iQueueIdx )
		{
			z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			return NULL;
		}
		memset(&pstPworldQueue->stWaitQueue[iQueueIdx], 0, sizeof(pstPworldQueue->stWaitQueue[iQueueIdx]));
		
		pstPworldQueue->stWaitQueue[iQueueIdx].iPworldInstID = pstPworldInst->iPworldInstID;
		pstPworldQueue->stWaitQueue[iQueueIdx].iQueuerNum = 0;
		pstPworldQueue->stWaitQueue[iQueueIdx].iQueuerType = pstPworld->stPworldDef.ApplyUnitType;
		pstPworldQueue->stWaitQueue[iQueueIdx].stPworldCamp.iCampNum = pstPworld->stPworldDef.CampNum;
		pstPworldQueue->stWaitQueue[iQueueIdx].iPworldID = pstPworld->stPworldDef.PworldID;

		pstPworldInst->iWaitQueueIdx = iQueueIdx;
	}

	pstPworldInst->iShiKongWaitQueueIdx = -1; 
	if(	pstPworld->stPworldDef.PolyType == PWORLD_POLY_SHIKONG)
	{
		//建立起等待队列与时空副本一一对应的关系
		int iShiKongWaitQueueIdx = 0;
		ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
		
		iShiKongWaitQueueIdx = shikong_get_wait_queue_free_idx(pstEnv);
		if(iShiKongWaitQueueIdx < 0)
		{
			z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			return NULL;
		}

		memset(&pstShiKongQueue->astWaitQueue[iShiKongWaitQueueIdx],0,sizeof(pstShiKongQueue->astWaitQueue[iShiKongWaitQueueIdx]));

		pstShiKongQueue->astWaitQueue[iShiKongWaitQueueIdx].iMemNum = 0;
		pstShiKongQueue->astWaitQueue[iShiKongWaitQueueIdx].iPworldID = pstPworld->stPworldDef.PworldID;
		pstShiKongQueue->astWaitQueue[iShiKongWaitQueueIdx].iPworldInstID = pstPworldInst->iPworldInstID;
		pstShiKongQueue->astWaitQueue[iShiKongWaitQueueIdx].tStarTime = pstAppCtx->stCurr.tv_sec;

		pstPworldInst->iShiKongWaitQueueIdx = iShiKongWaitQueueIdx;
	}

	pstPworldInst->iMapInst	=	0;

	if (pstPworld->stPworldDef.PworldID != UNREAL_PWORLD_ID)
	{
		for(i=0; i<pstPworld->iMap; i++)
		{
			if( z_in_new_map_inst(pstAppCtx, pstEnv, pstPworldInst->iPworldInstID, 
									pstPworldSelect, pstPworld->astMapIdx+i)<0 )
			{
				fFail	=	1;
				break;
			}
		}
	}
	else
	{
		if (unreal_new_map_inst(pstEnv, pstPworldSelect,  pstPworldInst->iPworldInstID) < 0)
		{
			return NULL;
		}	
	}
	

	if( fFail )
	{
		/* do rollback operation. */
		z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
		return NULL;
	}
	else
	{
		z_check_pworld_script(pstEnv, pstPworldInst, pstPworld);

		if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_START_MAP))
		{
			pstPworldInst->bDelayScript |= PWORLD_SCRIPT_PROC_START_MAP;
		}

		if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_REAL_START_MAP))
		{
			pstPworldInst->bDelayScript |= PWORLD_SCRIPT_PROC_REAL_START_MAP;
		}

		tlog_debug(pstEnv->pstLogCat, 0, 0, "new pworld inst %d defid %d", pstPworldInst->iPworldInstID, 
						pstPworldInst->stPworldIdx.iID);
		
		return pstPworldInst;
	}
}

int player_find_notenteragain(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstZonePworldInst)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ZONEPWORLD *pstPworld;
	ZONEPWORLDINST *pstTmpPworldInst;
	
	pstPworld	=	z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if( pstPworld == NULL )
	{
		return -1;
	}

	if (pstPworld->stPworldDef.PlayerNum)
	{
		int iPworldInstID; 
		iPworldInstID = pstPworld->iPworldInstHead;
		while(iPworldInstID > 0)
		{
			pstTmpPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, iPworldInstID);
			if(pstTmpPworldInst == NULL)
			{
				break;
			}

			iPworldInstID = pstTmpPworldInst->iNextPworldInstID;
			if (pstTmpPworldInst == pstZonePworldInst)
			{
				continue;
			}

			for (i=0; i<pstMiscInfo->OncePworldNum; i++)
			{
				if (pstMiscInfo->OncePworld[i].PworldWID == pstTmpPworldInst->ullPworldWID)
				{
					return 1;
				}
			}		
		}
	}
	
	for (i=0; i<pstMiscInfo->OncePworldNum; i++)
	{
		if (pstMiscInfo->OncePworld[i].PworldWID == pstZonePworldInst->ullPworldWID)
		{
			return 1;
		}
	}

	return 0;
}

void player_insert_notenterangain(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstZonePworldInst)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if (player_find_notenteragain(pstEnv, pstPlayer, pstZonePworldInst))
	{
		return ;
	}

	if (pstMiscInfo->OncePworldNum < MAX_ONCE_PWORLD_NUM)
	{
		pstMiscInfo->OncePworld[pstMiscInfo->OncePworldNum].PworldWID = pstZonePworldInst->ullPworldWID;
		pstMiscInfo->OncePworldNum++;
	}
	else
	{
		memmove(&pstMiscInfo->OncePworld[0], &pstMiscInfo->OncePworld[1],  
					(MAX_ONCE_PWORLD_NUM - 1) * sizeof(pstMiscInfo->OncePworld[0]));
		pstMiscInfo->OncePworld[MAX_ONCE_PWORLD_NUM - 1].PworldWID = pstZonePworldInst->ullPworldWID;
	}

	return;
}

ZONEPWORLDINST* z_new_single_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD * pstPworld, PWORLDSELECT *pstPworldSelect, int iTimeout)
{
	ZONEPWORLDINST*pstZonePworldInst;
	int iLevel = 0;

	if(pstPworld->stPworldDef.EnterUseVig > 0)
	{
		if(pstPlayer->stRoleData.RoleDetail.GreenJade < (unsigned int)pstPworld->stPworldDef.EnterUseVig)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_MAP129);
			return NULL;
		}

		player_add_vig(pstEnv, pstPlayer, pstPworld->stPworldDef.EnterUseVig, 1, 1);
	}

	iLevel = single_pworld_level_get(pstPlayer, pstPworld);
	
	pstZonePworldInst = z_in_new_pworld_inst(pstAppCtx, pstEnv, pstPworld, pstPworldSelect, iLevel, iTimeout);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pstZonePworldInst->iType = PWORLD_TYPE_SINGLE;
	pstZonePworldInst->stPworldTypeData.stSingle.ullRoleID = pstPlayer->stRoleData.RoleID;
	pstZonePworldInst->stPworldTypeData.stSingle.iID = pstPlayer->iMemID;

	if (pstPworld->stPworldDef.ClanLimit > 0)
	{
		pstZonePworldInst->ullClanGID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
	}
	
	return pstZonePworldInst;
}

void pworld_team_together(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLDINST *pstZonePworldInst, int fCaptain)
{
	pstZonePworldInst->iType = PWORLD_TYPE_TEAM;
	pstZonePworldInst->stPworldTypeData.stTeam.ullTeamID = pstZoneTeam->stTeamInfo.TeamID;
	pstZonePworldInst->stPworldTypeData.stTeam.iTeamIdx = pstZoneTeam->iTeamIdx;

	//pstZoneTeam->iPworldInstID = pstZonePworldInst->iPworldInstID;
	//pstZoneTeam->ullPworldWID = pstZonePworldInst->ullPworldWID;

	team_insert_pworld_inst(pstEnv, pstZoneTeam, pstZonePworldInst, fCaptain);

	return ;
}

void pworld_change_single(ZONEPWORLDINST *pstZonePworldInst, Player *pstPlayer)
{
	pstZonePworldInst->iType = PWORLD_TYPE_SINGLE;
	pstZonePworldInst->stPworldTypeData.stSingle.ullRoleID = pstPlayer->stRoleData.RoleID;
	pstZonePworldInst->stPworldTypeData.stSingle.iID = pstPlayer->iMemID;

	return ;
}

void z_get_team_max_level(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int *piLevelVal)
{
	int i = 0;
	Player *pstPlayer = NULL;
	int iLevel = 0;
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer)
		{	
			if (iLevel < pstPlayer->stRoleData.Level)
				iLevel = pstPlayer->stRoleData.Level;
		}
	}

	*piLevelVal = iLevel;

	return;
}

void z_get_team_min_level(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int *piLevelVal)
{
	int i = 0;
	int iLevel = MAX_LEVEL;
	Player *pstPlayer = NULL;
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer)
		{	
			if (iLevel > pstPlayer->stRoleData.Level)
				iLevel = pstPlayer->stRoleData.Level;
		}
	}
	
	*piLevelVal = iLevel;

	return;
}

void z_get_team_avg_level(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int *piLevelVal)
{
	int i = 0;
	int iLevel = 0;
	int iOnlineNum = 0;
	Player *pstPlayer = NULL;
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer)
		{	
			iLevel += pstPlayer->stRoleData.Level;
			iOnlineNum++;
		}
	}

	if (iOnlineNum > 0)
	{
		iLevel = iLevel/iOnlineNum;
	}

	*piLevelVal = iLevel;

	return;
}

ZONEPWORLDINST* z_new_team_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLD * pstPworld, PWORLDSELECT *pstPwolrdSelect, int iTimeout)
{
	int i;
	ZONEPWORLDINST*pstZonePworldInst;
	Player *pstPlayer;
	int iLevel = 0;
	Player *pstCaptain =player_team_get_captain_player(pstEnv, pstZoneTeam);

	if(pstCaptain)
	{
		if(pstPworld->stPworldDef.EnterUseVig > 0)
		{
			if(pstCaptain->stRoleData.RoleDetail.GreenJade < (unsigned int)pstPworld->stPworldDef.EnterUseVig)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE2, 
					ZONEERR_MAP129);
				z_team_sys_strf(pstEnv, pstZoneTeam, SYS_MIDDLE2, 
					ZONEERR_TEAM306);
				return NULL;
			}

			player_add_vig(pstEnv, pstCaptain, pstPworld->stPworldDef.EnterUseVig, 1, 1);
		}
	}

	iLevel = team_pworld_level_get(pstEnv, pstZoneTeam, pstPworld);
	pstZonePworldInst = z_in_new_pworld_inst(pstAppCtx, pstEnv, pstPworld, pstPwolrdSelect, iLevel, iTimeout);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pworld_team_together(pstEnv, pstZoneTeam, pstZonePworldInst, 0);

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer)
		{
			if (pstPworld->stPworldDef.ClanLimit > 0 && pstZonePworldInst->ullClanGID == 0)
			{
				pstZonePworldInst->ullClanGID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
			}
		}
	}

	return pstZonePworldInst;
}

ZONEPWORLDINST *get_war_poly_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZONEPWORLD * pstPworld)
{
	return NULL;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPworld);	//add by paraunused.pl
}

ZONEPWORLDINST* z_new_ploy_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv,
										ZONEPWORLD * pstPworld, int iTimeout,
										int iLevel )
{
	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;
	ZONEPWORLDINST *pstNextPworldInst;
	int iPworldInstID;
	//iLevel =  pstPworld->stPworldDef.MinLv;

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	if (0 == iLevel && pstPworld->stPworldDef.ApplyUnitType == 1)
	{
		iLevel = get_curr_max_level(pstEnv);
	}

	if (0 == iLevel)
	{
		iLevel = pstPworld->stPworldDef.MinLv;
	}
	
	pstZonePworldInst = z_in_new_pworld_inst(pstAppCtx, pstEnv, pstPworld, &stPworldSelect, iLevel, iTimeout);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pstZonePworldInst->iType = PWORLD_TYPE_PLOY;

	iPworldInstID =  pstPworld->iPworldInstHead;
	if (iPworldInstID)
	{
		pstNextPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, iPworldInstID);
		if (NULL == pstNextPworldInst)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "pworld %d error", iPworldInstID);
		}
		else
		{
			pstZonePworldInst->iNextPworldInstID = pstNextPworldInst->iPworldInstID;
		}
	}
	
	pstPworld->iPworldInstHead = pstZonePworldInst->iPworldInstID;
	pstPworld->iPworldInstNum++;
	
	return pstZonePworldInst;
}

int z_check_pworld_enter_num(ZONESVRENV * pstEnv, Player *pstPlayer, PWORLDDEF *pstPworldDef, int iNotify)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	int iUseItem = pstPlayer->stOnline.stBctOnline.iUseItemEnterPworld;
	int iWeekCheck = 1;
	
	assert(iUseItem==0 || iUseItem == 1);
	
	if (0 < pstPworldDef->NumEveryDay ||
		0 < pstPworldDef->ItemEnterNum ||
		0 < pstPworldDef->NumEveryWeek)
	{		
		//检查每日副本次数
		for (i=pstMiscInfo->LimitPworldNum-1; i>=0; i--)
		{
			if(pstPworldDef->NumEveryWeek > 0)
			{
				if (0 == IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec,pstMiscInfo->LimitPworld[i].WeekTime))
				{
					iWeekCheck = 0;
				}
				else
				{
					iWeekCheck = 1;
				}
			}
			else
			{
				iWeekCheck = 0;
			}
			if (0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time) &&
				0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime) &&
				0 == iWeekCheck)
			{
				if (0 == pstPworldDef->NumEveryWeek)
				{
					pstMiscInfo->LimitPworldNum--;
					if (i != pstMiscInfo->LimitPworldNum)
					{
						memmove(&pstMiscInfo->LimitPworld[i], &pstMiscInfo->LimitPworld[i+1],  (pstMiscInfo->LimitPworldNum - i) * sizeof(pstMiscInfo->LimitPworld[0]));
					}
				}
				
				
				continue;
			}
			
			if (pstMiscInfo->LimitPworld[i].PworldID == pstPworldDef->PworldID)
			{
				int iVip = 0;

				if (pstPworldDef->PworldID == 300) //大地之心
				{
					RESULT *pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_DADI);
					if (pstVipResult)
					{
						iVip = pstVipResult->ResultVal1;
					}
				}
				
				if (	0 < pstPworldDef->NumEveryDay &&
					pstMiscInfo->LimitPworld[i].Num >= (pstPworldDef->NumEveryDay+iVip) &&
					!iUseItem)
				{
					if(iNotify)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP130,pstPworldDef->Name);
					}
					return -1;
				}

				if (	0 < pstPworldDef->ItemEnterNum &&
					pstMiscInfo->LimitPworld[i].ItemNum >= pstPworldDef->ItemEnterNum &&
					iUseItem)
				{
					if(iNotify)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP130,pstPworldDef->Name);
					}
					return -1;
				}

				if (pstPworldDef->NumEveryWeek > 0 &&
					pstMiscInfo->LimitPworld[i].WeekNum >= pstPworldDef->NumEveryWeek)
				{
					if(iNotify)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP131,pstPworldDef->Name);
					}
					return -2;					
				}
			}
		}
	}

	return 0;
}

int z_check_single_pworld(ZONESVRENV * pstEnv, Player *pstPlayer, PWORLDDEF *pstPworldDef, tdr_ulonglong ullPworldWid, CSDIFFPWORLDTEAMTIPS *pstTeamTips)
{
	int iNotify = 1;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	Player *pstCaptain = team_get_captain_by_player(pstEnv,pstPlayer);
	int iRet = 0;

	if(pstTeamTips)
		iNotify = 0;
	
	if (0 > player_limit_time_pworld_check(pstEnv, pstPlayer, pstPworldDef, ullPworldWid))
	{
		int iMinute;

		iMinute = pstPworldDef->RedoLimitTime / 60;
		if(iMinute <= 0)
		{
			iMinute = 1;
		}

		if(pstTeamTips)
		{
			diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
				ZONEERR_DIFF302, iMinute);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_MAP132, iMinute);
		}

		if(pstCaptain && pstCaptain != pstPlayer)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE2, 
				ZONEERR_MAP133, pstPlayer->stRoleData.RoleName,iMinute);
		}
		
		return -4;
	}

	// 思过厅中的黑名玩家不让组队进副本
	if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
		player_is_zuolao(pstPlayer))
	{
		if(pstTeamTips)
		{
			diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
				ZONEERR_DIFF303);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_MAP134);
		}

		if(pstCaptain && pstCaptain != pstPlayer)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstCaptain, SYS_MIDDLE2, 
				ZONEERR_MAP135, pstPlayer->stRoleData.RoleName);
		}
		return -1;
	}

	//每日进入次数
	iRet = z_check_pworld_enter_num(pstEnv, pstPlayer, pstPworldDef, iNotify);
	if(0 > iRet)
	{
		if(pstTeamTips)
		{
			if ( -1 == iRet)
			{
				diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
					ZONEERR_DIFF304);
			}
			else
			{
				diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
					ZONEERR_DIFF305);
			}
		}

		if(pstCaptain && pstCaptain != pstPlayer)
		{
			if ( -1 == iRet)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE2, 
					ZONEERR_MAP136, pstPlayer->stRoleData.RoleName);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE2, 
					ZONEERR_MAP137, pstPlayer->stRoleData.RoleName);
			}
		}
		
		return -3;
	}

	if (pstPworldDef->ClanLimit > 0 )
	{
		if (pstClanInfo->ClanId.GID == 0)
		{
			if(pstTeamTips)
			{
				diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
					ZONEERR_DIFF306, pstPlayer->stRoleData.RoleName);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP101);
			}

			if(pstCaptain && pstCaptain != pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE2, 
					ZONEERR_MAP102, pstPlayer->stRoleData.RoleName);
			}
			
			return -1;
		}

		// 有工会要求 才能有贡献度要求
		if (pstPworldDef->ContriLimit > 0 && pstClanInfo->Contribute < pstPworldDef->ContriLimit)
		{
			if(pstTeamTips)
			{
				diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
					ZONEERR_DIFF307);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP103);
			}

			if(pstCaptain && pstCaptain != pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE2, 
					ZONEERR_MAP104, pstPlayer->stRoleData.RoleName);
			}
			return -1;
		}
	}

	if (pstPworldDef->MinLv > 0 && pstPlayer->stRoleData.Level < pstPworldDef->MinLv)
	{
		if(pstTeamTips)
		{
			diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
				ZONEERR_DIFF308,
				pstPworldDef->MinLv, pstPworldDef->MaxLv);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_MAP138, 
				pstPworldDef->MinLv, pstPworldDef->MaxLv);
		}

		if(pstCaptain && pstCaptain != pstPlayer)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstCaptain, SYS_MIDDLE2, 
				ZONEERR_MAP139, 
				pstPlayer->stRoleData.RoleName,pstPworldDef->MinLv, pstPworldDef->MaxLv);
		}
		
		return -2;
	}

	if (pstPworldDef->MaxLv > 0 && pstPlayer->stRoleData.Level > pstPworldDef->MaxLv)
	{
		if(pstTeamTips)
		{
			diff_pworld_team_tips_add(pstTeamTips, pstPlayer->stRoleData.RoleName, 
				ZONEERR_DIFF309,
				pstPworldDef->MinLv, pstPworldDef->MaxLv);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_MAP140, 
					pstPworldDef->MinLv, pstPworldDef->MaxLv);
		}
		if(pstCaptain && pstCaptain != pstPlayer)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstCaptain, SYS_MIDDLE2, 
					ZONEERR_MAP141, 
					pstPlayer->stRoleData.RoleName,pstPworldDef->MinLv, pstPworldDef->MaxLv);
		}
		
		return -2;
	}

	if (poly_is_mzt_pworld(pstPworldDef->PworldID))
	{
		if (pstPlayer->stRoleData.MiscInfo.PworldWID != ullPworldWid &&
			pstPlayer->stRoleData.MiscInfo.MztInfo.Layer >= poly_mzt_get_layer(pstPworldDef->PworldID))
		{
			return -1;
		}
	}

	return 0;
}

int z_check_team_pworld(ZONESVRENV * pstEnv, Player *pstCaptain, ZoneTeam *pstZoneTeam, PWORLDDEF *pstPworldDef, int iLevel)
{
	int i;
	int iRet;
	Player *pstPlayer;
	//CLANINFO *pstClanInfo = &pstCaptain->stRoleData.MiscInfo.ClanInfo;
	CLANINFO *pstMemberClanInfo;
	int iTeamFlag;
	
	iTeamFlag = 0;
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstPlayer)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE, ZONEERR_MAP142, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			iTeamFlag = 1;
			continue;
		}

		iRet = z_check_single_pworld(pstEnv, pstPlayer, pstPworldDef, 0, NULL);
		if (iRet < 0)
		{
			if (pstPlayer != pstCaptain)
			{
				if (iRet == -2)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE, 
						ZONEERR_MAP143, 
						pstPworldDef->MinLv, pstPworldDef->MaxLv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
				}
				else if(iRet == -3)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE, 
						ZONEERR_MAP144, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName,pstPworldDef->Name);
				}
			}

			iTeamFlag = 1;
			continue;
		}
		
		pstMemberClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
		if (pstPworldDef->ClanLimit > 0)
		{
			if (pstMemberClanInfo->ClanId.GID == 0 || 
				pstMemberClanInfo->ClanId.GID != pstCaptain->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstCaptain, SYS_MIDDLE, 
					ZONEERR_MAP145, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
				iTeamFlag = 1;
				continue;
			}

			if (pstPworldDef->ContriLimit > 0 &&pstMemberClanInfo->Contribute < pstPworldDef->ContriLimit)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstCaptain, SYS_MIDDLE,  
					ZONEERR_MAP146, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
				iTeamFlag = 1;
				continue;
			}
		}

		if (pstPlayer != pstCaptain)
		{
			if (pstPlayer->stRoleData.Map != pstCaptain->stRoleData.Map)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstCaptain, SYS_MIDDLE,  
					ZONEERR_MAP147, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
				iTeamFlag = 1;
				continue;
			}

			if (0 == z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstCaptain->stRoleData.Pos))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCaptain, SYS_MIDDLE,  
					ZONEERR_MAP148, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
				iTeamFlag = 1;
				continue;
			}

			iRet = team_trans_check_one(pstEnv, pstPlayer);
			if (iRet < 0)
			{
				team_trans_check_tip(pstEnv, pstCaptain, 0, iRet,pstPlayer);
				iTeamFlag = 1;
				continue;	
			}
		}
	}

	if(iTeamFlag)
	{
		return -1;
	}
	
	return 0;
	UNUSED(&iLevel);
}

ZONEPWORLDINST *z_find_group_pworld_inst(ZONESVRENV * pstEnv, ZONEPWORLD *pstZonePworld, int *piRet)
{
	int iPworldInstID = 0, iOtherInstID = 0;
	int iPworldPlayerNum = MAX_PWORLD_MEMBER;
	ZONEPWORLDINST *psPworldInst = NULL, *pstNextPworldInst = NULL;

	*piRet = 0;
	if(pstZonePworld->iPworldInstHead == 0)
	{
		return NULL;
	}

	//地图人数限制
	if (pstZonePworld->stPworldDef.PlayerNum != 0 && pstZonePworld->stPworldDef.PlayerNum < iPworldPlayerNum)
	{
		iPworldPlayerNum = pstZonePworld->stPworldDef.PlayerNum;
	}

	iOtherInstID = 0;
	iPworldInstID = pstZonePworld->iPworldInstHead;
	while(iPworldInstID > 0)
	{
		pstNextPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, iPworldInstID);
		if (NULL == pstNextPworldInst)
		{
			break;
		}

		*piRet = -1; 
		
		if (pstNextPworldInst->iPlayer == 0 && iOtherInstID == 0)
		{
			iOtherInstID = iPworldInstID;
		}

		if (pstNextPworldInst->iPlayer < iPworldPlayerNum && pstNextPworldInst->iPlayer != 0)
		{
			iPworldPlayerNum = pstNextPworldInst->iPlayer;
			psPworldInst = pstNextPworldInst;
		}
		
		iPworldInstID = pstNextPworldInst->iNextPworldInstID;
	}

	if (psPworldInst == NULL && iOtherInstID)
	{
		psPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, iOtherInstID);
		if (NULL == psPworldInst)
		{
			return NULL;
		}
	}
	
	return psPworldInst;
}

//获得根自己有关联的副本实例
ZONEPWORLDINST *z_find_player_pworld_inst(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstZoneTeam;
	ZONEPWORLDINST *pstZonePworldInst;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if (0 == pstMiscInfo->PworldWID)
	{
		return NULL;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMiscInfo->PworldInstID);
	if (NULL == pstZonePworldInst || 
		pstZonePworldInst->ullPworldWID != pstMiscInfo->PworldWID)
	{
		return NULL;
	}

	if(pstZonePworldInst->iType == PWORLD_TYPE_SINGLE )
	{
		if(pstZonePworldInst->stPworldTypeData.stSingle.ullRoleID != pstPlayer->stRoleData.RoleID)
		{
			return NULL;
		}
	}
	else if(pstZonePworldInst->iType == PWORLD_TYPE_TEAM)
	{
		pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if(NULL == pstZoneTeam)
		{
			return NULL;
		}

		if(pstZonePworldInst->ullPworldWID != pstZoneTeam->ullPworldWID)
		{
			return NULL;
		}
	}

	return pstZonePworldInst;
}

ZONEPWORLDINST *z_find_pworld_inst_bysingle(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if (0 == pstMiscInfo->PworldWID)
	{
		return NULL;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMiscInfo->PworldInstID);
	if (NULL == pstZonePworldInst || 
		pstZonePworldInst->ullPworldWID != pstMiscInfo->PworldWID)
	{
		return NULL;
	}

	//不是单人副本，或不是属于自己的单人副本
	if(pstZonePworldInst->iType != PWORLD_TYPE_SINGLE || 
		pstZonePworldInst->stPworldTypeData.stSingle.ullRoleID != pstPlayer->stRoleData.RoleID)
	{
		return NULL;
	}

	return pstZonePworldInst;
}

int player_login_single_pworld_update(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstPworldInst = z_find_pworld_inst_bysingle(pstEnv, pstPlayer);

	if(pstPworldInst && pstPworldInst->stPworldTypeData.stSingle.iID != pstPlayer->iMemID)
	{
		pstPworldInst->stPworldTypeData.stSingle.iID = pstPlayer->iMemID;
	}
	
	return 0;
}

ZONEPWORLDINST *z_find_id_pworld_inst_byteam(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int iPworldID)
{
	int i;
	ZONEPWORLDINST *pstZonePworldInst;

	for(i=0; i<pstZoneTeam->iPworldNum; i++)
	{
		pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneTeam->stPworldInfo[i].iPworldInstID);
		if (pstZonePworldInst && 
			pstZonePworldInst->ullPworldWID == pstZoneTeam->stPworldInfo[i].ullPworldWID &&
			iPworldID == pstZonePworldInst->stPworldIdx.iID)
		{
			return pstZonePworldInst;
		}
	}
	
	return NULL;
}

static int z_player_save_campfy_info(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	ROLEFAIRY *pstRoleFairy;
	
	if (pstPlayer->stOnline.stFairyOnline.ullCampWID > 0)
	{
		pstRoleFairy = get_pack_fairy(pstPlayer, pstPlayer->stOnline.stFairyOnline.ullCampWID);
		if (pstRoleFairy)
		{
			pstPlayer->stRoleData.MiscInfo.CampFyXP = pstPlayer->stRoleData.RoleDetail.XP;
			pstPlayer->stRoleData.MiscInfo.CampFyID = pstRoleFairy->FairyID;

			 fairy_back_in(pstEnv, pstPlayer, pstRoleFairy);
			 //z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "本场景禁止使用精灵,退出场景将自动还原");
		}
	}
	else
	{
		 //z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "本场景禁止使用精灵");
		 return 0;
	}

	if (pstPlayer->stOnline.cMiscFlag &MISC_FLAG_WUSHUANG)
	{
		z_player_del_wushuang_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_WUSHUANG;
	}
	
	return 0;
}


static int z_player_enter_pworld_bct_info(ZONESVRENV * pstEnv, Player *pstPlayer,ZONEPWORLDINST *pstZonePworldInst,
								int *pIMapIdx, RESPOS *pstPos)
{
	int i;
	BCTDEF *pstBctDef;
	
	if (pstPlayer->stOnline.stBctOnline.iStartLayer <= 0)
		return -1;

	pstBctDef = z_find_bctdef(pstEnv,pstPlayer->stOnline.stBctOnline.iStartLayer);
	if (!pstBctDef)
		return -1;
	
	pstPlayer->stRoleData.MiscInfo.BCTInfo.StartLayer = pstPlayer->stOnline.stBctOnline.iStartLayer;
	pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer = pstPlayer->stOnline.stBctOnline.iStartLayer;
	

	pstPos->X = pstBctDef->EnterX;
	pstPos->Y = pstBctDef->EnterY;

	for(i=0; i<pstZonePworldInst->iMapInst; i++)
	{
		if (pstBctDef->MapID == GET_MAPID(pstZonePworldInst->astMapInstIdx[i].iID) )
		{
			*pIMapIdx = pstZonePworldInst->astMapInstIdx[i].iID;
			break;
		}
	}

	if (i>=pstZonePworldInst->iMapInst)
		return -1;
	
	return 0;
}

int z_player_enter_pworld_map(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstZonePworldInst, 
									ZONEPWORLD *pstZonePworld)
{
	int iMapIndex;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ZONEPWORLDINST *pstPworldInstOld;
	RESPOS stPos = pstZonePworld->stPworldDef.InitMapPos;

	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstIdx;
//	int i;

	//多人骑乘，主人进入副本
	if((pstPlayer->stOnline.State & (CS_STAT_RIDE_PREPARE|CS_STAT_RIDE_ON |CS_STAT_FLY)) && 
		pstPlayer->stOnline.stRideOnInfo.iPassengerNum > 0)
	{
		switch(pstZonePworld->stPworldDef.Type)
		{
			//不让进
			case PWORLD_TYPE_PLOY:
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP105);
				return -1;
				break;
			//下马在进
			case PWORLD_TYPE_TEAM:
			case PWORLD_TYPE_ALL:
				z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				break;
			default:
				z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				break;
		}
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		Monster *pstMon = machine_get_by_player(pstEnv, pstPlayer);
		if(pstMon)
		{
			player_ride_machine_brk(pstEnv, pstPlayer, 1);
		}
	}
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !(pstPlayer->stOnline.State & CS_STAT_AUTOFLY))
	{
		pstPlayer->stRoleData.ExitMap = pstPlayer->stRoleData.Map;
		pstPlayer->stRoleData.ExitPos_x = pstPlayer->stRoleData.Pos.X;
		pstPlayer->stRoleData.ExitPos_y = pstPlayer->stRoleData.Pos.Y;
		player_exit_info_check(pstEnv, pstPlayer);
	}

	// 清除老副本
	if (pstMiscInfo->PworldWID != 0 && pstMiscInfo->PworldWID != pstZonePworldInst->ullPworldWID)
	{
		pstPworldInstOld = z_find_player_pworld_inst(pstEnv, pstPlayer);
		if (pstPworldInstOld) 
		{
			if (PWORLD_TYPE_SINGLE == pstPworldInstOld->iType)
			{
				z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInstOld);
			}
			else if (PWORLD_TYPE_TEAM == pstPworldInstOld->iType)
			{
				if (pstPworldInstOld->iPlayer <= 0)
				{
					ZoneTeam *pstZoneTeam;
					TEAMMEMBER *pstTeamMember;
					
					pstZoneTeam	= player_team_get(pstEnv, pstPlayer);
					if (pstZoneTeam)
					{
						pstTeamMember = player_team_get_captain(pstZoneTeam);
						if (pstTeamMember && 0 == strcmp(pstPlayer->stRoleData.RoleName, pstTeamMember->MemberName))
						{
							z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInstOld);
						}
					}
				}
			}
		}
	}

	if(pstMiscInfo->PworldWID != pstZonePworldInst->ullPworldWID)
	{
		//视为第一次进当前副本
		z_role_enter_pworld_oplog(pstEnv, pstPlayer, pstZonePworldInst->stPworldIdx.iID);
	}

	if (pstZonePworld->stPworldDef.PworldID == CLAN_CITY_PWORLD_ID)
	{
		
		if (pstMiscInfo->PworldWID != pstZonePworldInst->ullPworldWID)
		{
			pstMiscInfo->PworldWIDPre = pstMiscInfo->PworldWID;
		}
	}
	else
	{
		pstMiscInfo->PworldWIDPre = 0;
	}

	if (pstZonePworld->stPworldDef.PworldID == CLAN_MACHINE_PWORLD_ID)
	{
		pstMiscInfo->ShuNiuInfo.Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstMiscInfo->ShuNiuInfo.Wid = pstZonePworldInst->ullPworldWID;
	}

	pstMiscInfo->PworldInstID = pstZonePworldInst->iPworldInstID;
	pstMiscInfo->PworldWID = pstZonePworldInst->ullPworldWID;

	// 进副本请状态
	if (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_CLEAN_STATUS)
	{
		z_status_roledata(pstEnv->pstAppCtx,pstEnv, pstPlayer,1);
		player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if (pstZonePworld->stPworldDef.PworldID  == BCT_PWORLD_ID)
	{
		if (z_player_enter_pworld_bct_info(pstEnv, pstPlayer,pstZonePworldInst,&iMapIndex, &stPos) < 0)
		{
			return -1;
		}
	}
	else if (pstZonePworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
	{
		if (unreal_get_enter_pos(pstEnv,pstZonePworldInst, &iMapIndex, &stPos) < 0)
		{
			return -1;
		}	
	}
	else
	{
		iMapIndex = (pstZonePworldInst->iPworldInstID & 0xffff)<<16 | (pstZonePworld->stPworldDef.InitMapID & 0xffff);
	}
	
	stIdx.iID	=	iMapIndex;
	stIdx.iPos	=	-1;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
		return -1;

	pstIdx	= z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, stIdx.iID);
	if( !pstIdx )
		return -1;

	// 据点副本获取进入点
	if (strong_get_enter_pos( pstEnv, pstZonePworld,pstIdx, &stPos, pstPlayer,0) < 0)
	{
		return -1;
	}
	
	/*if (strong_is_normal(pstZonePworld))
	{
		if (strong_get_map_pos( pstEnv,pstZonePworld->stPworldDef.StrongMapID,pstIdx, &stPos, pstPlayer,0) < 0)
		{
			return -1;
		}
	}*/

	//末世巅峰副本获取红蓝方进入点
	if (pstZonePworld->stPworldDef.Type == PWORLD_TYPE_PLOY && pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG)
	{
		if (dianfeng_get_map_pos( pstEnv,pstIdx, &stPos, pstPlayer) < 0)
		{
			return -1;
		}

		//末世巅峰，要回血回魔
		dianfeng_revive_sth(pstEnv,pstPlayer);

		//末世巅峰。技能CD重置
		player_skill_cool_clean(pstEnv, pstPlayer, 0);
	}

	player_weixing_clear(pstEnv, pstPlayer);

	// 进入点
	if (is_battle_pworld(pstZonePworld)) // 跨服据点放这里了
	{
		memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));
		
		if (strong_is_span(pstZonePworld))
		{
			if (span_strong_enter_init(pstEnv,pstPlayer,pstZonePworld) < 0)
			{
				return -1;
			}
			span_strong_war_one_notify(pstEnv, pstPlayer,pstZonePworld->stPworldDef.StrongMapID);
		}
		else
		{
			if(0 > player_set_battle_info(pstEnv, pstPlayer, pstIdx, pstZonePworld, pstZonePworldInst, &stPos))
			{
				return -1;
			}
			normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstZonePworldInst, 0, 0);
		}
	}

	if(is_battle_sparta_svr(pstEnv))
	{
		player_set_sparta_info(pstEnv, pstPlayer, pstIdx, pstZonePworld, pstZonePworldInst, &stPos);
	}

	player_set_poly_info(pstEnv, pstPlayer, pstIdx, pstZonePworld, pstZonePworldInst, &stPos);

	//只可以进一次
	if (pstZonePworld->stPworldDef.NotEnterAgain)
	{
		player_insert_notenterangain(pstEnv, pstPlayer, pstZonePworldInst);
	}
	
	//每日创建次数限制
	player_insert_limit_pworld(pstEnv, pstPlayer, pstZonePworldInst,0);
	
	//战场次数
	player_insert_limit_battle(pstEnv, pstPlayer, pstZonePworld);

	if (pstZonePworld->stPworldDef.RedoLimitTime)
	{
          	player_insert_limit_time_pworld(pstEnv, pstPlayer, pstZonePworld, pstZonePworldInst);
	}

	if (pstZonePworld->stPworldDef.PlayerInTime > 0)
	{
		pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID = pstZonePworld->stPworldDef.PworldID;
		pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstZonePworld->stPworldDef.PlayerInTime;
	}

	if (poly_is_mzt_pworld(pstZonePworld->stPworldDef.PworldID))
	{
		pstPlayer->stRoleData.MiscInfo.MztInfo.Layer = poly_mzt_get_layer(pstZonePworld->stPworldDef.PworldID);
		pstPlayer->stRoleData.MiscInfo.MztInfo.PreTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	return z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, iMapIndex, &stPos, 
								pstZonePworld->stPworldDef.InitMapDir, pstZonePworld->stPworldDef.InitCameraDir);
}

int z_team_enter_pworld_map(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLDINST *pstZonePworldInst, 
									ZONEPWORLD *pstZonePworld)
{
	int i;
	Player *pstPlayer;

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer)
		{
			z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld);
		}
	}
	
	return 0;
}

int  z_enter_pworld_mon_speech(ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, ZONEPWORLDINST *pstZonePworldInst)
{
	int i, j;
	int iMapIndex;
	int iDyn;
	ZONEMAPINST* pstMapInst;
	AREADYN *pstAreaDyn;
	Monster *pstMon;
	ZONEIDX stIdx;

	iMapIndex = (pstZonePworldInst->iPworldInstID & 0xffff)<<16 | (pstPworld->stPworldDef.InitMapID & 0xffff);
	
	stIdx.iID	=	iMapIndex;
	stIdx.iPos	=	-1;
	
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
		return -1;

	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstMon)
			{
				z_refresh_mon_speech(pstEnv, pstMon);
			}
		}
	}
	
	return 0;
}

int z_player_enter_pworld_by_inst(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD *pstZonePworld, 
										ZONEPWORLDINST *pstZonePworldInst)
{
	//检查只能进入一次的副本
	if (player_find_notenteragain(pstEnv, pstPlayer, pstZonePworldInst))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP106);
		return -1;
	}

	if (pstPlayer->stRoleData.MiscInfo.PworldWID != pstZonePworldInst->ullPworldWID &&
		pstPlayer->stRoleData.MiscInfo.PworldWIDPre != pstZonePworldInst->ullPworldWID)
	{
		if (0 > z_check_single_pworld(pstEnv, pstPlayer, &pstZonePworld->stPworldDef, 
				pstZonePworldInst->ullPworldWID, NULL))
		{
			return -1;
		}
	}

	if (pstZonePworldInst->ullClanGID > 0)
	{
		if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP101);
			return -1;
		}

		// 公会枢纽--联合模式:其他公会的也可以进
		if (!pstZonePworldInst->cShareShuNiu)
		{
			if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID != pstZonePworldInst->ullClanGID)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP107);
				return -1;
			}
		}
	}

	if(pstZonePworld->stPworldDef.PworldID == CLAN_EVERYDAY_PWORLD_ID ||
		pstZonePworld->stPworldDef.PworldID == CLAN_MACHINE_PWORLD_ID)
	{
		CLANMEM *pstMem;
		ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
		if(!pstClan)
		{
			return -1;
		}
		
		pstMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID, NULL);
		if (!pstMem)
		{
			return -1;
		}

		if(IsSameDay(pstMem->JoinTime, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP108);
			return -1;
		}
	}

	//目前副本有人数上限的仅活动副本，在前面已经拦住了
	if (pstZonePworld->stPworldDef.PlayerNum&& 
		pstZonePworldInst->iPlayer >= pstZonePworld->stPworldDef.PlayerNum &&
		pstZonePworldInst->iPlayer >= MAX_PWORLD_MEMBER)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP109);
		return -1;
	}

	//单人进副本
	return z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld);

	return 0;
}

int z_player_enter_pworld(ZONESVRENV * pstEnv, Player *pstPlayer, int iPworldID, 
							PWORLDSELECT *pstPwolrdSelect, int iTrans)
{
	ZONEPWORLD *pstZonePworld = NULL;
	ZONEIDX stIdx;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	TEAMMEMBER *pstTeamMember;
	int iTimeout;
	ZoneTeam *pstZoneTeam =NULL;
	int   iTeamPworldFlag = 0;
	int iPInstID = 0;
	ZoneTeam *pstTeam =NULL;

	//终极熔岩巨兽召唤者不能进入副本
	if(0 < player_if_call_zj_rongyan(pstEnv,pstPlayer))
	{	
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP_702);
		return -1;
	}

	//lzk  融焰怪,	活动中限制玩家进入其他副本
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam && 1 == pstTeam->iRongYanBegin)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP_702);
		return -1;
	}

	//末世巅峰准备地图和决赛副本中不能进入副本
	if (pstPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
		(GET_PWORLDID(pstPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_DIANFENG33);
		return -1;
	}
	
	if (!pstPwolrdSelect)
	{
		return -1;
	}


	if (poly_is_mzt_pworld(iPworldID))
	{
		int iMztID;
		if (!pstTeam)
			return -1;

		if (z_find_id_pworld_inst_byteam(pstEnv, pstTeam, iPworldID))
		{
			stIdx.iID = iPworldID;
		}
		else
		{
			iMztID = ploy_mzt_get_pworldid( pstEnv, pstTeam);
			if (!iMztID)
				return -1;

			stIdx.iID = iMztID;
		}
	}
	else
	{
		stIdx.iID = iPworldID;
	}
	
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0, "player %d enter pworld %d", pstPlayer->stRoleData.Uin, iPworldID);

	iTimeout = pstZonePworld->stPworldDef.Timeout*60.0;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);

	if(pstZonePworld->stPworldDef.Type == PWORLD_TYPE_PLOY)
	{
		int iRet = 0;
		
		pstZonePworldInst = z_find_group_pworld_inst(pstEnv, pstZonePworld, &iRet);
		//严格限制一下,如果活动副本不存在防止创建
		if (pstZonePworldInst == NULL)	
		{
			if(iRet == 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP110);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP109);
			}
			return -1;
		}

		if(pstZonePworld->stPworldDef.ReadyTime+pstZonePworldInst->tStart < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_MACHINE_WAR)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_MAP111);
				return -1;
			}
		}
	}
	else
	{
		//看看队伍有没有创建了这个副本
		if(pstZoneTeam)
		{
			pstZonePworldInst = z_find_id_pworld_inst_byteam(pstEnv, pstZoneTeam, iPworldID);
			if(pstZonePworldInst)
			{
				iTeamPworldFlag = 1;//队伍副本
			}
		}
		else
		{
			pstZonePworldInst = NULL;
		}

		//看自己有没有创建了这个副本
		if (NULL == pstZonePworldInst)
		{
			pstZonePworldInst = z_find_pworld_inst_bysingle(pstEnv, pstPlayer);
			if (pstZonePworldInst && pstZonePworldInst->stPworldIdx.iID != iPworldID )
			{
				pstZonePworldInst = NULL;
			}
		}
	}

	if (NULL == pstZonePworldInst) //创建
	{
		int iTeamCreate;
		
		if ((pstEnv->pstNpcPool->iMax - pstEnv->pstNpcPool->iUsed < 300)  || 
		(pstEnv->pstMonsterPool->iMax - pstEnv->pstMonsterPool->iUsed < 3000) ||
		(pstEnv->pstMapInstPool->iMax - pstEnv->pstMapInstPool->iUsed < 10) ||
		(pstEnv->pstItemPool->iMax - pstEnv->pstItemPool->iUsed < 100) ||
		(pstEnv->pstAreaDynPool->iMax - pstEnv->pstAreaDynPool->iUsed < 640) )
		{
			char szBuff[256];

			if(!(pstEnv->iWarningFlag & ENV_NEW_PWORLD_WARNING))
			{
				pstEnv->iWarningFlag |= ENV_NEW_PWORLD_WARNING;
				snprintf(szBuff, CCH(szBuff), "new %d pworld limit:map inst used=%d, item used=%d, monster used=%d, npc used=%d, Area used=%d", 
						pstZonePworld->stPworldDef.PworldID, pstEnv->pstMapInstPool->iUsed, pstEnv->pstItemPool->iUsed,
						pstEnv->pstMonsterPool->iUsed, pstEnv->pstNpcPool->iUsed, pstEnv->pstAreaDynPool->iUsed);	
				z_game_notify(pstEnv, szBuff, CCH(szBuff));
			}

			tlog_error(pstEnv->pstLogCat, 0, 0, "new %d pworld limit:map inst used=%d, item used=%d, monster used=%d, npc used=%d, Area used=%d", 
						pstZonePworld->stPworldDef.PworldID, pstEnv->pstMapInstPool->iUsed, pstEnv->pstItemPool->iUsed,
						pstEnv->pstMonsterPool->iUsed, pstEnv->pstNpcPool->iUsed, pstEnv->pstAreaDynPool->iUsed);
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP112);
			
			return -1;
		}

		if (pstZoneTeam)
		{
			iTeamCreate = 1;
		}
		else
		{
			iTeamCreate = 0;
		}

		switch(pstZonePworld->stPworldDef.Type)
		{
			case PWORLD_TYPE_TEAM:
				if (NULL == pstZoneTeam)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP113);
					return -1;
				}

				break;

			case PWORLD_TYPE_SINGLE:
				iTeamCreate = 0;
				break;

			default:
				break;
		}

		if (iTeamCreate)
		{
			//队伍副本采用队长带领集体进入的方式
			if(iTrans == 0  && 
				pstZonePworld->stPworldDef.Type == PWORLD_TYPE_TEAM && 
				pstZoneTeam)
			{
				pstTeamMember = player_team_get_captain(pstZoneTeam);
				if (NULL == pstTeamMember || 0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP114);
					return -1;
				}

				pstZonePworldInst = z_new_team_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZoneTeam, 
														pstZonePworld, pstPwolrdSelect, iTimeout);
				if (NULL == pstZonePworldInst)
				{
					return -1;
				}

				iPInstID = pstZonePworldInst->iPworldInstID;

				if (pstZonePworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
				{
					if (unreal_team_enter_pworld(pstEnv, pstPlayer, pstZonePworld, pstZonePworldInst,pstZoneTeam) < 0)
					{
						return -1;
					}
				}
				else
				{
					if (0 > z_check_team_pworld(pstEnv, pstPlayer, pstZoneTeam, &pstZonePworld->stPworldDef, 
											pstPwolrdSelect->PworldDifficulty))
					{
						return -1;
					}

					
					//整个队伍进副本
					z_team_enter_pworld_map(pstEnv, pstZoneTeam, pstZonePworldInst, pstZonePworld);
				}
			}
			else if(pstZoneTeam)
			{
				if(pstZonePworld->stPworldDef.EnterUseVig > 0)
				{
					if(pstPlayer != player_team_get_captain_player(pstEnv, pstZoneTeam))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
								ZONEERR_MAP149);
						return -1;
					}
				}
				
				if (0 > z_check_single_pworld(pstEnv, pstPlayer, &pstZonePworld->stPworldDef, 0, NULL))
				{
					return -1;
				}
				
				pstZonePworldInst = z_new_team_pworld_inst(pstEnv->pstAppCtx, pstEnv, 
												pstZoneTeam, pstZonePworld, pstPwolrdSelect, iTimeout);
				if (NULL == pstZonePworldInst)
				{
					return -1;
				}

				iPInstID = pstZonePworldInst->iPworldInstID;

				//单人进副本
				z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld);
			}
		}
		else
		{
			if (0 > z_check_single_pworld(pstEnv, pstPlayer, &pstZonePworld->stPworldDef, 0, NULL))
			{
				return -1;
			}
			
			pstZonePworldInst = z_new_single_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
															pstZonePworld, pstPwolrdSelect, iTimeout);
			if (NULL == pstZonePworldInst)
			{
				return -1;
			}

			//单人进副本
			z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld);
		}

		z_enter_pworld_mon_speech(pstEnv, pstZonePworld, pstZonePworldInst);
	}
	else
	{
		//队伍进入副本后,又组队了，或掉队的人在进副本
		//第一次进这个已经存在的队伍副本的话,就要弹出那个保护面板
		if(iTeamPworldFlag && 
		   pstZonePworldInst->chLimitFlag && 
		   pstZonePworld->stPworldDef.Type != PWORLD_TYPE_PLOY && 
		   !player_check_same_pworld_by_wid(pstEnv, pstZonePworldInst,pstPlayer))
		{
			//设置下玩家的进入副本的时候可以有30秒的倒计时
			pstPlayer->stRoleData.MiscInfo.InPworldSafeTime = MAX_ENTER_PWORLD_SAFE_TIME;
		}
		
		return z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstZonePworld, pstZonePworldInst);
	}
	
	/*if(pstZoneTeam)
	{
		autoteam_check_pworld(pstEnv, pstZoneTeam, pstPlayer, iPworldID);
	}
	else
	{
		autoteam_check_pworld(pstEnv, NULL, pstPlayer, iPworldID);
	}*/

	return iPInstID;
}

int player_close_pworld(ZONESVRENV * pstEnv, Player *pstPlayer, int iPworldID)
{
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	ZoneTeam *pstZoneTeam;
	ZONEPWORLDINST *pstZonePworldInst;
	TEAMMEMBER *pstTeamMember;
	
	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	//看看队伍有没有创建了这个副本
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam)
	{
		pstZonePworldInst = z_find_id_pworld_inst_byteam(pstEnv, pstZoneTeam, iPworldID);
		if (pstZonePworldInst && pstZonePworldInst->stPworldIdx.iID != iPworldID)
		{
			pstZonePworldInst = NULL;
		}
	}
	else
	{
		pstZonePworldInst = NULL;
	}

	//看自己有没有创建了这个副本
	if (NULL == pstZonePworldInst)
	{
		pstZonePworldInst = z_find_pworld_inst_bysingle(pstEnv, pstPlayer);
		if (pstZonePworldInst && pstZonePworldInst->stPworldIdx.iID != iPworldID )
		{
			pstZonePworldInst = NULL;
		}
	}

	if (NULL == pstZonePworldInst)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP115);
		return -1;
	}

	if (PWORLD_TYPE_TEAM == pstZonePworldInst->iType)
	{
		if (NULL == pstZoneTeam)
		{
			return -1;
		}

		pstTeamMember = player_team_get_captain(pstZoneTeam);
		if (NULL == pstTeamMember || 0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP114);
			return -1;
		}
	}
	
	if (pstZonePworldInst->iPlayer > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP116);
		return -1;
	}

	z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP117);

	return 0;
}

int pworld_close_info(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
//	Player *pstPlayer;
//	char szMsg[256];
//	ZoneTeam *pstZoneTeam;
//	TMEMBLOCK *pstMemBlock;
//	int i;
//	ZONEPWORLD *pstPworld;
	int iLeftTime = PWORLD_NOPLAYER_TIME;

	if (pstPworldInst->tEnd - pstEnv->pstAppCtx->stCurr.tv_sec < iLeftTime)
	{
		iLeftTime = pstPworldInst->tEnd - pstEnv->pstAppCtx->stCurr.tv_sec;
		if (iLeftTime < 0)
		{
			iLeftTime = 0;
		}
	}

	/*
	pstPworld	=	z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (pstPworld)
	{
		sprintf(szMsg, "%s 副本将在%d 秒后关闭", pstPworld->stPworldDef.Name, iLeftTime);
	}
	else
	{
		if (pstPworldInst->bOver)
		{
			pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+iLeftTime;
		}
		
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0, "pworld %d defid %d %s", pstPworldInst->iPworldInstID, pstPworldInst->stPworldIdx.iID, szMsg);
	
	if (PWORLD_TYPE_TEAM == pstPworldInst->iType)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstPworldInst->stPworldTypeData.stTeam.iTeamIdx);
		if (pstMemBlock->fValid)
		{
			pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;
			if (pstZoneTeam->stTeamInfo.TeamID == pstPworldInst->stPworldTypeData.stTeam.ullTeamID)
			{
				for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
				{
					pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
					if (NULL == pstPlayer)
					{
						continue;
					}

					//如果在这种副本就不在通知了。
					if (pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE)
					{
						ZONEMAPINST *pstMapInst; 
						ZONEIDX stIdx;
						ZONEPWORLDINST *pstTmpPworldInst;
							
						stIdx.iID = pstPlayer->stRoleData.Map;
						stIdx.iPos = pstPlayer->stOnline.iMapPos;
						pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
						if (NULL == pstMapInst)
						{
							continue;
						}

						pstTmpPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
						if (pstTmpPworldInst && pstTmpPworldInst->stPworldIdx.iID == (int)pstPworld->stPworldDef.PworldID)
						{
							continue;
						}
					}
						
					z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szMsg);
				}
			}
		}
	}
	else if (PWORLD_TYPE_SINGLE == pstPworldInst->iType)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,  pstPworldInst->stPworldTypeData.stSingle.iID);
		if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN && pstPlayer->stRoleData.RoleID == pstPworldInst->stPworldTypeData.stSingle.ullRoleID)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szMsg);
		}
	}
	else
	{
		return -1;
	}
	*/

	if (pstPworldInst->bOver)
	{
		pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+iLeftTime;
	}
		
	return 0;
}

//  给与角色物品
int accord_player_item(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum, int iMail,
						int iLogType,	char *pszTitle,char *pszText, int iBind)
{
	ROLEGRID stRoleGrid;
	RESULT *pstResult;
	ITEMDEF *pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (!pstItemDef)
	{
		return -1;
	}
	
	pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_SWITCH);
	
	if (0==item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, iItemID))
	{
		if (pstResult)
		{
			stRoleGrid.GridData.RoleItem.InstVal1 = iNum;
		}
		else
		{
			stRoleGrid.GridData.RoleItem.Num = iNum;
		}
		
		if (iBind)
			stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0) < 0)
		{
			if (iMail)
			{
				if (player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, 
											pszTitle,pszText,&stRoleGrid, 1, 0,NULL, 0) >= 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
									ZONEERR_MAP118);
					return 1;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP119);
				return -1;
			}
			
		}
		else
		{
			package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_RAFFLE);
		}

		if (iLogType >= 0)
		{
			z_role_pickitem_oplog( pstEnv, pstPlayer, &stRoleGrid, 1, iLogType );
		}
		
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int z_assign_player_item(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszTitle,
							 char *pszText, int iItemID, int iNum, int iMail, ITEMDEC *pstOpGetItem)
{
	ROLEGRID stRoleGrid;

	if (0 >item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, iItemID))
	{
		return -1;
	}

	stRoleGrid.GridData.RoleItem.Num = iNum;
	if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0) < 0)
	{
		if (iMail)
		{
			if (player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, pszTitle,
									pszText,&stRoleGrid, 1, 0,NULL, 0) >= 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_MAP118 );
				return 1;
			}
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP119);
		}

		return -1;
	}
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, 0);

	pstOpGetItem->ID = iItemID;
	pstOpGetItem->Num = iNum;
	pstOpGetItem->BindNum = 0;
	if(stRoleGrid.GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{
		pstOpGetItem->BindNum = iNum;
	}
	
	return 0;
}

// iSort==1 意味着副本结束
int z_poly_pworld_role_info(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEPWORLD *pstZonePworld, int iSort)
{
	if (is_normal_battle_pworld(pstZonePworld))
	{
		normal_battle_svr_info(pstEnv,NULL, pstZonePworld,pstPworldInst, iSort, 0);
	}
	
	return 0;
}

int map_entity_count_if(ZONESVRENV *pstEnv,
                               const ZONEMAPINST *pstMapInst,
                               const void *pvCookie,
                               CountPred pfnPred)
{
	int i = 0, iCount = 0;

	for(i = 0; i < pstMapInst->iAreaDyn; ++i)
	{
		const AREADYN *pstAreaDyn = NULL;
		int j = 0;
		int iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iAnimate; ++j)
		{
			if(j >= MAX_AREA_ANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d",
				           pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				return -1;
			}

			if(pfnPred(pstEnv, pstAreaDyn->aiAnimate[j], (void*)pvCookie, &iCount) < 0)
			{
				return -1;
			}
		}
	}
	return iCount;
}

//活着的带某标记的怪
int AliveFlagMon(ZONESVRENV *pstEnv, int iID, void *pvCookie,
                    int *piCount)
{
	int iFlag = *((const int*)pvCookie);
	const Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
	
	if(pstMon &&  pstMon->cMonMiscFlag & iFlag &&
		pstMon->cStat != MONSTER_DIE)
	{
		++(*piCount);
	}
	
	return 0;
}

int pworld_wave_tip(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst,ZONEMAPINST* pstMapInst,char* pszMsg,int iType)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	ZoneClan *pstZoneClan;

	if (!pszMsg || (0 == *pszMsg))
	{
		return 0;
	}

	memset(pstSysMsg,0,sizeof(*pstSysMsg));

	pstSysMsg->Type = iType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, pszMsg, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	pstZoneClan = z_id_clan(pstEnv,pstPworldInst->stPworldTypeData.stClan.iID);
	if (pstZoneClan)
	{
		player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	}

	//pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);
	return 0;
}

int pworld_wave_countdown_action(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst)
{
	int i = 0;
	ZoneClan* pstZoneClan = NULL;
	for(i = 0; i < pstPworldInst->iPlayer; i ++)
	{
		Player* pstPlayer = z_id_player(pstEnv->pstAppCtx,pstEnv,pstPworldInst->aiPlayerID[i]);
		if (!pstPlayer)
		{
			continue;
		}
		
		player_action_clan_machine_pworld(pstEnv,pstPlayer,pstPworldInst);
	}

	if (pstPworldInst->iType == PWORLD_TYPE_PLOY ||
		pstPworldInst->iType == PWORLD_TYPE_CLAN)
	{
		pstZoneClan = z_id_clan(pstEnv,pstPworldInst->stPworldTypeData.stClan.iID);

		if (pstZoneClan)
		{
			clan_pworld_wave_countdown_action(pstEnv,pstZoneClan,pstPworldInst);
		}	
		
	}

	

	return 0;
}


int pworld_wave_player_award( ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst,ZONEMAPINST* pstMapInst,PWORLDWAVEMONDEF* pstWaveDef,PWORLDROUNDMONINFO *pstMonInfo)
{
	int i = 0;
	char szTip[512] = {0};
	char szContriTip[64] = {0};
	char szExpTip[64] = {0};
	char szItemTip[64] = {0};

	for(i = 0; i < pstPworldInst->iPlayer; i ++)
	{
		Player* pstPlayer = z_id_player(pstEnv->pstAppCtx,pstEnv,pstPworldInst->aiPlayerID[i]);
		ZoneClan* pstZoneClan = NULL;
		if (!pstPlayer)
		{
			continue;
		}

		pstZoneClan = player_get_clan(pstEnv, pstPlayer);

		if (!pstZoneClan)
		{
			continue;
		}

		if (pstZoneClan->stZoneClanInfo.ullMachinePworldWID != pstPworldInst->ullPworldWID)
		{
			continue;
		}
		
		if (pstZoneClan && (pstWaveDef->PlayerAwardContri > 0))
		{			
			clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_OWNED_CONTRIBUT,pstWaveDef->PlayerAwardContri);
			snprintf(szContriTip,sizeof(szContriTip)," %s:%d", LJY_CONTRI, pstWaveDef->PlayerAwardContri);
		}	

		if (pstWaveDef->PlayerAwardExp > 0)
		{
			player_add_exp(pstEnv,pstPlayer,pstWaveDef->PlayerAwardExp,0);
			snprintf(szExpTip,sizeof(szExpTip)," %s:%d", LJY_EXP,pstWaveDef->PlayerAwardExp);
		}

		if (pstWaveDef->PlayerAwardItemID && pstWaveDef->PlayerAwardItemNum > 0)
		{
			ROLEGRID stRoleGrid;
			ITEMDEF* pstItemDef = NULL;
			pstItemDef = z_find_itemdef(pstEnv, pstWaveDef->PlayerAwardItemID);
			
			if (pstItemDef &&
				0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstWaveDef->PlayerAwardItemID ) )
			{
				int iRet = 0;
				stRoleGrid.GridData.RoleItem.Num = pstWaveDef->PlayerAwardItemNum;
					//stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				iRet = package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SHUNIU);
				if (0 == iRet)
				{
				
					snprintf(szItemTip,sizeof(szItemTip)," %sX%d",pstItemDef->Name,pstWaveDef->PlayerAwardItemNum);
				}
			}
		}
		
		snprintf(szTip,sizeof(szTip),"%s%s%s%s",pstWaveDef->PlayerAwardTip,szExpTip,szContriTip,szItemTip);
		if (szTip[0])
		{
			z_sys_strf(pstEnv,pstPlayer,pstWaveDef->PlayerAwardTipType, szTip);
		}
	}


	pstMonInfo->iAwardFlag |= AWARD_FLAG_PLAYER;
	//pworld_wave_tip(pstEnv,pstPworldInst,pstMapInst,szTip,pstWaveDef->PlayerAwardTipType);
	return 0;
	UNUSED(pstMapInst);	//add by paraunused.pl
}

int pworld_wave_clan_award( ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst,ZONEMAPINST* pstMapInst,PWORLDWAVEMONDEF* pstWaveDef,PWORLDROUNDMONINFO *pstMonInfo)
{
	int i = 0;
	PworldWaveMonDefTail* pstWaveDefTail = NULL;
	ZoneClan* pstZoneClan = NULL;
	char szTip[512] = {0};
	char szClanMoneyTip[64] = {0};
	char szItemTip[256] = {0};
	char szItemTip2[256] = {0};
	char *pszItemTip = szItemTip;
	char *pszItemTipTmp = szItemTip2;

	if (pstPworldInst->iType != PWORLD_TYPE_PLOY)
	{
		return 0;
	}

	pstZoneClan = z_id_clan(pstEnv,pstPworldInst->stPworldTypeData.stClan.iID);

	if (!pstZoneClan)
	{
		return 0;
	}	

	pstWaveDefTail = z_get_pworld_wave_mon_tail(pstWaveDef);

	for(i = 0; i < pstWaveDefTail->iClanAwardDefCount; i++)
	{
		PWORLDWAVEMONCLANAWARDDEF* pstDef = NULL;
		int idx = i +  pstWaveDefTail->iClanAwardDefStart;		
		ZoneObj *pstObj = pstEnv->pstZoneObj;


		if (idx < pstObj->iPworldWaveMonPosDef && idx >= 0)
		{			
			pstDef = (PWORLDWAVEMONCLANAWARDDEF*)pstObj->sPworldWaveMonClanAwardDef[idx];
			if (pstDef->WaveMonDefID != pstWaveDef->WaveMonDefID)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "WaveMonPos error %d != %d",pstMonInfo->iWaveDefID,pstDef->WaveMonDefID);
				continue;
			}

			if (CLAN_WAVE_AWARD_ITEM == pstDef->Type)
			{
				ITEMDEF* pstItemDef = NULL;
				pstItemDef = z_find_itemdef(pstEnv,pstDef->ItemID);

				if(pstItemDef)
				{
					char *pszTmp = NULL;

					pszTmp = pszItemTip;
					pszItemTip = pszItemTipTmp;
					pszItemTipTmp = pszTmp;
					
					clan_resource_add_entries(pstEnv, pstZoneClan, pstDef->ItemID,
			                                pstDef->Num,NULL);
					
					snprintf(pszItemTip,sizeof(szItemTip),"%s %sX%d",pszItemTipTmp,pstItemDef->Name,pstDef->Num);

				}
			}
			else if (CLAN_WAVE_AWARD_MONEY == pstDef->Type)
			{
				pstZoneClan->stZoneClanInfo.iListVersion++;
				clan_property_adapt(pstEnv, pstZoneClan,
				                           CLAN_PROPERTY_MONEY, pstDef->Num);
				snprintf(szClanMoneyTip,sizeof(szClanMoneyTip)," %s:%d",MONEY_TYPE_CLAN_MONEY, pstDef->Num);
			}
		}		
	}

	
	
	snprintf(szTip,sizeof(szTip),"%s%s%s",pstWaveDef->ClanAwardTip,szClanMoneyTip,pszItemTip);

	pstMonInfo->iAwardFlag |= AWARD_FLAG_CLAN;
	pworld_wave_tip(pstEnv,pstPworldInst,pstMapInst,szTip,pstWaveDef->ClanAwardTipType);
	return 0;
}

int pworld_wave_tick( ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst,PWORLDROUNDMON *pstRound,int iFlag)
{
	PWORLDROUNDMONINFO *pstMonInfo = NULL;	
	PWORLDWAVEMONDEF* pstWaveDef = NULL;
	PworldWaveMonDefTail* pstWaveDefTail = NULL;
	int i = 0;
	ZONEMAPINST* pstMapInst = NULL;
	int k = 0;
	int iNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	PWORLDROUNDMONINFO *pstPreMonInfo = NULL;	
	PWORLDWAVEMONDEF* pstPreWaveDef = NULL;

	if (pstPworldInst->bOver)
	{
		return 0;
	}
	
	if (pstRound->iTotalRound <= 0)
	{	
		return 0;
	}


	if (pstRound->iCurrRound < 0 &&
		pstRound->iCurrRound >= PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound >= pstRound->iTotalRound)
	{
		return 0;
	}

	pstMonInfo = &pstRound->aRoundMonInfo[pstRound->iCurrRound];

	pstWaveDef = z_find_pworld_wave_mon_def(pstEnv,pstMonInfo->iWaveDefID);

	if (!pstWaveDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "WaveDefID error :%d",pstMonInfo->iWaveDefID);
		return 0;
	}

	pstWaveDefTail = z_get_pworld_wave_mon_tail(pstWaveDef);

	if (pstPworldInst->iMapInst > 0)
	{
		pstMapInst	= z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	}

	if (!pstMapInst)
	{
		return 0;
	}

	//上一波信息
	if (pstRound->iCurrRound > 0)
	{
		pstPreMonInfo = &pstRound->aRoundMonInfo[pstRound->iCurrRound - 1];

		pstPreWaveDef = z_find_pworld_wave_mon_def(pstEnv,pstPreMonInfo->iWaveDefID);

		if (!pstPreWaveDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "WaveDefID error :%d",pstPreMonInfo->iWaveDefID);
			return 0;
		}
	}

		//时间未设置
	if (0 == pstMonInfo->iRefreshTime)
	{
		pstMonInfo->iCountDown = pstWaveDef->RefreshCountDown;
		pstMonInfo->iRefreshTime = iNow + pstMonInfo->iCountDown;
		pworld_wave_countdown_action(pstEnv,pstPworldInst);
		return 0;
	}

		//刷完了,没有下一波
	if (pstMonInfo->iCurrNum >= pstMonInfo->iTotalNum && 
		pstRound->iCurrRound == pstRound->iTotalRound - 1)
	{
		//最后一波是否死光了

		int iNum = map_entity_count_if(pstEnv, pstMapInst, &iFlag, 
							   AliveFlagMon);
		if (iNum <= 0 )
		{
			if (!(pstMonInfo->iAwardFlag & AWARD_FLAG_PLAYER))
			{
				pworld_wave_player_award(pstEnv,pstPworldInst,pstMapInst,pstWaveDef,pstMonInfo);
			}

			if (!(pstMonInfo->iAwardFlag & AWARD_FLAG_CLAN))
			{
				pworld_wave_clan_award(pstEnv,pstPworldInst,pstMapInst,pstWaveDef,pstMonInfo);
			}
		}

		//pstRound->iCurrRound++;

		return 0;
	}

	//时间没到
	if (pstMonInfo->iRefreshTime > iNow)
	{
		int iEarly = 0;
		//上一波是否死光了
		if (pstPreMonInfo && pstPreWaveDef)
		{
			int iNum = map_entity_count_if(pstEnv, pstMapInst, &iFlag, 
		                           AliveFlagMon);
			if (iNum <= 0 )
			{
				if (pstWaveDef->RefreshEarly)
				{
					iEarly = 1;
				}
			}

			if ((pstMonInfo->iRefreshTime - iNow) <= pstWaveDef->PreWavePlayerAwardTime &&
			!(pstPreMonInfo->iAwardFlag & AWARD_FLAG_PLAYER))
			{
				pworld_wave_player_award(pstEnv,pstPworldInst,pstMapInst,pstPreWaveDef,pstPreMonInfo);
			}

			if ((pstMonInfo->iRefreshTime - iNow) <= pstWaveDef->PreWaveClanAwardTime &&
				!(pstPreMonInfo->iAwardFlag & AWARD_FLAG_CLAN))
			{
				pworld_wave_clan_award(pstEnv,pstPworldInst,pstMapInst,pstPreWaveDef,pstPreMonInfo);
			}
		}

		

		if (iEarly && !(pstMonInfo->iAwardFlag & AWARD_FLAG_EARLY))
		{
			int iTime = pstMonInfo->iCountDown - pstWaveDef->RefreshCountDown + pstWaveDef->WarningTime;
			if (iTime < pstMonInfo->iRefreshTime - iNow)
			{			
				pstMonInfo->iRefreshTime = iNow + iTime;
				pworld_wave_countdown_action(pstEnv,pstPworldInst);
				pstMonInfo->iAwardFlag |= AWARD_FLAG_EARLY;
			}
			
			
		}
		
		if ((pstMonInfo->iRefreshTime - iNow) <= pstWaveDef->WarningTime &&
			!(pstMonInfo->iAwardFlag & AWARD_FLAG_WARNING_TIP))
		{
			//预警提示			
			pworld_wave_tip(pstEnv,pstPworldInst,pstMapInst,pstWaveDef->WarningTip,pstWaveDef->WarningTipType);
			pstMonInfo->iAwardFlag |= AWARD_FLAG_WARNING_TIP;
		}
		
		return 0;
	}



	//时间到了,没刷完	
	if (pstMonInfo->iCurrNum < pstMonInfo->iTotalNum)
	{
		//还没到重复刷时间
		if (pstMonInfo->iRepeatTime && pstMonInfo->iRepeatTime > iNow)
		{
			return 0;
		}

		if (0 == pstMonInfo->iCurrNum)
		{
			//刷怪提示
			if(!(pstMonInfo->iAwardFlag & AWARD_FLAG_REFRESH_TIP))
			{
				pworld_wave_tip(pstEnv,pstPworldInst,pstMapInst,pstWaveDef->RefreshTip,pstWaveDef->RefreshTipType);
				pstMonInfo->iAwardFlag |= AWARD_FLAG_REFRESH_TIP;

				ZoneClan *pstZoneClan;

				pstZoneClan = z_id_clan(pstEnv, pstPworldInst->stPworldTypeData.stClan.iID);
				if (pstZoneClan)
				{
					clan_active_ui_info_clan_machine_pworld(pstEnv, pstZoneClan,
						ACTIVE_GAME_SVR_TYPE_UI_INFO, pstPworldInst, 1);
				}
			}

			if (pstPreMonInfo && pstPreWaveDef &&
				!(pstPreMonInfo->iAwardFlag & AWARD_FLAG_PLAYER))
			{
				pworld_wave_player_award(pstEnv,pstPworldInst,pstMapInst,pstPreWaveDef,pstPreMonInfo);
			}

			if (pstPreMonInfo && pstPreWaveDef &&
				!(pstPreMonInfo->iAwardFlag & AWARD_FLAG_CLAN))
			{
				pworld_wave_clan_award(pstEnv,pstPworldInst,pstMapInst,pstPreWaveDef,pstPreMonInfo);
			}
		}
		
		for (i = 0; i <  pstWaveDefTail->iMonPosDefCount; i++)
		{
			int idx = i +  pstWaveDefTail->iMonPosDefStart;
			PWORLDWAVEMONPOSDEF* pstDef = NULL;
			ZoneObj *pstObj = pstEnv->pstZoneObj;
			MONSTERDEF *pstMonDef = NULL;
			Monster* pstNew = NULL;

			if (idx < pstObj->iPworldWaveMonPosDef && idx >= 0)
			{	
				Monster stMonster;
				
				pstDef = (PWORLDWAVEMONPOSDEF*)pstObj->sPworldWaveMonPosDef[idx];
				if (pstDef->WaveMonDefID != pstWaveDef->WaveMonDefID)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "WaveMonPos error %d != %d",pstMonInfo->iWaveDefID,pstDef->WaveMonDefID);
					continue;
				}

				if (pstMonInfo->iCurrRepeat != pstDef->RefreshAtRepeatNum - 1)
				{
					continue;
				}
				
				memset(&stMonster, 0, sizeof(stMonster));
				
				stMonster.cStat = MONSTER_DIE;
				stMonster.iDefIdx = pstDef->MonID;
				stMonster.iDefPos = -1;
				stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
				stMonster.stMap.iPos =	pstMapInst->iIndex;
				stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
				stMonster.stInitPos.X = pstDef->X;
				stMonster.stInitPos.Y = pstDef->Y;
				stMonster.cRefresh	= 0;
				stMonster.bDirection = pstDef->Dir;

				pstMonDef = z_get_mon_def(pstEnv, &stMonster);
				if (NULL == pstMonDef)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0,	"pstMonDef NULL	%d", stMonster.iDefIdx);
					return 0;
				}

				for (k = 0; k < pstDef->Num; k++)
				{
					pstNew =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
				
					if( !pstNew )
					{
						tlog_error(pstEnv->pstLogCat, 0, 0,	"NEW MON FAIL  %d", stMonster.iDefIdx);
						return 0;
					}
			
					pstNew->iPatrolID = pstDef->PatrolID;
			
					pstNew->cFirstBorn = 1;
					if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstNew,NULL))
					{
						tmempool_free(pstEnv->pstMonsterPool, pstNew->iMIndex);

						tlog_error(pstEnv->pstLogCat, 0, 0,	"z_refresh_mon FAIL	%d", stMonster.iDefIdx);
						return 0;
					}
					
					pstNew->cFirstBorn = 0;
			
					if (pstDef->PatrolID > 0)
					{
						pstNew->bMoveType = MOVE_TYPE_PATROL;
					}	

					pstNew->cMonMiscFlag |= iFlag;

					pstMonInfo->iCurrNum++;
				}

			}
		}

		pstMonInfo->iRepeatTime = iNow + pstWaveDef->RefreshInterval;
		pstMonInfo->iCurrRepeat++;
	}	

	//刷完了,进入下一波
	if (pstMonInfo->iCurrNum >= pstMonInfo->iTotalNum)
	{
		pstMonInfo->iRepeatTime = 0;
		pstRound->iCurrRound++;

		pstRound->iCurrRound = min(pstRound->iTotalRound - 1,pstRound->iCurrRound);
		pworld_wave_countdown_action(pstEnv,pstPworldInst);
	}

	return 0;
		
}

int z_active_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long llMs=0;
	ZONEPWORLDINST *pstPworldInst;
	TMEMBLOCK *pstMemBlock;
	ZONEPWORLD *pstZonePworld;

//	int iOldDownTm = 0;

	if (0 == stRoundTv.tv_sec)
	{
		llMs = PWORLD_ACTIVE_TIMER;
	}
	else
	{
		TV_DIFF(stSub, pstAppCtx->stCurr, stRoundTv);
		TV_TO_MS(llMs, stSub);
	}

	if (0 > llMs)
	{
		TV_CLONE(stRoundTv, pstAppCtx->stCurr);
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
		
	if (llMs < PWORLD_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
			
	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= MAX_PWORLD_INST) 
		{
			break;
		}		
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPworldInstPool, iActivePos);
		iActivePos++;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}
		
		pstPworldInst = (ZONEPWORLDINST *)pstMemBlock->szData;

		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstZonePworld)
		{
			continue;
		}
		
		if (pstPworldInst->iPlayer <= 0 && (pstZonePworld->stPworldDef.CtrlFlag & ALL_DIE_DESTORY))
		{
			z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			continue;
		}

		if (pstPworldInst->bDelayScript & PWORLD_SCRIPT_PROC_START_MAP)
		{
			char szName[128];

			pstPworldInst->bDelayScript &= ~PWORLD_SCRIPT_PROC_START_MAP;
				
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = NULL;
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;

			snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
			szName[sizeof(szName) - 1] = 0;
			
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_START);
			continue;
		}

		if (pstPworldInst->iStrongEnd > 0 && pstPworldInst->iStrongEnd < pstAppCtx->stCurr.tv_sec)
		{
			z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			continue;
		}

		strong_pworld_tick(pstEnv, pstZonePworld, pstPworldInst,1);
		span_strong_pworld_tick(pstEnv, pstZonePworld, pstPworldInst,1);
		pworld_award_tick(pstEnv, pstPworldInst);
		poly_pworld_tick(pstEnv, pstPworldInst, iRound);

		if (0 < pstPworldInst->tEnd && pstPworldInst->tEnd < pstAppCtx->stCurr.tv_sec)
		{
			if (strong_is_normal(pstZonePworld))
			{
				strong_pworld_tick(pstEnv, pstZonePworld, pstPworldInst,0);
				continue;
			}

			if (strong_is_span(pstZonePworld))
			{
				span_strong_pworld_tick(pstEnv, pstZonePworld, pstPworldInst,0);
			}
			
			if (is_normal_battle_pworld(pstZonePworld))
			{
				if(pstPworldInst->bOver == 1)
				{
					z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				}
				else
				{
					//停止副本刷怪等。
					z_pworld_over(pstEnv, pstPworldInst);
					//结束信息
					normal_battle_svr_info(pstEnv,NULL, pstZonePworld,pstPworldInst, 2, 0);
				}
			}
			else if(pstZonePworld->stPworldDef.PworldID == PWORLD_LIUYI ||
				pstZonePworld->stPworldDef.PworldID == PWORLD_LIUYI2)
			{
				if(pstPworldInst->bOver == 1)
				{
					z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				}
				else
				{
					poly_pworld_end(pstEnv, pstPworldInst);
				}
			}
			else if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_DOUBLE_MONSTER ||
				pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_MACHINE_WAR ||
				pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_TANK)
			{
				if(pstPworldInst->bOver == 1)
				{
					z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				}
				else
				{
					poly_pworld_end(pstEnv, pstPworldInst);
				}
			}
			else if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA)
			{
				if(pstPworldInst->bOver == 1)
				{
					z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				}
				else
				{
					sparta_fight_info_update(pstEnv, pstPworldInst, NULL, 1, 0);
				}
			}
			else
			{
				z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			}
			
			continue;
		}
		
		if( pstPworldInst->tStart + PWORLD_ADD_LIMIT_TIME < pstAppCtx->stCurr.tv_sec)
		{
			pstPworldInst->chLimitFlag = 1;
		}

		// 10秒发给客户端活动fb信息
		if (iActivePos % 5 == iRound % 5) 
		{
			if (is_normal_battle_pworld(pstZonePworld) && 
				pstPworldInst->bOver == 0)
			{
				normal_battle_svr_info(pstEnv,NULL, pstZonePworld,pstPworldInst, 1, 0);
			}

			if (strong_is_normal(pstZonePworld)&& !pstPworldInst->bOver)
			{
				strong_war_all_notify(pstEnv,pstPworldInst, 0);
			}

			if (strong_is_span(pstZonePworld) && !pstPworldInst->bOver)
			{
				span_strong_war_all_notify( pstEnv, pstPworldInst, 0, pstZonePworld->stPworldDef.StrongMapID);
			}
		}

		if(0 == pstPworldInst->iPlayer && (is_normal_battle_pworld(pstZonePworld) || 
							is_shikong_pworld(pstZonePworld)))
		{
			int iEnterValidTime;

			iEnterValidTime = get_battle_enter_validtime(pstZonePworld);

			if(pstPworldInst->tStart + iEnterValidTime < pstAppCtx->stCurr.tv_sec)
			{
				z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				continue;
			}

			if(pstPworldInst->bOver)
			{
				z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				continue;
			}
		}

		if (0 == pstPworldInst->iPlayer &&
			(pstPworldInst->iType != PWORLD_TYPE_CLAN && 
			pstPworldInst->iType != PWORLD_TYPE_PLOY &&
			pstPworldInst->iType != PWORLD_TYPE_HOME))
		{

			if(pstZonePworld->stPworldDef.Type == PWORLD_TYPE_ALL || 
				pstZonePworld->stPworldDef.Type == PWORLD_TYPE_TEAM)
			{
				z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
				continue;
			}
			else
			{
				if (pstPworldInst->tNoPlayer > 0)
				{
					if (pstPworldInst->tNoPlayer + PWORLD_NOPLAYER_TIME < pstAppCtx->stCurr.tv_sec)
					{
						z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
						continue;
					}
				}
				else
				{
					pstPworldInst->tNoPlayer = pstAppCtx->stCurr.tv_sec;
					pworld_close_info(pstEnv, pstPworldInst);
				}
			}
		}
		else
		{
			pstPworldInst->tNoPlayer = 0;
		}

		if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_TIMER_MAP))
		{
			char szName[128];
			
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = NULL;
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;

			snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
			szName[sizeof(szName) - 1] = 0;
						
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_TIMER);
		}

		//波次怪
		pworld_wave_tick(pstEnv,pstPworldInst,&pstPworldInst->stWaveRound,MON_MISC_FLAG_PWORLD_WAVE);

		//定时怪
		pworld_wave_tick(pstEnv,pstPworldInst,&pstPworldInst->stTimerRound,MON_MISC_FLAG_PWORLD_TIMER);


		// 倒计时结束事件
		if (	pstPworldInst->iDownTm > 0 &&
			pstPworldInst->iDownTm <= pstAppCtx->stCurr.tv_sec )
		{
			// 防止多次进脚本
			pstPworldInst->iDownTm = 0;
			
			if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_DOWN_TIME_EVENT) )
			{
				char szName[128];
			
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = NULL;
				stSEnv.pstPworldInst = pstPworldInst;
				stSEnv.pstKiller = NULL;
				stSEnv.pstMapinst = NULL;

				snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
				szName[sizeof(szName) - 1] = 0;		
				z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, 
													PWORLD_SCRIPT_PROC_DOWN_TIME);
			}
			else if (pstZonePworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
			{
				z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstPworldInst);
			}
			else if (pstZonePworld->stPworldDef.Type == PWORLD_TYPE_PLOY && pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG )
			{
				z_dianfeng_pworld_down_tm_end(pstEnv,pstPworldInst,pstZonePworld);
			}
		}
	}
	if (iActivePos < MAX_PWORLD_INST)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;

	//一分钟上报一次
	if (iRound % 30 == 0) 
	{
		//if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode ) 
		//{
			//z_battle_report_pworld_info( pstEnv, pstAppCtx);
		//}
	}
	
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > PWORLD_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active pworld loop %lld ms", llMs);
	}
	
	return 0;
}


int z_active_mapinst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long llMs=0;
	TMEMBLOCK *pstMemBlock;
	ZONEMAPINST *pstMapInst;
	MapIndex* pstMapIdx;
	MapIndex* pstMapIndexRefer = NULL;

	if (0 == stRoundTv.tv_sec)
	{
		llMs = MAPINST_ACTIVE_TIMER;
	}
	else
	{
		TV_DIFF(stSub, pstAppCtx->stCurr, stRoundTv);
		TV_TO_MS(llMs, stSub);
	}

	if (0 > llMs)
	{
		TV_CLONE(stRoundTv, pstAppCtx->stCurr);
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
		
	if (llMs < MAPINST_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
	
	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= MAX_MAP_INST) 
		{
			break;
		}		
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstMapInstPool, iActivePos);
		iActivePos++;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}
		pstMapInst = (ZONEMAPINST *)pstMemBlock->szData;

		pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if( !pstMapIdx )
			continue;

		if (pstMapIdx->stMapDef.ReferMapID > 0 && pstMapIdx->stMapDef.MapID < PWORLD_MAPID_BASE)
		{
			pstMapIndexRefer = z_mappos_index(pstEnv->pstMapObj, &pstMapIdx->stMapDef.ReferMapIDPos, pstMapIdx->stMapDef.ReferMapID);
			if (NULL == pstMapIndexRefer)
			{
				continue;
			}
		}
		else
		{
			pstMapIndexRefer = pstMapIdx;
		}

		//刷新矿资源
		map_refresh_ore( pstEnv, pstMapIndexRefer, pstMapInst);

		world_city_tick(pstEnv, pstMapInst);
		
		if ((pstMapInst->cInstScriptFlag & MAP_INST_SCRIPT_PROC_TIME_FLAG))
		{
			char szName[128];
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = NULL;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstMapinst = pstMapInst;
			stSEnv.pstKiller = NULL;
					
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (!pstMapIdx)
				continue;
			
			snprintf(szName, sizeof(szName), "map%d.mac", pstMapIdx->stMapDef.MapID);
			szName[sizeof(szName) - 1] = 0;
						
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMapInst->stScriptIdx, MAP_INST_SCRIPT_PROC_TIME);
		}
		
	}
	
	if (iActivePos < MAX_MAP_INST)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > MAPINST_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active pworld loop %lld ms", llMs);
	}
	
	return 0;
}

static int z_player_restore_fairy(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	ROLEFAIRY *pstRoleFairy = NULL;

	pstRoleFairy = get_pack_fairy_by_id(pstPlayer, pstPlayer->stRoleData.MiscInfo.CampFyID);
	if (!pstRoleFairy)
	{
		return 0;
	}

	 set_fairy_camp(pstEnv, pstPlayer, pstRoleFairy);
	 player_add_xp(pstEnv, pstPlayer, pstPlayer->stRoleData.MiscInfo.CampFyXP, 0,1);
	 pstPlayer->stRoleData.MiscInfo.CampFyXP = 0;
	 pstPlayer->stRoleData.MiscInfo.CampFyID = 0;
	return 0;
}

static int z_player_chg_map_bct_info(ZONESVRENV* pstEnv, Player* pstPlayer, ZONEPWORLDINST *pstPworldInst)
{
	BCTDEF *pstBctDef = NULL;
	ZONEPWORLD *pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);

	if(pstZonePworld == NULL)
	{
		return -1;
	}

	pstPworldInst->chLimitFlag = 1;
	
	// 每层开始时间
	pstPlayer->stRoleData.MiscInfo.BCTInfo.LayerStartTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	// pstPlayer->stOnline.stBctOnline.iStartLayer = 0 表示不是进入副本
	if (pstPlayer->stOnline.stBctOnline.iStartLayer <= 0 )
	{
		pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer +=pstPlayer->stOnline.stBctOnline.iJump ;
	}
	
	// 白层塔每次切换地图 		通知客户端
	// 没必要广播给客户端
	bct_layer_history_notify(pstEnv, pstPlayer, pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer);

	pstBctDef = z_find_bctdef(pstEnv, pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer);
	if (pstBctDef && pstBctDef->TimeLimit > 0)
	{
		pstPworldInst->iDownTm = pstBctDef->TimeLimit *60 + pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	return 0;
}

//测试角色能否进入区域
int z_player_enter_area_test(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST* pstMapInst, RESPOS *pstPos)
{
	int iRet;
	int iAniNum;
	MapIndex* pstMapIdx;
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
		return -1;

	//要附带上乘客一起进，要么都不进
	iAniNum = player_get_ride_passenger_num(pstPlayer) + 1;

	iRet = z_ani_enter_area_test(pstMapIdx, pstEnv->pstAreaDynPool, iAniNum, pstPos, pstMapInst);
	
	return iRet;
}

int z_role_login_entermap_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	MapIndex* pstMapIdx = NULL;
	int iEnterMap;
	RESPOS stPos = pstPlayer->stRoleData.Pos;
	int iBaseCity = 0;

	//接管的过程不需要检查了
	if(pstPlayer->stOnline.bWaitRoleLogout > 0)
	{
		return 0;
	}

	if(WORLD_CITY_MAPID == GET_MAPID(pstPlayer->stRoleData.Map))
	{
		ZoneClan *pstClan= player_get_clan(pstEnv, pstPlayer);
		if (!pstClan)
		{
			goto _next1;
		}
		
		if(!is_world_city_owner(pstEnv, pstPlayer))
		{
			goto _next1;
		}
		
		if (clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
				                  &pstPlayer->stOnline.iClanMemIdx) == NULL)
		{
			goto _next1;
		}
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		goto _next1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		goto _next1;
	}

	if (pstMapIdx->stMapDef.CtrlFlag &MAP_CTRL_NO_LOGIN)
	{
		iBaseCity = 1;
		goto _next1;
	}

	if (pstMapIdx->stMapDef.CtrlFlag &MAP_CTRL_LOGOUT_TO_REVIVE_POINT)
	{
		goto _next1;
	}

	// 时间到了 回时间城复活点
	if (pstMapIdx->stMapDef.EnterTime > 0)
	{
		if (	pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime > 0 &&
			pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			iBaseCity = 1;
			goto _next1;	
		}
	}
	

	if (0 == z_player_enter_area_test(pstEnv, pstPlayer, pstMapInst, &stPos))
	{
		return 0;
	}

_next1:

	//送到复活点
	if (pstMapIdx && pstMapIdx->stMapDef.ReviveMap > 0 && !iBaseCity)
	{
		iEnterMap = pstMapIdx->stMapDef.ReviveMap;
		stPos = pstMapIdx->stMapDef.RevivePs[0];
	}
	else
	{
		//回到主城复活点
		iEnterMap = BASE_CITY_MAPID;
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iEnterMap);
		if (NULL == pstMapIdx)
		{
			return -1;
		}

		stPos = pstMapIdx->stMapDef.RevivePs[0];
	}
	
	stIdx.iID = iEnterMap;
	stIdx.iPos =	-1;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	if (0 == z_player_enter_area_test(pstEnv, pstPlayer, pstMapInst, &stPos))
	{
		pstPlayer->stRoleData.Map = iEnterMap;
		pstPlayer->stRoleData.Pos= stPos;
		return 0;
	}
	
	return -1;
}

int z_player_ride_machine_chg_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	Player *pstPlayer, int iMapIndex, unsigned char bDirection, int iChg, CSENTERMAP *pstEnter)
{
	int i;
	RESPOS stPos;
	Player *pstTmpPlayer;
	Monster * pstMachine;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERMAP *pstEnterMap = &pstPkg->Body.EnterMap;
	
	if(!is_player_driver_machine(pstEnv, pstPlayer))
	{
		return 0;
	}

	*pstEnterMap = *pstEnter;
	stPos.X = pstEnter->x;
	stPos.Y = pstEnter->y;

	pstMachine  = z_id_monster(pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(NULL == pstMachine)
	{
		return 0;
	}

	for(i=1; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i] <= 0)
		{
			continue;
		}
		
		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		z_player_leave_map(pstAppCtx, pstEnv, pstTmpPlayer, iChg);
		
		pstTmpPlayer->stOnline.iPosSeq += 1000;
		pstEnterMap->InitMoveSeq = pstTmpPlayer->stOnline.iPosSeq;
		
		pstTmpPlayer->stOnline.bDir = bDirection;
		pstEnterMap->Dir = pstTmpPlayer->stOnline.bDir;

		Z_CSHEAD_INIT(&pstPkg->Head, ENTER_MAP);
		z_cltmsg_send(pstEnv, pstTmpPlayer, pstPkg);
		
		z_player_entermap(pstAppCtx, pstEnv, pstTmpPlayer, iMapIndex, &stPos, iChg);
	}

	machine_chg_map(pstEnv, pstMachine, iMapIndex, &stPos);

	return 0;
}


int player_chg_map_timelimit(ZONESVRENV* pstEnv, Player* pstPlayer,MapIndex *pstIdx,int *piTime, int iLogin)
{
	
	if (pstIdx->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		if (pstIdx->stMapDef.EnterTime > 0)
		{
			/*if (pstPlayer->stOnline.stMapOL.iEndTime <= 0 )
			{
				*piTime = pstIdx->stMapDef.EnterTime*60;
				pstPlayer->stOnline.stMapOL.iEndTime = 
							pstEnv->pstAppCtx->stCurr.tv_sec+pstIdx->stMapDef.EnterTime*60;
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MAP150, 
							pstIdx->stMapDef.EnterTime);
			}
			else
			{
				*piTime = pstPlayer->stOnline.stMapOL.iEndTime - pstEnv->pstAppCtx->stCurr.tv_sec;
			}*/

			if (!iLogin &&  !g_iTimeLimit )
			{
				*piTime = pstIdx->stMapDef.EnterTime*60;
				pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime = 
						pstEnv->pstAppCtx->stCurr.tv_sec+pstIdx->stMapDef.EnterTime*60;
				//pstPlayer->stOnline.stMapOL.iEndTime = 
				//			pstEnv->pstAppCtx->stCurr.tv_sec+pstIdx->stMapDef.EnterTime*60;
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MAP150, 
							pstIdx->stMapDef.EnterTime);
			}
			else
			{
				*piTime = pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime - pstEnv->pstAppCtx->stCurr.tv_sec;
				//*piTime = pstPlayer->stOnline.stMapOL.iEndTime - pstEnv->pstAppCtx->stCurr.tv_sec;
			}
		}
		else
		{
			*piTime = pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime  = 0;
		}
	}
	return 0;
}

int player_chg_map_passenger_check(ZONESVRENV* pstEnv, Player* pstPlayer,MapIndex *pstIdx)
{
	int i;
	Monster *pstMachine;
	Player *pstTmpPlayer;

	if((pstPlayer->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY))&& 
		pstPlayer->stOnline.stRideOnInfo.iPassengerNum > 0)
	{
		for(i=0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			if(0 > player_follow_enter_map_check(pstEnv, pstTmpPlayer, pstIdx->stMapDef.MapID))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
					ZONEERR_MAP151, pstTmpPlayer->stRoleData.RoleName);
				return -1;
			}
		}
	}
	
	if(!is_player_driver_machine(pstEnv, pstPlayer))
	{
		return 0;
	}

	pstMachine  = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(NULL == pstMachine)
	{
		return 0;
	}

	for(i=1; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i] <= 0)
		{
			continue;
		}
		
		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(0 > player_follow_enter_map_check(pstEnv, pstTmpPlayer, pstIdx->stMapDef.MapID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_MAP151, pstTmpPlayer->stRoleData.RoleName);
			return -1;
		}
	}

	return 0;
}

int z_player_change_map2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iMapIndex, RESPOS *pstPos, unsigned char bDirection, int iCameraDir)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERMAP *pstEnterMap = &pstPkg->Body.EnterMap;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLD *pstPworld = NULL;
	ZONEPWORLDINST *pstPworldInst;
	MapIndex* pstIdx;
	RESPOS stPos;
	int i;
	int iRet = 0;
	int iOldMap = pstPlayer->stRoleData.Map;
	int iChgMap;
	int iUnrealNotify=0;
	ZoneTeam *pstTeam;
	Player *pstCap;
	int iStrongMap = 0;
	int iDir = 0;
	int dianfengflag = 0;

	SET_PLAYER_NOMOVE(pstPlayer);
	z_player_move_stop(pstAppCtx, pstEnv, pstPlayer);

	if (player_is_in_dart_car(pstEnv, pstPlayer) && !g_dartcar_allow)
	{
		player_ride_machine_clear(pstEnv, pstPlayer);	
	}

	stIdx.iID	=	iMapIndex;
	stIdx.iPos	=	-1;
	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
		return -1;

	pstIdx	= z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, stIdx.iID);
	if( !pstIdx )
		return -1;
	
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst)
	{
		pstEnterMap->PworldInfoFlag = 0;
	}
	else
	{
		pstEnterMap->PworldInfoFlag = 1;
		pstEnterMap->PworldInfo.PworldInstID = pstPworldInst->iPworldInstID;
		pstEnterMap->PworldInfo.SelectGrade = pstPworldInst->stPworldselect.PworldGrade;
		pstEnterMap->PworldInfo.StartTime = pstPworldInst->tStart;

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (pstPworld)
		{
			if (pstPworld->stPworldDef.PworldID == BCT_PWORLD_ID)
			{
				z_player_chg_map_bct_info(pstEnv, pstPlayer, pstPworldInst);
			}
			else if (pstPworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
			{
				iUnrealNotify = 1;
			}
			else if (pstPworld->stPworldDef.StrongMapID > 0)
			{
				iStrongMap = pstPworld->stPworldDef.StrongMapID ;
			}
		}
	
		
		if (pstPworld && 
			pstPworld->stPworldDef.PlayerInTime > 0)
		{
			if (pstPworld->stPworldDef.PworldID == pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID)
			{
				pstEnterMap->PworldInfo.LeftTime = pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime - pstAppCtx->stCurr.tv_sec;
			}
			else		//防范一下 
			{
				pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID = pstPworld->stPworldDef.PworldID;
				pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstPworld->stPworldDef.PlayerInTime;
				pstEnterMap->PworldInfo.LeftTime = pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime - pstAppCtx->stCurr.tv_sec;
			}
			
			if (0 > pstEnterMap->PworldInfo.LeftTime)
			{
				pstEnterMap->PworldInfo.LeftTime = 0;
			}
		}
		else if (pstPworldInst->iDownTm > 0 &&
			(pstPworld && 0 == pstPworld->stPworldDef.ApplyUnitType))
			
		{
			pstEnterMap->PworldInfo.LeftTime = pstPworldInst->iDownTm - pstAppCtx->stCurr.tv_sec;
			if (0 > pstEnterMap->PworldInfo.LeftTime)
			{
				pstEnterMap->PworldInfo.LeftTime = 0;
			}
		}
		else if (pstPworldInst->tEnd > 0)
		{
			pstEnterMap->PworldInfo.LeftTime = pstPworldInst->tEnd - pstAppCtx->stCurr.tv_sec;
			if (0 > pstEnterMap->PworldInfo.LeftTime)
			{
				pstEnterMap->PworldInfo.LeftTime = 0;
			}
		}
		else
		{
			pstEnterMap->PworldInfo.LeftTime = -1;
		}

	}

	if (iOldMap == iMapIndex)
	{
		iChgMap = 0;
	}
	else
	{
		iChgMap = 1;
	}

	//跨服且地图检查
	if(0 > player_span_map_check(pstEnv, pstPlayer, pstIdx, pstPos))
	{
		return 0;
	}

	//乘客不可以主动切换普通地图
	if(iChgMap && pstPworldInst == NULL && (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE))
	{
		return -1;
	}

	//怪物机械上的乘客也不可以主动切换地图
	if(iChgMap &&  is_player_normal_passenger_machine(pstEnv, pstPlayer))
	{
		player_ride_machine_brk(pstEnv, pstPlayer, 1);
	}

	//找一个进入地图的落脚点
	z_get_ran_pos(pstEnv, pstMapInst, pstIdx, pstPos, pstPos, 80, 20, &stPos);

	//确保能够进入地图的检查
	if(0 > z_player_enter_area_test( pstEnv, pstPlayer, pstMapInst, &stPos))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,  ZONEERR_MAP152);
		return -1;
	}

	//防止嵌套 change_map
	if(-2 == z_player_leave_map(pstAppCtx, pstEnv, pstPlayer, iChgMap))
	{
		return -1;
	}
	
	pstPlayer->stOnline.iPosSeq += 1000;
	pstPlayer->stOnline.bDir = bDirection;

	if (pstPworld && pstPworld->stPworldDef.Type == PWORLD_TYPE_PLOY && pstPworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG )
	{	
		//末世巅峰竞技副本面向
		if(0 == dianfeng_get_dir(pstEnv,pstPlayer,&iDir))
		{
			pstPlayer->stOnline.bDir = iDir;
			dianfengflag = 1;
		}
	}

	if(dianfengflag == 1)
	{
		pstEnterMap->CameraDir = iDir+90;
	}
	else
	{
		pstEnterMap->CameraDir = iCameraDir;
	}
	
	if (pstIdx->stMapDef.ReferMapID > 0)
	{
		pstEnterMap->Map = pstIdx->stMapDef.ReferMapID;
	}
	else
	{
		pstEnterMap->Map = pstIdx->stMapDef.MapID;
	}
	pstEnterMap->RealMap = pstIdx->stMapDef.MapID;
	
	pstEnterMap->x = stPos.X;
	pstEnterMap->y = stPos.Y;
	pstEnterMap->InitMoveSeq = pstPlayer->stOnline.iPosSeq;
	pstEnterMap->Dir = pstPlayer->stOnline.bDir;
	pstEnterMap->Effect = pstIdx->stMapDef.EffectID;
	pstEnterMap->MapEndTime = 0;
	pstEnterMap->Span = map_span_type_get(pstEnv);
	
	if (iChgMap)
	{
		// 主世界地图的玩家时间限制
		// 只有切换了地图才记录
		player_chg_map_timelimit(pstEnv, pstPlayer,pstIdx,&pstEnterMap->MapEndTime, 0);
	}
	else
	{
		if (	pstIdx->stMapDef.EnterTime > 0 && 
			pstIdx->stMapDef.MapID < PWORLD_MAPID_BASE &&
			/*pstPlayer->stOnline.stMapOL.iEndTime > 0*/
			pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime > 0)
		{
			//pstEnterMap->MapEndTime = pstPlayer->stOnline.stMapOL.iEndTime-
			//							pstEnv->pstAppCtx->stCurr.tv_sec;

			pstEnterMap->MapEndTime = pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime-
										pstEnv->pstAppCtx->stCurr.tv_sec;
			
			if (pstEnterMap->MapEndTime < 0)
			{
				pstEnterMap->MapEndTime = 0;
			}
		}
	}
	
	Z_CSHEAD_INIT(&pstPkg->Head, ENTER_MAP);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	if (iUnrealNotify && pstPworldInst)
	{
		// 通知客户端层数,怪物信息
		unreal_layer_info_notify(pstEnv, pstPlayer,pstPworldInst);
	}

	if (iStrongMap)
	{
		if (strong_is_normal( pstPworld))
		{
			strong_svr_pworld_info(pstEnv, pstPlayer,1, NULL,iStrongMap);
		}
		else
		{
			span_strong_pworld_info(pstEnv, pstPlayer, 1, NULL, iStrongMap);
		}
	}

	int i1= is_player_ride_on_machine(pstPlayer);
	iRet = z_player_entermap(pstAppCtx, pstEnv, pstPlayer, iMapIndex, &stPos, iChgMap);

	//if (iChgMap)
	{
		if (i1 && is_player_ride_on_machine(pstPlayer))
		{
			z_player_ride_machine_chg_map(pstAppCtx, pstEnv, pstPlayer, 
								iMapIndex, bDirection, iChgMap, pstEnterMap);
		}
	}
	
	//切换普通地图乘客跟随进入
	if((pstPlayer->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY))&& 
		pstPlayer->stOnline.stRideOnInfo.iPassengerNum > 0)
	{
		if (iChgMap && pstPworldInst == NULL)
		{
			Player *pstTmpPlayer;

			for(i=0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
			{
				pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
				if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					continue;
				}

				z_player_leave_map(pstAppCtx, pstEnv, pstTmpPlayer, iChgMap);

				pstTmpPlayer->stOnline.iPosSeq += 1000;
				pstEnterMap->InitMoveSeq = pstTmpPlayer->stOnline.iPosSeq;

				pstTmpPlayer->stOnline.bDir = bDirection;
				pstEnterMap->Dir = pstTmpPlayer->stOnline.bDir;

				Z_CSHEAD_INIT(&pstPkg->Head, ENTER_MAP);
				z_cltmsg_send(pstEnv, pstTmpPlayer, pstPkg);
				
				z_player_entermap(pstAppCtx, pstEnv, pstTmpPlayer, iMapIndex, &stPos, iChgMap);
			}
		}
	}

	if (iChgMap)
	{
		player_team_follow_chg_map(pstEnv, pstPlayer,iMapIndex, pstPos,pstPworldInst);
		// 且地图广播一下组队跟随的信息
		pstTeam = player_team_get(pstEnv,  pstPlayer);
		if (pstTeam)
		{
			pstCap = player_team_get_captain_player(pstEnv,pstTeam);	
			if (pstCap && pstCap == pstPlayer)
			{
				player_team_follow_tick(pstEnv,pstTeam);
			}
		}
	}

	return iRet;
}

//应该进那一条线路的地图
int get_change_line_map(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, Player *pstPlayer)
{
	int iLine = 0;
	ZoneTeam *pstTeam;
	Monster *pstMon;

	if(pstPlayer->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID > 0)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID);
		if(pstMon && is_single_convoy_car(pstMon))
		{
			return z_find_map_byline(pstMapIdx, pstPlayer->stRoleData.MiscInfo.MapLine);
		}
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam)
	{
		iLine = pstTeam->iTeamMapLine;
	}
	else
	{
		iLine = pstPlayer->stRoleData.MiscInfo.MapLine;
	}

	return z_find_map_byline(pstMapIdx, iLine);
}

int z_player_change_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iMapIndex, RESPOS *pstPos, unsigned char bDirection, int iCameraDir)
{
	MapIndex *pstMapIdx;
	int iMapID = iMapIndex;
	ZONEPWORLDINST*pstZonePworldInst;
	int iRet;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapIndex);
	if(NULL == pstMapIdx)
	{
		return -1;
	}

	//多线地图
	if(is_refer_line_map(pstMapIdx))
	{
		iMapID = get_change_line_map(pstEnv, pstMapIdx, pstPlayer);
		//有可能要求的线路不存在了
		if(iMapID <= 0 )
		{
			iMapID = iMapIndex;
		}

		if(pstEnv->iMiscFlag & GAME_FLAG_IGNORE_REFER_LINE_MAP)
		{
			iMapID = iMapIndex;
		}
		
		if (iMapIndex == pstPlayer->stOnline.stAutoMoveInfo.iMapID )
		{
			pstPlayer->stOnline.stAutoMoveInfo.iMapID = iMapID;
		}
	}

	if(0 > player_chg_map_passenger_check( pstEnv, pstPlayer, pstMapIdx))
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPlayer->stRoleData.MiscInfo.PworldInstID);
	if (pstZonePworldInst && pstZonePworldInst->ullPworldWID == pstPlayer->stRoleData.MiscInfo.PworldWID &&
		pstZonePworldInst->iType != PWORLD_TYPE_TEAM &&
		pstMapIdx->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		//出副本时清理下不是队伍副本的实例
		pstPlayer->stRoleData.MiscInfo.PworldInstID = 0;
	}

	if (iMapID == SHZ_READY_MAP_ID || iMapID == ZHZ_READY_MAP_ID ||
		iMapID == JNZ_READY_MAP_ID || iMapID == LHS_READY_MAP_ID ||
		iMapID == LSZ_READY_MAP_ID || iMapID == HMZ_READY_MAP_ID ||
		iMapID == SYZ_READY_MAP_ID || iMapID == YLS_READY_MAP_ID ||
		(GET_PWORLDID(iMapID) == DIANFENG_PWORLD_ID))
	{
		g_EnterDianFeng = 1;
	}

	iRet = z_player_change_map2(pstAppCtx, pstEnv, pstPlayer, 
		iMapID, pstPos, bDirection, iCameraDir);

	g_EnterDianFeng = 0;

	return iRet;
}

int z_new_mainworld_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	MapIndex *pstMapIdx;
	MapObj *pstMapObj = pstEnv->pstMapObj;

	for (i=0; i<pstMapObj->iMapIndex; i++)
	{
		pstMapIdx = (MapIndex *)pstMapObj->sMapIdx[i];
		if (PWORLD_MAPID_BASE > pstMapIdx->stMapDef.MapID)
		{
			if (0 > z_new_map_inst(pstAppCtx, pstEnv, pstMapIdx->stMapDef.MapID))
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "new map %d inst error", pstMapIdx->stMapDef.MapID);
				exit(-1);
			}
		}
	}
	
	return 0;
}

int z_load_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	MAPNPC *pstMapNpc;
	int iMapID = -1;
	int iStart=0;
	MapIndex *pstMapIdx = NULL;
	MapObj *pstMap = pstEnv->pstMapObj;
	int iMapPos;
	UNUSED(pstAppCtx);

	for (i=0; i<pstEnv->pstMapObj->iMapNpcDef; i++)
	{
		pstMapNpc = (MAPNPC *)pstEnv->pstMapObj->sMapNpcDef[i];
		
		if (-1 == iMapID)
		{
			iMapID = pstMapNpc->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapNpc->MapID);
			iStart = 0;
		}

		if (iMapID != (int)pstMapNpc->MapID)
		{
			if (pstMapIdx)
			{
				pstMapIdx->iNpcStart = iStart;
				pstMapIdx->iNpcCount = i-iStart;
			}

			iMapID = pstMapNpc->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapNpc->MapID);
			iStart = i;
		}
		
	}

	if (pstMapIdx)
	{
		pstMapIdx->iNpcStart = iStart;
		pstMapIdx->iNpcCount = i - iStart;
	}

	return 0;
}

int z_load_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	MAPMONSTER *pstMapMonster;
	int iMapID = -1;
	int iStart=0;
	MapIndex *pstMapIdx = NULL;
	MapObj *pstMap = pstEnv->pstMapObj;
	int iMapPos;
	UNUSED(pstAppCtx);

	for (i=0; i<pstEnv->pstMapObj->iMapMonsterDef; i++)
	{
		pstMapMonster = (MAPMONSTER *)pstEnv->pstMapObj->sMapMonsterDef[i];
		
		if (-1 == iMapID)
		{
			iMapID = pstMapMonster->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapMonster->MapID);
			iStart = 0;
		}

		if (iMapID != (int)pstMapMonster->MapID)
		{
			if (pstMapIdx)
			{
				pstMapIdx->iMonsterStart = iStart;
				pstMapIdx->iMonsterCount = i - iStart;
			}

			iMapID = pstMapMonster->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapMonster->MapID);
			iStart = i;
		}
		
	}

	if (pstMapIdx)
	{
		pstMapIdx->iMonsterStart = iStart;
		pstMapIdx->iMonsterCount = i - iStart;
	}

	return 0;
}

int z_load_ore_position(ZONESVRENV* pstEnv)
{
	int i;
	MAPOREPOSITIONDEF *pstMapOrePositionDef;
	int iMapID = -1;
	int iStart=0;
	MapIndex *pstMapIdx = NULL;
	MapObj *pstMap = pstEnv->pstMapObj;
	int iMapPos;

	for (i=0; i<pstEnv->pstMapObj->iMapOrePositionDef; i++)
	{
		pstMapOrePositionDef = (MAPOREPOSITIONDEF *)pstEnv->pstMapObj->sMapOrePositionDef[i];
		
		if (-1 == iMapID)
		{
			iMapID = pstMapOrePositionDef->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapOrePositionDef->MapID);
			iStart = 0;
		}

		if (iMapID != (int)pstMapOrePositionDef->MapID)
		{
			if (pstMapIdx)
			{
				pstMapIdx->iOrePositionStart = iStart;
				pstMapIdx->iOrePositionCount = i - iStart;
			}

			iMapID = pstMapOrePositionDef->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapOrePositionDef->MapID);
			iStart = i;
		}
		
	}

	if (pstMapIdx)
	{
		pstMapIdx->iOrePositionStart = iStart;
		pstMapIdx->iOrePositionCount = i - iStart;
	}

	return 0;
}

int z_load_tmp_item(ZONESVRENV* pstEnv)
{
	int i;
	MAPTMPITEMDEF *pstMapTmpItemDef;
	int iMapID = -1;
	int iStart=0;
	MapIndex *pstMapIdx = NULL;
	MapObj *pstMap = pstEnv->pstMapObj;
	int iMapPos;

	for (i=0; i<pstEnv->pstMapObj->iMapTmpItemDef; i++)
	{
		pstMapTmpItemDef = (MAPTMPITEMDEF *)pstEnv->pstMapObj->sMapTmpItemDef[i];
		
		if (-1 == iMapID)
		{
			iMapID = pstMapTmpItemDef->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapTmpItemDef->MapID);
			iStart = 0;
		}

		if (iMapID != (int)pstMapTmpItemDef->MapID)
		{
			if (pstMapIdx)
			{
				pstMapIdx->iTmpItemStart = iStart;
				pstMapIdx->iTmpItemCount = i - iStart;
			}

			iMapID = pstMapTmpItemDef->MapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapTmpItemDef->MapID);
			iStart = i;
		}
		
	}

	if (pstMapIdx)
	{
		pstMapIdx->iTmpItemStart = iStart;
		pstMapIdx->iTmpItemCount = i - iStart;
	}

	return 0;
}

int z_load_map_trans(ZONESVRENV* pstEnv)
{
	int i;
	MAPTRANSDEF *pstMapTransDef;
	int iMapID = -1;
	int iStart=0;
	MapIndex *pstMapIdx = NULL;
	MapObj *pstMap = pstEnv->pstMapObj;
	int iMapPos;

	for (i=0; i<pstEnv->pstZoneObj->iMapTransDef; i++)
	{
		pstMapTransDef = (MAPTRANSDEF *)pstEnv->pstZoneObj->sMapTransDef[i];
		
		if (-1 == iMapID)
		{
			iMapID = pstMapTransDef->FromMapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapTransDef->FromMapID);
			iStart = 0;
		}

		if (iMapID != pstMapTransDef->FromMapID)
		{
			if (pstMapIdx)
			{
				pstMapIdx->iMapTransStart = iStart;
				pstMapIdx->iMapTransCount = i - iStart;
			}

			iMapID = pstMapTransDef->FromMapID;
			iMapPos = -1;
			pstMapIdx = z_mappos_index(pstMap, &iMapPos, pstMapTransDef->FromMapID);
			iStart = i;
		}
		
	}

	if (pstMapIdx)
	{
		pstMapIdx->iMapTransStart = iStart;
		pstMapIdx->iMapTransCount = i - iStart;
	}

	return 0;
}



int z_clear_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstZoneMapInst;
	ZONEIDX stIdx;

	//先清副本
	for (i=0; i<MAX_PWORLD_INST; i++)
	{
		pstZonePworldInst = tmempool_get_bypos(pstEnv->pstPworldInstPool, i);
		if (!pstZonePworldInst)
		{
			continue;
		}

		z_in_destroy_pworld_inst(pstAppCtx, pstEnv, pstZonePworldInst);		
	}

	//再清主世界
	for (i=0; i<MAX_MAP_INST; i++)
	{
		pstZoneMapInst = tmempool_get_bypos(pstEnv->pstMapInstPool, i);
		if (!pstZoneMapInst)
		{
			continue;
		}

		stIdx.iID = pstZoneMapInst->iMapInstID;
		stIdx.iPos = -1;
		z_in_destroy_map_inst(pstAppCtx, pstEnv, NULL, NULL, &stIdx);	
	}
	
	return 0;
}


void z_init_mapdyn(ZONESVRENV* pstEnv)
{
	pstEnv->pstMapObj->iArea = 0;
	pstEnv->pstMapObj->iMask = 0;
	pstEnv->pstMapObj->iLastID = 0;

	return ;
}

int z_player_enter_world(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, i);
				
		if (pstPlayer && (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus || PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus))
		{
			if (CS_STAT_AUTOFLY & pstPlayer->stOnline.State)
			{
				continue;
			}
			
			z_player_entermap(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Map, &pstPlayer->stRoleData.Pos, 0);
		}
	}
	
	return 0;
}

int z_player_leave_world(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, i);
				
		if (pstPlayer && (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus || PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus))
		{
			if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //副本中的玩家
			{
				player_pworld_exit_chgmap(pstEnv, pstPlayer);
			}

			//拾取了地图刷新的临时道具
			if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID &&
				(pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemFlag & TMP_ITEM_FLAG_MAP_REFRESH))
			{
				tmp_item_drop(pstEnv,pstPlayer,1);
			}

			if(is_player_ride_on_machine(pstPlayer))
			{
				player_ride_machine_brk(pstEnv, pstPlayer, 1);
			}

			if (CS_STAT_AUTOFLY & pstPlayer->stOnline.State)
			{
				continue;
			}
			
			z_player_leave_map(pstAppCtx, pstEnv, pstPlayer, 0);
		}
	}
	
	return 0;
}

int z_clear_all_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	DropItem *pstDropItem;
	UNUSED(pstAppCtx);
	
	for (i=0; i<MAX_ITEM; i++)
	{
		pstDropItem = tmempool_get_bypos(pstEnv->pstItemPool, i);
		if (!pstDropItem)
		{
			continue;
		}

		if (DROP_ITEM_STAY == pstDropItem->cStat)
		{
			z_item_clean(pstEnv, pstDropItem);
		}
		else
		{
			tmempool_free(pstEnv->pstItemPool, pstDropItem->iMIndex);	
		}
	}

	return 0;
}

int z_clear_load_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	z_init_mapdyn(pstEnv);
		
 	z_load_msk_proc(pstAppCtx, pstEnv);
	z_load_npc(pstAppCtx, pstEnv);
	z_load_monster(pstAppCtx, pstEnv);
	z_load_ore_position(pstEnv);
	z_load_tmp_item(pstEnv);
	z_load_map_trans(pstEnv);

	z_player_leave_world(pstAppCtx, pstEnv);
	z_clear_all_npc(pstAppCtx, pstEnv);
	z_clear_all_monster(pstAppCtx, pstEnv);
	z_clear_all_item(pstAppCtx, pstEnv);
	z_clear_map(pstAppCtx, pstEnv);	

	if (pstEnv->cReladPworld) //重新初始化ZONEPWORLD
	{
		if (0 > z_init_pworld(pstAppCtx, pstEnv))
		{
			printf("z_init_pworld fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_init_pworld fail");
		}
	}

	z_new_mainworld_map_inst(pstAppCtx, pstEnv);
	z_player_enter_world(pstAppCtx, pstEnv);

	// 重新生成公会城市副本
	clan_init_all_city_pworld(pstEnv);

	global_world_ore_refresh(pstEnv);

	//清理MEMID，建筑实例已经不存在了
	global_city_build_online_clear(pstEnv);
	world_city_refresh(pstEnv);

	boss_data_init(pstEnv);

	home_init_all_pworld(pstEnv);
	return 0;
}

int z_reload_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	z_load_msk_proc(pstAppCtx, pstEnv);
	z_load_npc(pstAppCtx, pstEnv);
	z_load_monster(pstAppCtx, pstEnv);
	z_load_ore_position(pstEnv);
	z_load_tmp_item(pstEnv);
	z_load_map_trans(pstEnv);

	if (pstEnv->cReladPworld) //重新初始化ZONEPWORLD
	{
		if (0 > z_init_pworld(pstAppCtx, pstEnv))
		{
			printf("z_init_pworld fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_init_pworld fail");
		}
	}
	
	return 0;
}

int z_init_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	if (pstEnv->cReloadMap)
	{
		return z_clear_load_map(pstAppCtx, pstEnv);
	}
	else
	{
		return z_reload_map(pstAppCtx, pstEnv);
	}
}

int z_init_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i,j;
	ZONEPWORLD *pstPworld;
	MapIndex *pstMapIndex;
	int iMapID;
	int iPworldID;
	MapObj *pstMap = pstEnv->pstMapObj;
	UNUSED(pstAppCtx);
	

	for (i=0; i<pstMap->iPworld; i++)
	{
		pstPworld = (ZONEPWORLD *)pstMap->sPworld[i];
		pstPworld->iArea = 0;
		pstPworld->iMap = 0;
	}

	for(i=0; i<pstMap->iMapIndex; i++)
	{
		pstMapIndex	=	(MapIndex *)&pstMap->sMapIdx[i];
		iMapID		=	pstMapIndex->stMapDef.MapID;
		if (iMapID < PWORLD_MAPID_BASE)
			continue;
		
		iPworldID	=	GET_PWORLDID(iMapID);

		for(j=0; j<pstMap->iPworld; j++)
		{
			pstPworld	=	(ZONEPWORLD*)pstMap->sPworld[j];

			if( (int)pstPworld->stPworldDef.PworldID==iPworldID )
			{
				if (pstPworld->iMap >= MAX_PWORLD_MAP)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "pworld %d have too much map", iPworldID);
					return -1;
				}
				
				pstPworld->astMapIdx[pstPworld->iMap].iID	=	iMapID;
				pstPworld->astMapIdx[pstPworld->iMap].iPos	=	i;
				pstPworld->iMap++;
				pstPworld->iArea	+=	1.0*pstMapIndex->iXAreaNum*pstMapIndex->iYAreaNum;

				break;
			}
		}
	}

	return 0;
}

int z_check_pworld_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i,j;
	ZONEPWORLDINST *pstPworldInst;
	ZONEMAPINST *pstMapInst;

	for (i=0; i<MAX_PWORLD_INST; i++)
	{
		pstPworldInst = tmempool_get_bypos(pstEnv->pstPworldInstPool, i);
		if (!pstPworldInst)
		{
			continue;
		}

		for (j=0; j<pstPworldInst->iMapInst; j++)
		{
			pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, pstPworldInst->astMapInstIdx+j);
			if (NULL == pstMapInst)
			{
				char szNotify[256];
				
				tlog_error(pstEnv->pstLogCat, 0, 0, "pworld inst %d map inst %d", pstPworldInst->iPworldInstID, (pstPworldInst->astMapInstIdx+j)->iID);

				snprintf(szNotify, sizeof(szNotify), "pworld inst %d map inst %d", pstPworldInst->iPworldInstID, (pstPworldInst->astMapInstIdx+j)->iID);
				z_game_notify(pstEnv, szNotify, CCH(szNotify));
			}
		}
	}
	
	return 0;
}

int z_check_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{	
	int i;
	ZONEMAPINST *pstMapInst;
	UNUSED(pstAppCtx);
	
	for (i=0; i<MAX_MAP_INST; i++)
	{
		pstMapInst = tmempool_get_bypos(pstEnv->pstMapInstPool, i);
		if (!pstMapInst)
		{
			continue;
		}

		if (0 != pstMapInst->iPworldInstID && NULL == z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID))
		{
			char szNotify[256];
			
			tlog_error(pstEnv->pstLogCat, 0, 0, "map inst %d have no pworld inst", pstMapInst->iMapInstID);

			snprintf(szNotify, sizeof(szNotify), "map inst %d have no pworld inst", pstMapInst->iMapInstID);
			z_game_notify(pstEnv, szNotify, CCH(szNotify));
		}
	}
	
	return 0;
}

int z_check_area_dyn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
	Player* pstPeer;
	Npc* pstNpc;
	Monster* pstMon;
	int iDyn;
	int i,iID;
	int j,k;
	int iIndex;
	char cType;
	int iUnknown, iBadNpc, iBadMon, iBadPlayer, iBadItem;
	int iDynMskCount = 0;
	RESPOS stPos;
	MapIndex *pstMapIdx;
	int aiAnimate[MAX_AREA_ANIMATE];
	int iValid;
			
	iUnknown =	0;
	iBadPlayer	=	0;
	iBadMon	=	0;
	iBadNpc	=	0;
	iBadItem = 0;

	for (i=0; i<MAX_MAP_INST; i++)
	{
		pstMapInst = tmempool_get_bypos(pstEnv->pstMapInstPool, i);
		if (!pstMapInst)
		{
			continue;
		}

		for (j=0; j<pstMapInst->iAreaDyn; j++)
		{
			iDyn	=	pstMapInst->aiAreaDyn[j];

			if( iDyn<0)
			{
				//tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
				continue;
			}

			pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
			if (NULL == pstAreaDyn)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
				continue;
			}

			if (pstAreaDyn->iDynMskIdx >= 0)
			{
				iDynMskCount++;
			}

			iValid = 0;
			for(k=0; k<pstAreaDyn->iAnimate; k++)
			{
				if( k>=MAX_AREA_ANIMATE )
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
					break;
				}

				iID	=	pstAreaDyn->aiAnimate[k];
				iIndex	=	z_id_indextype(pstAppCtx, pstEnv, iID, &cType);

				if( -1==iIndex )
				{
					iUnknown++;
					tlog_error(pstEnv->pstLogCat, 0, 0, "unknown id %d, map %d", iID, pstMapInst->iMapInstID);
					continue;
				}

				switch(cType)
				{
					case OBJ_PLAYER:
						pstPeer	=	z_id_player(pstAppCtx, pstEnv, iID);
						if (NULL == pstPeer)
						{
							iBadPlayer++;
							tlog_error(pstEnv->pstLogCat, 0, 0, "map %d area player id=%d", pstMapInst->iMapInstID, iID);
							continue;
						}
						memcpy(&stPos, &pstPeer->stRoleData.Pos, sizeof(stPos));
						break;
					case OBJ_NPC:
						pstNpc	=	z_id_npc(pstAppCtx, pstEnv, iID);
						if (NULL == pstNpc)
						{
							iBadNpc++;
							tlog_error(pstEnv->pstLogCat, 0, 0, "map %d area npc id=%d", pstMapInst->iMapInstID, iID);
							continue;
						}
						memcpy(&stPos, &pstNpc->stMon.stCurrPos, sizeof(stPos));
						break;
					case OBJ_MONSTER:
						pstMon	=	z_id_monster(pstAppCtx, pstEnv, iID);
						if (NULL == pstMon)
						{
							iBadMon++;
							tlog_error(pstEnv->pstLogCat, 0, 0, "map %d area mon id=%d", pstMapInst->iMapInstID, iID);
							continue;
						}
						memcpy(&stPos, &pstMon->stCurrPos, sizeof(stPos));
						break;
					default:
						break;
				}

				pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
					
				if( pstAreaDyn==z_pos_area_dyn(pstMapIdx,  pstEnv->pstAreaDynPool, pstMapInst, &stPos) )
				{
					aiAnimate[iValid++] = iID;
					continue;
				}

				switch(cType)
				{
					case OBJ_PLAYER:
						iBadPlayer++;
						tlog_error(pstEnv->pstLogCat, 0, 0, "BadPlayer id %d, mapid %d pos (%d, %d)", 
									iID, pstMapInst->iMapInstID, stPos.X, stPos.Y);
						break;
					case OBJ_NPC:
						iBadNpc++;
						tlog_error(pstEnv->pstLogCat, 0, 0, "BadNpc id %d, mapid %d pos (%d, %d)", 
									iID, pstMapInst->iMapInstID, stPos.X, stPos.Y);
						break;
					case OBJ_MONSTER:
						iBadMon++;
						tlog_error(pstEnv->pstLogCat, 0, 0, "BadMon id %d, mapid %d pos (%d, %d)", 
									iID, pstMapInst->iMapInstID, stPos.X, stPos.Y);
						break;
					default:
						break;
				}
			}

			if (pstAreaDyn->iAnimate != iValid)
			{
				pstAreaDyn->iAnimate = iValid;
				memcpy(pstAreaDyn->aiAnimate, aiAnimate, iValid*sizeof(pstAreaDyn->aiAnimate[0]));
			}

			for (k=0; k<pstAreaDyn->iInanimate; k++)
			{
				if( k>=MAX_AREA_INANIMATE )
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iInanimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iInanimate);
					break;
				}

				iID	=	pstAreaDyn->aiInanimate[k];
				iIndex	=	z_id_indextype(pstAppCtx, pstEnv, iID, &cType);

				if( -1==iIndex )
				{
					iBadItem++;
					tlog_error(pstEnv->pstLogCat, 0, 0, "unknown id %d, map %d", iID, pstMapInst->iMapInstID);
					continue;
				}

				if (cType != OBJ_ITEM)
				{
					iBadItem++;
					tlog_error(pstEnv->pstLogCat, 0, 0, "unknown id %d, map %d", iID, pstMapInst->iMapInstID);
					continue;
				}
			}
		}
	}

	if (iUnknown | iBadPlayer | iBadMon | iBadNpc | iBadItem)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "Unknown:%d, BadPlayer:%d, BadNpc:%d, BadMon:%d, BadItem:%d", 
			iUnknown, iBadPlayer, iBadNpc, iBadMon, iBadItem);
		printf("Unknown:%d, BadPlayer:%d, BadNpc:%d, BadMon:%d, BadItem:%d\n", iUnknown, iBadPlayer, iBadNpc, iBadMon, iBadItem);
	}

	if (iDynMskCount != pstEnv->pstDynMskPool->iUsed)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iDynMskCount %d, pool used %d", iDynMskCount, pstEnv->pstDynMskPool->iUsed);
		printf("iDynMskCount %d, pool used %d\n", iDynMskCount, pstEnv->pstDynMskPool->iUsed);
	}

	return 0;
}

int z_check_one_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	ZONEMAPINST *pstMapInst;
	
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d %d have no map inst %d", pstMon->iID, pstMon->iDefIdx, pstMon->stMap.iID);
	}
	
	return 0;
}

int z_check_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Npc *pstNpc;

	for (i=0; i<MAX_NPC; i++)
	{
		pstNpc = (Npc *)tmempool_get_bypos(pstEnv->pstNpcPool, i);
		if (!pstNpc)
		{
			continue;
		}

		z_check_one_monster(pstAppCtx, pstEnv, &pstNpc->stMon);
	}
	
	return 0;
}

int z_check_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Monster *pstMon;

	for (i=0; i<MAX_MONSTER; i++)
	{
		pstMon = (Monster *)tmempool_get_bypos(pstEnv->pstMonsterPool, i);
		if (!pstMon)
		{
			continue;
		}

		z_check_one_monster(pstAppCtx, pstEnv, pstMon);
	}
	
	return 0;
}

//检查，泄漏可以及时发现
int z_check_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	if (pstEnv->pstMapObj->iMapInst != pstEnv->pstMapInstPool->iUsed)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "MapInst=%d, mempool used=%d", pstEnv->pstMapObj->iMapInst, pstEnv->pstMapInstPool->iUsed);
	}

	z_check_npc(pstAppCtx, pstEnv);
	z_check_monster(pstAppCtx, pstEnv);
	z_check_pworld_inst(pstAppCtx, pstEnv);
	z_check_map_inst(pstAppCtx, pstEnv);
	z_check_area_dyn(pstAppCtx, pstEnv);

	return 0;
}

int map_player_broadcast(ZONESVRENV* pstEnv, Player *pstMapPlayer, ZONEMAPINST* pstMapInst, int iEnterMap)
{
	int i, iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	ZONEPWORLDINST *pstPworldInst;
	Player *pstPlayer;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;

	if (is_player_ride_on_machine(pstMapPlayer))
	{
		return 0;
	}
		
	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst)
	{
		return -1;
	}

	Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);

	for (i = 0; i < pstPworldInst->iPlayer; i++) {
		if (i >= (int)(sizeof(pstPworldInst->aiPlayerID)/sizeof(pstPworldInst->aiPlayerID[0])))
		{
			break;
		}
			
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i], &cType);
		if (iIndex < 0 || cType != OBJ_PLAYER) continue;
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			continue;
		}
		pstPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || pstPlayer == pstMapPlayer)
		{
			continue;
		}

		if (iEnterMap)
		{
			z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMapPlayer);
			z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstMapPlayer, pstPlayer);
		}
		else
		{
			pstLeaveView->ID = pstPlayer->iMemID;
			pstLeaveView->HomeFlag = 0;
			z_cltmsg_send(pstEnv, pstMapPlayer, pstPkg);

			pstLeaveView->ID = pstMapPlayer->iMemID;
			pstLeaveView->HomeFlag = 0;
			z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
		}
	}

	return 0;
}

int map_mon_broadcast(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST* pstMapInst)
{
	int j, k, iDyn, iID, iIndex;
	AREADYN* pstAreaDyn;
	char cType;
	Monster *pstMon;
	CSMONSTER *pstCSMon;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	ZONEPWORLDINST* pstPworldInst = NULL;
	
	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);

	for (j=0; j<pstMapInst->iAreaDyn; j++)
	{
		iDyn	=	pstMapInst->aiAreaDyn[j];

		if( iDyn<0)
		{
			//tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(k=0; k<pstAreaDyn->iAnimate; k++)
		{
			if( k>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_MONSTER:
				case OBJ_NPC:
					pstMon	=	z_id_monster2(pstEnv->pstAppCtx, pstEnv, iID);
					if (NULL == pstMon)
					{
						tlog_error(pstEnv->pstLogCat, 0, 0, "area mon id=%d", iID);
						continue;
					}
						
					if ((pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD) ||
						pstMapInst->bBroadcast)
					{
						pstEnterView->ID = pstMon->iID;
						pstEnterView->x = pstMon->stCurrPos.X;
						pstEnterView->y = pstMon->stCurrPos.Y;
						pstEnterView->Dir = pstMon->bDirection;
						pstEnterView->Type = cType;

						if (OBJ_MONSTER == cType)
						{
							pstCSMon = &pstEnterView->Object.Monster;
							z_make_mon_view(pstEnv->pstAppCtx, pstEnv, pstCSMon, pstMon);
						}
						else
						{
							pstCSMon = &pstEnterView->Object.Monster;
							
							z_make_mon_view(pstEnv->pstAppCtx, pstEnv, pstCSMon, pstMon);
							//continue;

						}

						Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);

						z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

						if (pstMon->cMoving)
						{
							pstMoveOther->ID = pstMon->iID;
							pstMoveOther->x = pstMon->stCurrPos.X;
							pstMoveOther->y = pstMon->stCurrPos.Y;
							
							pstMoveOther->MoveType = CS_MOVE_WALK;
							pstWalk->Total = 1;

							pstWalk->Points[0].x = pstMon->stNextPos.X;
							pstWalk->Points[0].y = pstMon->stNextPos.Y;

							memcpy(&pstMoveOther->MspdInfo, &pstMon->stMspdInfo, sizeof(pstMoveOther->MspdInfo));
							
							Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

							z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
						}
					}
										
					break;

				default:
					break;
			}
		}
	}
			
	return 0;
}

int map_mon_broadcast_leave(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST* pstMapInst)
{
	int j, k, iDyn, iID, iIndex;
	AREADYN* pstAreaDyn;
	char cType;
	Monster *pstMon;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;
		
	for (j=0; j<pstMapInst->iAreaDyn; j++)
	{
		iDyn	=	pstMapInst->aiAreaDyn[j];

		if( iDyn<0)
		{
			//tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(k=0; k<pstAreaDyn->iAnimate; k++)
		{
			if( k>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_MONSTER:
				case OBJ_NPC:
					pstMon	=	z_id_monster2(pstEnv->pstAppCtx, pstEnv, iID);
					if (NULL == pstMon)
					{
						tlog_error(pstEnv->pstLogCat, 0, 0, "area mon id=%d", iID);
						continue;
					}

					if (pstMapInst->bBroadcast || (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
					{
						pstLeaveView->ID = iID;
						pstLeaveView->HomeFlag = 0;

						Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);
						
						z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
					}
										
					break;
				default:
					break;
			}
		}
	}
			
	return 0;
}

int z_player_entermap_pk_mode_chg(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex* pstMapIdx)
{
	UNUSED(pstEnv);
	UNUSED(pstPlayer);
	UNUSED(pstMapIdx);
	
	/*
	//地图PK规则限制
	if(pstMapIdx->stMapDef.MapLimit.PK == 0)
	{
		//安全地图只可以被动模式
		if(pstPlayer->stOnline.ePkMode != PK_MODE_NORMAL)
		{
			pstPlayer->stOnline.ePkMode = PK_MODE_NORMAL;
			player_pk_mode_action(pstEnv, pstPlayer, 0);
		}
	}
	else 
	{
		//中立地图 
		if(pstMapIdx->stMapDef.MapLimit.PKMode == 0)
		{
			
			//红名强制全体PK
			if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= KILL_PLAYER_NO_PKVAL)
			{
				if(pstPlayer->stOnline.ePkMode != PK_MODE_TEAM)
				{
					pstPlayer->stOnline.ePkMode = PK_MODE_TEAM;
					player_pk_mode_action(pstEnv, pstPlayer, 0);
				}
			}
			
			if(pstPlayer->stOnline.ePkMode != pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode)
			{
				pstPlayer->stOnline.ePkMode = pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode;
				player_pk_mode_action(pstEnv, pstPlayer, 0);
			}
		}
		//指定模式地图
		else
		{
			if(pstPlayer->stOnline.ePkMode != pstMapIdx->stMapDef.MapLimit.PKMode)
			{
				pstPlayer->stOnline.ePkMode = pstMapIdx->stMapDef.MapLimit.PKMode;
				player_pk_mode_action(pstEnv, pstPlayer, 0);
			}
		}
	}
	*/

	return 0;
}


int z_enter_pworld_check_pet(ZONESVRENV* pstEnv, Player *pstPlayer,ZONEPWORLD *pstZonePworld,int iTip)
{
	//地图不可出征宠物
	if (pstPlayer->stOnline.stPetOnline.bFightShiNum) 
	{
		if (	pstZonePworld && 
			(pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_PET_FIGHT))
		{
			if (pstPlayer->stOnline.stPetOnline.bFightShiNum > 0 && iTip)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP153);
			}

			if ( !is_pet_heti(pstPlayer))
			{
				z_player_backup_pet(pstEnv, pstPlayer);
			}
		}
	}
	return 0;
}

int z_enter_map_check_pet(ZONESVRENV* pstEnv, Player *pstPlayer,MapIndex* pstMapIdx,int iTip)
{
	if (	(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_PET_FIGHT ))
	{
		if (pstPlayer->stOnline.stPetOnline.bFightShiNum > 0 && iTip )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP153);
		}

		if ( !is_pet_heti(pstPlayer))
		{
			z_player_backup_pet(pstEnv, pstPlayer);
		}
	}
	return 0;
}


int player_chg_map_by_mapid(ZONESVRENV* pstEnv, Player *pstPlayer,int iMapID,RESPOS *pstPos)
{
	MapIndex *pstMapIdx;
	RESPOS stPos;

	if (pstPos)
	{
		stPos = *pstPos;
	}
	else
	{
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapID);
		if (NULL == pstMapIdx)
		{
			return -1;
		}
		
		stPos = pstMapIdx->stMapDef.RevivePs[0];
	}
	
	if (stPos.X <= 0 || stPos.Y <= 0 || !z_valid_pos(pstEnv->pstMapObj, iMapID, NULL, &stPos))
	{
		return -1;
	}

	z_player_change_map(pstEnv->pstAppCtx,  pstEnv, pstPlayer, iMapID, &stPos, RAND1(180), 0);		
	return 0;
}

int z_player_check_map_timelimit(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	// 时间到了 回时间城复活点
	if (	pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime > 0 &&
		pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		player_chg_map_by_mapid(pstEnv, pstPlayer,BASE_CITY_MAPID,NULL);
	}
	return 0;
}

int z_player_entermap_check(ZONESVRENV* pstEnv, Player *pstPlayer, 
								MapIndex* pstMapIdx, ZONEPWORLDINST* pstPworldInst, int iChg)
{
	ZONEPWORLD *pstZonePworld = NULL;

	if (pstPlayer->stOnline.State & CS_STAT_AUTOFLY)
	{
		pstPlayer->stOnline.State &= ~CS_STAT_AUTOFLY;
		pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0;
	}

	if (0 == iChg)
	{
		return 0;
	}

	if(pstPworldInst)
	{
		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	}

	//地图PK规则
	z_player_entermap_pk_mode_chg(pstEnv, pstPlayer, pstMapIdx);

	//地图限制
	if ((pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_FAIRY_F) ||
		(pstZonePworld && (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_FAIRY_F)))
	{
		if (pstPlayer->stOnline.stFairyOnline.ullCampWID > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_MAP154);
		}
		
		z_player_save_campfy_info(pstEnv, pstPlayer);
	}
	else
	{
		z_player_restore_fairy(pstEnv, pstPlayer);
	}

	if ((pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_PET_FIGHT) ||
		(pstZonePworld && (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_PET_FIGHT)))
	{
		z_enter_map_check_pet(pstEnv, pstPlayer,pstMapIdx,1);
		if(pstZonePworld)
		{
			z_enter_pworld_check_pet(pstEnv, pstPlayer, pstZonePworld, 1);
		}
	}
	else
	{
		z_player_restore_pet(pstEnv, pstPlayer);
	}


	if(pstPlayer->stOnline.State & (CS_STAT_RIDE_PREPARE |CS_STAT_RIDE_ON |CS_STAT_FLY))
	{
		RIDEDEF *pstRideDef;

		pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, 
									pstPlayer->stOnline.RideLevel);
		if(pstRideDef)
		{
			if(!is_valid_map_ride(pstMapIdx, pstRideDef))
			{
				z_ride_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer);
			}
		}
	}

	// fb 限制
	if(pstZonePworld)
	{
		//不可骑乘
		if (pstPlayer->stOnline.State & (CS_STAT_RIDE_PREPARE |CS_STAT_RIDE_ON |CS_STAT_FLY)) 
		{
			if ((pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE))
			{
				z_ride_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer);
			}
		}
		
		if (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_CLAN_ITEM)
		{
			if(pstPlayer->stOnline.ClanItemID>0)
			{
				player_drop_clan_tem(pstEnv,pstPlayer);
			}
		}

		if (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_WUSHUANG_SKILL)
		{
			if (pstPlayer->stOnline.cMiscFlag &MISC_FLAG_WUSHUANG)
			{
				z_player_del_wushuang_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_WUSHUANG;
			}
		}
	}
	else
	{
		if (	pstPlayer->stRoleData.Level < ROOKIE_PROTECT_LEVEL &&
			!(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_ROOKIE_PROTECT))
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_MAP_703);
		}
	}
	
	return 0;
}

int z_player_chg_map_check(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST* pstMapInst, int iOldMap)
{
	ZONEPWORLDINST* pstPworldInst;

	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		
	if ((CS_STAT_SIT|CS_STAT_GAOJI_SIT) & pstPlayer->stOnline.State)
	{
		player_sit_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if(iOldMap != pstPlayer->stRoleData.Map)
	{
		ZONEPWORLD *pstPworld;
		player_package_check_mapitem(pstEnv, pstPlayer);
		pstPworld = z_find_pworld2(pstEnv,iOldMap);
		if (pstPworld && pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
			player_pworld_exit_ctrl(pstEnv, pstPlayer,pstPworld);
	}	
	return 0;
}

int z_enter_map_check_safearea(ZONESVRENV* pstEnv, Player *pstPlayer,MapIndex* pstMapIdx)
{
	int iIsSafe = pstMapIdx->stMapDef.IsSafeMap;

	if (iIsSafe)
	{
		//if (!(pstPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_MAP))
		{
			pstPlayer->stOnline.cSecenOverLay |= SCENEOVERLAY_MAP;
			logic_area_action(pstEnv, pstPlayer, 0, pstPlayer->stOnline.cSecenOverLay);
			//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你已经进入安全地图");
		}
	}
	else
	{
		//if (	pstPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_MAP)
		{
			pstPlayer->stOnline.cSecenOverLay &= ~SCENEOVERLAY_MAP;
			//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你已经离开安全地图");
			logic_area_action(pstEnv, pstPlayer, 1, pstPlayer->stOnline.cSecenOverLay);
		}
	}
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}


int z_chgmap_check_qiecuo(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstQiecuo;
	if (pstPlayer->stOnline.stQiecuoInfo.iMemID <= 0)
		return 0;

	pstQiecuo = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);

	z_player_qiecuo_end(pstEnv, pstPlayer, 0);
	if (pstQiecuo)
		z_player_qiecuo_end(pstEnv, pstQiecuo, 1);
	return 0;
}

int z_map_newnpc_notify(ZONESVRENV* pstEnv, Player *pstPlayer, 
								ZONEMAPINST* pstMapInst)
{
	int i;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Npc *pstNpc;

	for(i=0; i<pstMapInst->iNewNpc; i++)
	{
		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstMapInst->aiNewNpc[i]);
		if(NULL == pstNpc)
		{
			continue;
		}
	
		pstActionRes->ID = pstNpc->stMon.iID;
		pstActionRes->ActionID = ACTION_OBJ_ENTER_MAP;

		pstActionRes->Data.ObjEnterMap.Type = OBJ_NPC;
		pstActionRes->Data.ObjEnterMap.DefID = pstNpc->stMon.iDefIdx;
		pstActionRes->Data.ObjEnterMap.x = pstNpc->stMon.stCurrPos.X;
		pstActionRes->Data.ObjEnterMap.y = pstNpc->stMon.stCurrPos.Y;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

int z_player_entermap(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iMapInstID, 
						RESPOS *pstPos, int iChg)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST* pstPworldInst = NULL;
	MapIndex* pstMapIdx;
	RESPOS stSearchPos;
	int iRet;
	int iProtect = 0;
	int iOldMap = pstPlayer->stRoleData.Map;
	RESPOS stPrePos = pstPlayer->stRoleData.Pos;
	int i = 0;
	
	stIdx.iID	=	iMapInstID;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, iMapInstID);
	if (!pstMapIdx)
		return -1;

	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);

	z_player_entermap_check(pstEnv, pstPlayer, pstMapIdx, pstPworldInst, iChg);
	
	
	pstPlayer->stRoleData.Map	= stIdx.iID;
	pstPlayer->stOnline.iMapPos = stIdx.iPos;
	pstPlayer->stRoleData.Pos.X = pstPos->X;
	pstPlayer->stRoleData.Pos.Y = pstPos->Y;	

	//记录线路
	if(is_refer_line_map(pstMapIdx))
	{
		pstPlayer->stRoleData.MiscInfo.MapLine = pstMapIdx->stMapDef.MapLine;

		//线路信息
		map_line_info(pstEnv, pstPlayer, pstMapIdx);
	}
	
	SET_PLAYER_NOMOVE(pstPlayer);
	z_player_move_stop(pstAppCtx, pstEnv, pstPlayer);
	//pstPlayer->stOnline.cCurrPoint = pstPlayer->stOnline.cPassPoints = pstPlayer->stOnline.cFuturePoints = 0;

	if (0 > (iRet = z_ani_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &pstPlayer->stRoleData.Pos, pstMapInst)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) (%d)z_ani_enter_area ret %d, map id %d, iXAreaNum %d, iYAreaNum %d, pos (x=%d y=%d)", 
					pstPlayer->stRoleData.Uin, pstPlayer->iMemID, iRet, pstMapIdx->stMapDef.MapID, pstMapIdx->iXAreaNum, pstMapIdx->iYAreaNum, pstPlayer->stRoleData.Pos.X,
					pstPlayer->stRoleData.Pos.Y);
		return -1;
	}

	map_mon_broadcast(pstEnv, pstPlayer, pstMapInst);

	// 切换地图,先清状态,再加状态
	if (iChg && (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_STATUS_CHGMAP_CLEAN))
	{
		pstPlayer->stOnline.iOldChangeMapID = iOldMap;
		Player_clean_matching_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,STATUS_CHG_MAP_CLEAN,MISC_FLAG_STATUS_CHGMAP_CLEAN);
		pstPlayer->stOnline.iOldChangeMapID = 0;
	}

	if( pstPworldInst )
	{
		ZONEPWORLD *pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (pstZonePworld)
		{
			//  进入Fb给状态
			if (	pstZonePworld->stPworldDef.CreateStatusID > 0 && 
				pstZonePworld->stPworldDef.CreateStatusLevel >0 &&
				pstZonePworld->stPworldDef.CreateStatusTime > 0 && iChg)
			{
				
				z_player_apply_status(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstZonePworld->stPworldDef.CreateStatusID,
								pstZonePworld->stPworldDef.CreateStatusLevel, pstZonePworld->stPworldDef.CreateStatusTime,
								1, 0);
				iProtect = 1;

				if (pstZonePworld->stPworldDef.PworldID == CLAN_CITY_PWORLD_ID)
				{
					pstEnv->iClanCityInitStatusID = pstZonePworld->stPworldDef.CreateStatusID;
					pstEnv->iClanCityInitStatusLevel = pstZonePworld->stPworldDef.CreateStatusLevel;
					pstEnv->iClanCityInitStatusTime = pstZonePworld->stPworldDef.CreateStatusTime;
				}
			}	
		}
	
		if (pstPworldInst->iPlayer < (int)(sizeof(pstPworldInst->aiPlayerID)/sizeof(pstPworldInst->aiPlayerID[0])))
		{
			pstPworldInst->aiPlayerID[pstPworldInst->iPlayer] = pstPlayer->iMemID;
			pstPworldInst->iPlayer++;
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "pworld inst player num");
		}


		if (pstPworldInst->bDelayScript & PWORLD_SCRIPT_PROC_START_MAP)
		{
			char szName[128];

			pstPworldInst->bDelayScript &= ~PWORLD_SCRIPT_PROC_START_MAP;
				
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;

			snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
			szName[sizeof(szName) - 1] = 0;
			
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_START);
		}

		//进入副本回调脚本
		if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_ENTER_MAP) && iChg)
		{
			char szName[128];
				
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;

			snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
			szName[sizeof(szName) - 1] = 0;
			
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_ENTER);
		}
	}

	if (iChg)
	{
		//客户端要在知道别人的阵营值前知道自己的阵营值
		player_enter_span_map_notify_msg(pstEnv,pstPlayer);
	}
	
	if (z_pos_search_pos(pstMapIdx, &pstPlayer->stRoleData.Pos, &stSearchPos))
		z_player_search_view(pstAppCtx, pstEnv, pstMapInst, &stSearchPos, pstPlayer);

	pstMapInst->iPlayerCount++;
	
	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &pstPlayer->stRoleData.Pos, 1);

	//切地图时间
	pstPlayer->stOnline.tRoleChgMap = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (pstMapInst->bBroadcast)
	{
		map_player_broadcast(pstEnv, pstPlayer, pstMapInst, 1);
	}

	player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_CHG_MAP);

	// 切换地图
	if (iChg)
	{
		pstPlayer->stOnline.iDieNum = 0;

		// 进入据点清除下数据
		if (pstPworldInst)
		{
			ZONEPWORLD *pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
			if (pstZonePworld && strong_is_normal(pstZonePworld))
			{
				memset(&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));
				pstPlayer->stOnline.stWarOL.tFightTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			}

			player_enter_poly_pworld_info(pstEnv, pstPworldInst, pstPlayer);
		}
		
		
		z_chgmap_check_qiecuo(pstEnv, pstPlayer);
		z_player_chg_map_check(pstEnv, pstPlayer, pstMapInst, iOldMap);

		task_player_trace( pstEnv, pstPlayer, TASK_OBJ_MAP);
			
		z_role_chg_map_oplog(pstEnv, pstPlayer, iOldMap, &stPrePos);

		if(!iProtect)
		{
			if(!(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_AREAPROTECTED))
			{
				z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					STATUS_ID_AREAPROTECTED, 1, MAP_AREA_PROTECTED_TIME, 1, 0);
			}
		}

		player_enter_beach(pstEnv,pstPlayer,iOldMap);
		player_enter_world_city(pstEnv,pstPlayer,iOldMap,&stPrePos);
		player_enter_span_map(pstEnv,pstPlayer,iOldMap,&stPrePos);

		
	}


	player_enter_world_city_info(pstEnv, pstPlayer);
	player_map_ore_info(pstEnv, pstPlayer);
	player_enter_map_add_machine(pstEnv, pstPlayer, pstMapIdx);
	
	if(pstPworldInst)
	{
		if (pstPworldInst->iType == PWORLD_TYPE_CLAN)
		{
			ZoneClan* pstZoneClan = z_id_clan(pstEnv,  pstPworldInst->stPworldTypeData.stClan.iID);
			if (pstZoneClan)
			{
				player_clan_city_update(pstEnv,pstPlayer,pstZoneClan);
			}

			clan_pet_player_enter_city(pstEnv, pstPlayer);
		}
	}

	z_enter_map_check_safearea(pstEnv, pstPlayer,pstMapIdx);

	z_map_newnpc_notify(pstEnv, pstPlayer, pstMapInst);

	//游戏事件
	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		GameEventInst *pstEvent = NULL;
		SHITEM * pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
	            continue;
	       }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		if(pstEvent->cScriptFlag & GAME_EVENT_SCRIPT_PROC_ENTER_MAP_MAP)
		{
			stSEnv.pstEvent= pstEvent;
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;
									
			z_exec_script(pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_ENTER_MAP);

		}
	}

	if (iChg && (
			(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY_FINI) ||
			(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY) ||
			(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY) ||
			(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY_FINI)
			))
	{
		player_action_view_flag_chg(pstEnv,pstPlayer);
	}

	// 称号
	if (g_iDesigVar == 2)
	{
		player_desig_get(pstEnv, pstPlayer, GET_DESIF_ENTER_JAIN_NUM);
		g_iDesigVar = 0;
	}
	
	return 0;
}

int z_player_leave_battle_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST* pstPworldInst)
{
	ZONEPWORLD *pstPworld = NULL;
	
	if (!pstPworldInst)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld || !is_battle_pworld(pstPworld))
	{
		return -1;
	}

	//逃跑BUFF
	z_add_runaway_buff(pstEnv, pstPlayer);
		
	if (pstPworld->stPworldDef.CampNum > 1 &&
		is_normal_battle_pworld(pstPworld))
	{
		if(pstPworldInst->stPworldCamp.iCampNum > 0 && pstPlayer->stOnline.stWarOL.cCampIdx > 0)
		{
			int iCamp =  pstPlayer->stOnline.stWarOL.cCampIdx - 1;
			PworldCamp *pstPworldCamp;

			pstPworldCamp = &pstPworldInst->stPworldCamp;
			
			pstPworldCamp->stCampInfo[iCamp].iPlayerNum--;
			if(pstPworldCamp->stCampInfo[iCamp].iPlayerNum < 0)
			{
				pstPworldCamp->stCampInfo[iCamp].iPlayerNum = 0;
			}
			
			pstPworldCamp->stCampInfo[iCamp].iCampLv -= pstPlayer->stOnline.stWarOL.iLevel;
			if(pstPworldCamp->stCampInfo[iCamp].iCampLv < 0)
			{
				pstPworldCamp->stCampInfo[iCamp].iCampLv = 0;
			}
		}
	}

	memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));

	return 0;
}

int z_player_leave_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iChgMap)
{
	int i, iIndex;
	char cType;
	Monster *pstVMon;
	Player *pstVPlayer;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST* pstPworldInst;
	
	TMEMBLOCK *pstMemBlock;
	ZONEIDX stIdx;
	MapIndex *pstMapIndex;
	int iRet;
	RESPOS stPos = pstPlayer->stRoleData.Pos;


	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if( !pstMapInst )
		return -1;

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIndex)
		return -1;
	
	if( 0>z_ani_leave_area_test(pstMapIndex, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &pstPlayer->stRoleData.Pos, pstMapInst))
	{
		return -1;
	}
		
	//think about pworld_again and reloadall
	map_mon_broadcast_leave(pstEnv, pstPlayer, pstMapInst);

	/*if (iChgMap)
	{
		//客户端表现需要，暂时不会清除离开视野的人物等，ENTER_MAP时在表现
		player_leave_map_action(pstEnv, pstPlayer);
	}*/
	
	if (pstMapInst->bBroadcast)
	{
		map_player_broadcast(pstEnv, pstPlayer, pstMapInst, 0);
	}

	cType = OBJ_UNKNOW;
	for (i = 0; i < pstPlayer->stOnline.iVAnimate;) 
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i], &cType);
		if (iIndex >= 0) 
		{
			switch (cType)
			{
				case OBJ_NPC:
				case OBJ_MONSTER:
					if (cType == OBJ_NPC) 
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
					}
					else
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					}

					pstVMon = NULL;
					
					if (!pstMemBlock->fValid)
					{
						break;
					}

					pstVMon = (Monster *)pstMemBlock->szData;

					//if (OBJ_MONSTER == cType)
					{
						mon_press_delete_player(pstEnv, pstVMon, pstPlayer);
					}
										
					bdelete_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int));
					break;
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					
					pstVPlayer = (Player *)pstMemBlock->szData;
					z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 1, pstPlayer->iMemID, OBJ_PLAYER, 0);
					break;
			}
		}

		if (iChgMap)
		{
			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, pstPlayer->stOnline.aiVAnimate[i], cType, 2) == 0) 
				continue;
		}
		else
		{
			if (cType == OBJ_MONSTER && pstVMon && machine_get_by_player( pstEnv, pstPlayer) == pstVMon)
			{
				bdelete_int(&pstPlayer->stOnline.aiVAnimate[i], &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int));
				continue;
			}
			
			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, pstPlayer->stOnline.aiVAnimate[i], cType, 0) == 0) 
				continue;
		}
		
		i++;
	}

	for (i = pstPlayer->stOnline.iVInanimate-1; i >=0; i--) 
	{
		z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 0, 
				pstPlayer->stOnline.aiVInanimate[i], OBJ_UNKNOW, 1);
	}
	
	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);

	if (iChgMap)
	{
		//z_player_leave_battle_pworld(pstEnv, pstPlayer, pstPworldInst);

		//离开活动副本(包含战场)
		z_player_leave_poly_pworld(pstEnv, pstPlayer, pstPworldInst);

		/*改变地图 把临时物品卸下来 */
		if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
		{	
			tmp_item_drop(pstEnv, pstPlayer,1);			
		}

		player_leave_map_machine_clear(pstEnv, pstMapIndex, pstPlayer);

		// 改变地图把自己的图腾删除掉
		player_totem_chg_map(pstEnv, pstPlayer);
		player_leave_beach(pstEnv,pstPlayer);
		player_leave_world_city(pstEnv,pstPlayer);

		z_chgmap_check_qiecuo(pstEnv, pstPlayer);

		losttotem_free( pstEnv,pstPlayer)	; // 切地图删除losttotem

		if(1 == iChgMap && g_EnterDianFeng== 0)
		{
			player_leave_dianfeng_repose(pstEnv,pstPlayer);
		}

		if (1 == iChgMap)
		{
			//迷之塔 去除特定坏buf
			poly_mzt_del_status(pstEnv, pstPlayer,pstPlayer->stRoleData.Map,MZT_THREE_LAYER_BAD_STATUS);

			if(0 == player_if_in_span_map(pstEnv,pstPlayer))
			{
				player_pk_mode_chg(pstEnv->pstAppCtx, pstEnv, 
						pstPlayer, pstPlayer->stRoleData.MiscInfo.PkInfo.SavePkMode);

				if(0 == g_EnterShiKong)
				{
					//一离开跨服野外就取消报名或等待队列
					//当进入时空副本时会去除报名或等待队列，这里不让调用，比较符合逻辑
					shikong_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stShiKongQueueInfo.iPworldID, 1);
				}
			}
		}		
	}

	//tmp_item_drop 有可能发成change_map, 踩到传送区域。就不重复做下面的事情了
	if( 0>z_ani_leave_area_test(pstMapIndex, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &stPos, pstMapInst))
	{
		return -2;
	}

	pstMapInst->iPlayerCount--;
	
	if( pstPworldInst )
	{
		LIMITPWORLD *pstLimitPworld=NULL;
		for (i=0; i<pstPworldInst->iPlayer; i++)
		{
			if (pstPworldInst->aiPlayerID[i] == pstPlayer->iMemID)
			{
				if (i != pstPworldInst->iPlayer-1)
				{
					memmove(&pstPworldInst->aiPlayerID[i], &pstPworldInst->aiPlayerID[i+1], 
								(pstPworldInst->iPlayer-1-i)*sizeof(pstPworldInst->aiPlayerID[0]));
				}
				
				break;
			}
		}
		
		pstPworldInst->iPlayer--;
		if(pstPworldInst->iPlayer < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "player num %d", pstPworldInst->iPlayer);
			pstPworldInst->iPlayer = 0;
		}

		//严格判断删除次数的约定,必须是第一次进入这个副本,并且有限制次数
		pstLimitPworld = player_check_same_pworld_by_wid(pstEnv, pstPworldInst,pstPlayer);
		if( iChgMap &&
			(pstPlayer->stRoleData.MiscInfo.InPworldSafeTime > 0 || pstPworldInst->chLimitFlag == 0 ) &&
			pstLimitPworld &&
			pstLimitPworld->InstTime == pstPlayer->stOnline.tRoleChgMap )
		{
			//退出游戏就不在补回了。当做消耗掉了次数
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGOUT_REQ)
			{
				ZONEPWORLD *pstZPworld;
				pstZPworld = z_find_pworld2(pstEnv,  pstPlayer->stRoleData.Map);
				if (pstZPworld &&  pstZPworld->stPworldDef.DynType)
				{
					player_insert_limit_pworld(pstEnv, pstPlayer, pstPworldInst,1);	
				}
				
				//player_insert_limit_pworld(pstEnv, pstPlayer, pstPworldInst,1);
				pstPlayer->stRoleData.MiscInfo.InPworldSafeTime = 0;
			}
		}
	}

	if( 0 > (iRet = z_ani_leave_area(pstMapIndex, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &pstPlayer->stRoleData.Pos, pstMapInst)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) (%d)z_ani_leave_area ret %d, map id %d, iXAreaNum %d, iYAreaNum %d, pos (x=%d y=%d)", 
					pstPlayer->stRoleData.Uin, pstPlayer->iMemID, iRet, pstMapIndex->stMapDef.MapID, pstMapIndex->iXAreaNum, pstMapIndex->iYAreaNum, pstPlayer->stRoleData.Pos.X,
					pstPlayer->stRoleData.Pos.Y);
		return -1;
	}

	return 0;
}

int z_valid_pos(MapObj *pstMap, int iMapIndex, int *piMapPos, RESPOS *pstPos)
{
	MapIndex *pstIdx;
	TMAPMASK *pstMask;

	pstIdx = z_mappos_index(pstMap, piMapPos, iMapIndex);
	if (!pstIdx) return 0;
	if (pstIdx->iMaskOrigin < 0) return 0;
	pstMask = (TMAPMASK *)(pstMap->sMask + pstIdx->iMaskOrigin);
	if (IsPixelStop(pstMask, pstPos->X, pstPos->Y)) return 0;
	if (NULL == z_pos_area(pstMap, pstIdx, pstPos)) return 0;
	return 1;
}

int z_get_ran_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, RESPOS* pstCurr, 
					RESPOS* pstOrigin, int iMaxDist, int iMinDist, RESPOS* pstPos)
{
	RESPOS stPos;
	TMAPMASK *pstMask;
	int i;
	int iDist;
	int iDir;
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if( iMaxDist<=0 || iMinDist > iMaxDist)
	{
		if( IsPixelLineThrough(pstMask, pstCurr->X, pstCurr->Y, pstOrigin->X, pstOrigin->Y, maskStop) &&
			dyn_msk_check(pstEnv, pstMapInst, pstMask, pstCurr->X, pstCurr->Y, pstOrigin->X, pstOrigin->Y,0, DYN_MSK_STOP))
		{
			memcpy(pstPos, pstOrigin, sizeof(*pstPos));
			return 0;	
		}
		else
		{
			memcpy(pstPos, pstCurr, sizeof(*pstPos));
			return -1;
		}
	}
	
	if ( !pstMapIdx)
	{
		memcpy(pstPos, pstCurr, sizeof(*pstPos));

		return -1;
	}

	for(i=0; i<MAX_TRY_POSITION; i++)
	{
		iDist = iMinDist + RAND1(iMaxDist-iMinDist+1);

		//计算分布角度 园三等分，再6等分
		iDir =( g_RanPosDirCurr +(g_RanPosFaceCurr%3*120 + g_RanPosFaceIdx*20))%360;
		g_RanPosFaceCurr++;
		if(g_RanPosFaceCurr%3 == 0 && g_RanPosFaceCurr!= 0)
		{
			g_RanPosFaceIdx = (g_RanPosFaceIdx+1)%6;
		}
		
		stPos.X=	pstOrigin->X + cosf(iDir*PI/180.0f)*iDist;
		stPos.Y=	pstOrigin->Y - sin(iDir*PI/180.0f)*iDist;

		if(stPos.X<0 || stPos.X>=pstMapIdx->stEnd.X)
			continue;

		if(stPos.Y<0 || stPos.Y>=pstMapIdx->stEnd.Y)
			continue;

		if( IsPixelLineThrough(pstMask, pstCurr->X, pstCurr->Y, stPos.X, stPos.Y, maskStop) &&
			dyn_msk_check(pstEnv, pstMapInst, pstMask, pstCurr->X, pstCurr->Y, stPos.X, stPos.Y,0, DYN_MSK_STOP) &&
			z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stPos))
		{
			pstPos->X	=	stPos.X;
			pstPos->Y	=	stPos.Y;

			return 0;
		}
	}

	memcpy(pstPos, pstCurr, sizeof(*pstPos));

	return -1;
}

/*
int z_get_ran_pos(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, RESPOS* pstCurr, RESPOS* pstOrigin, int iMaxDist, int iMinDist, RESPOS* pstPos)
{
	int x, y;
	RESPOS stPos;
	TMAPMASK *pstMask;
	int i;
	int j;
	int n;
	int iDist;
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if( iMaxDist<=0 || iMinDist > iMaxDist)
	{
		if( IsPixelLineThrough(pstMask, pstCurr->X, pstCurr->Y, pstOrigin->X, pstOrigin->Y, maskStop) )
		{
			memcpy(pstPos, pstOrigin, sizeof(*pstPos));
			return 0;	
		}
		else
		{
			memcpy(pstPos, pstCurr, sizeof(*pstPos));
			return -1;
		}
	}
	
	if ( !pstMapIdx)
	{
		memcpy(pstPos, pstCurr, sizeof(*pstPos));

		return -1;
	}

	n	=	sizeof(s_pos)/sizeof(RESPOS);
	iDir = FaceAngleBetween(pstOrigin->X,  pstOrigin->Y, 0, 0);

	for(i=0; i<MAX_TRY_POSITION; i++)
	{
		iDist = iMinDist + RAND() % (iMaxDist-iMinDist+1);
		
		x	=	( RAND() % (2*iDist+1) ) - iDist;
		y	=	(int) sqrt( iDist*iDist - x*x );

		if( x % 2 )
			y	=	-y;

		for(j=0; j<n; j++)
		{
			stPos.X=	pstOrigin->X + x*s_pos[j].X;
			stPos.Y=	pstOrigin->Y + y*s_pos[j].Y;

			if(stPos.X<0 || stPos.X>=pstMapIdx->stEnd.X)
				continue;

			if(stPos.Y<0 || stPos.Y>=pstMapIdx->stEnd.Y)
				continue;

			if( IsPixelLineThrough(pstMask, pstCurr->X, pstCurr->Y, stPos.X, stPos.Y, maskStop) &&
				z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stPos))
			{
				pstPos->X	=	stPos.X;
				pstPos->Y	=	stPos.Y;

				return 0;
			}
		}
	}

	memcpy(pstPos, pstCurr, sizeof(*pstPos));

	return -1;
}
*/


int z_get_safe_mappos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int *piMap, RESPOS *pstPos)
{
	BIRTHMAPDEF *pstBirthMapDef;
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj->iBirthMapDef <= 0)
		return -1;

	pstBirthMapDef = (BIRTHMAPDEF *)pstEnv->pstZoneObj->sBirthMapDef[0];
	if (0 >= pstBirthMapDef->PosNum)
	{
		return -1;
	}

	*piMap = pstBirthMapDef->MapID;
	memcpy(pstPos, &pstBirthMapDef->PosList[0], sizeof(*pstPos));
	
	return 0;
}

MapIndex* z_map_inst_index(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEIDX *pstIdx)
{
	ZONEMAPINST* pstMapInst;
	UNUSED(pstAppCtx);

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, pstIdx);
	if( !pstMapInst )
		return NULL;

	return z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
}

int open_pworld_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	int i;
	CSPKG stPkg;
	ZONEIDX stIdx;
	ZONEPWORLD *pstZonePworld;
	CSDYNPWORLDSVR *pstDynPworldSvr = &stPkg.Body.DynPworldSvr;
	UNUSED(pstAppCtx);

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}
	
	pstDynPworldSvr->PworldID = iPworldID;
	pstDynPworldSvr->MapNum = pstZonePworld->iMap;
	for (i = 0; i < pstZonePworld->iMap; i++)
	{
		pstDynPworldSvr->MapID[i] = pstZonePworld->astMapIdx[i].iID;
	}
		
	Z_CSHEAD_INIT(&stPkg.Head, DYN_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int player_enter_dyn_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSDYNPWORLDCLT *pstDynPworld = &pstCsPkg->Body.DynPworldClt;
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	
	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_PWORLD) < 0)
	{
		return -1;
	}

	if (pstPlayer->stOnline.iPworldID != pstDynPworld->PworldID)
	{
		return -1;
	}
	
	if ( pstDynPworld->DynPworldSelect.PworldGrade > PWORLD_GRADE_S ||pstDynPworld->DynPworldSelect.LevelType > PWORLD_LEVEL_AVG )
	{
		return -1;
	}
	
	pstDynPworld->DynPworldSelect.PworldDifficulty = 0;
	
	return z_player_enter_pworld(pstEnv, pstPlayer, pstDynPworld->PworldID, &pstDynPworld->DynPworldSelect, 0);
}

int make_tmp_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iItemID, int iNum)
{	
	int i = 0;
	ZONEIDX stMapIdx;
	RESPOS stPos;
	ITEMDEF *pstItemDef = NULL;
	MapIndex * pstMapIdx = NULL;
	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	UNUSED(pstAppCtx);
	
	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef || ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stMapIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if (ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
	{
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		if (!pstPworldInst)
		{
			return -1;
		}
	}
	
	for (i=0; i<iNum; i++)
	{
		z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 
						400, 50, &stPos);
		tmp_item_create(pstEnv, NULL,iItemID, &stMapIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, pstItemDef->ItemTmp.RefreshTime, 
							pstItemDef->ItemTmp.StayTime, 0,0,0,NULL);
	}

	return 0;
}

int z_pworld_queue_notify_region(ZONESVRENV* pstEnv,PworldApplyQueue *pstQueue)
{
	if (pstQueue->iPworldFlag > 0 )
	{
		SSPWORLDQUEUEINFOREQ stReq;

		ZONEIDX stIdx;
		ZONEPWORLD *pstPworld;

		stIdx.iID = pstQueue->iPworldID;
		stIdx.iPos = -1;
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
		if (!pstPworld)
			return -1;
	
		
		stReq.PworldDefID 		= pstQueue->iPworldID;
		stReq.CampNum 			= pstPworld->stPworldDef.CampNum;
		stReq.PworldNum 			= MAX_BATTLE_PWORLD_NUM;
		stReq.PworldPlayerNum 	= pstPworld->stPworldDef.PlayerNum;
		stReq.QueuerNum			= pstQueue->iQueuerNum;
		stReq.ZoneBusID			= pstEnv->pstAppCtx->iId;

		if (z_send_to_region_msg(pstEnv->pstAppCtx,pstEnv,BATTLE_QUEUE_INFO,(char*)&stReq,sizeof(stReq)) < 0)
		{
			return -1;
		}

		tlog_info(pstEnv->pstLogCat, 0, 0,	"notify_region PworldDefID|%d|CampNum|%d|PworldNum|%d|PworldPlayerNum|%d|QueuerNum|%d",
								stReq.PworldDefID,stReq.CampNum,
								stReq.PworldNum,stReq.PworldPlayerNum,stReq.QueuerNum);
	}
	return 0;
}

int z_del_battle_pworld_info(ZONESVRENV* pstEnv, int iBattleBusID, tdr_ulonglong ullPworldWID)
{
	/*int i;
	PworldWaitQueue* pstWaitQueue;
		
	for (i=0; i<MAX_PWORLD_WAIT_QUEUE; i++)
	{
		pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[i];
		
		if (	pstWaitQueue->PworldWID == ullPworldWID &&
			pstWaitQueue->iBattleBusID == iBattleBusID)
		{
			memset(pstWaitQueue, 0, sizeof(*pstWaitQueue));
			break;
		}
	}*/

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iBattleBusID);	//add by paraunused.pl
	UNUSED(ullPworldWID);	//add by paraunused.pl
}

int z_del_battle_player_num(ZONESVRENV* pstEnv, int iBattleBusID, tdr_ulonglong ullPworldWID)
{
	int i;
	PworldWaitQueue* pstWaitQueue;

	for (i=0; i<MAX_PWORLD_WAIT_QUEUE; i++)
	{
		pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[i];

		if (	pstWaitQueue->PworldWID <=	0 ||
			pstWaitQueue->iBattleBusID <= 0)
		{
			continue;
		}
		
		if (	pstWaitQueue->PworldWID == ullPworldWID &&
			pstWaitQueue->iBattleBusID == iBattleBusID &&
			pstWaitQueue->iPlayerNum > 0)
		{
			pstWaitQueue->iPlayerNum -- ;
			break;
		}
	}

	return 0;
}
int z_add_battle_player_num(ZONESVRENV* pstEnv, int iBattleBusID, tdr_ulonglong ullPworldWID)
{
	int i;
	PworldWaitQueue* pstWaitQueue;
	
	for (i=0; i<MAX_PWORLD_WAIT_QUEUE; i++)
	{
		pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[i];

		if (	pstWaitQueue->PworldWID <=	0 ||
			pstWaitQueue->iBattleBusID <= 0)
		{
			continue;
		}
		
		if ( pstWaitQueue->PworldWID == ullPworldWID &&
			 pstWaitQueue->iBattleBusID == iBattleBusID )
		{
			pstWaitQueue->iPlayerNum++ ;
			break;
		}
	}

	return 0;
}

int z_check_enter_again(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld)
{
	ROLEMISCINFO *pstMisc;
	int iPworldInstID; 
	ZONEPWORLDINST *pstPworldInst;
	int i;
	
	if (pstZonePworld->stPworldDef.NotEnterAgain)
	{
		iPworldInstID = pstZonePworld->iPworldInstHead;
		while(iPworldInstID > 0)
		{
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, iPworldInstID);
			if(pstPworldInst == NULL)
			{
				break;
			}

			iPworldInstID = pstPworldInst->iNextPworldInstID;
			pstMisc = &pstPlayer->stRoleData.MiscInfo;
			for (i=0; i<pstMisc->OncePworldNum; i++)
			{
				if (pstMisc->OncePworld[i].PworldWID == pstPworldInst->ullPworldWID)
				{
					return -1;
				}
			}		
		}
	}

	return 0;
}



int z_player_enter_pworld_inst(ZONESVRENV* pstEnv, Player *pstPlayer, 
				int iPworldID)
{
	RolePworldQueue *pstRoleQueue;
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;

	pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, iPworldID);
	if(NULL == pstRoleQueue || 
		(pstRoleQueue->iQueueType != PWORLD_WAIT_QUEUE && 
			pstRoleQueue->iQueueType != PWORLD_CLAN_QUEUE ) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_NOTINWAITQUEUE);
		return -1;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	if(pstPlayer->stOnline.State &  CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_MAP120);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_MAP121);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_MAP122);
		return -1;
	}

	if (	pstPworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT ||
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT ||
		is_normal_battle_pworld(pstPworld))
	{
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstRoleQueue->iPworldInstID);
		if(NULL == pstPworldInst)
		{
			return -1;
		}

		if (pstPworldInst->bOver)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_PWORLDOVER);
			return -1;
		}

		if (battle_enter_up_time_limit(pstEnv,pstPlayer, 1) < 0)
		{
			return -1;
		}
		
		if (0 > z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstPworld, pstPworldInst))
		{
			return -1;
		}
	}
	else if (pstPworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD)
	{
		
	}
	
	z_exit_pworld_queue(pstEnv, pstPlayer, iPworldID, 1);

	return 0;
}

int player_pworld_match_clt(ZONESVRENV* pstEnv,Player * pstPlayer,CSPWORLDMATCH *pstMatch)
{
	RolePworldQueue *pstRoleQueue;
	//ZONEPWORLDINST *pstPworldInst;	//comment by paraunused.pl
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	PworldClanQueue * pstClanQueue ;

	pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, pstMatch->PworldID);
	if(NULL == pstRoleQueue || 
		(pstRoleQueue->iQueueType != PWORLD_CLAN_QUEUE ))
	{
		return -1;
	}

	stIdx.iID = pstMatch->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	//这时候不应该在wait queue
	pstClanQueue = get_pworld_clan_queue_by_id(pstEnv,pstRoleQueue->iQueueIdx,pstRoleQueue->iPworldID);

	if (pstClanQueue->iQueueType != PWORLD_APPLY_QUEUE)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					"当前队列不在报名状态,无法进行匹配");
		return -1;
	}

	if (pstClanQueue->iMatchStat != PWORLD_SVR_MATCH_CAN)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					"当前队列不是可匹配状态,无法进行匹配");
		return -1;
	}

	pstClanQueue->iMatchStat = PWORLD_SVR_MATCH_DOING;
	
	player_pworld_clan_queue_all_match_stat(pstEnv,pstPworld, pstClanQueue);

	return 0;
}

int player_pworld_match_cancel_clt(ZONESVRENV* pstEnv,Player * pstPlayer,CSPWORLDMATCH *pstMatch)
{
	RolePworldQueue *pstRoleQueue;
	//ZONEPWORLDINST *pstPworldInst;	//comment by paraunused.pl
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	PworldClanQueue * pstClanQueue ;

	pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, pstMatch->PworldID);
	if(NULL == pstRoleQueue || 
		(pstRoleQueue->iQueueType != PWORLD_CLAN_QUEUE ))
	{
		return -1;
	}

	stIdx.iID = pstMatch->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	//这时候不应该在wait queue
	pstClanQueue = get_pworld_clan_queue_by_id(pstEnv,pstRoleQueue->iQueueIdx,pstRoleQueue->iPworldID);

	if (pstClanQueue->iQueueType != PWORLD_APPLY_QUEUE)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					"当前队列不在报名状态,无法取消匹配");
		return -1;
	}

	if (pstClanQueue->iMatchStat != PWORLD_SVR_MATCH_DOING)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					"当前队列不是可匹配状态,无法取消匹配");
		return -1;
	}

	pstClanQueue->iMatchStat = PWORLD_SVR_MATCH_CAN;
	
	player_pworld_clan_queue_all_match_stat(pstEnv,pstPworld, pstClanQueue);
	
	return 0;
}

int player_pworld_apply_invite_in(ZONESVRENV* pstEnv,ZoneClan* pstClan,Player * pstPlayer,CSPWORLDAPPLYINVITE *pstInvite)
{
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;

	//报名邀请	
	pstPworldSvr->SvrType = PWORLD_SVR_APPLY_INVITE;
	pstPworldSvr->SvrData.ApplyInvite = *pstInvite;

	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);

	//pstPlayer 可能为null
	player_clan_broadcast_except(pstEnv,pstClan,&stPkg,0,pstPlayer);

	return 0;
}

int player_pworld_apply_invite_clt(ZONESVRENV* pstEnv,Player * pstPlayer,CSPWORLDAPPLYINVITE *pstInvite)
{
	ZoneClan* pstClan;


	pstClan = player_get_clan(pstEnv,pstPlayer);

	if (!pstClan)
	{
		return 0;
	}


	player_pworld_apply_invite_in(pstEnv,pstClan, pstPlayer,pstInvite);
	
	return 0;
}

// 玩家是否在退出会回到复活点的地图
int player_is_in_logout_to_revivepoint_map(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	MapIndex *pstMapIdx;
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, GET_MAPID(pstPlayer->stRoleData.Map));
	if (pstMapIdx && pstMapIdx->stMapDef.CtrlFlag&MAP_CTRL_LOGOUT_TO_REVIVE_POINT)
	{
		return 1;
	}
	return 0;
}

int pworld_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPWORLDCLT *pstReq = &pstCsPkg->Body.PworldClt;
	UNUSED(pstFrameHead);

	/*
	if( PWORLD_CLT_EXIT_PWORLD!=pstReq->CltType &&
		(is_battle_pve_autoteam_svr(pstEnv) || 
		 pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "当前处于跨服状态，不允许该操作");
		return -1;
	}
	*/
	
	switch(pstReq->CltType)
	{
		case PWORLD_CLT_EXIT_QUEUE:
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstEnv->pstConf->BattleFlag)
			{
				return zone_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
			else
			{
				return z_player_exit_pworld_queue(pstEnv, pstPlayer, &pstReq->CltData.ExitPworldQueue);
			}
			break;

		// 进入跨服开始player_span_hold_req(确认进入战场)
		case PWORLD_CLT_ENTER_PWORLD:
			if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MAP155);
				return -1;
			}

			if (player_is_in_logout_to_revivepoint_map( pstEnv,  pstPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIANFENG31);
				return -1;
			}
			
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstEnv->pstConf->BattleFlag)
			{
				SSSPANDSTDATA stDstData;

				stDstData.DstType = SPAN_DST_TYPE_BATTLE;
				stDstData.Data.Battle.PworldID = pstReq->CltData.EnterPworldInst.PworldID;

				if(0 > player_span_state_check(pstEnv, pstPlayer))
				{
					return -1;
				}
				
				return player_span_hold_req(pstEnv, pstPlayer, &stDstData);
			}
			else
			{
				return z_player_enter_pworld_inst(pstEnv, pstPlayer, pstReq->CltData.EnterPworldInst.PworldID);
			}
			break;
		case PWORLD_CLT_EXIT_PWORLD:
			return  player_exit_fb_clt(pstAppCtx, pstEnv, pstPlayer, pstReq->CltData.ExitPworld.PworldID);
			break;

		// 进入排队队列(报名)
		case PWORLD_CLT_JOIN_QUEUE:
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
			{
				return z_player_normal_battle_queue(pstEnv, pstPlayer, &pstReq->CltData.JoinQueue);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
				return -1;
			}
			break;
		case PWORLD_CLT_ENTER_CLAN_CITY:
			if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING ||
				INST_MODE_ZONE_SVR != pstEnv->iInstMode)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
				return -1;
			}
			return player_enter_clan_city_clt(pstEnv, pstPlayer, pstReq->CltData.EnterClanCity.ClanID,pstReq->CltData.EnterClanCity.ClanSpyTransDefID);
			break;
		case PWORLD_CLT_MATCH:
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstEnv->pstConf->BattleFlag)
			{
				return zone_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
			else
			{
				return player_pworld_match_clt(pstEnv, pstPlayer, &pstReq->CltData.Match);
			}
			break;
			
		case PWORLD_CLT_MATCH_CANCEL:
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstEnv->pstConf->BattleFlag)
			{
				return zone_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
			else
			{
				return player_pworld_match_cancel_clt(pstEnv, pstPlayer, &pstReq->CltData.MatchCancel);
			}
			break;
			
		case PWORLD_CLT_APPLY_INVITE:
			return player_pworld_apply_invite_clt(pstEnv, pstPlayer, &pstReq->CltData.ApplyInvite);
			break;
			
		default:
			break;
	}

	return 0;
}

//战场上报副本信息
int z_battle_report_pworld_info(ZONESVRENV* pstEnv,TAPPCTX* pstAppCtx)
{
	//int i,j;
	//ZONEPWORLDINST *pstPworldInst;
	//SSBATTLEINFOREQ  stBattleInfoReq;
	SSBATTLEINFOSIMPLEREQ stBattleInfoSimpleReq;
	UNUSED(pstAppCtx);
	
	memset(&stBattleInfoSimpleReq,0,sizeof(stBattleInfoSimpleReq));
	//memset(&stBattleInfoReq,0,sizeof(stBattleInfoReq));

	if( pstEnv->iInstMode != INST_MODE_BATTLE_SVR )
	{
		// 不是战场就退出
		return -1;
	}
	
	/*for (i=0; i<MAX_PWORLD_INST; i++)
	{
		pstPworldInst = tmempool_get_bypos(pstEnv->pstPworldInstPool, i);
		if (!pstPworldInst)
		{
			continue;
		}

		if( stBattleInfoReq.PworldInstNum < MAX_BATTLE_PWORLD_NUM )
		{
			stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].CampNum = pstPworldInst->cCampNum;
			stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].PlayerNum = pstPworldInst->iPlayer;
			for( j=0; j<pstPworldInst->cCampNum && j<MAX_PWORLD_CAMP;j++)
			{
				
				stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].BattleCampInfo[j].PworldDefID = pstPworldInst->stPworldIdx.iID;
				stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].BattleCampInfo[j].PworldInstID = pstPworldInst->iPworldInstID;
				stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].BattleCampInfo[j].PworldWID = pstPworldInst->ullPworldWID;
				stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].BattleCampInfo[j].ZoneBusID = pstPworldInst->stPworldCampInfo[j].ZoneBusID;
				stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].BattleCampInfo[j].BattleBusID = pstAppCtx->iId;
				stBattleInfoReq.BattlePworldInfo[stBattleInfoReq.PworldInstNum].BattleCampInfo[j].PworldCampID = pstPworldInst->stPworldCampInfo[j].PworldCampID;
			}
			stBattleInfoReq.PworldInstNum++;
		}
		else
		{
			break;
		}
	}*/
	
	stBattleInfoSimpleReq.PlayerNum = pstEnv->pstPlayerPool->iUsed;
	stBattleInfoSimpleReq.PworldInstNum = pstEnv->pstPworldInstPool->iUsed;

	z_send_to_region_msg(pstEnv->pstAppCtx,pstEnv, BATTLE_REPORT_INFO,(char*)&stBattleInfoSimpleReq,sizeof(stBattleInfoSimpleReq));	

	tlog_info(pstEnv->pstLogCat, 0, 0, "report pworld info|PworldInstNum|%d|PlayerNum|%d",
				stBattleInfoSimpleReq.PworldInstNum,
				stBattleInfoSimpleReq.PlayerNum);

	return 0;
}

//npc 比较特殊视野不对称，保险起见可能看到的都通知一下
int z_sendpkg_mon_area_view_except(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, 
		CSPKG *pstPkg, Player *pstExceptPlayer)
{
	int i, j;
	AREADYN *pstArea;
	ZONEMAPINST *pstMapInst;
	Player *pstPlayer;
	Player *apstPlayers[MAX_PLAYER];
	int iCount = 0;
	TDRDATA stNet;
	int iRet;
	ZONEPWORLDINST *pstPworldInst;
	int iIndex;
	MapIndex *pstMapIdx;
	MonAtkInfo *pststMonAtkInfo = &pstMon->stMonAtkInfo;
	SKILLDEF* pstSkillDef = NULL;

	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if(!pstMapInst)
		return -1;

	if (pstMapInst->iPworldInstID != 0)
	{
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		if (pstPworldInst)
		{
			return pworld_map_broadcast_except(pstEnv->pstAppCtx, pstEnv, pstPworldInst, 
					pstMapInst, 0, pstPkg, pstExceptPlayer);
		}
	}

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_sendpkg_monview cmd=%d cs pack fail ret=%d",
					pstPkg->Head.Cmd, iRet);
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	pstArea = z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &pstMon->stCurrPos);
	if (pstArea && pstArea->iAnimate >= MAX_AREA_ANIMATE)
	{
		TMEMBLOCK *pstMemBlock;
			
		for (i=0; i<MAX_PLAYER; i++)
		{
			pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
				pstPlayer->stRoleData.Map != pstMon->stMap.iID)
			{
				continue;
			}

			if(pstPlayer == pstExceptPlayer)
			{
				continue;
			}

			if ( ( (MON_MISC_FLAG_IMPVIEW & pstMon->cMonMiscFlag) ||is_mon_machine(pstMon) ) &&
					pstPkg->Head.Cmd != LEAVE_VIEW)
			{
				if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos))
				{
					continue;
				}
			}
			
			apstPlayers[iCount++] = pstPlayer;	
		}
	}
	else
	{
		for (i = 0; i < pstMapInst->iAreaDyn; i++) 
		{
			iIndex	=	pstMapInst->aiAreaDyn[i];

			if( iIndex<0)
			{
				continue;
			}

			pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
			if (NULL == pstArea)
			{
				continue;
			}
			
			for (j = 0; j < pstArea->iAnimate; j++) 
			{
				if (pstArea->aiAnimate[j] == pstMon->iID) continue;

				pstPlayer = z_id_player(pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
				if(NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
				{
					continue;
				}

				if(pstPlayer == pstExceptPlayer)
				{
					continue;
				}

				if ( ( (MON_MISC_FLAG_IMPVIEW & pstMon->cMonMiscFlag) ||is_mon_machine(pstMon) ) &&
					pstPkg->Head.Cmd != LEAVE_VIEW)
				{
					if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos))
					{
						continue;
					}
				}

				apstPlayers[iCount++] = pstPlayer;
			}
		}
	}

	pstSkillDef = z_find_skill(pstEnv, pststMonAtkInfo->unSkillID, pststMonAtkInfo->bSkillLevel, &pststMonAtkInfo->iSkillIdx);
	if(pstSkillDef&&pstSkillDef->JiqiChgFlag==2 && 
		pstPkg->Head.Cmd == ACTION_RES &&
		pstPkg->Body.ActionRes.ActionID == ACTION_HIT)
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (!pstMapInst)
		{
			return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
		}
		z_sendpkg_atkposview(pstAppCtx, pstEnv, pstMapInst, &pststMonAtkInfo->stAtkPos,pstPkg,apstPlayers, iCount);
	}
	else if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	
	return 0;
}

//npc 比较特殊视野不对称，保险起见可能看到的都通知一下
int z_sendpkg_mon_area_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, CSPKG *pstPkg)
{
	z_sendpkg_mon_area_view_except(pstAppCtx, pstEnv, pstMon, pstPkg, NULL);
	return 0;
}

int map_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, CSPKG *pstPkg)
{
	int i, j;
	AREADYN *pstArea;
	Player *pstPlayer;
	Player *apstPlayers[MAX_PLAYER];
	int iCount = 0;
	TDRDATA stNet;
	int iRet;
	ZONEPWORLDINST *pstPworldInst;
	int iIndex;

	if(!pstMapInst)
		return -1;

	if (pstMapInst->iPworldInstID != 0)
	{
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		if (pstPworldInst)
		{
			return pworld_map_broadcast(pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, pstPkg);
		}
	}

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_sendpkg_monview cmd=%d cs pack fail ret=%d",
					pstPkg->Head.Cmd, iRet);
		return -1;
	}

	for (i = 0; i < pstMapInst->iAreaDyn; i++) 
	{
		iIndex	=	pstMapInst->aiAreaDyn[i];

		if( iIndex<0)
		{
			continue;
		}

		pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}
		
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstPlayer = z_id_player(pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if(NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
			{
				continue;
			}

			apstPlayers[iCount++] = pstPlayer;
		}
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	
	return 0;
}

int z_change_battle_map_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer)
{
	/*改变地图 把临时物品卸下来 */
	if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
	{	
		tmp_item_drop(pstEnv, pstPlayer,1);			
	}

	//清理马上得乘客
	if (pstPlayer->stOnline.stRideOnInfo.iPassengerNum && 
		(pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)))
	{
		int i;
		Player *pstTmpPlayer = NULL;

		for(i = 0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			//客人下马
			z_hitch_ride_clear(pstEnv, pstTmpPlayer);
		}

		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	//搭乘的下马
	else if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	else if(pstPlayer->stOnline.State & CS_STAT_RIDE_PREPARE)
	{
		z_ride_break(pstAppCtx, pstEnv, pstPlayer);
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		player_ride_machine_brk(pstEnv, pstPlayer, 1);
	}

	//逃跑buff
	z_add_runaway_buff(pstEnv, pstPlayer);
	
	return 0;
}

BCTDEF *z_find_bctdef_by_idx(ZONESVRENV *pstEnv, int iLayer, int *pIidx)
{
	return (BCTDEF *)z_find_def(pstEnv->pstZoneObj->sBCTDef, iLayer, BCT_DEF_DATA, 
									pstEnv->pstZoneObj->iBCTDef, pIidx);
}

BCTDEF *z_find_bctdef(ZONESVRENV* pstEnv, int iLayer)
{
	BCTDEF stBctDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stBctDef.BCTIdx = iLayer;
	
	iIndex = bsearch_int(&stBctDef, pstObj->sBCTDef, pstObj->iBCTDef, BCT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BCTDEF *)&pstObj->sBCTDef[iIndex];
	}

	return NULL;
}

BCTMON* get_mon_bct_dyn(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	BCTDEF *pstBctDef;

	if(pstMon->stBctInfo.iBctDefID <= 0)
	{
		return NULL;
	}
	
	pstBctDef = z_find_bctdef_by_idx(pstEnv, pstMon->stBctInfo.iBctDefID, &pstMon->stBctInfo.iBctDefIdx);
	if (!pstBctDef)
	{
		return NULL;
	}

	if(pstMon->stBctInfo.iBctMonIdx > 0 &&
		pstMon->stBctInfo.iBctMonIdx < BCT_MON_TYPE_NUM &&
		pstBctDef->BCTMon[pstMon->stBctInfo.iBctDefIdx].MonID == pstMon->iDefIdx)
	{
		return &pstBctDef->BCTMon[pstMon->stBctInfo.iBctDefIdx];
	}
	
	for (i=0; i<BCT_MON_TYPE_NUM; i++)
	{
		if (pstBctDef->BCTMon[i].MonID == pstMon->iDefIdx)
		{
			//快速索引
			pstMon->stBctInfo.iBctMonIdx = i;
			return &pstBctDef->BCTMon[i];
		}
	}		

	return NULL;
}

RNDAWDDEF *z_find_rndawddef(ZONESVRENV* pstEnv, int iIdx)
{
	RNDAWDDEF stRndDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stRndDef.RndIdx = iIdx;
	
	iIndex = bsearch_int(&stRndDef, pstObj->sRndAwdDef, pstObj->iRndAwdDef, RAND_AWARD_DATA, &iEqu);
	if (iEqu)
	{
		return (RNDAWDDEF *)&pstObj->sRndAwdDef[iIndex];
	}

	return NULL;
}

// 白层塔单层最高记录通知客户端
int bct_layer_history_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer)
{
	CSPKG stPak;
	CSBCTRES *pstBctRes = &stPak.Body.BctRes;
	BAICENGHISTORYENTRY *pstHistory = pstEnv->pstGlobalObj->stGlobal.Misc.BaiCengHistory.BaiCengHistoryEntrys;
	SINGLELAYER *pstBCTSingleLayers =  pstPlayer->stRoleData.MiscInfo.BCTSingleLayers;

	memset(pstBctRes, 0 ,sizeof(*pstBctRes));
	
	pstBctRes->Cmd = BCT_CMD_SVR_HISTORY;
	pstBctRes->SvrData.BctLayerHistory.Layer = iLayer;

	// 本层最好记录
	if (pstHistory[iLayer-1].Time > 0 || pstHistory[iLayer-1].TimeMs > 0)
	{
		pstBctRes->SvrData.BctLayerHistory.TopUseTime 	= pstHistory[iLayer-1].Time;
		pstBctRes->SvrData.BctLayerHistory.TopUseTimeMs 	= pstHistory[iLayer-1].TimeMs;
		strncpy(pstBctRes->SvrData.BctLayerHistory.TopName, pstHistory[iLayer-1].Name, 
				sizeof(pstBctRes->SvrData.BctLayerHistory.TopName));
	}
	
	// 自己的记录
	if (iLayer <= pstPlayer->stRoleData.MiscInfo.BCTHistoryTop)
	{
		pstBctRes->SvrData.BctLayerHistory.SelfTopUseTime  	= pstBCTSingleLayers[iLayer-1].Top;
		pstBctRes->SvrData.BctLayerHistory.SelfTopUseTimeMs	= pstBCTSingleLayers[iLayer-1].TopMs;
		pstBctRes->SvrData.BctLayerHistory.SelfTopRecordTime 	= pstBCTSingleLayers[iLayer-1].RecordTm;	
	}
	
	// 下层最高记录
	if (iLayer < BAICENG_CENG_NUM)
	{
		if (pstHistory[iLayer].Time > 0 || pstHistory[iLayer].TimeMs > 0)
		{
			pstBctRes->SvrData.BctLayerHistory.NextTopUseTime 	= pstHistory[iLayer].Time;
			pstBctRes->SvrData.BctLayerHistory.NextTopUseTimeMs	= pstHistory[iLayer].TimeMs;
			strncpy(pstBctRes->SvrData.BctLayerHistory.NextTopName, pstHistory[iLayer].Name, 
					sizeof(pstBctRes->SvrData.BctLayerHistory.NextTopName));
		}
	}
	
	Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int bct_enter(ZONESVRENV* pstEnv, Player *pstPlayer, BCTENTER *pstBctEnter)
{
	int iSelfTop = pstPlayer->stRoleData.MiscInfo.BCTHistoryTop;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	BCTDEF *pstBctDef;
	PWORLDSELECT stPworldSelect;
	ROLEBCTINFO *pstBctInfo = &pstPlayer->stRoleData.MiscInfo.BCTInfo;

	/*if (	pstBctEnter->StartLayer%10 != 1 ||
		pstBctEnter->StartLayer >BAICENG_CENG_NUM ||
		pstBctEnter->StartLayer < 1)
	{
		return -1;
	}*/

	if (	pstBctEnter->StartLayer >BAICENG_CENG_NUM ||
		pstBctEnter->StartLayer < 1)
	{
		return -1;
	}

	if (	pstBctEnter->StartLayer != 1 &&
		pstBctEnter->StartLayer != 5 &&
		pstBctEnter->StartLayer%10 != 0)
	{
		return -1;
	}

	if (iSelfTop != 0)
	{
		if (pstBctEnter->StartLayer > (iSelfTop+1) )
		{
			return -1;
		}
	}
	else
	{
		if (pstBctEnter->StartLayer != 1)
		{
			return -1;
		}
	}

	pstBctEnter->PworldID = 300;
	stIdx.iID = pstBctEnter->PworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (!pstZonePworld || pstZonePworld->stPworldDef.PolyType != PWORLD_POLY_BCT)
		return -1;
	if (pstBctEnter->UseItem > 0)
	{
		if (pstZonePworld->stPworldDef.ItemEnterID <= 0)
			return -1;

		if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstZonePworld->stPworldDef.ItemEnterID, 1, -1, NULL,PACKAGE_DEC_NONE) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP156);
			return -1;
		}
	}
	
	pstBctDef = z_find_bctdef(pstEnv, pstBctEnter->StartLayer);
	if (!pstBctDef)
		return -1;

	pstPlayer->stOnline.stBctOnline.iUseItemEnterPworld = pstBctEnter->UseItem>0?1:0;
	pstPlayer->stOnline.stBctOnline.iStartLayer = pstBctEnter->StartLayer;
	memset(&stPworldSelect, 0, sizeof(stPworldSelect));
	memset(pstBctInfo, 0, sizeof(*pstBctInfo));

	if (z_player_enter_pworld(pstEnv, pstPlayer, pstBctEnter->PworldID, &stPworldSelect, 0) == 0)
	{
		if (pstBctEnter->UseItem > 0)
		{
			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstZonePworld->stPworldDef.ItemEnterID, 1, -1, 
				NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BCT);
		}

		// 策划要求清空xp
		fairy_camp_clean_xp(pstEnv, pstPlayer);
	}
	
	pstPlayer->stOnline.stBctOnline.iUseItemEnterPworld = 0;
	pstPlayer->stOnline.stBctOnline.iStartLayer = 0;
	return 0;
}

int is_in_bct(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstPworld;
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return 0;
	}

	pstPworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstPworldInst)
	{
		return 0;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return 0;
	}

	if (pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT)
	{
		return 1;
	}
	return 0;
}
int is_player_in_pworld(Player *pstPlayer)
{
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return 0;
	}
	
	return 1;
}

int bct_again(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer)
{
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstPworld;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	BCTDEF *pstBctDef;
	RESPOS stPos;

	// 在fb中就退出   不再就不管
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP157);
		return -1;
	}

	if (iLayer != pstMiscInfo->BCTInfo.CurrLayer)
	{
		return -1;
	}

	pstPworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstPworldInst)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	pstBctDef = z_find_bctdef(pstEnv, iLayer);
	if (!pstBctDef)
		return -1;

	stPos.X = pstBctDef->EnterX;
	stPos.Y = pstBctDef->EnterY;

	if (pstBctDef && pstBctDef->TimeLimit > 0)
	{
		pstPworldInst->iDownTm = pstBctDef->TimeLimit *60 + pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pstPlayer->stRoleData.MiscInfo.BCTInfo.LayerStartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pworld_again(pstEnv, pstPworld,pstPworldInst,pstPlayer->stRoleData.Map, &stPos);
	return 0;
}


int bct_continue_clean_item(ZONESVRENV* pstEnv, ZONEIDX *pstMapInstIdx)
{

	int i, iID;
	MapObj* pstMap;
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
//	Monster *pstMon;
//	Npc *pstNpc;
	DropItem* pstDropItem;
	char cType;
	int iIndex;
	
	pstMap	=	pstEnv->pstMapObj;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, pstMapInstIdx);
	if( !pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstMapInstIdx->iID);
		return -1;
	}

	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		if (0 > pstMapInst->aiAreaDyn[i])
		{
			continue;
		}
		
		pstAreaDyn	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, pstMapInst->aiAreaDyn[i]);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_get %d area dyn fail", pstMapInst->aiAreaDyn[i]);
			continue;
		}

		while(pstAreaDyn->iInanimate>0)
		{
			iID	=	pstAreaDyn->aiInanimate[pstAreaDyn->iInanimate-1];

			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( iIndex<0 )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "inanimal id=%d iIndex=%d", iID, iIndex);
				pstAreaDyn->iInanimate--;
				continue;
			}

			switch(cType)
			{
			case OBJ_ITEM:
				pstDropItem = (DropItem *)tmempool_get(pstEnv->pstItemPool, iIndex);
				if (pstDropItem)
				{
					//这里面会pstAreaDyn->iInanimate--
					z_item_clean(pstEnv, pstDropItem);
					continue;
				}
				else
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "item iIndex=%d", iIndex);	
				}
				break;
			default:
				break;
			}

			pstAreaDyn->iInanimate--;
		}
	}

	return 0;
}

int bct_continue(ZONESVRENV* pstEnv, Player *pstPlayer,BCTCONTINUE *pstBctContinue)
{
	BCTDEF *pstBctDef;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	int iMap;
	ZONEPWORLDINST *pstPInst;
	int i;
	int iSelfTop = pstPlayer->stRoleData.MiscInfo.BCTHistoryTop;
	
	/*if (pstMisc->BCTInfo.CurrLayer + 1 != pstBctContinue->NextLayer)
	{
		return -1;
	}

	if ((pstMisc->BCTInfo.StartLayer + pstMisc->BCTInfo.PassLayer ) != pstBctContinue->NextLayer)
	{
		return -1;
	}*/

	// 跳层不能大于自己的历史记录
	if (pstBctContinue->NextLayer - pstMisc->BCTInfo.CurrLayer > 1)
	{
		if (pstBctContinue->NextLayer > iSelfTop)
		{
			return -1;
		}

		// 跳层只能是5的倍数
		if (pstBctContinue->NextLayer % 5 != 0)
		{
			return -1;
		}
	}
	else
	{
		if (pstMisc->BCTInfo.CurrLayer + 1 != pstBctContinue->NextLayer)
		{
			return -1;
		}
	}

	if (pstBctContinue->NextLayer > BAICENG_CENG_NUM)
	{
		return -1;
	}

	pstBctDef = z_find_bctdef(pstEnv, pstBctContinue->NextLayer);
	if (!pstBctDef)
	{	
		return -1;
	}

	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP157);
		return -1;
	}


	pstPInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstPInst)
	{
		return -1;
	}
	
	for (i=0; i<pstPInst->iMapInst; i++)
	{
		bct_continue_clean_item(pstEnv, &pstPInst->astMapInstIdx[i]);
	}

	iMap = pstBctDef->MapID;
	if (iMap >= PWORLD_MAPID_BASE)
	{
		iMap	=   (pstPlayer->stRoleData.Map & 0xffff0000) | iMap;
	}

	pstPlayer->stOnline.stBctOnline.iJump = pstBctContinue->NextLayer - pstMisc->BCTInfo.CurrLayer;
	return player_goto(pstEnv,pstPlayer,iMap,pstBctDef->EnterX, pstBctDef->EnterY,pstPlayer->stOnline.iMapPos);
}

int bct_exit_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPak;
	CSBCTRES *pstBctRes = &stPak.Body.BctRes;

	pstBctRes->Cmd = BCT_CMD_SVR_EXIT;
	pstBctRes->SvrData.BctSvrExit.CurrPassNum = pstMisc->BCTInfo.PassLayer;
	pstBctRes->SvrData.BctSvrExit.CurrPassLayer = pstMisc->BCTInfo.CurrLayer;
	pstBctRes->SvrData.BctSvrExit.TotalExp = pstMisc->BCTInfo.PassExp;
	pstBctRes->SvrData.BctSvrExit.TotalMoney = pstMisc->BCTInfo.PassMoney;
	pstBctRes->SvrData.BctSvrExit.PassItemNum = pstMisc->BCTInfo.PassItemNum;
	memcpy(pstBctRes->SvrData.BctSvrExit.PassItems, pstMisc->BCTInfo.PassItems, sizeof(pstBctRes->SvrData.BctSvrExit.PassItems));
	
	if (pstMisc->BCTInfo.MaxPassLayer > 0)
	{
		pstBctRes->SvrData.BctSvrExit.RaffleNum = pstMisc->BCTInfo.MaxPassLayer/BCT_RAFFLE_LAYER;
	}
	else
	{
		pstBctRes->SvrData.BctSvrExit.RaffleNum = (pstMisc->BCTInfo.CurrLayer-1)/BCT_RAFFLE_LAYER;
	}
	
	pstPlayer->stOnline.stBctOnline.iRaffleNum = pstBctRes->SvrData.BctSvrExit.RaffleNum;

	Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int bct_exit(ZONESVRENV* pstEnv, Player *pstPlayer,BCTEXIT *pstBctExit)
{
	ZONEPWORLDINST *pstPworldInst = NULL;
	
	if (player_exit_fb_clt(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstBctExit->PworldID) < 0)
	{
		return -1;
	}

	bct_exit_info(pstEnv, pstPlayer);

	pstPworldInst = z_find_pworld_inst_bysingle(pstEnv, pstPlayer);
	if(pstPworldInst && pstPworldInst->stPworldIdx.iID == BCT_PWORLD_ID)
	{
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInst);
	}
	
	memset (&pstPlayer->stRoleData.MiscInfo.BCTInfo, 0, sizeof(pstPlayer->stRoleData.MiscInfo.BCTInfo));
	return 0;
}


/*static int get_rand_award(RNDAWDDEF *pstRndDef, int *piIdx)
{
	int i;
	int iRate = 0;
	int iRnd;
	for(i=0; i<RAND_ITEM_NUM; i++)
	{
		iRate += pstRndDef->RandIAwards[i].Rate;
	}

	iRnd = (int)RAND1(iRate);

	iRate = 0;

	for (i=0; i<RAND_ITEM_NUM; i++)
	{
		iRate += pstRndDef->RandIAwards[i].Rate;
		if (iRnd < iRate)
		{
			*piIdx = i;
			break;
		}
	}
	return -1;
}*/

/*int bct_raffle_start(ZONESVRENV* pstEnv, Player *pstPlayer,BCTRAFFLESTART *pstBctRaffleStart)
{
	CSPKG stPak;
	CSBCTRES *pstBctRes = &stPak.Body.BctRes;
	int iIdx = -1;
	RNDAWDDEF *pstRndDef;

	pstBctRes->Cmd = BCT_CMD_SVR_RAFFLE_START;
	
	pstRndDef = z_find_rndawddef(pstEnv, pstBctRaffleStart->Index);
	if (!pstRndDef || pstRndDef->RndIdx != pstBctRaffleStart->Index)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bct_raffle_start pstRndDef error ");
		goto final;
	}

	if (pstPlayer->stOnline.stBctOnline.iRaffleNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bct_raffle_start RaffleNum error");
		z_sys_strf(pstEnv,pstPlayer, SYS_MIDDLE2, "当前不能抽奖");
		goto final;
	}

	// 记在online信息中 掉线下线属于自找的
	pstPlayer->stOnline.stBctOnline.iRaffleNum --;
	pstPlayer->stOnline.stBctOnline.iRndDefIdx =  pstBctRaffleStart->Index;
	get_rand_award(pstRndDef, &iIdx);
	if(iIdx == -1)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bct_raffle_start error iIdx = %d", iIdx);
		goto final;
	}

	// 掉线后会变成0,所以这里加1
	pstPlayer->stOnline.stBctOnline.iItemIdx = iIdx+1;
	
final:
	pstBctRes->SvrData.BctRaffleStart.Index = iIdx;	// 第几个物品

	Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int bct_raffle_end(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RNDAWDDEF *pstRndDef;
	RANDAWARD *pstAward;
	ITEMDEF *pstItemDef;
	int iRet;
	
	pstRndDef = z_find_rndawddef(pstEnv, pstPlayer->stOnline.stBctOnline.iRndDefIdx);
	if (!pstRndDef || pstRndDef->RndIdx != pstPlayer->stOnline.stBctOnline.iRndDefIdx)
	{
		return -1;
	}

	if (pstPlayer->stOnline.stBctOnline.iItemIdx <=0 || pstPlayer->stOnline.stBctOnline.iItemIdx >RAND_ITEM_NUM)
	{
		return -1;
	}

	pstAward = &pstRndDef->RandIAwards[pstPlayer->stOnline.stBctOnline.iItemIdx-1];
	// 金钱||布尔
	if (pstAward->Type == 0 ||pstAward->Type == 1)
	{
		int iType;
		char szMoneyName[32];
		if(pstAward->Type == 0)
		{
			iType = ATTR_ID_MONEY;
		}
		else
		{
			iType = ATTR_ID_BULL;
		}
		money_type_to_string(iType,szMoneyName,sizeof(szMoneyName));
		
		if (pstAward->Num > 0)
		{
			if (package_add_money_test(pstEnv, pstPlayer, iType,pstAward->Num) == 0 )
			{
				package_add_money(pstEnv, pstPlayer, iType,pstAward->Num);
				z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, "恭喜你!通过抽奖获得%d个 %s",
							pstAward->Num, szMoneyName);
				{
					ROLEGRID stRoleGrid;
					memset(&stRoleGrid, 0, sizeof(stRoleGrid));
					stRoleGrid.GridData.RoleItem.DefID = BOOTY_MONEY_ID;
					stRoleGrid.GridData.RoleItem.Flag = pstAward->Num;
					z_role_pickitem_oplog( pstEnv,pstPlayer, &stRoleGrid, 1, OP_BCT_END);		
				}
			}
		}
	}
	// 道具
	else if(pstAward->Type == 2)
	{
		if(pstAward->ItemID > 0)
		{
			iRet = accord_player_item(pstEnv, pstPlayer, pstAward->ItemID, pstAward->Num, 1,OP_BCT_END);
			if (iRet == 0)
			{
				pstItemDef = z_find_itemdef(pstEnv, pstAward->ItemID);
				if (pstItemDef)
				{
					z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, "恭喜你!通过抽奖获得%d 个%s", 
								pstAward->Num, pstItemDef->Name);

				}
			}
		}
	}
	else
	{
		return -1;
	}

	pstPlayer->stOnline.stBctOnline.iRndDefIdx = 0;
	pstPlayer->stOnline.stBctOnline.iItemIdx = 0;
	
	return 0;
}*/

int bct_self_top(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPak;
	CSBCTRES *pstBctRes = &stPak.Body.BctRes;
	
	pstBctRes->Cmd = BCT_CMD_SVR_TOP;
	pstBctRes->SvrData.BctSelfTop.Layer = pstPlayer->stRoleData.MiscInfo.BCTHistoryTop;
	memcpy(pstBctRes->SvrData.BctSelfTop.LayerAward, pstPlayer->stRoleData.MiscInfo.BctLayer.LayerAward,
				sizeof(pstBctRes->SvrData.BctSelfTop.LayerAward));
	Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);

	return 0;
}

int bct_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPak;
	CSBCTRES *pstBctRes = &stPak.Body.BctRes;
	
	pstBctRes->Cmd = BCT_CMD_SVR_INFO;
	pstBctRes->SvrData.BctInfo.CurrLayer = pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer;
	pstBctRes->SvrData.BctInfo.PassLayer = pstPlayer->stRoleData.MiscInfo.BCTInfo.PassLayer;
	pstBctRes->SvrData.BctInfo.StartLayer =  pstPlayer->stRoleData.MiscInfo.BCTInfo.StartLayer;
	
	Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);

	return 0;
}

int bct_award(ZONESVRENV* pstEnv, Player *pstPlayer, BCTAWARD *pstAward)
{
	int iTop = pstPlayer->stRoleData.MiscInfo.BCTHistoryTop;
	int iIdx;
	int iResidue;
	int i;
	BCTLAYER *pstLayer = &pstPlayer->stRoleData.MiscInfo.BctLayer;
	BCTDEF *pstBctDef;
	ROLEGRID stGrid[BCT_LAYER_AWARD_NUM];
	int iGrid = 0;

	if (pstAward->Layer > iTop)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MAP_704);
		return -1;	
	}

	iIdx = (pstAward->Layer-1)/32;
	iResidue = (pstAward->Layer-1)%32;

	if (iIdx >= MAX_AWARD_LAYER)
	{
		return -1;
	}

	pstBctDef = z_find_bctdef(pstEnv, pstAward->Layer);
	if (!pstBctDef)
		return -1;

	if ((pstLayer->LayerAward[iIdx]>>iResidue) &0x01)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_MAP_705);
		return -1;
	}

	for (i=0; i<BCT_LAYER_AWARD_NUM; i++)
	{
		if (	pstBctDef->GiftItem[i].ItemID == 0 ||
			pstBctDef->GiftItem[i].ItemNum == 0)
		{
			break;
		}

		if (item_create(pstEnv->pstAppCtx, pstEnv, &stGrid[iGrid], pstBctDef->GiftItem[i].ItemID) == 0)
		{
			stGrid[iGrid].GridData.RoleItem.InstFlag|=INST_ITEM_BIND;
			stGrid[iGrid].GridData.RoleItem.Num = pstBctDef->GiftItem[i].ItemNum;
			iGrid++;
		}
		else
		{
			return -1;
		}
	}

	if (iGrid > 0)
	{
		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGrid[0], iGrid, 0) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
			return -1;
		}
	}
	
	pstLayer->LayerAward[iIdx]|=(0x01<<iResidue);

	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGrid[0], iGrid, 0, 0);
	bct_self_top(pstEnv, pstPlayer);
	return 0;
}

int bct_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{

	CSBCTREQ *pstBctReq = &pstCsPkg->Body.BctReq;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}

	if (pstBctReq->Cmd == BCT_CMD_CLT_ENTER)
	{
		if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_BCT) < 0)
		{
			return -1;
		}
		return bct_enter(pstEnv, pstPlayer, &pstBctReq->CltData.BctEnter);
	}
	else if (pstBctReq->Cmd == BCT_CMD_CLT_CONTINUE)
	{
		return  bct_continue(pstEnv, pstPlayer,&pstBctReq->CltData.BctContinue);
	}
	else if(pstBctReq->Cmd == BCT_CMD_CLT_EXIT)
	{
		return bct_exit(pstEnv, pstPlayer,&pstBctReq->CltData.BctExit);
	}
	else if (pstBctReq->Cmd == BCT_CMD_CLT_RAFFLE_START)
	{
		//return bct_raffle_start(pstEnv,pstPlayer,&pstBctReq->CltData.BctRaffleStart);
	}
	else if (pstBctReq->Cmd == BCT_CMD_CLT_RAFFLE_END)
	{
		//return bct_raffle_end(pstEnv, pstPlayer);
	}
	else if (pstBctReq->Cmd == BCT_CMD_CLT_LAYER_TOP)
	{
		int iLayer = pstBctReq->CltData.BctTopInfo.Layer;
		if (iLayer < 1 || iLayer > 100)
			return -1;

		return bct_layer_history_notify(pstEnv, pstPlayer,iLayer);
	}
	else if (pstBctReq->Cmd == BCT_CMD_CLT_AGAIN)
	{
		return bct_again(pstEnv, pstPlayer, pstBctReq->CltData.BctAgain.Layer);
	}
	else if (pstBctReq->Cmd == BCT_CMD_CLT_AWARD)
	{
		return bct_award(pstEnv, pstPlayer, &pstBctReq->CltData.BctAward);
	}
	else
	{
		return -1;
	}
	return 0;
}


int open_bct_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID)
{		
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	UNUSED(pstAppCtx);
	
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = UI_TYPE_BCT;
	pstActionRes->ID = iNpcID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

//获取顺时针圆上的某个点,iAngle相对于pstOrigin这个点的角度
int z_get_clockwise_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, SKILLDEF *pstSkillDef,
							RESPOS	* pstCurr, RESPOS* pstOrigin, int iMaxDist, int iMinDist,int iAngle, RESPOS* pstPos)
{
	RESPOS stPos;
	TMAPMASK *pstMask;
	int i;
	int iDist;
	int iDir;
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if( iMaxDist<=0 || iMinDist > iMaxDist)
	{
		if( IsPixelLineThrough(pstMask, pstCurr->X, pstCurr->Y, pstOrigin->X, pstOrigin->Y, maskStop) &&
			dyn_msk_check(pstEnv, pstMapInst, pstMask, pstCurr->X, pstCurr->Y, pstOrigin->X, pstOrigin->Y, 0,DYN_MSK_STOP))
		{
			memcpy(pstPos, pstOrigin, sizeof(*pstPos));
			return 0;	
		}
		else
		{
			//喷水怪 和目标中间有阻挡,也要喷
			memcpy(pstPos, pstOrigin, sizeof(*pstPos));
			return -1;
		}
	}
	
	if ( !pstMapIdx)
	{
		memcpy(pstPos, pstOrigin, sizeof(*pstPos));

		return -1;
	}

	for(i=0; i<MAX_TRY_POSITION; i++)
	{
		iDist = iMinDist + RAND1(iMaxDist-iMinDist+1);
		iDir = iAngle+i;
		//stPos.X=	pstCurr->X + (iDist*1.00/pstOrigin->X+cosf(iDir*PI/180.0f))*iDist;
		//stPos.Y=	pstCurr->Y + (iDist*1.00/pstOrigin->Y - sin(iDir*PI/180.0f))*iDist;
		stPos = z_rotate_pos(*pstCurr,iAngle,*pstOrigin);

		if(stPos.X<0 || stPos.X>=pstMapIdx->stEnd.X)
			continue;

		if(stPos.Y<0 || stPos.Y>=pstMapIdx->stEnd.Y)
			continue;
              //看需求看是否考虑阻挡的问题
        if (!(pstSkillDef->NoSkillStop&NOSKILLSTOP_TYPE_MAGIC))
		{
			if( IsPixelLineThrough(pstMask, pstCurr->X, pstCurr->Y, stPos.X, stPos.Y, maskStop) &&
				dyn_msk_check(pstEnv, pstMapInst, pstMask, pstCurr->X, pstCurr->Y, stPos.X, stPos.Y, 0,DYN_MSK_STOP) &&
				z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stPos))
			{
				pstPos->X	=	stPos.X;
				pstPos->Y	=	stPos.Y;

				return 0;
			}
             }
	}

	memcpy(pstPos, pstOrigin, sizeof(*pstPos));

	return -1;
}

int z_find_map_byline(MapIndex* pstMapIdx, int iLine)
{
	if(iLine <= 0 || iLine > MAX_MAP_LINE)
	{
		return -1;
	}

	return pstMapIdx->stMapLine.iMapID[iLine -1];
}

//多线地图
int is_refer_line_map(MapIndex* pstMapIdx)
{
	//副本没有多线
	if(pstMapIdx->stMapDef.MapID > PWORLD_MAPID_BASE)
	{
		return 0;
	}
	
	if(pstMapIdx->stMapDef.MapLine > 0)
	{
		return 1;
	}

	return 0;
}

//有效的线路
int is_map_valid_line(MapIndex* pstMapIdx, MapIndex* pstLine)
{
	if(!is_refer_line_map(pstLine))
	{
		return 0;
	}

	//原地图
	if(pstMapIdx->stMapDef.MapLine == 1)
	{
		if(pstMapIdx->stMapDef.MapID == pstLine->stMapDef.ReferMapID)
		{
			return 1;
		}
	}
	else
	{
		if(pstLine->stMapDef.MapLine == 1)
		{
			if(pstLine->stMapDef.MapID == pstMapIdx->stMapDef.ReferMapID)
			{
				return 1;
			}
		}
		else if (pstLine->stMapDef.ReferMapID == pstMapIdx->stMapDef.ReferMapID)
		{
			return 1;
		}
	}
	
	return 0;
}

int is_map_same_line(ZONESVRENV* pstEnv, int iMainLine, int iOtherLine)
{
	MapIndex *pstMainLine;
	MapIndex *pstOtherLine;

	pstMainLine = z_mappos_index(pstEnv->pstMapObj, NULL, iMainLine);
	pstOtherLine = z_mappos_index(pstEnv->pstMapObj, NULL, iOtherLine);

	if (NULL == pstMainLine || NULL == pstOtherLine)
	{
		return 0;
	}
		
	return is_map_valid_line(pstMainLine, pstOtherLine);
}

int is_map_same_refer(ZONESVRENV* pstEnv, int iMainMap, int iOtherMap)
{
	MapIndex *pstMainMap;
	MapIndex *pstOtherMap;

	pstMainMap = z_mappos_index(pstEnv->pstMapObj, NULL, iMainMap);
	pstOtherMap = z_mappos_index(pstEnv->pstMapObj, NULL, iOtherMap);

	if (NULL == pstMainMap || NULL == pstOtherMap)
	{
		return 0;
	}
		
	if (pstMainMap == pstOtherMap || pstMainMap->stMapDef.MapID == pstOtherMap->stMapDef.ReferMapID ||
		pstOtherMap->stMapDef.MapID == pstMainMap->stMapDef.ReferMapID)
	{
		return 1;
	}

	return 0;
}

int z_insert_mapline_data(MapIndex *pstMapIndex, MapIndex *pstInsertLine)
{
	int iID = pstInsertLine->stMapDef.MapID;
	int iLine = pstInsertLine->stMapDef.MapLine;

	if(iLine <= 0 || iLine > MAX_MAP_LINE)
	{
		printf("MapID %d MapLine %d error", iID, iLine);
		return -1;
	}

	if(pstMapIndex->stMapLine.iMapID[iLine-1] > 0)
	{
		printf("MapID %d  and MapID %d  MapLine %d overlap", pstMapIndex->stMapLine.iMapID[iLine-1], iID, iLine);
		return -1;
	}

	pstMapIndex->stMapLine.iMapID[iLine-1] = iID;
	
	return 0;
}

int z_mapline_data(ZONESVRENV* pstEnv, MapIndex *pstMapIndex)
{
	int i;
	MapIndex *pstTmpMapIndex;
	
	//自己
	if (0 > z_insert_mapline_data(pstMapIndex, pstMapIndex))
	{
		return -1;
	}
		
	for(i= 0; i<pstEnv->pstMapObj->iMapIndex; i++)
	{
		pstTmpMapIndex = (MapIndex *)pstEnv->pstMapObj->sMapIdx[i];

		if(pstTmpMapIndex->stMapDef.MapID == pstMapIndex->stMapDef.MapID)
		{
			continue;
		}

		if(is_map_valid_line(pstMapIndex, pstTmpMapIndex))
		{
			if (0 > z_insert_mapline_data(pstMapIndex, pstTmpMapIndex))
			{
				return -1;
			}
		}
	}

	return 0;
}

int z_mapdef_pre(ZONESVRENV* pstEnv)
{
	int i;
	MapIndex *pstMapIndex;

	for(i= 0; i<pstEnv->pstMapObj->iMapIndex; i++)
	{
		pstMapIndex = (MapIndex *)pstEnv->pstMapObj->sMapIdx[i];

		memset(&pstMapIndex->stMapLine, 0, sizeof(pstMapIndex->stMapLine));

		if(is_refer_line_map(pstMapIndex))
		{
			//统计线路信息
			if (0 > z_mapline_data(pstEnv, pstMapIndex))
			{
				return -1;
			}
		}
	}

	return 0;
}

int player_change_map_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (pstPlayer->stOnline.State &  CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MAP123);
		return -1;
	}

	if (pstPlayer->stOnline.State &  CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MAP124);
		return -1;
	}

	if (pstPlayer->stOnline.State &  (CS_STAT_PREPARE | CS_STAT_MOVE_PREPARE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MAP125);
		return -1;
	}

	if (pstPlayer->stOnline.State &  CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MAP126);
		return -1;
	}

	if (pstPlayer->stOnline.State &  CS_STAT_AUTOFLY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MAP127);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MAP128);
		return -1;
	}

	return 0;
}

//切线
int map_change_line(ZONESVRENV* pstEnv, Player *pstPlayer, CSMAPCHGLINE *pstChg)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	MapIndex* pstTmpMapIdx;
	RESPOS stPos;
	int iRet;

	if (pstEnv->pstConf->CloseLine)
		return -1;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	//不是一个多线路的地图
	if(!is_refer_line_map(pstMapIdx))
	{
		return -1;
	}

	//同一个线路不用切换
	if(pstChg->MapID == (int)pstMapIdx->stMapDef.MapID)
	{
		return -1;
	}

	if(0 > player_change_map_check(pstEnv, pstPlayer))
	{
		return -1;
	}

	pstTmpMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstChg->MapID);
	if (NULL == pstTmpMapIdx || !is_map_valid_line(pstMapIdx, pstTmpMapIdx))
	{
		return -1;
	}
	
	stPos = pstPlayer->stRoleData.Pos;

	g_team_follow_flag = 2;
	iRet = z_player_change_map2(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstChg->MapID, &stPos, RAND1(180), 0);
	g_team_follow_flag = 0;
	
	return iRet;	
}

int get_map_line_busytype(ZONEMAPINST* pstMapInst)
{	
	if(pstMapInst->iPlayerCount > g_iMapPlayerBusyNum)
	{
		return MAP_BUSY_TYPE_FULL;
	}
	else if(pstMapInst->iPlayerCount > g_iMapPlayerNormalNum)
	{
		return MAP_BUSY_TYPE_BUSY;
	}
	else
	{
		return MAP_BUSY_TYPE_NORMAL;
	}
	
	return 0;
}

int map_line_info(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex* pstMapIdx)
{
	int i;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	CSPKG stPkg;
	CSMAPSVR *pstMapSvr = &stPkg.Body.MapSvr;
	CSMAPLINEINFO *pstLineInfo = &pstMapSvr->Data.MapLineInfo;
	
	pstLineInfo->LineNum = 0;
	for(i=0; i<MAX_MAP_LINE; i++)
	{
		if(pstLineInfo->LineNum >= MAX_MAP_LINE)
		{
			break;
		}

		if(pstMapIdx->stMapLine.iMapID[i] == 0)
		{
			continue;
		}
		
		stIdx.iID = pstMapIdx->stMapLine.iMapID[i];
		stIdx.iPos = -1;

		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst)
		{
			pstLineInfo->MapInfo[pstLineInfo->LineNum].MapID = pstMapIdx->stMapLine.iMapID[i];
			pstLineInfo->MapInfo[pstLineInfo->LineNum].Busy = get_map_line_busytype(pstMapInst);
			pstLineInfo->LineNum++;
		}
	}

	if(pstMapSvr->Data.MapLineInfo.LineNum > 0)
	{
		pstMapSvr->SvrOp = MAP_SVR_LINE_INFO;
		Z_CSHEAD_INIT(&stPkg.Head, MAP_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

int map_update_line_info(ZONESVRENV* pstEnv, Player *pstPlayer, CSUPDATELINEINFO*pstUpdate)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	
	if (pstEnv->pstConf->CloseLine)
		return -1;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	map_line_info(pstEnv, pstPlayer, pstMapIdx);

	return 0;
	UNUSED(pstUpdate);	//add by paraunused.pl
}

int map_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
	TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSMAPCLT *pstMapClt = &pstCsPkg->Body.MapClt;

	switch(pstMapClt->CltOp)
	{
		case MAP_CLT_CHANGE_LINE:
			return map_change_line(pstEnv, pstPlayer, &pstMapClt->Data.ChgLine);
			break;
		case MAP_CLT_UPDATE_LINE_INFO:
			return map_update_line_info(pstEnv, pstPlayer, &pstMapClt->Data.UpdateLineInfo);
			break;
		default:
			return -1;
			break;
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

//是否加副本的次数,只要在副本里攻击了怪物就要开始记数了
int map_pworld_chg_limitflag(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;

	memset(&stIdx,0,sizeof(stIdx));
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
	{
		return -1;
	}
		
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (!pstPworldInst )
	{
		return -1;
	}

	if(pstPworldInst->chLimitFlag)
	{
		return -1;
	}

	pstPworldInst->chLimitFlag = 1;

	return 0;
}
//看看是否是同一个副本
LIMITPWORLD *player_check_same_pworld_by_wid(ZONESVRENV* pstEnv, ZONEPWORLDINST* pstPworldInst,Player* pstPlayer)
{
	int i = 0;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	
	for (i=0; i<pstMiscInfo->LimitPworldNum; i++)
	{
		if ((int)pstMiscInfo->LimitPworld[i].PworldID == pstPworldInst->stPworldIdx.iID)
		{
			if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time))
			{
				//WID != 才算再次创建
				if( pstMiscInfo->LimitPworld[i].PworldWID == pstPworldInst->ullPworldWID )
				{
					return &pstMiscInfo->LimitPworld[i];
				}
			}
		}
	}

	return NULL;
}

int pworld_again(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld,ZONEPWORLDINST *pstPworldInst,
					int iMap, RESPOS *pstPos)
{
	int i;
	MapIndex* pstIdx;
	Player *pstOtherPlayer;
	int iMapIndex;
	RESPOS stPos;
	int iPlayer = 0;
	int aiPlayerID[MAX_PWORLD_MEMBER];
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERMAP *pstEnterMap = &pstPkg->Body.EnterMap;
	int iDir = 0;
	int dianfengflag = 0;
	
	for (i=pstPworldInst->iPlayer-1; i>=0; i--)
       {
	   	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstOtherPlayer)
		{
			aiPlayerID[iPlayer++] = pstOtherPlayer->iMemID;

			SET_PLAYER_NOMOVE(pstOtherPlayer);
			z_player_move_stop(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer);

			if(is_player_ride_on_machine(pstOtherPlayer))
			{
				player_ride_machine_brk(pstEnv, pstOtherPlayer, 1);
			}

			if (	pstPworld->stPworldDef.InitMapID != (pstOtherPlayer->stRoleData.Map & 0xffff) ||
				pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT)
			{
				z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, 1);
			}
			else
			{
				z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, 0);
			}
		}
	}

	while(pstPworldInst->iMapInst>0)
	{
		z_in_destroy_map_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, pstPworldInst, pstPworldInst->astMapInstIdx + pstPworldInst->iMapInst - 1);
		pstPworldInst->iMapInst--;
	}

	if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_START_MAP))
	{
		pstPworldInst->bDelayScript |= PWORLD_SCRIPT_PROC_START_MAP;
	}

	pstPworldInst->iPlayer = 0;
	memset(pstPworldInst->sScriptVar, 0, sizeof(pstPworldInst->sScriptVar));
	pstPworldInst->iMonHpTotal = 0;
	pstPworldInst->iMapCurr = 0;
	pstPworldInst->bOver = 0;
	memset(&pstPworldInst->stTimerRound,0,sizeof(pstPworldInst->stTimerRound));
	memset(&pstPworldInst->stWaveRound,0,sizeof(pstPworldInst->stWaveRound));

	for(i=0; i<pstPworld->iMap; i++)
	{
		 z_in_new_map_inst(pstEnv->pstAppCtx,pstEnv, pstPworldInst->iPworldInstID, 
		 					&pstPworldInst->stPworldselect, pstPworld->astMapIdx+i);
	}
	
	iMapIndex = (pstPworldInst->iPworldInstID & 0xffff)<<16 | iMap;
	stPos = *pstPos;
	//stPos = pstPworld->stPworldDef.InitMapPos;

	pstIdx	= z_mappos_index(pstEnv->pstMapObj, NULL, iMapIndex);
	if( NULL == pstIdx )
	{
		return 0;
	}

	for (i=0; i<iPlayer; i++)
	{
		pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, aiPlayerID[i]);
		if (pstOtherPlayer)
		{
			pstOtherPlayer->stOnline.iReviverID = 0;
			pstOtherPlayer->stOnline.State &= ~CS_STAT_DEAD;

			player_revive_attr_chg(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, 
							pstOtherPlayer->stOnline.stDyn.stMidAttrList.uiHPMax, pstOtherPlayer->stOnline.stDyn.stMidAttrList.uiMPMax);
			if (	pstPworld->stPworldDef.InitMapID != (pstOtherPlayer->stRoleData.Map & 0xffff) ||
				pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT || 
				pstPworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG)
			{
				dianfengflag = 0;

				if (pstIdx->stMapDef.ReferMapID > 0)
				{
					pstEnterMap->Map = pstIdx->stMapDef.ReferMapID;
				}
				else
				{
					pstEnterMap->Map = pstIdx->stMapDef.MapID;
				}
				pstEnterMap->RealMap = pstIdx->stMapDef.MapID;

				//末世巅峰副本获取红蓝方进入点
				if (pstPworld->stPworldDef.Type == PWORLD_TYPE_PLOY && pstPworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG)
				{
					if (dianfeng_get_map_pos( pstEnv,pstIdx, &stPos, pstOtherPlayer) < 0)
					{
						return -1;
					}

					//末世巅峰竞技副本面向
					if(dianfeng_get_dir(pstEnv,pstOtherPlayer,&iDir) == 0)
					{
						pstOtherPlayer->stOnline.bDir = iDir;
						dianfengflag = 1;
					}

					//末世巅峰。技能CD重置
					player_skill_cool_clean(pstEnv, pstOtherPlayer, 0);

					// 进副本请状态
					if (pstPworld->stPworldDef.CtrlFlag & PWORLD_CLEAN_STATUS)
					{
						player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer);
					}	
				}
				
				pstEnterMap->x = stPos.X;
				pstEnterMap->y = stPos.Y;

				pstOtherPlayer->stOnline.iPosSeq += 1000;
				pstEnterMap->InitMoveSeq = pstOtherPlayer->stOnline.iPosSeq;
				pstEnterMap->Dir = pstOtherPlayer->stOnline.bDir;
				pstEnterMap->Effect = pstIdx->stMapDef.EffectID;
				pstEnterMap->CameraDir = pstPworld->stPworldDef.InitCameraDir;

				if(dianfengflag == 1)
				{
					pstEnterMap->CameraDir = iDir+90;
				}
			
				pstEnterMap->PworldInfoFlag = 1;
				
				pstEnterMap->PworldInfo.SelectGrade = pstPworldInst->stPworldselect.PworldGrade;
				
				pstEnterMap->PworldInfo.LeftTime = pstPworldInst->tEnd - pstEnv->pstAppCtx->stCurr.tv_sec;
				if(	pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT ||
					pstPworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG)
				{	
					if (pstPworldInst->iDownTm > 0)
					{
						pstEnterMap->PworldInfo.LeftTime = pstPworldInst->iDownTm - pstEnv->pstAppCtx->stCurr.tv_sec;
						if (0 > pstEnterMap->PworldInfo.LeftTime)
						{
							pstEnterMap->PworldInfo.LeftTime = 0;
						}
					}
				}
				pstEnterMap->PworldInfo.PworldInstID = pstPworldInst->iPworldInstID;

				Z_CSHEAD_INIT(&pstPkg->Head, ENTER_MAP);
				z_cltmsg_send(pstEnv, pstOtherPlayer, pstPkg);

				z_player_entermap(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, iMapIndex, &stPos, 1);

				player_revive_action(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer);
			}
			else
			{
				z_player_entermap(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, iMapIndex, &stPos, 0);

				player_revive_action(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer);
			}
		}
	}
	return 0;
}


int is_die_exit_pworld_revive(ZONEPWORLD *pstZonePworld)
{
	if ((pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_BCT) ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA ||
		is_normal_battle_pworld(pstZonePworld))
	{
		return 1;
	}

	return 0;
}

int player_leave_map_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_BEGIN_LEAVE_MAP;

	pstActionRes->Data.BeginLeaveMap.LeaveMap = 0;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}
int map_check_lineseg_endpos(TMAPMASK* pstMask,RESPOS *pstFrom,RESPOS *pstTo)
{
	int maxx,maxy;
	LINESEG l1;
	LINESEG l2;
	RESPOS inter;

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

	l2.s.X = pstFrom->X;
	l2.s.Y = pstFrom->Y;
	l2.e.X = pstTo->X;
	l2.e.Y = pstTo->Y;
	
	l1.s.X = 0;
	l1.s.Y = 0;
	l1.e.X = 0;
	l1.e.Y = maxy;
	memset(&inter,0,sizeof(inter));
	if(intersection(l1,l2,&inter))
	{
		*pstTo = inter;
		return 0;
	}
	
	l1.s.X = 0;
	l1.s.Y = maxy;
	l1.e.X = maxx;
	l1.e.Y = maxy;
	memset(&inter,0,sizeof(inter));
	if(intersection(l1,l2,&inter))
	{
		*pstTo = inter;
		return 0;
	}	
	
	l1.s.X = maxx;
	l1.s.Y = maxy;
	l1.e.X = maxx;
	l1.e.Y = 0;
	memset(&inter,0,sizeof(inter));
	if(intersection(l1,l2,&inter))
	{
		*pstTo = inter;
		return 0;
	}	

	l1.s.X = maxx;
	l1.s.Y = 0;
	l1.e.X = 0;
	l1.e.Y = 0;
	memset(&inter,0,sizeof(inter));
	if(intersection(l1,l2,&inter))
	{
		*pstTo = inter;
		return 0;
	}

	return 0;
}

int map_check_safe_pos(TMAPMASK* pstMask,RESPOS *pstPos)
{
	int maxx,maxy;

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;
	
	if( pstPos->X<0 )
	{
		pstPos->X = 0;
	}
	if( pstPos->Y<0 )
	{
		pstPos->Y = 0;
	}
		
	if(pstPos->X >=maxx )
	{
		pstPos->X = maxx -1;
	}
	if(pstPos->Y>=maxy)
	{
		pstPos->Y = maxy -1;
	}

	return 0;
}
// 返回点p以点o为圆心逆时针旋转iAngle(单位：角度)后所在的位置
RESPOS z_rotate_pos(RESPOS o,int iAngle,RESPOS p)
{
	RESPOS tp;
	p.X -=o.X;
	p.Y -=o.Y;
	tp.X=p.X*cos(iAngle*PI/180.0f) - p.Y*sin(iAngle*PI/180.0f)+o.X;
	tp.Y=p.Y*cos(iAngle*PI/180.0f) + p.X*sin(iAngle*PI/180.0f)+o.Y;
	
	return tp;
}

int z_pworld_over(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst)
{
	int i = 0;
	
	pstPworldInst->bOver = 1;
	memset(pstPworldInst->sScriptVar, 0, sizeof(pstPworldInst->sScriptVar));

	for (i=0; i<pstPworldInst->iMapInst; i++)
	{
		z_clear_map_mon(pstEnv, &pstPworldInst->astMapInstIdx[i]);
	}


	if (pstPworldInst->iType == PWORLD_TYPE_PLOY ||
		pstPworldInst->iType == PWORLD_TYPE_CLAN)
	{
		ZoneClan* pstZoneClan = z_id_clan(pstEnv,pstPworldInst->stPworldTypeData.stClan.iID);

		if(pstZoneClan && pstZoneClan->stZoneClanInfo.ullMachinePworldWID == pstPworldInst->ullPworldWID)
		{
			clan_active_ui_info_clan_machine_pworld(pstEnv, pstZoneClan,
							ACTIVE_GAME_SVR_TYPE_UI_CLOSE,pstPworldInst, 0);
		}		
	}

	return 0;
}

int is_player_zhongli_area(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	MapIndex * pstMapIdx;
	ZONEMAPINST *pstMapInst;
	
	if (pstPlayer->stOnline.cSecenOverLay > 0)
	{
		return 0;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return 0;
	}

	if (pstMapIdx->stMapDef.MapID < PWORLD_MAPID_BASE && pstMapIdx->stMapDef.MapLimit.PK == 1 && pstMapIdx->stMapDef.MapLimit.PKMode == 0 &&
		0 == (MAP_CTRL_NO_PK_VAL & pstMapIdx->stMapDef.CtrlFlag))
	{
		return 1;
	}
	
	return 0;
}


// 判断玩家是否在公会枢纽
int player_is_in_clan_shuniu(Player *pstPlayer)
{
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return 0;
	}

	if (GET_PWORLDID(pstPlayer->stRoleData.Map) == CLAN_MACHINE_PWORLD_ID)
	{
		return 1;
	}
	return 0;
}

