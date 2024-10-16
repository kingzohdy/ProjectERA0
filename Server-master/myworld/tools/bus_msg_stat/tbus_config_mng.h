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

#define TBUS_DEFAULT_ADDRESS_TEMPLET	"8.8.8.8"  /*tbusϵͳȱʡ��ַģ��*/

/**<timeout for attaching shm by milli seconds*/
#define TBUS_ATTACH_SHM_TIMEOUT						30000	

/*timeout for lock by milli seconds*/
#define TBUS_RWLOCK_TIMEOUT			30000

/**<sleep time by micro seconds per loop*/
#define TBUS_SLEEP_PRELOOP						100	


#define TBUS_CHANNEL_SIDE_INDEX_1				0	/*ͨ��ĳһ�˵�����ֵ*/
#define TBUS_CHANNEL_SIDE_INDEX_2				1	/*ͨ��ĳһ�˵�����ֵ*/

#define TBUS_MAX_PATH_LEN					1024    /*path·������*/
#define TBUS_APPDATA_PATH					"SystemRoot" /*appdata ��������*/
#define TBUS_SHM_MMAP_PATH					"tbus" /*appdata ��������*/

#ifdef WIN32
#define TBUS_DEFAULT_MMAP_FILE_DIR			"C:"
#else
#define TBUS_DEFAULT_MMAP_FILE_DIR			"/tmp"
#endif

#define TBUS_GCIM_SHM_ACCESS	0664	//GCIM�����ڴ�ķ���Ȩ��
#define TBUS_GRM_SHM_ACCESS	0664	//GRM�����ڴ�ķ���Ȩ��

#define TBUS_GCIM_CHANNEL_FLAG_NONE                 0X00000000 	
#define TBUS_GCIM_CHANNEL_FLAG_ENABLE               0X00000001 	/*�������Ƿ���Ч*/
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
	pthread_rwlock_t stRWLock;						/**< TBUS���ù����ڴ��У�ͨ�������д�����л������*/
	volatile unsigned int dwVersion;                       	/*   ��shm�е����ݸ��º���Ҫ���¸���ֵ���Է���Ӧ��ģ����н������¼��� */
	unsigned int dwShmKey;                        	/*   GCIM�����ڴ�key����Ҫ��У���� */
	unsigned int dwShmSize;                       	/*   �����ڴ��С */
	unsigned int dwMaxCnt;                        	/*   �����ڴ����������ɵ�channel��Ϣ��slot�� */
	unsigned int dwUsedCnt;                       	/*   �����ڴ����õ�slot�� */
	unsigned int dwCreateTime;                    	/*  �����ڴ洴��ʱ�� */
	unsigned int dwLastStamp;                     	/*  �����ڴ�����޸�ʱ��� */
	unsigned int dwBusiID;	/**<ʹ��tbusϵͳ��ҵ���ID*/
	TBUSADDRTEMPLET	stAddrTemplet;					/**< ��busϵͳ��ʹ�õĵ��ʮ���Ƶ�ַģ��*/	
	unsigned int dwCRC;                           	/*  �����ڴ������ */
	unsigned int reserve[TBUS_SHM_GRM_HEAD_RESERV_NUM]; 	/*Ԥ��������*/
};

struct tagTbusShmGCIMHead
{
	volatile unsigned int dwVersion;                       	/*   ��shm�е����ݸ��º���Ҫ���¸���ֵ���Է���Ӧ��ģ����н������¼��� */
	unsigned int dwShmKey;                        	/*   GCIM�����ڴ�key����Ҫ��У���� */
	unsigned int dwShmSize;                       	/*   �����ڴ��С */
	unsigned int dwMaxCnt;                        	/*   �����ڴ����������ɵ�channel��Ϣ��slot�� */
	unsigned int dwUsedCnt;                       	/*   �����ڴ����õ�slot�� */
	unsigned int dwCreateTime;                    	/*  �����ڴ洴��ʱ�� */
	unsigned int dwLastStamp;                     	/*  �����ڴ�����޸�ʱ��� */
	unsigned int dwBusiID;	/**<ʹ��tbusϵͳ��ҵ���ID*/
	unsigned int dwCRC;                           	/*  �����ڴ������ */
	int iAlign;				/*ͨ�����ݶ��뷽ʽ*/
	int iAlignLevel;				/*ͨ�����ݶ���ֵ�Ķ���ֵ,��ֵͨ��iAlign�������*/
	pthread_rwlock_t stRWLock;						/**< TBUS���ù����ڴ��У�ͨ�������д�����л������*/
	TBUSADDRTEMPLET	stAddrTemplet;					/**< ��busϵͳ��ʹ�õĵ��ʮ���Ƶ�ַģ��*/	
	unsigned int reserve[TBUS_SHM_GCIM_HEAD_RESERV_NUM]; 	/*Ԥ��������*/
};

struct tagShmChannelCnf
{
	unsigned int dwFlag;                        	/*   bit set: 0001 -- route valid */
	unsigned int dwPriority;                     
	int iShmID;	/*id of share memory for channel*/                          
	TBUSADDR astAddrs[2];                        	/* ͨ�����˵ĵ�ַ  original ip format: xxx.xxx.xxx.xxx, converted by tbus_addr_aton() */
          
	unsigned int dwRecvSize;                      	/*����astAddrs[0]��˵����ն��еĴ�С, default is 2M */
	unsigned int dwSendSize;                      	/*����astAddrs[0]��˵�䷢�Ͷ��еĴ�С, default is 2M */
                   
               
	time_t dwCTime;                        
	time_t dwInvalidTime;	/*��ͨ��������ΪʧЧ����ʼʱ��*/ 
	int aiReserver[TBUS_CHANNEL_RESERVER_NUM]; 
};

/** �����ڴ��е�ͨ����Ϣ���ñ�
*/
struct tagTbusShmGCIM 
{
	TBUSSHMGCIMHEAD	stHead;
	TBUSSHMCHANNELCNF astChannels[TBUS_MAX_CHANNEL_NUM_PREHOST]; /*����ͨ����������Ϣ*/
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


/*�����ڴ��е�ͨ����Ϣ���ñ�*/
struct tagTbusShmGRM 
{
	TBUSSHMGRMHEAD	stHead;
	TBUSSHMRELAYCNF astRelays[TBUS_MAX_RELAY_NUM_PREHOST]; /*������תͨ����������Ϣ*/
};

/**����GCIM�����ڴ� 
*@param[in,out] ppstGCIM ����GCIM�����ڴ�ṹָ���ָ��
*@param[in] a_pszShmkey ���ɹ����ڴ�key�������Ϣ��
*@param[in]	a_iBussId ҵ��id
*@param[in]	a_iShmSize ָ�������ڴ��С�����ڼ����Ѵ��ڵĹ����ڴ�˲�����������Ϊ0
*@param[in]	iSHmFlag ���Ʊ�־λ������ָʾshm����Ȩ�ޣ��Ƿ���Ҫ��������Ϣ
*@retval 0 �ɹ�
*@retval <0 ʧ��
*/
int tbus_create_gcimshm(INOUT LPTBUSSHMGCIM *a_ppstGCIM, IN const char *a_pszShmkey, 
							   IN int a_iBussId, IN unsigned int a_iShmSize);

/**����GCIM�����ڴ� 
*@param[in,out] ppstGCIM ����GCIM�����ڴ�ṹָ���ָ��
*@param[in] a_pszShmkey ���ɹ����ڴ�key�������Ϣ��
*@param[in]	a_iBussId ҵ��id
*@param[in]	a_iShmSize ָ�������ڴ��С�����ڼ����Ѵ��ڵĹ����ڴ�˲�����������Ϊ0
*@param[in] a_iTimeout If the gcim share memory do not exist, try to attach again, until time out
*@retval 0 �ɹ�
*@retval <0 ʧ��
*/
int tbus_get_gcimshm(INOUT LPTBUSSHMGCIM *a_ppstGCIM, IN const char *a_pszShmkey, 
					 IN int a_iBussId, IN unsigned int a_iShmSize, IN int a_iTimeout);


/**��GCIM����д�������ڴ���
*@param[in] a_pstShmGCIM ָ��GCIM�����ڴ����ݽṹָ��
*@param[in] a_pstGCIM GCIM������Ϣ�ṹָ��
*@retval 0 �ɹ�
*@retval <0 ʧ��
*/
int tbus_set_gcim(IN LPTBUSSHMGCIM a_pstShmGCIM, IN LPTBUSGCIM a_pstGCIM);

void tbus_detach_gcim(IN LPTBUSSHMGCIM a_pstShmGCIM);

/**ɾ��ָ��������ͨ������ 
*@param[in] a_pstShmGCIM ָ��GCIM�����ڴ����ݽṹָ��
*@param[in] a_idx ָ��ͨ����GCIM�е�����
*@retval 0 �ɹ�
*@retval <0 ʧ��
*/
int tbus_delete_channel_by_index(IN LPTBUSSHMGCIM a_pstShmGCIM, IN int a_idx);

/** Register business id to Senderd-Agent system
*/
int tbus_register_bussid(IN int a_iBussId);

void tbus_set_shmgcimheader_stamp(LPTBUSSHMGCIMHEAD pstHeader);

/**���·�ɱ����ڴ�ļ�����,������гɹ�����0,���򷵻ط���ֵ 
*/
int tbus_check_shmgcimheader_stamp(LPTBUSSHMGCIMHEAD pstHeader);

void tbus_set_shmgrmheader_stamp(LPTBUSSHMGRMHEAD pstHeader);


int tbus_check_shmgrmheader_stamp(LPTBUSSHMGRMHEAD pstHeader);

/**����GRM�����ڴ� 
*@param[in,out] a_ppstGRM ����GRM�����ڴ�ṹָ���ָ��
*@param[in] a_pszShmkey ���ɹ����ڴ�key�������Ϣ��
*@param[in]	a_iBussId ҵ��id
*@param[in]	a_iShmSize ָ�������ڴ��С�����ڼ����Ѵ��ڵĹ����ڴ�˲�����������Ϊ0
*@retval 0 �ɹ�
*@retval <0 ʧ��
*/
int tbus_create_grmshm(INOUT LPTBUSSHMGRM *a_ppstGRM, IN const char *a_pszShmkey, 
						IN int a_iBussId, IN unsigned int a_iShmSize);

/**����GRM�����ڴ� 
*@param[in,out] a_ppstGRM ����GRM�����ڴ�ṹָ���ָ��
*@param[in] a_pszShmkey ���ɹ����ڴ�key�������Ϣ��
*@param[in]	a_iBussId ҵ��id
*@param[in]	a_iShmSize ָ�������ڴ��С�����ڼ����Ѵ��ڵĹ����ڴ�˲�����������Ϊ0
*@retval 0 �ɹ�
*@retval <0 ʧ��
*/
int tbus_get_grmshm(INOUT LPTBUSSHMGRM *a_ppstGRM, IN const char *a_pszShmkey, 
					 IN int a_iBussId, IN unsigned int a_iShmSize);


void tbus_detach_grmshm(IN LPTBUSSHMGRM a_pstGRM);

/**��GRM����д�������ڴ���
*@param[in] a_pstShmGRM ָ��GCIM�����ڴ����ݽṹָ��
*@param[in] a_pstRelayConf GRM������Ϣ�ṹָ��
*@retval 0 �ɹ�
*@retval <0 ʧ��
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

