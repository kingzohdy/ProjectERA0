#include ".\meditdecalprocess.h"

#include "stripbase.h"
#include "EditorCamera.h"
#include "MTerrain.h"
#include "TerrainTile.h"
#include "OgreLiquid.h"

using namespace TE;

MEditDecalProcess::MEditDecalProcess(void)
{
	m_nProcessType = DECAL_EVN ;
}

MEditDecalProcess::~MEditDecalProcess(void)
{
}

bool MEditDecalProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	switch( event->mouseEven )
	{
	case MouseSewheel:
		{
			if( pEditor->m_EditOption.nCurCameraTpye == 0)
			{	
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
			}
			else
			{
				g_Camera.WalkZ ( event->mz*5.0f );
			}
		}
		break;
	case MouseLDown:
		{
			AddDecalPos( pEditor , event->dir ,event->pos );
		}
		break;
	}
	return true;
};

bool MEditDecalProcess::AddDecalPos( MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos  )
{
 	MVec3  pos0;

	if( g_Terrain.TestAndRay (dir,pos,pos0))
	{	
		if ( pEditor->m_DecalType == DECAL_NORMAL )
		{
			g_StripBaseSet->AddCurSelectStripPos ( pos0 );
		}
		else
		{
			int  gx ,gy ;
			g_Terrain.GetGridX ( pos0.x , &gx );
			g_Terrain.GetGridZ ( pos0.z , &gy );

			if( pEditor->GetEditData() )
			{
				if( pEditor->GetEditData()->chunkSet [gy][gx].GetChunkData ()->bHaswater )
				{
					if( pos0.y <  pEditor->GetEditData()->chunkSet [gy][gx].GetChunkLiquid()->getHeight() )
					{
						pos0.y = pEditor->GetEditData()->chunkSet [gy][gx].GetChunkLiquid()->getHeight();
					}
				}
			}
		}
		return true;
	}
	return false;
}
