
//紧贴地形， 比如车轮印等
#pragma once
#include <string>
#include <vector>
#include "TerrainEditTypes.h"

namespace TE
{

#define MAX_STRIP_VEC_NUM   32

	struct StripVec_st
	{
		Ogre::Vector3  pos;
		Ogre::Vector2  uv;
	};

	struct StripData_st
	{
		float    stripWidth;
		std::vector <StripVec_st>     vecSet;
		std::string   filename;
	};

	struct StripRenderData_st
	{
		Ogre::VertexData *           pVb;
		Ogre::VertexDeclHandle  pLayout;
		Ogre::Texture *          pTex;
		Ogre::Material *pmtl;
	};
	//条带
	class TerrainStrip
	{
	public:
		TerrainStrip(void); 
		~TerrainStrip(void);

		void                   AddPos( Ogre::Vector3  pos );
		void                   Release();

		void                   Update();
		bool                   Init();

		void                   Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);

		void                   SetTextureFile(const char* filename);

		std::vector<Ogre::Vector3>      m_SelectPosSet; 

		StripData_st              m_Data;
		StripRenderData_st        m_RenderData;
	};
	//条带集合
	class CStripBaseSet
	{
	public:
		CStripBaseSet();
		~CStripBaseSet();

		void   Release();

		void   Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);

		void                  AddStrip(TerrainStrip*  pStrip);
		void                  DelCurSelectStrip();

		void                  AddCurSelectStripPos(Ogre::Vector3 pos);
		void                  SetCurSelectStripTex(const char* filename);

		std::vector <TerrainStrip*>               m_StripSet;
		int                                     m_nCurSelectStripID;
	};

	extern CStripBaseSet  *g_StripBaseSet;
}
