#include ".\mterraincull.h"

#include "EditorModel.h"
#include "EditorCamera.h"
#include "mterrainedit.h"

#include "OgreGameScene.h"
#include "OgreModel.h"
#include "OgreCamera.h"

using namespace TE;
using namespace Ogre;

bool   g_DrawModelType[TYPE_MODEL_NUM];

MTerrainCull::MTerrainCull(void)
{
	memset( m_ShowChunk , 0 , sizeof( MChunk*)*CHUNK_SHOW_SIZE_MAX );
    m_ShowChunkNum = 0 ;

	memset( m_ShowModel , 0 , sizeof( MChunk*)*SHOW_MODEL_SIZE_MAX );
	 m_ShowModelNum = 0;
}

MTerrainCull::~MTerrainCull(void)
{
}

void  MTerrainCull::Cull(MTerrain* pTerrain)
{
	if( pTerrain == NULL && pTerrain->GetTitle(0)!=NULL)
		return;

	CullFrustum frustum;
	g_pCamera->getCullFrustum(frustum);
			
    m_ShowChunkNum = 0 ;
	 m_ShowModelNum = 0;

	//优化算法：只计算6X6格子的区域
	MVec3   pos = g_Camera.m_Pos ;
	int gx  , gz ;
	gx =(int)( pos.x / TERRBLOCK_SIZE ) ;
	gz = (int)( pos.z / TERRBLOCK_SIZE ) ;

	int  mingx , mingz ;
	int  maxgx , maxgz ;

	/*
	mingx = gx - 7 ;
	mingz = gz - 7 ;

	if( mingx < 0 )
		mingx = 0 ;
	if( mingz < 0 )
		mingz = 0 ;

	maxgx = gx + 7 ;
	maxgz = gz + 7 ;
	*/
	mingx = mingz = 0;
	maxgx = TERRTILE_NUMBLOCK_X;
	maxgz = TERRTILE_NUMBLOCK_Z;
       
	int nTileX , nTileY ;  
	int nGridX , nGridY ;

    for( int i = mingz ; i < maxgz; i ++)
	{
		for( int j = mingx ; j < maxgx ; j ++ )
		{
			nTileX = j / TERRTILE_NUMBLOCK_X ;
			nTileY = i / TERRTILE_NUMBLOCK_Z ;

			nGridX = j % TERRTILE_NUMBLOCK_X ;
			nGridY = i % TERRTILE_NUMBLOCK_Z ;
            
			for( int k = 0 ; k < (int)pTerrain->m_TitleSet .size () ; k ++)
			{		
				MTitle*  pTile = pTerrain->m_TitleSet [k] ;
				if( pTile->GetHeadData()->tileX == nTileX 
					&& pTile->GetHeadData ()->tileY == nTileY )
				{
					if( nGridX >= pTile->GetHeadData()->gridX || nGridY >= pTile->GetHeadData()->gridY ) 
						continue;

					MChunk* pChunk = &(pTile->GetTitleData ()->chunkSet [nGridY][nGridX]);
				
					if( frustum.cull ( pChunk->m_AABBox ) != CullFrustum::CULL_OUT )
					{      
						m_ShowChunk[m_ShowChunkNum] =  pChunk ;
						m_ShowChunkNum ++;
						assert(m_ShowChunkNum < CHUNK_SHOW_SIZE_MAX);
					}                
				}
			}
		}
	}
		
	if( g_TerrainEdit.m_bShowObjectBox )
	{	
		for( int k = 0 ; k < (int)pTerrain->m_TitleSet .size () ; k ++)
		{		
			MTitle*  pTile = pTerrain->m_TitleSet [k] ;

			for( unsigned int i = 0 ; i < pTile->GetTileData()->modelData.modelSet.size() ; i ++ )
			{	
				TerrModel* pModel = pTile->GetTileData()->modelData.modelSet[i];

				if( pModel->GetModelData()->pDrawbox == NULL )
					continue;

				EditorModel* pMayModel = (EditorModel*)pModel->GetModelData()->pModel ;

				if( !pMayModel )
					continue;

				if( !pMayModel->GetShowModel() )
					continue;

				const BoxSphereBound &bound = pMayModel->GetShowModel()->getWorldBounds();
				if( frustum.cull(bound) != CullFrustum::CULL_OUT )
				{
					pModel->GetModelData()->bShow = true;

					m_ShowModel[m_ShowModelNum] = pModel ;
					m_ShowModelNum ++;
					assert(m_ShowModelNum < SHOW_MODEL_SIZE_MAX);
				} 
				else				
				{
					pModel->GetModelData()->bShow = false;
				}

			}
		}
	}
}



