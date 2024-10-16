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

	//���ڷ���true������false
	bool checkDBDataID(int id, string tableName,char* key);

	//////////////////////////////////////////////////////////////////////////

	//���״̬ (buff)id �Ƿ����
	bool checkStatusID(int id,int level);

	//��鼼���Ƿ����
	bool checkSkillID(int id,int level);

	void startCheckIconPath();	//�������ͼ��·��
	void startCheckTaskTable();	//���task��
	void startCheckNpcInfoTable();	//npcinfo��
	void startCheckMapNpcTable();	//mapnpc��
	void startCheckMapMonsterTable();	//mapmonster��
	void startCheckPetTable();	//pet��
	void startCheckRideTable();	//ride��
	void startCheckItemTable();	//item ��
	void startCheckNpcSellList();	//npcSelllist��
	void startCheckMonsterTable();	//monsterDef��
	void startCheckBootyTable();	//BootyGroupDef,DiffBootyDef,WorldBootyDef��


private:
	Ogre::ResDB *m_pResDB;
	int m_nStartID;
	int m_nEndID;
	int m_nCurID;
};

extern CResourceCheck g_resourceCheck;
