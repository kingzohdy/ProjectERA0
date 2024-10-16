#include "gamestat_desc.h"
#include "pal/pal.h"
#include "tdr/tdr.h"
#include "mysql/mysql.h"
#include "proto_comm.h"
#include "role_db_meta.h"
#include "comm/shtable.h"
#include "resdb_meta.h"

extern unsigned char g_szMetalib_gamestat[];

#define MAX_CACL_DATA 10000
#define MUCH_EXCHG 20
#define MUCH_MAIL 20
//#define MUCH_STALL 10

struct CaclData
{
	char cLogin; 	// 1-µ«¬º 0-ÕÀ≥ˆ
	time_t t;		//  ±º‰
};
typedef struct CaclData CACLDATA;

struct CaclOnline
{
	int iNum;
	CACLDATA stData[MAX_CACL_DATA]; 
};
typedef struct CaclOnline CACLONLINE;

struct LevelSecondSkillStatDetail
{
	int iRoleNum;
	long long llType4Num;
	long long iType4AvgLevel;
	int iType4MaxLevel;
	long long iType4AvgSkillVal;
	int iType4MaxSkillVal;
	long long iType4AvgRecipe;
	int iType4MaxRecipe;
	long long llType5Num;
	long long iType5AvgLevel;
	int iType5MaxLevel;
	long long iType5AvgSkillVal;
	int iType5MaxSkillVal;
	long long iType5AvgRecipe;
	int iType5MaxRecipe;
	long long llType6Num;
	long long iType6AvgLevel;
	int iType6MaxLevel;
	long long iType6AvgSkillVal;
	int iType6MaxSkillVal;
	long long iType6AvgRecipe;
	int iType6MaxRecipe;
};
typedef struct LevelSecondSkillStatDetail LEVELSECONDSKILLSTATDETAIL;

struct LevelStatDetail
{
	int iRoleNum;
	long long iMax;
	long long iMin;
	long long iAvg;
	long long llTotal;
};
typedef struct LevelStatDetail LEVELSTATDETAIL;

struct LevelArmStatDetail
{
	int iRoleNum;
	long long iMax;
	long long iMin;
	long long iAvg;
	long long llTotal;
	long long llGem;
	long long llFuMo;
	long long llBaseAttr;
	long long llFujiaAttr;
	long long llWashAttr;
	long long llStar;
	long long llPerfectStar;
};
typedef struct LevelArmStatDetail LEVELARMSTATDETAIL;

struct PetStatDetail
{
	int iPetID;
	int iTotal;
	int iPinZhi[MAX_PET_GENIUS];
	int iLevelMax;
	long long llLevelTotal;
	long long llStar;
	long long llPerfectStar;
};
typedef struct PetStatDetail PETSTATDETAIL;

struct PetSkillStatDetail
{
	int iSkillID;
	int iSkillLevelMax;
	long long llTotal;
	long long llSkillLevelTotal;
};
typedef struct PetSkillStatDetail PETSKILLSTATDETAIL;

struct SparStatDetail
{
	int iItemID;
	long long llTotal;
	long long llLevelTotal;
};
typedef struct SparStatDetail SPARSTATDETAIL;	

#define MAX_SPAR_ITEM 120
#define MAX_PETSKILL 512
#define MAX_PET 256
#define MAX_PINZHI 8
#define MAX_LEVEL_DUAN 13
#define ARM_POS 3
#define ARM_POS_ON 0
#define ARM_POS_PAK 1
#define ARM_POS_STASH 2

#define MAX_SUB_CAREER 12

struct GenuineStatDetail
{
	int iGenuineType;
	int iGenuineAttrID;
	int iSubCareer;
	int iRoleNum;
	int iMaxGenuineLevel;
	long long llTotal;
};
typedef struct GenuineStatDetail GENUINESTATDETAIL;

#define MAX_GEM_NUM 48
#define MAX_GEM_LEV 7

struct WorldStat
{
	int iRegionID;
	int iWorldID;
	LEVELSTATDETAIL astLevelDayTime[MAX_LEVEL+1];
	LEVELSTATDETAIL astLevel[MAX_LEVEL+1];
	LEVELSTATDETAIL astLevelMoney[MAX_LEVEL+1];
	LEVELSTATDETAIL astLevelBindMoney[MAX_LEVEL+1];
	LEVELSTATDETAIL astLevelGold[MAX_LEVEL+1];
	LEVELSTATDETAIL astLevelBindGold[MAX_LEVEL+1];
	LEVELARMSTATDETAIL astLevelArmPinZhi[MAX_PINZHI][MAX_LEVEL_DUAN][ARM_POS];
	LEVELSECONDSKILLSTATDETAIL astLevelSecondSkill[MAX_LEVEL+1];
	PETSTATDETAIL astPet[MAX_PET];
	PETSKILLSTATDETAIL astPetSkill[MAX_PETSKILL];
	SPARSTATDETAIL astSpar[MAX_LEVEL_DUAN][MAX_SPAR_ITEM];
	GENUINESTATDETAIL astGenuine[MAX_GENIUS_TYPE*MAX_GENIUS_SUBTYPE*MAX_SUB_CAREER][MAX_LEVEL+1];
	int aiGemAll[MAX_GEM]; 
	int aiGemLev[MAX_GEM_LEV][MAX_GEM];
};
typedef struct WorldStat WORLDSTAT;

struct tagImpArm
{
	int iDefID;
	int iPinZhi;
};
typedef struct tagImpArm ImpArm;

#define MAX_IMP_ARM 10000

struct GameStatEnv
{
	GAMESTAT_CONF stConf;
	int iSec;
	struct tm *pstTm;
	char szCreateDB[128];
	TDRDBHANDLE hStatDBHandle;
	LPTDRMETALIB pstMetaLib;
	int iWorldStat;
	WORLDSTAT *pstWorldStat[MAX_WORLDNUM];

	CACLONLINE stCaclOnline;
	LPSHTABLE pstImpArm;
};

typedef struct GameStatEnv GAMESTATENV;
GAMESTATENV stEnv;

int gamestat_get_role_online(	GAMESTATENV *pstEnv, 
								int iRegionID,
								int iWorldID,
								const char* pszStart, 
								const char *pszEnd,
								unsigned long long ullRoleID);

int HashCmp( const void * pv1 , const void *pv2 )
{
	return *(int *)pv1 - *(int *)pv2;
}

unsigned int HashCode(const void* pvData)
{
	return (unsigned int)(*(int *)pvData);
}

int get_imp_item(GAMESTATENV *pstEnv)
{
	FILE *fp;
	char szTxt[128];
	char *p;
	int iItem;
	int iItemType;
	int iPinZhi;
	ImpArm *pstImpArm;
	ImpArm stImpArm;
	
	fp = fopen("./imp_item.txt" , "r");
	if (NULL == fp)
	{
		return -1;
	}

	while(fgets(szTxt, sizeof(szTxt), fp))
	{
		p = strtok((char *)szTxt, " ");
		if (!p) continue;
		iItem = atoi(p);
		p = strtok(NULL, " ");
		if (!p) continue;
		iItemType = atoi(p);
		p = strtok(NULL, " ");
		if (!p) continue;
		iPinZhi = atoi(p);

		if (0 == iItemType)
		{
			stImpArm.iDefID = iItem;
			pstImpArm = sht_insert_unique(pstEnv->pstImpArm, &stImpArm, HashCmp, HashCode);
			if (pstImpArm)
			{
				pstImpArm->iDefID = iItem;
				pstImpArm->iPinZhi = iPinZhi;
			}
		}
	}
	
	return 0;
}

int gamestat_init(GAMESTATENV *pstEnv)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;
	GAMESTAT_CONF *pstConf;

	memset(pstEnv, 0, sizeof(*pstEnv));

	stHost.iBuff = sizeof(pstEnv->stConf);
	stHost.pszBuff = (char *)&pstEnv->stConf;	
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_gamestat[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "gamestat_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "gamestat.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}

	pstConf = &pstEnv->stConf;	
	if (0 > tdr_load_metalib(&pstEnv->pstMetaLib, pstEnv->stConf.szMetaFile))
	{
		printf("load meta lib role_db_meta fail\n");
		return -1;
	}

	pstEnv->pstImpArm = sht_create(MAX_IMP_ARM*3, MAX_IMP_ARM, sizeof(ImpArm), NULL);
	if (NULL == pstEnv->pstImpArm)
	{
		printf("sht_create fail\n");
		return -1;
	}
	
	get_imp_item(pstEnv);

	pstEnv->iSec = time((time_t*)NULL);
	pstEnv->pstTm = localtime((time_t*)&pstEnv->iSec);

	//printf ("stat start,time %d\n", pstEnv->iSec);
	//printf ("----------------\n");
	
	return 0;
}


TDRDBHANDLE gamestat_conn(GAMESTATENV *pstEnv, char *pszDBName)
{
	char szErrorMsg[2048]={0};
	TDRDBMS stDBMS;
	TDRDBHANDLE hDBHandle;

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pszDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstEnv->stConf.szDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstEnv->stConf.szDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstEnv->stConf.szPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&hDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return NULL;
	}
	
	return hDBHandle;
}

int gamestat_create_db(GAMESTATENV *pstEnv)
{
	MYSQL *conn = mysql_init(NULL);
	MYSQL *mysql = NULL;
	char szSql[1024]={0};
	int iRet = -1;
	int err;
	
	
	if(!conn)
		return -1;

	if(!(mysql = mysql_real_connect(
		conn,
		pstEnv->stConf.szDBConnect,
		pstEnv->stConf.szDBUser,
		pstEnv->stConf.szPasswd,
		NULL,
		0,
		NULL,
		0)))
	{
		goto end;
	}

	snprintf(	pstEnv->szCreateDB, sizeof(pstEnv->szCreateDB), "stat_%d_%d_%d",
			pstEnv->pstTm->tm_year+1900, 
			pstEnv->pstTm->tm_mon+1, 
			pstEnv->pstTm->tm_mday);

	snprintf(szSql, sizeof(szSql), "CREATE DATABASE %s;", pstEnv->szCreateDB);

	// ’‚¿Ô≥ˆ¥Ì“≤≤ªπ‹,÷ª“™œ¬√Êƒ‹πª¡¨Ω”æÕ––
	err = mysql_query(mysql, szSql);

	//  ¡¨Ω”¥¥Ω®µƒ ˝æ›ø‚
	pstEnv->hStatDBHandle = gamestat_conn(pstEnv, pstEnv->szCreateDB);
	if (NULL == pstEnv->hStatDBHandle)
	{
		goto end;
	}
	
	iRet = 0;
end:

	if (mysql)
		mysql_close(mysql) ;
	
	return iRet;
}

int get_day_six_time(int iCurr, int iDay)
{
	int iTime;
	struct tm *ptTm;
	struct tm stTm;

	iTime = iCurr - iDay*24*3600;
	ptTm = localtime((time_t*)&iTime);
	memcpy(&stTm, ptTm, sizeof(stTm));
	stTm.tm_hour = 6;
	stTm.tm_min = 0;
	stTm.tm_sec = 0;

	return mktime(&stTm);
}

int role_level_daytime_world_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int iTime;
	LEVELSTATDETAIL *pstLevelDetail;

	iTime = get_day_six_time(pstEnv->iSec, 1);
	if ((int)pstRoleData->LastLogin > iTime || (int)pstRoleData->LastLogout > iTime ||
		pstRoleData->LastLogin > pstRoleData->LastLogout)
	{
		pstLevelDetail = &pstWorldStat->astLevelDayTime[pstRoleData->Level];
		pstLevelDetail->iRoleNum++;
		pstLevelDetail->llTotal += pstRoleData->MiscInfo.ODTime.OndaySec;
		if (pstLevelDetail->iMax < pstRoleData->MiscInfo.ODTime.OndaySec)
		{
			pstLevelDetail->iMax = pstRoleData->MiscInfo.ODTime.OndaySec;
		}
		if (pstLevelDetail->iMin == 0)
		{
			pstLevelDetail->iMin = pstRoleData->MiscInfo.ODTime.OndaySec;
		}
		else if (pstLevelDetail->iMin > pstRoleData->MiscInfo.ODTime.OndaySec)
		{
			pstLevelDetail->iMin = pstRoleData->MiscInfo.ODTime.OndaySec;
		}
	}
	
	return 0;
}

int role_level_world_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	LEVELSTATDETAIL *pstLevelDetail;
	
	pstLevelDetail = &pstWorldStat->astLevel[pstRoleData->Level];
	pstLevelDetail->iRoleNum++;
			
	return 0;
}

int role_level_money_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	LEVELSTATDETAIL *pstLevelDetail;
	long long llMoney;
		
	pstLevelDetail = &pstWorldStat->astLevelMoney[pstRoleData->Level];
	pstLevelDetail->iRoleNum++;
	llMoney = pstRoleData->RoleDetail.Money;
	llMoney += pstRoleData->Package.StashPackage.Money;
	pstLevelDetail->llTotal += llMoney;
	if (pstLevelDetail->iMax < llMoney)
	{
		pstLevelDetail->iMax = llMoney;
	}
	if (pstLevelDetail->iMin == 0)
	{
		pstLevelDetail->iMin = llMoney;
	}
	else if (pstLevelDetail->iMin > llMoney)
	{
		pstLevelDetail->iMin = llMoney;
	}

	pstLevelDetail = &pstWorldStat->astLevelBindMoney[pstRoleData->Level];
	pstLevelDetail->iRoleNum++;
	llMoney = pstRoleData->RoleDetail.Bull;
	llMoney += pstRoleData->Package.StashPackage.Bull;
	pstLevelDetail->llTotal += llMoney;
	if (pstLevelDetail->iMax < llMoney)
	{
		pstLevelDetail->iMax = llMoney;
	}
	if (pstLevelDetail->iMin == 0)
	{
		pstLevelDetail->iMin = llMoney;
	}
	else if (pstLevelDetail->iMin > llMoney)
	{
		pstLevelDetail->iMin = llMoney;
	}

	pstLevelDetail = &pstWorldStat->astLevelGold[pstRoleData->Level];
	pstLevelDetail->iRoleNum++;
	llMoney = pstRoleData->RoleDetail.GodCoin;
	pstLevelDetail->llTotal += llMoney;
	if (pstLevelDetail->iMax < llMoney)
	{
		pstLevelDetail->iMax = llMoney;
	}
	if (pstLevelDetail->iMin == 0)
	{
		pstLevelDetail->iMin = llMoney;
	}
	else if (pstLevelDetail->iMin > llMoney)
	{
		pstLevelDetail->iMin = llMoney;
	}

	pstLevelDetail = &pstWorldStat->astLevelBindGold[pstRoleData->Level];
	pstLevelDetail->iRoleNum++;
	llMoney = pstRoleData->RoleDetail.BindGodCoin;
	pstLevelDetail->llTotal += llMoney;
	if (pstLevelDetail->iMax < llMoney)
	{
		pstLevelDetail->iMax = llMoney;
	}
	if (pstLevelDetail->iMin == 0)
	{
		pstLevelDetail->iMin = llMoney;
	}
	else if (pstLevelDetail->iMin > llMoney)
	{
		pstLevelDetail->iMin = llMoney;
	}
		
	return 0;
}

int get_arm_gem(ROLEARM *pstRoleArm)
{
	int iCount = 0;
	int i;

	for (i=0; i<pstRoleArm->HoleNum; i++)
	{
		if (pstRoleArm->HoleGem[i].HoleGemID)
		{
			iCount++;
		}
	}
	
	return iCount;
}

int get_arm_fumo(ROLEARM *pstRoleArm)
{
	if (pstRoleArm->ExtendedResult.ResultID)
	{
		return 1;
	}
	
	return 0;
}

int get_arm_perfectstar(ROLEARM *pstRoleArm)
{
	int i;
	int iCount = 0;

	for (i=0; i<pstRoleArm->StarLv; i++)
	{
		if (pstRoleArm->StarInfo[i].ValIdx >= MAX_STAR_MUL_LIST -1)
		{
			iCount++;
		}
		
	}
	
	return iCount;
}

int role_arm_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int i;
	ImpArm *pstImpArm;
	ImpArm stImpArm;
	LEVELARMSTATDETAIL *pstLevelArmStatDetail;
	ROLEARM *pstRoleArm;
	int aiMaxArm[MAX_PINZHI];
	
	for (i=0; i<MAX_PINZHI; i++)
	{
		aiMaxArm[i] = -1;
	}
	pstLevelArmStatDetail = NULL;
	for (i=0; i<pstRoleData->Wear.ArmNum; i++)
	{
		pstRoleArm = &pstRoleData->Wear.RoleArms[i];
		stImpArm.iDefID = pstRoleArm->DefID;
		pstImpArm = sht_find(pstEnv->pstImpArm, &stImpArm, HashCmp, HashCode);
		if (NULL == pstImpArm)
		{
			continue;
		}

		if (pstImpArm->iPinZhi > MAX_PINZHI || pstImpArm->iPinZhi < 1)
		{
			continue;
		}

		pstLevelArmStatDetail = &pstWorldStat->astLevelArmPinZhi[pstImpArm->iPinZhi-1][pstRoleData->Level/10][ARM_POS_ON];

		if (-1 == aiMaxArm[pstImpArm->iPinZhi-1])
		{
			pstLevelArmStatDetail->iRoleNum++;
			aiMaxArm[pstImpArm->iPinZhi-1] = pstLevelArmStatDetail->iMax;
			pstLevelArmStatDetail->iMax = 0;
		}
				
		pstLevelArmStatDetail->iMax++;
		pstLevelArmStatDetail->llTotal++;
		pstLevelArmStatDetail->llGem += get_arm_gem(pstRoleArm);
		pstLevelArmStatDetail->llFuMo += get_arm_fumo(pstRoleArm);
		pstLevelArmStatDetail->llBaseAttr += pstRoleArm->BaseAttrNum;
		pstLevelArmStatDetail->llFujiaAttr += pstRoleArm->AddAttrNum;
		pstLevelArmStatDetail->llWashAttr += pstRoleArm->WashAttrNum;
		pstLevelArmStatDetail->llStar += pstRoleArm->StarLv;
		pstLevelArmStatDetail->llPerfectStar += get_arm_perfectstar(pstRoleArm);
	}
	for (i=0; i<MAX_PINZHI; i++)
	{
		pstLevelArmStatDetail = &pstWorldStat->astLevelArmPinZhi[i][pstRoleData->Level/10][ARM_POS_ON];
		if (aiMaxArm[i] > pstLevelArmStatDetail->iMax)
		{
			pstLevelArmStatDetail->iMax = aiMaxArm[i];
		}
	}
	
	for (i=0; i<MAX_PINZHI; i++)
	{
		aiMaxArm[i] = -1;
	}
	pstLevelArmStatDetail = NULL;
	for (i=0; i<pstRoleData->Package.SundriesPackage.Num; i++)
	{
		pstRoleArm = &pstRoleData->Package.SundriesPackage.RoleGrids[i].GridData.RoleArm;
		stImpArm.iDefID = pstRoleArm->DefID;
		pstImpArm = sht_find(pstEnv->pstImpArm, &stImpArm, HashCmp, HashCode);
		if (NULL == pstImpArm)
		{
			continue;
		}

		if (pstImpArm->iPinZhi > MAX_PINZHI || pstImpArm->iPinZhi < 1)
		{
			continue;
		}

		pstLevelArmStatDetail = &pstWorldStat->astLevelArmPinZhi[pstImpArm->iPinZhi-1][pstRoleData->Level/10][ARM_POS_PAK];

		if (-1 == aiMaxArm[pstImpArm->iPinZhi-1])
		{
			pstLevelArmStatDetail->iRoleNum++;
			aiMaxArm[pstImpArm->iPinZhi-1] = pstLevelArmStatDetail->iMax;
			pstLevelArmStatDetail->iMax = 0;
		}
				
		pstLevelArmStatDetail->iMax++;
		pstLevelArmStatDetail->llTotal++;
		pstLevelArmStatDetail->llGem += get_arm_gem(pstRoleArm);
		pstLevelArmStatDetail->llFuMo += get_arm_fumo(pstRoleArm);
		pstLevelArmStatDetail->llBaseAttr += pstRoleArm->BaseAttrNum;
		pstLevelArmStatDetail->llFujiaAttr += pstRoleArm->AddAttrNum;
		pstLevelArmStatDetail->llWashAttr += pstRoleArm->WashAttrNum;
		pstLevelArmStatDetail->llStar += pstRoleArm->StarLv;
		pstLevelArmStatDetail->llPerfectStar += get_arm_perfectstar(pstRoleArm);

	}
	for (i=0; i<MAX_PINZHI; i++)
	{
		pstLevelArmStatDetail = &pstWorldStat->astLevelArmPinZhi[i][pstRoleData->Level/10][ARM_POS_PAK];
		if (aiMaxArm[i] > pstLevelArmStatDetail->iMax)
		{
			pstLevelArmStatDetail->iMax = aiMaxArm[i];
		}
	}

	for (i=0; i<MAX_PINZHI; i++)
	{
		aiMaxArm[i] = -1;
	}
	pstLevelArmStatDetail = NULL;
	for (i=0; i<pstRoleData->Package.StashPackage.Num; i++)
	{
		pstRoleArm = &pstRoleData->Package.StashPackage.RoleGrids[i].GridData.RoleArm;
		stImpArm.iDefID = pstRoleArm->DefID;
		pstImpArm = sht_find(pstEnv->pstImpArm, &stImpArm, HashCmp, HashCode);
		if (NULL == pstImpArm)
		{
			continue;
		}

		if (pstImpArm->iPinZhi > MAX_PINZHI || pstImpArm->iPinZhi < 1)
		{
			continue;
		}

		pstLevelArmStatDetail = &pstWorldStat->astLevelArmPinZhi[pstImpArm->iPinZhi-1][pstRoleData->Level/10][ARM_POS_STASH];

		if (-1 == aiMaxArm[pstImpArm->iPinZhi-1])
		{
			pstLevelArmStatDetail->iRoleNum++;
			aiMaxArm[pstImpArm->iPinZhi-1] = pstLevelArmStatDetail->iMax;
			pstLevelArmStatDetail->iMax = 0;
		}
		
		pstLevelArmStatDetail->iMax++;
		pstLevelArmStatDetail->llTotal++;
		pstLevelArmStatDetail->llGem += get_arm_gem(pstRoleArm);
		pstLevelArmStatDetail->llFuMo += get_arm_fumo(pstRoleArm);
		pstLevelArmStatDetail->llBaseAttr += pstRoleArm->BaseAttrNum;
		pstLevelArmStatDetail->llFujiaAttr += pstRoleArm->AddAttrNum;
		pstLevelArmStatDetail->llWashAttr += pstRoleArm->WashAttrNum;
		pstLevelArmStatDetail->llStar += pstRoleArm->StarLv;
		pstLevelArmStatDetail->llPerfectStar += get_arm_perfectstar(pstRoleArm);

	}
	for (i=0; i<MAX_PINZHI; i++)
	{
		pstLevelArmStatDetail = &pstWorldStat->astLevelArmPinZhi[i][pstRoleData->Level/10][ARM_POS_STASH];
		if (aiMaxArm[i] > pstLevelArmStatDetail->iMax)
		{
			pstLevelArmStatDetail->iMax = aiMaxArm[i];
		}
	}
			
	return 0;
}

int get_recipe_num(unsigned char *p)
{
	int i,j;
	int iCount = 0;
	unsigned char uOne;

	for (i=0; i<MAX_SECONDARY_SKILL_RECIPE; i++)
	{
		uOne = p[i];
		for (j=0; j<8; j++)
		{
			if((1<<j) & uOne)
			{
				iCount++;
			}
		}
	}

	return iCount;
}

int role_secondskill_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int i;
	int iRecipeNum;
	LEVELSECONDSKILLSTATDETAIL *pstLevelDetail;
	SECONDARYSKILLMISCINFO *pstSecondSkillInfo = &pstRoleData->MiscInfo.SecondarySkillMiscInfo;

	if (0 == pstSecondSkillInfo->SkillNum)
	{
		return 0;
	}
		
	pstLevelDetail = &pstWorldStat->astLevelSecondSkill[pstRoleData->Level];
	pstLevelDetail->iRoleNum++;

	for (i=0; i<pstSecondSkillInfo->SkillNum; i++)
	{
		if (pstSecondSkillInfo->SkillInfo[i].SkillType == 4)
		{
			pstLevelDetail->llType4Num++;
			if (pstSecondSkillInfo->SkillInfo[i].SkillLevel > pstLevelDetail->iType4MaxLevel)
			{
				pstLevelDetail->iType4MaxLevel = pstSecondSkillInfo->SkillInfo[i].SkillLevel;
			}
			pstLevelDetail->iType4AvgLevel += pstSecondSkillInfo->SkillInfo[i].SkillLevel;
			
			if ((int)pstSecondSkillInfo->SkillInfo[i].SkillVal > pstLevelDetail->iType4MaxSkillVal)
			{
				pstLevelDetail->iType4MaxSkillVal = pstSecondSkillInfo->SkillInfo[i].SkillVal;
			}
			pstLevelDetail->iType4AvgSkillVal += pstSecondSkillInfo->SkillInfo[i].SkillVal;
			
			iRecipeNum = get_recipe_num(pstSecondSkillInfo->SkillInfo[i].RecipeInfo);
			if (iRecipeNum > pstLevelDetail->iType4MaxRecipe)
			{
				pstLevelDetail->iType4MaxRecipe = iRecipeNum;
			}
			pstLevelDetail->iType4AvgRecipe += iRecipeNum;
		}

		if (pstSecondSkillInfo->SkillInfo[i].SkillType == 5)
		{
			pstLevelDetail->llType5Num++;
			if (pstSecondSkillInfo->SkillInfo[i].SkillLevel > pstLevelDetail->iType5MaxLevel)
			{
				pstLevelDetail->iType5MaxLevel = pstSecondSkillInfo->SkillInfo[i].SkillLevel;
			}
			pstLevelDetail->iType5AvgLevel += pstSecondSkillInfo->SkillInfo[i].SkillLevel;
			
			if ((int)pstSecondSkillInfo->SkillInfo[i].SkillVal > pstLevelDetail->iType5MaxSkillVal)
			{
				pstLevelDetail->iType5MaxSkillVal = pstSecondSkillInfo->SkillInfo[i].SkillVal;
			}
			pstLevelDetail->iType5AvgSkillVal += pstSecondSkillInfo->SkillInfo[i].SkillVal;
			
			iRecipeNum = get_recipe_num(pstSecondSkillInfo->SkillInfo[i].RecipeInfo);
			if (iRecipeNum > pstLevelDetail->iType5MaxRecipe)
			{
				pstLevelDetail->iType5MaxRecipe = iRecipeNum;
			}
			pstLevelDetail->iType5AvgRecipe += iRecipeNum;
		}

		if (pstSecondSkillInfo->SkillInfo[i].SkillType == 6)
		{
			pstLevelDetail->llType6Num++;
			if (pstSecondSkillInfo->SkillInfo[i].SkillLevel > pstLevelDetail->iType6MaxLevel)
			{
				pstLevelDetail->iType6MaxLevel = pstSecondSkillInfo->SkillInfo[i].SkillLevel;
			}
			pstLevelDetail->iType6AvgLevel += pstSecondSkillInfo->SkillInfo[i].SkillLevel;
			
			if ((int)pstSecondSkillInfo->SkillInfo[i].SkillVal > pstLevelDetail->iType6MaxSkillVal)
			{
				pstLevelDetail->iType6MaxSkillVal = pstSecondSkillInfo->SkillInfo[i].SkillVal;
			}
			pstLevelDetail->iType6AvgSkillVal += pstSecondSkillInfo->SkillInfo[i].SkillVal;
			
			iRecipeNum = get_recipe_num(pstSecondSkillInfo->SkillInfo[i].RecipeInfo);
			if (iRecipeNum > pstLevelDetail->iType6MaxRecipe)
			{
				pstLevelDetail->iType6MaxRecipe = iRecipeNum;
			}
			pstLevelDetail->iType6AvgRecipe += iRecipeNum;
		}
	}

	return 0;
}

PETSTATDETAIL *find_pet_stat(WORLDSTAT *pstWorldStat, int iPetID)
{
	int i;

	for (i=0; i<MAX_PET; i++)
	{
		if (pstWorldStat->astPet[i].iPetID == 0)
		{
			break;
		}

		if (pstWorldStat->astPet[i].iPetID == iPetID)
		{
			return &pstWorldStat->astPet[i];
		}
	}

	if (i < MAX_PET)
	{
		pstWorldStat->astPet[i].iPetID = iPetID;
		return &pstWorldStat->astPet[i];
	}

	return NULL;
}

PETSKILLSTATDETAIL *find_petskill_stat(WORLDSTAT *pstWorldStat, int iSkillID)
{
	int i;

	for (i=0; i<MAX_PETSKILL; i++)
	{
		if (pstWorldStat->astPetSkill[i].iSkillID == 0)
		{
			break;
		}

		if (pstWorldStat->astPetSkill[i].iSkillID == iSkillID)
		{
			return &pstWorldStat->astPetSkill[i];
		}
	}

	if (i < MAX_PETSKILL)
	{
		pstWorldStat->astPetSkill[i].iSkillID = iSkillID;
		return &pstWorldStat->astPetSkill[i];
	}

	return NULL;
}

int get_pet_perfectstar(ROLEPET *pstRolePet)
{
	int i;
	int iCount = 0;

	for (i=0; i<pstRolePet->StrengthenLevel; i++)
	{
		if (pstRolePet->StrengthenInfo[i] == 10)
		{
			iCount++;
		}
	}

	return iCount;
}

int role_petskill_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEPET *pstRolePet)
{
	int i;
	PETSKILLSTATDETAIL *pstPetSkillDetail;

	for (i=0; i<pstRolePet->PetSkills.SkillGridNum; i++)
	{
		if (0 == pstRolePet->PetSkills.SkillGrid[i].SkillID)
		{
			continue;
		}
		
		pstPetSkillDetail = find_petskill_stat(pstWorldStat, pstRolePet->PetSkills.SkillGrid[i].SkillID);
		if (NULL == pstPetSkillDetail)
		{
			continue;
		}

		if (pstRolePet->PetSkills.SkillGrid[i].SkillLevel > pstPetSkillDetail->iSkillLevelMax)
		{
			pstPetSkillDetail->iSkillLevelMax = pstRolePet->PetSkills.SkillGrid[i].SkillLevel;
		}

		pstPetSkillDetail->llTotal++;
		pstPetSkillDetail->llSkillLevelTotal += pstRolePet->PetSkills.SkillGrid[i].SkillLevel;
	}
	
	return 0;
}

int role_pet_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int i;
	ROLEPET *pstRolePet;
	PETSTATDETAIL *pstPetStatDetail;

	for (i=0; i<pstRoleData->Package.PetPackage.Num; i++)
	{
		pstRolePet = &pstRoleData->Package.PetPackage.RolePets[i];
		pstPetStatDetail = find_pet_stat(pstWorldStat, pstRolePet->PetID);
		if (NULL == pstPetStatDetail)
		{
			continue;
		}

		pstPetStatDetail->iTotal++;
		if (pstRolePet->Level > pstPetStatDetail->iLevelMax)
		{
			pstPetStatDetail->iLevelMax = pstRolePet->Level;
		}

		pstPetStatDetail->llLevelTotal += pstRolePet->Level;
		pstPetStatDetail->llStar += pstRolePet->StrengthenLevel;
		pstPetStatDetail->llPerfectStar += get_pet_perfectstar(pstRolePet);
		if (pstRolePet->PetGenius < MAX_PET_GENIUS)
		{
			pstPetStatDetail->iPinZhi[pstRolePet->PetGenius]++;
		}

		role_petskill_fetch(pstEnv, pstWorldStat, pstRolePet);
	}
	
	return 0;
}

SPARSTATDETAIL  *find_spar_stat(WORLDSTAT *pstWorldStat, int iLevel, int iDefID)
{
	int i;
	SPARSTATDETAIL *pstSparStatDetail;

	if (iLevel > MAX_LEVEL)
	{
		return NULL;
	}

	for (i=0; i<MAX_SPAR_ITEM; i++)
	{
		pstSparStatDetail = &pstWorldStat->astSpar[iLevel/10][i];

		if (pstSparStatDetail->iItemID == 0)
		{
			break;
		}
		
		if (pstSparStatDetail->iItemID == iDefID)
		{
			return pstSparStatDetail;
		}
	}

	if (i < MAX_SPAR_ITEM)
	{
		pstSparStatDetail = &pstWorldStat->astSpar[iLevel/10][i];

		pstSparStatDetail->iItemID = iDefID;
			
		return pstSparStatDetail;
	}
	
	return NULL;
}

GENUINESTATDETAIL * find_genuine_stat(WORLDSTAT *pstWorldStat, int iCareer,
											int iLevel, int iType, int iAttrID)
{
	int i;
	GENUINESTATDETAIL *pstGenuineDetail;

	if (iLevel > MAX_LEVEL)
	{
		return NULL;
	}

	for (i=0; i<MAX_GENIUS_TYPE*MAX_GENIUS_SUBTYPE*MAX_SUB_CAREER; i++)
	{
		pstGenuineDetail = &pstWorldStat->astGenuine[i][iLevel];
		if (pstGenuineDetail->iGenuineType == iType && pstGenuineDetail->iGenuineAttrID == iAttrID &&
			pstGenuineDetail->iSubCareer == iCareer)
		{
			return pstGenuineDetail;
		}
		else if (pstGenuineDetail->iSubCareer == 0)
		{
			break;
		}
	}

	if (i < MAX_GENIUS_TYPE*MAX_GENIUS_SUBTYPE*MAX_SUB_CAREER)
	{
		pstGenuineDetail->iSubCareer = iCareer;
		pstGenuineDetail->iGenuineType = iType;
		pstGenuineDetail->iGenuineAttrID = iAttrID;
		
		return 	pstGenuineDetail;
	}
	
	return NULL;
}

int role_genuine_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int i, j;
	GENUINESTATDETAIL *pstGenuineStat;
	GENIUSINFO *pstGenuineInfo;
	
	pstGenuineInfo = &pstRoleData->MiscInfo.GeniusInfo;

	for (i=0; i<pstGenuineInfo->Num; i++)
	{
		for (j=0; j<pstGenuineInfo->One[i].Num; j++)
		{
			if (pstGenuineInfo->One[i].SubOne[j].Lvl > 0)
			{
				pstGenuineStat = find_genuine_stat(pstWorldStat, pstRoleData->Career, 
									pstRoleData->Level, pstGenuineInfo->One[i].Type,
									pstGenuineInfo->One[i].SubOne[j].AttrID);

				if (NULL == pstGenuineStat)
				{
					return 0;
				}

				pstGenuineStat->iGenuineType = pstGenuineInfo->One[i].Type;
				pstGenuineStat->iGenuineAttrID = pstGenuineInfo->One[i].SubOne[j].AttrID;
				pstGenuineStat->iSubCareer = pstRoleData->Career;
				pstGenuineStat->iRoleNum++;
				if (pstGenuineInfo->One[i].SubOne[j].Lvl > pstGenuineStat->iMaxGenuineLevel)
				{
					pstGenuineStat->iMaxGenuineLevel = pstGenuineInfo->One[i].SubOne[j].Lvl;
				}
				pstGenuineStat->llTotal += pstGenuineInfo->One[i].SubOne[j].Lvl;
			}
		}
	}

	return 0;
}

//‘› ±”√“ªœ¬id
int gem_level_by_id(int iGemID)
{
	if (5213500 == iGemID)
	{
		return 4;
	}

	return iGemID%10+1;
}

int role_gem_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int i, j;
	int iGemAll = 0;
	int aiGemLev[MAX_GEM_LEV];
	int iGemLevel;
	ROLEWEAR *pstRoleWear = &pstRoleData->Wear;
	ROLEARM *pstArm;

	memset(aiGemLev, 0, sizeof(aiGemLev));	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstArm = &pstRoleWear->RoleArms[i];
		for (j=0; j<pstArm->HoleNum; j++)
		{
			if (pstArm->HoleGem[j].HoleGemID > 0)
			{
				iGemAll++;
				iGemLevel = gem_level_by_id(pstArm->HoleGem[j].HoleGemID);

				if (iGemLevel >= 1 && iGemLevel <= MAX_GEM_LEV)
				{
					aiGemLev[iGemLevel-1]++;
				}
			}
		}
	}

	if (iGemAll >= 1 && iGemAll <= MAX_GEM_NUM)
	{
		pstWorldStat->aiGemAll[iGemAll-1]++;
	}

	for (i=0; i<MAX_GEM_LEV; i++)
	{
		if (aiGemLev[i] >= 1 && aiGemLev[i] <= MAX_GEM_NUM)
		{
			pstWorldStat->aiGemLev[i][aiGemLev[i]-1]++;
		}
	}
	
	return 0;
}

int role_spar_fetch(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat, ROLEDATA *pstRoleData)
{
	int i;
	ROLEITEM *pstRoleItem;
	SPARSTATDETAIL  *pstSparStatDetail;
	SPARPKG *pstSparPkg = &pstRoleData->Package.SparPkg;

	for (i=0; i<pstSparPkg->Num; i++)
	{
		pstRoleItem = &pstSparPkg->RoleItems[i];
		pstSparStatDetail = find_spar_stat(pstWorldStat, pstRoleData->Level, pstRoleItem->DefID);
		if (NULL == pstSparStatDetail)
		{
			continue;
		}

		pstSparStatDetail->llTotal++;
		pstSparStatDetail->llLevelTotal += pstRoleItem->InstVal1;
	}

	return 0;
}

WORLDSTAT *world_stat_find(GAMESTATENV *pstEnv, int iRegionID, int iWorldID)
{
	int i;

	for (i=0; i<pstEnv->iWorldStat; i++)
	{
		if (pstEnv->pstWorldStat[i]->iRegionID == iRegionID &&
			pstEnv->pstWorldStat[i]->iWorldID == iWorldID)
		{
			return pstEnv->pstWorldStat[i];
		}
	}
	
	return NULL;
}

int role_top_7days_online(GAMESTATENV *pstEnv, int iRegionID, int iWorldID, TDRDBHANDLE hDBHandle)
{
	int i;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	TDRDBRESULTHANDLE hDBResult1;
	int iNumRows;
	char szRoleName[32];
	tdr_ulonglong ullRoleID;
	char *pszField;
	int iField;
	int iOnline;
	time_t tStart, tEnd;
	char szStart[32];
	char szEnd[32];
	
	tStart = get_day_six_time(pstEnv->iSec, 7);
	strftime(szStart,sizeof(szStart),"%Y-%m-%d %H:%M:%S",localtime(&tStart));
	tEnd = pstEnv->iSec;
	strftime(szEnd,sizeof(szEnd),"%Y-%m-%d %H:%M:%S",localtime(&tEnd));

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS roletop_7days_online_%d_%d ( "
		"RoleName varchar(32), RoleID bigint(20) unsigned, Online INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID, iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	snprintf(szSql, sizeof(szSql), "select FirstName,GID from TopRange where MainType=0 and SubType=0 order by TopRange Limit 50");
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, hDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	iNumRows = tdr_num_rows(hDBResult);

	for (i=0; i<iNumRows; i++)
	{
		if (0 == tdr_dbms_fetch_row(hDBResult))
		{
			if (0 == tdr_dbms_fetch_field(&pszField, &iField, hDBResult, "FirstName"))
			{
				STRNCPY(szRoleName, pszField, sizeof(szRoleName));
				if (0 == tdr_dbms_fetch_field(&pszField, &iField, hDBResult, "GID"))
				{
					ullRoleID = strtoull(pszField, NULL, 10);

					iOnline = gamestat_get_role_online(pstEnv, iRegionID, iWorldID, szStart, szEnd, ullRoleID);

					if (iOnline >= 0)
					{
						snprintf(szSql, sizeof(szSql), "INSERT INTO roletop_7days_online_%d_%d (RoleName,RoleID,Online) VALUES ('%s',%llu,%d);",
							iRegionID, iWorldID, szRoleName, ullRoleID, iOnline/60);
						stSqlData.pszBuff = szSql;
						stSqlData.iBuff = strlen(szSql);

						if(tdr_query(&hDBResult1, pstEnv->hStatDBHandle, &stSqlData) != 0)
						{
							printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
							return -1;
						}

						tdr_free_dbresult(&hDBResult1);
					}
				}
			}
		}
	}

	tdr_free_dbresult(&hDBResult);
	
	return 0;
}

int role_much_exchg(GAMESTATENV *pstEnv, int iRegionID, int iWorldID, TDRDBHANDLE hDBHandle)
{
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	char szRoleName[32];
	tdr_ulonglong ullRoleID;
	int iCount;
	time_t tStart;
	char szLogSuffix[32];
	MYSQL *conn;
	int iErr;
	MYSQL_RES *res;
	MYSQL_ROW row;
		
	tStart = get_day_six_time(pstEnv->iSec, 1);
	strftime(szLogSuffix,sizeof(szLogSuffix),"%y%m%d",localtime(&tStart));

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS role_much_exchg_%d_%d ( "
		"RoleName varchar(32), RoleID bigint(20) unsigned, Num INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID, iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	conn = mysql_init(NULL);
	if(!conn)
	{	
		printf("mysql_init fail\n");
		return -1;
	}

	snprintf(szSql, sizeof(szSql),"world_log_%d_%d", iRegionID, iWorldID);
	if(!(mysql_real_connect(
		conn,
		pstEnv->stConf.szLogDBConnect,
		pstEnv->stConf.szLogDBUser,
		pstEnv->stConf.szLogPasswd,
		szSql,
		0,
		NULL,
		CLIENT_COMPRESS)))
	{
		printf("mysql_real_connect fail\n");
		goto end;
	}

	snprintf(szSql, sizeof(szSql), "SELECT count(*) as a, RoleID,RoleName FROM LogRoleExchg_%s group by RoleID having a >= %d",
			szLogSuffix, MUCH_EXCHG);

	iErr = mysql_query(conn, szSql);
	if(!iErr)
	{
		res = mysql_store_result(conn);
		if(res)
		{
			while((row = mysql_fetch_row(res)) != NULL)
			{
				iCount = atoi(row[0]);	
				ullRoleID = strtoull(row[1], NULL, 10);
				STRNCPY(szRoleName, row[2], sizeof(szRoleName));
				snprintf(szSql, sizeof(szSql), "INSERT INTO role_much_exchg_%d_%d (RoleName,RoleID,Num) VALUES ('%s',%llu,%d);",
							iRegionID, iWorldID, szRoleName, ullRoleID, iCount);
				stSqlData.pszBuff = szSql;
				stSqlData.iBuff = strlen(szSql);

				if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
				{
					printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
					return -1;
				}

				tdr_free_dbresult(&hDBResult);
			}
		}

		mysql_free_result(res);
	}
	
end:
	if (conn) 
		mysql_close(conn);
	
	return 0;
}

int role_much_mail(GAMESTATENV *pstEnv, int iRegionID, int iWorldID, TDRDBHANDLE hDBHandle)
{
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	char szRoleName[32];
	tdr_ulonglong ullRoleID;
	int iCount;
	time_t tStart;
	char szLogSuffix[32];
	MYSQL *conn;
	int iErr;
	MYSQL_RES *res;
	MYSQL_ROW row;
		
	tStart = get_day_six_time(pstEnv->iSec, 1);
	strftime(szLogSuffix,sizeof(szLogSuffix),"%y%m%d",localtime(&tStart));

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS role_much_mail_%d_%d ( "
		"RoleName varchar(32), Num INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID, iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	conn = mysql_init(NULL);
	if(!conn)
	{	
		printf("mysql_init fail\n");
		return -1;
	}

	snprintf(szSql, sizeof(szSql),"world_log_%d_%d", iRegionID, iWorldID);
	if(!(mysql_real_connect(
		conn,
		pstEnv->stConf.szLogDBConnect,
		pstEnv->stConf.szLogDBUser,
		pstEnv->stConf.szLogPasswd,
		szSql,
		0,
		NULL,
		CLIENT_COMPRESS)))
	{
		printf("mysql_real_connect fail\n");
		goto end;
	}

	snprintf(szSql, sizeof(szSql), "SELECT count(*) as a, RoleMailData_To FROM LogRoleMail_%s "
		"where RoleMailData_Flags = 0 RoleMailData_ItemNum > 0 or RoleMailData_Money > 0 group by RoleMailData_To having a >= %d",
			szLogSuffix, MUCH_MAIL);

	iErr = mysql_query(conn, szSql);
	if(!iErr)
	{
		res = mysql_store_result(conn);
		if(res)
		{
			while((row = mysql_fetch_row(res)) != NULL)
			{
				iCount = atoi(row[0]);	
					STRNCPY(szRoleName, row[1], sizeof(szRoleName));
				snprintf(szSql, sizeof(szSql), "INSERT INTO role_much_mail_%d_%d (RoleName,Num) VALUES ('%s',%llu,%d);",
							iRegionID, iWorldID, szRoleName, ullRoleID, iCount);
				stSqlData.pszBuff = szSql;
				stSqlData.iBuff = strlen(szSql);

				if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
				{
					printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
					return -1;
				}

				tdr_free_dbresult(&hDBResult);
			}
		}

		mysql_free_result(res);
	}
	
end:
	if (conn) 
		mysql_close(conn);
	
	return 0;
}

/*
int role_much_stall(GAMESTATENV *pstEnv, int iRegionID, int iWorldID, TDRDBHANDLE hDBHandle)
{
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	char szRoleName[32];
	tdr_ulonglong ullRoleID;
	int iCount;
	time_t tStart;
	char szLogSuffix[32];
	MYSQL *conn;
	int iErr;
	MYSQL_RES *res;
	MYSQL_ROW row;
		
	tStart = get_day_six_time(pstEnv->iSec, 1);
	strftime(szLogSuffix,sizeof(szLogSuffix),"%y%m%d",localtime(&tStart));

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS role_much_stall_%d_%d ( "
		"RoleName varchar(32), RoleID bigint(20) unsigned, Num INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID, iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	conn = mysql_init(NULL);
	if(!conn)
	{	
		printf("mysql_init fail\n");
		return -1;
	}

	snprintf(szSql, sizeof(szSql),"world_log_%d_%d", iRegionID, iWorldID);
	if(!(mysql_real_connect(
		conn,
		pstEnv->stConf.szDBConnect,
		pstEnv->stConf.szDBUser,
		pstEnv->stConf.szPasswd,
		szSql,
		0,
		NULL,
		CLIENT_COMPRESS)))
	{
		printf("mysql_real_connect fail\n");
		goto end;
	}

	snprintf(szSql, sizeof(szSql), "SELECT count(*) as a, RoleID,RoleName FROM LogRoleMail_%s "
		"where RoleMailData_ItemNum > 0 or RoleMailData_Money > 0 group by RoleID having a >= %d",
			szLogSuffix, MUCH_MAIL);

	iErr = mysql_query(conn, szSql);
	if(!iErr)
	{
		res = mysql_store_result(conn);
		if(res)
		{
			while((row = mysql_fetch_row(res)) != NULL)
			{
				iCount = atoi(row[0]);	
				ullRoleID = strtoull(row[1], NULL, 10);
				STRNCPY(szRoleName, row[2], sizeof(szRoleName));
				snprintf(szSql, sizeof(szSql), "INSERT INTO role_much_stall_%d_%d (RoleName,RoleID,Num) VALUES ('%s',%llu,%d);",
							iRegionID, iWorldID, szRoleName, ullRoleID, iCount);
				stSqlData.pszBuff = szSql;
				stSqlData.iBuff = strlen(szSql);

				if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
				{
					printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
					return -1;
				}

				tdr_free_dbresult(&hDBResult);
			}
		}

		mysql_free_result(res);
	}
	
end:
	if (conn) 
		mysql_close(conn);
	
	return 0;
}
*/


int role_scan_get_item_num(ROLEDATA *pstRoleData, int iItemID)
{
	int iNum = 0;
	int i;
	
	for (i=0; i<pstRoleData->Package.StashPackage.Num; i++)
	{
		if (pstRoleData->Package.StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
		{
			iNum+=pstRoleData->Package.StashPackage.RoleGrids[i].GridData.RoleItem.Num;
		}	
	}

	for (i=0; i<pstRoleData->Package.SundriesPackage.Num; i++)
	{
		if (pstRoleData->Package.SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
		{
			iNum+=pstRoleData->Package.SundriesPackage.RoleGrids[i].GridData.RoleItem.Num;
		}
	}

	if (iNum > 0)
	{
		printf("Uin=%d  Name=%s Num=%d\n", pstRoleData->Uin, pstRoleData->RoleName, iNum);
	}
	return iNum;
}

int role_scan_world(GAMESTATENV *pstEnv, int iRegionID, int iWorldID)
{
	TDRDATA stSql;
	char szSql[102400];
	char szWhere[1024];
	TDRDBHANDLE hDBHandle;
	char szDBName[128];
	TDRDBOBJECT stDBObj;
	ROLEDATA stRoleData;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows;
	int i;
	WORLDSTAT *pstWorldStat;
	//int iCount = 0;

	pstWorldStat = world_stat_find(pstEnv, iRegionID, iWorldID);
	if (NULL == pstWorldStat)
	{
		printf("level_stat_find err:region %d world %d\n", iRegionID, iWorldID);
		return -1;
	}
		
	printf("----world role scan stat region %d world %d start:%ld-----\n", 
		iRegionID, iWorldID, time(NULL));

	snprintf(szDBName, sizeof(szDBName), "world_%d_%d", iRegionID, iWorldID);

	hDBHandle = gamestat_conn(pstEnv, szDBName);
	if (NULL == hDBHandle)
	{
		return -1;
	}

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	sprintf(szWhere,"WHERE RoleID !=0");

	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stRoleData);
	stDBObj.pszObj = (char*)&stRoleData;

	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_SELECT, &stDBObj, szWhere);
	if (0 != iRet)
	{
		printf("…˙≥…≤È—Ø◊ ‘¥º«¬ºµƒSQL”Ôæ‰ ß∞‹: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		goto _end;
	}

	if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		goto _end;
	}

	iNumRows = tdr_num_rows(hDBResult);
	printf("----world role scan stat rolenum:%d-----\n", iNumRows);

	if (iNumRows < 1)
	{
		goto _end;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &stDBObj);
		if (0 != iRet)
		{			
			if ((TDR_ERR_GET_ERROR_CODE(iRet) != TDR_ERROR_DB_NO_RECORD_IN_RESULTSET) ||	
				(0 != tdr_dbms_errno(hDBResult)))			
			{				
				printf("¥” ˝æ›ø‚±Ì÷–∂¡»° ˝æ›º«¬º ß∞‹:%s, dbms  error:%s\n",					
				tdr_error_string(iRet), tdr_dbms_error(hDBResult));
			}
			else	
			{		
				iRet = 0; 
			}	
			break;		
		}

		//iCount += role_scan_get_item_num(&stRoleData, 5051037);
		//role_level_daytime_world_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_level_world_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_level_money_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_arm_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_secondskill_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_pet_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_spar_fetch(pstEnv, pstWorldStat, &stRoleData);
		//role_genuine_fetch(pstEnv, pstWorldStat, &stRoleData);
		role_gem_fetch(pstEnv, pstWorldStat, &stRoleData);
	}

	//printf("iCount = %d  Scan end!!!\n", iCount);

	tdr_free_dbresult(&hDBResult);

	//role_top_7days_online(pstEnv, iRegionID, iWorldID, hDBHandle);
	//role_much_exchg(pstEnv, iRegionID, iWorldID, hDBHandle);
	//role_much_mail(pstEnv, iRegionID, iWorldID, hDBHandle);
	//role_much_stall(pstEnv, iRegionID, iWorldID, hDBHandle);
	
	printf("----world role scan stat region %d world %d end:%ld-----\n", 
		iRegionID, iWorldID, time(NULL));

_end:
	tdr_close_dbhanlde(hDBHandle);

	return 0;
}

int global_fetch(int iRegionID, int iWorldID, GLOBAL *pstGlobal)
{
	char szLevEnd[32];

	strftime(szLevEnd,sizeof(szLevEnd),"%Y-%m-%d %H:%M:%S",localtime(&pstGlobal->LevFeng.LevelFengEnd));
	
	printf("regionid: %d, worldid: %d, FengLev: %d, FengEnd: %s\n",
		iRegionID, iWorldID, pstGlobal->LevFeng.LevelFeng, szLevEnd);
	return 0;
}

int global_scan_world(GAMESTATENV *pstEnv, int iRegionID, int iWorldID)
{
	TDRDBHANDLE hDBHandle;
	char szDBName[128];
	TDRDATA stSql;
	char szSql[102400];
	TDRDBOBJECT stDBObj;
	GLOBAL stGlobal;
	int iRet;
	char szWhere[1024];
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows;

	snprintf(szDBName, sizeof(szDBName), "world_%d_%d", iRegionID, iWorldID);
	
	hDBHandle = gamestat_conn(pstEnv, szDBName);
	if (NULL == hDBHandle)
	{
		return -1;
	}

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Global");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stGlobal);
	stDBObj.pszObj = (char*)&stGlobal;

	sprintf(szWhere,"WHERE ID >=0");

	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_SELECT, &stDBObj, szWhere);
	if (0 != iRet)
	{
		printf("…˙≥…≤È—Ø◊ ‘¥º«¬ºµƒSQL”Ôæ‰ ß∞‹: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		goto _end;
	}

	if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		goto _end;
	}

	iNumRows = tdr_num_rows(hDBResult);

	if (iNumRows != 1)
	{
		goto _end;
	}

	
	{
		iRet = tdr_fetch_row(hDBResult, &stDBObj);
		
		if (iRet == 0)
		{
			global_fetch(iRegionID, iWorldID, &stGlobal);
		}
	}

	tdr_free_dbresult(&hDBResult);
	
_end:
	tdr_close_dbhanlde(hDBHandle);

	
	return 0;
}

int role_scan(GAMESTATENV *pstEnv)
{
	int i, j;
	
	printf("----role scan stat start:%ld-----\n", time(NULL));

	for (i=0; i<pstEnv->stConf.iRegionNum; i++)
	{
		for (j=0; j<pstEnv->stConf.astRegion[i].iWorldNum; j++)
		{
			role_scan_world(pstEnv, pstEnv->stConf.astRegion[i].iRegionid, 
				pstEnv->stConf.astRegion[i].astWorld[j].iWorldid);
		}
	}

	printf("----role scan stat end:%ld-----\n", time(NULL));
	
	return 0;
}

int global_scan(GAMESTATENV *pstEnv)
{
	int i, j;

	for (i=0; i<pstEnv->stConf.iRegionNum; i++)
	{
		for (j=0; j<pstEnv->stConf.astRegion[i].iWorldNum; j++)
		{
			global_scan_world(pstEnv, pstEnv->stConf.astRegion[i].iRegionid, 
				pstEnv->stConf.astRegion[i].astWorld[j].iWorldid);
		}
	}

	return 0;
}

int world_role_stat_pre(GAMESTATENV *pstEnv)
{
	int i, j;
	
	for (i=0; i<pstEnv->stConf.iRegionNum; i++)
	{
		for (j=0; j<pstEnv->stConf.astRegion[i].iWorldNum; j++)
		{
			pstEnv->pstWorldStat[pstEnv->iWorldStat] = calloc(1, sizeof(WORLDSTAT));
			pstEnv->pstWorldStat[pstEnv->iWorldStat]->iRegionID = pstEnv->stConf.astRegion[i].iRegionid;
			pstEnv->pstWorldStat[pstEnv->iWorldStat]->iWorldID = pstEnv->stConf.astRegion[i].astWorld[j].iWorldid;
			pstEnv->iWorldStat++;
		}
	}

	return 0;
}

int role_level_stat(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS level_stat_%d_%d ( "
		"level INT, rolenum INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);
	
	for (i=1; i<=MAX_LEVEL; i++)
	{
		if (0 == pstWorldStat->astLevel[i].iRoleNum)
		{
			continue;
		}

		pstWorldStat->astLevel[0].iRoleNum += pstWorldStat->astLevel[i].iRoleNum;

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_stat_%d_%d (level,rolenum) VALUES (%d,%d);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, pstWorldStat->astLevel[i].iRoleNum);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
		
		//printf("level %d, rolenum %d\n", i, pstWorldStat->astLevel[i].iRoleNum);
	}

	//level 0 total
	{
		snprintf(szSql, sizeof(szSql), "INSERT INTO level_stat_%d_%d (level,rolenum) VALUES (%d,%d);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 0, pstWorldStat->astLevel[0].iRoleNum);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
	}
	
	return 0;
}

int role_level_daytime(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS level_daytime_%d_%d ( "
		"level INT, rolenum INT, max INT, min INT, avg INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	for (i=1; i<=MAX_LEVEL; i++)
	{
		if (0 == pstWorldStat->astLevelDayTime[i].iRoleNum)
		{
			continue;
		}
		
		pstWorldStat->astLevelDayTime[i].iAvg = pstWorldStat->astLevelDayTime[i].llTotal/pstWorldStat->astLevelDayTime[i].iRoleNum;

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_daytime_%d_%d (level,rolenum,max,min,avg) VALUES (%d,%d,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, pstWorldStat->astLevelDayTime[i].iRoleNum, pstWorldStat->astLevelDayTime[i].iMax/60, pstWorldStat->astLevelDayTime[i].iMin/60, 
			pstWorldStat->astLevelDayTime[i].iAvg/60);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
		//printf("level %d, avg %d\n", i, pstWorldStat->astLevelDayTime[i].iAvg);
	}
	
	return 0;
}

int role_level_money(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS level_money_%d_%d ( "
		"level INT, moneytype INT, rolenum INT, max BIGINT, min BIGINT, avg BIGINT, total BIGINT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	for (i=1; i<=MAX_LEVEL; i++)
	{
		if (0 == pstWorldStat->astLevelMoney[i].iRoleNum)
		{
			continue;
		}

		pstWorldStat->astLevelMoney[0].llTotal += pstWorldStat->astLevelMoney[i].llTotal;
		pstWorldStat->astLevelMoney[0].iRoleNum += pstWorldStat->astLevelMoney[i].iRoleNum;
		pstWorldStat->astLevelBindMoney[0].llTotal += pstWorldStat->astLevelBindMoney[i].llTotal;
		pstWorldStat->astLevelBindMoney[0].iRoleNum += pstWorldStat->astLevelBindMoney[i].iRoleNum;
		pstWorldStat->astLevelGold[0].llTotal += pstWorldStat->astLevelGold[i].llTotal;
		pstWorldStat->astLevelGold[0].iRoleNum += pstWorldStat->astLevelGold[i].iRoleNum;
		pstWorldStat->astLevelBindGold[0].llTotal += pstWorldStat->astLevelBindGold[i].llTotal;
		pstWorldStat->astLevelBindGold[0].iRoleNum += pstWorldStat->astLevelBindGold[i].iRoleNum;
		
		
		pstWorldStat->astLevelMoney[i].iAvg = pstWorldStat->astLevelMoney[i].llTotal/pstWorldStat->astLevelMoney[i].iRoleNum;
		pstWorldStat->astLevelBindMoney[i].iAvg = pstWorldStat->astLevelBindMoney[i].llTotal/pstWorldStat->astLevelMoney[i].iRoleNum;
		pstWorldStat->astLevelGold[i].iAvg = pstWorldStat->astLevelGold[i].llTotal/pstWorldStat->astLevelMoney[i].iRoleNum;
		pstWorldStat->astLevelBindGold[i].iAvg = pstWorldStat->astLevelBindGold[i].llTotal/pstWorldStat->astLevelMoney[i].iRoleNum;

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, 1, pstWorldStat->astLevelMoney[i].iRoleNum,
			pstWorldStat->astLevelMoney[i].iMax, pstWorldStat->astLevelMoney[i].iMin,
			pstWorldStat->astLevelMoney[i].iAvg, pstWorldStat->astLevelMoney[i].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, 2, pstWorldStat->astLevelBindMoney[i].iRoleNum,
			pstWorldStat->astLevelBindMoney[i].iMax, pstWorldStat->astLevelBindMoney[i].iMin,
			pstWorldStat->astLevelBindMoney[i].iAvg, pstWorldStat->astLevelBindMoney[i].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, 3, pstWorldStat->astLevelGold[i].iRoleNum,
			pstWorldStat->astLevelGold[i].iMax, pstWorldStat->astLevelGold[i].iMin,
			pstWorldStat->astLevelGold[i].iAvg, pstWorldStat->astLevelGold[i].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, 4, pstWorldStat->astLevelBindGold[i].iRoleNum,
			pstWorldStat->astLevelBindGold[i].iMax, pstWorldStat->astLevelBindGold[i].iMin,
			pstWorldStat->astLevelBindGold[i].iAvg, pstWorldStat->astLevelBindGold[i].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
		
		//printf("level %d, avg %d\n", i, pstWorldStat->astLevelDayTime[i].iAvg);
	}

	//level 0 total
	{
		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 0, 1, pstWorldStat->astLevelMoney[0].iRoleNum,
			pstWorldStat->astLevelMoney[0].iMax, pstWorldStat->astLevelMoney[0].iMin,
			pstWorldStat->astLevelMoney[0].iAvg, pstWorldStat->astLevelMoney[0].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 0, 2, pstWorldStat->astLevelBindMoney[0].iRoleNum,
			pstWorldStat->astLevelBindMoney[0].iMax, pstWorldStat->astLevelBindMoney[0].iMin,
			pstWorldStat->astLevelBindMoney[0].iAvg, pstWorldStat->astLevelBindMoney[0].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 0, 3, pstWorldStat->astLevelGold[0].iRoleNum,
			pstWorldStat->astLevelGold[0].iMax, pstWorldStat->astLevelGold[0].iMin,
			pstWorldStat->astLevelGold[0].iAvg, pstWorldStat->astLevelGold[0].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		snprintf(szSql, sizeof(szSql), "INSERT INTO level_money_%d_%d (level,moneytype,rolenum,max,min,avg,total) VALUES (%d,%d, %d,%lld,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 0, 4, pstWorldStat->astLevelBindGold[0].iRoleNum,
			pstWorldStat->astLevelBindGold[0].iMax, pstWorldStat->astLevelBindGold[0].iMin,
			pstWorldStat->astLevelBindGold[0].iAvg, pstWorldStat->astLevelBindGold[0].llTotal);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
	}
	
	return 0;
}

int role_level_arm(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i, j, k;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	LEVELARMSTATDETAIL *pstLevelStat;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS level_arm_%d_%d ( "
		"pinzhi INT, levelduan INT, ArmPos INT, rolenum INT, max INT, total BIGINT, Gem BIGINT, "
		"FuMo BIGINT, BaseAttr BIGINT, FujiaAttr BIGINT, WashAttr BIGINT, Star BIGINT, PerfectStar BIGINT ) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	
	for (i=0; i<MAX_PINZHI; i++)
	{
		for (j=0; j<MAX_LEVEL_DUAN; j++)
		{
			for (k=0; k<ARM_POS; k++)
			{
				pstLevelStat = &pstWorldStat->astLevelArmPinZhi[i][j][k];
				if (pstLevelStat->iRoleNum > 0)
				{
					snprintf(szSql, sizeof(szSql), "INSERT INTO level_arm_%d_%d (pinzhi , levelduan , armpos, rolenum , max , total, Gem, FuMo, BaseAttr, FujiaAttr, WashAttr, Star, PerfectStar ) "
						"VALUES (%d,%d,%d,%d,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld);",
						pstWorldStat->iRegionID, pstWorldStat->iWorldID, i+1, j,  k,pstLevelStat->iRoleNum, pstLevelStat->iMax,pstLevelStat->llTotal,
						pstLevelStat->llGem, pstLevelStat->llFuMo, pstLevelStat->llBaseAttr, pstLevelStat->llFujiaAttr, pstLevelStat->llWashAttr,
						pstLevelStat->llStar, pstLevelStat->llPerfectStar);
					stSqlData.pszBuff = szSql;
					stSqlData.iBuff = strlen(szSql);

					if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
					{
						printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
						return -1;
					}

					tdr_free_dbresult(&hDBResult);
				}
			}
			
		}
	}
	
	return 0;
}

int role_level_secondskill(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	LEVELSECONDSKILLSTATDETAIL *pstLevelStat;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS level_secondskill_%d_%d ( "
		"level INT, rolenum INT, "
		"Type4Num BIGINT, Type4AvgLevel BIGINT, Type4MaxLevel INT, "
		"Type4AvgSkillVal BIGINT, Type4MaxSkillVal BIGINT, Type4AvgRecipe BIGINT, Type4MaxRecipe BIGINT, "
		"Type5Num BIGINT, Type5AvgLevel BIGINT, Type5MaxLevel INT, "
		"Type5AvgSkillVal BIGINT, Type5MaxSkillVal BIGINT, Type5AvgRecipe BIGINT, Type5MaxRecipe BIGINT, "
		"Type6Num BIGINT, Type6AvgLevel BIGINT, Type6MaxLevel INT, "
		"Type6AvgSkillVal BIGINT, Type6MaxSkillVal BIGINT, Type6AvgRecipe BIGINT, Type6MaxRecipe BIGINT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	
	for (i=0; i<MAX_LEVEL; i++)
	{
		pstLevelStat = &pstWorldStat->astLevelSecondSkill[i+1];
			
		if (0 == pstLevelStat->iRoleNum)
		{
			continue;
		}
		
		snprintf(szSql, sizeof(szSql), "INSERT INTO level_secondskill_%d_%d "
			"(level , rolenum , "
			"Type4Num, Type4AvgLevel , Type4MaxLevel , Type4AvgSkillVal, Type4MaxSkillVal, Type4AvgRecipe, Type4MaxRecipe, "
			"Type5Num, Type5AvgLevel , Type5MaxLevel , Type5AvgSkillVal, Type5MaxSkillVal, Type5AvgRecipe, Type5MaxRecipe, "
			"Type6Num, Type6AvgLevel , Type6MaxLevel , Type6AvgSkillVal, Type6MaxSkillVal, Type6AvgRecipe, Type6MaxRecipe "
			") " 
			"VALUES (%d,%d,%lld,%lld,%d,%lld,%d, %lld, %d, %lld,%lld,%d,%lld,%d, %lld, %d, %lld,%lld,%d,%lld,%d, %lld, %d);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i+1, pstLevelStat->iRoleNum,
			pstLevelStat->llType4Num, 
			pstLevelStat->llType4Num<=0?0:pstLevelStat->iType4AvgLevel/pstLevelStat->llType4Num, 
			pstLevelStat->iType4MaxLevel,
			pstLevelStat->llType4Num<=0?0:pstLevelStat->iType4AvgSkillVal/pstLevelStat->llType4Num, 
			pstLevelStat->iType4MaxSkillVal, 
			pstLevelStat->llType4Num<=0?0:pstLevelStat->iType4AvgRecipe/pstLevelStat->llType4Num, 
			pstLevelStat->iType4MaxRecipe,
			pstLevelStat->llType5Num, 
			pstLevelStat->llType5Num<=0?0:pstLevelStat->iType5AvgLevel/pstLevelStat->llType5Num, 
			pstLevelStat->iType5MaxLevel,
			pstLevelStat->llType5Num<=0?0:pstLevelStat->iType5AvgSkillVal/pstLevelStat->llType5Num, 
			pstLevelStat->iType5MaxSkillVal, 
			pstLevelStat->llType5Num<=0?0:pstLevelStat->iType5AvgRecipe/pstLevelStat->llType5Num, 
			pstLevelStat->iType5MaxRecipe,
			pstLevelStat->llType6Num, 
			pstLevelStat->llType6Num<=0?0:pstLevelStat->iType6AvgLevel/pstLevelStat->llType6Num, 
			pstLevelStat->iType6MaxLevel,
			pstLevelStat->llType6Num<=0?0:pstLevelStat->iType6AvgSkillVal/pstLevelStat->llType6Num, 
			pstLevelStat->iType6MaxSkillVal, 
			pstLevelStat->llType6Num<=0?0:pstLevelStat->iType6AvgRecipe/pstLevelStat->llType6Num, 
			pstLevelStat->iType6MaxRecipe
			);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
	}
	
	return 0;
}

int role_pet_stat(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	PETSTATDETAIL *pstPetStat;
	PETSKILLSTATDETAIL *pstPetSkillStat;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS pet_%d_%d ( "
		"petid INT, total INT, "
		"pinzhi1 INT, pinzhi2 INT, pinzhi3 INT, pinzhi4 INT, pinzhi5 INT, "
		"LevelMax INT, LevelTotal BIGINT, Star BIGINT, PerfectStar BIGINT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	
	for (i=0; i<MAX_PET; i++)
	{
		pstPetStat = &pstWorldStat->astPet[i];
			
		if (0 == pstPetStat->iPetID)
		{
			break;
		}
		
		snprintf(szSql, sizeof(szSql), "INSERT INTO pet_%d_%d "
			"(petid , total , "
			"pinzhi1, pinzhi2 , pinzhi3 , pinzhi4, pinzhi5, LevelMax, LevelTotal, "
			"Star, PerfectStar "
			") " 
			"VALUES (%d,%d,%d,%d,%d, %d, %d, %d,%lld,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 
			pstPetStat->iPetID,  pstPetStat->iTotal, 
			pstPetStat->iPinZhi[0], pstPetStat->iPinZhi[1],
			pstPetStat->iPinZhi[2], pstPetStat->iPinZhi[3], pstPetStat->iPinZhi[4],
			pstPetStat->iLevelMax, pstPetStat->llLevelTotal, pstPetStat->llStar,
			pstPetStat->llPerfectStar
			);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
	}

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS petskill_%d_%d ( "
		"skillid INT, skilllevelmax INT, total BIGINT, "
		"skillleveltotal BIGINT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	
	for (i=0; i<MAX_PETSKILL; i++)
	{
		pstPetSkillStat = &pstWorldStat->astPetSkill[i];
			
		if (0 == pstPetSkillStat->iSkillID)
		{
			break;
		}
		
		snprintf(szSql, sizeof(szSql), "INSERT INTO petskill_%d_%d "
			"(skillid , skilllevelmax , total, skillleveltotal "
			") " 
			"VALUES (%d,%d,%lld, %lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, 
			pstPetSkillStat->iSkillID, pstPetSkillStat->iSkillLevelMax,
			pstPetSkillStat->llTotal, pstPetSkillStat->llSkillLevelTotal
			);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
	}
	
	return 0;
}

int role_genuine_stat(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i, j;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	GENUINESTATDETAIL *pstGenuineDetail;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS genuine_%d_%d ( "
		"GenuineType INT, AttrID INT, Career INT, RoleNum INT,"
		"MaxGenuineLevel INT, AvgGenuineLevel INT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	for (i=1; i<=MAX_LEVEL; i++)
	{
		for (j=0; j<MAX_GENIUS_TYPE*MAX_GENIUS_SUBTYPE*MAX_SUB_CAREER; j++)
		{
			pstGenuineDetail = &pstWorldStat->astGenuine[j][i];
			if (pstGenuineDetail->iSubCareer == 0)
			{
				break;
			}

			snprintf(szSql, sizeof(szSql), "INSERT INTO genuine_%d_%d "
			"(GenuineType , AttrID , Career, RoleNum, "
			"MaxGenuineLevel, AvgGenuineLevel "
			") " 
			"VALUES (%d,%d,%d,%d, %d, %d);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, pstGenuineDetail->iGenuineType,
			pstGenuineDetail->iGenuineAttrID, pstGenuineDetail->iSubCareer, pstGenuineDetail->iRoleNum,
			pstGenuineDetail->iMaxGenuineLevel, (int)(pstGenuineDetail->llTotal/pstGenuineDetail->iRoleNum)
			);
			stSqlData.pszBuff = szSql;
			stSqlData.iBuff = strlen(szSql);

			if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
			{
				printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
				return -1;
			}

			tdr_free_dbresult(&hDBResult);
		}
	}

	return 0;
}

int role_spar_stat(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i, j;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	SPARSTATDETAIL *pstSparStat;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS spar_%d_%d ( "
		"levelduan INT, itemid INT, "
		"total BIGINT, leveltotal BIGINT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	
	for (i=0; i<MAX_LEVEL_DUAN; i++)
	{
		for (j=0; j<MAX_SPAR_ITEM; j++)
		{
			pstSparStat = &pstWorldStat->astSpar[i][j];
			if (pstSparStat->iItemID == 0)
				break;

			snprintf(szSql, sizeof(szSql), "INSERT INTO spar_%d_%d "
			"(levelduan , itemid , "
			"total, leveltotal "
			") " 
			"VALUES (%d,%d,%lld,%lld);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i, pstSparStat->iItemID, pstSparStat->llTotal, pstSparStat->llLevelTotal
			);
			stSqlData.pszBuff = szSql;
			stSqlData.iBuff = strlen(szSql);

			if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
			{
				printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
				return -1;
			}

			tdr_free_dbresult(&hDBResult);
		}
	}
	
	return 0;
}

int role_gem_stat(GAMESTATENV *pstEnv, WORLDSTAT *pstWorldStat)
{
	int i, j;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	SPARSTATDETAIL *pstSparStat;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS gemall_%d_%d ( "
		"gemnum INT, rolenum INT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		pstWorldStat->iRegionID, pstWorldStat->iWorldID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	for (i=0; i<MAX_GEM_NUM; i++)
	{
		snprintf(szSql, sizeof(szSql), "INSERT INTO gemall_%d_%d "
			"(gemnum , rolenum "
			") " 
			"VALUES (%d,%d);",
			pstWorldStat->iRegionID, pstWorldStat->iWorldID, i+1, pstWorldStat->aiGemAll[i]
			);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);
	}

	for (i=0; i<MAX_GEM_LEV; i++)
	{
		snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS gem%d_%d_%d ( "
		"gemnum INT, rolenum INT "
		") ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		i+1,pstWorldStat->iRegionID, pstWorldStat->iWorldID);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult);

		for (j=0; j<MAX_GEM_NUM; j++)
		{
			snprintf(szSql, sizeof(szSql), "INSERT INTO gem%d_%d_%d "
				"(gemnum , rolenum "
				") " 
				"VALUES (%d,%d);",
				i+1, pstWorldStat->iRegionID, pstWorldStat->iWorldID, j+1, pstWorldStat->aiGemLev[i][j]
				);
			stSqlData.pszBuff = szSql;
			stSqlData.iBuff = strlen(szSql);

			if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
			{
				printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
				return -1;
			}

			tdr_free_dbresult(&hDBResult);
		}

	}
	
	return 0;
}

int world_role_stat(GAMESTATENV *pstEnv)
{
	int i;

	for (i=0; i<pstEnv->iWorldStat; i++)
	{
		/*
		role_level_daytime(pstEnv, pstEnv->pstWorldStat[i]);
		role_level_stat(pstEnv, pstEnv->pstWorldStat[i]);
		role_level_money(pstEnv, pstEnv->pstWorldStat[i]);
		role_level_arm(pstEnv, pstEnv->pstWorldStat[i]);
		role_level_secondskill(pstEnv, pstEnv->pstWorldStat[i]);
		role_pet_stat(pstEnv, pstEnv->pstWorldStat[i]);
		role_spar_stat(pstEnv, pstEnv->pstWorldStat[i]);
		role_genuine_stat(pstEnv, pstEnv->pstWorldStat[i]);*/
		role_gem_stat(pstEnv, pstEnv->pstWorldStat[i]);
		
	}
	
	return 0;
}

int region_stat_liucun(GAMESTATENV *pstEnv, int iRegionID, TDRDBHANDLE hDBHandle)
{
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult;
	unsigned int iNewAccount;
	unsigned int iActiveNewAccount;
	unsigned int iLiuCunAccount;
	double fTmp;
	int iTime, iTime1, iTime2;

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS region_liucun_%d ( "
		"type INT, num DOUBLE) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	printf("--------region %d stat liucun start:%ld------\n", iRegionID, time(NULL));

	//◊ÚÃÏ–¬Ω®’ ∫≈
	iTime = get_day_six_time(pstEnv->iSec, 1);
	snprintf(szSql, sizeof(szSql), "where AccountCreateTime > %d", iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iNewAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}

	snprintf(szSql, sizeof(szSql), "INSERT INTO region_liucun_%d (type,num) VALUES (%d,%d);",
			iRegionID, 1, iNewAccount);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	//◊ÚÃÏ–¬Ω®’ ∫≈÷–µƒµ«¬ºπ˝µƒ’ ∫≈
	snprintf(szSql, sizeof(szSql), "where AccountCreateTime > %d and LastLoginTime != 0", iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iActiveNewAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}

	snprintf(szSql, sizeof(szSql), "INSERT INTO region_liucun_%d (type,num) VALUES (%d,%d);",
			iRegionID, 2, iActiveNewAccount);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	// 2 days ¡Ù¥Ê
	iTime1 = get_day_six_time(pstEnv->iSec, 2);
	snprintf(szSql, sizeof(szSql), 
			"where AccountCreateTime > %d and AccountCreateTime < %d and LastLoginTime != 0",
			 iTime1, iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iActiveNewAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}
	
	snprintf(szSql, sizeof(szSql), 
			"where AccountCreateTime > %d and AccountCreateTime < %d and LastLoginTime != 0 and "
			"(LastLoginTime > %d or LastLoginTime > LastLogout or LastLogout > %d)", 
			iTime1, iTime, iTime, iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iLiuCunAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}

	if (0 == iActiveNewAccount)
	{
		fTmp = 0;
	}
	else
	{
		fTmp = iLiuCunAccount*100.0/iActiveNewAccount;
	}

	snprintf(szSql, sizeof(szSql), "INSERT INTO region_liucun_%d (type,num) VALUES (%d,%.2f);",
			iRegionID, 3, fTmp);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	//7ÃÏ¡Ù¥Ê
	iTime1 = get_day_six_time(pstEnv->iSec, 7);
	iTime2 = get_day_six_time(pstEnv->iSec, 6);
	snprintf(szSql, sizeof(szSql), 
			"where AccountCreateTime > %d and AccountCreateTime < %d and LastLoginTime != 0",
			 iTime1, iTime2);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iActiveNewAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}
	
	snprintf(szSql, sizeof(szSql), 
			"where AccountCreateTime > %d and AccountCreateTime < %d and LastLoginTime != 0 and "
			"(LastLoginTime > %d or LastLoginTime > LastLogout or LastLogout > %d)", 
			iTime1, iTime2, iTime, iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iLiuCunAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}

	if (0 == iActiveNewAccount)
	{
		fTmp = 0;
	}
	else
	{
		fTmp = iLiuCunAccount*100.0/iActiveNewAccount;
	}

	snprintf(szSql, sizeof(szSql), "INSERT INTO region_liucun_%d (type,num) VALUES (%d,%.2f);",
			iRegionID, 4, fTmp);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	//30ÃÏ¡Ù¥Ê
	iTime1 = get_day_six_time(pstEnv->iSec, 30);
	iTime2 = get_day_six_time(pstEnv->iSec, 29);
	snprintf(szSql, sizeof(szSql), 
			"where AccountCreateTime > %d and AccountCreateTime < %d and LastLoginTime != 0",
			 iTime1, iTime2);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iActiveNewAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}
	
	snprintf(szSql, sizeof(szSql), 
			"where AccountCreateTime > %d and AccountCreateTime < %d and LastLoginTime != 0 and "
			"(LastLoginTime > %d or LastLoginTime > LastLogout or LastLogout > %d)", 
			iTime1, iTime2, iTime, iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iLiuCunAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}

	if (0 == iActiveNewAccount)
	{
		fTmp = 0;
	}
	else
	{
		fTmp = iLiuCunAccount*100.0/iActiveNewAccount;
	}

	snprintf(szSql, sizeof(szSql), "INSERT INTO region_liucun_%d (type,num) VALUES (%d,%.2f);",
			iRegionID, 5, fTmp);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	//◊ÚÃÏµ«¬º
	snprintf(szSql, sizeof(szSql), 
			"where  LastLoginTime > %d or LastLoginTime > LastLogout or LastLogout > %d", 
			iTime, iTime);
	if (0 > tdr_get_records_count(hDBHandle, "Account", szSql, &iLiuCunAccount))
	{
		printf("tdr_get_records_count fail\n");
		return -1;
	}

	snprintf(szSql, sizeof(szSql), "INSERT INTO region_liucun_%d (type,num) VALUES (%d,%d);",
			iRegionID, 6, iLiuCunAccount);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);
	
	printf("--------region %d stat liucun end:%ld------\n", iRegionID, time(NULL));

	return 0;
}

int get_max_level(GAMESTATENV *pstEnv, TDRDBHANDLE hWorldDBHandle, int iUin)
{
	int iLevel = 0;
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult = 0;
	char *pszField;
	int iField;

	snprintf(szSql, sizeof(szSql), "select Level from RoleData where Uin=%d ORDER BY Level DESC limit 1",iUin );
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, hWorldDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	if (0 == tdr_dbms_fetch_row(hDBResult))
	{
		if (0 == tdr_dbms_fetch_field(&pszField, &iField, hDBResult, "Level"))
		{
			iLevel = atoi(pszField);
		}
	}

	tdr_free_dbresult(&hDBResult);
	
	return iLevel;
}

int region_stat_7day_liushi(GAMESTATENV *pstEnv, int iRegionID, TDRDBHANDLE hDBHandle)
{
	char szSql[102400];
	TDRDATA stSqlData;
	TDRDBRESULTHANDLE hDBResult = 0;
	TDRDBRESULTHANDLE hDBResult1 = 0;
	ACCOUNT stAccount;
	TDRDBOBJECT stDBObj;
	char szWhere[1024];
	int iRet;
	int iNumRows;
	int i;
	int iTime1, iTime2;
	int iPreWorldID = 0;
	int iWorldCount = 0;
	int astLevel[MAX_LEVEL+1];
	int iLevel;
	TDRDBHANDLE hWorldDBHandle = NULL;


	memset(astLevel, 0, sizeof(astLevel));
	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS region_7dayliushi1_%d ( "
		"worldid INT, num INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	snprintf(szSql, sizeof(szSql), "CREATE TABLE IF NOT EXISTS region_7dayliushi2_%d ( "
		"level INT, num INT) ENGINE=InnoDB DEFAULT CHARSET=latin1;",
		iRegionID);
	stSqlData.pszBuff = szSql;
	stSqlData.iBuff = strlen(szSql);

	if(tdr_query(&hDBResult, pstEnv->hStatDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	tdr_free_dbresult(&hDBResult);

	printf("--------region %d stat 7 day liushi start:%ld------\n", iRegionID, time(NULL));

	iTime1 = get_day_six_time(pstEnv->iSec, 14);
	iTime2 = get_day_six_time(pstEnv->iSec, 7);	
	stSqlData.iBuff = sizeof(szSql);
	stSqlData.pszBuff = &szSql[0];

	sprintf(szWhere,"WHERE LastLoginTime != 0 and LastLogout != 0 and "
		"( LastLoginTime > %d or LastLogout > %d) and ( LastLoginTime < %d and LastLogout < %d ) "
		"ORDER BY LastLoginWorldID",
		iTime1, iTime1, iTime2, iTime2);

	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Account");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stAccount);
	stDBObj.pszObj = (char*)&stAccount;

	iRet = tdr_obj2sql(hDBHandle, &stSqlData, TDR_DBOP_SELECT, &stDBObj, szWhere);
	if (0 != iRet)
	{
		printf("…˙≥…≤È—Ø◊ ‘¥º«¬ºµƒSQL”Ôæ‰ ß∞‹: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return -1;
	}

	if(tdr_query(&hDBResult, hDBHandle, &stSqlData) != 0)
	{
		printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
		return -1;
	}

	iNumRows = tdr_num_rows(hDBResult);
	printf("--------region %d stat 7 day liushi num:%d------\n", iRegionID, iNumRows);
	for (i=0; i<iNumRows; i++)
	{
		iRet = tdr_fetch_row(hDBResult, &stDBObj);
		if (0 != iRet)
		{			
			if ((TDR_ERR_GET_ERROR_CODE(iRet) != TDR_ERROR_DB_NO_RECORD_IN_RESULTSET) ||	
				(0 != tdr_dbms_errno(hDBResult)))			
			{				
				printf("¥” ˝æ›ø‚±Ì÷–∂¡»° ˝æ›º«¬º ß∞‹:%s, dbms  error:%s\n",					
				tdr_error_string(iRet), tdr_dbms_error(hDBResult));
			}
			else	
			{		
				iRet = 0; 
			}	
			break;		
		}

		if (iPreWorldID == 0)
		{
			iPreWorldID = stAccount.LastLoginWorldID;
			snprintf(szSql, sizeof(szSql), "world_%d_%d", iRegionID, stAccount.LastLoginWorldID);
			hWorldDBHandle = gamestat_conn(pstEnv, szSql);
			if (NULL == hWorldDBHandle)
			{
				while (i < iNumRows-1 && iPreWorldID == stAccount.LastLoginWorldID)
				{
					iWorldCount++;
					tdr_fetch_row(hDBResult, &stDBObj);
					i++;
				}
			}
		}

_next:
		
		if (iPreWorldID != stAccount.LastLoginWorldID)
		{
			snprintf(szSql, sizeof(szSql), "INSERT INTO region_7dayliushi1_%d (worldid,num) VALUES (%d,%d);",
						iRegionID, iPreWorldID, iWorldCount);
			stSqlData.pszBuff = szSql;
			stSqlData.iBuff = strlen(szSql);

			if(tdr_query(&hDBResult1, pstEnv->hStatDBHandle, &stSqlData) != 0)
			{
				printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
				return -1;
			}

			tdr_free_dbresult(&hDBResult1);
						
			iWorldCount = 0;
			if (hWorldDBHandle)
			{
				tdr_close_dbhanlde(hWorldDBHandle);
			}

			snprintf(szSql, sizeof(szSql), "world_%d_%d", iRegionID, stAccount.LastLoginWorldID);
			hWorldDBHandle = gamestat_conn(pstEnv, szSql);
			if (NULL == hWorldDBHandle)
			{
				iPreWorldID = stAccount.LastLoginWorldID;
				while (i < iNumRows-1 && iPreWorldID == stAccount.LastLoginWorldID)
				{
					iWorldCount++;
										
					tdr_fetch_row(hDBResult, &stDBObj);
					i++;
				}
				
				goto _next;
			}
		}
		iWorldCount++;
		iPreWorldID = stAccount.LastLoginWorldID;

		iLevel = get_max_level(pstEnv, hWorldDBHandle, stAccount.Uin);
		if (iLevel > 0)
		{
			astLevel[iLevel]++;
		}
	}

	//◊Ó∫Û“ª∏ˆ
	{
		snprintf(szSql, sizeof(szSql), "INSERT INTO region_7dayliushi1_%d (worldid,num) VALUES (%d,%d);",
					iRegionID, iPreWorldID, iWorldCount);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult1, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult1);
	}

	for (i=1; i<=MAX_LEVEL; i++)
	{
		if (astLevel[i] == 0)
		{
			continue;
		}
		
		snprintf(szSql, sizeof(szSql), "INSERT INTO region_7dayliushi2_%d  (level,num) VALUES (%d,%d);",
			iRegionID, i, astLevel[i]);
		stSqlData.pszBuff = szSql;
		stSqlData.iBuff = strlen(szSql);

		if(tdr_query(&hDBResult1, pstEnv->hStatDBHandle, &stSqlData) != 0)
		{
			printf("÷¥––SQL”Ôæ‰ ß∞‹!\n");
			return -1;
		}

		tdr_free_dbresult(&hDBResult1);
	}

	tdr_free_dbresult(&hDBResult);

	printf("--------region %d stat 7 day liushi end:%ld------\n", iRegionID, time(NULL));

	return 0;
}	

int region_stat_one(GAMESTATENV *pstEnv, int iRegionID)
{
	char szDBName[128];
	TDRDBHANDLE hDBHandle;

	snprintf(szDBName, sizeof(szDBName), "account_%d", iRegionID);	
	hDBHandle = gamestat_conn(pstEnv, szDBName);
	if (NULL == hDBHandle)
	{
		return -1;
	}

	//region_stat_liucun(pstEnv, iRegionID, hDBHandle);
	//region_stat_7day_liushi(pstEnv, iRegionID, hDBHandle);

	tdr_close_dbhanlde(hDBHandle);
	
	return 0;
}

int region_stat(GAMESTATENV *pstEnv)
{
	int i;
	
	for (i=0; i<pstEnv->stConf.iRegionNum; i++)
	{
		region_stat_one(pstEnv, pstEnv->stConf.astRegion[i].iRegionid);
	}
	
	return 0;
}

//-------------º∆À„‘⁄œﬂ ±º‰-------------------start

time_t gamestat_strtime1(const char *szTime)
{
	time_t tTime;
	struct tm stTm;
	
	if (NULL == strptime(szTime, "%Y-%m-%d %H:%M:%S", &stTm))
	{
		return -1;
	}

	tTime = mktime(&stTm);
	return tTime;
}

// 120721
time_t gamestat_strtime2(const char *szTime)
{
	char szBuff[64]={0};
	struct tm tmTime; 

	szBuff[0] = '2';
	szBuff[1] = '0';
	strcat(szBuff,szTime);
	strcat(szBuff,"000000");

	sscanf(szBuff,"%4d%2d%2d%2d%2d%2d",
		                  &tmTime.tm_year,
		                  &tmTime.tm_mon,
		                  &tmTime.tm_mday,
		                  &tmTime.tm_hour,
		                  &tmTime.tm_min,
		                  &tmTime.tm_sec);
	tmTime.tm_year -= 1900;
	tmTime.tm_mon --;
	tmTime.tm_isdst=-1;
		
	return mktime(&tmTime);
}


static int gamestat_cmp(const void *p1, const void *p2)
{
	CACLDATA *pstData1;
	CACLDATA *pstData2;

	pstData1  = (CACLDATA *)p1;
	pstData2  = (CACLDATA *)p2;

	if (pstData1->t <= pstData2->t)
		return -1;
	return 1;
}


// 2012-07-21 12:12:30->120812
int gamestat_strtime3(const char *szTime)
{
	int iTime;
	struct tm stTm;
	char szStr[128];
	
	if (NULL == strptime(szTime, "%Y-%m-%d %H:%M:%S", &stTm))
	{
		return -1;
	}

	strftime(szStr, sizeof(szStr), "%y%m%d", &stTm);

	iTime = atoi(szStr);
	return iTime;
}

void gamestat_make_sql(char *szIn, int iSizeIn,int iStart, const char *pszStart, const char *pszEnd, 
							const char* pszTable,unsigned long long ullRoleID)
{
	memset(szIn, 0, iSizeIn);
	// 90815 ƒÍ «–°”⁄10µƒ
	if (iStart < 100000)
	{
		snprintf(szIn, iSizeIn, 
			"select LogTime from %s_0%d where RoleID=%llu && LogTime>='%s' &&LogTime<='%s'", 
			pszTable,iStart, ullRoleID,pszStart,pszEnd);
	}
	else
	{
		snprintf(szIn, iSizeIn, 
			"select LogTime from %s_%d where RoleID=%llu && LogTime>='%s' &&LogTime<='%s'", 
			pszTable,iStart, ullRoleID,pszStart,pszEnd);
	}
}


int gamestat_up_start_time(int *piStart, char *pszStart,int iLen)
{
	time_t iStart = gamestat_strtime1(pszStart);
	
	iStart += 24*60*60;
	memset(pszStart,0, iLen);
	strftime(pszStart,iLen,"%Y-%m-%d %H:%M:%S",localtime(&iStart));

	*piStart = gamestat_strtime3(pszStart);
	return 0;

}

int gamestat_get_role_online(	GAMESTATENV *pstEnv, 
								int iRegionID,
								int iWorldID,
								const char* pszStart, 
								const char *pszEnd,
								unsigned long long ullRoleID)
{
	int iStart = gamestat_strtime3(pszStart);
	int iEnd = gamestat_strtime3(pszEnd);
	int iErr;
	//int iYs,iMs,iDs;
	char szSql[1024];
	MYSQL_RES *res;
	MYSQL_ROW row;
	time_t t;
	char szDB[64];
	time_t tOnline = 0;
	char cPreFlag = 0x7F;
	int i;
	time_t tStart =  gamestat_strtime1(pszStart);
	time_t tEnd = gamestat_strtime1(pszEnd);
	char szSTime[64];
	

	if (iEnd <iStart  || tEnd < tStart)
		return -1;
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
	{	
		printf("mysql_init fail\n");
		return -1;
	}
		
	snprintf(szDB, sizeof(szDB),"world_log_%d_%d", iRegionID, iWorldID);
	if(!(mysql_real_connect(
		conn,
		pstEnv->stConf.szLogDBConnect,
		pstEnv->stConf.szLogDBUser,
		pstEnv->stConf.szLogPasswd,
		szDB,
		0,
		NULL,
		CLIENT_COMPRESS)))
	{
		printf("mysql_real_connect fail\n");
		goto end;
	}

	pstEnv->stCaclOnline.iNum = 0;
	strncpy(szSTime, pszStart, sizeof(szSTime));
	while(iStart <= iEnd)
	{
		gamestat_make_sql(szSql, sizeof(szSql), iStart, pszStart, pszEnd, "LogRoleLogin",ullRoleID);
		iErr = mysql_query(conn, szSql);
		if(!iErr)
		{
			res = mysql_store_result(conn);
			if(res)
			{
				while((row = mysql_fetch_row(res)) != NULL)
				{
					if(!row[0])
						continue;

					if (pstEnv->stCaclOnline.iNum >= MAX_CACL_DATA)
					{
						goto end;
					}
					t = gamestat_strtime1(row[0]);
					if (t > 0)
					{
						pstEnv->stCaclOnline.stData[pstEnv->stCaclOnline.iNum].t = t;
						pstEnv->stCaclOnline.stData[pstEnv->stCaclOnline.iNum].cLogin = 1;
						pstEnv->stCaclOnline.iNum ++;
					}
				}
			}

			mysql_free_result(res);
		}

		gamestat_make_sql(szSql, sizeof(szSql), iStart, pszStart, pszEnd, "LogRoleLogout",ullRoleID);
		iErr = mysql_query(conn, szSql);
		if(!iErr)
		{
			res = mysql_store_result(conn);
			if(res)
			{
				while((row = mysql_fetch_row(res)) != NULL)
				{
					if(!row[0])
						continue;

					if (pstEnv->stCaclOnline.iNum >= MAX_CACL_DATA)
					{
						goto end;
					}

					t = gamestat_strtime1(row[0]);
					if (t > 0)
					{
						pstEnv->stCaclOnline.stData[pstEnv->stCaclOnline.iNum].t = t;
						pstEnv->stCaclOnline.stData[pstEnv->stCaclOnline.iNum].cLogin = 0;
						pstEnv->stCaclOnline.iNum ++;
					}
				}
			}

			mysql_free_result(res);
		}

		gamestat_up_start_time(&iStart, szSTime, sizeof(szSTime));
		
	}

	qsort(&pstEnv->stCaclOnline.stData[0],pstEnv->stCaclOnline.iNum, sizeof(CACLDATA), gamestat_cmp);

	for (i=0; i<pstEnv->stCaclOnline.iNum; i++)
	{
		if (i==0)
		{
			if (pstEnv->stCaclOnline.stData[i].cLogin == 0)
			{
				tOnline += pstEnv->stCaclOnline.stData[i].t - tStart;
				continue;
			}
		}	

		if (i==pstEnv->stCaclOnline.iNum-1)
		{
			if (pstEnv->stCaclOnline.stData[i].cLogin == 1)
			{
				tOnline += tEnd - pstEnv->stCaclOnline.stData[i].t;
			}
		}

		// «∞“ª∏ˆ «µ«¬Ω, ∫Û“ª∏ˆ «ÕÀ≥ˆ≤≈º∆À„ ±º‰
		if (cPreFlag == 1 && pstEnv->stCaclOnline.stData[i].cLogin == 0)
		{
			tOnline += pstEnv->stCaclOnline.stData[i].t - pstEnv->stCaclOnline.stData[i-1].t;
		}
		
		cPreFlag = pstEnv->stCaclOnline.stData[i].cLogin;
	}

	if (conn) 
		mysql_close(conn);
	return tOnline;

end:
	if (conn) 
		mysql_close(conn);
	return -1;
	
}
// -----------------------------------º∆À„‘⁄œﬂ ±º‰----------------end


int main(int argc, char *argv[])
{	
	
	memset(&stEnv, 0, sizeof(stEnv));
	
	if (gamestat_init(&stEnv) < 0)
	{
		goto end;
	}

	/*if (gamestat_create_db(&stEnv) < 0)
	{
		goto end;
	}
	

	world_role_stat_pre(&stEnv);

	role_scan(&stEnv);

	world_role_stat(&stEnv);

	region_stat(&stEnv);
	*/

	global_scan(&stEnv);
	
		
end:
	if (stEnv.hStatDBHandle)
	{
		tdr_close_dbhanlde(stEnv.hStatDBHandle);
	}
	return 0;
}

