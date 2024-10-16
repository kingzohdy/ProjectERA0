#include ".\mpainttextureprocess.h"
#include "EditorCamera.h"

#include "mterrain.h"
#include "mtexmanager.h"
#include "mstream.h"
#include "plant.h"
#include "minputprocess.h"
#include "MSign.h"
#include "LinkTile.h"

#include "OgrePixelFormat.h"
#include "OgreTexture.h"
#include "OgreLog.h"
#include "OgreTerrainData.h"

using namespace TE;
using namespace Ogre;
extern float g_fTileSize;
extern bool  g_bUseOtherCamera;

extern BITMAPFILEHEADER g_header;		
extern BITMAPINFOHEADER  g_headInfo;

MPaintTextureProcess::MPaintTextureProcess()
{
     m_nProcessType = PANIT_EVN;
}

MPaintTextureProcess::~MPaintTextureProcess()
{
}


bool MPaintTextureProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	switch( event->mouseEven  )
	{
	case MouseLDown:
		{
			if(!pEditor->m_EditOption.bPickTexture && pEditor->m_EditOption.nCurLayer < LAYER_PLANT )
			{		
				pEditor->AddSaveUndoData();
			}

			if( pEditor->m_EditOption.nCurLayer == LAYER_PLANT )
			{
				pEditor->AddPlant( event->dir , event->pos );
				pEditor->PaintPlant( event->dir , event->pos , 1 );
			}
		}
		break;
	case MouseMove:
		{
			if( event->bMouseDown )
			{
				if( pEditor->m_EditOption.nCurLayer < LAYER_PLANT )
				{
					PaintTexture( pEditor , event->dir ,event->pos ,0, event->mx , event->my );
				}
				else
				{
					pEditor->PaintPlant( event->dir , event->pos);
				}
			}
			else
			{
				PaintTexture( pEditor , event->dir ,event->pos , 1,event->mx , event->my );
			}
		}
		break;
	case MouseLUp:
		{
			if(!pEditor->m_EditOption.bPickTexture && pEditor->m_EditOption.nCurLayer < LAYER_PLANT )
				pEditor->AddSaveRedoData();
		}
		break;
	case MouseSewheel:
		{	
			g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
		}
		break;
	case MouseRDown:
		{
			pEditor->SelectChunkTexInfo( event->dir , event->pos );
			pEditor->m_bOpenTextureInfoDlg = true;
			pEditor->m_bShowTextureInfoDlg = true;
		}
		break;
	}
/*
	if( event->mouseEven == MouseLDown )
	{
		if(!pEditor->m_EditOption.bPickTexture && pEditor->m_EditOption.nCurLayer < 6)
		{		
			pEditor->AddSaveUndoData();
		}

		if( pEditor->m_EditOption.nCurLayer == 6 )
		{
			pEditor->AddPlant( event->dir , event->pos );
			pEditor->PaintPlant( event->dir , event->pos , 1 );
		}
	}

	if( event->bMouseDown )
	{
		if( pEditor->m_EditOption.nCurLayer < 6 )
		{
			PaintTexture( pEditor , event->dir ,event->pos ,0, event->mx , event->my );
		}
		else
		{
			pEditor->PaintPlant( event->dir , event->pos);
		}
	}
	else if( event->mouseEven == MouseMove )
	{
		{
			PaintTexture( pEditor , event->dir ,event->pos , 1,event->mx , event->my );
		}
	}

	if( event->mouseEven == MouseLUp)
	{
		if(!pEditor->m_EditOption.bPickTexture && pEditor->m_EditOption.nCurLayer < 6 )
			pEditor->AddSaveRedoData();
	}
	if( event->mouseEven == MouseSewheel)
	{ 
		g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
	}

	if( event->mouseEven == MouseRDown )
	{
		pEditor->SelectChunkTexInfo( event->dir , event->pos );
		pEditor->m_bOpenTextureInfoDlg = true;
		pEditor->m_bShowTextureInfoDlg = true;
	}*/
	return true;

};


MVec3  MPaintTextureProcess::PaintTexture( int x ,int y )
{
	MVec3  pos ;

	MVec3  centerpos = g_Camera.m_Pos ;

	pos.x = centerpos.x +  ( x - g_Camera.m_ScreenWidth/2 )*( g_fTileSize / (g_Camera.m_ScreenWidth*1.0f) );  
	pos.z = centerpos.z - ( y - g_Camera.m_ScreenHeight/2 )* ( g_fTileSize / (g_Camera.m_ScreenWidth*1.0f) );

	return pos;
};

void MPaintTextureProcess::PaintTexture ( MTerrainEdit* pEditor ,MVec3 &dir, MVec3 &pos ,int mode ,int x,int y)
{
	if( pEditor->GetEditData() == NULL )
		return;

	MVec3 pos0 ;
		
	if( pEditor->m_pSignSet)
	{
		pEditor->m_pSignSet->m_PosSet .clear ();
	}

	if ( g_bUseOtherCamera )
	{
		pos0 = PaintTexture( x ,y );  

		if( !g_Terrain.GetHeight( pos0.x , pos0.z , &pos0.y ) )
			return;
	}

	if( g_Terrain.TestAndRay (dir,pos,pos0) || g_bUseOtherCamera)
	{
		pEditor->m_CurMousePos = pos0 ;

		int gx , gz;
		if( !g_Terrain.GetGridX(pos0.x,&gx))
			return;
		if(! g_Terrain.GetGridZ(pos0.z,&gz))
			return;

		if( pEditor->m_pSignSet)
		{	
			MVec3  _pos = pos0;
			MVec3  wpos[4];
			float _len = pEditor->m_EditOption.nCurSelectPaintRad[pEditor->m_EditOption.nCurLayer-1]*TERRBLOCK_SIZE / 64.0f;
	
			//更新圆
			pEditor->m_pSignSet->UpdateData ( pos0 , _len , 64 );
		}

		if( mode == 1)
			return;

		if( g_MInputProcess.GetUserInput()->bLCtrl )
		{
			pEditor->m_EditOption.bPickTexture = true;
		}
		else
		{
			pEditor->m_EditOption.bPickTexture = false;
		}	

		if( g_MInputProcess.GetUserInput()->bLShift )
		{	
			pEditor->SetCurAlphaMode (ALPHA_DEC);
		}
		else
		{
			pEditor->SetCurAlphaMode (ALPHA_ADD);
		}		

		if( pEditor->m_EditOption.bPickTexture )
		{
			for( int i = 0 ; i < pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texNum ; i++)
			{
				if( pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texInfo [i]!= 255 )
				strcpy( pEditor->m_EditOption.PaintTexInfo[i].texName  , pEditor->GetEditData()->texData .texFileSet [pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texInfo [i] ].c_str ());
			}
			ResetPaintTexture( pEditor );

			return;
		}	
		
		int  mdx,mdz;
		mdx = (int)(pEditor->GetEditData()->chunkSet [gz][gx].GetGridPosX(pos0.x)/TERRBLOCK_SIZE*64);
		mdz = (int)(pEditor->GetEditData()->chunkSet [gz][gx].GetGridPosZ(pos0.z)/TERRBLOCK_SIZE*64);
		int  dx =  mdx-31;
		int  dz =  mdz-31;
		    
		bool  bDraw[3][3];	
		int   sx[3][3],sz[3][3];

		int selectLay = pEditor->m_EditOption.nCurLayer;
		int radius = pEditor->m_EditOption.nCurSelectPaintRad [selectLay-1];
		if( selectLay >0)
		{		
			memset(bDraw, 0, sizeof(bDraw));
			int minx = mdx - radius;
			int minz = mdz - radius;
			int maxx = mdx + radius;
			int maxz = mdz + radius;

			int min_i = minx<0?0:1;
			int min_j = minz<0?0:1;
			int max_i = maxx>=64?2:1;
			int max_j = maxz>=64?2:1;

			for(int j=min_j; j<=max_j; j++)
			{
				for(int i=min_i; i<=max_i; i++)
				{
					bDraw[j][i] = true;
					sx[j][i] = dx - 64*(i-1);
					sz[j][i] = dz - 64*(j-1);
				}
			}

			/*
			if(mdx + pEditor->m_EditOption.nCurSelectPaintRad [selectLay-1]<64
				&&mdx - pEditor->m_EditOption.nCurSelectPaintRad [selectLay-1]>=0
				&&mdz + pEditor->m_EditOption.nCurSelectPaintRad [selectLay-1]<64
				&&mdz - pEditor->m_EditOption.nCurSelectPaintRad [selectLay-1]>=0)
			{
				sx[1][1] = dx ; sz[1][1] = dz;
				bDraw[1][1] = true;
			}
            else
			{
				if(dx<0&&dz<0)
				{
					sx[0][0] = 64+dx; sz[0][0] = 64+dz; 

					sx[0][1] = dx; sz[0][1] = dz+64;
					sx[1][0] = 64+dx; sz[1][0] = dz;
					sx[1][1] = dx   ; sz[1][1] = dz;
					bDraw[0][0] = bDraw[0][1] = bDraw[1][0] = bDraw[1][1] = true;
				}
				else if(dx<0&&dz>0)
				{
					sx[1][0] = 64+dx; sz[1][0] = dz;
					sx[1][1] = dx   ; sz[1][1] = dz;
					sx[2][0] = 64+dx; sz[2][0] = dz-64;
					sx[2][1] = dx   ; sz[2][1] = dz-64;
					bDraw[1][0] = bDraw[1][1] = bDraw[2][0] = bDraw[2][1] = true;
				}
				else if(dx>0&&dz<0)
				{
					sx[0][1] = dx; sz[0][1] = 64+dz;
					sx[0][2] = dx-64; sz[0][2] = 64+dz;
					sx[1][1] = dx   ; sz[1][1] = dz;
					sx[1][2] = dx-64   ; sz[1][2] = dz;
					bDraw[0][1] = bDraw[0][2] = bDraw[1][1] = bDraw[1][2] = true;
				}
				else if(dx>0&&dz>0)
				{
	    			sx[1][1] = dx; sz[1][1] = dz;
					sx[1][2] = dx-64; sz[1][2] = dz;
					sx[2][1] = dx   ; sz[2][1] = dz-64;
					sx[2][2] = dx-64   ; sz[2][2] = dz-64;
					bDraw[1][1] = bDraw[1][2] = bDraw[2][1] = bDraw[2][2] = true;
				}
				else
				{
					 sx[1][1] = dx ; sz[1][1] = dz;
					 bDraw[1][1] = true;
				}
			}*/
		}


		switch(selectLay)
		{
		case LAYER_0:
			{
				pEditor->SaveCurData(gx,gz,0);

				if(pEditor->m_EditOption.nCurSelectTexID == 0)
				{
					pEditor->GetEditData()->chunkSet [gz][gx].GetChunkRenderData ()->terrainTex [0] = g_TexManager.LoadTex (pEditor->GetEditData()->texData .texFileSet [0].c_str ());
					pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texInfo [0] = 0;

					if( pEditor->m_bShowTextureInfoDlg )
					{
						pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texUVInfo [0] = pEditor->m_EditOption.uUVRepeat [0];
					}
				}
				else
				{
					if( (int)pEditor->GetEditData()->texData .texFileSet.size () > pEditor->m_EditOption.nCurSelectTexID )
					{
						pEditor->GetEditData()->chunkSet [gz][gx].GetChunkRenderData ()->terrainTex [0] = g_TexManager.LoadTex (pEditor->GetEditData()->texData .texFileSet [pEditor->m_EditOption.nCurSelectTexID].c_str());
						pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texInfo [0] = pEditor->m_EditOption.nCurSelectTexID ;
					
						if( pEditor->m_bShowTextureInfoDlg )
						{
							pEditor->GetEditData()->chunkSet [gz][gx].GetChunkData ()->texUVInfo [0] = pEditor->m_EditOption.uUVRepeat [0];
						}
					}
				}
			}
		break;
		case LAYER_1:	
		case LAYER_2:
		case LAYER_3:
		case LAYER_SHADOW:
		case LAYER_COLOR:
			{	
				int nblock = pEditor->GetEditData()->gw;
				for(int w = -1 ; w <= 1 ; w++)
				{
					for(int q = -1 ; q <= 1; q++)
					{
						if(bDraw[w+1][q+1])
						{
							if(gx+q>=0&&gx+q<nblock&&gz+w>=0&&gz+w<nblock)
							{
                                pEditor->SaveCurData( gx+q,gz+w ,0);
								ModifyAlphaTex( pEditor , gx+q,gz+w,selectLay-1,sx[w+1][q+1],sz[w+1][q+1],pEditor->m_EditOption.alphaPaintData[selectLay-1].data,pEditor->m_EditOption.nCurSelectTexID);
					
								if( pEditor->m_bShowTextureInfoDlg )
								{
									pEditor->GetEditData()->chunkSet [gz+w][gx+q].GetChunkData ()->texUVInfo [selectLay] = pEditor->m_EditOption.uUVRepeat [selectLay];
								}
							}
						}
					}
				}
			}
			break;
		}
	}
}


void  MPaintTextureProcess::ModifyAlphaTex( MTerrainEdit* pEditor ,int gx,int gz ,int layer,int x,int z,unsigned char* data,int texid)
{
	//注意 layer 已经在外部被减 1
    if( layer < LAYER_0 || layer > LAYER_COLOR )
	{
		return;
	}

	TerrBlockData* pChunkData =  pEditor->GetEditData()->chunkSet[gz][gx].GetChunkData ();
	TerrBlockRenderData* pChunkRenderData = pEditor->GetEditData()->chunkSet[gz][gx].GetChunkRenderData();
     //0 , 1 , 2 为纹理半透明层
	if( layer < LAYER_3 )
	{	
		if(  (int)pEditor->GetEditData()->texData .texFileSet .size () <= texid )
			texid = 0;

		if( pChunkData->texInfo [layer + 1 ] != texid)
		{	
            if( pEditor->m_EditOption.mCurAlphaMode != ALPHA_DEC )
			{
				pChunkData->texInfo [layer + 1 ]  = texid ;
				pChunkRenderData->terrainTex [layer + 1] = g_TexManager.LoadTex  ( pEditor->GetEditData()->texData.texFileSet  [texid].c_str () );
			}
		}	
		
		if( pChunkData->texNum < layer + 2)
		{
			pChunkData->texNum = layer + 2 ;
		}

	   int di =0 ,dj = 0;
	   for(int j = x ; j < x+64 ; j ++)
	   {
		   dj = 0;
		  if(j<0||j>=64)
			  ;
		  else
		  {
			   for(int i = z ; i < z+64 ; i++)
			   {
				  
				  if(i<0||i>=64)
				   {
				   }
				  else
				  {
					  if( pEditor->m_EditOption.bUserPaintPic )
					  {
						if( pEditor->m_EditOption.mCurAlphaMode == ALPHA_ADD)
						{
							if( data[di*64+dj] != 0 )
							{
								unsigned char addcolor = unsigned char(float( pEditor->m_EditOption.nPaintVec/64.0f)*data[di*64+dj]);

								if( pChunkData->alpahMap[layer].data[i*64+j] + addcolor > 255 )
								{
									pChunkData->alpahMap[layer].data[i*64+j] = 255;
								}
								else
								{
									pChunkData->alpahMap[layer].data[i*64+j] += addcolor;
								}
							}
						}
						else if( pEditor->m_EditOption.mCurAlphaMode == ALPHA_DEC )
						{
							if( data[di*64+dj] != 0 )
							{
								unsigned char addcolor = unsigned char( float(pEditor->m_EditOption.nPaintVec/64.0f)*data[di*64+dj] );

								if( pChunkData->alpahMap[layer].data[i*64+j] - addcolor < 0 )
								{
									pChunkData->alpahMap[layer].data[i*64+j] = 0;
								}
								else
								{
									pChunkData->alpahMap[layer].data[i*64+j] -= addcolor;
								}
							}
						}
					  }
					  else
					  {
						if( pEditor->m_EditOption.mCurAlphaMode == ALPHA_ADD)
						{
							if(pChunkData->alpahMap[layer].data[i*64+j] +data[di*64+dj]>255)
							{
								pChunkData->alpahMap[layer].data[i*64+j] = 255;
							}
							else
							{
								pChunkData->alpahMap[layer].data[i*64+j] += data[di*64+dj];
							}
						}
						else if( pEditor->m_EditOption.mCurAlphaMode == ALPHA_DEC )
						{
							if(pChunkData->alpahMap[layer].data[i*64+j] -data[di*64+dj] < 0)
							{
								pChunkData->alpahMap[layer].data[i*64+j] = 0;
							}
							else
							{
								pChunkData->alpahMap[layer].data[i*64+j] -= data[di*64+dj];
							}
						}
					  }
				  }   
				  dj++;
			   }
		  }
		   di ++;
	   }

	   /*
	   if(pChunkRenderData->alphaTex[layer]==NULL)
	   {	  
		  pChunkRenderData->alphaTex[layer] = 
			   g_TexManager.CreateTex( pChunkData->alpahMap[layer].data,64,64,4096,
			   PF_A8);
	   }
	   else
	   {
		   int width = 64,minmap = 0;
		   for(;;)
		   {  
              g_TexManager.Update (  pChunkRenderData->alphaTex[layer] , pChunkData->alpahMap[layer].data ,64,64,minmap); 
			  pChunkRenderData->alphatexture->setPixels(layer, pChunkData->alpahMap[layer].data);
              width = width >> 1;
			  minmap ++;
			  if(width == 0)
				  break;
			   
		   }
	   }*/
		if(pChunkRenderData->alphatexture->getNumChannel() <= layer)
		{
			pChunkRenderData->alphatexture->setNumChannel(layer+1);
			for(int i=0; i<layer+1; i++)
			{
				pChunkRenderData->alphatexture->setPixels(i, pChunkData->alpahMap[i].data);
			}
		}
		else pChunkRenderData->alphatexture->setPixels(layer, pChunkData->alpahMap[layer].data);
	}
	else
	{   //阴影层 
		if( layer == 3 )
		{
			int di =0 ,dj = 0;
			for(int j = x ; j < x+64 ; j ++)
			{
				dj = 0;
				if(j<0||j>=64)
					;
				else
				{
					for(int i = z ; i < z+64 ; i++)
					{
						  
						if(i<0||i>=64)
						{
						}
						else
						{
							if( pEditor->m_EditOption.mCurAlphaMode == ALPHA_ADD)
							{
							if(data[di*64+dj]!=0)
								pChunkData->shadowMap[i*64+j] = data[di*64+dj];
							}
							else
							{
								if(data[di*64+dj]==85)
								pChunkData->shadowMap[i*64+j] = 0;
							}
						}   
						dj++;
					}
				}
				di ++;
			}
			   
			if( !pChunkData->bShadow )
			{
				pChunkData->bShadow = true;
				if( pChunkRenderData->shadowTex == NULL )
				{
					pChunkRenderData->shadowTex  = 
						g_TexManager.CreateTex(NULL,64,64,4096,
						PF_A8);
				}
			}
		          
			int width = 64,minmap = 0;
			
			for(;;)
			{  
				
				g_TexManager.Update (  pChunkRenderData->shadowTex  , pChunkData->shadowMap  ,64,64,minmap); 
				width = width >> 1;	
				minmap ++;
					
				if(width == 0)				
					break;			
			}		
		}
		//颜色层，主要是显示游戏生成的信息，包括阻挡信息
		else if( layer == 4 )
		{
		    //根据修改的颜色，更新各层信息
			TerrBlockLayerInfo *pInfoData =  pEditor->GetEditData()->chunkSet[gz][gx].GetChunkInfo();

			if( !pChunkData->bColor  )
			{
				pChunkData->bColor  = true;

				if( pChunkRenderData->colorTex  == NULL )
				{
					pChunkRenderData->colorTex   = g_TexManager.CreateTex(NULL,64,64,4096,PF_A8R8G8B8, 1);

					memset(  pChunkData->ColorMap , 127 , 4*64*64 );
				}
			}

		    int di =0 ,dj = 0;
			for(int j = x ; j < x+64 ; j ++)
			{
				dj = 0;
				if(j<0||j>=64)
					;
				else
				{
					for(int i = z ; i < z+64 ; i++)
					{
						  
						if(i<0||i>=64)
						{
						}
						else
						{
							if( pEditor->m_EditOption.mCurAlphaMode == ALPHA_ADD)
							{
								if(data[di*64+dj]!=0)
								{ 
									if( pEditor->m_EditOption.bUserPaintPic )
									{
										if( data[di*64+dj] != 0)
										{
											
											if( pChunkData->ColorAlpha [i*64+j] + data[di*64+dj] > 255 )
												pChunkData->ColorAlpha [i*64+j] = 255;
											else
												pChunkData->ColorAlpha [i*64+j] += data[di*64+dj];


											float zz =  pChunkData->ColorAlpha [i*64+j] / 255.0f ;
											if( zz > 1.0f)
												zz = 1.0f;

						                    pChunkData->ColorMap [4*i*64+4*j] =  unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j] + zz*pEditor->m_EditOption.mColorMap[0] );
											pChunkData->ColorMap [4*i*64+4*j+1] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+1]  + zz*pEditor->m_EditOption.mColorMap[1] );
								    		pChunkData->ColorMap [4*i*64+4*j+2] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+2] + zz*pEditor->m_EditOption.mColorMap[2] );
											pChunkData->ColorMap [4*i*64+4*j+3] = 127;

											//增加修改对应的信息层。
											pInfoData->info[pEditor->m_EditOption.gameInfo ][i*64+j] = 1;
										}
										
									}
									else
									{					
										if( data[di*64+dj] != 0 )
										{
								
											if( pChunkData->ColorAlpha [i*64+j] + data[di*64+dj] > 255 )
												pChunkData->ColorAlpha [i*64+j] = 255;
											else
												pChunkData->ColorAlpha [i*64+j] += data[di*64+dj] ;

											
											float zz =  pChunkData->ColorAlpha [i*64+j] / 255.0f ;
												if( zz > 1.0f)
													zz = 1.0f;

											pChunkData->ColorMap [4*i*64+4*j] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j] + zz*pEditor->m_EditOption.mColorMap[0] );
											pChunkData->ColorMap [4*i*64+4*j+1] =unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+1] + zz*pEditor->m_EditOption.mColorMap[1] );
								    		pChunkData->ColorMap [4*i*64+4*j+2] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+2]+ zz*pEditor->m_EditOption.mColorMap[2] );
											pChunkData->ColorMap [4*i*64+4*j+3] = 127;

											//增加修改对应的信息层。
											pInfoData->info[pEditor->m_EditOption.gameInfo ][i*64+j] = 1;
										}
										else
										{		
											pChunkData->ColorMap [4*i*64+4*j+3] = 0;
										}
									}

								}
							}
							else
							{
						    	if(data[di*64+dj]!=0)
								{ 
									if( pEditor->m_EditOption.bUserPaintPic )
									{
										if( data[di*64+dj] != 0)
										{
											
											if( pChunkData->ColorAlpha [i*64+j] - data[di*64+dj] <0)
												pChunkData->ColorAlpha [i*64+j] = 0;
											else
												pChunkData->ColorAlpha [i*64+j] -= data[di*64+dj];


											float zz =  pChunkData->ColorAlpha [i*64+j] / 255.0f ;
											if( zz > 1.0f)
												zz = 1.0f;

						                    pChunkData->ColorMap [4*i*64+4*j] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j] + zz*pEditor->m_EditOption.mColorMap[0] );
											pChunkData->ColorMap [4*i*64+4*j+1] =unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+1]  + zz*pEditor->m_EditOption.mColorMap[1] );
								    		pChunkData->ColorMap [4*i*64+4*j+2] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+2] + zz*pEditor->m_EditOption.mColorMap[2] );
											pChunkData->ColorMap [4*i*64+4*j+3] = 0;

											//增加修改对应的信息层。
											pInfoData->info[pEditor->m_EditOption.gameInfo ][i*64+j] = 0;
										}
										else
										{
											pChunkData->ColorMap [4*i*64+4*j+3] = 0;
										}
										
									}
									else
									{					
										if( data[di*64+dj] != 0 )
										{
											float zz = data[di*64+dj] / 255.0f;

											pChunkData->ColorMap [4*i*64+4*j] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j] + zz*127 );
											pChunkData->ColorMap [4*i*64+4*j+1] =unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+1] + zz*127 );
								    		pChunkData->ColorMap [4*i*64+4*j+2] = unsigned char( ( 1-zz )*pChunkData->ColorMap [4*i*64+4*j+2]+ zz*127 );
											pChunkData->ColorMap [4*i*64+4*j+3] = 0;

											//增加修改对应的信息层。
											pInfoData->info[pEditor->m_EditOption.gameInfo ][i*64+j] = 0;
										}
										else
										{
											pChunkData->ColorMap [4*i*64+4*j+3] = 0;
										}
									}

								}
							}
						}   
						dj++;
					}
				}
				di ++;
			}
		    
			//增加根据地形512X512的信息，更新颜色图
			pEditor->GetEditData()->chunkSet[gz][gx].UpdateMapInfoToColorMap( pEditor->m_EditOption.gameInfo);

			int width = 64,minmap = 0;
			
			if( pChunkRenderData->colorTex )
			{
				g_TexManager.Update (  pChunkRenderData->colorTex   , pChunkData->ColorMap   ,64,64,0); 
			}
		}
	}
};

int   MPaintTextureProcess::AddTextureFileToTitle( MTerrainEdit* pEditor , std::string name)
{
	if( pEditor->GetEditData() == NULL)
		return -1;

	for(unsigned int i = 0 ; i < pEditor->GetEditData()->texData .texFileSet.size () ; i++)
   {
	   if( pEditor->GetEditData()->texData .texFileSet [i] == name)
		   return i;
   }

    pEditor->GetEditData()->texData .texFileSet .push_back (name);

	return pEditor->GetEditData()->texData .texFileSet .size () - 1;

}

void   MPaintTextureProcess::ResetPaintTexture( MTerrainEdit* pEditor )
{
	if( pEditor->m_EditOption.PaintTexInfo[ pEditor->m_EditOption.nCurLayer ].texName [0]==0)
		return;

	std::string texname = pEditor->m_EditOption.PaintTexInfo[ pEditor->m_EditOption.nCurLayer ].texName;
	int index = AddTextureFileToTitle( pEditor , texname);
	pEditor->m_EditOption.nCurSelectTexID = index;

	pEditor->m_SelResData.ptexture = LoadTextureFromFile(texname.c_str());
    g_TexManager.LoadTex(texname.c_str());
}


void  MPaintTextureProcess::CreateAlphaPaint( MTerrainEdit* pEditor  )
{
	if( pEditor->m_EditOption.nCurLayer == LAYER_SHADOW )
	{
		//增加为阴影层
		float da = 255/(pEditor->m_EditOption.nCurSelectPaintRad[3]*1.0f);
		int i ,j;
		int testda;
		for(i = 0 ; i < 64 ; i++)
		{
			for(j = 0 ; j < 64 ; j++)
			{
				testda = (int)( 255 - (sqrt( (float)( (i-32)*(i-32) + (j-32)*(j-32)) ) )*da );
				if(testda<0)
				{
					pEditor->m_EditOption.alphaPaintData[3].data[i*64 + j]=0;
				}
				else
				{
					pEditor->m_EditOption.alphaPaintData[3].data[i*64 + j] = 85;
				}
			}
		}
		return;
	}
	
	if( pEditor->m_EditOption.mCurPaintMode == PAINT_QUAD )
	{
		CreateQuadPaint( pEditor );
		return;
	}
	else if( pEditor->m_EditOption.mCurPaintMode == PAINT_RAND_QUAD )
	{
		CreateRandPaint( pEditor );
		return;
	}
	else if ( pEditor->m_EditOption.mCurPaintMode == PAINT_RAND_CIRCLE )
	{
		CreateRandCirclePaint( pEditor );
		return;
	}

	int i,j;
    //总共对应五层，0，1，2为纹理半透明层，3为阴影层，4为颜色层
	for(i = 0 ; i < 5; i++)
	{
		if(pEditor->m_EditOption.nCurSelectPaintRad[i] == 0)
		{
			pEditor->m_EditOption.nCurSelectPaintRad[i] = 32;
		}
	}

	int testda;

	float freslut = 0.0f;
	float da = 0.0f;
    //0 , 1 ,2 层为对应的纹理半透明层
	for( int k = 0 ; k < 3 ; k ++ )
	{	
		da = 255 / (pEditor->m_EditOption.nCurSelectPaintRad[k]*1.0f);

		for(i = 0 ; i < 64 ; i++)
		{
			for(j = 0 ; j < 64 ; j++)
			{
				testda = (int)( 255 - (sqrt( (float)( (i-32)*(i-32) + (j-32)*(j-32)) ) )*da );
				if(testda<0)testda=0;
				pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = testda/2;

				freslut = pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j]*( pEditor->m_EditOption.nPaintVec / 32.0f ) ; 

				//增加，如果nPaintVec = 64 时，为硬边刷子 
				if( pEditor->m_EditOption.nPaintVec == 64 && testda!= 0 )
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = 255 ;
				}
				else
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = (unsigned char)freslut;
				}
			}
		}
	}

	//增加为颜色层
	da = 255/(pEditor->m_EditOption.nCurSelectPaintRad[4]*1.0f);
	for(i = 0 ; i < 64 ; i++)
	{
		for(j = 0 ; j < 64 ; j++)
		{
			testda =(int)( 255 - (sqrt( (float)( (i-32)*(i-32) + (j-32)*(j-32)) ) )*da );
			if(testda<0)testda=0;
			pEditor->m_EditOption.alphaPaintData[4].data[i*64 + j] = testda/2;
			
			freslut = pEditor->m_EditOption.alphaPaintData[4].data[i*64 + j]*( pEditor->m_EditOption.nPaintVec / 32.0f ) ; 
			pEditor->m_EditOption.alphaPaintData[4].data[i*64 + j] *=  (unsigned char)freslut;
		}
	}

	//增加为阴影层
	da = 255/(pEditor->m_EditOption.nCurSelectPaintRad[3]*1.0f);
	for(i = 0 ; i < 64 ; i++)
	{
		for(j = 0 ; j < 64 ; j++)
		{
			testda = (int)( 255 - (sqrt( (float)( (i-32)*(i-32) + (j-32)*(j-32)) ) )*da);
			if(testda<0)pEditor->m_EditOption.alphaPaintData[3].data[i*64 + j]=0;
			else
			pEditor->m_EditOption.alphaPaintData[3].data[i*64 + j] = 85;
		}
	}
};

void  MPaintTextureProcess::Undo( MTerrainEdit* pEditor )
{
	if( pEditor->m_SaveData.empty ())
		return ;

	if ( pEditor->m_nCurSaveDataIndex < 0 )
	{
		pEditor->m_nCurSaveDataIndex = 0;	
	} 
	else if ( pEditor->m_nCurSaveDataIndex >= (int)pEditor->m_SaveData.size ())
	{
		pEditor->m_nCurSaveDataIndex = pEditor->m_SaveData.size () - 1;	
	}

	for( unsigned int i = 0 ; i < pEditor->m_SaveData[pEditor->m_nCurSaveDataIndex].m_SaveSet .size (); i++)
	{		    
		MSaveChunkData *pData = pEditor->m_SaveData[pEditor->m_nCurSaveDataIndex].m_SaveSet[i] ;
		memcpy(  pEditor->GetEditData()->chunkSet [pData->gz][pData->gx ].GetChunkData (),&pData->saveChunkData,sizeof( TerrBlockData ));	
		pEditor->GetEditData()->chunkSet [pData->gz][pData->gx ].CreateTextureData( g_Terrain.GetTitle (0));
	}

	pEditor->m_nCurSaveDataIndex --;
};

void  MPaintTextureProcess::CreateQuadPaint( MTerrainEdit* pEditor )
{
	int i,j;

	for(i = 0 ; i < 4; i++)
	{  
		if( pEditor->m_EditOption.nCurSelectPaintRad[i] == 0)
		{
			pEditor->m_EditOption.nCurSelectPaintRad[i] = 32;
		}
	}

	for( int k = 0 ; k < 3 ; k ++ )
	{
		for(i = 0 ; i < 64 ; i++)
		{
			for(j = 0 ; j < 64 ; j++)
			{
				if( abs( i - 31) < pEditor->m_EditOption.nCurSelectPaintRad[k] 
				&& abs( j - 31) < pEditor->m_EditOption.nCurSelectPaintRad[k] )
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] += pEditor->m_EditOption.nPaintVec ;
					if( pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] >=255 ) 
					{
						pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = 255;
					}
				}
				else
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = 0;
				}
			}
		}
	}
}


void  MPaintTextureProcess::CreateRandPaint (  MTerrainEdit* pEditor )
{
    int i,j;

	for(i = 0 ; i < 4; i++)
	{
		if( pEditor->m_EditOption.nCurSelectPaintRad[i] == 0)
		{
			pEditor->m_EditOption.nCurSelectPaintRad[i] = 32;
		}
	}

	for( int k = 0 ; k < 3 ; k ++ )
	{
		for(i = 0 ; i < 64 ; i++)
		{
			for(j = 0 ; j < 64 ; j++)
			{
				if( abs( i - 31) < pEditor->m_EditOption.nCurSelectPaintRad[k] 
				&& abs( j - 31) < pEditor->m_EditOption.nCurSelectPaintRad[k] )	
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = rand()%pEditor->m_EditOption.nPaintVec;	
				}
				else
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = 0;
				}
			}
		}
	}

}

void  MPaintTextureProcess::CreateRandCirclePaint(MTerrainEdit* pEditor )
{
 	int i,j;

	for(i = 0 ; i < 4; i++)
	{
    if( pEditor->m_EditOption.nCurSelectPaintRad[i] == 0)
        pEditor->m_EditOption.nCurSelectPaintRad[i] = 32;
	}

	for( int k = 0 ; k < 3 ; k ++ )
	{
		float da = 255 / ( pEditor->m_EditOption.nCurSelectPaintRad[k]*1.0f);
		int testda;

		for(i = 0 ; i < 64 ; i++)
		{
			for(j = 0 ; j < 64 ; j++)
			{
				testda = (int)( 255 - (sqrt( (float)( (i-32)*(i-32) + (j-32)*(j-32)) ) )*da );
				if(testda<0)
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] =0;
				}
				else
				{
					pEditor->m_EditOption.alphaPaintData[k].data[i*64 + j] = rand()%pEditor->m_EditOption.nPaintVec;
				}
			}
		}
	}
}

int       MPaintTextureProcess::AddTextureFileToTile( MTerrainEdit* pEditor ,const char* filename)
{
	int texID = 0 ;

	bool bFind = false;
	for( size_t i = 0 ; i < pEditor->GetEditData()->texData.texFileSet .size () ; i++)
	{
		if( pEditor->GetEditData()->texData .texFileSet [i] == filename )
		{
			bFind = true;
			texID = i;
			break;
		}
	}

	if( !bFind )
	{
		pEditor->GetEditData()->texData .texFileSet .push_back ( filename  );
		texID = pEditor->GetEditData()->texData .texFileSet .size () - 1;
	}

	return texID ;
}

void      MPaintTextureProcess::UpdateAlphaData( MTerrainEdit* pEditor ,const char* filename, int layer)
{
	if( pEditor->GetEditData() == NULL )
		return;

	MStream  stream;
	
	if ( !stream.ReadFile ( filename ) )
		return;

	BITMAPFILEHEADER header;	

	BITMAPINFOHEADER  headInfo;

	stream.Read  ( header );
	stream.Read  ( headInfo );

	if( headInfo.biWidth != 1024 ||
		headInfo.biHeight != 1024 )
		return;


	if( headInfo.biBitCount == 24 )
	{
		int size = headInfo.biWidth * headInfo.biHeight ;

		unsigned char* alphaMap = new unsigned char [3*size];
		stream.ReadUnChar ( alphaMap , 3*size );

		unsigned char  *Map = new  unsigned char [size];

		for( int i = 0 ; i < headInfo.biHeight ; i ++)
		{
			for( int j = 0 ; j < headInfo.biWidth ; j ++)
			{
				Map[i*headInfo.biWidth + j ] = alphaMap[ ( headInfo.biHeight  - 1 - i ) * 3 *headInfo.biWidth + j*3 ] ;
			}
		}		
		
		delete [] alphaMap;

		unsigned char chunkMap[64*64];

		int indexX,indexY;

		int texID = 0 ;

		bool bFind = false;
		for( size_t i = 0 ; i < pEditor->GetEditData()->texData.texFileSet .size () ; i++)
		{
			if( pEditor->GetEditData()->texData .texFileSet [i] == g_PlantMgr->m_TextureFileSet [layer].textureName )
			{
				bFind = true;
				texID = i;
				break;
			}
		}

		if( !bFind )
		{
			pEditor->GetEditData()->texData .texFileSet .push_back ( g_PlantMgr->m_TextureFileSet [layer].textureName  );
			texID = pEditor->GetEditData()->texData .texFileSet .size () - 1;
		}

		int nblock = pEditor->GetEditData()->gw;
		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
			{
				indexY = i*nblock*64*64;
				indexX = j*64;

				for( int k = 0 ; k < 64 ; k++)
				{
					for( int l = 0 ; l < 64 ; l++)
					{
						chunkMap[k*64+l] = Map[indexX + indexY + l + k*nblock*64];
					}
				}

				TerrBlockData* pChunkData = pEditor->GetEditData()->chunkSet [i][j].GetChunkData ();
		

				memcpy( &pChunkData->alpahMap [layer] , &chunkMap , sizeof(unsigned char)*64*64 );

				TerrBlockRenderData *pChunkRenderData = pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ();
					
				if( pChunkRenderData->terrainTex [layer+1] == NULL )
				{
					if ( pEditor->m_ChunkTextureInfo[i][j].sign != 0 )
					{
						texID = AddTextureFileToTile( pEditor ,pEditor->m_ChunkTextureInfo[i][j].texFileInfo [ layer+1 ].texName );
						pChunkData->texUVInfo[layer+1] = (float)pEditor->m_ChunkTextureInfo[i][j].uvinfo [layer+1] ;
					}
					//else
					{
						pChunkRenderData->terrainTex [layer+1] = g_TexManager.LoadTex (pEditor->GetEditData()->texData .texFileSet [texID].c_str());
						pChunkData->texInfo [layer+1] = texID ;
					}
					pChunkData->texNum = layer+2;
				}

				/*
				if(pChunkRenderData->alphaTex[layer]==NULL)
				{	  
					pChunkRenderData->alphaTex[layer] = 
						g_TexManager.CreateTex( pChunkData->alpahMap[layer].data,64,64,4096,PF_A8);
				}
				else
				{
					int width = 64,minmap = 0;
					for(;;)
					{  
						//g_TexManager.Update (  pChunkRenderData->alphaTex[layer] , pChunkData->alpahMap[layer].data ,64,64,minmap); 
						pChunkRenderData->alphatexture->setPixels(layer, pChunkData->alpahMap[layer].data);
						width = width >> 1;
						minmap ++;
						if(width == 0)
							break;
						   
					}
				}*/
				if(pChunkRenderData->alphatexture->getNumChannel() <= layer) pChunkRenderData->alphatexture->setNumChannel(layer+1);
				pChunkRenderData->alphatexture->setPixels(layer, pChunkData->alpahMap[layer].data);

			}
		}

		delete [] Map;
	}
};

void    MPaintTextureProcess::SaveChunkTextureInfo( MTerrainEdit* pEditor )
{
	if( pEditor->GetEditData() == NULL )
		return;

	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
	{
		for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ;j ++ )
		{
			pEditor->m_ChunkTextureInfo[i][j].sign = 1;

			pEditor->m_ChunkTextureInfo[i][j].uvinfo [0] = (int) pEditor->GetEditData()->chunkSet [i][j].GetChunkData ()->texUVInfo[0]  ;
			pEditor->m_ChunkTextureInfo[i][j].uvinfo [1] = (int) pEditor->GetEditData()->chunkSet [i][j].GetChunkData ()->texUVInfo[1]  ;
			pEditor->m_ChunkTextureInfo[i][j].uvinfo [2] = (int) pEditor->GetEditData()->chunkSet [i][j].GetChunkData ()->texUVInfo[2]  ;
			pEditor->m_ChunkTextureInfo[i][j].uvinfo [3] = (int) pEditor->GetEditData()->chunkSet [i][j].GetChunkData ()->texUVInfo[3]  ;

			if( pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ()->terrainTex[0] != NULL )
                 strcpy( pEditor->m_ChunkTextureInfo[i][j].texFileInfo [0].texName , pEditor->GetEditData()->texData.texFileSet [pEditor->GetEditData()->chunkSet [i][j].GetChunkData()->texInfo[0]].c_str () );
			if( pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ()->terrainTex[1] != NULL )
                 strcpy( pEditor->m_ChunkTextureInfo[i][j].texFileInfo [1].texName  , pEditor->GetEditData()->texData.texFileSet [pEditor->GetEditData()->chunkSet [i][j].GetChunkData()->texInfo[1]].c_str () );
			if( pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ()->terrainTex[2] != NULL )
                 strcpy( pEditor->m_ChunkTextureInfo[i][j].texFileInfo [2].texName  , pEditor->GetEditData()->texData.texFileSet [pEditor->GetEditData()->chunkSet [i][j].GetChunkData()->texInfo[2]].c_str () );
			if( pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ()->terrainTex[3] != NULL )
                 strcpy( pEditor->m_ChunkTextureInfo[i][j].texFileInfo [3].texName  , pEditor->GetEditData()->texData.texFileSet [pEditor->GetEditData()->chunkSet [i][j].GetChunkData()->texInfo[3]].c_str () );
		}
	}

	MStream   stream;
	stream.WriteFile ("chunktexinfo.cti");

	int nblock = pEditor->GetEditData()->gw;
	stream.Write(nblock);
	stream.Write ( &pEditor->m_ChunkTextureInfo , nblock*nblock*sizeof( MChunkTextureInfo) );
	stream.WriteEnd ();

};

void   MPaintTextureProcess::LoadChunkTextureInfo( MTerrainEdit* pEditor )
{
	MStream   stream;
	
	int nblock = 16;
	if( stream.ReadFile ("chunktexinfo.cti") )
	{
		stream.Read(nblock);
		stream.Read ( &pEditor->m_ChunkTextureInfo , nblock*nblock*sizeof( MChunkTextureInfo ) );
	}
	else
	{
		memset( &pEditor->m_ChunkTextureInfo , 0 , sizeof( pEditor->m_ChunkTextureInfo )) ;
	}
};

void      MPaintTextureProcess::UpdateColorData( MTerrainEdit* pEditor ,const char* filename )
{
	TextureData *ptex = new TextureData();
	ptex->loadFromImageFile(filename);

	LMTex  pTex = ptex;
	if( pTex == NULL )
		 return;

	LockResult lockresult;
	int size = 1024*1024*4;
	unsigned char* colorMap = new unsigned char [size];

	void *pdestbits = pTex->lock(0, 0, false, lockresult);
	if(pdestbits != NULL)
	{
		unsigned char* pbit = (unsigned char*)pdestbits;

		int height = 1024 ;
		int width  = 1024 ;

		for(int i = 0 ; i < height ; i++)
		{
			for(int j = 0 ; j < width ; j++)
			{
				colorMap[i*4*width + j*4] = pbit[i*4*width + (width-1-j)*4];
				colorMap[i*4*width + j*4+1] = pbit[i*4*width + (width-1-j)*4+1];
				colorMap[i*4*width + j*4+2] = pbit[i*4*width + (width-1-j)*4+2];
				colorMap[i*4*width + j*4+3] = 125;//pbit[i*4*width + (width-1-j)*4+3];
			}
		}

		pTex->unlock(0, 0);
	}

	pTex->release();
  
		unsigned char chunkMap[64*64*4];

		int indexX,indexY;

		int texID = 0 ;

		int nblock = pEditor->GetEditData()->gw;
		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
			{
				indexY = i*nblock*64*64*4;
				indexX = j*64*4;

				for( int k = 0 ; k < 64 ; k++)
				{
					for( int l = 0 ; l < 64 ; l++)
					{
						chunkMap[4*k*64+4*l] = colorMap[indexX + indexY + l*4 + k*nblock*64*4];
						chunkMap[4*k*64+4*l+1] = colorMap[indexX + indexY + l*4 + k*nblock*64*4+1];
						chunkMap[4*k*64+4*l+2] = colorMap[indexX + indexY + l*4 + k*nblock*64*4+2];
						chunkMap[4*k*64+4*l+3] = colorMap[indexX + indexY + l*4 + k*nblock*64*4+3];
					}
				}

				TerrBlockData* pChunkData = pEditor->GetEditData()->chunkSet [i][j].GetChunkData ();

				pChunkData->bColor = true;
		
				memcpy( &pChunkData->ColorMap , &chunkMap , 4*64*64 );

				TerrBlockRenderData *pChunkRenderData = pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ();

				if(pChunkRenderData->colorTex == NULL)
				{	  
					pChunkRenderData->colorTex = g_TexManager.CreateTex( pChunkData->ColorMap ,64,64,4*4096, PF_A8R8G8B8, 1);
				}
				else
				{
					g_TexManager.Update (  pChunkRenderData->colorTex  , pChunkData->ColorMap  ,64,64,0); 
				}
			}
		}
	
		OGRE_DELETE_ARRAY( colorMap );
}

void      MPaintTextureProcess::UpdateShadowData( MTerrainEdit* pEditor ,const char* filename )
{
	if( pEditor->GetEditData() == NULL )
		return;

	MStream  stream;
	
	if ( !stream.ReadFile ( filename ) )
		return;

	BITMAPFILEHEADER header;	

	BITMAPINFOHEADER  headInfo;

	stream.Read  ( header );
	stream.Read  ( headInfo );

	if( headInfo.biWidth != 1024 ||
		headInfo.biHeight != 1024 )
		return;


	if( headInfo.biBitCount == 24 )
	{
		int size = headInfo.biWidth * headInfo.biHeight ;

		unsigned char* alphaMap = new unsigned char [3*size];
		stream.ReadUnChar ( alphaMap , 3*size );

		unsigned char  *Map = new  unsigned char [size];

		for( int i = 0 ; i < headInfo.biHeight ; i ++)
		{
			for( int j = 0 ; j < headInfo.biWidth ; j ++)
			{
				Map[i*headInfo.biWidth + j ] = alphaMap[ ( headInfo.biHeight  - 1 - i ) * 3 *headInfo.biWidth + j*3 ] ;
			}
		}		
		
		delete [] alphaMap;

		unsigned char chunkMap[64*64];

		int indexX,indexY;

		int texID = 0 ;

		int nblock = pEditor->GetEditData()->gw;
		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
			{
				indexY = i*nblock*64*64;
				indexX = j*64;

				unsigned char  *newshadowMap = new unsigned char [64*64];
				
				TerrBlockData* pChunkData = pEditor->GetEditData()->chunkSet [i][j].GetChunkData ();

				for( int k = 0 ; k < 64 ; k++)
				{
					for( int l = 0 ; l < 64 ; l++)
					{
						unsigned char color =  Map[indexX + indexY + l + k*nblock*64] ;

						int mapIndex = k*64 + l ;
						for( size_t pp = 0 ; pp < pEditor->GetEditData()->chunkSet [i][j].m_PlantSet .size () ; pp++ )
						{
							if( pEditor->GetEditData()->chunkSet [i][j].m_PlantSet[pp]->GetModelData ()->mapIndex == mapIndex )
							{
								if( color == 255 )
									pEditor->GetEditData()->chunkSet [i][j].m_PlantSet[pp]->GetModelData ()->unknow = 0 ;
								else								
									pEditor->GetEditData()->chunkSet [i][j].m_PlantSet[pp]->GetModelData ()->unknow = 1 ;
							}
						}

						chunkMap[k*64+l] = 255 - color;
						if( color == 0 )
						{
							newshadowMap[k*64+l] = 85 ;
						}
						else
						{
							newshadowMap[k*64+l] = 0 ;
						}
					}
				}


                pChunkData->bShadow = true;		
 
				memcpy( &pChunkData->shadowMap  , &chunkMap , sizeof(unsigned char)*64*64 );

				TerrBlockRenderData *pChunkRenderData = pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ();
					
                 
				if(pChunkRenderData->shadowTex ==NULL)
				{	  
					pChunkRenderData->shadowTex  = 
						g_TexManager.CreateTex( newshadowMap ,64,64,4096,
						PF_A8);

				}
				else
				{
					int width = 64,minmap = 0;
					for(;;)
					{  
						g_TexManager.Update (  pChunkRenderData->shadowTex , newshadowMap  ,64,64,minmap); 
						width = width >> 1;
						minmap ++;
						if(width == 0)
							break;
						   
					}
				}

				delete[] newshadowMap;
			}
		}

		delete [] Map;
	}
	else if (  headInfo.biBitCount == 32 )
	{
		int size = headInfo.biWidth * headInfo.biHeight ;

		unsigned char* alphaMap = new unsigned char [4*size];
		stream.ReadUnChar ( alphaMap , 4*size );

		unsigned char  *Map = new  unsigned char [size];

		for( int i = 0 ; i < headInfo.biHeight ; i ++)
		{
			for( int j = 0 ; j < headInfo.biWidth ; j ++)
			{
				Map[i*headInfo.biWidth + j ] = alphaMap[ ( i ) * 4 *headInfo.biWidth + j*4 ] ;
			}
		}		
		
		delete [] alphaMap;

		unsigned char chunkMap[64*64];

		int indexX,indexY;
		int texID = 0 ;

		int nblock = pEditor->GetEditData()->gw;
		for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
			{
				indexY = i*nblock*64*64;
				indexX = j*64;
			
				unsigned char  *newshadowMap = new unsigned char [64*64];

				for( int k = 0 ; k < 64 ; k++)
				{
					for( int l = 0 ; l < 64 ; l++)
					{
						unsigned char color =  Map[indexX + indexY + l + k*nblock*64] ;

												
						int mapIndex = k*64 + l ;
						for( size_t pp = 0 ; pp < pEditor->GetEditData()->chunkSet [i][j].m_PlantSet .size () ; pp++ )
						{
							if( pEditor->GetEditData()->chunkSet [i][j].m_PlantSet[pp]->GetModelData ()->mapIndex == mapIndex )
							{								
								pEditor->GetEditData()->chunkSet [i][j].m_PlantSet[pp]->GetModelData ()->unknow = color ;
							}
						}

						chunkMap[k*64+l] = 255 - color ;
						if( color == 0 )
						{
							newshadowMap[k*64+l] = 85 ;
						}
						else
						{
							newshadowMap[k*64+l] = 0 ;
						}
						/*
						if( color == 255 )
						   chunkMap[k*64+l] = 0;
						else
							chunkMap[k*64+l] = color;*/
					}
				}

				TerrBlockData* pChunkData = pEditor->GetEditData()->chunkSet [i][j].GetChunkData ();
                pChunkData->bShadow = true;		
 
				memcpy( &pChunkData->shadowMap  , &chunkMap , sizeof(unsigned char)*64*64 );

				TerrBlockRenderData *pChunkRenderData = pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ();
					
				if(pChunkRenderData->shadowTex ==NULL)
				{	  
					pChunkRenderData->shadowTex  = 
						g_TexManager.CreateTex( newshadowMap ,64,64,4096,
						PF_A8);
				}
				else
				{
					int width = 64,minmap = 0;
					for(;;)
					{  
						g_TexManager.Update (  pChunkRenderData->shadowTex , newshadowMap ,64,64,minmap); 
						width = width >> 1;
						minmap ++;
						if(width == 0)
							break;
					}
				}
				
				delete[] newshadowMap;
			}
		}

		delete [] Map;
	}
}

void   MPaintTextureProcess::SaveAlphaTex(  MTerrainEdit* pEditor ,const char* filename, unsigned char* pData,int layer)
{
	if( pEditor->GetEditData() == NULL )
		return;

	MStream  stream;	
	if ( !stream.WriteFile ( filename ) )
		return;
    
	unsigned char *Map = new unsigned char [1024*1024];

	int nblock = pEditor->GetEditData()->gw;
	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++)
	{
		for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j++)
		{
			for( int k = 0 ; k < 64 ; k++)
			{
				for( int l = 0 ; l < 64 ; l++)
				{	
					Map[i*nblock*64*64 + j*64 + k*nblock*64 + l] = pEditor->GetEditData()->chunkSet [i][j].GetChunkData ()->alpahMap[layer].data [k*64+l];
				}
			}
		}
	}

 	for( int i = 0 ; i < g_headInfo.biHeight ; i ++)	
	{
		for( int j = 0 ; j < g_headInfo.biWidth ; j ++)
		{
			pData [ ( g_headInfo.biHeight  - 1 - i ) * 3 *g_headInfo.biWidth + j*3 ]  = Map[i*g_headInfo.biWidth + j ];
			pData [ ( g_headInfo.biHeight  - 1 - i ) * 3 *g_headInfo.biWidth + j*3 +1]  = Map[i*g_headInfo.biWidth + j ];
			pData [ ( g_headInfo.biHeight  - 1 - i ) * 3 *g_headInfo.biWidth + j*3 +2]  = Map[i*g_headInfo.biWidth + j ];
		}
	}

	delete [] Map;

	stream.Write   ( g_header );
	stream.Write   ( g_headInfo );

	stream.WriteUnChar ( pData , 1024*1024*3 );
	stream.WriteEnd ();

}

void      MPaintTextureProcess::LoadTerainLayerInfo( MTerrainEdit* pEditor )
{
	if( pEditor->GetEditData() == NULL )
		return;  
		
	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )	
	{
		for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)		
		{		
			int texID = 0;

			TerrBlockData* pChunkData = pEditor->GetEditData()->chunkSet [i][j].GetChunkData ();
			TerrBlockRenderData *pChunkRenderData = pEditor->GetEditData()->chunkSet [i][j].GetChunkRenderData ();

			if ( pEditor->m_ChunkTextureInfo[i][j].sign != 0 )				
			{
				texID = AddTextureFileToTile( pEditor , pEditor->m_ChunkTextureInfo[i][j].texFileInfo [ 0 ].texName );
				pChunkData->texUVInfo[0] = (float)pEditor->m_ChunkTextureInfo[i][j].uvinfo [0] ;
				pChunkRenderData->terrainTex [0] = g_TexManager.LoadTex ( pEditor->GetEditData()->texData .texFileSet [texID].c_str());			
				pChunkData->texInfo [0] = texID ;	
			}
		}
	}
}

void  MPaintTextureProcess::SetCurSelectPaintTexName( MTerrainEdit* pEditor ,const char* filename)
{
	int index;

	LOG_INFO("SetCurSelectPaintTexName! ");
	if(!pEditor->isInit()) return;

	//std::string textPath = g_ProjSetting.GetTileTexturePath(false);
	//std::string texname = textPath + "\\" + filename;
	std::string texname = filename;

	index = AddTextureFileToTitle( pEditor , texname);
	pEditor->m_EditOption.nCurSelectTexID = index;
   
    strcpy( pEditor->m_EditOption.PaintTexInfo[pEditor->m_EditOption.nCurLayer].texName  , texname.c_str ());
	strcpy( pEditor->m_EditOption.PlantTexInfo.texName, texname.c_str());

	TextureData *ptex = LoadTextureFromFile(texname.c_str());
	pEditor->m_SelResData.ptexture = ptex;
    g_TexManager.LoadTex(texname.c_str());

	if(texname.find("linktexture") != - 1)
	{
		LOG_INFO( "cur edit mode linktexture! " );

		pEditor->SetCurEditEvent( AUTOLINK_TEXTURE_EVN );
		pEditor->GetAutoLinkTex()->SetTextureID( index );
	}
	else if ( texname.find("wallpaper") != - 1)
	{
		LOG_INFO( "cur edit mode wallpaper! " );

		pEditor->SetCurEditEvent( PAINT_TEXTURE_MODEL_EVN );
		pEditor->GetPaintWallPaper()->SetTextureID( index );
	}
	else
	{
		LOG_INFO( "cur edit mode normal! " );

		pEditor->SetCurEditEvent( PANIT_EVN );
	}

};

