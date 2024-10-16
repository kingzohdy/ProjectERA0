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
	int iIdx; //��memblock���ڴ���е�����
	int iNext; // ����������һ��ָ��
	int iCount; //��ʱû��ʹ�� 
	char szData[1]; //Ӧ��������ʼλ��
};

typedef struct tagTMemBlock		TMEMBLOCK;
typedef struct tagTMemBlock		*LPTMEMBLOCK;

#define TMEMBLOCK_IS_VALID(p)	((p)->fValid)
#define TMEMBLOCK_GET_DATA(p)	((p)->szData)

struct tagTMemPool
{
	int iMagic; //���ڼ��mempool�ڴ�
	int iBuild; //���ڼ��mempool�ڴ�
	int iMax;//mempool������
	int iUnit;//Ӧ�����ݵĴ�С

	int iUsed;//�ж���memblock�Ѿ���ʹ��
	int iRealUnit;//��mempool�п���Բ��������ռ���ڴ�Ĵ�С
	int iSize;//mempoolռ���ڴ�Ĵ�С
	int iRealSize;//��iSize��ͬ, Ŀǰû��ʹ��

	int iLastIdx;//�������memblock����
	int iInited;//mempool�Ƿ񾭹��˳�ʼ��
	int iIsCalloc;//mempool�Ƿ������ڴ�
	int iMethod;//TMEMPOOL_FIFO  memblock��������Ƚ��ȳ�,��Ĭ�ϵķ���; TMEMPOOL_LIFO 

	int iFreeHead; //��������ͷָ��
	int iFreeTail;//��������βָ��
	int iStart;//�ڲ�ʹ�ã���iEnd��ϼ���tmempool_alloc��tmempool_free������ԭ����
	int iEnd;

	char szRes[64];//Ԥ��

	char szBlocks[1];//��������ʼλ��
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

/**��ʼ��mempool�ڴ棬�γɿ�������
*@param[in,out]	 ppstPool  mempoolָ���ָ��
*@param[in]      iMax      mempool������
*@param[in]      iUnit     Ӧ�����ݵĴ�С
*@param[in]      pvBase    �����γ�mempool���ڴ��ָ��
*@param[in]      iSize     pvBase �ڴ�Ĵ�С
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int tmempool_init(TMEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, int iSize);

/**����mempool�ڴ沢��ʼ��
*@param[in,out]	 ppstPool  mempoolָ���ָ��
*@param[in]      iMax      mempool������
*@param[in]      iUnit     Ӧ�����ݵĴ�С
		
*@retval  0  �ɹ�
         -1  ʧ��

*@note   ��tmempool_destroy���ʹ��         
*/
int tmempool_new(TMEMPOOL** ppstPool, int iMax, int iUnit);

/**attach һ���Ѵ��ڵ�mempool�ڴ棬������ڴ��ʽ
*@param[in,out]	 ppstPool  mempoolָ���ָ��
*@param[in]      iMax      mempool������
*@param[in]      iUnit     Ӧ�����ݵĴ�С
*@param[in]      pvBase    �����γ�mempool���ڴ��ָ��
*@param[in]      iSize     pvBase �ڴ�Ĵ�С
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int tmempool_attach(TMEMPOOL** ppstPool, int iMax, int iUnit, void* pvBase, int iSize);

/**������tmempool_new�������ڴ��*/
int tmempool_destroy(TMEMPOOL** ppstPool);

/**����mempool��memblock���䷽ʽ
*@param[in]	     pstPool   mempoolָ��
*@param[in]      iMethod   TMEMPOOL_FIFO  �Ƚ��ȳ�,Ĭ�Ϸ�ʽ; TMEMPOOL_LIFO ����ȳ�
		
*@retval  0  �ɹ�
         -1  ʧ��
*/
int tmempool_set_method(TMEMPOOL* pstPool, int iMethod);

/**����������ȡӦ������
*@param[in]	     pstPool   mempoolָ��
*@param[in]      iIdx      memblock������
		
*@retval  not NULL  �ɹ�
          NULL      ʧ��
*/
void* tmempool_get(TMEMPOOL* pstPool, int iIdx);

/**����������ȡӦ������
*@param[in]	     pstPool   mempoolָ��
*@param[in]      iPos      memblock ��������0~max-1
		
*@retval  not NULL  �ɹ�
          NULL      ʧ��
*/
void* tmempool_get_bypos(TMEMPOOL* pstPool, int iPos);


/**����һ��memblock
*@param[in]	     pstPool   mempoolָ��
		
*@retval  >=0     �ɹ�,   ����memblock������
          -1      ʧ��
*/
int tmempool_alloc(TMEMPOOL* pstPool);

/**�ͷ�һ������ΪiIdx��memblock */
int tmempool_free(TMEMPOOL* pstPool, int iIdx);

/**�������ɿ�������һ����iStart!=iEndʱʹ��*/
int tmempool_fix(TMEMPOOL* pstPool);

/**������0��ʼ�ҵ���һ��ʹ���е�memblock
*@param[in]	     pstPool   mempoolָ��
*@param[out]     piPos     �洢memblock index
		
*@retval   0      �ɹ�
          -1      ʧ��
*/
int tmempool_find_used_first(TMEMPOOL* pstPool, int* piPos);

/**�������һ����������һ��������ʼ�ҵ�һ��ʹ���е�memblock
*@param[in]	     pstPool   mempoolָ��
*@param[in,out]  piPos     
                       in  ���������
                      out  >=0  �ҵ�������
                           -1   �Ѿ�û����һ��ʹ���е�memblock��
                      
*@retval  >=0     �ɹ�
          -1      ʧ��
*/
int tmempool_find_used_next(TMEMPOOL* pstPool, int* piPos);

#ifdef __cplusplus
}
#endif

#endif /* TMEMPOOL_H */
