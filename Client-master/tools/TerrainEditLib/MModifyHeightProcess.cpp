#include ".\mmodifyheightprocess.h"

#include "EditorCamera.h"
#include "mterrain.h"
#include "mstream.h"
#include "TerrainTile.h"
#include "MSign.h"
#include "LinkTile.h"
#include "SlopeMark.h"

using namespace TE;

inline int randex( int num , int id)
{
	static unsigned int _randNum = 8999;
	
	srand( id + _randNum );

	int nRand = rand() % num ;

	_randNum = nRand*7456;

	return nRand;
}


float GetRampHeight( MVec3  pos0,MVec3 pos1,MVec3  srcPos ,float height,float length)
{
	float _height;
	MVec2  _pos0,_pos1,_srcPos;
	_pos0.x = pos0.x ;
	_pos0.y = pos0.z ;
	_pos1.x = pos1.x ;
	_pos1.y = pos1.z ;
	_srcPos.x = srcPos.x ;
	_srcPos.y = srcPos.z ;

	MVec2  length1,length2;
	length1 = _srcPos - _pos0 ; float length_1 = length1.lengthSqr ();
	length2 = _srcPos - _pos1 ; float length_2 = length2.lengthSqr ();

	float dy = ( length_1 - length_2 + length*length ) / ( 2*length );
	_height = ( dy / length )*height;
	if( _height < 0 ) _height = 0 ;

	_height += pos0.y ;

	return _height;
}

MModifyHeightProcess::MModifyHeightProcess(void)
{     
	m_nProcessType = CHANGE_HEIGHT_EVN;
}

MModifyHeightProcess::~MModifyHeightProcess(void)
{
	for( size_t i = 0 ; i < m_SaveMapHeightData.size () ; i++)
	{
		m_SaveMapHeightData[i].clear ();
	}	
	m_SaveMapHeightData.clear ();
}

void  MModifyHeightProcess::SaveData( MTerrainEdit* pEditor )
{
	m_SaveMapHeightData.push_back ( pEditor->m_HeightMap.m_vHeight );

	if( m_SaveMapHeightData.size () >= 10 )
		m_SaveMapHeightData.pop_front ();
};

void  MModifyHeightProcess::Clear()
{
	for( size_t i = 0 ; i < m_SaveMapHeightData.size () ; i++)
	{
		m_SaveMapHeightData[i].clear ();
	}	
	m_SaveMapHeightData.clear ();
}

void  MModifyHeightProcess::Undo( MTerrainEdit *pEditor )
{
	if( !m_SaveMapHeightData.empty () )			
	{	
		pEditor->m_HeightMap.m_vHeight = m_SaveMapHeightData[m_SaveMapHeightData.size ()-1];
		
		for( size_t i = 0 ; i < pEditor->m_HeightMap.m_vHeight.size () ; i++)
		{
			pEditor->m_HeightMap.m_vVec[i].y = pEditor->m_HeightMap.m_vHeight[i];
		}

		UpdataNormalMap( pEditor );

	//	UpdateHeightMapToTileHeight( pEditor );

		UpdateHeightMapToAllTileHeight( pEditor );		
		m_SaveMapHeightData[m_SaveMapHeightData.size ()-1].clear();
		m_SaveMapHeightData.pop_back ();
	}
};

bool MModifyHeightProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	if( pEditor->m_EditOption.mCurReduceMode ==  NO_REDUCE)
	{
		if( event->mouseEven == MouseLDown )
		{
			pEditor->m_EditOption.bBeingModifyHeight = true;
			pEditor->m_bBeingRamp = true;

			pEditor->SelectCurPos ( event->dir , event->pos );

			pEditor->m_saveSelectInfo = pEditor->m_curSelectInfo;
			pEditor->m_SaveModifyVecInfo.clear ();

			SaveData( pEditor );
		}
		else if( event->mouseEven == MouseMove )
		{		
			if( event->bMouseDown )
			{
				pEditor->m_EditOption.bBeingModifyHeight = true;
				pEditor->SelectCurPos ( event->dir , event->pos );
			}
			else
			{		
				pEditor->SelectCurPos ( event->dir , event->pos );
				ModifyTerrainHeight( pEditor , 10 , 1);
			}
		}
		else if ( event->mouseEven == MouseLUp )
		{
			pEditor->m_bUpdateHeightmap = true;

			pEditor->m_EditOption.bBeingModifyHeight = false;
			pEditor->m_SaveLevelOffInfo.bCount = false;

			if( pEditor->m_EditOption.mCurChangeHeightMode == RAMP_MODE )
			{
				CreatRampMap( pEditor );
			}
		}
	}
	else
	{
		if( event->mouseEven == MouseMove && !event->bMouseDown )
		{
			pEditor->SelectCurPos ( event->dir , event->pos );
			ModifyTerrainHeight( pEditor,10 , 1);
		}

		if( event->mouseEven == MouseLDown ||
			(event->mouseEven == MouseMove && event->bMouseDown))
		{		
			pEditor->SelectCurPos ( event->dir , event->pos );
			ModifyTerrainHeight( pEditor,10 , 2);
		}

	}        

	if( event->mouseEven == MouseSewheel)
	{ 	
		g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
	}

	return true;
}

void  MModifyHeightProcess::ModifyTerrainHeight( MTerrainEdit* pEditor , int height,int mode)
{
    int mingx,maxgx,mingy,maxgy;
	int gw = 16;
	int gh = 16;
	if(g_Terrain.GetTitle(0))
	{
		gw = g_Terrain.GetTitle(0)->GetHeadData()->gridX;
		gh = g_Terrain.GetTitle(0)->GetHeadData()->gridY;
	}

	mingx = pEditor->m_curSelectInfo.gx - 1;
	if(mingx < 0 ) mingx = 0;
	maxgx = pEditor->m_curSelectInfo.gx + 1;
	if(maxgx >= gw ) maxgx = gw-1;
	mingy = pEditor->m_curSelectInfo.gy - 1;
	if(mingy < 0 ) mingy = 0;
	maxgy = pEditor->m_curSelectInfo.gy + 1;
	if(maxgy >=gh ) maxgy = gh-1;

	if(pEditor->m_pSignSet)	pEditor->m_pSignSet->m_PosSet.clear ();
   
	           
	pEditor->m_ChangeNormalSet.clear ();

	float height1 = (pEditor->m_EditOption.nCurMoveY ) * pEditor->m_EditOption.fCurHeightScale ;


	if ( pEditor->m_EditOption.mCurReduceMode == BEGIN_REDUCE
			||  pEditor->m_EditOption.mCurReduceMode == REDUCE_RESUME )

	{
		for( int i = mingy ; i <= maxgy ; i++)
		{
			for(int j = mingx ;  j <= maxgx ; j ++ )
			{
				ModifyChunkHeightEx(pEditor, j, i ,height1  ,mode);
			}
		}
	}
	else
	{
		ModifyChunkHeight( pEditor, height1 , mode );		
	}

}
bool  MModifyHeightProcess::AddSmoothHeightPos(  MTerrainEdit* pEditor ,int index)
{
	for( size_t i = 0 ; i < pEditor->m_SaveSmoothPos.size () ; i++)
	{
		if( pEditor->m_SaveSmoothPos[i] == index )
			return false;
	}

	pEditor->m_SaveSmoothPos.push_back ( index );
	return true;
};

void MModifyHeightProcess::ModifyChunkHeight ( MTerrainEdit* pEditor , float height,int mode)
{	
	if( !pEditor->GetEditData() )
		return ;	
	
	if( mode == 1)
		return;

	if( pEditor->m_EditOption.mCurChangeHeightMode == RAMP_MODE )
	{
		pEditor->m_pCampSign->SetPos ( pEditor->m_saveSelectInfo.pos ,pEditor->m_curSelectInfo.pos , pEditor->m_EditOption.nCurChangeHeightSize*( TERRBLOCK_SIZE / (pEditor->m_HeightMap.GerGridNum()*1.0f) ) );
		return;
	}
	
	pEditor->m_SaveReCountNormal.clear ();
	pEditor->m_SaveSmoothPos.clear ();

	pEditor->m_EditOption.nCurChangeHeightSize ;

	int mapIndexX = pEditor->m_curSelectInfo.gx * pEditor->m_HeightMap.GerGridNum() + pEditor->m_curSelectInfo.vx  ;
	int mapIndexY = pEditor->m_curSelectInfo.gy * pEditor->m_HeightMap.GerGridNum() + pEditor->m_curSelectInfo.vy  ;
	
	int mapIndexMinX = mapIndexX - pEditor->m_EditOption.nCurChangeHeightSize ;
	if( mapIndexMinX < 0 ) mapIndexMinX = 0;
	int mapIndexMaxX = mapIndexX + pEditor->m_EditOption.nCurChangeHeightSize ;
	if( mapIndexMaxX >= pEditor->m_HeightMap.GetMapWidth() )mapIndexMaxX =  pEditor->m_HeightMap.GetMapWidth() - 1;

	int mapIndexMinY = mapIndexY - pEditor->m_EditOption.nCurChangeHeightSize ;
	if( mapIndexMinY < 0 ) mapIndexMinY = 0;
	int mapIndexMaxY = mapIndexY + pEditor->m_EditOption.nCurChangeHeightSize ;
	if( mapIndexMaxY >= pEditor->m_HeightMap.GetMapWidth() )mapIndexMaxY = pEditor->m_HeightMap.GetMapWidth() -1 ;


	MVec2  minPos;
	minPos.x = pEditor->GetEditData()->box.minpos.x ;
	minPos.y = pEditor->GetEditData()->box.minpos.z ;


	int  lengthSqr = pEditor->m_EditOption.nCurChangeHeightSize*pEditor->m_EditOption.nCurChangeHeightSize ;
	float _length = lengthSqr*( TERRBLOCK_SIZE / (pEditor->m_HeightMap.GerGridNum()*1.0f) );

	MVec2  _posSet ;
	_posSet.x = pEditor->m_curSelectInfo.pos .x ;
	_posSet.y = pEditor->m_curSelectInfo.pos . z;


	float  saveHeight = 0;
	int    saveHeightNum = 0;

	for( int i = mapIndexMinY ; i < mapIndexMaxY ; i++)
	{
		for( int j = mapIndexMinX ; j < mapIndexMaxX ; j++ )
		{
			//目前高度图统一 , width = height;
			if( i == pEditor->m_HeightMap.GetMapWidth()-1 || j == pEditor->m_HeightMap.GetMapWidth()-1 || i == 0 || j == 0)
				continue;

			int mmx , mmy ;
			mmx = (j - mapIndexX ) * ( j - mapIndexX );
			mmy = (i - mapIndexY ) * ( i - mapIndexY );

			float  fx = minPos.x + j*( TERRBLOCK_SIZE / (pEditor->m_HeightMap.GerGridNum()*1.0f) ) ;
			float  fy = minPos.y + i*( TERRBLOCK_SIZE / (pEditor->m_HeightMap.GerGridNum()*1.0f) ) ; 
           
			MVec2 myPos ;
			myPos.x = fx;
			myPos.y = fy;

			myPos = myPos - _posSet ;

			float  dPos = myPos.lengthSqr ();
			//if( dPos <= _length )
			if( ( mmx + mmy ) <= lengthSqr )
			{
				switch( pEditor->m_EditOption.mCurChangeHeightMode )
			   {		
				case LEVEL_OFF:
					{
						if( pEditor->m_SaveModifyVecInfo.empty ())
						{
							saveHeight += pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j];
							saveHeightNum ++;
						}
						else
						{
							pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j] = pEditor->m_SaveModifyVecInfo[0].height;
						}
					}
					break;
				case HARD_MODE:
					{
						pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j] += height ; 	
					}
					break;
				case SOFT_MODE:
					{
						pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j] += height*cos( (mmx+mmy)/(lengthSqr*1.0f) * MAY_PI/2 ) ; 
					}
				  break;
				case RAMP_MODE:
					{

					}
					break;
				case DITCH_MODE:
					{
						pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j] = m_SaveMapHeightData[m_SaveMapHeightData.size () - 1][i*pEditor->m_HeightMap.GetMapWidth()+j] + ( pEditor->m_EditOption.fCurHeightScale*3000 - 50000 ) / 10.0f;
						AddSmoothHeightPos( pEditor , i*pEditor->m_HeightMap.GetMapWidth() + j );
					}
					break;
				case SMOOTH_MODE:
					{
						float  allHeight[4];
						if(i-1<0)
						{
							allHeight[0] = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j];
						}
						else
						{
							allHeight[0] = pEditor->m_HeightMap.m_vHeight[(i-1)*pEditor->m_HeightMap.GetMapWidth()+j];
						}
						if(i+1>(pEditor->m_HeightMap.GetMapWidth()-1))
						{
							allHeight[1] = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j];
						}
						else
						{
							allHeight[1] = pEditor->m_HeightMap.m_vHeight[(i+1)*pEditor->m_HeightMap.GetMapWidth()+j];
						}
						if(j-1<0)
						{
							allHeight[2] = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j];
						}
						else
						{
							allHeight[2] = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j-1];
						}
						if(j+1>( pEditor->m_HeightMap.GetMapWidth()-1))
						{
							allHeight[3] = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j];
						}
						else
						{
							allHeight[3] = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j+1];
						}

						pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j] = ( pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth()+j] + allHeight[0] + allHeight[1]+
							allHeight[2]+allHeight[3] ) / 5.0f;

					}
					break;
			  }
			  pEditor->m_HeightMap.m_vVec[i*pEditor->m_HeightMap.GetMapWidth()+j].y = pEditor->m_HeightMap.m_vHeight[i*pEditor->m_HeightMap.GetMapWidth() + j];			  
			  pEditor->m_SaveReCountNormal.push_back ( i*pEditor->m_HeightMap.GetMapWidth()+j );
			}
		}
	}


	if( pEditor->m_EditOption.mCurChangeHeightMode == LEVEL_OFF && 
		pEditor->m_SaveModifyVecInfo.empty () )
	{
		MTerrainEdit::SaveModifyVecInfo  saveInfo;
		saveInfo.height = saveHeight / (saveHeightNum*1.0f) ;
		pEditor->m_SaveModifyVecInfo.push_back ( saveInfo );

		return;
	}

	if( !pEditor->m_SaveReCountNormal.empty () )
	{
		ReCountNormal( pEditor );
	}


	//如果有需要平滑的点，则进行平滑处理
	if( !pEditor->m_SaveSmoothPos.empty () )
	{
		SmoothHeightMap( pEditor );
	}

	int mingx,maxgx,mingy,maxgy;

	mingx = pEditor->m_curSelectInfo.gx - 2;
	if(mingx < 0 ) mingx = 0;
	maxgx = pEditor->m_curSelectInfo.gx + 2;
	if(maxgx >= pEditor->GetEditData()->gw) maxgx = pEditor->GetEditData()->gw-1;
	mingy = pEditor->m_curSelectInfo.gy - 2;
	if(mingy < 0 ) mingy = 0;
	maxgy = pEditor->m_curSelectInfo.gy + 2;
	if(maxgy >= pEditor->GetEditData()->gh) maxgy = pEditor->GetEditData()->gh-1;

	MTitle  *pTile = g_Terrain.GetTitle ( 0 );
	if( pTile == NULL )
		return;

	for( int i = mingy ; i <= maxgy ; i++)
	{
		for(int j = mingx ;  j <= maxgx ; j ++ )
		{
			pTile->GetTileData ()->chunkSet [i][j].GetChunkOptionData ()->bChangeHeight = 1;
			pTile->GetTileData ()->chunkSet [i][j].UpdateQuadPath ();
		}
	}

	UpdateHeightMapToTileHeight( pEditor );
}

void MModifyHeightProcess::SmoothHeightMap( MTerrainEdit* pEditor )
{
	std::vector <int>  saveSmoothPos;
	int pos[4];
	for( size_t i = 0 ; i < pEditor->m_SaveSmoothPos.size () ; i ++)
	{
		pos[0] = pEditor->m_SaveSmoothPos[i] + 1;
		pos[1] = pEditor->m_SaveSmoothPos[i] - 1;
		pos[2] = pEditor->m_SaveSmoothPos[i] + pEditor->m_HeightMap.GetMapWidth();
		pos[3] = pEditor->m_SaveSmoothPos[i] - pEditor->m_HeightMap.GetMapWidth();
		
		int nedge = 0;
		for( int j = 0 ; j < 4 ; j++)
		{
			bool _bfind = false;

			for( size_t l = 0 ; l < pEditor->m_SaveSmoothPos.size () ; l++)
			{
				if( pEditor->m_SaveSmoothPos[l] == pos[j] )
				{
					_bfind = true;
					break;
				}
			}
			
			if( !_bfind )
			{
				bool bfind = false;
				for( size_t k = 0 ; k < saveSmoothPos.size () ; k ++)
				{
					if( saveSmoothPos[k] == pos[j] )
					{
						bfind = true;
						break;
					}
				}
				if( !bfind )
				{
					saveSmoothPos.push_back ( pos[j] ); 
				}
			}
			else
			{
				nedge++;
			}
		}
		if( nedge != 4)
		{
			saveSmoothPos.push_back ( pEditor->m_SaveSmoothPos[i] ); 
		}
	}

	for( size_t i = 0 ; i < saveSmoothPos.size () ; i++)
	{
		int _index = saveSmoothPos[i];

		pEditor->m_HeightMap.m_vHeight[_index] = ( pEditor->m_HeightMap.m_vHeight[_index] + 
			pEditor->m_HeightMap.m_vHeight[_index+1] + pEditor->m_HeightMap.m_vHeight[_index-1] +
			pEditor->m_HeightMap.m_vHeight[_index+pEditor->m_HeightMap.GetMapWidth()] + pEditor->m_HeightMap.m_vHeight[_index-pEditor->m_HeightMap.GetMapWidth()] ) / 5.0f;
       
		pEditor->m_HeightMap.m_vVec[_index].y = pEditor->m_HeightMap.m_vHeight[_index] ;
	}
}

void  MModifyHeightProcess::UpdateHeightMapToTileHeight(  MTerrainEdit* pEditor )
{
	MTitle*  pTile = g_Terrain.GetTitle ( 0 );
	if( pTile == NULL )
		return;

	int p = 0;		
	MChunk* pchunk = &pTile->GetTileData ()->chunkSet [0][0];
	
	{
		for( int i = 0 ; i < pTile->GetTileData()->gh ; i++)
		{
			for ( int j = 0 ; j < pTile->GetTileData()->gw ; j++)
			{
				pchunk = &pTile->GetTileData ()->chunkSet [i][j];
				TerrBlockVertTable* pVecData = pchunk->GetChunkNewVecData();

				if( pchunk->GetChunkOptionData ()->bChangeHeight != 0)
				{
					bool  bChange = false;
					for( size_t k = 0 ; k < pVecData->vVer.size() ; k ++)
					{								
						int mapIndex = i*pEditor->m_HeightMap.GerGridNum()*pEditor->m_HeightMap.GetMapWidth() + 
							j*pEditor->m_HeightMap.GerGridNum() + ( k / ( pEditor->m_HeightMap.GerGridNum()+1) )*pEditor->m_HeightMap.GetMapWidth() + k%( pEditor->m_HeightMap.GerGridNum()+1) ;
						
						if ( pVecData->vVer[k].pos .y != pEditor->m_HeightMap.m_vHeight [mapIndex] )
						{					
							bChange = true;
						}

						pVecData->vVer[k].pos .y = pEditor->m_HeightMap.m_vHeight [mapIndex] ;
						pVecData->vVer[k].normal = pEditor->m_HeightMap.m_vNormal[ mapIndex ] ;
					}

					if ( bChange )
					{
						pchunk->UpdateRenderHeight ();
						pchunk->UpdatePlantHeight();
						
						int nlinkgrid = pEditor->GetAutoLinkMesh()->getBlockLinkGrid(); //一个block里面有多少个linkmesh的格子
						pEditor->GetAutoLinkMesh()->UpdatePos( nlinkgrid*j, nlinkgrid*i , nlinkgrid*j+nlinkgrid , nlinkgrid*i+nlinkgrid);				
				
						pEditor->GetAutoLinkTex()->UpdatePos( nlinkgrid*j , nlinkgrid*i , nlinkgrid*j+nlinkgrid , nlinkgrid*i+nlinkgrid);									
						pEditor->GetPaintWallPaper()->UpdatePos( nlinkgrid*j , nlinkgrid*i , nlinkgrid*j+nlinkgrid , nlinkgrid*i+nlinkgrid);	
	
					}
					pchunk->GetChunkOptionData ()->bChangeHeight = 0;
				}
			}
		}

		pEditor->UpdateBreaHeight();
	}
};


void MModifyHeightProcess::ModifyChunkHeightEx( MTerrainEdit* pEditor , int gx,int gz , float height,int mode)
{	
	if( ! pEditor->GetEditData() )
		return ;	

	if( mode == 1)
		return ;

	if( pEditor->GetEditData()->chunkSet[gz][gx].GetChunkData ()->vecNum != 145 )
	{
		pEditor->SetChunkGridVisible( gx ,gz );
		return;
	}

	return;	
}

void  MModifyHeightProcess::ReCountNormal( MTerrainEdit* pEditor )
{
	for( size_t i = 0 ; i < pEditor->m_SaveReCountNormal.size () ; i++)
	{
		int index = pEditor->m_SaveReCountNormal[i] ;
		pEditor->m_HeightMap.UpdateNormal( index );			
	}
}

void MModifyHeightProcess::CreatRampMap( MTerrainEdit* pEditor )
{
	pEditor->m_SaveSmoothPos.clear ();

	pEditor->m_bBeingRamp = false;
	
	int mapIndexX = pEditor->m_curSelectInfo.gx * pEditor->m_HeightMap.GerGridNum() + pEditor->m_curSelectInfo.vx  ;
	int mapIndexY = pEditor->m_curSelectInfo.gy * pEditor->m_HeightMap.GerGridNum() + pEditor->m_curSelectInfo.vy  ;
	int mapSaveIndexX = pEditor->m_saveSelectInfo.gx * pEditor->m_HeightMap.GerGridNum() + pEditor->m_saveSelectInfo.vx  ;
	int mapSaveIndexY = pEditor->m_saveSelectInfo.gy * pEditor->m_HeightMap.GerGridNum() + pEditor->m_saveSelectInfo.vy  ;

	float  height[2] ;
	height[0] = pEditor->m_HeightMap.m_vHeight[mapIndexY*pEditor->m_HeightMap.GetMapWidth() + mapIndexX];
	height[1] = pEditor->m_HeightMap.m_vHeight[mapSaveIndexY*pEditor->m_HeightMap.GetMapWidth() + mapSaveIndexX];
	float  maxheight =  height[0] - height[1];

	int   dx = mapIndexX - mapSaveIndexX;
	int   dy = mapIndexY - mapSaveIndexY;

	float faddx;
	float faddy;


	MVec3   pos0 = pEditor->m_curSelectInfo.pos  , pos1 = pEditor->m_saveSelectInfo.pos  ;
	if( pos0.y > pos1.y )
	{
		pos0 = pEditor->m_saveSelectInfo.pos ;
		pos1 = pEditor->m_curSelectInfo.pos ;
	}
	MVec2 _pos0,_pos1;
	_pos0.x = pos0.x ;
	_pos0.y = pos0.z ;
	_pos1.x = pos1.x ;
	_pos1.y = pos1.z ;
	MVec2 length_0 = _pos1 - _pos0;
	float maxlength = length_0.length();

	
	if( abs(dx) > abs(dy) )
	{
		if( dx > 0 )
		{
			faddx = 1.0f;
			faddy = dy/(abs(dx)*1.0f);
		}
		else
		{
			faddx = -1.0f;
			faddy = dy/(abs(dx)*1.0f);
		}
		
		float minheight = maxheight / dx;

		std::vector <int > saveX,saveY;

		for(int j = 0 ; j < pEditor->m_EditOption.nCurChangeHeightSize ; j++)
		{
			bool  bfind1 = false , bfind2 = false;
			bool  bfind3 = false , bfind4 = false;

			int sx1 = mapSaveIndexX - (int)( j*faddy );
			int sy1 = mapSaveIndexY +  (int)( j*faddx );
			int sx2 = mapSaveIndexX +  (int)( j*faddy );
			int sy2 = mapSaveIndexY -  (int)( j*faddx );

			int sx3 = mapSaveIndexX - (int)(j*faddy) + 1 ;
			int sy3 = mapSaveIndexY + (int)(j*faddx) + 1;
			int sx4 = mapSaveIndexX + (int)(j*faddy) - 1;
			int sy4 = mapSaveIndexY - (int)(j*faddx) - 1;

			for( size_t i = 0 ; i < saveX.size () ; i ++ )
			{
				if( saveX[i] == sx1 && saveY[i] == sy1 )
					bfind1 = true;
				if( saveX[i] == sx2 && saveY[i] == sy2 )
					bfind2 = true;
				if( saveX[i] == sx3 && saveY[i] == sy3 )
					bfind3 = true;
				if( saveX[i] == sx4 && saveY[i] == sy4 )
					bfind4 = true;
			}
            if( !bfind1 )
			{
				saveX.push_back ( sx1 );saveY.push_back ( sy1 );
			}
			if( !bfind2 )
			{
				saveX.push_back ( sx2 );saveY.push_back ( sy2 );
			}
			if( !bfind3 )
			{
				saveX.push_back ( sx3 );saveY.push_back ( sy3 );
			}

			if( sx3 == sx4 && sy3 == sy4)
				continue;

			if( !bfind4 )
			{
				saveX.push_back ( sx4 );saveY.push_back ( sy4 );
			}
		}

        for( size_t l = 0 ; l < saveX.size () ; l++)
		{
			for( int i = 0 ; i < abs(dx) ; i++)
			{
				int index = saveX[l] + int(i*faddx) + ( saveY[l] + int(i*faddy) )*pEditor->m_HeightMap.GetMapWidth();
				pEditor->m_HeightMap.m_vHeight[index] =  GetRampHeight( pos0, pos1 , pEditor->m_HeightMap.m_vVec[index] , abs(maxheight) , maxlength );
				pEditor->m_HeightMap.m_vVec[index].y = pEditor->m_HeightMap.m_vHeight[index] ;

				AddSmoothHeightPos( pEditor , index );
			}
		}
		saveX.clear ();
		saveY.clear ();
	}
	else
	{
		if( dy > 0 )
		{
			faddy = 1.0f;
			faddx = dx/(abs(dy)*1.0f);
		}
		else
		{
			faddy = -1.0f;
			faddx = dx/(abs(dy)*1.0f);
		}

		float minheight = maxheight / dy;

		std::vector <int > saveX,saveY;

		for(int j = 0 ; j < pEditor->m_EditOption.nCurChangeHeightSize ; j++)
		{
			bool  bfind1 = false , bfind2 = false;
			bool  bfind3 = false , bfind4 = false;

			int sx1 = mapSaveIndexX - (int)(j*faddy) ;
			int sy1 = mapSaveIndexY + (int)(j*faddx) ;
			int sx2 = mapSaveIndexX + (int)(j*faddy) ;
			int sy2 = mapSaveIndexY - (int)(j*faddx) ;

			int sx3 = mapSaveIndexX - (int)(j*faddy) + 1;
			int sy3 = mapSaveIndexY + (int)(j*faddx) + 1;
			int sx4 = mapSaveIndexX + (int)(j*faddy) - 1;
			int sy4 = mapSaveIndexY - (int)(j*faddx) - 1;

			for( size_t i = 0 ; i < saveX.size () ; i ++ )
			{
				if( saveX[i] == sx1 && saveY[i] == sy1 )
					bfind1 = true;
				if( saveX[i] == sx2 && saveY[i] == sy2 )
					bfind2 = true;
				if( saveX[i] == sx3 && saveY[i] == sy3 )
					bfind3 = true;
				if( saveX[i] == sx4 && saveY[i] == sy4 )
					bfind4 = true;
			}
            if( !bfind1 )
			{
				saveX.push_back ( sx1 );saveY.push_back ( sy1 );
			}

			if( sx1 == sx2 && sy1 == sy2)
				continue;

			if( !bfind2 )
			{
				saveX.push_back ( sx2 );saveY.push_back ( sy2 );
			}
			
			if( sx2 == sx3 && sy2 == sy3)
				continue;

			if( !bfind3 )
			{
				saveX.push_back ( sx3 );saveY.push_back ( sy3 );
			}

			if( sx3 == sx4 && sy3 == sy4)
				continue;

			if( !bfind4 )
			{
				saveX.push_back ( sx4 );saveY.push_back ( sy4 );
			}
			
		}

        for( size_t l = 0 ; l < saveX.size () ; l++)
		{
			for( int i = 0 ; i < abs(dy) ; i++)
			{
				int index = saveX[l] + int(i*faddx) + ( saveY[l] + int(i*faddy) )*pEditor->m_HeightMap.GetMapWidth();
				pEditor->m_HeightMap.m_vHeight[index] = GetRampHeight( pos0, pos1 , pEditor->m_HeightMap.m_vVec[index] , abs(maxheight) , maxlength );
				pEditor->m_HeightMap.m_vVec[index].y = pEditor->m_HeightMap.m_vHeight[index] ;

				AddSmoothHeightPos( pEditor , index );
			}
		}

		saveX.clear ();
		saveY.clear ();
	}


	SmoothHeightMap( pEditor );
	
	MTitle  *pTile = g_Terrain.GetTitle ( 0 );

	for( int i = 0 ; i < pTile->GetTileData()->gh ; i++)
	{
		for(int j = 0 ;  j < pTile->GetTileData()->gw ; j ++ )
		{
			pTile->GetTileData ()->chunkSet [i][j].GetChunkOptionData ()->bChangeHeight = 1;
		}
	}

    UpdataNormalMap( pEditor );
	UpdateHeightMapToTileHeight( pEditor );
}

void  MModifyHeightProcess::UpdataNormalMap( MTerrainEdit* pEditor )
{
	pEditor->m_HeightMap.CountNormal();
}


bool MModifyHeightProcess::SaveCurHeightMap(  MTerrainEdit* pEditor ,const char* filename)
{
	MStream stream;

	if( !stream.WriteFile ( filename ))
		return false;
		
	BITMAPFILEHEADER header;	
	int num = 128*128  ;
	header.bfSize = num + 1078;
	header.bfType = (WORD)19778;
	header.bfOffBits = (DWORD)1078;//灰度图，所以便宜为 256*4 ,
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;

	BITMAPINFOHEADER  headInfo;

	headInfo.biWidth = 128;
	headInfo.biHeight = 128  ;
	headInfo.biBitCount = 8;
	headInfo.biSize = sizeof( BITMAPINFOHEADER );
    headInfo.biSizeImage = pEditor->m_HeightMap.m_vHeight.size ()  ;
	headInfo.biClrUsed = 0;
	headInfo.biXPelsPerMeter = 0;
	headInfo.biYPelsPerMeter = 0;
	headInfo.biClrImportant = 0;
	headInfo.biCompression = 0;
	headInfo.biPlanes = 1;

	stream.Write ( header );
	stream.Write ( headInfo );

	unsigned char data;
	for( int i = 0 ; i < 256 ; i++)
	{
		data = i;
		stream.WriteUnChar ( &data );
		stream.WriteUnChar ( &data );
		stream.WriteUnChar ( &data );
		data = 0xff;
		stream.WriteUnChar ( &data );
	}

	unsigned char color;
	for( size_t i = 0 ; i < pEditor->m_HeightMap.m_vHeight.size () ; i ++)
	{
		color = unsigned char(pEditor->m_HeightMap.m_vHeight[i] / 50.0f) ;
		stream.WriteUnChar( &color );
	}

	stream.WriteEnd ();

	return true;

}


void MModifyHeightProcess::SetHeightMap(  MTerrainEdit* pEditor ,int width,int height , unsigned char  *heightmap )
{
	MTitle*  pTile = g_Terrain.GetTitle ( 0 );
	if( pTile == NULL )
		return;

	if( width != pEditor->m_HeightMap.GetMapWidth() )
		return;

	if( height != pEditor->m_HeightMap.GetMapHeight() )
		return;

	pEditor->m_HeightMap.m_vHeight.clear ();	
	pEditor->m_HeightMap.m_vHeight.resize( width*height );

	for( int i = 0 ; i < height ; i ++)
	{
		for( int j = 0 ; j < width ; j ++ )
		{
			pEditor->m_HeightMap.m_vHeight[i*width + j ] = (float) ( 50*heightmap[i*width + j] );
			pEditor->m_HeightMap.m_vVec[i*width+j].y = (float)(  50*heightmap[i*width + j] );
		}
	}

	UpdataNormalMap( pEditor );
	UpdateHeightMapToAllTileHeight( pEditor ); 
}

void  MModifyHeightProcess::UpdateHeightMapToAllTileHeight(  MTerrainEdit* pEditor )
{
	MTitle*  pTile = g_Terrain.GetTitle ( 0 );
	if( pTile == NULL )
		return;

	for( int i = 0 ; i < pTile->GetTileHead()->gridY ; i++)
	{
		for ( int j = 0 ; j < pTile->GetTileHead()->gridX ; j++)
		{	
			MChunk* pchunk = &pTile->GetTileData ()->chunkSet [i][j];

			bool  bUpdate = false;


			{
				
				for ( size_t k = 0 ; k < pchunk->GetChunkNewVecData()->vVer.size() ; k ++ )
				{
					int nSize = pchunk->GetChunkNewVecData()->ngrid ;
					size_t mapIndex = i*nSize*(pTile->GetTileData ()->gh*nSize + 1) + j*nSize + ( k / (nSize+1) )*(pTile->GetTileData ()->gw*nSize+1) + k % (nSize+1) ;
									

					if ( pchunk->GetChunkNewVecData()->vVer[k].pos .y != pEditor->m_HeightMap.m_vHeight[ mapIndex ] )
					{
						bUpdate = true ;
					}

						pchunk->GetChunkNewVecData()->vVer[k].pos .y = pEditor->m_HeightMap.m_vHeight[ mapIndex ] ;
						pchunk->GetChunkNewVecData()->vVer[k].normal = pEditor->m_HeightMap.m_vNormal[ mapIndex ] ;

						if(  pEditor->m_HeightMap.m_vHeight[mapIndex] < pchunk->GetChunkData()->minVec .y )
							pchunk->GetChunkData ()->minVec .y =  pEditor->m_HeightMap.m_vHeight[mapIndex] ;

						if(  pEditor->m_HeightMap.m_vHeight[mapIndex] > pchunk->GetChunkData()->maxVec.y )
							pchunk->GetChunkData ()->maxVec .y =  pEditor->m_HeightMap.m_vHeight[mapIndex] ;                    
				}
				
				pchunk->m_AABBox .setRange ( pchunk->GetChunkData ()->minVec , pchunk->GetChunkData ()->maxVec );
			}
		
			if ( bUpdate )
			{
		
				pchunk->UpdateRenderHeight ();
				pchunk->UpdateQuadPath ();		
				pchunk->UpdatePlantHeight();

				int nlinkgrid = pEditor->GetAutoLinkMesh()->getBlockLinkGrid(); //一个block里面有多少个linkmesh的格子

				pEditor->GetAutoLinkMesh()->UpdatePos( nlinkgrid*j, nlinkgrid*i , nlinkgrid*j+nlinkgrid , nlinkgrid*i+nlinkgrid);				
				pEditor->GetAutoLinkTex()->UpdatePos( nlinkgrid*j , nlinkgrid*i , nlinkgrid*j+nlinkgrid , nlinkgrid*i+nlinkgrid);									
				pEditor->GetPaintWallPaper()->UpdatePos( nlinkgrid*j , nlinkgrid*i , nlinkgrid*j+nlinkgrid , nlinkgrid*i+nlinkgrid);	
			}

		}
	}
}

void  MModifyHeightProcess::UpdateTileHeightDataToHeightMap( MTerrainEdit* pEditor )
{
	MTitle*  pTile = g_Terrain.GetTitle ( 0 );
	if( pTile == NULL )
		return;

	{
		TerrBlockVertTable* pVecData = pEditor->GetEditData()->chunkSet[0][0].GetChunkNewVecData();
		MVec3 pos ;
		g_Terrain.GetGridPos( &pos , 0 , 0 );
		float fSize = TERRBLOCK_SIZE / ( pVecData->ngrid *1.0f );

		MTileHead *phead = pTile->GetHeadData();
		pEditor->m_HeightMap.CreateMap(MPoint2D(phead->gridX, phead->gridY), MPoint2D(phead->gridVecNum, phead->gridVecNum), pos, fSize , 100.0f );

		for( int i = 0  ; i < phead->gridY; i ++ )
		{
			for ( int j = 0 ; j < phead->gridX ; j ++ )
			{
				pVecData = pEditor->GetEditData()->chunkSet[i][j].GetChunkNewVecData() ;

				int index ;
				for( size_t k = 0 ; k < pVecData->vVer.size() ; k ++ )
				{
					index = i*pVecData->ngrid*(phead->gridX*pVecData->ngrid+1) + j*pVecData->ngrid + ( k / (pVecData->ngrid +1 )*(phead->gridX*pVecData->ngrid+1) ) + k % (pVecData->ngrid + 1);
					pEditor->m_HeightMap.m_vHeight[ index ] = pVecData->vVer[k].pos .y ; 
					pEditor->m_HeightMap.m_vNormal[ index ] = pVecData->vVer[k].normal ;
					pEditor->m_HeightMap.m_vVec[ index ] = pVecData->vVer[k].pos ;
				}
			}
		}
	}
};

void  MModifyHeightProcess::CreateHeightMapEX(  MTerrainEdit* pEditor, MPoint2D nblock, MPoint2D ngrid, int minheight ,int maxheight, int randsize,int smoothNum)
{
    MVec3 startPos(0,0,0);

	pEditor->m_HeightMap.CreateMap(nblock, ngrid, startPos, 1.0f , (float)minheight);
}

void  MModifyHeightProcess::SmoothHeightMap( MTerrainEdit* pEditor ,int width ,int height )
{
	pEditor->m_HeightMap.SmoothHeightMapData();
}



