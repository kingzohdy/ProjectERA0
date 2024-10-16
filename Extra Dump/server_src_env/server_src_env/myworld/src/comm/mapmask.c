/*
**  MapMask.cpp
**  map mask functions.
**
**  Jack, 2002.12.11.
*/

#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include "mapmask.h"
#define UNUSED( var ) ((void)var)

/////////////////////////////////////////////////////////////////////////////
// 兩點擴散搜索寻径

#define XPATH_MASK_SOURCE           0x01000000      // is source pos
#define XPATH_MASK_TARGET           0x02000000      // is target pos
#define XPATH_MASK_FROM_SOURCE      0x04000000      // is diffuse from source pos
#define XPATH_MASK_FROM_TARGET      0x08000000      // is diffuse from target pos
#define XPATH_MASK_FROM_WHO         0x0C000000      // is from who ?
#define XPATH_MASK_STOP             0x10000000      // is stop pos
#define XPATH_MASK_PATH             0x20000000      // is the find path
#define XPATH_MASK_OFFSET           0x00ffffff      // offset

#define XPATH_TABLE_WIDTH           80  // old value: (screen width / minimal mask pixel width)
#define XPATH_TABLE_HEIGHT          60  // old value: (screen height / minimal mask pixel height)
#define XPATH_TABLE_SIZE            (XPATH_TABLE_WIDTH * XPATH_TABLE_HEIGHT)

#define XPATH_MAKE_OFFSET(x,y)      ((y) * XPATH_TABLE_WIDTH + (x))
#define XPATH_GET_X(offset)         ((offset) % XPATH_TABLE_WIDTH )
#define XPATH_GET_Y(offset)         ((offset) / XPATH_TABLE_WIDTH )
#define XPATH_FATHER_OFFSET(offset) (xpath_mask[XPATH_GET_Y((offset) & XPATH_MASK_OFFSET)][XPATH_GET_X( (offset) & XPATH_MASK_OFFSET)] & XPATH_MASK_OFFSET)

#define XPATHPROCESS(x, y, xpath_mask, xpath_stack, pmask, xpath_to, from_who, from_other, offset, offset2)          do \
{																	  \
    pmask = &xpath_mask[(y)][(x)];                                    \
    if(!(*pmask & XPATH_MASK_STOP ))			              \
    {                                                                 \
        if(!(*pmask & from_who))  /* not from the current  */\
        {                                                             \
            if(*pmask & from_other)  /* have been from the other, found meet pos */     \
            {                                                         \
                offset2 = *pmask;       /* backup the meet mask */    \
                *pmask &= ~XPATH_MASK_OFFSET;                         \
                *pmask |= from_who | offset;                 \
                xpath_stack[xpath_to++] = XPATH_MAKE_OFFSET((x),(y)); \
                find = TRUE;                                          \
            }                                                         \
            else    /* is a fresh pos */                              \
            {                                                         \
                *pmask |= from_who | offset;                 \
                xpath_stack[xpath_to ++] = XPATH_MAKE_OFFSET((x),(y));\
            }                                                         \
        }                                                             \
    }                                                                 \
}																	  \
while(0);

/////////////////////////////////////////////////////////////////////////////
// Common functions
//
// 计算环绕指定点的第N个攻击坐标
// 
//________________________________________________________________
// N = F(X, Y)                                                    
//                                                                
//                                                                
//                        |                                       
//                        |                                       
//             35  34  33 | 32  31  30                            
//             29  15  14 | 13  12  28                            
//             27  11  02 | 03  10  26                            
//------------------------+---------------------------> (X)       
//             25  09  01 | 00  08  24                                 
//             23  07  06 | 05  04  22                                 
//             21  20  19 | 18  17  16                               
//                        |                                       
//                        |                                        
//                        V (Y)                                        
//________________________________________________________________
//(x)
//                                                                
//                                                                
//                        |                                       
//                        |                                       
//            -5  -3  -1  |  1  3  5                            
//            -5  -3  -1  |  1  3  5                            
//            -5  -3  -1  |  1  3  5                            
//------------------------+---------------------------> (X)       
//            -5  -3  -1  |  1  3  5                                 
//            -5  -3  -1  |  1  3  5                                 
//            -5  -3  -1  |  1  3  5                               
//                        |                                        
//                        |                                        
//                        V (Y)                                        
//________________________________________________________________
//(y)
//                                                                
//                        |                                       
//                        |                                       
//            -5  -5  -5  | -5 -5 -5                            
//            -3  -3  -3  | -3 -3 -3                            
//            -1  -1  -1  | -1 -1 -1                            
//------------------------+---------------------------> (X)       
//             1   1   1  |  1  1  1                                 
//             3   3   3  |  3  3  3                                 
//             5   5   5  |  5  5  5                               
//                        |                                        
//                        |                                        
//                        V (Y)                                        
//

BOOL WindingAttackAxisPosition(int n, int *x, int *y)
{
	int	k, r, under, top, rest;

	if(n < 0)
	{
		*x = 0;
		*y = 0;
		return	TRUE;
	}

	//計算 n 落於第幾圈中
	k = (int) sqrt(n);
	k = k / 2 * 2 + 2;	//每行(列)的數值的個數(圈陣的大小)

	under = (k - 2) * (k - 2);	//最底下一行的起始值
	top = k * k - 1;			//最頂上一行的最終值
	r = k / 2 * 2 - 1;	//角頂點的(X/Y)數值

	//位於最底下一行
	if(n < under + k)	
	{
		*x = r + (under - n) * 2 ;
		*y = r;
		return	TRUE;
	}

	//位於最頂上一行
	if( n > top - k)
	{
		*x = (top - n) * 2 - r;
		*y = -r;
		return	TRUE;
	}

	//位於左右兩側
	rest = n - (under + k);
	*x = (rest % 2) ? -r : r;
	*y = (r - 2) - rest / 2 * 2;

	return	TRUE;
}


//
// 计算环绕指定点的第N个道具坐标
//
//________________________________________________________________
//(n)
//                    |
//                    |
//             24 23 22 21 20
//             19  8  7  6 18
//----------   17  5  0  4 16   --------------------> (X)
//             15  3  2  1 14
//             13 12 11 10  9
//                    |
//                    |    
//                    V (Y)
//                    
//________________________________________________________________
//(x)
//                    |
//                    |
//             -2 -1  0  1  2
//             -2 -1  0  1  2
//----------   -2 -1  0  1  2   --------------------> (X)
//             -2 -1  0  1  2
//             -2 -1  0  1  2
//                    |
//                    |    
//                    V (Y)
//
//
//________________________________________________________________
//(y)
//                    |
//                    |
//             -2 -2 -2 -2 -2
//             -1 -1 -1 -1 -1
//----------    0  0  0  0  0   --------------------> (X)
//              1  1  1  1  1
//              2  2  2  2  2
//                    |
//                    |    
//                    V (Y)
//
BOOL WindingItemAxisPosition(int n, int *x, int *y)
{
	int	k, r, under, top, rest;

	if(n == 0)
	{
		*x = 0;
		*y = 0;
		return	TRUE;
	}

	k = (int) sqrt(n);
	k = (k + 1) / 2 * 2 + 1;
	r = k / 2;
	under = (k - 2) * (k - 2);
	top = k * k;

	if(n < under + k)
	{
		*x = under + r - n;
		*y = r;
		return	TRUE;
	}

	if( n >= top - k)
	{
		*x = (top-1) - n - r;
		*y = -r;
		return	TRUE;
	}

	rest = n - (under + k);
	*x = (rest % 2) ? -r : r;
	*y = (r - 1) - rest / 2;

	return	TRUE;
}

void CMapMask_Release(TMAPMASK* pstMask)
{
    pstMask->m_lWidthMasks = 0;
    pstMask->m_lHeightMasks = 0;
    pstMask->m_lMaskPixelWidth = 1;
    pstMask->m_lMaskPixelHeight = 1;
}


int CMapMask_GetWidthMasks(TMAPMASK* pstMask)
{
    return  pstMask->m_lWidthMasks;
}

int CMapMask_GetHeightMasks(TMAPMASK* pstMask)
{
    return  pstMask->m_lHeightMasks;
}

int CMapMask_GetMaskPixelWidth(TMAPMASK* pstMask)
{
    return  pstMask->m_lMaskPixelWidth;
}

int CMapMask_GetMaskPixelHeight(TMAPMASK* pstMask)
{
    return  pstMask->m_lMaskPixelHeight;
}

void CMapMask_Fill(TMAPMASK* pstMask, MASK_BIT_TYPE mask)
{
    memset(pstMask->m_pMaskData, mask, pstMask->m_lWidthMasks * pstMask->m_lHeightMasks * sizeof(MASK_BIT_TYPE));
}

MASK_BIT_TYPE CMapMask_GetMask(TMAPMASK* pstMask, int lMaskX, int lMaskY)
{
    return pstMask->m_pMaskData[lMaskY * pstMask->m_lWidthMasks + lMaskX];
}

void CMapMask_SetMask(TMAPMASK* pstMask, int lMaskX, int lMaskY, MASK_BIT_TYPE MaskData)
{
    pstMask->m_pMaskData[lMaskY * pstMask->m_lWidthMasks + lMaskX] = MaskData;
}

void CMapMask_PixelPosToMaskPos(TMAPMASK* pstMask, int lPixelX, int lPixelY, int *lMaskX, int *lMaskY)
{
    if(lMaskX) *lMaskX = lPixelX / pstMask->m_lMaskPixelWidth;
    if(lMaskY) *lMaskY = lPixelY / pstMask->m_lMaskPixelHeight;
}

BOOL CMapMask_IsMaskPosOutRange(TMAPMASK* pstMask, int lMaskX, int lMaskY)
{
    if(lMaskX < 0 || lMaskX >= pstMask->m_lWidthMasks)
        return  TRUE;
    if(lMaskY < 0 || lMaskY >= pstMask->m_lHeightMasks)
        return  TRUE;
    return  FALSE;
}

int CMapMask_ApproachMaskDir(TMAPMASK* pstMask, int lMaskDx, int lMaskDy)
{
    static  int   dir[3][3] = 
    {               /*dx<0*/        /*dx=0*/            /*dx>0*/
        /*dy>0*/{maskDirLeftUp      ,maskDirUp          ,maskDirRightUp     },
        /*dy=0*/{maskDirLeft        ,maskDirCenter      ,maskDirRight       },
        /*dy<0*/{maskDirLeftDown    ,maskDirDown        ,maskDirRightDown   }
    };
    int   x_index, y_index;

    if(lMaskDx < 0) 
        x_index = 0;
    else if(lMaskDx == 0)
        x_index = 1;
    else
        x_index = 2;

    if(lMaskDy > 0)
        y_index = 0;
    else if(lMaskDy == 0)
        y_index = 1;
    else
        y_index = 2;

    return dir[y_index][x_index];
	UNUSED(pstMask);	//add by paraunused.pl
}

BOOL CMapMask_IsPixelMaskMatch(TMAPMASK* pstMask, int lPixelX, int lPixelY, MASK_BIT_TYPE CheckMask)
{
    int   mask_x, mask_y;
    MASK_BIT_TYPE mask;
    
    CMapMask_PixelPosToMaskPos(pstMask, lPixelX, lPixelY, &mask_x, &mask_y);

    //if it out of range, we set to stop.
    if(CMapMask_IsMaskPosOutRange(pstMask, mask_x, mask_y))
    {
        if ( CheckMask != maskWater && CheckMask != maskMine )
            return TRUE;
        else
            return FALSE;
    }

    mask = CMapMask_GetMask(pstMask, mask_x, mask_y);
    if( mask & CheckMask)
        return  TRUE;

    return  FALSE;
}

BOOL CMapMask_IsPixelStop(TMAPMASK* pstMask, int lPixelX, int lPixelY)
{
    return  CMapMask_IsPixelMaskMatch(pstMask, lPixelX, lPixelY, maskStop);
}

BOOL CMapMask_IsMaskLineThrough(TMAPMASK* pstMask, int lMaskX1, int lMaskY1, int lMaskX2, int lMaskY2, MASK_BIT_TYPE StopMask)
{
    int   dx,dy,dm,dn,m,n,k,u,v,l,sum;
    MASK_BIT_TYPE    mask;

    dx = lMaskX2 - lMaskX1, dy = lMaskY2 - lMaskY1;
    if(dx == 0)
    {
        u = lMaskY1, v = lMaskY2;
        if(dy < 0) u = lMaskY2, v = lMaskY1;
        for (l = u; l <= v; l ++)
        {
            mask = CMapMask_GetMask(pstMask, lMaskX2, l);
            if(mask & StopMask)
                return FALSE;
        }
        return TRUE;
    }
    if(dy==0)
    {
        u = lMaskX1, v = lMaskX2;
        if(dx < 0) u = lMaskX2,v = lMaskX1;
        for(l = u; l <= v; l ++)
        {
            mask = CMapMask_GetMask(pstMask, l, lMaskY2);
            if(mask & StopMask)
                return FALSE;
        }
        return TRUE;
    }
    
    dm = dn = 1;
    if(dx < 0) dx = -dx, dm = -1;
    if(dy < 0) dy = -dy, dn = -1;
    m = dx, n = dy, k = 1, u = lMaskX1, v = lMaskY1;
    if(dx<dy) m = dy, n = dx, k = dm, dm = dn, dn = k, k = 0, u = lMaskY1, v = lMaskX1;
    l = 0;
    sum = m;

    mask = CMapMask_GetMask(pstMask, lMaskX1, lMaskY1);
    if(mask & StopMask)
        return FALSE;
    while (sum != 0)
    {
        sum--;
        l += n;
        u += dm;
        if(l >= m)
        {
            v += dn;
            l -= m;
        }
        if(k == 1)
        {
            mask = CMapMask_GetMask(pstMask, u, v);
            if(mask & StopMask)
                return FALSE;
        }
        else
        {
            mask = CMapMask_GetMask(pstMask, v, u);
            if(mask & StopMask)
                return FALSE;
        }
    }
    return TRUE;
}

BOOL CMapMask_IsPixelLineThrough2(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask)
{
    int   dx,dy,dm,dn,m,n,k,u,v,l,sum;
    MASK_BIT_TYPE    mask;

    dx = lPixelX2 - lPixelX1, dy = lPixelY2 - lPixelY1;
    if(dx == 0)
    {
        u = lPixelY1, v = lPixelY2;
        if(dy < 0) u = lPixelY2, v = lPixelY1;
        for (l = u; l <= v; l ++)
        {
            mask = CMapMask_GetMask(pstMask, lPixelX2 / pstMask->m_lMaskPixelWidth, l / pstMask->m_lMaskPixelHeight);
            if(mask & StopMask)
                return FALSE;
        }
        return TRUE;
    }
    if(dy==0)
    {
        u = lPixelX1, v = lPixelX2;
        if(dx < 0) u = lPixelX2,v = lPixelX1;
        for(l = u; l <= v; l ++)
        {
            mask = CMapMask_GetMask(pstMask, l / pstMask->m_lMaskPixelWidth, lPixelY2 / pstMask->m_lMaskPixelHeight);
            if(mask & StopMask)
                return FALSE;
        }
        return TRUE;
    }
    
    dm = dn = 1;
    if(dx < 0) dx = -dx, dm = -1;
    if(dy < 0) dy = -dy, dn = -1;
    m = dx, n = dy, k = 1, u = lPixelX1, v = lPixelY1;
    if(dx<dy) m = dy, n = dx, k = dm, dm = dn, dn = k, k = 0, u = lPixelY1, v = lPixelX1;
    l = 0;
    sum = m;

    mask = CMapMask_GetMask(pstMask, lPixelX1 / pstMask->m_lMaskPixelWidth, lPixelY1 / pstMask->m_lMaskPixelHeight);
    if(mask & StopMask)
        return FALSE;
    while (sum != 0)
    {
        sum--;
        l += n;
        u += dm;
        if(l >= m)
        {
            v += dn;
            l -= m;
        }
        if(k == 1)
        {
            mask = CMapMask_GetMask(pstMask, u / pstMask->m_lMaskPixelWidth, v / pstMask->m_lMaskPixelHeight);
            if(mask & StopMask)
                return FALSE;
        }
        else
        {
            mask = CMapMask_GetMask(pstMask, v / pstMask->m_lMaskPixelWidth, u / pstMask->m_lMaskPixelHeight);
            if(mask & StopMask)
                return FALSE;
        }
    }
    return TRUE;
}

//返回到下一个格子
typedef struct
{
	int x;
	int y;
} Point2D;

typedef struct {
	Point2D s_StartPt;
	Point2D s_dPt;
	Point2D s_CurGrid;
} GridPoint;

/*
static LineIntersectGrid(const Point2D *pt1, const Point2D *pt2, GridPoint *pstGP)
{
	int y = -1;
	if( pstGP->s_dPt.x > 0 ) y = pstGP->s_dPt.y*(pt2->x-pstGP->s_StartPt.x)/pstGP->s_dPt.x + pstGP->s_StartPt.y;
	else if( pstGP->s_dPt.x < 0 ) y = pstGP->s_dPt.y*(pt1->x-pstGP->s_StartPt.x)/pstGP->s_dPt.x + pstGP->s_StartPt.y;

	if( y>=pt1->y && y<=pt2->y )  //从边 x=sx或者x=ex出去这个格子
	{
		if( pstGP->s_dPt.x > 0 ) pstGP->s_CurGrid.x++;
		else pstGP->s_CurGrid.x--;
	}
	else //从边 y=sy或者y=ey出去
	{
		if( pstGP->s_dPt.y > 0 ) pstGP->s_CurGrid.y++;
		else pstGP->s_CurGrid.y--;
	}
}
*/

static void LineIntersectGrid(const Point2D *pt1, const Point2D *pt2, GridPoint *pstGP)
{
	int dx = pstGP->s_dPt.x;
	int y1 = pt1->y * dx;
	int y2 = pt2->y * dx;
	if( dx > 0 )
	{
		int y = pstGP->s_dPt.y*(pt2->x - pstGP->s_StartPt.x) + pstGP->s_StartPt.y*dx;
		if( y == y2 )
		{
			pstGP->s_CurGrid.x++;
			pstGP->s_CurGrid.y++;
			return;
		}
		if( y>=y1 && y<y2 )
		{
			pstGP->s_CurGrid.x++;
			return;
		}
	}
	else if( dx < 0 )
	{
		int y = pstGP->s_dPt.y*(pt1->x - pstGP->s_StartPt.x) + pstGP->s_StartPt.y*dx;
		if( y == y2 ) {
			pstGP->s_CurGrid.y++;
			return;
		}
		if( y>y2 && y<=y1 )
		{
			pstGP->s_CurGrid.x--;
			return;
		}
	}

	if( pstGP->s_dPt.y > 0 ) pstGP->s_CurGrid.y++;
	else pstGP->s_CurGrid.y--;
}


BOOL CMapMask_IsPixelLineThrough(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask)
{
    MASK_BIT_TYPE    mask;
	Point2D pt1, pt2;
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	GridPoint stGP;

	stGP.s_StartPt.x = lPixelX1;
	stGP.s_StartPt.y = lPixelY1;
	stGP.s_dPt.x = lPixelX2 - lPixelX1;
	stGP.s_dPt.y = lPixelY2 - lPixelY1;

	grid_sx = lPixelX1/pstMask->m_lMaskPixelWidth;
	grid_sy = lPixelY1/pstMask->m_lMaskPixelHeight;
	grid_ex = lPixelX2/pstMask->m_lMaskPixelWidth;
	grid_ey = lPixelY2/pstMask->m_lMaskPixelHeight;

	stGP.s_CurGrid.x = grid_sx;
	stGP.s_CurGrid.y = grid_sy;
	mask = CMapMask_GetMask(pstMask, stGP.s_CurGrid.x, stGP.s_CurGrid.y );
	//if(mask & StopMask) return FALSE;

	while( stGP.s_CurGrid.x!=grid_ex || stGP.s_CurGrid.y!=grid_ey )
	{
		pt1.x = stGP.s_CurGrid.x * pstMask->m_lMaskPixelWidth;
		pt1.y = stGP.s_CurGrid.y * pstMask->m_lMaskPixelHeight;
		pt2.x = pt1.x + pstMask->m_lMaskPixelWidth;
		pt2.y = pt1.y + pstMask->m_lMaskPixelHeight;

		LineIntersectGrid(&pt1, &pt2, &stGP);
		mask = CMapMask_GetMask(pstMask, stGP.s_CurGrid.x, stGP.s_CurGrid.y );
		if(mask & StopMask) return FALSE;
	}
	return TRUE;
}

BOOL CMapMask_PixelLineThrough_Count(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask, int iCount)
{
	int iCountTmp = 0;
	int iThroughFlag = 0;
       MASK_BIT_TYPE    mask;
	Point2D pt1, pt2;
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	GridPoint stGP;

	stGP.s_StartPt.x = lPixelX1;
	stGP.s_StartPt.y = lPixelY1;
	stGP.s_dPt.x = lPixelX2 - lPixelX1;
	stGP.s_dPt.y = lPixelY2 - lPixelY1;

	grid_sx = lPixelX1/pstMask->m_lMaskPixelWidth;
	grid_sy = lPixelY1/pstMask->m_lMaskPixelHeight;
	grid_ex = lPixelX2/pstMask->m_lMaskPixelWidth;
	grid_ey = lPixelY2/pstMask->m_lMaskPixelHeight;

	stGP.s_CurGrid.x = grid_sx;
	stGP.s_CurGrid.y = grid_sy;
	mask = CMapMask_GetMask(pstMask, stGP.s_CurGrid.x, stGP.s_CurGrid.y );
	
	while( stGP.s_CurGrid.x!=grid_ex || stGP.s_CurGrid.y!=grid_ey )
	{
		pt1.x = stGP.s_CurGrid.x * pstMask->m_lMaskPixelWidth;
		pt1.y = stGP.s_CurGrid.y * pstMask->m_lMaskPixelHeight;
		pt2.x = pt1.x + pstMask->m_lMaskPixelWidth;
		pt2.y = pt1.y + pstMask->m_lMaskPixelHeight;

		LineIntersectGrid(&pt1, &pt2, &stGP);
		mask = CMapMask_GetMask(pstMask, stGP.s_CurGrid.x, stGP.s_CurGrid.y );
		if(mask & StopMask)
		{
			if (0 == iThroughFlag)
			{
				iCountTmp++;

				if (iCount < iCountTmp)
				{
					return FALSE;
				}
			}

			iThroughFlag = 1;
		}
		else
		{
			iThroughFlag = 0;
		}
	}
	
	return TRUE;
}

BOOL CMapMask_IsPixelLineThrough3(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, 
				MASK_BIT_TYPE StopMask,int *lPixelX3, int *lPixelY3)
{
    MASK_BIT_TYPE    mask;
	Point2D pt1, pt2;
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	GridPoint stGP;

	stGP.s_StartPt.x = lPixelX1;
	stGP.s_StartPt.y = lPixelY1;
	stGP.s_dPt.x = lPixelX2 - lPixelX1;
	stGP.s_dPt.y = lPixelY2 - lPixelY1;

	grid_sx = lPixelX1/pstMask->m_lMaskPixelWidth;
	grid_sy = lPixelY1/pstMask->m_lMaskPixelHeight;
	grid_ex = lPixelX2/pstMask->m_lMaskPixelWidth;
	grid_ey = lPixelY2/pstMask->m_lMaskPixelHeight;

	stGP.s_CurGrid.x = grid_sx;
	stGP.s_CurGrid.y = grid_sy;
	mask = CMapMask_GetMask(pstMask, stGP.s_CurGrid.x, stGP.s_CurGrid.y );
	//if(mask & StopMask) return FALSE;

	while( stGP.s_CurGrid.x!=grid_ex || stGP.s_CurGrid.y!=grid_ey )
	{
		pt1.x = stGP.s_CurGrid.x * pstMask->m_lMaskPixelWidth;
		pt1.y = stGP.s_CurGrid.y * pstMask->m_lMaskPixelHeight;
		pt2.x = pt1.x + pstMask->m_lMaskPixelWidth;
		pt2.y = pt1.y + pstMask->m_lMaskPixelHeight;

		LineIntersectGrid(&pt1, &pt2, &stGP);	
		mask = CMapMask_GetMask(pstMask, stGP.s_CurGrid.x, stGP.s_CurGrid.y );
		if(mask & StopMask)
		{
			*lPixelX3 = pt1.x;
			*lPixelY3 = pt1.y;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMapMask_FindMaskNearOpenPixel(TMAPMASK* pstMask, int *lPixelX, int *lPixelY, MASK_BIT_TYPE StopMask)
{
    MASK_BIT_TYPE    mask;
    int   mask_cx, mask_cy;
    int   mask_x, mask_y;
    int   n, x, y;

    mask_cx = *lPixelX / pstMask->m_lMaskPixelWidth;
    mask_cy = *lPixelY / pstMask->m_lMaskPixelHeight;

    if(mask_cx < 0) mask_cx = 0;
    if(mask_cx >= pstMask->m_lWidthMasks) mask_cx = pstMask->m_lWidthMasks-1;
    if(mask_cy < 0) mask_cy = 0;
    if(mask_cy >= pstMask->m_lHeightMasks) mask_cy = pstMask->m_lHeightMasks -1;

    // . . . . . . . . . . . . . . . . . . . . .
    // . . . . . . . . . . . . . . . . . . . . .
    // . . . . . . . . . . . . . . . . . . . . .
    // . . . . . . . . n n n n n n n n n . . . .
    // . . . . . . . . n . . . . . . . n . . . .
    // . . . . . . . . n . 2 2 2 2 2 . n . . . .
    // . . . . . . . . n . 2 1 1 1 2 . n . . . .
    // . . . . . . . . n . 2 1 x 1 2 . n ------X
    // . . . . . . . . n . 2 1 1 1 2 . n . . . .
    // . . . . . . . . n . 2 2 2 2 2 . n . . . .
    // . . . . . . . . n . . . . . . . n . . . .
    // . . . . . . . . n n n n n n n n n . . . .
    // . . . . . . . . . . . . | . . . . . . . .
    // . . . . . . . . . . . . | . . . . . . . .
    // . . . . . . . . . . . . y . . . . . . . .

    for(n = 1; n < 5; n ++ )
    {
        //bottom line -------------------------------------
        y = n;
        mask_y = mask_cy + y;
        for(x = n; x <= -n; x --)
        {
            mask_x = mask_cx + x;
            mask = CMapMask_GetMask(pstMask, mask_x, mask_y);
            if(! (mask & StopMask) )
            {
                *lPixelX = mask_x * pstMask->m_lMaskPixelWidth;
                *lPixelY = mask_y * pstMask->m_lMaskPixelHeight;
                return TRUE;
            }
        }

        //left line ----------------------------------------
        x = -n;
        mask_x = mask_cx + x;
        for(y = n-1; y <= -n+1; y --)
        {
            mask_y = mask_cy + y;
            mask = CMapMask_GetMask(pstMask, mask_x, mask_y);
            if(! (mask & StopMask) )
            {
                *lPixelX = mask_x * pstMask->m_lMaskPixelWidth;
                *lPixelY = mask_y * pstMask->m_lMaskPixelHeight;
                return TRUE;
            }
        }

        //top line -----------------------------------------
        y = -n;
        mask_y = mask_cy + y;
        for(x = n; x <= -n; x --)
        {
            mask_x = mask_cx + x;
            mask = CMapMask_GetMask(pstMask, mask_x, mask_y);
            if(! (mask & StopMask) )
            {
                *lPixelX = mask_x * pstMask->m_lMaskPixelWidth;
                *lPixelY = mask_y * pstMask->m_lMaskPixelHeight;
                return TRUE;
            }
        }

        //right line ---------------------------------------
        x = n;
        mask_x = mask_cx + x;
        for(y = n-1; y <= -n+1; y --)
        {
            mask_y = mask_cy + y;
            mask = CMapMask_GetMask(pstMask, mask_x, mask_y);
            if(! (mask & StopMask) )
            {
                *lPixelX = mask_x * pstMask->m_lMaskPixelWidth;
                *lPixelY = mask_y * pstMask->m_lMaskPixelHeight;
                return TRUE;
            }
        }
    }
    return  FALSE;
}

BOOL CMapMask_FindMaskNearThroughPixel(TMAPMASK* pstMask, int lPixelSx, int lPixelSy, int *lPixelEx, int *lPixelEy, MASK_BIT_TYPE StopMask)
{
    int   dx,dy,dm,dn,m,n,k,u,v,l,sum;
    MASK_BIT_TYPE    stop, mask;
    int   x1, y1, x2, y2;
    int   last_x, last_y;

    x1 = lPixelSx;
    y1 = lPixelSy;
    x2 = *lPixelEx;
    y2 = *lPixelEy;
    stop = (MASK_BIT_TYPE)StopMask;

    dx = x2 - x1, dy = y2 - y1;
    if(dx == 0)
    {
        if(dy > 0)
        {
            for(l = y1; l <= y2; l ++)
            {
                mask = CMapMask_GetMask(pstMask, x2 / pstMask->m_lMaskPixelWidth, l / pstMask->m_lMaskPixelHeight);
                if(mask & stop)
                {
                    //get last point
                    *lPixelEx = x2;
                    *lPixelEy = l - 1;
                    return  FALSE;
                }
            }
            return  TRUE;
        }
        else
        {
            for(l = y1; l >= y2; l --)
            {
                mask = CMapMask_GetMask(pstMask, x2 / pstMask->m_lMaskPixelWidth, l / pstMask->m_lMaskPixelHeight);
                if(mask & stop)
                {
                    //get last point
                    *lPixelEx = x2;
                    *lPixelEy = l + 1;
                    return  FALSE;
                }
            }
            return  TRUE;
        }
    }
    if(dy == 0)
    {
        if( dx > 0)
        {
            for(l = x1; l <= x2; l ++)
            {
                mask = CMapMask_GetMask(pstMask, l / pstMask->m_lMaskPixelWidth, y2 / pstMask->m_lMaskPixelHeight);
                if(mask & stop)
                {
                    *lPixelEx = l - 1;
                    *lPixelEy = y2;
                    return FALSE;
                }
            }
            return  TRUE;
        }
        else
        {
            for(l = x1; l >= x2; l --)
            {
                mask = CMapMask_GetMask(pstMask, l / pstMask->m_lMaskPixelWidth, y2 / pstMask->m_lMaskPixelHeight);
                if(mask & stop)
                {
                    *lPixelEx = l + 1;
                    *lPixelEy = y2;
                    return FALSE;
                }
            }
            return  TRUE;
        }
    }
    
    dm = dn = 1;
    if(dx < 0) dx = -dx, dm = -1;
    if(dy < 0) dy = -dy, dn = -1;
    m = dx, n = dy, k = 1, u = x1, v = y1;
    if(dx < dy) m = dy, n = dx, k = dm, dm = dn, dn = k, k = 0, u = y1, v = x1;
    l = 0;
    sum = m;

    mask = CMapMask_GetMask(pstMask, x1 / pstMask->m_lMaskPixelWidth, y1 / pstMask->m_lMaskPixelHeight);
    if(mask & stop)
    {
        *lPixelEx = x1;
        *lPixelEy = y1;
        return  FALSE;
    }

    last_x = x1;
    last_y = y1;
    while (sum != 0)
    {
        sum --;
        l += n;
        u += dm;
        if(l >= m)
        {
            v += dn;
            l -= m;
        }
        if(k == 1)
        {
            mask = CMapMask_GetMask(pstMask, u / pstMask->m_lMaskPixelWidth, v / pstMask->m_lMaskPixelHeight);
            if(mask & stop)
            {
                *lPixelEx = last_x;
                *lPixelEy = last_y;
                return FALSE;
            }
            last_x = u;
            last_y = v;
        }
        else
        {
            mask = CMapMask_GetMask(pstMask, v / pstMask->m_lMaskPixelWidth, u / pstMask->m_lMaskPixelHeight);
            if(mask & stop)
            {
                *lPixelEx = last_x;
                *lPixelEy = last_y;
                return FALSE;
            }
            last_x = v;
            last_y = u;
        }
    }

    return TRUE;
}

void CMapMask_InterceptGeometryLine(TMAPMASK* pstMask, int lSx, int lSy, int *lEx, int *lEy, int lLen)
{
    int   dx, dy;
    double  dr, k;

    dx = *lEx - lSx;
    dy = *lEy - lSy;
    dr = sqrt( dx * dx + dy * dy );
    if(lLen < dr)
    {
        k = lLen / dr;
        *lEx = (int)( lSx + dx * k );
        *lEy = (int)( lSy + dy * k );
    }
	UNUSED(pstMask);	//add by paraunused.pl
}

void CMapMask_InterceptRasterLine(TMAPMASK* pstMask, int lSx, int lSy, int *lEx, int *lEy, int lLen)
{
    int   x1, y1, x2, y2;
    int   dx,dy,dm,dn,m,n,k,u,v,l,sum;
    int   dist, samp_dist;
    int   last_x, last_y;

    x1 = lSx, y1 = lSy;
    x2 = *lEx, y2 = *lEy;
    dx = x2 - x1, dy = y2 - y1;

    last_x = lSx;
    last_y = lSy;
    samp_dist = lLen * lLen;
    dist = dx * dx + dy * dy;
    if(dist <= samp_dist)
    {
        return;
    }

    if(dx == 0)
    {
        if(dy < 0)
        {
            last_x = x1;
            last_y = y1 - lLen;
        }
        else
        {
            last_x = x1;
            last_y = y1 + lLen;
        }
        *lEx = last_x;
        *lEy = last_y;
        return;
    }

    if(dy == 0)
    {
        if(dx < 0)
        {
            last_x = x1 - lLen;
            last_y = y1;
        }
        else
        {
            last_x = x1 + lLen;
            last_y = y1;
        }
        *lEx = last_x;
        *lEy = last_y;
        return;
    }
    
    dm = dn = 1;
    if(dx < 0) dx = -dx, dm = -1;
    if(dy < 0) dy = -dy, dn = -1;
    m = dx, n = dy, k = 1, u = x1, v = y1;
    if(dx < dy) m = dy, n = dx, k = dm, dm = dn, dn = k, k = 0, u = y1, v = x1;
    l = 0;
    sum = m;
    //pixel (x1,y1)
    //omit here...
    while (sum != 0)
    {
        sum--;
        l += n;
        u += dm;
        if(l >= m)
        {
            v += dn;
            l -= m;
        }
        if(k == 1)
        {
            //pixel (u, v)
            dist = (u - x1) * (u - x1) + (v - y1) * (v - y1);
            if(dist <= samp_dist)
            {
                last_x = u;
                last_y = v;
            }
            else
            {
                break;
            }
        }
        else
        {
            //pixel (v, u)
            dist = (v - x1) * (v - x1) + (u - y1) * (u - y1);
            if(dist <= samp_dist)
            {
                last_x = v;
                last_y = u;
            }
            else
            {
                break;
            }
        }
    }
    *lEx = last_x;
    *lEy = last_y;
    return;
	UNUSED(pstMask);	//add by paraunused.pl
}

BOOL CMapMask_NakedRead(TMAPMASK* pstMask, int* piSize, FILE *fp)
{
	int iSize;
	int iNeed;

	if( piSize )
		iSize	=	(*piSize - offsetof(TMAPMASK, m_pMaskData));
	else
		iSize	=	pstMask->m_iSize - offsetof(TMAPMASK, m_pMaskData);

    fread(&pstMask->m_lWidthMasks, 1, sizeof(pstMask->m_lWidthMasks), fp);
    fread(&pstMask->m_lHeightMasks, 1, sizeof(pstMask->m_lHeightMasks), fp);
    fread(&pstMask->m_lMaskPixelWidth, 1, sizeof(pstMask->m_lMaskPixelWidth), fp);
    fread(&pstMask->m_lMaskPixelHeight, 1, sizeof(pstMask->m_lMaskPixelHeight), fp);

	iNeed	=	pstMask->m_lWidthMasks * pstMask->m_lHeightMasks * sizeof(MASK_BIT_TYPE);

	if(  iNeed> iSize )
		return FALSE;

    memset(pstMask->m_pMaskData, 0, iNeed);

    fread(pstMask->m_pMaskData, 1, iNeed, fp);

	if( piSize )
	{
		*piSize	=	iNeed + offsetof(TMAPMASK, m_pMaskData);
	}

    return  TRUE;
}

BOOL CMapMask_NakedWrite(TMAPMASK* pstMask, FILE *fp)
{
    fwrite(&pstMask->m_lWidthMasks, 1, sizeof(pstMask->m_lWidthMasks), fp);
    fwrite(&pstMask->m_lHeightMasks, 1, sizeof(pstMask->m_lHeightMasks), fp);
    fwrite(&pstMask->m_lMaskPixelWidth, 1, sizeof(pstMask->m_lMaskPixelWidth), fp);
    fwrite(&pstMask->m_lMaskPixelHeight, 1, sizeof(pstMask->m_lMaskPixelHeight), fp);
    fwrite(pstMask->m_pMaskData, 1, pstMask->m_lWidthMasks * pstMask->m_lHeightMasks * sizeof(MASK_BIT_TYPE), fp);
        
	return TRUE;
}

void CMapMask_InitFileHeader(struct TMaskFileHeader *pHeader)
{
    pHeader->id = MAKEFOURCC('M','A','S','K');
    pHeader->version = nMaskFileVersion;
    pHeader->head_size = sizeof(struct TMaskFileHeader);
}


BOOL CMapMask_SaveMaskFile(TMAPMASK* pstMask, char *szFileName)
{
    FILE *fp = NULL;
    struct TMaskFileHeader fh;
	
    if(NULL == (fp = fopen(szFileName, "wb")))
		return FALSE;
	
    CMapMask_InitFileHeader(&fh);
    
    fwrite(&fh, 1, sizeof(struct TMaskFileHeader), fp);
    CMapMask_NakedWrite(pstMask, fp);
	
    if(fp) { fclose(fp); fp = NULL; }
	
    return  TRUE;
}

struct MNavHead
{
	char magic_number[4];
	int version;
	int headsize;
	int	filesize;
	int ngridx;
	int ngridz;
	int gridsizex;
	int gridsizez;
};

BOOL CMapMask_LoadMaskFile(TMAPMASK* pstMask, int* piSize, char *szFileName)
{
    FILE *fp = NULL;
	BOOL fRet;
    struct TMaskFileHeader fh;
	//struct MNavHead head;
	int flen;
	
    if(NULL == (fp = fopen(szFileName, "rb")))
	{
		//log_error(1, "file %s open error", szFileName);
		goto _error;
	}
	//fread(&head, sizeof(head), 1, fp);

    fread(&fh, 1, sizeof(struct TMaskFileHeader), fp);
    if(MAKEFOURCC('M','A','S','K') != fh.id)
    {
        //log_error(1, "map proc file format error");
        goto _error;
    }
    if(nMaskFileVersion < fh.version)
    {
        //log_error(1, "map proc file version error");
        goto _error;
    }

	fread(&flen, sizeof(int), 1, fp);

	CMapMask_Release(pstMask);
	fRet	=	CMapMask_NakedRead(pstMask, piSize, fp);

    if(fp) { fclose(fp); fp = NULL; }
	
    return  fRet;
	
_error:
    if(fp) { fclose(fp); fp = NULL; }
    return  FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Misc functions
//
// 寻找最佳攻击位置
//
BOOL CMapMask_FindBestAttackPosition(TMAPMASK* pstMask, int x, int y, int dest_x, int dest_y, int attack_distance, 
                                      int *attack_x, int *attack_y)
{
	int	mask_cx, mask_cy;
	int	mask_x, mask_y;
	int	find_x, find_y;
	int	mask_px, mask_py;
	int	near_distance, min_near_distance;
	int	walk_distance2, min_walk_distance2;
	int	best_x, best_y, find_best;
	int	i;
	MASK_BIT_TYPE	mask;

	mask_cx = dest_x / pstMask->m_lMaskPixelWidth;
	mask_cy = dest_y / pstMask->m_lMaskPixelHeight;

	min_walk_distance2 = 999999;
	min_near_distance = 999999;

	best_x = x;
	best_y = y;
	find_best = FALSE;
	for(i = 4; i < 64; i ++)	//4 ~ 64 for 3 circles(4~15, 16~35, 36~63).
	{
		WindingAttackAxisPosition(i, &find_x, &find_y);
		mask_x = mask_cx + find_x;
		mask_y = mask_cy + find_y;
		if( mask_x >= 0 && mask_x < pstMask->m_lWidthMasks && 
			mask_y >= 0 && mask_y < pstMask->m_lHeightMasks )
		{
			mask = CMapMask_GetMask(pstMask, mask_x, mask_y);
			if(mask & maskDynamicNpc)
				continue;

			mask_px = mask_x * pstMask->m_lMaskPixelWidth + pstMask->m_lMaskPixelWidth / 2;
			mask_py = mask_y * pstMask->m_lMaskPixelHeight + pstMask->m_lMaskPixelHeight / 2;

			near_distance = max( abs(mask_x - mask_cx), abs(mask_y - mask_cy) );	// circle radius for distance.
			if(near_distance > min_near_distance)
				continue;
			else if(near_distance < min_near_distance)
			{
				min_near_distance = near_distance;
				best_x = mask_px;
				best_y = mask_py;
				find_best = TRUE;
			}
			else //near_distance == min_near_distance
			{
				walk_distance2 = (mask_px - x) * (mask_px - x) + (mask_py - y) * (mask_py - y);
				if(walk_distance2 < min_walk_distance2)
				{
					min_walk_distance2 = walk_distance2;
					best_x = mask_px;
					best_y = mask_py;
					find_best = TRUE;
				}
			}
		}
	}

	*attack_x = best_x;
	*attack_y = best_y;

	return	find_best;
	UNUSED(attack_distance);	//add by paraunused.pl
}

void InitMapMask(TMAPMASK* pstMask, int iSize)
{
    pstMask->m_lWidthMasks = 0;
    pstMask->m_lHeightMasks = 0;
    pstMask->m_lMaskPixelWidth = 1;
    pstMask->m_lMaskPixelHeight = 1;
    pstMask->m_iSize	=	iSize;
}

TMAPMASK* AllocMapMask(int iSize)
{
	TMAPMASK* pstMask;

	if( (unsigned int)iSize<=offsetof(TMAPMASK, m_pMaskData) )
		return NULL;

	pstMask	=	(TMAPMASK*) calloc(1, iSize);

	if( !pstMask )
			return NULL;

	InitMapMask(pstMask, iSize);

	return pstMask;
}

void ReleaseMapMask(TMAPMASK* pstMask)
{
    pstMask->m_lWidthMasks = 0;
    pstMask->m_lHeightMasks = 0;
    pstMask->m_lMaskPixelWidth = 1;
    pstMask->m_lMaskPixelHeight = 1;
}

void FreeMapMask(TMAPMASK* pstMask)
{
	ReleaseMapMask(pstMask);

	free(pstMask);
}

BOOL	SaveMaskFile(TMAPMASK* pstMask, char *szFileName)
{
	return CMapMask_SaveMaskFile(pstMask, szFileName);
}

BOOL	LoadMaskFile(TMAPMASK* pstMask, int* piSize, char *szFileName)
{
	return CMapMask_LoadMaskFile(pstMask, piSize, szFileName);
}

BOOL    IsMaskLineThrough(TMAPMASK* pstMask, int lMaskX1, int lMaskY1, int lMaskX2, int lMaskY2, MASK_BIT_TYPE StopMask)
{
	if( lMaskX1<0 || lMaskX1>=pstMask->m_lWidthMasks ||
	    lMaskY1<0 || lMaskY1>=pstMask->m_lHeightMasks ||
	    lMaskX2<0 || lMaskX2>=pstMask->m_lWidthMasks ||
	    lMaskY2<0 || lMaskY2>=pstMask->m_lHeightMasks )
		return 0;
	
	return CMapMask_IsMaskLineThrough(pstMask, lMaskX1, lMaskY1, lMaskX2, lMaskY2, StopMask);
}

BOOL    IsPixelLineThrough(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask)
{
	int maxx,maxy;

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

	if( lPixelX1<0 || lPixelX1>=maxx ||
	    lPixelY1<0 || lPixelY1>=maxy ||
	    lPixelX2<0 || lPixelX2>=maxx ||
	    lPixelY2<0 || lPixelY2>=maxy )
		return 0;

	return CMapMask_IsPixelLineThrough(pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2, StopMask);
}

BOOL PixelLineThroughCount(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask, int iCount)
{
	int maxx,maxy;

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

	if( lPixelX1<0 || lPixelX1>=maxx ||
	    lPixelY1<0 || lPixelY1>=maxy ||
	    lPixelX2<0 || lPixelX2>=maxx ||
	    lPixelY2<0 || lPixelY2>=maxy )
		return 0;

	return CMapMask_PixelLineThrough_Count(pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2, StopMask, iCount);
}

BOOL    IsPixelLineThrough2(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask)
{
	int maxx,maxy;

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

	if( lPixelX1<0 || lPixelX1>=maxx ||
	    lPixelY1<0 || lPixelY1>=maxy ||
	    lPixelX2<0 || lPixelX2>=maxx ||
	    lPixelY2<0 || lPixelY2>=maxy )
		return 0;

	return CMapMask_IsPixelLineThrough2(pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2, StopMask);
}

BOOL    IsPixelLineThrough3(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask,
					int *lPixelX3, int *lPixelY3)
{
	int maxx,maxy;

	maxx	=	pstMask->m_lWidthMasks * pstMask->m_lMaskPixelWidth;
	maxy	=	pstMask->m_lHeightMasks* pstMask->m_lMaskPixelHeight;

	if( lPixelX1<0 )
	{
		lPixelX1 = 0;
	}
	if( lPixelY1<0 )
	{
		lPixelY1 = 0;
	}
	if( lPixelX2<0 )
	{
		lPixelX2 = 0;
	}
	if( lPixelY2<0 )
	{
		lPixelY2 = 0;
	}
	
	if(lPixelX1>=maxx )
	{
		lPixelX1 = maxx -1;
	}
	if(lPixelY1>=maxy)
	{
		lPixelY1 = maxy -1;
	}
	if(lPixelX2>=maxx )
	{
		lPixelX2 = maxx -1;
	}
	if(lPixelY2>=maxy )
	{
		lPixelY2 = maxy -1;
	}
	
	return CMapMask_IsPixelLineThrough3(pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2, StopMask,lPixelX3, lPixelY3);
}


void InterceptRasterLine(TMAPMASK* pstMask, int lSx, int lSy, int *lEx, int *lEy, int lLen)
{
	CMapMask_InterceptRasterLine(pstMask, lSx, lSy, lEx, lEy, lLen);
}

BOOL IsPixelStop(TMAPMASK* pstMask, int lPixelX, int lPixelY)
{
    return  CMapMask_IsPixelMaskMatch(pstMask, lPixelX, lPixelY, maskStop|maskDynamicNpc);
}

BOOL IsPixelMagicStop(TMAPMASK* pstMask, int lPixelX, int lPixelY)
{
    return  CMapMask_IsPixelMaskMatch(pstMask, lPixelX, lPixelY, maskLoMagic|maskDynamicNpc);
}

BOOL  IsPointInCircle( int x, int y, int ox, int oy, int radius )
{
	int r2 = (x-ox)*(x-ox) + (y-oy)*(y-oy);
	return r2<radius*radius ? 1 : 0;
}

#define MY_PI 3.1415926f

float mycos( float angle )
{
	return (float)cos(angle*MY_PI/180.0f);
}

float mysin( float angle )
{
	return (float)sin(angle*MY_PI/180.0f);
}

BOOL  IsPointInFan( int x, int y, int ox, int oy, int radius, int face_angle, int angle_range )
{
	float EPSILON = 5.0f;
	float r;
	float mydot;

	if( !IsPointInCircle(x, y, ox, oy, radius) ) return 0;
		
	r = (float)sqrt((x-ox)*(x-ox) + (y-oy)*(y-oy));
	if( r < EPSILON ) return 1;
	
	mydot = ((x-ox)/r)*mycos(face_angle) - ((y-oy)/r)*mysin(face_angle);
	return mydot>mycos(angle_range*0.5f) ? 1 : 0;
}

BOOL  IsPointInRectangle( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle )
{
	int x1 = (int)( (x-ox)*mycos(face_angle) - (y-oy)*mysin(face_angle) );
	int y1 = (int)((x-ox)*mysin(face_angle) + (y-oy)*mycos(face_angle) );
	
	if( x1>0 && x1<xlen && y1>-ylen/2 && y1<ylen/2 ) return 1;
	else return 0;
}

BOOL  IsPointInRectangle2( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle )
{
	int x1 = (int)( (x-ox)*mycos(face_angle) - (y-oy)*mysin(face_angle) );
	int y1 = (int)((x-ox)*mysin(face_angle) + (y-oy)*mycos(face_angle) );
	
	if( x1>-xlen/2 && x1<xlen/2 && y1>-ylen/2 && y1<ylen/2 ) return 1;
	else return 0;
}

BOOL IsPixelWater(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY, int lDir, int lLen)
{
	float farf;
	int lPixelX;
	int lPixelY;

	farf = 2.0f * (lDir*3.1415926f/180);
    lPixelX = lCurPixelX + (int)(cosf(farf)*(float)lLen);
    lPixelY = lCurPixelY + (int)(sinf(farf)*(float)lLen);
    return  CMapMask_IsPixelMaskMatch(pstMask, lPixelX, lPixelY, maskWater);
}
    
BOOL IsPixelMine(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY)
{
    return  CMapMask_IsPixelMaskMatch(pstMask, lCurPixelX, lCurPixelY, maskMine);
}

BOOL    IsPixelLoMagic(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY)
{
    return  CMapMask_IsPixelMaskMatch(pstMask, lCurPixelX, lCurPixelY, maskLoMagic);
}	

int FaceAngleBetween( int from_x, int from_y, int to_x, int to_y )
{
	int dx;
	int dy;
	double length;
	double tmp;
	double angle;

	dx = to_x - from_x;
	dy = to_y - from_y;

	length = sqrt(dx*dx + dy*dy);

	if( length < 0.5 ) return 0;

	tmp = dx / length;

	angle = acos(tmp) * 180 / MY_PI;

	if( dy > 0 ) angle = (360 - angle);

	return (int)angle;
}


//-----------------------------------------------------------------------------------------
int SetMapMask(TMAPMASK* pstMask, int x, int y, int w, int h, MASK_BIT_TYPE uchOrMask, MASK_BIT_TYPE uchAndMask)
{
	MASK_BIT_TYPE uchMask = 0;
	//int i = 0;
	//int j = 0;
	int iMaskX1 = 0;
	int iMaskX2 = 0;
	int iMaskY1 = 0;
	int iMaskY2 = 0;
	int iMaskX = 0;
	int iMaskY = 0;

	iMaskX1 = x / pstMask->m_lMaskPixelWidth;
	iMaskX2 = (x + w) / pstMask->m_lMaskPixelWidth;	
	iMaskY1 = y / pstMask->m_lMaskPixelHeight;
	iMaskY2 = (y + h) / pstMask->m_lMaskPixelHeight;
	for (  iMaskY = iMaskY1; iMaskY <= iMaskY2;  ++iMaskY )
	{
		if ( iMaskY < 0 ||iMaskY >= pstMask->m_lHeightMasks  )
		{
			continue;
		}
		
		for ( iMaskX = iMaskX1; iMaskX <= iMaskX2; ++iMaskX )
		{
			if ( iMaskX < 0 || iMaskX >= pstMask->m_lWidthMasks )
			{
				continue;
			}

			uchMask = CMapMask_GetMask(pstMask, iMaskX, iMaskY);
			uchMask |= uchOrMask;
			uchMask &= uchAndMask;
			CMapMask_SetMask(pstMask, iMaskX, iMaskY, uchMask);
		}
	}

	return 0;
}


