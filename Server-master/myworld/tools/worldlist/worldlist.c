

#include <stdio.h>
#include "worldlist_desc.h"
#include "comm_misc.h"


struct worldlistEnv
{
	WORLDLIST_CONF stConf;

	LPTDRMETALIB pstResLib;
};
typedef struct worldlistEnv WORLDLISTENV;

extern unsigned char g_szMetalib_worldlist[];

static int worldlist_init_lib(WORLDLISTENV* pstEnv)
{
	if (0 > tdr_load_metalib(&pstEnv->pstResLib, pstEnv->stConf.szMetaFile))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}
	return 0;
}

static int worldlist_get_conf(WORLDLISTENV* pstEnv)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(pstEnv->stConf);
	stHost.pszBuff = (char *)&pstEnv->stConf;
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_worldlist[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "worldlist_conf");
	if(NULL == pstMeta)
	{
		printf("get conf:tdr_get_meta_by_name error!\n");
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "worldlist.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		printf("get conf:tdr_input_file error!\n");
		return -1;
	}

	return 0;
}


static int worldlist_has_region(CSDIRWORLDLIST *pstWorldList, char *pszBigworldName)
{
	int i;

	for(i=0; i<pstWorldList->BigWorldNum; i++)
	{
		if (strcmp(pstWorldList->BigWorldList[i].BigWorldName,pszBigworldName) == 0)
		{
			return 1;
		}
	}
	return 0;
}

static int worldlist_has_world(CSDIRWORLDLIST *pstWorldList, int iWorldID, int iRegionID)
{
	int i;

	for(i=0; i<pstWorldList->WorldNum; i++)
	{
		if (pstWorldList->ListInfoList[i].RegionID == iRegionID &&
			pstWorldList->ListInfoList[i].WorldID == iWorldID)
		{
			return 1;
		}
	}
	return 0;
}

static int worldlist_fill(WORLDLISTENV* pstEnv,CSDIRWORLDLIST *pstWorldList, char *pszFileName)
{
	LPTDRMETA pstMeta = NULL;
		
	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "CSDirWorldList");
	if (!pstMeta)
	{
		printf("worldlist_fill:tdr_get_meta_by_name error!\n");
		return -1;
	}

	CSDIRWORLDLIST stWorldList;
	TDRDATA stHost;
	int i;

	memset (&stWorldList, 0, sizeof(stWorldList));

	stHost.pszBuff = (char*)&stWorldList;
	stHost.iBuff = sizeof(stWorldList);
	int iCountRegion; 	//大区
	int iCountWorld;		//服
	int iRegion = 0;

	if( tdr_input_file(pstMeta, &stHost, pszFileName, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		printf("worldlist_fill:tdr_input_file error!\n");
		return -1;
	}

	if (pstWorldList->LastVersion < stWorldList.LastVersion)
		pstWorldList->LastVersion = stWorldList.LastVersion;

	iCountRegion = (int)(sizeof(pstWorldList->BigWorldList)/sizeof(pstWorldList->BigWorldList[0]));
	iCountWorld = (int)(sizeof(pstWorldList->ListInfoList)/sizeof(pstWorldList->ListInfoList[0]));

	iRegion = pstWorldList->BigWorldNum;
	for (i=0; i<stWorldList.BigWorldNum; i++)
	{
		// 有这个大区不管了
		if (worldlist_has_region(pstWorldList, stWorldList.BigWorldList[i].BigWorldName))
		{
			continue;
		}
		
		if (pstWorldList->BigWorldNum >= iCountRegion)
		{
			break;
		}
		pstWorldList->BigWorldList[pstWorldList->BigWorldNum] = stWorldList.BigWorldList[i];
		pstWorldList->BigWorldNum++;
	}

	for (i=0; i<stWorldList.WorldNum; i++)
	{
		/*if (worldlist_has_world(pstWorldList, stWorldList.ListInfoList[i].WorldID, stWorldList.ListInfoList[i].RegionID))
		{
			continue;
		}*/
		
		if (pstWorldList->WorldNum >= iCountWorld)
		{
			break;
		}

		pstWorldList->ListInfoList[pstWorldList->WorldNum] = stWorldList.ListInfoList[i];
		pstWorldList->ListInfoList[pstWorldList->WorldNum].BigWorldIdx += iRegion;
		pstWorldList->WorldNum++;
	}

	return 0;
}


static int worldlist_read_src_path(WORLDLISTENV* pstEnv)
{
	DIR *dp = NULL;
	struct dirent *dirp;
	CSDIRWORLDLIST stWorldList;
	char *pszXml;
	struct stat sbuf;
	LPTDRMETA pstMeta = NULL;	
	char szFileName[256];

	if (pstEnv->stConf.szSrcPath[0] == 0)
	{
		printf("src path error!\n");
		return -1;
	}

	if (pstEnv->stConf.szDstFileName[0] == 0)
	{
		printf("dst filename error!\n");
		return -1;
	}

	if ((dp=opendir(pstEnv->stConf.szSrcPath)) == NULL)
	{
		printf("opendir error!\n");
		goto end;
	}

	memset (&stWorldList, 0, sizeof(stWorldList));
	while((dirp=readdir(dp)) != NULL)
	{
		// 必须是xml文件
		if ((pszXml = strstr(dirp->d_name, "xml")) == NULL)
		{
			continue;
		}

		snprintf(szFileName, sizeof(szFileName), "%s/%s",pstEnv->stConf.szSrcPath,dirp->d_name);
		if (stat(szFileName, &sbuf )< 0)
		{
			printf("can't open %s \n",szFileName);
			continue;
		}

		// 必须是文件
		if (S_ISDIR(sbuf.st_mode))
		{
			continue;
		}
		
		if (worldlist_fill(pstEnv,&stWorldList, szFileName) < 0)
		{
			goto end;
		}
	}

	

	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "CSDirWorldList");
	if (pstMeta == NULL)
		goto end;
	
	if (res_mem_to_xml(pstMeta, pstEnv->stConf.szDstFileName, (char*)&stWorldList, sizeof(stWorldList)) < 0)
	{
		goto end;
	}
	

	if(dp)
		closedir(dp);
	return 0;

	
 end:
 	if(dp)
		closedir(dp);
	return -1;
}

int main()
{
	WORLDLISTENV stEnv;
	memset(&stEnv, 0, sizeof(stEnv));

	if (worldlist_get_conf(&stEnv) < 0)
	{
		return -1;
	}

	if (worldlist_init_lib(&stEnv) < 0)
	{
		return -1;
	}
	
	if (worldlist_read_src_path(&stEnv) < 0)
	{
		return -1;
	}
	
	return 0;
}
