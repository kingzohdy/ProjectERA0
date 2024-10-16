
#pragma once


#include "TerrainTile.h"
#include "GameResDBTypes.h"

namespace TE
{

class MNPC
{
public:
	MNPC(const char* filename );
	MNPC(const char* npcName, const char* modelPath, unsigned int npcID ,MVec3 pos ,MVec3 dir,const char* script);
    

	~MNPC(void);

	void  Release();

	MVec3 GetPos();
	MVec3 GetDir();

	void  SetPos(MVec3 pos);
	void  SetRot(float rx,float ry,float rz);
	void  SetScale(float scale);
	void  SetNpcName(const char* filename);

	void  SetMapID( int  mapId){ m_nNPCID = mapId ;};
	int   GetMapID( ) { return m_nNPCID ; };

	char* GetNpcName(){return m_strNPCName;};

	TerrModel*    GetModel(){return &m_Model;};
public:
	TerrModel       m_Model;
	int               m_nNPCID;
	MVec3             m_Pos;
	MVec3             m_Dir;
	float             m_fScale;
	std::string       m_NPCPath;
	std::string       m_NPCScript;
	char              m_strNPCName[32];
	int               m_nMapID;
	int               m_nType;
};


class MapNpcManager
{
public:
	MapNpcManager();
	~MapNpcManager();

	bool AddNpc(MNPC *npc);
	bool DeleteIthNpc(int index);
	MNPC*  FindNpc( unsigned int npcID );

	void   SetCurNpcPos(MVec3 pos);
	void   SetCurNpcRot(float rx,float ry,float rz);
	void   SetCurNpcScale(float scale);

	void   Release();


	bool save(const char *filename);
	bool load(const char *filename);

	bool   SaveDB(int mapid, bool gen_jingying);
	bool   LoadDB(int mapid);

	void  Update(float tick);
	MNPC*    GetNpc(int index);
	int findNpcByModel(TerrModel *pmodel);

public:
	std::vector<MNPC*>    m_NpcSet;
};

};
