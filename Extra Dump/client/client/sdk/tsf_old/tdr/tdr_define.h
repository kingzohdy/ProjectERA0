/**
*
* @file     tdr_define.h 
* @brief    TDR常量定义
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-22 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TDR_DEFINE_H
#define TDR_DEFINE_H


/** @name 函数参数输入/输出属性
 *  @{
 */
/*表示该参数只是输入参数*/
#ifndef IN
#define IN  
#endif
 

/*表示该参数只是输出参数*/
#ifndef OUT
#define OUT
#endif

    
/*表示该参数既是输入参数，又是输出参数*/
#ifndef INOUT
#define INOUT
#endif
	        
/** @}*/ // 函数参数输入/输出属性

/**
*bool型变量取值定义
*/ 
typedef enum tagBoolean
{
	TDR_FALSE = 0,	    /**< false*/
	TDR_TRUE			/**< true*/ 
}TDRBOOLEAN;


/** @name 元数据描述常量
 *  @{
 */
#define TDR_MAGIC		0x02D6      /**< 元数据二进制文件中的"魔数"*/

        
#define	TDR_MAX_VERSION		0x7fffffff		/**< 元数据库最大版本号*/

#define TDR_NAME_LEN		32		/**< 元数据名字串最大字节数*/
#define TDR_DESC_LEN		1024		/**< 元数据描述串最大字节数*/
#define TDR_MACRO_LEN	64		/**< 元数据宏定义名字串最大字节数*/
#define TDR_CHNAME_LEN	512		/**< 元数据中文名串最大字节数*/
#define TDR_DEFAULT_VALUE_LEN	1024	/**< 元数据缺省值最大字节数*/
/** @}*/ // 元数据描述常量

#define TDR_MAX_FILES_IN_ONE_PARSE		128   /**< 一次解析的最大XML文件数*/

#define TDR_DEFAULT_ALIGN_VALUE		1	/**<缺省字节对齐值*/



/** @name 支持的元数据XML标签集的版本信息
 *  @{
 */
#define TDR_XML_TAGSET_VERSION_0             0      /**< 第0版本*/
#define TDR_SUPPORTING_MIN_XMLTAGSET_VERSION	TDR_XML_TAGSET_VERSION_0       /**< 支持的最少版本号*/
#define TDR_XML_TAGSET_VERSION_1             1      /**< 第1版本*/
#define TDR_SUPPORTING_MAX_XMLTAGSET_VERSION	TDR_XML_TAGSET_VERSION_1      /**< 支持的最大版本号*/
/** @}*/ // 支持的元数据XML标签集的版本信息


#define TDR_POINTER_UINT_SIZE 4         /**<指针变量存储的字节数*/



/** @name 元数据生成c语言头文件规则(各规则可以通过‘与’的方式组合)
*  @{
*/
/**成员名命名规则缺省规则：添加类型前缀，如果没有类型前缀则成员名首字母变成小写。
*例如某成员在DR中的名字属性值为"name",其类型为"string",则在c语言头文件中名字为:
*	szName	
*/
#define TDR_HPPRULE_DEFAULT   0x0000

#define TDR_HPPRULE_NO_TYPE_PREFIX	0x0001	/**<成员名命名规则：不添加类型前缀*/

/**成员名命名规则：添加自定义前缀规则：例如某成员在DR中的名字属性值为"name",其类型为"string",自定义前缀为"m_" 
*则在c语言头文件中名字为:	char*	m_Name	
*/
#define TDR_HPPRULE_ADD_CUSTOM_PREFIX 0x0002

#define TDR_HPPRULE_NO_LOWERCASE_PREFIX	0x0004	/**<成员名命名规则：不强制将成名首字母变成小写*/

#define TDR_HPPRULE_NO_TYPE_DECLARE		0x0008 /**<头文件中不添加类型前缀*/

/** @ }*/

#define TDR_MAX_CUSTOM_NAME_PREFIX_LEN	9		/**<用户自定义名字前缀的最大长度*/


/**@name IO属性值
*@ {*/
#define TDR_IO_NOLIMIT		0x00  /**<表示其IO输出/输出没有限制*/
#define TDR_IO_NOINPUT		0x01    /**<表示此成员不能输入*/
#define TDR_IO_NOOUTPUT		0x02    /**<表示此成员不能输出*/
#define TDR_IO_NOIO		0x03    /**< 表示此成员不参与输入/输出*/
#define TDR_IO_MIN_VALUE      TDR_IO_NOLIMIT
#define TDR_IO_MAX_VALUE TDR_IO_NOIO
/*@ }*/

/** @name 输入输出XML格式
*  @{
*/
#define TDR_IO_OLD_XML_VERSION		1		/**<旧的XML输入/输出格式*/
#define TDR_IO_NEW_XML_VERSION		2		/**<新的XML输入/输出格式*/
/** @} */

/**@name 排序方法
*@ {*/
enum tagTDRSortMethod
{
	TDR_SORTMETHOD_NONE_SORT =	0x00,  /**< 不排序*/
	TDR_SORTMETHOD_ASC_SORT	=	0x01,    /**<升序排序*/
	TDR_SORTMETHOD_DSC_SORT =0x02,    /**<降序排序*/
};
/*@ }*/

/**@name DBMS相关宏定义
*@ {*/
#define TDR_DBMS_NAME_LEN					32	/**<DBMS名称最大长度*/
#define TDR_DBMS_CONNECTION_INFO_LEN		256  /**< DBMS连接信息的最大长度*/
#define TDR_DBMS_DATABASE_NAME_LEN			64	/**<DBMS数据库名称最大长度*/
#define TDR_DBMS_USER_LEN					256 /**<DBMS用户名称最大长度*/
#define TDR_DBMS_PASSWORD_LEN				64  /**<DBMS用户密码最大长度*/
/*@ }*/


/** @name DB操作ID
*  @{
*/
typedef enum 
{
	TDR_DBOP_NONE = 0,		/**<无操作*/
	TDR_DBOP_INSERT,		/**<插入操作*/	
	TDR_DBOP_UPDATE,		/**<更新操作*/	
	TDR_DBOP_DELETE,		/**<删除操作*/
	TDR_DBOP_SELECT,		/**<查询操作*/
}TDRDBOPID;

#endif /* TDR_DEFINE_H */
