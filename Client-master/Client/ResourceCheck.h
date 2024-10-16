#pragma once
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreResDB.h"
#include <string>
#include "OgreSingleton.h"

using namespace std;

class CResourceCheck:public Ogre::Singleton<CResourceCheck>
{
public:
	CResourceCheck();
	~CResourceCheck();

	bool loadDB();
	void startCheck();

	void checkItemTips(int nBegin,int nEnd);
	bool checkNextTips();

private:
	template<typename T>
	void loadDBDef(std::vector<T>& def,string tableName);

	//存在返回true，否则false
	bool checkDBDataID(int id, string tableName,char* key);

	//////////////////////////////////////////////////////////////////////////

	//检查状态 (buff)id 是否存在
	bool checkStatusID(int id,int level);

	//检查技能是否存在
	bool checkSkillID(int id,int level);

	void startCheckIconPath();	//检查所有图标路径
	void startCheckTaskTable();	//检查task表
	void startCheckNpcInfoTable();	//npcinfo表
	void startCheckMapNpcTable();	//mapnpc表
	void startCheckMapMonsterTable();	//mapmonster表
	void startCheckPetTable();	//pet表
	void startCheckRideTable();	//ride表
	void startCheckItemTable();	//item 表
	void startCheckNpcSellList();	//npcSelllist表
	void startCheckMonsterTable();	//monsterDef表
	void startCheckBootyTable();	//BootyGroupDef,DiffBootyDef,WorldBootyDef表


private:
	Ogre::ResDB *m_pResDB;
	int m_nStartID;
	int m_nEndID;
	int m_nCurID;
};

extern CResourceCheck g_resourceCheck;
