#include ".\mterrainrender.h"

//#include "vld.h"


#include "mcamera.h"
#include "mterrainedit.h"

#include "mstream.h"

#include "OgreLog.h"
#include "OgreSceneRenderer.h"
#include "OgreShaderContext.h"
#include "OgreCamera.h"
#include "OgreRenderSystem.h"

using namespace Ogre;
using namespace TE;

unsigned int  g_ulFogColor;

extern unsigned long g_dwCurTime;

MTerrainRender  g_TerrainRender;


void  MessageLogSave( void* pData , const char* info )
{
	if( pData== NULL )
	{
		char logInfo[256];
		sprintf( logInfo , "%s load failed!" , info );
		//MessageBox( NULL , logInfo  ,"提示 ", MB_OK );
		LOG_SEVERE( "%s load failed !" , info );
	}
}

MTerrainRender::MTerrainRender(void)
{
	memset( &m_ChunkShader , 0 , sizeof( MShader_t ) );
	memset( &m_LiquidShader, 0 ,sizeof( MShader_t ) );
	memset( &m_DrawOption , 0 , sizeof( MDrawOption) );

	m_DrawOption.bDrawChunk = true;
	m_DrawOption.bDrawModel = true;
	m_DrawOption.bDrawQuad = true;
	m_DrawOption.bDrawLiquid = true;

	m_fAdd.x = 8.0f;
	m_fAdd.y = 8.0f;
	m_fAdd.z = 8.0f;

	m_DrawChunkMode = DRAW_CHUNK_HL ;
	
	m_DrawChunkFillMode = DRAW_SOLID ;

	m_DrawOption.bUserColorMap = true;

	m_nDrawTriNum = 318 ;
}

MTerrainRender::~MTerrainRender(void)
{
	Release();
}
	
void MTerrainRender::Init()
{
	/*
	m_ChunkOneHLLayerShader.pmtl = new Material("terrain_1layer");
	m_ChunkFourLayerShader.pmtl = new Material("terrain_4layer");
	m_ChunkThreeLayerShader.pmtl = new Material("terrain_3layer");
	m_ChunkOneLayerShader.pmtl = new Material("terrain_1layer");
	m_ChunkTowLayerShader.pmtl = new Material("terrain_1layer");
	m_ChunkBaseShader.pmtl = new Material("terrain_1layer");

    //绘制，阻挡层
	m_ChunkColorMapShader.pmtl = new Material("terrain_1layer");
	m_ChunkBaseColorShader.pmtl = new Material("terrain_1layer");
	m_ChunkColorShader.pmtl = new Material("terrain_1layer");
	m_ChunkShader.pmtl = new Material("terrain_1layer");
	*/

	VertexFormat vertfmt;
	vertfmt.addElement(VET_FLOAT3, VES_POSITION);
	vertfmt.addElement(VET_FLOAT3, VES_NORMAL);
	vertfmt.addElement(VET_COLOUR, VES_COLOR);
	vertfmt.addElement(VET_FLOAT2, VES_TEXCOORD);
	vertfmt.addElement(VET_FLOAT2, VES_TEXCOORD, 1);

	m_ChunkShader.pLayout = RenderSystem::getSingleton().getInputLayout(vertfmt);
	m_LiquidShader.pmtl = new Material("stdmtl");

	VertexFormat vertfmt2;
	vertfmt2.addElement(VET_FLOAT3, VES_POSITION);
	vertfmt2.addElement(VET_FLOAT3, VES_NORMAL);
	vertfmt2.addElement(VET_FLOAT2, VES_TEXCOORD);

    m_LiquidShader.pLayout = RenderSystem::getSingleton().getInputLayout(vertfmt2);
    assert( m_LiquidShader.pLayout );

		short *defstrip = new short[TERRBLOCK_STRIPSIZE];
		
		for (int i=0; i<TERRBLOCK_STRIPSIZE; i++) defstrip[i] = i; 
		short *mapstrip = new short[TERRBLOCK_STRIPSIZE];
		stripifyEx<short>(defstrip, mapstrip);
		delete[] defstrip;

		m_ChunkShader.ib = new IndexData(TERRBLOCK_STRIPSIZE);
		m_ChunkShader.ib->setVertexRange(0, TERRBLOCK_NUMVERT0);
		
		unsigned short*  idata = (unsigned short*)m_ChunkShader.ib->lock();
	   memcpy(idata,mapstrip,sizeof(unsigned short)*TERRBLOCK_STRIPSIZE);
	   m_ChunkShader.ib->unlock();

	   delete []  mapstrip ;


	   m_ChunkBaseShader.pLayout = m_ChunkShader.pLayout ;

	   //画线部分渲染
	   VertexFormat vertfmt3;
	   vertfmt3.addElement(VET_FLOAT3, VES_POSITION);
	   vertfmt3.addElement(VET_COLOUR, VES_COLOR);

	   m_LineShader.pLayout = RenderSystem::getSingleton().getInputLayout(vertfmt3);
	   m_LineShader.pmtl = new Material("line");
}
	
void  MTerrainRender::SetTerrain(MTerrain *pTerrain)
{
     assert( pTerrain );

	 m_pTerrain = pTerrain;
};

void MTerrainRender::doRender()
{
	//m_Cull.Update(TickToTime(dtick));
	m_Cull.Cull(m_pTerrain);

	ShaderEnvData envdata;

	envdata.eyepos = m_pCamera->getPosition().toVector3();
	envdata.view = m_pCamera->getViewMatrix();
	envdata.proj = m_pCamera->getProjectMatrix();
	envdata.viewproj = envdata.view * envdata.proj;

	size_t i;
	for(i=0; i<m_Cull.m_ShowChunkNum; i++)
	{
		m_Cull.m_ShowChunk[i]->render(this, envdata);
	}

	for(i=0; i<m_Cull.m_ShowModelNum; i++)
	{
		//m_Cull.m_ShowModel[i]->render(this, envdata);
	}
}
	
void  MTerrainRender::Update(unsigned int dtick)
{	
};

 
void  MTerrainRender::Release()
{
     OGRE_RELEASE( m_ChunkShader.ib   );
	 OGRE_RELEASE( m_ChunkShader.ib1 );
	 
	 OGRE_RELEASE( m_ChunkShader.pmtl  );
	 OGRE_RELEASE( m_LiquidShader.pmtl );
	 OGRE_RELEASE( m_ChunkBaseShader.pmtl );
	 OGRE_RELEASE( m_ChunkNoDetailShader.pmtl  );
	 OGRE_RELEASE( m_LineShader.pmtl );

	 OGRE_RELEASE( m_ChunkOneLayerShader.pmtl );
	 OGRE_RELEASE( m_ChunkTowLayerShader.pmtl );
	 OGRE_RELEASE( m_ChunkThreeLayerShader.pmtl );
	 OGRE_RELEASE( m_ChunkFourLayerShader.pmtl  );

	 OGRE_RELEASE( m_ChunkOneHLLayerShader.pmtl );
	 OGRE_RELEASE( m_ChunkBaseColorShader.pmtl );
	 OGRE_RELEASE( m_ChunkColorShader.pmtl );

	 OGRE_RELEASE( m_ChunkColorMapShader.pmtl );
};

	
void  MTerrainRender::DrawChunkPer()
{
	/*
	IShader *pshader = m_ChunkShader.pmtl;

	if( pshader == NULL)
		return;

	MVec4  fogcolor(0.2f , 0.2f ,0.2f ,0.0f );

	Vector4 color,ambientColor;	
	Vector4 vp;
	color.w = color.x = color.y = color .z = 1.0f;
	ambientColor.x = ambientColor.y = ambientColor.z = ambientColor.w = 0.5f;

	MMatrix WorldViewProj = g_pCamera->GetViewMatrix() * g_pCamera->GetProjectMatrix();
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	Vector3  dir = g_AmbientInfo.LightDirect;
	pmtl->setParamValue("g_LightDir",&g_AmbientInfo.LightDirect);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor );
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);

	pshader->SetConstant("g_ViewPoint", &vp);

    pmtl->setParamValue("g_FogColor" , &fogcolor );
	pmtl->setParamValue("g_CameraPos" , &g_Camera.GetPosition () );


	pshader = m_ChunkBaseShader.pshader;
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);

    pmtl->setParamValue("g_FogColor" , &fogcolor );
	pmtl->setParamValue("g_CameraPos" , &g_Camera.GetPosition () );

	//增加地图编辑层
	pshader = m_ChunkColorMapShader.pshader;

	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);

	pmtl->setParamValue("g_FogColor" , &fogcolor );
	pmtl->setParamValue("g_CameraPos" , &g_Camera.GetPosition () );


	pshader = m_ChunkOneLayerShader.pshader;
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);


	pshader = m_ChunkTowLayerShader.pshader;
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);


	pshader = m_ChunkThreeLayerShader.pshader;
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);

	pshader = m_ChunkFourLayerShader.pshader;
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);

	pshader = m_ChunkOneHLLayerShader.pshader;
   
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue("g_LightDir",&dir);
	pmtl->setParamValue("g_AmibentColor",&g_AmbientInfo.ambientColor);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);
	pshader->SetConstant("g_ViewPoint", &vp);
	pmtl->setParamValue("g_CameraPos" , &g_Camera.GetPosition () );
	pmtl->setParamValue("g_fogColor" ,&g_AmbientInfo.fogColor ); 
	pmtl->setParamValue ("g_LightStrength" , &g_AmbientInfo.fLightStrength );
	*/

};
	
void  MTerrainRender::DrawChunkShadow(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{	
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();
		
		if( pData->bShadow && pOptionData->bShowShadow )
		{
			Material *pmtl = pRenderData->pmtl;
			pmtl->setParamTexture("g_BaseTex", pRenderData->shadowTex);
			pmtl->setParamTexture("g_AlphaTex",pRenderData->shadowTex );
			ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, m_ChunkShader.pLayout, pRenderData->vb, NULL, PRIM_TRIANGLESTRIP, 0, false);
			DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

            DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod,m_DrawChunkFillMode);		
		}
	}
}

void  MTerrainRender::DrawChunkOtherLayer(SceneRenderer* prenderer, const ShaderEnvData &envdata, int layer)
{
	LMTex    pSaveTex = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();
				
		if( layer > pOptionData->showLayer)		
			break;
		
		Material *pmtl = pRenderData->pmtl;
		pmtl->setParamValue("g_Repeat", &pData->texUVInfo[layer]);

		if( layer == 0 )
		{
			pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex [layer]);
			pmtl->setParamTexture("g_AlphaTex",NULL);
			ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLESTRIP, 0, false);
			DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

            DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod,m_DrawChunkFillMode);	
			continue;
		}
			
		if( pRenderData->terrainTex [layer] != NULL )			
		{		
			if( pRenderData->terrainTex [layer] != pSaveTex )
			{
			    pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex [layer]);
				pSaveTex = pRenderData->terrainTex [layer];
			}
		
			if( pData->bColor )
			{
	       		pmtl->setParamTexture("g_ColorTex",pRenderData->colorTex );
			}
			else
			{
	       		pmtl->setParamTexture("g_ColorTex",NULL );
			}

			pmtl->setParamTexture("g_AlphaTex",pRenderData->alphaTex[layer-1]);
			ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLESTRIP, 0, false);
			DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

			DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod,m_DrawChunkFillMode);
		}
	}
};

void  MTerrainRender::DrawChunkWidthHightLight(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{
	MVec3   lightPos;

	lightPos.x += g_Camera.m_Pos .x ;
	lightPos.z += g_Camera.m_Pos .z ;
	lightPos.y += g_Camera.m_Pos .y ;

	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();

		if ( pRenderData->vb == NULL )
			continue;

		/*
		if( pData->texNum == 1 )
		{ 	
			{
				pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex[0]);   
				pmtl->setParamValue("g_CameraPos" , &g_Camera.m_Pos    );
				pmtl->setParamValue("g_fAdd" , &m_fAdd );
				pmtl->setParamValue("g_UV_0" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo[0]);
			}
		}
     	else if( pData->texNum == 2)
		{
			{
				pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex[0]);   
			    pmtl->setParamTexture("g_BaseTex1", pRenderData->terrainTex[1]);  
			    pmtl->setParamTexture("g_AlphaTex", pRenderData->alphaTex [0]);
				pmtl->setParamValue("g_CameraPos" , &g_Camera.m_Pos   );
				pmtl->setParamValue("g_fAdd" , &m_fAdd );
				pmtl->setParamValue("g_UV_0" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo[0] );
				pmtl->setParamValue("g_UV_1" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo[1] );
			}

		}
		else if ( pData->texNum == 3)
		{		
			pmtl->setParamTexture("g_BaseTex",  pRenderData->terrainTex[0]);  
			pmtl->setParamTexture("g_BaseTex1", pRenderData->terrainTex[1]); 
			pmtl->setParamTexture("g_BaseTex2", pRenderData->terrainTex[2]);

			pmtl->setParamTexture("g_AlphaTex",  pRenderData->alphaTex [0]);   
			pmtl->setParamTexture("g_AlphaTex1", pRenderData->alphaTex [1]); 
				pmtl->setParamValue("g_CameraPos" ,&g_Camera.m_Pos    );
				pmtl->setParamValue("g_fAdd" , &m_fAdd );

				pmtl->setParamValue("g_UV_0" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo[0]);
				pmtl->setParamValue("g_UV_1" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo[1] );
				pmtl->setParamValue("g_UV_2" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo[2] );
		}
		else if( pData->texNum == 4)
		{		
			pmtl->setParamTexture("g_BaseTex",  pRenderData->terrainTex[0]);  
			pmtl->setParamTexture("g_BaseTex1", pRenderData->terrainTex[1]); 
			pmtl->setParamTexture("g_BaseTex2", pRenderData->terrainTex[2]);
			pmtl->setParamTexture("g_BaseTex3", pRenderData->terrainTex[3]);

			pmtl->setParamTexture("g_AlphaTex",  pRenderData->alphaTex [0]);   
			pmtl->setParamTexture("g_AlphaTex1", pRenderData->alphaTex [1]);
			pmtl->setParamTexture("g_AlphaTex2", pRenderData->alphaTex [2]);
				pmtl->setParamValue("g_CameraPos" , &g_Camera.m_Pos  );
				pmtl->setParamValue("g_fAdd" , &m_fAdd );

			pmtl->setParamValue("g_UV_0" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo [0] );
			pmtl->setParamValue("g_UV_1" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo [1] );
			pmtl->setParamValue("g_UV_2" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo [2] );
			pmtl->setParamValue("g_UV_3" , &m_Cull.m_ShowChunk [i]->GetChunkData ()->texUVInfo [3] );
		}

		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLESTRIP, 0, false);
		DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

        if( pData->vecNum == 145 )
	    	DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
		else
			DrawTerainMesh(pcontext, pRenderData , m_DrawChunkFillMode );
			*/
	}
}

void  MTerrainRender::DrawChunkWidthPS(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{
	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();

		/*
		if(pRenderData->mtlflags != pData->texNum)
		{
			pRenderData->mtlflags = pData->texNum;
			OGRE_RELEASE(pRenderData->pmtl);
			pRenderData->pmtl = new Material()
		}*/
		Material *pmtl = pRenderData->pmtl;

		if( pData->texNum == 1 )
		{
			pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex[0]);
			if( pRenderData->terrainTex [0] != pCurTexture )
			{
				pCurTexture = pRenderData->terrainTex [0];
			}
	
		}
     	else if( pData->texNum == 2)
		{
			pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex[0]);  
			pmtl->setParamTexture("g_BaseTex1", pRenderData->terrainTex[1]);  
			pmtl->setParamTexture("g_AlphaTex", pRenderData->alphaTex [0]);  
		}
		else if ( pData->texNum == 3)
		{
			//pshader = m_ChunkTowLayerShader.pshader;	

			pmtl->setParamTexture("g_BaseTex",  pRenderData->terrainTex[0]);  
			pmtl->setParamTexture("g_BaseTex1", pRenderData->terrainTex[1]); 
			pmtl->setParamTexture("g_BaseTex2", pRenderData->terrainTex[2]);

			pmtl->setParamTexture("g_AlphaTex",  pRenderData->alphaTex [0]);   
			pmtl->setParamTexture("g_AlphaTex1", pRenderData->alphaTex [1]); 

		}
		else if( pData->texNum == 4)
		{
			//pshader = m_ChunkFourLayerShader.pshader;	

			pmtl->setParamTexture("g_BaseTex",  pRenderData->terrainTex[0]);  
			pmtl->setParamTexture("g_BaseTex1", pRenderData->terrainTex[1]); 
			pmtl->setParamTexture("g_BaseTex2", pRenderData->terrainTex[2]);
			pmtl->setParamTexture("g_BaseTex3", pRenderData->terrainTex[3]);

			pmtl->setParamTexture("g_AlphaTex",  pRenderData->alphaTex [0]);   
			pmtl->setParamTexture("g_AlphaTex1", pRenderData->alphaTex [1]);
			pmtl->setParamTexture("g_AlphaTex2", pRenderData->alphaTex [2]);

		}

		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
		DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);
		
		if( pData->vecNum == 145)
	    	DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
		else
			DrawTerainMesh(pcontext, pRenderData , m_DrawChunkFillMode );
	}
}
	
void  MTerrainRender::DrawWater(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata)
{
	if ( m_DrawOption.bDrawLiquid )
	{
		DrawLiquidPer();
		DrawLiquid(prenderer, envdata);
	}
};
	
void  MTerrainRender::DrawPlant(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata)
{
	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		MChunk::PLANT_MAP::iterator iter = m_Cull.m_ShowChunk[i]->m_PlantMap.begin();
        
		for(  ; iter!=  m_Cull.m_ShowChunk[i]->m_PlantMap.end() ; iter ++ )
		{
			iter->second->RenderEx(prenderer, envdata);
			//iter->second->RenderEx();
		}
	}
}

void  MTerrainRender::DrawAlphaMap(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{
	if( m_DrawOption.bDrawQuad )
	{
		DrawLinePer();
		DrawLine(prenderer, envdata);
	}

	if ( !m_DrawOption.bUserColorMap )
		return;

	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();

		if( pRenderData->colorTex == NULL )
			continue ;

		Material *pmtl = pRenderData->pmtl;

		float uvInfo = 1.0f;
		pmtl->setParamValue( "g_Repeat" , &uvInfo );
	
		pmtl->setParamTexture("g_BaseTex", pRenderData->colorTex );   

		if( pData->vecNum == 145 )
		{
			//DrawTerainMesh(pshader,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
		}		 
		else		 
		{	
			//这里暂时使用0，0的CHUNK的三角行数，及对应索引。
		
			if( g_TerrainEdit.GetEditData() )
			{            
				TerrBlockRenderData*  pRenderDataFirst = g_TerrainEdit.GetEditData()->chunkSet[0][0].GetChunkRenderData();
				ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, pRenderDataFirst->ib[1], PRIM_TRIANGLESTRIP, pRenderDataFirst->triNum, false);
				DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);
			}
			//DrawTerainMesh( pshader , pRenderData , m_DrawChunkFillMode );
		}

		//DrawTerainMesh(pshader,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
	}
};
	
void MTerrainRender::DrawChunkNoColorFirstLayer(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{
	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();

		Material *pmtl = pRenderData->pmtl;
		pmtl->setParamValue( "g_Repeat" , &pData->texUVInfo[0] );

		if( pRenderData->terrainTex [0] != pCurTexture )
		{
			pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex[0]);   
			pCurTexture = pRenderData->terrainTex [0];
		}

		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
		DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

		if( pData->vecNum == 145 )
	    	DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
		else
			DrawTerainMesh(pcontext, pRenderData , m_DrawChunkFillMode );
	}

};
	
void MTerrainRender::DrawChunkNoColorOtherLayer(SceneRenderer* prenderer, const ShaderEnvData &envdata, int layer)
{
	LMTex    pSaveTex = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();
				
		if( layer > pOptionData->showLayer)		
			break;
		
		Material *pmtl = pRenderData->pmtl;
		pmtl->setParamValue( "g_Repeat" , &pData->texUVInfo[layer] );

		if( layer == 0 )
		{	
			ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
			DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);
	
			pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex [layer]);
			pmtl->setParamTexture("g_AlphaTex",NULL);
			DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod,m_DrawChunkFillMode);
			continue;
		}
			
		if( pRenderData->terrainTex [layer] != NULL )			
		{
			ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
			DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

			if( pRenderData->terrainTex [layer] != pSaveTex )
			{
			    pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex [layer]);
				pSaveTex = pRenderData->terrainTex [layer];
			}

			pmtl->setParamTexture("g_AlphaTex",pRenderData->alphaTex[layer-1]);
		
			if( pData->vecNum == 145 )
	    	   DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
		   else
			   DrawTerainMesh(pcontext, pRenderData , m_DrawChunkFillMode );		}
	}
};

void  MTerrainRender::DrawChunkFirstLayer(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{
	//pshader= m_ChunkBaseShader.pshader;	
	//pshader = m_ChunkBaseColorShader.pshader ;
	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();

		Material *pmtl = pRenderData->pmtl;
		pmtl->setParamValue( "g_Repeat" , &pData->texUVInfo[0] );
		if( pRenderData->terrainTex [0] != pCurTexture )
		{
			pmtl->setParamTexture("g_BaseTex", pRenderData->terrainTex[0]);   
			pCurTexture = pRenderData->terrainTex [0];
		}

		if ( pData->bColor )
		{
			pmtl->setParamTexture("g_ColorTex", pRenderData->colorTex );   
		}
		else
		{
			pmtl->setParamTexture("g_ColorTex", NULL );  
		}

		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
		DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

		DrawTerainMesh(pcontext,pRenderPass,m_DrawOption.nDrawLod, m_DrawChunkFillMode );
	}
}

void  MTerrainRender::DrawChunkLine(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{
	LMTex pCurTexture = NULL;
	for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum  ; i++)
	{
		TerrBlockRenderData*  pRenderData = m_Cull.m_ShowChunk[i]->GetChunkRenderData ();
		TerrBlockOption*  pOptionData = m_Cull.m_ShowChunk[i]->GetChunkOptionData ();
		TerrBlockData*        pData = m_Cull.m_ShowChunk[i]->GetChunkData ();
		MChunkRenderPassSet*  pRenderPass = m_Cull.m_ShowChunk[i]->GetRenderPassSet ();

		Material *pmtl = pRenderData->pmtl;

		pmtl->setParamValue( "g_Repeat" , &pData->texUVInfo[0] );
		pmtl->setParamTexture("g_BaseTex", NULL ); 

		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, pRenderData->pLayout, pRenderData->vb, NULL, PRIM_TRIANGLELIST, 0, false);
		DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

		DrawTerainMesh(pcontext, pRenderPass,m_DrawOption.nDrawLod, DRAW_WIREFRAME );
	}
}

void MTerrainRender::DrawTerainMesh(Ogre::ShaderContext *pcontext, MChunkRenderPassSet *pRenderPassSet,int lod , int nPass)
{
}

void MTerrainRender::DrawTerainMesh(Ogre::ShaderContext *pcontext, TerrBlockRenderData* pRender , int nPass )
{
}

void  MTerrainRender::DrawChunk(SceneRenderer* prenderer, const ShaderEnvData &envdata)
{

	switch( m_DrawChunkMode )
	{
	case DRAW_CHUNK_VS:
		{
			/*if( m_DrawOption.bUserColorMap  )
			{
				DrawChunkFirstLayer();

				for(int i = 1 ; i < 4 ; i++)
				{
					DrawChunkOtherLayer( i );
				}
			}
			else*/
			{
				DrawChunkNoColorFirstLayer(prenderer, envdata);

				for(int i = 1 ; i < 4 ; i++)
				{
					DrawChunkNoColorOtherLayer(prenderer, envdata, i);
				}
			}
		}
		break;
	case DRAW_CHUNK_PS:
		{
           DrawChunkWidthPS(prenderer, envdata);
		}
		break;
	case DRAW_CHUNK_HL:
		{
           DrawChunkWidthHightLight(prenderer, envdata);
		}
		break;
	}

 	DrawChunkShadow(prenderer, envdata);
};

	
void  MTerrainRender::DrawLiquidPer()
{	
	//IShader *pshader = m_LiquidShader.pshader;

	/*
	if( pshader == NULL )
		return;

	MMatrix WorldViewProj = g_pCamera->GetViewMatrix() * g_pCamera->GetProjectMatrix();
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	pmtl->setParamValue( "g_fogNear" , &g_AmbientInfo.fogNear );
	pmtl->setParamValue( "g_fogLong", &g_AmbientInfo.fogFar ); 
	pmtl->setParamValue( "g_fogColor" , &g_AmbientInfo.fogColor  );

	pshader->SetRawConstant( "g_fogColor" , &g_ulFogColor , 0 , sizeof( unsigned long ) );
	*/
};


void  MTerrainRender::DrawLiquid(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata)
{
	LMTex pTex = NULL;
	MVec4 curColor( 0.5f ,0.5f ,0.5f ,0.5f);

//	IShader *pshader = m_LiquidShader.pshader;
	/*
    for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum ; i++)
	{
        if( m_Cull.m_ShowChunk [i]->GetChunkData ()->bHaswater  )
		{	
			int frame = int(g_dwCurTime / m_Cull.m_ShowChunk [i]->GetChunkLiquid ()->m_fAniTime ) % 30;

			Material *pmtl = NULL;
			if( m_Cull.m_ShowChunk[i]->GetChunkLiquid ()->m_pTexSet [frame] != pTex)
			{
				pTex = m_Cull.m_ShowChunk [i]->GetChunkLiquid ()->m_pTexSet [frame] ; 
				pmtl->setParamTexture ( 	"g_BaseTex", pTex );
			}
			if ( curColor != m_Cull.m_ShowChunk [i]->GetChunkLiquid ()->m_Color )
			{
				curColor = m_Cull.m_ShowChunk [i]->GetChunkLiquid ()->m_Color;
	            pmtl->setParamValue("g_LightColor", &curColor);
			}

			ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, pmtl, m_LiquidShader.pLayout, m_Cull.m_ShowChunk [i]->GetChunkLiquid ()->m_pVB, m_ChunkShader.ib, PRIM_TRIANGLESTRIP, 0, true);
			DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);
		}
	}
	*/
};
   
void  MTerrainRender::DrawModel()
{
	if ( !m_DrawOption.bDrawModel )
		return;


	ShaderEnvData envdata;
	for( int i = 0 ; i < (int)m_Cull.m_ShowModelNum ; i++ )
	{
//		m_Cull.m_ShowModel [i]->render(this, envdata);
	}
};

void MTerrainRender::DrawLinePer()
{ 	
	/*
	IShader *pshader = m_LineShader.pshader;
	if( pshader == NULL)
		return;

	MMatrix WorldViewProj = g_pCamera->GetViewMatrix() * g_pCamera->GetProjectMatrix();
	pshader->SetConstant("g_WorldViewProj", &WorldViewProj);
	*/

};
		
void  MTerrainRender::CreateIB( int nSize )
{
	OGRE_RELEASE( m_ChunkShader.ib1 );
    	
	int nStripNum = (nSize-1)*2*nSize + 2*(nSize-1-1)  ;

	m_nDrawTriNum = nStripNum ;

	unsigned short *stripsize2 = new unsigned short [nStripNum];

	int index = 0;
	for( int i = 0 ; i < nSize - 1 ; i ++ )
	{
		for( int j = 0 ;j < nSize ; j ++ )
		{
			stripsize2[index] = i*nSize + j;
			stripsize2[index+1] = (i+1)*nSize + j ;
			index += 2;
		}

		if( i!= nSize - 2 )
		{
			stripsize2[index] = (i+1)*nSize + nSize-1 ;
			stripsize2[index+1] = (i+1)*nSize; 
			index += 2;
		}
	}
	
	m_ChunkShader.ib1 = new IndexData(nStripNum);
	m_ChunkShader.ib1->setVertexRange(0, nSize*nSize); 
	
	unsigned short* idata = (unsigned short*)m_ChunkShader.ib1->lock();
	memcpy(idata,stripsize2,sizeof(unsigned short)*nStripNum);
	m_ChunkShader.ib1->unlock();
		
	delete[] stripsize2;

	return;
};

void MTerrainRender::DrawLine(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata)
{	
	//IShader *pshader = m_LineShader.pshader;
     for( int i = 0 ; i < (int)m_Cull.m_ShowChunkNum; i++)
	 {          	
		 MPath*  pLine = m_Cull.m_ShowChunk[i]->GetQuadLine ();
         if( pLine == NULL)
			 continue;

		 Material *pmtl = NULL;
		 //prenderer->newContext(RL_SCENE, envdata, pmtl, m_LineShader.pLayout, pLine->GetPathRenderData()->vb, NULL, PRIM_LINESTRIP, pLine->GetPathData ()->data.size(), false);
	 }
}
