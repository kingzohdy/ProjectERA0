#ifndef _CS_CGI_H_
#define _CS_CGI_H_

#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "op_log_meta.h"
#include "proto_ss.h"

#if defined(WIN32)
#	if defined(_USRDLL)
#		define CS_CGI_API __declspec(dllexport)
#	else
#		define CS_CGI_API extern __declspec(dllimport)
#	endif
#else
#	define CS_CGI_API
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**初始化函数, 一般进程只需调用一次
*@param[in]   szSSBinPath …proto_ss.tdr的路径	
*@param[out]   ppHandle …句柄指针	
*@param[in]   szXmlPath …cs_cgi.xml配置文件的路径	

*@retval 0   succ
*@retval <0  fail  错误原因参见enum tagCSErr
*/
CS_CGI_API
int init_interface(const char *szSSBinPath, int **ppHandle,
                   const char *szXmlPath);

/**退出函数，一般进程只需要调用一次
*@param[in]   pHandle …	不能为空,init_interface初始化的句柄指针
*/
CS_CGI_API
void destory_interface(int *pHandle);

/**打印数据结构里面的数据
*@param[in]   pHandle …	不能为空,init_interface初始化的句柄指针
*@param[in]   pszMetaName 
*@param[in]   pvData 结构体的buf
*@param[in]   iDataSize 结构体的大小
*/
CS_CGI_API
int cs_print_data(int *pHandle, const char *pszMetaName, const void *pvData,
	              int iDataSize);
CS_CGI_API
int cs_sprintf(int *pHandle, const char *pszMetaName, const void *pvData,
               int iDataSize, char *pszBuf, int iBufSize);

CS_CGI_API
int cs_sprint_xml(int *pHandle, const char *pszMetaName, const void *pvData,
               int iDataSize, char *pszBuf, int iBufSize);


/**初始化blob数据解析接口
@param[in]  szMetaPath  路径proto_ss.tdr或其他tdr的路径
@param[out] ppHandle  句柄指针 不能为空,解析数据时需传入此指针

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int init_analy_interface(const char* szMetaPath, int **ppHandle);

/**解析Blob数据
*
@param[in]  pHandle  通过init_analy_interface获得的指针
@param[in]  pszBlobMetaName  Blob数据块对应的Meta名, 这个meta名需要看原始
                   proto_cs.xml,proto_ss.xml或op_log_meta.xml, proto_comm.xml中可得到，
                   这些文件也拷到cs_cgi_dll目录下。例如，要解析LogRoleSnapshot
                   中的LogRoleData字段，其metaname参见op_log_meta.xml中的
                   <entry name="OpRoleSnapshot" type="OpRoleSnapshot" id="OP_LOG_ROLE_SNAPSHOT" version="158" />
                   再到proto_comm.xml中找
                   <struct name="OpRoleSnapshot" version="158" uniqueentryname="false" >
                   其中
                   <entry name="LogRoleData" type="RoleData"  version="160"/>
                   所以，LogRoleData这个blob字段的metaname就是RoleData
@param[in]  pszBlobBuff  Blob buff 指针
@param[in]  iBlobBuff  Blob数据块大小
@param[out]  pszXml  char指针 不能为空,保存导出的XML格式的字符串
@param[in]  iXml  pszXml指向的数组大小

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int analy_interface(int *pHandle, char *pszBlobMetaName,char *pszBlobBuff
								,int iBlobBuff, char *pszXml, int* iXml);

/**解析接口结束函数
*
@param[out]  *ppHandle  指针 解析数据时需传入的指针

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int fini_analy_interface(int **ppHandle);

/**获取区服信息接口, 一般进程只需调用一次
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in,out]   pstRegionWorldInfo …函数外部定义一个结构REGIONWORLDINFO stInfo，将其
                           地址传入&stInfo

*@retval 0   succ
*@retval <0  fail  错误原因参见enum tagCSErr
*/
CS_CGI_API
int cs_get_regionworld_info(int *pHandle, REGIONWORLDINFO *pstRegionWorldInfo);


/*-------------------------------------------------------*/

/**查询全区全服帐号名
 */
struct _FindAccountContext;
typedef struct _FindAccountContext *FIND_ACCOUNT_HANDLE;


/**通过帐号名模糊查询所有区用户的帐号名的第一部分
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   pszLoginName …	登录名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	帐号信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_find_next_account
    需要调用cs_close_handle 关闭查询句柄pFind，否则会内存泄漏
*/
CS_CGI_API
int cs_find_first_account_by_loginname(int *pHandle, const char *pszLoginName,
                                       FIND_ACCOUNT_HANDLE *pFind,
                                       ACCFINDRES *pstOut, int iTimeOut);


/**通过角色名模糊查询所有区拥有此角色名的帐号名的第一部分
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   pszRoleName …	角色名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	帐号信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_find_next_account
    需要调用cs_close_handle 关闭查询句柄pFind
*/
CS_CGI_API
int cs_find_first_account_by_rolename(int *pHandle, const char *pszRoleName,
                                      FIND_ACCOUNT_HANDLE *pFind,
                                      ACCFINDRES *pstOut, int iTimeOut);


/**获取帐号名的其他部分
*@param[in]   hFind …	查询句柄,由cs_find_first_account_by_loginname或
                        cs_find_first_account_by_loginname函数返回。
*@param[out]   pstOut …	帐号信息
                                         请见proto_ss.h中的注释
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请反复调用本函数，直至返回值<0，返回值等于CS_ERR_NOT_FOUND
  说明已获取了全部的帐号名.
*/
CS_CGI_API
int cs_find_next_account(FIND_ACCOUNT_HANDLE hFind, ACCFINDRES *pstOut);


/**指定大区通过账户名模糊查找帐号信息
 */
struct _LimitedFindAccountContext;
typedef struct _LimitedFindAccountContext *LIMITED_FIND_ACCOUNT_HANDLE;


/**通过账户名模糊查找帐号信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginNamePattern …	账户名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	帐号信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_limited_find_next_account
    需要调用cs_close_handle 关闭查询句柄pFind
*/
CS_CGI_API
int cs_limited_find_first_account_by_loginname(
	int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	LIMITED_FIND_ACCOUNT_HANDLE *pFind,
	ACCFINDRES *pstOut,
	int iTimeOut);


/**通过账户名模糊查找帐号信息
*@param[in]   hFind …	查询句柄,由cs_limited_find_first_account_by_loginname
                        打开
*@param[out]   pstOut …	帐号信息
                                         请见proto_ss.h中的注释
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*/
CS_CGI_API
int cs_limited_find_next_account(LIMITED_FIND_ACCOUNT_HANDLE hFind,
                                 ACCFINDRES *pstOut);

/** 查找角色列表
 */
struct _FindAccountRoleContext;
typedef struct _FindAccountRoleContext *FIND_ACCOUNT_ROLE_HANDLE;


/**通过帐号名模糊查询指定区指定服务器的角色列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID ，-1 表示查大区里的所有服
*@param[in]   pszLoginNamePattern …	登录名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	角色列表信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_find_next_role
     需要调用cs_close_handle 关闭查询句柄pFind
*/
CS_CGI_API
int cs_find_first_role_by_loginname(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszLoginNamePattern,
	FIND_ACCOUNT_ROLE_HANDLE *pFind,
	ACCFINDROLELISTRES *pstOut,
	int iTimeOut);


/**通过角色名模糊查询指定区指定服务器的角色列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleNamePattern …	角色名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	角色列表信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_find_next_role
     需要调用cs_close_handle 关闭查询
*/
CS_CGI_API
int cs_find_first_role_by_rolename(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszRoleNamePattern,
	FIND_ACCOUNT_ROLE_HANDLE *pFind,
	ACCFINDROLELISTRES *pstOut,
	int iTimeOut);


/**获取角色列表的其他部分
*@param[in]   hFind …	查询句柄,由cs_find_first_role_by_loginname或
                        cs_find_first_role_by_rolename函数返回。
*@param[out]   pstOut …	角色列表信息
                                         请见proto_ss.h中的注释
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请反复调用本函数，直至返回值<0；返回值等于CS_ERR_NOT_FOUND
  说明已获取了全部的帐号名.
*/
CS_CGI_API
int cs_find_next_role(FIND_ACCOUNT_ROLE_HANDLE hFind, ACCFINDROLELISTRES *pOut);

/** 通过角色名查找帐号名和帐号ID
 */
typedef struct _CSAccountIDRes
{
	int iResNum;
	struct
	{
		int iUin;
		char szPass9Account[ACCOUNT_LOGIN_NAME_LEN];
	}astAccountIDs[ROLE_MAX_RES];
}CSAccountIDRes;


/**通过角色名模糊查找拥有此角色的帐号名和帐号ID
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID , -1 表示这个大区的所有服
*@param[in]   pszRoleNamePattern …	角色名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	角色列表信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_find_next_account_id
     需要调用cs_close_handle 关闭查询
*/
CS_CGI_API
int cs_find_first_account_id_by_rolename(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszRoleNamePattern,
	FIND_ACCOUNT_ROLE_HANDLE *pFind,
	CSAccountIDRes *pOut,
	int iTimeOut);


/**获取角色列表的其他部分
*@param[in]   hFind …	查询句柄,由cs_find_first_account_id_by_rolename。
*@param[out]   pstOut …	角色列表信息
                                         请见proto_ss.h中的注释
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请反复调用本函数，直至返回值<0；返回值等于CS_ERR_NOT_FOUND
  说明已获取了全部的帐号名.
*/
CS_CGI_API
int cs_find_next_account_id(FIND_ACCOUNT_ROLE_HANDLE hFind,
                            CSAccountIDRes *pstOut);


/** 关闭各种查询句柄
 * 关闭由以下函数打开的句柄
 * cs_find_first_account_by_loginname,
 * cs_find_first_account_by_rolename,
 * cs_find_first_role_by_loginname,
 * cs_find_first_role_by_rolename,
 * cs_find_first_account_id_by_rolename,
 * cs_limited_find_first_account_by_loginname,
 *@param[in]   hHandle …	由以上函数打开的句柄
 */
CS_CGI_API
void cs_close_handle(void *hHandle);


/**通过帐号名精确获取帐号信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[out]   pstOut …	帐号信息
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_account_info(int *pHandle, int iRegionID, const char *pszLoginName,
                        SSACCINFO *pstOut, int iTimeOut);


/**获取账号安全锁密码
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[out]   pstOut …	帐号信息                  
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_acc_safelock(int *pHandle, int iRegionid,const char *pszLoginName, 
							GETACCSAFELOCKRES *pstOut,int iTimeOut);

/**设置账号安全锁密码
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[in]   pszPassPwd	安全锁密码，不能为NULL ，必须大于6个字符
*@param[out]   pstOut …	可以为NULL                   
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_set_acc_safelock(int *pHandle, int iRegionid,const char *pszLoginName, const char* pszPassPwd,
							SETACCSAFELOCKRES *pstOut,int iTimeOut);

/*强制消息请求
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
			pstIn		见以下结构描述
					<struct name="ForceMsgReq" version="1122" >
						<entry name="Type" type="int" desc="0-强制消息框 1-强制活动" />
						<entry name="IconID" type="int" desc="0-没有图标  |非0图标ID" />
						<entry name="Msg" type="string" size="SS_MSG_LEN" desc="Type为0时此字段为消息内容 |1时为URL" />
						<entry name="TStart" type="string" size="SS_TIME_LEN"  desc="开始时间|格式为2012-12-12 18:30:30" />
						<entry name="TLife" type="int"   desc="	单位:秒持续时间" />
						<entry name="Interval" type="int" desc="单位:秒|间隔-多久弹出一次" />
					</struct>
			pstOut		输出
			iTimeOut		设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_force_msg_req(int *pHandle, int iRegionid, int iWolrdID, 
						FORCEMSGREQ *pstIn, FORCEMSGRES*pstOut,int iTimeOut);



/**通过帐号名精确设置帐号信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[in]   nUinFlag …	flag值 
*@param[in]   nType …	 类型 0直接赋值 1加标记 2去除标记 3只通知(已修改) ACC_SET_UIN_FLAG_TYPE

                                         请见proto_comm.h中的enum tagUIN_FLAG
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/


/*金币交易工具接口
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID

<macrosgroup name="SHOP_CGI_OP" >
		<macro name="SHOP_CGI_OP_SELL_GOLD" value="0"  desc="出售金币" />
		<macro name="SHOP_CGI_OP_BUY_GOLD" value="1"  desc="购买金币" />
		<macro name="SHOP_CGI_OP_SELL_MONEY" value="2"  desc="出售金币" />
		<macro name="SHOP_CGI_OP_BUY_MONEY" value="3"  desc="购买金币" />
		
		<macro name="SHOP_CGI_OP_SELECT" value="4" desc="查询系统交易信息" />
		<macro name="SHOP_CGI_OP_SELL_REMOVE" value="5" desc="撤销上一次出售操作" />
		<macro name="SHOP_CGI_OP_BUY_REMOVE" value="6" desc="撤销上一次购买操作"/>
		<macro name="SHOP_CGI_OP_GOLD_TRADE_REMOVE" value="7" desc="撤销某一个系统金币单"/>
		<macro name="SHOP_CGI_OP_MONEY_TRADE_REMOVE" value="8" desc="撤销某一个系统银币单"/>
</macrosgroup>
	
ShopCgiReq
{
    char Type;                           // Bind Macrosgroup:SHOP_CGI_OP,
    int Num;                             	//   出售或购买的数量 
    int Price;                           	//   价格 
    int ExchgNum;                        	//   每天总单数 
    unsigned int StartTime;                       	//  开始时间 
    unsigned int EndTime;                         	//  结束时间 
    int InterVal;                        	//   发布间隔时间|秒 
    tdr_ulonglong TradeID;                         	//   撤销系统单需要传入ID 
};

ShopCgiRes
{
    char Type;                            	//   Bind Macrosgroup:SHOP_CGI_OP,
    unsigned char Succ;                            	//  0-成功
    GOLDEXCHGINFO GoldExchgInfo;                   	//   系统交易信息 
};
iTimeOut		设置超时时间
*/
CS_CGI_API
int cs_shop_cgi_req(int *pHandle, int iRegionid, int iWolrdID,SHOPCGIREQ *pstIn, SHOPCGIRES *pstOut, int iTimeOut);


CS_CGI_API
int cs_set_account_uin_flag(int *pHandle, int iRegionID,
                            const char *pszLoginName, unsigned int nUinFlag,int nType,
                            int iTimeOut);


/**帐号禁言
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[in]   iEnable …	0 解除禁言，!= 0 帐号禁言
*@param[in]   iSilenceTime …	禁言时间，单位秒，当iEnable != 0的时候才有效
* -1 表示无限禁言
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_set_account_silence(int *pHandle, int iRegionID, const char *pszLoginName, 
                           int iEnable, int iSilenceTime, int iTimeOut);


/**踢帐号下线
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   pszLoginName …	登录名
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/
CS_CGI_API
int cs_kick_off_by_loginname(int *pHandle, int iRegionID,
                             const char *pszLoginName, int iTimeOut);


/**帐号冻结
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[in]   iEnable …	0 解除冻结，!= 0 帐号冻结
*@param[in]   iFreezeTime …	冻结时间，单位秒，当iEnable != 0的时候才有效
* -1 表示无限冻结
*@param[in]   pszReason …	冻结原因
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/
CS_CGI_API
int cs_set_account_freeze(int *pHandle, int iRegionID, const char *pszLoginName,
                          int iEnable, int iFreezeTime,char* pszReason, int iTimeOut);

/** 根据ip地址和mask掩码冻结一个区的账号
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   pszIP …	点分十进制IP地址   	不能为NULL
*@param[in]   pszMask …	点分十进制mask掩码   如果为NULL       默认mask等于255.255.255.255

*@param[in]   iOpType         
						enum tagIPLimitOp 
						{
						    OP_ADD = 0, 		// 添加一个ip  和mask    如果db中有此ip 和mask     返回失败   
						    OP_DEL = 1,  		// 删除一个ip  和mask    如果db中无此ip 和mask     返回失败
						    OP_MODIFY = 2, 	// 修改一个ip  和mask    如果db中无此ip 和mask     返回失败
						};


*@param[in]   iSec …		冻结时间，单位秒 -1 表示无限冻结0表示解冻
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/


CS_CGI_API
int cs_ip_block_by_region(int *pHandle, int iRegionID, const char * pszIP,
                          const char* pszMask, int iSec, int iOpType,
                          int iTimeOut);


/** 获取某个区被冻结IP段列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …大区ID
*@param[in]   pszIP …	点分十进制IP地址, 
						如果为NULL 将返回整个大区被冻结IP段信息，否则返回指定IP信息
*@param[in]   pstOut      …不能为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/
CS_CGI_API
int cs_get_ip_block_list_by_region(int *pHandle, int iRegionID,
                                   const char *pszIP, ZONEGETIPLIMITRES *pstOut,
                                   int iTimeOut);


/**获取某个帐号的所有角色列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   pszLoginName …	登录名
*@param[out]   pstOut …		返回的角色列表数据
                                                请见proto_ss.h中的注释
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/
//CS_CGI_API
//int cs_get_account_rolelist(int *pHandle, int iRegionID,
//                            const char *pszLoginName, ACCGETROLELISTRES *pstOut,
//                            int iTimeOut);


/*----------------------------------------------------------*/






/**踢角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName …	角色名
*@param[out]   pstOut …   输出,可以为NULL
                                         
*@iTimeout[in]  iTimeout      设置超时时间,单位毫秒
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR

*@note  要求角色在线
*/
CS_CGI_API
int cs_role_kick_off(int *pHandle, int iRegionid, int iWolrdID, 
	                 const char *pszRoleName, ZONEROLELOGOUTRES *pstOut,
	                 int iTimeOut);


/**设置角色GM权限
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName …	角色名
*@param[out]   pstOut …   输出,可以为NULL
*@param[in]   iPerm …	GM权限值                                     
*@iTimeout[in]  iTimeout      设置超时时间,单位毫秒
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR

*/
CS_CGI_API
int cs_set_role_perm(int *pHandle, int iRegionid, int iWolrdID,
	                 const char *pszRoleName, int iPerm,
	                 ZONEDELROLERES *pstOut, int iTimeOut);


/**删除角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名		

*@param[out]   pstOut …   输出,可以为NULL
                                         
*@iTimeout[in]  iTimeout      设置超时时间,单位毫秒
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR

*@note  要求角色不在线
*/
CS_CGI_API
int cs_del_role(int *pHandle, int iRegionid, int iWolrdID,
                const char *pszRoleName, ZONEDELROLERES *pstOut, int iTimeOut);



/*获取角色生活技能信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名		

*@param[out]   pstOut …   输出,可以为NULL

						包含的生活技能结构信息
						struct tagSecondarySkillInfo
						{
						    unsigned short SkillType;                       技能类型: 采矿采药等等 Bind Macrosgroup:SECONDARY_SKILL_TYPE,
						    unsigned short SkillLevel                      	技能等级 
						    unsigned int SkillVal;                        	  技能熟练度 
						    unsigned char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
						    int CoolDownNum;                    
						    RECIPECOOLDOWNINFO CoolDownInfo[MAX_SECONDARY_SKILL_COOLDOWN_RECIPE];  配方CD
						}

						unsigned char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
						32个char 按位记录了学习的配方。位置就是RecipeID配方ID(查表RecipeDef) 
						配方ID 0-255 (实际应用1-255)

						例如: 查找是否学习了某个配方ID

						cFlag	=	1 << (iRecipeID%8); 
						
						if(pstSkillInfo->RecipeInfo[iRecipeID/8] & cFlag)
						{
							学习了
						}
						 
                                         
*@iTimeout[in]  iTimeout      设置超时时间,单位毫秒
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/
CS_CGI_API
int cs_get_life_skill(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, GETLIFTSKILLRES*pstOut,
	                  int iTimeOut);

/*设置角色生活技能
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pstReq		生活技能信息

			struct tagSetLiftSkillReq
			{
			    int SkillOpType;                   			// 	取值为SETLIFTSKILL_OP_DEL(删除)  SETLIFTSKILL_OP_MOD(修改)
			    int SkillType;                      			// 	技能类型
			    int SkillVal;                       			//  	熟练度
			    int RecipeOpType;                    		// 	取值为RECIPE_OP_DEL(删除)  RECIPE_OP_ADD(添加)  RECIPE_OP_NO(配方不改变)
			    int RecipeID;      					// 	配方ID                 
			    char RoleName[MAXROLENAME];           	//  	角色名 
			};
			如果SkillOpType等于SETLIFTSKILL_OP_DEL，只需要填写SkillType和RoleName字段

			
*@param[out]   pstOut …   输出,可以为NULL
                                         
*@iTimeout[in]  iTimeout      设置超时时间,单位毫秒
*@retval 0   succ
*@retval <0  fail  错误原因参见proto_ss.h中的enum tagCS_ERR
*/
CS_CGI_API
int cs_set_life_skill(int *pHandle, int iRegionid, int iWolrdID, SETLIFTSKILLREQ *pstReq,
					SETLIFTSKILLRES *pstOut,int iTimeOut);


/**获取角色信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID

*@param[in]   pszRoleName   角色名


*@param[out]   pstOut …   输出角色信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见enum tagCSErr
*/
CS_CGI_API
int cs_get_role_info(int *pHandle, int iRegionid, int iWorldid, char *pszRoleName,
                     ZONEGETROLEDETAILRES *pstOut,  int iTimeout);




/**设置角色信息 调用此接口之前,必须先调用cs_get_role_info来获取角色信息.
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID

*@param[in]   pstSSRoleDetail 角色信息指针,必须用cs_get_role_info接口获取的信息初始化

						

*@param[out]   pstOut …   输出角色名, 可以为NULL
                                          请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见enum tagCSErr
*@note  要求角色不在线

*@note  此接口只能修改Level  MapID  X  Y  Silence  Block  Perm 和RoleDetailInfo结构中的字段
*/
CS_CGI_API
int cs_set_role_info(int *pHandle, int iRegionid,  int iWolrdID,
                     ZONEGETROLEDETAILRES *pstSSRoleDetail, 
                     ZONESETROLEDETAILRES *pstOut, int iTimeOut);

/** 查找公会列表
 */
struct _FindClanContext;
typedef struct _FindClanContext *FIND_CLAN_HANDLE;


/**通过公会名模糊查询指定区指定服务器的公会列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   iWorldID …	服ID
*@param[in]   pszClanNamePattern …	公会名，支持MySQL通配符
*@param[out]   pFind …	查询句柄，用完此句柄后请调用cs_close_handle关闭。
*@param[out]   pstOut …	角色列表信息
                                         请见proto_ss.h中的注释
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请调用cs_find_next_clan_profile
     需要调用cs_close_handle 关闭查询
*/
CS_CGI_API
int cs_find_first_clan_profile_by_name(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanNamePattern,
	FIND_CLAN_HANDLE *pFind,
	ZONEFINDCLANRES *pstOut,
	int iTimeOut);


/**获取公会列表的其他部分
*@param[in]   hFind …	查询句柄,由cs_find_first_clan_profile_by_name函数返回。
*@param[out]   pstOut …	公会列表信息。请见proto_ss.h中的注释
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部帐号名，请反复调用本函数，直至返回值<0；返回值等于CS_ERR_NOT_FOUND
  说明已获取了全部的帐号名.
*/
CS_CGI_API
int cs_find_next_clan_profile(FIND_CLAN_HANDLE hFind, ZONEFINDCLANRES *pstOut);


/**将指定公会的会长职位与某一指定的成员交换
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   iWorldID …	服ID
*@param[in]   pszClanName …	公会名
*@param[in]   pszRoleName …	角色名，将公会会长与此人交换职位。
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API
int cs_exchange_clan_leader(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanName,
	const char *pszRoleName,
	int iTimeOut);


/**将指定公会的指定外交关系删除
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionID …	大区ID
*@param[in]   iWorldID …	服ID
*@param[in]   pszClanName …	公会名
*@param[in]   pszOtherClanName …	另一公会名，将此与此公会的外交关系删除
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API
int cs_del_clan_diplomatism(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanName,
	const char *pszOtherClanName,
	int iTimeOut);


/**获取公会信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针

*@param[in]   pszClanName  公会名, 可由cs_get_role_info 返回参数中获取
*@param[out] pstOut            输出公会的概要信息        如果需要详细信息再加  

*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_clan_info(int *pHandle, int iRegionID, int iWolrdID,
                     char *pszClanName, ZONEGETCLANINFORES *pstOut,
                     int iTimeOut);


/**获取全部公会信息公会信息
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
*@param[in]  pszClanName  公会名, 可由cs_get_role_info 返回参数中获取,或从数据库中
* 直接查询得到.
*@param[out] pstOut       输出公会的全部信息
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_get_clan_all_info(int *pHandle, int iRegionID, int iWolrdID,
                         const char *pszClanName, ZONEGETCLANALLINFORES *pstOut,
                         int iTimeOut);


/**从指定的公会中删除一个指定的成员
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
*@param[in]  pszClanName  公会名, 可由cs_get_role_info 返回中获取,或从数据库中
* 直接查询得到.
*@param[in]  pszRoleName  角色名, 此角色必须存在与pszClanName指定的公会中
* 注意:你不能从公会中删除会长, 因为这样会导致这个公会永远没有会长.
* 如果尝试删除会长, 将会得到一个错误的结果.
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_del_clan_member(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszClanName, const char *pszRoleName,
                       int iTimeOut);


/**解散一个指定的公会
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
*@param[in]  pszClanName  公会名, 可由cs_get_role_info 返回中获取,或从数据库中
* 直接查询得到.
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_disband_clan(int *pHandle, int iRegionID, int iWolrdID,
                    const char *pszClanName, int iTimeOut);


/**清除一个指定公会的外交关系
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
*@param[in]  pszClanName  公会名, 可由cs_get_role_info 返回中获取,或从数据库中
* 直接查询得到.
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_clear_clan_diplomatism(int *pHandle, int iRegionID, int iWolrdID,
                              const char *pszClanName, int iTimeOut);

/**设置一个公会的基本资源,金,定,繁,科
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
@param[in]  pszClanName  公会名 可由cs_get_role_info 返回中获取,或从数据库中直接查询得到.
@param[in]  iMoney  金钱 
@param[in]  iStable  安定度 
@param[in]  iActive  繁荣度 ，这个值修改了暂时无意义，会根据公式算回去
@param[in]  iTech  科技点数 

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_set_clan_base_attr(int *pHandle, int iRegionID, int iWorldID
                              , const char *pszClanName, int iMoney, int iStable
                              , int iActive, int iTech,int iTimeout);


/**获取黑名单信息(包括黑名单和仇人名单)
*@param[in]   pHandle …	不能为空,init_interface初始化的指针

*@param[in]   pszRoleName  角色名
*@param[out] pstOut           不能NULL

*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_black_info(int *pHandle, int iRegionID, int iWolrdID,
                      char *pszRoleName, ZONEGETBLACKRES *pstOut, int iTimeOut);



/**获取好友信息(包括好友 ,兄弟,夫妻,师傅徒弟 )
*@param[in]   pHandle …	不能为空,init_interface初始化的指针

*@param[in]   pszRoleName  角色名
*@param[out] pstOut       不能NULL    

*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_friend_info(int *pHandle, int iRegionID, int iWolrdID,
                       char *pszRoleName, ZONEGETFIRENDRES *pstOut,
                       int iTimeOut);


/**创建物品实例，一般在给角色道具时使用
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   pstReq …	请求创建的物品ID和数量
*@param[out]   pstOut …	返回的格子信息
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_create_role_item(int *pHandle, const ZONECREATEITEMREQ *pstReq,
                        ZONECREATEITEMRES *pstOut);



/**获取角色道具和身上装备信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名

*@param[out]   pstOut …   输出角色背包和身上装备信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  错误原因参见enum tagCSErr
*/
CS_CGI_API
int cs_get_item_info(int *pHandle, int iRegionid, int iWolrdID, 
                     char *pszRoleName, ZONEGETROLEITEMLISTRES *pstOut, 
                     int iTimeOut);



/**设置角色道具和装备信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iListType        背包类型    取值为:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// 材料包 
			    LIST_TYPE_TASK = 2, 		// 任务包 
			    LIST_TYPE_SUNDRIES = 3, 	// 杂物包 
			    LIST_TYPE_WEAR = 6, 		// 身上
			    LIST_TYPE_STASH = 5		// 仓库
			};
			
*@param[in]    pstGrid 		角色物品结构,可用cs_get_item_info获取
*@param[in]    iMode           操作类型  取值为
			 ROLE_ITEM_DEL = 0, 			// 删除
   			 ROLE_ITEM_MODIFY = 1, 		// 修改
    			 ROLE_ITEM_ADD = 2			// 添加

    			 
*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API
int cs_set_item_info(int *pHandle, int iRegionid,  int iWolrdID,
					 const char *pszRoleName,int iListType, 
					 const ROLEGRID *pstGrid, int iMode,
					 ZONESETROLEITEMLISTRES *pstOut, int iTimeOut);


/**获取角色技能信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   llWid …		RoleID
*@param[in]   pszRoleName 角色名

*@param[out]   pstOut …   输出技能信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_skill_info(int *pHandle, int iRegionid, int iWolrdID,
                      const char *pszRoleName, ZONEGETROLESKILLINFORES *pstOut,
                      int iTimeOut);


/**设置角色技能信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iSkillID       技能ID
*@param[in]   iSkillLevel   技能等级
*@param[in]   iMode	    取值为:
			enum tagRoleSkillOp 
			{
			    ROLE_SKILL_DEL = 0, 		// 删除一个技能
			    ROLE_SKILL_MODIFY = 1, 	// 修改一个技能
			    ROLE_SKILL_ADD = 2, 		// 添加一个技能
			};

*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API
int cs_set_skill_info(int *pHandle, int iRegionid,  int iWolrdID,
					  const char *pszRoleName, unsigned int iSkillID, 
					  unsigned int iSkillLevel, int iMode, 
					  ZONESETROLESKILLINFORES *pstOut, int iTimeOut);


/**获取角色素质信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名

*@param[out]   pstOut …   输出, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_diathesis_info(int *pHandle, int iRegionid, int iWolrdID, 
                          const char *pszRoleName,
                          ZONEGETROLEDIATHESISRES *pstOut, int iTimeOut);



/**设置角色素质信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iDiathsisID … 配件ID
*@param[in]   iDiathsisLevel   配件等级
*@param[in]   iMode: 取值为:
			enum tagRoleDiathesisOp 
			{
			    ROLE_DIATHESIS_DEL = 0, 		// 删除
			    ROLE_DIATHESIS_MODIFY = 1,      // 修改
			    ROLE_DIATHESIS_ADD = 2,           // 添加
			};

*@param[out]   pstOut …   输出角色名可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API
int cs_set_diathsis_info(int *pHandle, int iRegionid,  int iWolrdID,
                         const char *pszRoleName,
                         unsigned int iDiathsisID, unsigned int iDiathsisLevel,
                         int iMode, ZONESETROLEDIATHESISRES*pstOut,
                         int iTimeOut);



/**获取角色任务信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名

*@param[out]   pstOut …   输出, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_task_info(int *pHandle, int iRegionid, int iWolrdID, 
                     const char *pszRoleName, ZONEGETROLETASKRES *pstOut,
                     int iTimeOut);



/**删除角色一个已接任务
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   uiTaskID		任务ID

*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API
int cs_del_task_by_id(int *pHandle, int iRegionid,  int iWolrdID,
					  const char *pszRoleName,unsigned int uiTaskID,
					  ZONEDELROLETASKRES *pstOut, int iTimeOut);


/**冻结角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID

*@param[in]   pszRoleName 角色名

*@param[in]   iBlockTime	冻结时间  单位秒(负数表示永久冻结  不能为0)

*@param[out]   pstOut …   输出, 可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_block_role(int *pHandle, int iRegionid, int iWolrdID, 
                  const char *pszRoleName, int iBlockTime, 
                  ZONEBLOCKPLAYERRES *pstOut, int iTimeOut);

/**解冻角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID

*@param[in]   pszRoleName 角色名


*@param[out]   pstOut …   输出, 可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_unblock_role(int *pHandle, int iRegionid, int iWolrdID, 
                    const char *pszRoleName,  ZONEBLOCKPLAYERRES *pstOut,
                    int iTimeOut);


/**禁言角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iSilenceTime	禁言时间  单位秒(输入负数永久禁言 不能为0)

*@param[out]   pstOut …   输出, 可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_silence_role(int *pHandle, int iRegionid, int iWolrdID, 
                    const char *pszRoleName, int iSilenceTime,
                    ZONESILENCEROLERES *pstOut, int iTimeOut);


/**角色解除禁言
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名

*@param[out]   pstOut …   输出, 可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_unsilence_role(int *pHandle, int iRegionid, int iWolrdID,  
                      const char *pszRoleName,ZONESILENCEROLERES *pstOut, int iTimeOut);

/**跳转角色地图和位置
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName   角色名
*@param[in]   iMap			地图ID
*@param[in]	  x,y			坐标

*@param[out]   pstOut …    输出, 可以为NULL
*@iTimeout[in]  iTimeout    设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_chg_role_pos(int *pHandle, int iRegionid, int iWolrdID, 
	                const char *pszRoleName, int iMap,int x,int y,
	                ZONECHGROLEPOSRES *pstOut,int iTimeOut);


/**修改角色名
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName         角色名
*@param[in]   pszChgRoleName   修改角色名

*@param[out]   pstOut …   输出, 可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间,
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_chg_role_name(int *pHandle, int iRegionid, int iWolrdID, const char *pszRoleName, 
	                 const char *pszChgRoleName,ZONECHGROLENAMERES *pstOut, int iTimeOut);


/**恢复逻辑删除的角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   llWid …		RoleID
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_restore_deleted_role(int *pHandle, int iRegionID, int iWorldID, 
	                        long long llWID, int iTimeOut);


/** 查询某个角色的邮件头列表
 */
struct _FindMaillistContext;
typedef struct _FindMaillistContext *FIND_MAILLIST_HANDLE;


/**获取某个角色邮件列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName …		角色名
*@param[in]   llWid …		RoleID
*@param[out]   pstOut …		角色邮件列表
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/

/*
CS_CGI_API 
int cs_get_role_maillist(int *pHandle, int iRegionID, int iWorldID, 
	                     const char *pszRoleName, ZONEGETMAILLISTRES *pstOut, 
	                     int iTimeOut);
*/

/**获取某个角色邮件头列表的第一部分
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName …		角色名
*@param[in]   pFind …		返回的查询句柄,使用后请用cs_close_handle
*@param[out]   pstOut …		角色邮件列表
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_find_first_maillist(int *pHandle, int iRegionID, int iWorldID,
                           const char *pszRoleName, FIND_MAILLIST_HANDLE *pFind,
                           ZONEGETMAILLISTRES *pstOut, int iTimeOut);


/**获取角色邮件头列表的其他部分
*@param[in]   hFind …	查询句柄,由cs_find_first_maillist函数返回。
*@param[out]   pstOut …	邮件头列表信息
                                         请见proto_ss.h中的注释
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*@如需获取全部邮件头列表，请反复调用本函数，直至返回值<0，
  返回值等于CS_ERR_NOT_FOUND说明已获取了全部的帐号名.
*/
CS_CGI_API
int cs_find_next_maillist(FIND_MAILLIST_HANDLE hFind,
                          ZONEGETMAILLISTRES *pstOut);



/**获取某个角色某封邮件的具体数据
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName …		角色名
*@param[in]   llMailID …		邮件ID
*@param[out]   pstOut …		邮件完整数据
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_role_maildata(int *pHandle, int iRegionID, int iWorldID, 
	                       const char *pszRoleName, long long llMailID, 
	                       MAIL *pstOut, int iTimeOut);


/**删除某个角色的某封邮件
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName …		角色名
*@param[in]   llMailID …		邮件ID
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_del_role_mail(int *pHandle, int iRegionID, int iWorldID, 
                     const char *pszRoleName, long long llMailID, int iTimeOut);


/**给某个角色发送系统邮件
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszTo …		收件人名
*@param[in]   pszSend …		发件人名
*@param[in]   pszTitle …		邮件标题，可以为NULL
*@param[in]   pszText …		邮件正文，可以为NULL
*@param[in]   astItems …		邮寄的物品，可以为NULL
*@param[in]   iItemNum …		邮寄的物品数量
*@param[in]   nMoney …		邮寄的金钱
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_send_sys_mail(int *pHandle, int iRegionID, int iWorldID, 
                     const char *pszTo, const char *pszSend, const char *pszTitle, 
	                   const char *pszText, const ROLEGRID *astItems, int iItemNum,
	                   unsigned int nMoney, int iTimeOut);



/**移动某个角色的物品到另一角色身上
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   pstSrc …	源角色,从此角色身上取出物品
*@param[in]   pstDst …	目的角色,从pstSrc身上取下的物品放到此角色身上
*@param[in]   iListType …		移动的物品在哪个包中    取值为:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// 材料包 
			    LIST_TYPE_TASK = 2, 		// 任务包 
			    LIST_TYPE_SUNDRIES = 3, 	// 杂物包 
			    LIST_TYPE_WEAR = 6, 		// 身上
			    LIST_TYPE_STASH = 5		// 仓库
			};
*@param[in]   pstGrid …		移动的物品的格子信息
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求双方角色都不在线
*/
CS_CGI_API 
int cs_mov_role_item(int *pHandle, const SSROLESTORAGEPATH *pstSrc,
                     const SSROLESTORAGEPATH *pstDst, int iListType,
                     const ROLEGRID *pstGrid, int iTimeOut);


/**复制角色
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   llSrcID …	源角色ID
*@param[in]   llDstID …	目标角色ID
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求双方角色都不在线
*/
CS_CGI_API 
int cs_copy_role(int *pHandle, int iRegionID, int iWorldID, long long llDstID,
                 const char *pszDstName, long long llSrcID,
                 const char *pszSrcName, int iTimeOut);



/**获取一个服务器中的公告列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[out]   pstOut …	返回的公告列表
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_bulletin_list(int *pHandle, int iRegionID, int iWorldID,
	                     ZONEGETBULLETINLISTRES *pstOut, int iTimeOut);




/**添加一条公告信息到一个服务器
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszText …	公告文本
*@param[in]   iRollInterval …	滚动间隔时间
*@param[in]   iActiveTime …	活跃时间

*@param[in]   tActiveTimeStart …	与time(NULL) 相同格式的时间
                                                       0 表示服务器收到消息后马上显示公告
								
*@param[out]   piID …	如果添加成功,则返回公告ID,可为NULL
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_add_bulletin(int *pHandle, int iRegionID, int iWorldID, 
                    const char *pszText, int iRollInterval, int iActiveTime, int tActiveTimeStart,
                    int *piID, int iTimeOut);



/**添加一条公告信息到所有服务器
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   pszText …	公告文本
*@param[in]   iRollInterval …	滚动间隔时间
*@param[in]   iActiveTime …	活跃时间

*@param[in]   *@param[in]   tActiveTimeStart …	与time(NULL) 相同格式的时间
                                           0 表示服务器收到消息后马上显示公告
								
*@param[out]   piID …	如果添加成功,则返回公告ID,可为NULL
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_add_bulletin_to_all(int *pHandle, const char *pszText, int iRollInterval,
                           int iActiveTime, int tActiveTimeStart, int *piID, int iTimeOut);



/**删除一条公告信息从一个服务器
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   iID …	被删除的公告ID
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_del_bulletin(int *pHandle, int iRegionID, int iWorldID, int iID,
                    int iTimeOut);


/**删除一条公告信息从所有的服务器
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iID …	被删除的公告ID
*@param[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_del_bulletin_from_all(int *pHandle, int iID, int iTimeOut);


/**获取角色坐骑信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[out]   pstOut …   输出坐骑信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_ride_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName,ZONEGETROLERIDEINFORES *pstOut,
	                    int iTimeOut);

/**设置角色骑乘信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iRideID       骑乘ID
*@param[in]   iRideLevel   骑乘等级
*@param[in]   iEndTime  骑乘到期时间(从1970 开始到现在的秒) , 0 则永久
*@param[in]   iMode	    取值为:
			enum tagRoleSkillOp 
			{
			    ROLE_RIDE_DEL = 0, 		// 删除一个坐骑
			    ROLE_RIDE_MODIFY = 1, 	// 修改一个坐骑
			    ROLE_RIDE_ADD = 2, 		// 添加一个坐骑?
			};

*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_set_ride_info(int *pHandle, int iRegionid,  int iWolrdID,
                     const char *pszRoleName, unsigned int iRideID,
                     unsigned int iRideLevel, int iEndTime, int iMode,
                     ZONESETROLERIDEINFORES *pstOut, int iTimeOut);

/**获取角色契约列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[out]   pstOut …    输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note
*/
CS_CGI_API 
int cs_get_role_qiyue_list(int *pHandle, int iRegionid,  int iWolrdID,
                           const char *pszRoleName, GETROLEQIYUELISTRES *pstOut,
                           int iTimeOut);

/**设置角色当前使用的契约
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[in]   iQiYue        需要设置使用的契约id
*@param[out]   pstOut …    输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_set_role_qiyue(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, unsigned int iQiYueID,
                      SETROLEQIYUERES *pstOut, int iTimeOut);

/**增加角色契约
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[in]   iQiYue        需要设置使用的契约id
*@param[out]   pstOut …    输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_add_role_qiyue(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName,unsigned int iQiYueID,
                      ADDROLEQIYUERES *pstOut, int iTimeOut);

/**删除角色契约
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[in]   iQiYue        需要设置使用的契约id
*@param[out]   pstOut …    输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_del_role_qiyue(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, unsigned int iQiYueID,
                      DELROLEQIYUERES *pstOut, int iTimeOut);


/**修改精灵等级
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[in]   FairyID       精灵ID
*@param[in]   FairyLv       精灵等级
*@param[out]   pstOut …    输出,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_modify_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					      const char *pszRoleName, unsigned int FairyID,
					      unsigned int FairyLv, ZONESETROLEFAIRYRES*pstOut,
					      int iTimeOut);


/**删除精灵
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[in]   FairyID       精灵ID
*@param[out]   pstOut …    输出,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_del_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					            const char *pszRoleName, unsigned int FairyID,
					            ZONESETROLEFAIRYRES*pstOut, int iTimeOut);


/**添加精灵
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 	角色名
*@param[in]   FairyID       精灵ID
*@param[out]   pstOut …    输出,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_add_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					   const char *pszRoleName, unsigned int FairyID,
					   ZONESETROLEFAIRYRES*pstOut, int iTimeOut);


/**获取角色宠物信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[out]   pstOut …   输出宠物信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_pet_info(int *pHandle, int iRegionid, int iWolrdID, 
	                const char *pszRoleName, ZONEGETROLEPETINFORES *pstOut,
	                int iTimeOut);

/**设置角色宠物信息
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   pstRolePet     宠物结构,可用cs_get_pet_info获取
*@param[in]   iMode	    取值为:
			enum tagRoleSkillOp 
			{
			    ROLE_PET_DEL = 0, 		// 删除一个宠物
			    ROLE_PET_MODIFY = 1, 	// 修改一个宠物
			    ROLE_PET_ADD = 2, 		// 添加一个宠物
			};

*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_set_pet_info(int *pHandle, int iRegionid,  int iWolrdID,
                    const char *pszRoleName,  ROLEPET *pstRolePet, int iMode,
                    ZONESETROLEPETINFORES *pstOut, int iTimeOut);


/**为角色添加一个称号
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iDesignationID 称号ID
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_add_designation(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszRoleName, int iDesignationID,
                       int iTimeOut);


/**删除角色的一个称号
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iDesignationID 称号ID
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_del_designation(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszRoleName, int iDesignationID,
                       int iTimeOut);


/**获取角色称号列表及当前使用的称号
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[out]   pstOut …   返回称号信息
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_get_designation_list(int *pHandle, int iRegionid, int iWolrdID,
                            const char *pszRoleName, ROLEDESIGNATIONS *pstOut,
                            int iTimeOut);


/**为角色添加/设置一个声望数据
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iRespactPartyID 阵营ID
*@param[in]   iRespactValue 声望值
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@note  如果角色没有这个阵营的声望数据，将会增加一个，
*       并用iRespactValue设置声望值。
*/
CS_CGI_API 
int cs_set_respact(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, int iRespactPartyID,
                   int iRespactValue, int iTimeOut);

/**删除一个角色声望数据
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iRespactPartyID 阵营ID
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_del_respact(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, int iRespactPartyID, int iTimeOut);



/*获取角色安全锁密码		要求角色不在线
*@param[in]   pszRoleName 角色名
*@param[out]   pstOut 不能为空
*/
CS_CGI_API 
int cs_get_safelock(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, GETSAFELOCKRES *pstOut,int iTimeOut);


/*设置角色安全锁密码		要求角色不在线
*@param[in]   	pszRoleName 		角色名
*@param[in]   	pszPassWD 		密码字符串
*@param[out]   	pstOut 			可以为空
*/
CS_CGI_API 
int cs_set_safelock(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, const char* pszPassWD, SETSAFELOCKRES *pstOut, int iTimeOut);



/*设置角色的二级密码 */
int cs_set_second_pwd(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, const char* pszPassWD, int iTimeOut);




/**获取角色声望列表
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   llWid …		RoleID
*@param[in]   pszRoleName 角色名
*@param[out]   pstOut …   返回声望列表
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_respact_list(int *pHandle, int iRegionid, int iWolrdID,
                    const char *pszRoleName, RESPACTINFO *pstOut, int iTimeOut);


/**cs接口增加按大区和帐号查询点券相关的信息（注：只查询，不修改）
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginNamePattern …	账户名，支持MySQL通配符
*@param[out]   pstOut …	返回的相关billing的数据
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*/
CS_CGI_API
int cs_query_bill_point(
	int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	BILLRECORD *pstOut,
	int iTimeOut);

/**cs接口 冲值后对应礼包增加的点数通知到大区
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   pszLoginName …	账户名
*@param[in]   iUin … 帐号Uin
*@param[in]   iGroupID … 礼包id
*@param[in]   iAddCash …	礼包对应的充值点数增加值

*@param[out]   pstOut …	返回的相关billing的数据
                                         请见proto_ss.h中的注释
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND 没有找到
*/
int cs_cash_props_gift_bill(int *pHandle,
	int iRegionID,
	const char *pszLoginName,
	int iUin,
	int iGroupID,
	int iAddCash,
	CASHPROPSGIFTINFORES *pstOut,
	int iTimeOut);


/**获取角色指定背包解锁格子信息。
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iListType       背包类型, 取值为:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// 材料包 
			    LIST_TYPE_TASK = 2, 		// 任务包 
			    LIST_TYPE_SUNDRIES = 3, 	// 杂物包 
			    LIST_TYPE_STASH = 5		// 仓库
			};

*@param[out]   pstOut …   输出查询的背包解锁格子信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/

CS_CGI_API
int cs_get_package_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, ZONEGETROLEPACKAGERES *pstOut,
	                    int iTimeOut);

/**设置角色指定背包的格子解锁
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iListType       	背包类型, 取值为:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// 材料包 
			    LIST_TYPE_TASK = 2, 		// 任务包 
			    LIST_TYPE_SUNDRIES = 3, 	// 杂物包 
			    LIST_TYPE_STASH = 5		// 仓库
			};
			
*@param[in]   iNum       	背包增加的解锁格子数量
*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API 
int cs_set_package_info(int * pHandle, int iRegionid, int iWolrdID, 
				const char * pszRoleName, int iListType, int iNum, ZONESETROLEPACKAGERES * pstOut, 
				int iTimeOut);


/**查询商城临时背包(指定背包)中的物品
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iListType       背包类型, 取值为:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// 材料包 
			    LIST_TYPE_TASK = 2, 		// 任务包 
			    LIST_TYPE_SUNDRIES = 3, 	// 杂物包 
			    LIST_TYPE_STASH = 5		// 仓库
			    LIST_TYPE_SHOPPING = 9, 	// 商品栏 
			};

*@param[out]   pstOut …   输出指定背包的数据 
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/

CS_CGI_API
int cs_get_package_item_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, ZONEGETPACKAGEITEMINFORES*pstOut,
	                    int iTimeOut);


/*根据LogRoleSnapshot表让角色回档
	*@param[in]   pHandle …	不能为空,init_interface初始化的指针
	*@param[in]   iRegionid …	大区ID
	*@param[in]   iWorldid …	服ID
	*@param[in]   pszRoleName 角色名
	*@param[in]   pszDate 		回档时间  固定格式为"2011-05-12 20:23:16",建议从LogRoleSnapshot表中复制时间
*/

CS_CGI_API
int cs_role_back_record(int *pHandle, int iRegionid, int iWolrdID,const char *pszRoleName,const char *pszDate, 
							ZONEROLEBACKRES *pstOut, int iTimeOut);

/**获取角色丢弃的重要物品等信息。
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名

*@param[out]   pstOut …   输出查询的角色丢弃信息, 必须是有效的指针.
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*/

CS_CGI_API
int cs_get_role_drop_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, ZONEGETROLEDROPRES *pstOut,
	                    int iTimeOut);

/**恢复角色丢弃的重要物品
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   iDropType      丢弃类型, 取值为:
			enum tagROLE_DROP_TYPE 
			{
			    ROLE_DROP_GRID = 0, 		// 物品
			    ROLE_DROP_PET = 1, 		// 宠物
			};
*@param[in]   pstRestoreInfo      丢弃数据结构,填充cs_get_role_drop_info获取的信息
*@param[in]   pszRoleName 角色名
			
*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/
CS_CGI_API
int cs_role_drop_restore(int *pHandle, int iRegionid, int iWolrdID,
			     	int iDropType, const ROLEDROPRESTOREINFO *pstRestoreInfo, const char *pszRoleName, 
			     	ROLEDROPRESTORERES *pstOut, int iTimeOut);

/**制造宠物
*@param[in]   pHandle …	不能为空,init_interface初始化的指针
*@param[in]   iRegionid …	大区ID
*@param[in]   iWorldid …	服ID
*@param[in]   pszRoleName 角色名
*@param[in]   iPetID      	表格PetDef中定义ID
*@param[in]   iPetLevel      	宠物等级上限 MAX_PET_LEVEL

*@param[in]   iPetGenius	宠物资质，取值为:
			enum tagPET_GENIUS_TYPE 
			{
			    PET_GENIUS_D = 0, 		普通资质
			    PET_GENIUS_C = 1, 		良好资质 
			    PET_GENIUS_B = 2, 	 	优秀资质
			    PET_GENIUS_A = 3, 		杰出资质
			    PET_GENIUS_S = 4, 	 	完美资质 
			}
*@param[in]   iStrengthenLevel      强化等级 上限:MAX_PET_STRENGTHEN_LEVEL

*@param[out]   pstOut …   输出角色名,可以为NULL
*@iTimeout[in]  iTimeout      设置超时时间
*@retval 0   succ
*@retval <0  fail  
*@note  要求角色不在线
*/

CS_CGI_API
int cs_role_hatch_pet(int *pHandle, int iRegionid, int iWolrdID,
			     	const char *pszRoleName, int iPetID, int iPetLevel, int iPetGenius, 
			     	int iStrengthenLevel, ZONEROLEHATCHPETRES *pstOut, int iTimeOut);

/**通过角色名模糊查询所有区用户角色信息的第一部分
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
@param[in]  pszRoleNamePattern  角色名 支持MySQL通配符
@param[out]  pFind  查询句柄 用完此句柄后请调用cs_close_handle关闭。
@param[out]  pstOut  角色列表信息 
@iTimeout[in]  iTimeout      设置超时时间 a

*@retval 0   succ
*@retval < 0 fail  
*@ retval CS_ERR_NOT_FOUND 没有找到
*@ 如需获取全部角色，请调用cs_find_all_next_role
*@ 需要调用cs_close_handle 关闭查询
*/
CS_CGI_API
int cs_find_all_first_role_by_rolename(int *pHandle
                              , char *pszRoleNamePattern, FIND_ACCOUNT_ROLE_HANDLE *pFind, ACCFINDROLELISTRES *pstOut
                              ,int iTimeOut);


/**获取所有区模糊查询角色名的其他部分
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
@param[in]  hFind  查询句柄 用cs_find_all_first_role_by_rolename获取
@param[out]  pstOut  角色列表信息 
*@retval 0   succ
*@retval < 0 fail  
*@ retval CS_ERR_NOT_FOUND 没有找到
*@ 如需获取全部角色名，请反复调用本函数，直至返回值<0
*@ 返回值等于CS_ERR_NOT_FOUND说明已获取了全部的角色名.
*/
CS_CGI_API
int cs_find_all_next_role(int *pHandle
                              , FIND_ACCOUNT_ROLE_HANDLE hFind, ACCFINDROLELISTRES *pstOut);

/**通过uin获得角色列表
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
@param[in]  iRegionid  区id
@param[in]  iWolrdID  区id
@param[in]  iUin  区id
@param[out]  pstOut  角色列表信息 
@param[in]  iTimeout      设置超时时间 
*@retval 0   succ
*@retval < 0 fail  
*/

int cs_get_role_list(int *pHandle, int iRegionid, int iWolrdID, int iUin,
	                   ZONEROLELISTRES *pstOut, int iTimeOut);

/**指定服务器reload
*@param[in]  pHandle …   不能为空,init_interface初始化的指针
@param[out]  pstOut  reload返回信息 

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_reload_server(int *pHandle, int iRegionID, int iWorldID
                              , ZONERELOADSERVERRES *pstOut, int iTimeOut);

			     	
#if defined(__cplusplus)
}
#endif

#endif // _CS_CGI_H_
