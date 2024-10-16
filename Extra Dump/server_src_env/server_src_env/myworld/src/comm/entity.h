#ifndef _ENTITY_H_
#define _ENTITY_H_

/*
    进程id编号规则由3段x.x.x改为x.x.x.x，编号规则为：
第一段：大区（region）编号，0~63，为0表示没有region，例如dir_svr
第二段：服（world）编号，0~1023，为0表示没有world，例如dir_svr
第三段：功能（Func）编号，0~255，
第四段：实例（Inst）编号，0~255

*/

/*These are for region share process func id*/
#define FUNC_DIR_SVR	1
#define FUNC_DIR_BAK  2
#define FUNC_DIR_SLAVE  3
#define FUNC_DIR_SVR_CONN	4
#define FUNC_DIR_BAK_CONN 5
#define FUNC_DIR_SLAVE_CONN 6
#define FUNC_DIR_SLAVE1  9
#define FUNC_DIR_SLAVE1_CONN 10

#define FUNC_VERSION_SVR 80
#define FUNC_VERSION_BAK 81
#define FUNC_VERSION_SLAVE 82
#define FUNC_VERSION_CONN 83
#define FUNC_VERSION_BAK_CONN 84
#define FUNC_VERSION_SLAVE_CONN 85
#define FUNC_VERSION_SLAVE1 86
#define FUNC_VERSION_SLAVE1_CONN 87

/*These are for region process func id*/
#define FUNC_AUTH_SVR 70
//#define FUNC_AUTH_BAK 71
//#define FUNC_AUTH_SLAVE 72
#define FUNC_AUTH_CONN 73
//#define FUNC_AUTH_BAK_CONN 74
//#define FUNC_AUTH_SLAVE_CONN 75
#define FUNC_ACCOUNT_CS_CONN 90
#define FUNC_ACCOUNT_CS_SVR 91
#define FUNC_INFORMER_SVR 94
#define FUNC_ACCOUNT_DB 95
#define FUNC_REGION_GATE	101
#define FUNC_ASS_SVR	102
#define FUNC_BILL_SVR	103
#define FUNC_BILL_DB	104
#define FUNC_BATTLE_SVR	105
#define FUNC_REGION_DB	106
#define FUNC_REGION_LOG_SVR	107
#define FUNC_REGION_LOG_DB	108
#define FUNC_SERVICE_SVR 109


/*These are for tcm process */
#define FUNC_TCM_CONN 20
#define FUNC_TCM_CONSOLE 21
#define FUNC_TCM_CENTER   22


/*These are for world's process */
#define FUNC_CS_CONN 7
#define FUNC_CS_SVR 8
#define FUNC_ROLE_DB  30 
#define FUNC_LOG_SVR  31
#define FUNC_MISC_DB 32
#define FUNC_LOG_DB 33
#define FUNC_ZONE_CONN 	60
#define FUNC_ZONE_SVR   61 
#define FUNC_ZONE_CTRL  62
#define FUNC_CLEAN_MAILS 39
#define FUNC_MUD_SVR 63
#define FUNC_MUD_CONN 64
#define FUNC_MUD_CTRL 65


#define FUNC_ONLINEINFO 110

//int 是32 bit，而且有字节序的要求
#define GET_WORLD_ENTITY_ID(iRet, iID, ucEntity) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[2] = ucEntity; \
	szID[3] = 1; \
	memcpy(&iRet, szID, sizeof(szID)); \
}

#define GET_CLUSTER_ENTITY_ID(iRet, iID, ucEntity) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[0]=0; \
	szID[1] = 0; \
	szID[2] = ucEntity; \
	szID[3] = 1; \
	memcpy(&iRet, szID, sizeof(szID)); \
}

#define GET_REGION_ENTITY_ID(iRet, iID, ucEntity) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[2] = ucEntity; \
	szID[3] = 1; \
	memcpy(&iRet, szID, sizeof(szID)); \
	iRet = iRet & 0xffff00fc; \
}

#define GET_WORLD_ID(iRet, iID) \
{ \
	iRet = NTOHL(iID) << 6 >> 22; \
}

#define GET_REGION_ID(iRet, iID) \
{ \
	iRet = NTOHL(iID) >> 26; \
}

#define GET_FUNC_ENTITY(ucEntity, iID) \
{ \
	ucEntity = ((char *)&(iID))[2]; \
}

#define GET_INSTANCE_ENTITY(ucEntity, iID) \
{ \
	ucEntity = ((char *)&(iID))[3]; \
}

//战场获取regin的唯一服务器
#define GET_REGION_ENTITY_ID_BY_BATTLE(iRet, iID, ucEntity) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[2] = ucEntity; \
	szID[3] = 1; \
	memcpy(&iRet, szID, sizeof(szID)); \
	iRet = iRet & 0xffff00fc; \
}

#define GET_WORLD_ENTITY_ID_INSTANCE(iRet, iID, ucEntity, ucInst) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[2] = ucEntity; \
	szID[3] = ucInst; \
	memcpy(&iRet, szID, sizeof(szID)); \
}

#endif


