
#pragma once
#include  <vector>
#include "TerrainEditTypes.h"

namespace TE
{

struct CWallPaperRenderData
{
	LMTex    pTex;
	LMVB     pVb;
	LMIB     pIb;
	MIInputLayout   pLayout;
	Ogre::Material   *pmtl;
};

struct CWallPaperVec_st
{
	MVec3  pos;
    MVec2  texcoord;
};

class CWallPaper
{
public:
	CWallPaper(void);
	~CWallPaper(void);

	bool   Init();

	void   UpdateData();
	void   Render(Ogre::SceneRenderer *prenderer, Ogre::ShaderEnvData &envdata);

	void   Release();
	void   SetPos(MVec3  pos0,MVec3 pos1,MVec3 pos2 , MVec3 pos3);
public:
	std::vector <MVec3>      m_posSet;
    
	CWallPaperRenderData       m_RenderData;         
};


};