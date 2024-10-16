#include "EditEvent_Decal.h"

#include "TerrainStrip.h"
#include "EditorCamera.h"
#include "Terrain.h"
#include "TerrainTile.h"
#include "OgreLiquid.h"

namespace TE
{
	DecalEditHandler::DecalEditHandler(void)
	{
		m_nProcessType = DECAL_EVN ;
	}

	DecalEditHandler::~DecalEditHandler(void)
	{
	}

	bool DecalEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
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

	bool DecalEditHandler::AddDecalPos( TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos  )
	{
		Ogre::Vector3  pos0;

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
}
