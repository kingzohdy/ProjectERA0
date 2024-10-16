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
	maskDirCenter       =   0,      // ����
	maskDirUp           =   1,      // ��
	maskDirDown         =   2,      // ��
	maskDirLeft         =   3,      // ��
	maskDirRight        =   4,      // ��
	maskDirLeftUp       =   5,      // ����
	maskDirLeftDown     =   6,      // ����
	maskDirRightUp      =   7,      // ����
	maskDirRightDown    =   8,      // ����
};

enum MaskBits
{
	maskStop            =   0x01,   // ���߲�
	maskWater           =   0x02,   // ˮ��
	maskMagic           =   0x04,  //���ܻ�����в�
	maskFly			    =   0x08,   //��ɫ���в�
	maskLoMagic         =   0x10,   // �ݲ�ʹ��
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
	maskPathArrive      =   0,  //�ѽ��_��
	maskPathFound       =   1,  //�ҵ����Ե��_��·��
	maskPathNear        =   2,  //����Ŀ��
	maskPathFail        =   3,  //����ʧ��
	maskPathError       =   -1, //�e�`
};

enum MaskStepResults
{
	maskStepArrive      =   0,  //���_
	maskStepOnTheWay    =   1,   //��;��
	maskStepObstruct    =   2,   //�����
	maskStepOutstretch  =   3,   //��������
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

//�ж��Ƿ�Ϊˮ��: pstMaskΪ��ǰ������, 
//lCurPixelX, lCurPixelY, lDir Ϊ����ĵ�ǰλ�úͷ���, lLenΪ��ͳ���
BOOL    IsPixelWater(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY, int lDir, int lLen);
    
//�ж��Ƿ�Ϊ���: lCurPixelX, lCurPixelY Ϊ����ĵ�ǰλ��
BOOL    IsPixelMine(TMAPMASK* pstMask, int lCurPixelX, int lCurPixelY);

BOOL	WindingAttackAxisPosition(int n, int *x, int *y);
BOOL	WindingItemAxisPosition(int n, int *x, int *y);
	
/**
 * \param x,y  �жϵĵ�
 *\param ox,oy Բ�����ĵ���߾��ε�ԭ��
 * \param face_angle  ����������[0, 360)
 *\param angle_range ���νǶ�[0, 360]
 *\param xlen,ylen  �ֱ��ʾ������face_angle�����ϵĳ��Ⱥͺ�����
 */
BOOL  IsPointInCircle( int x, int y, int ox, int oy, int radius );
BOOL  IsPointInFan( int x, int y, int ox, int oy, int radius, int face_angle, int angle_range );
BOOL  IsPointInRectangle( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle );

int FaceAngleBetween( int from_x, int from_y, int to_x, int to_y );

/** ����ָ����ͼ,ָ���ľ���������赲��Ϣ
@param pstMask ��ǰ��ͼ����Ϣ
@param x,y,w,h ��Ҫ���õľ�������(������Ϊ��λ)
@param uchOrMask ��������ӵ��赲����
@param uchAndMask ������ɾ�����赲����
*/
int SetMapMask(TMAPMASK* pstMask, int x, int y, int w, int h, MASK_BIT_TYPE uchOrMask, MASK_BIT_TYPE uchAndMask);

#ifdef __cplusplus
}
#endif

#endif//_MAPMASK_H_
