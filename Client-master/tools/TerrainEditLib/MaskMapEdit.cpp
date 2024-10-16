
#include "MaskMapEdit.h"
#include "MaskMap.h"
#include "EditorModel.h"
#include "Terrain.h"

#include "OgreModel.h"
#include "OgreRay.h"

using namespace Ogre;

namespace TE
{
	extern std::string  g_path;

	MaskMapEditMgr   g_NavMapProcess;

	MaskMapEditMgr::MaskMapEditMgr(void)
	{
	}

	MaskMapEditMgr::~MaskMapEditMgr(void)
	{
	}

	void MaskMapEditMgr::copyWalkMaskToFly(TerrainEditMgr* pEditor, int nmask_block)
	{
		if( !pEditor->GetEditData() )
			return;

		int gx = pEditor->GetEditData()->gw*nmask_block;
		int gz = pEditor->GetEditData()->gh*nmask_block;

		float  sizeX = TERRBLOCK_SIZE/nmask_block;
		float  sizeZ = TERRBLOCK_SIZE/nmask_block;

		for( int i = 0 ; i < gz ; i++)
		{
			for ( int j = 0 ; j < gx ; j++)
			{
				int tx,ty;
				int mx , my ;
				tx = j / nmask_block;
				ty = i / nmask_block;
				mx = (j % nmask_block)*64/nmask_block;
				my = (i % nmask_block)*64/nmask_block;

				unsigned char *pdata1 = pEditor->GetEditData()->chunkSet[ty][tx].GetChunkInfo()->info[0];
				unsigned char *pdata2 = pEditor->GetEditData()->chunkSet[ty][tx].GetChunkInfo()->info[2];
				pdata2[my*64+mx] = pdata1[my*64+mx];
				pdata2[my*64+mx+1] = pdata1[my*64+mx+1];
				pdata2[my*64+mx+64] = pdata1[my*64+mx+64];
				pdata2[my*64+mx+1+64] = pdata1[my*64+mx+1+64];
			}
		}	

		EditorTerrainTile* pTile = g_Terrain.GetTitle (0) ;
		if( pTile )
		{
			for( int i = 0 ; i < pTile->GetTileData()->gh ; i ++)
			{
				for( int j = 0 ;j < pTile->GetTileData()->gw ; j++)
				{
					pTile->GetTitleData ()->chunkSet[i][j].CreateColorMap();
					pTile->GetTitleData ()->chunkSet[i][j].UpdateMapInfoToColorMap(2);

					pTile->GetTitleData ()->chunkSet [i][j].UpdateColorMap();
				}
			}
		}
	}

	void  MaskMapEditMgr::calNaviMapData( TerrainEditMgr* pEditor, int nmask_block, float height)
	{
		if( !pEditor->GetEditData() )
			return;

		int gx = pEditor->GetEditData()->gw*nmask_block;
		int gz = pEditor->GetEditData()->gh*nmask_block;

		float  sizeX = TERRBLOCK_SIZE/nmask_block;
		float  sizeZ = TERRBLOCK_SIZE/nmask_block;

		unsigned char *pdata = new unsigned char[gx*gz];

		float basex,basez;
		EditorTerrainTile* pTitle = (EditorTerrainTile*)g_Terrain.GetTitle (0);
		basex = pTitle->GetTitleData()->box.minpos.x ;
		basez = pTitle->GetTileData ()->box.minpos.z;

		float mx,mz;

		for( int i = 0 ; i < gz ; i ++)
		{
			for( int j = 0 ; j < gx ; j ++)
			{
				mx = basex + j*sizeX;
				mz = basez + i*sizeZ;
				pdata[i*gx + j] = GetMapSign( mx ,mz , sizeX , sizeZ ,height );
			}
		}

#ifdef ___BBBBB
		for( size_t i=0; i<pEditor->GetEditData()->modelData.modelSet .size() ; i ++ )
		{
			/*if( pEditor->GetEditData()->modelData.modelSet[i]->GetModelData()->cType == 4 || 
			pEditor->GetEditData()->modelData.modelSet[i]->GetModelData()->cType == 5 )
			continue;
			*/

			EditorModel* pModel = (EditorModel*)pEditor->GetEditData()->modelData.modelSet[i]->GetModelData()->pModel ;
			Model *pNode = pModel->GetShowModel()->getMainModel();
			//根据模型的包围盒，更新数据
			if(pNode) SetModelSign(pNode, height, 0, 0, pdata, gx, gz, sizeX, sizeZ);
		}
#endif

		//g_NavMap.SaveBmpFile ("test.bmp");

		//测试行走层
		for( int i = 0 ; i < gz ; i++)
		{
			for ( int j = 0 ; j < gx ; j++)
			{
				int tx,ty;
				int mx , my ;
				tx = j / nmask_block;
				ty = i / nmask_block;
				mx = (j % nmask_block)*64/nmask_block;
				my = (i % nmask_block)*64/nmask_block;

				pEditor->GetEditData()->chunkSet[ty][tx].GetChunkInfo()->info [0][my*64+mx] = pdata[i*gx + j];
				pEditor->GetEditData()->chunkSet[ty][tx].GetChunkInfo()->info [0][my*64+mx+1] = pdata[i*gx + j];
				pEditor->GetEditData()->chunkSet[ty][tx].GetChunkInfo()->info [0][my*64+mx+64] = pdata[i*gx + j];
				pEditor->GetEditData()->chunkSet[ty][tx].GetChunkInfo()->info [0][my*64+mx+1+64] = pdata[i*gx + j];
			}
		}	
		// 测试水层	
		//每格小格子的大小	
		float  fsize = TERRBLOCK_SIZE / nmask_block;	

		for( int i = 0 ; i < pEditor->GetEditData()->gh ; i++ )
		{
			for( int j = 0 ; j < pEditor->GetEditData()->gw ;j++ )
			{
				if( pEditor->GetEditData()->chunkSet[i][j].GetChunkData()->bHaswater )
				{
					Ogre::Vector3 pos = pEditor->GetEditData()->chunkSet[i][j].GetChunkData()->vecData[0].pos  ;

					float fWaterHeight = pEditor->GetEditData()->chunkSet[i][j].GetLiquidData()->waterheight;
					Ogre::Vector3  vHeight[4] ;

					for( int k = 0 ; k < nmask_block ; k ++ )
					{
						for( int l = 0 ; l < nmask_block ; l ++ )
						{
							vHeight[0].x = pos.x + l*fsize;
							vHeight[0].z = pos.z + k*fsize;

							vHeight[1].x = pos.x + (l+1)*fsize;
							vHeight[1].z = pos.z + k*fsize;

							vHeight[2].x = pos.x + l*fsize;
							vHeight[2].z = pos.z + (k+1)*fsize;

							vHeight[3].x = pos.x + (l+1)*fsize;
							vHeight[3].z = pos.z + (k+1)*fsize;

							bool bWater = false;
							for( int u = 0 ; u < 4 ; u++ )
							{
								g_Terrain.GetHeight( vHeight[u].x , vHeight[u].z , &vHeight[u].y );
								if( vHeight[u].y < fWaterHeight )
								{
									bWater = true ;
									break;
								}
							}

							int k2 = k*64/nmask_block;
							int l2 = l*64/nmask_block;

							if( bWater )
							{
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2] = 1; 
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+1] = 1; 
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+64] = 1; 
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+1+64] = 1; 
							}
							else
							{
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2] = 0; 
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+1] = 0; 
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+64] = 0; 
								pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+1+64] = 0; 
							}
						}
					}
				}
				else
				{
					for( int k = 0 ; k < nmask_block ; k ++ )
					{
						for( int l = 0 ; l < nmask_block ; l ++ )
						{
							int k2 = k*64/nmask_block;
							int l2 = l*64/nmask_block;

							pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2] = 0; 
							pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+1] = 0; 
							pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+64] = 0; 
							pEditor->GetEditData()->chunkSet[i][j].GetChunkInfo()->info [1][k2*64+l2+1+64] = 0; 

						}
					}
				}
			}
		}

		EditorTerrainTile* pTile = g_Terrain.GetTitle (0) ;

		if( pTile )
		{
			for( int i = 0 ; i < pTile->GetTileData()->gh ; i ++)
			{
				for( int j = 0 ;j < pTile->GetTileData()->gw ; j++)
				{
					pTile->GetTitleData ()->chunkSet[i][j].CreateColorMap();
					pTile->GetTitleData ()->chunkSet[i][j].UpdateMapInfoToColorMap( 0 );

					pTile->GetTitleData ()->chunkSet [i][j].UpdateColorMap();
				}
			}
		}

		delete[] pdata;
	};

	unsigned char MaskMapEditMgr::GetMapSign(float x,float z,float sx,float sz,float height)
	{
		float _height[4];

		g_Terrain.GetHeight ( x , z , &_height[0], TERR_PICK_VB_BSP);
		g_Terrain.GetHeight ( x + sx , z , &_height[1], TERR_PICK_VB_BSP);
		g_Terrain.GetHeight ( x , z +sz, &_height[2], TERR_PICK_VB_BSP);
		g_Terrain.GetHeight ( x +sx, z +sz , &_height[3], TERR_PICK_VB_BSP);

		for( int i = 0 ; i < 3 ; i ++)
		{
			if( (_height[i+1] - _height[i]) > height)
				return 0xff;
			if( (_height[i] - _height[i+1]) > height )
				return 0xff;
		}

		if( _height[3] - _height[0] > height )
			return 0xff;
		if( _height[0] - _height[3] > height )
			return 0xff;

		return 0;
	}

	void  MaskMapEditMgr::SetModelSign( MovableObject *pNode,float height , float basex,float basez, unsigned char *pdestdata, int ngridx, int ngridz, int gridsizex, int gridsizez)
	{
		int  minx,minz;
		int  maxx,maxz;

		Ogre::Vector3 minPos,maxPos;

		if(IS_KIND_OF(Model, pNode))
		{
			const BoxSphereBound &bound = pNode->getWorldBounds();

			float fLength = bound.m_Radius;

			minPos = bound.getMin();
			maxPos = bound.getMax();

			minx = (int)((minPos.x - basex)/ gridsizex) ;
			maxx = (int)((maxPos.x - basex)/ gridsizex) ;
			minz = (int)((minPos.z - basez)/ gridsizez);
			maxz = (int)((maxPos.z - basez)/ gridsizez);

			Ogre::Vector3  dirPos;
			Ogre::Vector3  dir(0,-1,0);

			float heightSet[4];

			if ( minz < 0 )
				return;
			if( minx < 0 )
				return;

			for( int i = minz ; i < maxz ; i++)
			{
				for ( int j = minx ; j < maxx ; j++)
				{	
					float fSaveHeight = -999999;	  

					dirPos.x = basex + j*gridsizex;
					dirPos.z = basez + i*gridsizez;
					dirPos.y = maxPos.y + 10.0f; 
					//测试模型及可以获得

					bool  bIntersectRay = false;  
					bool  bIntersectChun = false;

					Ray ray;

					ray.m_Dir = dir;
					ray.m_Origin = dirPos ;

					float    len;
					if(pNode->intersectRay(INTERSECT_PICK, ray, &len))
					{
						heightSet[0] = dirPos.y - len;
						bIntersectRay = true;
					}
					else
					{
						bIntersectChun = true;
						g_Terrain.GetHeight(dirPos.x ,dirPos.z , &heightSet[0]);
					}

					dirPos.x = basex + (j+1)*gridsizex;						  
					ray.m_Origin = dirPos;
					if(pNode->intersectRay(INTERSECT_PICK, ray, &len))
					{
						heightSet[1] = dirPos.y - len;
						bIntersectRay = true;
					}
					else
					{
						bIntersectChun = true;
						g_Terrain.GetHeight ( dirPos.x ,dirPos.z , &heightSet[1]);
					}          

					dirPos.z = basez + (i+1)*gridsizez;
					ray.m_Origin = dirPos;
					if(pNode->intersectRay(INTERSECT_PICK, ray, &len))
					{
						heightSet[2] = dirPos.y - len;
						bIntersectRay = true;
					}
					else
					{
						bIntersectChun = true;
						g_Terrain.GetHeight ( dirPos.x ,dirPos.z , &heightSet[2]);
					}  

					dirPos.x = basex + j*gridsizex;
					ray.m_Origin = dirPos;
					if(pNode->intersectRay(INTERSECT_PICK, ray, &len))
					{
						heightSet[3] = dirPos.y - len;
						bIntersectRay = true;
					}
					else
					{
						bIntersectChun = true;
						g_Terrain.GetHeight ( dirPos.x ,dirPos.z , &heightSet[3]);
					} 

					if( !bIntersectRay )
						continue;

					bool ifTest = false;
					for( int k = 0 ; k < 3 ; k ++)
					{
						if ( heightSet[k+1] - heightSet[k] >= height )
						{
							pdestdata[i*ngridx + j] = 0xff;
							ifTest = true;
							break;
						}
						else if( heightSet[k] - heightSet[k+1] >= height )
						{
							pdestdata[i*ngridx + j] =  0xff;
							ifTest = true;
							break;
						}
					}
					//
					if( heightSet[0] - heightSet[3] >= height )
					{
						pdestdata[i*ngridx + j] =  0xff;
					}
					else if( heightSet[3] - heightSet[0] >= height )
					{
						pdestdata[i*ngridx + j] =  0xff;
					}
					else
					{
						if( !ifTest )
							pdestdata[i*ngridx + j] =  0;
					}

					if( fSaveHeight != -999999)
					{
						//补充检测，误差
						if( pdestdata[ i*ngridx + j] == 0)
						{
							if( pdestdata[i*ngridx + j - 1] == 0)
							{
								float curHeight = (heightSet[0] + heightSet[1] + heightSet[2] + heightSet[3]) / 4.0f;
								if( curHeight - fSaveHeight > height )
								{
									pdestdata[ i*ngridx + j] = 0xff;
								}
							}
						}
					}
					fSaveHeight = (heightSet[0] + heightSet[1] + heightSet[2] + heightSet[3]) / 4.0f;
				}
			}
		}
	}

	bool MaskMapEditMgr::isPointWalkable(TerrainEditMgr* pEditor, float x, float z)
	{
		if(!pEditor->GetEditData())
			return false;

		int gw = pEditor->GetEditData()->gw;
		int gh = pEditor->GetEditData()->gh;
		int gx = int(x/TERRBLOCK_SIZE);
		int gz = int(z/TERRBLOCK_SIZE);

		if(gx<0 || gx>=gw || gz<0 || gz>=gh) return false;

		float gridsize = TERRBLOCK_SIZE/64;
		int mx = (x - gx*TERRBLOCK_SIZE)/gridsize;
		int mz = (z - gz*TERRBLOCK_SIZE)/gridsize;
		if(mx > 63) mx = 63;
		if(mz > 63) mz = 63;

		TerrBlockLayerInfo *pchunkinfo = pEditor->GetEditData()->chunkSet[gz][gx].GetChunkInfo();
		if(pchunkinfo->info[0][64*mz + mx] != 0)
		{
			return false;
		}
		else return true;
	}

	bool MaskMapEditMgr::saveNaviDataToMaskMap(EditorMaskMap &destmap, TerrainEditMgr* pEditor, int nmask_block)
	{
		if(!pEditor->GetEditData())
			return false;

		int gx = pEditor->GetEditData()->gw*nmask_block;
		int gz = pEditor->GetEditData()->gh*nmask_block;

		MaskMapHeader  *pHead = destmap.GetHeadData();
		memcpy(pHead->magic_number, "MASK", 4);

		pHead->gridsizex = TERRBLOCK_SIZE/nmask_block;
		pHead->gridsizez = TERRBLOCK_SIZE/nmask_block;
		pHead->ngridx = gx;
		pHead->ngridz = gz;
		pHead->version = 100;
		pHead->headsize = sizeof(MaskMapHeader);
		pHead->filesize = pHead->headsize + gx*gz;

		MaskMapData *pData = destmap.GetNavData();
		pData->pdata = new unsigned char[gx*gz];
		memset(pData->pdata, 0, gx*gz);

		float basex,basez;
		EditorTerrainTile* pTitle = (EditorTerrainTile*)g_Terrain.GetTitle (0);
		basex = pTitle->GetTitleData()->box.minpos.x ;
		basez = pTitle->GetTileData ()->box.minpos.z;

		for( int i = 0 ; i < gz ; i ++)
		{
			for( int j = 0 ; j < gx ; j ++)
			{
				int ggx , ggy ;
				int mx , my ;

				ggx = j / nmask_block;
				ggy = i / nmask_block;
				mx = (j%nmask_block)*64/nmask_block;
				my = (i%nmask_block)*64/nmask_block;

				TerrBlockLayerInfo *pchunkinfo = pEditor->GetEditData()->chunkSet[ggy][ggx].GetChunkInfo() ;
				for(int k=0; k<4 ; k ++)
				{
					if( pchunkinfo->info[k][64*my + mx] != 0 )
					{
						pData->pdata[i*gx + j] |= 1<<k;
					}
				}
			}
		}

		return true;
	}

	bool MaskMapEditMgr::loadMaskBMPMapToNaviData(EditorMaskMap &destmap, TerrainEditMgr* pEditor, int nmask_block)
	{
		if(!pEditor->GetEditData())
			return false;

		int gx = pEditor->GetEditData()->gw*nmask_block;
		int gz = pEditor->GetEditData()->gh*nmask_block;


		MaskMapData *pData = destmap.GetNavData();

		int k = 0; //行走层比特位
		for( int i = 0 ; i < gz ; i ++)
		{
			for( int j = 0 ; j < gx ; j ++)
			{
				int ggx , ggy ;
				int mx , my ;

				ggx = j / nmask_block;
				ggy = i / nmask_block;
				mx = (j%nmask_block)*64/nmask_block;
				my = (i%nmask_block)*64/nmask_block;

				TerrBlockLayerInfo *pchunkinfo = pEditor->GetEditData()->chunkSet[ggy][ggx].GetChunkInfo() ;
				if ( 0 != pData->pdata[i*gx + j] )
				{
					pchunkinfo->info[k][64*my + mx] = 1;
				}
				else
				{
					pchunkinfo->info[k][64*my + mx] = 0;
				}

			}
		}

		return true;
	}

}

