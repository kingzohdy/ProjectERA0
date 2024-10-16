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

#define ZONE_OFFLINE_EXP_SEC 600 //ZONE 离线经验开始计算秒数
#define MUD_OFFLINE_EXP_SEC 600 //mud 离线经验开始计算秒数
#define SYNC_TIME_SEC 60 //zone mud 之间允许的误差秒数

typedef struct  tagSSHandle		SSHANDLE;
typedef struct  tagSSHandle		*LPSSHANDLE;

typedef struct tagListEntry
{
	int iIdx; // 在mempool中的索引
	int iNext; // 下一节点/头节点
	int iPrev; // 上一节点/尾节点
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
 * 内部数据, 千万不要试图修改以下结构体的成员.
 */
struct grav_slab_page
{
	uintptr_t slab;
	uintptr_t next;
	uintptr_t prev;
};

/* 分配器的内存布局:
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
	uint32_t magic;               // 幻数
	
	size_t min_size;              // 能分配的最小内存
	size_t min_shift;             // log2(min_size)
	size_t slab_exact_size;       // 中等大小内存
	size_t slab_exact_shift;      // log2(slab_exact_size)
	size_t page_size;             // 一页的大小
	size_t page_shift;            // log2(page_size)
	size_t slab_max_size;         // 大内存, 超过此大小一律分配一页
	
	struct grav_slab_page *pages; // 内存页
	struct grav_slab_page free;   // 空闲页
	
	char *start;                  // 指向实际被分配的空闲内存头
	char *end;                    // 指向实际被分配的空闲内存尾
};

/* 从raw指向的,n大小的内存块初始化分配器.
 * raw不能为NULL, raw的值必要对其到4, 否则分配器无法工作.
 * n 的大小必须大于等于(1 << GRAV_PAGE_SIZE_SHIFT).
 *
 * 初始化成功返回分配器句柄, 否则返回NULL.
 *
 * GRAV_SLAB_POOL_SIZEOF宏可帮助计算正确的内存块大小.
 * 如果想使用1000字节的内存块用于分配,可用以下代码实现:
 * char buf[GRAV_SLAB_POOL_SIZEOF(1000)];
 * struct grav_slab_pool* pool = grav_slab_pool_init(buf, sizeof(buf));
 */
struct grav_slab_pool* grav_slab_pool_init(char *raw, size_t n);


/* 从已经初始化过的内存块获取分配器句柄.
 * raw, n的要求同grav_slab_pool_init, 但raw指向的内存块必须是已经被
 * grav_slab_pool_init 初始化过.
 *
 * 成功返回分配器句柄, 否则返回NULL.
 *
 * 此函数将初略的检查内存块是否有效.
 */
struct grav_slab_pool* grav_slab_pool_attach(char *raw, size_t n);


/* 从分配器中分配指定大小的内存.
 * pool是grav_slab_pool_init函数返回的分配器句柄.
 * n表示需要分配的内存大小.
 *
 * 成功返回指向已分配的内存块的指针, 否则返回NULL.
 *
 * 失败的原因是内存已耗尽. 分配好的内存都被初始化为0xdeaddead.
 */
void *grav_slab_alloc_locked(struct grav_slab_pool *pool, size_t size);


/* 从分配器中释放已分配的内存.
 * pool是grav_slab_pool_init函数返回的分配器句柄.
 * p从grav_slab_alloc_locked返回的指针.
 *
 * 没有返回值.
 */
void grav_slab_free_locked(struct grav_slab_pool *pool, void *p);


/* 同grav_slab_alloc_locked, 不过返回的是内存句柄
 * 成功返回非0, 否则返回0
 */
static inline
uintptr_t grav_slab_alloc(struct grav_slab_pool *pool, size_t size)
{
	char *chunk = grav_slab_alloc_locked(pool, size);
	return !chunk ? 0U : (uintptr_t)(chunk - (char *)pool);
}


/* 获取内存句柄所表示的内存块的指针.
 * pool是grav_slab_pool_init函数返回的分配器句柄.
 * local是从grav_slab_alloc返回的内存句柄.
 *
 * 成功返回指向内存块的指针, 否则返回NULL.
 *
 * 本函数会简单的检查local的有效性.
 */
static inline 
void* grav_slab_grab(struct grav_slab_pool *pool, uintptr_t local)
{
	char *chunk = (char *)pool + local;
	return (chunk >= pool->start && chunk < pool->end) ? chunk : NULL;
}


/*
 * 同grav_slab_free_locked, 不过第二个参数是grav_slab_alloc返回的内存句柄
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

// 一页内存的log2对数, 一页内存的大小就是(1 << GRAV_PAGE_SIZE_SHIFT)
#define GRAV_PAGE_SIZE_SHIFT 12

// 计算分配器需要的内存块大小, n为你想实际使用的内存大小
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

/* 基本几何结构 */
// 直线的解析方程 a*x+b*y+c=0 为统一表示，约定 a>= 0
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


/* 判断点p是否在线段l上
条件：(p在线段l所在的直线上)&& (点p在以线段l为对角线的矩形内) */
int online(LINESEG l, RESPOS p);

// 根据已知两点坐标，求过这两点的直线解析方程： a*x+b*y+c = 0 (a >= 0)
LINE makeline(DPOINT p1, DPOINT p2);

// 如果两条直线 l1(a1*x+b1*y+c1 = 0), l2(a2*x+b2*y+c2 = 0)相交，返回true，且返回交点p
// 是 L1,L2
int lineintersect(LINE l1, LINE l2, RESPOS *p);

// 如果线段l1和l2相交，返回true且交点由(inter)返回，否则返回false
int intersection(LINESEG l1,LINESEG l2,RESPOS *inter);
int IsTransPkgCmd(int iCmd);//是否zone和mud可以转发的cmd,返回true,false

//点p3到线段p1,p2的最短距离
double ptolinesegdist_ex(RESPOS p1,RESPOS p2,RESPOS p3);

int region_world_func_inst_aton_busid(int iRegion, int iWorldID, int iFuncID, int iInst);

int str_to_passwd_card(char* pszStr,int iXLen,int iYLen,PASSWDCARD *pstCard);


#endif /* COMM_MISC_H */
