#ifndef COMM_MISC_H
#define COMM_MISC_H

#include "pal/pal.h" 
#include "tdr/tdr.h"
#include "pal/pal.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "op_log_meta.h"
#include "proto_ss.h"
#include "resdb_meta.h"
#include "comm/tmempool.h"
#include <stdint.h>
#include <stddef.h>

#define dlist_head( pool, entry ) dlist_next( pool, entry )

#define dlist_tail( pool, entry ) dlist_prev( pool, entry )

#define dlist_foreach( pool, pos, list ) \
	for( pos = dlist_head( pool, list ); pos != list; pos = dlist_next( pool, pos ) )

#define dlist_foreach_range( pool, pos, begin, end ) \
	for( pos = begin; pos != end; pos = dlist_next( pool, pos ) )

#define dlist_foreach_reverse( pool, pos, list ) \
	for( pos = dlist_tail( pool, list ); pos != list; pos = dlist_prev( pool, pos ) )

#define dlist_foreach_range_reverse( pool, pos, begin, end ) \
	for( pos = begin; pos != end; pos = dlist_prev( pool, pos ) )

struct tagSSHandle
{
	LPTDRMETALIB pstSSProtoLib;
	LPTDRMETA pstSSProtoPkgMeta;
};

#define ZONE_OFFLINE_EXP_SEC 600 //ZONE ���߾��鿪ʼ��������
#define MUD_OFFLINE_EXP_SEC 600 //mud ���߾��鿪ʼ��������
#define SYNC_TIME_SEC 60 //zone mud ֮��������������

typedef struct  tagSSHandle		SSHANDLE;
typedef struct  tagSSHandle		*LPSSHANDLE;

typedef struct tagListEntry
{
	int iIdx; // ��mempool�е�����
	int iNext; // ��һ�ڵ�/ͷ�ڵ�
	int iPrev; // ��һ�ڵ�/β�ڵ�
}ListEntry;

static inline void dlist_init( TMEMPOOL * pstPool, int iEntry )
{
	ListEntry * pstEntry = (ListEntry *)tmempool_get( pstPool, iEntry );

	if (NULL == pstEntry)  return;

	pstEntry->iIdx = iEntry;
	pstEntry->iPrev = iEntry;
	pstEntry->iNext = iEntry;
}

static inline int dlist_create( TMEMPOOL * pstPool )
{
	int iList = tmempool_alloc( pstPool );

	if( iList < 0 )
	{
		return -1;
	}
	
	dlist_init( pstPool, iList );
	return iList;
}

static inline void dlist_push_front( TMEMPOOL * pstPool, int iList, int iEntry )
{
	ListEntry * pstList;
	ListEntry * pstEntry;
	ListEntry * pstNext;
	
	 pstEntry = (ListEntry *)tmempool_get( pstPool, iEntry ); 
	if (!pstEntry)
	{
		return;
	}
	
	 pstList = (ListEntry *)tmempool_get( pstPool, iList );
	if (!pstList)
	{
		return;
	}
	
	 pstNext = (ListEntry *)tmempool_get( pstPool, pstList->iNext );
	if (!pstNext)
	{
		return;
	}

	pstEntry->iNext = pstList->iNext;
	pstNext->iPrev = iEntry;
	pstEntry->iPrev = iList;
	pstList->iNext = iEntry;
}

static inline void dlist_push_back( TMEMPOOL * pstPool, int iList, int iEntry )
{
	ListEntry * pstEntry;
	ListEntry * pstList;
	ListEntry * pstPrev ;
	
	
	 pstEntry = (ListEntry *)tmempool_get( pstPool, iEntry ); 
	if (!pstEntry)
	{
		return;
	}
	
	 pstList = (ListEntry *)tmempool_get( pstPool, iList );
	if (!pstList)
	{
		return;
	}
	
	 pstPrev = (ListEntry *)tmempool_get( pstPool, pstList->iPrev );
	if (!pstPrev)
	{
		return;
	}

	pstEntry->iPrev = pstList->iPrev;
	pstPrev->iNext = iEntry;
	pstEntry->iNext = iList;
	pstList->iPrev = iEntry;
}

static inline void dlist_remove( TMEMPOOL * pstPool, int iEntry )
{
	ListEntry * pstEntry;
	ListEntry * pstNext;
	ListEntry * pstPrev;
	
	 pstEntry = (ListEntry *)tmempool_get( pstPool, iEntry ); 
	if (!pstEntry)
	{
		return;
	}
	
	pstNext = (ListEntry *)tmempool_get( pstPool, pstEntry->iNext );
	if (!pstNext)
	{
		return;
	}
	
	pstPrev = (ListEntry *)tmempool_get( pstPool, pstEntry->iPrev );
	if (!pstPrev)
	{
		return;
	}

	pstNext->iPrev = pstEntry->iPrev;
	pstPrev->iNext = pstEntry->iNext;
	pstEntry->iPrev = iEntry;
	pstEntry->iNext = iEntry;
}

static inline int dlist_next( TMEMPOOL * pstPool, int iEntry )
{
	ListEntry * pstEntry = (ListEntry *)tmempool_get( pstPool, iEntry );
	return !pstEntry ? -1 : pstEntry->iNext;
}

static inline int dlist_prev(TMEMPOOL *pstPool, int iEntry)
{
	ListEntry * pstEntry = (ListEntry *)tmempool_get( pstPool, iEntry );
	return !pstEntry ? -1 : pstEntry->iPrev;
}

int set_rlimit(unsigned int uiMaxConn);

int ss_han_create(unsigned char *pszLib, LPSSHANDLE *ppSSHandle);
int ss_han_destroy(LPSSHANDLE *ppSSHandle);
int ss_unpack(SSHANDLE  *pstSSHandler, char *pszBuf, int iBuf, SSPKG *pstPkg, int iVersion);
int ss_pack(SSHANDLE  *pstSSHandler, TDRDATA *pstNet, SSPKG *pstPkg, int iVersion);
int cmp_modify_time(char *sFile, time_t *plMTime);
int bsearch_int (const void *key, const void *base, int nmemb, int size, int *piEqual);
int binsert_int (const void *key, const void *base, int *pnmemb, int size, int iUnique);
int bdelete_int (const void *key, const void *base, int *pnmemb, int size);
int my_bsearch (const void *key, const void *base, size_t nmemb, size_t size, int *piEqual, int (*compar) (const void *, const void *));
int my_bdelete(const void *key, const void *base, size_t *pnmemb, size_t size, int (*compar) (const void *, const void *));
int my_binsert(const void *key, const void *base, size_t *pnmemb, size_t size, int iUnique, int (*compar) (const void *, const void *));

char* cistrstr(char *s1, char *s2);
int cistrcmp(char *s1, char *s2);

int res_mem_to_db(TDRDBHANDLE hDBHandle, LPTDRMETA pstMeta, char* pszData, int iUnit);
int res_mem_to_xml(LPTDRMETA pstMeta, char *pszOutFile, char *pData, int iUnit);

int load_resfile_one(char *pszPath, char *pszMetaName, char *pAddr, int *piCount, 
							time_t *ptTime, int iUnitSize, int iUnitMax, int *piReload);

int load_resdb_one(LPTDRMETALIB pstResLib, TDRDBHANDLE hResDBHandle, char *pszMetaName, 
						char *pAddr, int *piCount, int iUintSize, int iUnitMax);

//int load_resdb(LPTDRMETALIB pstResLib, TDRDBHANDLE hResDBHandle,LPTDRMETA pstMeta,char *pszMetaName, 
//						char *pAddr, int iCount, int iMetaSize);
int check_fittings(char *pAddr, int iCount,int iUint);
int check_diathesis(char *pAddr, int iCount,int iUint);
int check_skilldef(char *pAddr, int iCount,int iUint);
int check_patrol(char *pAddr, int iCount, int iUint);

int check_item(char *pAddrItem, int piCountItem, int iUnitItem, char *pAddrSuit, int piCountSuit, int iUnitSuit);
SUITSET *find_suitset(char *pAddr, int iCount,int iUnit, int iSuitSetID);

int itemid_num(ITEMDEF *pstItemDef);
int pre_item(char *pAddr, int iCount, int iUnit);
int IsPointInProcPoly(PROCPOLY *poly, int x, int y);
int IsSameDay(time_t t1, time_t t2);
int IsSameWeek(time_t t1, time_t t2);

int z_check_petsubracedef(char *pAddrPetSubRace, int iCountPetSubRace, int iPetSubRaceUnit,
							char *pAddrPetDef, int iCountPetDef, int iPetDefUnit);

int z_check_pinzhidef(char *pAddrPinZhiDef, int iCount, int iUnit);

/* e.g  20100810120000 */
char * time2string(time_t tTime);
time_t  string2time(const char *szTime);
unsigned long long atoull(const char *p);
char *str2hex(const void *pBuff, int iSize);


////////////////////////////////////////////////////////////////////////////////
// grav_slab.h

//#ifndef GRAB_SLAB_H
//#define GRAB_SLAB_H

//#include <stdint.h>
//#include <stddef.h>

/*
 * �ڲ�����, ǧ��Ҫ��ͼ�޸����½ṹ��ĳ�Ա.
 */
struct grav_slab_page
{
	uintptr_t slab;
	uintptr_t next;
	uintptr_t prev;
};

/* ���������ڴ沼��:
 * +------+
 * |      |
 * |      |
 * | head | grav_slab_pool
 * |      |
 * |      |
 * +------+
 * |   8b | slots[0] (grav_slab_page)
 * +------+
 * |  16b | slots[1]
 * +------+
 * |  32b | slots[2]
 * +------+
 * |  64b | slots[3]
 * +------+
 *   ....
 * +------+ grav_slab_pool.free.next
 * | page |
 * +------+
 * | page |
 * +------+
 *   ...
 * +------+ grav_slab_pool.start
 * | block|
 *   ...
 * | block|
 * +------+ grav_slab_pool.end
 */
struct grav_slab_pool
{
	uint32_t magic;               // ����
	
	size_t min_size;              // �ܷ������С�ڴ�
	size_t min_shift;             // log2(min_size)
	size_t slab_exact_size;       // �еȴ�С�ڴ�
	size_t slab_exact_shift;      // log2(slab_exact_size)
	size_t page_size;             // һҳ�Ĵ�С
	size_t page_shift;            // log2(page_size)
	size_t slab_max_size;         // ���ڴ�, �����˴�Сһ�ɷ���һҳ
	
	struct grav_slab_page *pages; // �ڴ�ҳ
	struct grav_slab_page free;   // ����ҳ
	
	char *start;                  // ָ��ʵ�ʱ�����Ŀ����ڴ�ͷ
	char *end;                    // ָ��ʵ�ʱ�����Ŀ����ڴ�β
};

/* ��rawָ���,n��С���ڴ���ʼ��������.
 * raw����ΪNULL, raw��ֵ��Ҫ���䵽4, ����������޷�����.
 * n �Ĵ�С������ڵ���(1 << GRAV_PAGE_SIZE_SHIFT).
 *
 * ��ʼ���ɹ����ط��������, ���򷵻�NULL.
 *
 * GRAV_SLAB_POOL_SIZEOF��ɰ���������ȷ���ڴ���С.
 * �����ʹ��1000�ֽڵ��ڴ�����ڷ���,�������´���ʵ��:
 * char buf[GRAV_SLAB_POOL_SIZEOF(1000)];
 * struct grav_slab_pool* pool = grav_slab_pool_init(buf, sizeof(buf));
 */
struct grav_slab_pool* grav_slab_pool_init(char *raw, size_t n);


/* ���Ѿ���ʼ�������ڴ���ȡ���������.
 * raw, n��Ҫ��ͬgrav_slab_pool_init, ��rawָ����ڴ��������Ѿ���
 * grav_slab_pool_init ��ʼ����.
 *
 * �ɹ����ط��������, ���򷵻�NULL.
 *
 * �˺��������Եļ���ڴ���Ƿ���Ч.
 */
struct grav_slab_pool* grav_slab_pool_attach(char *raw, size_t n);


/* �ӷ������з���ָ����С���ڴ�.
 * pool��grav_slab_pool_init�������صķ��������.
 * n��ʾ��Ҫ������ڴ��С.
 *
 * �ɹ�����ָ���ѷ�����ڴ���ָ��, ���򷵻�NULL.
 *
 * ʧ�ܵ�ԭ�����ڴ��Ѻľ�. ����õ��ڴ涼����ʼ��Ϊ0xdeaddead.
 */
void *grav_slab_alloc_locked(struct grav_slab_pool *pool, size_t size);


/* �ӷ��������ͷ��ѷ�����ڴ�.
 * pool��grav_slab_pool_init�������صķ��������.
 * p��grav_slab_alloc_locked���ص�ָ��.
 *
 * û�з���ֵ.
 */
void grav_slab_free_locked(struct grav_slab_pool *pool, void *p);


/* ͬgrav_slab_alloc_locked, �������ص����ڴ���
 * �ɹ����ط�0, ���򷵻�0
 */
static inline
uintptr_t grav_slab_alloc(struct grav_slab_pool *pool, size_t size)
{
	char *chunk = grav_slab_alloc_locked(pool, size);
	return !chunk ? 0U : (uintptr_t)(chunk - (char *)pool);
}


/* ��ȡ�ڴ�������ʾ���ڴ���ָ��.
 * pool��grav_slab_pool_init�������صķ��������.
 * local�Ǵ�grav_slab_alloc���ص��ڴ���.
 *
 * �ɹ�����ָ���ڴ���ָ��, ���򷵻�NULL.
 *
 * ��������򵥵ļ��local����Ч��.
 */
static inline 
void* grav_slab_grab(struct grav_slab_pool *pool, uintptr_t local)
{
	char *chunk = (char *)pool + local;
	return (chunk >= pool->start && chunk < pool->end) ? chunk : NULL;
}


/*
 * ͬgrav_slab_free_locked, �����ڶ���������grav_slab_alloc���ص��ڴ���
 */
static inline
void grav_slab_free(struct grav_slab_pool *pool, uintptr_t local)
{
	char *chunk = grav_slab_grab(pool, local);
	if(chunk)
		grav_slab_free_locked(pool, chunk);
}



#define grav_align(d, a) (((d) + ((a) - 1)) & ~((a) - 1))

#define grav_align_ptr(p, a) \
	(char *) (((uintptr_t) (p) + \
	((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define RAND1(range)  ((int)((double)(range)*rand()/(RAND_MAX+1.0)))	

// һҳ�ڴ��log2����, һҳ�ڴ�Ĵ�С����(1 << GRAV_PAGE_SIZE_SHIFT)
#define GRAV_PAGE_SIZE_SHIFT 12

// �����������Ҫ���ڴ���С, nΪ����ʵ��ʹ�õ��ڴ��С
#define GRAV_SLAB_POOL_SIZEOF(n) \
	grav_align(sizeof(struct grav_slab_pool) + n, (1 << GRAV_PAGE_SIZE_SHIFT))

#define MEMSET(m)\
	          (memset(&m,0,sizeof(m)))
//#endif // GRAB_SLAB_H

typedef struct tagDPoint
{
    double X;                              
    double Y;                              
}DPOINT;

/* �������νṹ */
// ֱ�ߵĽ������� a*x+b*y+c=0 Ϊͳһ��ʾ��Լ�� a>= 0
typedef struct  tagLINE
{
	double a;
	double b;
	double c;
}LINE;
typedef struct LINESEGS
{
	DPOINT s;
	DPOINT e;
}LINESEG;

double multiply(DPOINT sp,DPOINT ep,DPOINT op);

int InsideConvexPolygon(int vcount, RESPOS polygon[], RESPOS q);


/* �жϵ�p�Ƿ����߶�l��
������(p���߶�l���ڵ�ֱ����)&& (��p�����߶�lΪ�Խ��ߵľ�����) */
int online(LINESEG l, RESPOS p);

// ������֪�������꣬����������ֱ�߽������̣� a*x+b*y+c = 0 (a >= 0)
LINE makeline(DPOINT p1, DPOINT p2);

// �������ֱ�� l1(a1*x+b1*y+c1 = 0), l2(a2*x+b2*y+c2 = 0)�ཻ������true���ҷ��ؽ���p
// �� L1,L2
int lineintersect(LINE l1, LINE l2, RESPOS *p);

// ����߶�l1��l2�ཻ������true�ҽ�����(inter)���أ����򷵻�false
int intersection(LINESEG l1,LINESEG l2,RESPOS *inter);
int IsTransPkgCmd(int iCmd);//�Ƿ�zone��mud����ת����cmd,����true,false

//��p3���߶�p1,p2����̾���
double ptolinesegdist_ex(RESPOS p1,RESPOS p2,RESPOS p3);

int region_world_func_inst_aton_busid(int iRegion, int iWorldID, int iFuncID, int iInst);

int str_to_passwd_card(char* pszStr,int iXLen,int iYLen,PASSWDCARD *pstCard);


#endif /* COMM_MISC_H */
