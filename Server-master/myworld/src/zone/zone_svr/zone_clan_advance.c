/*
**  @file $RCSfile: zone_clan_advance.c,v $
**  general description of this module
**  $Id: zone_clan_advance.c,v 1.138 2014/02/27 02:10:42 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2014/02/27 02:10:42 $
**  @version $Revision: 1.138 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_clan.h"
#include "zone_clan_advance.h"
#include "zone_clan_city.h"
#include "zone_clan_construction.h"
#include "zone_machine.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_db.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_package.h"
#include "zone_move.h"
#include "zone_skill.h"
#include "zone_relation.h"
#include "zone_oplog.h"
#include "zone_task.h"
#include "zone_attr.h"
#include "zone_task.h"
#include "zone_map.h"
#include "zone_range.h"
#include "zone_player.h"
#include "zone_npc.h"
#include "zone_status.h"
#include "zone_act_status.h"
#include "zone_secondaryskill.h"
#include "zone_err.h"

// Forward Declare:

extern int g_iOptype;
extern int g_iOpsubtype;

typedef struct _ClanStorageDummy
{
	int iWhich;               // �ĸ��ֿ�
	CLANSTORAGEGRID *pstGrid; // ��������
	int iMax;                 // ��������
	int *piNum;               // ��Ч����
}ClanStorageDummy;

#define INIT_CLAN_STORAGE_DUMMY() { 0, NULL, 0, NULL, }


typedef struct _ClanProgressDummy
{
	int iWhich;                       // ���ֽ���
	CLANRUNNINGPROGRESS *pstProgress; // �����б�
	int iMax;                         // ��Ч����
	int *piNum;                       // ��������
}ClanProgressDummy;

#define INIT_CLAN_PROGRESS_DUMMY() { 0, NULL, 0, NULL, }

typedef struct _ClanMachineGridInitialParam
{
	const CLANPRODUCTDEF *pstDef;
	const MONSTERDEF *pstMonDef;
	const MONMACHINEDEF *pstMacDef;
	const ClanMachinePropertyEntry *pstAdded;
	int iPos;
}ClanMachineGridInitialParam;

#define INIT_CLAN_MACHINE_GRID_INITIAL_PARAM() \
	{ NULL, NULL, NULL, NULL, -1, }


enum ClanRunningType
{
	CLAN_RUNNING_COLLECTION_MAKING = CLAN_MAKE_FOR_COLLECTION,
	CLAN_RUNNING_ARM_MAKING        = CLAN_MAKE_FOR_ARM,
	CLAN_RUNNING_RESEARCH          = 99,
};

#define COND_DO_(cond, expr) (void)((cond) ? (expr) : 0)


static ClanMachinePropertyEntry
gstDummyMacPro = INIT_CLAN_MACHINE_PROPERTY_ENTRY();

static CLANGENERALRESULT gstDummyGelRes = { 0, 0, 0, 0, 0, };

const ClanMachinePropertyEntry *kEmptyMachineProperty = &gstDummyMacPro;
const CLANGENERALRESULT *kEmptyGeneralResult = &gstDummyGelRes;

static const int kTech2Property[MAX_CLAN_TECH] = 
{
    -1, //CLAN_TECH_NONE = 0, 
    CLAN_MACHINE_MAX_HP, //CLAN_TECH_COLLECTOR_ADD_HP = 1,
    CLAN_MACHINE_DEF, //CLAN_TECH_COLLECTOR_ADD_DEF = 2,
    CLAN_MACHINE_ATK, //CLAN_TECH_COLLECTOR_ADD_ATK = 3,
    CLAN_MACHINE_CAP, //CLAN_TECH_COLLECTOR_ADD_CAP = 4,
    CLAN_MACHINE_COLLECTING_SPEED, //CLAN_TECH_COLLECTOR_ADD_COLLECT_SPEED = 5,
    CLAN_MACHINE_MOVING_SPEED, //CLAN_TECH_COLLECTOR_ADD_MOVE_SPEED = 6,
    -1, //CLAN_TECH_COLLECTOR = 7,
    -1, //CLAN_TECH_MAKING = 8,
    -1, //CLAN_TECH_SELL = 9,
    CLAN_MACHINE_MAX_HP, //CLAN_TECH_TANK_ADD_HP = 10,
    CLAN_MACHINE_DEF, //CLAN_TECH_TANK_ADD_DEF = 11,
    CLAN_MACHINE_ATK, //CLAN_TECH_TANK_ADD_ATK = 12,
    -1, //CLAN_TECH_PET_ADD_LEVEL_LIMIT = 13,
    -1, //CLAN_TECH_PET_ADD_HP = 14,
    -1, //CLAN_TECH_PET_ADD_DEF = 15,
    -1, //CLAN_TECH_PET_ADD_ATK = 16,
};




// Ϊ��������������Դ
static int clan_cost_for_making(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const CLANPRODUCTDEF *pstDef, int iNum);

// �ӹ������������Դ
static int clan_making_rollback(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, 
                                const CLANPRODUCTDEF *pstDef,
                                const CLANRUNNINGPROGRESS *pstProgress);

int clan_resource_add_entries_in(ZONESVRENV *pstEnv, CLANRESOURCE *pstResource,
                                      int iID, int iNum,CLANRESOURCEINFO* pstNotify,int* piRealNum,ZoneClan* pstClan);

int clan_resource_dec_entries_in(ZONESVRENV *pstEnv, CLANRESOURCE *pstResource,
                                      int iID, int iNum,CLANRESOURCEINFO* pstNotify);
static int player_get_res_info_cmp(const void *p1, const void *p2);



// ��ݵĻ�ȡһ�������Ʒ�ڹ���ֿ��е����ѵ����Ƕ���
static inline int clan_product_max_pile(const CLANPRODUCTDEF *pstDef)
{
	return (pstDef->Category == CLAN_PRODUCT_ITEM)
	     ? (pstDef->MaxPile <= 1 ? 1 : pstDef->MaxPile)
	     : 1;
}

/*
// ��ȡ����ֿ�Dummy,����һЩ����
static int clan_storage_dummy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iWhich, ClanStorageDummy *pstDummy);

// ����ɫ����ֿ���ӵ�Ȩ��
static int clan_storage_grid_permission(ZONESVRENV *pstEnv, int iToken,
	                                    const CLANSTORAGEGRID *pstGrid);

static inline int clan_storage_check_grid_permission(ZONESVRENV *pstEnv,
                                                     ZoneClan *pstZoneClan,
                                                     Player *pstPlayer,
	                                             const CLANSTORAGEGRID *pstGrid)
{
	int iToken = player_clan_position(pstEnv, pstPlayer);
	UNUSED(pstZoneClan);
	if (clan_storage_grid_permission(pstEnv, iToken, pstGrid) < 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��û��Ȩ�޲������������Ʒ");
		return -1;
	}
	return 0;
}
*/
static int clan_resource_grid_permission(ZONESVRENV *pstEnv, int iToken,
	                                    const CLANRESOURCEINFO *pstInfo)
{
	UNUSED(pstEnv);

	if (iToken == CLAN_POS_PRESIDENT)
	{
		return 0;
	}

	switch(pstInfo->State)
	{
	case CLAN_GRID_LIMITED:
		if (iToken <= CLAN_POS_NORMAL)
		{
			return -1;
		}
		break;

	case CLAN_GRID_LOCKED:
		return -1;
		break;

	case CLAN_GRID_OPEN:
		break;

	default:
		return -1;
		break;
	}
	return 0;
}


static inline int clan_resource_check_grid_permission(ZONESVRENV *pstEnv,
                                                     ZoneClan *pstZoneClan,
                                                     Player *pstPlayer,
	                                             const CLANRESOURCEINFO* pstInfo)
{
	int iToken = player_clan_position(pstEnv, pstPlayer);
	
	
	if (clan_resource_grid_permission(pstEnv, iToken, pstInfo) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN64);
		
		return -1;
	}
	return 0;
	UNUSED(pstZoneClan);	//add by paraunused.pl
}


// ��ȡ�������Dummy,����һЩ����
static int clan_progress_dummy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               int iWhich, ClanProgressDummy *pstDummy);

/*
// �������еĲ�Ʒ��ӵ��ֿ���, Ϊ��Ԥ��ֿ��Ƿ��ö�������
static int clan_storage_fake_add_produce(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	const ClanProgressDummy *pstDummy,
	ClanStorageDummy *pstFake,
	int iCategory);

// ���ֿ������Ƿ��㹻���½�Ҫ����Ķ���
static int clan_storage_forecasted_check_capacity(ZONESVRENV *pstEnv,
                                                  ZoneClan *pstZoneClan,
                                                  const CLANPRODUCTDEF *pstDef,
                                                  int iNum);

// ��ʼ��һ�����Ӳ�����Ʒ��ȥ
static void clan_storage_init_item_grid(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        const CLANPRODUCTDEF *pstDef, int iNum,
                                        int iPos, CLANSTORAGEGRID *pstGrid);

// ��ʼ��һ�����Ӳ���һ����е��ȥ
static int clan_storage_init_machine_grid(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	const ClanMachineGridInitialParam *pstParam,
	CLANSTORAGEGRID *pstGrid);


// ����Ƿ��ܹ���ָ���Ĳֿ���ɾ������
static int clan_storage_check_deleting(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       const ClanStorageDummy *pstDummy,
                                       const CLANPRODUCTDEF *pstDef, int iNum);
                                       
// �ӹ���ֿ���ɾ������
static int clan_storage_do_del_entries(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       ClanStorageDummy *pstDummy,
                                       const CLANPRODUCTDEF *pstDef, int iNum,
                                       int fFake);

// ����Ƿ��ܹ���ָ���Ĳֿ������Ӷ���
static int clan_storage_check_adding(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                     const ClanStorageDummy *pstDummy,
                                     const CLANPRODUCTDEF *pstDef, int iNum);

// ��Ӷ���������ֿ���
static int clan_storage_do_add_entries(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       ClanStorageDummy *pstDummy,
                                       const CLANPRODUCTDEF *pstDef, int iNum,
                                       int fFake);

// ɾ��һ������,����һЩ���´���
static void clan_storage_do_grid_delete(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        ClanStorageDummy *pstDummy, int iPos);


// �ӹ���ֿ���ɾ��ָ��ResultID�Ķ���, ������Ч����ֵ
static int clan_storage_del_by_result(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      ClanStorageDummy *pstDummy, int iToken,
                                      int iID, int iResultID, int iNeedValue,
                                      int *piReserved,
                                      int *piPremDeny);


// �ֿ���ӱ仯֪ͨ
static int clan_storage_grid_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    const CLANSTORAGEGRID *pstGrid, int iWhat,
                                    int iWhich);

// ����ĳ����ƷӦ�÷ŵ��ĸ��ֿ�
static inline int clan_storage_should_in(int iProductCategory)
{
	switch(iProductCategory)
	{
	case CLAN_PRODUCT_ITEM:
		return CLAN_STORAGE_RESOURCE;
		break;

	case CLAN_PRODUCT_MACHINE:
		return CLAN_STORAGE_MACHINE;
		break;
	}

	return -1;
};

// ��ʼ����е�������õĲ���
static inline int clan_mach_init_param(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       const CLANPRODUCTDEF *pstDef,
                                       ClanMachineGridInitialParam *pstParam)
{
	Monster stKey = { .iDefPos = -1, .iDefIdx = pstDef->ID, };
	pstParam->pstDef = pstDef;
	pstParam->pstMonDef = z_get_mon_def(pstEnv, &stKey);
	pstParam->pstMacDef = z_find_mon_machine_def(pstEnv, pstDef->ID);
	pstParam->pstAdded = clan_machine_cached_effect(pstEnv, pstZoneClan,
	                                                pstDef->ID);
	return (!pstParam->pstMonDef || !pstParam->pstMacDef) ? -1 : 0;
}

// ��ȡָ���Ĺ���ֿ����
static CLANSTORAGEGRID* clan_identifier_grid(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	const CSCLANSTORAGEGRIDIDENTIFIER *pstIdentifier);

// ��ȡ��������Ч�ĸ���
static inline CLANSTORAGEGRID* clan_freed_valid_grid(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	const CSCLANSTORAGEGRIDIDENTIFIER *pstIdentifier)
{
	CLANSTORAGEGRID *pstGrid = clan_identifier_grid(pstEnv, pstZoneClan,
	                                                pstPlayer, pstIdentifier);
	if (pstGrid && pstGrid->Extension.AsMachineGrid.Using)
	{
		COND_DO_(pstPlayer,
		         z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		                        SYS_MIDDLE,  "��е����ʹ����, �޷���������"));
		return NULL;
	}
	return pstGrid;
}

// �ڻ�е�ֿ��л�ȡ���еĻ�е���ڵĸ���
static inline CLANSTORAGEGRID* clan_freed_machine_grid(ZONESVRENV *pstEnv,
                                                       ZoneClan *pstZoneClan,
                                                       Player *pstPlayer,
                                                       int iPos)
{
	CSCLANSTORAGEGRIDIDENTIFIER stIdentifier = {
		.Category = CLAN_STORAGE_MACHINE,
		.GridPos = iPos,
	};
	return clan_freed_valid_grid(pstEnv, pstZoneClan, pstPlayer, &stIdentifier);
};

// �ݻ�һ������ֿ�����е���Ʒ/��е
static int clan_storage_grid_destroy(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	CLANSTORAGEGRID *pstGrid,
	const CSCLANSTORAGEGRIDIDENTIFIER *pstIdentifier,char *pszRoleName);

// ����һ������ֿ���λ�л�е, �����޺�
static int clan_storage_repair(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               int iMoneyCost, int iMaxHP, int iNeedHP,
                               CLANSTORAGEGRID *pstGrid);

// ���ҹ����Ʒ��ĳ��Result
static const CLANGENERALRESULT* clan_product_result(ZONESVRENV *pstEnv,
                                                    int iDefID,
                                                    int iResultID);

// Ϊ�ֿ��еĻ�е��������
static int clan_machine_supply_fuel(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   Player *pstPlayer,
                                   const MONMACHINEDEF *pstMacDef,
                                   CLANSTORAGEGRID *pstGrid);

// Ϊ�ֿ��еĻ�е����
static int clan_machine_repair(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               Player *pstPlayer,
                               const MONMACHINEDEF *pstMacDef,
                               CLANSTORAGEGRID *pstGrid);


// ����ʹ���еĻ�е�ڹ���ֿ��еĸ�������
static inline int clan_machine_using_index_grid(ZONESVRENV *pstEnv,
                                                ZoneClan *pstZoneClan,
                                                const Monster *pstMon)
{
	int i = pstZoneClan->stClan.ClanAdvance.Storage.MachineNum;
	UNUSED(pstEnv);
	while(i--)
	{
		if (pstZoneClan->stZoneClanInfo.iMachineIDMap[i] == pstMon->iMIndex)
		{
			return i;
		}
	}
	return -1;
}

// ����ʹ���еĻ�е�ڹ���ֿ��еĸ���
static inline CLANSTORAGEGRID* clan_machine_using_grid(ZONESVRENV *pstEnv,
                                                       ZoneClan *pstZoneClan,
                                                       const Monster *pstMon)
{
	CSCLANSTORAGEGRIDIDENTIFIER stIdentifier = {
		.Category = CLAN_STORAGE_MACHINE,
		.GridPos = clan_machine_using_index_grid(pstEnv, pstZoneClan, pstMon),
	};
	return clan_identifier_grid(pstEnv, pstZoneClan, NULL, &stIdentifier);
}

// �Ͻ������е�ϵĿ�ʯ������ֿ�
static int clan_machine_commit_ore(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                  Player *pstPlayer, ROLEGRID *pastRoleGrid, int iNum);

// ���¹����е�ĸ�������(�����������ڹ���Ƽ���Ӱ��)
static int clan_machine_update_properties(ZONESVRENV *pstEnv,
                                          ZoneClan *pstZoneClan,
                                          const CLANMACHINEEXTENSION *pstExt,
                                          Monster *pstMachine);
*/
// ���Ҷ�ӦMachineID��������������, ����Ҳ������½�һ��
static ClanMachinePropertyEntry* 
clan_find_or_cached_effect(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID);

// ���ٻ����ȡ�����е/���������
static inline ClanMachinePropertyEntry* clan_fast_cached_effect(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	ClanMachinePropertyEntry *pstCachedProperty,
	int iID)
{
	static ClanMachinePropertyEntry stDummy = INIT_CLAN_MACHINE_PROPERTY_ENTRY();
	if (pstCachedProperty && pstCachedProperty->iID == iID)
	{
		return pstCachedProperty;
	}
	pstCachedProperty = clan_find_or_cached_effect(pstEnv, pstZoneClan, iID);
	return !pstCachedProperty ? &stDummy : pstCachedProperty;
}

// ���ݿƼ�ˢ�¹����е/��������Ի���
static int clan_effect_cache_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// ���ݿƼ����¹����е/��������Ի���
static int clan_effect_cache_update(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    const CLANTECHDEF *pstDef);

/*
// ���ҹ���ֿ���Ӷ�Ӧ��ʹ���еĻ�е
static inline Monster* clan_storage_mapped_machine(ZONESVRENV *pstEnv,
                                                   ZoneClan *pstZoneClan,
                                                   int iIndex)
{
	int iMemID = pstZoneClan->stZoneClanInfo.iMachineIDMap[iIndex];
	return tmempool_get(pstEnv->pstMonsterPool, iMemID);
};

// ��ɫ����һ�������е
static int clan_callback_machine(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 Player *pstPlayer, CLANSTORAGEGRID *pstGrid);
*/

// ��ȡʵ�ʵ���������
static int clan_checked_making_num(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   const CSCLANMAKE *pstReq);
/*
// ����һ�������е
static Monster* clan_machine_spawn(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   const CLANSTORAGEGRID *pstGrid,
                                   ZONEMAPINST *pstMapInst, RESPOS *pstPos);

// ��ɫ��ȡһ�������е
static int clan_machine_grab(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             Player *pstPlayer, CLANSTORAGEGRID *pstGrid);
*/                         

// ����Ƿ�ɽ�������
static int clan_check_making(ZONESVRENV *pstEnv, Player *pstPlayer,
                             ZoneClan *pstZoneClan,
                             const ClanProgressDummy *pstDummy,
                             const CLANPRODUCTDEF *pstDef, int iNum);

// ����Ƿ�ɽ���ĳ���з�
static int clan_check_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                               ZoneClan *pstZoneClan,
                               const ClanProgressDummy *pstDummy,
                               const CLANTECHDEF *pstDef);

// �����������
static int clan_making_process(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               ClanProgressDummy *pstDummy);

// �����з�����
static int clan_research_process(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 ClanProgressDummy *pstDummy);

// ��֪ĳ���Ƽ����ĸ�������ص�
static int clan_tech2building(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iTechID);

// �����ύ������Ʒ�����ݽṹ�뺯��
typedef struct _ClanCommittedItem
{
	int iID;
	int iNum;
	const char *pszName;
	const RESULT *pstResult;
}ClanCommittedItem;

typedef struct _ClanCommittedList
{
	ClanCommittedItem astItem[MAX_SUNDRIES_PACKAGE];
	int iNum;
}ClanCommittedList;

static inline ClanCommittedItem *clit_find_or_add(ClanCommittedList *pstList,
                                                  int iID)
{
	int i;
	for (i = 0; i < pstList->iNum; ++i)
	{
		if (pstList->astItem[i].iID == iID)
		{
			return pstList->astItem + i;
		}
	}
	return pstList->astItem + pstList->iNum++;
}

// Define:
const CLANTECHDEF* clan_tech_def(ZONESVRENV *pstEnv, int iCategory, int iLevel)
{
	CLANTECHDEF stKey = {
		.Category = (short)iCategory,
		.Level = (short)iLevel
	};
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	iIndex = bsearch_int(&stKey, pstObj->sClanTechDef, pstObj->iClanTechDef, 
		                 CLAN_TECH_DEF_DATA, &iEqu);
	return iEqu ? (CLANTECHDEF *)pstObj->sClanTechDef[iIndex] : NULL;
}

CLANPRODUCTDEF* clan_product_def(ZONESVRENV *pstEnv, int iID)
{
	CLANPRODUCTDEF stKey = { .ID = iID, };
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	iIndex = bsearch_int(&stKey, pstObj->sClanProductDef,
	                     pstObj->iClanProductDef, CLAN_PRODUCT_DEF_DATA, &iEqu);
	return iEqu ? (CLANPRODUCTDEF *)pstObj->sClanProductDef[iIndex] : NULL;
}

int clan_running_progress_notify(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        const CLANRUNNINGPROGRESS *pstProgress,
                                        int iNotify,
                                        int iCode)
{
	CSPKG stPkg;
	CSCLANPROGRESSNOTIFY *pstData = NULL;
	const CLANRUNNINGPROGRESS *pstBase = NULL;

	switch(iNotify)
	{
	case CLAN_SVR_RESEARCH_NOTIFY:
		pstBase = pstZoneClan->stClan.ClanAdvance.Tech.TechResearchProgress;
		pstData = &stPkg.Body.ClanSvr.ClanSvrData.ClanResearchNotify;
		break;

	case CLAN_SVR_MAKING_NOTIFY:
		switch(pstProgress->Category)
		{
		case CLAN_MAKE_FOR_ARM:
			pstBase = pstZoneClan->stClan.ClanAdvance.Produce.ArmMakingProgress;
			break;

		case CLAN_MAKE_FOR_COLLECTION:
			pstBase =
			   pstZoneClan->stClan.ClanAdvance.Produce.CollectionMakingProgress;
			break;

		default:
			return -1;
			break;
		}
		pstData = &stPkg.Body.ClanSvr.ClanSvrData.ClanMakingNotify;
		break;

	default:
		return -1;
		break;
	}
	if(!pstData)
	{
		return -1;
	}
	
	stPkg.Body.ClanSvr.Operation = iNotify;
	pstData->What = iCode;
	switch(iCode)
	{
	case CLAN_PROGRESS_STARTED:
		pstData->Data.Started = *pstProgress;
		break;

	case CLAN_PROGRESS_FINISHED:
		pstData->Data.Finished.Category = pstProgress->Category;
		pstData->Data.Finished.Pos = pstProgress - pstBase;
		break;

	case CLAN_PROGRESS_CANCELED:
		pstData->Data.Canceled.Category = pstProgress->Category;
		pstData->Data.Canceled.Pos = pstProgress - pstBase;
		break;

	case CLAN_PROGRESS_SHIFTED:
		pstData->Data.Shifted.Identifier.Category = pstProgress->Category;
		pstData->Data.Shifted.Identifier.Pos = pstProgress - pstBase;
		pstData->Data.Shifted.BeginTime = pstProgress->BeginTime;
		pstData->Data.Shifted.EndTime = pstProgress->EndTime;
		break;

	default:
		return -1;
		break;
	}

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

/*
static int clan_storage_grid_notify(ZONESVRENV *pstEnv,
                                    ZoneClan *pstZoneClan,
                                    const CLANSTORAGEGRID *pstGrid,
                                    int iWhat,
                                    int iWhich)
{
	CSPKG stPkg;
	CSCLANSTORAGEGRIDNOTIFY *pstData =
		&stPkg.Body.ClanSvr.ClanSvrData.ClanStorageGridNotify;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();

	if(clan_storage_dummy(pstEnv, pstZoneClan, iWhich, &stDummy) < 0)
	{
		return -1;
	}

	pstData->Which = iWhich;
	pstData->What = iWhat;
	switch(iWhat)
	{
	case CLAN_STORAGE_GRID_ADD:
		pstData->Data.Added = *pstGrid;
		break;

	case CLAN_STORAGE_GRID_DEL:
		pstData->Data.Deleted = pstGrid - stDummy.pstGrid;
		break;

	case CLAN_STORAGE_GRID_MOD:
		pstData->Data.Modified = *pstGrid;
		break;

	default:
		return -1;
		break;
	}

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_STORAGE_GRID_NOTIFY;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}
*/
static int clan_cost_for_making(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const CLANPRODUCTDEF *pstDef, int iNum)
{
	int i = 0;

	g_iOptype = OP_OPTYPE_MAKE;
	g_iOpsubtype = pstDef->ID;
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
	                     pstDef->NeedMoneyCost * iNum, 1);

	for(i = 0; i < (int)CCH(pstDef->NeedMaterialCost); ++i)
	{
		if(!pstDef->NeedMaterialCost[i].ID)
		{
			break;
		}

		clan_resource_dec_entries(pstEnv, pstZoneClan,
		                         pstDef->NeedMaterialCost[i].ID,
		                         pstDef->NeedMaterialCost[i].Num * iNum);
	}
	g_iOptype = 0;
	g_iOpsubtype = 0;
	
	return 0;
}

static int clan_making_rollback(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, 
                                const CLANPRODUCTDEF *pstDef,
                                const CLANRUNNINGPROGRESS *pstProgress)
{
	int i = 0;

	g_iOptype = OP_OPTYPE_ROLLBACK;
	g_iOpsubtype = pstDef->ID;
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
	                     pstDef->NeedMoneyCost * pstProgress->Value, 0);

	for(i = 0; i < (int)CCH(pstDef->NeedMaterialCost); ++i)
	{
		if(!pstDef->NeedMaterialCost[i].ID)
		{
			break;
		}

		clan_resource_add_entries(pstEnv, pstZoneClan, 
		                         pstDef->NeedMaterialCost[i].ID,
		                         pstDef->NeedMaterialCost[i].Num * pstProgress->Value,NULL);
	}

	g_iOptype = 0;
	g_iOpsubtype = 0;

	clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
	return 0;
}
/*
static int clan_storage_grid_permission(ZONESVRENV *pstEnv, int iToken,
	                                    const CLANSTORAGEGRID *pstGrid)
{
	UNUSED(pstEnv);

	if (iToken == CLAN_POS_PRESIDENT)
	{
		return 0;
	}

	switch(pstGrid->State)
	{
	case CLAN_GRID_LIMITED:
		if (iToken <= CLAN_POS_NORMAL)
		{
			return -1;
		}
		break;

	case CLAN_GRID_LOCKED:
		return -1;
		break;

	case CLAN_GRID_OPEN:
		break;

	default:
		return -1;
		break;
	}
	return 0;
}
*/
static int clan_progress_dummy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               int iWhich, ClanProgressDummy *pstDummy)
{
	static int iFake = 0;
	CLANPRODUCE *pstProduce = &pstZoneClan->stClan.ClanAdvance.Produce;
	CLANTECH *pstTech = &pstZoneClan->stClan.ClanAdvance.Tech;

	UNUSED(pstEnv);

	switch(iWhich)
	{
	case CLAN_RUNNING_RESEARCH:
		pstDummy->pstProgress = pstTech->TechResearchProgress;
		pstDummy->iMax = CCH(pstTech->TechResearchProgress);
		pstDummy->piNum = &pstTech->TechResearchNum;
		break;

	case CLAN_RUNNING_ARM_MAKING:
		pstDummy->pstProgress = pstProduce->ArmMakingProgress;
		pstDummy->iMax = pstProduce->MaxArmMakingNum;
		pstDummy->piNum = &pstProduce->ArmMakingNum;
		break;

	case CLAN_RUNNING_COLLECTION_MAKING:
		pstDummy->pstProgress = pstProduce->CollectionMakingProgress;
		pstDummy->iMax = pstProduce->MaxCollectionMakingNum;
		pstDummy->piNum = &pstProduce->CollectionMakingNum;
		break;

	default:
		iFake = 0;
		pstDummy->piNum = &iFake;
		return -1;
		break;
	}
	pstDummy->iWhich = iWhich;
	return 0;
}

/*
static int clan_storage_dummy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iWhich, ClanStorageDummy *pstDummy)
{
	static int iFake = 0;
	CLANSTORAGE *pstStorage = &pstZoneClan->stClan.ClanAdvance.Storage;

	UNUSED(pstEnv);

	switch(iWhich)
	{
	case CLAN_STORAGE_MACHINE:
		pstDummy->pstGrid = pstStorage->MachineGrid;
		pstDummy->iMax = pstStorage->MaxMachineNum;
		pstDummy->piNum = &pstStorage->MachineNum;
		break;

	case CLAN_STORAGE_RESOURCE:
		pstDummy->pstGrid = pstStorage->ResourceGrid;
		pstDummy->iMax = pstStorage->MaxResourceNum;
		pstDummy->piNum = &pstStorage->ResourceNum;
		break;

	default:
		iFake = 0; // Ϊ��ֹ��ָ������, Ū����ֵ
		pstDummy->piNum = &iFake;
		return -1;
		break;
	}
	pstDummy->iWhich = iWhich;
	return 0;
}

static void clan_storage_init_item_grid(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        const CLANPRODUCTDEF *pstDef, int iNum,
                                        int iPos, CLANSTORAGEGRID *pstGrid)
{
	UNUSED(pstEnv);
	UNUSED(pstZoneClan);

	pstGrid->Category = CLAN_GRID_ITEM;
	pstGrid->ID = pstDef->ID;
	pstGrid->State = CLAN_GRID_LIMITED;
	pstGrid->Pos = iPos;
	pstGrid->Extension.AsItemGrid.Num = iNum;
}

static int clan_storage_init_machine_grid(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	const ClanMachineGridInitialParam *pstParam,
	CLANSTORAGEGRID *pstGrid)
{
	UNUSED(pstEnv);
	UNUSED(pstZoneClan);

	if (pstParam->iPos < 0)
	{
		return -1;
	}

	pstGrid->Category = CLAN_GRID_MACHINE;
	pstGrid->ID = pstParam->pstDef->ID;
	pstGrid->Pos = pstParam->iPos;
	pstGrid->State = CLAN_GRID_LIMITED;
	pstGrid->Extension.AsMachineGrid.Using = 0;
	pstGrid->Extension.AsMachineGrid.Fuel = pstParam->pstMacDef->MaxEnergy;
	pstGrid->Extension.AsMachineGrid.HP = 
		pstParam->pstMonDef->HP +
		pstParam->pstAdded->aiAdded[CLAN_MACHINE_MAX_HP];
	return 0;
}

static int clan_storage_check_adding(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                     const ClanStorageDummy *pstDummy,
                                     const CLANPRODUCTDEF *pstDef, int iNum)
{
	int i = 0;
	const int kMaxPile = clan_product_max_pile(pstDef);

	UNUSED(pstEnv);
	UNUSED(pstZoneClan);

	if (kMaxPile > 1)
	{
		for(i = 0; i < *pstDummy->piNum; ++i)
		{
			if (pstDummy->pstGrid[i].ID == pstDef->ID)
			{
				iNum -= (kMaxPile -
				        pstDummy->pstGrid[i].Extension.AsItemGrid.Num);
				if (iNum <= 0)
				{
					break;
				}
			}
		}
	}

	if (iNum > 0)
	{
		int iStack = (iNum + (kMaxPile - 1)) / kMaxPile;
		if (*pstDummy->piNum + iStack > pstDummy->iMax)
		{
			return -1;
		}
	}
	return 0;
}

static int clan_storage_extenal_add_entries(ZONESVRENV *pstEnv,
                                            ZoneClan *pstZoneClan,
                                            ClanStorageDummy *pstDummy,
                                            const CLANPRODUCTDEF *pstDef,
			                                int iNum, int fFake)
{
	const int kMaxPile = clan_product_max_pile(pstDef);
	int i = 0, iStack = (iNum + (kMaxPile - 1)) / kMaxPile;

	switch(pstDef->Category)
	{
	case CLAN_PRODUCT_ITEM:
		{
			for(i = *pstDummy->piNum; i < *pstDummy->piNum + iStack; ++i)
			{
				int iAdded = min(iNum, kMaxPile);
				clan_storage_init_item_grid(pstEnv, pstZoneClan, pstDef, iAdded,
						                    i, pstDummy->pstGrid + i);
				if (!fFake)
				{
					clan_storage_grid_notify(pstEnv, pstZoneClan,
					                         pstDummy->pstGrid + i,
					                         CLAN_STORAGE_GRID_ADD,
					                         pstDummy->iWhich);
				}
				iNum -= iAdded;
			}
		}
		break;

	case CLAN_PRODUCT_MACHINE:
		{
			ClanMachineGridInitialParam stParam = 
				INIT_CLAN_MACHINE_GRID_INITIAL_PARAM();

			if (clan_mach_init_param(pstEnv, pstZoneClan, pstDef, &stParam) < 0)
			{
				return -1;
			}
			for(i = *pstDummy->piNum; i < *pstDummy->piNum + iStack; ++i)
			{
				stParam.iPos = i;
				if (clan_storage_init_machine_grid(pstEnv, pstZoneClan,
					                               &stParam,
				                                   pstDummy->pstGrid + i) < 0)
				{
					return -1;
				}
				if (!fFake)
				{
					clan_storage_grid_notify(pstEnv, pstZoneClan,
					                         pstDummy->pstGrid + i,
					                         CLAN_STORAGE_GRID_ADD,
					                         pstDummy->iWhich);
				}
			}
		}
		break;

	default:
		return -1;
		break;
	}

	*pstDummy->piNum += iStack;
	if (!fFake)
	{
		clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
	}
	return 0;
}

static int clan_storage_do_add_entries(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       ClanStorageDummy *pstDummy,
                                       const CLANPRODUCTDEF *pstDef, int iNum,
                                       int fFake)
{
	int i = 0;
	const int kMaxPile = clan_product_max_pile(pstDef);

	if (kMaxPile > 1)
	{
		for(i = 0; i < *pstDummy->piNum && iNum > 0; ++i)
		{
			CLANSTORAGEGRID *pstGrid = pstDummy->pstGrid + i;

			if (pstGrid->ID == pstDef->ID &&
				pstGrid->Extension.AsItemGrid.Num < kMaxPile)
			{
				int iAdded = min((kMaxPile - pstGrid->Extension.AsItemGrid.Num),
				                 iNum);

				pstGrid->Extension.AsItemGrid.Num += iAdded;
				if(!fFake)
				{
					clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
					                         CLAN_STORAGE_GRID_MOD,
					                         pstDummy->iWhich);
					clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
				}

				iNum -= iAdded;
			}
		}
	}

	if (iNum > 0)
	{
		if (clan_storage_extenal_add_entries(pstEnv, pstZoneClan, pstDummy,
		                                     pstDef, iNum, fFake) < 0)
		{
			return -1;
		}
	}
	return 0;
}

static int clan_storage_check_deleting(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       const ClanStorageDummy *pstDummy,
                                       const CLANPRODUCTDEF *pstDef, int iNum)
{
	int i = *pstDummy->piNum;
	const int kMaxPile = clan_product_max_pile(pstDef);

	UNUSED(pstEnv);
	UNUSED(pstZoneClan);

	while(i--)
	{
		if (pstDummy->pstGrid[i].ID == pstDef->ID)
		{
			if (kMaxPile == 1)
			{
				--iNum;
			}
			else
			{
				iNum -= (min(pstDummy->pstGrid[i].Extension.AsItemGrid.Num,
				             iNum));
			}

			if (!iNum)
			{
				break;
			}
		}
	}

	return !iNum ? 0 : -1;
}

static void clan_storage_do_grid_delete(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        ClanStorageDummy *pstDummy, int iPos)
{
	const int kRemoved = (*pstDummy->piNum) - iPos - 1;
	int i = kRemoved;
	CLANSTORAGEGRID *pstIter = pstDummy->pstGrid + iPos;

	UNUSED(pstEnv);

	// ɾ�������ٸ���Pos�ֶ�
	memmove(pstIter, pstIter + 1, sizeof(*pstIter) * kRemoved);
	while(i--)
	{
		--(pstIter->Pos);
		++pstIter;
	}

	// ����ǻ�е�ֿ�, ��Ҫ����һ��ӳ��!��
	if (pstDummy->iWhich == CLAN_STORAGE_MACHINE)
	{
		memmove(pstZoneClan->stZoneClanInfo.iMachineIDMap + iPos,
		        pstZoneClan->stZoneClanInfo.iMachineIDMap + iPos + 1,
		        sizeof(pstZoneClan->stZoneClanInfo.iMachineIDMap[0]) * kRemoved);
	}
	--(*pstDummy->piNum);
}

static int clan_storage_do_del_entries(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan,
                                       ClanStorageDummy *pstDummy,
                                       const CLANPRODUCTDEF *pstDef, int iNum,
                                       int fFake)
{
	int i = *pstDummy->piNum;
	const int kMaxPile = clan_product_max_pile(pstDef);

		 

	while(i-- && iNum > 0)
	{
		int fShouldErase = 0;
		CLANSTORAGEGRID *pstGrid = pstDummy->pstGrid + i;

		if (pstGrid->ID != pstDef->ID)
		{
			continue;
		}

		if (kMaxPile == 1)
		{
			fShouldErase = 1;
			--iNum;
		}
		else
		{
			int iDeleted = min(pstGrid->Extension.AsItemGrid.Num, iNum);

			pstGrid->Extension.AsItemGrid.Num -= iDeleted;
			if (pstGrid->Extension.AsItemGrid.Num > 0)
			{
				if (!fFake)
				{
					clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
					                         CLAN_STORAGE_GRID_MOD,
					                         pstDummy->iWhich);
					clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
				}
			}
			fShouldErase = (pstGrid->Extension.AsItemGrid.Num == 0) ? 1 : 0;
			iNum -= iDeleted;
		}

		if (fShouldErase)
		{
			if (!fFake)
			{
				clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
				                         CLAN_STORAGE_GRID_DEL,
				                         pstDummy->iWhich);
				clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
			}
			clan_storage_do_grid_delete(pstEnv, pstZoneClan, pstDummy, i);
		}
	}

	return !iNum ? 0 : -1;
}

static int clan_storage_del_by_result(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	ClanStorageDummy *pstDummy,
	int iToken,
	int iID,
	int iResultID,
	int iNeedValue,
	int *piReserved,
	int *piPremDeny)
{
	int i = *pstDummy->piNum, iCount = 0;
	*piPremDeny = 0;
	while(i-- && iNeedValue > 0)
	{
		const CLANGENERALRESULT* pstResult = NULL;
		CLANSTORAGEGRID *pstIter = pstDummy->pstGrid + i;

		if (iID > 0 && pstIter->ID != iID)
		{
			continue;
		}

		pstResult = clan_product_result(pstEnv, pstIter->ID, iResultID);
		if (pstResult->Value1 > 0)
		{
			int iNeedNum = 0;
			int iRealNum = 0;
			int iRealValue  = 0;
			
			if (clan_storage_grid_permission(pstEnv, iToken, pstIter) < 0)
			{
				*piPremDeny = 1;
				continue; // Ȩ�޼��
			}
		
			// ������
			iNeedNum = (iNeedValue + pstResult->Value1 - 1) /
			               pstResult->Value1;
			// ��ʵ��
			iRealNum = min(iNeedNum, pstIter->Extension.AsItemGrid.Num);
			// Ч��ֵ
			iRealValue = min(iNeedValue, iRealNum * pstResult->Value1);

			*piReserved += iRealValue;
			iNeedValue -= iRealValue;
			pstIter->Extension.AsItemGrid.Num -= iRealNum;
			iCount += iRealNum;

			if (!pstIter->Extension.AsItemGrid.Num)
			{
				clan_storage_grid_notify(pstEnv, pstZoneClan, pstIter,
                                         CLAN_STORAGE_GRID_DEL,
                                         pstDummy->iWhich);
				clan_storage_do_grid_delete(pstEnv, pstZoneClan, pstDummy, i);
			}
			else
			{
				clan_storage_grid_notify(pstEnv, pstZoneClan, pstIter,
                                         CLAN_STORAGE_GRID_MOD,
                                         pstDummy->iWhich);
			}
		}
	}
	return iCount;
}

static int clan_storage_fake_add_produce(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	const ClanProgressDummy *pstDummy,
	ClanStorageDummy *pstFake,
	int iCategory)
{
	int i = 0;
	for(i = 0; i < *pstDummy->piNum; ++i)
	{
		const CLANRUNNINGPROGRESS *pstProgress = pstDummy->pstProgress + i;
		const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv,
		                                                pstProgress->ID);
		if (!pstDef)
		{
			return -1;
		}
		if (pstDef->Category != iCategory)
		{
			continue;
		}

		if (clan_storage_check_adding(pstEnv, pstZoneClan, pstFake, pstDef,
			                          pstProgress->Value) < 0 ||
			clan_storage_do_add_entries(pstEnv, pstZoneClan, pstFake, pstDef,
			                            pstProgress->Value, 1) < 0)
		{
			return -1;
		}
	}
	return 0;
}

static int clan_storage_forecasted_check_capacity(ZONESVRENV *pstEnv,
                                                  ZoneClan *pstZoneClan,
                                                  const CLANPRODUCTDEF *pstDef,
                                                  int iNum)
{
	int iDummyNum = 0;
	CLANSTORAGEGRID astDummyGrids[max(MAX_CLAN_MACHINE_GRID_NUM,
	                                  MAX_CLAN_RESOURCE_GRID_NUM)];
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	ClanStorageDummy stDoll = INIT_CLAN_STORAGE_DUMMY();
	ClanProgressDummy stProgress = INIT_CLAN_PROGRESS_DUMMY();

	switch(pstDef->Category)
	{
	case CLAN_PRODUCT_ITEM:
		clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_RESOURCE,
		                   &stDummy);
		break;

	case CLAN_PRODUCT_MACHINE:
		clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_MACHINE,
		                   &stDummy);
		break;

	default:
		return -1;
		break;
	}

	// �����Ŀ���һ���ֿ����
	stDoll = stDummy;
	stDoll.pstGrid = astDummyGrids;
	stDoll.piNum = &iDummyNum;
	memcpy(stDoll.pstGrid, stDummy.pstGrid,
	       sizeof(stDoll.pstGrid[0]) * (*stDummy.piNum));
	*stDoll.piNum = *stDummy.piNum;

	// ���Խ������е���Ʒ�ӵ���������Ĳֿ���,�Բ��Բֿ�Ŀ�λ�Ƿ��㹻.
	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_ARM_MAKING,
	                    &stProgress);
	if (clan_storage_fake_add_produce(pstEnv, pstZoneClan, &stProgress,
		                              &stDoll, pstDef->Category) < 0)
	{
		return -1;
	}
	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_COLLECTION_MAKING,
	                    &stProgress);
	if (clan_storage_fake_add_produce(pstEnv, pstZoneClan, &stProgress,
		                              &stDoll, pstDef->Category) < 0)
	{
		return -1;
	}

	// �����Խ�Ҫ��ӽ�������Ʒ
	if (clan_storage_check_adding(pstEnv, pstZoneClan, &stDoll, pstDef,
	                              iNum) < 0 ||
		clan_storage_do_add_entries(pstEnv, pstZoneClan, &stDoll, pstDef,
		                            iNum, 1) < 0)
	{
		return -1;
	}
	return 0;
}
*/

static int clan_resource_forecasted_check_capacity(ZONESVRENV *pstEnv,
                                                  ZoneClan *pstZoneClan,
                                                  const CLANPRODUCTDEF *pstDef,
                                                  int iNum)
{
	CLANRESOURCE stFakeResource = pstZoneClan->stClan.ClanAdvance.Resource;//һ���ٵ���Դ����
	ClanProgressDummy stProgress = INIT_CLAN_PROGRESS_DUMMY();
	int i = 0;


	// ���Խ������е���Ʒ�ӵ���������Ĳֿ���,�Բ��Բֿ�Ŀ�λ�Ƿ��㹻.
	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_ARM_MAKING,
	                    &stProgress);

	
	for(i = 0; i < *stProgress.piNum; ++i)
	{
		const CLANRUNNINGPROGRESS *pstProgress = stProgress.pstProgress + i;

		if (clan_resource_add_entries_in(pstEnv, &stFakeResource,pstProgress->ID, pstProgress->Value,NULL,NULL,pstZoneClan)< 0)
		{
			return -1;
		}
	}

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_COLLECTION_MAKING,
	                    &stProgress);
	
	for(i = 0; i < *stProgress.piNum; ++i)
	{
		const CLANRUNNINGPROGRESS *pstProgress = stProgress.pstProgress + i;

		if (clan_resource_add_entries_in(pstEnv, &stFakeResource,pstProgress->ID, pstProgress->Value,NULL,NULL,pstZoneClan)< 0)
		{
			return -1;
		}
	}
	
	// �����Խ�Ҫ��ӽ�������Ʒ
	if (clan_resource_add_entries_in(pstEnv, &stFakeResource,pstDef->ID, iNum,NULL,NULL,pstZoneClan) < 0)
	{
		return -1;
	}
	return 0;
}

/*
static int clan_storage_check_material_for_making(ZONESVRENV *pstEnv,
                                                  ZoneClan *pstZoneClan,
                                                  const CLANPRODUCTDEF *pstDef,
                                                  int iNum)
{
	int i = 0;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();

	for(i = 0; i < (int)CCH(pstDef->NeedMaterialCost); ++i)
	{
		const CLANPRODUCTDEF *pstTestDef = NULL;

		if (!pstDef->NeedMaterialCost[i].ID)
		{
			break;
		}
		
		pstTestDef = clan_product_def(pstEnv, pstDef->NeedMaterialCost[i].ID);
		if (!pstTestDef)
		{
			return -1;
		}

		if (pstTestDef->Category != CLAN_PRODUCT_ITEM)
		{
			return -1;
		}

		clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_RESOURCE, &stDummy);
		if (clan_storage_check_deleting(pstEnv, pstZoneClan, &stDummy,
		                                pstTestDef,
		                                pstDef->NeedMaterialCost[i].Num *
		                                iNum) < 0)
		{
			return -1;
		}
	}
	return 0;
}
*/

static int clan_resource_check_material_for_making(ZONESVRENV *pstEnv,
                                                  ZoneClan *pstZoneClan,
                                                  const CLANPRODUCTDEF *pstDef,
                                                  int iNum,Player* pstPlayer)
{
	int i = 0;
	CLANRESOURCE stResource = pstZoneClan->stClan.ClanAdvance.Resource;

	for(i = 0; i < (int)CCH(pstDef->NeedMaterialCost); ++i)
	{
		const CLANPRODUCTDEF *pstTestDef = NULL;

		if (!pstDef->NeedMaterialCost[i].ID)
		{
			break;
		}
		
		pstTestDef = clan_product_def(pstEnv, pstDef->NeedMaterialCost[i].ID);
		if (!pstTestDef)
		{
			if (pstPlayer)
			{
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "ָ���Ĳ��ϲ�����,�޷�����");
			}
			return -1;
		}

		if (clan_resource_dec_entries_in(pstEnv, &stResource,
		                                pstDef->NeedMaterialCost[i].ID,
		                                pstDef->NeedMaterialCost[i].Num *iNum,
		                                NULL) < 0)
		{
			if (pstPlayer)
			{
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "ָ���Ĳ��� %s ����,�޷�����",pstTestDef->Name);
			}
			return -1;
		}
	}
	return 0;
}

/*

static int clan_storage_repair(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               int iMoneyCost, int iMaxHP, int iNeedHP,
                               CLANSTORAGEGRID *pstGrid)
{
	double dNeedMoney = !iMaxHP ?
	                    0 : ((double)(iNeedHP / (double)iMaxHP) / 2.0f) * iMoneyCost;
	double dRealMoney = min(dNeedMoney, (int)pstZoneClan->stClan.ClanProf.Money);
	double dRealHP = (dRealMoney / iMoneyCost * 2.0f) * iMaxHP;

	pstGrid->Extension.AsMachineGrid.HP += (int)dRealHP;
	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
                             CLAN_STORAGE_GRID_MOD,
                             CLAN_STORAGE_MACHINE);

	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
	                     (int)dRealMoney, 1);
	if (!pstZoneClan->stClan.ClanProf.Money)
	{
		return -1;
	}
	if ((int)dNeedMoney > (int)pstZoneClan->stClan.ClanProf.Money)
	{
		return 1;
	}
	return 0;
}

int clan_storage_add_entries_external(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum)
{
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv, iID);

	if (!pstDef)
	{
		return -1;
	}

	if (clan_storage_dummy(pstEnv, pstZoneClan,
	                       clan_storage_should_in(pstDef->Category),
	                       &stDummy) < 0)
	{
		return -1;
	}

	if (clan_storage_forecasted_check_capacity(pstEnv, pstZoneClan, pstDef,
	                                           iNum) < 0)
	{
		return -1;
	}

	return clan_storage_do_add_entries(pstEnv, pstZoneClan, &stDummy, pstDef,
	                                   iNum, 0);
}


static const RESULT *clan_item_matched(ZONESVRENV *pstEnv, ROLEITEM *pstItem,
                                       int iRealID, const char **ppsName)
{
	int i = 0;
	const ITEMDEF *pstDef = z_get_itemdef_roleitem(pstEnv, pstItem);
	if (!pstDef)
	{
		return NULL;
	}

	for (i = 0; i < (int)CCH(pstDef->Result); ++i)
	{
		switch (pstDef->Result[i].ResultID)
		{
		case RESULT_CLAN_COMMITTED:
			if (ppsName)
			{
				*ppsName = pstDef->Name;
			}
			if (iRealID > 0)
			{
				return iRealID == pstDef->Result[i].ResultVal2
				       ? pstDef->Result + i : NULL;
			}
			else if (iRealID < 0)
			{
				return pstDef->Result[i].ResultVal3
				       ? pstDef->Result + i : NULL;
			}
			return pstDef->Result + i;
			break;

		case 0:
			return NULL;

		default:
			break;
		}
	}
	return NULL;
}


static CLANSTORAGEGRID* clan_identifier_grid(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	const CSCLANSTORAGEGRIDIDENTIFIER *pstIdentifier)
{
	CLANSTORAGEGRID *pstGrid = NULL;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();

	if (clan_storage_dummy(pstEnv, pstZoneClan, pstIdentifier->Category,
	                       &stDummy) < 0)
	{
		COND_DO_(pstPlayer, z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
		                               "��������, �޷���������"));
		return NULL;
	}

	if (pstIdentifier->GridPos < 0 || pstIdentifier->GridPos >= *stDummy.piNum)
	{
		COND_DO_(pstPlayer, z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
		                               "ָ���Ĺ�����Ʒ������, �޷���������"));
		return NULL;
	}

	pstGrid = stDummy.pstGrid + pstIdentifier->GridPos;
	if (pstGrid->Pos != pstIdentifier->GridPos)
	{
		COND_DO_(pstPlayer, z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
		                               "��λ����, �޷��������� (%d vs %d)",
		                               pstIdentifier->GridPos, pstGrid->Pos));

		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "clan storage(%d) grid pos error: (%d vs %d) id: %d",
		           pstIdentifier->Category, pstIdentifier->GridPos,
		           pstGrid->Pos, pstGrid->ID);
		return NULL;
	}

	return pstGrid;
}

static int clan_storage_grid_destroy(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	CLANSTORAGEGRID *pstGrid,
	const CSCLANSTORAGEGRIDIDENTIFIER *pstIdentifier,char *pszRoleName)
{
	int iPos = pstGrid->Pos;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	OPCLANDATA stData;

	if (clan_storage_dummy(pstEnv, pstZoneClan, pstIdentifier->Category,
	                       &stDummy) < 0)
	{
		return -1;
	}

	stData.ClanMachineDel.ClanMachineDelID = stDummy.pstGrid->ID;
	stData.ClanMachineDel.ClanMachineDelNum = *stDummy.piNum;

	// �������Ƿ���Ŀ��ֿ�ĸ���
	if (pstGrid < stDummy.pstGrid ||
		pstGrid >= stDummy.pstGrid + (*stDummy.piNum))
	{
		return -1;
	}

	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
	                         CLAN_STORAGE_GRID_DEL, pstIdentifier->Category);
	clan_storage_do_grid_delete(pstEnv, pstZoneClan, &stDummy, iPos);

	if (stDummy.iWhich == CLAN_STORAGE_MACHINE)
	{
		z_role_clan_oplog(pstEnv, pszRoleName, clan_id_warp(&pstZoneClan->stClan),
				OP_CLAN_MACHINE_DEL,&stData);
	}
	return 0;
}
*/

static int clan_effect_cache_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	const CLANTECH *pstTech = &pstZoneClan->stClan.ClanAdvance.Tech;
	int i = MAX_CLAN_TECH_CATEGORY;

	pstZoneClan->stZoneClanInfo.stMacProCache.iNum = 0; // ˢ��ȫ������
	while(i-- > 1)
	{
		int j = pstTech->TechLevel[i] + 1;
		while(j-- > 1)
		{
			const CLANTECHDEF *pstDef = clan_tech_def(pstEnv, i, j);
			if (!pstDef)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0,
				           "clan tech def is not exist, category:%d, level:%d",
				           i, j);
				continue;
			}

			clan_effect_cache_update(pstEnv, pstZoneClan, pstDef);
		}
	}
	return 0;
}


static int clan_effect_cache_update(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const CLANTECHDEF *pstDef)
{
	//ClanMachinePropertyEntry *pstProperty = NULL;
	int i = 0;

	for(i = 0; i < (int)CCH(pstDef->Result); ++i)
	{
		switch(pstDef->Result[i].ID)
		{
		case CLAN_TECH_NONE:
			goto out;
			break;

		case CLAN_TECH_COLLECTOR_ADD_HP:
		case CLAN_TECH_TANK_ADD_HP:
		case CLAN_TECH_COLLECTOR_ADD_DEF:
		case CLAN_TECH_TANK_ADD_DEF:
		case CLAN_TECH_COLLECTOR_ADD_ATK:
		case CLAN_TECH_TANK_ADD_ATK:
		case CLAN_TECH_COLLECTOR_ADD_CAP:
		case CLAN_TECH_COLLECTOR_ADD_COLLECT_SPEED:
		case CLAN_TECH_COLLECTOR_ADD_MOVE_SPEED:
			break;

		default:
			// TODO: ��������, �繫�����֮��
			break;
		}
	}

out:
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstZoneClan);	//add by paraunused.pl
}

static ClanMachinePropertyEntry* 
clan_find_or_cached_effect(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID)
{
	ClanMachineProperyCache *pstCache =
		&pstZoneClan->stZoneClanInfo.stMacProCache;
	int i = pstCache->iNum;
	
	UNUSED(pstEnv);

	while(i--)
	{
		if (pstCache->astProperties[i].iID == iID)
		{
			return pstCache->astProperties + i;
		}
	}

	if (pstCache->iNum >= (int)CCH(pstCache->astProperties))
	{
		return NULL;
	}

	// ������½���, ����һ��������
	bzero(pstCache->astProperties + pstCache->iNum,
	      sizeof(pstCache->astProperties[0]));
	pstCache->astProperties[pstCache->iNum].iID = iID;
	return pstCache->astProperties + pstCache->iNum++;
}
/*

static const CLANGENERALRESULT* clan_product_result(ZONESVRENV *pstEnv,
                                                    int iDefID,
                                                    int iResultID)
{
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv, iDefID);
	if (pstDef)
	{
		int i = 0;
		for(i = 0; i < (int)CCH(pstDef->Result); ++i)
		{
			if (!pstDef->Result[i].ID)
			{
				break;
			}
			if (pstDef->Result[i].ID == iResultID)
			{
				return pstDef->Result + i;
			}
		}
	}
	return kEmptyGeneralResult;
}

static int clan_machine_supply_fuel(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    Player *pstPlayer,
                                    const MONMACHINEDEF *pstMacDef,
                                    CLANSTORAGEGRID *pstGrid)
{
	int iNeedFuel = pstMacDef->MaxEnergy - pstGrid->Extension.AsMachineGrid.Fuel;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	int iToken = player_clan_position(pstEnv, pstPlayer);
	int iPremDeny;

	if (iNeedFuel <= 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��е��������, ���貹��");
		return -1;
	}

	clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_RESOURCE, &stDummy);
	if (clan_storage_del_by_result(pstEnv, pstZoneClan, &stDummy, iToken,
		                           pstMacDef->EnergyID, CLAN_ITEM_ADD_FUEL,
		                           iNeedFuel,
		                           &pstGrid->Extension.AsMachineGrid.Fuel,
		                           &iPremDeny) <= 0)
	{
		if (iPremDeny)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
			           "������ӵ�е�����Ϊ����/����״̬���޷����䣡");
		}
		else
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
			           "ȼ�Ͽ�ʹ����������, �޷�����");
		}
		return -1;
	}

	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
	                         CLAN_STORAGE_GRID_MOD, CLAN_STORAGE_MACHINE);
	return 0;
}

static int clan_machine_repair(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               Player *pstPlayer,
                               const MONMACHINEDEF *pstMacDef,
                               CLANSTORAGEGRID *pstGrid)
{
	int iNeedHP = 0, iMaxHP = 0, iRet;
	Monster stKey = { .iDefPos = -1, .iDefIdx = pstMacDef->MonsterID, };
	const MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, &stKey);
	const CLANPRODUCTDEF *pstProDef = clan_product_def(pstEnv, pstGrid->ID);

	UNUSED(pstMacDef);

	if (!pstProDef)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "ѡȡ�Ĳ��ǹ����Ʒ");
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "clan product def is not exist, id:%d", pstGrid->ID);
		return -1;
	}

	if (!pstMonDef)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��������");
		tlog_error(pstEnv->pstLogCat, 0, 0, "mon def is not exist, id:%d",
		           pstMacDef->MonsterID);
		return -1;
	}

	iMaxHP += pstMonDef->HP;
	iMaxHP += clan_machine_cached_effect(pstEnv, pstZoneClan, pstGrid->ID)
	                                    ->aiAdded[CLAN_MACHINE_MAX_HP];

	if ((iNeedHP = iMaxHP - pstGrid->Extension.AsMachineGrid.HP) <= 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��еѪ������, ���貹��");
		return -1;
	}

	iRet = clan_storage_repair(pstEnv, pstZoneClan, pstProDef->NeedMoneyCost,
	                           iMaxHP, iNeedHP, pstGrid);
	if (iRet < 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "�����ʽ���, ��������"
		           "ָ���Ļ�е");
		return -1;
	}

	if (iRet > 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "�����ʽ���, ������ȫ����"
		           "ָ���Ļ�е");
	}
	return 0;
}
*/
//�����С���ύ����
int clan_get_commit_min_num(float fContributeUnit)
{
	int iMinNum = 1;

	fContributeUnit = fContributeUnit - ((int)fContributeUnit) * 1.0f ;
	if (fContributeUnit != 0.00f)
	{
		
		//��С��λ	
		iMinNum = (int)(1.0f / fContributeUnit + 0.6);//��ҪԼ����,С��λ�ܱ�1����
		if (iMinNum < 1)
		{
			iMinNum = 1;
		}
	}

	return iMinNum;
}

static int clan_machine_commit_ore(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   Player *pstPlayer, ROLEGRID *pastRoleGrid, int iNum)
{
	
	CLANPRODUCTDEF *pstOreDef = NULL;

	int iCommitNum = 0;
	int i = 0;
	int iRealNum = 0;

	for(i = 0; i < iNum; i++)
	{
		iCommitNum = pastRoleGrid[i].GridData.RoleItem.Num;

		
		pstOreDef = clan_product_def(pstEnv, pastRoleGrid[i].GridData.RoleItem.DefID);
		
		if (!pstOreDef)
		{
			continue;
		}

		if (pstOreDef->ResourceRate > 0.0f)
		{
			iCommitNum = (int) (iCommitNum * pstOreDef->ResourceRate);
		}
		
		if (iCommitNum < 1)
		{
			continue;
		}

		if (clan_resource_forecasted_check_capacity(pstEnv, pstZoneClan, pstOreDef, 
			                   iCommitNum) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN65,pstOreDef->Name);
			continue;
		}

		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
								  ZONEERR_CLANBROAD247,
		                          //"%s �򹫻���Դ�����ύ��[%s]��%d",
		                          pstPlayer->stRoleData.RoleName,
		                          pstOreDef->Name,
		                          iCommitNum);
		
		clan_resource_add_entries(pstEnv, pstZoneClan, 
		                         pstOreDef->ID,
		                         iCommitNum,&iRealNum);
		
		//clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_OWNED_CONTRIBUT,
		                  //iCommitNum *
		                  //pstOreDef->ContributValue);
		
		{
			OPCLANDATA stData;
			stData.ClanResourceCommit.ResouceID = pstOreDef->ID;
			stData.ClanResourceCommit.ResouceNum = iRealNum;
			stData.ClanResourceCommit.TaskID = 0;

			z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
			OP_CLAN_RESOURCE_PLAYER_COMMIT,&stData);
		}

	}
	return 0;
}
/*
static int clan_machine_update_properties(ZONESVRENV *pstEnv,
                                          ZoneClan *pstZoneClan,
                                          const CLANMACHINEEXTENSION *pstExt,
                                          Monster *pstMachine)
{
	const ClanMachinePropertyEntry *pstPropery = clan_machine_cached_effect(
		pstEnv, pstZoneClan, pstMachine->iDefIdx);

	pstMachine->iHP = pstExt->HP;
	pstMachine->iMaxHP += pstPropery->aiAdded[CLAN_MACHINE_MAX_HP];
	// TODO: ����
	// TODO: ����
	this_machine(pstEnv, pstMachine)->stMachineInfo.MaxPackOre +=
		pstPropery->aiAdded[CLAN_MACHINE_CAP];
	this_machine(pstEnv, pstMachine)->stMachineInfo.CollcetTime -=
		pstPropery->aiAdded[CLAN_MACHINE_COLLECTING_SPEED];
	if (this_machine(pstEnv, pstMachine)->stMachineInfo.CollcetTime <= 0)
	{
		this_machine(pstEnv, pstMachine)->stMachineInfo.CollcetTime = 1;
	}
	// TODO: �ƶ��ٶ�

	return 0;
}
*/
static int clan_making_process(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                               ClanProgressDummy *pstDummy)
{
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int i = *pstDummy->piNum;
	while(i--)
	{
		CLANRUNNINGPROGRESS *pstProgress = pstDummy->pstProgress + i;
		const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv,
		                                                pstProgress->ID);
		if (!pstDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "clan product def is not exist, id:%d",
			           pstProgress->ID);
			return -1;
		}

		if (pstProgress->EndTime > kCurr)
		{
			continue;
		}

		if (clan_resource_add_entries(pstEnv, pstZoneClan,			                         
		                             pstProgress->ID, 1,NULL) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "can not add item to resource, id:%d",
			           pstProgress->ID);
			return -1;
		}

		--(pstProgress->Value);
		clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
		                             CLAN_SVR_MAKING_NOTIFY,
		                             CLAN_PROGRESS_FINISHED);

		if (pstDef->ProductTech >0)
		{
			clan_resource_inc(pstEnv, pstZoneClan, NULL, CLAN_TECH_POINT,
			                  pstDef->ProductTech);
		}
				
		if (pstProgress->Value > 0)
		{
			pstProgress->BeginTime = kCurr;
			pstProgress->EndTime = pstProgress->BeginTime + pstDef->TimeCost;
			continue;
		}

		if (i < *pstDummy->piNum - 1)
		{
			memmove(pstProgress, pstProgress + 1,
			        sizeof(pstProgress[0]) * (*pstDummy->piNum - i - 1));
		}
		--(*pstDummy->piNum);

		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          ZONEERR_CLANBROAD248, pstDef->Name);



		{
			OPCLANDATA stData;
			stData.ClanMake.ClanMakeID = pstProgress->ID;
			stData.ClanMake.ClanMakeNum = 1;
				
			z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
				OP_CLAN_MAKE,&stData);
		}


	}
	return 0;
}

static int clan_research_process(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 ClanProgressDummy *pstDummy)
{
	CLANTECH *pstTech = &pstZoneClan->stClan.ClanAdvance.Tech;
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int i = *pstDummy->piNum;
	OPCLANDATA stData;
	
	while(i--)
	{
		CLANRUNNINGPROGRESS *pstProgress = pstDummy->pstProgress + i;
		if (pstProgress->EndTime <= kCurr)
		{
			const CLANTECHDEF *pstDef = clan_tech_def(pstEnv,
			                                          pstProgress->Category,
			                                          pstProgress->Value);
			if (!pstDef)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0,
				           "clan tech def is not exist, category:%s, level:%s",
				           pstProgress->Category, pstProgress->Value);
				return -1;
			}

			stData.ClanTechCh.ClanTechType = pstProgress->Category;
			stData.ClanTechCh.ClanTechLevelPre = pstTech->TechLevel[(int)pstProgress->Category];
			stData.ClanTechCh.ClanTechLevel = pstProgress->Value;
			
			pstTech->TechLevel[(int)pstProgress->Category] = pstProgress->Value;
			clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);

			clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
			                             CLAN_SVR_RESEARCH_NOTIFY,
			                             CLAN_PROGRESS_FINISHED);
			if (i < *pstDummy->piNum - 1)
			{
				memmove(pstProgress, pstProgress + 1,
				        sizeof(pstProgress[0]) * (*pstDummy->piNum - i - 1));
			}
			--(*pstDummy->piNum);

			clan_effect_cache_update(pstEnv, pstZoneClan, pstDef);
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
			                          ZONEERR_CLANBROAD249, pstDef->Name);

			z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
				OP_CLAN_TECH_CH,&stData);
		}
	}
	return 0;
}

static int clan_checked_making_num(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   const CSCLANMAKE *pstReq)
{
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv,
	                                                pstReq->ItemOrMachineID);

	UNUSED(pstZoneClan);

	if (!pstDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "clan product def is not exist, id:%d",
		           pstReq->ItemOrMachineID);
		return 0;
	}

	switch(pstDef->Category)
	{
	case CLAN_PRODUCT_MACHINE:
	case CLAN_PRODUCT_ITEM:
		return pstReq->Num;
		break;

	default:
		break;
	}
	return 0;
}
/*
static int clan_callback_machine(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 Player *pstPlayer, CLANSTORAGEGRID *pstGrid)
{
	Monster *pstMon = clan_storage_mapped_machine(pstEnv, pstZoneClan,
	                                              pstGrid->Pos);
	if (!pstMon)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
		           "�Ҳ���ʹ���еĻ�е, �޷�����");
		return -1;
	}

	// ֻ�л᳤����ǿ�ƻ��ջ�е
	if (player_clan_position(pstEnv, pstPlayer) != CLAN_POS_PRESIDENT &&
		!is_empty_machine(pstEnv,pstMon))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
		           "��ǰ��е�г�Ա���ڳ���������Ȩ��ǿ���ջ�");
		return -1;
	}

	if (mon_machine_back(pstEnv, pstMon) < 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��е����ʧ��");
		return -1;
	}
	return 0;	
}

static Monster* clan_machine_spawn(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   const CLANSTORAGEGRID *pstGrid,
                                   ZONEMAPINST *pstMapInst, RESPOS *pstPos)
{
	Monster stDummy = {
		.cStat = MONSTER_DIE,
		.iDefIdx = pstGrid->ID,
		.iInitMap = pstMapInst->iMapInstID,
		.stMap = {
			.iID = pstMapInst->iMapInstID,
			.iPos = pstMapInst->iIndex,
		},
		.iInitMapPos = pstMapInst->iMapInstID,
		.stInitPos = *pstPos,
		.stCurrPos = *pstPos,
		.bDirection = RAND1(180),
	};

	Monster *pstMon = NULL;
	MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, &stDummy);
	MapIndex *pstMapIdx = z_mappos_index(pstEnv->pstMapObj,
	                                     &pstMapInst->iMapPos,
	                                     pstMapInst->iMapInstID);

	if (!pstMapIdx || !pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_MACHINE)
	{
		return NULL;
	}

	pstMon = z_new_monster(pstEnv->pstAppCtx, pstEnv, &stDummy, pstMonDef);
	if (!pstMon)
	{
		return NULL;
	}

	//ռ��union
	pstMon->iCommType = MON_UNION_TYPE_MACHINE;
	
	//����Ȩ����Ϣ
	this_machine(pstEnv, pstMon)->nOwnerFastID =
		pstZoneClan->stClan.ClanProf.Id;
	this_machine(pstEnv, pstMon)->stMachineInfo.OwnerType = MACHINE_OWNER_CLAN;
	this_machine(pstEnv, pstMon)->stMachineInfo.OwnerID =
		pstZoneClan->stClan.ClanProf.GID;
	STRNCPY(this_machine(pstEnv, pstMon)->stMachineInfo.OwnerName, 
	        pstZoneClan->stClan.ClanProf.Name, 
			CCH(this_machine(pstEnv, pstMon)->stMachineInfo.OwnerName));

	// ӳ���е��ID
	pstZoneClan->stZoneClanInfo.iMachineIDMap[pstGrid->Pos] = pstMon->iMIndex;

	// ���»�е�ĸ�������
	mon_machine_data(pstEnv, pstMon);

	pstMon->cFirstBorn = 1;
	if (z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon,NULL) < 0)
	{
		pstZoneClan->stZoneClanInfo.iMachineIDMap[pstGrid->Pos] = -1;
		return NULL;
	}
	pstMon->cFirstBorn = 0;
	return pstMon;
}

static int clan_machine_grab(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             Player *pstPlayer, CLANSTORAGEGRID *pstGrid)
{
	Monster *pstMachine = NULL;
	ZONEIDX stIdx = {
		.iID = pstPlayer->stRoleData.Map,
		.iPos = pstPlayer->stOnline.iMapPos,
	};
	ZONEMAPINST *pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);

	if (!pstMapInst)
	{
		return -1;
	}

	if (!(pstMachine = clan_machine_spawn(pstEnv, pstZoneClan, pstGrid,
	                                      pstMapInst,
	                                      &pstPlayer->stRoleData.Pos)))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��е����ʧ��, �޷�ȡ����е");
		return -1;
	}

	z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY,
	                   pstGrid->Extension.AsMachineGrid.Fuel);

	//����
	z_ride_on_machine(pstEnv, pstPlayer, pstMachine, machine_get_free_seatidx(pstEnv,pstMachine), 0);

	pstGrid->Extension.AsMachineGrid.Using = 1;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
	                         CLAN_STORAGE_GRID_MOD, CLAN_STORAGE_MACHINE);
	return 0;
}
*/
static int clan_check_making(ZONESVRENV *pstEnv, Player *pstPlayer,
                             ZoneClan *pstZoneClan,
                             const ClanProgressDummy *pstDummy,
                             const CLANPRODUCTDEF *pstDef, int iNum)
{
	int i = 0;

	if (!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
		               ZONEERR_CLAN33);
		return -1;
	}

	if (*pstDummy->piNum >= pstDummy->iMax)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
		               ZONEERR_CLAN34);
		return -1;
	}

	if (clan_resource_forecasted_check_capacity(pstEnv, pstZoneClan, pstDef,
                                               iNum) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN35);
		return -1;
	}

	if ((int)pstZoneClan->stClan.ClanProf.Money < pstDef->NeedMoneyCost * iNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN36);
		return -1;
	}

	if (clan_resource_check_material_for_making(pstEnv, pstZoneClan, pstDef,
	                                           iNum,pstPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN66);
		return -1;
	}

	for(i = 0; i < (int)CCH(pstDef->TechRequired); ++i)
	{
		const CLANTECH *pstTech = &pstZoneClan->stClan.ClanAdvance.Tech;

		if (!pstDef->TechRequired[i].TechCategory)
		{
			break;
		}

		if (pstDef->TechRequired[i].TechCategory < 0 ||
			pstDef->TechRequired[i].TechCategory >=
			MAX_CLAN_TECH_CATEGORY)
		{
			return -1;
		}

		if (pstTech->TechLevel[(int)pstDef->TechRequired[i].TechCategory] <
			pstDef->TechRequired[i].Level)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
			           ZONEERR_CLAN67, pstDef->Name);
			return -1;
		}
	}
	return 0;
}

static int clan_check_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                               ZoneClan *pstZoneClan,
                               const ClanProgressDummy *pstDummy,
                               const CLANTECHDEF *pstDef)
{
	CLANTECH *pstTech = &pstZoneClan->stClan.ClanAdvance.Tech;
	int i = *pstDummy->piNum, iIdx = clan_tech2building(pstEnv, pstZoneClan,
	                                                    pstDef->Category);
	
	CLANCITYBUILDING *pstBuilding = &pstZoneClan->stClan.ClanCity.Buildings[CLAN_IDX_TECH_CENTER];

	if (!pstDef || iIdx < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN37);
		return -1;
	}

	//�Ƽ����ĵȼ�
	if (pstBuilding->Level < pstDef->Required)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN38);
		return -1;
	}

	while(i--)
	{
		if (pstDummy->pstProgress[i].Category == pstDef->Category &&
			pstDummy->pstProgress[i].Value == pstDef->Level)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
			           ZONEERR_CLAN39, pstDef->Name);
			return -1;
		}
	}

	/*
	if (pstDef->Level > 3)
	{
		if (pstZoneClan->stClan.ClanCity.Buildings[iIdx].Level < 2)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
					   "1����%s����ѧϰ����3����<%s>", kClanBuildingName[iIdx],
					   pstDef->Name);
			return -1;
		}
	}
	*/
	if ((int)pstZoneClan->stClan.ClanProf.TechPoint <
		pstDef->NeedTechPointCost)
	{		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN40);	
		return -1;
	}

	

	if (pstDef->NeedTech > 0 && pstDef->NeedTech < MAX_CLAN_TECH_CATEGORY)
	{
		if (pstDef->NeedTechLevel > pstTech->TechLevel[pstDef->NeedTech])
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN41);	
			
			return -1;
		}
	}

	if ((int)pstZoneClan->stClan.ClanProf.Money < pstDef->NeedMoneyCost)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN42);
		
		return -1;
	}

	if (pstDef->Level - pstTech->TechLevel[(int)pstDef->Category] != 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN43, pstDef->Name, pstTech->TechLevel[(int)pstDef->Category] + 1);	
		return -1;
	}

	if (pstTech->TechLevel[(int)pstDef->Category] >= pstDef->Level)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN44, pstDef->Name);		
		return -1;
	}

	// ͬʱ�з���������
	if (*pstDummy->piNum >= pstDummy->iMax)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN45, pstDummy->iMax);			
		return -1;
	}

	return 0;
}
/*
int clan_machine_on_callback(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             Monster *pstMon)
{
	int iStoredHP = 0;
	const MONSTERDEF *pstDef = z_get_mon_def(pstEnv, pstMon);
	CLANSTORAGEGRID *pstGrid = clan_machine_using_grid(pstEnv, pstZoneClan,
	                                                   pstMon);
	if (!pstGrid || !pstDef)
	{
		return -1;
	}

	// Ѫ������, ��ʱ�ðٷֱ�
	if (pstMon->iHP <= 0)
	{
		iStoredHP = 0;
	}
	else
	{
		int iOriginHP = pstDef->HP + clan_machine_cached_effect(
			pstEnv,
			pstZoneClan,
			pstDef->MonsterID)->aiAdded[CLAN_MACHINE_MAX_HP];
		if (pstMon->iMaxHP <= 0)
		{
			return -1;
		}
		iStoredHP = iOriginHP * ((float)pstMon->iHP / (float)pstMon->iMaxHP);
	}

	pstGrid->Extension.AsMachineGrid.Using = 0;
	pstGrid->Extension.AsMachineGrid.HP = iStoredHP;
	pstGrid->Extension.AsMachineGrid.Fuel =
		this_machine(pstEnv, pstMon)->stMachineInfo.Energy;
	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
	                         CLAN_STORAGE_GRID_MOD,
	                         CLAN_STORAGE_MACHINE);
	return 0;
}

int clan_machine_on_destroy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                            Monster *pstMon)
{
	CLANSTORAGEGRID *pstGrid = clan_machine_using_grid(pstEnv, pstZoneClan,
	                                                   pstMon);
	if(!pstGrid)
	{
		return -1;
	}

	pstGrid->Extension.AsMachineGrid.Using = 0;
	pstGrid->Extension.AsMachineGrid.HP = 0;
	pstGrid->Extension.AsMachineGrid.Fuel =
		this_machine(pstEnv, pstMon)->stMachineInfo.Energy;
	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
	                         CLAN_STORAGE_GRID_MOD,
	                         CLAN_STORAGE_MACHINE);
	return 0;
}

int clan_machine_on_update(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                           Monster *pstMon)
{
	CLANSTORAGEGRID *pstGrid = clan_machine_using_grid(pstEnv, pstZoneClan,
	                                                   pstMon);
	if (!pstGrid)
	{
		return -1;
	}
	return clan_machine_update_properties(pstEnv, pstZoneClan,
	                                      &pstGrid->Extension.AsMachineGrid,
	                                      pstMon);
}
*/

const ClanMachinePropertyEntry* 
clan_machine_cached_effect(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID)
{
	const ClanMachineProperyCache *pstCache =
		&pstZoneClan->stZoneClanInfo.stMacProCache;
	int i = pstCache->iNum;
	
	UNUSED(pstEnv);

	while(i--)
	{
		if (pstCache->astProperties[i].iID == iID)
		{
			return pstCache->astProperties + i;
		}
	}

	return kEmptyMachineProperty;
}

/*
int clan_storage_add_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int iWhich, int iID, int iNum)
{
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv, iID);

	if (!pstDef)
	{
		return -1;
	}

	if (clan_storage_dummy(pstEnv, pstZoneClan, iWhich, &stDummy) < 0)
	{
		return -1;
	}

	if (clan_storage_check_adding(pstEnv, pstZoneClan, &stDummy, pstDef,
	                              iNum) < 0)
	{
		return -1;
	}

	return clan_storage_do_add_entries(pstEnv, pstZoneClan, &stDummy, pstDef,
	                                   iNum, 0);
}


int clan_storage_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich)
{
	UNUSED(pstEnv);

	switch(iWhich)
	{
	case CLAN_STORAGE_MACHINE:
		pstZoneClan->stClan.ClanAdvance.Storage.MachineNum = 0;
		break;

	case CLAN_STORAGE_RESOURCE:
		pstZoneClan->stClan.ClanAdvance.Storage.ResourceNum = 0;
		break;

	default:
		return -1;
		break;
	}
	clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
	return 0;
}

int clan_storage_del_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int iWhich, int iID, int iNum)
{
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv, iID);

	if (!pstDef)
	{
		return -1;
	}

	if (clan_storage_dummy(pstEnv, pstZoneClan, iWhich, &stDummy) < 0)
	{
		return -1;
	}

	if (clan_storage_check_deleting(pstEnv, pstZoneClan, &stDummy, pstDef,
	                                iNum) < 0)
	{
		return -1;
	}

	return clan_storage_do_del_entries(pstEnv, pstZoneClan, &stDummy, pstDef,
	                                   iNum, 0);
}

int clan_storage_check_del_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   int iWhich, int iID, int iNum)
{
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv, iID);

	if (!pstDef)
	{
		return -1;
	}

	if (clan_storage_dummy(pstEnv, pstZoneClan, iWhich, &stDummy) < 0)
	{
		return -1;
	}

	return clan_storage_check_deleting(pstEnv, pstZoneClan, &stDummy, pstDef,
	                                   iNum);
}
*/
// �������ֻӦ������һ��
void clan_advance_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	/*
	int i = 0;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();

	clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_MACHINE, &stDummy);
	i = *stDummy.piNum;
	while(i--)
	{
		// ��������ػ�������ԭ���»�еû����ȷ����, ���ﴦ��һ����λ���
		if (stDummy.pstGrid[i].Category == CLAN_GRID_MACHINE &&
			stDummy.pstGrid[i].Extension.AsMachineGrid.Using)
		{
			stDummy.pstGrid[i].Extension.AsMachineGrid.Using = 0;
			clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
		}

		// ���Pos�ֶ��ǲ�����ȷ
		if (stDummy.pstGrid[i].Pos != i)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
					   "clan machine storage grid pos error: (%d vs %d) id: %d",
					   i, stDummy.pstGrid[i].Pos, stDummy.pstGrid[i].ID);
		}
	}

	clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_RESOURCE, &stDummy);
	i = *stDummy.piNum;
	while(i--)
	{
		// ���Pos�ֶ��ǲ�����ȷ
		if (stDummy.pstGrid[i].Pos != i)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "clan resource storage grid pos"
					   " error: (%d vs %d) id: %d", i,
					   stDummy.pstGrid[i].Pos,
					   stDummy.pstGrid[i].ID);
		}
	}
	*/
	// ���ݿƼ����еĿƼ�, ����һ�����Ի���, ���㹫���е/����ʹ��
	clan_effect_cache_init(pstEnv, pstZoneClan);
}

// ��������ڹ��ᱻɾ����ʱ�����һ��
void clan_advance_cleanup(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	/*
	int i = 0;
	ClanStorageDummy stDummy = INIT_CLAN_STORAGE_DUMMY();

	clan_storage_dummy(pstEnv, pstZoneClan, CLAN_STORAGE_MACHINE, &stDummy);
	i = *stDummy.piNum;
	while(i--)
	{
		if (stDummy.pstGrid[i].Category == CLAN_GRID_MACHINE &&
			stDummy.pstGrid[i].Extension.AsMachineGrid.Using)
		{
			Monster *pstMon = clan_storage_mapped_machine(pstEnv, pstZoneClan,
			                                              i);
			if (!pstMon)
			{
				continue;
			}

			if (mon_machine_back(pstEnv, pstMon) < 0)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "can not callback clan "
				           "machine, clan id:%d, mon id:%d, mon defid:%d",
				           pstZoneClan->stClan.ClanProf.Id, pstMon->iID,
				           pstMon->iDefIdx);
				continue;
			}
		}
	}
	*/
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstZoneClan);	//add by paraunused.pl
}

int clan_advance_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_RESEARCH, &stDummy);
	clan_research_process(pstEnv, pstZoneClan, &stDummy);

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_ARM_MAKING, &stDummy);
	clan_making_process(pstEnv, pstZoneClan, &stDummy);

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_COLLECTION_MAKING,
	                    &stDummy);
	clan_making_process(pstEnv, pstZoneClan, &stDummy);
	return 0;
}
/*

int clan_extend_storage(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
		                int iGridNum)
{
	CLANSTORAGE *pstStorage = &pstZoneClan->stClan.ClanAdvance.Storage;

	if (iGridNum < 0)
	{
		// �ݲ�֧�������ֿ���λ
		return -1;
	}

	switch(iWhich)
	{
	case CLAN_STORAGE_RESOURCE:
		if (clan_property_modify(pstEnv, pstZoneClan,
		                         CLAN_PROPERTY_MAX_RESOURCE_GRID_NUM, iGridNum,
		                         0) < 0)
		{
			return -1;
		}
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          "��Դ�ֿ���λ�����䵽: %d",
		                          pstStorage->MaxResourceNum);
		break;

	case CLAN_STORAGE_MACHINE:
		if (clan_property_modify(pstEnv, pstZoneClan,
		                         CLAN_PROPERTY_MAX_MACHINE_GRID_NUM, iGridNum,
		                         0) < 0)
		{
			return -1;
		}
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          "��е�ֿ���λ�����䵽: %d",
		                          pstStorage->MaxMachineNum);
		break;

	default:
		return -1;
		break;
	}

	return 0;
}
*/
int clan_used_tech_point(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                         int iTechID)
{
	if(iTechID > 0 && iTechID < MAX_CLAN_TECH_CATEGORY)
	{
		return clan_tech_used_tech_point(pstEnv, iTechID,
		               pstZoneClan->stClan.ClanAdvance.Tech.TechLevel[iTechID]);
	}
	else if (iTechID == 0)
	{
		return pstZoneClan->stClan.ClanAdvance.UsedTechPoint;
	}
	else if(iTechID < 0)
	{
		return clan_building_used_tech_point(pstEnv, pstZoneClan, -iTechID);
	}
	return -1;
}

static int clan_tech2building(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iTechID)
{
	UNUSED(pstEnv);
	UNUSED(pstZoneClan);

	switch (iTechID)
	{
	case CLAN_TECH_CATEGORY_COLLECTOR_ATTRIBUTES:
	case CLAN_TECH_CATEGORY_COLLECTOR_MAKING:
	case CLAN_TECH_CATEGORY_COLLECTOR_ALTERATION:
	case CLAN_TECH_CATEGORY_MACHINE_MAKING:
	case CLAN_TECH_CATEGORY_TANK_MAKING:
	case CLAN_TECH_CATEGORY_SPANNER:
	case CLAN_TECH_CATEGORY_GRENADE:
	case CLAN_TECH_CATEGORY_BAZOOKA:
	case CLAN_TECH_CATEGORY_FUEL_MACHINE:
	case CLAN_TECH_CATEGORY_CLAN_RIDE:
	case CLAN_TECH_CATEGORY_MACHINE_MON:
	case CLAN_TECH_CATEGORY_TANK:
	case CLAN_TECH_CATEGORY_MISSILE:
	case CLAN_TECH_CATEGORY_BOMB:
	case CLAN_TECH_CATEGORY_MINE:
	case CLAN_TECH_CATEGORY_ANTI_MINE_MACHINE:
	case CLAN_TECH_CATEGORY_ANTI_MINE_PKG:
	case CLAN_TECH_CATEGORY_BIG_MACHINE:
	case CLAN_TECH_CATEGORY_COLLECTOR_FUEL:
	case CLAN_TECH_CATEGORY_RESOURCE_CAPTURE:
	case CLAN_TECH_CATEGORY_SPY:
	case CLAN_TECH_CATEGORY_RADAR:
		return CLAN_IDX_FACTORY;
		break;

	case CLAN_TECH_CATEGORY_PET:
	case CLAN_TECH_CATEGORY_PET_ALTERATION:
		return CLAN_IDX_FARM;
		break;

	default:
		return CLAN_IDX_FACTORY;
		break;
	}
	return -1;
}
/*
static float clan_commited_owned_contribut(ZONESVRENV *pstEnv,
                                           const RESULT *pstResult, int iNum)
{
	const CLANPRODUCTDEF *pstDef = NULL;
	if (pstResult->ResultVal1 < 0)
	{
		return 0;
	}
	if (pstResult->ResultVal1 > 0)
	{
		return (float)iNum / (float)pstResult->ResultVal1;
		//return iNum * pstResult->ResultVal1;
	}

	pstDef = clan_product_def(pstEnv, pstResult->ResultVal2);
	if (!pstDef)
	{
		return 0;
	}
	return (float)iNum * pstDef->ContributValue;
}
*/

//����������,û�۹����ʽ�
int player_clan_shop_buy(ZONESVRENV *pstEnv, Player *pstPlayer, Npc *pstNpc,
                         const SELLGOODS *pstSellGoods, int iNum)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	int iPos = player_clan_position(pstEnv, pstPlayer);
	const CLANPRODUCTDEF *pstDef = NULL;

	//����������,û�۹����ʽ�
	return 0;

	UNUSED(pstNpc);

	if (!pstZoneClan ||
		(iPos != CLAN_POS_PRESIDENT && iPos != CLAN_POS_VICE_PRESIDENT))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN26);
		return -1;
	}

	if (!(pstDef = clan_product_def(pstEnv, pstSellGoods->GoodsID)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN68 );
		return -1;
	}

	if (pstDef->BuyPrice * iNum > (int)pstZoneClan->stClan.ClanProf.Money)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN69);
		return -1;
	}

	if (clan_resource_forecasted_check_capacity(pstEnv, pstZoneClan, pstDef,
	                                           iNum) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN70);
		return -1;
	}

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD250,
	                          pstPlayer->stRoleData.RoleName,
	                          pstDef->Name, iNum);
	clan_resource_add_entries(pstEnv, pstZoneClan,	                         
				             pstSellGoods->GoodsID, iNum,NULL);
	return 0;
}

// ������Ϣֻ����һ�Σ��Ժ��͵Ķ���������Ϣ
int player_clan_advance(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstZoneClan)
	{
		return -1;
	}

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_ADVANCE_SNAPSHOT;
	stPkg.Body.ClanSvr.ClanSvrData.ClanAdvanceSnapshot.Snapshot =
		pstZoneClan->stClan.ClanAdvance;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);	
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int player_clan_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const CSCLANRESEARCH *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRUNNINGPROGRESS *pstProgress = NULL;
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();
	const CLANTECHDEF *pstDef = clan_tech_def(pstEnv, pstReq->Category,
	                                          pstReq->Level);
	// TODO: ���UI���

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_RESEARCH, &stDummy);
	if (clan_check_research(pstEnv, pstPlayer, pstZoneClan, &stDummy,
	                        pstDef) < 0)
	{
		return -1;
	}

	// �о���ʼ
	pstProgress = stDummy.pstProgress + (*stDummy.piNum)++;
	bzero(pstProgress, sizeof(*pstProgress));

	pstProgress->Category = pstDef->Category;
	pstProgress->Value = pstDef->Level;
	pstProgress->BeginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstProgress->EndTime = pstProgress->BeginTime + pstDef->TimeCost;

	clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
	                             CLAN_SVR_RESEARCH_NOTIFY,
	                             CLAN_PROGRESS_STARTED);

	// �з��Ļ���
	g_iOptype = OP_OPTYPE_RESEARCH;
	g_iOpsubtype = pstDef->Category;
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
	                     pstDef->NeedMoneyCost, 1);
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_TECH_POINT,
	                     pstDef->NeedTechPointCost, 1);
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_USED_TECH_POINT,
	                     pstDef->NeedTechPointCost, 0);
	g_iOptype = 0;
	g_iOpsubtype = 0;
	return 0;
}


int player_clan_cancel_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                                const CSCLANCANCELRESEARCH *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRUNNINGPROGRESS *pstProgress = NULL;
	const CLANTECHDEF *pstDef = NULL;
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();

	// TODO: ���UI���

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_RESEARCH, &stDummy);

	if (pstReq->ProgressPos < 0 ||
		pstReq->ProgressPos >= *stDummy.piNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN46);
		return -1;
	}

	if (!*stDummy.piNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN47);
		return -1;
	}

	pstProgress = stDummy.pstProgress + pstReq->ProgressPos;
	pstDef = clan_tech_def(pstEnv, pstProgress->Category, pstProgress->Value);
	if(!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN48);
		return -1;
	}

	clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
	                             CLAN_SVR_RESEARCH_NOTIFY,
	                             CLAN_PROGRESS_CANCELED);
	if(pstReq->ProgressPos < *stDummy.piNum - 1)
	{
		memmove(pstProgress, pstProgress + 1,
		        sizeof(*pstProgress) *
		        (*stDummy.piNum - pstReq->ProgressPos - 1));
	}
	--(*stDummy.piNum);

	// �˻��з�������
	g_iOptype = OP_OPTYPE_RESEARCH_CANCEL;
	g_iOpsubtype = pstDef->Category;
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
	                     pstDef->NeedMoneyCost, 0);
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_TECH_POINT,
	                     pstDef->NeedTechPointCost, 0);
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_USED_TECH_POINT,
	                     pstDef->NeedTechPointCost, 1);
	g_iOptype = 0;
	g_iOpsubtype = 0;

	return 0;
}

int player_clan_research_speedup(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANRESEARCHSPEEDUP *pstReq)
{
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRUNNINGPROGRESS *pstProgress = NULL;
	const CLANTECHDEF *pstDef = NULL;
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();
	int iWantMoney = 0, iShouldTime = 0, iM = 0;

	// TODO: ���UI���

	clan_progress_dummy(pstEnv, pstZoneClan, CLAN_RUNNING_RESEARCH, &stDummy);

 	if (pstReq->ProgressPos < 0 || pstReq->ProgressPos >= *stDummy.piNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN49);
		return -1;
	}

	if (!*stDummy.piNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN50);
		return -1;
	}

	pstProgress = stDummy.pstProgress + pstReq->ProgressPos;
	pstDef = clan_tech_def(pstEnv, pstProgress->Category, pstProgress->Value);
	if(!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN51);
		return -1;
	}

	// �Ƽ����� ����=�Ƽ��ȼ�^2*����ʱ��/12
	if (pstReq->DecreasingTime < 0)
	{
		iShouldTime = pstProgress->EndTime - kCurr;
	}
	else
	{
		iShouldTime = min(pstProgress->EndTime - kCurr, pstReq->DecreasingTime);
	}
	if (iShouldTime <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN309);
		return -1;
	}

	iM = iShouldTime/60;
	if(iShouldTime % 60 > 0)
	{
		iM ++;
	}

	iWantMoney = 2000*pstDef->Level * pstDef->Level * iM;
	
	if ((int)pstZoneClan->stClan.ClanProf.Money < iWantMoney)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		           ZONEERR_CLAN308,
		           iWantMoney - pstZoneClan->stClan.ClanProf.Money);
		return -1;
	}

	g_iOptype = OP_OPTYPE_RESEARCH_SPEED;
	g_iOpsubtype = pstDef->Category;
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, iWantMoney,
	                     1);
	g_iOptype = 0;
	g_iOpsubtype = 0;
	
	pstProgress->EndTime -= iShouldTime;
	clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
	                             CLAN_SVR_RESEARCH_NOTIFY,
	                             CLAN_PROGRESS_SHIFTED);

	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN307,
	           pstDef->Name, pstPlayer->stRoleData.RoleName, iShouldTime);
	clan_research_process(pstEnv, pstZoneClan, &stDummy);
	return 0;
}

int player_clan_make(ZONESVRENV *pstEnv, Player *pstPlayer,
                     const CSCLANMAKE *pstReq)
{
	//int iJob = player_clan_position(pstEnv, pstPlayer);
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	int iNum = 0;
	CLANRUNNINGPROGRESS *pstProgress = NULL;
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();
	const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv,
	                                                pstReq->ItemOrMachineID);

	// TODO: ���UI���

	/*
	// ��ϸ΢�ļ��Ȩ��
	switch (pstReq->Category)
	{
	case CLAN_MAKE_FOR_COLLECTION:
		if (iJob != CLAN_POS_COLLECTION_OFFICER &&
			iJob != CLAN_POS_VICE_PRESIDENT &&
			iJob != CLAN_POS_PRESIDENT)
		{
			goto deny;
		}
		break;

	case CLAN_MAKE_FOR_ARM:
		if (iJob != CLAN_POS_FACTORY_OFFICER &&
			iJob != CLAN_POS_VICE_PRESIDENT &&
			iJob != CLAN_POS_PRESIDENT)
		{
			goto deny;
		}
		break;
	}
	*/

	if (clan_progress_dummy(pstEnv, pstZoneClan, pstReq->Category,
		                    &stDummy) < 0)
	{
		return -1;
	}

	if ((iNum = clan_checked_making_num(pstEnv, pstZoneClan, pstReq)) == 0)
	{
		return -1;
	}

	if (clan_check_making(pstEnv, pstPlayer, pstZoneClan, &stDummy, pstDef,
		                  iNum) < 0)
	{
		return -1;
	}

	pstProgress = stDummy.pstProgress + (*stDummy.piNum)++;
	bzero(pstProgress, sizeof(*pstProgress));

	pstProgress->Category = pstReq->Category;
	pstProgress->ID = pstDef->ID;
	pstProgress->Value = pstReq->Num;
	pstProgress->BeginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstProgress->EndTime = pstProgress->BeginTime + pstDef->TimeCost;

	clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
	                             CLAN_SVR_MAKING_NOTIFY,
	                             CLAN_PROGRESS_STARTED);
	clan_cost_for_making(pstEnv, pstZoneClan, pstDef, iNum);
	return 0;

/*
deny:
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
		           ZONEERR_CLAN26);
	return -1;
*/
}


int player_clan_cancel_making(ZONESVRENV *pstEnv, Player *pstPlayer,
                              const CSCLANCANCELMAKING *pstReq)
{
	//int iJob = player_clan_position(pstEnv, pstPlayer);
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRUNNINGPROGRESS *pstProgress = NULL;
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();
	const CLANPRODUCTDEF *pstDef = NULL;

	// TODO: ���UI���

	/*
	// ��ϸ΢�ļ��Ȩ��
	switch (pstReq->Category)
	{
	case CLAN_MAKE_FOR_COLLECTION:
		if (iJob != CLAN_POS_COLLECTION_OFFICER &&
			iJob != CLAN_POS_VICE_PRESIDENT &&
			iJob != CLAN_POS_PRESIDENT)
		{
			goto deny;
		}
		break;

	case CLAN_MAKE_FOR_ARM:
		if (iJob != CLAN_POS_FACTORY_OFFICER &&
			iJob != CLAN_POS_VICE_PRESIDENT &&
			iJob != CLAN_POS_PRESIDENT)
		{
			goto deny;
		}
		break;
	}
	*/

	if (clan_progress_dummy(pstEnv, pstZoneClan, pstReq->Category,
		                    &stDummy) < 0)
	{
		return -1;
	}

	if (!*stDummy.piNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN52);
		return -1;
	}

	if (pstReq->ProgressPos < 0 || pstReq->ProgressPos >= stDummy.iMax)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN53);
		return -1;
	}

	pstProgress = stDummy.pstProgress + pstReq->ProgressPos;
	pstDef = clan_product_def(pstEnv, pstProgress->ID);
	if (!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN54);
		return -1;
	}

	if (clan_making_rollback(pstEnv, pstZoneClan, pstDef, pstProgress) < 0)
	{
		return -1;
	}

	clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
	                             CLAN_SVR_MAKING_NOTIFY,
	                             CLAN_PROGRESS_CANCELED);
	if (pstReq->ProgressPos < *stDummy.piNum - 1)
	{
		memmove(pstProgress, pstProgress + 1,
		        sizeof(*pstProgress) *
		        (*stDummy.piNum - pstReq->ProgressPos - 1));
	}
	--(*stDummy.piNum);
	return 0;

/*
deny:
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
		           ZONEERR_CLAN26);
	return -1
*/
}


int player_clan_making_speedup(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const CSCLANMAKINGSPEEDUP *pstReq)
{
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRUNNINGPROGRESS *pstProgress = NULL;
	ClanProgressDummy stDummy = INIT_CLAN_PROGRESS_DUMMY();
	const CLANPRODUCTDEF *pstDef = NULL;
	int iWantMoney = 0, iShouldTime = 0, iLevel = 0, iM = 0;

	// TODO: ���UI���

	if (clan_progress_dummy(pstEnv, pstZoneClan, pstReq->Category,
		                    &stDummy) < 0)
	{
		return -1;
	}

	if (!*stDummy.piNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN55);
		return -1;
	}

	if (pstReq->ProgressPos < 0 || pstReq->ProgressPos >= stDummy.iMax)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		              ZONEERR_CLAN56);
		return -1;
	}

	pstProgress = stDummy.pstProgress + pstReq->ProgressPos;
	pstDef = clan_product_def(pstEnv, pstProgress->ID);
	if (!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN57);
		return -1;
	}

	// ������� ����=����Ƽ��ȼ�^2*����ʱ��/12
	if (pstReq->DecreasingTime < 0)
	{
		iShouldTime = pstProgress->EndTime - kCurr;
	}
	else
	{
		iShouldTime = min(pstProgress->EndTime - kCurr, pstReq->DecreasingTime);
	}
	if (iShouldTime <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN58);
		
		return -1;
	}

	
	//iWantMoney = 5 * iLevel * iLevel * iShouldTime / 6;

	iLevel = (pstDef->TechRequired[0].Level <= 0) ? 1 : pstDef->TechRequired[0].Level;
	
	iM = iShouldTime/60;
	if(iShouldTime % 60 > 0)
	{
		iM++;
	}
	
	iWantMoney = 2000*iLevel*iLevel*iM;
	if ((int)pstZoneClan->stClan.ClanProf.Money < iWantMoney)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		           ZONEERR_CLAN59,
		           iWantMoney - pstZoneClan->stClan.ClanProf.Money);
		
		return -1;
	}

	g_iOptype = OP_OPTYPE_MAKE_SPEED;
	g_iOpsubtype = pstDef->ID;
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, iWantMoney,
	                     1);
	g_iOptype = 0;
	g_iOpsubtype = 0;
	
	pstProgress->EndTime -= iShouldTime;
	clan_running_progress_notify(pstEnv, pstZoneClan, pstProgress,
	                             CLAN_SVR_MAKING_NOTIFY,
	                             CLAN_PROGRESS_SHIFTED);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN306,
	           pstDef->Name, pstPlayer->stRoleData.RoleName, iShouldTime);
	clan_making_process(pstEnv, pstZoneClan, &stDummy);
	return 0;
}

/*
int player_clan_grab_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                             const CSCLANGRABMACHINE *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANSTORAGEGRID *pstGrid = clan_freed_machine_grid(pstEnv, pstZoneClan,
	                                                   pstPlayer,
	                                                   pstReq->StorageGridPos);
	if (!pstGrid)
	{
		//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��������, �޷���������");
		return -1;
	}

	if (clan_storage_check_grid_permission(pstEnv, pstZoneClan, pstPlayer,
	                                       pstGrid) < 0)
	{
		return -1;
	}

	if (!pstGrid->Extension.AsMachineGrid.HP)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��е�ѳ�����, �������ʹ��");
		return -1;
	}

	if (is_player_ride_on_machine(pstPlayer))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "���Ѿ��ڻ�е����, ��������");
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��ǰ״̬�޷�������е");
		return -1;
	}
	
	if (!z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		RES_ACTION_RIDE_MACHINE))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "��ǰ״̬�޷�������е");
		return -1;
	}

	return clan_machine_grab(pstEnv, pstZoneClan, pstPlayer, pstGrid);
}


int player_clan_callback_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANCALLBACKMACHINE *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CSCLANSTORAGEGRIDIDENTIFIER stIdentifier = {
		.Category = CLAN_STORAGE_MACHINE,
		.GridPos = pstReq->StorageGridPos,
	};
	CLANSTORAGEGRID *pstGrid = clan_identifier_grid(pstEnv, pstZoneClan,
	                                                pstPlayer, &stIdentifier);
	if (!pstGrid)
	{
		return -1;
	}

	if (pstGrid->Category != CLAN_GRID_MACHINE)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "ָ������Ʒ���ǻ�е");
		return -1;
	}

	if (!pstGrid->Extension.AsMachineGrid.Using)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "ָ���Ļ�е�Ѿ����յ��ֿ���");
		return -1;
	}

	if (clan_callback_machine(pstEnv, pstZoneClan, pstPlayer, pstGrid) < 0)
	{
		return -1;
	}
	return 0;
}


int player_clan_machine_supply(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const CSCLANMACHINESUPPLY *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	const MONMACHINEDEF *pstMacDef = NULL;
	CLANSTORAGEGRID *pstGrid = clan_freed_machine_grid(pstEnv, pstZoneClan,
	                                                   pstPlayer,
	                                                   pstReq->StorageGridPos);
	if (!pstGrid ||
		clan_storage_check_grid_permission(pstEnv, pstZoneClan, pstPlayer,
		                                   pstGrid) < 0)
	{
		return -1;
	}

	if (!(pstMacDef = z_find_mon_machine_def(pstEnv, pstGrid->ID)))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "ָ���Ĳ��ǻ�е");
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "mon machine def is not exist, id:%d", pstGrid->ID);
		return -1;
	}

	switch(pstReq->SupplyFor)
	{
	case CLAN_MACHINE_SUPPLY_FOR_FUEL:
		clan_machine_supply_fuel(pstEnv, pstZoneClan, pstPlayer, pstMacDef,
		                         pstGrid);
		break;

	case CLAN_MACHINE_SUPPLY_FOR_REPAIR:
		clan_machine_repair(pstEnv, pstZoneClan, pstPlayer, pstMacDef, pstGrid);
		break;

	default:
		return -1;
		break;
	}
	return 0;
}


int player_clan_storage_grid_lock_switch(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANSTORAGEGRIDLOCKSWITCH *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANSTORAGEGRID *pstGrid = clan_freed_valid_grid(pstEnv, pstZoneClan,
	                                                 pstPlayer,
	                                                 &pstReq->GridIdentifier);
	// TODO: ���UI���

	if (!pstGrid ||
		clan_storage_check_grid_permission(pstEnv, pstZoneClan, pstPlayer,
		                                   pstGrid) < 0)
	{
		return -1;
	}

	switch(pstReq->LockState)
	{
	case CLAN_GRID_LOCKED:
	case CLAN_GRID_LIMITED:
	case CLAN_GRID_OPEN:
		break;

	default:
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               "��������, �޷���������");
		return -1;
	}

	pstGrid->State = pstReq->LockState;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	clan_storage_grid_notify(pstEnv, pstZoneClan, pstGrid,
	                         CLAN_STORAGE_GRID_MOD,
	                         pstReq->GridIdentifier.Category);
	return 0;
}
*/

CLANRESOURCEINFO* clan_resource_get_info_by_pos(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iPos)
{
	
	CLANRESOURCE* pstResource = &pstZoneClan->stClan.ClanAdvance.Resource;

	if (iPos < 0)
	{
		return NULL;
	}

	if (iPos >= MAX_CLAN_RESOURCE_GRID_NUM)
	{
		return NULL;
	}
			
	if (iPos >= pstResource->ItemNum)
	{
		return NULL;
	}
	
	return &pstResource->ItemInfo[iPos];
	UNUSED(pstEnv);	//add by paraunused.pl
}

int player_clan_res_grab_record(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan, 
									Player *pstPlayer, int iGetID, int iGetNum, int iUseContri)
{
	CLANRESOURCE *pstRes = &pstZoneClan->stClan.ClanAdvance.Resource;
//	int i;
	GETRESINFOONE *pstOne;
//	int iTime;
//	int iIdx;

	// ���µļ�¼���������һ��
	if (pstRes->GetInfoNum >= MAX_GET_RES_INFO)
	{
		memmove(&pstRes->GetResInfos[1], &pstRes->GetResInfos[0], 
					sizeof(pstRes->GetResInfos[0])*(pstRes->GetInfoNum-1));	
	}
	else
	{
		if (pstRes->GetInfoNum > 0)
		{
			memmove(&pstRes->GetResInfos[1], &pstRes->GetResInfos[0], 
					sizeof(pstRes->GetResInfos[0])*pstRes->GetInfoNum);	
		}

		pstRes->GetInfoNum ++;
	}
	
	/*if (pstRes->GetInfoNum >= MAX_GET_RES_INFO)
	{
		iTime = pstRes->GetResInfos[0].GetTime;
		iIdx = 0;
		for (i=1; i<pstRes->GetInfoNum; i++)
		{
			if (pstRes->GetResInfos[i].GetTime < iTime)
			{
				iTime = pstRes->GetResInfos[i].GetTime;
				iIdx = i;
			}	
		}

		pstOne = &pstRes->GetResInfos[iIdx];
	}
	else
	{
		pstOne = &pstRes->GetResInfos[pstRes->GetInfoNum];
		pstRes->GetInfoNum ++;
	}

	memset(pstOne, 0, sizeof(*pstOne));*/
	pstOne = &pstRes->GetResInfos[0];
	pstOne->GetNum = iGetNum;
	pstOne->GetTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstOne->ItemID = iGetID;
	pstOne->Position = player_clan_position(pstEnv, pstPlayer);
	pstOne->UseContri = iUseContri;
	STRNCPY(pstOne->Name, pstPlayer->stRoleData.RoleName, sizeof(pstOne->Name));

	//qsort(&pstRes->GetResInfos[0], pstRes->GetInfoNum, sizeof(pstRes->GetResInfos[0]), player_get_res_info_cmp);
	return 0;
}


int clan_getres_info_msg(ZONESVRENV* pstEnv, ZoneClan *pstClan, Player *pstPlayer, int iPage)
{
	CSPKG stPkg;
	CSCLANSVR *pstClanSvr = &stPkg.Body.ClanSvr;
	CSCLANVIEWGETRESSVR *pstView = &pstClanSvr->ClanSvrData.ViewGetResSvr;
	CLANRESOURCE *pstRes = &pstClan->stClan.ClanAdvance.Resource;
	int iPageCount;
	int iTmp;
	int i;

	iTmp = pstRes->GetInfoNum%10;
	iPageCount = pstRes->GetInfoNum/10;
	if (iTmp > 0)
		iPageCount ++;

	if (iPage >= iPageCount)
	{
		return 0;
	}

	
	pstClanSvr->Operation = CLAN_SVR_GETRES_INFO;
	pstView->PageCurr = iPage;
	pstView->PageCount = iPageCount;
	pstView->InfoNum = 0;
	for (i=iPage*CLAN_VIEW_GETRES_NUM; i<pstRes->GetInfoNum; i++)
	{
		pstView->GetResInfos[pstView->InfoNum] = pstRes->GetResInfos[i];
		pstView->InfoNum ++;
		if (pstView->InfoNum>= CLAN_VIEW_GETRES_NUM)
		{
			break;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
	return 0;
}

int player_clan_getres_info(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANVIEWGETRES *pstReq)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	clan_getres_info_msg(pstEnv, pstClan, pstPlayer, pstReq->Page);
	return 0;
}

static int player_get_res_info_cmp(const void *p1, const void *p2)
{
	GETRESINFOONE *pstOne1 = (GETRESINFOONE*)p1;
	GETRESINFOONE *pstOne2 = (GETRESINFOONE*)p2;

	// ��ȡʱ������ǰ��
	return pstOne2->GetTime - pstOne1->GetTime;
}

// ����ȡ��¼���
int player_open_res_info_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return 0;
	}
	
	open_ui(pstEnv, pstPlayer, iNpcID, iUIType);

	// ��ui��ʱ��ѵ�һҳ����ȥ
	clan_getres_info_msg(pstEnv, pstClan, pstPlayer,0);
	return 0;
}

int player_clan_resource_grab_item(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANRESOURCEGRABITEM* pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRESOURCEINFO *pstInfo = NULL;
	CLANPRODUCTDEF* pstDef = NULL;
	unsigned int uiClanContri = 0;
	ROLEGRID stRoleGrid;
	CLANMEM *pstMem = NULL;

	if (!pstZoneClan )
	{
		return -1;
	}	

	if (pstReq->Num < 1)
	{
		return -1;
	}

	if (player_check_clan(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	pstInfo = clan_resource_get_info_by_pos(pstEnv,pstZoneClan,pstReq->ResourceID);

	if (!pstInfo)
	{
		return -1;
	}
	
	pstDef = clan_product_def(pstEnv, pstInfo->ResourceID);
	if (!pstDef)
	{
		return -1;
	}

	if (clan_resource_check_grid_permission(pstEnv, pstZoneClan, pstPlayer,
		                                   pstInfo) < 0)
	{
		return -1;
	}

	//�������
	if (pstInfo->Num < pstReq->Num)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN60);
		return -1;
	}
	
	//��鹱�׶�


	uiClanContri = pstDef->ContributPrice * pstReq->Num;
	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute < uiClanContri)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN61);
		return -1;
	}

	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstInfo->ResourceID))
	{
		return -1;
	}

	if (GRID_ITEM_ITEM == stRoleGrid.Type)
	{
		stRoleGrid.GridData.RoleItem.Num = pstReq->Num;
	}

	if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}

	pstMem = clan_id_mem(pstZoneClan,pstPlayer->stRoleData.RoleID, NULL);
	if (!pstMem)
		return -1;

	if (0 > clan_resource_dec_entries(pstEnv,pstZoneClan,pstInfo->ResourceID,pstReq->Num))
	{
		return -1;
	}

	if (uiClanContri > 0)
	{
		pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute -= 	uiClanContri;
		pstMem->Contribute = pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute;
		z_contri_consume_oplog(pstEnv, pstZoneClan, OP_OPTYPE_GRAB, pstInfo->ResourceID, uiClanContri);
		
		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem,CLAN_MEMBER_CHG_MOD);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN62, uiClanContri);
	}	

	if (package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_CLAN_RESOURCE) < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "clan_resource_grab package_add %d player name= %s",
					pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleName);	
		return -1;
	}

	// ��¼�¹�����Դ��ȡ��¼
	player_clan_res_grab_record(pstEnv,pstZoneClan, pstPlayer, pstInfo->ResourceID,pstReq->Num,uiClanContri);

	{
		OPCLANDATA stData;
		stData.ClanGrab.ClanGrabID = pstInfo->ResourceID;
		stData.ClanGrab.ClanGrabNum = pstReq->Num;
		stData.ClanGrab.ClanContr = uiClanContri;

		z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
		OP_CLAN_GRAB,&stData);
	}

	return 0;
}

int player_clan_resource_lock_switch(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANRESOURCELOCKSWITCH *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANRESOURCEINFO *pstInfo = NULL;

	if (!pstZoneClan)
	{
		return -1;
	}

	pstInfo = clan_resource_get_info_by_pos(pstEnv,pstZoneClan,pstReq->ResourceID);

	if (!pstInfo)
	{
		return -1;
	}

	if (clan_resource_check_grid_permission(pstEnv, pstZoneClan, pstPlayer,
		                                   pstInfo) < 0)
	{
		return -1;
	}

	switch(pstReq->LockState)
	{
	case CLAN_GRID_LOCKED:
	case CLAN_GRID_LIMITED:
	case CLAN_GRID_OPEN:
		break;

	default:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		               ZONEERR_CLAN63);
		return -1;
	}

	pstInfo->State = pstReq->LockState;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	clan_resource_notify(pstEnv,pstZoneClan,pstInfo);
	return 0;
}
/*
int player_clan_storage_grid_destroy(ZONESVRENV *pstEnv, Player *pstPlayer,
		                             const CSCLANSTORAGEGRIDDESTROY *pstReq)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANSTORAGEGRID *pstGrid = clan_freed_valid_grid(pstEnv, pstZoneClan,
	                                                 pstPlayer,
	                                                 &pstReq->GridIdentifier);
	
	// TODO: ���UI���

	if (!pstGrid ||
		clan_storage_check_grid_permission(pstEnv, pstZoneClan, pstPlayer,
		                                   pstGrid) < 0)
	{
		return -1;
	}

	clan_storage_grid_destroy(pstEnv, pstZoneClan, pstGrid,
	                          &pstReq->GridIdentifier,pstPlayer->stRoleData.RoleName);

	return 0;
}
*/

int player_clan_commit_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 Npc *pstNpc)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	Monster *pstMachine = NULL;

	UNUSED(pstNpc);

	if (!pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		           ZONEERR_CLAN71);
		return -1;
	}

	if (!(pstMachine = z_id_monster(pstEnv->pstAppCtx, pstEnv,
		                      pstPlayer->stOnline.stMachineInfo.iMachineMonID)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN72);
		return -1;
	}

	if (this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID !=
		pstZoneClan->stClan.ClanProf.GID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN73);
		return -1;
	}

	if (mon_machine_back(pstEnv, pstMachine) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN74);
		return -1;
	}
	return 0;
}

int player_clan_commit_ore(ZONESVRENV *pstEnv, Player *pstPlayer, Npc *pstNpc, ROLEGRID *pastRoleGrid, int iNum)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	UNUSED(pstNpc);

	if (!pstZoneClan)
	{
		/*
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,
		           "�㲻�ǹ����Ա, ���ܽ��д˲���");
		*/
		return -1;
	}



	if (clan_machine_commit_ore(pstEnv, pstZoneClan, pstPlayer,
	                             pastRoleGrid, iNum) < 0)
	{
		return -1;		
	}

	return 0;
}

int clan_resource_add_entries_in(ZONESVRENV *pstEnv, CLANRESOURCE *pstResource,
                                      int iID, int iNum,CLANRESOURCEINFO* pstNotify,int* piRealNum,ZoneClan* pstClan)
{
	const CLANPRODUCTDEF* pstProductDef = clan_product_def(pstEnv, iID);
	CLANRESOURCEINFO* pstResouceInfo = NULL;
	long long llTmp = 0;
	int iSum = 0;
	int i = 0;
	int iIdx = -1;
	CLANLEVELDEF* pstClanLevel = NULL;	
	int iFirstEmpty = -1;

	if (!pstProductDef || iNum < 1 || !pstClan)
	{
		return -1;
	}	
	
	if (pstProductDef->ResourceID)//��Դ
	{
		for(i = 0; i < MAX_CLAN_RESOURCE; i++)
		{
			//�ҵ�ͬid,�����ҵ����һ��,���ߵ�ǰ�����Ѿ�Ϊ0��
			if (pstResource->ResourceInfo[i].ResourceID == pstProductDef->ResourceID)
			{
				iIdx = i;
				break;
			}

			if (i >= pstResource->ResourceNum)
			{
				break;
			}

			if (pstResource->ResourceInfo[i].Num == 0 &&
				-1 == iFirstEmpty)
			{
				iFirstEmpty = i;
			}
		}
		
		if (iIdx < 0 )
		{
			if (iFirstEmpty >= 0)
			{
				iIdx = iFirstEmpty;
			}
			else
			{
				iIdx = pstResource->ResourceNum;
			}
		}
		
		if (iIdx < 0 || iIdx >= MAX_CLAN_RESOURCE)
		{
			return -1;
		}
		
		pstResouceInfo = &pstResource->ResourceInfo[iIdx];

		if (pstResouceInfo->Num > 0 &&
			pstResouceInfo->ResourceID != pstProductDef->ResourceID)
		{
			return -1;
		}
		
		pstResouceInfo->Pos = iIdx;
		
		pstResouceInfo->ResourceID = pstProductDef->ResourceID;
		
		if (iIdx >= pstResource->ResourceNum)
		{
			pstResource->ResourceNum = iIdx + 1;
			pstResouceInfo->Num = 0;
		}		

		
	}
	else//��Ʒ
	{
		for(i = 0; i < MAX_CLAN_RESOURCE_GRID_NUM; i++)
		{
			//�ҵ�ͬid,�����ҵ����һ��,��ǰ�����Ѿ�Ϊ0
			if (pstResource->ItemInfo[i].ResourceID == iID)
			{
				iIdx = i;
				break;
			}

			if (i >= pstResource->ItemNum)
			{
				break;
			}

			if (pstResource->ItemInfo[i].Num == 0 &&
				-1 == iFirstEmpty)
			{
				iFirstEmpty = i;
			}
		}
		
		if (iIdx < 0 )
		{
			if (iFirstEmpty >= 0)
			{
				iIdx = iFirstEmpty;
			}
			else
			{
				iIdx = pstResource->ItemNum;
			}
		}
				
		if (iIdx < 0 || iIdx >= MAX_CLAN_RESOURCE_GRID_NUM)
		{
			return -1;
		}
		
		pstResouceInfo = &pstResource->ItemInfo[iIdx];

		if (pstResouceInfo->Num > 0 &&
			pstResouceInfo->ResourceID != iID)
		{
			return -1;
		}
		
		pstResouceInfo->ResourceID = iID;
		pstResouceInfo->Pos = iIdx;

		if (iIdx >= pstResource->ItemNum)
		{
			pstResource->ItemNum = iIdx + 1;
			pstResouceInfo->Num = 0;
			pstResouceInfo->State = CLAN_GRID_LOCKED;
		}
	}
	
	if (!pstResouceInfo)
	{
		return -1;
	}
	
	llTmp = (long long)pstResouceInfo->Num + (long long)iNum;
	
	iSum = pstResouceInfo->Num + iNum;

	if (llTmp != (long long)iSum)
	{
		return -1;
	}
	
	pstResouceInfo->Num += iNum;

	pstClanLevel = z_clan_level_def(pstEnv,pstClan->stClan.ClanProf.Level);
	if (pstClanLevel)
	{
		int k = 0;
		RESOURCELIMIT* pstLimit = NULL;
		for(k = 0; k < MAX_CLAN_RESOURCE; k++)
		{
			pstLimit = &pstClanLevel->ResourceLimit[k];
			if (pstLimit->ResourceID == 0)
			{
				break;
			}
			if (pstLimit->ResourceID == pstResouceInfo->ResourceID)
			{

				if (pstResouceInfo->Num >= pstLimit->Limit)
				{
					if (pstResouceInfo->Num - iNum < pstLimit->Limit)
					{
						player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
    	    	  					ZONEERR_CLANBROAD251,
        	  					pstProductDef->Name);
					}
					
					pstResouceInfo->Num = pstLimit->Limit;				

				}
				break;
			}
		}
		
	}

	if (pstNotify)
	{
		(*pstNotify) = (*pstResouceInfo);
	}

	if (piRealNum)
	{
		(*piRealNum) = iNum;
	}
	
	return 0;
}

int clan_resource_dec_entries_in(ZONESVRENV *pstEnv, CLANRESOURCE *pstResource,
                                      int iID, int iNum,CLANRESOURCEINFO* pstNotify)
{
	const CLANPRODUCTDEF* pstProductDef = clan_product_def(pstEnv, iID);
	CLANRESOURCEINFO* pstResouceInfo = NULL;
	int i = 0;
	int iIdx = -1;
	

	if (!pstProductDef)
	{
		return -1;
	}
	
	
	if (pstProductDef->ResourceID)//��Դ
	{
		for(i = 0; i < MAX_CLAN_RESOURCE; i++)
		{
			if (i >= pstResource->ResourceNum)
			{
				break;
			}
			//�ҵ�ͬid
			if (pstResource->ResourceInfo[i].ResourceID == pstProductDef->ResourceID)
			{
				iIdx = i;
				break;
			}

		}
		if (iIdx < 0 || iIdx >= pstResource->ResourceNum)
		{
			return -1;
		}
		
		pstResouceInfo = &pstResource->ResourceInfo[iIdx];

	}
	else//��Ʒ
	{
		for(i = 0; i < MAX_CLAN_RESOURCE_GRID_NUM; i++)
		{
			if (i >= pstResource->ItemNum)
			{
				break;
			}
			
			//�ҵ�ͬid
 			if (pstResource->ItemInfo[i].ResourceID == iID && 
				pstResource->ItemInfo[i].Num >= iNum )
			{
				iIdx = i;
				break;
			}
		}
		if (iIdx < 0 || iIdx >= pstResource->ItemNum)
		{
			return -1;
		}
		pstResouceInfo = &pstResource->ItemInfo[iIdx];
	}
	
	if (!pstResouceInfo || (pstResouceInfo->ResourceID != iID))
	{
		return -1;
	}
	
	if (pstResouceInfo->Num < iNum)
	{
		return -1;
	}
	
	pstResouceInfo->Num -= iNum;

	if (pstNotify)
	{
		(*pstNotify) = (*pstResouceInfo);
	}
	
	return 0;
}

int clan_resource_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      CLANRESOURCEINFO* pstNotify)
{
	CSPKG stPkg;

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_RESOURCE_NOTIFY;
	stPkg.Body.ClanSvr.ClanSvrData.ClanResourceNotify = *pstNotify;	

	Z_CSHEAD_INIT(&stPkg.Head,CLAN_SVR);	
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}


int clan_resource_add_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum,int* piRealNum)
{
	CLANRESOURCEINFO stNotify;
	int iRet = 0;

	iRet = clan_resource_add_entries_in(pstEnv,&pstZoneClan->stClan.ClanAdvance.Resource,iID,iNum,&stNotify,piRealNum,pstZoneClan);
	if (iRet < 0)
	{
		return iRet;
	}	

	clan_resource_notify(pstEnv,pstZoneClan,&stNotify);
	
	return 0;
}




CLANRESOURCEINFO* clan_resource_get_info(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID)
{
	const CLANPRODUCTDEF* pstProductDef = clan_product_def(pstEnv, iID);
	CLANRESOURCE* pstResource = &pstZoneClan->stClan.ClanAdvance.Resource;
	int i = 0;
	
	if (!pstProductDef)
	{
		return NULL;
	}	
	
	if (pstProductDef->ResourceID)//��Դ
	{
		for(i = 0; i < MAX_CLAN_RESOURCE; i++)
		{
			
			if (i >= pstResource->ResourceNum)
			{
				return NULL;
			}
 			if (pstResource->ResourceInfo[i].ResourceID == pstProductDef->ResourceID)
			{
				return &pstResource->ResourceInfo[i];
			}
		}

	}
	else//��Ʒ
	{
		for(i = 0; i < MAX_CLAN_RESOURCE_GRID_NUM; i++)
		{
			
			if (i >= pstResource->ItemNum)
			{
				return NULL;
			}
 			if (pstResource->ItemInfo[i].ResourceID == iID)
			{
				return &pstResource->ItemInfo[i];
			}
		}

	}
	
	return NULL;
}


int clan_resource_dec_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum)
{
	CLANRESOURCEINFO stNotify;
	int iRet = 0;

	iRet = clan_resource_dec_entries_in(pstEnv,&pstZoneClan->stClan.ClanAdvance.Resource,iID,iNum,&stNotify);
	if (iRet < 0)
	{
		return iRet;
	}	

	clan_resource_notify(pstEnv,pstZoneClan,&stNotify);
	return 0;
}

int clan_commit_item_byid(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iItemID,int iNum)
{
	
	CLANPRODUCTDEF* pstProductDef = NULL;
	int iRealNum = 0;


	if (!pstZoneClan)
	{		
		return -1;
	}

	pstProductDef = clan_product_def(pstEnv, iItemID);
	if (!pstProductDef || !pstProductDef->ResourceID)//Ŀǰֻ�ύ��Դ
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0,
		           "Can not get commit clan resource, ItemID:%d,Num:%d",iItemID,iNum);
		return -1;
	}


	if (clan_resource_add_entries(pstEnv, pstZoneClan,
										  pstProductDef->ID,
										  iNum,&iRealNum) < 0)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0,
		           "Can not get commit clan resource, ItemID:%d,Num:%d",iItemID,iNum);
		return -1;
	}
	
	{
		OPCLANDATA stData;
		stData.ClanResourceGather.ResouceID = pstProductDef->ID;
		stData.ClanResourceGather.ResouceNum = iRealNum;
		stData.ClanResourceGather.TaskID = 0;

		z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
		OP_CLAN_RESOURCE_GATHER,&stData);
	}


	return 0;

}

int player_clan_commit_item_byid(ZONESVRENV *pstEnv, Player *pstPlayer, int iItemID,int* piNum)
{
	float rTotalOwnedContribut = 0.0f;
	int iRet = -1;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	int iMinNum = 1;
	float fContributeUnit = 0.0f;
	CLANPRODUCTDEF* pstProductDef = NULL;
	int iContributeGroup = 0;
	int iNum = *piNum;
	int iRealNum = 0;


	if (!pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN75);
		return -1;
	}

	pstProductDef = clan_product_def(pstEnv, iItemID);
	if (!pstProductDef || !pstProductDef->ResourceID)//Ŀǰֻ�ύ��Դ
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN76);
		return -1;
	}

	//��С�ύ��
	fContributeUnit = pstProductDef->ContributValue;
	iMinNum = clan_get_commit_min_num(fContributeUnit);
	if (fContributeUnit > 0.0)
	{
		iContributeGroup = (int)max(fContributeUnit*iMinNum,1.0f);
	}
	
	if (iNum < iMinNum && iMinNum > 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
				    ZONEERR_CLAN77, pstProductDef->Name,iMinNum);
		return -1;
	}

	if (iNum > z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
				   ZONEERR_CLAN78, pstProductDef->Name,iNum);
		return -1;
	}
	

	iNum = (iNum /iMinNum) * iMinNum;

	if (clan_resource_add_entries(pstEnv, pstZoneClan,
										  pstProductDef->ID,
										  iNum,&iRealNum) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN79);
		goto final;
	}

	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstProductDef->ID,
				iNum, LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CLAN_RESOURCE);		
	
	rTotalOwnedContribut += iContributeGroup * iNum/iMinNum;

	//z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "���򹫻���Դ�����ύ��[%s]��%d",
			  // pstProductDef->Name, iNum);

	*piNum = iNum;

	{
		OPCLANDATA stData;
		stData.ClanResourceGather.ResouceID = pstProductDef->ID;
		stData.ClanResourceGather.ResouceNum = iRealNum;
		stData.ClanResourceGather.TaskID = 0;

		z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
		OP_CLAN_RESOURCE_PLAYER_COMMIT,&stData);
	}

	

final:
	if (rTotalOwnedContribut >= 1.0f)
	{
		g_iOptype = OP_OPTYPE_COMMIT;
		g_iOpsubtype = iItemID;
		iRet = clan_resource_inc(pstEnv, pstZoneClan, pstPlayer,
								 CLAN_OWNED_CONTRIBUT, rTotalOwnedContribut);
		g_iOptype = 0;
		g_iOpsubtype = 0;
	}
	else
	{
		iRet = 0;
	}
	return iRet;

}

int player_clan_commit_item(ZONESVRENV *pstEnv, Player *pstPlayer, int iGridIdx)
{

	float rTotalOwnedContribut = 0.0f;
	int iRet = -1;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	int iMinNum = 1;
	float fContributeUnit = 0.0f;
	CLANPRODUCTDEF* pstProductDef = NULL;
	int iNum = 0;
	ROLEITEM *pstItem = NULL;
	int iPos = 0;
	int iContributeGroup = 0;

	if (!pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN71);
		return -1;
	}

	iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids, pstPackage->SundriesPackage.Num, iGridIdx);
	if (0 > iPos)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN80);
		return -1;
	}

	pstItem =
		pstPackage->SundriesPackage.RoleGrids[iPos].Type == GRID_ITEM_ITEM ?
		&pstPackage->SundriesPackage.RoleGrids[iPos].GridData.RoleItem : NULL;

	if (!pstItem || !pstItem->Num)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN81);
		return 0;
	}

	pstProductDef = clan_product_def(pstEnv, pstItem->DefID);
	if (!pstProductDef || !pstProductDef->ResourceID)//Ŀǰֻ�ύ��Դ
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN81);
		return 0;
	}

	//��С�ύ��
	fContributeUnit = pstProductDef->ContributValue;
	iMinNum = clan_get_commit_min_num(fContributeUnit);

	if (fContributeUnit > 0.0)
	{
		iContributeGroup = (int)max(fContributeUnit*iMinNum,1.0f);
	}
	
	if (pstItem->Num < iMinNum && iMinNum > 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
		           ZONEERR_CLAN77, pstProductDef->Name,iMinNum);
		return 0;
	}
	

	iNum = (pstItem->Num /iMinNum) * iMinNum;

	if (clan_resource_add_entries(pstEnv, pstZoneClan,
	                                      pstItem->DefID,
	                                      iNum,NULL) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
		           				ZONEERR_CLAN79);
		goto final;
	}

	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItem->DefID,
	            iNum, LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CLAN_RESOURCE);		
	
	rTotalOwnedContribut += iContributeGroup * iNum/iMinNum;

	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN82,
	           pstProductDef->Name, iNum);

	

final:
	if (rTotalOwnedContribut >= 1.0f)
	{
		g_iOptype = OP_OPTYPE_COMMIT;
		g_iOpsubtype = pstItem->DefID;
		iRet = clan_resource_inc(pstEnv, pstZoneClan, pstPlayer,
		                         CLAN_OWNED_CONTRIBUT, rTotalOwnedContribut);
		g_iOptype = 0;
		g_iOpsubtype = 0;
	}
	else
	{
		iRet = 0;
	}
	return iRet;
}

int player_clan_machine_ensure_down(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	Monster *pstMachine = NULL;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstZoneClan || !is_player_ride_on_machine(pstPlayer))
	{
		return 0;
	}

	if (!(pstMachine = z_id_monster(pstEnv->pstAppCtx, pstEnv,
		                      pstPlayer->stOnline.stMachineInfo.iMachineMonID)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "Can not get monster instance, id:%d",
		           pstPlayer->stOnline.stMachineInfo.iMachineMonID);
		return -1;
	}

	if (this_machine(pstEnv, pstMachine)->stMachineInfo.OwnerID !=
		pstZoneClan->stClan.ClanProf.GID ||
		!is_mon_machine(pstMachine))
	{
		return 0;
	}

	//��Ա�³�
	z_ride_machine_clear(pstEnv, pstMachine);
	return 0;
}

int clan_touch_check(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,int iNotify)
{
	int iYesterday = pstEnv->pstAppCtx->stCurr.tv_sec - 86400;
	
	if (IsSameDay(iYesterday,pstZoneClan->stClan.ClanCore.YesterdayClanTouchTime))
	{
		return 0;
	}
	else
	{
		pstZoneClan->stClan.ClanCore.YesterdayClanTouchTime = 0;
		pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum = 0;
	}

	if (IsSameDay(iYesterday,pstZoneClan->stClan.ClanCore.ClanTouchTime))
	{
		pstZoneClan->stClan.ClanCore.YesterdayClanTouchTime = pstZoneClan->stClan.ClanCore.ClanTouchTime;
		pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum = pstZoneClan->stClan.ClanCore.ClanTouchNum;

		
	}

	if (!IsSameDay(pstZoneClan->stClan.ClanCore.ClanTouchTime,pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZoneClan->stClan.ClanCore.ClanTouchTime = 0;
		pstZoneClan->stClan.ClanCore.ClanTouchNum = 0;
	}

	if (iNotify)
	{
		player_clan_yesterday_clan_touch_notify_broadcast(pstEnv,pstZoneClan);
	}
	return 0;
}

