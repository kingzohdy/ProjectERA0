/*
**  @file $RCSfile: tmempool.h,v $
**  general description of this module
**  $Id: tmempool.h,v 1.3 2012/07/27 05:08:56 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:08:56 $
**  @version $Revision: 1.3 $
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
	int iIdx; //此memblock在内存池中的索引
	int iNext; // 空闲链表下一个指针
	int iCount; //暂时没有使用 
	char szData[1]; //应用数据起始位置
};

typedef struct tagTMemBlock		TMEMBLOCK;
typedef struct tagTMemBlock		*LPTMEMBLOCK;

#define TMEMBLOCK_IS_VALID(p)	((p)->fValid)
#define TMEMBLOCK_GET_DATA(p)	((p)->szData)

struct tagTMemPool
{
	int iMagic; //用于检查mempool内存
	int iBuild; //用于检查mempool内存
	int iMax;//mempool的容量
	int iUnit;//应用数据的大小

	int iUsed;//有多少memblock已经被使用
	int iRealUnit;//在mempool中考虑圆整后数据占用内存的大小
	int iSize;//mempool占用内存的大小
	int iRealSize;//与iSize相同, 目前没有使用

	int iLastIdx;//最后分配的memblock索引
	int iInited;//mempool是否经过了初始化
	int iIsCalloc;//mempool是否分配过内存
	int iMethod;//TMEMPOOL_FIFO  memblock分配采用先进先出,是默认的方法; TMEMPOOL_LIFO 

	int iFreeHead; //空闲链表头指针
	int iFreeTail;//空闲链表尾指针
	int iStart;//内部使用，与iEnd配合检验tmempool_alloc和tmempool_free操作的原子性
	int iEnd;

	char szRes[64];//预留

	char szBlocks[1];//数据区起始位置
};

typedef struct tagTMemPool		TMEMPOOL;
typedef struct tagTMemPool		*LPTMEMPOOL;


#define TMEMPOOL_GET_PTR(pool, idx)	(TMEMBLOCK*)((pool)->szBlocks + (pool)->iRealUnit*((idx) % (pool)->iMax) )
#define TMEMPOOL_GET_PTR_BYPOS(pool, idx)   (TMEMBLOCK*)((pool)->szBlocks + (pool)->iRealUnit*(idx))
#define TMEMPOOL_GET_CAP(pool)		((pool)->iMax)

#define TMEMPOOL_GET_BLOCK_IDX(blk)	((blk)->iIdx)

#define TMEMPOOL_MIN_SIZE()			( offsetof(TMEMPOOL, szBlocks) )

#define TMEMPOOL_ROUND(unit, round)	( ( ( (unit) + (round) - 1 ) / (round) ) * (round) )

#define TMEMPOOL_ROUND_UNIT(unit)	TMEMPOOL_ROUND(unit, 256)

#define TMEMPOOL_ROUND_POOL(unit)	TMEMPOOL_ROUND(unit, 1024)

#define TMEMPOOL_REAL_UNIT(unit)	TMEMPOOL_ROUND_UNIT( ( (unit) + offsetof(TMEMBLOCK, szData) ) )

#define TMEMPOOL_CALC(max, unit)	( offsetof(TMEMPOOL, szBlocks) + (max)*TMEMPOOL_REAL_UNIT(unit) )

/**初始化mempool内存，形成空闲链表
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@param[in]      pvBase    用于形成mempool的内存的指针
*@param[in]      iSize     pvBase 内存的大小
		
*@retval  0  成功
         -1  失败
*/
int tmempool_init(TMEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, int iSize);

/**分配mempool内存并初始化
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
		
*@retval  0  成功
         -1  失败

*@note   与tmempool_destroy配对使用         
*/
int tmempool_new(TMEMPOOL** ppstPool, int iMax, int iUnit);

/**attach 一块已存在的mempool内存，并检查内存格式
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@param[in]      pvBase    用于形成mempool的内存的指针
*@param[in]      iSize     pvBase 内存的大小
		
*@retval  0  成功
         -1  失败
*/
int tmempool_attach(TMEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, int iSize);

/**销毁由tmempool_new创建的内存池*/
int tmempool_destroy(TMEMPOOL** ppstPool);

/**设置mempool的memblock分配方式
*@param[in]	     pstPool   mempool指针
*@param[in]      iMethod   TMEMPOOL_FIFO  先进先出,默认方式; TMEMPOOL_LIFO 后进先出
		
*@retval  0  成功
         -1  失败
*/
int tmempool_set_method(TMEMPOOL* pstPool, int iMethod);

/**根据索引获取应用数据
*@param[in]	     pstPool   mempool指针
*@param[in]      iIdx      memblock的索引
		
*@retval  not NULL  成功
          NULL      失败
*/
void* tmempool_get(TMEMPOOL* pstPool, int iIdx);

/**根据索引获取应用数据
*@param[in]	     pstPool   mempool指针
*@param[in]      iPos      memblock 数组索引0~max-1
		
*@retval  not NULL  成功
          NULL      失败
*/
void* tmempool_get_bypos(TMEMPOOL* pstPool, int iPos);


/**分配一块memblock
*@param[in]	     pstPool   mempool指针
		
*@retval  >=0     成功,   返回memblock的索引
          -1      失败
*/
int tmempool_alloc(TMEMPOOL* pstPool);

/**释放一块索引为iIdx的memblock */
int tmempool_free(TMEMPOOL* pstPool, int iIdx);

/**重新生成空闲链表，一般在iStart!=iEnd时使用*/
int tmempool_fix(TMEMPOOL* pstPool);

/**从索引0开始找到第一个使用中的memblock
*@param[in]	     pstPool   mempool指针
*@param[out]     piPos     存储memblock index
		
*@retval   0      成功
          -1      失败
*/
int tmempool_find_used_first(TMEMPOOL* pstPool, int* piPos);

/**从输入的一个索引的下一个索引开始找到一个使用中的memblock
*@param[in]	     pstPool   mempool指针
*@param[in,out]  piPos     
                       in  输入的索引
                      out  >=0  找到的索引
                           -1   已经没有下一个使用中的memblock了
                      
*@retval  >=0     成功
          -1      失败
*/
int tmempool_find_used_next(TMEMPOOL* pstPool, int* piPos);

#ifdef __cplusplus
}
#endif

#endif /* TMEMPOOL_H */
