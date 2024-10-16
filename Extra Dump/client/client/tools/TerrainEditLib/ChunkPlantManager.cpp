#include ".\chunkplantmanager.h"
#include "GTitle.h"
#include "MTerrain.h"

#include "mstream.h"

using namespace TE;

CChunkPlantManager::CChunkPlantManager(void)
{
}

CChunkPlantManager::~CChunkPlantManager(void)
{
}
//����CHUNKֲ���ļ�;���� ( �༭��ָ��,�ļ��� );���� ( Ϊ���ʾ�ɹ� )	
bool   CChunkPlantManager::SaveChunkPlant( MTerrainEdit* pEditor ,const char* filename )
{
    MStream  mstream;
	mstream.WriteFile( filename );

	MChunkPlantHead  head;
	head.version = 10001;
	head.chunkNum = 0;

	MTitle* pTile = g_Terrain.GetTitle( 0 );

	if( pTile == NULL )
		return false;
	
	for ( int i = 0 ; i < pTile->GetTileData()->gh ; i ++ )
	{
		for( int j = 0 ; j < pTile->GetTileData()->gw ; j ++ )
		{
			if( !pEditor->GetEditData()->chunkSet[i][j].m_PlantMap.empty() )
			{
				head.chunkNum ++;
			}
		}
	}

	mstream.Write( head );

	for ( int i = 0 ; i < pTile->GetTileData()->gh ; i ++ )
	{
		for( int j = 0 ; j < pTile->GetTileData()->gw ; j ++ )
		{
			if( !pEditor->GetEditData()->chunkSet[i][j].m_PlantMap.empty() )
			{
				MChunkPlantInfo  chunkPlantInfo;
				chunkPlantInfo.gx = j ;
				chunkPlantInfo.gy = i ;
				chunkPlantInfo.plantKind = pEditor->GetEditData()->chunkSet[i][j].m_PlantMap.size() ;
				mstream.Write( chunkPlantInfo );

				MChunk::PLANT_MAP::iterator  iter = pEditor->GetEditData()->chunkSet[i][j].m_PlantMap.begin() ;

				for( ; iter != pEditor->GetEditData()->chunkSet[i][j].m_PlantMap.end() ; iter ++ )
				{
					MPlantInfo plantInfo;
					strcpy( plantInfo.plantTex , iter->first.c_str() );
					plantInfo.plantNum = iter->second->GetCurPlantNum();

					mstream.Write( plantInfo );

					for( int k = 0 ; k < plantInfo.plantNum ; k ++ )
					{
						mstream.Write ( iter->second->m_SavePosSet[k] );
					}

				}
			}
		}
	}

	mstream.WriteEnd();

	return true;
};
//����chunkֲ����Ϣ;����( �༭��ָ��,�ļ��� );���� ( Ϊ���ʾ�ɹ� )	
bool   CChunkPlantManager::LoadChunkPlant( MTerrainEdit* pEditor ,const char* filename )
{
    MStream  mstream;
	mstream.ReadFile( filename );

	MChunkPlantHead  head;
	if( !mstream.Read( head ) )
	{
		return false;
	}

	MTitle* pTile = g_Terrain.GetTitle( 0 );

	if( pTile == NULL )
	{
		return false;
	}

	for( int i = 0 ; i < head.chunkNum ; i ++ )
	{

	}

	return true;
};
