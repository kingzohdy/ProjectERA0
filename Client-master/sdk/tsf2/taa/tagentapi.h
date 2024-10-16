/*
**  @file $RCSfile: tagentapi.h,v $
**  general description of this module
**  $Id: tagentapi.h,v 1.9 2008/11/12 06:16:39 sean Exp $
**  @author $Author: sean $
**  @date $Date: 2008/11/12 06:16:39 $
**  @version $Revision: 1.9 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef __AGENT_API__
#define __AGENT_API__


#ifdef WIN32
# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */


#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libtagentapi"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libtagentapi.lib")
#endif

#endif/*#ifdef WIN32*/



#ifdef __cplusplus
extern "C" 
{
#endif

#if defined(__i386__) && !defined(_WIN64)
#define uint2korr(A)    (*((uint16_t *) (A)))
#define uint4korr(A)    (*((unsigned long *) (A)))

#define int2store(T,A)  *((uint16_t*) (T))= (uint16_t) (A)
#define int4store(T,A)  *((long *) (T))= (long) (A)
#endif /* __i386__ */

#define PKGHEADLEN sizeof(tagPkgHead)
#define MSG_MAGIC				(unsigned short)0x8899	//包头Magic
#define ID_MSG_HELLO			0x00	//心跳协议 agent->centerd or dirtyd->centerd
#define ID_MSG_AGENTREGISTER	0x01	//agent注册协议 agent->centerd
#define ID_MSG_MASTEREGISTER	0x02	//master注册协议 dirtyd->centerd

#define ID_MSG_FILEUPDATE		0x0a	//文件下发通知协议 dirty->agent
#define ID_MSG_FILEREQUEST		0x0b	//文件请求协议 agent->dirty
#define ID_MSG_FILEPUSH			0x0c	//文件下发协议 dirty->agent
#define ID_MSG_FILEREPORT		0x0d	//文件获取成功应答 agent->dirty
#define ID_MSG_CHROMO			0xff	//染色协议 测试网络通路
#define ID_MSG_TRANSFER			0xfff0	//网络透传

#define ID_CMD_DISPATCH			0x00	//消息广播
#define ID_CMD_UP				0x01	//消息根据包头AppID转发
#define ID_CMD_DOWN				0x02	//消息根据包头destIP转发

#define ID_BUSSID_BROADCAST		0xffff	//appid内的消息群发

#define ERROR_NET_ERROR			(-0x01)
#define ERROR_HEAD_ERROR		(-0x02)
#define ERROR_SIZE_ERROR		(-0x03)

#define ERROR_BUS_NODATA		(-0x11)
#define ERROR_LOGIC				(-0x21)

#define ERROR_FILE_NOEXIST		(-0x31)
#define ERROR_FILE_POSITION		(-0x32)

#define MSG_MAGIC_POS 14

typedef struct tagPkgHead
{
	unsigned short uLen;
	unsigned short uMsgCmd;

	unsigned int uiAppid;
	unsigned int uiBusid;
	
	unsigned char cMsgVer;
	unsigned char cType;
	unsigned short uMagic;
	
	unsigned int uiSourceIP;
	unsigned int uiDestIP;
} tagPkgHead;

#define ID_APPID_CENTERD        0x00    //默认centerd 	appid
#define ID_APPID_DIRTY          0x01    //默认dirtyd  	appid
#define ID_APPID_BUSCONFIG      0x02    //默认busconfig appid
#define ID_APPID_TCONND         0x03    //默认tconnd    appid

#define TAGENT_DEFAULT_BUSINESS_ID	0	//default business id

typedef struct tagExchnageHead {
	int iExchangeSize;
	int iBlockSize;
	int iCreateTime;
	char szDescription[64-12];
} EXCHANGEHEAD, *LPEXCHANGEHEAD;

typedef struct tagExchangeBlock
{
	char *pBlk;					//block起始地址
	unsigned int uiAppid;		//功能ID
	unsigned int uiBusid;		//业务ID
} EXCHANGEBLOCK, *LPEXCHANGEBLOCK;

typedef struct tagExchangeMng 
{
	char *pShm;					
	void *aSem;					
	
	size_t size;				
	size_t used;
	
	EXCHANGEHEAD 	stExcHead;
	LPEXCHANGEBLOCK	*ppstBlock;
	
} EXCHANGEMNG, *LPEXCHANGEMNG;

#define EXCHANGESIZE 1024*1024
#define EXCBLOCKSIZE 4096

#define EXCHANGEKEY 0xFFEE0001
#define EXCHEADSIZE 64

/*********************************************************************
  Fuction         	:  agent_api_init
  Descption       	:  初始化agent APIs
  Input           	:  ppstExcMng		-- 句柄指针指针
  Output          	:  ppstExcMng		-- 返回句柄实例
  Return          	:  0            	-- 注册成功
                      -1           		-- 内存类错误
  Note            	:  ppstExcMng 初始化为NULL，agent_api_init会分配
  					   内存
*********************************************************************/
int agent_api_init(LPEXCHANGEMNG *ppstExcMng);

/*********************************************************************
  Fuction         	:  agent_api_register
  Descption       	:  注册到busAgent
  Input           	:  pstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   uiBusid			-- 业务ID
  Output          	:  ppstExcMng		-- 返回句柄实例
  Return          	:  0            	-- 注册成功
                      -1           		-- 注册失败
  Note            	:  ppstExcMng 初始化为NULL
*********************************************************************/
int agent_api_register(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);
/*********************************************************************
  Fuction         	:  agent_api_get_bussid
  Descption       	:  根据appid获得bussid
  Input           	:  ppstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   astBussID		-- 业务ID数组
			   		   piSize			-- 业务ID个数指针
  Output          	:  astBussID		-- 业务ID
			   		   piBussSize		-- 业务 ID个数
  Return          	:  0            	-- 获取成功
                       -1       		-- 获取失败，可能原因是pstExcMng
                       					   没有初始化
  Note            	:  
*********************************************************************/
int agent_api_get_bussid(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int astuiBusid[], int *piSize);

/*********************************************************************
  Fuction         	:  agent_api_get_blocks
  Descption       	:  获取已经注册的blocks信息
  Input           	:  ppstExcMng		-- 句柄指针
                       ppstBlock		-- block数组指针的指针
			   		   piBlockSize		-- block个数
  Output          	:  ppstBlock		-- block数组指针的指针
			   		   piBlockSize		-- block个数
  Return          	:  0            	-- 获取成功
                       -1       		-- 获取失败，可能原因是pstExcMng
                       					   没有初始化
  Note            	:  ppstBlock 需要free
*********************************************************************/
int agent_api_get_blocks(LPEXCHANGEMNG pstExcMng, LPEXCHANGEBLOCK **pppstBlock, int *piBlockSize);

/*********************************************************************
  Fuction         	:  agent_api_get_freebufptr
  Descption       	:  根据appid和bussid返回freebuf指针
  Input           	:  ppstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   uBussSize		-- 业务ID
  Output          	: 
  Return          	:  0            	-- 获取成功
                       -1       		-- 获取失败
  Note            	:  
*********************************************************************/
char *agent_api_get_freebufptr(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);

/*********************************************************************
  Fuction         	:  agent_api_update_exchange
  Descption       	:  修改Exchange区信息
  Input           	:  pstExcMng	-- 句柄
  					   uiAppid		-- 应用ID
  					   uiBusid		-- 业务ID
                       szbuf		-- 源缓冲区大小
  Output          	:  off_t		-- FreeBuffer偏移
                       size			-- szbuf长度
  Return          	:  0            -- 更新成功
                      -1       		-- 更新失败
  Note            	:  off+size 必须小于 4096-64
*********************************************************************/
int agent_api_update_exchange(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid, char *szbuf, unsigned int off, size_t size);
/*********************************************************************
  Fuction         	:  agent_api_destroy
  Descption       	:  注销句柄
  Input           	:  pstExcMng		-- 句柄
  Output          	:  
  Return          	: 
  Note            	:  
*********************************************************************/

void agent_api_destroy(LPEXCHANGEMNG pstExcMng);


#ifdef __cplusplus
}
#endif

#endif /* __TAGENT_API__*/
