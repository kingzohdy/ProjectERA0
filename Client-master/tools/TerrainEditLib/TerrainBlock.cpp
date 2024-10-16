
#include "TerrainBlock.h"
#include "TerrainEdit.h"
#include "TextureManager.h"
#include "TerrainTile.h"
#include "Terrain.h"
#include "EditorStream.h"

#include "OgrePixelFormat.h"
#include "OgreSceneRenderer.h"
#include "OgreShaderContext.h"
#include "OgreTerrainData.h"
#include "OgreTexture.h"
#include "OgreLiquid.h"
#include "OgreRenderSystem.h"
#include "OgreRay.h"
#include "ogrereflect.h"
#include "OgreSceneManager.h"
#include "OgreRoot.h"

using namespace Ogre;
namespace TE
{
	bool  g_bUserWaterHeigh = false;
	extern unsigned long g_dwCurTime;
	extern bool  g_bLoading ;
	extern  void ScaleMapPoint(Ogre::Vector3 &pt);

	static VertexFormat s_ChunkVertFmt;
	static bool s_ChunkVertFmtInit = false;

	static VertexData *CreateChunkVertexBuffer(size_t nvert)
	{
		return new VertexData(s_ChunkVertFmt, nvert);
	}

	EditorTerrainBlock::EditorTerrainBlock(void)
	{
		if(!s_ChunkVertFmtInit)
		{
			s_ChunkVertFmt.addElement(VET_FLOAT3, VES_POSITION);
			s_ChunkVertFmt.addElement(VET_FLOAT3, VES_NORMAL);
			s_ChunkVertFmt.addElement(VET_COLOUR, VES_COLOR);
			s_ChunkVertFmt.addElement(VET_FLOAT2, VES_TEXCOORD);
			s_ChunkVertFmt.addElement(VET_FLOAT2, VES_TEXCOORD, 1);

			s_ChunkVertFmtInit = true;
		}

		memset(&m_ChunkData,0,sizeof(TerrBlockData));
		memset(&m_ChunkRenderData,0,sizeof(TerrBlockRenderData));	
		memset(&m_ChunkOptionData,0,sizeof(TerrBlockOption));
		memset(&m_LiquidData,0,sizeof(TerrBlockLiquidData));
		memset(&m_InfoData , 0 ,sizeof(TerrBlockLayerInfo));

		m_InfoData.color[0][1] = 255;//行走层，，绿色
		m_InfoData.color[1][0] = 255;//水层，蓝色
		m_InfoData.color[2][2] = 255;//法术阻挡层，红色

		m_InfoData.color[0][3] = 127;
		m_InfoData.color[1][3] = 127;
		m_InfoData.color[2][3] = 127;

		m_ChunkOptionData.showLayer = 5;
		m_ChunkOptionData.nRepeat   = 12;
		m_ChunkData.texInfo [0] = m_ChunkData.texInfo[1] = m_ChunkData.texInfo[2] = 
			m_ChunkData.texInfo[3] = 0xff;

		m_pLiquid = NULL;
		m_pQuadPath = NULL;

		m_ChunkOptionData.bShowShadow = true;

		m_ChunkStatus = CHUNK_UNLOAD ;

		for( int i = 0 ; i < 4 ; i ++)
		{
			m_ChunkData.texUVInfo [i] = 12;
		}

		m_NewVecData.ngrid = 0;

		m_bBakeStaticLight = true;
		addRenderUsageBits(RU_REFLECT);
		addRenderUsageBits(RU_REFRACT);

		m_pLightMap = NULL;
	}

	EditorTerrainBlock::~EditorTerrainBlock(void)
	{
		OGRE_RELEASE(m_ChunkRenderData.alphatexture);
		OGRE_RELEASE(m_ChunkRenderData.pmtl);
		OGRE_RELEASE(m_ChunkRenderData.pcolormaskmtl);
	}

	void EditorTerrainBlock::attachToScene(GameScene *pscene)
	{
	}

	void EditorTerrainBlock::detachFromScene(GameScene *pscene)
	{
	}

	static void SetContextPrimitive(ShaderContext *pcontext, PrimitiveType type, size_t nprim, IndexBuffer *pib)
	{
		pcontext->m_PrimitiveType = type;
		pcontext->m_NumPrimitive = nprim;
		pcontext->setIB(pib);
	}

	void EditorTerrainBlock::drawChunkMesh(Ogre::ShaderContext *pcontext, int fillmode, int layer)
	{
		TerrBlockRenderData *pRender = GetChunkRenderData();
		IndexData *pib = pRender->ib[layer];
		size_t nindex = pib->getNumIndex();
		if(nindex == 0) return;

		switch(fillmode)
		{
		case DRAW_SOLID:
			SetContextPrimitive(pcontext, PRIM_TRIANGLESTRIP, nindex-2, pib);
			break;
		case DRAW_WIREFRAME:
			SetContextPrimitive(pcontext, PRIM_LINESTRIP, nindex-1, pib);
			break;
		case DRAW_POINT:
			SetContextPrimitive(pcontext, PRIM_POINTLIST, nindex, pib);
			break;
		}
	}

	void EditorTerrainBlock::update(unsigned int dtick)
	{
		if(m_pLiquid)
			m_pLiquid->update(dtick);

		EditorTerrainBlock::PLANT_MAP::iterator iter = m_PlantMap.begin() ;
		for(; iter != m_PlantMap.end() ; iter++)
		{
			iter->second->UpdateEx_1(dtick);
			//iter->second->Update( time );
		}

		m_WorldBounds.fromBox(m_AABBox.getMinPos(), m_AABBox.getMaxPos());
	}

	void EditorTerrainBlock::getRenderPassRequired(RenderPassDesc &desc)
	{
		if(GetChunkData()->bHaswater)
		{
			//desc.pass_usages |= (1<<RU_REFLECT)|(1<<RU_REFRACT);
			desc.pass_usages |= (1<<RU_REFLECT);
			desc.reflect_height = GetLiquidData()->waterheight;
		}
	}

	void EditorTerrainBlock::render(SceneRenderer *prenderer, const ShaderEnvData &envdata)
	{
		int texnum = 0;
		TerrBlockRenderData *pRenderData = GetChunkRenderData();
		TerrBlockData *pchunkdata = GetChunkData();
		float uvRepeat[4];
		Texture *ptex[4];

		int nchannel = pRenderData->alphatexture->getNumChannel() + 1;
		for(int k=0; k<nchannel; k++)
		{	
			if(pRenderData->terrainTex[k])
			{
				ptex[texnum] = pRenderData->terrainTex[k];
				uvRepeat[texnum] = (float)pchunkdata->texUVInfo[k];
			}
			else
			{
				ptex[texnum] = pRenderData->terrainTex[0];
				uvRepeat[texnum] = (float)pchunkdata->texUVInfo[k];
			}
			texnum++;
		}


		//render terrain
		if(pRenderData->mtlflags != texnum)
		{
			OGRE_RELEASE(pRenderData->pmtl);

			pRenderData->pmtl = new Material("terrain_all");
			pRenderData->pmtl->setParamMacro("TERRAIN_LAYERS", texnum);
			pRenderData->mtlflags = texnum;
		}

		Ogre::Material *pmtl = pRenderData->pmtl;
		pmtl->setParamValue("g_UVRepeat", uvRepeat);

		int mode = Root::getSingleton().getLightMode();
		pmtl->setParamMacro("USE_STATICLIGHT", mode);

		if (mode == 2)
		{
			if (m_pLightMap)
				pmtl->setParamTexture("g_LightMap", m_pLightMap);
		}

		if(texnum >= 1)
		{
			pmtl->setParamTexture("g_BlendColorTex0", ptex[0]);
		}
		if(texnum >= 2)
		{
			pmtl->setParamTexture("g_BlendColorTex1", ptex[1]);  
			pmtl->setParamTexture("g_BlendAlphaTex", pRenderData->alphatexture);
		}
		if(texnum >= 3)
		{
			pmtl->setParamTexture("g_BlendColorTex2", ptex[2]);
		}
		if(texnum >= 4)
		{	
			pmtl->setParamTexture("g_BlendColorTex3", ptex[3]);
		}
		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
		DEBUG_SHADERCONTEXT(pcontext);

		pcontext->setInstanceEnvData(prenderer, NULL, envdata, NULL);

		drawChunkMesh(pcontext, g_TerrainEdit.m_DrawFillMode, 1);

		if(g_TerrainEdit.m_DrawOption.bUserColorMap && GetChunkData()->bColor)
		{
			if(pRenderData->pcolormaskmtl == NULL)
			{
				pRenderData->pcolormaskmtl = new Material("terrain_colormask");
			}
			pRenderData->pcolormaskmtl->setParamTexture("g_ColorMaskTex", pRenderData->colorTex);

			pcontext = prenderer->newContext(RL_SCENE, envdata, pRenderData->pcolormaskmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
			DEBUG_SHADERCONTEXT(pcontext);

			pcontext->setInstanceEnvData(prenderer, NULL, envdata, NULL);
			drawChunkMesh(pcontext, DRAW_SOLID, 0);
		}

		//render water
		if(GetChunkData()->bHaswater && prenderer->m_CurRenderUsage!=RU_REFLECT && prenderer->m_CurRenderUsage!=RU_REFRACT)
		{
			GetChunkLiquid()->render(prenderer, envdata);
		}

		//render plant
		if(g_TerrainEdit.m_DrawOption.bDrawPlant)
		{
			PLANT_MAP::iterator iter = m_PlantMap.begin();
			for(; iter!=m_PlantMap.end(); iter++)
			{
				GrassGroup *plantset = iter->second;
				plantset->Render(prenderer, envdata);
			}
		}
	}

	void   EditorTerrainBlock::UpdateRenderHeight()
	{
		if( m_ChunkRenderData.vb == NULL )
			return;

		if(m_pLiquid)
			createOrUpdateDepthTexture(m_pLiquid);

		TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb ->lock();
		for( size_t i = 0 ; i < m_NewVecData.vVer.size() ; i++)
		{
			vdata[i].pos .y = m_NewVecData.vVer [i].pos . y ;

			if( m_NewVecData.vVer [i].pos . y < m_ChunkData.minVec. y  )
				m_ChunkData.minVec. y =  m_NewVecData.vVer[i].pos . y ;
			if( m_NewVecData.vVer [i].pos . y > m_ChunkData.maxVec .y )
				m_ChunkData.maxVec .y =   m_NewVecData.vVer [i].pos . y ;

			vdata[i].normal =  m_NewVecData.vVer[i].normal ;
		}
		m_ChunkRenderData.vb ->unlock();

	};		
	//更新对应格子高度	
	void    EditorTerrainBlock::SetGridHeight(int mx ,int my , float height )
	{
		if( mx < 0 || mx > m_NewVecData.ngrid )
			return;

		if( my < 0 || my > m_NewVecData.ngrid )
			return;

		if( m_ChunkRenderData.vb == NULL )
			return;

		float _height[4];
		TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb ->lock();
		{
			//vdata[mx+my*(m_NewVecData.nSize+1)].normal = MVec3( 0 , 1 ,0 );
			_height[0] = m_NewVecData.vVer[mx+my*(m_NewVecData.ngrid+1)].pos.y +height;
			vdata[mx+my*(m_NewVecData.ngrid+1)].pos.y = _height[0];

			//vdata[mx+1+my*(m_NewVecData.nSize+1)].normal = MVec3( 0 , 1 ,0 );
			_height[1] = m_NewVecData.vVer[mx+1+my*(m_NewVecData.ngrid+1)].pos.y+height;
			vdata[mx+1+my*(m_NewVecData.ngrid+1)].pos.y =  _height[1];

			//vdata[mx+(my+1)*(m_NewVecData.nSize+1)].normal = MVec3( 0 , 1 ,0 );
			_height[2] = m_NewVecData.vVer[mx+(my+1)*(m_NewVecData.ngrid+1)].pos.y+height;
			vdata[mx+(my+1)*(m_NewVecData.ngrid+1)].pos.y =  _height[2];

			//vdata[mx+1+(my+1)*(m_NewVecData.nSize+1)].normal = MVec3( 0 , 1 ,0 );
			_height[3] = m_NewVecData.vVer[mx+1+(my+1)*(m_NewVecData.ngrid+1)].pos.y +height;
			vdata[mx+1+(my+1)*(m_NewVecData.ngrid+1)].pos.y = _height[3];
		}
		m_ChunkRenderData.vb ->unlock();

		for( int i = 0 ; i < 4 ; i ++ )
		{
			if( m_ChunkData.maxVec.y < _height[i] )
			{
				m_ChunkData.maxVec.y = _height[i] ;
			}
			else if ( m_ChunkData.minVec.y > _height[i] )
			{
				m_ChunkData.minVec.y = _height[i] ;
			}
		}

		m_AABBox.setRange( m_ChunkData.minVec , m_ChunkData.maxVec );

	};

	Ogre::Vector3   EditorTerrainBlock::GetChunkVec( int mx ,int my )
	{
		Ogre::Vector3 pos(0,0,0);
		if ( m_ChunkData.vecNum == 145 )
			return pos ;

		return m_NewVecData.vVer[my*(m_NewVecData.ngrid+1) + mx ].pos ;

	};

	bool EditorTerrainBlock::GetNormal(float x , float z , Ogre::Vector3* normal)
	{
		if(x > m_ChunkData.maxVec.x || x < m_ChunkData.minVec .x )
			return false;

		if(z > m_ChunkData.maxVec.z || z < m_ChunkData.minVec .z )
			return false;

		Ogre::Vector3 _normal(0,1,0) ;

		if( m_ChunkData.vecNum != 145 )//如果是wow格式
		{
			int gx,gz ;

			float unitsize = TERRBLOCK_SIZE / m_NewVecData.ngrid ;

			float fx =  (x - m_ChunkData.minVec.x )/ unitsize;
			float fz =  (z - m_ChunkData.minVec .z ) / unitsize;
			gx = (int)fx ;
			gz = (int)fz ;

			float dx = fx - gx;
			float dz = fz - gz;
			int hpos[4];

			hpos[0] = gx + (m_NewVecData.ngrid+1)*gz;

			if ( dx > 0.5f )
				hpos[0] += 1;

			if( dz > 0.5f )
				hpos[0] += m_NewVecData.ngrid + 1;

			(*normal) = m_NewVecData.vVer [hpos[0]].normal ;

			return true;
		}

		return true;
	};

	bool EditorTerrainBlock::GetHeightEx(float x,float z,float* height0)
	{
		if(x > m_ChunkData.maxVec.x || x < m_ChunkData.minVec .x )
			return false;

		if(z > m_ChunkData.maxVec.z || z < m_ChunkData.minVec .z )
			return false;

		float height ;


		{
			int gx,gz ;

			float unitsize = TERRBLOCK_SIZE / m_NewVecData.ngrid ;

			float fx =  (x - m_ChunkData.minVec.x )/ unitsize;
			float fz =  (z - m_ChunkData.minVec .z ) / unitsize;
			gx = (int)fx ;
			gz = (int)fz ;

			float dx = fx - gx;
			float dz = fz - gz;
			int hpos[4];

			hpos[0] = gx + (m_NewVecData.ngrid+1)*gz;

			if( m_NewVecData.vVer[hpos[0]].pos .x == x
				&& m_NewVecData.vVer [hpos[0]].pos .z == z)
			{
				(*height0) = m_NewVecData.vVer [hpos[0]].pos .y;
				return true;
			}

			hpos[1] = gx + 1 + (m_NewVecData.ngrid+1)*gz;

			if(hpos[1] >= (m_NewVecData.ngrid+1)*(m_NewVecData.ngrid+1))
				hpos[1] = hpos[0];

			hpos[2] = gx  + (gz+1)*(m_NewVecData.ngrid+1) ;
			if(hpos[2] >= (m_NewVecData.ngrid+1)*(m_NewVecData.ngrid+1))
				hpos[2] = hpos[0];

			hpos[3] = gx + 1 +(gz+1)*(m_NewVecData.ngrid+1);
			if(hpos[3] >= (m_NewVecData.ngrid+1)*(m_NewVecData.ngrid+1))
				hpos[3] = hpos[0];

			float height1 , height2 ;

			height1 =  m_NewVecData.vVer[hpos[0]].pos.y*(1-dx) + m_NewVecData.vVer[hpos[1]].pos.y*(dx) ;
			height2 =  m_NewVecData.vVer[hpos[2]].pos.y*(1-dx) + m_NewVecData.vVer[hpos[3]].pos.y*(dx) ;

			height = height1*(1-dz) + height2*dz;

		}

		(*height0) = height ;   

		return true;
	}

	bool EditorTerrainBlock::GetHeight(float x,float z,float* height0, bool use_vb)
	{
		//assert(x>=m_ChunkData.minVec.x-0.1f && x<m_ChunkData.maxVec.x+0.1f);
		//assert(z>=m_ChunkData.minVec.z-0.1f && z<m_ChunkData.maxVec.z+0.1f);

		x = Clamp(x, m_ChunkData.minVec.x, m_ChunkData.maxVec.x);
		z = Clamp(z, m_ChunkData.minVec.z, m_ChunkData.maxVec.z);

		float height;
		float unitsize = TERRBLOCK_SIZE / m_NewVecData.ngrid;
		float fx = (x - m_ChunkData.minVec.x )/ unitsize;
		float fz = (z - m_ChunkData.minVec .z ) / unitsize;
		int gx = (int)fx;
		if(gx > m_NewVecData.ngrid) gx = m_NewVecData.ngrid;
		int gz = (int)fz;
		if(gz > m_NewVecData.ngrid) gz = m_NewVecData.ngrid;

		float dx = fx - gx;
		float dz = fz - gz;
		int hpos[4];

		int gx2 = (gx==m_NewVecData.ngrid?gx:gx+1);
		int gz2 = (gz==m_NewVecData.ngrid?gz:gz+1);

		hpos[0] = gx + (m_NewVecData.ngrid+1)*gz;
		hpos[1] = gx2 + (m_NewVecData.ngrid+1)*gz;
		hpos[2] = gx + (m_NewVecData.ngrid+1)*gz2;
		hpos[3] = gx2 + (m_NewVecData.ngrid+1)*gz2;

		float height1 , height2 ;
		if(use_vb)
		{
			TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb->lock();
			height1 =  vdata[hpos[0]].pos.y*(1-dx) + vdata[hpos[1]].pos.y*(dx);
			height2 =  vdata[hpos[2]].pos.y*(1-dx) + vdata[hpos[3]].pos.y*(dx);
			m_ChunkRenderData.vb ->unlock();
		}
		else
		{
			height1 =  m_NewVecData.vVer[hpos[0]].pos.y*(1-dx) + m_NewVecData.vVer[hpos[1]].pos.y*(dx);
			height2 =  m_NewVecData.vVer[hpos[2]].pos.y*(1-dx) + m_NewVecData.vVer[hpos[3]].pos.y*(dx);
		}

		height = height1*(1-dz) + height2*dz;

		if(g_bUserWaterHeigh && m_ChunkData.bHaswater)
		{
			if(m_pLiquid->getHeight() > height) 
				height = m_pLiquid->getHeight();
		}

		(*height0) = height;
		return true;
	};

	bool EditorTerrainBlock::GetWaterHeight (float x, float y, float* height)
	{
		if (m_pLiquid)
		{
			*height = m_pLiquid->getHeight();

			return true;
		}
		else
			*height = 0.0f;

		return false;
	}

	void  EditorTerrainBlock::SetLiquidHeight(float height)
	{
		if( m_pLiquid )
		{
			m_pLiquid->setHeight(height);
			createOrUpdateDepthTexture(m_pLiquid);
			m_LiquidData.waterheight = height;
		}

	}
	void EditorTerrainBlock::SetLiquidColor(float r, float g, float b, float a)
	{
	}

	void  EditorTerrainBlock::UpdateQuadPath()
	{	
		if(!m_pQuadPath->GetPathData ()->data.empty())
		m_pQuadPath->GetPathData ()->data .clear ();

		Ogre::Vector3 pos;


		{
			if( m_NewVecData.vVer.empty() )
				return;

			for( int i = 0 ; i < m_NewVecData.ngrid+1 ; i++)
			{
				pos = m_NewVecData.vVer [i].pos;
				pos.y += 0.1f;
				m_pQuadPath->GetPathData ()->data .push_back ( pos );
			}
			for( int i = 1 ; i < m_NewVecData.ngrid+1 ; i++)
			{
				pos =  m_NewVecData.vVer[(m_NewVecData.ngrid+1)*i+m_NewVecData.ngrid].pos;
				pos.y += 0.1f;
				m_pQuadPath->GetPathData ()->data .push_back ( pos );
			}
			for( int i = 1 ; i < m_NewVecData.ngrid+1 ; i++)
			{
				pos =   m_NewVecData.vVer[(m_NewVecData.ngrid+1)*(m_NewVecData.ngrid+1)-1-i].pos;
				pos.y += 0.1f;
				m_pQuadPath->GetPathData ()->data .push_back ( pos ); 
			}
			for(int i = m_NewVecData.ngrid-1 ; i >=0 ; i--)
			{
				pos =  m_NewVecData.vVer[(m_NewVecData.ngrid+1)*i].pos;
				pos.y += 0.1f;
				m_pQuadPath->GetPathData ()->data .push_back ( pos );
			}
		}

		m_pQuadPath->UpdataData ();
	}
	bool  EditorTerrainBlock::CreateQuadPath()
	{
		m_pQuadPath = new MapPath;

		UpdateQuadPath();

		return true;
	};
	bool EditorTerrainBlock::CreateTextureData(EditorTerrainTile* title)
	{
		for( int i = 0 ; i < 4 ; i ++)
		{
			m_ChunkRenderData.terrainTex[i] = NULL ; 
		}

		for(int i = 0 ; i < m_ChunkData.texNum ; i++)
		{
			if( m_ChunkData.texInfo[i]==0xff )
				continue;

			m_ChunkRenderData.terrainTex [i] = g_TexManager.LoadTex (title->GetTitleData ()->texData .texFileSet [m_ChunkData.texInfo [i]].c_str ()) ;
			if( m_ChunkRenderData.terrainTex [i] == NULL )
			{
				return false;
			}
		}


		for( int i = 0 ; i < 3 ; i ++ )
		{
			m_ChunkRenderData.alphaTex [i] = NULL;
		}

		uchar *pbits[4];
		for(int i = 0 ; i < m_ChunkData.texNum - 1 ; i++)
		{
			m_ChunkRenderData.alphaTex [i] = g_TexManager.CreateTex(m_ChunkData.alpahMap [i].data ,64,64,4096,PF_A8);
			pbits[i] = m_ChunkData.alpahMap[i].data;
		}
		m_ChunkRenderData.alphatexture = new AlphaTexture(64, 64, m_ChunkData.texNum-1);
		m_ChunkRenderData.alphatexture->setPixels(pbits);

		m_ChunkRenderData.shadowTex = g_TexManager.CreateTex(m_ChunkData.shadowMap ,64,64,4096,PF_A8);

		if( m_ChunkData.bColor )
		{
			UpdateMapInfoToColorMap(m_InfoData.nSelectMode);
			m_ChunkRenderData.colorTex = g_TexManager.CreateTex (m_ChunkData.ColorMap  ,64,64,4096, PF_A8R8G8B8, 1);
		}

		return true;
	}


	void  EditorTerrainBlock::CreatePass( )
	{
		m_RenderPassSet.bDraw.resize( m_NewVecData.ngrid*m_NewVecData.ngrid );
		memset( &m_RenderPassSet.bDraw[0] ,0, m_NewVecData.ngrid*m_NewVecData.ngrid );
	};


	bool EditorTerrainBlock::CreateOtherVecData()
	{
		m_AABBox.setRange ( m_ChunkData.minVec , m_ChunkData.maxVec );

		int nSize = (m_NewVecData.ngrid+1)*(m_NewVecData.ngrid+1) ;

		int nTestSize;
		nTestSize = m_NewVecData.vVer.size();

		if( m_ChunkRenderData.vb == NULL)
		{
			int index = 0;
			float u1 = 1 / (m_NewVecData.ngrid*1.0f);
			float v1 = 1 / (m_NewVecData.ngrid*1.0f);

			for( int i = 0 ; i< m_NewVecData.ngrid+1 ; i ++ )
			{
				for ( int j = 0 ; j < m_NewVecData.ngrid+1 ; j++ )
				{
					m_NewVecData.vVer[index].texcoord.x  = u1*j;
					m_NewVecData.vVer[index].texcoord.y = v1*i;

					m_NewVecData.vVer [index].texcoord2.x  = m_ChunkOptionData.nRepeat*m_NewVecData.vVer [index].texcoord2.x;
					m_NewVecData.vVer [index].texcoord2.y  = m_ChunkOptionData.nRepeat*m_NewVecData.vVer [index].texcoord2.y;
					index++;
				}
			}

			m_ChunkRenderData.vb = CreateChunkVertexBuffer(nSize);
		}

		TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb ->lock();
		for( size_t i = 0 ; i < m_NewVecData.vVer.size () ; i ++ )
		{
			vdata[i].normal = m_NewVecData.vVer[i].normal;
			vdata[i].pos = m_NewVecData.vVer[i].pos;
			vdata[i].color = m_NewVecData.vVer[i].color;
			vdata[i].texcoord = m_NewVecData.vVer[i].texcoord;
			vdata[i].texcoord2 = m_NewVecData.vVer[i].texcoord2;

		}
		m_ChunkRenderData.vb ->unlock();



		return true;
	}

	bool EditorTerrainBlock::CreateVecData()
	{	
		return CreateOtherVecData();
	}	

	Ogre::IndexData *EditorTerrainBlock::createIndexBuffer(int layer)
	{
		vector<unsigned short>  vIndex;
		int drawsize = m_RenderPassSet.bDraw.size() ;
		int  nSize = m_NewVecData.ngrid ;
		int  beginIdx = 0 , endIdx = 0;
		bool bChange = false;
		bool  bBegin = false;

		for(int i = 0 ; i<nSize ; i ++)
		{	
			bool blast = false;
			bool bStart = false;
			for( int j = 0 ;j < nSize ; j ++ )
			{
				if(layer==0 || m_RenderPassSet.bDraw[i*nSize+j]==0)
				{ // 地形层？
					bBegin = true;
					if( !bStart )
					{
						if( bChange )
						{
							bChange = false;
							vIndex.push_back( beginIdx );
							vIndex.push_back( j + i*(nSize+1) );
						}

						bStart = true;
						vIndex.push_back( j + i*(nSize+1) );
						vIndex.push_back( j+(i+1)*(nSize+1) );
					}

					vIndex.push_back( j+1 + i*(nSize+1) );
					vIndex.push_back( j+1+(i+1)*(nSize+1) );

					beginIdx = j+1+(i+1)*(nSize+1);
				}
				else
				{ // Mask层？
					if( bStart )
					{
						if( i == ( nSize- 1 )  && j == ( nSize - 1 ) )
							break;

						vIndex.push_back( j+(i+1)*(nSize+1) );

						int nx,ny ;
						nx = j+1;
						ny = i;
						if( nx > nSize )
						{
							nx = 0 ;
							ny += 1;
						}
						vIndex.push_back( nx+ny*(nSize+1) );

						beginIdx = nx + ny*(nSize+1) ;
						bStart = false;
					}
					else
					{
						if( bChange && !bStart)
						{
						}
						else
						{
							if( bBegin )
							{
								vIndex.pop_back();
								int nx,ny ;
								nx = j+1;
								ny = i;
								if( nx > nSize )
								{
									nx = 0 ;
									ny += 1;
								}
								vIndex.push_back( nx+ny*(nSize+1) );
								beginIdx = nx + ny*(nSize+1) ;
							}
							else
							{
								beginIdx = j+1+i*(nSize+1);
							}
						}
					}
				}
			}

			bChange = true;
		}

		IndexData *pindexdata = new IndexData(vIndex.size());
		pindexdata->setVertexRange(0, (nSize+1)*(nSize+1));

		if(pindexdata->getNumIndex() > 0)
		{
			unsigned short* idata = (unsigned short*)pindexdata->lock();
			memcpy(idata,&vIndex[0],sizeof(unsigned short)*vIndex.size());
			pindexdata->unlock();
		}

		return pindexdata;
	}

	void EditorTerrainBlock::CreateRenderPassEx()
	{
		m_RenderPassSet.passSet .clear ();

		for(int i=0; i<2; i++)
		{
			OGRE_RELEASE(m_ChunkRenderData.ib[i]);
			m_ChunkRenderData.ib[i] = createIndexBuffer(i);
		}

		m_ChunkRenderData.triNum = m_ChunkRenderData.ib[1]->getNumIndex()/3;
	}

	void EditorTerrainBlock::CreateRenderPass()
	{
		m_RenderPassSet.passSet .clear ();

		MChunkRenderPass  pass;
		pass.numTri = 0;
		pass.startIdx = 0;
		bool  bPushBack = false;
		bool  bStart    = true;

		for( int i = 0 ; i < 64 ; i++)
		{
			if( m_RenderPassSet.bDraw [i] == 0)
			{
				if( bStart )
				{
					pass.startIdx = i*2 + (i / 8)*4;
					pass.numTri = 4;
					bStart = false;
				}
				else
				{
					pass.numTri += 2;
					if( i % 8 == 0)
					{
						pass.numTri += 4;
					}
				}
				bPushBack = false;
			}
			else
			{
				if( !bPushBack )
				{
					if(pass.numTri != 0)
						m_RenderPassSet.passSet .push_back ( pass );
				}
				bPushBack = true;
				pass.numTri = 0;
				bStart = true;
			}
		}

		if( !bPushBack )
		{		
			if(pass.numTri != 0)
				m_RenderPassSet.passSet.push_back ( pass );
		}
	}
	bool EditorTerrainBlock::CreateRenderData(EditorTerrainTile* title)
	{
		if( m_ChunkStatus == CHUNK_LOADED )
			return true;

		CreateTextureData(title);
		CreateVecData();

		CreateQuadPath();

		if(m_ChunkData.bHaswater != 0)
		{
			TerrBlockLiquidData *pliquid = GetLiquidData();
			m_pLiquid = new Ogre::TLiquid;
			if (pliquid->type == TLiquid::WATER_GENERAL)
			{
				if (string(pliquid->name) == string("scene\\environment\\water\\magma\\magma"))
					m_pLiquid->createGeneralLiquid(pliquid->name, 51);
				else
					m_pLiquid->createGeneralLiquid(pliquid->name, 30);
			}
			else if (pliquid->type == TLiquid::WATER_REFLECT)
				m_pLiquid->createReflectLiquid("scene\\environment\\water\\LakeWaterNormal.png", "scene\\environment\\water\\bump.dds");

			m_pLiquid->createVBIB(m_ChunkData.gx , m_ChunkData.gy, m_ChunkData.minVec.x , m_ChunkData.minVec.z, m_ChunkData.maxVec.x, m_ChunkData.maxVec.z, pliquid->waterheight);

			m_pLiquid->m_ShallowWaterColor = pliquid->shallowcolor;
			m_pLiquid->m_DeepWaterColor = pliquid->deepcolor;
			m_pLiquid->m_ShallowWaterDepth = pliquid->shallowdepth;
			m_pLiquid->m_DeepWaterDepth = pliquid->deepdepth;
			m_pLiquid->m_MaxWaterDepth = pliquid->maxdepth;
			m_pLiquid->m_NumTexRepeat = pliquid->texrepeat;
			m_pLiquid->m_Speed			= pliquid->speed;
			m_pLiquid->m_Amp			= pliquid->amp;
			createOrUpdateDepthTexture(m_pLiquid);

			//临时代码
			//g_Terrain.m_pReflect->m_Height = pliquid->waterheight;
		}

		if( m_NewVecData.ngrid == 0 )
		{
			//如果不是wow格式
			m_RenderPassSet.bDraw.resize( 64 );
			for( int i = 0 ; i < 64 ; i ++ )
				m_RenderPassSet.bDraw[i] = 0 ;

			CreateRenderPass();
		}
		else
		{
			CreateRenderPassEx();
		}

		m_ChunkRenderData.pmtl = NULL;
		m_ChunkRenderData.mtlflags = -1;

		m_ChunkRenderData.pLayout = RenderSystem::getSingleton().getInputLayout(s_ChunkVertFmt);

		m_ChunkStatus = CHUNK_LOADED ;
		return true;
	};

	bool  EditorTerrainBlock::DelPlant(Ogre::Vector3 pos , float fSize )
	{
		PLANT_MAP::iterator iter = m_PlantMap.begin();

		for( ; iter != m_PlantMap.end () ; iter ++ )
		{
			iter->second->DelPos( pos , fSize );
		}

		return true;
	};


	bool EditorTerrainBlock::AddPlant( const char* filename , Ogre::Vector3 pos , float fScale , unsigned int color)
	{
		std::string name = filename ;

		PLANT_MAP::iterator   iter = m_PlantMap.find( name );
		if( iter != m_PlantMap.end() )
		{
			//iter->second->AddPosEx( pos );
			iter->second->AddPosEx_1( pos , fScale , color );
			return true;
		}

		GrassGroup* pPlant = new GrassGroup;

		//pPlant->InitEx();
		pPlant->InitEx_1();

		pPlant->SetTexture( filename );

		pPlant->m_fHeight = 200.0f;
		pPlant->m_fWidth = 50;

		pPlant->AddPosEx_1( pos , fScale ,color );
		//pPlant->AddPosEx( pos );

		m_PlantMap[name] = pPlant ;

		return true;
	}


	void  EditorTerrainBlock::UpdateMapInfoToColorMap(int nSelectModel)
	{
		if( nSelectModel< 0 || nSelectModel >=4 )
			return;

		for ( int i = 0 ; i < 64 ; i ++ )		
		{
			for( int j = 0 ; j < 64 ; j++ )
			{	

				unsigned char  colormap[4] ;
				if( ( i + j ) % 2 == 0 )
				{
					colormap[0] = m_InfoData.color [nSelectModel][0];
					colormap[1] = m_InfoData.color [nSelectModel][1];          
					colormap[2] = m_InfoData.color [nSelectModel][2];
					colormap[3] = 85;//m_InfoData.color [nSelectModel][3];
				}
				else
				{
					colormap[0] = 255 - m_InfoData.color [nSelectModel][0];
					colormap[1] = 255 - m_InfoData.color [nSelectModel][1];          
					colormap[2] = 255 - m_InfoData.color [nSelectModel][2];
					colormap[3] = 85;//255 - m_InfoData.color [nSelectModel][3];
				}

				if ( m_InfoData.info[nSelectModel][i*64+j] != 0 )
				{
					m_ChunkData.ColorAlpha[i*64+j] = 255;

					m_ChunkData.ColorMap[4*i*64+4*j] = colormap[0];
					m_ChunkData.ColorMap[4*i*64+4*j+1] = colormap[1];
					m_ChunkData.ColorMap[4*i*64+4*j+2] = colormap[2];
					m_ChunkData.ColorMap[4*i*64+4*j+3] = colormap[3];
				}
				else
				{
					m_ChunkData.ColorAlpha[i*64+j] = 255;

					m_ChunkData.ColorMap[4*i*64+4*j] =  0;
					m_ChunkData.ColorMap[4*i*64+4*j+1] =  127;
					m_ChunkData.ColorMap[4*i*64+4*j+2] =  127;
					m_ChunkData.ColorMap[4*i*64+4*j+3] =  0;
				}
			}
		}

		return ;


		for ( int i = 0 ; i < 32 ; i ++ )		
		{
			for( int j = 0 ; j < 32 ; j++ )
			{		
				unsigned char  colormap[4] ;
				if( ( i + j ) % 2 == 0 )
				{
					colormap[0] = m_InfoData.color [nSelectModel][0];
					colormap[1] = m_InfoData.color [nSelectModel][1];          
					colormap[2] = m_InfoData.color [nSelectModel][2];
					colormap[3] = 85;//m_InfoData.color [nSelectModel][3];
				}
				else
				{
					colormap[0] = 255 - m_InfoData.color [nSelectModel][0];
					colormap[1] = 255 - m_InfoData.color [nSelectModel][1];          
					colormap[2] = 255 - m_InfoData.color [nSelectModel][2];
					colormap[3] = 85;//255 - m_InfoData.color [nSelectModel][3];
				}

				if ( ( m_InfoData.info[nSelectModel][2*i*64+2*j] != 0 ) &&
					( m_InfoData.info[nSelectModel][2*i*64+2*j+1] != 0 ) && 
					( m_InfoData.info[nSelectModel][2*(i)*64+64+2*j] != 0 ) && 
					( m_InfoData.info[nSelectModel][2*(i)*64+64+2*j+1] != 0 ) )
				{
					m_ChunkData.ColorAlpha[2*i*64+2*j] = 255;
					m_ChunkData.ColorAlpha[2*i*64+2*j+1] = 255;
					m_ChunkData.ColorAlpha[2*(i)*64+64+2*j] = 255;
					m_ChunkData.ColorAlpha[2*(i)*64+64+2*j+1] = 255;

					m_ChunkData.ColorMap[2*4*i*64+2*4*j] = colormap[0];
					m_ChunkData.ColorMap[2*4*i*64+2*4*j+1] = colormap[1];
					m_ChunkData.ColorMap[2*4*i*64+2*4*j+2] = colormap[2];
					m_ChunkData.ColorMap[2*4*i*64+2*4*j+3] = colormap[3];

					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4] = colormap[0];
					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4+1] = colormap[1];
					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4+2] = colormap[2];
					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4+3] = colormap[3];

					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j] = colormap[0];
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j+1] = colormap[1];
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j+2] = colormap[2];
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j+3] = colormap[3];

					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4] = colormap[0];
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4+1] = colormap[1];
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4+2] = colormap[2];
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4+3] = colormap[3];

				}
				else
				{
					m_ChunkData.ColorAlpha[2*i*64+2*j] = 255;
					m_ChunkData.ColorAlpha[2*i*64+2*j+1] = 255;
					m_ChunkData.ColorAlpha[2*(i)*64+64+2*j] = 255;
					m_ChunkData.ColorAlpha[2*(i)*64+64+2*j+1] = 255;

					m_ChunkData.ColorMap[2*4*i*64+2*4*j] =  0;
					m_ChunkData.ColorMap[2*4*i*64+2*4*j+1] =  127;
					m_ChunkData.ColorMap[2*4*i*64+2*4*j+2] =  127;
					m_ChunkData.ColorMap[2*4*i*64+2*4*j+3] =  0;

					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4] =  0;
					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4+1] =  127;
					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4+2] =  127;
					m_ChunkData.ColorMap[2*4*i*64+2*4*(j)+4+3] =  0;

					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j] =  0;
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j+1] = 127;
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j+2] =  127;
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*j+3] =  0;

					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4] =  0;
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4+1] =  127;
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4+2] =  127;
					m_ChunkData.ColorMap[2*4*(i)*64+4*64+2*4*(j)+4+3] = 0;
				}
			}
		}
	}

	//更新地图信息（行走层等信息）
	void  EditorTerrainBlock::UpdateMapInfoToColorMap()
	{
		int  nSelectModel = m_InfoData.nSelectMode ;

		assert( nSelectModel>0 && nSelectModel <4 );

		for ( int i = 0 ; i < 64*64 ; i ++ )
		{
			if( m_InfoData.info[ nSelectModel ][i] != 0)
			{
				m_ChunkData.ColorAlpha[i] = 255;
				m_ChunkData.ColorMap[4*i] = m_InfoData.color [nSelectModel][0];
				m_ChunkData.ColorMap[4*i+1] = m_InfoData.color [nSelectModel][1];
				m_ChunkData.ColorMap[4*i+2] = m_InfoData.color [nSelectModel][2];
				m_ChunkData.ColorMap[4*i+3] = m_InfoData.color [nSelectModel][3];
			}
			else
			{
				m_ChunkData.ColorAlpha[i] = 127 ;
				m_ChunkData.ColorMap[4*i] = 0 ;
				m_ChunkData.ColorMap[4*i+1] = 127 ;
				m_ChunkData.ColorMap[4*i+2] = 127 ;
				m_ChunkData.ColorMap[4*i+3] = 127 ;
			}
		}
	}

	void EditorTerrainBlock::createOrUpdateDepthTexture(Ogre::TLiquid *pliquid)
	{
		TerrBlockData *pdata = GetChunkData();

		Ogre::TextureData *ptex = NULL;
		if(pliquid->m_pDepthTex == NULL)
		{
			TextureDesc desc;
			memset(&desc, 0, sizeof(desc));
			desc.type = TEXTURE_2D;
			desc.format = PF_A8;
			desc.width = 64;
			desc.height = 64;
			desc.nmipmap = 1;

			ptex = new TextureData(desc, false);
		}
		else
		{
			assert(IS_KIND_OF(TextureData, pliquid->m_pDepthTex));
			ptex = static_cast<TextureData *>(pliquid->m_pDepthTex);
		}

		LockResult lr;
		uchar *pdestbits = (uchar *)ptex->lock(0, 0, false, lr);
		if(pdestbits)
		{
			float height = m_pLiquid->getHeight();
			float shallowdepth = m_pLiquid->m_ShallowWaterDepth;
			float deepdepth = m_pLiquid->m_DeepWaterDepth;
			if(deepdepth <= shallowdepth) deepdepth = shallowdepth + 100.0f;

			for(int ii=0;ii<64;ii++)
			{
				for(int jj=0;jj<64;jj++)
				{
					float dx = pdata->minVec.x + ii*(pdata->maxVec.x - pdata->minVec.x)/63.0f;
					float dz = pdata->minVec.z + jj*(pdata->maxVec.z - pdata->minVec.z)/63.0f;
					float y;
					GetHeight(dx, dz, &y, true);

					y = height - y; //depth
					float t = (y-shallowdepth) / (deepdepth - shallowdepth);
					t = Clamp(t, 0.0f, 1.0f);

					pdestbits[jj*64+ii] = (uchar)(t*255);
				}
			}

			ptex->unlock(0, 0);
		}

		pliquid->m_pDepthTex = ptex;
	}

	bool EditorTerrainBlock::UpdateLiquid(EditOptionWater &water)
	{
		char filename[256];
		if(water.watertype == 0) 
		{
			sprintf(filename,"scene\\environment\\water\\%s\\%s", water.animtex, water.animtex);
		}

		bool update_depth = false;
		if(water.watertype!=m_LiquidData.type || strcmp(filename, m_LiquidData.name)!=0)
		{
			m_LiquidData.type = water.watertype;
			OGRE_RELEASE(m_pLiquid);
			if(water.watertype < 0)
				return true;
			if(water.watertype==0 && filename[0]==0) 
				return true;

			m_pLiquid = new Ogre::TLiquid;
			if(water.watertype == 0)
			{
				strcpy_s(m_LiquidData.name,256,filename);
				m_pLiquid->m_TextureName.insert(filename,256);

				if (string(water.animtex) == string("magma"))
					m_pLiquid->createGeneralLiquid(filename, 51);
				else
					m_pLiquid->createGeneralLiquid(filename, 30);
			}
			else if (water.watertype == 1)
			{
				assert(water.watertype == 1);
				m_pLiquid->createReflectLiquid("scene\\environment\\water\\LakeWaterNormal.png", "scene\\environment\\water\\bump.dds");
			}
			update_depth = true;
			strcpy(m_LiquidData.name, filename);
		}

		if(water.waterheight != m_LiquidData.waterheight || water.waterdepth[0]!=m_LiquidData.shallowdepth || water.waterdepth[1]!=m_LiquidData.deepdepth)
		{
			m_LiquidData.waterheight = water.waterheight;
			m_LiquidData.shallowdepth = water.waterdepth[0];
			m_LiquidData.deepdepth = water.waterdepth[1];
			update_depth = true;
		}

		m_pLiquid->m_ShallowWaterColor = water.watercolor[0];
		m_pLiquid->m_DeepWaterColor = water.watercolor[1];
		m_pLiquid->m_ShallowWaterDepth = water.waterdepth[0];
		m_pLiquid->m_DeepWaterDepth = water.waterdepth[1];
		m_pLiquid->m_MaxWaterDepth = water.maxwaterdepth;

		m_pLiquid->m_Speed = water.speed;
		m_pLiquid->m_Amp = water.amp;
		m_pLiquid->m_NumTexRepeat = water.ntexrepeat;

		//由于水的高度更新，新的包围盒 ; //注意点，如果有液体后，在修改地形，可能会导致有液体的包围盒出错
		if(GetChunkData ()->maxVec .y < water.waterheight)
			GetChunkData ()->maxVec .y =  water.waterheight;

		if (GetChunkData ()->minVec .y  > water.waterheight)
			GetChunkData ()->minVec  .y =  water.waterheight;

		m_ChunkData.bHaswater = true;

		if(update_depth)
		{
			m_pLiquid->createVBIB(m_ChunkData.gx , m_ChunkData.gy, m_ChunkData.minVec.x , m_ChunkData.minVec.z, m_ChunkData.maxVec.x, m_ChunkData.maxVec.z, water.waterheight);
			createOrUpdateDepthTexture(m_pLiquid);
		}
		return true;
	}

	void EditorTerrainBlock::CreateColorMap()
	{
		if( !m_ChunkData.bColor  )
		{
			m_ChunkData.bColor  = true;

			if( m_ChunkRenderData.colorTex  == NULL )
			{
				m_ChunkRenderData.colorTex = g_TexManager.CreateTex(NULL,64,64,4096, PF_A8R8G8B8, 1);
				memset(  m_ChunkData.ColorMap , 0 , 4*64*64 );
			}
		}
	}

	void   EditorTerrainBlock::UpdateColorMap()
	{
		if( m_ChunkRenderData.colorTex == NULL )
			return;

		g_TexManager.Update(m_ChunkRenderData.colorTex, m_ChunkData.ColorMap, 64, 64, 0); 
	}



	void EditorTerrainBlock::Release() 
	{

		OGRE_DELETE(m_pLiquid);

		OGRE_RELEASE( m_ChunkRenderData.pmtl );
		OGRE_RELEASE( m_ChunkRenderData.vb );
		OGRE_RELEASE( m_ChunkRenderData.ib [0]);
		OGRE_RELEASE( m_ChunkRenderData.ib [1]);

		OGRE_DELETE(m_pQuadPath);

		for( unsigned int i = 0 ; i < m_ChunkPlantData.size () ; i ++)
		{
			OGRE_DELETE( m_ChunkPlantData[i] );
		}

		m_ChunkPlantData.clear ();

		m_RenderPassSet.bDraw.clear();
		m_RenderPassSet.passSet.clear();

		EditorTerrainBlock::PLANT_MAP::iterator iter = m_PlantMap.begin();

		for(  ; iter!=  m_PlantMap.end() ; iter ++ )
		{
			OGRE_DELETE( iter->second ) ;
		}
		m_PlantMap.clear();

	};

	bool  EditorTerrainBlock::SetCullInfoEx(int x,int z, unsigned char nCull , bool bUpdate , int mode )
	{
		if( !bUpdate )
			return false;

		if( m_RenderPassSet.bDraw[ z*m_NewVecData.ngrid + x] == nCull )
			return false;

		if( m_RenderPassSet.bDraw [z*m_NewVecData.ngrid + x] == 1 && g_bLoading )
			return false;

		m_RenderPassSet.bDraw[z*m_NewVecData.ngrid + x] = nCull ;
		
		// 重新创建顶点索引
		CreateRenderPassEx();
		return true;
	}
	//mode == 1 ，表示恢复裁剪
	void  EditorTerrainBlock::SetCullInfo(int x,int z,unsigned char bCull , int mode )
	{

		if( m_ChunkData.vecNum != 145 )
		{
			SetCullInfoEx( x , z , bCull , true , mode );
			return;
		}

		if( m_RenderPassSet.bDraw [z*8 + x ] == bCull )
			return;

		m_RenderPassSet.bDraw [z*8 + x] = bCull;

		CreateRenderPass();
	};

	void   EditorTerrainBlock::UpdateTexRepeat( int nRepeat )
	{

		TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb ->lock();

		for( unsigned int i = 0 ; i < m_NewVecData.vVer.size() ; i ++)
		{
			m_NewVecData.vVer[i].texcoord = m_NewVecData.vVer[i].texcoord2*(float)nRepeat ; 
		}
		m_ChunkRenderData.vb ->unlock();

	}

	float EditorTerrainBlock::GetGridPosX(float x)
	{
		{
			return x-m_NewVecData.vVer[0].pos.x ;
		}
	}
	float EditorTerrainBlock::GetGridPosY(float y)
	{

		return y-m_NewVecData.vVer[0].pos.y ;
	}
	float EditorTerrainBlock::GetGridPosZ(float z)
	{

		return z-m_NewVecData.vVer[0].pos.z ;
	}

	bool  EditorTerrainBlock::SavePlantFile(const char* filename)
	{
		MChunkPlantHead_st head;

		head.numKind = m_PlantMap.size();

		if( head.numKind == 0 )
			return true;

		EditorStream  stream;
		stream.WriteFile( filename );

		stream.Write( &head ,sizeof(MChunkPlantHead_st) );

		PLANT_MAP::iterator iter = m_PlantMap.begin() ;

		MChunkPlantInfo_st plantInfo;

		for( iter ; iter != m_PlantMap.end () ; iter ++ )
		{

			strcpy( plantInfo.plantFileName , iter->first.c_str() );

			plantInfo.numPos = iter->second ->m_nCurVecNum / 4 ;

			stream.Write( &plantInfo , sizeof( MChunkPlantInfo_st) );

			for ( int i = 0 ; i < plantInfo.numPos ; i ++ )
			{
				stream.WriteFloat( &iter->second ->m_SavePosSet[i].x  );
				stream.WriteFloat( &iter->second ->m_SavePosSet[i].y  );
				stream.WriteFloat( &iter->second ->m_SavePosSet[i].z  );

				stream.WriteFloat( &iter->second->m_SaveScaleSet[i] );
				stream.Write( &iter->second->m_unSaveColorSet[i] , sizeof( int ));
			}
		}

		stream.WriteEnd();

		return false;

	};

	bool  EditorTerrainBlock::LoadPlantFile(const char* filename)
	{
		EditorStream stream;
		if ( !stream.ReadFile( filename ) )
			return true;

		MChunkPlantHead_st head;

		stream.Read( &head ,sizeof( MChunkPlantHead_st ));

		MChunkPlantInfo_st plantInfo;
		for( int i = 0 ; i < head.numKind ; i ++ )
		{
			stream.Read( &plantInfo , sizeof(MChunkPlantInfo_st ) );

			for( int j = 0 ; j < plantInfo.numPos ; j ++ )
			{
				Ogre::Vector3 pos ;
				float scale;
				unsigned int color;

				stream.ReadFloat( &pos.x );
				stream.ReadFloat( &pos.y );
				stream.ReadFloat( &pos.z );

				ScaleMapPoint(pos);

				stream.ReadFloat( &scale );
				stream.Read( &color , sizeof( unsigned int ) );

				AddPlant( plantInfo.plantFileName , pos , scale , color );

			}
		}

		EditorTerrainBlock::PLANT_MAP::iterator iter = m_PlantMap.begin();

		for( ; iter != m_PlantMap.end() ; iter ++ )
		{
			iter->second->UpdateDataEx_1();
		}

		return true;
	};	

	void   EditorTerrainBlock::SetLightColor( unsigned int color )
	{	

		TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb ->lock();

		for( size_t i = 0 ; i < m_NewVecData.vVer.size ()  ; i ++ )
		{
			m_NewVecData.vVer[i].color = color;
			vdata[i].color = color ;
		}

		m_ChunkRenderData.vb ->unlock();

		//如果有植物，则算植物的顶点颜色
		EditorTerrainBlock::PLANT_MAP ::iterator iter = m_PlantMap.begin();

		for( ; iter != m_PlantMap.end() ; iter ++ )
		{
			iter->second ->ClearLightColor();
		}
	};

	void  EditorTerrainBlock::UpdatePlantLightColor()
	{
		//如果有植物，则算植物的顶点颜色
		EditorTerrainBlock::PLANT_MAP ::iterator iter = m_PlantMap.begin();

		for( ; iter != m_PlantMap.end() ; iter ++ )
		{
			iter->second ->UpdateLightColor();
		}
	}

	void	EditorTerrainBlock::UpdatePlantHeight()
	{
		//如果有植物，则更新植物顶点位置
		EditorTerrainBlock::PLANT_MAP ::iterator iter = m_PlantMap.begin();
		for( ; iter != m_PlantMap.end() ; iter ++ )
		{
			iter->second->UpdateHeight(this);
		}

	}

	void  EditorTerrainBlock::CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity)
	{
		// 点光源
		if(lighttype == 0)
		{ // 如果是点光源，且不影响当前地形块
			if( range <= 0 )
				return;

			if( pos.x - range > m_ChunkData.maxVec.x )
				return ;

			if( pos.z - range > m_ChunkData.maxVec.z )
				return ;

			if( pos.x + range < m_ChunkData.minVec.x )
				return;

			if( pos.z + range < m_ChunkData.minVec.z )
				return;
		}

		// 烘焙地形
		float drange = range*range ;
		TerrainVertex* vdata = (TerrainVertex*)m_ChunkRenderData.vb ->lock();

		for( size_t i = 0 ; i < m_NewVecData.vVer.size ()  ; i ++ )
		{
			float strength;
			Ogre::Vector3 normal = m_NewVecData.vVer[i].normal ;
			Normalize( normal );

			if(lighttype == 0)
			{
				Ogre::Vector3 pos0 = vdata[i].pos ;
				Ogre::Vector3 dst = pos - pos0 ;

				float fLength = dst.lengthSqr() ;
				if( fLength >= drange )
				{
					continue;
				}

				Normalize(dst);
				float fdot = DotProduct( normal , dst ) ;
				if( fdot <= 0 )
				{
					continue;
				}
				strength =  sqrt( fLength ) / range ;
				strength = 1.0f - strength * strength ;
				if( strength < 0 ) 
					strength = 0 ;
				strength *= fdot;
			}
			else
			{
				strength = DotProduct(normal, pos);
				if(strength <= 0)
				{
					continue;
				}

				if(shadowdensity > 0)
				{
					WorldRay ray;
					ray.m_Origin = vdata[i].pos;
					ray.m_Dir = pos;
					ray.m_Range = 100000.0f;
					if(g_Terrain.pickObject(INTERSECT_LIGHT, ray, NULL, 0xffffffff))
					{
						strength *= 1.0f - shadowdensity;
					}
				}
			}

			ColourValue vertcolor;
			vertcolor.setAsARGB(m_NewVecData.vVer[i].color);
			ColourValue lighting = strength*color + ambient + vertcolor;
			ColorQuad cq(lighting);

			m_NewVecData.vVer[i].color = cq.c;
			vdata[i].color = m_NewVecData.vVer[i].color;
		}	

		m_ChunkRenderData.vb ->unlock();

		//如果有植物，则算植物的顶点颜色
		EditorTerrainBlock::PLANT_MAP ::iterator iter = m_PlantMap.begin();

		for( ; iter != m_PlantMap.end() ; iter ++ )
		{
			iter->second ->CountLightColor(lighttype, pos , color , ambient, range, shadowdensity);
		}

		// 烘焙所有模型
		for (int i=0; i<(int)m_ModelSet.size(); i++)
		{
			m_ModelSet[i]->CountLightColor(lighttype, pos , color , ambient, range, shadowdensity);
		}
	};

	//判断每个格子是否有水
	bool  EditorTerrainBlock::GetGridIfWater( int mx , int my )
	{
		if ( m_ChunkData.vecNum == 145 )
			return false;

		if( !m_ChunkData.bHaswater )
			return false;

		float height = m_pLiquid->getHeight();
		if ( m_NewVecData.vVer[mx + my*(m_NewVecData.ngrid+1) ].pos .y 
			< height )
			return true;

		if ( m_NewVecData.vVer[mx + 1 + my*(m_NewVecData.ngrid+1) ].pos .y 
			< height )
			return true;

		if ( m_NewVecData.vVer[mx + (my+1)*(m_NewVecData.ngrid+1) ].pos .y 
			< height )
			return true;

		if ( m_NewVecData.vVer[mx + 1 + (my+1)*(m_NewVecData.ngrid+1) ].pos .y 
			< height )
			return true;

		return false;

	}
}

