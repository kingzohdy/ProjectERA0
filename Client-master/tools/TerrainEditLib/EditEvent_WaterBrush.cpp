#include "EditEvent_WaterBrush.h"
#include "EditorInput.h"
#include "EditorCamera.h"
#include "Terrain.h"
#include "TerrainTile.h"

#include "OgreLiquid.h"

namespace TE
{
	WaterBrushEditHandler::WaterBrushEditHandler(void)
	{	
		m_nProcessType = MODIFY_LIQUID ;
	}

	WaterBrushEditHandler::~WaterBrushEditHandler(void)
	{
	}


	bool WaterBrushEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
	{	
		switch( event->mouseEven )
		{
		case MouseLDown:
			AddLiquid(  pEditor , event->dir , event->pos );
			break;
		case MouseMove:		
			if(event->bMouseDown)
			{
				//AddLiquid( pEditor , event->dir , event->pos );
			}
			else
			{
			}
			break;
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
		}
		return true;
	};

	bool  WaterBrushEditHandler::AddLiquid( TerrainEditMgr* pEditor , Ogre::Vector3 &dir, Ogre::Vector3 &pos )
	{
		Ogre::Vector3  pos0;
		EditOptionWater &wateropt = pEditor->m_EditOption.water;

		if(g_Terrain.TestAndRay (dir,pos,pos0))
		{
			int gx , gz;
			if(!g_Terrain.GetGridX(pos0.x,&gx))
				return false;
			if(!g_Terrain.GetGridZ(pos0.z,&gz))
				return false;	

			wateropt.chunk_x = gx;
			wateropt.chunk_z = gz;
			EditorTerrainBlock &curchunk = pEditor->GetEditData()->chunkSet[gz][gx];

			if(g_MInputProcess.GetUserInput()->bLShift) 
			{
				curchunk.GetChunkData()->bHaswater = false;
				return true;
			}
			else if( g_MInputProcess.GetUserInput()->bLCtrl )
			{
				if(curchunk.GetChunkData ()->bHaswater)
				{
					wateropt.watertype = curchunk.m_pLiquid->m_WaterType;
					strcpy(wateropt.animtex, curchunk.m_LiquidData.name);
					strcpy(wateropt.envtex, "");
					wateropt.waterheight = curchunk.m_pLiquid->getHeight();
					wateropt.watercolor[0] = curchunk.m_pLiquid->m_ShallowWaterColor;
					wateropt.watercolor[1] = curchunk.m_pLiquid->m_DeepWaterColor;
					wateropt.waterdepth[0] = curchunk.m_pLiquid->m_ShallowWaterDepth;
					wateropt.waterdepth[1] = curchunk.m_pLiquid->m_DeepWaterDepth;
					wateropt.maxwaterdepth = curchunk.m_pLiquid->m_MaxWaterDepth;
					wateropt.ntexrepeat = curchunk.m_pLiquid->m_NumTexRepeat;
					wateropt.speed = curchunk.m_pLiquid->m_Speed;
					wateropt.amp = curchunk.m_pLiquid->m_Amp;

					g_TerrainEdit.SetUpdateWaterData(true);
				}

				return true;
			}

			if(wateropt.watertype==-1 || (wateropt.animtex[0]==0 && wateropt.watertype==0 ))
			{
				return false;
			}

			curchunk.UpdateLiquid(wateropt);
			return true;
		}

		return false;

	}
}
