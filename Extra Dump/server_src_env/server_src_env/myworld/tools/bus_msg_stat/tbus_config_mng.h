#ifndef TBUS_CONFIG_MNG_H
#define TBUS_CONFIG_MNG_H

#include "pal/pal.h"
#include "tbus_addr_templet.h"
#include "tbus/tbus_macros.h"
#include "tbus_comm.h"
#include "tbus_desc.h"

#define TBUS_SHM_GCIM_HEAD_RESERV_NUM                 	8 	/* reserve*/
#define TBUS_SHM_GRM_HEAD_RESERV_NUM                 	8 	/* reserve*/

#define TBUS_CHANNEL_RESERVER_NUM						4 

#define TBUS_DEFAULT_ADDRESS_TEMPLET	"8.8.8.8"  /*tbus系统缺省地址模板*/

/**<timeout for attaching shm by milli seconds*/
#define TBUS_ATTACH_SHM_TIMEOUT						30000	

/*timeout for lock by milli seconds*/
#define TBUS_RWLOCK_TIMEOUT			30000

/**<sleep time by micro seconds per loop*/
#define TBUS_SLEEP_PRELOOP						100	


#define TBUS_CHANNEL_SIDE_INDEX_1				0	/*通道某一端的索引值*/
#define TBUS_CHANNEL_SIDE_INDEX_2				1	/*通道某一端的索引值*/

#define TBUS_MAX_PATH_LEN					1024    /*path路径长度*/
#define TBUS_APPDATA_PATH					"SystemRoot" /*appdata 环境变量*/
#define TBUS_SHM_MMAP_PATH					"tbus" /*appdata 环境变量*/

#ifdef WIN32
#define TBUS_DEFAULT_MMAP_FILE_DIR			"C:"
#else
#define TBUS_DEFAULT_MMAP_FILE_DIR			"/tmp"
#endif

#define TBUS_GCIM_SHM_ACCESS	0664	//GCIM共享内存的访问权限
#define TBUS_GRM_SHM_ACCESS	0664	//GRM共享内存的访问权限

#define TBUS_GCIM_CHANNEL_FLAG_NONE                 0X00000000 	
#define TBUS_GCIM_CHANNEL_FLAG_ENABLE               0X00000001 	/*此配置是否有效*/
#define TBUS_GCIM_CHANNEL_IS_ENABLE(pstChannel)     ((pstChannel)->dwFlag & TBUS_GCIM_CHANNEL_FLAG_ENABLE)
#define TBUS_GCIM_CHANNEL_SET_ENABLE(pstChannel)       ((pstChannel)->dwFlag |= TBUS_GCIM_CHANNEL_FLAG_ENABLE)
#define TBUS_GCIM_CHANNEL_CLR_ENABLE(pstChannel)		((pstChannel)->dwFlag &= ~TBUS_GCIM_CHANNEL_FLAG_ENABLE)


#ifdef __cplusplus
extern "C" 
{
#endif


typedef struct tagTbusShmGRMHead                        	TBUSSHMGRMHEAD;
typedef struct tagTbusShmGRMHead                        	*LPTBUSSHMGRMHEAD;

typedef struct tagTbusShmGCIMHead TBUSSHMGCIMHEAD;
typedef struct tagTbusShmGCIMHead *LPTBUSSHMGCIMHEAD;

typedef struct tagTbusShmGCIM	TBUSSHMGCIM;
typedef struct tagTbusShmGCIM	*LPTBUSSHMGCIM;

typedef struct tagTbusShmRelayCnf		TBUSSHMRELAYCNF;
typedef struct tagTbusShmRelayCnf	*LPTBUSSHMRELAYCNF;




typedef struct tagShmChannelCnf                         	TBUSSHMCHANNELCNF;
typedef struct tagShmChannelCnf                         	*LPTBUSSHMCHANNELCNF;

typedef struct tagChannelHead                      	CHANNELHEAD;
typedef struct tagChannelHead                      	*LPCHANNELHEAD;

typedef struct tagTbusShmGRM  TBUSSHMGRM;
typedef struct tagTbusShmGRM *LPTBUSSHMGRM;

struct tagTbusShmGRMHead
{
	pthread_rwlock_t stRWLock;						/**< TBUS配置共享内存中，通过互斥读写锁进行互斥访问*/
	volatile unsigned int dwVersion;                       	/*   当shm中的数据更新后，需要更新该数值，以方便应用模块进行进行重新加载 */
	unsigned int dwShmKey;                        	/*   GCIM共享内存key，主要做校验用 */
	unsigned int dwShmSize;                       	/*   共享内存大小 */
	unsigned int dwMaxCnt;                        	/*   共享内存中最大可容纳的channel信息的slot数 */
	unsigned int dwUsedCnt;                       	/*   共享内存已用的slot数 */
	unsigned int dwCreateTime;                    	/*  共享内存创建时间 */
	unsigned int dwLastStamp;                     	/*  共享内存最后修改时间戳 */
	unsigned int dwBusiID;	/**<使用tbus系统的业务的ID*/
	TBUSADDRTEMPLET	stAddrTemplet;					/**< 本bus系统所使用的点分十进制地址模板*/	
	unsigned int dwCRC;                           	/*  共享内存检验码 */
	unsigned int reserve[TBUS_SHM_GRM_HEAD_RESERV_NUM]; 	/*预留数据区*/
};

struct tagTbusShmGCIMHead
{
	volatile unsigned int dwVersion;                       	/*   当shm中的数据更新后，需要更新该数值，以方便应用模块进行进行重新加载 */
	unsigned int dwShmKey;                        	/*   GCIM共享内存key，主要做校验用 */
	unsigned int dwShmSize;                       	/*   共享内存大小 */
	unsigned int dwMaxCnt;                        	/*   共享内存中最大可容纳的channel信息的slot数 */
	unsigned int dwUsedCnt;                       	/*   共享内存已用的slot数 */
	unsigned int dwCreateTime;                    	/*  共享内存创建时间 */
	unsigned int dwLastStamp;                     	/*  共享内存最后修改时间戳 */
	unsigned int dwBusiID;	/**<使用tbus系统的业务的ID*/
	unsigned int dwCRC;                           	/*  共享内存检验码 */
	int iAlign;				/*通道数据对齐方式*/
	int iAlignLevel;				/*通道数据对齐值的对数值,其值通过iAlign计算出来*/
	pthread_rwlock_t stRWLock;						/**< TBUS配置共享内存中，通过互斥读写锁进行互斥访问*/
	TBUSADDRTEMPLET	stAddrTemplet;					/**< 本bus系统所使用的点分十进制地址模板*/	
	unsigned int reserve[TBUS_SHM_GCIM_HEAD_RESERV_NUM]; 	/*预留数据区*/
};

struct tagShmChannelCnf
{
	unsigned int dwFlag;                        	/*   bit set: 0001 -- route valid */
	unsigned int dwPriority;                     
	int iShmID;	/*id of share memory for channel*/                          
	TBUSADDR astAddrs[2];                        	/* 通道两端的地址  original ip format: xxx.xxx.xxx.xxx, converted by tbus_addr_aton() */
          
	unsigned int dwRecvSize;                      	/*对于astAddrs[0]来说其接收队列的大小, default is 2M */
	unsigned int dwSendSize;                      	/*对于astAddrs[0]来说其发送队列的大小, default is 2M */
                   
               
	time_t dwCTime;                        
	time_t dwInvalidTime;	/*此通道被设置为失效的起始时间*/ 
	int aiReserver[TBUS_CHANNEL_RESERVER_NUM]; 
};

/** 共享内存中的通道信息配置表
*/
struct tagTbusShmGCIM 
{
	TBUSSHMGCIMHEAD	stHead;
	TBUSSHMCHANNELCNF astChannels[TBUS_MAX_CHANNEL_NUM_PREHOST]; /*具体通道的配置信息*/
};



struct tagTbusShmRelayCnf
{
	unsigned int dwFlag;                        	/*   bit set: 0001 -- relay node valid */
	TBUSADDR dwAddr; 	 /*  Message sent to this addr would be forwarded via network */
	unsigned int dwPriority;                     
	char szMConn[TBUS_MAX_HOST_URL_LEN];  	/*   master connection information */
	char szSConn[TBUS_MAX_HOST_URL_LEN];  	/*   slave connection information */
	unsigned int dwStrategy;                      	/*   strategy between master connection and slave connection */
};


/*共享内存中的通道信息配置表*/
struct tagTbusShmGRM 
{
	TBUSSHMGRMHEAD	stHead;
	TBUSSHMRELAYCNF astRelays[TBUS_MAX_RELAY_NUM_PREHOST]; /*具体中转通道的配置信息*/
};

/**创建GCIM共享内存 
*@param[in,out] ppstGCIM 保存GCIM共享内存结构指针的指针
*@param[in] a_pszShmkey 生成共享内存key所需的信息串
*@param[in]	a_iBussId 业务id
*@param[in]	a_iShmSize 指定共享内存大小，对于加载已存在的共享内存此参数可以设置为0
*@param[in]	iSHmFlag 控制标志位，用于指示shm访问权限，是否需要创建等信息
*@retval 0 成功
*@retval <0 失败
*/
int tbus_create_gcimshm(INOUT LPTBUSSHMGCIM *a_ppstGCIM, IN const char *a_pszShmkey, 
							   IN int a_iBussId, IN unsigned int a_iShmSize);

/**加载GCIM共享内存 
*@param[in,out] ppstGCIM 保存GCIM共享内存结构指针的指针
*@param[in] a_pszShmkey 生成共享内存key所需的信息串
*@param[in]	a_iBussId 业务id
*@param[in]	a_iShmSize 指定共享内存大小，对于加载已存在的共享内存此参数可以设置为0
*@param[in] a_iTimeout If the gcim share memory do not exist, try to attach again, until time out
*@retval 0 成功
*@retval <0 失败
*/
int tbus_get_gcimshm(INOUT LPTBUSSHMGCIM *a_ppstGCIM, IN const char *a_pszShmkey, 
					 IN int a_iBussId, IN unsigned int a_iShmSize, IN int a_iTimeout);


/**将GCIM配置写道共享内存中
*@param[in] a_pstShmGCIM 指向GCIM共享内存数据结构指针
*@param[in] a_pstGCIM GCIM配置信息结构指针
*@retval 0 成功
*@retval <0 失败
*/
int tbus_set_gcim(IN LPTBUSSHMGCIM a_pstShmGCIM, IN LPTBUSGCIM a_pstGCIM);

void tbus_detach_gcim(IN LPTBUSSHMGCIM a_pstShmGCIM);

/**删除指定索引的通道配置 
*@param[in] a_pstShmGCIM 指向GCIM共享内存数据结构指针
*@param[in] a_idx 指定通道在GCIM中的索引
*@retval 0 成功
*@retval <0 失败
*/
int tbus_delete_channel_by_index(IN LPTBUSSHMGCIM a_pstShmGCIM, IN int a_idx);

/** Register business id to Senderd-Agent system
*/
int tbus_register_bussid(IN int a_iBussId);

void tbus_set_shmgcimheader_stamp(LPTBUSSHMGCIMHEAD pstHeader);

/**检查路由表共享内存的检验码,如果教研成功返回0,否则返回非零值 
*/
int tbus_check_shmgcimheader_stamp(LPTBUSSHMGCIMHEAD pstHeader);

void tbus_set_shmgrmheader_stamp(LPTBUSSHMGRMHEAD pstHeader);


int tbus_check_shmgrmheader_stamp(LPTBUSSHMGRMHEAD pstHeader);

/**创建GRM共享内存 
*@param[in,out] a_ppstGRM 保存GRM共享内存结构指针的指针
*@param[in] a_pszShmkey 生成共享内存key所需的信息串
*@param[in]	a_iBussId 业务id
*@param[in]	a_iShmSize 指定共享内存大小，对于加载已存在的共享内存此参数可以设置为0
*@retval 0 成功
*@retval <0 失败
*/
int tbus_create_grmshm(INOUT LPTBUSSHMGRM *a_ppstGRM, IN const char *a_pszShmkey, 
						IN int a_iBussId, IN unsigned int a_iShmSize);

/**加载GRM共享内存 
*@param[in,out] a_ppstGRM 保存GRM共享内存结构指针的指针
*@param[in] a_pszShmkey 生成共享内存key所需的信息串
*@param[in]	a_iBussId 业务id
*@param[in]	a_iShmSize 指定共享内存大小，对于加载已存在的共享内存此参数可以设置为0
*@retval 0 成功
*@retval <0 失败
*/
int tbus_get_grmshm(INOUT LPTBUSSHMGRM *a_ppstGRM, IN const char *a_pszShmkey, 
					 IN int a_iBussId, IN unsigned int a_iShmSize);


void tbus_detach_grmshm(IN LPTBUSSHMGRM a_pstGRM);

/**将GRM配置写道共享内存中
*@param[in] a_pstShmGRM 指向GCIM共享内存数据结构指针
*@param[in] a_pstRelayConf GRM配置信息结构指针
*@retval 0 成功
*@retval <0 失败
*/
int tbus_set_grm(IN LPTBUSSHMGRM a_pstShmGRM, IN LPRELAYMNGER a_pstRelayConf);

int tbus_check_channel_shm_i(LPTBUSSHMCHANNELCNF a_pstShmChl, LPTBUSSHMGCIMHEAD a_pstHead);

int tbus_delete_channel_shm_i(LPTBUSSHMCHANNELCNF a_pstShmChl);

int tbus_wrlock(pthread_rwlock_t *a_pstRWLock);

int tbus_rdlock(pthread_rwlock_t *a_pstRWLock);

void tbus_unlock(pthread_rwlock_t *a_pstRWLock);
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif/*TBUS_CONFIG_MNG_H*/

