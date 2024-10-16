#include "ui_modelview.h"
#include "OgreCamera.h"
#include "OgreGameScene.h"
#include "OgreLight.h"
#include "AvatarEntity.h"
#include "OgreResourceManager.h"
#include "OgreSkeleton.h"
#include "ui_framemgr.h"
#include "OgreAnimationPlayer.h"
#include "OgreRenderTarget.h"
#include "OgreHardwarePixelBuffer.h"
#include "ui_frame.h"
#include "Equip_Common.h"
#include "../include/comm/proto_comm.h"
#include "../include/comm/proto_cs.h"
#include "../include/comm/resdb_meta.h"
#include <string>
#include <utility>
using std::string;

using namespace Ogre;

extern FrameManager *g_pFrameMgr;

ModelViewTextureData::ModelViewTextureData() : m_hRes( NULL ), m_pModelTexture( NULL ), m_pRenderTarget( NULL )
{}

ModelViewTextureData::~ModelViewTextureData()
{
	/*g_pDisplay->ReleaseUIRes( m_hRes );
	OGRE_RELEASE( m_pRenderTarget );
	OGRE_RELEASE( m_pModelTexture );*/
}


IMPLEMENT_UIOBJECT_CLONE(ModelView)
ModelView::ModelView() : m_pScene(NULL), m_pCamera(NULL), m_pCamAnimMod(NULL)
{
	memset(m_pEntities, 0, sizeof(m_pEntities));
	memset(m_pAppends, 0, sizeof(m_pAppends));
	memset( m_AvatarAttrib, 0, sizeof(m_AvatarAttrib) );
	memset( m_AvatarStar, 0, sizeof(m_AvatarStar) );

	m_pCamera = new Camera;
	m_pCamera->setFov(33.0f);
	m_pCamera->setLookAt(Vector3(0, 150.0f, -300.0f), Vector3(0, 100.0f, 0), Vector3(0, 1.0f, 0));
	m_pScene = new SimpleGameScene;

	Light *plight = new Light(LT_DIRECT);
	Vector3 dir(1.0f,1.0f, 1.0f);
	Normalize(dir);
	plight->setDirection(dir);
	plight->m_AmbientColor = ColourValue(1.0f, 1.0f, 1.0f);
	plight->m_DiffuseColor = ColourValue(1.0f, 1.0f, 1.0f);
	plight->enableAmbient();
	plight->enableShadow();
	plight->attachToScene(m_pScene);
	plight->release();
}

ModelView::~ModelView()
{
	for ( int i = 0, size = (int)m_BGObjs.size(); i < size; i++ )
	{
		m_BGObjs[i]->detachFromScene();
		m_BGObjs[i]->release();
	}

	for ( int i = 0; i < ARRAY_ELEMENTS( m_pEntities ); i++ )
	{
		if ( m_pEntities[i] != NULL )
		{
			m_pEntities[i]->detachFromScene();
			m_pEntities[i]->release();
		}
	}
	for ( int i = 0; i < ARRAY_ELEMENTS( m_pAppends ); i++ )
	{
		if ( m_pAppends[i] != NULL )
		{
			m_pAppends[i]->detachFromScene();
			m_pAppends[i]->release();
		}
	}

	OGRE_RELEASE(m_pCamAnimMod);
	OGRE_RELEASE(m_pCamera);
	OGRE_RELEASE(m_pScene);
	
	if ( !m_strTextureResPath.empty() )
	{
		Ogre::UITargetEffect* pUiTarget = g_pDisplay->getUITargetEffect( m_strTextureResPath.c_str() );
		Ogre::HUIRES hRes = g_pDisplay->GetTextureRes( m_strTextureResPath.c_str() ); 
		g_pDisplay->ReleaseUIRes( hRes );
		OGRE_RELEASE( pUiTarget->m_pUIRT );
	}
}

void ModelView::CopyMembers(ModelView *pdest)
{
	if (pdest)
	{
		LayoutFrame::CopyMembers(pdest);
	}
}

void ModelView::setBackground(const char *path)
{
	addBackground(path);
}

void ModelView::addBackground(const char *path)
{
	Entity *pentity = new Entity;
	pentity->load(path, true);
	pentity->attachToScene(m_pScene);

	m_BGObjs.push_back(pentity);
}

void ModelView::setAvatarAttrib(int index, int grid, int id, int starLv, bool bfirst )
{
	assert(grid < 32);
	if( bfirst )
	{
		memset(m_AvatarAttrib[index], 0, sizeof(m_AvatarAttrib[index]));
		memset(m_AvatarStar[index], 0, sizeof(m_AvatarStar[index]));
	}

	m_AvatarAttrib[index][grid] = id;
	m_AvatarStar[index][grid] = starLv;
	int WeaponId = m_AvatarAttrib[index][EquipPos_2_PosId[EQUIP_POS_RIGHTHAND]];
	if (WeaponId / 10000 == ARM_HEVY_SCATTERGUN)
	{
		m_AvatarAttrib[index][EquipPos_2_PosId[EQUIP_POS_LEFT]] = 0;
	}
	if ( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->updateAttrib(m_AvatarAttrib[index],m_AvatarStar[index]);
	}
}

int ModelView::getAvatarAttrib(int index,int grid)
{
	if( grid >=32 )
	{
		assert(0);
		return 0;
	}
	return m_AvatarAttrib[index][grid];
}


void ModelView::setEntityAvatar(int index )
{
	assert( index < 16 );
	bool need_attach = false;
	if ( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->detachFromScene();
		m_pEntities[index]->release();
	}

	m_pEntities[index] = new AvatarEntity;

	m_pEntities[index]->updateAttrib( m_AvatarAttrib[index],m_AvatarStar[index] );

	if ( m_AvatarAttrib[index][1] == 0 ) 
	{
		m_pEntities[index]->playAnim( 24100 );
	}
	else 
	{
		m_pEntities[index]->playAnim( 30100 );
	}

	m_pEntities[index]->update( 0 );

	m_pEntities[index]->attachToScene( m_pScene );
	CreateModelTexture();
}

bool ModelView::isPlayingAnim( int nIndex )
{
	Ogre::AvatarEntity* pEntity = m_pEntities[nIndex];
	if ( pEntity == NULL )
	{
		return false;
	}

	return pEntity->IsPlaying();
}

void ModelView::setPetModel( int index, int nAnimId, const char* szModelPath, const char *szAppendPath/* = NULL*/ )
{
	if ( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->detachFromScene();
		m_pEntities[index]->release();
	}
	if ( m_pAppends[index] != NULL )
	{
		m_pAppends[index]->detachFromScene();
		m_pAppends[index]->release();
		m_pAppends[index] = NULL;
	}

	m_pEntities[index] = new AvatarEntity;	
	m_pEntities[index]->load( szModelPath,false );
	m_pEntities[index]->getMainModel();
	m_pEntities[index]->playAnim( nAnimId );
	m_pEntities[index]->update( 0 );
	m_pEntities[index]->attachToScene( m_pScene );
	if (szAppendPath != NULL && szAppendPath[0] != '\0')
	{
		m_pAppends[index] = new Entity;
		m_pAppends[index]->load( szAppendPath,true );
		m_pAppends[index]->update( 0 );
		m_pAppends[index]->attachToScene( m_pScene );
	}
	CreateModelTexture();
}

void ModelView::setPetScale(int index, float x, float y, float z )
{
	if(  m_pEntities[index] == NULL )
	{
		assert(0&&"entities is null");
		return;
	}
	m_pEntities[index]->setScale( Vector3( x, y, z ) );
	if (m_pAppends[index] != NULL)
	{
		m_pAppends[index]->setScale( Vector3( x, y, z ) );
	}
}

void ModelView::setEntityPosition(int index, float x, float y, float z)
{
	if(  m_pEntities[index] == NULL )
	{
		assert(0&&"entities is null");
		return;
	}
	m_pEntities[index]->setPosition( Vector3( x, y, z ) );
	if (m_pAppends[index] != NULL)
	{
		m_pAppends[index]->setPosition( Vector3( x, y, z ) );
	}
}

void ModelView::setEntityRotate(int index, float angle)
{
	if(  m_pEntities[index] == NULL )
	{
		assert(0&&"entities is null");
		return;
	}
	m_pEntities[index]->setRotation(angle, 0, 0);
	if (m_pAppends[index] != NULL)
	{
		m_pAppends[index]->setRotation(angle, 0, 0);
	}
}
void ModelView::setEntieyTextureByID( int index, int id, const char* szPath )
{
	if ( m_pEntities[index] == NULL )
	{
		assert(0&&"entities is null");
		return;
	}
	m_pEntities[index]->setTextureByID( id, szPath );
}

void ModelView::playEntityAnim(int index, int seq, const char *motion)
{
	int num = motion==NULL?0:1;
	if(  m_pEntities[index] == NULL )
	{
		assert(0&&"entities is null");
		return;
	}
	m_pEntities[index]->playMotion( seq, &motion, num );
}

bool ModelView::isHaveCameraAnim()
{
	return m_pCamAnimMod != NULL;
}

void ModelView::setCameraAnim( const char* path )
{
	Resource* pres = ResourceManager::getSingleton().blockLoad( path );
	if ( pres==NULL || !IS_KIND_OF( ModelData, pres ) )
	{
		return;
	}

	m_pCamAnimMod  = new Model( static_cast<ModelData*>( pres ) );
	pres->release();
}

void ModelView::playCameraAnim( int seq, int fadetick )
{
	float dtime = TickToTime( fadetick );
	m_pCamAnimMod->getAnimPlayer()->setAnimDelayInOut( dtime, dtime );
	m_pCamAnimMod->playAnim( seq );
}

void ModelView::UpdateSelf(float deltatime)
{
	if ( !m_bShow )
	{
		return;
	}
	AdjustFrameByViewPoint();

	uint dtick = TimeToTick( deltatime );
	for ( int i = 0; i < ARRAY_ELEMENTS( m_pEntities ); i++ )
	{
		if ( m_pEntities[i] != NULL ) 
		{
			m_pEntities[i]->update(dtick);
		}
	}
	for ( int i = 0; i < ARRAY_ELEMENTS( m_pAppends ); i++ )
	{
		if ( m_pAppends[i] != NULL ) 
		{
			m_pAppends[i]->update(dtick);
		}
	}

	for ( int i=0; i < (int)m_BGObjs.size(); i++ )
	{
		m_BGObjs[i]->update(dtick);
	}

	updateCameraAnim( dtick );
}

void ModelView::NormalDraw()
{
	int x			= m_AbsRect.m_Left;
	int y			= m_AbsRect.m_Top;
	int width		= m_AbsRect.getWidth();
	int height		= m_AbsRect.getHeight();

	if ( width == 0 || height == 0 )
	{
		return;
	}

	m_pCamera->setRatio( float( width ) / height );
	m_pCamera->setViewport( (float)x, (float)y, (float)width, (float)height );
	m_pCamera->update( 0 );

	g_pDisplay->renderSceneToUI( m_pScene, m_pCamera, m_AbsRect );
}

void ModelView::DrawOutOfScreen()
{
	Ogre::HUIRES hRes = g_pDisplay->GetTextureRes( m_strTextureResPath.c_str() ); 
	if ( hRes == NULL )
	{
		return;
	}

	int x			= 0;
	int y			= 0;
	int width		= m_AbsRect.getWidth();
	int height		= m_AbsRect.getHeight();

	m_pCamera->setRatio( float( width ) / height );
	m_pCamera->setViewport( (float)x, (float)y, (float)width, (float)height );
	m_pCamera->update( 0 );
	
	int nOffsetX = 0;
	if ( m_AbsRect.m_Left < 0 )
	{
		nOffsetX = abs( m_AbsRect.m_Left );
	}
	else
	{
		nOffsetX -= m_AbsRect.m_Left;
	}

	Ogre::UITargetEffect* pUiTarget = g_pDisplay->getUITargetEffect( m_strTextureResPath.c_str() );
	assert( pUiTarget != NULL );

	Ogre::RectInt srcRect( m_AbsRect );
	Ogre::RectInt destRect( 0, 0, m_AbsRect.getWidth(), m_AbsRect.getHeight() );
	if( srcRect.m_Left < 0 )
	{
		destRect.m_Left += abs(srcRect.m_Left);
		srcRect.m_Left = 0;
	}
	if( srcRect.m_Top < 0 )
	{
		destRect.m_Top += abs(srcRect.m_Top);
		srcRect.m_Top = 0;
	}
	g_pDisplay->renderSceneToTarget( m_pScene, m_pCamera, srcRect, destRect, pUiTarget->m_pUIRT );

#ifdef _DEBUG
	static int nCounter = 0;

	if ( nCounter < 10 )
	{
		char buffer[256];
		sprintf( buffer, "在位置1\\target_%d.dds", nCounter );
		m_ModelTexData.m_pRenderTarget->getColorTexture()->dumpToDDS( buffer );
		++nCounter;
	}
#endif
	
	g_pDisplay->BeginDraw( hRes );

	g_pDisplay->StretchRect((float)m_AbsRect.m_Left, (float)m_AbsRect.m_Top, 
		(float)m_AbsRect.getWidth(), (float)m_AbsRect.getHeight(), 
		Ogre::ColorQuad(255, 255, 255,255).c, false, x, y, 
		m_AbsRect.getWidth(), m_AbsRect.getHeight(), Ogre::UI_UVT_NORMAL, 0, 0, 0 );

	g_pDisplay->EndDraw();
}

void ModelView::Draw()
{
	if( m_AbsRect.m_Right <= 0 || m_AbsRect.m_Bottom <=0 )
	{
		return ;
	}
	int x			= m_AbsRect.m_Left;
	int y			= m_AbsRect.m_Top;
	int width		= m_AbsRect.getWidth();
	int height		= m_AbsRect.getHeight();
	if ( x <  0 || y < 0 )
	{
		DrawOutOfScreen();
	}
	else
	{
		NormalDraw();
	}
}

// 创建纹理
void ModelView::CreateModelTexture()
{
	m_strTextureResPath = string( GetName() ) + string( "virtual path" );
	// 创建一次即保存在屏幕纹理管理器中，不要每一帧的都创建删除，但是
	if ( g_pDisplay->IsCreatedUIRenderTarget( m_strTextureResPath ) )
	{
		return;
	}

	TextureDesc desc;
	desc.type		= TEXTURE_2D;
	desc.width		= m_AbsRect.getWidth();
	desc.height		= m_AbsRect.getHeight();
	desc.depth		= 1;
	desc.format		= PF_X8R8G8B8;
	desc.nmipmap	= 1;
	desc.cubefaces	= 0;

	m_ModelTexData.m_pModelTexture	= new RT_TEXTURE( desc,HBU_RENDERTARGET );
	m_ModelTexData.m_pRenderTarget	= m_ModelTexData.m_pModelTexture->getHardwareTexture()->createRenderTarget( 0, 16, 0 );
	g_pDisplay->AddUIRenderTarget( m_strTextureResPath.c_str(), m_ModelTexData.m_pModelTexture, 
									m_ModelTexData.m_pRenderTarget );
}

void ModelView::updateCameraAnim(unsigned int dtick)
{
	if ( m_pCamAnimMod == NULL )
	{
		return;
	}

	// 这不会执行到
	m_pCamAnimMod->update( dtick );

	int boneid = m_pCamAnimMod->getSkeletonInstance()->findBoneID("Camera01");
	if(boneid >= 0)
	{
		Matrix4 tm = m_pCamAnimMod->getSkeletonInstance()->getBoneTM(boneid);
		Vector3 dir(0, -1.0, 0), up(0, 0, 1.0f);
		tm.transformNormal(dir, dir);
		tm.transformNormal(up, up);
		m_pCamera->setLookDirect(tm.getTranslate(), dir, up);
	}
}
