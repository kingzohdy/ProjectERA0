/**
*
* @file     torm_protocol_api.h  
* @brief    TORM服务器前端接入层使用的通信协议处理接口头文件
*	此接口完成消息编码和解码工作
* 
* @author jackyai  
* @version 1.0
* @date 2007-11-23 
*
*
* All rights reserved.
*
*/

#ifndef TORM_PROTOCOL_API_H
#define TORM_PROTOCOL_API_H

#include <stdio.h>
#include "tlog/tlog.h"
#include "tormapi/torm_error.h"
#include "tormapi/torm_dbprimaykey.h"
#include "tormapi/torm_protocol.h"

#define TORM_PROTOCOL_HEAD_META	"TORMPkgHead"  /**<ORM协议头部元数据描述的名称*/
#define TORM_PROTOCOL_BODY_META	"TORMPkgBody"	/**<ORM协议消息体元数据描述的名称*/
#define TORM_PROTOCOL_PKG_META	"TORMPkg"	/**<ORM协议消息元数据描述的名称*/


#define TORM_PRO_DEFAULT_LOGCNF_FILE	"tormapilog.xml"  /*tormapi缺省的日志系统配置文件名*/
#define TORM_PRO_LOGCAT_NAME	"tsf4g.tormapi"  /*tormapi缺省的日志系统中日志类的名称*/

#ifdef WIN32

# ifdef _DEBUG
#  define LIB_D    "_d"
# else
#  define LIB_D
# endif /* _DEBUG */

# if defined(LIB_D) 
# pragma comment( lib, "libtormprotocolapi"  LIB_D ".lib" )
# else
# pragma comment( lib, "libtormprotocolapi.lib" )
# endif

#endif



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**ORM协议消息处理句柄 
*/
typedef unsigned int TORMPROTOCOLHANDLE;
typedef TORMPROTOCOLHANDLE *LPTORMPROTOCOLHANDLE;


/**@defgroup  TORM_CS_METALIB_MANAGE TORM_协议处理句柄与元数据描述
* @{
*/

/** 根据存放在共享内存中的元数据描述库创建协议处理句柄
*@param[in,out] a_phTormHandler 获取创建出来的协议处理句柄的指针
*@param[in] a_pszKey	保存元数据库的共享内存块key值串
*@param[in] a_paszLibNames 描述存储数据的元数据库名称列表；
*@param[in] a_iLibNum 描述存储数据的元数据库的个数
*@param[in] a_fpError 如果创建失败，通过此句柄指定的文件来记录错误信息
*
*@note 本接口创建的协议处理句柄只能在同一个线程内使用，即多个线程不能共享同一个协议处理句柄
*@note 消息编解码接口使用两套元数据库:
*	- 其中一套元数据库描述ORM服务器与客户端之间的通信协议，此元数据库与本接口已经编译在一起,不需加载
*	- 另一套元数据库描述存储数据库系统中的数据，本接口使用tsf4g/mng模块来管理这些元数据库。调用本套
*	ORM协议接口的应用程序启动前必须使用tsf4g/mng/agent提供的工具加载到共享内存，由于这类元数据库容许同时存在多个，
*
*@note 协议处理句柄成功创建后，使用完毕必须调用torm_delete_handle释放句柄，否则会有内存资源泄露；
*@note 如果需要改变本套接口的日志系统设置，请在调用本接口之前调用torm_set_logcat接口
*
*@return 0 成功; 非零，失败
*
*@see torm_delete_handle
*@see torm_open_metalib
*@see tsf4g/mng
*
*@pre \e a_phTormHandler不能为NULL
*@pre \e a_pszKey 不能为NULL，所指的共享内存已经创建好，并且存储数据的元数据描述库已经加载到此共享内存中
*@pre \e a_paszLibNames 不能为NULL，且前a_iLibNum个元数据库名不为NULL
*@pre \e a_iLibNum 必须大于0
*@pre \e 如果存在多个描述存储数据的元数据描述库，则这些元数据库中出现的元数据结构名称不能重复
*
*@par 示例代码:
*  @code
	上层应用使用本套接口的大致流程如下：
	//step1:初始化协议处理句柄
	TORMPROTOCOLHANDLE hTormHandler;
	char *aszDataLib[] = {"ZoneDBData","WordDBData"};

	torm_create_handle(&hTormHandler, "666666", aszDataLib, 2,"torm_net");

	//step2: 交换协议版本
	TORMPKG stPkg;
	......
	stPkg.Head.HeadCmd.Cmd = TORM_EXGVER_REQ;
	......
	stPkg.Body.ExgVerReq.Version = TDR_METALIB_TORM_NET_VERSION;
	torm_make_req_msg(hTormHandler, &stNetMsgInfo, &stPkg, NULL, 0);
	send_orm_msg(stNetMsgInfo);
	........
	recv_orm_msg(stNetMsgInfo);
	....
	torm_attach_msg(hTormHandler, &stNetMsgInfo, &stPkg,0);

	//step3: 数据操作
	for(;;)
	{
		...........
	}
	

	//step4：使用完毕，释放协议处理句柄
	torm_delete_handle(&hTormHandler);

*  @endcode
*@see torm_set_logcat
*/
int torm_create_handle(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler, IN const char *a_pszKey, 
					   IN char *a_paszLibNames[], IN int a_iLibNum, IN FILE *a_fpError);


/** 根据二进制格式元数据描述库创建协议处理句柄
*@param[in,out] a_phTormHandler 获取创建出来的协议处理句柄的指针
*@param[in] a_paszLibFile 描述存储数据的二进制元数据库文件名列表；
*@param[in] a_iFileNum 描述存储数据的元数据库的个数
*@param[in] a_fpError 如果创建失败，通过此句柄指定的文件来记录错误信息
*
*@note 本接口创建的协议处理句柄只能在同一个线程内使用，即多个线程不能共享同一个协议处理句柄
*@note 消息编解码接口使用两套元数据库:
*	- 其中一套元数据库描述ORM服务器与客户端之间的通信协议，此元数据库与本接口已经编译在一起,不需加载
*	- 另一套元数据库描述存储数据库系统中的数据，本接口使用tsf4g/mng模块来管理这些元数据库。调用本套
*	ORM协议接口的应用程序启动前必须使用tsf4g/mng/agent提供的工具加载到共享内存，由于这类元数据库容许同时存在多个，
*
*@note 协议处理句柄成功创建后，使用完毕必须调用torm_delete_handle释放句柄，否则会有内存资源泄露；
*
*@return 0 成功; 非零，失败
*
*@see torm_delete_handle
*@see torm_open_metalib
*@see tsf4g/mng
*
*@pre \e 所有原数据库的名称不重名
*@pre \e a_phTormHandler不能为NULL
*@pre \e a_paszLibFile 不能为NULL，二进制的元数据描述库使用TDR的工具tdr生成
*@pre \e a_iFileNum 必须大于0
*@pre \e a_pszOrmProLib 不能为NULL,并且存储数据的元数据描述库已经加载到此共享内存中
*@pre \e 如果存在多个描述存储数据的元数据描述库，则这些元数据库中出现的元数据结构名称不能重复
*
*/
int torm_create_handle_files(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler, IN char *a_paszLibFile[], IN int a_iFileNum, IN FILE *a_fpError);

/*
对torm_create_handle_files 的封装，某个目录中的所有文件都为lib 的meta 文件
*/
int torm_create_handle_from_dir(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler, IN char *a_paszLibDir);


/**销毁协议处理句柄，以释放资源
*@param[in] a_phTormHandler 保存协议处理句柄的指针
*
*/
void torm_destroy_handle(INOUT LPTORMPROTOCOLHANDLE a_phTormHandler);



/**将协议处理句柄的内容导出到文件中以便调试
*@param[out] a_fpFile 保存导出信息的文件句柄
*@param[in] a_hTormHandler 保存协议处理句柄
*
*/
void torm_dump_handle(OUT FILE *a_fpFile,  IN TORMPROTOCOLHANDLE a_hTormHandler);


/**根据元数据名获取操作数据的元数据描述句柄
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pszMetaName 操作数据的元数据名称
*
*@pre \e a_phTormHandler不能为NULL
*@pre \e a_pszLibName 不能为NULL
*
*@return 如果存在指定的名字的元数据，则返回其句柄；否则返回NULL
*/
LPTDRMETA torm_get_meta(IN TORMPROTOCOLHANDLE a_hTormHandler, IN const char *a_pszMetaName);

/**
@brief set log category instance for tormapi log system

@param[in] a_pstLogCat -- category instance of tormapi log system@note 
*/
void torm_set_logcat( IN LPTLOGCATEGORYINST a_pstLogCat);

/**@} */ /*  defgroup  TORM_CS_METALIB_MANAGE TORM_元数据资源管理*/


/**@defgroup  TORM_CS_MSG_PACK_UNPACK_PUB TORM_协议消息处理_cs公共接口
* @{
*/
/** 
*/

/**从网络消息中解析出ORM协议消息的头部
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pstPkgHead ORM协议消息头部结构的指针
*@param[in] a_pszNetMsg 保存网络消息的缓冲区指针
*@param[in,out] a_piMsgLen 网络消息缓冲区长度信息 
*	- 输入 待解析网络消息长度
*	- 输出 以解析的网络消息长度
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_pstPkgHead不能为NULL
*@pre \e a_pszNetMsg 不能为NULL
*@pre \e a_piMsgLen 不能为NULL
*@pre \e *a_piMsgLen必须大于0
*@pre \e a_hTormHandler 不能为NULL
*
*@return 0：成功；非零：失败
*/
int torm_fetch_pkghead(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTORMPKGHEAD a_pstPkgHead, IN const char *a_pszNetMsg, INOUT int *a_piMsgLen, IN int a_iVersion);

/**以可视化的打印协议数据头部，打印输出到指定文件中
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_fpFile 保存打印数据的文件句柄
*@param[in] a_pstPkgHead 消息头部结构的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_fpFile不能为NULL
*@pre \e a_pstPkg 不能为NULL
*@pre \e a_hTormHandler 不能为NULL
*
*@return 0：成功；非零：失败
*/
int torm_fprintf_pkghead(IN TORMPROTOCOLHANDLE a_hTormHandler, IN FILE* a_fpFile, IN LPTORMPKGHEAD a_pstPkgHead, IN int a_iVersion);

/**以可视化的打印协议数据头部，打印输出到指定缓冲区中
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pszBuf 保存打印数据的缓冲区句柄
*@param[in,out] a_piBuf 保存打印数据的缓冲区长度信息
*	- 输入 缓冲区可以使用最大长度
*	- 输出 缓冲区已使用长度
*@param[in] a_pstPkgHead 消息头部结构的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_pszBuf不能为NULL
*@pre \e a_pstPkg 不能为NULL
*@pre \e a_pstPkgHead 不能为NULL
*@pre \e a_hTormHandler 不能为NULL
*@pre \e *a_piBuf必须大于0 
*
*@return 0：成功；非零：失败
*/
int torm_sprintf_pkghead(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszBuf, INOUT int* a_piBuf, IN LPTORMPKGHEAD a_pstPkgHead, IN int a_iVersion);


/**以可视化的打印协议数据，打印输出到指定文件中
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_fpFile 保存打印数据的文件句柄
*@param[in] a_pstPkg 消息结构的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_fpFile不能为NULL
*@pre \e a_pstPkg 不能为NULL
*@pre \e a_hTormHandler 不能为NULL
*
*@return 0：成功；非零：失败
*/
int torm_fprintf_pkg(IN TORMPROTOCOLHANDLE a_hTormHandler, IN FILE* a_fpFile, IN LPTORMPKG a_pstPkg, IN int a_iVersion);

/**以可视化的打印协议数据，打印输出到指定缓冲区中
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pszBuf 保存打印数据的缓冲区句柄
*@param[in,out] a_piBuf 保存打印数据的缓冲区长度信息
*	- 输入 缓冲区可以使用最大长度
*	- 输出 缓冲区已使用长度
*@param[in] a_pstPkg 消息结构的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_pszBuf不能为NULL
*@pre \e a_pstPkg 不能为NULL
*@pre \e a_hTormHandler 不能为NULL
*@pre  \e a_piBuf 不能为NULL
*@pre \e *a_piBuf必须大于0 
*
*@return 0：成功；非零：失败
*/
int torm_sprintf_pkg(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszBuf, INOUT int* a_piBuf, IN LPTORMPKG a_pstPkg, IN int a_iVersion);

/**将协议数据以XML格式输出到指定缓冲区中
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pszXml 保存打印数据的缓冲区信息
*@param[in,out] a_piXml 保存打印数据的缓冲区长度信息
*	- 输入 缓冲区可以使用最大长度
*	- 输出 缓冲区已使用长度
*@param[in] a_pstPkg 消息结构的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_pszXml不能为NULL
*@pre \e a_pstPkg 不能为NULL
*@pre \e a_hTormHandler 不能为NULL
*@pre \e a_piXml 不能为NULL
*@pre \e *a_piXml 必须大于0
*
*@return 0：成功；非零：失败
*/
int torm_output_pkg(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszXml, INOUT int* a_piXml, IN LPTORMPKG a_pstPkg, IN int a_iVersion);
/**@} */ /*  defgroup  TORM_CS_MSG_PACK_UNPACK TORM_协议消息编解码_公共*/


/**@defgroup  TORM_CS_MSG_PACK_UNPACK_CLI TORM_协议消息处理_客户端使用
* @{
*/

/**根据协议结构体数据和操作数据生成ORM请求网络消息
*@param[in] a_hTormHandler 协议处理句柄
*@param[in,out]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*	- 输出  a_pstNet.iBuff  实际编码网络信息的总长度
*@param[in] a_pstPkg 消息结构的指针
*@param[in]	a_pstMetaData 存储操作数据的缓冲区信息,对于Cmd为TORM_EXGVER_REQ和TORM_SVRINFO_REQ的请求，本参数可以为NULL
*	- 输入	a_pstMetaData.pszBuff 指定保存操作数据的缓冲区起始地址
*	- 输入  a_pstMetaData.iBuff	指定缓冲区的大小
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@note 本接口主要是为生成数据操作（插入，更新、查找、删除）请求服务的,对于Cmd为TORM_EXGVER_REQ和TORM_SVRINFO_REQ的请求可以调用
*	tdr_hton生成网络消息。本接口的主要目的是自动为上层生成TORMOPERINFO结构中的PriKeyInfo，OperContent内容，上层只需指定TORMOPERINFO结构中
*	的EntryNameList，WhereDef，ResFlag字段即可。
*
*@note 数据操作（插入，更新、查找、删除）请求TORMOPERINFO结构EntryNameList字段用来描述上层感兴趣的数据成员的元数据描述名称列表，如果此列表为空，则表示
*	对操作数据的所有成员感兴趣。
*
*@note TORMOPERINFO结构WhereDef字段的用意：
	- 数据操作请求再ORM端会生成相应的SQL语句，
	- 插入，更新、查找、删除四种SQL操作语句可以总结为分成四部分的模式：Operate(Select/update/insert/delete) data_description table_description restriction_description(WHERE,ORDER,LIMIT)
	- WhereDef字段保存restriction_description这一部分的信息，这样即可对上层应用屏蔽具体数据与SQL转换细节，同时又为上层应用灵活指定操作约束提供了可能
*
*@pre \e 对于数据操作（插入，更新、查找、删除）请求，如果TORMOPERINFO结构WhereDef字段不为空，则必须包含数据操作所需的Where条件约束信息
*@pre \e 自定义数据操作（插入，更新、查找、删除）请求TORMOPERINFO结构WhereDef字段，需要了解数据在数据库中的表结构定义，以使用表结构定义中的列名来定义约束串F
*
*@return 0：成功；非零：失败
*
*@par 示例代码:
*  @code
	假定某个存储数据的原数据描述下：
	<struct name="UserBaseInfo" version="10" primarykey="Uin,RoleName" >
		<entry name="Uin" type="uint" />
		<entry name="RoleName" type="longlong" />
		<entry name="Level" type="int">
		<entry name="LastLogin" type="datetime" />
	</struct>

	则ORM客户端根据Uin和RoleName查找同角色信息的伪代码如下：
	TORMPKG stPkg;
	USERBASEINFO stBaseInfo;
	char szNet[32765];
	TDRDATA stNetInfo;
	TDRDATA stMetaData;
	int iRet;

	stPkg.Head.HeadComm.Magic = TORM_MAGIC;
	stPkg.Head.HeadComm.InstID = 100;
	stPkg.Head.HeadCmd.Cmd = TORM_SELECT_REQ;
	strcpy(stPkg.Head.HeadCmd.MetaNameVer.MetaName,"UserBaseInfo");
	stPkg.Head.HeadCmd.MetaNameVer.MetaVer = 10;
	stPkg.Head.HeadApp.Seq = 100001
	stPkg.Head.HeadApp.ObjID = 2;

	stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryNameNum = 0;

	stBaseInfo.Uin = 10000;
	strcpy(stBaseInfo.RoleName, "test")

	stNetInfo.pszBuff = &szNet[0];
	stNetInfo.iBuff = sizeof(szNet);
	stMetaData.pszBuff =  &stBaseInfo;
	stBaseInfo.iBuff = sizeof(stBaseInfo);
	iRet = torm_make_req_msg(hTormHandler, &stNetInfo, &stPkg, &stBaseInfo,0);

	指定条件的查找伪代码如下：
	stPkg.Head.HeadComm.Magic = TORM_MAGIC;
	stPkg.Head.HeadComm.InstID = 100;
	stPkg.Head.HeadCmd.Cmd = TORM_SELECT_REQ;
	strcpy(stPkg.Head.HeadCmd.MetaNameVer.MetaName,"UserBaseInfo");
	stPkg.Head.HeadCmd.MetaNameVer.MetaVer = 10;
	stPkg.Head.HeadApp.Seq = 100001;
	stPkg.Head.HeadApp.ObjID = 2;

	stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryNameNum = 2;
	strcpy(stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryName[0],"Uin");
	strcpy(stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryName[0],"RoleName");

	strcpy(stPkg.Body.SelectReq.OperInfo.WhereDef,"Where Level>110 limit=100 Order by asc");

	stBaseInfo.Uin = 10000;

	stNetInfo.pszBuff = &szNet[0];
	stNetInfo.iBuff = sizeof(szNet);
	stMetaData.pszBuff =  &stBaseInfo;
	stBaseInfo.iBuff = sizeof(stBaseInfo);
	iRet = torm_make_req_msg(hTormHandler, &stNetInfo, &stPkg, &stBaseInfo,0);


*  @endcode
*@pre \e a_hTormHandler 不能为NULL
*@pre \e a_pstNet 不能为NULL
*@pre \e a_pstNet.pszBuff 不能为NULL
*@pre \e a_pstNet.iBuff 必须大于0
*@pre \e a_pstPkg 不能为NULL
*/
int torm_make_req_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, IN LPTORMPKG a_pstPkg, IN LPTDRDATA a_pstMetaData, IN int a_iVersion);

/** 功能和torm_make_req_msg基本相同，只是在构造操作项的时候和torm_make_req_msg相反。
  如果:
      stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryNameNum = 1;
      strcpy(stPkg.Body.SelectReq.OperInfo.EntryNameList.EntryName[0],"LastLogin");
则: 表示操作项是除了LastLogin 字段以外的所有字段
*/
int torm_make_req_msg_r(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, IN LPTORMPKG a_pstPkg, IN LPTDRDATA a_pstMetaData, IN int a_iVersion);




/**加载ORM响应消息
* － 对于Cmd不是TORM_SELECT_RES的消息，本接口直接将网络消息解析到a_pstPkg结构中
* -  对于Cmd为TORM_SELECT_RES的消息，本接口将从网络消息中解析出除查找结果数据以外成员，通过调用torm_fetch_one_meta接口可以从加载的响应消息中分析出操作数据
*@param[in] a_hTormHandler 协议处理句柄
*@param[in,out]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*	- 输出  a_pstNet.iBuff  实际编码网络信息的总长度
*@param[in,out] a_pstPkg 保存解析后消息信息的结构体的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_hTormHandler不能为NULL
*@pre \e a_pstNet 不能为NULL
*@pre \e a_pstNet.pszBuff 不能为NULL
*@pre \e a_pstNet.iBuff 必须大于0
*@pre \e a_pstPkg 不能为NULL
*
*@return 0：成功；非零：失败
*
*@see torm_fetch_one_meta
*/
int torm_attach_res_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, INOUT LPTORMPKG a_pstPkg, IN int a_iVersion);

/**从加载的ORM 查找响应中取出一个结构的数据
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pszBuf 保存查找数据的缓冲区
*@param[in] a_iBuf 保存打印数据的缓冲区首地址
*	- 输入 缓冲区可以使用最大长度
*
*@note 调用此接口读取一个特定数据,不管调用是否成功,以后不能再通过此接口读取到此特定数据
*@pre \e a_hTormHandler不能为NULL
*@pre \e a_pszBuf 不能为NULL
*@pre \e a_iBuf 其所指缓冲区大小必须能满足存储一个元数据结构体
*@pre \e 之前必须调用torm_attach_res_msg加载ORM响应消息，如果再一次调用torm_attach_res_msg,则前一次加载消息中的数据将失效,因此上层应用必须
* 确保在再一次调用torm_attach_res_msg之前取出所有所有数据
*
**@return 0：成功；非零：失败
*
*@return 0：成功；非零：失败
*
*@see torm_attach_res_msg
*@par 示例代码:
*  @code
	以Select响应为例，说明消息处理和数据获取的伪代码：
	char szMsgBuff[246000];
	int iMsgLen;
	TDRDATA stNetInfo

	//接收网络消息
	recv_orm_msg(fio, &szMsgBuff[0], &iMsgLen);
	........

	//分发消息
	TORMPKG stPkg;
	int iRet;

	stNetInfo.pszBuff = &szMsgBuff[0];
	stNetInfo.iBuff = iMsgLen;
	
	iRet = torm_attach_res_msg(hTormHandler, &stNetInfo, &stPkg, 0);
	switch(stPkg.Head.HeadCmd.Cmd)
	{
	case TORM_SELECT_RES:
		iRet = process_select_res(&stPkg);
		break;
		......
	default:
		break;
	}

	//响应消息处理
	int process_select_res(LPTORMPKG pstPkg)
	{
		LPTDRMETA pstMeta;
		char *pszMetaBuff;
		int iSize;
		......

		//根据查找总数据数，分配或获取保存数据的内存空间
		pstMeta = torm_get_meta(hTormHandler, pstPkg->Head.HeadCmd.MetaNameVer.MetaName)
		if (m_pszMetaBuff == NULL)
		{
			m_pszMetaBuff ＝ get_buff(pstPkg->Body.SelectRes.SelectResult.ResultTotal* tdr_get_meta_size(pstMeta));
		}

		pszMetaBuff = get_free_meta_buff(m_pszMetaBuff);
		for(i = 0; i < pstPkg->Body.SelectRes.SelectResult.ResultNum;i++)
		{
			iSize = tdr_get_meta_size(pstMeta);
			torm_fetch_one_meta(hTormHandler, pszMetaBuff, &iSize);
			pszMetaBuff += tdr_get_meta_size(pstMeta);
		}
	}
*  @endcode
*/
int torm_fetch_one_meta(IN TORMPROTOCOLHANDLE a_hTormHandler, IN char* a_pszBuf, IN int a_iBuf);

/** 从被torm_attach_res_msg封装的操作数据中解码出数据到内存结构中
*@param[in] a_hTormHandler 协议处理句柄
*@param[in] a_pstMeta 描述内存结构的元数据结构句柄
*@param[in] a_pstOperInfo 封装的操作数据结构体指针
*@param[in,out]	a_pstHost 存储数据的内存缓冲区结构体指针
*	- 输入	a_pstHost.pszBuff 存储数据的内存缓冲区起始地址
*	- 输入  a_pstHost.iBuff	缓冲区可使用大小
*	- 输出  a_pstHost.iBuff  实际使用的内存缓冲区大小
*@param[in] a_iVersion 数据解码剪裁版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_hTormHandler不能为NULL
*@pre \e a_pstHost 不能为NULL
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_pstOperInfo 不能为NULL
*
*@return 0：成功；非零：失败
*
*@see torm_attach_res_msg
*/
int torm_unpack_operinfo(IN TORMPROTOCOLHANDLE a_hTormHandler, IN LPTDRMETA a_pstMeta, IN LPTORMOPERINFO a_pstOperInfo, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/**@} */ /*  defgroup  TORM_CS_MSG_PACK_UNPACK TORM_协议消息编解码_客户端使用*/


/**@defgroup  TORM_CS_MSG_PACK_UNPACK_SVR TORM_协议消息处理_服务器使用
* @{
*/
/**从网络消息中分析出主键信息 
*@param[in] a_hTormHandler 协议处理句柄
*@param[in]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*@param[in] a_pstPkgHead 此网络消息的头部结构体指针.
*	-	如果上层应用已经从消息分析出消息头部，通过此参数本接口将加快处理过程；
*	-	此参数为NULL，则本接口将从消息中分析出消息头部
*@param[out] a_pstKeyInfo 保存主键信息的结构体指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_hTormHandler不能为NULL
*@pre \e a_pstNet 不能为NULL
*@pre \e a_pstNet.pszBuff 不能为NULL
*@pre \e a_pstNet.iBuff 必须大于0
*@pre \e a_pstPkgHead 不能为NULL
*@pre \e a_pstKeyInfo 不能为NULL
*/
int  torm_get_primaykey(IN TORMPROTOCOLHANDLE a_hTormHandler, IN LPTDRDATA a_pstNet, IN LPTORMPKGHEAD a_pstPkgHead, OUT LPTORMPRIMAYKEYINFO a_pstKeyInfo, IN int a_iVersion);

/**ORM协议消息编码
*@param[in] a_hTormHandler 协议处理句柄
*@param[in,out]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*	- 输出  a_pstNet.iBuff  实际编码网络信息的总长度
*@param[in] a_pstPkg 消息结构的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@return 0：成功；非零：失败
*
*/
int torm_pack_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, IN LPTORMPKG a_pstPkg, IN int a_iVersion);

/**ORM协议消息解码
@param[in] a_hTormHandler 协议处理句柄
*@param[in,out]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*	- 输出  a_pstNet.iBuff  实际编码网络信息的总长度
*@param[in,out] a_pstPkg 保存解析后消息信息的结构体的指针
*@param[in] a_iVersion 协议版本号，如果其值为0，则按当前最高版本进行解析
*
*@pre \e a_hTormHandler不能为NULL
*@pre \e a_pstNet 不能为NULL
*@pre \e a_pstPkg 不能为NULL
*
*@return 0：成功；非零：失败
*
*@see torm_fetch_one_meta
*/
int torm_unpack_msg(IN TORMPROTOCOLHANDLE a_hTormHandler, INOUT LPTDRDATA a_pstNet, INOUT LPTORMPKG a_pstPkg, IN int a_iVersion);


/**@} */ /*  TORM_CS_MSG_PACK_UNPACK_SVR TORM_协议消息处理_服务器使用*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
