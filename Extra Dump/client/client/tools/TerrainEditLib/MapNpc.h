
#pragma once


#include "TerrainTile.h"
#include "GameResDBTypes.h"

namespace TE
{

	class MapNpc
	{
	public:
		MapNpc(const char* filename );
		MapNpc(const char* npcName, const char* modelPath, unsigned int npcID ,Ogre::Vector3 pos ,Ogre::Vector3 dir,const char* script);


		~MapNpc(void);

		void  Release();

		Ogre::Vector3 GetPos();
		Ogre::Vector3 GetDir();

		void  SetPos(Ogre::Vector3 pos);
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
		Ogre::Vector3             m_Pos;
		Ogre::Vector3             m_Dir;
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

		bool AddNpc(MapNpc *npc);
		bool DeleteIthNpc(int index);
		void deleteAllNpc();
		MapNpc*  FindNpc( unsigned int npcID );

		TerrModel *intersectRay(Ogre::Ray &ray);
		void onSelectByModel(TerrModel *pmodel);

		void   Release();


		bool save(const char *filename);
		bool load(const char *filename);

		bool   SaveDB(int mapid, bool gen_jingying);
		bool   LoadDB(int mapid);

		void  Update(float tick);
		MapNpc*    GetNpc(int index);
		int findNpcByModel(TerrModel *pmodel);
		void onCull();

	public:
		std::vector<MapNpc*>    m_NpcSet;
	};

};
