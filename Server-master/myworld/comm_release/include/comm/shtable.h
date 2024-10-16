#ifndef SHTABLE_H
#define SHTABLE_H

/*************************************************************
#FUNCTION:	static hash table defintion.
#AUTHOR:	Niezhiming
#DATE:		14-7-2005
*************************************************************/
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SHT_MIN_HEAD			0x100
#define SHT_BUCKET_ALIGN		0x100
#define SHT_MIN_ALIGN			0x08
#define SHT_VERSION				0x0101

#define SHTF_NEEDFREE			0x01

struct tagSHitem
{
	int iPrev;
	int iNext;
	unsigned uCode;
	int fValid;
	char szData[1];
};

typedef struct tagSHitem		SHITEM;
typedef struct tagSHitem		*LPSHITEM;

struct tagSHbucket
{
	int iCount;
	int iHead;
};

typedef struct tagSHbucket		SHBUCKET;
typedef struct tagSHbucket		*LPSHBUCKET;

typedef int (*SHT_CMP)(const void* pvData1, const void* pvData2);
typedef unsigned int (*SHT_HASHCODE)(const void* pvData);
typedef int (*SHT_PRINT)(FILE* fp, const void* pvData);

struct tagSHtable
{
	unsigned int cbSize;		/* the size of this struct. */
	unsigned int uFlags;		/* some flags. */
	int iVersion;				/* version number. */
	int iBuff;					/* the size of the buff. */

	int iBucket;				/* bucket number used. */
	int iMax;					/* maximum items can store. */
	int iItem;					/* current item number. */
	int iHeadSize;

	int iBucketOff;
	int iBucketSize;

	int iDataOff;
	int iDataSize;
	int iDataUnitMin;			/* the data-unit's real size. */
	int iDataUnitMax;			/* the data-unit's occupy size.*/

	int iFreeHead;
	int iRes;					/* reserved. */
};

typedef struct tagSHtable			SHTABLE;
typedef struct tagSHtable			*LPSHTABLE;

/*内存对齐*/
#define SHT_ROUND(size)					( ( (size) + SHT_MIN_ALIGN - 1) /SHT_MIN_ALIGN*SHT_MIN_ALIGN )

#define SHT_HEADSIZE()					( SHT_MIN_HEAD < sizeof(SHTABLE) ? sizeof(SHTABLE) : SHT_MIN_HEAD )

#define SHT_BUCKETSIZE(buck)			( (buck) * sizeof(SHBUCKET) )

#define SHT_DATAUNIT(data)				SHT_ROUND((data) + offsetof(SHITEM, szData))

#define SHT_DATASIZE(max, unit)			( (max) * SHT_DATAUNIT(unit) )

#define SHT_SIZE(buck, max, unit)		( SHT_HEADSIZE() + SHT_BUCKETSIZE(buck) + SHT_DATASIZE(max, unit) )

#define SHT_GET_BUCKET(pstTab, i)		( (LPSHBUCKET) ( ((int)(pstTab)) + pstTab->iBucketOff + i*sizeof(SHBUCKET) ) )

#define SHT_GET_ITEM(pstTab, i)			( (LPSHITEM) ( ((int)(pstTab)) + pstTab->iDataOff + i*pstTab->iDataUnitMax ) )

#define SHT_DATA2ITEM(pvData)			( (SHITEM*) ( ((int)(pvData)) - offsetof(SHITEM, szData)) )
#define SHT_ITEM2DATA(pvItem)			( (pvItem)->szData )

/**提供一种默认的字符串hash code方法
*@param[in]	 szKey  一个以0结尾的字符串
		
*@retval  hash code
*/
unsigned int sht_get_code(const char* szKey);

/**检查内存是否符合shtable的格式，一般是   重新attach shtable shm 时使用
*@param[in]	 pvBuff  内存地址指针
*@param[in]  iBuff   内存大小
*@param[in]  iBucket  Bucket num
*@param[in]  iMax    Max item num
*@param[in]  iUnit   每个item的大小
		
*@retval  0  成功
         -1  失败
*/
int sht_check(void* pvBuff, int iBuff, int iBucket, int iMax, int iUnit);

/**在内存中生成一个shtable
*@param[in]      iBucket  Bucket num
*@param[in]      iMax    Max item num
*@param[in]      iUnit   每个item的大小
*@param[in,out]  piBuff  指向生成的shtable的大小的指针
		
*@retval  not NULL  成功shtable 指针
         NULL  失败

*@note  此函数与sht_destroy 配对使用         
*/
LPSHTABLE sht_create(int iBucket, int iMax, int iUnit, int* piBuff);

/**初始化shtable 内存
*@param[in]	 pvBuff  内存地址指针
*@param[in]  iBuff   内存大小
*@param[in]  iBucket  Bucket num
*@param[in]  iMax    Max item num
*@param[in]  iUnit   每个item的大小
		
*@retval  not NULL  成功shtable 指针
         NULL  失败
*/
LPSHTABLE sht_init(void* pvBuff, int iBuff, int iBucket, int iMax, int iUnit);

/**attach shtable 内存，实际上是执行sht_check检查内存再返回shtable 指针
*@param[in]	 pvBuff  内存地址指针
*@param[in]  iBuff   内存大小
*@param[in]  iBucket  Bucket num
*@param[in]  iMax    Max item num
*@param[in]  iUnit   每个item的大小
		
*@retval  not NULL  成功shtable 指针
         NULL  失败
*/
LPSHTABLE sht_attach(void* pvBuff, int iBuff, int iBucket, int iMax, int iUnit);

/**insert unique item into shtable
*@param[in]	 pstTab  shtable 指针
*@param[in]  pvData  insert item data 指针
*@param[in]  pfnCmp  比较两个item data 的函数指针, 此比较函数的
                     返回值为=0 两item data相同, <0 或>0 两item data不相同
*@param[in]  pfnHashCode  根据item data 生成hash code 的函数指针
		
*@retval  not NULL  成功，shtable 内存放data的地址指针
          NULL      失败
*@note    此函数调用成功后只是在shtable内分配了一块内存，需要外部
          将要插入的数据再copy到shtable中，此方式是为了增加灵活性，
          可参见文档中的代码示例
*/
void* sht_insert_unique(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode); /* insert a item into hashtable, keep it unique.*/

/** insert a item into hashtable, do not keep unique.*/
void* sht_insert_multi(LPSHTABLE pstTab, const void* pvData, SHT_HASHCODE pfnHashCode); 

/**find item in shtable
*@param[in]	 pstTab  shtable 指针
*@param[in]  pvData  find item data 指针
*@param[in]  pfnCmp  比较两个item data 的函数指针, 此比较函数的
                     返回值为=0 两item data相同, <0 或>0 两item data不相同
*@param[in]  pfnHashCode  根据item data 生成hash code 的函数指针
		
*@retval  not NULL  成功，shtable 内存放data的地址指针
          NULL      失败

*@note   pvData只需要填入与pfnCmp和pfnHashCode 相关的部分即可          
*/
void* sht_find(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);

/**remove item in shtable
*@param[in]	 pstTab  shtable 指针
*@param[in]  pvData  remove item data 指针
*@param[in]  pfnCmp  比较两个item data 的函数指针, 此比较函数的
                     返回值为=0 两item data相同, <0 或>0 两item data不相同
*@param[in]  pfnHashCode  根据item data 生成hash code 的函数指针
		
*@retval  not NULL  成功，shtable 内存放data的地址指针
          NULL      失败

*@note   pvData只需要填入与pfnCmp和pfnHashCode 相关的部分即可          
*/
void* sht_remove(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);

/**remove item in shtable by pos
*@param[in]	 pstTab  shtable 指针
*@param[in]  iPos    remove item data 在shtable 在item list 中的索引位置

*@retval  not NULL  成功，shtable 内存放data的地址指针
          NULL      失败

*@note   item list 是一个数组，其索引>=0 , <pstTab->iMax          
*/
void* sht_remove_by_pos(LPSHTABLE pstTab, int iPos);

/**remove item in shtable by addr
*@param[in]	 pstTab  shtable 指针
*@param[in]  pstItem  item 在shtable中的地址指针

*@retval  not NULL  成功，shtable 内存放data的地址指针
          NULL      失败
       
*/
void* sht_remove_by_addr(LPSHTABLE pstTab, LPSHITEM pstItem);


/**get item in shtable by iPos
*@param[in]	 pstTab  shtable 指针
*@param[in]  iPos    item data 在shtable 在item list 中的索引位置
*@param[in,out] pfValid  标识此item data 是否有效, *pfValid = 1 有效，=0 无效

*@retval  not NULL  成功，shtable 内存放data的地址指针
          NULL      失败
          
*@note   item list 是一个数组，其索引>=0 , <pstTab->iMax          
  
*/
void* sht_pos(LPSHTABLE pstTab, int iPos, int* pfValid);	

/**与sht_create配对使用，释放资源*/
int sht_destroy(LPSHTABLE* ppstTab);

/**重建item 空闲链表和bucket 链，较少使用*/
int sht_rebuild(LPSHTABLE pstTab);

/**判断shtable item data空间是否为空, 返回1 为空, 0 不空*/
int sht_is_empty(LPSHTABLE pstTab);

/**判断shtable item data空间是否已满, 返回1 已满, 0 未满*/
int sht_is_full(LPSHTABLE pstTab);

/**遍历shtable的item list 中的所有项，对每一个valid item data 调用pfnPrint(fp, pvData)*/
int sht_dump_all(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

/**遍历shtable的item list 中的所有valid item项，对每一个item data 调用pfnPrint(fp, pvData)*/
int sht_dump_valid(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

/**已知item hash值强制插入，特殊情况下使用，by kent 2008.1.18*/
void *sht_insert_force(LPSHTABLE pstTab, unsigned int uCode);

#ifdef __cplusplus
}
#endif

#endif /* SHTABLE_H */
