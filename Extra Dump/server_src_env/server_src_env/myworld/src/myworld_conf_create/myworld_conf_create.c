#include "pal/pal.h"
#include "tormsvr_conf_desc.h"
#include "zone_conn_conf_desc.h"
#include "zone_svr_conf_desc.h"
#include "dir_svr_conf_desc.h"
#include "cs_svr_conf_desc.h"
#include "tconnddef.h"
#include "tlog/tlog.h"
#include "tcm_config_plunin.h"
#include "tcm_config_api.h"
#include "entity.h"
#include "tdr/tdr.h"
#include "tlog_db_conf_desc.h"
#include "tbus/tbus.h"
#include "proto_comm.h"
#include "aas_svr_conf_desc.h"
#include "version_svr_conf_desc.h"
#include "auth_svr_conf_desc.h"
#include "region_gate_conf_desc.h"
#include "informer_svr_conf_desc.h"
#include "bill_svr_conf_desc.h"
#include "account_cs_svr_conf_desc.h"
#include "mud_svr_conf_desc.h"
#include "recv_report_conf_desc.h"
#include "service_svr_conf_desc.h"


static int generate_oplog_conf(char* dir, int id, unsigned char ucEntity);

#define UNUSED( var ) ((void)var)

typedef struct SoEnv
{
	HAPIHANDLE handle; /*maintain the api handle*/
	LPTDRMETA pstZoneSvrMeta;
	LPTDRMETA pstConndMeta;
	LPTDRMETA pstDirSvrMeta;
	LPTDRMETA pstDBSvrMeta;
	LPTDRMETA pstTlogdSvrMeta;
	LPTDRMETA pstCsSvrMeta;
	LPTDRMETA pstTlogdbMeta;
	LPTDRMETA pstAasMeta;
	LPTDRMETA pstAuthMeta;
	LPTDRMETA pstBillMeta;
	LPTDRMETA pstInformerMeta;
	LPTDRMETA pstRegionMeta;
	LPTDRMETA pstVersionMeta;
	LPTDRMETA pstAccountCsSvrMeta;
	LPTDRMETA pstMudMeta;
	LPTDRMETA pstRecvMeta;
	LPTDRMETA pstServiceMeta;
	
}SoEnv;

SoEnv mydata;

LPTLOGCATEGORYINST g_pstConfCat = NULL;	/*日志类*/
extern unsigned char g_szMetalib_dir_svr[];
extern unsigned char g_szMetalib_zone_conn[];
extern unsigned char g_szMetalib_zone_svr[];
extern unsigned char g_szMetalib_tormsvr[];
extern unsigned char g_szMetalib_cs_svr[];
extern unsigned char g_szMetalib_tlog_db[];
extern unsigned char g_szMetalib_aas_svr[];
extern unsigned char g_szMetalib_auth_svr[];
extern unsigned char g_szMetalib_bill_svr[];
extern unsigned char g_szMetalib_informer_svr[];
extern unsigned char g_szMetalib_region_gate[];
extern unsigned char g_szMetalib_version_svr[];
extern unsigned char g_szMetalib_account_cs_svr[];
extern unsigned char g_szMetalib_mud_svr[];
extern unsigned char g_szMetalib_recv_report[];
extern unsigned char g_szMetalib_service_svr[];


/*每一个id 可以有15 种不同shm key*/
#define ZONE_PLAYER_SHM 1
#define ZONE_UIN_SHM 2
#define ZONE_RES_SHM 3
#define ZONE_MAP_SHM 4


#define MUD_PLAYER_SHM 1
#define MUD_UIN_SHM 2
#define MUD_OBJ_SHM 3

#define REGION_OBJ_SHM 1


#define DB_PROTO_VER_SHM 3
//#define DB_CACHE_SHM 4

#define DIR_UDP_PROT_DEF 8500
#define DIR_CONN_PROT_DEF 80
#define CS_CONN_PROT_DEF 8480
#define ZONE_CONN_PORT_DEF 8680
#define VERSION_CONN_PORT_DEF 80
#define AUTH_CONN_PORT_DEF 80
#define TLOG_DB_PORT_START 9300
#define TLOGD_PORT_START 12400
#define REGION_TLOGD_PORT_START 9500
#define REGION_TLOG_DB_PORT_START 9600
#define ACCOUNT_CS_CONN_DEF 15000
#define MUD_CONN_DEF 16000

//先在instance id 借用4bit，以后不够再到world id 借4bit
int myworld_get_shm_key(int iID, unsigned char ucShmID)
{
	unsigned char szID[4]; 
	int iRet;
	int iNetid;

	iNetid = HTONL(iID);
	
	memcpy(szID, &iNetid, sizeof(szID));
	szID[3] +=  ucShmID*16;
	memcpy(&iRet, szID, sizeof(iRet));

	return iRet;
}

//////////////////////////////////////////////
/*
Fuction            :  myworld_conf_getname
Descption          :  获取so名称
Input              :  p             -- 插件指针
Return             :  0             -- 成功
其他          -- 失败
*/
char* myworld_conf_getname(struct config_plunin *p)
{
	UNUSED(p);
	return "myworld";
}


/*
Fuction            :  myworld_conf_getversion
Descption          :  获取so版本
Input              :  p             --插件指针
Return             :  0             -- 成功
其他          -- 失败
*/
char* myworld_conf_getversion(struct config_plunin *p)
{
	UNUSED(p);
	return "1.0.0";
}

/*
Fuction            :  myworld_conf_init
Descption          :  so 自身的初始化函数，server在config_plunin_init初始化插件成功后会调用
Input              :  p            --插件指针
Return             :  0            -- 成功
其他         -- 失败
*/
int myworld_conf_init(struct config_plunin *p)
{
	LPTDRMETALIB pstLib;

	SoEnv* pstEnv = (SoEnv*)p->data;

	g_pstConfCat = (LPTLOGCATEGORYINST)tcmapi_get_logcatinst(mydata.handle);

	pstLib = (LPTDRMETALIB)&g_szMetalib_zone_svr[0];
	pstEnv->pstZoneSvrMeta = tdr_get_meta_by_name(pstLib, "zone_svrconf");
	if (NULL == pstEnv->pstZoneSvrMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"zone_svr\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_dir_svr[0];
	pstEnv->pstDirSvrMeta = tdr_get_meta_by_name(pstLib, "dir_svrconf");
	if (NULL == pstEnv->pstDirSvrMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"dir_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_zone_conn[0];
	pstEnv->pstConndMeta = tdr_get_meta_by_name(pstLib, "zone_connconf");
	if (NULL == pstEnv->pstConndMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"zone_connconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_tormsvr[0];
	pstEnv->pstDBSvrMeta = tdr_get_meta_by_name(pstLib, "tormsvrconf");
	if (NULL == pstEnv->pstDBSvrMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"tormsvrconf\"");
		return -1;
	}	

	if (0 > tdr_load_metalib(&pstLib, "../cfg/tconnddef.tdr"))
	{
		printf("load meta lib tlogd fail\n");
		return -1;
	}
	pstEnv->pstTlogdSvrMeta =  tdr_get_meta_by_name(pstLib, "tlogdconf");
	if (NULL == pstEnv->pstTlogdSvrMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"tlogdconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_cs_svr[0];
	pstEnv->pstCsSvrMeta = tdr_get_meta_by_name(pstLib, "cs_svrconf");
	if (NULL == pstEnv->pstCsSvrMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"cs_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_tlog_db[0];
	pstEnv->pstTlogdbMeta= tdr_get_meta_by_name(pstLib, "tlog_dbconf");
	if (NULL == pstEnv->pstTlogdbMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"tlog_dbconf\"");
		return -1;
	}


	pstLib = (LPTDRMETALIB)&g_szMetalib_aas_svr[0];
	pstEnv->pstAasMeta = tdr_get_meta_by_name(pstLib, "aas_svrconf");
	if (NULL == pstEnv->pstAasMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"aas_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_auth_svr[0];
	pstEnv->pstAuthMeta = tdr_get_meta_by_name(pstLib, "auth_svrconf");
	if (NULL == pstEnv->pstAuthMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"auth_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_bill_svr[0];
	pstEnv->pstBillMeta = tdr_get_meta_by_name(pstLib, "bill_svrconf");
	if (NULL == pstEnv->pstBillMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"bill_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_informer_svr[0];
	pstEnv->pstInformerMeta = tdr_get_meta_by_name(pstLib, "informer_svrconf");
	if (NULL == pstEnv->pstInformerMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"informer_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_service_svr[0];
	pstEnv->pstServiceMeta = tdr_get_meta_by_name(pstLib, "service_svrconf");
	if (NULL == pstEnv->pstServiceMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"service_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_region_gate[0];
	pstEnv->pstRegionMeta = tdr_get_meta_by_name(pstLib, "region_gateconf");
	if (NULL == pstEnv->pstRegionMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"region_gateconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_version_svr[0];
	pstEnv->pstVersionMeta = tdr_get_meta_by_name(pstLib, "version_svrconf");
	if (NULL == pstEnv->pstVersionMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"version_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_account_cs_svr[0];
	pstEnv->pstAccountCsSvrMeta = tdr_get_meta_by_name(pstLib, "account_cs_svrconf");
	if (NULL == pstEnv->pstAccountCsSvrMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"account_cs_svrconf\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_mud_svr[0];
	pstEnv->pstMudMeta = tdr_get_meta_by_name(pstLib, "mud_svrconf");
	if (NULL == pstEnv->pstMudMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"pstMudMeta\"");
		return -1;
	}

	pstLib = (LPTDRMETALIB)&g_szMetalib_recv_report[0];
	pstEnv->pstRecvMeta = tdr_get_meta_by_name(pstLib, "recv_reportconf");
	if (NULL == pstEnv->pstRecvMeta)
	{
		tlog_error(g_pstConfCat, 0,0, "failed to get meta by name \"pstRecvMeta\"");
		return -1;
	}
	
	tlog_info(g_pstConfCat,0,0, "myworld_conf_init successfully!!!");

	return 0;
}


/*
Fuction            :  myworld_conf_cleanup
Descption          :  so 自身的初始化函数，server在释放插件之前调用
Input              :  插件指针
Return             :  0          -- 成功
其他       -- 失败
*/
int myworld_conf_cleanup(struct config_plunin *p)
{
	UNUSED(p);
	return 0;
}

int myworld_generate_sh(char *dir, int id, unsigned char ucEntity)
{
	int iRet;
	APIPROCINST stProc;
	char szSH[2048];
	char szSHPath[512];
	FILE *fp;
	char *pszId;
	
	int iNetID;
	char szID[64];

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -1;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	pszId = stProc.szAddr;

	if (FUNC_ZONE_CTRL == ucEntity || FUNC_MUD_CTRL == ucEntity)
	{
		sprintf(szSH ,"#!/bin/sh\n../bin/%s --id=%s --business-id=66  --log-level=700 --log-path=../log --bus-key=7381 --conf-path=../cfg --daemon $1 start\n",
				stProc.szProcName, pszId);
	}
	else if(FUNC_LOG_SVR == ucEntity || FUNC_LOG_DB == ucEntity || FUNC_ONLINEINFO == ucEntity || FUNC_REGION_LOG_SVR == ucEntity  || FUNC_REGION_LOG_DB == ucEntity )
	{
		sprintf(szSH ,"#!/bin/sh\n../bin/%s  --id=%s --business-id=66 --log-level=600 --log-path=../log --pid-path=../cfg --conf-path=../cfg --daemon  start\n",
				stProc.szProcName,pszId);
	}
	else
	{
		sprintf(szSH ,"#!/bin/sh\n../bin/%s --id=%s --business-id=66  --log-level=600 --log-path=../log --bus-key=7381 --conf-path=../cfg --daemon start\n",
				stProc.szProcName, pszId);
	}
	snprintf(szSHPath, sizeof(szSHPath), "%s/start_%s_%s_66.sh", dir, stProc.szProcName, szID);

	fp = fopen(szSHPath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	fwrite(szSH, strlen(szSH), 1, fp);
	fclose(fp);

	chmod(szSHPath, S_IRWXU|S_IRWXG|S_IRWXO);
	//snprintf(szCmd, sizeof(szCmd), "chmod 777 %s", szSHPath);
	//system(szCmd);
	
	if (FUNC_LOG_SVR == ucEntity || FUNC_LOG_DB == ucEntity)
	{
		sprintf(szSH, "#!/bin/sh\n../bin/%s --pid-path=../cfg  stop\n",
				stProc.szProcName);
	}
	else
	{
		sprintf(szSH, "#!/bin/sh\n../bin/%s --id=%s --business-id=66  stop\n",
				stProc.szProcName, pszId);
	}
	
	snprintf(szSHPath, sizeof(szSHPath), "%s/stop_%s_%s_66.sh", dir, stProc.szProcName, szID);

	fp = fopen(szSHPath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	fwrite(szSH, strlen(szSH), 1, fp);
	fclose(fp);

	chmod(szSHPath, S_IRWXU|S_IRWXG|S_IRWXO);
	//snprintf(szCmd, sizeof(szCmd), "chmod 777 %s", szSHPath);
	//system(szCmd);
	return 0;
}

int myworld_get_custom_string(const char *pszCustomAttr, char *pszKey, char *pszStr, int iStrLen)
{
	char *pch1, *pch2;

	pch1 = strstr(pszCustomAttr, pszKey);
	if (NULL == pch1)
	{
		return -1;
	}

	pch1 += strlen(pszKey);
	
	pch2 = strchr(pch1, ',');
	if (NULL == pch2)
	{
		STRNCPY(pszStr, pch1, iStrLen);
	}
	else
	{
		if (pch2 - pch1 >= iStrLen)
		{
			memcpy(pszStr, pch1, iStrLen-1);
			pszStr[iStrLen-1] = 0;
		}
		else
		{
			memcpy(pszStr, pch1, pch2 - pch1);
			pszStr[pch2 - pch1] = 0;
		}
	}

	return 0;
}

int myworld_generate_tlogd_conf(char* dir, int id, unsigned char ucEntity)
{
	TLOGDCONFLOGD  stLogdConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	char szOpLogPath[512];
	int iPort;
	int iWorldID;
	const char *pszCustomAttr;
	char szCustomStr[64];
	APIPROCINST stProc;
	APIHOST stConfHost;
	
	LPTDRMETA pstMeta;
	TLOGCONF stOpLogConf;
	TDRDATA stTdrHostOpLog;

	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stLogdConf);
	stTdrHost.pszBuff = (char *)&stLogdConf;
	strcpy(szFilePath, "../cfg/tlogd.xml");

	iRet = tdr_input_file(mydata.pstTlogdSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if(pszCustomAttr == NULL)
	{
		return -1;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	GET_WORLD_ID(iWorldID, iNetID);
	memset(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern, 0,
			sizeof(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern));
	snprintf(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern,
			sizeof(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern),
			"../oplog_%d/%s.dat", iWorldID,"%F/%H");
	
	
	iPort = TLOGD_PORT_START + iWorldID;
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlogd_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}
	
	pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)(tlog_get_meta_data()), "TLOGConf");
	if(pstMeta == NULL)
	{
		return -1;
	}
	stTdrHostOpLog.iBuff = sizeof(stOpLogConf);
	stTdrHostOpLog.pszBuff = (char *)&stOpLogConf;
	strcpy(szOpLogPath, "../cfg/op_log.xml");

	iRet = tdr_input_file(pstMeta, &stTdrHostOpLog, &szOpLogPath[0], 
						0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szOpLogPath,tdr_error_string(iRet));
		return -2;
	}

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -3;
	}
	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}
	
	snprintf(stLogdConf.stListenerList.stListeners[0].szUrl, sizeof(stLogdConf.stListenerList.stListeners[0].szUrl),
			"udp://%s:%d", stConfHost.szInnerIP, iPort);

	snprintf(stOpLogConf.astCategoryList[0].stDevice.stDevice.stNet.szUrl, sizeof(stOpLogConf.astCategoryList[0].stDevice.stDevice.stNet.szUrl),
			"udp://%s:%d", stConfHost.szInnerIP, iPort);

	//tlogd->tlog_db udp port
	iPort = TLOG_DB_PORT_START + iWorldID;
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlog_db_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}

	if (0 != myworld_get_custom_string(pszCustomAttr, "tlog_db_ip:", szCustomStr, sizeof(szCustomStr)))
	{
		iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
		if (0 > iRet)
		{
			return -1;
		}

		STRNCPY(szCustomStr, stConfHost.szInnerIP, sizeof(szCustomStr));
	}

	snprintf(stLogdConf.stSerializerList.astSerializers[1].stLogger.stDevice.stNet.szUrl, 
			sizeof(stLogdConf.stSerializerList.astSerializers[1].stLogger.stDevice.stNet.szUrl),
			"udp://%s:%d", szCustomStr, iPort);

	
	//输出配置文件
	stTdrHost.iBuff = sizeof(stLogdConf);
	stTdrHost.pszBuff = (char *)&stLogdConf;
	snprintf(szFilePath, sizeof(szFilePath), "%s/tlogd_%s_66.xml", dir, szID);
	
	iRet = tdr_output_file(mydata.pstTlogdSvrMeta, &szFilePath[0], &stTdrHost, 
		0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	stTdrHostOpLog.iBuff = sizeof(stOpLogConf);
	stTdrHostOpLog.pszBuff = (char *)&stOpLogConf;
	snprintf(szOpLogPath, sizeof(szOpLogPath), "%s/op_log_%s_66.xml", dir, szID);
	
	iRet = tdr_output_file(pstMeta, &szOpLogPath[0], &stTdrHostOpLog, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szOpLogPath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

int create_zone_svr_rmshm_sh(char* dir, int id, ZONE_SVRCONF  *stConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/zone_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d\nipcrm -M %d\nipcrm -M %d\nipcrm -M %d",
		stConf->PlayerShmKey,stConf->ResShmKey,stConf->MapShmKey,stConf->UinCacheShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int create_battle_svr_rmshm_sh(char* dir, int id, ZONE_SVRCONF  *stConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/battle_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d\nipcrm -M %d\nipcrm -M %d\nipcrm -M %d",
		stConf->PlayerShmKey,stConf->ResShmKey,stConf->MapShmKey,stConf->UinCacheShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int create_auth_rmshm_sh(char* dir, int id, AUTH_SVRCONF *pstConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/auth_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d",pstConf->ResShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int create_aas_rmshm_sh(char* dir, int id, AAS_SVRCONF  *stConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/aas_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d", stConf->ResShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int create_region_rmshm_sh(char* dir, int id, REGION_GATECONF  *stConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/region_gate_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d", 
			stConf->ObjShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int create_mud_rmshm_sh(char* dir, int id, MUD_SVRCONF  *stConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/mud_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d\nipcrm -M %d\nipcrm -M %d", 
			stConf->ObjShmKey,stConf->PlayerShmKey,stConf->UinCacheShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int create_cs_svr_rmshm_sh(char* dir, int id, CS_SVRCONF  *pstConf)
{
	FILE *fp;
	char szStr[1024];
	char szFilePath[256];
	int iNetID;
	char szID[64];	

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/cs_%s_66_rmshm.sh", dir, szID);
	fp = fopen(szFilePath, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	sprintf(szStr,"#!/bin/sh\nipcrm -M %d", pstConf->iResShmKey);

	fwrite(szStr, strlen(szStr), 1, fp);
	fclose(fp);
	
	return 0;
}

int myworld_generate_zonesvr_conf(char* dir, int id, unsigned char ucEntity)
{
	ZONE_SVRCONF  stZoneSvrConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	APIPROCINST stProc; 			// 进程信息结构
	int iProcNum;
	unsigned char ucEntityTmp;
	APIHOST stConfHost;			// 主机信息结构
	const char *pszCustomAttr;
	char szCustomStr[64];
	int i;
	int iPort;
	int aiOtherPids[8];
	int iProcCount;
	int iMinCltProtoVer;
	int iMaxCltProtoVer;
	int iBlackWhite;
	int iNetID;
	char szID[64];

	if (NULL == dir)
	{
		return -1;
	}

	memset(&stZoneSvrConf, 0, sizeof(stZoneSvrConf));
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stZoneSvrConf);
	stTdrHost.pszBuff = (char *)&stZoneSvrConf;
	strcpy(szFilePath, "../cfg/zone_svr.xml");
		
	iRet = tdr_input_file(mydata.pstZoneSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	stZoneSvrConf.PlayerShmKey = myworld_get_shm_key(id, ZONE_PLAYER_SHM);
	stZoneSvrConf.UinCacheShmKey = myworld_get_shm_key(id, ZONE_UIN_SHM);
	stZoneSvrConf.ResShmKey = myworld_get_shm_key(id, ZONE_RES_SHM);
	stZoneSvrConf.MapShmKey = myworld_get_shm_key(id, ZONE_MAP_SHM);

	
	//host
	iProcNum = tcmapi_get_proc_num(mydata.handle);
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_DIR_SVR == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stZoneSvrConf.DirMasterUrl, sizeof(stZoneSvrConf.DirMasterUrl),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);

			if (pszCustomAttr)
			{
				if (0 == myworld_get_custom_string(pszCustomAttr, "udpbak:", szCustomStr, sizeof(szCustomStr)))
				{
					snprintf(stZoneSvrConf.DirBakUrl, sizeof(stZoneSvrConf.DirBakUrl),
							"udp://%s", szCustomStr);

					break;
				}
			}
		}
		else if (FUNC_DIR_BAK == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stZoneSvrConf.DirBakUrl, sizeof(stZoneSvrConf.DirBakUrl),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
		}
	}

	// 获取与指定进程  通过tbus进行通信的其他进程列表
	iProcCount = sizeof(aiOtherPids)/sizeof(aiOtherPids[0]);
	iRet = tcmapi_get_procs_with_buscommunication(mydata.handle, id, &aiOtherPids[0], &iProcCount);
	if (0 > iRet || iProcCount<=0)
	{
		tlog_error(g_pstConfCat,0,0, "failed to tcmapi_get_procs_with_buscommunication %d", id);
		return -3;
	}
	for (i=0; i<iProcCount; i++)
	{
		iNetID = HTONL(aiOtherPids[i]);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_ZONE_CONN == ucEntityTmp)
		{
			iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, aiOtherPids[i]);
			if (0 > iRet)
			{
				return -4;
			}
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet || stConfHost.iOuterIpNum <= 0)
			{
				return -5;
			}
			STRNCPY(stZoneSvrConf.ConnIP, stConfHost.szOuterIP[0], sizeof(stZoneSvrConf.ConnIP));
			if (stConfHost.iOuterIpNum > 1)
			{
				STRNCPY(stZoneSvrConf.ConnIP_bak1, stConfHost.szOuterIP[1], sizeof(stZoneSvrConf.ConnIP_bak1));
			}
			if (stConfHost.iOuterIpNum > 2)
			{
				STRNCPY(stZoneSvrConf.ConnIP_bak2, stConfHost.szOuterIP[2], sizeof(stZoneSvrConf.ConnIP_bak2));
			}
		}
	}

	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if (NULL == pszCustomAttr)
	{
		iPort = ZONE_CONN_PORT_DEF;
		iBlackWhite = 0;
		iMinCltProtoVer = 0;
		iMaxCltProtoVer = 0;
	}
	else
	{
		if (0 > myworld_get_custom_string(pszCustomAttr, "conn_port:", szCustomStr, sizeof(szCustomStr)))
		{
			iPort = ZONE_CONN_PORT_DEF;
		}
		else
		{
			iPort = atoi(szCustomStr);
		}

		if (0 > myworld_get_custom_string(pszCustomAttr, "minver:", szCustomStr, sizeof(szCustomStr)))
		{
			iMinCltProtoVer = 0;
		}
		else
		{
			iMinCltProtoVer = atoi(szCustomStr);
		}

		if (0 > myworld_get_custom_string(pszCustomAttr, "maxver:", szCustomStr, sizeof(szCustomStr)))
		{
			iMaxCltProtoVer = 0;
		}
		else
		{
			iMaxCltProtoVer = atoi(szCustomStr);
		}
	}
	stZoneSvrConf.ConnPort = iPort;
	stZoneSvrConf.MinCltProtoVer = iMinCltProtoVer;
	stZoneSvrConf.MaxCltProtoVer = iMaxCltProtoVer;
	if (iBlackWhite)
	{
		strcpy(stZoneSvrConf.BlackWhiteCfgPath, "../cfg/black_white.cfg");
	}
	else
	{
		stZoneSvrConf.BlackWhiteCfgPath[0] = 0;
	}
	

	//输出配置文件
	stTdrHost.iBuff = sizeof(stZoneSvrConf);
	stTdrHost.pszBuff = (char *)&stZoneSvrConf;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/zone_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstZoneSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出op_log配置
	generate_oplog_conf(dir, id, ucEntity);

	//输出起停脚本
	
	create_zone_svr_rmshm_sh(dir, id, &stZoneSvrConf);
	iRet = myworld_generate_sh(dir, id, ucEntity);

	return iRet;
}

static int modify_dir_xml_name(DIR_SVRCONF  *pstDirSvrConf, const char* pszInnerIP)
{
	snprintf(pstDirSvrConf->XmlFileName, sizeof(pstDirSvrConf->XmlFileName),
			"./dirxml/dir%s.xml",pszInnerIP);
	return 0;
}

int myworld_generate_dirsvr_conf(char* dir, int id, unsigned char ucEntity)
{
	DIR_SVRCONF  stDirSvrConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	APIHOST stConfHost;
	APIPROCINST stProc;
	int i;
	int iProcNum;
	const char *pszCustomAttr;
	int iPort;
	int iBlackWhite;
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	char szReport[64];

	unsigned char ucEntityTmp;
	
	//读取配置文件模版中的数据
	memset(&stDirSvrConf, 0, sizeof(stDirSvrConf));
	
	stTdrHost.iBuff = sizeof(stDirSvrConf);
	stTdrHost.pszBuff = (char *)&stDirSvrConf;
	strcpy(szFilePath, "../cfg/dir_svr.xml");
		
	iRet = tdr_input_file(mydata.pstDirSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	stDirSvrConf.DirSlaveAddrNum = 0;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	printf("dirsvr ucEntity=%d",ucEntity);

	if (FUNC_DIR_SVR == ucEntity)
	{
		stDirSvrConf.MasterFlag = 0;
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_svr_%s_66.xml", dir, szID);
	}
	else if (FUNC_DIR_BAK == ucEntity)
	{
		stDirSvrConf.MasterFlag = 1;
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_bak_%s_66.xml", dir, szID);
	}
	else if (FUNC_DIR_SLAVE1 == ucEntity)
	{
		stDirSvrConf.MasterFlag = 3;
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_slave1_%s_66.xml", dir, szID);
	}
	else
	{
		stDirSvrConf.MasterFlag = 2;
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_slave_%s_66.xml", dir, szID);
	}

	iProcNum = tcmapi_get_proc_num(mydata.handle);
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		if ((int)stProc.iID == id)
		{
			// 获取进程部署树自定义信息
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
			
			// 获取端口号和黑白名单信息
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
				iBlackWhite = 0;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}

				if (0 > myworld_get_custom_string(pszCustomAttr, "blackwhite:", szCustomStr, sizeof(szCustomStr)))
				{
					iBlackWhite = 0;
				}
				else
				{
					iBlackWhite = atoi(szCustomStr);
				}

				if (0 == myworld_get_custom_string(pszCustomAttr, "report:", szReport, sizeof(szReport)))
				{
					snprintf(stDirSvrConf.ReportAddr, sizeof(stDirSvrConf.ReportAddr),
							"udp://%s?reuse=1", szReport);
				}
				
			}

			if (iBlackWhite)
			{
				strcpy(stDirSvrConf.BlackWhiteCfgPath, "../cfg/black_white.cfg");
			}
			else
			{
				stDirSvrConf.BlackWhiteCfgPath[0] = 0;
			}

			// 根据主机名获取主机信息
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			snprintf(stDirSvrConf.BindInnerAddr, sizeof(stDirSvrConf.BindInnerAddr),
						"udp://%s:%d?reuse=1", stConfHost.szInnerIP, iPort);

			if (FUNC_DIR_SVR == ucEntity)
			{
				modify_dir_xml_name(&stDirSvrConf,  stConfHost.szInnerIP);
			}

			
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_DIR_BAK == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stDirSvrConf.DirBakAddr, sizeof(stDirSvrConf.DirBakAddr),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
		}
		else if (FUNC_DIR_SLAVE == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}

			if (stDirSvrConf.DirSlaveAddrNum >= MAX_DIR_SLAVE)
			{
				return -2;
			}
			
			snprintf(stDirSvrConf.DirSlaveAddrList[stDirSvrConf.DirSlaveAddrNum], 
					sizeof(stDirSvrConf.DirSlaveAddrList[0]),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
			stDirSvrConf.DirSlaveAddrNum++;
		}
		else if (FUNC_DIR_SLAVE1 == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}

			if (stDirSvrConf.DirSlaveAddrNum >= MAX_DIR_SLAVE)
			{
				return -2;
			}
			
			snprintf(stDirSvrConf.DirSlaveAddrList[stDirSvrConf.DirSlaveAddrNum], 
					sizeof(stDirSvrConf.DirSlaveAddrList[0]),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
			stDirSvrConf.DirSlaveAddrNum++;
		}
		
	}

	memset (&stProc, 0, sizeof(stProc));
	stDirSvrConf.AuthConnNum = 0;
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);

		if (FUNC_AUTH_SVR == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}

			STRNCPY(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnIP,stConfHost.szOuterIP[0], 
					sizeof(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnIP));
			
			if (stConfHost.iOuterIpNum > 1)
			{
				STRNCPY(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnIP_Bak1,
							stConfHost.szOuterIP[1], sizeof(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnIP_Bak1));
			}
			if (stConfHost.iOuterIpNum > 2)
			{
				STRNCPY(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnIP_Bak2,
							stConfHost.szOuterIP[2], sizeof(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnIP_Bak2));
			}	
			
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = AUTH_CONN_PORT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "conn_port:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = AUTH_CONN_PORT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].AuthConnPort = iPort;
			GET_REGION_ID(stDirSvrConf.AuthConn[stDirSvrConf.AuthConnNum].RegionID, iNetID);
			stDirSvrConf.AuthConnNum++;

		}
		
	}
	
	
	
	//输出配置文件
	stTdrHost.iBuff = sizeof(stDirSvrConf);
	stTdrHost.pszBuff = (char *)&stDirSvrConf;
	iRet = tdr_output_file(mydata.pstDirSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

int myworld_generate_conn_conf(char* dir, int id, unsigned char ucEntity)
{
	ZONE_CONNCONF  stZoneConnConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iPort;
	const char *pszCustomAttr;
	char szCustomStr[64];
	APIPROCINST stProc;
	APIHOST stConfHost;
	int aiOtherPids[8];
	int iProcCount;
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stZoneConnConf);
	stTdrHost.pszBuff = (char *)&stZoneConnConf;

	if (FUNC_ZONE_CONN == ucEntity)
	{
		strcpy(szFilePath, "../cfg/zone_conn.xml");
		iPort = ZONE_CONN_PORT_DEF;
		pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	}
	else if(FUNC_CS_CONN == ucEntity)
	{
		strcpy(szFilePath, "../cfg/cs_conn.xml");
		iPort = CS_CONN_PROT_DEF;
		pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	}
	else if(FUNC_ACCOUNT_CS_CONN == ucEntity)
	{
		strcpy(szFilePath, "../cfg/cs_conn.xml");
		iPort = ACCOUNT_CS_CONN_DEF;
		pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	}
	else if (FUNC_MUD_CONN == ucEntity)
	{
		strcpy(szFilePath, "../cfg/mud_conn.xml");
		iPort = MUD_CONN_DEF;
		pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	}
	else if (FUNC_AUTH_CONN == ucEntity)
	{
		strcpy(szFilePath, "../cfg/auth_conn.xml");
		iPort = AUTH_CONN_PORT_DEF;
		pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	}
	else
	{
		strcpy(szFilePath, "../cfg/dir_svr_conn.xml");
		iPort = DIR_CONN_PROT_DEF;
		pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	}

	iRet = tdr_input_file(mydata.pstConndMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	
	if (pszCustomAttr)
	{
		if(FUNC_CS_CONN == ucEntity)
		{
			if(0 == myworld_get_custom_string(pszCustomAttr, "cs_conn_port:", szCustomStr, sizeof(szCustomStr)))
			{
				iPort = atoi(szCustomStr);
			}
		}
		else if (FUNC_MUD_CONN == ucEntity)
		{
			if(0 == myworld_get_custom_string(pszCustomAttr, "mud_conn_port:", szCustomStr, sizeof(szCustomStr)))
			{
				iPort = atoi(szCustomStr);
			}
		}
		else
		{
			if(0 == myworld_get_custom_string(pszCustomAttr, "conn_port:", szCustomStr, sizeof(szCustomStr)))
			{
				iPort = atoi(szCustomStr);
			}
		}
	}

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -3;
	}
	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}

	switch (ucEntity)
	{
		case FUNC_CS_CONN:
		case FUNC_ACCOUNT_CS_CONN:
			snprintf(stZoneConnConf.ListenerList.Listeners[0].Url, sizeof(stZoneConnConf.ListenerList.Listeners[0].Url),
					"tcp://%s:%d?reuse=1", stConfHost.szInnerIP, iPort);
			break;
		default:
			/*
			snprintf(stZoneConnConf.ListenerList.Listeners[0].Url, sizeof(stZoneConnConf.ListenerList.Listeners[0].Url),
					"tcp://%s:%d?reuse=1", stConfHost.szOuterIP[0], iPort);*/
			snprintf(stZoneConnConf.ListenerList.Listeners[0].Url, sizeof(stZoneConnConf.ListenerList.Listeners[0].Url),
					"tcp://%s:%d?reuse=1", "0.0.0.0", iPort);		
			break;
	}

	iProcCount = sizeof(aiOtherPids)/sizeof(aiOtherPids[0]);
	iRet = tcmapi_get_procs_with_buscommunication(mydata.handle, id, &aiOtherPids[0], &iProcCount);
	if (0 > iRet || iProcCount<=0)
	{
		tlog_error(g_pstConfCat,0,0, "failed to tcmapi_get_procs_with_buscommunication %d", id);
		return -5;
	}

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, aiOtherPids[0]);
	if (0 > iRet)
	{
		return -6;
	}

	STRNCPY(stZoneConnConf.SerializerList.Serializers[0].Url, stProc.szAddr, 
				sizeof(stZoneConnConf.SerializerList.Serializers[0].Url));
	
	//输出配置文件
	stTdrHost.iBuff = sizeof(stZoneConnConf);
	stTdrHost.pszBuff = (char *)&stZoneConnConf;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	if (FUNC_ZONE_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/zone_conn_%s_66.xml", dir, szID);
	}
	else if (FUNC_DIR_SVR_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_svr_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_DIR_BAK_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_bak_conn_%s_66.xml", dir,szID);
	}
	else if(FUNC_CS_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/cs_conn_%s_66.xml", dir, szID);
	}
	else if (FUNC_DIR_SLAVE_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_slave_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_VERSION_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_VERSION_BAK_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_bak_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_VERSION_SLAVE_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_slave_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_VERSION_SLAVE1_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_slave1_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_AUTH_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/auth_conn_%s_66.xml", dir,szID);
	}
	/*else if (FUNC_AUTH_BAK_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/auth_bak_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_AUTH_SLAVE_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/auth_slave_conn_%s_66.xml", dir,szID);
	}*/
	else if (FUNC_ACCOUNT_CS_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/account_cs_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_DIR_SLAVE1_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/dir_slave1_conn_%s_66.xml", dir,szID);
	}
	else if (FUNC_MUD_CONN == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/mud_conn_%s_66.xml", dir,szID);
	}
	else
	{
		return -1;
	}

	szFilePath[sizeof(szFilePath) - 1] = 0;
	
	iRet = tdr_output_file(mydata.pstConndMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;

}

int myworld_generate_roledb_conf(char* dir, int id, unsigned char ucEntity)
{
	TORMSVRCONF  stRoleConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iWorld;
	int iRegionID;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;
	strcpy(szFilePath, "../cfg/role_db.xml");
		
	iRet = tdr_input_file(mydata.pstDBSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	strcpy(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, "localhost");
	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "dbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));
		}
	}

	sprintf(stRoleConf.ProtoVerHashShm.Key, "%d", myworld_get_shm_key(id, DB_PROTO_VER_SHM));

	iNetID = HTONL(id);
	GET_WORLD_ID(iWorld, iNetID);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(szCustomStr, sizeof(szCustomStr), "world_%d_%d",iRegionID,iWorld);
	STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName, szCustomStr, 
				sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));

	//输出配置文件
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/role_db_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstDBSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

int myworld_generate_miscdb_conf(char* dir, int id, unsigned char ucEntity)
{
	TORMSVRCONF  stRoleConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iWorld;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	int iRegionID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;
	strcpy(szFilePath, "../cfg/misc_db.xml");
		
	iRet = tdr_input_file(mydata.pstDBSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	strcpy(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, "localhost");
	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "dbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));
		}
	}

	sprintf(stRoleConf.ProtoVerHashShm.Key, "%d", myworld_get_shm_key(id, DB_PROTO_VER_SHM));
	//sprintf(stRoleConf.CacheConf.CacheShm.Key, "%d", myworld_get_shm_key(id, DB_CACHE_SHM));

	iNetID = HTONL(id);
	GET_WORLD_ID(iWorld, iNetID);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(szCustomStr, sizeof(szCustomStr), "world_%d_%d", iRegionID, iWorld);
	STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName, szCustomStr, 
				sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));

	//输出配置文件
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/misc_db_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstDBSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

int myworld_generate_mudctrl_conf(char* dir, int id, unsigned char ucEntity)
{
	char szCmd[1024];
	int iRet;
	char szID[64];
	int iNetID;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	snprintf(szCmd, sizeof(szCmd), "cp ../cfg/zone_ctrl.xml %s/mud_ctrl_%s_66.xml", dir, szID);
	system(szCmd);

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}


int myworld_generate_zonectrl_conf(char* dir, int id, unsigned char ucEntity)
{
	char szCmd[1024];
	int iRet;
	char szID[64];
	int iNetID;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	snprintf(szCmd, sizeof(szCmd), "cp ../cfg/zone_ctrl.xml %s/zone_ctrl_%s_66.xml", dir, szID);
	system(szCmd);

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

int myworld_generate_cssvr_conf(char* dir, int id, unsigned char ucEntity)
{
	CS_SVRCONF  stCsSvrConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	int iZoneSvrID;
	char szID[64];
	const char *pszCustomAttr;
	char szWorldID[32] = {0};
	char szCustomStr[64];
	int iRegionID,iWorldID;

	TLOG_DBCONF stLogdbConf; 
	TDRDATA stTdrHost1;
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stCsSvrConf);
	stTdrHost.pszBuff = (char *)&stCsSvrConf;
	strcpy(szFilePath, "../cfg/cs_svr.xml");
		
	iRet = tdr_input_file(mydata.pstCsSvrMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	stTdrHost1.iBuff = sizeof(stLogdbConf);
	stTdrHost1.pszBuff = (char *)&stLogdbConf;
	strcpy(szFilePath, "../cfg/tlog_db.xml");

	iRet = tdr_input_file(mydata.pstTlogdbMeta, &stTdrHost1, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if(pszCustomAttr == NULL)
	{
		return -1;
	}
	
	iNetID = HTONL(id);
	GET_WORLD_ENTITY_ID(iZoneSvrID, iNetID, FUNC_ZONE_SVR);
	iZoneSvrID = NTOHL(iZoneSvrID);
	stCsSvrConf.iResShmKey= myworld_get_shm_key(iZoneSvrID, ZONE_RES_SHM);

	STRNCPY(stCsSvrConf.szSzLogDBConnect, "localhost", sizeof(stCsSvrConf.szSzLogDBConnect));
	if (0 == myworld_get_custom_string(pszCustomAttr, "dbip:", szCustomStr, sizeof(szCustomStr)))
	{
		snprintf(stCsSvrConf.szSzLogDBConnect, sizeof(stCsSvrConf.szSzLogDBConnect),"%s", szCustomStr);
	}

	GET_WORLD_ID(iWorldID, iNetID);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(szWorldID, sizeof(szWorldID), "world_log_%d_%d", iRegionID,iWorldID);
	STRNCPY(stCsSvrConf.szSzLogDBName, szWorldID, sizeof(stCsSvrConf.szSzLogDBName));
	strncpy(stCsSvrConf.szSzLogDBPasswd,stLogdbConf.szLogDBPasswd,sizeof(stCsSvrConf.szSzLogDBPasswd));
	strncpy(stCsSvrConf.szSzLogDBUser,stLogdbConf.szLogDBUser,sizeof(stCsSvrConf.szSzLogDBUser));


	//输出配置文件
	stTdrHost.iBuff = sizeof(stCsSvrConf);
	stTdrHost.pszBuff = (char *)&stCsSvrConf;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/cs_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstCsSvrMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本

	create_cs_svr_rmshm_sh(dir, id, &stCsSvrConf);
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}

int myworld_generate_logdb_conf(char* dir, int id, unsigned char ucEntity)
{
	TLOG_DBCONF stLogdbConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iPort;
	const char *pszCustomAttr;
	char szCustomStr[64];

	int iNetID;
	char szID[64];
	int iWorldID = 0;
	int iRegionID = 0;
	char szWorldID[32] = {0};
	APIPROCINST stProc;
	APIHOST stConfHost;
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stLogdbConf);
	stTdrHost.pszBuff = (char *)&stLogdbConf;
	strcpy(szFilePath, "../cfg/tlog_db.xml");

	iRet = tdr_input_file(mydata.pstTlogdbMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if(pszCustomAttr == NULL)
	{
		return -1;
	}
	
	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	GET_WORLD_ID(iWorldID, iNetID);
	GET_REGION_ID(iRegionID, iNetID);

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -3;
	}
	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}
	
	iPort = TLOG_DB_PORT_START + iWorldID;
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlog_db_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}
	snprintf(stLogdbConf.szUri, sizeof(stLogdbConf.szUri),"udp://%s:%d", stConfHost.szInnerIP, iPort);


	STRNCPY(stLogdbConf.szLogDBConnect, "localhost", sizeof(stLogdbConf.szLogDBConnect));
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlogdbip:", szCustomStr, sizeof(szCustomStr)))
	{
		//snprintf(stLogdbConf.szUri, sizeof(stLogdbConf.szUri),"udp://%s:%d", szCustomStr, iPort);
		snprintf(stLogdbConf.szLogDBConnect, sizeof(stLogdbConf.szLogDBConnect),"%s", szCustomStr);
	}

	snprintf(szWorldID, sizeof(szWorldID), "world_log_%d_%d", iRegionID,iWorldID);
	STRNCPY(stLogdbConf.szLogDBName, szWorldID, sizeof(stLogdbConf.szLogDBName));


	//输出配置文件
	stTdrHost.iBuff = sizeof(stLogdbConf);
	stTdrHost.pszBuff = (char *)&stLogdbConf;
	snprintf(szFilePath, sizeof(szFilePath), "%s/tlog_db_%s_66.xml", dir, szID);
	
	iRet = tdr_output_file(mydata.pstTlogdbMeta, &szFilePath[0], &stTdrHost, 
		0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);

	return 0;
}


// 防沉迷配置
int myworld_generate_aas_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	AAS_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	//int iZoneSvrID;
	int iRegionID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/aas_svr.xml");
		
	iRet = tdr_input_file(mydata.pstAasMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	iNetID = HTONL(id);
	//GET_WORLD_ENTITY_ID(iZoneSvrID, iNetID, FUNC_ZONE_SVR);
	//iZoneSvrID = NTOHL(iZoneSvrID);
	//stConf.ResShmKey= myworld_get_shm_key(iZoneSvrID, ZONE_RES_SHM);
	GET_REGION_ID(iRegionID, iNetID);
	stConf.ResShmKey = iRegionID+1000;


	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/aas_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstAasMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本

	create_aas_rmshm_sh(dir, id, &stConf);
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}

int generate_oplog_conf(char* dir, int id, unsigned char ucEntity)
{

	int iRet;
	char szOpLogPath[512];
	int iPort;
	int iWorldID, iRegionID;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iLogdID;
	
	LPTDRMETA pstMeta;
	TLOGCONF stOpLogConf;
	TDRDATA stTdrHostOpLog;
	APIPROCINST stProc;
	APIHOST stConfHost;

	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if(pszCustomAttr == NULL)
	{
		return -1;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	GET_WORLD_ID(iWorldID, iNetID);
	GET_REGION_ID(iRegionID, iNetID);

	if (FUNC_BATTLE_SVR == ucEntity)
	{
		iPort = REGION_TLOGD_PORT_START + iRegionID;
	}
	else
	{
		iPort = TLOGD_PORT_START + iWorldID;
	}
	
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlogd_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}
	
	pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)(tlog_get_meta_data()), "TLOGConf");
	if(pstMeta == NULL)
	{
		return -1;
	}
	stTdrHostOpLog.iBuff = sizeof(stOpLogConf);
	stTdrHostOpLog.pszBuff = (char *)&stOpLogConf;
	strcpy(szOpLogPath, "../cfg/op_log.xml");

	iRet = tdr_input_file(pstMeta, &stTdrHostOpLog, &szOpLogPath[0], 
						0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szOpLogPath,tdr_error_string(iRet));
		return -2;
	}

	if (FUNC_BATTLE_SVR == ucEntity)
	{
		iLogdID = 0;
		GET_REGION_ENTITY_ID(iLogdID, iNetID, FUNC_REGION_LOG_SVR);
		iLogdID = NTOHL(iLogdID);
	}
	else
	{
		iLogdID = 0;
		GET_WORLD_ENTITY_ID(iLogdID, iNetID, FUNC_LOG_SVR);
		iLogdID = NTOHL(iLogdID);
	}
	
	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, iLogdID);
	if (0 > iRet)
	{
		return -3;
	}
	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}
	
	snprintf(stOpLogConf.astCategoryList[0].stDevice.stDevice.stNet.szUrl, sizeof(stOpLogConf.astCategoryList[0].stDevice.stDevice.stNet.szUrl),
			"udp://%s:%d", stConfHost.szInnerIP, iPort);
	
	snprintf(stOpLogConf.astCategoryList[1].stDevice.stDevice.stFile.szPattern, sizeof(stOpLogConf.astCategoryList[1].stDevice.stDevice.stFile.szPattern),
			"../log/oplog/local_%s.dat", szID);

	stTdrHostOpLog.iBuff = sizeof(stOpLogConf);
	stTdrHostOpLog.pszBuff = (char *)&stOpLogConf;
	snprintf(szOpLogPath, sizeof(szOpLogPath), "%s/op_log_%s_66.xml", dir, szID);
	
	iRet = tdr_output_file(pstMeta, &szOpLogPath[0], &stTdrHostOpLog, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to output file %s， %s",
					szOpLogPath,tdr_error_string(iRet));
	}
	
	return iRet;
	UNUSED(ucEntity);	//add by paraunused.pl
}


// mud 配置
int myworld_generate_mud_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	MUD_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	int i,iProcNum;
	APIPROCINST stProc;
	unsigned char ucEntityTmp;
	APIHOST stConfHost;			// 主机信息结构
	const char *pszCustomAttr;
	char szCustomStr[64];
	char szID[64];
	int iPort;
	
	//读取配置文件模版中的数据
	memset(&stConf, 0, sizeof(stConf));
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/mud_svr.xml");
		
	iRet = tdr_input_file(mydata.pstMudMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}
	
	stConf.PlayerShmKey= myworld_get_shm_key(id, MUD_PLAYER_SHM);
	stConf.UinCacheShmKey = myworld_get_shm_key(id, MUD_UIN_SHM);
	stConf.ObjShmKey = myworld_get_shm_key(id, MUD_OBJ_SHM);

	iProcNum = tcmapi_get_proc_num(mydata.handle);
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_DIR_SVR == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stConf.DirMasterUrl, sizeof(stConf.DirMasterUrl),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
		}
		else if (FUNC_DIR_BAK == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stConf.DirBakUrl, sizeof(stConf.DirBakUrl),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
		}
	}

	iPort = MUD_CONN_DEF;
	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "mud_conn_port:", szCustomStr, sizeof(szCustomStr)))
		{
			stConf.ConnPort = atoi(szCustomStr);
			//iPort = atoi(szCustomStr);
		}
	}

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -1;
	}

	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}

	STRNCPY(stConf.ConnIP, stConfHost.szOuterIP[0], sizeof(stConf.ConnIP));
	if (stConfHost.iOuterIpNum > 1)
	{
		STRNCPY(stConf.ConnIP_bak1, stConfHost.szOuterIP[1], sizeof(stConf.ConnIP_bak1));
	}
	if (stConfHost.iOuterIpNum > 2)
	{
		STRNCPY(stConf.ConnIP_bak2, stConfHost.szOuterIP[2], sizeof(stConf.ConnIP_bak2));
	}
	
	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/mud_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstMudMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出mud的oplog xml文件
	iRet = generate_oplog_conf(dir,id,ucEntity);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to generate_mud_oplog_conf");
	}
	
	//输出起停脚本
	create_mud_rmshm_sh(dir, id, &stConf);
	iRet = myworld_generate_sh(dir, id, ucEntity);

	return iRet;
}

// 大区配置
int myworld_region_gate_conf(char* dir, int id, unsigned char ucEntity)
{
	REGION_GATECONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/region_gate.xml");
		
	iRet = tdr_input_file(mydata.pstRegionMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	stConf.ObjShmKey = myworld_get_shm_key(id, REGION_OBJ_SHM);

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	snprintf(szFilePath, sizeof(szFilePath), "%s/region_gate_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstRegionMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本

	create_region_rmshm_sh(dir, id, &stConf);
	
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}

// 计费进程配置
int myworld_bill_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	BILL_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/bill_svr.xml");
		
	iRet = tdr_input_file(mydata.pstBillMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	snprintf(szFilePath, sizeof(szFilePath), "%s/bill_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstBillMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}

// service_svr 配置
int myworld_service_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	SERVICE_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	const char* pszCustomAttr = NULL;
	char szCustomStr[256];
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/service_svr.xml");
		
	iRet = tdr_input_file(mydata.pstServiceMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "webservice_url:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stConf.WebServiceUrl, szCustomStr, sizeof(stConf.WebServiceUrl));
		}
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	snprintf(szFilePath, sizeof(szFilePath), "%s/service_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstServiceMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}


// informer_svr 配置
int myworld_informer_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	INFORMER_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/informer_svr.xml");
		
	iRet = tdr_input_file(mydata.pstInformerMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	snprintf(szFilePath, sizeof(szFilePath), "%s/informer_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstInformerMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}

// version_svr配置
int myworld_version_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	VERSION_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/version_svr.xml");
		
	iRet = tdr_input_file(mydata.pstVersionMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	if (FUNC_VERSION_SVR == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_svr_%s_66.xml", dir, szID);
	}
	else if (FUNC_VERSION_BAK== ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_bak_%s_66.xml", dir, szID);
	}
	else if (FUNC_VERSION_SLAVE1 == ucEntity)
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_slave1_%s_66.xml", dir, szID);
	}
	else
	{
		snprintf(szFilePath, sizeof(szFilePath), "%s/version_slave_%s_66.xml", dir, szID);
	}

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	iRet = tdr_output_file(mydata.pstVersionMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}

// 认证服务器配置
int myworld_auth_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	
	AUTH_SVRCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[256];
	int iNetID;
	int iRegionID;
	const char *pszCustomAttr;
	char szCustomStr[64];
	char szID[64];
	int iProcNum;
	int i;
	APIPROCINST stProc;
	unsigned char ucEntityTmp;
	APIHOST stConfHost;			// 主机信息结构
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/auth_svr.xml");
		
	iRet = tdr_input_file(mydata.pstAuthMeta, &stTdrHost, &szFilePath[0], 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	GET_REGION_ID(iRegionID, iNetID);
	snprintf(stConf.IPLimitDBName, sizeof(stConf.IPLimitDBName),"account_%d", iRegionID);

	strcpy(stConf.IPLimitDBConnect, "localhost");

	iProcNum = tcmapi_get_proc_num(mydata.handle);
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_ACCOUNT_DB == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (pszCustomAttr)
			{
				if (0 ==  myworld_get_custom_string(pszCustomAttr, "accountdbip:", szCustomStr, sizeof(szCustomStr)))
				{
					STRNCPY(stConf.IPLimitDBConnect, szCustomStr, sizeof(stConf.IPLimitDBConnect));
				}	
			}
			break;
		}
	}
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/auth_svr_%s_66.xml", dir, szID);

	// 生成共享内存ID
	stConf.ResShmKey = myworld_get_shm_key(id, 0);

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	iRet = tdr_output_file(mydata.pstAuthMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	create_auth_rmshm_sh(dir, id, &stConf);
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}


int myworld_account_db_conf(char* dir, int id, unsigned char ucEntity)
{
	TORMSVRCONF  stRoleConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
//	int iWorld;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	int iRegionID;
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;
	strcpy(szFilePath, "../cfg/account_db.xml");
		
	iRet = tdr_input_file(mydata.pstDBSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	strcpy(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, "localhost");
	pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "accountdbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));
		}
	}
	iNetID = HTONL(id);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName, 
			sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName), "account_%d", iRegionID);

	strcpy(stRoleConf.ConnectDBInfo[1].DBMSConnectionInfo, "localhost");
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "accountdbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[1].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[1].DBMSCurDatabaseName));
		}
	}
	iNetID = HTONL(id);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(stRoleConf.ConnectDBInfo[1].DBMSCurDatabaseName, 
			sizeof(stRoleConf.ConnectDBInfo[1].DBMSCurDatabaseName), "bill_%d", iRegionID);
	
	sprintf(stRoleConf.ProtoVerHashShm.Key, "%d", myworld_get_shm_key(id, DB_PROTO_VER_SHM));


	//输出配置文件
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;

	//iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/account_db_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstDBSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}


int myworld_bill_db_conf(char* dir, int id, unsigned char ucEntity)
{
	TORMSVRCONF  stRoleConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iRegionID;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;
	strcpy(szFilePath, "../cfg/bill_db.xml");
		
	iRet = tdr_input_file(mydata.pstDBSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	strcpy(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, "localhost");
	pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "billdbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));
		}
	}

	sprintf(stRoleConf.ProtoVerHashShm.Key, "%d", myworld_get_shm_key(id, DB_PROTO_VER_SHM));


	iNetID = HTONL(id);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(szCustomStr, sizeof(szCustomStr), "bill_%d", iRegionID);
	STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName, szCustomStr, 
				sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));

	//输出配置文件
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/bill_db_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstDBSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}


int myworld_account_cs_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	int iRet;
	ACCOUNT_CS_SVRCONF stConf;
	TDRDATA stTdrHost;
	char szFilePath[512];
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/account_cs_svr.xml");
		
	iRet = tdr_input_file(mydata.pstAccountCsSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}


	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/account_cs_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstAccountCsSvrMeta, &szFilePath[0], &stTdrHost, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	return iRet;
}


int myworld_online_info_conf(char* dir, int id, unsigned char ucEntity)
{
	RECV_REPORTCONF stConf;
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[128];
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	char szID[64];

	int iProcNum;
	int i;
	APIPROCINST stProc;
	unsigned char ucEntityTmp;
//	APIHOST stConfHost;			// 主机信息结构
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;
	strcpy(szFilePath, "../cfg/recv_report.xml");
		
	iRet = tdr_input_file(mydata.pstRecvMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	// dbip
	strcpy(stConf.szOnlineDBConnect, "localhost");
	pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "onlinedbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stConf.szOnlineDBConnect, szCustomStr, sizeof(stConf.szOnlineDBConnect));
		}
	}

	// 监听
	iProcNum = tcmapi_get_proc_num(mydata.handle);
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_DIR_SLAVE == ucEntityTmp)
		{
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (pszCustomAttr)
			{
				if (0 ==  myworld_get_custom_string(pszCustomAttr, "report:", szCustomStr, sizeof(szCustomStr)))
				{
					snprintf(stConf.szUri, sizeof(stConf.szUri), "udp://%s", szCustomStr);
					//STRNCPY(stConf.szUri, szCustomStr, sizeof(stConf.szUri));
				}	
			}
			break;
		}
	}
	

	//输出配置文件
	stTdrHost.iBuff = sizeof(stConf);
	stTdrHost.pszBuff = (char *)&stConf;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/recv_report_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstRecvMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

//battle 配置
int myworld_battle_svr_conf(char* dir, int id, unsigned char ucEntity)
{
	ZONE_SVRCONF  stZoneSvrConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	APIHOST stConfHost;			// 主机信息结构
	const char *pszCustomAttr;
	int iProcNum;
	unsigned char ucEntityTmp;
	APIPROCINST stProc; 		
	int i;
	int iPort;

	if (NULL == dir)
	{
		return -1;
	}
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stZoneSvrConf);
	stTdrHost.pszBuff = (char *)&stZoneSvrConf;
	strcpy(szFilePath, "../cfg/battle_svr.xml");
		
	iRet = tdr_input_file(mydata.pstZoneSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	//注意此处导致 battle  instId  范围仅仅1~15
	stZoneSvrConf.PlayerShmKey = myworld_get_shm_key(id, ZONE_PLAYER_SHM);
	stZoneSvrConf.UinCacheShmKey = myworld_get_shm_key(id, ZONE_UIN_SHM);
	stZoneSvrConf.ResShmKey = myworld_get_shm_key(id, ZONE_RES_SHM);
	stZoneSvrConf.MapShmKey = myworld_get_shm_key(id, ZONE_MAP_SHM);

	//host
	iProcNum = tcmapi_get_proc_num(mydata.handle);
	for (i=0; i<iProcNum; i++)
	{
		iRet = tcmapi_get_proc_inst_bypos(mydata.handle, &stProc, i);
		if (0 > iRet)
		{
			continue;
		}

		iNetID = HTONL(stProc.iID);
		GET_FUNC_ENTITY(ucEntityTmp, iNetID);
		if (FUNC_DIR_SVR == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stZoneSvrConf.DirMasterUrl, sizeof(stZoneSvrConf.DirMasterUrl),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
		}
		else if (FUNC_DIR_BAK == ucEntityTmp)
		{
			iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
			if (0 > iRet)
			{
				continue;
			}
			pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, stProc.iID);
			if (NULL == pszCustomAttr)
			{
				iPort = DIR_UDP_PROT_DEF;
			}
			else
			{
				if (0 > myworld_get_custom_string(pszCustomAttr, "udpport:", szCustomStr, sizeof(szCustomStr)))
				{
					iPort = DIR_UDP_PROT_DEF;
				}
				else
				{
					iPort = atoi(szCustomStr);
				}
			}
			
			snprintf(stZoneSvrConf.DirBakUrl, sizeof(stZoneSvrConf.DirBakUrl),
					"udp://%s:%d", stConfHost.szInnerIP, iPort);
		}
	}
	
	//输出配置文件
	stTdrHost.iBuff = sizeof(stZoneSvrConf);
	stTdrHost.pszBuff = (char *)&stZoneSvrConf;

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	snprintf(szFilePath, sizeof(szFilePath), "%s/battle_svr_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstZoneSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出oplog配置
	generate_oplog_conf(dir, id, ucEntity);

	//输出起停脚本
	create_battle_svr_rmshm_sh(dir, id, &stZoneSvrConf);
	iRet = myworld_generate_sh(dir, id, ucEntity);

	return iRet;
}

int myworld_region_logdb_conf(char* dir, int id, unsigned char ucEntity)
{
	TLOG_DBCONF stLogdbConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iPort;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	int iRegionID = 0;
	char szWorldID[32] = {0};
	APIPROCINST stProc;
	APIHOST stConfHost;
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stLogdbConf);
	stTdrHost.pszBuff = (char *)&stLogdbConf;
	strcpy(szFilePath, "../cfg/region_tlog_db.xml");

	iRet = tdr_input_file(mydata.pstTlogdbMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if(pszCustomAttr == NULL)
	{
		return -1;
	}
	
	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	GET_REGION_ID(iRegionID, iNetID);

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -3;
	}
	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}
	
	iPort = REGION_TLOG_DB_PORT_START + iRegionID;
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlog_db_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}
	snprintf(stLogdbConf.szUri, sizeof(stLogdbConf.szUri),"udp://%s:%d", stConfHost.szInnerIP, iPort);


	STRNCPY(stLogdbConf.szLogDBConnect, "localhost", sizeof(stLogdbConf.szLogDBConnect));
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlogdbip:", szCustomStr, sizeof(szCustomStr)))
	{
		//snprintf(stLogdbConf.szUri, sizeof(stLogdbConf.szUri),"udp://%s:%d", szCustomStr, iPort);
		snprintf(stLogdbConf.szLogDBConnect, sizeof(stLogdbConf.szLogDBConnect),"%s", szCustomStr);
	}

	snprintf(szWorldID, sizeof(szWorldID), "region_log_%d", iRegionID);
	STRNCPY(stLogdbConf.szLogDBName, szWorldID, sizeof(stLogdbConf.szLogDBName));


	//输出配置文件
	stTdrHost.iBuff = sizeof(stLogdbConf);
	stTdrHost.pszBuff = (char *)&stLogdbConf;
	snprintf(szFilePath, sizeof(szFilePath), "%s/region_tlog_db_%s_66.xml", dir, szID);
	
	iRet = tdr_output_file(mydata.pstTlogdbMeta, &szFilePath[0], &stTdrHost, 
		0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);

	return 0;
}


int myworld_region_logsvr_conf(char* dir, int id, unsigned char ucEntity)
{
	TLOGDCONFLOGD  stLogdConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	char szOpLogPath[512];
	int iPort;
	int iRegionID;
	const char *pszCustomAttr;
	char szCustomStr[64];
 	APIPROCINST stProc;
 	APIHOST stConfHost;
	
	LPTDRMETA pstMeta;
	TLOGCONF stOpLogConf;
	TDRDATA stTdrHostOpLog;

	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stLogdConf);
	stTdrHost.pszBuff = (char *)&stLogdConf;
	strcpy(szFilePath, "../cfg/region_tlogd.xml");

	iRet = tdr_input_file(mydata.pstTlogdSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	pszCustomAttr = tcmapi_get_layer_customattr(mydata.handle, id);
	if(pszCustomAttr == NULL)
	{
		return -1;
	}

	iNetID = HTONL(id);
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));

	GET_REGION_ID(iRegionID, iNetID);
	memset(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern, 0,
			sizeof(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern));
	snprintf(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern,
			sizeof(stLogdConf.stSerializerList.astSerializers[0].stLogger.stDevice.stFile.szPattern),
			"../oplog_%d/%s.dat", iRegionID,"%F/%H");
	
	
	iPort = REGION_TLOGD_PORT_START + iRegionID;
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlogd_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}
	
	pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)(tlog_get_meta_data()), "TLOGConf");
	if(pstMeta == NULL)
	{
		return -1;
	}
	stTdrHostOpLog.iBuff = sizeof(stOpLogConf);
	stTdrHostOpLog.pszBuff = (char *)&stOpLogConf;
	strcpy(szOpLogPath, "../cfg/op_log.xml");

	iRet = tdr_input_file(pstMeta, &stTdrHostOpLog, &szOpLogPath[0], 
						0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szOpLogPath,tdr_error_string(iRet));
		return -2;
	}

	iRet = tcmapi_get_proc_inst(mydata.handle, &stProc, id);
	if (0 > iRet)
	{
		return -3;
	}
	iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
	if (0 > iRet)
	{
		return -4;
	}
	
	snprintf(stLogdConf.stListenerList.stListeners[0].szUrl, sizeof(stLogdConf.stListenerList.stListeners[0].szUrl),
			"udp://%s:%d", stConfHost.szInnerIP, iPort);

	snprintf(stOpLogConf.astCategoryList[0].stDevice.stDevice.stNet.szUrl, sizeof(stOpLogConf.astCategoryList[0].stDevice.stDevice.stNet.szUrl),
			"udp://%s:%d", stConfHost.szInnerIP, iPort);


	snprintf(stOpLogConf.astCategoryList[1].stDevice.stDevice.stFile.szPattern, sizeof(stOpLogConf.astCategoryList[1].stDevice.stDevice.stFile.szPattern),
			"../../logsvr/local/local_region_%d.dat", iRegionID);

	

	//tlogd->tlog_db udp port
	iPort = TLOG_DB_PORT_START + iRegionID;
	if (0 == myworld_get_custom_string(pszCustomAttr, "tlog_db_port:", szCustomStr, sizeof(szCustomStr)))
	{
		iPort = atoi(szCustomStr);
	}

	if (0 != myworld_get_custom_string(pszCustomAttr, "tlog_db_ip:", szCustomStr, sizeof(szCustomStr)))
	{
		iRet = tcmapi_get_host_by_name(mydata.handle, &stConfHost, stProc.szHostName);
		if (0 > iRet)
		{
			return -1;
		}

		STRNCPY(szCustomStr, stConfHost.szInnerIP, sizeof(szCustomStr));
	}

	snprintf(stLogdConf.stSerializerList.astSerializers[1].stLogger.stDevice.stNet.szUrl, 
			sizeof(stLogdConf.stSerializerList.astSerializers[1].stLogger.stDevice.stNet.szUrl),
			"udp://%s:%d", szCustomStr, iPort);

	
	//输出配置文件
	stTdrHost.iBuff = sizeof(stLogdConf);
	stTdrHost.pszBuff = (char *)&stLogdConf;
	snprintf(szFilePath, sizeof(szFilePath), "%s/region_tlogd_%s_66.xml", dir, szID);
	
	iRet = tdr_output_file(mydata.pstTlogdSvrMeta, &szFilePath[0], &stTdrHost, 
		0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	stTdrHostOpLog.iBuff = sizeof(stOpLogConf);
	stTdrHostOpLog.pszBuff = (char *)&stOpLogConf;
	snprintf(szOpLogPath, sizeof(szOpLogPath), "%s/op_log_%d_66.xml", dir, iRegionID);
	
	iRet = tdr_output_file(pstMeta, &szOpLogPath[0], &stTdrHostOpLog, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szOpLogPath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}


int myworld_region_db_conf(char* dir, int id, unsigned char ucEntity)
{
	TORMSVRCONF  stRoleConf; 
	TDRDATA stTdrHost;
	int iRet;
	char szFilePath[512];
	int iRegionID;
	const char *pszCustomAttr;
	char szCustomStr[64];
	int iNetID;
	char szID[64];
	
	//读取配置文件模版中的数据
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;
	strcpy(szFilePath, "../cfg/region_db.xml");
		
	iRet = tdr_input_file(mydata.pstDBSvrMeta, &stTdrHost, &szFilePath[0], 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
		return -2;
	}

	strcpy(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, "localhost");
	pszCustomAttr = tcmapi_get_group_customattr(mydata.handle, id);
	if (pszCustomAttr)
	{
		if (0 == myworld_get_custom_string(pszCustomAttr, "regiondbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo));
		}

		if (0 == myworld_get_custom_string(pszCustomAttr, "wanyoudbip:", szCustomStr, sizeof(szCustomStr)))
		{
			STRNCPY(stRoleConf.ConnectDBInfo[1].DBMSConnectionInfo, szCustomStr, sizeof(stRoleConf.ConnectDBInfo[0].DBMSConnectionInfo));
		}
		else
		{
			//wanyou数据库是可选的
			int i;
			int iTableNum = 0;                       
    			TORMTABLECONF astTableConf[TORM_MAX_TABLE];   
			
			stRoleConf.ConnectDBNum = 1;//没有wanyou库
			
			for(i = 0; i < stRoleConf.TableNum;i++)
			{
				if ( 2 == stRoleConf.TableConf[i].TableDBConnIdx)
				{
				}
				else
				{
					astTableConf[iTableNum] = stRoleConf.TableConf[i];
					iTableNum++;
				}
			}

			stRoleConf.TableNum = iTableNum;
			
			memcpy(stRoleConf.TableConf,astTableConf,iTableNum*sizeof(stRoleConf.TableConf[0]));
			
		}
	}

	sprintf(stRoleConf.ProtoVerHashShm.Key, "%d", myworld_get_shm_key(id, DB_PROTO_VER_SHM));


	iNetID = HTONL(id);
	GET_REGION_ID(iRegionID, iNetID);
	snprintf(szCustomStr, sizeof(szCustomStr), "region_%d", iRegionID);
	STRNCPY(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName, szCustomStr, 
				sizeof(stRoleConf.ConnectDBInfo[0].DBMSCurDatabaseName));

	//输出配置文件
	stTdrHost.iBuff = sizeof(stRoleConf);
	stTdrHost.pszBuff = (char *)&stRoleConf;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	
	snprintf(szFilePath, sizeof(szFilePath), "%s/region_db_%s_66.xml", dir, szID);
	iRet = tdr_output_file(mydata.pstDBSvrMeta, &szFilePath[0], &stTdrHost, 
							0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	if (0 != iRet)
	{
		tlog_error(g_pstConfCat,0,0, "failed to input file %s， %s",
					szFilePath,tdr_error_string(iRet));
	}

	//输出起停脚本
	iRet = myworld_generate_sh(dir, id, ucEntity);
	
	return iRet;
}

/*
Fuction            :  myworld_conf_createconfig
Descption          :  生成配置实现函数
Input              :  p         --插件指针
id        --进程id
dir       --生成配置文件存放目录
error     --错误信息预留地址
maxlen    --错误信息可用最大空间，包括结尾空字符
Output             :  error     --错误信息
Return             :  0         -- 成功
其他      -- 失败
*/
int myworld_conf_createconfig(struct config_plunin *p, int id, char* dir, char* error, int maxlen)
{
	UNUSED(p);
	int iRet = 0;
	unsigned char ucEntity;
	int iNetID;

	iNetID = HTONL(id);	
	GET_FUNC_ENTITY(ucEntity, iNetID);

	printf("ucEntity=%d",ucEntity);

	switch(ucEntity)
	{
	case FUNC_DIR_SVR:
	case FUNC_DIR_BAK:
	case FUNC_DIR_SLAVE:
	case FUNC_DIR_SLAVE1:
		iRet = myworld_generate_dirsvr_conf(dir, id, ucEntity);
		break;
	case FUNC_DIR_SVR_CONN:
	case FUNC_DIR_BAK_CONN:	
	case FUNC_DIR_SLAVE_CONN:
	case FUNC_DIR_SLAVE1_CONN:
	case FUNC_ZONE_CONN:
	case FUNC_CS_CONN:
	case FUNC_VERSION_CONN:
	case FUNC_VERSION_BAK_CONN:
	case FUNC_VERSION_SLAVE_CONN:
	case FUNC_VERSION_SLAVE1_CONN:
	case FUNC_AUTH_CONN:
	//case FUNC_AUTH_BAK_CONN:
	//case FUNC_AUTH_SLAVE_CONN:
	case FUNC_ACCOUNT_CS_CONN:
	case FUNC_MUD_CONN:
	
		iRet = myworld_generate_conn_conf(dir, id, ucEntity);
		break;
	case FUNC_ZONE_SVR:
		iRet = myworld_generate_zonesvr_conf(dir, id, ucEntity);
		break;
	case FUNC_ZONE_CTRL:
		iRet = myworld_generate_zonectrl_conf(dir, id, ucEntity);
		break;
	case FUNC_ROLE_DB:
		iRet = myworld_generate_roledb_conf(dir, id, ucEntity);
		break;
	case FUNC_MISC_DB:
		iRet = myworld_generate_miscdb_conf(dir, id, ucEntity);
		break;	
	case FUNC_LOG_SVR:
		iRet = myworld_generate_tlogd_conf(dir, id, ucEntity);
		break;
	case FUNC_CS_SVR:
		iRet = myworld_generate_cssvr_conf(dir, id, ucEntity);
		break;
	case FUNC_CLEAN_MAILS:
		return 0;
		break;
	case FUNC_LOG_DB:
		iRet = myworld_generate_logdb_conf(dir, id, ucEntity);
		break;

	case FUNC_MUD_CTRL:
		iRet = myworld_generate_mudctrl_conf(dir, id, ucEntity);
		break;


		
	case FUNC_REGION_GATE:
		iRet = myworld_region_gate_conf(dir, id, ucEntity);
		break;
	case FUNC_ASS_SVR:
		iRet = myworld_generate_aas_svr_conf(dir, id, ucEntity);
		break;
	case FUNC_BILL_SVR:
		iRet = myworld_bill_svr_conf(dir, id, ucEntity);
		break;
		
	case FUNC_SERVICE_SVR:
		iRet = myworld_service_svr_conf(dir, id, ucEntity);
		break;
		
	case FUNC_INFORMER_SVR:
		iRet = myworld_informer_svr_conf(dir, id, ucEntity);
		break;
		
	case FUNC_VERSION_SVR:
	case FUNC_VERSION_BAK:
	case FUNC_VERSION_SLAVE:
	case FUNC_VERSION_SLAVE1:
		iRet = myworld_version_svr_conf(dir,  id, ucEntity);
		break;
	case FUNC_AUTH_SVR:
	//case FUNC_AUTH_BAK:
	//case FUNC_AUTH_SLAVE:
		iRet = myworld_auth_svr_conf(dir,  id, ucEntity);
		break;
	case FUNC_ACCOUNT_DB:
		iRet = myworld_account_db_conf(dir,  id, ucEntity);
		break;

	case FUNC_BILL_DB:
		iRet = myworld_bill_db_conf(dir,  id, ucEntity);
		break;
	case FUNC_ACCOUNT_CS_SVR:
		iRet = myworld_account_cs_svr_conf(dir, id, ucEntity);
		break;
	case FUNC_MUD_SVR:
		iRet = myworld_generate_mud_svr_conf(dir, id, ucEntity);
		break;
	case FUNC_ONLINEINFO:
		iRet = myworld_online_info_conf(dir, id, ucEntity);
		break;
	case FUNC_BATTLE_SVR:
		iRet = myworld_battle_svr_conf(dir, id, ucEntity);
		break;
	case FUNC_REGION_DB:
		iRet = myworld_region_db_conf(dir, id, ucEntity);
		break;
	case FUNC_REGION_LOG_SVR:
		iRet = myworld_region_logsvr_conf(dir, id, ucEntity);
		break;
	case FUNC_REGION_LOG_DB:
		iRet = myworld_region_logdb_conf(dir, id, ucEntity);
		break;
	
		
	default:
		break;
	
	}

	if (0 > iRet)
	{
		char szErrMsg[256];
		sprintf(szErrMsg, "Entity %d generate conf error %d", ucEntity, iRet);
		STRNCPY(error, szErrMsg, maxlen);
	}

	return iRet;
}


/*
Fuction            :  config_plunin_init
Descption          :  初始化插件,so必须实现该函数
Input              :  p             --插件指针
handle        --api handle
Return             :  0             -- 成功
其他          -- 失败
*/
int config_plunin_init(struct config_plunin *p, HAPIHANDLE handle)
{
	p->getname = myworld_conf_getname;
	p->getversion = myworld_conf_getversion;
	p->init = myworld_conf_init;
	p->cleanup = myworld_conf_cleanup;
	p->createconfig = myworld_conf_createconfig;

	mydata.handle = handle;
	p->data = (void*)&mydata;

	return 0;
}


