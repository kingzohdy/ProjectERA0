#include ".\wallpaper.h"

#include "mtexmanager.h"

#include "OgreSceneRenderer.h"
#include "OgreShaderContext.h"
#include "OgreCamera.h"
#include "OgreRenderSystem.h"

using namespace TE;
using namespace Ogre;

CWallPaper::CWallPaper(void)
{
	memset( &m_RenderData , 0 , sizeof( CWallPaperRenderData )) ;
}

CWallPaper::~CWallPaper(void)
{
	Release();
}
	
void   CWallPaper::SetPos(MVec3  pos0,MVec3 pos1,MVec3 pos2 , MVec3 pos3)
{
	if( m_RenderData.pVb == NULL )
		return;

	CWallPaperVec_st* vdata = (CWallPaperVec_st*)m_RenderData.pVb->lock();
   
	if(vdata == NULL)
		return ;

	vdata[0].pos = pos0;
	vdata[1].pos = pos1;
	vdata[2].pos = pos2;
	vdata[3].pos = pos3;

	m_RenderData.pVb->unlock();
};

bool   CWallPaper::Init()
{
	m_RenderData.pTex = g_TexManager.LoadTex ("tex.wallpaper.attack");

	VertexFormat vertfmt;
	vertfmt.addElement(VET_FLOAT3, VES_POSITION);
	vertfmt.addElement(VET_FLOAT2, VES_TEXCOORD);
	m_RenderData.pVb = new VertexData(vertfmt, 4);
	m_RenderData.pmtl = new Material("wallpaper");

	m_RenderData.pLayout = RenderSystem::getSingleton().getInputLayout(vertfmt);

	MVec3  pos(0,0,0);

	CWallPaperVec_st* vdata = (CWallPaperVec_st*)m_RenderData.pVb->lock();
   
	if(vdata == NULL)
		return false;

	vdata[0].texcoord = MVec2(0,0);
	vdata[1].texcoord = MVec2(1,0);
	vdata[2].texcoord = MVec2(0,1);
	vdata[3].texcoord = MVec2(1,1);

	m_RenderData.pVb->unlock();

	SetPos(pos,pos,pos,pos);

	return true;
};
	
void   CWallPaper::UpdateData()
{

};
	
void   CWallPaper::Render(Ogre::SceneRenderer *prenderer, Ogre::ShaderEnvData &envdata)
{
	m_RenderData.pmtl->setParamTexture("g_BaseTex", m_RenderData.pTex);
	ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, m_RenderData.pmtl, m_RenderData.pLayout, m_RenderData.pVb, m_RenderData.pIb, PRIM_TRIANGLESTRIP, 4, false);
	DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

	MMatrix WorldViewProj = g_pCamera->getViewMatrix() * g_pCamera->getProjectMatrix();
	pcontext->addValueParam(SPU_WORLDVIEWPROJ, &WorldViewProj, sizeof(MMatrix));
};

void   CWallPaper::Release()
{
     OGRE_RELEASE( m_RenderData.pIb );
	 OGRE_RELEASE( m_RenderData.pVb );
	 OGRE_RELEASE( m_RenderData.pmtl );
};