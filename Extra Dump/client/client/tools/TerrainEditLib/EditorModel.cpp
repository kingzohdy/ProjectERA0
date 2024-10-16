
#include "EditorModel.h"
#include "OgreModel.h"
#include "Terrain.h"

using namespace Ogre;

namespace TE
{
	EditorModel::EditorModel() 
	{
		m_pModel = NULL;
		m_pLinkMesh = NULL;

		m_Dir = Ogre::Vector3(0, 0, 1.0f);
		m_Scale = 1.0f;
	}

	EditorModel::~EditorModel()
	{
		Release();
	}

	void  EditorModel::Release()
	{
		OGRE_RELEASE( m_pModel );	

		m_LinkMeshVec.clear();
		m_LinkMeshHeight.clear();
	}

	float EditorModel::GetLinkMeshHeight( int index )
	{
		if ( index < 0 || index >=  (int)m_LinkMeshVec.size() )
			return 0;

		return m_LinkMeshVec[index].y ;
	}

	bool EditorModel::LoadModel(const char* filename  )
	{
		std::string  filenameex = filename;

		m_pModel = new Entity;
		if(!m_pModel->load(filename, true))
		{
			m_pModel->release();
			m_pModel = NULL;
			return false;
		}

		m_pModel->playAnim(0);
		m_pModel->update(0);

		MeshInstance *plinkmesh = NULL;
		if(m_pModel->getMainModel())
		{
			for(size_t i=0; i<m_pModel->getMainModel()->getNumSkin(); i++)
			{
				MeshInstance *pmesh = m_pModel->getMainModel()->getIthSkin(i);
				const char *pname = (const char *)pmesh->getName();
				if(strstr(pname, "link") == pname)
				{
					plinkmesh = pmesh;
					break;
				}
			}
		}

		if(plinkmesh)
		{     
			if( filenameex.find("linkmesh") != -1)
			{
				m_ucModleType = LINK_MESH;
			}
			else if ( filenameex.find( "basemodel" ) != -1 )
			{
				m_ucModleType = PAINT_TEXTURE_MESH ;
			}
			else if ( filenameex.find("braemesh") != -1 )
			{
				m_ucModleType = BRAE_MESH ;
			}
			else if ( filenameex.find("linktexture") )
			{

				m_ucModleType = LINK_TEXTURE;
			}
			else
			{
				m_ucModleType = LINK_MESH ;
			}

			plinkmesh->makeInstance(INSTANCE_VB);
			m_pLinkMesh = plinkmesh;

			size_t verNum = m_pLinkMesh->getNumVertex();
			m_LinkMeshVec.resize( verNum);
			m_LinkMeshHeight.resize( verNum );
			m_LinkMeshColor.resize( verNum );

			size_t index = 0 ;
			for(size_t j= 0; j<plinkmesh->getNumSubMesh(); j++)
			{
				VertexData *pvd = plinkmesh->getSubMesh(j)->getVertexData();

				for( size_t i = 0 ; i < pvd->getNumVertex(); i ++ )
				{
					m_LinkMeshVec[index] = *(Vector3 *)pvd->getPosition(i);
					m_LinkMeshHeight[index] = m_LinkMeshVec[index].y;

					m_LinkMeshColor[index] = 0xff000000 ;//(unsigned int )( pData[i*( vecLen / sizeof(float))+nColorOff ] );
					index++;
				}
			}
		}

		return true;
	};	

	void    EditorModel::ClearLightColor()
	{

	};

	inline bool   TestBoxAndBox( Ogre::Vector3 minSrc , Ogre::Vector3 maxSrc , Ogre::Vector3 minDes , Ogre::Vector3 maxDes )
	{
		//创建8个点，判断是否在在源包围盒
		Ogre::Vector3  vec[8];
		vec[0].x = minDes.x ; vec[0].y = minDes.y ; vec[0].z = minDes.z ;
		vec[1].x = maxDes.x ; vec[1].y = minDes.y ; vec[1].z = minDes.z ;
		vec[2].x = minDes.x ; vec[2].y = minDes.y ; vec[2].z = maxDes.z ;
		vec[3].x = maxDes.x ; vec[3].y = minDes.y ; vec[3].z = maxDes.z ;

		vec[4].x = minDes.x ; vec[4].y = maxDes.y ; vec[4].z = minDes.z ;
		vec[5].x = maxDes.x ; vec[5].y = maxDes.y ; vec[5].z = minDes.z ;
		vec[6].x = minDes.x ; vec[6].y = maxDes.y ; vec[6].z = maxDes.z ;
		vec[7].x = maxDes.x ; vec[7].y = maxDes.y ; vec[7].z = maxDes.z ;

		for( int i = 0 ; i < 8 ; i ++ )
		{
			if( vec[i].x >= minSrc.x && vec[i].x <= maxSrc.x )
			{	
				if( vec[i].y >= minSrc.y && vec[i].y <= maxSrc.y )
				{
					if( vec[i].z >= minSrc.z && vec[i].z <= maxSrc.z )
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	void   EditorModel::CountLightColor( LightInfo &lightInfo )
	{
		if(m_pModel)
		{	
			BoxSphereBound bound;
			m_pModel->getMainModel()->getLocalBounds(bound);

			/*MVec3  minDes,maxDes;
			minDes = pBox->GetMinPos() + m_wPos ;
			maxDes = pBox->GetMaxPos() + m_wPos ;

			MVec3  minSrc,maxSrc ;
			minSrc.x = lightInfo.pos .x - lightInfo.fRange ;  
			minSrc.y = lightInfo.pos .y - lightInfo.fRange ;  
			minSrc.z = lightInfo.pos .z - lightInfo.fRange ;  
			maxSrc.x = lightInfo.pos .x + lightInfo.fRange ;  
			maxSrc.y = lightInfo.pos .y + lightInfo.fRange ;  
			maxSrc.z = lightInfo.pos .z + lightInfo.fRange ;  
			*/

			//测试函数
			//	if( !TestBoxAndBox( minSrc , maxSrc , minDes , maxDes ) )
			//	return;

			float range = lightInfo.fRange ;
			float drange = range*range ;

			unsigned char r,g,b ;
			unsigned char r1,g1,b1 ;

			int index = 0 ;

			for ( size_t j = 0 ;j <m_pLinkMesh->getNumSubMesh(); j ++ )
			{
				VertexData *pvd = m_pLinkMesh->getSubMesh(j)->getVertexData();
				for(size_t i=0; i<pvd->getNumVertex(); i++)
				{
					Ogre::Vector3 pos = *(Vector3 *)pvd->getPosition(i);
					Ogre::Vector3 normal = *(Vector3 *)pvd->getNormal(i);

					Ogre::Vector3 pos0 = m_wPos + pos ;
					Ogre::Vector3 dst = lightInfo.pos - pos0 ;

					float fLength = dst.lengthSqr() ;
					if( fLength >= drange )
					{
						index ++;
						continue;
					}

					Normalize( dst );
					Normalize( normal );

					float fdot = DotProduct( normal , dst ) ;
					if( fdot <= 0 )
					{
						index ++;
						continue;
					}

					float strength = Sqrt( fLength ) / range ;
					float k = 1.0f - strength * strength ;

					if( k < 0 )
					{
						k = 0 ;
					}

					r = unsigned char( ( k*lightInfo.color.r * fdot )*255 ) ;
					g = unsigned char( ( k*lightInfo.color.g * fdot )*255 ) ;
					b = unsigned char( ( k*lightInfo.color.b * fdot )*255 ) ;

					r1 = ( m_LinkMeshColor[index] & 0x00ff0000 ) >> 16 ;
					g1 = ( m_LinkMeshColor[index] & 0x0000ff00 ) >> 8 ;
					b1 =   m_LinkMeshColor[index] & 0x000000ff ;

					r = Min(255, r+r1);
					g = Min(255, g+g1);
					b = Min(255, b+b1);

					m_LinkMeshColor[index] =    ( r << 16 ) | ( g << 8 ) | b ;

					index ++;
				}
			}
		}
	};

	void   EditorModel::UpdateNormal()
	{
		if ( m_pModel )
		{		
			int index = 0 ;

			for ( size_t j = 0 ;j <m_pLinkMesh->getNumSubMesh(); j ++ )
			{
				VertexData *pvd = m_pLinkMesh->getSubMesh(j)->getVertexData();
				for(size_t i=0; i<pvd->getNumVertex(); i++)
				{
					Vector3 vPos = *(Vector3 *)pvd->getPosition(i);
					g_Terrain.GetNormal( vPos.x , vPos.z , (Vector3 *)pvd->getNormal(i));
					index ++;
				}
			}
		}
	};

	void  EditorModel::UpdateMesh(Ogre::Vector3 startPos)
	{
		if ( m_pModel )
		{
			m_wSavePos = m_wPos ;

			int index = 0 ;

			if(m_pLinkMesh == NULL)
			{
				return;
			}

			for ( size_t j = 0 ;j <m_pLinkMesh->getNumSubMesh(); j ++ )
			{
				BoxBound box;
				VertexData *pvd = m_pLinkMesh->getSubMesh(j)->getVertexData();
				pvd->lock();
				for(size_t i=0; i<pvd->getNumVertex(); i++)
				{
					Ogre::Vector3 vPos = m_LinkMeshVec[index] + startPos ;
					float curHeight ;
					float fHeight ;

					if( m_ucModleType == LINK_TEXTURE )
					{
						g_Terrain.GetHeight( vPos.x , vPos.z , &curHeight );
						g_Terrain.GetNormal( vPos.x , vPos.z , (Vector3 *)pvd->getNormal(i));
						fHeight =  0.1f;
					}
					else if ( m_ucModleType == PAINT_TEXTURE_MESH )
					{
						g_Terrain.GetHeight( vPos.x , vPos.z , &curHeight );
						g_Terrain.GetNormal( vPos.x , vPos.z , (Vector3 *)pvd->getNormal(i));
						fHeight = 0 ;
					}
					else if ( m_ucModleType == BRAE_MESH )
					{
						g_Terrain.GetHeightEx( vPos.x , vPos.z , &curHeight );
						fHeight = 0.2f ;
					}
					else
					{			
						g_Terrain.GetHeightEx( vPos.x , vPos.z , &curHeight );
						fHeight = 0.2f ;
					}

					{
						vPos.x = vPos.x - startPos.x ;
						vPos.y = vPos.y  + ( curHeight - startPos.y  ) - startPos.y + fHeight ;
						vPos.z = vPos.z - startPos.z ;

						*(Vector3 *)pvd->getPosition(i) = vPos;
						m_LinkMeshHeight[index] = vPos.y ;

						box += vPos;
					}
					index ++;
				}
				pvd->unlock();
				pvd->m_MinPos = box.getMinPos();
				pvd->m_MaxPos = box.getMaxPos();
			}
		}
	};

	bool  EditorModel::LoadBuildingModel(const char* filename )
	{
		/*	BuildingRoomChunk* pChunk = (BuildingRoomChunk*)LoadChunkFromFile("chunkdata", filename);
		BuildingRoom *proom = MAY_NEW BuildingRoom(pChunk->m_pBuildingRoomSource);
		delete pChunk;

		proom->SetAmbientLight(0.5f, 0.5f, 0.5f);

		g_pEngine->GetSceneMgr()->AttachRoom(proom);
		MAY_RELEASE(proom);

		g_pEngine->GetSceneMgr()->AttachRoom( proom );

		SetPos(MVec3(0,0,0));*/


		return true;
	};

	void  EditorModel::Update( float time )
	{
		if( m_pModel )
		{
			m_pModel->update(TimeToTick(time));
		}
	};

	void EditorModel::UpdatePos()
	{
		Ogre::Vector3  scale;
		scale.x = scale.y = scale.z = m_Scale ;

		if(m_pModel)
		{
			m_pModel->setPosition ( m_wPos );
			m_pModel->setScale ( scale );

			m_pModel->setRotation ( m_Dir.x , m_Dir.y , m_Dir.z );
		}
	}

	void EditorModel::SetPos(Ogre::Vector3 pos)
	{
		m_wPos = pos ;	 

		UpdateMesh( pos );


		if(m_pModel)
		{
			m_pModel->setPosition ( m_wPos );
		}

	};

	void EditorModel::SetDir(Ogre::Vector3 dir)
	{
		m_Dir = dir;

		if(m_pModel)
		{
			m_pModel->setRotation ( dir.x , dir.y , dir.z );
		}

	};

	void EditorModel::SetScale(float scale)
	{
		m_Scale = scale;

		Ogre::Vector3  vScale;
		vScale.x = vScale.y = vScale.z = m_Scale ;

		if(m_pModel)
		{
			m_pModel->setScale ( vScale );
		}
	};

	void EditorModel::SetLinkMeshTexture(Ogre::Texture * ptex)
	{
		if(m_pLinkMesh)
		{
			m_pLinkMesh->setTexture("g_DiffuseTex", ptex);
		}
	}
}

