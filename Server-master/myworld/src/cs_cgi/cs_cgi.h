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

/**��ʼ������, һ�����ֻ�����һ��
*@param[in]   szSSBinPath ��proto_ss.tdr��·��	
*@param[out]   ppHandle �����ָ��	
*@param[in]   szXmlPath ��cs_cgi.xml�����ļ���·��	

*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�enum tagCSErr
*/
CS_CGI_API
int init_interface(const char *szSSBinPath, int **ppHandle,
                   const char *szXmlPath);

/**�˳�������һ�����ֻ��Ҫ����һ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ���ľ��ָ��
*/
CS_CGI_API
void destory_interface(int *pHandle);

/**��ӡ���ݽṹ���������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ���ľ��ָ��
*@param[in]   pszMetaName 
*@param[in]   pvData �ṹ���buf
*@param[in]   iDataSize �ṹ��Ĵ�С
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


/**��ʼ��blob���ݽ����ӿ�
@param[in]  szMetaPath  ·��proto_ss.tdr������tdr��·��
@param[out] ppHandle  ���ָ�� ����Ϊ��,��������ʱ�贫���ָ��

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int init_analy_interface(const char* szMetaPath, int **ppHandle);

/**����Blob����
*
@param[in]  pHandle  ͨ��init_analy_interface��õ�ָ��
@param[in]  pszBlobMetaName  Blob���ݿ��Ӧ��Meta��, ���meta����Ҫ��ԭʼ
                   proto_cs.xml,proto_ss.xml��op_log_meta.xml, proto_comm.xml�пɵõ���
                   ��Щ�ļ�Ҳ����cs_cgi_dllĿ¼�¡����磬Ҫ����LogRoleSnapshot
                   �е�LogRoleData�ֶΣ���metaname�μ�op_log_meta.xml�е�
                   <entry name="OpRoleSnapshot" type="OpRoleSnapshot" id="OP_LOG_ROLE_SNAPSHOT" version="158" />
                   �ٵ�proto_comm.xml����
                   <struct name="OpRoleSnapshot" version="158" uniqueentryname="false" >
                   ����
                   <entry name="LogRoleData" type="RoleData"  version="160"/>
                   ���ԣ�LogRoleData���blob�ֶε�metaname����RoleData
@param[in]  pszBlobBuff  Blob buff ָ��
@param[in]  iBlobBuff  Blob���ݿ��С
@param[out]  pszXml  charָ�� ����Ϊ��,���浼����XML��ʽ���ַ���
@param[in]  iXml  pszXmlָ��������С

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int analy_interface(int *pHandle, char *pszBlobMetaName,char *pszBlobBuff
								,int iBlobBuff, char *pszXml, int* iXml);

/**�����ӿڽ�������
*
@param[out]  *ppHandle  ָ�� ��������ʱ�贫���ָ��

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int fini_analy_interface(int **ppHandle);

/**��ȡ������Ϣ�ӿ�, һ�����ֻ�����һ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in,out]   pstRegionWorldInfo �������ⲿ����һ���ṹREGIONWORLDINFO stInfo������
                           ��ַ����&stInfo

*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�enum tagCSErr
*/
CS_CGI_API
int cs_get_regionworld_info(int *pHandle, REGIONWORLDINFO *pstRegionWorldInfo);


/*-------------------------------------------------------*/

/**��ѯȫ��ȫ���ʺ���
 */
struct _FindAccountContext;
typedef struct _FindAccountContext *FIND_ACCOUNT_HANDLE;


/**ͨ���ʺ���ģ����ѯ�������û����ʺ����ĵ�һ����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   pszLoginName ��	��¼����֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	�ʺ���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_find_next_account
    ��Ҫ����cs_close_handle �رղ�ѯ���pFind��������ڴ�й©
*/
CS_CGI_API
int cs_find_first_account_by_loginname(int *pHandle, const char *pszLoginName,
                                       FIND_ACCOUNT_HANDLE *pFind,
                                       ACCFINDRES *pstOut, int iTimeOut);


/**ͨ����ɫ��ģ����ѯ������ӵ�д˽�ɫ�����ʺ����ĵ�һ����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   pszRoleName ��	��ɫ����֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	�ʺ���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_find_next_account
    ��Ҫ����cs_close_handle �رղ�ѯ���pFind
*/
CS_CGI_API
int cs_find_first_account_by_rolename(int *pHandle, const char *pszRoleName,
                                      FIND_ACCOUNT_HANDLE *pFind,
                                      ACCFINDRES *pstOut, int iTimeOut);


/**��ȡ�ʺ�������������
*@param[in]   hFind ��	��ѯ���,��cs_find_first_account_by_loginname��
                        cs_find_first_account_by_loginname�������ء�
*@param[out]   pstOut ��	�ʺ���Ϣ
                                         ���proto_ss.h�е�ע��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ������뷴�����ñ�������ֱ������ֵ<0������ֵ����CS_ERR_NOT_FOUND
  ˵���ѻ�ȡ��ȫ�����ʺ���.
*/
CS_CGI_API
int cs_find_next_account(FIND_ACCOUNT_HANDLE hFind, ACCFINDRES *pstOut);


/**ָ������ͨ���˻���ģ�������ʺ���Ϣ
 */
struct _LimitedFindAccountContext;
typedef struct _LimitedFindAccountContext *LIMITED_FIND_ACCOUNT_HANDLE;


/**ͨ���˻���ģ�������ʺ���Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginNamePattern ��	�˻�����֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	�ʺ���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_limited_find_next_account
    ��Ҫ����cs_close_handle �رղ�ѯ���pFind
*/
CS_CGI_API
int cs_limited_find_first_account_by_loginname(
	int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	LIMITED_FIND_ACCOUNT_HANDLE *pFind,
	ACCFINDRES *pstOut,
	int iTimeOut);


/**ͨ���˻���ģ�������ʺ���Ϣ
*@param[in]   hFind ��	��ѯ���,��cs_limited_find_first_account_by_loginname
                        ��
*@param[out]   pstOut ��	�ʺ���Ϣ
                                         ���proto_ss.h�е�ע��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*/
CS_CGI_API
int cs_limited_find_next_account(LIMITED_FIND_ACCOUNT_HANDLE hFind,
                                 ACCFINDRES *pstOut);

/** ���ҽ�ɫ�б�
 */
struct _FindAccountRoleContext;
typedef struct _FindAccountRoleContext *FIND_ACCOUNT_ROLE_HANDLE;


/**ͨ���ʺ���ģ����ѯָ����ָ���������Ľ�ɫ�б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID ��-1 ��ʾ�����������з�
*@param[in]   pszLoginNamePattern ��	��¼����֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	��ɫ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_find_next_role
     ��Ҫ����cs_close_handle �رղ�ѯ���pFind
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


/**ͨ����ɫ��ģ����ѯָ����ָ���������Ľ�ɫ�б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleNamePattern ��	��ɫ����֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	��ɫ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_find_next_role
     ��Ҫ����cs_close_handle �رղ�ѯ
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


/**��ȡ��ɫ�б����������
*@param[in]   hFind ��	��ѯ���,��cs_find_first_role_by_loginname��
                        cs_find_first_role_by_rolename�������ء�
*@param[out]   pstOut ��	��ɫ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ������뷴�����ñ�������ֱ������ֵ<0������ֵ����CS_ERR_NOT_FOUND
  ˵���ѻ�ȡ��ȫ�����ʺ���.
*/
CS_CGI_API
int cs_find_next_role(FIND_ACCOUNT_ROLE_HANDLE hFind, ACCFINDROLELISTRES *pOut);

/** ͨ����ɫ�������ʺ������ʺ�ID
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


/**ͨ����ɫ��ģ������ӵ�д˽�ɫ���ʺ������ʺ�ID
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID , -1 ��ʾ������������з�
*@param[in]   pszRoleNamePattern ��	��ɫ����֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	��ɫ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_find_next_account_id
     ��Ҫ����cs_close_handle �رղ�ѯ
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


/**��ȡ��ɫ�б����������
*@param[in]   hFind ��	��ѯ���,��cs_find_first_account_id_by_rolename��
*@param[out]   pstOut ��	��ɫ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ������뷴�����ñ�������ֱ������ֵ<0������ֵ����CS_ERR_NOT_FOUND
  ˵���ѻ�ȡ��ȫ�����ʺ���.
*/
CS_CGI_API
int cs_find_next_account_id(FIND_ACCOUNT_ROLE_HANDLE hFind,
                            CSAccountIDRes *pstOut);


/** �رո��ֲ�ѯ���
 * �ر������º����򿪵ľ��
 * cs_find_first_account_by_loginname,
 * cs_find_first_account_by_rolename,
 * cs_find_first_role_by_loginname,
 * cs_find_first_role_by_rolename,
 * cs_find_first_account_id_by_rolename,
 * cs_limited_find_first_account_by_loginname,
 *@param[in]   hHandle ��	�����Ϻ����򿪵ľ��
 */
CS_CGI_API
void cs_close_handle(void *hHandle);


/**ͨ���ʺ�����ȷ��ȡ�ʺ���Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[out]   pstOut ��	�ʺ���Ϣ
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_account_info(int *pHandle, int iRegionID, const char *pszLoginName,
                        SSACCINFO *pstOut, int iTimeOut);


/**��ȡ�˺Ű�ȫ������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[out]   pstOut ��	�ʺ���Ϣ                  
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_acc_safelock(int *pHandle, int iRegionid,const char *pszLoginName, 
							GETACCSAFELOCKRES *pstOut,int iTimeOut);

/**�����˺Ű�ȫ������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[in]   pszPassPwd	��ȫ�����룬����ΪNULL ���������6���ַ�
*@param[out]   pstOut ��	����ΪNULL                   
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_set_acc_safelock(int *pHandle, int iRegionid,const char *pszLoginName, const char* pszPassPwd,
							SETACCSAFELOCKRES *pstOut,int iTimeOut);

/*ǿ����Ϣ����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
			pstIn		�����½ṹ����
					<struct name="ForceMsgReq" version="1122" >
						<entry name="Type" type="int" desc="0-ǿ����Ϣ�� 1-ǿ�ƻ" />
						<entry name="IconID" type="int" desc="0-û��ͼ��  |��0ͼ��ID" />
						<entry name="Msg" type="string" size="SS_MSG_LEN" desc="TypeΪ0ʱ���ֶ�Ϊ��Ϣ���� |1ʱΪURL" />
						<entry name="TStart" type="string" size="SS_TIME_LEN"  desc="��ʼʱ��|��ʽΪ2012-12-12 18:30:30" />
						<entry name="TLife" type="int"   desc="	��λ:�����ʱ��" />
						<entry name="Interval" type="int" desc="��λ:��|���-��õ���һ��" />
					</struct>
			pstOut		���
			iTimeOut		���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_force_msg_req(int *pHandle, int iRegionid, int iWolrdID, 
						FORCEMSGREQ *pstIn, FORCEMSGRES*pstOut,int iTimeOut);



/**ͨ���ʺ�����ȷ�����ʺ���Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[in]   nUinFlag ��	flagֵ 
*@param[in]   nType ��	 ���� 0ֱ�Ӹ�ֵ 1�ӱ�� 2ȥ����� 3ֻ֪ͨ(���޸�) ACC_SET_UIN_FLAG_TYPE

                                         ���proto_comm.h�е�enum tagUIN_FLAG
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/


/*��ҽ��׹��߽ӿ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID

<macrosgroup name="SHOP_CGI_OP" >
		<macro name="SHOP_CGI_OP_SELL_GOLD" value="0"  desc="���۽��" />
		<macro name="SHOP_CGI_OP_BUY_GOLD" value="1"  desc="������" />
		<macro name="SHOP_CGI_OP_SELL_MONEY" value="2"  desc="���۽��" />
		<macro name="SHOP_CGI_OP_BUY_MONEY" value="3"  desc="������" />
		
		<macro name="SHOP_CGI_OP_SELECT" value="4" desc="��ѯϵͳ������Ϣ" />
		<macro name="SHOP_CGI_OP_SELL_REMOVE" value="5" desc="������һ�γ��۲���" />
		<macro name="SHOP_CGI_OP_BUY_REMOVE" value="6" desc="������һ�ι������"/>
		<macro name="SHOP_CGI_OP_GOLD_TRADE_REMOVE" value="7" desc="����ĳһ��ϵͳ��ҵ�"/>
		<macro name="SHOP_CGI_OP_MONEY_TRADE_REMOVE" value="8" desc="����ĳһ��ϵͳ���ҵ�"/>
</macrosgroup>
	
ShopCgiReq
{
    char Type;                           // Bind Macrosgroup:SHOP_CGI_OP,
    int Num;                             	//   ���ۻ�������� 
    int Price;                           	//   �۸� 
    int ExchgNum;                        	//   ÿ���ܵ��� 
    unsigned int StartTime;                       	//  ��ʼʱ�� 
    unsigned int EndTime;                         	//  ����ʱ�� 
    int InterVal;                        	//   �������ʱ��|�� 
    tdr_ulonglong TradeID;                         	//   ����ϵͳ����Ҫ����ID 
};

ShopCgiRes
{
    char Type;                            	//   Bind Macrosgroup:SHOP_CGI_OP,
    unsigned char Succ;                            	//  0-�ɹ�
    GOLDEXCHGINFO GoldExchgInfo;                   	//   ϵͳ������Ϣ 
};
iTimeOut		���ó�ʱʱ��
*/
CS_CGI_API
int cs_shop_cgi_req(int *pHandle, int iRegionid, int iWolrdID,SHOPCGIREQ *pstIn, SHOPCGIRES *pstOut, int iTimeOut);


CS_CGI_API
int cs_set_account_uin_flag(int *pHandle, int iRegionID,
                            const char *pszLoginName, unsigned int nUinFlag,int nType,
                            int iTimeOut);


/**�ʺŽ���
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[in]   iEnable ��	0 ������ԣ�!= 0 �ʺŽ���
*@param[in]   iSilenceTime ��	����ʱ�䣬��λ�룬��iEnable != 0��ʱ�����Ч
* -1 ��ʾ���޽���
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_set_account_silence(int *pHandle, int iRegionID, const char *pszLoginName, 
                           int iEnable, int iSilenceTime, int iTimeOut);


/**���ʺ�����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/
CS_CGI_API
int cs_kick_off_by_loginname(int *pHandle, int iRegionID,
                             const char *pszLoginName, int iTimeOut);


/**�ʺŶ���
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[in]   iEnable ��	0 ������ᣬ!= 0 �ʺŶ���
*@param[in]   iFreezeTime ��	����ʱ�䣬��λ�룬��iEnable != 0��ʱ�����Ч
* -1 ��ʾ���޶���
*@param[in]   pszReason ��	����ԭ��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/
CS_CGI_API
int cs_set_account_freeze(int *pHandle, int iRegionID, const char *pszLoginName,
                          int iEnable, int iFreezeTime,char* pszReason, int iTimeOut);

/** ����ip��ַ��mask���붳��һ�������˺�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   pszIP ��	���ʮ����IP��ַ   	����ΪNULL
*@param[in]   pszMask ��	���ʮ����mask����   ���ΪNULL       Ĭ��mask����255.255.255.255

*@param[in]   iOpType         
						enum tagIPLimitOp 
						{
						    OP_ADD = 0, 		// ���һ��ip  ��mask    ���db���д�ip ��mask     ����ʧ��   
						    OP_DEL = 1,  		// ɾ��һ��ip  ��mask    ���db���޴�ip ��mask     ����ʧ��
						    OP_MODIFY = 2, 	// �޸�һ��ip  ��mask    ���db���޴�ip ��mask     ����ʧ��
						};


*@param[in]   iSec ��		����ʱ�䣬��λ�� -1 ��ʾ���޶���0��ʾ�ⶳ
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/


CS_CGI_API
int cs_ip_block_by_region(int *pHandle, int iRegionID, const char * pszIP,
                          const char* pszMask, int iSec, int iOpType,
                          int iTimeOut);


/** ��ȡĳ����������IP���б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ������ID
*@param[in]   pszIP ��	���ʮ����IP��ַ, 
						���ΪNULL ��������������������IP����Ϣ�����򷵻�ָ��IP��Ϣ
*@param[in]   pstOut      ������ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/
CS_CGI_API
int cs_get_ip_block_list_by_region(int *pHandle, int iRegionID,
                                   const char *pszIP, ZONEGETIPLIMITRES *pstOut,
                                   int iTimeOut);


/**��ȡĳ���ʺŵ����н�ɫ�б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   pszLoginName ��	��¼��
*@param[out]   pstOut ��		���صĽ�ɫ�б�����
                                                ���proto_ss.h�е�ע��
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/
//CS_CGI_API
//int cs_get_account_rolelist(int *pHandle, int iRegionID,
//                            const char *pszLoginName, ACCGETROLELISTRES *pstOut,
//                            int iTimeOut);


/*----------------------------------------------------------*/






/**�߽�ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��	��ɫ��
*@param[out]   pstOut ��   ���,����ΪNULL
                                         
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��,��λ����
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR

*@note  Ҫ���ɫ����
*/
CS_CGI_API
int cs_role_kick_off(int *pHandle, int iRegionid, int iWolrdID, 
	                 const char *pszRoleName, ZONEROLELOGOUTRES *pstOut,
	                 int iTimeOut);


/**���ý�ɫGMȨ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��	��ɫ��
*@param[out]   pstOut ��   ���,����ΪNULL
*@param[in]   iPerm ��	GMȨ��ֵ                                     
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��,��λ����
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR

*/
CS_CGI_API
int cs_set_role_perm(int *pHandle, int iRegionid, int iWolrdID,
	                 const char *pszRoleName, int iPerm,
	                 ZONEDELROLERES *pstOut, int iTimeOut);


/**ɾ����ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��		

*@param[out]   pstOut ��   ���,����ΪNULL
                                         
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��,��λ����
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR

*@note  Ҫ���ɫ������
*/
CS_CGI_API
int cs_del_role(int *pHandle, int iRegionid, int iWolrdID,
                const char *pszRoleName, ZONEDELROLERES *pstOut, int iTimeOut);



/*��ȡ��ɫ�������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��		

*@param[out]   pstOut ��   ���,����ΪNULL

						����������ܽṹ��Ϣ
						struct tagSecondarySkillInfo
						{
						    unsigned short SkillType;                       ��������: �ɿ��ҩ�ȵ� Bind Macrosgroup:SECONDARY_SKILL_TYPE,
						    unsigned short SkillLevel                      	���ܵȼ� 
						    unsigned int SkillVal;                        	  ���������� 
						    unsigned char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
						    int CoolDownNum;                    
						    RECIPECOOLDOWNINFO CoolDownInfo[MAX_SECONDARY_SKILL_COOLDOWN_RECIPE];  �䷽CD
						}

						unsigned char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
						32��char ��λ��¼��ѧϰ���䷽��λ�þ���RecipeID�䷽ID(���RecipeDef) 
						�䷽ID 0-255 (ʵ��Ӧ��1-255)

						����: �����Ƿ�ѧϰ��ĳ���䷽ID

						cFlag	=	1 << (iRecipeID%8); 
						
						if(pstSkillInfo->RecipeInfo[iRecipeID/8] & cFlag)
						{
							ѧϰ��
						}
						 
                                         
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��,��λ����
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/
CS_CGI_API
int cs_get_life_skill(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, GETLIFTSKILLRES*pstOut,
	                  int iTimeOut);

/*���ý�ɫ�����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pstReq		�������Ϣ

			struct tagSetLiftSkillReq
			{
			    int SkillOpType;                   			// 	ȡֵΪSETLIFTSKILL_OP_DEL(ɾ��)  SETLIFTSKILL_OP_MOD(�޸�)
			    int SkillType;                      			// 	��������
			    int SkillVal;                       			//  	������
			    int RecipeOpType;                    		// 	ȡֵΪRECIPE_OP_DEL(ɾ��)  RECIPE_OP_ADD(���)  RECIPE_OP_NO(�䷽���ı�)
			    int RecipeID;      					// 	�䷽ID                 
			    char RoleName[MAXROLENAME];           	//  	��ɫ�� 
			};
			���SkillOpType����SETLIFTSKILL_OP_DEL��ֻ��Ҫ��дSkillType��RoleName�ֶ�

			
*@param[out]   pstOut ��   ���,����ΪNULL
                                         
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��,��λ����
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�proto_ss.h�е�enum tagCS_ERR
*/
CS_CGI_API
int cs_set_life_skill(int *pHandle, int iRegionid, int iWolrdID, SETLIFTSKILLREQ *pstReq,
					SETLIFTSKILLRES *pstOut,int iTimeOut);


/**��ȡ��ɫ��Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID

*@param[in]   pszRoleName   ��ɫ��


*@param[out]   pstOut ��   �����ɫ��Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�enum tagCSErr
*/
CS_CGI_API
int cs_get_role_info(int *pHandle, int iRegionid, int iWorldid, char *pszRoleName,
                     ZONEGETROLEDETAILRES *pstOut,  int iTimeout);




/**���ý�ɫ��Ϣ ���ô˽ӿ�֮ǰ,�����ȵ���cs_get_role_info����ȡ��ɫ��Ϣ.
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID

*@param[in]   pstSSRoleDetail ��ɫ��Ϣָ��,������cs_get_role_info�ӿڻ�ȡ����Ϣ��ʼ��

						

*@param[out]   pstOut ��   �����ɫ��, ����ΪNULL
                                          ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�enum tagCSErr
*@note  Ҫ���ɫ������

*@note  �˽ӿ�ֻ���޸�Level  MapID  X  Y  Silence  Block  Perm ��RoleDetailInfo�ṹ�е��ֶ�
*/
CS_CGI_API
int cs_set_role_info(int *pHandle, int iRegionid,  int iWolrdID,
                     ZONEGETROLEDETAILRES *pstSSRoleDetail, 
                     ZONESETROLEDETAILRES *pstOut, int iTimeOut);

/** ���ҹ����б�
 */
struct _FindClanContext;
typedef struct _FindClanContext *FIND_CLAN_HANDLE;


/**ͨ��������ģ����ѯָ����ָ���������Ĺ����б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   iWorldID ��	��ID
*@param[in]   pszClanNamePattern ��	��������֧��MySQLͨ���
*@param[out]   pFind ��	��ѯ���������˾���������cs_close_handle�رա�
*@param[out]   pstOut ��	��ɫ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ����������cs_find_next_clan_profile
     ��Ҫ����cs_close_handle �رղ�ѯ
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


/**��ȡ�����б����������
*@param[in]   hFind ��	��ѯ���,��cs_find_first_clan_profile_by_name�������ء�
*@param[out]   pstOut ��	�����б���Ϣ�����proto_ss.h�е�ע��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʺ������뷴�����ñ�������ֱ������ֵ<0������ֵ����CS_ERR_NOT_FOUND
  ˵���ѻ�ȡ��ȫ�����ʺ���.
*/
CS_CGI_API
int cs_find_next_clan_profile(FIND_CLAN_HANDLE hFind, ZONEFINDCLANRES *pstOut);


/**��ָ������Ļ᳤ְλ��ĳһָ���ĳ�Ա����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   iWorldID ��	��ID
*@param[in]   pszClanName ��	������
*@param[in]   pszRoleName ��	��ɫ����������᳤����˽���ְλ��
*@param[in]  iTimeout      ���ó�ʱʱ��
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


/**��ָ�������ָ���⽻��ϵɾ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionID ��	����ID
*@param[in]   iWorldID ��	��ID
*@param[in]   pszClanName ��	������
*@param[in]   pszOtherClanName ��	��һ��������������˹�����⽻��ϵɾ��
*@param[in]  iTimeout      ���ó�ʱʱ��
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


/**��ȡ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��

*@param[in]   pszClanName  ������, ����cs_get_role_info ���ز����л�ȡ
*@param[out] pstOut            �������ĸ�Ҫ��Ϣ        �����Ҫ��ϸ��Ϣ�ټ�  

*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_clan_info(int *pHandle, int iRegionID, int iWolrdID,
                     char *pszClanName, ZONEGETCLANINFORES *pstOut,
                     int iTimeOut);


/**��ȡȫ��������Ϣ������Ϣ
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
*@param[in]  pszClanName  ������, ����cs_get_role_info ���ز����л�ȡ,������ݿ���
* ֱ�Ӳ�ѯ�õ�.
*@param[out] pstOut       ��������ȫ����Ϣ
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_get_clan_all_info(int *pHandle, int iRegionID, int iWolrdID,
                         const char *pszClanName, ZONEGETCLANALLINFORES *pstOut,
                         int iTimeOut);


/**��ָ���Ĺ�����ɾ��һ��ָ���ĳ�Ա
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
*@param[in]  pszClanName  ������, ����cs_get_role_info �����л�ȡ,������ݿ���
* ֱ�Ӳ�ѯ�õ�.
*@param[in]  pszRoleName  ��ɫ��, �˽�ɫ���������pszClanNameָ���Ĺ�����
* ע��:�㲻�ܴӹ�����ɾ���᳤, ��Ϊ�����ᵼ�����������Զû�л᳤.
* �������ɾ���᳤, ����õ�һ������Ľ��.
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_del_clan_member(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszClanName, const char *pszRoleName,
                       int iTimeOut);


/**��ɢһ��ָ���Ĺ���
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
*@param[in]  pszClanName  ������, ����cs_get_role_info �����л�ȡ,������ݿ���
* ֱ�Ӳ�ѯ�õ�.
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_disband_clan(int *pHandle, int iRegionID, int iWolrdID,
                    const char *pszClanName, int iTimeOut);


/**���һ��ָ��������⽻��ϵ
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
*@param[in]  pszClanName  ������, ����cs_get_role_info �����л�ȡ,������ݿ���
* ֱ�Ӳ�ѯ�õ�.
*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_clear_clan_diplomatism(int *pHandle, int iRegionID, int iWolrdID,
                              const char *pszClanName, int iTimeOut);

/**����һ������Ļ�����Դ,��,��,��,��
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
@param[in]  pszClanName  ������ ����cs_get_role_info �����л�ȡ,������ݿ���ֱ�Ӳ�ѯ�õ�.
@param[in]  iMoney  ��Ǯ 
@param[in]  iStable  ������ 
@param[in]  iActive  ���ٶ� �����ֵ�޸�����ʱ�����壬����ݹ�ʽ���ȥ
@param[in]  iTech  �Ƽ����� 

*@retval 0   succ
*@retval < 0 fail  
*/
CS_CGI_API
int cs_set_clan_base_attr(int *pHandle, int iRegionID, int iWorldID
                              , const char *pszClanName, int iMoney, int iStable
                              , int iActive, int iTech,int iTimeout);


/**��ȡ��������Ϣ(�����������ͳ�������)
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��

*@param[in]   pszRoleName  ��ɫ��
*@param[out] pstOut           ����NULL

*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_black_info(int *pHandle, int iRegionID, int iWolrdID,
                      char *pszRoleName, ZONEGETBLACKRES *pstOut, int iTimeOut);



/**��ȡ������Ϣ(�������� ,�ֵ�,����,ʦ��ͽ�� )
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��

*@param[in]   pszRoleName  ��ɫ��
*@param[out] pstOut       ����NULL    

*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_friend_info(int *pHandle, int iRegionID, int iWolrdID,
                       char *pszRoleName, ZONEGETFIRENDRES *pstOut,
                       int iTimeOut);


/**������Ʒʵ����һ���ڸ���ɫ����ʱʹ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   pstReq ��	���󴴽�����ƷID������
*@param[out]   pstOut ��	���صĸ�����Ϣ
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_create_role_item(int *pHandle, const ZONECREATEITEMREQ *pstReq,
                        ZONECREATEITEMRES *pstOut);



/**��ȡ��ɫ���ߺ�����װ����Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��

*@param[out]   pstOut ��   �����ɫ����������װ����Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  ����ԭ��μ�enum tagCSErr
*/
CS_CGI_API
int cs_get_item_info(int *pHandle, int iRegionid, int iWolrdID, 
                     char *pszRoleName, ZONEGETROLEITEMLISTRES *pstOut, 
                     int iTimeOut);



/**���ý�ɫ���ߺ�װ����Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iListType        ��������    ȡֵΪ:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// ���ϰ� 
			    LIST_TYPE_TASK = 2, 		// ����� 
			    LIST_TYPE_SUNDRIES = 3, 	// ����� 
			    LIST_TYPE_WEAR = 6, 		// ����
			    LIST_TYPE_STASH = 5		// �ֿ�
			};
			
*@param[in]    pstGrid 		��ɫ��Ʒ�ṹ,����cs_get_item_info��ȡ
*@param[in]    iMode           ��������  ȡֵΪ
			 ROLE_ITEM_DEL = 0, 			// ɾ��
   			 ROLE_ITEM_MODIFY = 1, 		// �޸�
    			 ROLE_ITEM_ADD = 2			// ���

    			 
*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API
int cs_set_item_info(int *pHandle, int iRegionid,  int iWolrdID,
					 const char *pszRoleName,int iListType, 
					 const ROLEGRID *pstGrid, int iMode,
					 ZONESETROLEITEMLISTRES *pstOut, int iTimeOut);


/**��ȡ��ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   llWid ��		RoleID
*@param[in]   pszRoleName ��ɫ��

*@param[out]   pstOut ��   ���������Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_skill_info(int *pHandle, int iRegionid, int iWolrdID,
                      const char *pszRoleName, ZONEGETROLESKILLINFORES *pstOut,
                      int iTimeOut);


/**���ý�ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iSkillID       ����ID
*@param[in]   iSkillLevel   ���ܵȼ�
*@param[in]   iMode	    ȡֵΪ:
			enum tagRoleSkillOp 
			{
			    ROLE_SKILL_DEL = 0, 		// ɾ��һ������
			    ROLE_SKILL_MODIFY = 1, 	// �޸�һ������
			    ROLE_SKILL_ADD = 2, 		// ���һ������
			};

*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API
int cs_set_skill_info(int *pHandle, int iRegionid,  int iWolrdID,
					  const char *pszRoleName, unsigned int iSkillID, 
					  unsigned int iSkillLevel, int iMode, 
					  ZONESETROLESKILLINFORES *pstOut, int iTimeOut);


/**��ȡ��ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��

*@param[out]   pstOut ��   ���, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_diathesis_info(int *pHandle, int iRegionid, int iWolrdID, 
                          const char *pszRoleName,
                          ZONEGETROLEDIATHESISRES *pstOut, int iTimeOut);



/**���ý�ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iDiathsisID �� ���ID
*@param[in]   iDiathsisLevel   ����ȼ�
*@param[in]   iMode: ȡֵΪ:
			enum tagRoleDiathesisOp 
			{
			    ROLE_DIATHESIS_DEL = 0, 		// ɾ��
			    ROLE_DIATHESIS_MODIFY = 1,      // �޸�
			    ROLE_DIATHESIS_ADD = 2,           // ���
			};

*@param[out]   pstOut ��   �����ɫ������ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API
int cs_set_diathsis_info(int *pHandle, int iRegionid,  int iWolrdID,
                         const char *pszRoleName,
                         unsigned int iDiathsisID, unsigned int iDiathsisLevel,
                         int iMode, ZONESETROLEDIATHESISRES*pstOut,
                         int iTimeOut);



/**��ȡ��ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��

*@param[out]   pstOut ��   ���, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_get_task_info(int *pHandle, int iRegionid, int iWolrdID, 
                     const char *pszRoleName, ZONEGETROLETASKRES *pstOut,
                     int iTimeOut);



/**ɾ����ɫһ���ѽ�����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   uiTaskID		����ID

*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API
int cs_del_task_by_id(int *pHandle, int iRegionid,  int iWolrdID,
					  const char *pszRoleName,unsigned int uiTaskID,
					  ZONEDELROLETASKRES *pstOut, int iTimeOut);


/**�����ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID

*@param[in]   pszRoleName ��ɫ��

*@param[in]   iBlockTime	����ʱ��  ��λ��(������ʾ���ö���  ����Ϊ0)

*@param[out]   pstOut ��   ���, ����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_block_role(int *pHandle, int iRegionid, int iWolrdID, 
                  const char *pszRoleName, int iBlockTime, 
                  ZONEBLOCKPLAYERRES *pstOut, int iTimeOut);

/**�ⶳ��ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID

*@param[in]   pszRoleName ��ɫ��


*@param[out]   pstOut ��   ���, ����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_unblock_role(int *pHandle, int iRegionid, int iWolrdID, 
                    const char *pszRoleName,  ZONEBLOCKPLAYERRES *pstOut,
                    int iTimeOut);


/**���Խ�ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iSilenceTime	����ʱ��  ��λ��(���븺�����ý��� ����Ϊ0)

*@param[out]   pstOut ��   ���, ����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_silence_role(int *pHandle, int iRegionid, int iWolrdID, 
                    const char *pszRoleName, int iSilenceTime,
                    ZONESILENCEROLERES *pstOut, int iTimeOut);


/**��ɫ�������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��

*@param[out]   pstOut ��   ���, ����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_unsilence_role(int *pHandle, int iRegionid, int iWolrdID,  
                      const char *pszRoleName,ZONESILENCEROLERES *pstOut, int iTimeOut);

/**��ת��ɫ��ͼ��λ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName   ��ɫ��
*@param[in]   iMap			��ͼID
*@param[in]	  x,y			����

*@param[out]   pstOut ��    ���, ����ΪNULL
*@iTimeout[in]  iTimeout    ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_chg_role_pos(int *pHandle, int iRegionid, int iWolrdID, 
	                const char *pszRoleName, int iMap,int x,int y,
	                ZONECHGROLEPOSRES *pstOut,int iTimeOut);


/**�޸Ľ�ɫ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName         ��ɫ��
*@param[in]   pszChgRoleName   �޸Ľ�ɫ��

*@param[out]   pstOut ��   ���, ����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��,
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_chg_role_name(int *pHandle, int iRegionid, int iWolrdID, const char *pszRoleName, 
	                 const char *pszChgRoleName,ZONECHGROLENAMERES *pstOut, int iTimeOut);


/**�ָ��߼�ɾ���Ľ�ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   llWid ��		RoleID
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_restore_deleted_role(int *pHandle, int iRegionID, int iWorldID, 
	                        long long llWID, int iTimeOut);


/** ��ѯĳ����ɫ���ʼ�ͷ�б�
 */
struct _FindMaillistContext;
typedef struct _FindMaillistContext *FIND_MAILLIST_HANDLE;


/**��ȡĳ����ɫ�ʼ��б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��		��ɫ��
*@param[in]   llWid ��		RoleID
*@param[out]   pstOut ��		��ɫ�ʼ��б�
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/

/*
CS_CGI_API 
int cs_get_role_maillist(int *pHandle, int iRegionID, int iWorldID, 
	                     const char *pszRoleName, ZONEGETMAILLISTRES *pstOut, 
	                     int iTimeOut);
*/

/**��ȡĳ����ɫ�ʼ�ͷ�б�ĵ�һ����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��		��ɫ��
*@param[in]   pFind ��		���صĲ�ѯ���,ʹ�ú�����cs_close_handle
*@param[out]   pstOut ��		��ɫ�ʼ��б�
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API
int cs_find_first_maillist(int *pHandle, int iRegionID, int iWorldID,
                           const char *pszRoleName, FIND_MAILLIST_HANDLE *pFind,
                           ZONEGETMAILLISTRES *pstOut, int iTimeOut);


/**��ȡ��ɫ�ʼ�ͷ�б����������
*@param[in]   hFind ��	��ѯ���,��cs_find_first_maillist�������ء�
*@param[out]   pstOut ��	�ʼ�ͷ�б���Ϣ
                                         ���proto_ss.h�е�ע��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*@�����ȡȫ���ʼ�ͷ�б��뷴�����ñ�������ֱ������ֵ<0��
  ����ֵ����CS_ERR_NOT_FOUND˵���ѻ�ȡ��ȫ�����ʺ���.
*/
CS_CGI_API
int cs_find_next_maillist(FIND_MAILLIST_HANDLE hFind,
                          ZONEGETMAILLISTRES *pstOut);



/**��ȡĳ����ɫĳ���ʼ��ľ�������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��		��ɫ��
*@param[in]   llMailID ��		�ʼ�ID
*@param[out]   pstOut ��		�ʼ���������
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_role_maildata(int *pHandle, int iRegionID, int iWorldID, 
	                       const char *pszRoleName, long long llMailID, 
	                       MAIL *pstOut, int iTimeOut);


/**ɾ��ĳ����ɫ��ĳ���ʼ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��		��ɫ��
*@param[in]   llMailID ��		�ʼ�ID
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_del_role_mail(int *pHandle, int iRegionID, int iWorldID, 
                     const char *pszRoleName, long long llMailID, int iTimeOut);


/**��ĳ����ɫ����ϵͳ�ʼ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszTo ��		�ռ�����
*@param[in]   pszSend ��		��������
*@param[in]   pszTitle ��		�ʼ����⣬����ΪNULL
*@param[in]   pszText ��		�ʼ����ģ�����ΪNULL
*@param[in]   astItems ��		�ʼĵ���Ʒ������ΪNULL
*@param[in]   iItemNum ��		�ʼĵ���Ʒ����
*@param[in]   nMoney ��		�ʼĵĽ�Ǯ
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_send_sys_mail(int *pHandle, int iRegionID, int iWorldID, 
                     const char *pszTo, const char *pszSend, const char *pszTitle, 
	                   const char *pszText, const ROLEGRID *astItems, int iItemNum,
	                   unsigned int nMoney, int iTimeOut);



/**�ƶ�ĳ����ɫ����Ʒ����һ��ɫ����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   pstSrc ��	Դ��ɫ,�Ӵ˽�ɫ����ȡ����Ʒ
*@param[in]   pstDst ��	Ŀ�Ľ�ɫ,��pstSrc����ȡ�µ���Ʒ�ŵ��˽�ɫ����
*@param[in]   iListType ��		�ƶ�����Ʒ���ĸ�����    ȡֵΪ:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// ���ϰ� 
			    LIST_TYPE_TASK = 2, 		// ����� 
			    LIST_TYPE_SUNDRIES = 3, 	// ����� 
			    LIST_TYPE_WEAR = 6, 		// ����
			    LIST_TYPE_STASH = 5		// �ֿ�
			};
*@param[in]   pstGrid ��		�ƶ�����Ʒ�ĸ�����Ϣ
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ��˫����ɫ��������
*/
CS_CGI_API 
int cs_mov_role_item(int *pHandle, const SSROLESTORAGEPATH *pstSrc,
                     const SSROLESTORAGEPATH *pstDst, int iListType,
                     const ROLEGRID *pstGrid, int iTimeOut);


/**���ƽ�ɫ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   llSrcID ��	Դ��ɫID
*@param[in]   llDstID ��	Ŀ���ɫID
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ��˫����ɫ��������
*/
CS_CGI_API 
int cs_copy_role(int *pHandle, int iRegionID, int iWorldID, long long llDstID,
                 const char *pszDstName, long long llSrcID,
                 const char *pszSrcName, int iTimeOut);



/**��ȡһ���������еĹ����б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[out]   pstOut ��	���صĹ����б�
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_bulletin_list(int *pHandle, int iRegionID, int iWorldID,
	                     ZONEGETBULLETINLISTRES *pstOut, int iTimeOut);




/**���һ��������Ϣ��һ��������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszText ��	�����ı�
*@param[in]   iRollInterval ��	�������ʱ��
*@param[in]   iActiveTime ��	��Ծʱ��

*@param[in]   tActiveTimeStart ��	��time(NULL) ��ͬ��ʽ��ʱ��
                                                       0 ��ʾ�������յ���Ϣ��������ʾ����
								
*@param[out]   piID ��	�����ӳɹ�,�򷵻ع���ID,��ΪNULL
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_add_bulletin(int *pHandle, int iRegionID, int iWorldID, 
                    const char *pszText, int iRollInterval, int iActiveTime, int tActiveTimeStart,
                    int *piID, int iTimeOut);



/**���һ��������Ϣ�����з�����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   pszText ��	�����ı�
*@param[in]   iRollInterval ��	�������ʱ��
*@param[in]   iActiveTime ��	��Ծʱ��

*@param[in]   *@param[in]   tActiveTimeStart ��	��time(NULL) ��ͬ��ʽ��ʱ��
                                           0 ��ʾ�������յ���Ϣ��������ʾ����
								
*@param[out]   piID ��	�����ӳɹ�,�򷵻ع���ID,��ΪNULL
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_add_bulletin_to_all(int *pHandle, const char *pszText, int iRollInterval,
                           int iActiveTime, int tActiveTimeStart, int *piID, int iTimeOut);



/**ɾ��һ��������Ϣ��һ��������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   iID ��	��ɾ���Ĺ���ID
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_del_bulletin(int *pHandle, int iRegionID, int iWorldID, int iID,
                    int iTimeOut);


/**ɾ��һ��������Ϣ�����еķ�����
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iID ��	��ɾ���Ĺ���ID
*@param[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_del_bulletin_from_all(int *pHandle, int iID, int iTimeOut);


/**��ȡ��ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[out]   pstOut ��   ���������Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_ride_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName,ZONEGETROLERIDEINFORES *pstOut,
	                    int iTimeOut);

/**���ý�ɫ�����Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iRideID       ���ID
*@param[in]   iRideLevel   ��˵ȼ�
*@param[in]   iEndTime  ��˵���ʱ��(��1970 ��ʼ�����ڵ���) , 0 ������
*@param[in]   iMode	    ȡֵΪ:
			enum tagRoleSkillOp 
			{
			    ROLE_RIDE_DEL = 0, 		// ɾ��һ������
			    ROLE_RIDE_MODIFY = 1, 	// �޸�һ������
			    ROLE_RIDE_ADD = 2, 		// ���һ������?
			};

*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_set_ride_info(int *pHandle, int iRegionid,  int iWolrdID,
                     const char *pszRoleName, unsigned int iRideID,
                     unsigned int iRideLevel, int iEndTime, int iMode,
                     ZONESETROLERIDEINFORES *pstOut, int iTimeOut);

/**��ȡ��ɫ��Լ�б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[out]   pstOut ��    �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note
*/
CS_CGI_API 
int cs_get_role_qiyue_list(int *pHandle, int iRegionid,  int iWolrdID,
                           const char *pszRoleName, GETROLEQIYUELISTRES *pstOut,
                           int iTimeOut);

/**���ý�ɫ��ǰʹ�õ���Լ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[in]   iQiYue        ��Ҫ����ʹ�õ���Լid
*@param[out]   pstOut ��    �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_set_role_qiyue(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, unsigned int iQiYueID,
                      SETROLEQIYUERES *pstOut, int iTimeOut);

/**���ӽ�ɫ��Լ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[in]   iQiYue        ��Ҫ����ʹ�õ���Լid
*@param[out]   pstOut ��    �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_add_role_qiyue(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName,unsigned int iQiYueID,
                      ADDROLEQIYUERES *pstOut, int iTimeOut);

/**ɾ����ɫ��Լ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[in]   iQiYue        ��Ҫ����ʹ�õ���Լid
*@param[out]   pstOut ��    �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_del_role_qiyue(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, unsigned int iQiYueID,
                      DELROLEQIYUERES *pstOut, int iTimeOut);


/**�޸ľ���ȼ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[in]   FairyID       ����ID
*@param[in]   FairyLv       ����ȼ�
*@param[out]   pstOut ��    ���,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_modify_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					      const char *pszRoleName, unsigned int FairyID,
					      unsigned int FairyLv, ZONESETROLEFAIRYRES*pstOut,
					      int iTimeOut);


/**ɾ������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[in]   FairyID       ����ID
*@param[out]   pstOut ��    ���,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_del_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					            const char *pszRoleName, unsigned int FairyID,
					            ZONESETROLEFAIRYRES*pstOut, int iTimeOut);


/**��Ӿ���
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName 	��ɫ��
*@param[in]   FairyID       ����ID
*@param[out]   pstOut ��    ���,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_add_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					   const char *pszRoleName, unsigned int FairyID,
					   ZONESETROLEFAIRYRES*pstOut, int iTimeOut);


/**��ȡ��ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[out]   pstOut ��   ���������Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/
CS_CGI_API 
int cs_get_pet_info(int *pHandle, int iRegionid, int iWolrdID, 
	                const char *pszRoleName, ZONEGETROLEPETINFORES *pstOut,
	                int iTimeOut);

/**���ý�ɫ������Ϣ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   pstRolePet     ����ṹ,����cs_get_pet_info��ȡ
*@param[in]   iMode	    ȡֵΪ:
			enum tagRoleSkillOp 
			{
			    ROLE_PET_DEL = 0, 		// ɾ��һ������
			    ROLE_PET_MODIFY = 1, 	// �޸�һ������
			    ROLE_PET_ADD = 2, 		// ���һ������
			};

*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_set_pet_info(int *pHandle, int iRegionid,  int iWolrdID,
                    const char *pszRoleName,  ROLEPET *pstRolePet, int iMode,
                    ZONESETROLEPETINFORES *pstOut, int iTimeOut);


/**Ϊ��ɫ���һ���ƺ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iDesignationID �ƺ�ID
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_add_designation(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszRoleName, int iDesignationID,
                       int iTimeOut);


/**ɾ����ɫ��һ���ƺ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iDesignationID �ƺ�ID
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_del_designation(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszRoleName, int iDesignationID,
                       int iTimeOut);


/**��ȡ��ɫ�ƺ��б���ǰʹ�õĳƺ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[out]   pstOut ��   ���سƺ���Ϣ
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_get_designation_list(int *pHandle, int iRegionid, int iWolrdID,
                            const char *pszRoleName, ROLEDESIGNATIONS *pstOut,
                            int iTimeOut);


/**Ϊ��ɫ���/����һ����������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iRespactPartyID ��ӪID
*@param[in]   iRespactValue ����ֵ
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@note  �����ɫû�������Ӫ���������ݣ���������һ����
*       ����iRespactValue��������ֵ��
*/
CS_CGI_API 
int cs_set_respact(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, int iRespactPartyID,
                   int iRespactValue, int iTimeOut);

/**ɾ��һ����ɫ��������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iRespactPartyID ��ӪID
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_del_respact(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, int iRespactPartyID, int iTimeOut);



/*��ȡ��ɫ��ȫ������		Ҫ���ɫ������
*@param[in]   pszRoleName ��ɫ��
*@param[out]   pstOut ����Ϊ��
*/
CS_CGI_API 
int cs_get_safelock(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, GETSAFELOCKRES *pstOut,int iTimeOut);


/*���ý�ɫ��ȫ������		Ҫ���ɫ������
*@param[in]   	pszRoleName 		��ɫ��
*@param[in]   	pszPassWD 		�����ַ���
*@param[out]   	pstOut 			����Ϊ��
*/
CS_CGI_API 
int cs_set_safelock(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, const char* pszPassWD, SETSAFELOCKRES *pstOut, int iTimeOut);



/*���ý�ɫ�Ķ������� */
int cs_set_second_pwd(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, const char* pszPassWD, int iTimeOut);




/**��ȡ��ɫ�����б�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   llWid ��		RoleID
*@param[in]   pszRoleName ��ɫ��
*@param[out]   pstOut ��   ���������б�
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*/
CS_CGI_API 
int cs_respact_list(int *pHandle, int iRegionid, int iWolrdID,
                    const char *pszRoleName, RESPACTINFO *pstOut, int iTimeOut);


/**cs�ӿ����Ӱ��������ʺŲ�ѯ��ȯ��ص���Ϣ��ע��ֻ��ѯ�����޸ģ�
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginNamePattern ��	�˻�����֧��MySQLͨ���
*@param[out]   pstOut ��	���ص����billing������
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*/
CS_CGI_API
int cs_query_bill_point(
	int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	BILLRECORD *pstOut,
	int iTimeOut);

/**cs�ӿ� ��ֵ���Ӧ������ӵĵ���֪ͨ������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   pszLoginName ��	�˻���
*@param[in]   iUin �� �ʺ�Uin
*@param[in]   iGroupID �� ���id
*@param[in]   iAddCash ��	�����Ӧ�ĳ�ֵ��������ֵ

*@param[out]   pstOut ��	���ص����billing������
                                         ���proto_ss.h�е�ע��
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail
*@retval CS_ERR_NOT_FOUND û���ҵ�
*/
int cs_cash_props_gift_bill(int *pHandle,
	int iRegionID,
	const char *pszLoginName,
	int iUin,
	int iGroupID,
	int iAddCash,
	CASHPROPSGIFTINFORES *pstOut,
	int iTimeOut);


/**��ȡ��ɫָ����������������Ϣ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iListType       ��������, ȡֵΪ:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// ���ϰ� 
			    LIST_TYPE_TASK = 2, 		// ����� 
			    LIST_TYPE_SUNDRIES = 3, 	// ����� 
			    LIST_TYPE_STASH = 5		// �ֿ�
			};

*@param[out]   pstOut ��   �����ѯ�ı�������������Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/

CS_CGI_API
int cs_get_package_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, ZONEGETROLEPACKAGERES *pstOut,
	                    int iTimeOut);

/**���ý�ɫָ�������ĸ��ӽ���
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iListType       	��������, ȡֵΪ:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// ���ϰ� 
			    LIST_TYPE_TASK = 2, 		// ����� 
			    LIST_TYPE_SUNDRIES = 3, 	// ����� 
			    LIST_TYPE_STASH = 5		// �ֿ�
			};
			
*@param[in]   iNum       	�������ӵĽ�����������
*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API 
int cs_set_package_info(int * pHandle, int iRegionid, int iWolrdID, 
				const char * pszRoleName, int iListType, int iNum, ZONESETROLEPACKAGERES * pstOut, 
				int iTimeOut);


/**��ѯ�̳���ʱ����(ָ������)�е���Ʒ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iListType       ��������, ȡֵΪ:
			enum tagLIST_TYPE 
			{
			    LIST_TYPE_STUFF = 1, 		// ���ϰ� 
			    LIST_TYPE_TASK = 2, 		// ����� 
			    LIST_TYPE_SUNDRIES = 3, 	// ����� 
			    LIST_TYPE_STASH = 5		// �ֿ�
			    LIST_TYPE_SHOPPING = 9, 	// ��Ʒ�� 
			};

*@param[out]   pstOut ��   ���ָ������������ 
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/

CS_CGI_API
int cs_get_package_item_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, ZONEGETPACKAGEITEMINFORES*pstOut,
	                    int iTimeOut);


/*����LogRoleSnapshot���ý�ɫ�ص�
	*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
	*@param[in]   iRegionid ��	����ID
	*@param[in]   iWorldid ��	��ID
	*@param[in]   pszRoleName ��ɫ��
	*@param[in]   pszDate 		�ص�ʱ��  �̶���ʽΪ"2011-05-12 20:23:16",�����LogRoleSnapshot���и���ʱ��
*/

CS_CGI_API
int cs_role_back_record(int *pHandle, int iRegionid, int iWolrdID,const char *pszRoleName,const char *pszDate, 
							ZONEROLEBACKRES *pstOut, int iTimeOut);

/**��ȡ��ɫ��������Ҫ��Ʒ����Ϣ��
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��

*@param[out]   pstOut ��   �����ѯ�Ľ�ɫ������Ϣ, ��������Ч��ָ��.
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*/

CS_CGI_API
int cs_get_role_drop_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, ZONEGETROLEDROPRES *pstOut,
	                    int iTimeOut);

/**�ָ���ɫ��������Ҫ��Ʒ
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   iDropType      ��������, ȡֵΪ:
			enum tagROLE_DROP_TYPE 
			{
			    ROLE_DROP_GRID = 0, 		// ��Ʒ
			    ROLE_DROP_PET = 1, 		// ����
			};
*@param[in]   pstRestoreInfo      �������ݽṹ,���cs_get_role_drop_info��ȡ����Ϣ
*@param[in]   pszRoleName ��ɫ��
			
*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/
CS_CGI_API
int cs_role_drop_restore(int *pHandle, int iRegionid, int iWolrdID,
			     	int iDropType, const ROLEDROPRESTOREINFO *pstRestoreInfo, const char *pszRoleName, 
			     	ROLEDROPRESTORERES *pstOut, int iTimeOut);

/**�������
*@param[in]   pHandle ��	����Ϊ��,init_interface��ʼ����ָ��
*@param[in]   iRegionid ��	����ID
*@param[in]   iWorldid ��	��ID
*@param[in]   pszRoleName ��ɫ��
*@param[in]   iPetID      	���PetDef�ж���ID
*@param[in]   iPetLevel      	����ȼ����� MAX_PET_LEVEL

*@param[in]   iPetGenius	�������ʣ�ȡֵΪ:
			enum tagPET_GENIUS_TYPE 
			{
			    PET_GENIUS_D = 0, 		��ͨ����
			    PET_GENIUS_C = 1, 		�������� 
			    PET_GENIUS_B = 2, 	 	��������
			    PET_GENIUS_A = 3, 		�ܳ�����
			    PET_GENIUS_S = 4, 	 	�������� 
			}
*@param[in]   iStrengthenLevel      ǿ���ȼ� ����:MAX_PET_STRENGTHEN_LEVEL

*@param[out]   pstOut ��   �����ɫ��,����ΪNULL
*@iTimeout[in]  iTimeout      ���ó�ʱʱ��
*@retval 0   succ
*@retval <0  fail  
*@note  Ҫ���ɫ������
*/

CS_CGI_API
int cs_role_hatch_pet(int *pHandle, int iRegionid, int iWolrdID,
			     	const char *pszRoleName, int iPetID, int iPetLevel, int iPetGenius, 
			     	int iStrengthenLevel, ZONEROLEHATCHPETRES *pstOut, int iTimeOut);

/**ͨ����ɫ��ģ����ѯ�������û���ɫ��Ϣ�ĵ�һ����
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
@param[in]  pszRoleNamePattern  ��ɫ�� ֧��MySQLͨ���
@param[out]  pFind  ��ѯ��� ����˾���������cs_close_handle�رա�
@param[out]  pstOut  ��ɫ�б���Ϣ 
@iTimeout[in]  iTimeout      ���ó�ʱʱ�� a

*@retval 0   succ
*@retval < 0 fail  
*@ retval CS_ERR_NOT_FOUND û���ҵ�
*@ �����ȡȫ����ɫ�������cs_find_all_next_role
*@ ��Ҫ����cs_close_handle �رղ�ѯ
*/
CS_CGI_API
int cs_find_all_first_role_by_rolename(int *pHandle
                              , char *pszRoleNamePattern, FIND_ACCOUNT_ROLE_HANDLE *pFind, ACCFINDROLELISTRES *pstOut
                              ,int iTimeOut);


/**��ȡ������ģ����ѯ��ɫ������������
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
@param[in]  hFind  ��ѯ��� ��cs_find_all_first_role_by_rolename��ȡ
@param[out]  pstOut  ��ɫ�б���Ϣ 
*@retval 0   succ
*@retval < 0 fail  
*@ retval CS_ERR_NOT_FOUND û���ҵ�
*@ �����ȡȫ����ɫ�����뷴�����ñ�������ֱ������ֵ<0
*@ ����ֵ����CS_ERR_NOT_FOUND˵���ѻ�ȡ��ȫ���Ľ�ɫ��.
*/
CS_CGI_API
int cs_find_all_next_role(int *pHandle
                              , FIND_ACCOUNT_ROLE_HANDLE hFind, ACCFINDROLELISTRES *pstOut);

/**ͨ��uin��ý�ɫ�б�
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
@param[in]  iRegionid  ��id
@param[in]  iWolrdID  ��id
@param[in]  iUin  ��id
@param[out]  pstOut  ��ɫ�б���Ϣ 
@param[in]  iTimeout      ���ó�ʱʱ�� 
*@retval 0   succ
*@retval < 0 fail  
*/

int cs_get_role_list(int *pHandle, int iRegionid, int iWolrdID, int iUin,
	                   ZONEROLELISTRES *pstOut, int iTimeOut);

/**ָ��������reload
*@param[in]  pHandle ��   ����Ϊ��,init_interface��ʼ����ָ��
@param[out]  pstOut  reload������Ϣ 

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
