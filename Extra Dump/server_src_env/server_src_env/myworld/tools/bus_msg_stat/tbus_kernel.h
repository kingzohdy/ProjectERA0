#ifndef TBUS_KERNEL_H
#define TBUS_KERNEL_H

#include "tdr/tdr.h"
#include "tbus/tbus.h"
#include "tbus_config_mng.h"
#include "tbus_head.h"



#define TBUS_VERSION                             	1 	/* Tbus�汾 */


#define HANDLE_COUNT                             	129 	/* Only support 128 handle max at one process,the first not use */
#define TUBS_MAX_CHANNEL_COUNT_PREHANDLE         	1024 	/* Only support 1024 address max at one handle */
#define TBUS_MAX_BIND_ADDR_NUM_PREHANDLE			32		/*��������Ͽ��԰󶨵��������*/

#define TBUS_HANDLE_FLAG_ENABLE			0x00000001	/**<��ʾ��ͨ������Ƿ����*/


#define	TBUS_CHANNEL_FLAG_ENABLE		0x00000001	/**<�����˴˱�ʶλ��ʾ��ͨ������*/
#define TBUS_CHANNEL_FLAG_SRC_ENABLE	0x00000002	/**<�����˴˱�ʶλ��ʾ��ͨ�����ص�ַ�Ѿ�����*/
#define TBUS_CHANNEL_FLAG_DST_ENABLE	0x00000004	/**<�����˴˱�ʶλ��ʾ��ͨ���Զ˵�ַ�Ѿ�����*/
#define TBUS_CHANNEL_FLAG_NOT_IN_GCIM		0X00000008	/**<�����˴˱�ʶλ��ʾ��ͨ���Ѿ���ȫ���������Ƴ�*/

#ifdef _DEBUG_
/**<���Ӽ�⵽ͨ�����������е���ͨ�����óɲ�����֮���ʱ��������λΪ�룬ĿǰΪ2��*/
#define TBUS_CHANNEL_SET_DISABLE_TIMEOUTGAP		20	

/**<���Ӽ�⵽ͨ�����������е���������ͨ��ͨ�����֮���ʱ��������λΪ�룬ĿǰΪ7��*/
#define TBUS_DISABLE_CHANNEL_CLEAR_DISABLE_TIMEOUTGAP		40	
#else
/**<���Ӽ�⵽ͨ�����������е���ͨ�����óɲ�����֮���ʱ��������λΪ�룬ĿǰΪ2��*/
#define TBUS_CHANNEL_SET_DISABLE_TIMEOUTGAP		172800	

/**<���Ӽ�⵽ͨ�����������е���������ͨ��ͨ�����֮���ʱ��������λΪ�룬ĿǰΪ7��*/
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


/*���ټ��㰴ָ����ʽ����ĳ���ֵ�ķ�������iLen����0x7FFFFFʱ�������*/
#define	TBUS_CALC_ALIGN_VALUE_BY_LEVEL(iLen, iAlignLevel)\
	{													 \
	  if (0 < (iAlignLevel))							\
	  {													\
		iLen = (int)(( ((tdr_longlong)iLen>>(iAlignLevel)) + 1) <<(iAlignLevel));\
	  }													\
	}
	

/*���㰴ָ����ʽ����ĳ���ֵ*/
#define	TBUS_CALC_ALIGN_VALUE(iLen, iAlign)\
	{										\
		int iTmp = (iLen) % (iAlign);		\
		if (0 != iTmp)						\
		{									\
			iLen += (iAlign) - iTmp;		\
		}									\
	}

/*���ݶ���ֵ���������ֵ*/
#define TBUS_CALC_ALIGN_LEVEL(iAlignLevel, iAlign) \
{												\
	int iTmp = 1;								\
	iAlignLevel = 0;							\
	for(; iTmp < iAlign; iAlignLevel++)			\
		iTmp <<=1;								\
}

/*������ֵ����Ϊ2������*/
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







/**�����ڴ���ͨ����Ϣ���в������ݽṹ
*/
struct tagChannelVar
{
	unsigned int dwSize;                          	/*   ���ݶ��еĿ����ɵ��ֽ��� */
	volatile unsigned int dwHead;							/*��Ϣ���е�ͷָ��*/						                       
	volatile unsigned int dwTail;							/*��Ϣ���е�βָ��*/
	int iSeq;								/*��Ϣ���к�*/                  
	unsigned int dwGet;								/*��Ϣ��ȡ��ʼλ��*/                      
	int iRecvSeq;                       	/*   ��ʾ��ǰ�յ������ݰ�����ˮ�ţ�����ֵ�����ֹ�����������Ҫ��ͬ������ */
	char chAuth;                          	/*   ֵΪ1ʱ��ʾ����ǰ���ڴ�channel����relayģ���������,Ŀǰ�Ѳ���ʹ�� */
	char chGStart;                        	/*   ֵΪ1ʱ��ʾ����ǰ��channel�Ѿ���ʼ��Զ˷������� */
	char chPStart;                        	/*   ֵΪ1ʱ��ʾ����ǰ��channel�Ѿ���ʼ���նԶ˷��͹��������� */
	char sReserve[256 - sizeof(int)*6 -3];					/*��֤tagChannelVar��sizeΪ256�ֽ�*/
};

#define  TBUS_CHANNEL_HEAD_RESERVE_NUM   8  /*ͨ��ͷ��Ԥ��������*/


/*�����ڴ���ͨ��ͷ����Ϣ�ṹ*/
struct tagChannelHead
{
	TBUSADDR astAddr[2];   /*����ͨ�����˵ĵ�ַ*/	            
	CHANNELVAR astQueueVar[2];    /*ͨ���Ķ���д������Ϣ���еĲ�����Ϣ*/   
	unsigned int dwAlignLevel;   /*ͨ�����ݶ��뷽ʽ,�����¼���Ƕ���ֵ�Ķ���ֵ,��������뷽ʽΪ8.��dwAlignLevelΪ3*/                   
	int iShmID;	/*id of share memory for channel*/    
	unsigned int dwMagic;		/*tbus ����ͨ����magic*/
	char sReserve[TBUS_CHANNEL_HEAD_RESERVE_NUM]; /*Ԥ��������*/      
};


/*busͨ��*/
struct tagTbusChannel
{
	unsigned int dwFlag;		/**<��־��Ϣλ*/
	int iRecvSide;              /*ͨ�����ݽ��նˣ����Ӧ��ַΪ�Զ˵ĵ�ַ��ֵΪ0��1*/
	int iSendSide;               /*ͨ�����ݷ��Ͷˣ����Ӧ��ַΪ�Զ˵ĵ�ַ����ֵΪ(iRecvSide+1)%2*/
	int iShmID;	/*id of share memory for channel*/  

	/*ͨ����Ϣͷ��������Աָ�����ڴ���ͨ��ͷ����pstHead->astAddr[iRecvSide]Ϊ���ص�ַ*/ 
	CHANNELHEAD *pstHead;  

	/*��д��Ϣ����, ����Աָ�����ڴ���ͨ�����������У�pszQueues[iRecvSide]Ϊ�����У�
	pszQueues[iSendSide]Ϊд����*/
	char *pszQueues[2];
	
	time_t tBeginDisable;		/*��ʼʧЧ����ʼʱ��*/
};




struct tagTbusHandle
{
	unsigned int dwFlag;                        	/* ��־λ */
	unsigned int dwChannelCnt;                       	/* �˾�������ͨ������ */
	TBUSCHANNEL *pastChannelSet[TUBS_MAX_CHANNEL_COUNT_PREHANDLE]; 	/*ͨ����Ϣ���� */
	int iRecvPos;                        	/*   new field, current received address position */
	unsigned int dwRecvPkgCnt;                    	/*   received packages count in one channel */
	TBUSHEAD stRecvHead;                          	/*   head of recv Pkg  package */
	TBUSHEAD stHeadBak;                          	/*  backup of the head of send Pkg  package */

	volatile unsigned int dwGCIMVersion;						/**< version of GCIM in share memory*/
	int iBindAddrNum;		/*�˾�����Ѿ��󶨵ĵ�ַ��*/
	TBUSADDR aiBindAddr[TBUS_MAX_BIND_ADDR_NUM_PREHANDLE];  /*�˾�����Ѿ�bind�ĵ�ַ��*/	
};

struct tagTbusGlobal
{
	unsigned int dwInited;                        	/*   ȫ����Ϣ�ṹ�Ƿ� */
	pthread_mutex_t tMutex;						/**<�����д�����л�����ʱ�ȫ������ȥ��ʵ��һЩ�̰߳�ȫ����*/
	int	iBussId;									/**<ҵ��ID*/
	unsigned int dwDyedMsgID;                     	/*   Ⱦɫ��Ϣ��ID */	
	unsigned int dwHandleCnt;                     	/*   new field, tbus handle count */

	TBUSHANDLE *pastTbusHandle[HANDLE_COUNT];    	/*   new field, tbus interface */
	volatile unsigned int dwGCIMVersion;						/**< version of GCIM in share memory*/
	TBUSSHMGCIM *pstGCIM;                     	/*   global channel info map */
	LPTDRMETA	pstHeadMeta;					/*meta of tbus head*/

	TBUSHEAD stCounterfeitPkg;		/*Ϊ����tbus������������ͨ��β����ӵ�α�����ݰ�*/
	int iCounterfeitPkgLen;		/*α�����ݰ��ĵĳ���*/	
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


/*������ݶ�����ز����Ƿ���Ҫ����*/
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
