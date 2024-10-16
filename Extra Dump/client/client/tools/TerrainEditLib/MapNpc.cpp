
#include "MapNpc.h"
#include "EditorModel.h"
#include "TerrainEdit.h"
#include "Terrain.h"
#include "EditorStream.h"
#include "MaskMapEdit.h"
#include "EditorMessageQue.h"

#include "ShowActor.h"
#include "GameDBData.h"

#include <windows.h>

using namespace Ogre;

namespace TE
{
	MapNpc::MapNpc(const char* filename)
	{
		strcpy(m_strNPCName,"NPC");
		m_Model.GetModelData ()->pModel = new EditorModel;
		strcpy( m_Model.GetModelData ()->name , filename ); 
		m_Model.GetModelData ()->pModel ->LoadModel ( filename ); 
		m_nMapID = 0;
		m_nType = 0;
	}

	MapNpc::MapNpc(const char* npcName, const char* modelPath, unsigned int npcID , Ogre::Vector3 pos ,Ogre::Vector3 dir, const char *script)
	{
		ShowActor* pActor = new ShowActor(npcID);
		m_fScale = pActor->getActorScale();

		Ogre::Entity *pshowmodel = pActor->getCurEntity();
		pshowmodel->addRef();
		pActor->release();

		assert(strlen(npcName) < sizeof(m_strNPCName));
		strcpy( m_strNPCName,npcName );

		assert(strlen(modelPath) < sizeof(m_Model.GetModelData()->name));
		strcpy(m_Model.GetModelData ()->name , modelPath ); 

		m_Model.SetShowModel( pshowmodel );

		m_NPCPath = npcName;
		m_NPCScript = script;
		m_nNPCID    = npcID;
		m_Dir = dir;
		m_nMapID = 0;
		m_nType = 0;

		SetScale( m_fScale );
		SetPos( pos );
		SetRot( dir.x , dir.y , dir.z );

		m_Model.UpdatePos();
	};

	MapNpc::~MapNpc(void)
	{
		Release();
	}

	void  MapNpc::Release()
	{
	};

	Ogre::Vector3  MapNpc::GetPos()
	{
		return m_Model.GetModelData()->pos;
	}

	Ogre::Vector3  MapNpc::GetDir ()
	{
		return m_Model.GetModelData ()->dir ;
	}

	void  MapNpc::SetPos(Ogre::Vector3 pos)
	{
		m_Pos = pos;
		m_Model.GetModelData ()->pos = pos;

		if(m_Model.GetModelData ()->pModel)
			m_Model.GetModelData ()->pModel ->SetPos (pos);
	};

	void  MapNpc::SetRot(float rx,float ry,float rz)
	{
		Ogre::Vector3 dir;
		dir.x = rx;
		dir.y = ry;
		dir.z = rz;
		m_Model.GetModelData ()->dir = dir;
		m_Dir = dir;

		if(m_Model.GetModelData ()->pModel)
			m_Model.GetModelData ()->pModel ->SetDir ( dir );
	};

	void  MapNpc::SetScale(float scale)
	{	
		m_Model.GetModelData ()->scale = scale;

		if(m_Model.GetModelData ()->pModel)
			m_Model.GetModelData ()->pModel ->SetScale (scale );
	};

	void  MapNpc::SetNpcName(const char* name)
	{
		strcpy( m_strNPCName , name);
	};

	MapNpcManager::MapNpcManager()
	{
	}

	MapNpcManager::~MapNpcManager()
	{
		Release();
	};

	bool   MapNpcManager::AddNpc( MapNpc*   npc)
	{
		if(npc == NULL)
		{
			return false;
		}

		npc->GetModel()->AttachRoom();

		m_NpcSet.push_back ( npc );

		return true;
	};

	bool MapNpcManager::DeleteIthNpc(int index)
	{
		assert(index>=0 && index<m_NpcSet.size());

		MapNpc *npc = m_NpcSet[index];
		npc->GetModel()->DetachRoom();
		delete npc;

		m_NpcSet.erase(m_NpcSet.begin()+index);
		return true;
	}

	void MapNpcManager::deleteAllNpc()
	{
		for(size_t i=0; i<m_NpcSet.size(); i++)
		{
			MapNpc *npc = m_NpcSet[i];
			npc->GetModel()->DetachRoom();
			delete npc;
		}

		m_NpcSet.resize(0);
	}

	MapNpc*  MapNpcManager::FindNpc( unsigned int npcID )
	{
		for(unsigned int i = 0 ; i < m_NpcSet.size () ; i ++)
		{
			if ( m_NpcSet[i]->m_nNPCID == npcID )
			{
				return m_NpcSet[i];
			}
		}

		return NULL;
	};

	struct NpcFileHeader
	{
		char magic_num[4];
		int version;
		int num_npc;
	};

	bool MapNpcManager::save(const char *filename)
	{
		EditorStream  stream;

		if(!stream.WriteFile(filename))
		{
			return false;
		}

		NpcFileHeader header;
		memcpy(header.magic_num, "MNPC", 4);
		header.version = 100;
		header.num_npc = (int)m_NpcSet.size();
		stream.Write(header);

		for(size_t i=0; i<m_NpcSet.size(); i++)
		{
			MapNpc *pnpc = m_NpcSet[i];

			stream.Write(int(i));
			stream.Write(pnpc->m_nNPCID);
			stream.Write(pnpc->GetModel()->GetModelData()->dir);
			stream.Write(pnpc->GetModel()->GetModelData()->pos);
		}

		stream.WriteEnd();
		return true;
	}

	bool MapNpcManager::load(const char *filename)
	{
		Release();

		EditorStream stream;
		if(!stream.ReadFile(filename))
		{
			return false;
		}

		NpcFileHeader header;
		stream.Read(header);

		for(int i=0; i<header.num_npc; i++)
		{
			int index, npcid;
			Ogre::Vector3 dir, pos;

			stream.Read(index);
			stream.Read(npcid);
			stream.Read(dir);
			stream.Read(pos);

			const NPCDISPLAYINFO &resinfo = DBDataManager::getSingleton().GetNpcDisplayInfo(npcid);
			const MONSTERDEF &mdef = DBDataManager::getSingleton().getMonsterDef(npcid);

			MapNpc *npc = new MapNpc(mdef.Name, resinfo.model, npcid, pos, dir, "");
			npc->m_nType = 0;
			AddNpc(npc);
		}

		return true;
	}

	bool   MapNpcManager::SaveDB(int mapid, bool gen_jingying)
	{
		std::vector<MAPMONSTER>monsters;
		monsters.resize(m_NpcSet.size());

		for(size_t i=0; i<m_NpcSet.size(); i++)
		{
			MapNpc *pnpc = m_NpcSet[i];
			MAPMONSTER &monster = monsters[i];

			monster.MapID = mapid;
			monster.ListIndex = i;
			monster.MonsterID = pnpc->m_nNPCID;
			if(gen_jingying)
			{
				monster.ListIndex += m_NpcSet.size();
				monster.MonsterID += PWORLD_LEVEL_MON_BASE;
			}

			monster.Dir = int(pnpc->GetModel()->GetModelData()->dir.x)/2;
			monster.Pos.X = int(pnpc->GetModel()->GetModelData()->pos.x);
			monster.Pos.Y = int(pnpc->GetModel()->GetModelData()->pos.z);

			if(!g_NavMapProcess.isPointWalkable(&g_TerrainEdit, monster.Pos.X, monster.Pos.Y))
			{
				char info[256];
				sprintf(info, "NPC(%d-%s)站立点不能行走", i, pnpc->GetNpcName());
				MessageBox(NULL, info, "错误", MB_OK);
			}
		}

		if(!DBDataManager::getSingleton().saveMapMonsters(mapid, monsters))
		{
			MessageBox(NULL, "保存到数据库错误， 可能是此用户名不能修改数据库", "错误", MB_OK);
			return false;
		}
		return true;
	};

	bool   MapNpcManager::LoadDB(int mapid)
	{
		Release();

		std::vector<MAPMONSTER>monsters;
		DBDataManager::getSingleton().loadMapMonsters(mapid, monsters);

		for(size_t i=0; i<monsters.size(); i++)
		{
			MAPMONSTER &src = monsters[i];

			Ogre::Vector3 pos, dir;
			pos.x = src.Pos.X;
			pos.z = src.Pos.Y;
			g_Terrain.GetHeight(pos.x, pos.z, &pos.y, TERR_PICK_VB_BSP);

			dir.x = float(src.Dir*2);
			dir.y = dir.z = 0;

			const NPCDISPLAYINFO &resinfo = DBDataManager::getSingleton().GetNpcDisplayInfo(src.MonsterID);
			const MONSTERDEF &mdef = DBDataManager::getSingleton().getMonsterDef(src.MonsterID);

			MapNpc *npc = new MapNpc(mdef.Name, resinfo.model, src.MonsterID, pos, dir, "");
			npc->m_nType = 0;
			AddNpc(npc);
		}

		return true;
	};

	MapNpc*  MapNpcManager::GetNpc( int index )
	{
		if( index < 0 || index >= (int)m_NpcSet.size () )
		{
			return NULL ;
		}

		return m_NpcSet[index];
	};

	int MapNpcManager::findNpcByModel(TerrModel *pmodel)
	{
		for(size_t i=0; i<m_NpcSet.size(); i++)
		{
			if(m_NpcSet[i]->GetModel() == pmodel) return int(i);
		}
		return -1;
	}

	void MapNpcManager::onCull()
	{
		for(size_t i=0; i<m_NpcSet.size(); i++)
		{
			m_NpcSet[i]->GetModel()->onCull();
		}
	}

	void   MapNpcManager::Update( float tick )
	{
		for ( size_t i = 0 ; i < m_NpcSet.size() ; i ++ )
		{
			m_NpcSet[i]->m_Model.Update( tick );
		}
	}

	void   MapNpcManager::Release()
	{
		for(unsigned int i = 0 ; i < m_NpcSet.size () ; i++)
		{
			OGRE_DELETE( m_NpcSet[i] );
		}
		m_NpcSet.clear();
	};

	TerrModel *MapNpcManager::intersectRay(Ogre::Ray &ray)
	{
		for(size_t i=0; i<m_NpcSet.size(); i++)
		{
			TerrModel *pmodel = m_NpcSet[i]->GetModel();
			if(pmodel->GetModelData()->pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL))
			{
				return pmodel;
			}
		}

		return NULL;
	}

	Ogre::Vector3  ChangeStrToVec(const char* strPos)
	{
		Ogre::Vector3 pos(0,0,0);
		std::string     _strPos = strPos;
		int index1,index2;
		index1 = _strPos.find (",");
		index2 = _strPos.rfind (",");
		std::string posx,posy,posz;

		if( index1 == -1)
		{
			return pos;
		}


		posx = _strPos.substr ( 0 , index1);
		posy = _strPos.substr ( index1+1 , index2-index1-1);
		posz = _strPos.substr ( index2+1 );

		pos.x = (float)atof( posx.c_str () );
		pos.y = (float)atof( posy.c_str () );
		pos.z = (float)atof( posz.c_str () );

		return pos;
	}
}
