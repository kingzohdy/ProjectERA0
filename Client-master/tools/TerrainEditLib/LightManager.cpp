
#include ".\lightmanager.h"

#include "EditorModel.h"
#include "mterrainedit.h"
#include "MTerrain.h"
#include "marchive.h"
#include "Coordinate.h"
#include "Area.h"

#include "OgreLight.h"
#include "OgreGameScene.h"

using namespace TE;
using namespace Ogre;

extern std::string                g_path  ;

CLightManager                     g_LightManager;

CLightManager::CLightManager(void)
{
	m_nCurSelectIdx = 0 ;
	
	m_bNoAddLight   = false ;
	m_bLockHeight   = false ;
}

CLightManager::~CLightManager(void)
{
	Release();
}			

void CLightManager::update(float dtime)
{
	unsigned int dtick = TimeToTick(dtime);
	for(size_t i=0; i<m_LightSet.size(); i++)
	{
		m_LightSet[i].pLightNode->update(dtick);
		m_LightSet[i].pModel->Update(dtime);
	}
}

void   CLightManager::Release()
{
	DetachLightToRoom();

	for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
	{
		OGRE_RELEASE( m_LightSet[i].pLightNode );
		delete( m_LightSet[i].pModel );
	}

	m_LightSet.clear();
};

bool   CLightManager::LoadLightInfo( const char* filename ,MTerrainEdit* pEditor )
{
	MArchive*  Ar  = g_FileManager.CreateFileReader( filename , 0 );
   
	if( Ar == NULL )
	{
		return false;
	}
	
	m_bNoAddLight = false;

	pEditor->m_pCurSelectModel  = NULL ;	
	pEditor->m_EditOption.pSelectModel = NULL ;
	pEditor->GetCoordinate()->bindObject( NULL );
	//释放当前灯光资源
	Release();

	Serialize( *Ar );

	MSAFE_DELETE( Ar );
	

	for( size_t i = 0 ; i < m_LightInfoSet.size() ; i ++ )
	{
		AddLight( m_LightInfoSet[i] , CLightManager::FILE_ADD_LIGHT );
	}

	return true;
};

bool   CLightManager::SaveLightInfo( const char* filename )
{
    MArchive*  Ar  = g_FileManager.CreateFileWriter( filename , 0 );
   
	if( Ar == NULL )
	{
		return false;
	}

	Serialize( *Ar );

	MSAFE_DELETE( Ar );

	return true;
};

//序列化成 参数( Archive );
void   CLightManager::Serialize( MArchive& Ar )
{
	LightInfoHead  head ;

	if( Ar.IsLoading() )
	{
		Ar << head ;
		m_LightInfoSet.resize( head.nLightNum );
	}
	else
	{
		head.nVersion = 0 ;
		head.nLightNum = m_LightInfoSet.size() ;

		Ar << head;
	}

	for( size_t i = 0 ; i < m_LightInfoSet.size() ; i ++ )
	{
		Ar << m_LightInfoSet[i];
	}
};

//更新对应索引的灯光信息 ; 参数 ( 索引ID )
void   CLightManager::UpdateLight( int index )
{
	if( index >= (int)m_LightInfoSet.size() )
	{
		return;
	}

	m_nCurSelectIdx = index;

	m_LightInfoSet[index].pos = m_LightSet[index].pModel->GetModelData()->pos ;
	m_LightInfoSet[index].pos .y += LIGHT_UP_MODEL_HEIGHT ;

	m_LightSet[index].pLightNode ->setPosition( m_LightInfoSet[index].pos );


	//计算顶点颜色
	CountChunkLight( m_LightInfoSet[index].pos , m_LightInfoSet[index].color , m_LightInfoSet[index].fRange );
};

void CLightManager::flushAllLights(bool bake_dirlight)
{
	int nblockx = g_Terrain.GetTitle(0)->GetHeadData()->gridX;
	int nblockz = g_Terrain.GetTitle(0)->GetHeadData()->gridY;

	Ogre::AmbientInfo &ambient = *g_AreaSet.GetArea(0)->GetAmbientInfo();
	Vector3 dir;
	dir.x = Cos(ambient.dirlight_angle.y)*Cos(ambient.dirlight_angle.x);
	dir.y = Sin(ambient.dirlight_angle.y);
	dir.z = Cos(ambient.dirlight_angle.y)*Sin(ambient.dirlight_angle.x);

	//更新计算中间3X3格子的光照
	for(int i=0; i<nblockx; i++)
	{
		for(int j=0; j<nblockz; j++)
		{
			MChunk &chunk = g_TerrainEdit.GetEditData()->chunkSet[j][i];
			//清除光照颜色
			chunk.SetLightColor( 0 );

			for( size_t k = 0 ; k < m_LightInfoSet.size() ; k ++ )
			{
				chunk.CountLightColor(0, m_LightInfoSet[k].pos, *(ColourValue *)&m_LightInfoSet[k].color, ColourValue(0,0,0,0), m_LightInfoSet[k].fRange);
			}

			if(bake_dirlight)
			{
				float shadowdensity = ambient.shadowdensity;
				if(!ambient.shaow) shadowdensity = 0;
				chunk.CountLightColor(1, dir, ambient.dirlight_color, ambient.ambient_color, 0, shadowdensity);
			}

			//更新植物光照颜色。
			chunk.UpdatePlantLightColor();
		}
	}
}
	
//计算CHUNK的光照，记录在顶点颜色里面 ；参数（ 位置，颜色，范围 ）
void   CLightManager::CountChunkLight( MVec3 pos , MVec4 color , float fRange )
{
	int gx,gz ;

	if( !g_Terrain.GetGridX( pos.x , &gx ) )
		return;

	if( !g_Terrain.GetGridZ( pos .z , &gz ) )
		return;

	int nblockx = g_Terrain.GetTitle(0)->GetHeadData()->gridX;
	int nblockz = g_Terrain.GetTitle(0)->GetHeadData()->gridY;
	//更新计算中间3X3格子的光照
	for( int i = gx - 1 ; i <= gx + 1 ; i ++ )
	{
		for( int j = gz - 1 ; j <= gz + 1 ; j ++ )
		{
			if(i<0||j<0||i>=nblockx||j>=nblockz)
			{
				continue;
			}

			MChunk &chunk = g_TerrainEdit.GetEditData()->chunkSet[j][i];

            //清除光照颜色
			chunk.SetLightColor( 0 );

			for( size_t k = 0 ; k < m_LightInfoSet.size() ; k ++ )
			{
				chunk.CountLightColor(0, m_LightInfoSet[k].pos, *(ColourValue *)&m_LightInfoSet[k].color, ColourValue(0,0,0,0), m_LightInfoSet[k].fRange);
			}
            //更新植物光照颜色。
			chunk.UpdatePlantLightColor();

		}
	}
};

//增加灯光 ; 参数 ( 灯光信息, 灯光模式 ) ; 返回 ( 灯光节点信息 )
LightNodeInfo   CLightManager::AddLight( LightInfo  &lightInfo , ADD_LIGHT_MODE nMode )
{
	LightNodeInfo lightNodeInfo;

	lightNodeInfo.pModel = new TerrModel ;
	
	if( nMode == CLightManager::ADD_LIGHT )
	{
		lightNodeInfo.pModel->GetModelData()->pos = lightInfo.pos ;
	}
	else
	{
		//如果是从文件读出信息，则先对调整位置
		MVec3 pos = lightInfo.pos ;
		pos.y -= LIGHT_UP_MODEL_HEIGHT ;
		lightNodeInfo.pModel->GetModelData()->pos = pos ;
	}

	std::string  fileName = LIGHT_MODEL_PATH ;
	strcpy( lightNodeInfo.pModel->GetModelData()->name ,  fileName.c_str() );

	lightNodeInfo.pModel->Init() ;
	lightNodeInfo.pModel->AttachRoom();

	EditorModel*  pMayModel = (EditorModel*)lightNodeInfo.pModel->GetModelData()->pModel;	
	assert( pMayModel );
	pMayModel->SetPos( lightNodeInfo.pModel->GetModelData()->pos );
	
	lightNodeInfo.pLightNode = new Light( LT_POINT );
	lightNodeInfo.pLightNode->setColor(ColourValue( lightInfo.color .x ,lightInfo.color .y , lightInfo.color .z , lightInfo.color .w));
	lightNodeInfo.pLightNode->m_Attenuation = 1.0f;
	lightNodeInfo.pLightNode->m_Range = lightInfo.fRange ;
	lightNodeInfo.pLightNode ->setStatic( true );

	//如果是编辑增加的灯光，则需要调整光的位置
	if( nMode == CLightManager::ADD_LIGHT )
	{
		lightInfo.pos .y += LIGHT_UP_MODEL_HEIGHT ;
	}

	lightNodeInfo.pLightNode->setPosition( lightInfo.pos );

	lightNodeInfo.pLightNode->attachToScene(&g_Terrain);

	m_LightSet.push_back( lightNodeInfo );

	//如果是编辑增加，需要把信息加入数组
	if( nMode == CLightManager::ADD_LIGHT )
	{
		m_nCurSelectIdx = m_LightInfoSet.size() ;
		m_LightInfoSet.push_back( lightInfo );	

		g_TerrainEdit.m_pCurSelectModel = lightNodeInfo.pModel ;
		g_TerrainEdit.m_EditOption.bUpdateLightData = false;

		g_TerrainEdit.m_bUpdateLightList = true;	
		
	}
		
	//计算顶点颜色	
	CountChunkLight( lightInfo.pos , lightInfo.color , lightInfo.fRange );

	return lightNodeInfo;
};
		
//删除当前选择灯光	
void   CLightManager::DelCurSelectLight( MTerrainEdit *pEditor )
{
	if( m_LightInfoSet.empty() )
	{
		return;
	}

	if(m_nCurSelectIdx < 0) return;

	m_LightSet[m_nCurSelectIdx].pLightNode->detachFromScene();

	OGRE_RELEASE( m_LightSet[m_nCurSelectIdx].pLightNode );
	delete(  m_LightSet[m_nCurSelectIdx].pModel );
					
	pEditor->m_pCurSelectModel  = NULL ;	
	pEditor->m_EditOption.pSelectModel = NULL ;
	pEditor->GetCoordinate()->bindObject( NULL );
	pEditor->m_bUpdateLightList = true;
	
	MVec3  pos = m_LightInfoSet[m_nCurSelectIdx].pos ;

	m_LightSet.erase( m_LightSet.begin() + ( m_nCurSelectIdx ) );
	m_LightInfoSet.erase( m_LightInfoSet.begin() + m_nCurSelectIdx );
	if(m_nCurSelectIdx >= m_LightInfoSet.size())
	{
		m_nCurSelectIdx = m_LightInfoSet.size()-1;
	}

	//计算顶点颜色
	CountChunkLight( pos , MVec4(1.0f,1.0f,1.0f,1.0f)  , 0.01f );
};

//得到当前选择灯光信息 ; 参数 ( 灯光信息 ) 返回 ( 为真表示成功 )
bool   CLightManager::GetCurSelectLightInfo( LightInfo&  lightInfo )
{
	if( m_LightInfoSet.empty())
	{
		return false;
	}
	
	memcpy( &lightInfo , &m_LightInfoSet[m_nCurSelectIdx] ,sizeof( LightInfo ) );

	return true ;
};
//拷贝当前选中灯光
void   CLightManager::CopyCurLight()
{
	if( m_LightSet.empty() )
	{
		return;
	}
	
	m_nCopyLightNum = 0 ;
   
	m_SaveLightInfo.clear ();
	m_SaveLightInfo.push_back( m_LightInfoSet[m_nCurSelectIdx] );
	
};
//copy选中的灯光给tile
void   CLightManager::CopySelectLightToTile()
{
	if( m_SaveLightInfo.empty() )
	{
		return;
	}

	m_nCopyLightNum ++ ;

	LightInfo  lightInfo;
	lightInfo.color = m_SaveLightInfo[0].color ;
	lightInfo.fRange = m_SaveLightInfo[0].fRange ;
	lightInfo.nType = m_SaveLightInfo[0].nType ;
	lightInfo.pos   = m_SaveLightInfo[0].pos ;
	lightInfo.pos .y -= LIGHT_UP_MODEL_HEIGHT ;
	lightInfo.pos .x += m_nCopyLightNum*50 ;
	lightInfo.pos .z += m_nCopyLightNum*50 ;



	AddLight( lightInfo , ADD_LIGHT );
}
//增加显示对应的灯光模型 
void   CLightManager::AttachLightModelToRoom()
{
	for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
	{
		m_LightSet[i].pModel->AttachRoom();
	}
};
	
//移除对应的灯光模型
void   CLightManager::DetachLightModelToRoom()
{
	for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
	{
		m_LightSet[i].pModel->DetachRoom();
	}
};
	
bool   CLightManager::SetEnableShadow( int bEnable)
{
 	if( m_LightInfoSet.empty())
	{
		return false;
	}
    
	if( bEnable )
	{
		m_LightSet[m_nCurSelectIdx].pLightNode ->setStatic( false );
		m_LightSet[m_nCurSelectIdx].pLightNode ->enableShadow();
	}
	else
	{
		m_LightSet[m_nCurSelectIdx].pLightNode ->setStatic( true );
		m_LightSet[m_nCurSelectIdx].pLightNode ->disableShadow();
	}

	return true;
};

//设置当前灯光的位置 ; 参数( 位置 ) ; 返回 ( 为真表示成功 )
bool   CLightManager::SetCurSelectLightPos( MVec3 pos )
{
	if( m_LightInfoSet.empty())
	{
		return false;
	}

	m_LightInfoSet[m_nCurSelectIdx].pos = pos ;
	m_LightSet[m_nCurSelectIdx].pLightNode ->setPosition ( pos );
	pos.y -= LIGHT_UP_MODEL_HEIGHT ;

	m_LightSet[m_nCurSelectIdx].pModel->GetModelData()->pos = pos ;
	m_LightSet[m_nCurSelectIdx].pModel->ResetMat();

	g_Terrain.onObjectPosChange(m_LightSet[m_nCurSelectIdx].pLightNode);

    //计算对应的chunk颜色
	CountChunkLight( m_LightInfoSet[m_nCurSelectIdx].pos , m_LightInfoSet[m_nCurSelectIdx].color  , 
		m_LightInfoSet[m_nCurSelectIdx].fRange );

	return true;
};
	
//设置当前灯光的范围 ; 参数: ( 范围 ) 返回 ( 为真表示成功 )
bool   CLightManager::SetCurSelectLightRange( float fRange)
{
	if( m_LightInfoSet.empty())
	{
		return false;
	}

	m_LightInfoSet[m_nCurSelectIdx].fRange = fRange ;
	m_LightSet[m_nCurSelectIdx].pLightNode ->m_Range = fRange ;

    //计算对应的chunk颜色
	CountChunkLight( m_LightInfoSet[m_nCurSelectIdx].pos , m_LightInfoSet[m_nCurSelectIdx].color  , 
		m_LightInfoSet[m_nCurSelectIdx].fRange );

	return true;

};
	
//设置当前灯光的颜色 ; 参数 ( 颜色 ) 返回 ( 为真表示成功 )
bool   CLightManager::SetCurSelectLightColor( MVec4 color )
{
	if( m_LightInfoSet.empty())
	{
		return false;
	}
	
	m_LightInfoSet[m_nCurSelectIdx].color = color ;
	m_LightSet[m_nCurSelectIdx].pLightNode ->setColor(ColourValue(color.x ,color.y,color.z,color.w));


    //计算对应的chunk颜色
	CountChunkLight( m_LightInfoSet[m_nCurSelectIdx].pos , m_LightInfoSet[m_nCurSelectIdx].color  , 
		m_LightInfoSet[m_nCurSelectIdx].fRange );

	return true;
};
	
//对场景增加灯光
void   CLightManager::AttachLightToRoom()
{
	for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
	{
		m_LightSet[i].pLightNode->attachToScene(&g_Terrain);
		m_LightSet[i].pModel->AttachRoom();
	}

};

//移除灯光
void   CLightManager::DetachLightToRoom()
{
	for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
	{
		m_LightSet[i].pLightNode->detachFromScene();

		m_LightSet[i].pModel->DetachRoom();
	}
};
