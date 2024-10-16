#ifndef _ENTITY_H_
#define _ENTITY_H_

/*
    1.The EntityId like this: xxxxxx.xxx.xxx
		the first byte stands for its world ID
		the second byte stands for its function
		the third byte stands for instance id
    2.If the first are zero ( 0 ) , it means this entity is a CLUSTER rproc .

    example:  0.30.1 is dir_svr, 1.61.1 is zone_svr in world 1
*/

/*These are for cluster's process */
#define FUNC_DIR_SVR	1
#define FUNC_DIR_BAK  2
#define FUNC_DIR_SLAVE  3
#define FUNC_DIR_SVR_CONN	4
#define FUNC_DIR_BAK_CONN 5
#define FUNC_DIR_SLAVE_CONN 6

#define FUNC_TCM_CONN 20
#define FUNC_TCM_CONSOLE 21
#define FUNC_TCM_CENTER   22


/*These are for world's process */
#define FUNC_ROLE_DB  30 
#define FUNC_LOG_SVR  31
#define FUNC_NAMEID_MAP_SVR 32
#define FUNC_ZONE_CONN 	60
#define FUNC_ZONE_SVR   61 
#define FUNC_ZONE_CTRL  62

//int 是32 bit，而且有字节序的要求
#define GET_WORLD_ENTITY_ID(iRet, iID, ucEntity) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[2] = ucEntity; \
	memcpy(&iRet, szID, sizeof(szID)); \
}

#define GET_CLUSTER_ENTITY_ID(iRet, iID, ucEntity) \
{ \
	unsigned char szID[4]; \
	memcpy(szID, &iID, sizeof(szID)); \
	szID[0]=0; \
	szID[1] = 0; \
	szID[2] = ucEntity; \
	memcpy(&iRet, szID, sizeof(szID)); \
}

#define GET_WORLD_ID(iRet, iID) \
{ \
	iRet = NTOHL(iID)>>16; \
}

#define GET_FUNC_ENTITY(ucEntity, iID) \
{ \
	ucEntity = ((char *)&(iID))[2]; \
}


#endif


