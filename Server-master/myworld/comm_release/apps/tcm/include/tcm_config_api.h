
/**
*
* @file     tcm_config_api.h 
* @brief   so访问数据接口
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

/*进程信息*/
struct tagAPIProcInst
{
    unsigned int iID;	/*整个业务系统procunit唯一id标示 */
    char szAddr[TCMAPI_MAX_ADDR_STR_LEN]; /*点分十进制表示地址串*/
    char szHostName[TCMAPI_MAX_HOST_NAME_LEN];	/*进程所在主机名称*/
    char szLayerName[TCMAPI_NODE_NAME_LEN];	   /*进程所在层名称*/
    char szProcName[TCMAPI_MAX_PROC_NAME_LEN]; 	/*   进程程序名,缺省取功能类描述名作为程序名 */
    char szWorkPath[TCMAPI_MAX_PATH_LEN];    	/*   此层次下进程的工作目录，此目录为相对路径 */	
};
typedef struct tagAPIProcInst APIPROCINST;
typedef struct tagAPIProcInst *LPAPIPROCINST;

/*主机信息*/
struct tagAPIHost
{
    char szHostName[TCMAPI_MAX_HOST_NAME_LEN]; 	/*   机器的主机名 */
    char szInnerIP[TCMAPI_MAX_IP_LEN];   /*此主机的内网ip*/
	int iOuterIpNum;
    char szOuterIP[TCMAPI_MAX_OUTER_IP_NUM][TCMAPI_MAX_IP_LEN];	/*此主机的外网ip*/
    char szGCIMShmKey[TCMAPI_BUS_MAX_KEY_STR_LEN];
    char szCustomAttr[TCMAPI_BUSS_CUSTOM_DATA_SIZE];
};
typedef struct tagAPIHost APIHOST;
typedef struct tagAPIHost *LPAPIHOST;


/*根据进程id获得进程信息结构，proc需要传入有效指针*/
int tcmapi_get_proc_inst(IN HAPIHANDLE handle, INOUT LPAPIPROCINST proc_inst, IN int proc_id);

int tcmapi_get_proc_inst_bypos(IN HAPIHANDLE handle, INOUT LPAPIPROCINST proc_inst, IN int idx);

/*根据进程id获得所在部署组自定义信息*/
const char* tcmapi_get_group_customattr(IN HAPIHANDLE handle, IN int proc_id);

/*根据进程id获得所在部署层的自定义信息*/
const char* tcmapi_get_layer_customattr(IN HAPIHANDLE handle, IN int proc_id);

/*根据主机名称获得主机信息结构，host需要传入有效指针*/
int tcmapi_get_host_by_name(IN HAPIHANDLE handle, INOUT APIHOST* host, IN const char* host_name);


//获取记录日志的 日志类句柄指针
void *tcmapi_get_logcatinst(IN HAPIHANDLE handle);

/**获取与指定进程  通过tbus进行通信的其他进程列表
@param[in] a_handle 服务器处理句柄
@param[in] a_iProcID 指定进程ID
@param[in] a_iProcs 保持相关进程id的数组
@param[in,out] a_piCount 保持进程数目的指针
	-	in 指定a_iProcs数组的长度
	-	out	获取实际的进程数目
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


