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



/** @name ������������/�������
*  @{
*/
/*��ʾ�ò���ֻ���������*/
#ifndef IN
#define IN  
#endif


/*��ʾ�ò���ֻ���������*/
#ifndef OUT
#define OUT
#endif


/*��ʾ�ò���������������������������*/
#ifndef INOUT
#define INOUT
#endif

/** @}*/ // ������������/�������


/* TbusFlag*/
enum tagTbusFlag 
{
	TBUS_FLAG_START_DYE_MSG = 1, 	/* ָʾtbusϵͳ�Դ���Ϣ����Ⱦɫ */
	TBUS_FLAG_KEEP_DYE_MSG = 2, 	/* ָʾtbusϵͳ����ǰһ����Ϣ��Ⱦɫ��Ϣ */
};

/* initialize flags*/
#define TBUS_INIT_FLAG_NONE		0x0		/*��������*/


#define TBUS_ADDR_DEFAULT                        	0 	/*  ��ʾ�����һ����ַ*/
#define TBUS_ADDR_ALL                            	-1 	/* ��ʾ�������е�ַ */


#define TBUS_LOGCAT_NAME	"tsf4g.tbus"		/*tbus��־������*/

#define TBUS_DEFAULT_GCIM_KEY	"1688"	/*defalt share memory key fro GCIM(Global Channel Infomation Map)*/
#define TBUS_DEFAULT_GRM_KEY	"2688"	/*defalt share memory key fro GRM(Global Route Map)*/

#define TBUS_ROUTE_GLOBAL_SHMKEY	1688 /*defalt share memory key fro GCIM(Global Channel Infomation Map), for tbus_init*/

#define tbus_initialize	tbus_init_ex		/*tbus_initialize�ӿڵ����ָĳ�tbus_init_ex��*/


#define TBUS_CHANNEL_MAGIC	0x42545553		//TBUS


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef  int	TBUSADDR;		/*tbusϵͳͨ�ŵ�ַ*/
typedef  int	*LPTBUSADDR;		/*tbusϵͳͨ�ŵ�ַ*/


typedef struct tagTbusChannel                          	*LPTBUSCHANNEL;

/*route info of package */
struct tagHeadRoute
{
	TBUSADDR iSrc;                           
};
typedef struct tagHeadRoute                        	HEADROUTE;
typedef struct tagHeadRoute                        	*LPHEADROUTE;


/** @defgroup TBUS_INIT TBUS_�������
*  @{
*/

/**brief ��ʼ��Tbusϵͳ
*	����Ҫְ������tbus���ù���ϵͳע��ҵ��ID�������Թ���(attach)�ϱ���tbus
*	ͨ��������Ϣ�Ĺ����ڴ�GCIM(Global Channel Infomation Map)
*@param[in] a_pszShmkey	��������GCIM�����ڴ�key����Ϣ��������Ϣ���е���Ϣֻ��Ϊʮ��������
*	����Ϊһ���ļ���·���������ļ�������ʱ��tbusϵͳ�᳢�����ɴ��ļ��������ݴ���Ϣ������GCIM�����ڴ�key���㷨��:
*	- ʮ�������ִ�	�򽫴����ִ�ת����������������ΪGCIM�Ĺ����ڴ��key
*	- �ļ�·��	�����ļ�·����a_iBussId��Ϊ����������ftok�ӿ�����GCIM�����ڴ��key
*	- NULL��մ�	ʹ��ȱʡֵ TBUS_ROUTE_GLOBAL_SHMKEY ��ΪGCIM�����ڴ��key	
*@param[in]	a_iFlag ��ʼ��������־λ��Ŀǰ�˲���ΪԤ������,�Ա�������չ��Ŀǰ����ȱʡֵ0(TBUS_INIT_FLAG_NONE)����
*
*@note	���ӿڱ���������tbus�ӿڵ���֮ǰ����
*@note	��ҵ������˳�ʱ���������tbus_fini�ӿڣ��Ƿ񱾽ӿڷ������Դ
*@note	��ͬһ�������ڣ����ӿ�ֻ�е�һ�γɹ����û�������ã�һ��tbusϵͳ��ʼ���ú󣬺������ñ��ӿ�
*	��ֱ�ӷ���
*@note	���ӿ��Ƿ��̰߳�ȫ�ģ�ͬһ������������ж���̣߳������߱������б�֤�˽ӿڻ������
*
*@pre \e ���ӿڵ���֮ǰ����ͨ�� tbusmgr���߻�tbus���ù���ϵͳ�ɹ�����GCIM�����ڴ棬���򱾽ӿڵ��û�
*	ʧ��
*
*@retval 0 ����ɹ�
*@retval <0 ����ʧ��,ͨ��TBUS_ERR_GET_ERROR_CODE(retval),���Ի�ȡ������룬���ܵĴ�������У�
*	- TBUS_ERROR_INIT_LOGSYSTEM_FAILED ��ʼ����־ϵͳʧ��
*	- TBUS_ERROR_GEN_SHMKEY_FAILED	���ɹ����ڴ��keyֵʧ��
*	- TBUS_ERROR_SHMGET_FAILED	ͨ��shmget���乲���ڴ�ʧ��
*	- TBUS_ERROR_SHMAT_FAILED	ͨ��shmat���ع����ڴ�ʧ��
*	- TBUS_ERROR_CHECK_GCIMSHM_FAILED	GCIM�����ڴ�У��ʧ��
*
*see tbus_fini
*@note ���a_pszShmkeyΪNULL��������"1688"Ϊkey�Ĺ����ڴ�
*/
TSF4G_API int tbus_init_ex(IN const char *a_pszShmkey, IN int a_iFlag);



/**
@brief ����tbus������

@param[in,out] a_piHandle �洢��������ָ��

@retval 0 -- success, get handle ok
@retval <0 -- failed, ͨ��TBUS_ERR_GET_ERROR_CODE(retval)���Ի�ȡ������룬���Ի᷵�����´������
	-	TBUS_ERROR_INVALID_ARG	���ݸ��ӿڵĲ�������
	-	TBUS_ERROR_NOT_INITIALIZED	busϵͳ��û�г�ʼ��
	-	TBUS_ERROR_HANDLE_FULL	�ѷ���tbus��������Ѿ��ﵽ�趨�������Ŀ�������ڷ���
@pre \e   a_piHandle should be not NULL 
@note ���ӿ��Ƿ��̰߳�ȫ�ģ����������ľ��Ҳֻ����ͬһ���߳̿ռ���ʹ�� 
@note ���ӿڴ����ľ������ʹ��tbus_delete���رգ����ͷ���Դ
*/
TSF4G_API int tbus_new( INOUT int *a_piHandle ) ;

/**
@brief ɾ��һ��tubs������

@param[in,out] a_piHandle ����tbus��������ָ�룬tbus���ͨ��tbus_new()��ȡ

@retval 0 -- success, deleted ok
@retval !0 -- failed

@note
*/
TSF4G_API void tbus_delete ( INOUT int *a_piHandle ) ;


/**
@brief �ر�tbusϵͳ

@retval 0 -- success, closed ok
@retval !0 -- failed

@note ֻҪ�����˱��ӿڣ�tbus�����ӿڵĹ��ܽ����ٿ���
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


/** @defgroup TBUS_ADDR_CONVERT TBUS_ͨ�ŵ�ַת��
*  @{
*/

/**
*@brief �����ʮ����ͨ�ŵ�ַ�ַ���ת����tbusϵͳ�ڴ��ַ
*
*@param[in] a_pszAddr ���ʮ����ͨ�ŵ�ַ�ַ���
*@param[out] a_piAddr ����tbusͨ�ŵ�ַ��ָ��
*
*@pre \e a_pszAddr����ΪNULL
*@pre \e a_piAddr ����ΪNULL
*
*@retval 0 success
*@retval <0 failed, ���ܻ᷵�����´������
	-	TBUS_ERROR_NOT_INITIALIZED	busϵͳ��û�г�ʼ��
	-	TBUS_ERROR_INVALID_ADDR_STRING	���ʮ���Ƶ�ַ���Ƿ������ֵ��ʮ����ͨ�ŵ�ַģ�岻����
*
*@note	a_pszAddr�б���ĵ��ʮ����ͨ�ŵ�ַ��������ϵ��ʮ����ͨ�ŵ�ַģ���Լ��
*@note	���ñ��ӿ�֮ǰ�������tbus_init_ex�Ѿ���ʼ����tbusϵͳ
*@see tbus_init_ex
*
*/
TSF4G_API int tbus_addr_aton(IN const char *a_pszAddr, OUT TBUSADDR *a_piAddr) ;


/**
*@brief ��tbusͨ�ŵ�ַת���ɵ��ʮ���Ƶ�ַ��Ϣ��
*
*@param[in] a_iAddr tbusxtͨ�ŵ�ַ
*
*@return ����ɹ�ת���򷵻ص��ʮ���Ƶ�ַ�������򷵻ؿմ�""
*
*@note ���صĳɵ��ʮ���Ʊ�ʾ�ĵ�ַ��������һ����̬�������У���һ�ε��ûḲ��ǰһ�ε���ʱ��ȡ����Ϣ
*@note	���ñ��ӿ�֮ǰ�������tbus_init_ex�Ѿ���ʼ����tbusϵͳ
*@see tbus_init_ex
*/
TSF4G_API char *tbus_addr_ntoa(IN TBUSADDR a_iAddr) ;


/**  @} */



/** @defgroup TBUS_CHANNEL_MNG TBUS_ͨ������
*  @{
*/




/**
  @brief ָ��tbusͨ�����ص�ַ,��tbus�ӿڵ����߽��̵�ͨ�ŵ�ַ

  @param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ
  @param[in] a_szSrcAddr ���ص�ַ�����ʮ���Ʊ�ʾ��ͨ�ŵ�ַ������ʽ������: xxx.xxx.xxx.xxx.xxx

  @retval 0 success
  @retval <0 failed, ���ܵĴ����������:
	-	TBUS_ERR_ARG	���ݸ��ӿڵĲ�������
	-	TBUS_ERROR_NOT_INITIALIZED	busϵͳ��û�г�ʼ��
	-	TBUS_ERROR_INVALID_ADDR_STRING	���ʮ���Ƶ�ַ���Ƿ������ֵ��ʮ����ͨ�ŵ�ַģ�岻����
	-	TBUS_ERROR_CHANNEL_NUM_LIMITED
	-	TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
	-	TBUS_ERROR_BIND_FAILED
	@note ���ӿڻ�ͨ����д���������GCIM�����ڴ��е���Ϣ����˱��ӿ�������ʽ��
	@pre a_szSrcAddr ����ΪNULL
	@pre a_iHandle Ϊ��Ч���
  @see tbus_new
*/
TSF4G_API int tbus_bind_by_str ( IN const int a_iHandle, IN const char *a_szSrcAddr ) ;


/**
  @brief ָ��tbusͨ�����ص�ַ,��tbus�ӿڵ����߽��̵�ͨ�ŵ�ַ

  @param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ
  @param[in] a_iSrcAddr ���ص�ַ��ͨ��tbus_addr_aton�ӿڿ��Ի�ȡ

  @retval 0 success
  @retval <0 failed, ���ܵĴ����������:
	-	TBUS_ERR_ARG	���ݸ��ӿڵĲ�������
	-	TBUS_ERROR_NOT_INITIALIZED	busϵͳ��û�г�ʼ��
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_INVALID_ADDR_STRING	���ʮ���Ƶ�ַ���Ƿ������ֵ��ʮ����ͨ�ŵ�ַģ�岻����
	-	TBUS_ERROR_CHANNEL_NUM_LIMITED
	-	TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
	-	TBUS_ERROR_BIND_FAILED
	@note ���ӿڻ�ͨ����д���������GCIM�����ڴ��е���Ϣ����˱��ӿ�������ʽ��
	@see tbus_new
*/
TSF4G_API int tbus_bind( IN const int a_iHandle, IN const TBUSADDR a_iSrcAddr) ;


/**
@brief ����ȫ��GCIM�е�����ˢ��tbus�����������ͨ��
	����󶨵�ַ���ͨ����������Զ���ӵ�tbus�У����tbus�����������ͨ���Ѿ�����gcim����
	�����ڻ���
@param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ

@retval 0 success
@retval <0 failed, ���ܵĴ����������:
-	TBUS_ERR_ARG	���ݸ��ӿڵĲ�������
-	TBUS_ERROR_NOT_INITIALIZED	busϵͳ��û�г�ʼ��
-	TBUS_ERROR_CHANNEL_NUM_LIMITED
-	TBUS_ERROR_NO_MEMORY
-	TBUS_ERROR_SHMGET_FAILED
-	TBUS_ERROR_SHMAT_FAILED
-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
-	TBUS_ERROR_BIND_FAILED
@note tbus APIʹ���߿��Զ�ʱ���ñ��ӿ���ˢ�¸þ�������ͨ������

@pre a_iHandle Ϊ��Ч���
@see tbus_new
@see tbus_bind
*/
TSF4G_API int tbus_refresh_handle( IN const int a_iHandle) ;

/**
  @brief ָ��tbusͨ���ĶԶ˵�ַ

  @param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ
  @param[in] a_szDstAddr ���ʮ���Ʊ�ʾ��ͨ�ŵ�ַ������ʽ������: xxx.xxx.xxx.xxx.xxx

  @retval 0 -- success, 
  @retval !0 -- failed,���ܵĴ����������:
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
  @brief ָ��tbusͨ���ĶԶ˵�ַ,�ӿڹ�����tbus_connect_by_str  ��ͬ

  @param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ
  @param[in] a_iDstAddr �Զ˵�ַ��ͨ��tbus_addr_aton()��ȡ

  @retval 0 -- success, 
  @retval !0 -- failed,���ܵĴ����������:
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
@brief ͨ���Զ˵�ַ���ƣ���ֹ/���� �շ�ĳ���Զ˵�ַ��ͨ������

@param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ
@param[in] a_iDstAddr ��Ҫ���ƵĶԶ˵�ַ
@param[in] a_iMode ����λ������ȡ��������ֵ: 
	��	TBUS_MODE_DISABLE	��ֹ�Ӵ˶Զ˵�ַ�շ�����
	��	TBUS_MODE_ENABLE	�����Ӵ˶Զ˵�ַ�շ�����
@param[in] a_iBatch ͨ��ѡ���ʾλ������ȡ��������ֵ��
	- TBUS_PEER_CTRL_ONE	�����bus���������ĵ�һ��ͨ�����п���
	- TBUS_PEER_CTRL_ALL	���bus��������������ͨ�����п���

@retval 0 -- success
@retval !0 -- failed,���ܵĴ����������:
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_NO_CHANNEL

@note
*/
TSF4G_API int tbus_peer_ctrl( IN const int a_iHandle, IN const TBUSADDR a_iDstAddr, IN const int a_iMode, IN const int a_iBatch ) ;

/**
@brief ����ͨ����ַ��ȡָ��ͨ���ľ��

@param[in] a_iHandle tbus��������ͨ������tbus_new() ��ȡ
@param[out] a_ppstChannel ����ָ��ͨ��ָ���ָ��
@param[in] a_iSrcAddr ͨ���ı��ص�ַ
@param[in] a_iDstAddr ͨ���ĶԶ˵�ַ

@retval 0 -- success
@retval !0 -- failed,���ܵĴ����������:
	-	TBUS_ERROR_INVALID_HANDLE
	-	TBUS_ERROR_NO_CHANNEL
@pre a_ppstchannel ����ΪNULL
*/
TSF4G_API int tbus_get_channel( IN const int a_iHandle, OUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iSrcAddr, IN TBUSADDR a_iDstAddr) ;



/**
@brief ͨ��ָ�������ڴ��һ��ͨ��ͨ�� �˽ӿ����һ��tbusϵͳ�ļ���ʵ��
*	@param[in,out] a_ppstChannel ����򿪵�ͨ�������ָ��
*	@param[in] a_iLocalAddr ͨ�����ص�ַ,��ֵַͨ��inet_aton�������Ϊ�Զ�������
*	@param[in] a_iPeerAddr ͨ���Զ˵�ַ ����ֵַͨ��inet_aton�������Ϊ�Զ�������
*	@param[in] a_iShmkey �����ڴ�key
*	@param[in] a_iSize ͨ�����ݶ��еĴ�С����д���еĴ�С��ͬ
*	@retval 0 success, 
*	@retval !0 failed�����ܵĴ����������:
	- TBUS_ERROR_INIT_LOGSYSTEM_FAILED
	- TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
*	
@note ͨ���˽ӿڴ򿪵�ͨ��������������tbus_detach_channel�ر�
@pre a_ppstchannel ����ΪNULL
@pre a_iSize �������0
*	@see tbus_close_channel
*/
TSF4G_API int tbus_open_channel(INOUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iLocalAddr,
					  IN TBUSADDR a_iPeerAddr,IN int a_iShmkey, IN int a_iSize) ;

/**
@brief ͨ��ָ�������ڴ��һ��ͨ��ͨ�� �˽ӿ����һ��tbusϵͳ�ļ���ʵ��
*	@param[in,out] a_ppstChannel ����򿪵�ͨ�������ָ��
*	@param[in] a_pszLocalAddr ͨ�����ص�ַ,��ַ��ʽΪIPv4�����ַ ���ʮ���Ʊ�ʾ��,��x.x.x.x
*	@param[in] a_pszPeerAddr ͨ���Զ˵�ַ ����ַ��ʽΪIPv4�����ַ ���ʮ���Ʊ�ʾ��,��x.x.x.x
*	@param[in] a_iShmkey �����ڴ�key
*	@param[in] a_iSize ͨ�����ݶ��еĴ�С����д���еĴ�С��ͬ
*	@retval 0 success, 
*	@retval !0 failed�����ܵĴ����������:
	- TBUS_ERROR_INIT_LOGSYSTEM_FAILED
	- TBUS_ERROR_NO_MEMORY
	-	TBUS_ERROR_SHMGET_FAILED
	-	TBUS_ERROR_SHMAT_FAILED
	-	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT
	-	TBUS_ERROR_INVALID_ADDR_STRING
*
@note ͨ���˽ӿڴ򿪵�ͨ��������������tbus_detach_channel�ر�
*	@see tbus_close_channel
@pre a_ppstchannel ����ΪNULL
@pre a_iSize �������0
@pre  a_pszLocalAddr ����ΪNULL
@pre a_pszPeerAddr ����ΪNULL
*/
TSF4G_API int tbus_open_channel_by_str(INOUT LPTBUSCHANNEL *a_ppstChannel, IN const char *a_pszLocalAddr,
					  IN const char *a_pszPeerAddr,IN int a_iShmkey, IN int a_iSize) ;

/**
@brief �ر�һ��ͨ��ͨ��
*	@param[in,out] a_ppstChannel ����򿪵�ͨ�������ָ��
*	@retval 0 success, 
*	@retval !0 failed.
@note a_ppstChannel��ָ��ͨ����������ǵ���tbus_open_channel�򿪵�
*	@see tbus_open_channel
*/
TSF4G_API void tbus_close_channel(INOUT LPTBUSCHANNEL *a_ppstChannel) ;

/**  @} */ 








/** @defgroup TBUS_DATA_TRANSFER TBUS_���ݴ���
*  @{
*/

/**
  @brief �������ݣ�����ʵ��ֻ��һ��ͨ���������ݣ�Ҳ���ԶԶ��ͨ�����ʹ����ݣ�������Щ
	ͨ���跢��������Դ��ַ��Ŀ�ĵ�ַ����
	
  @param[in] a_iHandle tbus���, ͨ������tbus_new()��ȡ
  @param[in,out] a_piSrc ָ���������ݵ�Դ��ַ,ͨ��tbus_addr_aton()ת��������
	��	����	����ָ�����ݵ�Դ��ַ����ֵ��Ϊ�������������
		��	*a_piSrc == 0	��Ϊȱʡֵ����ʹ�õ�һ��ƥ��Ŀ�ĵ�ַ��ͨ������������
		��	*a_piSrc == -1	��Ϊ�㲥��ַ����������ƥ��Ŀ�ĵĵ�ַ*a_piDst��ͨ������������
		��	�����ֵַ		ָ������Դ��ַ��ֻ��ƥ���Դ��ַ��ͨ���ſ��ܻᷢ������
	��	���	�õ�����ʹ��ͨ����Դ��ַ                          
  @param[in,out] a_piDst ָ���������ݵ�Ŀ�ĵ�ַ��ͨ��tbus_addr_aton()ת��������
  ��	����	����ָ�����ݵ�Ŀ�ĵ�ַ����ֵ��Ϊ�������������
	��	*a_piDst == 0	��Ϊȱʡֵ����ʹ�õ�һ��ƥ��Դ��ַ��ͨ�����������ݣ������ʱ*a_piSrc == 0��ʹ��
		tbus�������ĵ�һ��ͨ������������
	��	*a_piDst == -1	��Ϊ�㲥��ַ����������ƥ��Դ��ַ��ͨ������������
	��	�����ֵַ		ָ������Ŀ�ĵ�ַ��ֻ��ƥ���Ŀ�ĵ�ַ��ͨ���ſ��ܻᷢ������
  ��	���	�õ�����ʹ��ͨ����Ŀ�ĵ�ַ

  @param[in] a_pvData ���淢�����ݵĻ�����֧��
  @param[in] a_iDataLen ���������ݵĳ���
  @param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
	��	TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
	��	TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @note	������ȷ������TBUS_FLAG_KEEP_DYE_MSG��־λ��tbus_sendȱʡ���̳���һ����Ϣ��Ⱦɫ����
  @note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
	 ��ȡ��Ⱦɫ��Ϣ�������룬�Ա����
  @see	tbus_get_dyedmsgid
  @see tbus_addr_aton
  @see tbus_new
  @pre a_piSrc ����ΪNULL
  @pre a_piDst ����ΪNULL
  @pre a_pvData ����ΪNULL
  @pre a_iDataLen �������0
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
  @param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
	- TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
	- TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @note	������ȷ������TBUS_FLAG_KEEP_DYE_MSG��־λ�����ӿ�ȱʡ���̳���һ����Ϣ��Ⱦɫ����
  @note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
  ��ȡ��Ⱦɫ��Ϣ�������룬�Ա����
    @see	tbus_get_dyedmsgid
	@see	tbus_send
	@pre a_piSrc ����ΪNULL
	@pre a_piDst ����ΪNULL
	@pre a_ptVector ����ΪNULL
	@pre a_iVecCnt �������0
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
@brief ��ͨ���ж�ȡ��һ����Ϣ������ʵ��ֻ��ĳһ��ָ��ͨ���������ݣ�Ҳ���ԶԳ��ԴӴӶ��ͨ���ж�ȡ���ݣ�����ѡ���Ǹ�
	ͨ��������Ϣ���в�����ָ����Դ��Ŀ�ĵ�ַȷ��

@param[in] a_iHandle tbus���, ͨ������tbus_new()��ȡ
@param[in,out] a_piSrc ָ�����ݵ�Դ��ַ��
	��	����	����ָ�����ݵ�Դ��ַ����ֵ��Ϊ�������������
		��	*a_piSrc == 0	��Ϊȱʡֵ����ʹ�õ�һ��ƥ��Ŀ�ĵ�ַ��ͨ������������
		��	*a_piSrc == -1	��Ϊ�㲥��ַ����������ƥ��Ŀ�ĵĵ�ַ*a_piDst��ͨ�������Խ�������
		��	�����ֵַ		ָ������Դ��ַ��ֻ��ƥ���Դ��ַ��ͨ���ſ��ܻ��������
	��	���	�õ�ʵ�ʶ�ȡ���ݵ�ͨ����Դ��ַ                          
@param[in,out] a_piDst ָ�����ݵ�Ŀ�ĵ�ַ��ͨ��tbus_addr_aton()ת��������
	��	����	����ָ�����ݵ�Ŀ�ĵ�ַ����ֵ��Ϊ�������������
		��	*a_piDst == 0	��Ϊȱʡֵ����ʹ�õ�һ��ƥ��Դ��ַ��ͨ�����������ݣ������ʱ*a_piSrc == 0��ʹ��
			tbus�������ĵ�һ��ͨ������������
		��	*a_piDst == -1	��Ϊ�㲥��ַ����������ƥ��Դ��ַ��ͨ�������Խ�������
		��	�����ֵַ		ָ������Ŀ�ĵ�ַ��ֻ��ƥ���Ŀ�ĵ�ַ��ͨ���ſ��ܻ��������
	��	���	�õ�ʵ�ʶ�ȡ���ݵ�ͨ����Ŀ�ĵ�ַ

@param[in] a_pvData ����������ݵĻ�����ָ��
@param[in,out] a_piDataLen ���������ݵĳ���
	-	����	�����������ó���
	��	���	ʵ�ʽ������ݵĳ���
@param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
	-  TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
	-  TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @note	���ӿ�ȱʡ�̳���һ����Ϣ��Ⱦɫ����
  @note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
  ��ȡ��Ⱦɫ��Ϣ�������룬�Ա����
  @see	tbus_get_dyedmsgid
  @pre a_piSrc ����ΪNULL
  @pre a_piDst ����ΪNULL
  @pre a_pvData ����ΪNULL
  @pre a_iDataLen �������0
*/
TSF4G_API int tbus_recv (IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
	INOUT void *a_pvData,	INOUT size_t *a_piDataLen,	IN const int a_iFlag) ;


/**
@brief ������ͨ���л�ȡ������ͷ����һ����Ϣ�Ĵ洢λ�úʹ���Ϣ�ĳ�����Ϣ�����ӿ�ʵ��ֻ��ĳһ��ָ��ͨ����ȡ���ݣ�Ҳ���ԶԳ��ԴӴӶ��ͨ���ж�ȡ���ݣ�����ѡ���Ǹ�
ͨ��������Ϣ���в�����ָ����Դ��Ŀ�ĵ�ַȷ��

@param[in] a_iHandle tbus���, ͨ������tbus_new()��ȡ
@param[in,out] a_piSrc ָ�����ݵ�Դ��ַͨ��tbus_addr_aton()ת��������
��	����	����ָ�����ݵ�Դ��ַ����ֵ��Ϊ�������������
��	*a_piSrc == 0	��Ϊȱʡֵ����ʹ�õ�һ��ƥ��Ŀ�ĵ�ַ��ͨ������������
��	*a_piSrc == -1	��Ϊ�㲥��ַ����������ƥ��Ŀ�ĵĵ�ַ*a_piDst��ͨ�������Խ�������
��	�����ֵַ		ָ������Դ��ַ��ֻ��ƥ���Դ��ַ��ͨ���ſ��ܻ��������
��	���	�õ�����ʹ��ͨ����Դ��ַ                          
@param[in,out] a_piDst ָ�����ݵ�Ŀ�ĵ�ַ��ͨ��tbus_addr_aton()ת��������
��	����	����ָ�����ݵ�Ŀ�ĵ�ַ����ֵ��Ϊ�������������
��	*a_piDst == 0	��Ϊȱʡֵ����ʹ�õ�һ��ƥ��Դ��ַ��ͨ�����������ݣ������ʱ*a_piSrc == 0��ʹ��
tbus�������ĵ�һ��ͨ������������
��	*a_piDst == -1	��Ϊ�㲥��ַ����������ƥ��Դ��ַ��ͨ�������Խ�������
��	�����ֵַ		ָ������Ŀ�ĵ�ַ��ֻ��ƥ���Ŀ�ĵ�ַ��ͨ���ſ��ܻ��������
��	���	�õ�����ʹ��ͨ����Ŀ�ĵ�ַ

@param[in,out] a_ppvData ����ɹ���ȡ����Ϣ����˲���Ϊ���ڱ������ݴ洢��ַ��ָ��
@param[out] a_piDataLen ��ȡ���ݳ��ȵ�ָ��
@param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
-  TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
-  TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

@retval 0 -- success, sent ok
@retval !0 -- failed

@note ���ӿ���tbus_recv�Ĳ�֮ͬ������
	��	tbus_recv�����ݶ����ж�ȡ�����ݺ󣬻Ὣ���ݴӶ������Ƴ���
	�� �����ӿڽ���ֻ��	���������ڶ����еĴ洢λ�úͳ��ȣ������Ὣ���ݴӶ������Ƴ����������ݱ����������ص���
		tbus_delete_msg�����ݴӶ������Ƴ�������tbus_peek_msg�����Ƕ�ȡ��ͬһ����Ϣ
@note	���ӿ�ȱʡ�̳���һ����Ϣ��Ⱦɫ����
@note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
��ȡ��Ⱦɫ��Ϣ�������룬�Ա����


@see	tbus_get_dyedmsgid
@see	tbus_recv
@see	tbus_delete_msg
@pre a_piSrc ����ΪNULL
@pre a_piDst ����ΪNULL
@pre a_ppvData ����ΪNULL
@pre a_piDataLen ����ΪNULL
*/
TSF4G_API int tbus_peek_msg(IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
						 INOUT const char **a_ppvData,	OUT size_t *a_piDataLen,	IN const int a_iFlag) ;


/**
@brief ��ָ��ͨ���������ݶ���ͷ����һ��Ϣ�Ƴ�

@param[in] a_iHandle tbus���, ͨ������tbus_new()��ȡ
@param[in] a_iSrc ָ�����ݵ�Դ��ַ                    
@param[in] a_iDst ָ�����ݵ�Ŀ�ĵ�ַ

@retval 0 -- success, sent ok
@retval !0 -- failed

@note	���ӿ���tbus_peek_msg�ӿ����ʹ�ôӶ����ж�ȡ���ݽ��д������ڶ�ȡ������ֱ�ӱ���������
	�����У���˿��Լ���һ�����ݿ�������

@see	tbus_peek_msg
*/
TSF4G_API int tbus_delete_msg(IN const int a_iHandle,	IN TBUSADDR a_iSrc,	INOUT TBUSADDR a_iDst);



/**
  @brief ��ǰת����Ϣ���书����tbus_send���ƣ���������tbus_forward���¼��Ϣ��������е�·����Ϣ
	�������ʹ��tbus_backward��Ϣ����ԭ·����

	@param[in] a_iHandle tbus���, ͨ������tbus_new()��ȡ
	@param[in,out] a_piSrc ָ���������ݵ�Դ��ַ,
		��	����	����ָ�����ݵ�Դ��ַ����ֵ��Ϊ�������������
			��	*a_piSrc == 0	��*a_piSrc�޸�Ϊϵͳǰһ�����յ���Ϣ��Ŀ�ĵ�ַ�����֮ǰû���յ�������Ϣ��
				��˴ε��û�ʧ��
			��	*a_piSrc == -1	��Ϊ�㲥��ַ����������ƥ��Ŀ�ĵĵ�ַ*a_piDst��ͨ������������
			��	�����ֵַ		ָ������Դ��ַ��ֻ��ƥ���Դ��ַ��ͨ���ſ��ܻᷢ������
		��	���	�õ�����ʹ��ͨ����Դ��ַ                          
	@param[in,out] a_piDst ָ���������ݵ�Ŀ�ĵ�ַ��
		��	����	���������
			��	*a_piDst == 0	ȡ��һ��������Ϣ�м�¼��·�ɵ�ַ��Ϣ��ΪĿ�ĵ�ַ
				tbus�������ĵ�һ��ͨ������������
			��	*a_piDst == -1	��Ϊ�㲥��ַ����������ƥ��Դ��ַ��ͨ������������
			��	�����ֵַ		ָ������Ŀ�ĵ�ַ��ֻ��ƥ���Ŀ�ĵ�ַ��ͨ���ſ��ܻᷢ������
		��	���	�õ�����ʹ��ͨ����Ŀ�ĵ�ַ

	@param[in] a_pvData ���淢�����ݵĻ�����֧��
	@param[in] a_iDataLen ���������ݵĳ���
	@param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
		��	TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
		��	TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  
  @note	���ӿ�ȱʡ�̳���һ����Ϣ��Ⱦɫ����  
  @note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
  ��ȡ��Ⱦɫ��Ϣ�������룬�Ա����
  @see	tbus_get_dyedmsgid
  @see tbus_backward
  @pre a_piSrc ����ΪNULL
  @pre a_piDst ����ΪNULL
  @pre a_pvData ����ΪNULL
  @pre a_iDataLen �������0
*/
TSF4G_API int tbus_forward (IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
	IN const void *a_pvData,	IN const size_t a_iDataLen,	IN const int a_iFlag) ;




/**
@brief ��tbusͷ����¼��·�����ģ����ת����Ϣ

@param[in] a_iHandle tbus���, ͨ������tbus_new()��ȡ
@param[in,out] a_piSrc ָ���������ݵ�Դ��ַ,ͨ��tbus_addr_aton()ת��������
	��	����	����ָ�����ݵ�Դ��ַ����ֵ��Ϊ�������������
		��	*a_piSrc == 0	��Ϊȱʡֵ��ʹ��ǰһ����Ϣ��Ŀ�ĵ�ַ��ΪԴ��ַ
		��	*a_piSrc == -1	��Ϊ�㲥��ַ����������ƥ��Ŀ�ĵĵ�ַ*a_piDst��ͨ������������
		��	�����ֵַ		ָ������Դ��ַ��ֻ��ƥ���Դ��ַ��ͨ���ſ��ܻᷢ������
	��	���	�õ�����ʹ��ͨ����Դ��ַ                          
@param[in,out] a_piDst ָ���������ݵ�Ŀ�ĵ�ַ��ͨ��tbus_addr_aton()ת��������
	��	����	ͨ������Ϊ0���ɣ�����tbus���Զ�ȡ��һ��������Ϣ�м�¼��·�ɵ�ַ��Ϣ��ΪĿ�ĵ�ַ����ֵ��Ϊ�������������
		��	*a_piDst == 0	��Ϊȱʡֵ���Զ�ȡ��һ��������Ϣ�м�¼��·�ɵ�ַ��Ϣ��ΪĿ�ĵ�ַ
		��	*a_piDst == -1	��Ϊ�㲥��ַ����������ƥ��Դ��ַ��ͨ������������
		��	�����ֵַ		ָ������Ŀ�ĵ�ַ��ֻ��ƥ���Ŀ�ĵ�ַ��ͨ���ſ��ܻᷢ������
	��	���	�õ�����ʹ��ͨ����Ŀ�ĵ�ַ

@param[in] a_pvData ���淢�����ݵĻ�����֧��
@param[in] a_iDataLen ���������ݵĳ���
@param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
	��	TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
	��	TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

  @retval 0 -- success, sent ok
  @retval !0 -- failed

  @pre \e ���ӿ�Ҫ��������������֮ǰ���յ���һ��ͨ��tbus_forward�����������ݰ�
  @note	���ӿ�ȱʡ�̳���һ����Ϣ��Ⱦɫ����
  @note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
  ��ȡ��Ⱦɫ��Ϣ�������룬�Ա����
  @pre a_piSrc ����ΪNULL
  @pre a_piDst ����ΪNULL
  @pre a_pvData ����ΪNULL
  @pre a_iDataLen �������0
*/
TSF4G_API int tbus_backward (IN const int a_iHandle,	INOUT TBUSADDR *a_piSrc,	INOUT TBUSADDR *a_piDst,
	IN const void *a_pvData,	IN const size_t a_iDataLen,	IN const int a_iFlag) ;


/**
@brief ��ָ��ͨ���Ϸ���һ�����ݰ�
*	@param[in] a_pstChannel ͨ�����
*	@param[in] a_pvData �������ݵĻ������׵�ַ
*	@param[in] a_iDataLen ���ݳ���
*	@param[in] a_iFlag �����ֶ�
*	@retval 0 success, 
*	@retval !0 failed.
@pre a_pstChannel ����ΪNULL
@pre a_pvData ����ΪNULL
@pre a_iDataLen �������0
*/
TSF4G_API int tbus_channel_send(IN LPTBUSCHANNEL a_pstChannel,IN const void *a_pvData,  
					  IN const int a_iDataLen, IN const int a_iFlag) ;

/**
@brief ��ָ��ͨ���Ͻ���һ�����ݰ�
*	@param[in] a_pstChannel ͨ�����
*	@param[in] a_pvData �������ݵĻ������׵�ַ
*	@param[in,out] a_piDataLen �������ݳ��ȵ�ָ��
*	in	-	���ܻ���������С
*	out -	�������ݵ�ʵ�ʴ�С
*	@param[in] a_iFlag �����ֶ�
*	@retval 0 success, 
*	@retval !0 failed.
@pre a_pstChannel ����ΪNULL
@pre a_pvData ����ΪNULL
@pre a_iDataLen �������0
*/
TSF4G_API int tbus_channel_recv(IN LPTBUSCHANNEL a_pstChannel, INOUT void *a_pvData,
					  INOUT int *a_piDataLen,
					  IN const int a_iFlag) ;

/**
@brief ��ָ����������ͨ���л�ȡ������ͷ����һ����Ϣ�Ĵ洢λ�úʹ���Ϣ�ĳ�����Ϣ

*	@param[in] a_pstChannel ͨ�����
@param[in,out] a_ppvData ����ɹ���ȡ����Ϣ����˲���Ϊ���ڱ������ݴ洢��ַ��ָ��
@param[out] a_piDataLen ��ȡ���ݳ��ȵ�ָ��
@param[in] a_iFlag -- �����ֶΣ������Ʊ�־֧��'�루|��'��ʽ���ʹ�ã�Ŀǰ�Ѷ���ı�־λ�У�
-  TBUS_FLAG_START_DYE_MSG	�Է�����Ϣ����Ⱦɫ����
-  TBUS_FLAG_KEEP_DYE_MSG	�̳���һ����Ϣ��Ⱦɫ���ٵĹ��ܣ��������һ�����յ���Ϣ��Ⱦɫ�ģ���˷�����ϢҲ����Ⱦɫ

@retval 0 -- success, sent ok
@retval !0 -- failed

@note ���ӿ���tbus_channel_recv�Ĳ�֮ͬ������
��	tbus_channel_recv�����ݶ����ж�ȡ�����ݺ󣬻Ὣ���ݴӶ������Ƴ���
�� �����ӿڽ���ֻ��	���������ڶ����еĴ洢λ�úͳ��ȣ������Ὣ���ݴӶ������Ƴ����������ݱ����������ص���
tbus_channel_delete_msg�����ݴӶ������Ƴ�������tbus_channel_peek_msg�����Ƕ�ȡ��ͬһ����Ϣ
@note	���ӿ�ȱʡ�̳���һ����Ϣ��Ⱦɫ����
@note	���������TBUS_FLAG_START_DYE_MSG��־λ��������Ӧ���ڱ��ӿڳɹ����غ����tbus_get_dyedmsgid�ӿ�
��ȡ��Ⱦɫ��Ϣ�������룬�Ա����


@see	tbus_get_dyedmsgid
@see	tbus_channel_delete_msg
@see	tbus_channel_recv
@pre a_ppvData ����ΪNULL
@pre a_piDataLen ����ΪNULL
*/
TSF4G_API int tbus_channel_peek_msg(IN LPTBUSCHANNEL a_pstChannel, INOUT const char **a_ppvData,	OUT int *a_piDataLen,
									IN const int a_iFlag) ;


/**
@brief ��ָ��ͨ���������ݶ���ͷ����һ��Ϣ�Ƴ�

*	@param[in] a_pstChannel ͨ�����
@retval 0 -- success, sent ok
@retval !0 -- failed

@note	���ӿ���tbus_channel_peek_msg�ӿ����ʹ�ôӶ����ж�ȡ���ݽ��д������ڶ�ȡ������ֱ�ӱ���������
�����У���˿��Լ���һ�����ݿ�������

@see	tbus_peek_msg
*/
TSF4G_API int tbus_channel_delete_msg(IN LPTBUSCHANNEL a_pstChannel);

/**
@brief ��ȡ��һ��Ⱦɫ��Ϣ�������룬��tbusϵͳ�ӿڵ��ϲ�Ӧ�öԷ���/���յ���Ϣ����Ⱦɫ���ٺ��������ô˽ӿ�
*	���Ի�ȡ����������Ⱦɫ��Ϣ�������룬�ϲ�Ӧ�ü�¼�������룬�Ϳɱ��ֶ�Ⱦɫ��Ϣ����
@retval Ⱦɫ��Ϣ��������
@note
*/
TSF4G_API unsigned int tbus_get_dyedmsgid();

/**
@brief ��ȡͨ�����˶�����ʣ����Ϣ���ֽ�����
@param[in] a_hHandle tbus���
@param[in] a_iLocalAddr ��tbusͨ������ڱ��˵ĵ�ַ
@param[in] a_iPeerAddr ��tbusͨ������ڶԶ˵ĵ�ַ
@param[in] a_piInFlux ��ȡ����ͨ��(a_iLocalAddr <-- a_iPeerAddr)ʣ����Ϣ�ֽ�����ָ��
@param[in] a_piOutFlux ��ȡ���ͨ��(a_iLocalAddr --> a_iPeerAddr)ʣ����Ϣ�ֽ�����ָ��
@retval 0 -- success
@retval !0 -- failed
@note
*/
TSF4G_API int tbus_get_channel_flux(IN int a_hHandle, IN TBUSADDR a_iLocalAddr, IN TBUSADDR a_iPeerAddr,
											 OUT int *a_piInFlux, OUT int *a_piOutFlux);

/** @} */ 



/** @defgroup TBUS_ROUTE_MNG TBUS_·����Ϣ����
*  @{
*/


/**
  @brief get current package's route information

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in,out] a_ptRouteVec -- vector to be stored route information, the first one is the latest route
  @param[in,out] a_piCnt -- route vector count, if get package route ok, this value would be set to real route count

  @retval 0 -- success, get route information ok
  @retval !0 -- failed

  @pre a_ptRouteVec ����ΪNULL
  @pre a_piCnt ����ΪNULL�����䱣�������ֵ�������0
*/
TSF4G_API int tbus_get_pkg_route ( IN const int a_iHandle, INOUT HEADROUTE *a_ptRouteVec, INOUT int *a_piCnt ) ;

/**
  @brief set current package's route information

  @param[in] a_iHandle -- tbus handle, acquired by tbus_new()
  @param[in] a_ptRouteVec -- vector to store route information, the first one is the latest route
  @param[in] a_iCnt -- route vector count, indicated how many routes should be set into package header

  @retval 0 -- success, set route information ok
  @retval !0 -- failed

  @pre a_ptRouteVec ����ΪNULL
  @pre a_iCnt �䱣�������ֵ�������0
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
  @pre a_pvBuffer ����ΪNULL
  @pre a_piLen ����ΪNULL�����䱣�������ֵ�������0
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
  @pre a_pvBuffer ����ΪNULL
  @pre a_iLen �䱣�������ֵ�������0
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
  @pre a_piDstList ����ΪNULL
  @pre a_piVecCnt ����ΪNULL�����䱣�������ֵ�������0
*/
TSF4G_API int tbus_get_dst_list ( IN const int a_iHandle, INOUT unsigned int *a_piDstList, INOUT int *a_piVecCnt ) ;

/** @} */ 



/** @defgroup TBUS_LOG TBUS_��־����
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

