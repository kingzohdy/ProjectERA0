/**
*
* @file     torm_dbprimaykey.h  
* @brief    定义数据库管理系统主键信息
* 
* @author jackyai  
* @version 1.0
* @date 2007-11-23 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TORM_DBPRIMAYKEY_H
#define TORM_DBPRIMAYKEY_H

#include "tdr/tdr_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TORM_MAX_STRING_KEY_DATA	1024	/**< 字符串主键成员最大数据长度 */
#define TORM_PRIMAYKEY_MAX_ENTRY	8		/**< 组成数据库表主键的最大元素数*/

/**DB关键字类型定义 
*/
enum tagTormDBKeyType
{
	TORM_DBKEY_TYPE_INT32	= 0,	/**< 32位整数类型*/	
	TORM_DBKEY_TYPE_INT64,		/**< 64位整数类型*/
	TORM_DBKEY_TYPE_FLOAT,		/**< 浮点数*/
	TORM_DBKEY_TYPE_STR,		/**< 字符串类型,以NULL字符结尾*/
	TORM_DBKEY_TYPE_BIN,		/**< 二进制数据*/
};
typedef enum tagTormDBKeyType	ENRORMDBKEYTYPE;

union tagTormDBKeyData
{
	int iData;
	tdr_longlong llData;
	float fData;
	char szData[TORM_MAX_STRING_KEY_DATA];
	struct 
	{
		int iLen;
		char bData[TORM_MAX_STRING_KEY_DATA];
	}binData;
};
typedef union tagTormDBKeyData TORMDBKEYDATA;

struct  tagTormDBKeyInfo
{
	ENRORMDBKEYTYPE enType;
	TORMDBKEYDATA stData;
};
typedef struct  tagTormDBKeyInfo  TORMDBKEYINFO;
typedef struct  tagTormDBKeyInfo  *LPTORMDBKEYINFO;

struct tagTormPrimayKeyInfo
{
	int iNum;		/*组成主键的成员数*/
	TORMDBKEYINFO stKeys[TORM_PRIMAYKEY_MAX_ENTRY];
};
typedef struct tagTormPrimayKeyInfo TORMPRIMAYKEYINFO;
typedef struct tagTormPrimayKeyInfo *LPTORMPRIMAYKEYINFO;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
