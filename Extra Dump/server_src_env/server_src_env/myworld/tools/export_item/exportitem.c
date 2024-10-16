
#include "mysql/mysql.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "proto_comm.h"
#include "resdb_meta.h"

// ./exportitem resdb localhost mmog mmog > 1.txt
 int main(int argc, char *argv[])
 {
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	int err;
	int iRet = -1;
	int iType;
	int iItemID;
	int iImportant;
	int iWearPos;

	if (argc != 5)
	{
		return -1;
	}
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
		return -1;

	if(!(mysql_real_connect(
		conn,
		argv[2],
		argv[3],
		argv[4],
		argv[1],
		0,
		NULL,
		CLIENT_COMPRESS)))
	{
		goto end;
	}

	err = mysql_query(conn, "select ItemID,Important,WearPos1 from ItemDef where Important>=1");
	if(err)
		goto end;
	
	res = mysql_store_result(conn);
	if(!res)
		goto end;

	while((row = mysql_fetch_row(res)) != NULL)
	{
		iItemID = atoi(row[0]);
		iImportant = atoi(row[1]);
		if (row[2] == NULL)
		{
			continue;
		}
		else
		{
			iWearPos = atoi(row[2]);
			if (!(iWearPos >= WEAR_POS_RIGHTHAND && iWearPos <= WEAR_POS_AMULET2 &&
				iWearPos != WEAR_POS_BACK && iWearPos != WEAR_POS_FASHION_BODY))
			{
				continue;
			}
		}
		
		if (iItemID <= 0)
			continue;
		
		iType = 1;
		if (iItemID >= ITEM_NUM_ARM_MIN && iItemID < ITEM_NUM_ARM_MAX)
		{
			iType = 0;

			printf("%d %d %d\n", iItemID, iType, iImportant);
		}
		else
		{
			continue;
		}
	}

	iRet = 0;

 end:

	if (res) mysql_free_result(res);
	if (conn) mysql_close(conn);
	
	return iRet;
 }

