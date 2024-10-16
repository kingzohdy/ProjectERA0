#ifndef TBUS_KERNEL_H
#define TBUS_KERNEL_H

#include "tdr/tdr.h"
#include "tbus/tbus.h"
#include "tbus_config_mng.h"
#include "tbus_head.h"



#define TBUS_VERSION                             	1 	/* Tbus版本 */


#define HANDLE_COUNT                             	129 	/* Only support 128 handle max at one process,the first not use */
#define TUBS_MAX_CHANNEL_COUNT_PREHANDLE         	1024 	/* Only support 1024 address max at one handle */
#define TBUS_MAX_BIND_ADDR_NUM_PREHANDLE			32		/*单个句柄上可以绑定的最大句柄数*/

#define TBUS_HANDLE_FLAG_ENABLE			0x00000001	/**<表示此通道句柄是否可用*/


#define	TBUS_CHANNEL_FLAG_ENABLE		0x00000001	/**<设置了此标识位表示此通道可用*/
#define TBUS_CHANNEL_FLAG_SRC_ENABLE	0x00000002	/**<设置了此标识位表示此通道本地地址已经激活*/
#define TBUS_CHANNEL_FLAG_DST_ENABLE	0x00000004	/**<设置了此标识位表示此通道对端地址已经激活*/
#define TBUS_CHANNEL_FLAG_NOT_IN_GCIM		0X00000008	/**<设置了此标识位表示此通道已经从全局配置中移除*/

#ifdef _DEBUG_
/**<当从检测到通道不在配置中到将通道设置成不可用之间的时间间隔，单位为秒，目前为2天*/
#define TBUS_CHANNEL_SET_DISABLE_TIMEOUTGAP		20	

/**<当从检测到通道不在配置中到将不可用通道通道清除之间的时间间隔，单位为秒，目前为7天*/
#define TBUS_DISABLE_CHANNEL_CLEAR_DISABLE_TIMEOUTGAP		40	
#else
/**<当从检测到通道不在配置中到将通道设置成不可用之间的时间间隔，单位为秒，目前为2天*/
#define TBUS_CHANNEL_SET_DISABLE_TIMEOUTGAP		172800	

/**<当从检测到通道不在配置中到将不可用通道通道清除之间的时间间隔，单位为秒，目前为7天*/
#define TBUS_DISABLE_CHANNEL_CLEAR_DISABLE_TIMEOUTGAP		604800	
#endif


#define TBUS_INVALID_HANDLE	-1

#define TBUS_RELAY_DISABLE                       	0 	/*  */
#define TBUS_RELAY_ENABLE                        	1 	/*  */

#define TBUS_ADDR_DISABLE                        	0 	/*  */
#define TBUS_ADDR_ENABLE                         	1 	/*  */
#define TBUS_ADDR_SRC_ENABLE                        	2 	/*  */
#define TBUS_ADDR_DST_ENABLE                        	4 	/*  */



#define TBUS_MODULE_NOT_INITED                   	0 	/*  */
#define TBUS_MODULE_INITED                       	1 	/*  */

#define TBUS_CHANNEL_SIDE_UP                     	1 	/*  */
#define TBUS_CHANNEL_SIDE_DOWN                   	2 	/*  */

#define TBUS_IDX_INVALID                         	-1 	/*  */
#define TBUS_RECV_POS_INVALID                    	-1 	/*  */


#define TBUS_MAX_NOMSG_CNT                       	3 	/*  */
#define TBUS_DEFAULT_SENT_CNT                    	10 	/*  */
#define TBUS_STEP_SENT_CNT                       	5 	/* how sent count added at one time */
#define TBUS_MAX_SENT_CNT                        	40 	/*  */
#define TBUS_MAX_RECV_CNT                        	10 	/*  */

#ifdef TSF4G_THREAD_SAFE
#define TBUS_MUTEX_LOCK(tMutex)		pthread_mutex_lock(&(tMutex))
#define TBUS_MUTEX_UNLOCK(tMutex)		pthread_mutex_unlock(&(tMutex))
#else
#define TBUS_MUTEX_LOCK(tMutex)     
#define TBUS_MUTEX_UNLOCK(tMutex)		
#endif /*#ifdef TSF4G_THREAD_SAFE*/


/*快速计算按指定方式对齐的长度值的方法，当iLen大于0x7FFFFF时可以溢出*/
#define	TBUS_CALC_ALIGN_VALUE_BY_LEVEL(iLen, iAlignLevel)\
	{													 \
	  if (0 < (iAlignLevel))							\
	  {													\
		iLen = (int)(( ((tdr_longlong)iLen>>(iAlignLevel)) + 1) <<(iAlignLevel));\
	  }													\
	}
	

/*计算按指定方式对齐的长度值*/
#define	TBUS_CALC_ALIGN_VALUE(iLen, iAlign)\
	{										\
		int iTmp = (iLen) % (iAlign);		\
		if (0 != iTmp)						\
		{									\
			iLen += (iAlign) - iTmp;		\
		}									\
	}

/*根据对其值计算其对数值*/
#define TBUS_CALC_ALIGN_LEVEL(iAlignLevel, iAlign) \
{												\
	int iTmp = 1;								\
	iAlignLevel = 0;							\
	for(; iTmp < iAlign; iAlignLevel++)			\
		iTmp <<=1;								\
}

/*将对其值设置为2的幂数*/
#define TBUS_NORMALIZE_ALIGN(iAlign)            \
{												\
	int i;										\
	if (0 >= (iAlign))							\
	{											\
		iAlign = TBUS_DEFAULT_CHANNEL_DATA_ALIGN;\
	}else if (TBUS_MAX_CHANNEL_DATA_ALIGN < (iAlign))\
	{											\
		iAlign = TBUS_MAX_CHANNEL_DATA_ALIGN;	\
	}											\
	for(i = 1; i < (iAlign); i <<=1);			\
	iAlign = i;									\
}

#ifdef __cplusplus
extern	"C" 
{
#endif


struct tagTbusHandle;
typedef struct tagTbusHandle                        	TBUSHANDLE;
typedef struct tagTbusHandle                        	*LPTBUSHANDLE;

typedef struct tagTbusChannel                          	TBUSCHANNEL;


typedef struct tagTbusGlobal                       	TBUSGLOBAL;
typedef struct tagTbusGlobal                       	*LPTBUSGLOBAL;

typedef struct tagChannelVar                       	CHANNELVAR;
typedef struct tagChannelVar                       	*LPCHANNELVAR;

typedef struct tagTbusPkg                       	TBUSPKG;
typedef struct tagTbusPkg                       	*LPTBUSPKG;

struct tagTbusPkg
{
	TBUSHEAD stHead;                         
	char *szData;      
};







/**共享内存中通道消息队列参数数据结构
*/
struct tagChannelVar
{
	unsigned int dwSize;                          	/*   数据队列的可容纳的字节数 */
	volatile unsigned int dwHead;							/*消息队列的头指针*/						                       
	volatile unsigned int dwTail;							/*消息队列的尾指针*/
	int iSeq;								/*消息序列号*/                  
	unsigned int dwGet;								/*消息读取起始位置*/                      
	int iRecvSeq;                       	/*   表示当前收到的数据包的流水号，该数值在握手过程中起到了重要的同步作用 */
	char chAuth;                          	/*   值为1时表示，当前的内存channel是由relay模块来处理的,目前已不再使用 */
	char chGStart;                        	/*   值为1时表示，当前的channel已经开始向对端发送数据 */
	char chPStart;                        	/*   值为1时表示，当前的channel已经开始接收对端发送过来的数据 */
	char sReserve[256 - sizeof(int)*6 -3];					/*保证tagChannelVar的size为256字节*/
};

#define  TBUS_CHANNEL_HEAD_RESERVE_NUM   8  /*通道头部预留数据区*/


/*共享内存中通道头部信息结构*/
struct tagChannelHead
{
	TBUSADDR astAddr[2];   /*保存通道两端的地址*/	            
	CHANNELVAR astQueueVar[2];    /*通道的读，写两个消息队列的参数信息*/   
	unsigned int dwAlignLevel;   /*通道数据对齐方式,这里记录的是对齐值的对数值,即如果对齐方式为8.则dwAlignLevel为3*/                   
	int iShmID;	/*id of share memory for channel*/    
	unsigned int dwMagic;		/*tbus 数据通道的magic*/
	char sReserve[TBUS_CHANNEL_HEAD_RESERVE_NUM]; /*预留数据区*/      
};


/*bus通道*/
struct tagTbusChannel
{
	unsigned int dwFlag;		/**<标志信息位*/
	int iRecvSide;              /*通道数据接收端，其对应地址为对端的地址其值为0或1*/
	int iSendSide;               /*通道数据发送端，其对应地址为对端的地址，其值为(iRecvSide+1)%2*/
	int iShmID;	/*id of share memory for channel*/  

	/*通道信息头部，本成员指向共享内存中通道头部，pstHead->astAddr[iRecvSide]为本地地址*/ 
	CHANNELHEAD *pstHead;  

	/*读写消息队列, 本成员指向共享内存中通道的两个队列，pszQueues[iRecvSide]为读队列，
	pszQueues[iSendSide]为写队列*/
	char *pszQueues[2];
	
	time_t tBeginDisable;		/*开始失效的起始时间*/
};




struct tagTbusHandle
{
	unsigned int dwFlag;                        	/* 标志位 */
	unsigned int dwChannelCnt;                       	/* 此句柄管理的通道数据 */
	TBUSCHANNEL *pastChannelSet[TUBS_MAX_CHANNEL_COUNT_PREHANDLE]; 	/*通道信息数据 */
	int iRecvPos;                        	/*   new field, current received address position */
	unsigned int dwRecvPkgCnt;                    	/*   received packages count in one channel */
	TBUSHEAD stRecvHead;                          	/*   head of recv Pkg  package */
	TBUSHEAD stHeadBak;                          	/*  backup of the head of send Pkg  package */

	volatile unsigned int dwGCIMVersion;						/**< version of GCIM in share memory*/
	int iBindAddrNum;		/*此句柄上已经绑定的地址数*/
	TBUSADDR aiBindAddr[TBUS_MAX_BIND_ADDR_NUM_PREHANDLE];  /*此句柄上已经bind的地址数*/	
};

struct tagTbusGlobal
{
	unsigned int dwInited;                        	/*   全局信息结构是否 */
	pthread_mutex_t tMutex;						/**<互斥读写锁进行互斥访问本全局数据去和实现一些线程安全操作*/
	int	iBussId;									/**<业务ID*/
	unsigned int dwDyedMsgID;                     	/*   染色消息的ID */	
	unsigned int dwHandleCnt;                     	/*   new field, tbus handle count */

	TBUSHANDLE *pastTbusHandle[HANDLE_COUNT];    	/*   new field, tbus interface */
	volatile unsigned int dwGCIMVersion;						/**< version of GCIM in share memory*/
	TBUSSHMGCIM *pstGCIM;                     	/*   global channel info map */
	LPTDRMETA	pstHeadMeta;					/*meta of tbus head*/

	TBUSHEAD stCounterfeitPkg;		/*为方便tbus处理，而在数据通道尾部添加的伪造数据包*/
	int iCounterfeitPkgLen;		/*伪造数据包的的长度*/	
};

#define TBUS_SET_LAST_ROUTE(stHead, a_iSrc) \
{												\
	if (0 == (stHead).stExtHead.stDataHead.bRoute) \
	{													\
		(stHead).stExtHead.stDataHead.bRoute = 1;	\
		(stHead).bFlag = TBUS_HEAD_FLAG_WITH_ROUTE;	\
	}												\
	(stHead).stExtHead.stDataHead.routeAddr[(stHead).stExtHead.stDataHead.bRoute -1] = a_iSrc;\
}

#define TBUS_IS_NON_SPECIFIED_ADDRESS(addr)  ((TBUS_ADDR_DEFAULT == (addr)) || (TBUS_ADDR_ALL == (addr)))

#define TBUS_COMPRESS_HEAD_LEN_VERSION	0	/*In this tbus head version, the net length of tbus head is divide 4 */


#define TBUS_HEAD_CODE_BUFFER_SIZE		1024	/*size of buffer to coding tbus head */

/*offset of head.headlen in network, pack by one byte */
#define TBUS_HEAD_LEN_NET_OFFSET	offsetof(TBUSHEAD, bHeadLen)

#define TBUS_MIN_NET_LEN_TO_GET_HEADLEN	 (int)(TBUS_HEAD_LEN_NET_OFFSET + TDR_SIZEOF_BYTE)

/*offset of head.seq in network msg, pack by one byte*/
#define TBUS_HEAD_SEQ_NET_OFFSET	offsetof(TBUSHEAD, iSeq)
#define TBUS_MIN_NET_LEN_TO_GET_HEADSEQ	 (int)(TBUS_HEAD_SEQ_NET_OFFSET + TDR_SIZEOF_INT)

#define TBUS_MIN_HEAD_VERSION	1

#define TBUS_HEAD_EXTHEAD_DATA_OFFSET	offsetof(TBUSHEAD, stExtHead)

#define TBUS_CHANNEL_VAR_PUSH(a_ptChannel)  (CHANNELVAR *)&((a_ptChannel)->pstHead->astQueueVar[(a_ptChannel)->iSendSide])

#define TBUS_CHANNEL_VAR_GET(a_ptChannel)  (CHANNELVAR *)&((a_ptChannel)->pstHead->astQueueVar[(a_ptChannel)->iRecvSide])

#define TBUS_CHANNEL_QUEUE_GET(a_ptChannel)	(a_ptChannel)->pszQueues[(a_ptChannel)->iRecvSide]

#define TBUS_CHANNEL_QUEUE_PUSH(a_ptChannel)	(a_ptChannel)->pszQueues[(a_ptChannel)->iSendSide]

#define TBUS_CHANNEL_LOCAL_ADDR(a_ptChannel)  ((a_ptChannel)->pstHead->astAddr[(a_ptChannel)->iRecvSide])

#define TBUS_CHANNEL_PEER_ADDR(a_ptChannel)  ((a_ptChannel)->pstHead->astAddr[(a_ptChannel)->iSendSide])


#define TBUS_CHANNEL_IS_ENABLE(pstChannel)     ((pstChannel)->dwFlag & TBUS_CHANNEL_FLAG_ENABLE)
#define TBUS_CHANNEL_SET_ENABLE(pstChannel)       ((pstChannel)->dwFlag |= TBUS_CHANNEL_FLAG_ENABLE)
#define TBUS_CHANNEL_CLR_ENABLE(pstChannel)		((pstChannel)->dwFlag &= ~TBUS_CHANNEL_FLAG_ENABLE)

#define TBUS_CHANNEL_IS_NOT_IN_GCIM(pstChannel)     ((pstChannel)->dwFlag & TBUS_CHANNEL_FLAG_NOT_IN_GCIM)
#define TBUS_CHANNEL_SET_NOT_IN_GCIM(pstChannel)       ((pstChannel)->dwFlag |= TBUS_CHANNEL_FLAG_NOT_IN_GCIM)
#define TBUS_CHANNEL_CLR_NOT_IN_GCIM(pstChannel)		((pstChannel)->dwFlag &= ~TBUS_CHANNEL_FLAG_NOT_IN_GCIM)


/*检查数据队列相关参数是否需要调整*/
extern TBUSGLOBAL g_stBusGlobal;
#define TBUS_CHECK_QUEUE_HEAD_VAR(a_pszQueue, a_iSize, a_iHead) \
{															 \
	int iTailLen = (a_iSize) - (a_iHead);					 \
	if (iTailLen <= g_stBusGlobal.iCounterfeitPkgLen)		 \
	{														 \
		tbus_log(TLOG_PRIORITY_INFO, "the data taillen(%d) is not bigger than the counterfeit pkglen(%d)," \
			 "so change head pointer to zero");						\
		 a_iHead = 0;												\
	}else															\
	{																\
		LPTBUSHEAD pstTmpHead = (LPTBUSHEAD)((a_pszQueue) + (a_iHead));\
		if (pstTmpHead->bFlag == TBUS_HEAD_FLAG_COUNTERFEIT_DATA)		\
		{																\
			tbus_log(TLOG_PRIORITY_INFO, "the pkg in the data tail(len:%d) is a counterfeit pkg,"\
			"so change head pointer to zero");							\
			a_iHead = 0;													\
		}/*if (a_pstHead->bFlag == TBUS_HEAD_FLAG_COUNTERFEIT_DATA)*/	\
	}/*if (iTailLen <= g_stBusGlobal.iCounterfeitPkgLen)*/				\
}


int tbus_encode_head(IN LPTBUSHEAD a_pstHead, IN char *a_pszNet, INOUT int *a_piLen, IN int a_iVersion);

int tbus_decode_head(OUT LPTBUSHEAD a_pstHead, IN char *a_pszNet, IN int a_iLen, IN int a_iVersion);

int tbus_get_pkghead(OUT LPTBUSHEAD a_pstHead, const char* a_pszQueue, unsigned int dwSize,
					 INOUT unsigned int *a_pdwHead, unsigned int dwTail);



#ifdef __cplusplus
}
#endif
#endif /*TBUS_KERNEL_H*/
