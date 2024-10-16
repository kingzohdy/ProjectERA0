
#include "TerrainStrip.h"
#include "Terrain.h"
#include "TextureManager.h"

#include "OgreSceneRenderer.h"
#include "OgreShaderContext.h"
#include "OgreCamera.h"
#include "OgreRenderSystem.h"

using namespace Ogre;

namespace TE
{
	TerrainStrip::TerrainStrip(void)
	{
		memset( &m_Data , 0 , sizeof( StripData_st ) );
		memset( &m_RenderData , 0 , sizeof( StripRenderData_st ) );

		m_Data.stripWidth = 100.0f;
	}

	TerrainStrip::~TerrainStrip(void)
	{
		Release();
	}

	void  TerrainStrip::AddPos( Ogre::Vector3  pos )
	{
		if( m_SelectPosSet.size() >= MAX_STRIP_VEC_NUM ) 
		{
			m_SelectPosSet.clear ();
			return;
		}

		m_SelectPosSet.push_back ( pos );

		Update();
	};

	void  TerrainStrip::SetTextureFile(const char* filename)
	{
		m_Data.filename  = filename;

		m_RenderData.pTex = g_TexManager.LoadTex ( filename );
	}

	void  TerrainStrip::Release()
	{
		m_SelectPosSet.clear ();

		m_Data.filename .clear ();
		m_Data.vecSet .clear ();

		OGRE_RELEASE( m_RenderData.pVb );
	};

	void  TerrainStrip::Update()
	{
		if( m_SelectPosSet.size () < 2 )
			return;

		m_Data.vecSet.clear ();

		for( size_t  i = 0 ; i < m_SelectPosSet.size () ; i ++)
		{
			if( i != m_SelectPosSet.size () - 1 )
			{
				Ogre::Vector3  dir = m_SelectPosSet[i+1] - m_SelectPosSet[i] ;
				Normalize( dir );
				dir.y = 0 ;
				Ogre::Vector3  yDir = Ogre::Vector3(0,1,0);

				Ogre::Vector3 vDir = CrossProduct( dir ,  yDir );
				Normalize( vDir );

				Ogre::Vector2  _dir( vDir.x , vDir.z );
				Ogre::Vector3 pos1,pos2;

				pos1.x = _dir.x * m_Data.stripWidth + m_SelectPosSet[i].x ;
				pos1.z = _dir.y * m_Data.stripWidth + m_SelectPosSet[i].z ;

				_dir.x =  -vDir.x ;
				_dir.y =  -vDir.z ;

				pos2.x = _dir.x * m_Data.stripWidth + m_SelectPosSet[i].x ;
				pos2.z = _dir.y * m_Data.stripWidth + m_SelectPosSet[i].z ;

				g_Terrain.GetHeight ( pos1.x ,pos1.z , &pos1.y );
				g_Terrain.GetHeight ( pos2.x ,pos2.z , &pos2.y );

				StripVec_st vec1,vec2;
				vec1.pos = pos1 ; vec2.pos = pos2;

				vec1.uv.x = (float)i ; vec1.uv .y = 0.0f;
				vec2.uv.x = (float)i ; vec2.uv .y = 1.0f;

				m_Data.vecSet.push_back ( vec1 );
				m_Data.vecSet .push_back ( vec2 );	
			}
			else if( i == m_SelectPosSet.size () - 1 )
			{
				Ogre::Vector3  dir = m_SelectPosSet[i] - m_SelectPosSet[i-1] ;
				dir.y = 0 ;
				Ogre::Vector3  yDir = Ogre::Vector3(0,1,0);

				Ogre::Vector3 vDir = CrossProduct( dir ,  yDir );
				Normalize( vDir );

				Ogre::Vector2  _dir( vDir.x , vDir.z );
				Ogre::Vector3 pos1,pos2;

				pos1.x = _dir.x * m_Data.stripWidth + m_SelectPosSet[i].x ;
				pos1.z = _dir.y * m_Data.stripWidth + m_SelectPosSet[i].z ;

				_dir.x = -vDir.x ;
				_dir.y = -vDir.z ;

				pos2.x = _dir.x * m_Data.stripWidth + m_SelectPosSet[i].x ;
				pos2.z = _dir.y * m_Data.stripWidth + m_SelectPosSet[i].z ;

				g_Terrain.GetHeight ( pos1.x ,pos1.z , &pos1.y );
				g_Terrain.GetHeight ( pos2.x ,pos2.z , &pos2.y );


				StripVec_st vec1,vec2;
				vec1.pos = pos1 ; vec2.pos = pos2;
				vec1.uv.x = (float)i ; vec1.uv .y = 0;
				vec2.uv.x = (float)i ; vec2.uv .y = 1;

				m_Data.vecSet.push_back ( vec1 );
				m_Data.vecSet .push_back ( vec2 );
			}
			else
			{

			}
		}

		StripVec_st* vdata = (StripVec_st*)m_RenderData.pVb ->lock();

		if(vdata == NULL)
			return;

		for( size_t i = 0 ; i < m_Data.vecSet .size () ; i++)
		{
			vdata[i].uv   = m_Data.vecSet [i].uv ;
			vdata[i].pos = m_Data.vecSet [i].pos ;
		}

		m_RenderData.pVb ->unlock();

		return ;

	};

	bool  TerrainStrip::Init()
	{
		VertexFormat vertfmt;
		vertfmt.addElement(VET_FLOAT3, VES_POSITION);
		vertfmt.addElement(VET_FLOAT2, VES_TEXCOORD);
		m_RenderData.pVb   = new VertexData(vertfmt, MAX_STRIP_VEC_NUM);

		m_RenderData.pmtl = new Material("strip");
		m_RenderData.pLayout = RenderSystem::getSingleton().getInputLayout(vertfmt);

		return true;
	};

	void  TerrainStrip::Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata)
	{
		if(m_Data.vecSet .size () < 2)
			return;

		m_RenderData.pmtl->setParamTexture("g_BaseTex", m_RenderData.pTex);
		ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, m_RenderData.pmtl, m_RenderData.pLayout, m_RenderData.pVb, NULL, PRIM_TRIANGLESTRIP, m_Data.vecSet.size(), false);
		DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

		Ogre::Matrix4 WorldViewProj = g_pCamera->getViewMatrix() * g_pCamera->getProjectMatrix();
		pcontext->addValueParam(SPU_WORLDVIEWPROJ, &WorldViewProj, sizeof(WorldViewProj));
	};

	CStripBaseSet  *g_StripBaseSet;

	CStripBaseSet::CStripBaseSet()
	{
		m_nCurSelectStripID = -1 ;
	};


	CStripBaseSet::~CStripBaseSet()
	{
		Release();
	};

	void   CStripBaseSet::Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata)
	{

		for( size_t i = 0 ; i < m_StripSet.size () ; i ++ )
		{
			m_StripSet[i]->Render(prenderer, envdata);
		}
	};

	void   CStripBaseSet::AddStrip ( TerrainStrip* pStrip)
	{
		m_StripSet.push_back ( pStrip );

		m_nCurSelectStripID = m_StripSet.size () - 1;

	}

	void   CStripBaseSet::SetCurSelectStripTex(const char* filename)
	{
		if( m_nCurSelectStripID < 0 || (size_t)m_nCurSelectStripID >= m_StripSet.size () )
			return;

		m_StripSet[m_nCurSelectStripID]->SetTextureFile ( filename );
	}

	void   CStripBaseSet::AddCurSelectStripPos(Ogre::Vector3 pos )
	{
		m_StripSet[m_nCurSelectStripID]->AddPos ( pos );
	}

	void   CStripBaseSet::Release()
	{
		for( size_t i = 0 ; i < m_StripSet.size () ; i ++)
		{
			OGRE_DELETE( m_StripSet[i] );
		}

		m_StripSet.clear ();
	}
}
