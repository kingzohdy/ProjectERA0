#include <vector>
#include <string>
#include "ResourceCheck.h"
#include "OgreLog.h"
#include "../uilib/ui_framemgr.h"
#include "GameManager.h"

#include <windows.h>
using namespace Ogre;


#define LOGL_RESCOURCE	64	//���ش���ʹ�ã����ø�ͷ�ļ��ˡ�����
#define LOG_RESOURCE	LogSetCurParam(__FILE__, __LINE__, LOGL_RESCOURCE),	LogMessage


CResourceCheck g_resourceCheck;

CResourceCheck::CResourceCheck():m_pResDB(NULL),
								m_nCurID(-1),
								m_nEndID(-1),
								m_nStartID(-1)
{}

CResourceCheck::~CResourceCheck()
{
	delete m_pResDB;
}

bool CResourceCheck::loadDB()
{
	if (m_pResDB == NULL)
	{
		GameConfig cfg = GameManager::getSingleton().GetConfig();
		m_pResDB = CreateResDBFromRealDB("db/resdb_meta.tdr", cfg.resdb_server, cfg.resdb_database, cfg.resdb_user, cfg.resdb_passwd);
		if(m_pResDB == NULL)
		{
			return false;
		}
	}
	return true;
}

void CResourceCheck::startCheck()
{
	Ogre::LogAddFileHandler("log\\Resource.log",LOGL_RESCOURCE);

 	startCheckIconPath();	//�������ͼ��·��
	startCheckTaskTable();	//���task��
	startCheckNpcInfoTable();	//npcinfo��
	startCheckMapNpcTable();	//mapnpc��
	startCheckMapMonsterTable();	//mapmonster��
	startCheckPetTable();	//pet��
	startCheckRideTable();	//ride��
	startCheckItemTable();	//item ��
	startCheckNpcSellList();	//npcSelllist��
	startCheckMonsterTable();	//monsterDef��
	startCheckBootyTable();	//BootyGroupDef,DiffBootyDef,WorldBootyDef��
}

template<typename T>
void CResourceCheck::loadDBDef(std::vector<T>& def,string tableName)
{
	assert(m_pResDB != NULL);
	ResTable *ptable = m_pResDB->GetTable(tableName.c_str());
	int num = ptable->FindRecord(NULL, 0, NULL);
	def.resize(num);
	if(num > 0) ptable->FindRecord(&def[0], num, NULL);
}

bool CResourceCheck::checkDBDataID(int id, string tableName,char* key)
{
	//TODO HERE ��ID Ϊ 0 ���ж�
	ResTable* ptable = m_pResDB->GetTable(tableName.c_str());
	if(ptable->FindRecord(NULL,0,key,id) > 0 )
	{
		return true;
	}
	//TODO HERE write error log
	return false;
}


bool CResourceCheck::checkStatusID(int id,int level)
{
	assert( m_pResDB != NULL);
	ResTable* pStatusTable = m_pResDB->GetTable("StatusDef");
	STATUSDEF statusSrc;
	statusSrc.StatusID = id;
	statusSrc.Level = level;
	if (pStatusTable->FindRecord(NULL,0,&statusSrc) > 0 )
	{
		return true;
	}
	return false;
}

bool CResourceCheck::checkSkillID(int id,int level)
{
	assert( m_pResDB != NULL );
	ResTable* pSkillTable = m_pResDB->GetTable("SkillDef");
	SKILLDEF skillSrc;
	skillSrc.SkillID = id;
	skillSrc.Level = level;
	if( pSkillTable->FindRecord(NULL,0,&skillSrc) > 0)
	{
		return true;
	}
	return false;
}


void CResourceCheck::startCheckIconPath()
{

	// skill
	vector<tagSkillDef> skilldef;
	loadDBDef(skilldef,"SkillDef");
	LOG_RESOURCE("*******************************************************************");
	LOG_RESOURCE("                         SKILL ICONS                               ");
	LOG_RESOURCE("*******************************************************************");
	for( size_t i = 0 ; i < skilldef.size(); i++ )
	{
		if( skilldef[i].SkillID >= 10000 && skilldef[i].SkillID <30000 )
		{
			//���＼��
			continue;
		}
		if( skilldef[i].IconID1 == 0 )
		{
			LOG_RESOURCE("[ERROR] skill id %d's iconid is null!",skilldef[i].SkillID);
		}else{
			char szIconPath[MAX_PATH] = {0};
			sprintf(szIconPath,"uires\\TuPianLei\\JiNeng\\JiNeng\\%d.tga",skilldef[i].IconID1);
			bool bFound = false;
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find skill icon!id is %d,path is %s",skilldef[i].SkillID,szIconPath);
			}
		}
	}

	//monster and npc
	vector<MONSTERDEF> monsterDef;
	loadDBDef(monsterDef,"MonsterDef");
	LOG_RESOURCE("\n*******************************************************************");
	LOG_RESOURCE("                        MONSTER ICONS                              ");
	LOG_RESOURCE("*******************************************************************\n");
	for( size_t i = 0 ; i < monsterDef.size(); i++ )
	{
		if( monsterDef[i].IconID == 0 )
		{
			LOG_RESOURCE("[ERROR] monster id %d's iconid is null!",monsterDef[i].MonsterID);
		}
		else{
			char szIconPath[MAX_PATH] = {0};
			sprintf(szIconPath,"uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\%d.tga",monsterDef[i].IconID);
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find monster icon!id is %d,path is %s",monsterDef[i].MonsterID,szIconPath);
			}
		}
	}

	//item
	vector<ITEMDEF> itemDef;
	loadDBDef(itemDef,"ItemDef");
	LOG_RESOURCE("\n*******************************************************************");
	LOG_RESOURCE("                          ITEM ICONS                               ");
	LOG_RESOURCE("*******************************************************************\n");
	for( size_t i = 0 ; i < itemDef.size(); i++ )
	{
		if( itemDef[i].IconID == 0 )
		{
			LOG_RESOURCE("[ERROR] item id %d's iconid is null!",itemDef[i].ItemID);
		}else{
			char szIconPath[MAX_PATH] = {0};
			sprintf(szIconPath,"uires\\TuPianLei\\WuPin\\%d.tga",itemDef[i].IconID);
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find item icon!id is %d,path is %s",itemDef[i].ItemID,szIconPath);
			}
			if( itemDef[i].ItemTmp.Type == ITEM_TMP_ITEM ||itemDef[i].ItemTmp.Type == ITEM_TMP_ARM ||itemDef[i].ItemTmp.Type == ITEM_TMP_MACHINE )
			{
				sprintf(szIconPath,"uires\\TuPianLei\\LinShiWuPin\\%d.tga",itemDef[i].IconID );
				if( IsInExistence(szIconPath) )
				{
					LOG_RESOURCE("[ERROR] Can't find item icon!id is %d,path is %s",itemDef[i].ItemID,szIconPath);
				}
			}
		}
	}

	//buff
	vector<STATUSDEF> statusDef;
	loadDBDef(statusDef,"StatusDef");
	LOG_RESOURCE("\n*******************************************************************");
	LOG_RESOURCE("                          STATUS ICONS                             ");
	LOG_RESOURCE("*******************************************************************\n");
	for( size_t i = 0 ; i < statusDef.size(); i++ )
	{
		if( statusDef[i].IconID == 0 )
		{
			LOG_RESOURCE("[ERROR] status id %d's iconid is null!",statusDef[i].StatusID);
		}else{
			char szIconPath[MAX_PATH] = {0};
			sprintf (szIconPath,"uires\\TuPianLei\\JiNeng\\Buff\\%d.tga",statusDef[i].IconID);
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find status icon!id is %d,path is %s",statusDef[i].StatusID,szIconPath);
			}
		}
	}

	//pet
	vector<PETDEF> petDef;
	loadDBDef(petDef,"PetDef");

	LOG_RESOURCE("\n*******************************************************************");
	LOG_RESOURCE("                           PET ICONS                               ");
	LOG_RESOURCE("*******************************************************************\n");
	for( size_t i = 0 ; i < petDef.size(); i++ )
	{
		if( petDef[i].IconID == 0 )
		{
			LOG_RESOURCE("[ERROR] pet id %d's iconid is null!",petDef[i].PetID);
		}else{
			char szIconPath[MAX_PATH] = {0};
			sprintf(szIconPath,"uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\%d.tga",petDef[i].IconID);
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find pet icon!id is %d,path is %s",petDef[i].PetID,szIconPath);
			}
		}
	}

	//����
	vector<FAIRYDEF> fairyDef;
	loadDBDef(fairyDef,"FairyDef");

	LOG_RESOURCE("\n*******************************************************************");
	LOG_RESOURCE("                          FAIRY ICONS                              ");
	LOG_RESOURCE("*******************************************************************\n");
	for( size_t i = 0 ; i < fairyDef.size(); i++ )
	{
		if( fairyDef[i].ModeID == 0 )
		{
			LOG_RESOURCE("[ERROR] fairy id %d's iconid is null!",fairyDef[i].FairyID);
		}else{
			char szIconPath[MAX_PATH] = {0};
			sprintf(szIconPath,"uires\\TuPianLei\\TouXiang\\JingLingTouXiang\\%d.tga",fairyDef[i].ModeID);
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find fairy icon!id is %d,path is %s",fairyDef[i].FairyID,szIconPath);
			}
			sprintf(szIconPath,"uires\\TuPianLei\\TouXiang\\WuPin\\%d.tga",fairyDef[i].ModeID);
			if( !IsInExistence(szIconPath) )
			{
				LOG_RESOURCE("[ERROR] Can't find fairy icon!id is %d,path is %s",fairyDef[i].FairyID,szIconPath);
			}
		}
	}
}




void CResourceCheck::startCheckTaskTable()
{
	vector<tagTaskDef> taskDef;
	loadDBDef(taskDef,"TaskDef");
	char szMsg[512] = "";
	for( size_t index = 0; index < taskDef.size() ; index ++ )
	{
		tagTaskDef& task = taskDef[index];

		for( int i = 0; i < MAX_TASK_ITEM ; i++ )
		{
			if( task.GetItem[i].ItemID != 0 )
			{
				if(!checkDBDataID(task.GetItem[i].ItemID,"ItemDef","ItemID"))
				{
					// ������					
					sprintf(szMsg,"TaskDef��IDΪ%d��GetItem%d��itemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.GetItem[i].ItemID);
					LOG_RESOURCE(szMsg);
				}
			}
		}
		for( int i = 0 ; i < MAX_TASK_MON  ; i ++)
		{
			if(task.KillMon[i].MonID1 != 0 )
			{
				if( !checkDBDataID(task.KillMon[i].MonID1,"MonsterDef","MonsterID") )
				{
					//������
					sprintf(szMsg,"TaskDef��IDΪ%d��KillMon%d��MonID1Ϊ%d,��MonsterDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.KillMon[i].MonID1);
					LOG_RESOURCE(szMsg);
				}
			}
			if(task.KillMon[i].MonID2 !=0 )
			{
				if( !checkDBDataID(task.KillMon[i].MonID2,"MonsterDef","MonsterID") )
				{
					//������
					sprintf(szMsg,"TaskDef��IDΪ%d��KillMon%d��MonID2Ϊ%d,��MonsterDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.KillMon[i].MonID2);
					LOG_RESOURCE(szMsg);
				}
			}
			if(task.KillMon[i].MonID3 !=0 )
			{
				if( !checkDBDataID(task.KillMon[i].MonID3,"MonsterDef","MonsterID") )
				{
					//������
					sprintf(szMsg,"TaskDef��IDΪ%d��KillMon%d��MonID3Ϊ%d,��MonsterDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.KillMon[i].MonID3);
					LOG_RESOURCE(szMsg);
				}
			}
			if(task.KillMon[i].MonID4 !=0 )
			{
				if( !checkDBDataID(task.KillMon[i].MonID4,"MonsterDef","MonsterID") )
				{
					//������
					sprintf(szMsg,"TaskDef��IDΪ%d��KillMon%d��MonID4Ϊ%d,��MonsterDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.KillMon[i].MonID4);
					LOG_RESOURCE(szMsg);
				}
			}
			if(task.KillMon[i].MonID5 !=0 )
			{
				if( !checkDBDataID(task.KillMon[i].MonID5,"MonsterDef","MonsterID") )
				{
					//������
					sprintf(szMsg,"TaskDef��IDΪ%d��KillMon%d��MonID5Ϊ%d,��MonsterDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.KillMon[i].MonID5);
					LOG_RESOURCE(szMsg);
				}
			}
		}

		for( int i = 0 ; i < MAX_TASK_AWARD_ITEM ; i ++ )
		{
			if(task.TaskAward.Items[i].ID != 0 )
			{
				if( !checkDBDataID(task.TaskAward.Items[i].ID,"ItemDef","ItemID") )
				{
					//������
					sprintf(szMsg,"TaskDef��IDΪ%d��TaskAward_Items%d��itemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",task.ID,i + 1,task.TaskAward.Items[i].ID);
					LOG_RESOURCE(szMsg);
				}
			}
		}
		if( task.TaskAward.Buff.TaskBuffID != 0 )	//status
		{
			if( !checkStatusID(task.TaskAward.Buff.TaskBuffID,task.TaskAward.Buff.TaskBuffLevel))
			{
				//������
				sprintf(szMsg,"TaskDef��IDΪ%d��TaskAward_Buff_TaskBuffIDΪ%d,levelΪ%d,��StatusDef���в���ʧ�ܣ�\n",task.ID,task.TaskAward.Buff.TaskBuffID,task.TaskAward.Buff.TaskBuffLevel);
				LOG_RESOURCE(szMsg);
			}

		}
		if( task.TaskAward.Respact.ID != 0 )
		{
			if( !checkDBDataID(task.TaskAward.Respact.ID,"RespactPartyDef","ID" ) )
			{
				//������
				sprintf(szMsg,"TaskDef��IDΪ%d��TaskAward_Respact_IDΪ%d,��RespactPartyDef���в���ʧ�ܣ�\n",task.ID,task.TaskAward.Respact.ID);
				LOG_RESOURCE(szMsg);
			}

		}		
		if( task.TaskItem != 0 )
		{
			if( !checkDBDataID(task.TaskItem,"ItemDef","ItemID") )
			{
				//������
				sprintf(szMsg,"TaskDef��IDΪ%d��TaskItem,itemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",task.ID,task.TaskItem);
				LOG_RESOURCE(szMsg);

			}
		}
		if( task.TaskGetLimit.PreTaskID!=0 )
		{
			if( !checkDBDataID(task.TaskGetLimit.PreTaskID,"TaskDef","ID") )
			{
				//������
				sprintf(szMsg,"TaskDef��IDΪ%d��TaskGetLimit_PreTaskIDΪ%d,��TaskDef���в���ʧ�ܣ�\n",task.ID,task.TaskGetLimit.PreTaskID);
				LOG_RESOURCE(szMsg);
			}
		}
	}
}

void CResourceCheck::startCheckNpcInfoTable()
{
	char szMsg[512] = "";
	vector<NPCINFODEF> npcinfoDef;
	loadDBDef(npcinfoDef,"NpcInfoDef");
	for( size_t index = 0; index < npcinfoDef.size(); index++)
	{
		NPCINFODEF& npc = npcinfoDef[index];

		if( !checkDBDataID(npc.ID,"MonsterDef","MonsterID") )
		{
			//������
			sprintf(szMsg,"NpcInfoDef��IDΪ%d��ID,��MonsterDef���в���ʧ�ܣ�\n",npc.ID);
			LOG_RESOURCE(szMsg);
		}
		if ( !checkDBDataID(npc.ID,"NpcDisplayInfo","id") )
		{
			//������
			sprintf(szMsg,"NpcInfoDef��IDΪ%d��ID,��NpcDisplayInfo���в���ʧ�ܣ�\n",npc.ID);
			LOG_RESOURCE(szMsg);
		}

		for( int j = 0 ; j < MAX_NPC_TASK ; j++ )
		{
			if(npc.AcceptTaskIDs[j] != 0 )
			{
				if( !checkDBDataID(npc.AcceptTaskIDs[j],"TaskDef","ID") )
				{
					//������
					sprintf(szMsg,"NpcInfoDef��IDΪ%d��AcceptTaskIDs_%d,taskIDΪ%d,��TaskDef���в���ʧ�ܣ�\n",npc.ID,j + 1,npc.AcceptTaskIDs[j]);
					LOG_RESOURCE(szMsg);
				}
			}
			if(npc.CommitTaskIDs[j] != 0 )
			{
				if( !checkDBDataID(npc.CommitTaskIDs[j],"TaskDef","ID") )
				{
					//������
					sprintf(szMsg,"NpcInfoDef��IDΪ%d��CommitTaskIDs_%d,taskIDΪ%d,��TaskDef���в���ʧ�ܣ�\n",npc.ID,j + 1,npc.CommitTaskIDs[j]);
					LOG_RESOURCE(szMsg);
				}
			}
		}

		for( int j = 0 ; j < MAX_NPC_TASKCYCLE ; j ++ )
		{
			if( npc.AcceptTaskCycleIDs[j] != 0 )
			{
				if( !checkDBDataID(npc.AcceptTaskCycleIDs[j],"TaskDef","ID") )
				{
					//������
					sprintf(szMsg,"NpcInfoDef��IDΪ%d��AcceptTaskCycleIDs_%d,taskIDΪ%d,��TaskDef���в���ʧ�ܣ�\n",npc.ID,j + 1,npc.AcceptTaskCycleIDs[j]);
					LOG_RESOURCE(szMsg);
				}
			}
			if( npc.CommitTaskCycleIDs[j] != 0 )
			{
				if( !checkDBDataID(npc.CommitTaskCycleIDs[j],"TaskDef","ID") )
				{
					//������
					sprintf(szMsg,"NpcInfoDef��IDΪ%d��CommitTaskCycleIDs_%d,taskIDΪ%d,��TaskDef���в���ʧ�ܣ�\n",npc.ID,j + 1,npc.CommitTaskCycleIDs[j]);
					LOG_RESOURCE(szMsg);
				}
			}
		}	
	}
}

void CResourceCheck::startCheckMapNpcTable()
{
	vector<MAPNPC> mapNpcDef;
	loadDBDef(mapNpcDef,"MapNpc");
	char szMsg[512] = "";
	for( size_t index = 0 ; index < mapNpcDef.size() ; index++ )
	{
		MAPNPC& npc = mapNpcDef[index];
		if( !checkDBDataID(npc.MonsterID,"MonsterDef","MonsterID") )
		{
			//������
			sprintf(szMsg,"MapNpc��MonsterIDΪ%d��MonsterID,��MonsterDef���в���ʧ�ܣ�\n",npc.MonsterID);
			LOG_RESOURCE(szMsg);
		}
		if( !checkDBDataID(npc.MonsterID,"NpcDisplayInfo","id") )
		{
			//������
			sprintf(szMsg,"MapNpc��MonsterIDΪ%d��MonsterID,��NpcDisplayInfo���в���ʧ�ܣ�\n",npc.MonsterID);
			LOG_RESOURCE(szMsg);
		}
	}
}

void CResourceCheck::startCheckMapMonsterTable()
{
	vector<MAPMONSTER> mapmonster;
	loadDBDef(mapmonster,"MapMonster");
	char szMsg[512] = "";
	for( size_t index = 0 ; index < mapmonster.size() ; index ++ )
	{
		MAPMONSTER& monster = mapmonster[index];
		if( !checkDBDataID(monster.MonsterID,"MonsterDef","MonsterID") )
		{
			//������
			sprintf(szMsg,"MapMonster��MonsterIDΪ%d��MonsterID,��MonsterDef���в���ʧ�ܣ�\n",monster.MonsterID);
			LOG_RESOURCE(szMsg);
		}
		if( !checkDBDataID(monster.MonsterID,"NpcDisplayInfo","id") )
		{
			//������
			sprintf(szMsg,"MapMonster��MonsterIDΪ%d��MonsterID,��NpcDisplayInfo���в���ʧ�ܣ�\n",monster.MonsterID);
			LOG_RESOURCE(szMsg);
		}		
	}
}

void CResourceCheck::startCheckPetTable()
{
	vector<PETDEF> petDef;
	loadDBDef(petDef,"PetDef");
	char szMsg[512] = "";
	for( size_t index = 0 ; index < petDef.size() ; index ++ )
	{
		PETDEF& pet = petDef[index];
		if( !checkDBDataID(pet.MouldID,"NpcDisplayInfo","id") )
		{
			//������
			sprintf(szMsg,"PetDef��PetIDΪ%d��MouldIDΪ%d,��NpcDisplayInfo���в���ʧ�ܣ�\n",pet.PetID,pet.MouldID);
			LOG_RESOURCE(szMsg);
		}
	}
}

void CResourceCheck::startCheckRideTable()
{
	vector<RIDEDEF> rideDef;
	loadDBDef(rideDef,"RideDef");
	char szMsg[512] = "";
	for( size_t index = 0 ; index < rideDef.size(); index ++ )
	{
		RIDEDEF ride = rideDef[index];
		if( !checkDBDataID(ride.ModelID,"NpcDisplayInfo","id") )
		{
			//������
			sprintf(szMsg,"RideDef��RideIDΪ%d��ModelIDΪ%d,��NpcDisplayInfo���в���ʧ�ܣ�\n",ride.RideID,ride.ModelID);
			LOG_RESOURCE(szMsg);
		}
	}
}

void CResourceCheck::startCheckItemTable()
{
	vector<ITEMDEF> itemDef;
	loadDBDef(itemDef,"ItemDef");
	char szMsg[512] = "";
	for( size_t index = 0; index < itemDef.size(); index ++ )
	{
		ITEMDEF& item = itemDef[index];
		if(item.ItemID < 2500000 && item.WearPos1 > 0 )
		{
			if( !checkDBDataID(item.ItemID,"ItemDisplayInfo","id") )
			{
				//������
				sprintf(szMsg,"ItemDef��ItemIDΪ%d��ItemID,��ItemDisplayInfo���в���ʧ�ܣ�\n",item.ItemID);
				LOG_RESOURCE(szMsg);
			}
		}
	}
}

void CResourceCheck::startCheckNpcSellList()
{
	vector<NPCSELLLIST> npcSellList;
	loadDBDef(npcSellList,"NpcSellList");
	char szMsg[512] = "";
	for( size_t index = 0; index < npcSellList.size(); index ++ )
	{
		NPCSELLLIST& npcSell = npcSellList[index];
		for( int j = 0 ; j < MAX_SELL_GOODS ; j ++)
		{
			if( npcSell.Goods[j].GoodsID != 0 )
			{
				if( !checkDBDataID(npcSell.Goods[j].GoodsID,"ItemDef","ItemID") )
				{
					// ������					
					sprintf(szMsg,"NpcSellList��SellListIDΪ%d��Goods_%d_GoodsIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",npcSell.SellListID,j + 1,npcSell.Goods[j].GoodsID);
					LOG_RESOURCE(szMsg);
				}
			}
		}
	}
}

void CResourceCheck::startCheckMonsterTable()
{
	vector<MONSTERDEF> monsterDef;
	loadDBDef(monsterDef,"MonsterDef");
	char szMsg[512] = "";
	for( size_t index = 0 ; index < monsterDef.size() ; index ++ )
	{
		MONSTERDEF& monster = monsterDef[index];
		for( int j = 0 ; j < MAX_MONBOOTY_ITEM ; j ++ )
		{
			if( monster.Booty.Items[j].ItemID != 0 )
			{
				if( !checkDBDataID(monster.Booty.Items[j].ItemID,"ItemDef","ItemID") )
				{
					// ������					
					sprintf(szMsg,"MonsterDef��MonsterIDΪ%d��Booty_Items%d_ItemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",monster.MonsterID,j + 1,monster.Booty.Items[j].ItemID);
					LOG_RESOURCE(szMsg);
				}
			}
		}

		for( int j = 0 ; j < MAX_NORM_SKILL ; j ++ )
		{
			if( monster.AINormalSkills[j].SkillID != 0 )
			{
				if( !checkSkillID(monster.AINormalSkills[j].SkillID,monster.AINormalSkills[j].SkillLevel) )
				{
					//������
					sprintf(szMsg,"MonsterDef��MonsterIDΪ%d��AINormalSkills%d_SkillIDΪ%d,��SkillDef���в���ʧ�ܣ�\n",monster.MonsterID,j + 1,monster.AINormalSkills[j].SkillID);
					LOG_RESOURCE(szMsg);
				}
			}
		}
		for( int j = 0 ; j < MAX_ATKEE_SKILL ; j ++)
		{
			if( monster.AIAtkeeSkills[j].SkillID != 0 )
			{
				if( !checkSkillID(monster.AIAtkeeSkills[j].SkillID,monster.AIAtkeeSkills[j].SkillLevel) )
				{
					//������
					sprintf(szMsg,"MonsterDef��MonsterIDΪ%d��AIAtkeeSkills%d_SkillIDΪ%d,��SkillDef���в���ʧ�ܣ�\n",monster.MonsterID,j + 1,monster.AIAtkeeSkills[j].SkillID);
					LOG_RESOURCE(szMsg);
				}
			}
			if( monster.AIAtkerSkills[j].SkillID != 0 )
			{
				if( !checkSkillID(monster.AIAtkerSkills[j].SkillID,monster.AIAtkerSkills[j].SkillLevel) )
				{
					//������
					sprintf(szMsg,"MonsterDef��MonsterIDΪ%d��AIAtkerSkills%d_SkillIDΪ%d,��SkillDef���в���ʧ�ܣ�\n",monster.MonsterID,j + 1,monster.AIAtkerSkills[j].SkillID);
					LOG_RESOURCE(szMsg);
				}
			}
		}
	}
}

void CResourceCheck::startCheckBootyTable()
{
// 	vector<BOOTYGROUPDEF> bootyDef;
// 	loadDBDef(bootyDef,"BootyGroupDef");
// 	char szMsg[512] = "";
// 	for( size_t index = 0 ; index < bootyDef.size() ; index ++)
// 	{
// 		BOOTYGROUPDEF& booty = bootyDef[index];
// 		for( int j = 0 ; j < MAX_BOOTY_GROUP_ITEM ; j++ )
// 		{
// 			if( booty.Items[j].ItemID != 0 )
// 			{
// 				if( !checkDBDataID(booty.Items[j].ItemID,"ItemDef","ItemID"))
// 				{
// 					//������					
// 					sprintf(szMsg,"BootyGroupDef��BootyGroupIDΪ%d��Items%d_ItemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",booty.BootyGroupID,j + 1,booty.Items[j].ItemID);
// 					LOG_RESOURCE(szMsg);
// 				}
// 			}
// 		}
// 	}
// 
// 	vector<DIFFBOOTYDEF> diffBootyDef;
// 	loadDBDef(diffBootyDef,"DiffBootyDef");
// 	for( size_t index = 0 ; index < diffBootyDef.size() ; index ++ )
// 	{
// 		DIFFBOOTYDEF& diffBooty = diffBootyDef[index];
// 		for( int j = 0 ; j < DIFF_BOOTY_MAX_ITEM_NUM && j < diffBooty.ItemNum; j++ )
// 		{
// 			if( !checkDBDataID(diffBooty.DiffBootyItem[j].ItemID,"ItemDef","ItemID") )
// 			{
// 				// ������				
// 				sprintf(szMsg,"DiffBootyDef��MonIDΪ%d��DiffBootyItem%d_ItemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",diffBooty.MonID,j + 1,diffBooty.DiffBootyItem[j].ItemID);
// 				LOG_RESOURCE(szMsg);
// 			}
// 		}
// 	}
// 	vector<WORLDBOOTYDEF> worldBootyDef;
// 	loadDBDef(worldBootyDef,"WorldBootyDef");
// 	for( size_t index = 0 ; index < worldBootyDef.size() ; index++ )
// 	{
// 		WORLDBOOTYDEF& worldBoot = worldBootyDef[index];
// 		for( int j = 0 ; j < MAX_BOOTY_GROUP_ITEM && j < worldBoot.BootyNum; j ++ )
// 		{
// 			if( !checkDBDataID(worldBoot.Items[j].ItemID ,"ItemDef","ItemID") )
// 			{
// 				//������
// 				sprintf(szMsg,"WorldBootyDef��LVΪ%d��Items%d_ItemIDΪ%d,��ItemDef���в���ʧ�ܣ�\n",worldBoot.LV,j + 1,worldBoot.Items[j].ItemID);
// 				LOG_RESOURCE(szMsg);
// 			}
// 		}
// 	}
}

void CResourceCheck::checkItemTips(int nBegin,int nEnd)
{
	if( nBegin < 0 || nEnd < 0 )
	{
		return;
	}
	if( nBegin >= nEnd )
	{
		return;
	}
	m_nStartID = nBegin;
	m_nEndID = nEnd;
	m_nCurID = nBegin;
	checkNextTips();
}

bool CResourceCheck::checkNextTips()
{
	if( m_nCurID > 0 )
	{
		char buf[128] = {0};
		sprintf(buf,"showTestTips(%d)",m_nCurID);
		GameManager::getSingleton().getScriptVM()->callString(buf);
		m_nCurID ++;
		if( m_nCurID > m_nEndID )
		{
			m_nCurID = -1;
		}
		return true;
	}
	return false;
}