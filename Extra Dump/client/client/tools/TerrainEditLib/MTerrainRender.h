
#pragma once
#include "mterrain.h"
#include "gchunk.h"

#include "mterraincull.h"

#include "OgreSceneRenderer.h"

namespace TE
{
 

template <class V>
void stripifyEx(V *in, V *out)
{
	for (int row=0; row<8; row++) {
		V *thisrow = &in[indexMapBuf(0,row*2)];
		V *nextrow = &in[indexMapBuf(0,(row+1)*2)];

		if (row>0) *out++ = thisrow[0];
		for (int col=0; col<9; col++) {
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}
		if (row<7) *out++ = nextrow[8];
	}
};

struct MShader_t
{
	LMIB           ib;
	LMIB           ib1;
	MIInputLayout  pLayout;
	//MIShader       *pshader;
	Ogre::Material *pmtl;
};

struct MDrawOption
{
    bool   bDrawChunk;
	bool   bDrawModel;
	bool   bDrawQuad;
	bool   bDrawLiquid;
	bool   bUserColorMap;
	bool   bDrawPlant;
	int    nDrawLod;
};

enum MDRAW_CHUNK_MODE
{
	DRAW_CHUNK_VS = 0,
	DRAW_CHUNK_PS ,
	DRAW_CHUNK_HL ,
};

enum MDRAW_FILL_MODE
{
   DRAW_SOLID = 0 ,
   DRAW_WIREFRAME ,
   DRAW_POINT,
};

class MTerrainRender : public Ogre::SceneRenderer
{
public:
	MTerrainRender(void);
	~MTerrainRender(void);

	virtual void doRender();

	void Init();
	void SetTerrain(MTerrain *pTerrain);

	void Update(unsigned int dtick);

    void Release();
	void  AddHightLight( float  hight ){ m_fAdd.x += hight;}

	MDrawOption*   GetDrawOption(){ return &m_DrawOption;}
	void   SetDrawChunkMode( MDRAW_CHUNK_MODE mode ){ m_DrawChunkMode = mode;}

	void   SetDrawChunkFillMode( MDRAW_FILL_MODE mode ){ m_DrawChunkFillMode = mode ; }
	MDRAW_FILL_MODE   GetDrawChunkFillMode(){ return m_DrawChunkFillMode ;}  

	void   SetDrawColorMap( bool bUse ){ m_DrawOption.bUserColorMap = bUse ;}
		
	void  CreateIB( int nSize );

	void  DrawAlphaMap(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);
	void  DrawPlant(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);

	void  DrawWater(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);  
	void DrawModel();
private:

	void DrawChunkLine(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);

	void DrawChunkPer();
	void DrawChunk(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);

	void DrawChunkWidthPS(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);
	void DrawChunkWidthHightLight(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);

	void DrawChunkFirstLayer(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);
	void DrawChunkOtherLayer(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata, int layer);

	void DrawChunkNoColorFirstLayer(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);
	void DrawChunkNoColorOtherLayer(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata, int layer);

	void DrawChunkShadow(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);

	void DrawLiquidPer();
	void DrawLiquid(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);


	void DrawTerainMesh(Ogre::ShaderContext *pcontext, MChunkRenderPassSet *pRenderPassSet,int lod , int nPass);
	void  DrawTerainMesh(Ogre::ShaderContext *pcontext, TerrBlockRenderData* pRender , int nPass );

	void DrawLinePer();
	void DrawLine(Ogre::SceneRenderer* prenderer, const Ogre::ShaderEnvData &envdata);

	void  DrawInterface( MIInputLayout pLayout, MIShader *pshader , LMVB pvb , int size ,int nPass, LMIB pIB = 0);
	void  DrawLineInterface(MIShader *pShader,LMVB  pvb , int size , size_t nPass);



public:
	MShader_t    m_ChunkShader;
private:
	MDRAW_CHUNK_MODE   m_DrawChunkMode;
	MDRAW_FILL_MODE    m_DrawChunkFillMode;

	MTerrain    *m_pTerrain;

	MShader_t    m_ChunkBaseColorShader;
	MShader_t    m_ChunkColorShader;

	MShader_t    m_ChunkNoDetailShader;
	MShader_t    m_ChunkBaseShader;
	MShader_t    m_ChunkColorMapShader;

	MShader_t    m_LiquidShader;
	MShader_t    m_LineShader;
	MShader_t    m_ChunkOneLayerShader;
	MShader_t    m_ChunkTowLayerShader;
	MShader_t    m_ChunkThreeLayerShader;
	MShader_t    m_ChunkFourLayerShader;
	MShader_t    m_ChunkOneHLLayerShader;

public:
	MTerrainCull   m_Cull;
	MDrawOption    m_DrawOption;

	MVec3          m_fAdd;

	int            m_nDrawTriNum;

};

};

extern TE::MTerrainRender g_TerrainRender;