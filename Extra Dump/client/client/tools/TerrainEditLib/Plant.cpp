#include ".\plant.h"

#include "mtexmanager.h"
#include "mterrainedit.h"
#include "EditorModel.h"
#include "mstream.h"
#include "MTerrain.h"

#include "OgreSceneRenderer.h"
#include "OgreShaderContext.h"
#include "OgreCamera.h"
#include "OgreRenderSystem.h"
#include "OgreRay.h"

using namespace TE;
using namespace Ogre;

extern std::string g_path;
CPlantManager    *g_PlantMgr;

extern MVec4 g_WorldAmbientLight;


CPlantSet::CPlantSet(void)
{
	m_nCurVecNum = 0;
    m_nCurIndexNum = 0;

	m_fHeight = 2;
	m_fWidth  = 1;

	for( int i = 0 ; i < 4 ; i++)
	{
		m_MovePos[i] = MVec3(0 ,0 , 0);
	}


	m_fMovetime  = 0.01f;
	m_fMovetime1 = 0.02f;
	m_fMovetime2 = 0.03f;
	m_fMovetime3 = 0.04f;

}

CPlantSet::~CPlantSet(void)
{
	Release();
}
	 	 
void   CPlantSet::ClearLightColor()
{
	if ( m_nCurVecNum == 0 )
		return;

	for( int i = 0 ; i < m_nCurVecNum / 4 ; i ++ )
	{	
		m_unSaveColorSet[i] = 0;
	}
};
	
void   CPlantSet::CountLightColor(int lighttype, MVec3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity)
{
	 unsigned char r,g,b ;
	unsigned char r1,g1,b1 ;

	float drange = range*range;

	 for( int i = 0 ; i < m_nCurVecNum / 4  ; i ++ )
	 {
		 float strength;
		 MVec3 normal(0,1,0);

		 if(lighttype == 0)
		 {
			 MVec3 pos0 = m_SavePosSet[i];
			 MVec3 dst = pos - pos0 ;

			 float fLength = dst.lengthSqr() ;
			 if( fLength >= drange )
			 {
				 continue;
			 }

			 Normalize( dst );
			 float fdot = DotProduct( normal , dst ) ;
			 if( fdot <= 0 )
			 {
				 continue;
			 }

			 strength =  sqrt( fLength ) / range;
			 strength = 1.0f - strength * strength;

			 if( strength < 0 ) strength = 0;
			 strength *= fdot;
		 }
		 else
		 {
			 strength = DotProduct(normal, pos);
			 if(strength < 0) strength = 0;

			 if(shadowdensity > 0)
			 {
				 WorldRay ray;
				 ray.m_Origin = m_SavePosSet[i];
				 ray.m_Dir = pos;
				 ray.m_Range = 100000.0f;
				 if(g_Terrain.pickObject(INTERSECT_LIGHT, ray, NULL, 0xffffffff))
				 {
					 strength *= 1.0f - shadowdensity;
				 }
			 }
		 }

		 ColourValue vertcolor;
		 vertcolor.setAsARGB(m_unSaveColorSet[i]);
		 ColourValue lighting = color*strength + ambient + vertcolor;
		 ColorQuad cq(lighting);

		m_unSaveColorSet[i] = cq.c;
	 }	    
};
	 
void   CPlantSet::UpdateLightColor()
{
	CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb->lock();

	if(vdata == NULL)
	{
		return ;
	}
	
	for( int i = 0 ; i < m_nCurVecNum   ; i++)
	{
		vdata[i].color = m_unSaveColorSet[i/4]; 
	}	
	m_RenderData.pVb->unlock();
};

void CPlantSet::UpdateHeight(MChunk* pChunk)
{
	for(int i=0;i<m_nCurVecNum/4;i++)
	{
		float newheight;
		g_Terrain.GetHeight(m_PosSet[4*i+0].x,m_PosSet[4*i+0].z,&newheight);
		float deltaheight = m_PosSet[4*i+0].y - newheight;
		m_PosSet[4*i+0].y = newheight;

		m_PosSet[4*i+1].y -=deltaheight;
		m_PosSet[4*i+2].y -=deltaheight;
		m_PosSet[4*i+3].y -=deltaheight;
	}

	UpdateDataEx_1();
};

void   CPlantSet::UpdateDataEx_1()
{
	if( m_nCurVecNum <= 0)
	{
		m_nCurVecNum = 0;
		m_nCurIndexNum = 0;
		m_RenderData.pIb->setVertexRange(0, m_nCurVecNum);
		return;
	}

	CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb->lock();

	if(vdata == NULL)
		return ;
	
	for( int i = 0 ; i < m_nCurVecNum   ; i++)
	{
		vdata[i].pos  = m_PosSet[i];
		vdata[i].color = m_unSaveColorSet[i/4];
	}
	
	m_RenderData.pVb->unlock();

	unsigned short*  idata = (unsigned short*)m_RenderData.pIb->lock();

	int k = 0 ; 

	m_nCurIndexNum = 0;
	
	for( int i = 0 ; i < m_nCurVecNum / 4 ; i++)
	{				
		idata[m_nCurIndexNum] = i* 4 + 0;
		idata[m_nCurIndexNum+1] = i * 4 + 2;
		idata[m_nCurIndexNum+2] = i * 4 + 1;

		idata[m_nCurIndexNum+3] = i * 4 + 1;
		idata[m_nCurIndexNum+4] = i * 4 + 2;
		idata[m_nCurIndexNum+5] = i * 4 + 3;
								
		m_nCurIndexNum += 6;
	}

	m_RenderData.pIb->unlock();
	

	m_RenderData.pIb->setVertexRange(0, m_nCurVecNum);
}

void   CPlantSet::UpdateDataEx(int mode)
{
	if( m_nCurVecNum <= 0)
		return;

	CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb->lock();

	if(vdata == NULL)
		return ;
	
	for( int i = 0 ; i < m_nCurVecNum ; i++)
	{
		vdata[i].pos  = m_PosSet[i];
	}
	
	m_RenderData.pVb->unlock();

	return;

};
	
void   CPlantSet::Reset()
{
     m_nCurVecNum = 0 ;
}

void   CPlantSet::UpdateData()
{
	if( m_nCurVecNum <= 0)
		return;

	CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb->lock();

	if( vdata == NULL)
		return ;
	
	for( int i = 0 ; i < m_nCurVecNum ; i++)
	{
		vdata[i].pos  = m_PosSet[i];
	}
	
	m_RenderData.pVb->unlock();
};
     
	 
bool   CPlantSet::InitEx_1()
{
	VertexFormat vertfmt;
	vertfmt.addElement(VET_FLOAT3, VES_POSITION);
	vertfmt.addElement(VET_COLOUR, VES_COLOR);
	vertfmt.addElement(VET_FLOAT2, VES_TEXCOORD);

	m_RenderData.pVb  = new VertexData(vertfmt, PLANT_VEC_MAX_NUM);
    m_RenderData.pmtl  = new Material("plant");
	m_RenderData.pLayout = RenderSystem::getSingleton().getInputLayout(vertfmt);

	CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb->lock();

	int k = 0 ; 
	for( int i = 0 ; i < PLANT_VEC_MAX_NUM / 4  ; i ++)
	{	
		vdata[i*4+0].texcoord = MVec2(1,1);
		vdata[i*4+1].texcoord = MVec2(0,1);
		vdata[i*4+2].texcoord = MVec2(1,0);
		vdata[i*4+3].texcoord = MVec2(0,0);
	}

	m_RenderData.pVb->unlock();
		
	m_RenderData.pIb  = new IndexData(6*PLANT_MAX_NUM);
	m_RenderData.pIb->setVertexRange(0, PLANT_VEC_MAX_NUM);

	return true;

};
bool   CPlantSet::SetTexture(LMTex  tex)
{
   m_RenderData.pTex = tex;

   return true;
};

bool   CPlantSet::SetTexture(const char* filename )
{
	m_TextureName = filename;

	m_RenderData.pTex = g_TexManager.LoadTex ( m_TextureName.c_str () );

	if( m_RenderData.pTex == NULL )
		return false;

	return true;
};

void   CPlantSet::Release()
{
     OGRE_RELEASE( m_RenderData.pIb );
	 OGRE_RELEASE( m_RenderData.pVb );
	// OGRE_RELEASE( m_RenderData.pTex );
	 OGRE_RELEASE( m_RenderData.pmtl );

};	

void CPlantSet::render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata)
{
	if(m_nCurIndexNum == 0)
	{
		return;
	}

	m_RenderData.pmtl->setParamTexture("g_BaseTex", m_RenderData.pTex);
	ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, m_RenderData.pmtl, m_RenderData.pLayout, m_RenderData.pVb, m_RenderData.pIb, PRIM_TRIANGLELIST, m_nCurVecNum/2, true);
	DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

	pcontext->setInstanceEnvData(prenderer, NULL, envdata, NULL);
}

void   CPlantSet::RenderEx(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata)
{
	if( m_nCurIndexNum == 0)
		return;
   
	m_RenderData.pmtl->setParamTexture("g_BaseTex", m_RenderData.pTex);
	ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, m_RenderData.pmtl, m_RenderData.pLayout, m_RenderData.pVb, m_RenderData.pIb, PRIM_TRIANGLELIST, m_nCurVecNum/2, true);
	DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

	MMatrix WorldViewProj = g_pCamera->getViewMatrix() * g_pCamera->getProjectMatrix();
	pcontext->addValueParam(SPU_WORLDVIEWPROJ, &WorldViewProj, sizeof(WorldViewProj));

	/*
	pshader->SetConstant ( "g_PosExcursion" , &m_MovePos[0] );	
	pshader->SetConstant ( "g_PlayerPos" , &m_MovePos[1] );
	pshader->SetConstant ( "g_PlayerDir" , &m_MovePos[2] );
	pshader->SetConstant ( "g_WindInfo" , &m_MovePos[3] );

	pshader->SetConstant ("g_LightDir",&g_AmbientInfo.LightDirect);
	pshader->SetConstant("g_LightColor", &g_AmbientInfo.directColor);

	pshader->SetConstant ( "g_AmibentColor", &g_AmbientInfo.ambientColor );

	pshader->SetTexture ( "g_BaseTex" , m_RenderData.pTex );
	*/
}

void   CPlantSet::Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata)
{
	m_RenderData.pmtl->setParamTexture("g_DiffuseTex", m_RenderData.pTex);
	m_RenderData.pmtl->setParamValue("g_PosExcursion", &m_MovePos[0]);
	ShaderContext *pcontext = prenderer->newContext(RL_SCENE, envdata, m_RenderData.pmtl, m_RenderData.pLayout, m_RenderData.pVb, m_RenderData.pIb, PRIM_TRIANGLELIST, m_nCurVecNum/2, true);
	DEBUG_SHADERCONTEXT_PTR(pcontext, NULL);

	pcontext->setInstanceEnvData(prenderer, NULL, envdata, NULL);
	/*
	pshader->SetConstant ( "g_PosExcursion" , &m_MovePos[0] );
	pshader->SetConstant ( "g_PlayerPos" , &m_MovePos[1] );
	pshader->SetConstant ( "g_PlayerDir" , &m_MovePos[2] );
	pshader->SetConstant ( "g_WindInfo" , &m_MovePos[3] );

	pshader->SetTexture ( "g_BaseTex" , m_RenderData.pTex );
	*/
};
	 	 
void   CPlantSet::UpdateEx_1(float tick)
{

	if( m_RenderData.pVb == NULL )
		return;

	/*m_fMovetime += rand()%10 * 0.005f;
	m_fMovetime1 += rand()%10 * 0.005f;
	m_fMovetime2 += rand()%10 * 0.005f;
	m_fMovetime3 += rand()%10 * 0.005f;*/

	m_MovePos[0].x = m_fMovetime;
	m_MovePos[0].y = m_fMovetime1;

	float fRefreshTime = tick / 500.0f;

//	m_MovePos[0].x = fRefreshTime + rand()%10 * 0.005f;;//m_fMovetime;
//	m_MovePos[0].y = fRefreshTime + rand()%10 * 0.005f;;//m_fMovetime1;


	m_fMovetime  += fRefreshTime;//0.1f;
	m_fMovetime1 += fRefreshTime;//0.1f;

	m_fMovetime2 += fRefreshTime;//0.1f;
	m_fMovetime3 += fRefreshTime;//0.1f;

/*	CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb  ->Lock(LOCK_WRITE);

	if(vdata == NULL)
		return ;

	for( int i = 0 ; i < m_nCurVecNum / 4 ; i++)
	{
		{
			if( m_SaveMovePos[i].y != 0 )
			{
				if ( m_bCountPos[i] == 0 )
				{
				   m_SaveDestPos[i] = - 0.85f*m_SaveMovePos[i];					
				   m_bCountPos[i] = 1;
				   m_bNormal[i] = ( m_SaveDestPos[i] - m_SaveMovePos[i] ) / 10.0f;
				}
                  
				m_SaveMovePos[i].x += m_bNormal[i].x ;
				m_SaveMovePos[i].z += m_bNormal[i].z ;

				if(  ( m_SaveDestPos[i].x * m_SaveDestPos[i].x + m_SaveDestPos[i].z * m_SaveDestPos[i].z )
					< (  m_SaveMovePos[i].x * m_SaveMovePos[i].x + m_SaveMovePos[i].z * m_SaveMovePos[i].z) )

				{
					m_bCountPos[i] = 0;
				}
				else
				{
		    		vdata[i*4 + 2].pos.x = m_PosSet[i*4 + 2].x +  m_SaveMovePos[i].x; 
		    		vdata[i*4 + 2].pos.z = m_PosSet[i*4 + 2].z +  m_SaveMovePos[i].z; 
		    		vdata[i*4 + 3].pos.x = m_PosSet[i*4 + 3].x  +  m_SaveMovePos[i].x; 
		    		vdata[i*4 + 3].pos.x = m_PosSet[i*4 + 3].x  +  m_SaveMovePos[i].x; 
				}		
				
				if(  ( m_SaveDestPos[i].x*m_SaveDestPos[i].x  + m_SaveDestPos[i].z*m_SaveDestPos[i].z  ) < 0.002 )
				{
					m_SaveMovePos[i].y = 0;
				}
			}
		}
	}

	m_RenderData.pVb ->Unlock();*/
};

void  CPlantSet::Update(float  tick)
{	

	//植物系统暂时不用

	if( m_RenderData.pVb == NULL )
		return;

	m_fMovetime += rand()%10 * 0.005f;
	m_fMovetime1 += rand()%10 * 0.005f;
	m_fMovetime2 += rand()%10 * 0.005f;
	m_fMovetime3 += rand()%10 * 0.005f;

	m_MovePos[0].x = m_fMovetime;
	m_MovePos[0].y = m_fMovetime1;

	float fRefreshTime = tick / 500.0f;

	m_fMovetime  += fRefreshTime;//0.1f;
	m_fMovetime1 += fRefreshTime;//0.1f;

	m_fMovetime2 += fRefreshTime;//0.1f;
	m_fMovetime3 += fRefreshTime;//0.1f;

	/*CPlantVec_st* vdata = (CPlantVec_st*)m_RenderData.pVb  ->Lock(LOCK_WRITE);

	if(vdata == NULL)
		return ;

	for( int i = 0 ; i < m_nCurVecNum / 8 ; i++)
	{
		{
			if( m_SaveMovePos[i].y != 0 )
			{
				if ( m_bCountPos[i] == 0 )
				{
				   m_SaveDestPos[i] = - 0.85f*m_SaveMovePos[i];					
				   m_bCountPos[i] = 1;
				   m_bNormal[i] = ( m_SaveDestPos[i] - m_SaveMovePos[i] ) / 10.0f;
				}
                  
				m_SaveMovePos[i].x += m_bNormal[i].x ;
				m_SaveMovePos[i].z += m_bNormal[i].z ;

				if(  ( m_SaveDestPos[i].x * m_SaveDestPos[i].x + m_SaveDestPos[i].z * m_SaveDestPos[i].z )
					< (  m_SaveMovePos[i].x * m_SaveMovePos[i].x + m_SaveMovePos[i].z * m_SaveMovePos[i].z) )

				{
					m_bCountPos[i] = 0;
				}
				else
				{
		    		vdata[i*8 + 2].pos.x = m_PosSet[i*8 + 0].x +  m_SaveMovePos[i].x; 
		    		vdata[i*8 + 2].pos.z = m_PosSet[i*8 + 0].z +  m_SaveMovePos[i].z; 
		    		vdata[i*8 + 3].pos.x = m_PosSet[i*8 + 1].x  +  m_SaveMovePos[i].x; 
		    		vdata[i*8 + 3].pos.x = m_PosSet[i*8 + 1].x  +  m_SaveMovePos[i].x; 

		    		vdata[i*8 + 6].pos.x = m_PosSet[i*8 + 4].x +  m_SaveMovePos[i].x ; 
		    		vdata[i*8 + 6].pos.z = m_PosSet[i*8 + 4].z +  m_SaveMovePos[i].z ; 

					vdata[i*8 + 7].pos.x  = m_PosSet[i*8 + 5].x  +  m_SaveMovePos[i].x ; 
					vdata[i*8 + 7].pos.z  = m_PosSet[i*8 + 5].z  +  m_SaveMovePos[i].z ;
				}		
				
				if(  ( m_SaveDestPos[i].x*m_SaveDestPos[i].x  + m_SaveDestPos[i].z*m_SaveDestPos[i].z  ) < 0.002 )
				{
					m_SaveMovePos[i].y = 0;
				}
			}
		}
	}

	m_RenderData.pVb ->Unlock();*/

};
	
void   CPlantSet::DelPos( MVec3 pos , float fSize )
{
	int nPosNum = m_nCurVecNum / 4 ;

	for( int i = 0 ; i < nPosNum ; i ++ )
	{
		MVec3 vLen;
		vLen = pos - m_SavePosSet[i] ;
		float len = vLen.length() ;

		if( len < fSize )
		{
			for( int j = i ; j < nPosNum - 1 ; j ++ )
			{
				m_SavePosSet[j] = m_SavePosSet[j+1];
			
				m_PosSet[4*j + 0]    = m_PosSet[4*(j+1) + 0];
				m_PosSet[4*j + 1]    = m_PosSet[4*(j+1) + 1];
				m_PosSet[4*j + 2]    = m_PosSet[4*(j+1) + 2];
				m_PosSet[4*j + 3]    = m_PosSet[4*(j+1) + 3];
			}
			
			m_nCurVecNum -= 4;
		}
	}

	UpdateDataEx_1();

}

bool   CPlantSet::AddPosEx_1(MVec3 pos , float fScale , unsigned int color )
{
	if( m_nCurVecNum / 4 >= ( PLANT_MAX_NUM - 12)  || m_nCurVecNum < 0 )
		return false;

	 MVec3  posSet[4];

	 float heightdx = ( rand() % 100 + 1) /500.0f;

	 posSet[0].y = pos.y ;
	 posSet[1].y = pos.y ;
	 posSet[2].y = pos.y  + fScale*m_fHeight + heightdx ;
	 posSet[3].y = pos.y  + fScale*m_fHeight + heightdx ;

	 m_SavePosSet[m_nCurVecNum/4] = pos;
     m_unSaveColorSet[m_nCurVecNum/4] = color ;
	 m_SaveScaleSet[m_nCurVecNum/4] = fScale ;

	 float angle = float(rand()%360);
	 float fSin = Ogre::Sin(angle);
	 float fCos = Ogre::Cos(angle);

	 posSet[0].x = pos.x + fScale*m_fWidth*fSin;
	 posSet[0].z = pos.z + fScale*m_fWidth*fCos;

	 posSet[1].x = pos.x - fScale*m_fWidth*fSin;
	 posSet[1].z = pos.z - fScale*m_fWidth*fCos;

	 posSet[2].x = pos.x + fScale*m_fWidth*fSin;
	 posSet[2].z = pos.z + fScale*m_fWidth*fCos;

	 posSet[3].x = pos.x - fScale*m_fWidth*fSin;
	 posSet[3].z = pos.z - fScale*m_fWidth*fCos;
	
	 m_PosSet[m_nCurVecNum + 0]    = posSet[0];
	 m_PosSet[m_nCurVecNum + 1]    = posSet[1];
	 m_PosSet[m_nCurVecNum + 2]    = posSet[2];
	 m_PosSet[m_nCurVecNum + 3]    = posSet[3];


	 m_nCurVecNum += 4;
	 
	 return true;
}

void   CPlantSet::SetPos(MVec3 pos)
{
     MVec3  posSet[8];

	 m_nCurVecNum = 0;

	 posSet[0].y = pos.y ;
	 posSet[1].y = pos.y ;
	 posSet[2].y = pos.y ;
	 posSet[3].y = pos.y ;

	 posSet[4].y = pos.y + m_fHeight;
	 posSet[5].y = pos.y + m_fHeight;
	 posSet[6].y = pos.y + m_fHeight;
	 posSet[7].y = pos.y + m_fHeight;

	 posSet[0].x = pos.x + m_fWidth;
	 posSet[1].x = pos.x + m_fWidth;
	 posSet[2].x = pos.x - m_fWidth;
	 posSet[3].x = pos.x - m_fWidth;

	 posSet[4].x = pos.x + m_fWidth;
	 posSet[5].x = pos.x + m_fWidth;
	 posSet[6].x = pos.x - m_fWidth;
	 posSet[7].x = pos.x - m_fWidth;

	 posSet[0].z = pos.z + m_fWidth;
	 posSet[1].z = pos.z - m_fWidth;
	 posSet[2].z = pos.z - m_fWidth;
	 posSet[3].z = pos.z + m_fWidth;

	 posSet[4].z = pos.z + m_fWidth;
	 posSet[5].z = pos.z - m_fWidth;
	 posSet[6].z = pos.z - m_fWidth;
	 posSet[7].z = pos.z + m_fWidth;

	 m_PosSet[m_nCurVecNum]    = posSet[0];
	 m_PosSet[m_nCurVecNum+1] = posSet[2];
	 m_PosSet[m_nCurVecNum+2] = posSet[4];

	 m_PosSet[m_nCurVecNum+3] = posSet[2];
	 m_PosSet[m_nCurVecNum+4] = posSet[4];	 
	 m_PosSet[m_nCurVecNum+5] = posSet[6];

	 m_PosSet[m_nCurVecNum+6] = posSet[1];
	 m_PosSet[m_nCurVecNum+7] = posSet[3];
	 m_PosSet[m_nCurVecNum+8] = posSet[5];

	 m_PosSet[m_nCurVecNum+9] = posSet[3];
	 m_PosSet[m_nCurVecNum+10] = posSet[5];	 
	 m_PosSet[m_nCurVecNum+11] = posSet[7];

	 m_nCurVecNum = 12;
};

//植物管理，根据信息生成大量植被
CPlantManager::CPlantManager()
{
	memset( m_PlantMap,0,1024*1024 );
	m_strPlantInfoFile[0] = 0 ;
}

void    CPlantManager::Init()
{

	m_PlantInfoSet.clear ();

	char  plantInfo[260];

   strcpy(plantInfo,g_path.c_str ());
	strcat(plantInfo,"\\plantInfo.ini");

	int  texNum = GetPrivateProfileInt("Info","textureNum",0,plantInfo);
		
	MTextureInfo  texInfo;	
	GetPrivateProfileString( "Info" , "tex1" ,"" , texInfo.textureName  ,260 , plantInfo );
	m_TextureFileSet.push_back ( texInfo );
	GetPrivateProfileString( "Info" , "tex2" ,"" , texInfo.textureName ,260 , plantInfo );
	m_TextureFileSet.push_back ( texInfo );
	GetPrivateProfileString( "Info" , "tex3" ,"" , texInfo.textureName  ,260 , plantInfo );
	m_TextureFileSet.push_back ( texInfo );

	GetPrivateProfileString( "Info" , "sizeInfoFile" ,"" , m_strPlantInfoFile  ,260 , plantInfo );

	char Info[64];

	for( int i = 0 ; i < texNum ; i++)
	{	
		MPlantInfo_t info_t;
		sprintf( Info,"Info_%d", i );
		info_t.plantKindNum = GetPrivateProfileInt( Info , "fileNum" , 0 ,plantInfo );

		info_t.indexSet .resize ( info_t.plantKindNum );
		for( int j = 0 ; j < info_t.plantKindNum ; j ++)
		{
			PlantName_t  texFileInfo;

			char fileInfo[260];
			sprintf( fileInfo , "filename_%d" , j );
			GetPrivateProfileString( Info , fileInfo ,"" , texFileInfo.plantFileName   ,260 , plantInfo );  

			info_t.plantNameSet .push_back ( texFileInfo );
			sprintf( fileInfo , "rand_%d" , j );
			int rand_index = GetPrivateProfileInt( Info,fileInfo,0,plantInfo);
			info_t.randIdx .push_back (rand_index);
		}

		info_t.bUserSizeInfo = GetPrivateProfileInt( Info,"useInfoFile",0,plantInfo);
	    info_t.color     =  GetPrivateProfileInt( Info,"color",0,plantInfo);
		info_t.height = GetPrivateProfileInt( Info,"height",0,plantInfo);
		info_t.width = GetPrivateProfileInt( Info,"width",0,plantInfo);
		info_t.type = GetPrivateProfileInt( Info,"type",0,plantInfo);
		info_t.maxScale = GetPrivateProfileInt( Info,"maxScale",0,plantInfo) ;
		info_t.minScale = GetPrivateProfileInt( Info,"minScale",0,plantInfo) ;
		info_t.nAutoNormal = GetPrivateProfileInt( Info,"autoNormal" , 0 , plantInfo );

		m_PlantInfoSet.push_back ( info_t );
	}
};
//根据灰度图文件生成大量植被
void    CPlantManager::CreateChunkPlant()
{
	if( g_TerrainEdit.GetEditData() == NULL )
		return;

	MStream   mstream;
	unsigned char *pSizeInfo = new unsigned char[1024*1024];

	if( mstream.ReadFile ( m_strPlantInfoFile ) )
	{		
		BITMAPFILEHEADER header;	
    	BITMAPINFOHEADER  headInfo;

        mstream.Read ( header );
    	mstream.Read  ( headInfo );

		if( headInfo.biBitCount == 24 )
		{
			int size = headInfo.biWidth * headInfo.biHeight ;

			unsigned char* alphaMap = new unsigned char [3*size];
			mstream.ReadUnChar ( alphaMap , 3*size );

			for( int i = 0 ; i < headInfo.biHeight ; i ++)
			{
				for( int j = 0 ; j < headInfo.biWidth ; j ++)
				{
					pSizeInfo[i*headInfo.biWidth + j ] = alphaMap[ ( headInfo.biHeight  - 1 - i ) * 3 *headInfo.biWidth + j*3 ] ;
				}
			}		
			
			delete [] alphaMap;
		}
		else
		{
			LogMessage( "加载植物灰度图不是24位，根据灰度图创建植被失败！");
			delete[] pSizeInfo;
			return;
		}
	}
	else
	{
		memset( pSizeInfo , 0 , 1024*1024 );
	}

	int posx,posy;
	std::string   str;


	for( unsigned int p = 0 ; p < m_PlantInfoSet.size () ; p ++)
	{
		bool  bfind = false;		

		for( int j = 0 ; j < m_PlantInfoSet[p].plantKindNum ; j ++ )
		{
			str = m_PlantInfoSet[p].plantNameSet [j].plantFileName ;

			for( unsigned int k = 0 ; k < g_TerrainEdit.GetEditData()->modelData .modelFileSet .size () ; k ++)
			{
				if( g_TerrainEdit.GetEditData() ->modelData .modelFileSet [k] == str )
				{
					m_PlantInfoSet[p].indexSet [j] = k;
					bfind = true;
					break;
				}
			}

			if( !bfind )
			{
				g_TerrainEdit.GetEditData() ->modelData .modelFileSet .push_back ( str );
				m_PlantInfoSet[p].indexSet [j] = g_TerrainEdit.GetEditData() ->modelData .modelFileSet .size () - 1;
			}
		}
	}

	for( unsigned int i = 0 ; i < g_TerrainEdit.GetEditData()->modelData .modelPlantSet .size () ; i ++)
	{
		delete( g_TerrainEdit.GetEditData()->modelData .modelPlantSet[i] );
	}

	g_TerrainEdit.GetEditData()->modelData .modelPlantSet .clear ();

	bool bfindModel = false;
	for( unsigned int i = 0 ; i < m_PlantInfoSet.size ()  ; i ++)
	{
	    if( m_PlantInfoSet[i].type != 3)
		{
			bfindModel = true;
			break;
		}
	}

	if( bfindModel )
	{
		for(unsigned  int i = 0 ; i < g_TerrainEdit.GetEditData()->modelData .modelSet  .size () ; i ++)
		{
			delete( g_TerrainEdit.GetEditData()->modelData .modelSet[i] );
		}
		g_TerrainEdit.GetEditData()->modelData .modelSet .clear ();

		for( int i = 0 ;i < TERRTILE_NUMBLOCK_Z ; i ++ )
		{
			for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ; j++ )
			{
				g_TerrainEdit.GetEditData()->chunkSet [j][i].m_ModelSet.clear();
			}
		}
	}

	int nblock = g_TerrainEdit.GetEditData()->gw;
	 for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
	 {
		 for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
		 {
			 posx = i*nblock*64*64 ;
			 posy = j*64;
			
			 MChunk *pChunk = &g_TerrainEdit.GetEditData() ->chunkSet [i][j];
                        
			 pChunk->m_PlantSet .clear ();

			 int index = 0;

			 for( int k = 0 ; k < 64 ; k ++)
			 {
				 for( int l = 0 ; l < 64 ; l++)
				 {
					 int plantKind =  m_PlantMap[posx + posy + k*64*nblock + l] ;

					for( unsigned int p = 0 ; p < m_PlantInfoSet.size () ; p++ )
					 {
						 int  color = m_PlantInfoSet[p].color ;
						if( plantKind ==  color )
						{	
							TerrModel*  model = new TerrModel;
							model->GetModelData ()->gridX = j ;
							model->GetModelData ()->gridY = i ;

							int randID ;
							randID = rand() % 100;

							int randstart = 0 ;
							for( int m = 0 ; m < m_PlantInfoSet[p].plantKindNum ; m++)
							{
								if( randID > randstart && randID <= randstart + m_PlantInfoSet[p].randIdx  [m] )
								{
									randID = m ;
									break;
								}
								else
								{
									randstart += m_PlantInfoSet[p].randIdx  [m];
								}
							}

							//randID = rand() % m_PlantInfoSet[p].plantKindNum ;

							if( randID >= m_PlantInfoSet[p].plantKindNum  )
							{
								randID = 0;
							}

							strcpy(model->GetModelData ()->name , m_PlantInfoSet[p].plantNameSet [randID].plantFileName   );
							model->GetModelData ()->modelID = m_PlantInfoSet[p].indexSet [randID] ;
							model->GetModelData ()->cType = m_PlantInfoSet[p].type ;
							model->SetShow ( false );

							MVec3  pos(0,0,0) , dir(0,0,0) ;
							dir.x = (float)( rand() % 360 );

							float  fscale = ( m_PlantInfoSet[p].minScale + rand() % ( m_PlantInfoSet[p].maxScale - m_PlantInfoSet[p].minScale ) ) / 100.0f;

							pos.x = pChunk->GetChunkData ()->vecData [0].pos .x + TERRUNIT_SIZE / 8.0f * l;
							pos.z = pChunk->GetChunkData ()->vecData [0].pos .z + TERRUNIT_SIZE / 8.0f * k;

							if( m_PlantInfoSet[p].nAutoNormal == 1 )
							{
								MVec3 normal ;
								int nnx ,nny ;
								nnx = l % 8 ;
								nny = k / 8 ;
								int nindex = nnx + nny*17;
								
								normal = pChunk->GetChunkData ()->vecData [nindex].normal ;


								float fry ;
								if ( normal.x == normal.z )
								{
									fry = 0 ;
								}
								else 
								{
									fry = normal.x / normal.z ;
								}

                                float frx;
								if( normal.y == 0 )
								{
									frx = 0;
								}
								else 
								{
									frx = sqrt( normal.x * normal.x + normal.z *normal.z ) / normal.y ; 
								}

								dir.x = atan ( fry ) / MAY_PI *180;
								dir.y = -atan( frx ) / MAY_PI*180;

							}

							g_Terrain.GetHeight ( pos.x , pos.z , &pos.y );

							if( m_PlantInfoSet[p].bUserSizeInfo == 1 ) 
							{
								int plantsizeInfo = pSizeInfo[posx + posy + k*64*nblock + l] ;
							
								if ( plantsizeInfo == 255 )
								{
									//fscale = m_PlantInfoSet[p].maxScale / 100.0f;
								}
								else 
								{
									fscale = ( m_PlantInfoSet[p].minScale + ( m_PlantInfoSet[p].maxScale - m_PlantInfoSet[p].minScale )*( plantsizeInfo / 254.0f ) ) / 100.0f;
								}
							}
 
							model->GetModelData ()->pos = pos ;
							model->GetModelData ()->dir = dir ;
							model->GetModelData ()->scale = fscale;
							model->GetModelData ()->mapIndex = k*64 + l ;

							if( m_PlantInfoSet[p].type == 3 )
							{
								g_TerrainEdit.GetEditData()->modelData .modelPlantSet .push_back ( model );	
								pChunk->m_PlantSet .push_back ( model );
							}
							else
							{		
								g_TerrainEdit.GetEditData() ->modelData .modelSet .push_back ( model );

								pChunk->m_ModelSet .push_back ( model );
							}
						}
					 }
				 }
			 }			 
		 }
	 }


	 delete [] pSizeInfo;

}

void    CPlantManager::UpdateDataPlant()
{
	return;
};
	
void    CPlantManager::Render()
{
}

void    CPlantManager::Release()
{
	for(unsigned int i = 0 ; i < m_PlantManager.size () ;  i ++)
	{
		OGRE_DELETE( m_PlantManager[i] );
	}

	m_PlantManager.clear();

	m_PlantInfoSet.clear();
	m_TextureFileSet.clear();
};




