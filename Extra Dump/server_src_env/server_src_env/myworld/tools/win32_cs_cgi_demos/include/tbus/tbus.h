/** @file $RCSfile: tbus.h,v $
  Invoked function header file for tsf4g-tbus module
  $Id: tbus.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
@author $Author: niko $
@date $Date: 2011/01/04 11:02:51 $
@version $Revision: 1.1.1.1 $
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/


#ifndef _TBUS_H
#define _TBUS_H

/* automatically include the correct library on windows */
#ifdef WIN32

# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */


#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libtbus"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libtbus.lib")
#endif
#endif /*#ifdef WIN32*/

#include "pal/pal.h"
#include "tlog/tlog.h"
#include "tbus/tbus_macros.h"
#include "tbus/tbus_error.h"



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


/* TbusFlag*/
enum tagTbusFlag 
{
	TBUS_FLAG_START_DYE_MSG = 1, 	/* 指示tbus系统对此消息进行染色 */
	TBUS_FLAG_KEEP_DYE_MSG = 2, 	/* 指示tbus系统保持前一个消息的染色信息 */
};

/* initialize flags*/
#define TBUS_INIT_FLAG_NONE		0x0		/*无特殊标记*/


#define TBUS_ADDR_DEFAULT                        	0 	/*  表示适配第一个地址*/
#define TBUS_ADDR_ALL                            	-1 	/* 表示适配所有地址 */


#define TBUS_LOGCAT_NAME	"tsf4g.tbus"		/*tbus日志类名称*/

#define TBUS_DEFAULT_GCIM_KEY	"1688"	/*defalt share memory key fro GCIM(Global Channel Infomation Map)*/
#define TBUS_DEFAULT_GRM_KEY	"2688"	/*defalt share memory key fro GRM(Global Route Map)*/

#define TBUS_ROUTE_GLOBAL_SHMKEY	1688 /*defalt share memory key fro GCIM(Global Channel Infomation Map), for tbus_init*/

#define tbus_initialize	tbus_init_ex		/*tbus_initialize接口的名字改成tbus_init_ex了*/


#define TBUS_CHANNEL_MAGIC	0x42545553		//TBUS


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef  int	TBUSADDR;		/*tbus系统通信地址*/
typedef  int	*LPTBUSADDR;		/*tbus系统通信地址*/


typedef struct tagTbusChannel                          	*LPTBUSCHANNEL;

/*route info of package */
struct tagHeadRoute
{
	TBUSADDR iSrc;                           
};
typedef struct tagHeadRoute                        	HEADROUTE;
typedef struct tagHeadRoute                        	*LPHEADROUTE;


/** @defgroup TBUS_INIT TBUS_句柄管理
*  @{
*/

/**brief 初始化Tbus系统
*	其主要职责是向tbus配置管理系统注册业务ID，并尝试挂载(attach)上保存tbus
*	通道配置信息的共享内存GCIM(Global Channel Infomation Map)
*@param[in] a_pszShmkey	保存生产GCIM共享内存key的信息串，此信息串中的信息只能为十进制数字
*	串或为一个文件的路径（当此文件不存在时，tbus系统会尝试生成此文件），根据此信息串生成GCIM共享内存key的算法是:
*	- 十进制数字串	则将此数字串转换整数，此整数作为GCIM的共享内存的key
*	- 文件路径	将此文件路径和a_iBussId作为参数，调用ftok接口生产GCIM共享内存的key
*	- NULL或空串	使用缺省值 TBUS_ROUTE_GLOBAL_SHMKEY 作为GCIM共享内存的key	
*@param[in]	a_iFlag 初始化操作标志位，目前此参数为预留参数,以备后续扩展。目前设置缺省值0(TBUS_INIT_FLAG_NONE)即可
*
*@note	本接口必须在其他tbus接口调用之前调用
*@note	当业务进程退出时，必须调用tbus_fini接口，是否本接口分配的资源
*@note	在同一个进程内，本接口只有第一次成功调用会产生作用，一旦tbus系统初始化好后，后续调用本接口
*	将直接返回
*@note	本接口是非线程安全的，同一个进程内如果有多个线程，调用者必须自行保证此接口互斥调用
*
*@pre \e 本接口调用之前必须通过 tbusmgr工具或tbus配置管理系统成功创建GCIM共享内存，否则本接口调用会
*	失败
*
*@retval 0 处理成功
*@retval <0 处理失败,通过TBUS_ERR_GET_ERROR_CODE(retval),可以获取错误代码，可能的错误代码有：
*	- TBUS_ERROR_INIT_LOGSYSTEM_FAILED 初始化日志系统失败
*	- TBUS_ERROR_GEN_SHMKEY_FAILED	生成共享内存的key值失败
*	- TBUS_ERROR_SHMGET_FAILED	通过shmget分配共享内存失败
*	- TBUS_ERROR_SHMAT_FAILED	通过shmat加载共享内存失败
*	- TBUS_ERROR_CHECK_GCIMSHM_FAILED	GCIM共享内存校验失败
*
*see tbus_fini
*@note 如果a_pszShmkey为NULL，将尝试"1688"为key的共享内存
*/
TSF4G_API int tbus_init_ex(IN const char *a_pszShmkey, IN int a_iFlag);



/**
@brief 创建tbus处理句柄

@param[in,out] a_piHandle 存储处理句柄的指针

@retval 0 -- success, get handle ok
@retval <0 -- failed, 通过TBUS_ERR_GET_ERROR_CODE(retval)可以获取错误代码，可以会返回以下错误代码
	-	TBUS_ERROR_INVALID_ARG	传递给接口的参数不对
	-	TBUS_ERROR_NOT_INITIALIZED	bus系统还没有初始化
	-	TBUS_ERROR_HANDLE_FULL	已分配tbus句柄数据已经达到设定的最大数目，不能在分配
@pre \e   a_piHandle should be not NULL 
@note 本接口是非线程安全的，创建出来的句柄也只能在同一个线程空间里使用 
@note 本接口创建的句柄必须使用tbus_delete来关闭，以释放资源
*/
TSF4G_API int tbus_new( INOUT int *a_piHandle ) ;

/**
@brief 删除一个tubs处理句柄

@param[in,out] a_piHandle 保存tbus处理句柄的指针，tbus句柄通过tbus_new()获取

@retval 0 -- success, deleted ok
@retval !0 -- failed

@note
*/
TSF4G_API void tbus_delete ( INOUT int *a_piHandle ) ;


/**
@brief 关闭tbus系统

@retval 0 -- success, closed ok
@retval !0 -- failed

@note 只要调用了本接口，tbus其他接口的功能将不再可用
*/
TSF4G_API void tbus_fini ( ) ;

/**
@brief Global init function

@param[in] a_iMapKey -- share memory key pointed to global channel information.
If this value set to 0, default value, key 1688 would be used
@param[in] a_iSize -- channel count in share memory.  If this value set to 0, default value, size 1024 would be used

@retval 0 -- success, service inited ok
@retval !0 -- failed
*
@note This function should be invoked before using any tbus functions
*deprecated	this function is been replaced with tbus_init_ex
*
*see tbus_init_ex
*/
TSF4G_API int tbus_init ( IN const unsigned int a_iMapKey, IN const unsigned int a_iSize ) ;

/**
@brief Set business id
@param[in] a_iBussId -- business id assigned by Senderd-Agent system, if do not get business id from Senderd-Agent,
*	just set 0
@retval 0 -- success
@retval !0 -- failed
*
*@note if business id is not euqal to zero, it should call this function before tbus_init/tbus_init_ex
*
*see tbus_init_ex
*see tbus_ini
*/
TSF4G_API int tbus_set_bussid(IN const int a_iBussId ) ;

/** @} */


/** @defgroup TBUS_ADDR_CONVERT TBUS_通信地址转换
*  @{
*/

/**
*@brief 将点分十进制通信地址字符串转换成tbus系统内存地址
*
*@param[in] a_pszAddr 点分十进制通信地址字符串
*@param[out] a_piAddr 保存tbus通信地址的指针
*
*@pre \e a_pszAddr不能为NULL
*@pre \e a_piAddr 不能为NULL
*
*@retval 0 success
*@retval <0 failed, 可能会返回以下错误代码
	-	TBUS_ERROR_NOT_INITIALIZED	bus系统还没有初始化
	-	TBUS_ERROR_INVALID_ADDR_STRING	点分十进制地址串非法，即分点分十进制通信地址模板不符合
*
*@note	a_pszAddr中保存的点分十进制通信地址串必须符合点分十进制通信地址模板的约定
*@note	调用本接口之前必须调用tbus_init_ex已经初始化好tbus系统
*@see tbus_init_ex
*
*/
TSF4G_API int tbus_addr_aton(IN const char *a_pszAddr, OUT TBUSADDR *a_piAddr) ;


/**
*@brief 将tbus通信地址转换成点分十进制地址信息串
*
*@param[in] a_iAddr tbusxt通信地址
*
*@return 如果成功转换则返回点分十进制地址串，否则返回空串""
*
*@note 返回的成点分十进制表示的地址串保存在一个静态缓冲区中，后一次调用会覆盖前一次调用时获取的信息
*@note	调用本接口之前必须调用tbus_init_ex已经初始化好tbus系统
*@see tbus_init_ex
*/
TSF4G_API char *tbus_addr_ntoa(IN TBUSADDR a_iAddr) ;


/**  @} */



/** @defgroup TBUS_CHANNEL_MNG TBUS_通道管理
*  @{
*/




/**
  @brief 指定tbus通道本地地址,即tbus接口调用者进程的通信地址

  @param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取
  @param[in] a_szSrcAddr 本地地址，点分十进制表示的通信地址串，格式类似如: xxx.xxx.xxx.xxx.xxx

  @retval 0 success
  @retval <0 failed, 可能的错误代码如下:
	-	TBUS_ERR_ARG	传递给接口的参数不对
	-	TBUS_ERROR_NOT_INITIALIZED	bus系统还没有初始化
	-	TBUS_ERROR_INVALID_ADDR_STRING	点分十进制地址串非法，即分点分十进制通信地址模板不符合
	-	TBUS_ERROR_CHANNEL_NUM_LIMITED
	-	TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
	-	TBUS_ERROR_BIND_FAILED
	@note 本接口会通过读写锁互斥访问GCIM共享内存中的信息，因此本接口是阻塞式的
	@pre a_szSrcAddr 不能为NULL
	@pre a_iHandle 为有效句柄
  @see tbus_new
*/
TSF4G_API int tbus_bind_by_str ( IN const int a_iHandle, IN const char *a_szSrcAddr ) ;


/**
  @brief 指定tbus通道本地地址,即tbus接口调用者进程的通信地址

  @param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取
  @param[in] a_iSrcAddr 本地地址，通过tbus_addr_aton接口可以获取

  @retval 0 success
  @retval <0 failed, 可能的错误代码如下:
	-	TBUS_ERR_ARG	传递给接口的参数不对
	-	TBUS_ERROR_NOT_INITIALIZED	bus系统还没有初始化
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_INVALID_ADDR_STRING	点分十进制地址串非法，即分点分十进制通信地址模板不符合
	-	TBUS_ERROR_CHANNEL_NUM_LIMITED
	-	TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
	-	TBUS_ERROR_BIND_FAILED
	@note 本接口会通过读写锁互斥访问GCIM共享内存中的信息，因此本接口是阻塞式的
	@see tbus_new
*/
TSF4G_API int tbus_bind( IN const int a_iHandle, IN const TBUSADDR a_iSrcAddr) ;


/**
@brief 根据全局GCIM中的配置刷新tbus句柄管理的相关通道
	如果绑定地址相关通道有添加则自动添加到tbus中；如果tbus句柄管理的相关通道已经不再gcim配置
	中则定期回收
@param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取

@retval 0 success
@retval <0 failed, 可能的错误代码如下:
-	TBUS_ERR_ARG	传递给接口的参数不对
-	TBUS_ERROR_NOT_INITIALIZED	bus系统还没有初始化
-	TBUS_ERROR_CHANNEL_NUM_LIMITED
-	TBUS_ERROR_NO_MEMORY
-	TBUS_ERROR_SHMGET_FAILED
-	TBUS_ERROR_SHMAT_FAILED
-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
-	TBUS_ERROR_BIND_FAILED
@note tbus API使用者可以定时调用本接口以刷新该句柄下相关通道配置

@pre a_iHandle 为有效句柄
@see tbus_new
@see tbus_bind
*/
TSF4G_API int tbus_refresh_handle( IN const int a_iHandle) ;

/**
  @brief 指定tbus通道的对端地址

  @param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取
  @param[in] a_szDstAddr 点分十进制表示的通信地址串，格式类似如: xxx.xxx.xxx.xxx.xxx

  @retval 0 -- success, 
  @retval !0 -- failed,可能的错误代码如下:
	-	TBUS_ERR_ARG	
	-	TBUS_ERROR_NOT_INITIALIZED	
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_INVALID_ADDR_STRING	
	-	TBUS_ERROR_NO_CHANNEL
	-	TBUS_ERROR_NO_PEER_ADDRESS_MATCHED

  @see tbus_new
*/
TSF4G_API int tbus_connect_by_str ( IN const int a_iHandle, IN const char *a_szDstAddr ) ;


/**
  @brief 指定tbus通道的对端地址,接口功能与tbus_connect_by_str  相同

  @param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取
  @param[in] a_iDstAddr 对端地址，通过tbus_addr_aton()获取

  @retval 0 -- success, 
  @retval !0 -- failed,可能的错误代码如下:
  -	TBUS_ERR_ARG	
  -	TBUS_ERROR_NOT_INITIALIZED	
  -	TBUS_ERROR_INVALID_HANDLE
  -	TBUS_ERROR_INVALID_ADDR_STRING	
  -	TBUS_ERROR_NO_CHANNEL
  -	TBUS_ERROR_NO_PEER_ADDRESS_MATCHED

  @see tbus_new
  @see tbus_addr_aton
*/
TSF4G_API int tbus_connect ( IN const int a_iHandle, IN const TBUSADDR a_iDstAddr ) ;


/**
@brief 通道对端地址控制，禁止/开启 收发某个对端地址的通道数据

@param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取
@param[in] a_iDstAddr 需要控制的对端地址
@param[in] a_iMode 控制位，可用取以下两个值: 
	－	TBUS_MODE_DISABLE	禁止从此对端地址收发数据
	－	TBUS_MODE_ENABLE	开启从此对端地址收发数据
@param[in] a_iBatch 通道选项表示位，可用取以下两个值：
	- TBUS_PEER_CTRL_ONE	仅针对bus句柄所管理的第一个通道进行控制
	- TBUS_PEER_CTRL_ALL	针对bus句柄所管理的所有通道进行控制

@retval 0 -- success
@retval !0 -- failed,可能的错误代码如下:
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_NO_CHANNEL

@note
*/
TSF4G_API int tbus_peer_ctrl( IN const int a_iHandle, IN const TBUSADDR a_iDstAddr, IN const int a_iMode, IN const int a_iBatch ) ;

/**
@brief 根据通道地址获取指定通道的句柄

@param[in] a_iHandle tbus处理句柄，通过调用tbus_new() 获取
@param[out] a_ppstChannel 保存指定通道指针的指针
@param[in] a_iSrcAddr 通道的本地地址
@param[in] a_iDstAddr 通道的对端地址

@retval 0 -- success
@retval !0 -- failed,可能的错误代码如下:
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_NO_CHANNEL
@pre a_ppstchannel 不能为NULL
*/
TSF4G_API int tbus_get_channel( IN const int a_iHandle, OUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iSrcAddr, IN TBUSADDR a_iDstAddr) ;



/**
@brief 通过指定共享内存打开一个通信通道 此接口提过一个tbus系统的简易实现
*	@param[in,out] a_ppstChannel 保存打开的通道句柄的指针
*	@param[in] a_iLocalAddr 通道本地地址,地址值通过inet_aton计算出或为自定义整数
*	@param[in] a_iPeerAddr 通道对端地址 ，地址值通过inet_aton计算出或为自定义整数
*	@param[in] a_iShmkey 共享内存key
*	@param[in] a_iSize 通道数据队列的大小，读写队列的大小相同
*	@retval 0 success, 
*	@retval !0 failed，可能的错误代码如下:
	- TBUS_ERROR_INIT_LOGSYSTEM_FAILED
	- TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
*	
@note 通过此接口打开的通道句柄，必须调用tbus_detach_channel关闭
@pre a_ppstchannel 不能为NULL
@pre a_iSize 必须大于0
*	@see tbus_close_channel
*/
TSF4G_API int tbus_open_channel(INOUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iLocalAddr,
					  IN TBUSADDR a_iPeerAddr,IN int a_iShmkey, IN int a_iSize) ;

/**
@brief 通过指定共享内存打开一个通信通道 此接口提过一个tbus系统的简易实现
*	@param[in,out] a_ppstChannel 保存打开的通道句柄的指针
*	@param[in] a_pszLocalAddr 通道本地地址,地址格式为IPv4网络地址 点分十进制表示串,即x.x.x.x
*	@param[in] a_pszPeerAddr 通道对端地址 ，地址格式为IPv4网络地址 点分十进制表示串,即x.x.x.x
*	@param[in] a_iShmkey 共享内存key
*	@param[in] a_iSize 通道数据队列的大小，读写队列的大小相同
*	@retval 0 success, 
*	@retval !0 failed，可能的错误代码如下:
	- TBUS_ERROR_INIT_LOGSYSTEM_FAILED
	- TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
	-	TBUS_ERROR_INVALID_ADDR_STRING
*
@note 通过此接口打开的通道句柄，必须调用tbus_detach_channel关闭
*	@see tbus_close_channel
@pre a_ppstchannel 不能为NULL
@pre a_iSize 必须大于0
@pre  a_pszLocalAddr 不能为NULL
@pre a_pszPeerAddr 不能为NULL
*/
TSF4G_API int tbus_open_channel_by_str(INOUT LPTBUSCHANNEL *a_ppstChannel, IN const char *a_pszLocalAddr,
					  IN const char *a_pszPeerAddr,IN int a_iShmkey, IN int a_iSize) ;

/**
@brief 关闭一个通信通道
*	@param[in,out] a_ppstChannel 保存打开的通道句柄的指针
*	@retval 0 success, 
*	@retval !0 failed.
@note a_ppstChannel所指的通道句柄必须是调用tbus_open_channel打开的
*	@see tbus_open_channel
*/
TSF4G_API void tbus_close_channel(INOUT LPTBUSCHANNEL *a_ppstChannel) ;

/**  @} */ 








/** @defgroup TBUS_DATA_TRANSFER TBUS_数据传输
*  @{
*/

/**
  @brief 发送数据，可以实现只对一个通道发送数据，也可以对多个通道发送此数据，具体哪些
	通道需发送数据由源地址和目的地址决定
	
  @param[in] a_iHandle tbus句柄, 通过调用tbus_new()获取
  @param[in,out] a_piSrc 指定发送数据的源地址,通过tbus_addr_aton()转换而来：
	－	输入	输入指定数据的源地址，其值分为以下三种情况：
		－	*a_piSrc == 0	此为缺省值，即使用第一个匹配目的地址的通道来发送数据
		－	*a_piSrc == -1	此为广播地址，即所有能匹配目的的地址*a_piDst的通道都发送数据
		－	具体地址值		指定具体源地址，只有匹配此源地址的通道才可能会发送数据
	－	输出	得到具体使用通道的源地址                          
  @param[in,out] a_piDst 指定发送数据的目的地址，通过tbus_addr_aton()转换而来：
  －	输入	输入指定数据的目的地址，其值分为以下三种情况：
	－	*a_piDst == 0	此为缺省值，即使用第一个匹配源地址的通道来发送数据，如果此时*a_piSrc == 0则使用
		tbus句柄管理的第一个通道来发送数据
	－	*a_piDst == -1	此为广播地址，即所有能匹配源地址的通道都发送数据
	－	具体地址值		指定具体目的地址，只有匹配此目的地址的通道才可能会发送数据
  －	输出	得到具体使用通道的目的地址

  @param[in] a_pvData 保存发送数据的缓冲区支持
  @param[in] a_iDataLen 带发送数据的长度
  @param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
	－	TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
	－	TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @note	除非明确设置了TBUS_FLAG_KEEP_DYE_MSG标志位，tbus_send缺省不继承上一个消息的染色功能
  @note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
	 获取此染色消息的特征码，以便跟踪
  @see	tbus_get_dyedmsgid
  @see tbus_addr_aton
  @see tbus_new
  @pre a_piSrc 不能为NULL
  @pre a_piDst 不能为NULL
  @pre a_pvData 不能为NULL
  @pre a_iDataLen 必须大于0
*/
TSF4G_API int tbus_send (IN const int a_iHandle, INOUT TBUSADDR *a_piSrc, INOUT TBUSADDR *a_piDst,
	IN const void *a_pvData,	IN const size_t a_iDataLen,	IN const int a_iFlag) ;


/**
  @brief Send data which stored at multiple buffers

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in,out] a_piSrc -- see tbus_send()
  @param[in,out] a_piDst -- see tbus_send()
  @param[in] a_ptVector -- data vector buffers
  @param[in] a_iVecCnt -- data vector count
  @param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
	- TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
	- TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @note	除非明确设置了TBUS_FLAG_KEEP_DYE_MSG标志位，本接口缺省不继承上一个消息的染色功能
  @note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
  获取此染色消息的特征码，以便跟踪
    @see	tbus_get_dyedmsgid
	@see	tbus_send
	@pre a_piSrc 不能为NULL
	@pre a_piDst 不能为NULL
	@pre a_ptVector 不能为NULL
	@pre a_iVecCnt 必须大于0
*/
TSF4G_API int tbus_sendv (
	IN const int a_iHandle,
	INOUT TBUSADDR *a_piSrc,
	INOUT TBUSADDR *a_piDst,
	IN const struct iovec *a_ptVector,
	IN const int a_iVecCnt,
	IN const int a_iFlag
) ;


/**
@brief 从通道中读取出一个消息，可以实现只从某一个指定通道接收数据，也可以对尝试从从多个通道中读取数据，具体选择那个
	通道接收消息，有参数中指定的源，目的地址确定

@param[in] a_iHandle tbus句柄, 通过调用tbus_new()获取
@param[in,out] a_piSrc 指定数据的源地址，
	－	输入	输入指定数据的源地址，其值分为以下三种情况：
		－	*a_piSrc == 0	此为缺省值，即使用第一个匹配目的地址的通道来接收数据
		－	*a_piSrc == -1	此为广播地址，即所有能匹配目的的地址*a_piDst的通道都可以接收数据
		－	具体地址值		指定具体源地址，只有匹配此源地址的通道才可能会接收数据
	－	输出	得到实际读取数据的通道的源地址                          
@param[in,out] a_piDst 指定数据的目的地址，通过tbus_addr_aton()转换而来：
	－	输入	输入指定数据的目的地址，其值分为以下三种情况：
		－	*a_piDst == 0	此为缺省值，即使用第一个匹配源地址的通道来接收数据，如果此时*a_piSrc == 0则使用
			tbus句柄管理的第一个通道来接收数据
		－	*a_piDst == -1	此为广播地址，即所有能匹配源地址的通道都可以接收数据
		－	具体地址值		指定具体目的地址，只有匹配此目的地址的通道才可能会接收数据
	－	输出	得到实际读取数据的通道的目的地址

@param[in] a_pvData 保存接收数据的缓冲区指针
@param[in,out] a_piDataLen 带发送数据的长度
	-	输入	缓冲区最大可用长度
	－	输出	实际接收数据的长度
@param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
	-  TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
	-  TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @note	本接口缺省继承上一个消息的染色功能
  @note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
  获取此染色消息的特征码，以便跟踪
  @see	tbus_get_dyedmsgid
  @pre a_piSrc 不能为NULL
  @pre a_piDst 不能为NULL
  @pre a_pvData 不能为NULL
  @pre a_iDataLen 必须大于0
*/
TSF4G_API int tbus_recv (IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
	INOUT void *a_pvData,	INOUT size_t *a_piDataLen,	IN const int a_iFlag) ;


/**
@brief 从数据通道中获取出队列头部第一个消息的存储位置和此消息的长度信息，本接口实现只从某一个指定通道读取数据，也可以对尝试从从多个通道中读取数据，具体选择那个
通道接收消息，有参数中指定的源，目的地址确定

@param[in] a_iHandle tbus句柄, 通过调用tbus_new()获取
@param[in,out] a_piSrc 指定数据的源地址通过tbus_addr_aton()转换而来：
－	输入	输入指定数据的源地址，其值分为以下三种情况：
－	*a_piSrc == 0	此为缺省值，即使用第一个匹配目的地址的通道来接收数据
－	*a_piSrc == -1	此为广播地址，即所有能匹配目的的地址*a_piDst的通道都可以接收数据
－	具体地址值		指定具体源地址，只有匹配此源地址的通道才可能会接收数据
－	输出	得到具体使用通道的源地址                          
@param[in,out] a_piDst 指定数据的目的地址，通过tbus_addr_aton()转换而来：
－	输入	输入指定数据的目的地址，其值分为以下三种情况：
－	*a_piDst == 0	此为缺省值，即使用第一个匹配源地址的通道来接收数据，如果此时*a_piSrc == 0则使用
tbus句柄管理的第一个通道来接收数据
－	*a_piDst == -1	此为广播地址，即所有能匹配源地址的通道都可以接收数据
－	具体地址值		指定具体目的地址，只有匹配此目的地址的通道才可能会接收数据
－	输出	得到具体使用通道的目的地址

@param[in,out] a_ppvData 如果成功读取到消息，则此参数为用于保存数据存储地址的指针
@param[out] a_piDataLen 获取数据长度的指针
@param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
-  TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
-  TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

@retval 0 -- success, sent ok
@retval !0 -- failed

@note 本接口与tbus_recv的不同之处在于
	－	tbus_recv从数据队列中读取出数据后，会将数据从队列中移除；
	－ 但本接口仅仅只是	返回数据在队列中的存储位置和长度，并不会将数据从队列中移除，当此数据被处理后，请务必调用
		tbus_delete_msg将数据从队列中移除，否则tbus_peek_msg将总是读取出同一个消息
@note	本接口缺省继承上一个消息的染色功能
@note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
获取此染色消息的特征码，以便跟踪


@see	tbus_get_dyedmsgid
@see	tbus_recv
@see	tbus_delete_msg
@pre a_piSrc 不能为NULL
@pre a_piDst 不能为NULL
@pre a_ppvData 不能为NULL
@pre a_piDataLen 不能为NULL
*/
TSF4G_API int tbus_peek_msg(IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
						 INOUT const char **a_ppvData,	OUT size_t *a_piDataLen,	IN const int a_iFlag) ;


/**
@brief 将指定通道输入数据队列头部第一消息移除

@param[in] a_iHandle tbus句柄, 通过调用tbus_new()获取
@param[in] a_iSrc 指定数据的源地址                    
@param[in] a_iDst 指定数据的目的地址

@retval 0 -- success, sent ok
@retval !0 -- failed

@note	本接口与tbus_peek_msg接口配合使用从队列中读取数据进行处理，由于读取的数据直接保存在数据
	队列中，因此可以减少一次数据拷贝过程

@see	tbus_peek_msg
*/
TSF4G_API int tbus_delete_msg(IN const int a_iHandle,	IN TBUSADDR a_iSrc,	INOUT TBUSADDR a_iDst);



/**
  @brief 向前转发消息，其功能与tbus_send类似，其区别是tbus_forward会记录消息传输过程中的路由信息
	这样如果使用tbus_backward消息可以原路返回

	@param[in] a_iHandle tbus句柄, 通过调用tbus_new()获取
	@param[in,out] a_piSrc 指定发送数据的源地址,
		－	输入	输入指定数据的源地址，其值分为以下三种情况：
			－	*a_piSrc == 0	则将*a_piSrc修改为系统前一个接收到消息的目的地址，如果之前没有收到到过消息，
				则此次调用会失败
			－	*a_piSrc == -1	此为广播地址，即所有能匹配目的的地址*a_piDst的通道都发送数据
			－	具体地址值		指定具体源地址，只有匹配此源地址的通道才可能会发送数据
		－	输出	得到具体使用通道的源地址                          
	@param[in,out] a_piDst 指定发送数据的目的地址，
		－	输入	三种情况：
			－	*a_piDst == 0	取上一个接收消息中记录的路由地址信息做为目的地址
				tbus句柄管理的第一个通道来发送数据
			－	*a_piDst == -1	此为广播地址，即所有能匹配源地址的通道都发送数据
			－	具体地址值		指定具体目的地址，只有匹配此目的地址的通道才可能会发送数据
		－	输出	得到具体使用通道的目的地址

	@param[in] a_pvData 保存发送数据的缓冲区支持
	@param[in] a_iDataLen 带发送数据的长度
	@param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
		－	TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
		－	TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  
  @note	本接口缺省继承上一个消息的染色功能  
  @note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
  获取此染色消息的特征码，以便跟踪
  @see	tbus_get_dyedmsgid
  @see tbus_backward
  @pre a_piSrc 不能为NULL
  @pre a_piDst 不能为NULL
  @pre a_pvData 不能为NULL
  @pre a_iDataLen 必须大于0
*/
TSF4G_API int tbus_forward (IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
	IN const void *a_pvData,	IN const size_t a_iDataLen,	IN const int a_iFlag) ;




/**
@brief 延tbus头部记录的路由信心，向后转发消息

@param[in] a_iHandle tbus句柄, 通过调用tbus_new()获取
@param[in,out] a_piSrc 指定发送数据的源地址,通过tbus_addr_aton()转换而来：
	－	输入	输入指定数据的源地址，其值分为以下三种情况：
		－	*a_piSrc == 0	此为缺省值，使用前一个消息的目的地址作为源地址
		－	*a_piSrc == -1	此为广播地址，即所有能匹配目的的地址*a_piDst的通道都发送数据
		－	具体地址值		指定具体源地址，只有匹配此源地址的通道才可能会发送数据
	－	输出	得到具体使用通道的源地址                          
@param[in,out] a_piDst 指定发送数据的目的地址，通过tbus_addr_aton()转换而来：
	－	输入	通常设置为0即可，这样tbus会自动取上一个接收消息中记录的路由地址信息做为目的地址，其值分为以下三种情况：
		－	*a_piDst == 0	此为缺省值，自动取上一个接收消息中记录的路由地址信息做为目的地址
		－	*a_piDst == -1	此为广播地址，即所有能匹配源地址的通道都发送数据
		－	具体地址值		指定具体目的地址，只有匹配此目的地址的通道才可能会发送数据
	－	输出	得到具体使用通道的目的地址

@param[in] a_pvData 保存发送数据的缓冲区支持
@param[in] a_iDataLen 带发送数据的长度
@param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
	－	TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
	－	TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @pre \e 本接口要正常工作，必须之前以收到过一个通过tbus_forward发过来的数据包
  @note	本接口缺省继承上一个消息的染色功能
  @note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
  获取此染色消息的特征码，以便跟踪
  @pre a_piSrc 不能为NULL
  @pre a_piDst 不能为NULL
  @pre a_pvData 不能为NULL
  @pre a_iDataLen 必须大于0
*/
TSF4G_API int tbus_backward (IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
	IN const void *a_pvData,	IN const size_t a_iDataLen,	IN const int a_iFlag) ;


/**
@brief 在指定通道上发送一个数据包
*	@param[in] a_pstChannel 通道句柄
*	@param[in] a_pvData 保存数据的缓冲区首地址
*	@param[in] a_iDataLen 数据长度
*	@param[in] a_iFlag 控制字段
*	@retval 0 success, 
*	@retval !0 failed.
@pre a_pstChannel 不能为NULL
@pre a_pvData 不能为NULL
@pre a_iDataLen 必须大于0
*/
TSF4G_API int tbus_channel_send(IN LPTBUSCHANNEL a_pstChannel,IN const void *a_pvData,  
					  IN const int a_iDataLen, IN const int a_iFlag) ;

/**
@brief 在指定通道上接受一个数据包
*	@param[in] a_pstChannel 通道句柄
*	@param[in] a_pvData 保存数据的缓冲区首地址
*	@param[in,out] a_piDataLen 保存数据长度的指针
*	in	-	接受缓冲区最大大小
*	out -	接受数据的实际大小
*	@param[in] a_iFlag 控制字段
*	@retval 0 success, 
*	@retval !0 failed.
@pre a_pstChannel 不能为NULL
@pre a_pvData 不能为NULL
@pre a_iDataLen 必须大于0
*/
TSF4G_API int tbus_channel_recv(IN LPTBUSCHANNEL a_pstChannel, INOUT void *a_pvData,
					  INOUT int *a_piDataLen,
					  IN const int a_iFlag) ;

/**
@brief 从指定数据数据通道中获取出队列头部第一个消息的存储位置和此消息的长度信息

*	@param[in] a_pstChannel 通道句柄
@param[in,out] a_ppvData 如果成功读取到消息，则此参数为用于保存数据存储地址的指针
@param[out] a_piDataLen 获取数据长度的指针
@param[in] a_iFlag -- 控制字段，各控制标志支持'与（|）'方式结合使用，目前已定义的标志位有：
-  TBUS_FLAG_START_DYE_MSG	对发送消息进行染色跟踪
-  TBUS_FLAG_KEEP_DYE_MSG	继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色

@retval 0 -- success, sent ok
@retval !0 -- failed

@note 本接口与tbus_channel_recv的不同之处在于
－	tbus_channel_recv从数据队列中读取出数据后，会将数据从队列中移除；
－ 但本接口仅仅只是	返回数据在队列中的存储位置和长度，并不会将数据从队列中移除，当此数据被处理后，请务必调用
tbus_channel_delete_msg将数据从队列中移除，否则tbus_channel_peek_msg将总是读取出同一个消息
@note	本接口缺省继承上一个消息的染色功能
@note	如果设置了TBUS_FLAG_START_DYE_MSG标志位，调用者应该在本接口成功返回后调用tbus_get_dyedmsgid接口
获取此染色消息的特征码，以便跟踪


@see	tbus_get_dyedmsgid
@see	tbus_channel_delete_msg
@see	tbus_channel_recv
@pre a_ppvData 不能为NULL
@pre a_piDataLen 不能为NULL
*/
TSF4G_API int tbus_channel_peek_msg(IN LPTBUSCHANNEL a_pstChannel, INOUT const char **a_ppvData,	OUT int *a_piDataLen,
									IN const int a_iFlag) ;


/**
@brief 将指定通道输入数据队列头部第一消息移除

*	@param[in] a_pstChannel 通道句柄
@retval 0 -- success, sent ok
@retval !0 -- failed

@note	本接口与tbus_channel_peek_msg接口配合使用从队列中读取数据进行处理，由于读取的数据直接保存在数据
队列中，因此可以减少一次数据拷贝过程

@see	tbus_peek_msg
*/
TSF4G_API int tbus_channel_delete_msg(IN LPTBUSCHANNEL a_pstChannel);

/**
@brief 获取上一个染色消息的特征码，当tbus系统接口的上层应用对发送/接收的消息进行染色跟踪后，立即调用此接口
*	可以获取区别与其他染色消息的特数码，上层应用记录此特征码，就可保持对染色消息跟踪
@retval 染色消息的特征码
@note
*/
TSF4G_API unsigned int tbus_get_dyedmsgid();

/**
@brief 获取通道两端队列上剩余消息的字节总数
@param[in] a_hHandle tbus句柄
@param[in] a_iLocalAddr 此tbus通道相对于本端的地址
@param[in] a_iPeerAddr 此tbus通道相对于对端的地址
@param[in] a_piInFlux 获取输入通道(a_iLocalAddr <-- a_iPeerAddr)剩余消息字节数的指针
@param[in] a_piOutFlux 获取输出通道(a_iLocalAddr --> a_iPeerAddr)剩余消息字节数的指针
@retval 0 -- success
@retval !0 -- failed
@note
*/
TSF4G_API int tbus_get_channel_flux(IN int a_hHandle, IN TBUSADDR a_iLocalAddr, IN TBUSADDR a_iPeerAddr,
											 OUT int *a_piInFlux, OUT int *a_piOutFlux);

/** @} */ 



/** @defgroup TBUS_ROUTE_MNG TBUS_路由信息管理
*  @{
*/


/**
  @brief get current package's route information

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in,out] a_ptRouteVec -- vector to be stored route information, the first one is the latest route
  @param[in,out] a_piCnt -- route vector count, if get package route ok, this value would be set to real route count

  @retval 0 -- success, get route information ok
  @retval !0 -- failed

  @pre a_ptRouteVec 不能为NULL
  @pre a_piCnt 不能为NULL，且其保存的整数值必须大于0
*/
TSF4G_API int tbus_get_pkg_route ( IN const int a_iHandle, INOUT HEADROUTE *a_ptRouteVec, INOUT int *a_piCnt ) ;

/**
  @brief set current package's route information

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in] a_ptRouteVec -- vector to store route information, the first one is the latest route
  @param[in] a_iCnt -- route vector count, indicated how many routes should be set into package header

  @retval 0 -- success, set route information ok
  @retval !0 -- failed

  @pre a_ptRouteVec 不能为NULL
  @pre a_iCnt 其保存的整数值必须大于0
*/
TSF4G_API int tbus_set_pkg_route ( IN const int a_iHandle, IN const HEADROUTE *a_ptRouteVec, IN const int a_iCnt ) ;





/**
  @brief saved current package's header into internal buffer

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()

  @retval 0 -- success, saved ok
  @retval !0 -- failed

  @note this function should be invoked explicitly
*/
TSF4G_API int tbus_save_pkg_header ( IN const int a_iHandle ) ;


/**
  @brief restored package's header

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()

  @retval 0 -- success, restored ok
  @retval !0 -- failed

  @note this function should be invoked explicitly
*/
TSF4G_API int tbus_restore_pkg_header ( IN const int a_iHandle ) ;


/**
  @brief get current package's header and save it into external buffer

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in,out] a_pvBuffer -- value to be stored return header buffer
  @param[in,out] a_piLen -- buffer length as in value, and if get header successfully, this value would be set as real buffer length

  @retval 0 -- success, get ok
  @retval !0 -- failed

  @note this function should be invoked explicitly
  @pre a_pvBuffer 不能为NULL
  @pre a_piLen 不能为NULL，且其保存的整数值必须大于0
*/
TSF4G_API int tbus_get_pkg_header ( IN const int a_iHandle, INOUT void *a_pvBuffer, INOUT int *a_piLen ) ;


/**
  @brief set current package's header via specified external data

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in] a_pvBuffer -- value to store header buffer
  @param[in] a_iLen -- buffer length

  @retval 0 -- success, set ok
  @retval !0 -- failed

  @note this function should be invoked explicitly
  @pre a_pvBuffer 不能为NULL
  @pre a_iLen 其保存的整数值必须大于0
*/
TSF4G_API int tbus_set_pkg_header ( IN const int a_iHandle, IN const void *a_pvBuffer, IN const int a_iLen ) ;


/**
  @brief get destination address list via specified external data

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in,out] a_piDstList -- vector buffer to store destination list
  @param[in,out] a_piVecCnt --  vector count as in value, and if get list successfully, this value would be set as real destination address count

  @retval 0 -- success, get ok
  @retval TBUS_ERR_EXCEED -- failed, vector buffer could not contain all dst addresses
  @retval !0 -- failed

  @note this function is added at 2008/03/27 to meet FO2's needs
  @pre a_piDstList 不能为NULL
  @pre a_piVecCnt 不能为NULL，且其保存的整数值必须大于0
*/
TSF4G_API int tbus_get_dst_list ( IN const int a_iHandle, INOUT unsigned int *a_piDstList, INOUT int *a_piVecCnt ) ;

/** @} */ 



/** @defgroup TBUS_LOG TBUS_日志处理
*  @{
*/

/**
@brief set log category instance for tbus log system

@param[in] a_pstLogCat -- category instance of tbus log system


@note 
*/
TSF4G_API void tbus_set_logcat( IN LPTLOGCATEGORYINST a_pstLogCat) ;

/**
@brief set priority  of log category  used by tbus
@param[in] a_iPriority -- Priority  log category  used by tbus
*/
TSF4G_API void tbus_set_logpriority( IN int a_iPriority) ;



/**  @} */



#ifdef __cplusplus
}
#endif


#endif /**< _TBUS_H */

