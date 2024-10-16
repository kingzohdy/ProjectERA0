#include <Windows.h>
#include "GameBackgroundScene.h"
#include "GameManager.h"
#include "GamePlayManager.h"
#include "GameCamera.h"
#include "ShowGameMap.h"
#include "GameDBData.h"

#include "OgreCamera.h"
#include "OgreGameScene.h"
#include "OgreLight.h"
#include "AvatarEntity.h"
#include "OgreResourceManager.h"
#include "OgreSkeleton.h"
#include "OgreAnimationPlayer.h"
#include "ogrebloom.h"
#include "OgreShadowmap.h"
#include "OgreSoundSystem.h"
#include "OgreBorderScene.h"

#include "OgreRoot.h"

using namespace Ogre;

static const char* PULL_CONTROL_FILE_PATH	= "client.cfg";

const float GameBackground::m_fFarScale		= 1.0f;
static unsigned int	s_uConfigMaxPullTime	= 2000u;
static unsigned int	s_uConfiImmediateTime	= 2000u;

struct tagDefaultPosConfig
{
	float	m_fX;
	float	m_fY;
	float	m_fZ;
};

const int MAX_GENDER = 2;
const int MAX_CAREER = 4;

static tagDefaultPosConfig s_backGroundFarAnimConfig[MAX_GENDER+1][MAX_CAREER+1] = 
{
	0.0f
}; 

static tagDefaultPosConfig s_backGroundNearAnimConfig[MAX_GENDER+1][MAX_CAREER+1] = 
{
	0.0f
};

GameBackground::GameBackground(int mapid) : m_MapID(mapid), m_pScene(NULL), m_pCamera(NULL), m_pCamAnimMod(NULL), 
											m_bShow(false), m_pGameMap(NULL)
{
	memset(m_pEntities, 0, sizeof(m_pEntities));
	memset(m_pAppends, 0, sizeof(m_pAppends));

	m_pCamera = new Camera;
	m_pCamera->setFov(50.0f);
	m_pCamera->setDepth(50.0f, 50000.0f);
	m_pCamera->setLookAt(Vector3(0, 250.0f, -300.0f), Vector3(0, 100.0f, 0), Vector3(0, 1.0f, 0));

	setCameraAnim("scene\\model\\DL\\dl_cam.omod");
	
	memset( m_AvatarAttrib, 0, sizeof(m_AvatarAttrib) );
	memset( m_AvatarStar, 0, sizeof(m_AvatarStar) );

	for(size_t i=0; i<ARRAY_ELEMENTS(m_EntityTargetAlpha); i++)
	{
		m_EntityTargetAlpha[i] = 1.0f;
		m_EntityStepAlpha[i] = 0;
	}

	loadPullSceneParam();
	m_animCameraConfigKey.m_nCareer = 0;
	m_animCameraConfigKey.m_nGender	= 0;

	const tagDefaultPosConfig& noneDefaultPosConfig = s_backGroundNearAnimConfig[0][0];
	m_pullInfo.m_fXCurScale	= noneDefaultPosConfig.m_fX;
	m_pullInfo.m_fYCurScale	= noneDefaultPosConfig.m_fY;
	m_pullInfo.m_fZCurScale	= noneDefaultPosConfig.m_fZ;
	m_nViewFlag = 0;
}

GameBackground::~GameBackground()
{
	unloadres();

	OGRE_RELEASE(m_pCamAnimMod);
	OGRE_RELEASE(m_pCamera);
} 

void GameBackground::loadPullSceneParam()
{
	const char* pszBackGroundSceneConfig = "uires\\Scripts\\Config\\LoginCamera.xml";
	Ogre::XMLData xmlData;
	if( !xmlData.loadFile( pszBackGroundSceneConfig ) )
	{
		return;
	}
	
	XMLNode root			= xmlData.getRootNode();
	XMLNode backGroundNode	= root.getChild( "BackGround" );
	if ( backGroundNode.isNull() )
	{
		return;
	}

	s_uConfigMaxPullTime		= backGroundNode.attribToFloat( "maxPullTime" ) * 1000;
	s_uConfiImmediateTime		= backGroundNode.attribToFloat( "immediateTime" ) * 1000;
	
	XMLNode roleConfigNode	= backGroundNode.iterateChild( );
	while ( !roleConfigNode.isNull() )
	{
		if ( stricmp( roleConfigNode.getName(), "FarSingleRole" ) == 0 )
		{
			int nCareer		= roleConfigNode.attribToInt( "career" );
			int nGender		= roleConfigNode.attribToInt( "gender" );
			float fDefaultX	= roleConfigNode.attribToFloat( "x" );
			float fDefaultY	= roleConfigNode.attribToFloat( "y" );
			float fDefaultZ	= roleConfigNode.attribToFloat( "z" );
			
			if ( nCareer > MAX_CAREER || nGender > MAX_GENDER )
			{
				assert( false && "配置表错误" );
				continue;
			}

			tagDefaultPosConfig& oneDefaultPosConfig = s_backGroundFarAnimConfig[nGender][nCareer];
			oneDefaultPosConfig.m_fX = fDefaultX;
			oneDefaultPosConfig.m_fY = fDefaultY;
			oneDefaultPosConfig.m_fZ = fDefaultZ;
		}
		
		if ( stricmp( roleConfigNode.getName(), "NearSingleRole" ) == 0 )
		{
			int nCareer		= roleConfigNode.attribToInt( "career" );
			int nGender		= roleConfigNode.attribToInt( "gender" );
			float fDefaultX	= roleConfigNode.attribToFloat( "x" );
			float fDefaultY	= roleConfigNode.attribToFloat( "y" );
			float fDefaultZ	= roleConfigNode.attribToFloat( "z" );

			if ( nCareer > MAX_CAREER || nGender > MAX_GENDER )
			{
				assert( false && "配置表错误" );
				continue;
			}

			tagDefaultPosConfig& oneDefaultPosConfig = s_backGroundNearAnimConfig[nGender][nCareer];
			oneDefaultPosConfig.m_fX = fDefaultX;
			oneDefaultPosConfig.m_fY = fDefaultY;
			oneDefaultPosConfig.m_fZ = fDefaultZ;
		}

		roleConfigNode	= backGroundNode.iterateChild( roleConfigNode );
	}
}

void GameBackground::loadres()
{
	if(m_pGameMap != NULL) return;

	m_pGameMap = new ShowGameMap;
	MAPDEF mapdef;
	memset(&mapdef, 0, sizeof(mapdef));
	mapdef.MapID = m_MapID;
	m_pGameMap->LoadFromMapDesc(mapdef, WorldPos(0, 0, 0));
	m_pScene = m_pGameMap->getGameScene();
	m_pScene->addRef();
}

void GameBackground::unloadres()
{
	size_t i;
	for(i=0; i<ARRAY_ELEMENTS(m_pEntities); i++)
	{
		if(m_pEntities[i])
		{
			m_pEntities[i]->detachFromScene();
			OGRE_RELEASE(m_pEntities[i]);
		}
	}
	for(i=0; i<ARRAY_ELEMENTS(m_pAppends); i++)
	{
		if(m_pAppends[i])
		{
			m_pAppends[i]->detachFromScene();
			OGRE_RELEASE(m_pAppends[i]);
		}
	}
	OGRE_RELEASE(m_pScene);
	OGRE_DELETE(m_pGameMap);
}

void GameBackground::setAvatarAttrib(int index, int grid, int id, int starLv, bool bfirst )
{
	assert(grid < 32);
	if( bfirst )
	{
		memset(m_AvatarAttrib[index], 0, sizeof(m_AvatarAttrib[index]));
		memset(m_AvatarStar[index], 0, sizeof(m_AvatarStar[index]));
	}

	m_AvatarAttrib[index][grid] = id;	
	m_AvatarStar[index][grid] = starLv;
}

void GameBackground::updateAttrib(int index)
{
	if( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->updateAttrib(m_AvatarAttrib[index],m_AvatarStar[index]);
	}
}

int GameBackground::getAvatarAttrib(int index,int grid)
{
	if( grid >=32 )
	{
		assert(0);
		return 0;
	}
	return m_AvatarAttrib[index][grid];
}

void GameBackground::setEntityAvatar(int index )
{
	if (m_pGameMap==NULL || m_pScene == NULL )
	{
		return;
	}
	assert(index < 16);
	bool need_attach = false;
	if(m_pEntities[index] != NULL)
	{
		m_pEntities[index]->detachFromScene();
		OGRE_RELEASE( m_pEntities[index] );
	}
	if(m_pAppends[index] != NULL)
	{
		m_pAppends[index]->detachFromScene();
		OGRE_RELEASE( m_pAppends[index] );
	}

	m_pEntities[index] = new AvatarEntity;
	m_pEntities[index]->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f, 0.1f));

	m_pEntities[index]->updateAttrib(m_AvatarAttrib[index],m_AvatarStar[index]);

	if(m_AvatarAttrib[index][1] == 0)
	{
		m_pEntities[index]->playAnim(24100);
	}else{
		m_pEntities[index]->playAnim(30100);
	}

	m_pEntities[index]->update(0);

	m_pEntities[index]->attachToScene(m_pScene);

	m_pEntities[index]->setPostSceneRenderer(GameManager::getSingleton().GetMainSceneRenderer());
}

void GameBackground::setPetModel( int index, int nAnimId, const char* szModelPath, const char *szAppendPath, int nResId )
{
	if (m_pEntities[index]!=NULL)
	{
		m_pEntities[index]->detachFromScene();
		OGRE_RELEASE(m_pEntities[index]);
	}
	if (m_pAppends[index]!=NULL)
	{
		m_pAppends[index]->detachFromScene();
		OGRE_RELEASE(m_pAppends[index]);
	}
	m_pEntities[index] = new AvatarEntity;	
	m_pEntities[index]->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f, 0.1f));
	m_pEntities[index]->load(szModelPath,true);
	m_pEntities[index]->getMainModel();
	m_pEntities[index]->playAnim(nAnimId);
	m_pEntities[index]->update(0);
	m_pEntities[index]->attachToScene(m_pScene);
	m_pEntities[index]->setPostSceneRenderer(GameManager::getSingleton().GetMainSceneRenderer());
	if ( index < 9 )
	{
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(nResId);
		if ( &info != NULL && info.visuals[0] != '\0' )
		{
			m_pEntities[index]->playMotion( info.visuals, false, 800 );
		}
	}
	
	if (szAppendPath != NULL && szAppendPath[0] != '\0')
	{
		m_pAppends[index] = new Entity;
		m_pAppends[index]->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f));
		m_pAppends[index]->load(szAppendPath,true);
		m_pAppends[index]->update(0);
		m_pAppends[index]->attachToScene(m_pScene);
	}
}

void GameBackground::setEntityScale(int index, float x, float y, float z )
{
	assert( m_pEntities[index] != NULL );
	if( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->setScale(Vector3(x,y,z));
	}
	if( m_pAppends[index] != NULL )
	{
		m_pAppends[index]->setScale(Vector3(x,y,z));
	}
}

void GameBackground::setEntityPosition(int index, float x, float y, float z)
{
	assert( m_pEntities[index] != NULL );
	if( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->setPosition(Vector3(x,y,z));
	}
	if( m_pAppends[index] != NULL )
	{
		m_pAppends[index]->setPosition(Vector3(x,y,z));
	}
}

void GameBackground::setEntityRotate(int index, float angle)
{
	assert( m_pEntities[index] != NULL );
	if( m_pEntities[index] != NULL )
	{
		m_pEntities[index]->setRotation(angle, 0, 0);
	}
	if( m_pAppends[index] != NULL )
	{
		m_pAppends[index]->setRotation(angle, 0, 0);
	}
}

void GameBackground::playEntityAnim(int index, int seq, const char *motion)
 {
	int num = motion==NULL?0:1;
	if( m_pEntities[index != NULL] )
	{
		m_pEntities[index]->playMotion(seq, &motion, num);
	}
}

void GameBackground::setCameraAnim(const char *path)
{
	Resource *pres = ResourceManager::getSingleton().blockLoad(path);
	if(pres==NULL || !IS_KIND_OF(ModelData, pres))
	{
		return;
	}

	m_pCamAnimMod  = new Model(static_cast<ModelData *>(pres));
	pres->release();
}

void GameBackground::playCameraAnim(int seq, int fadetick)
{
	float dtime = TickToTime(fadetick);
	m_pCamAnimMod->getAnimPlayer()->setAnimDelayInOut(dtime, dtime);
	m_pCamAnimMod->playAnim(seq);
}

void GameBackground::updateCameraAnim(unsigned int dtick)
{
	if(m_pCamAnimMod == NULL)
	{
		return;
	}
	updatePullScale();
	
	//
	// Y调大人物向下移动显示，Z调大人物靠近显示
	//static Ogre::Vector3 saveVec( 1.01* 1920.730347f, 1.39 * 209.999878f, 1.13 * 2690.532715f );
	m_pCamAnimMod->update(dtick);

	int boneid = m_pCamAnimMod->getSkeletonInstance()->findBoneID("Camera01");
	if(boneid >= 0)
	{
		Matrix4 tm = m_pCamAnimMod->getSkeletonInstance()->getBoneTM(boneid);
		tm *= m_pCamAnimMod->getWorldMatrix();

		Vector3 dir(0, -1.0, 0), up(0, 0, 1.0f);
		tm.transformNormal(dir, dir);
		tm.transformNormal(up, up);
		const Ogre::Vector3& curVec = tm.getTranslate();
		
		m_pCamera->setLookDirect( Ogre::Vector3(	
			curVec.x * m_pullInfo.m_fXCurScale, 
			curVec.y * m_pullInfo.m_fYCurScale, 
			curVec.z * m_pullInfo.m_fZCurScale ), 
			dir, up );
	}
}

void GameBackground::show()
{
	loadres();

	SceneRenderer *prenderer = GameManager::getSingleton().GetMainSceneRenderer();
	prenderer->setRenderScene(m_pScene);
	prenderer->setCamera(m_pCamera);
	m_bShow = true;	
}

void GameBackground::hide()
{
	if(!m_bShow) return;

	SceneRenderer *prenderer = GameManager::getSingleton().GetMainSceneRenderer();
	GameManager::getSingleton().GetMainSceneRenderer()->setRenderScene(NULL);
	prenderer->setCamera(GamePlayManager::getSingleton().getMainCamera()->GetCameraNode());

	unloadres();
	m_bShow = false;
}

void GameBackground::fadeEntity(int index, float target_alpha, int fadetick)
{
	if( m_pEntities[index] != NULL )
	{
		if(fadetick == 0)
		{
			m_pEntities[index]->setTransparent(target_alpha);
			m_EntityTargetAlpha[index] = target_alpha;
			if (m_pAppends[index] != NULL)
			{
				m_pAppends[index]->setTransparent(target_alpha);
			}
			return;
		}
		float curalpha = m_pEntities[index]->getLocalTransparent();

		m_EntityTargetAlpha[index] = target_alpha;
		m_EntityStepAlpha[index] = (target_alpha-curalpha)/float(fadetick);
	}
}

void GameBackground::releaseEntity( int index )
{
	if(m_pEntities[index] != NULL)
	{
		m_pEntities[index]->detachFromScene();
		OGRE_RELEASE(m_pEntities[index]);
	}
	if(m_pAppends[index] != NULL)
	{
		m_pAppends[index]->detachFromScene();
		OGRE_RELEASE(m_pAppends[index]);
	}
}

void GameBackground::preLoadRes(const char *path)
{
	Resource *pres = ResourceManager::getSingleton().blockLoad(path);
	OGRE_RELEASE(pres);
}

void GameBackground::update(unsigned int dtick, int x, int y, int width, int height)
{
	if (!m_bShow)
	{
		return;
	}

	if(dtick > 100) dtick = 100;

	size_t total, count = 0;
	Vector3 center(0,0,0);
	if ( m_nViewFlag != VIEW_POINT_LOGIN )
	{
		total = m_nViewFlag != VIEW_POINT_SELECT ? ARRAY_ELEMENTS(m_pEntities):1;
		for(size_t i=0; i<total; i++)
		{
			Ogre::Entity *pentity = m_pEntities[i];
			if(pentity)
			{
				float curalpha = pentity->getLocalTransparent();
				float target = m_EntityTargetAlpha[i];
				if(curalpha != target)
				{
					float step = float(m_EntityStepAlpha[i]*dtick);
					curalpha += step;
					if(step<0 && curalpha<target) curalpha = target;
					else if(step>0 && curalpha>target) curalpha = target;

					pentity->setTransparent(curalpha);
					if (m_pAppends[i] != NULL)
					{
						m_pAppends[i]->setTransparent(curalpha);
					}
				}
				m_pEntities[i]->update(dtick);
				if (m_pAppends[i] != NULL)
				{
					m_pAppends[i]->update(dtick);
				}
				center += m_pEntities[i]->getWorldPosition();
				count++;
			}
		}
		if(count > 0) center *= float(1.0f/count);
		Ogre::Shadowmap::getSingletonPtr()->m_fAccuracy = 1.0f;
	}else{
		//登录界面影子处理
		Ogre::Shadowmap::getSingletonPtr()->m_fAccuracy = 4.0f;
		center += Vector3(6540.0f,1500.f,-100.0f);
	}
	
	updateCameraAnim(dtick);

	m_pCamera->setRatio(float(width)/height);
	m_pCamera->setViewport((float)x, (float)y, (float)width, (float)height);
	m_pCamera->update(0);

	
	if(m_pGameMap) m_pGameMap->update(dtick, m_pCamera, center);
	else m_pScene->setViewpoint(Vector3(-388.0f, 214.0f, 463.0f));
}

void GameBackground::setModelBorder(int index)
{
	assert(index < 16);
	if (index == -1 )
	{
		if(BorderGameScene::getSingletonPtr())
			BorderGameScene::getSingleton().clear();
		return;
	}
	if(BorderGameScene::getSingletonPtr())
	{
		BorderGameScene::getSingleton().setBorderStrength(1.2f);
		BorderGameScene::getSingleton().onAttachObject(m_pEntities[index]);
	}
}

tagEntityWinPos* GameBackground::getEntityWinPos( int nIndex )
{
	assert( nIndex < 16 );
	if( m_pEntities[nIndex] != NULL )
	{
		Ogre::Vector3 pos = m_pEntities[nIndex]->getWorldPosition();
		Ogre::Vector3 scrpos;
		pos = m_pEntities[nIndex]->getAnchorWorldPos(106);
		m_pCamera->pointWorldToWindow( scrpos, pos );
		m_winPos.m_nPosX = (int)scrpos.x;
		m_winPos.m_nPosY = (int)scrpos.y;
		return &m_winPos;
	}
	return NULL;
}

void GameBackground::stopEntityAnim(int nIndex)
{
	if( m_pEntities[nIndex] != NULL )
	{
		m_pEntities[nIndex]->stopAnim();
	}
}

void GameBackground::ChangeAnimModelConfig( int nGender, int nCareer )
{
	m_animCameraConfigKey.m_nGender = nGender;
	m_animCameraConfigKey.m_nCareer	= nCareer;

	if ( nGender == 0 && nCareer == 0 )
	{
		beginPull( PULL_ROLLBACK, PULL_SPEED_MODE_IMMEDIATE );
	}
	else
	{
		beginPull( PULL_OUT, PULL_SPEED_MODE_IMMEDIATE );
	}
}

bool GameBackground::isPullingCamera()
{
	return m_pullInfo.m_nPullType != PULL_NONE;
}

void GameBackground::beginPull( int nPullType, int nSpeedMode )
{
	if ( nPullType == m_pullInfo.m_nPullType )
	{
		return;
	}

	m_pullInfo.m_uLastPullTime	= timeGetTime();
	m_pullInfo.m_nPullType		= nPullType;

	if ( nSpeedMode == PULL_SPEED_MODE_SLOW )
	{
		m_uMaxPullTime = s_uConfigMaxPullTime;
	}
	else
	{
		m_uMaxPullTime = s_uConfiImmediateTime;
	}

	int nGender = m_animCameraConfigKey.m_nGender;
	int nCareer	= m_animCameraConfigKey.m_nCareer;
	if ( nGender > MAX_GENDER || nGender < 0 )
	{
		nGender = 0;
	}

	if ( nCareer > MAX_CAREER || nCareer < 0 )
	{
		nCareer = 0;
	}

	const tagDefaultPosConfig& noneDefaultPosConfig = s_backGroundNearAnimConfig[0][0];
	float fXDestScale = 0;
	float fYDestScale = 0;
	float fZDestScale = 0;
	if ( m_pullInfo.m_nPullType == PULL_ROLLBACK )
	{
		fXDestScale = noneDefaultPosConfig.m_fX;
		fYDestScale = noneDefaultPosConfig.m_fY;
		fZDestScale = noneDefaultPosConfig.m_fZ;
	}
	else if ( nPullType == PULL_IN )
	{
		const tagDefaultPosConfig& oneDefaultNearPosConfig = s_backGroundNearAnimConfig[nGender][nCareer];
		fXDestScale = oneDefaultNearPosConfig.m_fX;
		fYDestScale = oneDefaultNearPosConfig.m_fY;
		fZDestScale = oneDefaultNearPosConfig.m_fZ;
	}
	else if ( nPullType == PULL_OUT )
	{
		const tagDefaultPosConfig& oneDefaultFarPosConfig = s_backGroundFarAnimConfig[nGender][nCareer];
		fXDestScale = oneDefaultFarPosConfig.m_fX;
		fYDestScale = oneDefaultFarPosConfig.m_fY;
		fZDestScale = oneDefaultFarPosConfig.m_fZ;
	}

	m_pullInfo.m_fXDestScale	= fXDestScale;
	m_pullInfo.m_fYDestScale	= fYDestScale;
	m_pullInfo.m_fZDestScale	= fZDestScale;

	m_pullInfo.m_fXChgScale	= ( fXDestScale - m_pullInfo.m_fXCurScale ) / m_uMaxPullTime;
	m_pullInfo.m_fYChgScale	= ( fYDestScale - m_pullInfo.m_fYCurScale ) / m_uMaxPullTime;
	m_pullInfo.m_fZChgScale	= ( fZDestScale - m_pullInfo.m_fZCurScale ) / m_uMaxPullTime;
}

void GameBackground::updatePullScale()
{
	if ( m_pullInfo.m_nPullType == PULL_NONE )
	{
		return;
	}
	
	
	if ( m_pullInfo.m_fXCurScale == m_pullInfo.m_fXDestScale && m_pullInfo.m_fYCurScale == m_pullInfo.m_fYDestScale &&
		m_pullInfo.m_fZCurScale == m_pullInfo.m_fZDestScale )
	{
		m_pullInfo.m_nPullType		= PULL_NONE;
		m_pullInfo.m_uLastPullTime	= 0;
		return;
	}
	
	unsigned int uCurTime = timeGetTime();
	unsigned int uChgTime = uCurTime - m_pullInfo.m_uLastPullTime;
	m_pullInfo.m_fXCurScale += ( m_pullInfo.m_fXChgScale * uChgTime );
	roudLimit( m_pullInfo.m_fXCurScale, m_pullInfo.m_fXChgScale, m_pullInfo.m_fXDestScale );

	m_pullInfo.m_fYCurScale += ( m_pullInfo.m_fYChgScale * uChgTime );
	roudLimit( m_pullInfo.m_fYCurScale, m_pullInfo.m_fYChgScale, m_pullInfo.m_fYDestScale );

	m_pullInfo.m_fZCurScale += ( m_pullInfo.m_fZChgScale * uChgTime );
	roudLimit( m_pullInfo.m_fZCurScale, m_pullInfo.m_fZChgScale, m_pullInfo.m_fZDestScale );

	m_pullInfo.m_uLastPullTime = uCurTime;
}

void GameBackground::roudLimit( float& fCurValue, float& fChgValue, float& fDestValue )
{
	if ( fChgValue < 0 )
	{
		if ( fCurValue < fDestValue )
		{
			fCurValue = fDestValue;
		}
		return;
	}

	if ( fChgValue > 0 )
	{
		if ( fCurValue > fDestValue )
		{
			fCurValue = fDestValue;
		}
		return;
	}
}

void GameBackground::setViewFlag( int nFlag )
{
	assert( nFlag < VIEW_POINT_CREATE + 1 );
	m_nViewFlag = nFlag;
}

void GameBackground::playBGM( int nTime )
{
	SoundSystem::getSingleton().playMusic( 0, "sounds\\sounds_2\\music01-160kbps.wma", true, nTime, 1.0 );
}