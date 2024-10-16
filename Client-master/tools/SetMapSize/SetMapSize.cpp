// SetMapSize.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "proto_comm.h"
#include "resdb_meta.h"
#include "OgreResDB.h"
#include "OgreRoot.h"
#include "OgreResourceManager.h"
#include "OgreTerrainData.h"
using namespace Ogre;

#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#endif



int _tmain(int argc, _TCHAR* argv[])
{
	Root *proot = new Root("client.cfg");
	proot->Initlize();
	ResDB* pResDB = CreateResDBFromRealDB("db/resdb_meta.tdr", 
										"192.168.0.77", 
										"resdb", "server", "tulin#1234" );

	ResTable *ptable = pResDB->GetTable("MapDef");
	if(ptable == NULL)
	{
		OGRE_DELETE( proot );
		OGRE_DELETE( pResDB );
		return false;
	}
	size_t unitsize = sizeof(MAPDEF);
	assert(unitsize == ptable->GetRecordSize());
	int count = ptable->FindRecord(NULL, 0, NULL);
	if(count < 0)
	{
		OGRE_DELETE( proot );
		OGRE_DELETE( pResDB );
		return false;
	}
	std::vector<MAPDEF> mapVector;
	mapVector.resize( count );
	ptable->FindRecord(&mapVector[0], count, NULL);

	for( size_t i = 0 ; i < mapVector.size() ; i++ )
	{
		int mapid = mapVector[i].ReferMapID > 0 ? mapVector[i].ReferMapID : mapVector[i].MapID;
		char chunkname[256] = { '\0' };
		sprintf(chunkname, "maps\\%d\\%d.tile", mapid, mapid);
		Resource *pres = ResourceManager::getSingleton().blockLoad(chunkname);
		if(pres != NULL)
		{
			TerrainTileSource *ptiledata = static_cast<TerrainTileSource *>(pres);
			int		nWidthGrid = ptiledata->m_TileDesc.gridX;
			int		nHeightGrid = ptiledata->m_TileDesc.gridY;
			float	fGridSize = ptiledata->m_TileDesc.gridSize;

			mapVector[i].MapWidth  = (int)(nWidthGrid  * fGridSize);
			mapVector[i].MapHeight = (int)(nHeightGrid * fGridSize);
			ptable->UpdateRecord( &mapVector[i] );
			OGRE_RELEASE(pres);
		}
	}
	OGRE_DELETE( pResDB );
	OGRE_DELETE( proot );
	return 0;
}

