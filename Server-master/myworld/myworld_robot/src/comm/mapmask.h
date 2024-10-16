/*
**  MapMask.h
**  map mask functions header.
**
**  Jack, 2001.12.110.
**
**  Port to linux, and change it to C, and make it multi-threads-safe.
**  Steve 2003.12.11
*/

#ifndef _MAPMASK_H_
#define _MAPMASK_H_

#include "masktypes.h"

#ifndef WIN32
struct tagPOINT
{
	long x;
	long y;
};
#endif

typedef struct tagPOINT 	POINT;

enum
{
	PATH_STEPS  =   200,
	VIA_STEPS   =   80 * 60,
};

enum
{
	nMaskFileVersion	=	100,
};

enum MaskDirs 
{
	maskDirCenter       =   0,      // 中央
	maskDirUp           =   1,      // 上
	maskDirDown         =   2,      // 下
	maskDirLeft         =   3,      // 左
	maskDirRight        =   4,      // 右
	maskDirLeftUp       =   5,      // 左上
	maskDirLeftDown     =   6,      // 左下
	maskDirRightUp      =   7,      // 右上
	maskDirRightDown    =   8,      // 右下
};

enum MaskBits
{
	maskStop            =   0x01,   // 行走层
	maskWater           =   0x02,   // 水层
	maskMagic           =   0x04,  //技能火球飞行层
	maskFly			    =   0x08,   //角色飞行层
	maskLoMagic         =   0x10,   // 暂不使用
	maskDynamicNpc      =   0x20,
	maskMine                 = 0x40,
};

enum MaskShowFlags
{
	maskShowLoMagic     =   0x00000001, 
	maskShowMidMagic    =   0x00000002,
	maskShowHiMagic     =   0x00000004,
	maskShowDynamicNpc  =   0x00000020,
	maskShowFindPath    =   0x00000040,
	maskShowStop        =   0x00000080,
	maskShowGrid        =   0x10000000,
	maskShowAll         =   0xffffffff,
};

enum MaskPathResults
{
	maskPathArrive      =   0,  //已經達到
	maskPathFound       =   1,  //找到可以到達的路徑
	maskPathNear        =   2,  //靠近目標
	maskPathFail        =   3,  //尋徑失敗
	maskPathError       =   -1, //錯誤
};

enum MaskStepResults
{
	maskStepArrive      =   0,  //到達
	maskStepOnTheWay    =   1,   //在途中
	maskStepObstruct    =   2,   //被阻隔
	maskStepOutstretch  =   3,   //超出範圍
};

struct TMaskFileHeader 
{
	unsigned int id;             //file id 'MASK'
	unsigned int version;        //file version
	unsigned int head_size;      //size of the file head
};


struct TMapMask
{
	int		m_iSize;
    int   m_lWidthMasks;
    int   m_lHeightMasks;
    int   m_lMaskPixelWidth;
    int   m_lMaskPixelHeight;

	MASK_BIT_TYPE   m_pMaskData[1];
};

typedef struct TMapMask		TMAPMASK;

#ifdef __cplusplus
extern "C"
{
#endif

void	InitMapMask(TMAPMASK* pstMask, int iSize);
TMAPMASK* AllocMapMask(int iSize);
void	ReleaseMapMask(TMAPMASK* pstMask);
void	FreeMapMask(TMAPMASK* pstMask);
BOOL	SaveMaskFile(TMAPMASK* pstMask, char *szFileName);
BOOL	LoadMaskFile(TMAPMASK* pstMask, int* piSize, char *szFileName);
BOOL    IsMaskLineThrough(TMAPMASK* pstMask, int lMaskX1, int lMaskY1, int lMaskX2, int lMaskY2, MASK_BIT_TYPE StopMask);
BOOL    IsPixelLineThrough(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask);
BOOL    IsPixelLineThrough2(TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, MASK_BIT_TYPE StopMask);
void	InterceptRasterLine(TMAPMASK* pstMask, int lSx, int lSy, int *lEx, int *lEy, int lLen);

BOOL 	IsPixelStop(TMAPMASK* pstMask, int lPixelX, int lPixelY);
BOOL    IsPixelMagicStop(TMAPMASK* pstMask, int lPixelX, int lPixelY);

//判断是否为水层: pstMask为当前层数据, 
//lCurPixelX, lCurPixelY, lDir 为人物的当前位置和方向, lLen为鱼竿长度
BOOL    IsPixelWater(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY, int lDir, int lLen);
    
//判断是否为矿层: lCurPixelX, lCurPixelY 为人物的当前位置
BOOL    IsPixelMine(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY);

BOOL	WindingAttackAxisPosition(int n, int *x, int *y);
BOOL	WindingItemAxisPosition(int n, int *x, int *y);
	
/**
 * \param x,y  判断的点
 *\param ox,oy 圆的中心点或者矩形的原点
 * \param face_angle  扇形面向方向[0, 360)
 *\param angle_range 扇形角度[0, 360]
 *\param xlen,ylen  分别表示矩形在face_angle方向上的长度和横向宽度
 */
BOOL  IsPointInCircle( int x, int y, int ox, int oy, int radius );
BOOL  IsPointInFan( int x, int y, int ox, int oy, int radius, int face_angle, int angle_range );
BOOL  IsPointInRectangle( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle );

int FaceAngleBetween( int from_x, int from_y, int to_x, int to_y );

/** 设置指定地图,指定的矩形区域的阻挡信息
@param pstMask 当前地图的信息
@param x,y,w,h 需要设置的矩形区域(以像素为单位)
@param uchOrMask 可用于添加的阻挡类型
@param uchAndMask 可用于删除的阻挡类型
*/
int SetMapMask(TMAPMASK* pstMask, int x, int y, int w, int h, MASK_BIT_TYPE uchOrMask, MASK_BIT_TYPE uchAndMask);

#ifdef __cplusplus
}
#endif

#endif//_MAPMASK_H_
