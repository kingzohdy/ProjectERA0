
#include "TerrainTile.h"
#include "EditorStream.h"
#include "TextureManager.h"
#include "EditorCamera.h"
#include "EditorModel.h"
#include "TerrainEdit.h"

#include "OgreModel.h"
#include "OgreTerrainBlock.h"
#include "OgreLiquid.h"
#include "OgrePlant.h"
#include "OgreRay.h"
#include "OgrePhysicsScene.h"
#include "OgreLog.h"

using namespace Ogre;

#define TERRAIN_MAPFILE_VERSION 103

namespace TE
{
	void TerrModel::Init()
	{ 
		m_ModelData.bInit = true;
		m_ModelData.bDrawBox = CanEditModelType(m_ModelData.cType);
		m_ModelData.pModel = new EditorModel;
		if(!m_ModelData.pModel ->LoadModel(m_ModelData.name))
		{
			char errorbuf[256];
			sprintf(errorbuf, "打开文件错误:%s", m_ModelData.name);
			MessageBox(NULL, errorbuf, "error", MB_OK);
			return;
		}

		if(strstr(m_ModelData.name, "character\\") == m_ModelData.name)
		{
			m_ModelData.pModel->GetShowModel()->setInstanceAmbient(ColourValue(0.23f, 0.23f, 0.23f, 0));
			m_ModelData.pModel->GetShowModel()->setUserType(1);
		}

		const BoxSphereBound &bound = ((EditorModel*)m_ModelData.pModel)->GetShowModel()->getWorldBounds();
		m_ModelData.m_pWireBox = new WireframeBox;
		m_ModelData.m_pWireBox->Init(bound.m_Center, bound.m_Extent);
	};


	//更新当前位置	
	void  TerrModel::UpdatePos()
	{
		EditorModel* pModel =  (EditorModel*)( m_ModelData.pModel ) ;	
		if( pModel == NULL )
		{
			return;
		}

		pModel->SetPos ( m_ModelData.pos );
		pModel->Update(0);
	};

	void  TerrModel::ClearLightColor( )
	{
		EditorModel*  pModel = (EditorModel*)( m_ModelData.pModel ) ;	   
		assert( pModel );

		pModel->ClearLightColor( );
	};

	void  TerrModel::CountLightColor( LightInfo &lightInfo )
	{
		EditorModel*  pModel = (EditorModel*)( m_ModelData.pModel ) ;	   
		assert( pModel );

		pModel->CountLightColor( lightInfo );
	};

	void  TerrModel::CountLightColor(
		int lighttype, 
		Ogre::Vector3 pos,
		Ogre::ColourValue color,
		Ogre::ColourValue ambient, 
		float range, 
		float shadowdensity)
	{
		EditorModel*  pModel = (EditorModel*)( m_ModelData.pModel ) ;	   
		assert( pModel );

		LightInfo lightInfo;
		lightInfo.pos = pos;
		lightInfo.color = color;
		lightInfo.fRange = range;
		lightInfo.nType = 0;

		//pModel->CountLightColor( lighttype,pos,color,ambient,range,shadowdensity);
		pModel->CountLightColor(lightInfo);
	}

	//测试模型与射线相交;参数( 射线位置,射线方向) 返回 ( 为真表示成功 )
	bool  TerrModel::TestIntersect( Ogre::Vector3 pos , Ogre::Vector3 dir )
	{
		EditorModel*  pModel = (EditorModel*)( m_ModelData.pModel ) ;	   
		assert( pModel );

		/*
		Model *pNode = pModel->GetShowModel()->getMainModel();  
		assert( pNode );

		Ray ray(pos, dir);
		return pNode->intersectRay(ray, NULL);
		*/
		Ray ray(pos, dir);
		return pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL);
	};

	//设置半透明; 参数（ 半透明值 ( 0 --- 1.0f ) );	目前暂时不用
	void  TerrModel::SetTransparent(float alpha)
	{
		EditorModel *pModel = (EditorModel*)m_ModelData.pModel ;

		pModel->GetShowModel()->setTransparent(alpha);
	};

	void TerrModel::AttachRoom()
	{
		if(m_pAttachedScene != NULL)
		{
			DetachRoom();
		}

		m_PhyModelHandle = NULL;
		EditorModel *pModel = (EditorModel*)m_ModelData.pModel;

		m_pAttachedScene = &g_Terrain;
		pModel->GetShowModel()->attachToScene(m_pAttachedScene);

		Model *pmodel = pModel->GetShowModel()->getMainModel();
		if(pmodel && pmodel->getModelData()->m_pBspData)
		{
			m_PhyModelHandle = g_Terrain.getPhysicsScene()->addBSPData(pmodel->getModelData()->m_pBspData, pmodel->getWorldMatrix());
		}
	}

	void TerrModel::AddToPhysicsScene2()
	{
		EditorModel *pModel = (EditorModel*)m_ModelData.pModel;

		Model *pmodel = pModel->GetShowModel()->getMainModel();
		if(pmodel && pmodel->getModelData()->m_pBspData)
		{
			g_Terrain.getPhysicsScene2()->addStaticBSPData(pmodel->getModelData()->m_pBspData, pmodel->getWorldMatrix());
		}
	}

	void TerrModel::DetachRoom()
	{
		EditorModel *pModel = (EditorModel*)m_ModelData.pModel;

		if(m_pAttachedScene)
		{
			g_Terrain.getPhysicsScene()->removeBSPData(m_PhyModelHandle);
			pModel->GetShowModel()->detachFromScene();
			m_pAttachedScene = NULL;
		}
	}

	//更新时间说； 参数（ 每次间隔时间 )
	void TerrModel::Update(float time)
	{
		EditorModel *pModel = (EditorModel*)m_ModelData.pModel ;

		MovableObject *pobj = pModel->GetShowModel();
		pobj->setPosition(m_ModelData.pos);
		pobj->setRotation(m_ModelData.dir.x, m_ModelData.dir.y, m_ModelData.dir.z);
		pobj->setScale(m_ModelData.scale);

		pobj->update(TimeToTick(time));

		const BoxSphereBound &bound = pobj->getWorldBounds();

		m_ModelData.m_pWireBox->SetRange(bound.getCenter(), bound.getExtension());
	}

	//设置模型 ; 参数（ 模型指针 ) 返回真表示成功 )
	bool TerrModel::SetShowModel( Ogre::Entity * pModel )
	{
		if( pModel == NULL )
			return false;

		//安全删除
		delete( m_ModelData.pModel );

		m_ModelData.pModel = new EditorModel;

		((EditorModel*)m_ModelData.pModel)->SetShowModel ( pModel );


		const BoxSphereBound bound = ((EditorModel*)m_ModelData.pModel)->GetShowModel()->getWorldBounds();
		m_ModelData.m_pWireBox = new WireframeBox;
		m_ModelData.m_pWireBox->Init(bound.m_Center, bound.m_Extent);
		m_ModelData.bDrawBox = true;

		pModel->playMotion("0");

		m_ModelData.bInit = true ;
		return true;

	};
	//设置是否显示，参数（ 为真表示显示 ）	
	void TerrModel::SetShow(bool show )
	{
		m_ModelData.bShow = show;
		if(m_ModelData.pModel) m_ModelData.pModel->GetShowModel()->show(show);

		/*
		if( show )
		{

		AttachRoom();
		}
		else
		{
		DetachRoom();
		}*/
	}

	TerrModel::TerrModel()
	{
		memset(&m_ModelData,0,sizeof(TerrModelData));

		m_ModelData.scale = 1.0f;
		m_ModelData.bShow = true;
		m_ModelData.bInit = false;

		m_pAttachedScene = NULL;
	};

	TerrModel::~TerrModel()
	{
		DetachRoom();

		delete( m_ModelData.pModel ) ;

		delete( m_ModelData.m_pWireBox );
	}
	//重新设置包围盒	
	void TerrModel::ResetBox()
	{
		if( m_ModelData.pModel )
		{
			EditorModel* pMayModel = static_cast<EditorModel*>( m_ModelData.pModel );

			Model *pmodel = ((EditorModel*)m_ModelData.pModel)->GetShowModel() ->getMainModel();
			if(pmodel)
			{
				const BoxSphereBound &bound = pmodel->getWorldBounds();
				m_ModelData.m_pWireBox ->GetData ()->center = bound.m_Center;
				m_ModelData.m_pWireBox ->UpdataData ();
			}
		}
	}
	//重置矩阵
	void TerrModel::ResetMat()
	{
		Quaternion quat;
		quat.setEulerAngle(m_ModelData.dir.x, m_ModelData.dir.y, m_ModelData.dir.z);
		m_ModelData.mat.makeSRTMatrix(Vector3(m_ModelData.scale, m_ModelData.scale, m_ModelData.scale), quat, m_ModelData.pos);

		m_ModelData.pModel->SetDir(m_ModelData.dir);
		m_ModelData.pModel->SetPos(m_ModelData.pos);
		m_ModelData.pModel ->SetScale ( m_ModelData.scale );

		EditorModel *pModel = (EditorModel*)m_ModelData.pModel ;	
		if( pModel )
		{
			//如果不是linkMesh ,则生成包围盒
			if(!pModel->HasLinkMesh())
			{
				Model *pNode = pModel->GetShowModel()->getMainModel();

				//如果是植被物件或者linkmesh物件，则不显示对应的BOX
				if(CanEditModelType(m_ModelData.cType))
				{
					m_ModelData.m_pWireBox->GetData()->tm = m_ModelData.mat;
					ResetBox();
				}
			}
			pModel->Update(0);
		}

		if(m_pAttachedScene)
		{
			DetachRoom();
			AttachRoom();
		}

		if(m_ModelData.reflected)
			pModel->GetShowModel()->addRenderUsageBits(RU_REFLECT);
		else
			pModel->GetShowModel()->clearRenderUsageBits(RU_REFLECT);

		if(m_ModelData.refracted) 
			pModel->GetShowModel()->addRenderUsageBits(RU_REFRACT);
		else pModel->GetShowModel()->clearRenderUsageBits(RU_REFRACT);

		if (m_ModelData.transable)
			pModel->GetShowModel()->addRenderUsageBits(RU_TRANSABLE);
		else
			pModel->GetShowModel()->clearRenderUsageBits(RU_TRANSABLE);
	};

	void TerrModel::onCull()
	{
		if( !m_ModelData.bInit )
		{
			Init();
			ResetMat();
		}

		if( !m_ModelData.bShow )
		{
			return;
		}

		if( m_ModelData.m_pWireBox && m_ModelData.bDrawBox )
		{
			m_ModelData.m_pWireBox ->Render();
		}
	}

	EditorTerrainTile::EditorTerrainTile(void)
	{
		m_TitleData.bShowChunk = true;
		m_TitleData.bShowFog = true;
		m_TitleData.bShowObject = true;
		m_TitleData.bShowQuadPath = false;
		m_TitleData.bShowWater = true;

		for( int i = 0 ;i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ; j++)
			{
				m_pBlock[i][j] = NULL ;
				m_pLiquid[i][j] = NULL ;

				m_pPlantNodeSet[i][j].clear();;
			}
		}
	}

	EditorTerrainTile::~EditorTerrainTile(void)
	{
		Release();
	}
	//更新地形半透明纹理
	void EditorTerrainTile::UpDateTerrainAlphaTex( int x ,int y)
	{
		//对输入参数进行检测
		if( x < 0 || x >= TERRTILE_NUMBLOCK_X )
		{
			LOG_INFO("UpDateTerrainAlphaTex( int x ,int y) ; error! x = %d" , x );
			return;
		}
		if( y < 0 || y < TERRTILE_NUMBLOCK_Z )
		{
			LOG_INFO("UpDateTerrainAlphaTex( int x ,int y) ; error! x = %d" , x );
			return;
		}

		Ogre::Texture *  pTex = m_TitleData.chunkSet[y][x].GetChunkRenderData()->mterrainAlphaTex ;
		if( pTex == NULL )
		{
			return;
		}

		TerrBlockData *pChunkData = m_TitleData.chunkSet[y][x].GetChunkData();

		unsigned char RGBATexdata[64*64][4];

		for(int i = 0;i< 64*64; i++)
		{
			for(int j = 0 ; j< pChunkData->texNum  - 1;j++)
			{
				RGBATexdata[i][j+1] = pChunkData->alpahMap [j].data[i];
			}
		}
		g_TexManager.Update( pTex , RGBATexdata, 64 ,64 , 0);

	}

	void   EditorTerrainTile::UpdateRepeat( int nRepeat )
	{
		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i++)
		{
			for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ; j++)
			{
				m_TitleData.chunkSet [i][j].UpdateTexRepeat ( nRepeat );
			}
		}
	};	

	void   EditorTerrainTile::SetRepeat(int nRepeat)
	{
		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i++)
		{
			for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ; j++)
			{
				m_TitleData.chunkSet [i][j].GetChunkOptionData ()->nRepeat = nRepeat ;
			}
		}
	};

	bool    EditorTerrainTile::InsterectRay(Ogre::Vector3 dir , Ogre::Vector3 pos , Ogre::Vector3 &result )
	{
		return false;
	};

	bool    EditorTerrainTile::SaveFile(const char* name)
	{      
		std::string mayName = name ;
		mayName = mayName + ".map" ;

		saveMapFile(mayName.c_str () );

		return true;
	}

	void   EditorTerrainTile::UpdateBox()
	{
		m_minPos = Ogre::Vector3(999999 , 999999 , 999999 );
		m_maxPos = Ogre::Vector3(-999999 , - 999999 , - 999999 );

		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
			{
				m_minPos = Minimize(m_minPos, m_TitleData.chunkSet [j][i].GetChunkData()->minVec);
				m_maxPos = Maximize(m_maxPos, m_TitleData.chunkSet [j][i].GetChunkData()->maxVec);
			}
		}
	}

	struct MapFileHeader
	{
		char magic_number[4];
		int version;
	};

	static float g_MapScale;
	void ScaleMapPoint(Ogre::Vector3 &pt)
	{
		pt.x *= g_MapScale;
		pt.z *= g_MapScale;
	}

	struct MTileMesh_100
	{
		Ogre::Vector3 pos;//位置
		Ogre::Vector3 rot;//旋转
		float scale;//缩放
		unsigned char meshfileID;//模型文件ID
		unsigned char type;//类型
		unsigned char gx,gy;//对应哪个格子
		int          unknow;//预留
		int          index;//索引
	};

	void LoadTileMesh(MTileMesh &_tileMesh, EditorStream &readStream, int version)
	{
		if(version < 101)
		{
			MTileMesh_100 oldmesh;
			readStream.Read(oldmesh);
			_tileMesh.pos = oldmesh.pos;
			_tileMesh.rot = oldmesh.rot;
			_tileMesh.scale = oldmesh.scale;
			_tileMesh.meshfileID = oldmesh.meshfileID;
			_tileMesh.type = oldmesh.type;
			_tileMesh.gx = oldmesh.gx;
			_tileMesh.gy = oldmesh.gy;
			_tileMesh.unknow = oldmesh.unknow;
			_tileMesh.index = oldmesh.index;
			_tileMesh.reflected = _tileMesh.refracted = _tileMesh.transable = false;
		}
		else if(version == 101)
		{
			MTileMesh_101 oldmesh;
			readStream.Read(oldmesh);
			_tileMesh.pos = oldmesh.pos;
			_tileMesh.rot = oldmesh.rot;
			_tileMesh.scale = oldmesh.scale;
			_tileMesh.meshfileID = oldmesh.meshfileID;
			_tileMesh.type = oldmesh.type;
			_tileMesh.gx = oldmesh.gx;
			_tileMesh.gy = oldmesh.gy;
			_tileMesh.unknow = oldmesh.unknow;
			_tileMesh.index = oldmesh.index;
			_tileMesh.reflected = _tileMesh.refracted = _tileMesh.transable = false;
		}
		else if (version == 102)
		{
			MTileMesh_102 oldmesh;
			readStream.Read(oldmesh);
			_tileMesh.pos = oldmesh.pos;
			_tileMesh.rot = oldmesh.rot;
			_tileMesh.scale = oldmesh.scale;
			_tileMesh.meshfileID = oldmesh.meshfileID;
			_tileMesh.type = oldmesh.type;
			_tileMesh.gx = oldmesh.gx;
			_tileMesh.gy = oldmesh.gy;
			_tileMesh.unknow = oldmesh.unknow;
			_tileMesh.index = oldmesh.index;
			_tileMesh.reflected = false;
			_tileMesh.refracted = false;
			_tileMesh.transable = false;
		}
		else readStream.Read(_tileMesh);
	}

	bool   EditorTerrainTile::loadMapFile(const char* name)
	{
		EditorStream   readStream;

		readStream.ReadFile(name);

		unsigned char test_oldver;
		readStream.Read(test_oldver);
		readStream.Rewind();

		MapFileHeader header;
		memset(&header, 0, sizeof(header));
		if(test_oldver != 0) //new version
		{
			readStream.Read(header);		
		}

		readStream.Read (m_TileHead);
		g_MapScale = TERRBLOCK_SIZE/m_TileHead.gridSize;
		m_TileHead.gridSize = TERRBLOCK_SIZE;

		float xbaseT ,zbaseT;
		xbaseT =  m_TileHead.tileX * m_TileHead.gridSize * m_TileHead.gridX ;
		zbaseT =  m_TileHead.tileY * m_TileHead.gridSize * m_TileHead.gridY ;

		float unitsize = m_TileHead.gridSize / 8.0f;

		m_TitleData.gw = m_TileHead.gridX ;
		m_TitleData.gh = m_TileHead.gridY ;

		m_TitleData.box.minpos = Ogre::Vector3(999999.9f , 999999.9f , 999999.9f);
		m_TitleData.box.maxpos = Ogre::Vector3(-999999.9f , -999999.9f , -999999.9f);

		float xbase,zbase;

		m_TitleData.texData .texFileSet.resize ( m_TileHead.texFileNum );
		m_TitleData.modelData .modelFileSet.resize ( m_TileHead.MeshFileNum );


		MTileTexture _filename;
		for(int i = 0 ; i < m_TileHead.texFileNum ; i++)
		{	
			readStream.Read  ( _filename );
			m_TitleData.texData .texFileSet [i] = _filename.filename ;
		}

		for(unsigned int i = 0 ; i < m_TileHead.MeshFileNum ; i++)
		{
			readStream.Read ( _filename );
			m_TitleData.modelData .modelFileSet [i] = _filename.filename ;
		}

		TerrBlockHeader  _chunkHead;
		for( int j = 0 ; j < m_TileHead.gridY ; j ++)
		{
			for( int i = 0 ; i < m_TileHead.gridX ; i++)
			{
				EditorTerrainBlock &curchunk = m_TitleData.chunkSet[j][i];
				xbase = xbaseT + m_TileHead.gridSize*i ;
				zbase = zbaseT + m_TileHead.gridSize*j ;

				curchunk.GetChunkData ()->gx = i;
				curchunk.GetChunkData ()->gy = j;

				readStream.Read ( _chunkHead );
				ScaleMapPoint(_chunkHead.MinVec);
				ScaleMapPoint(_chunkHead.MaxVec);

				for( int uu = 0 ; uu < 4 ; uu ++ )
				{
					curchunk.GetChunkData ()->texUVInfo[uu] = _chunkHead.texUVInfo[uu];
				}

				if( _chunkHead.bHasLiquid != 0 )
					curchunk.GetChunkData ()->bHaswater = true  ;
				else	
					curchunk.GetChunkData ()->bHaswater = false;

				if( _chunkHead.bShadowMap != 0)
					curchunk.GetChunkData ()->bShadow = true  ;
				else
					curchunk.GetChunkData ()->bShadow = false  ;


				xbase = _chunkHead.MinVec .x ;
				zbase = _chunkHead.MinVec .z ;

				for(int k = 0 ; k < 4 ; k++)			
				{
					curchunk.GetChunkData ()->texInfo [k] = _chunkHead.texInfo [k]  ;
				}			  

				curchunk.GetChunkData ()->vecNum = m_TileHead.gridVecNum  ;


				{

					TerrBlockVertTable* pVecData = curchunk.GetChunkNewVecData();

					pVecData->ngrid = m_TileHead.gridVecNum ;

					unitsize = m_TileHead.gridSize / ( pVecData->ngrid*1.0f ) ;

					pVecData->vVer.resize( (m_TileHead.gridVecNum+1)*(m_TileHead.gridVecNum+1) );

					//创建，对应的索引列表
					curchunk.CreatePass();

					int vIndex = 0;
					for( int tt = 0 ; tt < pVecData->ngrid +1 ; tt ++ )
					{
						for ( int ww = 0 ; ww < pVecData->ngrid +1 ; ww++ )
						{
							TerrainVertex vec ;
							readStream.ReadFloat( &vec.pos.y );
							vec.pos.x = xbase + unitsize*ww ;
							vec.pos.z = zbase + unitsize*tt ;
							pVecData->vVer[vIndex] = vec;
							vIndex++;
						}
					}

				}

				curchunk.GetChunkData ()->texNum = _chunkHead.texNum   ;		  
				curchunk.GetChunkData ()->minVec = _chunkHead.MinVec ;
				curchunk.GetChunkData ()->maxVec = _chunkHead.MaxVec ;	

				m_TitleData.box.minpos = Minimize(m_TitleData.box.minpos, _chunkHead.MinVec);
				m_TitleData.box.maxpos = Maximize(m_TitleData.box.maxpos, _chunkHead.MaxVec);

				{			
					TerrBlockVertTable* pVecData = curchunk.GetChunkNewVecData();
					for( size_t tt = 0 ; tt < pVecData->vVer.size() ; tt++)
					{
						readStream.ReadFloat  ( &pVecData->vVer[tt].normal.x );
						readStream.ReadFloat  ( &pVecData->vVer[tt].normal.y );
						readStream.ReadFloat  ( &pVecData->vVer[tt].normal.z );
					}


					//增加顶点颜色
					for( size_t tt = 0 ; tt < pVecData->vVer.size() ; tt++)
					{
						readStream.Read( &pVecData->vVer[tt].color , sizeof( unsigned int ) );
					}
				}



				for( int k = 0 ; k < _chunkHead.texNum -1 ; k++)
				{
					readStream.ReadUnChar ( curchunk.GetChunkData ()->alpahMap [k].data ,64*64);
				}

				if( _chunkHead.bShadowMap != 0)
				{
					readStream.ReadUnChar  ( curchunk.GetChunkData ()->shadowMap ,64*64);

					for( int tt = 0 ; tt < 64*64 ; tt ++ )
					{
						if( curchunk.GetChunkData ()->shadowMap[tt] == 255 )
						{
							curchunk.GetChunkData ()->shadowMap[tt] = 85 ;
						}
						else
						{
							curchunk.GetChunkData ()->shadowMap[tt] = 0 ;
						}
					}
				}

				if( _chunkHead.bColorMap != 0 )
				{
					if(header.version == 0)
					{
						readStream.ReadUnChar(curchunk.GetChunkData ()->ColorMap , 64*64*4);
						curchunk.GetChunkData()->bColor = true;
					}
					else
					{
						readStream.Read(curchunk.m_InfoData.info);
						curchunk.GetChunkData()->bColor = true;
					}
				}

				if( _chunkHead.bHasLiquid != 0)
				{
					TerrBlockLiquidData *pliquid = curchunk.GetLiquidData();
					readStream.Read(pliquid, sizeof(TerrBlockLiquidData));
				}
			}
		}

		MTileMesh  _tileMesh;

		m_TitleData.modelData.modelSet.resize(0);
		for( unsigned int i = 0 ; i < m_TileHead.MeshNum ; i ++)
		{
			LoadTileMesh(_tileMesh, readStream, header.version);
			ScaleMapPoint(_tileMesh.pos);
			if(_tileMesh.meshfileID >= m_TitleData.modelData.modelFileSet.size())
			{
				continue;
			}

			TerrModel *pmodel = new TerrModel;

			TerrModelData *pmodeldata = pmodel->GetModelData();
			strcpy(pmodeldata->name, m_TitleData.modelData.modelFileSet [_tileMesh.meshfileID].c_str());
			pmodeldata->modelID  = _tileMesh.meshfileID;
			pmodeldata->pos      = _tileMesh.pos;
			pmodeldata->scale    = _tileMesh.scale;
			pmodeldata->dir      = _tileMesh.rot;
			pmodeldata->cType    = _tileMesh.type;
			pmodeldata->mapIndex = _tileMesh.index;
			pmodeldata->reflected = _tileMesh.reflected;
			pmodeldata->refracted = _tileMesh.refracted;
			pmodeldata->transable = _tileMesh.transable;

			pmodel->SetShow(false);
			pmodeldata->bInit  = false;

			m_TitleData.modelData.modelSet.push_back(pmodel);
		}

		m_TitleData.modelData .modelPlantSet .resize ( m_TileHead.PlantNum );
		for( int i = 0 ; i < m_TileHead.PlantNum ; i++)
		{
			LoadTileMesh(_tileMesh, readStream, header.version);
			ScaleMapPoint(_tileMesh.pos);

			m_TitleData.modelData.modelPlantSet[i] = new TerrModel;
			TerrModelData *pmodeldata = m_TitleData.modelData.modelPlantSet[i]->GetModelData();

			strcpy( pmodeldata->name , m_TitleData.modelData .modelFileSet [_tileMesh.meshfileID].c_str ());
			pmodeldata->modelID = _tileMesh.meshfileID;
			pmodeldata->pos     = _tileMesh.pos;
			pmodeldata->scale   = _tileMesh.scale;
			pmodeldata->dir     = _tileMesh.rot;
			pmodeldata->mapIndex = _tileMesh.index;
			pmodeldata->reflected = _tileMesh.reflected;
			pmodeldata->refracted = _tileMesh.refracted;
			pmodeldata->transable = _tileMesh.transable;

			m_TitleData.modelData.modelPlantSet[i]->SetShow(false);
			pmodeldata->bInit  = false;

			pmodeldata->cType    = _tileMesh.type;

			//保留对于大量植物的处理
			m_TitleData.chunkSet [_tileMesh.gy][_tileMesh.gx].m_PlantSet.push_back(m_TitleData.modelData.modelPlantSet[i]);

		}

		//引入新的索引列表
		if( m_TitleData.chunkSet [0][0].GetChunkData ()->vecNum != 145 )
		{	
			int size = m_TitleData.chunkSet [0][0].GetChunkNewVecData()->ngrid ;
			m_TitleData.chunkSet [0][0].GetRenderPassSet()->bDraw.resize( size*size );

			for( int i = 0 ; i < m_TileHead.gridY ; i ++ ) 
			{
				for ( int j = 0 ;j < m_TileHead.gridX ;j ++ )
				{
					unsigned char cullInfo;
					for( int k = 0 ; k < size*size ; k ++ )
					{
						if( !readStream.Read( cullInfo ) )
						{
							LOG_SEVERE( "文件读取错误 :idx = %d" , k );
							i = TERRTILE_NUMBLOCK_Z ; j = TERRTILE_NUMBLOCK_X ;
							break;
						}
						else
						{
							if (cullInfo != 0 )
							{
								//	  LOG_SEVERE( "cull info: x = %d , y = %d , mx = %d , my = %d " , j , i , k % 32 , k / 32 );
							}

							m_TitleData.chunkSet [i][j].GetRenderPassSet()->bDraw[k] = cullInfo ;
						}
					}
				}
			}
		}

		return true;
	}

	bool    EditorTerrainTile::saveMapFile(const char* name)
	{
		EditorStream   saveStream;
		saveStream.WriteFile(name);

		MapFileHeader header;
		memcpy(header.magic_number, "EMAP", 4);
		header.version = TERRAIN_MAPFILE_VERSION;
		saveStream.Write(header);

		m_TileHead.texFileNum  = m_TitleData.texData .texFileSet .size ();

		//由于增加根据地形变化的网格，因此模型输出时，不输出自动拼接的模型
		int _nMeshNum = 0 ;
		std::string meshName ;

		for( size_t i = 0 ; i < m_TitleData.modelData.modelSet.size() ; i ++ )
		{
			if ( m_TitleData.modelData.modelSet[i]->GetModelData()->cType != TYPE_LINK_MODEL
				&&  m_TitleData.modelData.modelSet[i]->GetModelData()->cType != TYPE_LINK_TEXTURE )
			{
				_nMeshNum++;
			}

			meshName =  m_TitleData.modelData.modelSet[i]->GetModelData()->name ;

			if( meshName.find("physic")!= -1 )
			{	 
				m_TitleData.modelData.modelSet[i]->GetModelData()->cType = TYPE_PHYSIC_MODEL ;
			}
		}
		m_TileHead.MeshNum     = _nMeshNum;


		m_TileHead.MeshFileNum = m_TitleData.modelData .modelFileSet .size ();
		m_TileHead.PlantNum    = m_TitleData.modelData .modelPlantSet .size ();
		m_TileHead.gridX = m_TitleData.gw ;
		m_TileHead.gridY = m_TitleData.gh ;

		m_TileHead.gridSize = TERRBLOCK_SIZE ;

		bool  bWow = false;
		m_TileHead.gridVecNum = m_TitleData.chunkSet[0][0].GetChunkNewVecData()->ngrid ;

		saveStream.Write (m_TileHead);

		MTileTexture _filename;
		for(int i = 0 ; i < m_TileHead.texFileNum ; i++)
		{
			strcpy( _filename.filename , m_TitleData.texData .texFileSet [i].c_str ());
			saveStream.Write ( _filename );
		}

		for(unsigned int i = 0 ; i < m_TileHead.MeshFileNum ; i++)
		{
			strcpy( _filename.filename , m_TitleData.modelData .modelFileSet [i].c_str () );
			saveStream.Write ( _filename );
		}

		TerrBlockHeader  _chunkHead;
		for( int j = 0 ; j < m_TileHead.gridY ; j ++)
		{
			for( int i = 0 ; i < m_TileHead.gridX ; i++)
			{
				EditorTerrainBlock &curchunk = m_TitleData.chunkSet[j][i];
				if( curchunk.GetChunkData ()->bHaswater )
					_chunkHead.bHasLiquid = 1;
				else
					_chunkHead.bHasLiquid = 0;

				if( curchunk.GetChunkData ()->bColor )
					_chunkHead.bColorMap = 1;
				else
					_chunkHead.bColorMap = 0;

				_chunkHead.gx = i;
				_chunkHead.gy = j;
				_chunkHead.bShadowMap = curchunk.GetChunkData ()->bShadow ;
				_chunkHead.texNum =  curchunk.GetChunkData ()->texNum ;	
				_chunkHead.MinVec = curchunk.GetChunkData ()->minVec ;
				_chunkHead.MaxVec = curchunk.GetChunkData ()->maxVec ;

				for(int k = 0 ; k < 4 ; k++)			
				{
					_chunkHead.texInfo [k] = curchunk.GetChunkData ()->texInfo [k];
				}

				for( int k = 0 ; k < 4 ; k++)
				{
					unsigned char nRepeat = (unsigned char)curchunk.GetChunkData ()->texUVInfo[k] ;
					_chunkHead.texUVInfo [k] = nRepeat;
				}	

				saveStream.Write ( _chunkHead );


				{
					TerrBlockVertTable* pVecData = curchunk.GetChunkNewVecData();
					for( size_t k = 0 ; k < pVecData->vVer.size()  ; k++)
					{
						saveStream.WriteFloat ( &pVecData->vVer[k].pos .y  );
					}

					for(size_t k = 0 ; k <  pVecData->vVer.size() ; k++)
					{
						saveStream.WriteFloat ( &pVecData->vVer[k].normal.x );
						saveStream.WriteFloat ( &pVecData->vVer[k].normal.y );
						saveStream.WriteFloat ( &pVecData->vVer[k].normal.z );
					}

					//增加顶点颜色
					for(size_t k = 0 ; k <  pVecData->vVer.size() ; k++)
					{
						saveStream.Write ( &pVecData->vVer[k].color , sizeof( unsigned int ) );
					}
				}

				for( int k = 0 ; k < _chunkHead.texNum -1 ; k++)
				{
					saveStream.WriteUnChar ( curchunk.GetChunkData ()->alpahMap [k].data ,64*64);
				}

				if( _chunkHead.bShadowMap != 0)
				{
					saveStream.WriteUnChar ( curchunk.GetChunkData ()->shadowMap ,64*64);
				}

				if(  _chunkHead.bColorMap == 1 )
				{
					//saveStream.WriteUnChar  ( curchunk.GetChunkData ()->ColorMap ,4*64*64 );
					saveStream.Write(curchunk.m_InfoData.info);
				}

				if( _chunkHead.bHasLiquid != 0)
				{
					Ogre::TLiquid *pliquid = curchunk.m_pLiquid;
					curchunk.m_LiquidData.type = pliquid->m_WaterType;
					curchunk.m_LiquidData.waterheight = pliquid->getHeight();
					curchunk.m_LiquidData.texrepeat = pliquid->m_NumTexRepeat;

					if(pliquid->m_WaterType == 0) 
					{
					}
					//	strcpy(curchunk.m_LiquidData.name, (const char *)pliquid->m_TextureName);
					else
						curchunk.m_LiquidData.name[0] = 0;

					//general water
					curchunk.m_LiquidData.deepcolor = pliquid->m_DeepWaterColor;
					curchunk.m_LiquidData.deepdepth = pliquid->m_DeepWaterDepth;
					curchunk.m_LiquidData.shallowcolor = pliquid->m_ShallowWaterColor;
					curchunk.m_LiquidData.shallowdepth = pliquid->m_ShallowWaterDepth;
					curchunk.m_LiquidData.maxdepth = pliquid->m_MaxWaterDepth;

					//reflect water
					curchunk.m_LiquidData.amp		  = pliquid->m_Amp;
					curchunk.m_LiquidData.speed		  = pliquid->m_Speed;

					saveStream.Write(&curchunk.m_LiquidData, sizeof(TerrBlockLiquidData));
				}
			}
		}

		MTileMesh  _tileMesh;

		for( size_t k = 0 ; k < m_TitleData.modelData.modelSet .size () ; k ++)
		{
			TerrModelData *pmodeldata = m_TitleData.modelData.modelSet[k]->GetModelData();
			_tileMesh.meshfileID = pmodeldata->modelID ;
			_tileMesh.pos        = pmodeldata->pos ;
			_tileMesh.scale      = pmodeldata->scale ;
			_tileMesh.rot        = pmodeldata->dir ;
			_tileMesh.type       = pmodeldata->cType ;
			_tileMesh.index      = pmodeldata->mapIndex ;
			_tileMesh.reflected = pmodeldata->reflected;
			_tileMesh.refracted = pmodeldata->refracted;
			_tileMesh.transable = pmodeldata->transable;

			m_TitleData.modelData.modelSet [k]->GetModelData ()->gridX = (int)( _tileMesh.pos.x /   m_TileHead.gridSize );
			m_TitleData.modelData.modelSet [k]->GetModelData ()->gridY = (int)( _tileMesh.pos .z /  m_TileHead.gridSize );

			//如果该模型为 4 -- 表示link模型 ，则不记录地图信息
			if ( _tileMesh.type == TYPE_LINK_MODEL || _tileMesh.type == TYPE_LINK_TEXTURE )
				continue;

			assert(pmodeldata->modelID < 65535);

			_tileMesh.gx         = 0;
			_tileMesh.gy         = 0;

			_tileMesh.unknow =  pmodeldata->unknow ;

			saveStream.Write ( _tileMesh );

		}

		for( int i = 0 ; i < m_TileHead.gridY  ; i ++)
		{
			for( int j = 0 ; j < m_TileHead.gridX  ; j++)
			{
				for( size_t k = 0 ; k < m_TitleData.chunkSet [i][j].m_PlantSet .size () ; k ++)
				{
					TerrModelData *pmodeldata = m_TitleData.chunkSet [i][j].m_PlantSet[k]->GetModelData ();
					assert(pmodeldata->modelID < 65535);

					_tileMesh.meshfileID = pmodeldata->modelID ;
					_tileMesh.pos        = pmodeldata->pos ;
					_tileMesh.scale      = pmodeldata->scale ;
					_tileMesh.rot        = pmodeldata->dir ;
					_tileMesh.type       = pmodeldata->cType ;
					_tileMesh.index      = pmodeldata->mapIndex ;
					_tileMesh.reflected = pmodeldata->reflected;
					_tileMesh.refracted = pmodeldata->refracted;
					_tileMesh.transable = pmodeldata->transable;

					pmodeldata->gridX = j ;
					pmodeldata->gridY = i ;

					_tileMesh.gx          = j;
					_tileMesh.gy         = i;

					_tileMesh.unknow =  pmodeldata->unknow ;

					saveStream.Write ( _tileMesh );
				}
			}
		}

		int worldnum = 0 ;
		if( m_TitleData.chunkSet [0][0].GetChunkData()->vecNum != 145 )
		{
			int size = m_TitleData.chunkSet [0][0].GetChunkNewVecData()->ngrid ;


			for( int i = 0 ;i < m_TileHead.gridY ; i ++ )
			{
				for ( int j = 0 ;j < m_TileHead.gridX ; j ++ )
				{
					for( size_t k = 0 ; k < m_TitleData.chunkSet [i][j].GetRenderPassSet()->bDraw.size() ; k ++ )
					{
						unsigned char sign = m_TitleData.chunkSet [i][j].GetRenderPassSet()->bDraw[k];

						worldnum ++ ;

						if( sign != 0 )
						{
							//	  LOG_SEVERE( "cull pos: x = %d , y = %d ; mx = %d ; my = %d",j,i,k%32 , k / 32 ) ;
						}

						if( !saveStream.Write( sign) )
						{
							LOG_SEVERE( "写出错，x = %d, y = %d ,index = %d" , j , i ,k);
						}
					}
				}
			}
		}

		saveStream.WriteEnd ();

		return true;
	}	

	bool EditorTerrainTile::BuildPhysicsScene2 ()
	{
		g_Terrain.getPhysicsScene2()->reset();

		for( unsigned int i = 0 ; i < m_TitleData.modelData .modelSet.size () ; i++)
		{
			m_TitleData.modelData.modelSet [i]->AddToPhysicsScene2();
		}

		g_Terrain.getPhysicsScene2()->calAABBTree();

		return true;
	}

	bool EditorTerrainTile::GetNormal(float x, float z ,Ogre::Vector3* normal)
	{
		for(int i = 0 ; i < m_TitleData.gh ;i++)
		{
			for(int j = 0 ; j < m_TitleData.gw ; j++)
			{
				if(m_TitleData.chunkSet [i][j].GetNormal (x,z,normal))
				{
					return true;	
				}
			}
		}
		return false;
	};

	bool  EditorTerrainTile::GetHeightEx(float x, float z ,float* height )
	{
		for(int i=0; i<m_TitleData.gh;i++)
		{
			for(int j=0; j<m_TitleData.gw; j++)
			{
				if(m_TitleData.chunkSet [i][j].GetHeightEx (x,z,height))
				{
					return true;	
				}
			}
		}
		return false;
	}

	bool EditorTerrainTile::GetHeight(float x,float z,float* height, bool use_vb)
	{
		x -= m_TileHead.tileX*TERRTILE_SIZE;
		z -= m_TileHead.tileY*TERRTILE_SIZE;
		if(x<0 || x>=TERRTILE_SIZE || z<0 || z>=TERRTILE_SIZE)
		{
			return false;
		}

		int i = int(z/TERRBLOCK_SIZE);
		int j = int(x/TERRBLOCK_SIZE);
		if(i<0 || i>=m_TitleData.gh || j<0 || j>=m_TitleData.gw)
		{
			return false;
		}

		return m_TitleData.chunkSet[i][j].GetHeight(x, z, height, use_vb);
	}

	 bool EditorTerrainTile::GetWaterHeight (float x, float z, float* height)
	 {
		 x -= m_TileHead.tileX*TERRTILE_SIZE;
		 z -= m_TileHead.tileY*TERRTILE_SIZE;
		 if(x<0 || x>=TERRTILE_SIZE || z<0 || z>=TERRTILE_SIZE)
		 {
			 return false;
		 }

		 int i = int(z/TERRBLOCK_SIZE);
		 int j = int(x/TERRBLOCK_SIZE);
		 if(i<0 || i>=m_TitleData.gh || j<0 || j>=m_TitleData.gw)
		 {
			 return false;
		 }

		 return m_TitleData.chunkSet[i][j].GetWaterHeight(x, z, height);
	 }

	bool EditorTerrainTile::CreateChunkRenderData(int nWidth ,int nHeight)
	{
		for(int i = 0 ; i < nHeight; i++)
		{
			for(int j = 0 ; j < nWidth ; j++)
			{
				m_TitleData.chunkSet [i][j].CreateRenderData(this);
			}
		}
		return true;
	}

	bool EditorTerrainTile::LoadFile(const char* filename)
	{
		if(loadMapFile(filename))
		{	
			CreateChunkRenderData(m_TitleData.gw, m_TitleData.gh); 
			CreateModel();

			//*AddRenderDataToRoom();
			return true;
		}
		return false;

	}

	void EditorTerrainTile::update(unsigned int dtick)
	{
		for(int i = 0 ; i < m_TitleData.gh ; i++)	
		{
			for(int j = 0 ; j < m_TitleData.gw ; j++)
			{
				m_TitleData.chunkSet[i][j].update(dtick);
			}
		}
	}

	void EditorTerrainTile::Release()
	{
		for(int i = 0 ; i < m_TitleData.gh ; i++)	
		{
			for(int j = 0 ; j < m_TitleData.gw ; j++)
			{
				m_TitleData.chunkSet[i][j].Release () ;

				OGRE_RELEASE( m_pLiquid[i][j] );
				OGRE_RELEASE( m_pBlock[i][j] );

				PLANT_NODE_SET::iterator iter = m_pPlantNodeSet[i][j].begin();

				for( ; iter != m_pPlantNodeSet[i][j].end() ; iter ++ )
				{
					OGRE_RELEASE( iter->second );
				}

				m_pPlantNodeSet[i][j].clear();

			}
		}

		m_TitleData.texData.texFileSet.clear();
		m_TitleData.modelData.modelFileSet.clear();

		for(unsigned int i = 0 ; i < m_TitleData.modelData.modelSet.size() ; i ++)
		{
			delete(m_TitleData.modelData.modelSet[i]);
		}
		for(unsigned int i = 0 ; i < m_TitleData.modelData.modelPlantSet .size() ; i ++)
		{
			delete(m_TitleData.modelData.modelPlantSet[i]);
		}

		m_TitleData.modelData.modelSet.clear();
		m_TitleData.modelData.modelPlantSet.clear ();
	};


	void   EditorTerrainTile::SetChunkShowLayer(int layer)
	{
		for(int i = 0 ; i < m_TitleData.gh ; i++)
		{
			for(int j = 0 ; j < m_TitleData.gw ; j++)
			{
				m_TitleData.chunkSet [j][i].GetChunkOptionData()->showLayer = layer;
			}
		}
	};

	bool   EditorTerrainTile::CreateModel()
	{
		for( unsigned int i = 0 ; i < m_TitleData.modelData .modelSet.size () ; i++)
		{
			m_TitleData.modelData.modelSet[i]->GetModelData()->bShow = true;
			m_TitleData.modelData .modelSet[i]->Init ();
			m_TitleData.modelData.modelSet [i]->ResetMat ();	
			m_TitleData.modelData.modelSet [i]->AttachRoom();
		}

		return true;
	};


	bool    EditorTerrainTile::LoadPlantFile(const char* name)
	{

		std::string plantName = name;

		std::string nameStr;

		for( int i = 0 ; i < m_TitleData.gh ; i ++ )
		{
			for( int j = 0 ;j < m_TitleData.gw ; j ++ )
			{
				char info[64];
				sprintf( info , "_%d_%d.rawplant",i,j );
				nameStr = plantName + info ;
				m_TitleData.chunkSet[j][i].LoadPlantFile( nameStr.c_str () );
			}
		}

		return true;

	};

	bool    EditorTerrainTile::SavePlantFile(const char* name)
	{
		std::string plantName = name;

		std::string nameStr;

		for( int i = 0 ; i < m_TitleData.gh ; i ++ )
		{
			for( int j = 0 ;j < m_TitleData.gw ; j ++ )
			{
				char info[64];
				sprintf( info , "_%d_%d.rawplant",i,j );
				nameStr = plantName + info ;
				m_TitleData.chunkSet[j][i].SavePlantFile( nameStr.c_str () );
			}
		}

		return true;
	};

	bool   EditorTerrainTile::GetGridX(float posx,int *gx)
	{    
		if(posx < 	m_TitleData.box.minpos.x  || posx > m_TitleData.box.maxpos.x)
		{
			return false;
		}

		*gx =int( (posx - m_TitleData.box.minpos.x )  / TERRBLOCK_SIZE +0.002);

		return true;
	}
	//创建渲染数据 ; 返回 ( 为真表示成功 )
	bool     EditorTerrainTile::CreateData()
	{
		CreateChunkRenderData(m_TitleData.gw, m_TitleData.gh);

		return true;
	}
	//得到格子位置；参数（ Z坐标， 格子信息 ）；返回（ 为真表示成功 ）
	bool   EditorTerrainTile::GetGridZ(float posz,int *gz)
	{
		if(posz < 	m_TitleData.box.minpos.z || posz > 	m_TitleData.box.maxpos.z )
		{
			return false;
		}

		*gz =int( (posz - m_TitleData.box.minpos.z  )  / TERRBLOCK_SIZE +0.002);

		return true;
	}
}
