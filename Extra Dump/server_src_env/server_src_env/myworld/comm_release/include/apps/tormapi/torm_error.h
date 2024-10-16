/**
*
* @file     torm_error.h 
* @brief    TORM错误处理函数
* 
* @author jackyai  
* @version 1.0
* @date 2007-11-28 
*
*
* All rights reserved.
*
*/

#ifndef TORM_ERROR_H
#define TORM_ERROR_H

#include "comm/tmodid.h"
#include "comm/terr.h"
#include "tdr/tdr.h"

/** @defgroup TORM_ERR TORM_错误处理
* @{
*@note TORM错误处理的基本思想:
*  - 使用一个整数来存储错误代码信息，此整数值的含义如下:
*      -# <0 表示发生错误，其值为具体错误的代码
*      -# 0 表示成功
*      -# >0 表示成功，但发生了某些特殊事情
*  - 错误代码存储在4字节的整数中，由三部分组成:
*      -# 错误级别: 1个字节，具体为高位的第1个字节
*      -# 模块ID:  1字节，具体为高位的第2个字节
*      -# 错误号:  2字节，0-1023号表示通用错误，各模块定义错误从1024开始
* 
*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define TORM_SUCCESS				0
#define TORM_ERR_ERROR				-1


#define TORM_ERR_MAKE_DEBUG(errno)		TERR_MAKE_DEBUG(TMODID_ORM, errno)
#define TORM_ERR_MAKE_WARN(errno)		TERR_MAKE_WARN(TMODID_ORM, errno)
#define TORM_ERR_MAKE_ERROR(errno)		TERR_MAKE_ERROR(TMODID_ORM, errno)
#define TORM_ERR_MAKE_SEVERE(errno)		TERR_MAKE_SEVERE(TMODID_ORM, errno)


#define TORM_ERR_IS_ERROR(error)			TERR_IS_ERROR(error)

/*ORM各模块错误代码基点*/
#define TORM_ERRNO_SVE_BASE		TERR_MAX_ERRNO	/*ORM服务器使用的错误代码基点*/
#define TORM_ERRNO_PROTOCOLAPI_BASE		TERR_MAX_ERRNO +1024	/*ORM 协议处理API的错误代码基点*/
#define TORM_ERRNO_DBAPI_BASE		TERR_MAX_ERRNO +2048	/*ORM 协议处理API的错误代码基点*/

/*定义组合错误代码*/
#define TORM_ERR_MAKE_SVR_ERROR(err)    TORM_ERR_MAKE_ERROR(TORM_ERRNO_SVE_BASE + err)
#define TORM_ERR_MAKE_PROTOCOLAPI_ERROR(err)    TORM_ERR_MAKE_ERROR(TORM_ERRNO_PROTOCOLAPI_BASE + err)
#define TORM_ERR_MAKE_DBAPI_ERROR(err)    TORM_ERR_MAKE_ERROR(TORM_ERRNO_DBAPI_BASE + err)

	/**
	*TORM SVR错误代码定义 
	*/
	typedef enum
	{
		TORM_ERROR_SVR_NONE = 0,                    /**< 没有错误*/

		TORM_ERROR_CHANNEL_CONFUSE,
		TORM_ERROR_CHANNEL_BADMSG,
		TORM_ERROR_CHANNEL_FULL,
		TORM_ERROR_CHANNEL_EMPTY,
		TORM_ERROR_CHANNEL_ROUND,
		TORM_ERROR_CHANNEL_NOENOUGH,
		TORM_ERROR_QUERY,
		TORM_ERROR_QUERY_DUPKEY,
		TORM_ERROR_PART_PRIKEY,
		TORM_ERROR_CONDITION_INSERT,		/*不支持where条件的插入操作*/
		
		TORM_ERROR_SVR_INVALID_META,		/*无效的操作数据描述*/
		TORM_ERROR_SVR_GENERATE_SQL,		/*生成数据操作SQL失败*/
		TORM_ERROR_SVR_PACK_RECORD,			/*打包检索出的数据时失败*/
		TORM_ERROR_SVR_COUNT                     /**< 错误代码结束标志*/ 
	} TORMSVRERROR;

	/**
	*TORM Protocolapi错误代码定义 
	*/
	typedef enum
	{
		TORM_ERROR_PROTOCOLAPI_NONE = 0,                    /**< 没有错误*/
		TORM_ERROR_PROTOCOLAPI_NOMEM,				/**<分配内存失败*/
		TORM_ERROR_PROTOCOLAPI_OPENMB,				/**<打开管理共享内存中元数据库集合的句柄失败*/
		TORM_ERROR_PROTOCOLAPI_UNFINDMETALIB,       /**<在共享内存中元数据库集合查找指定元数据库失败*/
		TORM_ERROR_PTOTOCOLAPI_UNFINDMETA,			/**<在元数据描述库中找不到指定数据的元数据描述*/
		TORM_ERROR_PROTOCOLAPI_UNSUPPORTCMD,		/**<消息中的CmdID尚不支持*/
		TORM_ERROR_PROTOCOLAPI_NOPRIMRYKEY,			/**<操作数据结构体中缺少数据库表的主键信息*/
		TORM_ERROR_PROTOCOLAPI_METANAMECONFLICT,		/**<操作数据的元数据描述库中的元数据名同名,目前不支持*/
		TORM_ERROR_PROTOCOLAPI_METAINDEXNUMCONFLICT,	/**<操作数据的元数据描述的实际索引数与预期的不一致*/
		TORM_ERROR_PROTOCOLAPI_INVALID_PROTOCOL_CUT_VER,	/**<协议的剪裁版本比协议元数据的基版本号小*/
		TORM_ERROR_PROTOCOLAPI_DATA_CUT_VER,			/**<操作数据的剪裁版本比其元数据的基版本号小*/
		TORM_ERROR_PRO_NO_PACK_BUFF,					/**<数据打包时存储空间不够*/
		TORM_ERROR_PRO_INVALID_DATA_BUFF,				/**<存储操作数据的缓冲区无效:空指针或空间小于预期*/
		TORM_ERROR_PRO_UNFINDENTRYOFFINDEX,				/**<在成员偏移索引表中找不到指定成员的偏移*/
		TORM_ERROR_PRO_INSUFFICIENT_DATA_BUFF,			/**<操作数据缓冲区小于预期的大小*/
		TORM_ERROR_PRO_NOSPLITENTRY,					/**<此数据对应的数据库表是分表存储的,且指定了分表成员,但此操作数据元数据描述中没有指定分表成员*/
		TORM_ERROR_PRO_CUTOFF_SPLITENTRY,				/**<此数据对应的数据库表是分表存储的,且指定了分表成员,剪裁版本将分表成员剪裁掉了*/
		TORM_ERROR_PRO_UNFINEDENTRYNAMEINDEX,			/**<在成员名字-指针索引表中根据名字找不到此成员的信息*/
		TORM_ERROR_PRO_NAMELISTLIMITED,					/**<需要在操作成员列表中加入操作成员,但此列表已经达到了最大成员数限制*/
		TORM_ERROR_PRO_FAILEDCALCADDR,					/**<通过元数据索引表定位成员的地址失败*/
		TORM_ERROR_PRO_INVALIDREFERVALUE,				/**<成员的refer属性不正确*/
		TORM_ERROR_PRO_NODATA,							/**<已经没有数据可取*/
		TORM_ERROR_PRO_NOUNPACKBUFF,					/**<待解包的数据缓冲区大小比预期的要小*/
		TORM_ERROR_PRO_INVALIDSTRINGLEN,				/**<string类型信息的长度信息不正确*/
		TORM_ERROR_PRO_INVALIDPKGLEN,					/**<ORM协议消息长度校验失败*/
		TORM_ERROR_PRO_PRIMARYKEYNUMLIMITED,			/**<组成主键信息的成员数超过了最大数目限制*/
		TORM_ERROR_PRO_NOSQLBUFF,						/**<保存SQL语句的缓冲区空间不够*/
		TORM_ERROR_PRO_DATA_VERSION_TOO_HIGHT,			/**<操作数据的剪裁版本不能比元数据描述库的版本高*/
		TORM_ERROR_PRO_NOTNULLLIMIT,					/**<此操作数据对应的数据库表包含NOT NULL成员,但是根据指定版本做剪裁时,将不包含NOT NULL成员*/
		TORM_ERROR_PRO_INVALIDSELECTOR,					/**<数据成员的select属性所指的属性值不正确*/
		TORM_ERROR_PRO_EXTERN_COMPOSITE_ARRAY,			/**<当对结构体展开存储到数据库中时，由于其子成员的名字不唯一，会导致生成的数据库表列名冲突*/
		TORM_ERROR_PRO_SAVE_RECORDS,					/**<保存select响应中检索出的数据到句柄中时失败*/
		TORM_ERROR_PRO_INVALID_PARAM,					/**<传递给接口的参数无效*/
		TORM_ERROR_PRO_ADD_LIMITS_INFO,					/**<在自定义查询条件中条件LIMIT信息时失败*/
		TORM_ERROR_PROTOCOLAPI_COUNT                     /**< 错误代码结束标志*/ 		
	} TORMPROTOCOLERROR;

	/**
	*TORM db api错误代码定义 
	*/
	typedef enum
	{
		TORM_ERROR_DBAPI_NONE = 0,                    /**< 没有错误*/
		TORM_ERROR_DBAPI_COUNT                     /**< 错误代码结束标志*/ 
	} TORMDBERROR;


	/**
	* 根据错误代码获取错误信息
	* @param[in] iErrorCode 错误代码
	*
	* @return  错误信息串的指针
	*/
	extern char const*	torm_error_string(IN int iErrorCode);

	/** @} */ // TDR_ERR TDR错误处理

#ifdef __cplusplus
}
#endif




#endif /* TORM_ERROR_H */
