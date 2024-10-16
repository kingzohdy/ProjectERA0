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

typedef int (*SHT_CMP)(const void* pvItem1, const void* pvItem2);
typedef unsigned int (*SHT_HASHCODE)(const void* pvItem);
typedef int (*SHT_PRINT)(FILE* fp, const void* pvItem);

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

/* hash function. */
unsigned int sht_get_code(const char* szKey);

int sht_check(void* pvBuff, int iBuff, int iBucket, int iMax, int iUnit);

/* some operation on hash table. */
LPSHTABLE sht_create(int iBucket, int iMax, int iUnit, int* piBuff);
LPSHTABLE sht_init(void* pvBuff, int iBuff, int iBucket, int iMax, int iUnit);
LPSHTABLE sht_attach(void* pvBuff, int iBuff, int iBucket, int iMax, int iUnit);

void* sht_insert_unique(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode); /* insert a item into hashtable, keep it unique.*/
void* sht_insert_multi(LPSHTABLE pstTab, const void* pvData, SHT_HASHCODE pfnHashCode); /* insert a item into hashtable, do not keep unique.*/
void* sht_find(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);
void* sht_remove(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);
void* sht_remove_by_pos(LPSHTABLE pstTab, int iPos);

void* sht_pos(LPSHTABLE pstTab, int iPos, int* pfValid);	/* get item by pos from table. */

int sht_destroy(LPSHTABLE* ppstTab);

int sht_rebuild(LPSHTABLE pstTab);

int sht_is_empty(LPSHTABLE pstTab);
int sht_is_full(LPSHTABLE pstTab);

int sht_dump_all(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);
int sht_dump_valid(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

#ifdef __cplusplus
}
#endif

#endif /* SHTABLE_H */
