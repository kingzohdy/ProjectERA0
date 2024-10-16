
#include "GameResDBTypes.h"
#include "mtileprocess.h"
#include "TerrainTile.h"
#include "EditorCamera.h"
#include "EditorModel.h"
#include "MArchive.h"
#include "MapLights.h"
#include "EnvRegion.h"
#include "LinkTile.h"
#include "MNPC.h"

#include "OgreTerrainData.h"
#include "OgreTexture.h"
#include "OgrePlantData.h"

using namespace TE;
using namespace Ogre;

extern bool g_bNewMap;

extern void  MMaxVec(MVec3 *srcVec , MVec3 *desVec);
extern void  MMinVec(MVec3 *srcVec , MVec3 *desVec);

MTileProcess   g_TileProcess;

MTileProcess::MTileProcess(void)
{
}

MTileProcess::~MTileProcess(void)
{
}

bool MTileProcess::CreateTitle( MTerrainEdit* pEditor , int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum,
								const char* basetex , int nRepeat,
								int gwidthnum ,int gheightnum )
{	
	//每次新建或者加载时，进行初始化
	pEditor->GetAutoLinkMesh()->Init(2, MPoint2D(gx,gy), MPoint2D(gwidthnum, gheightnum));
	pEditor->GetAutoLinkTex()->Init(2, MPoint2D(gx,gy), MPoint2D(gwidthnum, gheightnum));
	pEditor->GetPaintWallPaper()->Init(2, MPoint2D(gx,gy), MPoint2D(gwidthnum, gheightnum));
//	pEditor->GetAutoLinkWhiteWater()->Init(2, MPoint2D(gx,gy), MPoint2D(gwidthnum, gheightnum));

	g_bNewMap = true;
	return CreateTitle_Internal(pEditor,tX,tY,fSize,gx,gy,height,heightmax,randsize,smoothnum,basetex,nRepeat,gwidthnum,gheightnum);
}   

//计算崖壁的顶点光照	
void  MTileProcess::CountLinkMeshColor( MTerrainEdit* pEditor )
{
	if( pEditor->GetEditData() == NULL )
		return;

	for( size_t i = 0 ; i < pEditor->GetEditData()->modelData.modelSet.size() ; i ++ )
	{		
		//如果是崖壁则，计算对应模型的光照进入顶点
		if( pEditor->GetEditData()->modelData.modelSet[i]->GetModelData()->cType == 4 
				|| pEditor->GetEditData()->modelData.modelSet[i]->GetModelData()->cType == 5 )
			
		{ 		
			pEditor->GetEditData()->modelData.modelSet[i]->ClearLightColor();
			for ( size_t j = 0 ; j < g_LightManager.m_LightInfoSet.size() ; j ++ )
			{    
				pEditor->GetEditData()->modelData.modelSet[i]->CountLightColor( g_LightManager.m_LightInfoSet[j] );
			}
		}
	}
};

bool MTileProcess::CreateTitle_Internal( MTerrainEdit* pEditor , int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum, const char* basetex , int nRepeat, int gwidthnum ,int gheightnum)
{
	if( gx <= 0 )
		gx = 1;

	if( gy <= 0 )
		gy = 1;

	if( gx > TERRTILE_NUMBLOCK_X )
		gx = TERRTILE_NUMBLOCK_X;

	if( gy > TERRTILE_NUMBLOCK_Z )
		gy = TERRTILE_NUMBLOCK_Z;

	pEditor->ClearAll();

	g_Terrain.Release ();

	if( height == heightmax )
		heightmax = height + 1;

	MTitle* title = new MTitle;
	title->SetRepeat( nRepeat );

    title->GetTileData ()->gw = gx ;
    title->GetTileData ()->gh = gy ;	

	title->GetTileHead()->gridX = gx ;
	 title->GetTileHead()->gridY = gy ;

	//为了使用任意CHUNK数的地图，防止不必要的BUG， 产生时，还是按16X16产生高度图，方便以后可以任意扩充地图大小
	pEditor->CreateHeightMapEX(MPoint2D(gx,gy), MPoint2D(gwidthnum,gwidthnum),(int)height , (int)heightmax , randsize , smoothnum);

	float xbaseT,zbaseT;
	xbaseT = fSize * tX;
	zbaseT = fSize * tY;

	title->GetTileHead()->tileX = tX ;
	title->GetTileHead()->tileY = tY ;

	float chunksize = fSize / (TERRTILE_NUMBLOCK)*1.0f;
	float unitsize  = chunksize / (gwidthnum*1.0f);
	float xbase,zbase;

	pEditor->SetEditData( title->GetTitleData () );

	pEditor->GetEditData() ->texData .texFileSet .push_back ( basetex );

	title->GetHeadData ()->gridSize = TERRBLOCK_SIZE;

	for( int j = 0 ; j < gx ; j++)
	{
		for( int i = 0 ; i < gy ; i++)
		{
			xbase = xbaseT + chunksize*i;
			zbase = zbaseT + chunksize*j;
           
			TerrBlockVertTable *pVecData = pEditor->GetEditData() ->chunkSet[j][i].GetChunkNewVecData();
			TerrBlockData* pData = pEditor->GetEditData() ->chunkSet[j][i].GetChunkData ();

			pData->gx = i;
			pData->gy = j;

			pData->maxVec = MVec3(-99999999.0f,-99999999.0f,-99999999.0f);
			pData->minVec = MVec3(9999999.0f,99999999.0f,99999999.0f);

			pVecData->ngrid = gwidthnum ;
			pData->vecNum = gwidthnum ;

			pVecData->vVer.resize( (gwidthnum+1)*(gwidthnum+1) );

			MChunkRenderPassSet* pPassData = pEditor->GetEditData() ->chunkSet[j][i].GetRenderPassSet();
			pPassData->bDraw.resize( pVecData->ngrid*pVecData->ngrid );
			memset( &pPassData->bDraw[0],0 , pVecData->ngrid*pVecData->ngrid );

			pEditor->GetEditData()->chunkSet[j][i].CreatePass();

			for(int jj = 0 ; jj < gheightnum +1; jj ++)
			{
				for(int ii = 0 ; ii < gwidthnum +1 ; ii++)
				{            
					height = pEditor->m_HeightMap.m_vHeight[(j*gwidthnum+jj)*(gy*gwidthnum+1)  + (i*gwidthnum+ ii)];

					float nrepeat = (float)pEditor->getMapInfo()->texrepeat;
			       pData->texUVInfo[0] = nrepeat;
			       pData->texUVInfo[1] = nrepeat;
			       pData->texUVInfo[2] = nrepeat;
			       pData->texUVInfo[3] = nrepeat;

					float xpos,zpos;
	
					xpos = ii * unitsize;
					zpos = jj * unitsize;
			
					MVec3  _uPos ;
					_uPos .x = xbase+xpos;
					_uPos.y = height;
					_uPos.z = zbase+zpos; 

					pData->maxVec = Maximize(pData->maxVec, _uPos);
					pData->minVec = Minimize(pData->minVec, _uPos);

				   pVecData->vVer[ii+jj*(gwidthnum+1)].pos  = _uPos;

				   pVecData->vVer [ii+jj*(gwidthnum+1)].color = 0xff000000 ;

			       pVecData->vVer[ii+jj*(gwidthnum+1)].normal .x = 0;
			       pVecData->vVer[ii+jj*(gwidthnum+1)].normal .y = 1;
			       pVecData->vVer[ii+jj*(gwidthnum+1)].normal .z = 0;

				   pEditor->m_HeightMap.m_vVec[(j*gwidthnum+jj)*(gy*gwidthnum+1)  + (i*gwidthnum+ ii)] =  pVecData->vVer[ii+jj*(gwidthnum+1)].pos;
                   pEditor->m_HeightMap.m_vNormal[(j*gwidthnum+jj)*(gy*gwidthnum+1)  + (i*gwidthnum+ ii)].x = 0;
                   pEditor->m_HeightMap.m_vNormal[(j*gwidthnum+jj)*(gy*gwidthnum+1)  + (i*gwidthnum+ ii)].y =  1;
                   pEditor->m_HeightMap.m_vNormal[(j*gwidthnum+jj)*(gy*gwidthnum+1)  + (i*gwidthnum+ ii)].z = 0;
				}
			}

			pData->texNum = 1;
			pData->texInfo [0] = 0;	

			pData->texUVInfo[0] = (float)nRepeat;
		}
	}

	pEditor->GetEditData() ->box.maxpos = MVec3(-9999999,-9999999,-9999999);
	pEditor->GetEditData() ->box.minpos = MVec3(9999999,9999999,9999999);

	for(int i = 0 ; i < gx ; i++)
	{
		for(int j = 0 ; j < gy ; j++)
		{
			TerrBlockData* pchunkdata = pEditor->GetEditData() ->chunkSet[j][i].GetChunkData ();

			pEditor->GetEditData()->box.maxpos = Maximize(pEditor->GetEditData()->box.maxpos, pchunkdata->maxVec);
			pEditor->GetEditData()->box.minpos = Minimize(pEditor->GetEditData()->box.minpos, pchunkdata->minVec);
		}
	}

	title->CreateData () ;
    g_Terrain.AddTitle( title );
			
	pEditor->UpdataNormalMap();
	pEditor->UpdateHeightMapToAllTileHeight(); 

	//title->AddRenderDataToRoom();

    MVec3 pos;
	g_Terrain.GetGridPos ( &pos , gx/2 , gy/2 );

	pos.y += 500.0f;
	g_Camera.reset();
	g_Camera.SetPosition ( pos.x  , pos.y  ,pos.z );	

	/*
    }
	catch(...)
	{
		LOG_SEVERE( "CreateTitleEx error!");
	}*/

	g_AreaSet.release();
	g_AreaSet.init();

	g_TerrainEdit.SetUpdateAreaData(true);

	return true;
}

static void ConvertLiquidData(TerrainBlockLiquidData &dest, const TerrBlockLiquidData &src)
{
	dest.type = src.type;
	strcpy(dest.name, src.name);

	dest.waterheight = src.waterheight;
	dest.shallowcolor = src.shallowcolor;
	dest.deepcolor = src.deepcolor;
	dest.shallowdepth = src.shallowdepth;
	dest.deepdepth = src.deepdepth;
	dest.maxdepth = src.maxdepth;
	dest.texrepeat = src.texrepeat;
	dest.speed = src.speed;
	dest.amp = src.amp;
}

bool  MTileProcess::SaveTerrainFile(  MTerrainEdit* pEditor ,const char* filename )
{
	pEditor->m_curSaveFile = filename;

	//测试先计算点光源光照进入对应数据
	CountLinkMeshColor( pEditor );

	//保存拼接地图信息
	std::string infofilename = filename ;
	infofilename = infofilename + ".link";
	pEditor->GetAutoLinkMesh()->SaveLinkMap( infofilename.c_str() );

	infofilename = filename ;
	infofilename = infofilename + ".linktex";
	pEditor->GetAutoLinkTex()->SaveLinkMap( infofilename.c_str() );

	infofilename = filename ;
	infofilename = infofilename + ".paper";
	pEditor->GetPaintWallPaper()->SaveLinkMap( infofilename.c_str() );

	infofilename = filename ;
	infofilename = infofilename + ".brae";
	pEditor->SaveBrae( infofilename.c_str() );

    // 保存区域信息
    infofilename = filename;
    infofilename = infofilename + ".area";
    pEditor->SaveAreaFile(infofilename.c_str());

	//保存npc信息
	infofilename = filename;
	infofilename = infofilename + ".npc";
	pEditor->getNpcManager()->save(infofilename.c_str());

	//保存灯光信息
	infofilename = filename ;
	infofilename = infofilename + ".light";
	g_LightManager.SaveLightInfo( infofilename.c_str() );

	infofilename = filename ;

	int pathIndex = infofilename.find ( "maps" ); 
	std::string pathStr = infofilename.substr( 0 , pathIndex + 5 );
	std::string nameStr = infofilename.substr( pathIndex + 5 , infofilename.length () - pathIndex );
	infofilename =  pathStr +  nameStr  ;
	g_Terrain.GetTitle(0)->SavePlantFile( infofilename.c_str() );

	if( !g_Terrain.SaveCurTile( filename))
	{
		return false;
	}
    
	std::string fileEx = filename;
	fileEx = fileEx  ;


	//保存MAY的地图植物资源
	std::string strPath , strName ;
	int _index = fileEx.rfind ("\\") ;
	if( _index == -1 )
		_index = fileEx.rfind ( "/" );

	strPath = fileEx.substr( 0 , _index+1 );
	strName = fileEx.substr( _index+1 );

	savePlantsForEachBlock(pEditor, strPath.c_str(), strName.c_str());
   
	//根据当前地图产生新地图格式=========================================================================    
	TerrainTileSource *pData = new TerrainTileSource;
	pData->m_TileDesc .texFileNum = pEditor->GetEditData()->texData .texFileSet .size ();
    
	for(unsigned int i = 0 ;i < pEditor->GetEditData()->texData .texFileSet .size () ;i ++)
	{
        pData->m_TileData .m_texFileSet.push_back ( pEditor->GetEditData()->texData .texFileSet[i] );
	}
	pData->m_TileDesc .tileX = g_Terrain.GetTitle (0)->GetHeadData()->tileX ;
	pData->m_TileDesc .tileY = g_Terrain.GetTitle (0)->GetHeadData()->tileY ;
	
	pData->m_TileDesc .gridSize = TERRBLOCK_SIZE;

	pData->m_TileDesc .gridX = pEditor->GetEditData()->gw ;
	pData->m_TileDesc .gridY = pEditor->GetEditData()->gh ;


	pData->m_TileDesc .gridVecNum = pEditor->GetEditData()->chunkSet[0][0].GetChunkData()->vecNum ;
	
	pData->m_TileDesc .ModelFileNum = pEditor->GetEditData()->modelData .modelFileSet .size ();
	for(unsigned int i = 0 ;i < pEditor->GetEditData()->modelData .modelFileSet .size () ; i++)
	{
	   pData->m_TileData .m_modelFileSet.push_back ( pEditor->GetEditData()->modelData .modelFileSet[i] );
	}
    
	int nModelNum = 0;
	//根据模型文件的信息过滤，地图的出生点等POI信息
	//g_MapInfoManager.m_nPoiNum = 0;

	for(unsigned int i = 0 ; i < pEditor->GetEditData()->modelData .modelSet .size () ; i++)
	{		
		if ( SaveSignToMySql( pEditor , i ) )
			continue;
	 
		std::string strname;
		if ( pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->cType == TYPE_LINK_MODEL ||  pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->cType == TYPE_LINK_TEXTURE )
			strname = pEditor->GetEditData()->modelData.modelFileSet[pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->modelID&0x0000ffff];
		else
			strname = pEditor->GetEditData()->modelData.modelFileSet[pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->modelID];

		if ( strname.find("_C_C") != -1 && strname.find("linkmesh") != -1)
			continue;

		TileModel *pModel = new TileModel;

	   pModel->getModelData ()->pos = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->pos ; 
	   pModel->getModelData ()->dir  = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->dir ;
	   pModel->getModelData ()->scale  = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->scale  ;
	   pModel->getModelData ()->modelID  = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->modelID  ;
	   pModel->getModelData ()->type = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->cType ;
	   pModel->getModelData ()->gridX   = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData()->gridX ;
	   pModel->getModelData ()->gridY  = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData()->gridY ;

	   pModel->getModelData ()->userdata   = 
		   pEditor->GetEditData()->modelData .modelSet [i]->GetModelData()->unknow ;
		  
	   pData->m_TileData .m_pModelSet .push_back ( pModel );
	 
	   nModelNum++;
	   //如果有linkmesh ， 则增加对应修改后的高度
	   EditorModel *pMayModel = (EditorModel*)pEditor->GetEditData()->modelData .modelSet [i]->GetModelData()->pModel ;
	   if(pMayModel->HasLinkMesh())
	   {
		   pModel->getModelData ()->userdata = pData->m_TileDesc.linkInfoNum;

		   pData->m_TileDesc.linkInfoNum += pMayModel->m_LinkMeshHeight.size() ;
		   
		   for( size_t kk = 0 ; kk < pMayModel->m_LinkMeshHeight.size() ; kk ++ )
		   {
			   TerrainLinkMeshData _meshData;
			   _meshData.fHeight  = pMayModel->m_LinkMeshHeight[kk];
			   _meshData.ulColor  = pMayModel->m_LinkMeshColor[kk];

			   pData->m_TileData.m_LinkMeshData.push_back( _meshData  ) ;
		   }
	   }
	}

	pData->m_TileDesc.ModelNum = nModelNum;

	pData->m_TileDesc .PlantNum = pEditor->GetEditData()->modelData .modelPlantSet .size ();

	for(unsigned int i = 0 ; i < pEditor->GetEditData()->modelData .modelPlantSet .size () ; i++)
	{	
		TileModel *pModel = new TileModel;

	    pModel->getModelData ()->pos = 
		   pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData ()->pos ;
	    pModel->getModelData ()->dir  = 
		    pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData ()->dir ;
	    pModel->getModelData ()->scale  = 
		   pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData ()->scale  ;
	     pModel->getModelData ()->modelID  = 
		   pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData ()->modelID  ;

		pModel->getModelData ()->type = 
				pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData ()->cType ;
			pModel->getModelData ()->gridX   = 
				pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData()->gridX ;
			pModel->getModelData ()->gridY  = 
				pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData()->gridY ;	

	   pModel->getModelData ()->userdata   = 
		   pEditor->GetEditData()->modelData .modelPlantSet [i]->GetModelData()->unknow;
			
		pData->m_TileData .m_pPlantModelSet .push_back ( pModel );
	}

	pData->m_TileData .m_pBlockSet.clear ();
	
	//增加该变量，主要是用于保存裁减网格
	pData->m_TileDesc.drawSize = pEditor->GetEditData()->chunkSet[0][0].GetChunkData()->vecNum ;

    for( int j = 0 ; j < pData->m_TileDesc .gridY  ; j ++)
	{
		for( int i = 0 ; i < pData->m_TileDesc .gridX ; i++)
		{     
			TerrainBlockSource *pBlock = new TerrainBlockSource;
			TerrBlockData *pchunkdata = pEditor->GetEditData()->chunkSet[j][i].GetChunkData();

			//test data
			pBlock ->m_Desc .ngridx = i;
			pBlock ->m_Desc .ngridy = j;
			pBlock ->m_Desc.alphamap_size = 64;
			pBlock ->m_Desc .shadowmap_size = 64;
			pBlock ->m_Desc .has_shadow = pchunkdata->bShadow ;
			pBlock ->m_Desc .has_water = pchunkdata->bHaswater ;
			pBlock ->m_Desc .gridsize = TERRBLOCK_SIZE;

			pBlock ->m_Desc .ngridVecNum = (pchunkdata->vecNum+1)*(pchunkdata->vecNum+1);

			pBlock ->m_Desc .visible = 1;
			pBlock ->m_Desc .minPos = pchunkdata->minVec;
			pBlock ->m_Desc .maxPos = pchunkdata->maxVec;

			pBlock->m_Heights.resize(pBlock ->m_Desc.ngridVecNum);
			pBlock->m_Normals.resize(pBlock ->m_Desc.ngridVecNum);
			pBlock->m_VertColors.resize(pBlock->m_Desc.ngridVecNum);
			//pBlock->m_VecData.resize ( pBlock ->m_Desc .ngridVecNum );
			
			{
				TerrBlockVertTable* pVecData = pEditor->GetEditData()->chunkSet[j][i].GetChunkNewVecData();
			    TerrBlockRenderData* pRenderData = pEditor->GetEditData()->chunkSet[j][i].GetChunkRenderData();
				//得到实际的高度
				if( pRenderData->vb )
				{	
					TerrainVertex* vdata = (TerrainVertex*)pRenderData->vb ->lock();
					for(int k = 0 ; k < pBlock ->m_Desc .ngridVecNum ; k++)
					{
						pBlock->m_Heights[k] = vdata[k].pos.y;
						pBlock->m_Normals[k] = pVecData->vVer[k].normal;
						pBlock->m_VertColors[k] = vdata[k].color;
						//pBlock->m_VecData[k].color = pVecData->vVer[k].color;
						//pBlock->m_VecData[k].color = vdata[k].color;
					}	
					pRenderData->vb->unlock();
				}
			}

			int  usemap = 0;
			uchar *psrcbits[4] = {NULL, NULL, NULL, NULL};
		    for(int k=0; k<pchunkdata->texNum; k++)
			{
				if(pchunkdata->texInfo[k] == 0xff) continue;

				if(usemap==0 || !TsetMapDataIsEmpty(pchunkdata->alpahMap[k-1].data))
				{
					if(usemap > 0) psrcbits[usemap-1] = pchunkdata->alpahMap[k-1].data;
					pBlock->m_Desc.nlayerInfo[usemap] = pchunkdata->texInfo[k];
					pBlock->m_Desc.nlayerUVInfo[usemap] = (unsigned char)pchunkdata->texUVInfo[k];

					usemap++;
					assert(usemap <= 4);
				}  
			}
			pBlock->m_pAlphaMap = new AlphaTexture(pBlock->m_Desc.alphamap_size, pBlock->m_Desc.alphamap_size, usemap-1);
			pBlock->m_pAlphaMap->setPixels(psrcbits);

			pBlock ->m_Desc.nlayers = usemap;

             //优化导出阴影图
			if( pBlock ->m_Desc .has_shadow != 0)
			{
				if ( !TsetMapDataIsEmpty( pchunkdata->shadowMap ) )
				{
					TextureDesc texdesc;
					memset(&texdesc, 0, sizeof(texdesc));
					texdesc.width = texdesc.height = pBlock->m_Desc.shadowmap_size;
					texdesc.format = PF_A8;
					texdesc.type = TEXTURE_2D;
					texdesc.nmipmap = 1;

					pBlock->m_pLightMap = new TextureData(texdesc);

					LockResult lockresult;
					unsigned char *pShadow = (uchar *)pBlock->m_pLightMap->lock(0, 0, false, lockresult);
					for( int uu = 0  ; uu < 64*64 ; uu ++ )
					{
						pShadow[uu] = 255 - pchunkdata->shadowMap[uu] ;
					}
					pBlock->m_pLightMap->unlock(0, 0);
				}
				else
				{
					pBlock ->m_Desc .has_shadow = 0;
				}
			}    


			if( pBlock ->m_Desc.has_water !=0 )
			{
				ConvertLiquidData(pBlock->m_LiquidData, *pEditor->GetEditData()->chunkSet [j][i].GetLiquidData());
			}
			
			pData->m_TileData .m_pBlockSet.push_back ( pBlock  );
			pData->m_TileData.m_pBlockSet[i+j*(pData->m_TileDesc.gridX )]->m_DrawInfo.resize( pData->m_TileDesc.drawSize*pData->m_TileDesc.drawSize);

			memcpy( &pData->m_TileData.m_pBlockSet[i+j*(pData->m_TileDesc.gridX )]->m_DrawInfo[0] ,
				&pEditor->GetEditData()->chunkSet[j][i].GetRenderPassSet ()->bDraw[0],pData->m_TileDesc.drawSize*pData->m_TileDesc.drawSize );

		}
	}

	//================================================================================================

/*	std::string terrainName = pEditor->m_curSaveFile + ".terrain" ;

		MArchive *Ar = g_FileManager.CreateFileWriter( terrainName.c_str()  , 0 );
	pTileChunk->m_pTile->Serialize( *Ar );
    MSAFE_DELETE( Ar );
*/

	std::string respath = std::string(filename) + ".tile";
	pData->save(respath.c_str());
	pData->release();
	return true;
}	

bool  MTileProcess::savePlantsForEachBlock(  MTerrainEdit* pEditor , const char* path ,const char* filename )
{
	for( int i = 0 ; i < TERRTILE_NUMBLOCK ; i ++ )
	{
		for( int j = 0 ; j < TERRTILE_NUMBLOCK ; j ++ )
		{
			MChunk &chunk = pEditor->GetEditData()->chunkSet[i][j];
			int plantKind = chunk.m_PlantMap.size(); 

			if(plantKind == 0) continue;

			PlantSource *plant = new PlantSource;
			MChunk::PLANT_MAP::iterator iter = chunk.m_PlantMap.begin();
			for(; iter != chunk.m_PlantMap.end() ; iter ++)
			{
				PlantVecInfo_T vecInfo;
				CPlantSet *plantset = iter->second;

				int nplant = plantset->m_nCurVecNum/4;
				vecInfo.m_PosSet.resize(nplant);
				vecInfo.m_ScaleSet.resize(nplant);
				vecInfo.m_VertColorSet.resize(nplant);
				vecInfo.m_pTexture = NULL;
				for( int k = 0 ; k < plantset ->m_nCurVecNum/4; k ++ )
				{
					vecInfo.m_PosSet[k] = plantset->m_SavePosSet[k];
					vecInfo.m_ScaleSet[k] = plantset->m_SaveScaleSet[k];
					vecInfo.m_VertColorSet[k] = plantset->m_unSaveColorSet[k];
				}

				Ogre::FixedString keystr = iter->first.c_str();
				plant->m_PlantData[keystr] = vecInfo;
			}

			char fullpath[256];
			sprintf(fullpath, "%s%s_%d_%d.plant", path, filename, j, i);

			plant->save(fullpath);
			plant->release();
		}
	}

	return true;
};

bool  MTileProcess::SaveSignToMySql(  MTerrainEdit* pEditor ,unsigned int index )
{/*

	MTitleModel *pModel = pEditor->GetEditData()->modelData.modelSet[index];

	assert( pModel );


	std::string str ;
	
	if( pModel->GetModelData()->cType == TYPE_LINK_TEXTURE || pModel->GetModelData()->cType == TYPE_LINK_MODEL )
	{
		str = pEditor->GetEditData()->modelData.modelFileSet[ pModel->GetModelData()->modelID & 0x0000ffff ] ;
	}
	else 
	{
		str = pEditor->GetEditData()->modelData.modelFileSet[ pModel->GetModelData()->modelID ] ;
	}

	bool bSign = false;
	int index1 = str.find("sign_0");
	if( str.find( "sign_0") != -1 )
	{
		bSign = true;
		//出生点
		g_MapInfoManager.m_CurSaveMapPoi.type = 0 ;
	}
	else if ( str.find("sign_1") != -1)
	{
		bSign = true;
		//回城点
		g_MapInfoManager.m_CurSaveMapPoi.type = 2 ;
	}
	else if ( str.find("sign_2")!= -1 )
	{
		bSign = true;
		//跳转点
		g_MapInfoManager.m_CurSaveMapPoi.type = 3 ;
	}
				
	if( bSign )
	{
		g_MapInfoManager.m_CurSaveMapPoi.angle = (int)pModel->GetModelData()->dir.x ;
		int mapID = (g_MapInfoManager.m_MapData.MapID&0x0000ffff)<<16 ;
		g_MapInfoManager.m_CurSaveMapPoi.id = mapID  + g_MapInfoManager.m_nPoiNum ;
		g_MapInfoManager.m_CurSaveMapPoi.size = 200;
		g_MapInfoManager.m_CurSaveMapPoi.x = int( pModel->GetModelData()->pos.x*10 );
		g_MapInfoManager.m_CurSaveMapPoi.y = int( pModel->GetModelData()->pos.y*10 );
		g_MapInfoManager.m_CurSaveMapPoi.z = int( pModel->GetModelData()->pos.z*10 );

		g_MapInfoManager.m_nPoiNum ++;

		g_MapInfoManager.SaveMapPoi();
		return true;
	}

*/	return false;;
}

bool  MTileProcess::TsetMapDataIsEmpty(unsigned char *data)
{
	for( int i = 0 ; i < 64*64 ; i ++)
	{
		if( data[i]!=0)
			return false;
	}

	return true;
}

bool  MTileProcess::LoadTerrainFile(  MTerrainEdit* pEditor ,const char* filename)
{
	pEditor->ClearAll();

	std::string   _filename = filename;
	int index = _filename.rfind (".");

	pEditor->m_curSaveFile = _filename.substr ( 0 , index );

	if ( g_Terrain.LoadTitle (filename ) )
	{
		MTileHead *phead = g_Terrain.GetTitle(0)->GetHeadData();
		pEditor->GetAutoLinkMesh()->Init(2, MPoint2D(phead->gridX, phead->gridY), MPoint2D(phead->gridVecNum, phead->gridVecNum));
		pEditor->GetAutoLinkTex()->Init(2, MPoint2D(phead->gridX, phead->gridY), MPoint2D(phead->gridVecNum, phead->gridVecNum));
		pEditor->GetPaintWallPaper()->Init(2, MPoint2D(phead->gridX, phead->gridY), MPoint2D(phead->gridVecNum, phead->gridVecNum));

		g_bNewMap = false;

		pEditor->SetTerrain( &g_Terrain );
			
		pEditor->UpdateTileHeightDataToHeightMap();

		MVec3 pos;
		if(!g_Terrain.GetGridPos(&pos, phead->gridX/2,phead->gridY/2))
			return false;

		pos.y += 500;
		g_Camera.reset();
		g_Camera.SetPosition(pos.x ,pos.y ,pos.z );			
		
		
		std::string  infofilename = pEditor->m_curSaveFile ;

		int pathIndex = infofilename.find ( "maps" ); 
		std::string pathStr = infofilename.substr( 0 , pathIndex + 5 );
		std::string nameStr = infofilename.substr( pathIndex + 5, infofilename.length () - pathIndex);
		infofilename =  pathStr + nameStr ;

		g_Terrain.GetTitle(0)->LoadPlantFile( infofilename.c_str() );

		//加载地图拼接对应信息
		infofilename = pEditor->m_curSaveFile + ".link";
		pEditor->GetAutoLinkMesh()->LoadLinkMap( infofilename.c_str() );

		infofilename = pEditor->m_curSaveFile + ".linktex";
		pEditor->GetAutoLinkTex()->LoadLinkMap( infofilename.c_str() );

	/*	infofilename = pEditor->m_curSaveFile + ".paper";
		pEditor->GetPaintWallPaper()->LoadLinkMap( infofilename.c_str() );*/

		infofilename = pEditor->m_curSaveFile + ".brae" ;
		pEditor->LoadBrae( infofilename.c_str() );

		infofilename = pEditor->m_curSaveFile + ".area" ;
        pEditor->LoadAreaFile( infofilename.c_str() );

		infofilename = pEditor->m_curSaveFile + ".npc";
		pEditor->getNpcManager()->load(infofilename.c_str());

		pEditor->GetAutoLinkMesh()->SetBeginLoadIDPos( g_Camera.m_Pos );
		pEditor->GetAutoLinkTex()->SetBeginLoadIDPos( g_Camera.m_Pos );

		return true;

	}

	return false;
}
