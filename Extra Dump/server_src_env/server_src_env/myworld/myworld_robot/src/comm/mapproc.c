/*
 *	MapProc.cpp
 *	map proc functions.
 *
 *	Jim, 2003.12.5.
 */

#include "mapproc.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <stddef.h>

void InitProc(TMAPPROC* pstProc)
{
	memset(pstProc, 0, sizeof(TMAPPROC));
}


BOOL IsProcAvailable(TMAPPROC* pstProc)
{
	return (pstProc->m_nID == 0) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////
// TMapProcs

void CMapProc_ClearAll(TMAPPROCS* pstProcs)
{
	int i;
	
	pstProcs->m_nTotalProcs = 0;
	for(i=0; i<nMaxProcs; i++)
	{
		InitProc(pstProcs->m_MapProc+i);
	}
}


unsigned int CMapProc_GetCount(TMAPPROCS* pstProcs)
{
	return pstProcs->m_nTotalProcs;
}


BOOL CMapProc_NakedWrite(TMAPPROCS* pstProcs, FILE *fp)
{
	if(!fp)
		return FALSE;

	fwrite(&pstProcs->m_nTotalProcs, 1, sizeof(unsigned int), fp);
	if(pstProcs->m_nTotalProcs > 0 && pstProcs->m_nTotalProcs < nMaxProcs)
	{
		fwrite(pstProcs->m_MapProc, 1, sizeof(TMAPPROC) * pstProcs->m_nTotalProcs, fp);
	}

	return TRUE;
}


BOOL CMapProc_NakedRead(TMAPPROCS* pstProcs, FILE *fp)
{
	if(! fp)
		return FALSE;

	fread(&pstProcs->m_nTotalProcs, 1, sizeof(unsigned int), fp);

	if(pstProcs->m_nTotalProcs > 0 && pstProcs->m_nTotalProcs < nMaxProcs)
	{
		fread(pstProcs->m_MapProc, 1, sizeof(TMAPPROC) * pstProcs->m_nTotalProcs, fp);
	}

	return TRUE;
}


void CMapProc_InitFileHeader(TMAPPROCS* pstProcs, struct TProcFileHeader *pHeader)
{
    pHeader->copyright = MAKEFOURCC('Q','Q','F',0x1a);
    pHeader->id = MAKEFOURCC('P','R','O','C');
    pHeader->version = nProcFileVersion;
    pHeader->head_size = sizeof(struct TProcFileHeader);
}

int CMapProc_AddAreaProc(TAREAPROCINFO* pstArea, int nProcIndex)
{
	int i;
	
	for(i=0; i<pstArea->total; i++)
	{
		if( pstArea->index[i]==nProcIndex )
			return -1;
	}
	
	if( pstArea->total>=nMaxAreaProcs )
		return -1;
	
	pstArea->index[pstArea->total]	=	nProcIndex;
	
	return pstArea->total++;
}

void CMapProc_GenerateAreaProcInfo(TMAPPROCS* pstProcs, TAREAPROCINFO* pstArea)
{
	unsigned int x, y;
	unsigned int area_index;
	int left, top;
	POLY plArea;
	unsigned int n;

	for(y=0; y<pstProcs->m_nVertAreas; y++)
	{
		top = y * pstProcs->m_nAreaHeight;
		for(x=0; x<pstProcs->m_nHorzAreas; x++)
		{
			area_index = y * pstProcs->m_nHorzAreas + x;
			left = x * pstProcs->m_nAreaWidth;

			
			D3_clear_poly(&plArea);
			D3_add_point_to_poly(&plArea, left, top, 0, 0, ADD_SPECIFY_X | ADD_SPECIFY_Y);
			D3_add_point_to_poly(&plArea, left + pstProcs->m_nAreaWidth, top, 0, 0, ADD_SPECIFY_X | ADD_SPECIFY_Y);
			D3_add_point_to_poly(&plArea, left + pstProcs->m_nAreaWidth, top + pstProcs->m_nAreaHeight, 0, 0, ADD_SPECIFY_X | ADD_SPECIFY_Y);
			D3_add_point_to_poly(&plArea, left, top + pstProcs->m_nAreaHeight, 0, 0, ADD_SPECIFY_X | ADD_SPECIFY_Y);
			D2_barycenter_poly(&plArea);

			for(n=0; n<pstProcs->m_nTotalProcs; n++)
			{
				if(D2_is_overlap_poly_poly(&plArea, &pstProcs->m_MapProc[n].m_Poly))
				{
					CMapProc_AddAreaProc(pstArea + area_index, n);
				}
			}
		}
	}
}

BOOL CMapProc_SaveProcFile(TMAPPROCS* pstProcs, char *szFileName, unsigned int dwVersion)
{
    FILE *fp = NULL;
    unsigned int i;
    struct TProcFileHeader fh;
    TAREAPROCINFO* pstArea=NULL;
    int fRet=TRUE;
	
    if(NULL == (fp = fopen(szFileName, "wb")))
		return FALSE;

    CMapProc_InitFileHeader(pstProcs, &fh);
    
    fh.version = dwVersion;

    fwrite(&fh, 1, sizeof(struct TProcFileHeader), fp);

    CMapProc_NakedWrite(pstProcs, fp);
    
    //{{{
    if(dwVersion >= nFileVersion2)
    {
    	pstArea	=	calloc( 1, sizeof(TAREAPROCINFO)*pstProcs->m_nHorzAreas*pstProcs->m_nVertAreas );
    	
    	if( pstArea )
    	{
    		CMapProc_GenerateAreaProcInfo(pstProcs, pstArea);

		fwrite(&pstProcs->m_nMapWidth, 1, sizeof(unsigned int), fp);
		fwrite(&pstProcs->m_nMapHeight, 1, sizeof(unsigned int), fp);
		fwrite(&pstProcs->m_nAreaWidth, 1, sizeof(unsigned int), fp);
		fwrite(&pstProcs->m_nAreaHeight, 1, sizeof(unsigned int), fp);
		fwrite(&pstProcs->m_nHorzAreas, 1, sizeof(unsigned int), fp);
		fwrite(&pstProcs->m_nVertAreas, 1, sizeof(unsigned int), fp);
	
		for(i=0; i<pstProcs->m_nHorzAreas * pstProcs->m_nVertAreas; i++)
		{
			fwrite(pstArea + i, 1, sizeof(TAREAPROCINFO), fp);
		}
		
		free( pstArea );
	}
	else
		fRet	=	FALSE;
    }
    //}}}


    if(fp) { fclose(fp); fp = NULL; }

    return  fRet;
}


BOOL CMapProc_LoadProcFile(TMAPPROCS* pstProcs, char *szFileName, TAREAPROCINFO* pstArea, int *piAreaSize)
{
    FILE *fp = NULL;
    unsigned int i;
    int fRet = TRUE;
    struct TProcFileHeader fh;
	
    if(NULL == (fp = fopen(szFileName, "rb")))
	{
		//log_error(1, "file %s open error", szFileName);
		goto _error;
	}
    fread(&fh, 1, sizeof(struct TProcFileHeader), fp);
    if(MAKEFOURCC('Q','Q','F',0x1a) != fh.copyright)
    {
        //log_error(1, "copyright error");
        goto _error;
    }
    if(MAKEFOURCC('P','R','O','C') != fh.id)
    {
        goto _error;
    }

/*
    if(nProcFileVersion != fh.version)
    {
        //log_error(1, "map proc file version error");
        goto _error;
    }
*/
    CMapProc_ClearAll(pstProcs);
    CMapProc_NakedRead(pstProcs, fp);
    
    //{{{
    if(fh.version >= nFileVersion2)
    {
	fread(&pstProcs->m_nMapWidth, 1, sizeof(unsigned int), fp);
	fread(&pstProcs->m_nMapHeight, 1, sizeof(unsigned int), fp);
	fread(&pstProcs->m_nAreaWidth, 1, sizeof(unsigned int), fp);
	fread(&pstProcs->m_nAreaHeight, 1, sizeof(unsigned int), fp);
	fread(&pstProcs->m_nHorzAreas, 1, sizeof(unsigned int), fp);
	fread(&pstProcs->m_nVertAreas, 1, sizeof(unsigned int), fp);
		
	if( pstArea && piAreaSize )
	{
		if( *piAreaSize >=pstProcs->m_nHorzAreas * pstProcs->m_nVertAreas * sizeof(TAREAPROCINFO) )
		{
			for(i=0; i<pstProcs->m_nHorzAreas * pstProcs->m_nVertAreas; i++)
			{
				fread(pstArea+i, 1, sizeof(TAREAPROCINFO), fp);
			}
			
			fRet	=	TRUE;
		}
		else
		{
			fRet	=	FALSE;
		}
		
		*piAreaSize	=	pstProcs->m_nHorzAreas * pstProcs->m_nVertAreas * sizeof(TAREAPROCINFO);
	}
	else
	{
		fRet	=	TRUE;
	}
    }
    //}}}

    if(fp) { fclose(fp); fp = NULL; }

    return  TRUE;
	
_error:
    if(fp) { fclose(fp); fp = NULL; }
    return  FALSE;
}


BOOL CMapProc_DelProc(TMAPPROCS* pstProcs, int lIndex)
{
    if(lIndex >= 0 && lIndex < nMaxProcs)
	{
        pstProcs->m_MapProc[lIndex].m_nID = 0;
		return TRUE;
	}

	return FALSE;
}


unsigned int CMapProc_FindMaxProcID(TMAPPROCS* pstProcs)
{
    unsigned int i;
    unsigned int proc_id = nMinProcID;
	
    for(i=0; i<pstProcs->m_nTotalProcs; i++)
	{
		if(pstProcs->m_MapProc[i].m_nID > proc_id)
		{
			proc_id = pstProcs->m_MapProc[i].m_nID;
		}
	}

	return proc_id;
}


unsigned int CMapProc_FindMinProcID(TMAPPROCS* pstProcs)
{
	unsigned int i;
	unsigned int proc_id = nMaxProcID;

	for(i=0; i<pstProcs->m_nTotalProcs; i++)
	{
		if(pstProcs->m_MapProc[i].m_nID < proc_id)
		{
			proc_id = pstProcs->m_MapProc[i].m_nID;
		}
	}

	return proc_id;
}


int CMapProc_FindEmptyProc(TMAPPROCS* pstProcs)
{
	int i, index = -1;

	for(i = 0; i < nMaxProcs; i ++)
	{
		if(IsProcAvailable(pstProcs->m_MapProc+i))
		{
			index = i;
			break;
		}
	}

	return index;
}


int CMapProc_AddProc(TMAPPROCS* pstProcs, unsigned int nID, int nType, POLY Poly)
{
    int index;

	index = CMapProc_FindEmptyProc(pstProcs);
	if(index >= 0)
	{
		pstProcs->m_MapProc[index].m_nID = nID;
		pstProcs->m_MapProc[index].m_nType = nType;
		pstProcs->m_MapProc[index].m_Poly = Poly;

		if((unsigned int)index >= pstProcs->m_nTotalProcs)
			pstProcs->m_nTotalProcs = index+1;
	}
    return index;
}


int CMapProc_MatchProc(TMAPPROCS* pstProcs, int x, int y, unsigned int *pnProcID, unsigned int *pnProcType)
{
	unsigned int i;
	int	matched_index;
	
    matched_index = -1;
    *pnProcID = -1;
    *pnProcType = typeDummyProc;
	
    for(i=0; i<pstProcs->m_nTotalProcs; i++)
    {
        if(pstProcs->m_MapProc[i].m_nID > 0)
        {
            if(D3_is_point_in_poly_underside( (POLY *)&pstProcs->m_MapProc[i].m_Poly, x, y) )
            {
                *pnProcID = pstProcs->m_MapProc[i].m_nID;
                *pnProcType = pstProcs->m_MapProc[i].m_nType;
                matched_index = i;
                break;
            }
        }
    }
    return matched_index;
}


int CMapProc_FindProcOfCoverPoint(TMAPPROCS* pstProcs, int x, int y)
{
	unsigned int i;
	int   find;
	
	find = -1;
	for(i=0; i<pstProcs->m_nTotalProcs; i++)
	{
		if(pstProcs->m_MapProc[i].m_nID > 0)
		{
			if(D3_is_point_in_poly_cover( &pstProcs->m_MapProc[i].m_Poly, x, y) )
			{
				find = i;
				break;
			}
		}
	}
	return(find);
}


char* CMapProc_GetProcTypeName(TMAPPROCS* pstProcs, unsigned int nType)
{
	switch(nType)
	{
	case typeDummyProc:
		return (char*)"无效事件";
	case typeAutoProc:
		return (char*)"自动事件";
	case typeItemProc:
		return (char*)"道具事件";
	case typeKeyProc:
		return (char*)"键盘事件";
	default:
		return (char*)"未知事件";
	}
}


unsigned int CMapProc_GetProcType(TMAPPROCS* pstProcs, int lIndex)
{
	unsigned int proc_type = typeDummyProc;
	if(lIndex >= 0 && lIndex < (int)pstProcs->m_nTotalProcs)
	{
		proc_type = pstProcs->m_MapProc[lIndex].m_nType;
	}
	return proc_type;
}


void CMapProc_SetProcType(TMAPPROCS* pstProcs, int lIndex, unsigned int nType)
{
	if(lIndex >= 0 && lIndex < (int)pstProcs->m_nTotalProcs)
	{
		pstProcs->m_MapProc[lIndex].m_nType = nType;
	}
}


unsigned int CMapProc_GetProcID(TMAPPROCS* pstProcs, int lIndex)
{
	unsigned int proc_id = 0;
	if(lIndex >= 0 && lIndex < (int)pstProcs->m_nTotalProcs)
	{
		proc_id = pstProcs->m_MapProc[lIndex].m_nID;
	}
	return proc_id;
}


void CMapProc_IncProcID(TMAPPROCS* pstProcs, int lIndex)
{
	if(lIndex >= 0 && lIndex < (int)pstProcs->m_nTotalProcs)
	{
		pstProcs->m_MapProc[lIndex].m_nID ++;
		if(pstProcs->m_MapProc[lIndex].m_nID > nMaxProcID)
		{
			pstProcs->m_MapProc[lIndex].m_nID = nMaxProcID;
		}
	}
}


void CMapProc_DecProcID(TMAPPROCS* pstProcs, int lIndex)
{
	if(lIndex >= 0 && lIndex < (int)pstProcs->m_nTotalProcs)
	{
		pstProcs->m_MapProc[lIndex].m_nID --;
		if(pstProcs->m_MapProc[lIndex].m_nID < nMinProcID)
		{
			pstProcs->m_MapProc[lIndex].m_nID = nMinProcID;
		}
	}
}

/*
void CMapProc_Redraw(TMAPPROCS* pstProcs, int lMapLeft, int lMapTop, unsigned int dwFlags)
{
    const int draw_poly_flags = PDP_VALID | PDP_BASE | PDP_HEIGHT | PDP_ARRIS 
        | PDP_TOP | PDP_CROSS | PDP_PROJECTION;//| PDP_ZHEIGHT;
	
    //if(! dwFlags) return;
	if(! g_pDisplay) return;

    for(unsigned int i=0; i<pstProcs->m_nTotalProcs; i++)
    {
        if(pstProcs->m_MapProc[i].m_nID > 0)
        {
            D3_draw_poly(lMapLeft, lMapTop, 0, draw_poly_flags, &pstProcs->m_MapProc[i].m_Poly);
			
			char info[128];
            sprintf((char *)info,"%u(%u)", pstProcs->m_MapProc[i].m_nID, pstProcs->m_MapProc[i].m_nType);
			g_pDisplay->DrawText(pstProcs->m_MapProc[i].m_Poly.bx + lMapLeft - 1, pstProcs->m_MapProc[i].m_Poly.by + lMapTop + 1,
                info, ARGB(255,0,0,0), 16);
			g_pDisplay->DrawText(pstProcs->m_MapProc[i].m_Poly.bx + lMapLeft, pstProcs->m_MapProc[i].m_Poly.by + lMapTop,
				info, ARGB(255,255,255,255), 16);
        }
    }
	
}
*/

void	InitMapProcs(TMAPPROCS* pstProcs)
{
	pstProcs->m_nTotalProcs = 0;
}


TMAPPROCS* AllocMapProcs(void)
{
	TMAPPROCS* pstProcs;

	pstProcs	=	(TMAPPROCS*) calloc(1, sizeof(TMAPPROCS));

	if( !pstProcs )
			return NULL;

	InitMapProcs(pstProcs);

	return pstProcs;
}


void	ReleaseMapProcs(TMAPPROCS* pstProcs)
{
	pstProcs->m_nTotalProcs = 0;
}

void	FreeMapProcs(TMAPPROCS* pstProcs)
{
	ReleaseMapProcs(pstProcs);
	
	free( pstProcs );
}

BOOL	SaveProcFile(TMAPPROCS* pstProcs, char *szFileName)
{
	unsigned int dwVersion;
	
	if( pstProcs->m_nAreaWidth && pstProcs->m_nAreaHeight && pstProcs->m_nHorzAreas && pstProcs->m_nVertAreas )
		dwVersion	=	nFileVersion2;
	else
		dwVersion	=	nProcFileVersion;
		
	return CMapProc_SaveProcFile(pstProcs, szFileName, dwVersion);
}

BOOL	LoadProcFile(TMAPPROCS* pstProcs, int* piMapSize, TAREAPROCINFO* pstArea, int* piAreaSize, char *szFileName)
{
	TMAPPROCS stProcs;
	int iSize;

	*piMapSize	=	sizeof(TMAPPROCS);

	if( !CMapProc_LoadProcFile(&stProcs, szFileName, pstArea, piAreaSize) )
		return FALSE;

	iSize	=	offsetof(TMAPPROCS, m_MapProc) + stProcs.m_nTotalProcs * sizeof(TMAPPROC) ;

	if( piMapSize && iSize > *piMapSize )
		return FALSE;

	*piMapSize	=	iSize;

	memcpy(pstProcs, &stProcs, iSize);

	return TRUE;
}

int	MatchProc(TMAPPROCS* pstProcs, int x, int y, unsigned int *pnProcID, unsigned int *pnProcType)
{
	return CMapProc_MatchProc(pstProcs, x, y, pnProcID, pnProcType);
}

int	ProcIDCompFunc(const void* pvKey, const void* pvItem)
{
	int nID;
	const TMAPPROC* pstProc;

	nID	=	(int)pvKey;
	pstProc = (const TMAPPROC*)pvItem;

	return nID - pstProc->m_nID;
}

int	MatchProcByID(TMAPPROCS* pstProcs, unsigned int* paiIDs, int iIDs, int x, int y, unsigned int *pnProcID, unsigned int *pnProcType)
{
	TMAPPROC* pstProc;
	int i;

	for(i=0; i<iIDs; i++)
	{
		// pstProc	=	bsearch((void*)paiIDs[i], (void*)pstProcs->m_MapProc, pstProcs->m_nTotalProcs, sizeof(TMAPPROC), ProcIDCompFunc);
		pstProc = &pstProcs->m_MapProc[paiIDs[i]];

		if( pstProc && D3_is_point_in_poly_underside( (POLY *)&pstProc->m_Poly, x, y) )
		{
			*pnProcID	=	pstProc->m_nID;
			*pnProcType	=	pstProc->m_nType;

			return pstProc - pstProcs->m_MapProc;
		}
	}

	return -1;
}

int	FindProcOfCoverPoint(TMAPPROCS* pstProcs, int x, int y)
{
	return CMapProc_FindProcOfCoverPoint(pstProcs, x, y);
}
