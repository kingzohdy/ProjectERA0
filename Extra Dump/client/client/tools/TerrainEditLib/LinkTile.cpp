
#include "LinkTile.h"
#include "TerrainEdit.h"
#include "EditorModel.h"
#include "Terrain.h"
#include "EditorStream.h"
#include "TextureManager.h"
#include "OgreResourceManager.h"
#include "OgreModel.h"

#include "EditEvent_HeightMap.h"
#include "EditEventHandlerMgr.h"

using namespace Ogre;
namespace TE
{
	bool   g_bLoading = false;

	extern GameTerrainScene *g_pRoom ;

	//自动连接对应模型的后缀名
	const char g_AutoLinkFileName[11][8] =
	{
		"_L_U","_R_U","_C_U",
		"_C_C","_L_C","_R_C",
		"_L_D","_C_D","_R_D",
		"_LD_RU","_RD_LU"
	};
	//对应模型的文件目录
	const char g_LinkMeshPath[] = "scene\\model\\linkmesh\\";
	const char g_LinkTexturePath[] = "scene\\model\\linktexture\\";
	const char g_WallPaperPath[] = "scene\\model\\basemodel\\";
	const char g_LinkWaterMeshPath[] = "scene\\model\\linkwater\\";

	LinkMeshManager::LinkMeshManager(void)
	{
		m_LinkMap = NULL;
		m_NewMap  = NULL;
		m_UsedModelMap = NULL;
		m_ModelMapInfo = NULL;
		m_nMapSize = 0;


		m_SignSet[SIGN_UP] = MAP_U_SIGN;
		m_SignSet[SIGN_LEFT] = MAP_L_SIGN;
		m_SignSet[SIGN_CENTER] = MAP_C_SIGN;
		m_SignSet[SIGN_RIGHT] = MAP_R_SIGN;
		m_SignSet[SIGN_DOWN] = MAP_D_SIGN;

		m_SignMapSet[SIGN_TYPE_L_U] = MAP_CENTER_L_U ;
		m_SignMapSet[SIGN_TYPE_R_U] = MAP_CENTER_R_U ;
		m_SignMapSet[SIGN_TYPE_C_U] = MAP_CENTER_C_U ;
		m_SignMapSet[SIGN_TYPE_C_C] = MAP_CENTER_C_C ;
		m_SignMapSet[SIGN_TYPE_L_C] = MAP_CENTER_L_C ;
		m_SignMapSet[SIGN_TYPE_R_C] = MAP_CENTER_R_C ;
		m_SignMapSet[SIGN_TYPE_L_D] = MAP_CENTER_L_D ;
		m_SignMapSet[SIGN_TYPE_C_D] = MAP_CENTER_C_D ;
		m_SignMapSet[SIGN_TYPE_R_D] = MAP_CENTER_R_D ;
		m_SignMapSet[SIGN_TYPE_LD_RU] = MAP_RU_LD ;
		m_SignMapSet[SIGN_TYPE_RD_LU] = MAP_RD_LU ;


		LinkMeshCullInfo info;
		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 1;
		int signID = MAP_CENTER_L_U ;
		m_CullInfoMap[signID] = info;
		info.info[0] = 1 ;info.info[1] = 1 ; info.info[2] = 1 ; info.info[3] = 0;
		m_CullInfoMap[44] = info;	

		signID = MAP_CENTER_R_U ;
		info.info[0] = 0 ;info.info[1] = 0; info.info[2] = 1 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;
		info.info[0] = 1 ;info.info[1] = 1 ; info.info[2] = 0 ; info.info[3] = 1;
		m_CullInfoMap[33] = info;	

		signID = MAP_CENTER_C_U ;
		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 1 ; info.info[3] = 1;
		m_CullInfoMap[signID] = info;

		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[55] = info;

		signID = MAP_CENTER_L_C;
		info.info[0] = 0 ;info.info[1] = 1 ; info.info[2] = 0 ; info.info[3] = 1;
		m_CullInfoMap[signID] = info;

		signID = MAP_CENTER_R_C;
		info.info[0] = 1 ;info.info[1] = 0 ; info.info[2] = 1 ; info.info[3] = 0;	
		m_CullInfoMap[signID] = info;

		signID = MAP_CENTER_L_D ;
		info.info[0] = 0 ;info.info[1] = 1 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;
		info.info[0] = 1 ;info.info[1] = 0 ; info.info[2] = 1 ; info.info[3] = 1;
		m_CullInfoMap[11] = info;	

		signID = MAP_CENTER_C_D ;
		info.info[0] = 1 ;info.info[1] = 1 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;

		signID = MAP_CENTER_R_D ;
		info.info[0] = 1 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;
		info.info[0] = 0 ;info.info[1] = 1 ; info.info[2] = 1 ; info.info[3] = 1;
		m_CullInfoMap[123] = info;	

		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 0;

		m_CullInfoMap[MAP_RU_LD] = info;	
		m_CullInfoMap[MAP_RD_LU] = info;

		m_fMoveHeight = 0 ;
		m_strLinkPath = g_LinkMeshPath;

		m_nAutoDelBlock = 0;

		m_SignCullInfo = NULL ;
		m_bLoadedInfo  = NULL ;

		m_nCurSelectX = m_nCurSelectY = -1;

		m_bLoading = false ;
		g_bLoading = false;

		//按顺序的序列   		
		m_bOderLinkMesh = false;

	}
	//释放
	void    LinkMeshManager::Release()
	{

		m_CullInfoMap.clear();

		Clear();
		Destory();
	};

	LinkMeshManager::~LinkMeshManager(void)
	{	
		Release();
	}
	//清除
	void   LinkMeshManager::Clear()
	{
		m_MeshName.clear();
		m_SaveCurEditMeshName.clear();

		m_fMoveHeight = 0;
	}
	//摧毁
	void LinkMeshManager::Destory()
	{                     
		m_bLoading = false;
		g_bLoading = false;


		OGRE_DELETE_ARRAY( m_LinkMap );
		OGRE_DELETE_ARRAY( m_NewMap );
		OGRE_DELETE_ARRAY( m_UsedModelMap );
		OGRE_DELETE_ARRAY( m_ModelMapInfo );
		OGRE_DELETE_ARRAY( m_SignCullInfo );
		OGRE_DELETE_ARRAY ( m_bLoadedInfo );

		m_fMoveHeight = 0;
	}
	//得到LINK模型的类型ID ; 参数 ( 文件名 ) ; 返回 ( 类型ID )
	int   LinkMeshManager::GetLinkModelKindID( std::string &name )
	{
		int idx = name.rfind(".");

		std::string newName = name.substr( 0 , idx );

		MODEL_KIND_MAP::iterator  iter = m_ModelKindMap.find( newName );

		if( iter != m_ModelKindMap.end() )
		{
			return iter->second ; 
		}

		int id = m_ModelKindMap.size() + 1;

		m_ModelKindMap[newName] = id ;

		return id ;
	};	
	//后退操作	
	void  LinkMeshManager::UnDo()
	{
		if( m_SaveMapData.empty() )
		{
			return;
		}

		assert( m_LinkMap );

		CurrLinkMapSaveData  saveData;
		saveData = m_SaveMapData[m_SaveMapData.size()-1]; 

		for( int i = 0 ; i < 4 ; i ++ )
		{
			m_LinkMap[saveData.saveData[i].index] = saveData.saveData[i].sign;
		}

		int mx , my ;

		assert( m_nMapSize != 0 );

		mx = saveData.saveData[0].index % m_nMapSize;
		my = saveData.saveData[0].index / m_nMapSize;

		UpdateLinkMap( mx-1, my-1 ,3,3 );

		m_SaveMapData.pop_back();
	};
	//更新CHUNK的地形高度 ; 参数( X , Y ) 
	void  LinkMeshManager::UpdateChunkHeight( int x, int y  )
	{
		int gx ,gy ;
		int mx , my ;

		assert( m_ModelMapInfo );
		assert( m_nMapSize != 0 );

		int nlinkgrid = m_nMapSize/m_nBlock.x; //一个block里面有多少个linkmesh的格子
		gx = x/nlinkgrid;
		gy = y/nlinkgrid;

		mx = m_UnitSize*( x % nlinkgrid);
		my = m_UnitSize*( y % nlinkgrid);

		float height ;

		if( m_ModelMapInfo[y*m_nMapSize+x].pModel == NULL )
			return ;

		EditorModel *pModel = (EditorModel*)m_ModelMapInfo[y*m_nMapSize+x].pModel->GetModelData()->pModel ; 

		if( pModel == NULL )
			return;

		height = pModel->GetLinkMeshHeight( 0 );

		g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx, my , height );
		//暂时支持2X2,1X1
		if( m_UnitSize == 2 )
		{
			g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx+1, my , height );
			g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx, my+1 , height );
			g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx+1, my+1 , height );
		}
		m_ModelMapInfo[y*m_nMapSize+x].pModel->DetachRoom();
	};
	//设置选择LINK模型的文件名; 参数 ( 文件名 ) ; 返回 ( 为真表示成功 )
	bool  LinkMeshManager::SetSelectLinkMeshName( std::string &filename)
	{
		int index2 = filename.find("_");

		if( index2 == -1 )
		{
			return false;
		}

		std::string  meshName = filename.substr( 0 , index2  );

		if( meshName == m_SaveCurEditMeshName )
		{
			return true;
		}

		m_SaveCurEditMeshName = meshName;

		//获得当前该连接模型的向上，或者向下高度    
		TerrModel*  model = new TerrModel;
		model->GetModelData ()->cType = TYPE_LINK_MODEL;

		std::string strname = m_SaveCurEditMeshName;
		strname += "_C_C";

		sprintf(model->GetModelData()->name, "%s.omod", strname.c_str());
		model->GetModelData ()->modelID = 0;
		model->Init ();

		EditorModel* pModel = (EditorModel*)model->GetModelData()->pModel ;

		if( pModel )
		{
			m_fMoveHeight = pModel->GetLinkMeshHeight( 0 );
			delete( model );
		}	

		return true;

	};
	//设置开始引导ID的位置;参数( 位置 )	
	void  LinkMeshManager::SetBeginLoadIDPos( Ogre::Vector3  pos )
	{
		int mx,my;
		mx = (int )( pos.x / m_fWidthSize ) ;
		my = (int)( pos.z / m_fWidthSize ) ;

		int nBeginLoadID = mx + my * m_nMapSize ;

		m_iLoadingForwardID = m_iLoadingBackID = nBeginLoadID ;

	};
	//初始化LINK模型;参数( 地图大小 )
	void  LinkMeshManager::Init(int unitsize, Ogre::Point2D nblock, Ogre::Point2D ngrid)
	{
		Destory();

		m_nBlock = nblock;
		m_nGrid = ngrid;
		m_UnitSize = unitsize;

		m_nMapSize = m_nBlock.x*m_nGrid.x/unitsize;

		m_LinkMap = new unsigned char [m_nMapSize*m_nMapSize] ;
		m_NewMap  = new unsigned char [m_nMapSize*m_nMapSize] ;
		m_UsedModelMap = new unsigned char [m_nMapSize*m_nMapSize] ;
		m_SignCullInfo = new unsigned char [m_nMapSize*m_nMapSize];

		m_ModelMapInfo = new LinkMeshInfo [m_nMapSize*m_nMapSize];

		memset( m_ModelMapInfo , 0 , m_nMapSize*m_nMapSize*sizeof(LinkMeshInfo) );
		memset( m_LinkMap , 0 , m_nMapSize*m_nMapSize );
		memset( m_NewMap , 0 , m_nMapSize*m_nMapSize );
		memset( m_UsedModelMap ,0xff , m_nMapSize*m_nMapSize );
		memset( m_SignCullInfo , 0 , m_nMapSize*m_nMapSize  );

		m_fWidthSize = TERRBLOCK_SIZE*m_UnitSize/m_nGrid.x;
		m_fHeightSize = TERRBLOCK_SIZE*m_UnitSize/m_nGrid.y;
	};
	//设置地图的标记;参数( 位置 , 标记 ) 		
	void  LinkMeshManager::SetMapSign(Ogre::Vector3 pos , unsigned char sign )
	{
		int x ,y ;

		x = (int) ( pos.x / m_fWidthSize );
		y = (int) ( pos.z / m_fHeightSize );

		m_nCurSelectX = x ;
		m_nCurSelectY = y ;
		//记录修改前的位置，以便进行undo处理	
		SaveUpdateLinkMapData( x , y , 2 , 2 );

		SetMapSign(x , y , sign);
		SetMapSign(x+1, y, sign);
		SetMapSign(x, y+1, sign);
		SetMapSign(x+1, y+1, sign);

		UpdateLinkMap(x-1, y-1, 3, 3);

	};
	//设置地图标记; 参数( 格子 X , Y , 对应标记 )
	void  LinkMeshManager::SetMapSign( int x,int y, unsigned char sign )
	{
		if( x < 0 || x >= m_nMapSize )
			return;

		if( y < 0 || y >= m_nMapSize )
			return;

		m_LinkMap[y*m_nMapSize + x] = sign;
	};
	//测试是否是LINK模型; 参数 ( 文件名 ) ; 返回 ( 为真表示成功 )			
	bool  LinkMeshManager::TestLinkMesh( std::string &filename )
	{
		if( filename.find( m_strLinkPath.c_str() ) != -1 )
		{
			SetSelectLinkMeshName( filename );
			return true;
		}

		return false;
	};
	//得到地图标记的文件名;参数( X , Y , 是否测试 , 是否更新 ) ;返回( 对应的文件名 ,为NULL表示空 )
	const char* LinkMeshManager::GetMapSingFileName(int x,int y , bool btest , bool *bUpdate)
	{
		assert(x>=0 && x<m_nMapSize && y>=0 && y<m_nMapSize);

		if ( btest )
		{
			unsigned char sign_ex[4];

			sign_ex[0] = getLinkMapUnit(x+1, y+1);
			sign_ex[1] = getLinkMapUnit(x-1, y+1);
			sign_ex[2] = getLinkMapUnit(x-1, y-1);
			sign_ex[3] = getLinkMapUnit(x+1, y-1);

			if ( sign_ex[0] ==0 && sign_ex[1] > 0 && sign_ex[2] > 0 && sign_ex[3] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = 11 ;
				m_MeshName = "_L_D_I";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}

				return m_MeshName.c_str();
			}
			if ( sign_ex[1] ==0 && sign_ex[0] > 0 && sign_ex[2] > 0 && sign_ex[3] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = 123 ;
				m_MeshName = "_R_D_I";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}

				return m_MeshName.c_str();
			}
			if ( sign_ex[2] ==0 && sign_ex[1] > 0 && sign_ex[0] > 0 && sign_ex[3] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = 33 ;
				m_MeshName = "_R_U_I";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}

				return m_MeshName.c_str();
			}
			if ( sign_ex[3] ==0 && sign_ex[1] > 0 && sign_ex[2] > 0 && sign_ex[0] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = 44 ;
				m_MeshName = "_L_U_I";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}

				return m_MeshName.c_str();
			}		
			if ( sign_ex[3] > 0 && sign_ex[1] > 0 && sign_ex[2] > 0 && sign_ex[0] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = 55 ;
				m_MeshName = "_C_C";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}

				return m_MeshName.c_str();
			}
			if ( sign_ex[1] == 0 && sign_ex[3] == 0 && sign_ex[2] > 0 && sign_ex[0] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = MAP_RU_LD ;
				m_MeshName = "_LD_RU";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}
			}
			if ( sign_ex[0] == 0 && sign_ex[2] == 0 && sign_ex[1] > 0 && sign_ex[3] > 0 )
			{
				m_NewMap[x+y*m_nMapSize] = MAP_RD_LU ;
				m_MeshName = "_RD_LU";
				if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
				{
					*bUpdate = true;
				}
			}
		}

		SIGN_TYPE type = GetMapSignType( x ,y );

		if ( type == SIGN_TYPE_UNKOWN )
		{
			if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
			{
				*bUpdate = true;
			}

			return NULL ;
		}

		m_MeshName  =  g_AutoLinkFileName[(int)type];


		if( m_NewMap[x+y*m_nMapSize] != m_UsedModelMap[x+y*m_nMapSize] )
		{
			*bUpdate = true;
		}

		return m_MeshName.c_str() ;
	};
	//得到地图标记类型;参数(X,Y ) ;返回 ( 标记类型 )
	SIGN_TYPE   LinkMeshManager::GetMapSignType( int x, int y )
	{
		if( x < 0 || x >= m_nMapSize )
		{
			return SIGN_TYPE_UNKOWN;
		}

		if( y < 0 || y >= m_nMapSize )
		{
			return SIGN_TYPE_UNKOWN;
		}

		for ( int i = 0 ;i < SIGN_TYPE_NUM ; i ++ )
		{
			if ( m_NewMap[y*m_nMapSize + x] == m_SignMapSet[i] )
			{
				return (SIGN_TYPE)i;
			}
		}

		return  SIGN_TYPE_UNKOWN;
	};
	//删除LINK模型;参数( X , Y ) ;返回( 为真表示成功 )	
	bool  LinkMeshManager::DelLinkMesh(int x,int y )
	{		
		std::vector<TerrModel*>::iterator  iter;

		for( iter = g_TerrainEdit.GetEditData()->modelData .modelSet.begin() ; iter != g_TerrainEdit.GetEditData()->modelData .modelSet.end() ; iter++ )
		{        
			if( *iter == m_ModelMapInfo[x+y*m_nMapSize].pModel )	
			{   
				delete( m_ModelMapInfo[x+y*m_nMapSize].pModel );
				m_ModelMapInfo[x+y*m_nMapSize].pModel = NULL;

				g_TerrainEdit.GetEditData()->modelData .modelSet.erase( iter );

				m_UsedModelMap[x+y*m_nMapSize] = 0 ;

				return true;  
			}	
		}	
		return false;
	};

	unsigned char LinkMeshManager::getLinkMapUnit(int x, int y)
	{
		if(x<0 || y<0 || x>=m_nMapSize || y>=m_nMapSize) return 1;
		else return m_LinkMap[y*m_nMapSize+x];
	}

	//更新MESH;参数 ( X, Y ,文件名 , 是否更新 ); 返回( 为真表示成功 )
	bool  LinkMeshManager::UpdateMesh(int x,int y,const char* filename ,bool *bUpdate)
	{
		if ( !*bUpdate )
		{
			return false;
		}

		if ( filename == NULL )
		{
			if( m_ModelMapInfo[x+y*m_nMapSize].pModel )
			{ 		 
				DelLinkMesh( x ,y );	

				int gx,gy,mx,my;
				bool bUpdateChunk = false;

				int nlinkgrid = m_nMapSize/m_nBlock.x; //一个block里面有多少个linkmesh的格子

				gx = x / nlinkgrid;
				gy = y / nlinkgrid;

				mx = m_UnitSize*( x % nlinkgrid);
				my = m_UnitSize*( y % nlinkgrid);

				if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my+1 , 0 ) )
					bUpdateChunk = true;

				if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my+1 ,0 ) )		
					bUpdateChunk = true;

				if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my , 0 ) )	
					bUpdateChunk = true;

				if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my , 0 ) )							
					bUpdateChunk = true;

				g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx, my, 0.0f );
				g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx+1, my,  0.0f  );
				g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx+1, my+1,  0.0f  );
				g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx, my+1, 0.0f  );

				return true;
			}

			return false;
		}

		std::string strname(m_SaveCurEditMeshName.c_str());
		strname = strname + filename;

		//增加测试随即生成模型
		if ( strname.find("_C_C") == - 1 )
		{   
			//如果不是按顺序LINK的模型,则随机
			if( !m_bOderLinkMesh )
			{
				m_SignCullInfo[y*m_nMapSize + x] &= 0x0f; 
				int randmodeltype = rand()%3;

				if ( randmodeltype > 0 )
				{
					char typeInfo[12] ;
					sprintf( typeInfo , "_%d" , randmodeltype );

					std::string strname1 = strname + typeInfo;

					if (ResourceManager::getSingleton().hasResource(strname1.c_str()))
					{
						strname = strname1;
					}
				}
			}
			else
			{
				m_SignCullInfo[y*m_nMapSize + x] = ( m_SignCullInfo[y*m_nMapSize + x]&0x0f) | 0x80 ;

				//按顺序0,1,0,1	
				int curIdx = ( x + y ) % 2;

				if( curIdx > 0 )
				{
					char typeInfo[12] ;	
					sprintf( typeInfo , "_%d" , curIdx );

					std::string strname1 = strname + typeInfo;

					if (ResourceManager::getSingleton().hasResource(strname1.c_str()))	
					{	    
						strname = strname1;
					}
				}
			}
		}

		int   index = g_TerrainEdit.AddModelName( strname );

		//如果已经加载模型，则返回
		if( m_ModelMapInfo[x+y*m_nMapSize].modelID == index &&
			m_ModelMapInfo[x+y*m_nMapSize].pModel )
		{
			if( strname.find("_C_C") != -1 )
			{
				UpdateChunkHeight( x,y );
			}

			return true;
		}
		//删除当前模型
		if ( //m_ModelMapInfo[x+y*m_nMapSize].modelID != index &&
			m_ModelMapInfo[x+y*m_nMapSize].pModel != NULL )
		{
			DelLinkMesh( x ,y );
		}

		TerrModel*  model = new TerrModel;
		//该模型类型为link模型
		model->GetModelData ()->cType = TYPE_LINK_MODEL ;
		sprintf(model->GetModelData()->name, "%s.omod", strname.c_str());

		int modeKindID = GetLinkModelKindID( strname );

		model->GetModelData ()->modelID = ( modeKindID << 16 ) | index;

		model->Init ();

		Ogre::Vector3 pos;
		pos.x = m_fWidthSize*x ;
		pos.z = m_fHeightSize*y ;

		if(!g_Terrain.GetHeight( pos.x, pos.z, &pos.y, TERR_PICK_HEIGHTMAP))
			return false;

		model->GetModelData()->pos = pos ;
		model->ResetMat();

		assert(!strname.empty());
		model->UpdatePos();
		model->AttachRoom();

		g_TerrainEdit.GetEditData()->modelData .modelSet .push_back (model);

		m_ModelMapInfo[x+y*m_nMapSize].modelID = index;
		m_ModelMapInfo[x+y*m_nMapSize].pModel = model;
		m_ModelMapInfo[x+y*m_nMapSize].fMoveHeight = m_fMoveHeight;

		if( strname.find("_C_C") != -1 )
		{	
			UpdateChunkHeight( x,y );	
			UpdateCullMap( x , y  );
		}
		else
		{
			UpdateCullMap( x, y );
		}

		return true;
	}	
	//保存LINK的信息;参数( 文件名 )	
	void        LinkMeshManager::SaveLinkMap(const char* filename)
	{
		EditorStream saveStream;
		if( !saveStream.WriteFile(filename) )
			return ;

		saveStream.WriteUnChar( m_LinkMap , m_nMapSize*m_nMapSize );

		for( int i = 0 ; i < m_nMapSize ; i ++ )
		{
			for( int j = 0 ;j < m_nMapSize ; j ++ )
			{
				int sign = m_ModelMapInfo[j+i*m_nMapSize].modelID ;
				saveStream.WriteInt( &sign );
			}
		}

		saveStream.WriteUnChar( m_SignCullInfo ,  m_nMapSize*m_nMapSize );

		saveStream.WriteEnd();
	};
	//引导LINK模型	
	void        LinkMeshManager::Loading()
	{
		if( !m_bLoading )
			return;

		for( int i = 0 ; i < 16 ; i ++ )
		{
			if( m_iLoadingForwardID > 2 )
			{
				LoadingLinkMesh( m_iLoadingForwardID  );
			}

			if( m_iLoadingBackID < m_nMapSize*m_nMapSize-4 )
			{
				LoadingLinkMesh( m_iLoadingBackID );
			}

			m_iLoadingForwardID -- ;
			m_iLoadingBackID ++ ;

			if( m_iLoadingForwardID < 2 && m_iLoadingBackID > m_nMapSize*m_nMapSize-4 )
			{
				m_bLoading = false;
				g_bLoading = false;
				g_TerrainEdit.LoadingBrea();
				return;
			}
		}
	}
	//引导LINK模型;参数( 引导ID )	
	void        LinkMeshManager::LoadingLinkMesh( int loadingID )
	{
		if( !m_bLoading )
			return;

		if( m_bLoadedInfo == NULL )
		{

			return;
		}

		//重现创建新的linkmesh数据
		unsigned char signResult = 0;
		unsigned char sign[SIGN_NUM];

		int j = loadingID % m_nMapSize ;
		int i = loadingID / m_nMapSize ;

		if ( m_bLoadedInfo[i+j*m_nMapSize] != 0 )
			return;

		m_bLoadedInfo[i+j*m_nMapSize] = 1;

		{
			{
				std::string strname = g_TerrainEdit.GetEditData()->modelData.modelFileSet[ m_ModelMapInfo[i+j*m_nMapSize].modelID ];

				if( strname.find("linkmesh") == -1 )
				{			
					m_curLoadingID ++;
					return;
				}

				if( !SetSelectLinkMeshName( strname ) )
				{
					m_curLoadingID ++;
					return;
				}

				SetAuotDelBlock( ( m_SignCullInfo[i+j*m_nMapSize])&0x0f );
				//设置是否按顺序刷崖壁
				if( ( m_SignCullInfo[i+j*m_nMapSize]>>4 ) == 0 )
				{
					SetLinkOder( false  );
				}
				else
				{
					SetLinkOder( true );
				}

				signResult = 0;

				sign[SIGN_UP] = getLinkMapUnit(i, j+1);
				sign[SIGN_LEFT] = getLinkMapUnit(i-1, j);
				sign[SIGN_CENTER] = getLinkMapUnit(i,j);
				sign[SIGN_RIGHT] = getLinkMapUnit(i+1, j);
				sign[SIGN_DOWN] = getLinkMapUnit(i, j-1);

				for ( int k = 0 ; k < SIGN_NUM ; k ++ )
				{
					if ( sign[k] != 0 )
					{
						signResult |= m_SignSet[k] ;
					}
				}	

				bool bTest = false;
				if ( sign[0] > 0 && sign[1]>0&& sign[2]>0 && sign[3]>0 && sign[4]>0 )
					bTest = true;

				m_NewMap[i+j*m_nMapSize] = signResult ;

				bool bUpdate = false ;
				UpdateMesh( i,j,  GetMapSingFileName( i , j ,bTest ,&bUpdate ),&bUpdate);		

				m_UsedModelMap[i+j*m_nMapSize] = m_NewMap[i+j*m_nMapSize] ;
			}
		}

		m_curLoadingID ++;
		//m_bLoading = false ;
	};
	//引导LINK信息从文件;参数 ( 文件名 ); 返回( 为真表示成功 )
	bool   LinkMeshManager::LoadLinkMap(const char* filename)
	{
		if ( g_TerrainEdit.GetEditData()->modelData.modelFileSet.empty() )
			return false;

		EditorStream LoadStream;
		if( !LoadStream.ReadFile(filename) )
			return false;

		OGRE_DELETE_ARRAY ( m_bLoadedInfo );

		m_bLoadedInfo = new unsigned char [m_nMapSize*m_nMapSize];
		memset( m_bLoadedInfo , 0 , m_nMapSize*m_nMapSize );


		m_bLoading = true;
		g_bLoading = true;

		m_curLoadingID = 0 ;

		LoadStream.ReadUnChar( m_LinkMap , m_nMapSize*m_nMapSize );

		for( int i = 0 ; i < m_nMapSize ; i ++ )
		{
			for( int j = 0 ;j < m_nMapSize ; j ++ )
			{
				int sign ;
				LoadStream.ReadInt( &sign ); 
				m_ModelMapInfo[j+i*m_nMapSize].modelID = sign;
			}
		}

		LoadStream.ReadUnChar( m_SignCullInfo , m_nMapSize*m_nMapSize );
		return true;
	};
	//更新位置;参数( X , Y , 宽度,高度 )
	void   LinkMeshManager::UpdatePos( int x,int y ,int width ,int height )
	{
		for( int j = y ; j < height ; j ++ )
		{
			for( int i = x ;  i < width ; i ++ )
			{
				if( m_ModelMapInfo[i+j*m_nMapSize].pModel )
				{
					SetAuotDelBlock( ( m_SignCullInfo[i+j*m_nMapSize]&0x0f) );

					if ( g_TerrainEdit.GetEditData()->modelData.modelFileSet[ m_ModelMapInfo[i+j*m_nMapSize].modelID ].find("_C_C")!=-1)
					{		
						UpdateChunkHeight( i,j );
					}
					else
					{
						EditorModel* pModel = (EditorModel*)m_ModelMapInfo[i+j*m_nMapSize].pModel->GetModelData()->pModel;
						if( pModel )
						{
							pModel->SetPos( m_ModelMapInfo[i+j*m_nMapSize].pModel->GetModelData()->pos );
						}
					}

					UpdateCullMap( i , j );
				}
			}
		}
	}

	//更新LINK地图;参数 ( X ,Y , 宽度,高度 )
	void  LinkMeshManager::UpdateLinkMap(int x,int y ,int width,int height)
	{
		//周围一圈不做拼接
		unsigned char signResult = 0;
		unsigned char sign[SIGN_NUM];
		int maxy = y+height;
		if(maxy > m_nMapSize) maxy = m_nMapSize;
		int maxx = x+width;
		if(maxx > m_nMapSize) maxx = m_nMapSize;
		if(x < 0) x = 0;
		if(y < 0) y = 0;

		for(int j=y ; j<maxy; j++)
		{
			for(int i=x; i<maxx; i++)
			{
				signResult = 0;

				sign[SIGN_UP] = getLinkMapUnit(i, j+1);
				sign[SIGN_LEFT] = getLinkMapUnit(i-1, j);
				sign[SIGN_CENTER] = getLinkMapUnit(i,j);
				sign[SIGN_RIGHT] = getLinkMapUnit(i+1,j);
				sign[SIGN_DOWN] = getLinkMapUnit(i, j-1);

				for ( int k = 0 ; k < SIGN_NUM ; k ++ )
				{
					if ( sign[k] != 0 )
					{
						signResult |= m_SignSet[k] ;
					}
				}	

				bool bTest = false;
				if ( sign[0] > 0 && sign[1]>0&& sign[2]>0 && sign[3]>0 && sign[4]>0 )
					bTest = true;

				m_NewMap[i+j*m_nMapSize] = signResult;

				bool bUpdate = false ;
				UpdateMesh( i,j,  GetMapSingFileName( i , j ,bTest ,&bUpdate ),&bUpdate);		

				m_UsedModelMap[i+j*m_nMapSize] = m_NewMap[i+j*m_nMapSize] ;
			}
		}
	}

	//保存更新LINK地图数据;参数( X,Y , 宽度,高度 ) 	
	void  LinkMeshManager::SaveUpdateLinkMapData( int x ,int y , int width ,int height )
	{	

		CurrLinkMapSaveData   curData;

		int index = 0;
		int maxy = y+height;
		if(maxy > m_nMapSize) maxy = m_nMapSize;
		int maxx = x+width;
		if(maxx > m_nMapSize) maxx = m_nMapSize;
		if(x < 0) x = 0;
		if(y < 0) y = 0;

		for(int j = y ; j <maxy; j ++)
		{
			for ( int i = x; i<maxx; i++)
			{
				curData.saveData[index].index = i + j*m_nMapSize;
				curData.saveData[index].sign = getLinkMapUnit(i,j);

				index++;
			}
		}

		m_SaveMapData.push_back( curData );

		//限制后退最多为20
		if( m_SaveMapData.size() > 20 )
		{
			m_SaveMapData.pop_front();
		}
	}
	//更新裁减地图 ; 参数 ( X , Y )	
	void  LinkMeshManager::UpdateCullMap(int x,int y )
	{
		int gx,gy,mx,my;

		int nlinkgrid = m_nMapSize/m_nBlock.x; //一个block里面有多少个linkmesh的格子

		gx = ( x / nlinkgrid);
		gy = ( y / nlinkgrid);

		mx = m_UnitSize*( x % nlinkgrid);
		my = m_UnitSize*( y % nlinkgrid);

		//MAY::LogMessage( "裁减位置：gx = %d, gy = %d, mx = %d, my = %d " , gx,gy,mx,my);

		bool bUpdateChunk = false;

		MESHCULLINFO_MAP::iterator iter = m_CullInfoMap.find( m_NewMap[x+y*m_nMapSize] );

		if( iter != m_CullInfoMap.end() )
		{
			LinkMeshCullInfo cullInfo ;
			for( int k = 0 ; k < 4 ; k ++ )
				cullInfo.info[k] = iter->second.info[k];

			//根据文件名，进行，上下的区分
			float moveHeight = m_ModelMapInfo[x+y*m_nMapSize].fMoveHeight ;
			if( moveHeight < 0 )
			{
				if( m_nAutoDelBlock == 0 )
				{
					if( m_NewMap[x+y*m_nMapSize] != 55 )
					{
						for( int k = 0 ; k < 4 ; k ++ )
						{
							if( cullInfo.info[k] == 0 )
							{
								cullInfo.info[k] = 1;
							}
							else
							{
								cullInfo.info[k] = 0;
							}
						}
					}
				}
				else
				{
					for( int k = 0 ; k < 4 ; k ++ )
					{
						cullInfo.info[k] = m_nAutoDelBlock;
					}
				}		
				//保存是否自动删除标记
				if( m_nAutoDelBlock == 0 )
				{
					m_SignCullInfo[x+y*m_nMapSize] =  m_SignCullInfo[x+y*m_nMapSize]&0xf0 ;
				}
				else
				{
					m_SignCullInfo[x+y*m_nMapSize] = ( m_SignCullInfo[x+y*m_nMapSize]&0xf0 )|1 ;
				}

				if( cullInfo.info[0] == 0 )
				{	
					g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx, my+1 ,moveHeight - 0.2f );

				}
				if( cullInfo.info[1] == 0 )
				{	
					g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx+1, my+1 ,moveHeight-  0.2f  );

				}
				if( cullInfo.info[2] == 0 )
				{	
					g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx, my ,moveHeight -  0.2f  );

				}
				if( cullInfo.info[3] == 0 )
				{	
					g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetGridHeight( mx+1, my ,moveHeight - 0.2f );

				}
			}

			if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my+1 , cullInfo.info[0] ) ,!m_bLoading )			
				bUpdateChunk = true;

			if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my+1 ,cullInfo.info[1] ) , !m_bLoading )				
				bUpdateChunk = true;

			if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my , cullInfo.info[2] ) , !m_bLoading )		
				bUpdateChunk = true;

			if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my , cullInfo.info[3] ) , !m_bLoading )								
				bUpdateChunk = true;
		}
	};
	//自动拼接地表功能;类似WC3模式
	LinkTextureManager::LinkTextureManager()
	{
		m_strLinkPath = g_LinkTexturePath;

		m_SaveCurEditMeshName = m_strLinkPath + "dm5.link";

		m_nCurTextureID = 0;

		LinkMeshCullInfo info;
		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 2;
		int signID = MAP_CENTER_L_U ;
		m_CullInfoMap[signID] = info;
		info.info[0] = 2 ;info.info[1] = 2 ; info.info[2] = 2 ; info.info[3] = 0;
		m_CullInfoMap[44] = info;	

		signID = MAP_CENTER_R_U ;
		info.info[0] = 0 ;info.info[1] = 0; info.info[2] = 2 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;
		info.info[0] = 2 ;info.info[1] = 2 ; info.info[2] = 0 ; info.info[3] = 2;
		m_CullInfoMap[33] = info;	

		signID = MAP_CENTER_C_U ;
		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 2 ; info.info[3] = 2;
		m_CullInfoMap[signID] = info;

		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[55] = info;

		signID = MAP_CENTER_L_C;
		info.info[0] = 0 ;info.info[1] = 2 ; info.info[2] = 0 ; info.info[3] = 2;
		m_CullInfoMap[signID] = info;

		signID = MAP_CENTER_R_C;
		info.info[0] = 2 ;info.info[1] = 0 ; info.info[2] = 2 ; info.info[3] = 0;	
		m_CullInfoMap[signID] = info;

		signID = MAP_CENTER_L_D ;
		info.info[0] = 0 ;info.info[1] = 2 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;
		info.info[0] = 2 ;info.info[1] = 0 ; info.info[2] = 2 ; info.info[3] = 2;
		m_CullInfoMap[11] = info;	

		signID = MAP_CENTER_C_D ;
		info.info[0] = 2 ;info.info[1] = 2 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;

		signID = MAP_CENTER_R_D ;
		info.info[0] = 2 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 0;
		m_CullInfoMap[signID] = info;
		info.info[0] = 0 ;info.info[1] = 2 ; info.info[2] = 2 ; info.info[3] = 2;
		m_CullInfoMap[123] = info;	

		info.info[0] = 0 ;info.info[1] = 0 ; info.info[2] = 0 ; info.info[3] = 0;

		m_CullInfoMap[MAP_RU_LD] = info;	
		m_CullInfoMap[MAP_RD_LU] = info;


	}

	void    LinkTextureManager::UpdateChunkHeight( int x, int y )
	{
		return ;
	}

	void     LinkTextureManager::UpdateCullMap(int x,int y )
	{
		int gx,gy,mx,my;

		int nlinkgrid = m_nMapSize/m_nBlock.x; //一个block里面有多少个linkmesh的格子

		gx = ( x / nlinkgrid );
		gy = ( y / nlinkgrid );

		mx = m_UnitSize*( x % nlinkgrid );
		my = m_UnitSize*( y % nlinkgrid );

		//MAY::LogMessage( "裁减位置：gx = %d, gy = %d, mx = %d, my = %d " , gx,gy,mx,my);

		bool bUpdateChunk = false;

		//如果该位置为CC，则需要裁剪地形
		if( m_NewMap[x+y*m_nMapSize] == MAP_CENTER_C_C 
			|| m_NewMap[x+y*m_nMapSize] == 55 )
		{
			LinkMeshCullInfo cullInfo ;
			for( int k = 0 ; k < 4 ; k ++ )
				cullInfo.info[k] = 2;

			if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my+1 , cullInfo.info[0] ) )			
				bUpdateChunk = true;

			if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my+1 ,cullInfo.info[1] ) )				
				bUpdateChunk = true;

			if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my , cullInfo.info[2] ) )		
				bUpdateChunk = true;

			if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my , cullInfo.info[3] ) )								
				bUpdateChunk = true;

		}
		else
		{
			if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my+1 , 0 ) )			
				bUpdateChunk = true;

			if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my+1 ,0 ) )				
				bUpdateChunk = true;

			if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my , 0 ) )		
				bUpdateChunk = true;

			if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my , 0 ) )								
				bUpdateChunk = true;

		}
	};

	bool  LinkTextureManager::TestIfLinkMesh( int x , int y , std::string *strName ,int *_index)
	{

		//被裁剪，则不用替换地表
		{

			if( g_TerrainEdit.GetAutoLinkMesh()->m_SignCullInfo[x+y*m_nMapSize] )
				return true;

		}

		if( g_TerrainEdit.GetAutoLinkMesh()->m_NewMap[x+y*m_nMapSize] == 0 
			|| g_TerrainEdit.GetAutoLinkMesh()->m_NewMap[x+y*m_nMapSize] == 55 
			|| g_TerrainEdit.GetAutoLinkMesh()->m_NewMap[x+y*m_nMapSize] == SIGN_TYPE_UNKOWN ) 
		{
			return false; 
		}

		std::string signName = GetSignName( x , y );

		if( signName.size() <= 1 )
			return false;

		if (  g_TerrainEdit.GetAutoLinkMesh()->m_ModelMapInfo[x+y*m_nMapSize].fMoveHeight > 0 )
		{
			(*strName) = "model.linktexture.up.link" + signName ;
		}
		else
		{
			return true;
			(*strName) = "model.linktexture.down.link" + signName ;
		}

		int   index = g_TerrainEdit.AddModelName( *strName );
		*_index = index ;

		return false;

	}

	bool  LinkTextureManager::UpdateMesh(int x,int y,const char* filename ,bool *bUpdate)
	{
		//if ( !*bUpdate )
		//	return false;

		if ( filename == NULL )
		{
			if( m_ModelMapInfo[x+y*m_nMapSize].pModel )
			{ 		 
				DelLinkMesh( x ,y );	

				int gx,gy,mx,my;
				bool bUpdateChunk = false;

				int nlinkgrid = m_nMapSize/m_nBlock.x; //一个block里面有多少个linkmesh的格子

				gx = ( x / nlinkgrid );
				gy = ( y / nlinkgrid );

				mx = m_UnitSize*( x % nlinkgrid );
				my = m_UnitSize*( y % nlinkgrid );
				if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my+1 , 0 ) )
					bUpdateChunk = true;

				if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my+1 ,0 ) )		
					bUpdateChunk = true;

				if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my , 0 ) )	
					bUpdateChunk = true;

				if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my , 0 ) )							
					bUpdateChunk = true;
				return true;
			}

			return false;
		}

		std::string strname(m_SaveCurEditMeshName.c_str());
		strname = strname + filename;

		if( strname.find("_C_C") != - 1 )
		{
			int nRandKind = rand() % 17 ;
			char infoRand[32] ;
			sprintf( infoRand , "%d" , nRandKind );
			strname = strname + infoRand ;
		}

		int   index = g_TerrainEdit.AddModelName( strname );

		//如果已经加载模型，则返回
		if( m_ModelMapInfo[x+y*m_nMapSize].modelID == index &&
			m_ModelMapInfo[x+y*m_nMapSize].pModel )
		{
			if( strname.find("_C_C") != -1 )
			{
				UpdateChunkHeight( x,y );
			}
			else
			{
				return true;
			}
		}
		//删除当前模型
		if ( //m_ModelMapInfo[x+y*m_nMapSize].modelID != index &&
			m_ModelMapInfo[x+y*m_nMapSize].pModel != NULL )
		{
			DelLinkMesh( x ,y );
		}

		if ( TestIfLinkMesh( x , y , &strname , &index ) )
		{
			return true;
		}


		TerrModel*  model = new TerrModel;
		//该模型类型为link模型
		model->GetModelData ()->cType = TYPE_LINK_TEXTURE ;

		strcpy(model->GetModelData ()->name , strname.c_str() );
		model->GetModelData ()->modelID = index | ( m_nCurTextureID << 16 ) ;

		model->Init ();

		Ogre::Vector3 pos;
		pos.x = m_fWidthSize*x ;
		pos.z = m_fHeightSize*y ;

		if(!g_Terrain.GetHeightEx( pos.x , pos.z , &pos.y ) )
			return false;

		if( !strname.empty() )
		{
			EditorModel * pMayModel = (EditorModel*)( model->GetModelData()->pModel );

			if( pMayModel )
			{
				Ogre::Texture * ptex = g_TexManager.LoadTex( g_TerrainEdit.GetEditData()->texData.texFileSet[m_nCurTextureID].c_str());
				pMayModel->SetLinkMeshTexture(ptex);

				pMayModel->SetPos( pos );
				model->AttachRoom();			
			}

		}

		model->GetModelData()->pos = pos ;
		model->ResetMat();

		g_TerrainEdit.GetEditData()->modelData .modelSet .push_back (model);

		m_ModelMapInfo[x+y*m_nMapSize].modelID = index | ( m_nCurTextureID << 16 ) ;
		m_ModelMapInfo[x+y*m_nMapSize].pModel = model;
		m_ModelMapInfo[x+y*m_nMapSize].fMoveHeight = m_fMoveHeight;

		if( strname.find("_C_C") != -1 )
		{
			UpdateChunkHeight( x,y );
			UpdateCullMap( x , y  );
		}
		else
		{
			UpdateCullMap( x, y );
		}

		return true;
	}	

	void   LinkTextureManager::SetTextureID( int  nID )
	{
		m_nCurTextureID = nID ;
	};

	void   LinkTextureManager::UpdatePos( int x,int y ,int width ,int height )
	{
		for( int j = y ; j < height ; j ++ )
		{
			for( int i = x ;  i < width ; i ++ )
			{
				if( m_ModelMapInfo[i+j*m_nMapSize].pModel )
				{
					EditorModel* pModel = (EditorModel*)m_ModelMapInfo[i+j*m_nMapSize].pModel->GetModelData()->pModel;

					if( pModel )
					{
						pModel->SetPos( m_ModelMapInfo[i+j*m_nMapSize].pModel->GetModelData()->pos );
						m_ModelMapInfo[i+j*m_nMapSize].pModel->ResetMat();	
					}

					UpdateCullMap( i , j );
				}
			}
		}
	};

	const char*  LinkTextureManager::GetSignName( int x , int y )
	{
		int sign = g_TerrainEdit.GetAutoLinkMesh()->m_NewMap[x+y*m_nMapSize] ;
		switch( sign )
		{
		case 11:
			return "_L_D_I";
			break;
		case 123:
			return "_R_D_I";
			break;
		case 33:
			return "_R_U_I";
			break;
		case 44:
			return  "_L_U_I";
			break;
		case 55:
			return "_C_C";
			break;
		case MAP_RU_LD:
			return "_LD_RU";
			break;
		case MAP_RD_LU:
			return "_RD_LU";
			break;
		}


		if( x < 0 || x >= m_nMapSize )
			return "";

		if( y < 0 || y >= m_nMapSize )
			return "";

		int type = SIGN_TYPE_UNKOWN;
		for ( int i = 0 ;i < SIGN_TYPE_NUM ; i ++ )
		{
			if ( sign == m_SignMapSet[i] )
				type =  (SIGN_TYPE)i;
		}


		if ( type == SIGN_TYPE_UNKOWN )
		{
			return "";
		}

		return g_AutoLinkFileName[(int)type];
	};


	void   LinkTextureManager::LoadingLinkMesh( int loadingID )
	{
		//重现创建新的linkmesh数据
		unsigned char signResult = 0;
		unsigned char sign[SIGN_NUM];

		int j = m_curLoadingID % m_nMapSize ;
		int i = m_curLoadingID / m_nMapSize ;

		//如果是边缘则，不需要更新自动拼接
		if( i == 0 || j == 0 )
			return;

		{
			{
				m_nCurTextureID =  (m_ModelMapInfo[i+j*m_nMapSize].modelID&0xffff0000)>>16   ;

				signResult = 0;

				sign[SIGN_UP] = getLinkMapUnit(i,j+1);
				sign[SIGN_LEFT] = getLinkMapUnit(i-1,j);
				sign[SIGN_CENTER] = getLinkMapUnit(i,j);
				sign[SIGN_RIGHT] = getLinkMapUnit(i+1,j);
				sign[SIGN_DOWN] = getLinkMapUnit(i,j-1);

				for ( int k = 0 ; k < SIGN_NUM ; k ++ )
				{
					if ( sign[k] != 0 )
					{
						signResult |= m_SignSet[k] ;
					}
				}	

				bool bTest = false;
				if ( sign[0] > 0 && sign[1]>0&& sign[2]>0 && sign[3]>0 && sign[4]>0 )
					bTest = true;

				m_NewMap[i+j*m_nMapSize] = signResult ;

				bool bUpdate = false ;
				UpdateMesh( i,j,  GetMapSingFileName( i , j ,bTest ,&bUpdate ),&bUpdate);		

				m_UsedModelMap[i+j*m_nMapSize] = m_NewMap[i+j*m_nMapSize] ;
			}
		}

		m_curLoadingID ++;
	}

	bool   LinkTextureManager::LoadLinkMap(const char* filename)
	{
		if ( g_TerrainEdit.GetEditData()->modelData.modelFileSet.empty() )
			return false;


		EditorStream LoadStream;
		if( !LoadStream.ReadFile(filename) )
			return false;

		m_curLoadingID = 0;
		m_bLoading = true;

		LoadStream.ReadUnChar( m_LinkMap , m_nMapSize*m_nMapSize );

		for( int i = 0 ; i < m_nMapSize ; i ++ )
		{
			for( int j = 0 ;j < m_nMapSize ; j ++ )
			{
				int sign ;
				LoadStream.ReadInt( &sign ); 
				m_ModelMapInfo[j+i*m_nMapSize].modelID = sign;
			}
		}

		return true;
	};

	LinkOverlayManager::LinkOverlayManager()
	{
		m_SaveCurEditMeshName = g_WallPaperPath ;
	}

	void   LinkOverlayManager::SetMapSign(Ogre::Vector3 pos , unsigned char sign )
	{
		int x ,y ;

		x = (int) ( pos.x / m_fWidthSize );
		y = (int) ( pos.z / m_fHeightSize );

		//记录修改前的位置，以便进行undo处理	
		SaveUpdateLinkMapData( x , y , 1 , 1 );

		if( x < 0 || x >= m_nMapSize )
			return;

		if( y < 0 || y >= m_nMapSize )
			return;

		m_LinkMap[y*m_nMapSize + x] = sign;

		if( sign )
		{
			bool bUpdate = true;
			UpdateMesh( x,y,"linka" ,&bUpdate);	
		}
		else
		{
			DelLinkMesh( x , y );
		}
	};

	bool   LinkOverlayManager::LoadLinkMap(const char* filename)
	{
		if ( g_TerrainEdit.GetEditData()->modelData.modelFileSet.empty() )
			return false;

		EditorStream LoadStream;
		if( !LoadStream.ReadFile(filename) )
			return false;

		LoadStream.ReadUnChar( m_LinkMap , m_nMapSize*m_nMapSize );

		for( int i = 0 ; i < m_nMapSize ; i ++ )
		{
			for( int j = 0 ;j < m_nMapSize ; j ++ )
			{
				int sign ;
				LoadStream.ReadInt( &sign ); 
				m_ModelMapInfo[j+i*m_nMapSize].modelID = sign;
			}
		}

		for ( int j = 1 ; j < m_nMapSize-2 ; j ++ )
		{
			for ( int i = 1 ; i < m_nMapSize-2 ; i ++ )
			{	
				bool bUpdate = true ;

				if( getLinkMapUnit(i,j) != 0 )
				{
					SetTextureID( ( m_ModelMapInfo[i+j*m_nMapSize].modelID & 0xffff0000 ) >> 16 );
					UpdateMesh( i,j, "linka",&bUpdate);		
				}
			}
		}

		return true;
	}

	void     LinkOverlayManager::UpdateCullMap(int x,int y )
	{
		int gx,gy,mx,my;

		int nlinkgrid = m_nMapSize/m_nBlock.x; //一个block里面有多少个linkmesh的格子

		gx = ( x / nlinkgrid );
		gy = ( y / nlinkgrid );

		mx = m_UnitSize*( x % nlinkgrid );
		my = m_UnitSize*( y % nlinkgrid );

		bool bUpdateChunk = false;

		if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my+1 , 1 ) )			
			bUpdateChunk = true;

		if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my+1 ,1 ) )					
			bUpdateChunk = true;

		if( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx, my , 1) )			
			bUpdateChunk = true;			

		if ( g_TerrainEdit.GetEditData()->chunkSet[gy][gx].SetCullInfoEx( mx+1, my , 1) )								
			bUpdateChunk = true;
	};

	bool   LinkOverlayManager::UpdateMesh(int x,int y,const char* filename ,bool *bUpdate)
	{
		std::string strname(m_SaveCurEditMeshName.c_str());
		strname = strname + filename;

		int   index = g_TerrainEdit.AddModelName( strname );
		//删除当前模型
		if ( (  m_ModelMapInfo[x+y*m_nMapSize].modelID == (index | ( m_nCurTextureID << 16 ) ) )&&
			( m_ModelMapInfo[x+y*m_nMapSize].pModel != NULL ) )
		{
			return true;
		}

		DelLinkMesh(x,y);

		TerrModel*  model = new TerrModel;
		//该模型类型为link模型
		model->GetModelData ()->cType = TYPE_LINK_TEXTURE ;

		strcpy(model->GetModelData ()->name , strname.c_str() );
		model->GetModelData ()->modelID = index | ( m_nCurTextureID << 16 ) ;

		model->Init ();

		Ogre::Vector3 pos;
		pos.x = m_fWidthSize*x ;
		pos.z = m_fHeightSize*y ;

		if(!g_Terrain.GetHeightEx( pos.x , pos.z , &pos.y ) )
			return false;

		if( !strname.empty() )
		{
			EditorModel * pMayModel = (EditorModel*)( model->GetModelData()->pModel );

			Ogre::Texture * ptex = g_TexManager.LoadTex( g_TerrainEdit.GetEditData()->texData.texFileSet[m_nCurTextureID].c_str());
			pMayModel->SetLinkMeshTexture(ptex);

			pMayModel->SetPos( pos );
			model->AttachRoom();		
		}

		model->GetModelData()->pos = pos ;
		model->ResetMat();

		g_TerrainEdit.GetEditData()->modelData .modelSet .push_back (model);

		m_ModelMapInfo[x+y*m_nMapSize].modelID = index | ( m_nCurTextureID << 16 ) ;
		m_ModelMapInfo[x+y*m_nMapSize].pModel = model;
		m_ModelMapInfo[x+y*m_nMapSize].fMoveHeight = m_fMoveHeight;

		UpdateCullMap( x, y );

		return true;
	}
}

