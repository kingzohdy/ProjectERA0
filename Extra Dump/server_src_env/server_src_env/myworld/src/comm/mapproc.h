/*
 *	MapProc.h
 *	map proc functions.
 *
 *	Jim, 2003.12.2.
 *
 *	Port to linux, and change it to C.
 *	Steve 2003.12.11
 */
#ifndef _MAPPROC_H_
#define _MAPPROC_H_
#include "poly.h"

enum
{
	nMaxAreaProcs	=	32,
};

struct TAreaProcInfo	//�����¼�����
{
	unsigned int	total;
	unsigned int	index[nMaxAreaProcs];
};

typedef struct TAreaProcInfo	TAREAPROCINFO;

//////////////////////////////////////////////////////////////////////////
// struct TMapProcs

struct TMapProc
{
	unsigned int	m_nID;		//�¼�ID
	unsigned int	m_nType;	//�¼�����
	POLY	m_Poly;		//�¼�����
};

typedef struct TMapProc		TMAPPROC;

enum
{
	nMaxProcs		=	256,
	nProcFileVersion=	0x00010000,
	nFileVersion2	=	0x00010001,	//2004.1.7, 2nd version
	nFileVersionLast = nFileVersion2,

	nMinProcID		=	1,
	nMaxProcID		=	65535,
};
enum ProcTypes
{
	typeDummyProc	=	0,
	typeAutoProc	=	1,
	typeItemProc	=	2,
	typeKeyProc		=	3,
};
enum ShowFlags
{
	showAll			=	0xffffffff,
};

struct TProcFileHeader 
{
	unsigned int   copyright;      //copyright 'QQF'+0x1a
	unsigned int   id;             //file id 'PROC'
	unsigned int   version;        //file version
	unsigned int   head_size;      //size of the file head
};


struct TMapProcs
{
	unsigned int	m_nTotalProcs;
	unsigned int	m_nMapWidth;	//��ͼ���
	unsigned int	m_nMapHeight;	//��ͼ�߶�
	unsigned int	m_nAreaWidth;	//������
	unsigned int	m_nAreaHeight;	//����߶�
	unsigned int	m_nHorzAreas;	//ˮƽ����������Ŀ
	unsigned int	m_nVertAreas;	//��ֱ����������Ŀ
	TMAPPROC	m_MapProc[nMaxProcs];
};

typedef struct TMapProcs	TMAPPROCS;

#ifdef __cplusplus
extern "C"
{
#endif

void	InitProc(TMAPPROC* pstProc);
BOOL	IsProcAvailable(TMAPPROC* pstProc);

void	InitMapProcs(TMAPPROCS* pstProcs);
TMAPPROCS* AllocMapProcs(void);
void	ReleaseMapProcs(TMAPPROCS* pstProcs);
void	FreeMapProcs(TMAPPROCS* pstProcs);

BOOL	SaveProcFile(TMAPPROCS* pstProcs, char *szFileName);
BOOL	LoadProcFile(TMAPPROCS* pstProcs, int* piMapSize, TAREAPROCINFO* pstArea, int* piAreaSize, char *szFileName);

int	MatchProc(TMAPPROCS* pstProcs, int x, int y, unsigned int *pnProcID, unsigned int *pnProcType);
int	MatchProcByID(TMAPPROCS* pstProcs, unsigned int* paiIDs, int iIDs, int x, int y, unsigned int *pnProcID, unsigned int *pnProcType);
int	FindProcOfCoverPoint(TMAPPROCS* pstProcs, int x, int y);

#ifdef __cplusplus
}
#endif


#endif//_MAPPROC_H_
