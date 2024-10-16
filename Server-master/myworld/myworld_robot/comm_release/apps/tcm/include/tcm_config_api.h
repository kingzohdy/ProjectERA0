
/**
*
* @file     tcm_config_api.h 
* @brief   so�������ݽӿ�
* 
* @author vinsonzuo
* @version 1.0
* @date 2009-5-13
*
*
* All rights reserved.
*
*/


#ifndef __TCM_CONFIG_API__
#define __TCM_CONFIG_API__


#ifdef __cplusplus
extern "C" 
{
#endif

#include "tcm_config_plunin.h"

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif



#define TCMAPI_MAX_ADDR_STR_LEN 256
#define TCMAPI_MAX_HOST_NAME_LEN 128
#define TCMAPI_MAX_IP_LEN 64
#define TCMAPI_MAX_OUTER_IP_NUM 8
#define TCMAPI_NODE_NAME_LEN 64
#define TCMAPI_BUSS_CUSTOM_DATA_SIZE 1024
#define TCMAPI_MAX_PROC_NAME_LEN 64
#define TCMAPI_MAX_PATH_LEN  256
#define TCMAPI_BUS_MAX_KEY_STR_LEN 256

/*������Ϣ*/
struct tagAPIProcInst
{
    unsigned int iID;	/*����ҵ��ϵͳprocunitΨһid��ʾ */
    char szAddr[TCMAPI_MAX_ADDR_STR_LEN]; /*���ʮ���Ʊ�ʾ��ַ��*/
    char szHostName[TCMAPI_MAX_HOST_NAME_LEN];	/*����������������*/
    char szLayerName[TCMAPI_NODE_NAME_LEN];	   /*�������ڲ�����*/
    char szProcName[TCMAPI_MAX_PROC_NAME_LEN]; 	/*   ���̳�����,ȱʡȡ��������������Ϊ������ */
    char szWorkPath[TCMAPI_MAX_PATH_LEN];    	/*   �˲���½��̵Ĺ���Ŀ¼����Ŀ¼Ϊ���·�� */	
};
typedef struct tagAPIProcInst APIPROCINST;
typedef struct tagAPIProcInst *LPAPIPROCINST;

/*������Ϣ*/
struct tagAPIHost
{
    char szHostName[TCMAPI_MAX_HOST_NAME_LEN]; 	/*   ������������ */
    char szInnerIP[TCMAPI_MAX_IP_LEN];   /*������������ip*/
	int iOuterIpNum;
    char szOuterIP[TCMAPI_MAX_OUTER_IP_NUM][TCMAPI_MAX_IP_LEN];	/*������������ip*/
    char szGCIMShmKey[TCMAPI_BUS_MAX_KEY_STR_LEN];
    char szCustomAttr[TCMAPI_BUSS_CUSTOM_DATA_SIZE];
};
typedef struct tagAPIHost APIHOST;
typedef struct tagAPIHost *LPAPIHOST;


/*���ݽ���id��ý�����Ϣ�ṹ��proc��Ҫ������Чָ��*/
int tcmapi_get_proc_inst(IN HAPIHANDLE handle, INOUT LPAPIPROCINST proc_inst, IN int proc_id);

int tcmapi_get_proc_inst_bypos(IN HAPIHANDLE handle, INOUT LPAPIPROCINST proc_inst, IN int idx);

/*���ݽ���id������ڲ������Զ�����Ϣ*/
const char* tcmapi_get_group_customattr(IN HAPIHANDLE handle, IN int proc_id);

/*���ݽ���id������ڲ������Զ�����Ϣ*/
const char* tcmapi_get_layer_customattr(IN HAPIHANDLE handle, IN int proc_id);

/*�����������ƻ��������Ϣ�ṹ��host��Ҫ������Чָ��*/
int tcmapi_get_host_by_name(IN HAPIHANDLE handle, INOUT APIHOST* host, IN const char* host_name);


//��ȡ��¼��־�� ��־����ָ��
void *tcmapi_get_logcatinst(IN HAPIHANDLE handle);

/**��ȡ��ָ������  ͨ��tbus����ͨ�ŵ����������б�
@param[in] a_handle ������������
@param[in] a_iProcID ָ������ID
@param[in] a_iProcs ������ؽ���id������
@param[in,out] a_piCount ���ֽ�����Ŀ��ָ��
	-	in ָ��a_iProcs����ĳ���
	-	out	��ȡʵ�ʵĽ�����Ŀ
@return zero: succ;  nonzero: failed
*/
int tcmapi_get_procs_with_buscommunication(IN HAPIHANDLE a_handle, IN int a_iProcID, IN int a_iProcs[],
									   INOUT int *a_piCount);


//for test
int tcmapi_get_host_num(IN HAPIHANDLE handle);
int tcmapi_get_proc_num(IN HAPIHANDLE handle);


#ifdef __cplusplus
}
#endif

#endif /* __TCM_CONFIG_API__*/


