
#include "EditEvent_Slope.h"
#include "TerrainEdit.h"
#include "EditorModel.h"
#include "EditorStream.h"
#include "TextureManager.h"
#include "Terrain.h"
#include "LinkTile.h"
#include "EditorCamera.h"
#include "MapMark.h"

#include "OgreLog.h"
#include "OgreModel.h"

using namespace Ogre;

namespace TE
{
	SlopeEditHandler::SlopeEditHandler(void)
	{
		//设置处理的类型
		m_nProcessType = BRAE_EDIT_EVN ;

		m_nCurMoveGx = m_nCurMoveGy = -1;
		m_nCurMoveMx = m_nCurMoveMy = -1;

		m_bLoaded = false;
	}


	SlopeEditHandler::~SlopeEditHandler(void)
	{
	}
	//更新	
	void   SlopeEditHandler::Update()
	{
		for( size_t i = 0 ; i < m_BreaInfoSet.size() ; i ++ )
		{
			m_BreaInfoSet[i].pLModel ->UpdatePos();
			m_BreaInfoSet[i].pRmodel ->UpdatePos();

			UpdateSaveChunkHeight( m_BreaInfoSet[i].gx , m_BreaInfoSet[i].gy , 
				m_BreaInfoSet[i].mx , m_BreaInfoSet[i].my , m_BreaInfoSet[i].fHeight );
		}

	};	
	//增加模型; 参数( 文件名,位置 )
	TerrModel*  SlopeEditHandler::AddModel( const char* filename , Ogre::Vector3 pos )
	{
		std::string strname = filename;

		int   index = g_TerrainEdit.AddModelName( strname );

		TerrModel*  model = new TerrModel;
		//该模型类型为link模型
		model->GetModelData ()->cType = TYPE_LINK_MODEL ;
		strcpy(model->GetModelData ()->name , filename );

		std::string kindName = filename ;
		int modelTypeID = g_TerrainEdit.GetAutoLinkMesh()->GetLinkModelKindID( kindName ); 

		model->GetModelData ()->modelID = ( modelTypeID<<16 )|index ;

		model->Init ();

		model->GetModelData()->pos = pos ;
		model->ResetMat();

		model->UpdatePos();
		model->AttachRoom();

		g_TerrainEdit.GetEditData()->modelData .modelSet .push_back (model);

		return model ;
	};
	//保存;参数( 文件名 ) ; 返回 ( 为真表示成功 )	
	bool    SlopeEditHandler::Save( const char* filename )
	{	

		EditorStream  stream;
		stream.WriteFile( filename );

		int nBraeNum = m_BreaSet.size();
		stream.WriteInt( &nBraeNum );

		for( size_t i = 0 ; i < m_BreaSet.size() ; i ++ )
		{
			stream.Write( m_BreaSet[i] );
		}

		stream.WriteEnd();

		return true;
	};
	//动态引导; 参数( 编辑器指针 )
	void    SlopeEditHandler::Loading( TerrainEditMgr* pEditor )
	{
		if( !m_bLoaded )
			return;

		for( size_t i = 0 ; i < m_SaveBreaInfoSet.size() ; i ++ )
		{
			AddBrea( pEditor , m_SaveBreaInfoSet[i].dir , m_SaveBreaInfoSet[i].pos );
		}

		m_bLoaded = false;
	};
	//读取;参数( 编辑器指针, 文件名 ) ;返回( 为真表示成功)
	bool    SlopeEditHandler::Read( TerrainEditMgr* pEditor ,const char* filename )
	{

		EditorStream  stream;
		if ( !stream.ReadFile( filename ))
			return false ;

		int nBraeNum ;
		stream.ReadInt( &nBraeNum );

		if ( nBraeNum == 0 )
			return true;

		m_SaveBreaInfoSet.clear();

		std::string name ;
		SetBraeModelName( name );

		MAddBreaInfo info;

		for( int i = 0 ; i < nBraeNum ; i ++ )
		{
			stream.Read( info );

			m_SaveBreaInfoSet.push_back( info );
		}

		m_bLoaded = true;

		return true;
	};
	//删除当前斜坡; 参数( 编辑器指针 )
	void   SlopeEditHandler::DelCurBrae( TerrainEditMgr* pEditor )
	{
		MBREAINFO_VEC::iterator iter ;

		MADDBREA_VEC::iterator  iter1 = m_BreaSet.begin() ;

		for( iter = m_BreaInfoSet.begin()  ;iter != m_BreaInfoSet.end() ; iter ++ ,iter1++)
		{
			MBreaInfo info = ( *iter );

			if ( (*iter).gx == m_nCurMoveGx &&
				(*iter).gy == m_nCurMoveGy && 
				(*iter).mx == m_nCurMoveMx && 
				(*iter).my == m_nCurMoveMy )
			{

				DelBrea( pEditor , &(*iter));

				m_BreaInfoSet.erase( iter );
				m_BreaSet.erase( iter1 );

				return;
			}

		}

	};
	//删除斜破; 参数( 编辑器指针, 斜坡信息 )
	void  SlopeEditHandler::DelBrea(  TerrainEditMgr* pEditor ,MBreaInfo*  pInfo)
	{
		std::vector<TerrModel*>::iterator  iter;

		for( iter = g_TerrainEdit.GetEditData()->modelData .modelSet.begin() ; iter != g_TerrainEdit.GetEditData()->modelData .modelSet.end() ; iter++ )
		{        
			if( *iter == pInfo->pLModel )	
			{   
				delete( *iter );

				g_TerrainEdit.GetEditData()->modelData .modelSet.erase( iter );

				break;
			}	
		}	

		for( iter = g_TerrainEdit.GetEditData()->modelData .modelSet.begin() ; iter != g_TerrainEdit.GetEditData()->modelData .modelSet.end() ; iter++ )
		{        
			if( *iter == pInfo->pRmodel )	
			{   
				delete( *iter );

				g_TerrainEdit.GetEditData()->modelData .modelSet.erase( iter );

				break;
			}	
		}	

		UpdateSaveChunkHeight( pInfo->gx , pInfo->gy ,pInfo->mx ,pInfo->my , 0 ); 

		pEditor->GetAutoLinkMesh()->UpdateLinkMap( pInfo->x  - 1 , pInfo->y - 1 ,5 , 3 );
	};
	//增加斜坡; 参数( 编辑器指针,射线方向,射线位置)
	void  SlopeEditHandler::AddBrea( TerrainEditMgr* pEditor , Ogre::Vector3 dir , Ogre::Vector3 pos ) 
	{		
		Ogre::Vector3  pos0;

		if(g_Terrain.TestAndRay( dir,pos,pos0))
		{	
			int ggx,ggy ;

			float fSize = 2*TERRBLOCK_SIZE / 32.0f ;

			if ( g_Terrain.GetGridX( pos0.x , &ggx ) && g_Terrain.GetGridZ( pos0.z , &ggy ) )	
			{

				Ogre::Vector3 chunkPos ;

				g_Terrain.GetGridPos( &chunkPos , ggx , ggy );

				float ddx , ddy ;

				ddx = pos0.x - chunkPos.x ;
				ddy = pos0.z - chunkPos.z ;

				unsigned char  mmx ,mmy ;

				mmx = (unsigned char)( ddx / ( fSize ) );
				mmy = (unsigned char)( ddy / ( fSize ) );

				int index1,index2 ;

				int nblock = g_TerrainEdit.GetEditData()->gw;
				index1 = ggx*nblock +  mmx + ( ggy*nblock +mmy ) * 256 ;
				index2 = index1 + 1;


				if( pEditor->GetAutoLinkMesh()->m_NewMap[index1] == ( MAP_CENTER_C_D ) && 
					pEditor->GetAutoLinkMesh()->m_NewMap[index2] ==  ( MAP_CENTER_C_D ) &&
					pEditor->GetAutoLinkMesh()->m_NewMap[index2+1] ==  ( MAP_CENTER_C_D ))
				{	

					if ( FindeBrea( ggx ,ggy , 2*mmx , 2*mmy ) )
						return;

					int indx ,indy ;
					indx = ggx*nblock + mmx ;
					indy = ggy*nblock + mmy ;

					MBreaInfo  breaInfo;

					pEditor->GetAutoLinkMesh()->DelLinkMesh( indx ,indy  );
					pEditor->GetAutoLinkMesh()->DelLinkMesh( indx+1 ,indy  );
					pEditor->GetAutoLinkMesh()->DelLinkMesh( indx+2 ,indy  );

					pos0 = pEditor->GetEditData()->chunkSet[ggy][ggx].GetChunkVec( 2*mmx ,2*mmy );
					breaInfo.pLModel = AddModel( "model.braemesh.model2.link_cd_l" , pos0 );

					if( 2*(mmx + 2) > 32 )
					{
						pos0 = pEditor->GetEditData()->chunkSet[ggy][ggx+1].GetChunkVec( 2 ,2*(mmy) );
					}
					else
					{
						pos0 = pEditor->GetEditData()->chunkSet[ggy][ggx].GetChunkVec( 2*(mmx+2) ,2*(mmy) );
					}
					breaInfo.pRmodel = AddModel( "model.braemesh.model2.link_cd_r" , pos0 );

					breaInfo.x = indx ;
					breaInfo.y = indy ;
					breaInfo.gx = ggx ;
					breaInfo.gy = ggy ;
					breaInfo.mx = 2*mmx ;
					breaInfo.my = 2*mmy ;
					breaInfo.fHeight = ( m_fMaxHeight - m_fMinHeight ) / 6.0f ;
					// 对Undo进行操作
					m_BreaInfoSet.push_back( breaInfo );

					UpdateChunkHeight( ggx , ggy , 2*mmx , 2*mmy );

					MAddBreaInfo brea;
					brea.x = indx ;
					brea.y = indy ;
					brea.nType = 0;
					brea.pos = pos ;
					brea.dir = dir;
					m_BreaSet.push_back( brea );
				}   

			}
		}
	};
	//设置斜坡模型; 参数( 文件名 )
	void   SlopeEditHandler::SetBraeModelName( std::string &name )
	{	
		TerrModel*  model = new TerrModel;
		strcpy(model->GetModelData ()->name , "model.braemesh.model2.link_cd_c" );

		model->Init ();

		EditorModel * pMayModel = (EditorModel*)( model->GetModelData()->pModel );

		BoxSphereBound bound;
		pMayModel->GetShowModel()->getMainModel()->getLocalBounds(bound);
		m_fMinHeight = bound.getMin().y ;
		m_fMaxHeight = bound.getMax().y ;

		delete( model );
	};	
	//查找斜坡; 参数( 格子坐标X,Y ,小格子坐标 X,Y ) ; 返回( 为真表示找到 )
	bool   SlopeEditHandler::FindeBrea( int gx ,int gy ,int mx ,int my )
	{
		for( size_t i = 0 ; i < m_BreaInfoSet.size() ; i ++ )
		{
			if ( m_BreaInfoSet[i].gx == gx && 
				m_BreaInfoSet[i].gy == gy && 
				m_BreaInfoSet[i].mx  == mx && 
				m_BreaInfoSet[i].my == my )
			{
				return true;
			}
		}

		return false;
	};
	//更新保存的CHUNK高度 ; 参数( 格子坐标X,Y ,小格子坐标 X,Y , 高度 )
	void   SlopeEditHandler::UpdateSaveChunkHeight( int gx ,int gy ,int mx ,int my , float fHeight )
	{
		int indexx = mx - 4 ;
		int indexy = my - 4 ;

		float dxHeight = fHeight ;

		int cgx[4],cmx[4];
		cgx[0] = cgx[1] = cgx[2] = cgx[3] = gx ;

		cmx[0] = mx + 1;
		cmx[1] = mx + 2;
		cmx[2] = mx + 3;
		cmx[3] = mx + 4;

		for( int i = 0 ; i < 4 ; i ++ )
		{
			if ( cmx[i] >= 32 )
			{
				cmx[i] = cmx[i] - 32 ;
				cgx[i] += 1;
			}
		}

		for ( int i = 0 ; i < 7 ; i ++ )
		{
			float height = i*dxHeight;

			int ngx , ngy ;
			int  nmy ;

			ngx = gx ;
			ngy = gy ;

			nmy = indexy ;

			if( indexy < 0 )
			{
				ngy = gy - 1;
				nmy = 32 + indexy;
			}
			else if ( indexy >= 32 )
			{
				ngy = gy + 1;
				nmy = indexy - 32 ;
			}


			if ( i < 6 )
			{
				if( fHeight == 0 )
				{
					g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[0]].SetCullInfoEx( cmx[0], nmy , 0) ;	
					g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[3]].SetCullInfoEx( cmx[3], nmy , 0) ;	
				}
				else
				{
					g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[0]].SetCullInfoEx( cmx[0], nmy , 1) ;	
					g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[3]].SetCullInfoEx( cmx[3], nmy , 1) ;	
				}
			}

			g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[1]].SetGridHeight( cmx[1] , nmy , height );
			g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[2]].SetGridHeight( cmx[2] , nmy , height ); 

			if( nmy == 0 )
			{
				g_TerrainEdit.GetEditData()->chunkSet[ngy-1][cgx[1]].SetGridHeight( cmx[1] , 32 , height );
				g_TerrainEdit.GetEditData()->chunkSet[ngy-1][cgx[2]].SetGridHeight( cmx[2] , 32 , height ); 
			}
			else if( nmy == 32 )
			{
				g_TerrainEdit.GetEditData()->chunkSet[ngy+1][cgx[1]].SetGridHeight( cmx[1] , 0 , height );
				g_TerrainEdit.GetEditData()->chunkSet[ngy+1][cgx[2]].SetGridHeight( cmx[2] , 0 , height ); 
			}

			indexy++;
		}				

		if( fHeight != 0 )
		{
			g_TerrainEdit.GetEditData()->chunkSet[gy][cgx[1]].SetCullInfoEx( cmx[1], my+1 , 0) ;	
			g_TerrainEdit.GetEditData()->chunkSet[gy][cgx[2]].SetCullInfoEx( cmx[2], my+1 , 0) ;	
		}
	};

	//更新chunk高度; 参数( 格子坐标X,Y ,小格子坐标 X,Y )
	void  SlopeEditHandler::UpdateChunkHeight( int gx, int gy  ,int mx ,int my )
	{
		int indexx = mx - 4 ;
		int indexy = my - 4 ;

		float dxHeight = ( m_fMaxHeight - m_fMinHeight ) / 6.0f ;


		int cgx[4],cmx[4];
		cgx[0] = cgx[1] = cgx[2] = cgx[3] = gx ;

		cmx[0] = mx + 1;
		cmx[1] = mx + 2;
		cmx[2] = mx + 3;
		cmx[3] = mx + 4;

		for( int i = 0 ; i < 4 ; i ++ )
		{
			if ( cmx[i] >= 32 )
			{
				cmx[i] = cmx[i] - 32 ;
				cgx[i] += 1;
			}
		}

		for ( int i = 0 ; i < 7 ; i ++ )
		{
			float height = i*dxHeight;

			int ngx , ngy ;
			int  nmy ;

			ngx = gx ;
			ngy = gy ;

			nmy = indexy ;

			if( indexy < 0 )
			{
				ngy = gy - 1;
				nmy = 32 + indexy;
			}
			else if ( indexy >= 32 )
			{
				ngy = gy + 1;
				nmy = indexy - 32 ;
			}


			if ( i < 6 )
			{
				g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[0]].SetCullInfoEx( cmx[0], nmy , 1) ;	
				g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[3]].SetCullInfoEx( cmx[3], nmy , 1) ;	
			}

			g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[1]].SetGridHeight( cmx[1] , nmy , height );
			g_TerrainEdit.GetEditData()->chunkSet[ngy][cgx[2]].SetGridHeight( cmx[2] , nmy , height ); 

			if( nmy == 0 )
			{
				g_TerrainEdit.GetEditData()->chunkSet[ngy-1][cgx[1]].SetGridHeight( cmx[1] , 32 , height );
				g_TerrainEdit.GetEditData()->chunkSet[ngy-1][cgx[2]].SetGridHeight( cmx[2] , 32 , height ); 
			}
			else if( nmy == 32 )
			{
				g_TerrainEdit.GetEditData()->chunkSet[ngy+1][cgx[1]].SetGridHeight( cmx[1] , 0 , height );
				g_TerrainEdit.GetEditData()->chunkSet[ngy+1][cgx[2]].SetGridHeight( cmx[2] , 0 , height ); 
			}

			indexy++;
		}			

		g_TerrainEdit.GetEditData()->chunkSet[gy][cgx[1]].SetCullInfoEx( cmx[1], my+1 , 0) ;	
		g_TerrainEdit.GetEditData()->chunkSet[gy][cgx[2]].SetCullInfoEx( cmx[2], my+1 , 0) ;	
	}

	//处理过程;参数( 编辑器指针, 消息响应事件 ) ; 返回 ( 为真表示成功 )
	bool SlopeEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
	{

		switch( event->mouseEven )
		{
		case MouseLDown:
			{	
				AddBrea( pEditor ,event->dir , event->pos );
			}
			break;
		case MouseMove:
			{		
				Ogre::Vector3  pos0;
				if(g_Terrain.TestAndRay( event->dir,event->pos,pos0))
				{			
					int ggx,ggy ;

					float fSize = 2*TERRBLOCK_SIZE / 32.0f ;

					if ( g_Terrain.GetGridX( pos0.x , &ggx ) && g_Terrain.GetGridZ( pos0.z , &ggy ) )
					{
						Ogre::Vector3 chunkPos ;
						g_Terrain.GetGridPos( &chunkPos , ggx , ggy );

						float ddx , ddy ;
						ddx = pos0.x - chunkPos.x ;
						ddy = pos0.z - chunkPos.z ;

						unsigned char  mmx ,mmy ;
						mmx = (unsigned char)( ddx / ( fSize ) );
						mmy = (unsigned char)( ddy / ( fSize ) );	

						m_nCurMoveGx = ggx ; m_nCurMoveGy = ggy ;
						m_nCurMoveMx = 2*mmx ; m_nCurMoveMy = 2*mmy;

						pos0 = pEditor->GetEditData()->chunkSet[ggy][ggx].GetChunkVec( 2*mmx+1 ,2*mmy+1 );
					}

					pEditor->m_pSignSet->m_PosSet.clear ();
					pEditor->m_pSignSet->UpdateData ( pos0 , fSize/2.0f , 16 );
				}
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

		case MouseRUp:
			{
			}
			break;
		}

		return true;
	};
	//后退操作 ; 参数( 编辑器指针 )
	void  SlopeEditHandler::Undo( TerrainEditMgr *pEditor )
	{
		if( m_BreaInfoSet.empty() )
		{
			return;
		}

		MBreaInfo info = m_BreaInfoSet[ m_BreaInfoSet.size()-1 ];

		DelBrea( pEditor , &info );

		m_BreaSet.pop_back();
		m_BreaInfoSet.pop_back();

		return ;
	};
	//保存数据 ; 参数( 编辑器指针)
	void  SlopeEditHandler::SaveData( TerrainEditMgr* pEditor )
	{

	}
}
