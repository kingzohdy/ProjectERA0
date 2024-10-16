#include "OgreEntity.h"
#include "OgreResourceManager.h"
#include "OgreUIRenderer.h"
#include "OgreCamera.h"
#include "OgreRay.h"
#include "OgreEntityMotionEvent.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameDBData.h"
#include "AvatarEntity.h"
#include "ShowGameMap.h"
#include "OgreRoot.h"
#include "OgreGameScene.h"
#include <Windows.h>
#include <MMSystem.h>
#include "OgreLog.h"
#include "GameActorManager.h"

namespace Ogre
{
	class FootprintHandler : public MotionEventHandler
	{
	public:
		FootprintHandler( GameActor* pActor ) : m_pActor(pActor) ,m_LeftFoot("Foot_L"), m_RightFoot("Foot_R")
		{
		}

		virtual int	OnEvent(MotionEventParam* pParam);

	public:
		FixedString m_LeftFoot;
		FixedString m_RightFoot;
		ShowGameMap *m_pMap;
		GameActor* m_pActor;
	};

	int	FootprintHandler::OnEvent(MotionEventParam* pParam)
	{
		Matrix4 tm = pParam->m_pModel->getWorldMatrix();
		WorldPos pos = tm.getTranslate();
		Vector3 dir(0, 0, -1.0f), binormal(1.0f, 0, 0);
		tm.transformNormal(dir, dir);
		tm.transformNormal(binormal, binormal);
		Normalize(dir);
		Normalize(binormal);

		if(pParam->m_strEventName == m_LeftFoot)
		{
			pos += binormal*10.0f;
		}
		else if(pParam->m_strEventName == m_RightFoot)
		{
			pos -= binormal*10.0f;
		}
		if( m_pMap != NULL && m_pActor != NULL )
		{
			m_pActor->SetFootEffect( pos, m_pActor->getWanMeiStarNum());
			if ( m_pActor->getType() == GAT_MAINPLAYER && !m_pActor->isInRide() )
			{
				m_pMap->addFootprint( pos, dir );
			}
		}
		return 0;
	}

	void ShowActor::initCommon()
	{
		RoleText::initFont();
	}

	void ShowActor::releaseCommon()
	{
		RoleText::releaseFont();
	}


	ShowActor::ShowActor(int resid, bool blockload) : m_ResID(resid), m_CurResID(resid), m_CurActionId(0), m_pCurEntity(NULL), m_pFootprintHandler(NULL),m_bSnapToMap(false),m_nFaceTo(0)
	{
		m_pRepEntity = NULL;
		memset(m_SkinResHandle, 0, sizeof(m_SkinResHandle));
		memset(m_pSkinRes, 0, sizeof(m_pSkinRes));
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
		if( info.MiscType & NPCDISPLAYINFO_MISC_TYPE_FOLLOW )
		{
			m_bSnapToMap = true;
		}
		if(info.model[0])
		{
			m_pCurEntity = new Entity;
			m_isAvatar = false;
			if (!blockload && !ResourceManager::getSingleton().isSmartResOK(info.model))
			{
				m_pRepEntity = new Entity;
				m_pRepEntity->load("effect\\entity\\guaiwuzairu.ent", true);
			}
			m_pCurEntity->load(info.model, blockload);
			std::string prefix(info.model);
			{
				int pos = prefix.rfind('\\');
				if (pos == prefix.npos)
				{
					int pos = prefix.rfind('/');
				}
				if (pos != prefix.npos)
				{
					prefix = prefix.substr(0, pos + 1);
				}
				else
				{
					prefix = "";
				}
			}

			if (blockload)
			{
				Resource *pres = NULL;

				if (info.skin1[0])
				{
					pres = ResourceManager::getSingleton().blockLoad((prefix + info.skin1).c_str());
					if (pres)
					{
						if(IS_KIND_OF(Texture, pres) && m_pCurEntity->getMainModel())
						{
							m_pCurEntity->getMainModel()->setTextureByID(1, static_cast<Texture *>(pres));
							pres->release();
						}
					}
				}
				if (info.skin2[0])
				{
					pres = ResourceManager::getSingleton().blockLoad((prefix + info.skin2).c_str());
					if (pres)
					{
						if(IS_KIND_OF(Texture, pres) && m_pCurEntity->getMainModel())
						{
							m_pCurEntity->getMainModel()->setTextureByID(2, static_cast<Texture *>(pres));
							pres->release();
						}
					}
				}
				if (info.skin3[0])
				{
					pres = ResourceManager::getSingleton().blockLoad((prefix + info.skin3).c_str());
					if (pres)
					{
						if(IS_KIND_OF(Texture, pres) && m_pCurEntity->getMainModel())
						{
							m_pCurEntity->getMainModel()->setTextureByID(3, static_cast<Texture *>(pres));
							pres->release();
						}
					}
				}
			}
			else
			{
				if (info.skin1[0])
				{
					m_SkinResHandle[0] = backgroundLoad((prefix + info.skin1).c_str(),
						ResourceManager::DP_MONSTER_NPC);
				}
				if (info.skin2[0])
				{
					m_SkinResHandle[1] = backgroundLoad((prefix + info.skin2).c_str(),
						ResourceManager::DP_MONSTER_NPC);
				}
				if (info.skin3[0])
				{
					m_SkinResHandle[2] = backgroundLoad((prefix + info.skin3).c_str(),
						ResourceManager::DP_MONSTER_NPC);
				}
			}

			for (size_t i = 0; i < 24; ++i)
			{
				if (info.userdata[i] != 0)
				{
					hideMeshInstance(info.userdata[i]);
				}
			}
		}
		else
		{
			AvatarEntity *pavatar = new AvatarEntity;
			m_isAvatar = true;
			int attrib[MAX_AVATAR_SLOT];
			memset(attrib, 0, sizeof(attrib));
			attrib[0] = info.race;
			attrib[1] = info.gender;
			memcpy(&attrib[2], info.userdata, 24*sizeof(int));
			//pavatar->updateAttrib(attrib);
			pavatar->setAvatarAttrib(0, info.race);
			pavatar->setAvatarAttrib(1, info.gender);
			for (int i = 0; i < 24; ++i)
			{
				pavatar->setAvatarAttrib(i + 2, info.userdata[i]);
			}

			m_pCurEntity = pavatar;
		}
		m_lstEntity.push_back(std::pair<int, Entity*>(resid, m_pCurEntity));
		m_pCurEntity->setScale(Vector3(info.scale, info.scale, info.scale));
		m_pCurEntity->setTransparent(info.alpha);
		m_pCurEntity->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f, 0));
		m_pCurEntity->setUserType(1);

		if (m_pRepEntity)
		{
			m_pRepEntity->setUserType(0);
		}
		
		if( info.visuals[0] )
		{
			m_pCurEntity->playMotion( info.visuals, false, 800 );
		}

		if (info.socket[0] != '\0')
		{
			m_pAppendEntity = new Entity;
			m_pAppendEntity->load(info.socket, blockload);
			m_pAppendEntity->setScale(Vector3(info.scale, info.scale, info.scale));
		}
		else
		{
			m_pAppendEntity = NULL;
		}

		m_ActorHeight	= info.height;
		m_ActorRadius	= info.radius; 
		m_ActorScale	= info.scale;
		m_ActorXparent	= info.alpha;
		m_offsetZ		= info.ZHigh;

		if( m_pCurEntity->getMainModel() != NULL && info.model[0] )
		{
			if( !m_pCurEntity->getMainModel()->checkBonesSize() )
			{
				LOG_WARNING( "%s的骨骼数量过多", info.model );
			}
		}
	}

	ShowActor::~ShowActor()
	{
		if(m_pFootprintHandler) m_pFootprintHandler->m_pMap = NULL;

		for (size_t i = 0; i < SKINS_MAX; ++i)
		{
			OGRE_RELEASE(m_pSkinRes[i]);
		}

		for (int i = 0; i < SKINS_MAX; ++i)
		{
			if (m_SkinResHandle[i] != NULL)
			{
				breakLoad(m_SkinResHandle[i]);
			}
		}

		for (std::list<std::pair<int, Entity*>>::iterator i = m_lstEntity.begin();
			i != m_lstEntity.end(); ++i)
		{
			setFootprint(i->second, NULL, false);
			OGRE_RELEASE(i->second);
		}
		OGRE_RELEASE(m_pRepEntity);
		OGRE_RELEASE(m_pAppendEntity);
		delete m_pFootprintHandler;
		m_pFootprintHandler = NULL;
	}

	void ShowActor::update(Ogre::uint dtick)
	{
		if (m_pRepEntity != NULL)
		{
			if (!m_pCurEntity->isLoading())
			{
				m_pRepEntity->detachFromScene();
				OGRE_RELEASE(m_pRepEntity);
			}
			else
			{
				m_pRepEntity->update(dtick);
			}
		}
		m_pCurEntity->update(dtick);
		if (m_pAppendEntity != NULL)
		{
			m_pAppendEntity->update(dtick);
		}

		if (m_pCurEntity->getMainModel())
		{
			for (size_t i = 0; i < SKINS_MAX; ++i)
			{
				if (m_pSkinRes[i])
				{
					if(IS_KIND_OF(Texture, m_pSkinRes[i]))
					{
						m_pCurEntity->getMainModel()->setTextureByID(i + 1, static_cast<Texture *>(m_pSkinRes[i]));
						m_pSkinRes[i]->release();
						m_pSkinRes[i] = NULL;
					}
				}
			}
		}

		if (m_pCurEntity->getMainModel() && !m_lstHideMeshs.empty())
		{
			char name[256];
			for (std::list<int>::iterator i = m_lstHideMeshs.begin();
				i != m_lstHideMeshs.end(); ++i)
			{
				sprintf(name, "skin_%d", *i);
				m_pCurEntity->getMainModel()->showSkin(name, false);
			}
			m_lstHideMeshs.clear();
		}
		if( m_bSnapToMap )
		{
			snapToMapPoint( m_pCurEntity->getPosition().x,m_pCurEntity->getPosition().z, m_nFaceTo );
		}
	}

	void ShowActor::playAction(const char *name)
	{
		m_CurAction = name;
		m_pCurEntity->playMotion(m_CurAction);
	}
	
	void ShowActor::playAction( int seqid, const char **pmotions, int num_motion, int motion_class )
	{
		m_pCurEntity->playMotion( seqid, pmotions, num_motion, motion_class );
		m_CurActionId = seqid;

		int ani				= seqid%1000 + 10000;
		const int nonani	= 10100;
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_HAIR, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_HAIR, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_CLOSES, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_CLOSES, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_LEFTHAND, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_LEFTHAND, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_RIGHTHAND, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_RIGHTHAND, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_HAT, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_HAT, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_SHOULD, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_SHOULD, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_BACK, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_BACK, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_FASHION_BODY, ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_FASHION_BODY, nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_FASHION_HEAD,ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_FASHION_HEAD,nonani );
		if( !m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_FASHION_BACK,ani ) )
			m_pCurEntity->playBindAnim( AVATAR_BIND_CLASS+POS_ID_FASHION_BACK,nonani );
	}
	
	bool ShowActor::hasAnim(int seq)
	{
		if( m_pCurEntity->getMainModel() )
		{
			return m_pCurEntity->getMainModel()->hasAnim( seq );
		}
		return false;
	}

	void ShowActor::stopCurAction()
	{
		if( m_CurActionId != 0 )
		{
			m_pCurEntity->stopAnim(m_CurActionId);
			m_CurActionId = 0;
		}else{
			m_pCurEntity->stopMotion(m_CurAction);
		}	
	}

	void ShowActor::setPosition(int x, int y, int z)
	{
		WorldPos pos = WorldPos(x, y, z);
		if(m_pCurEntity->getFather() == NULL) //没有绑定
		{
			m_pCurEntity->setPosition(pos);
		}
		if (m_pRepEntity != NULL)
		{
			m_pRepEntity->setPosition(pos);
		}
		if (m_pAppendEntity != NULL)
		{
			m_pAppendEntity->setPosition(pos);
		}
	}

	void ShowActor::setFaceTo(int angle)
	{
		if(m_pCurEntity->getFather() == NULL)
		{
			m_pCurEntity->setRotation(angle-90.0f, 0, 0);
		}
		m_nFaceTo = angle;
	}

	void ShowActor::snapToMapPoint( int x, int z, int faceangle)
	{
		Vector3 normal;
		WPOS_T y = 0;
		GameScene* pscene = m_pCurEntity->getScene();
		if( pscene == NULL || !pscene->pickGround(x, z, &y, &normal, NULL) )
		{
			return;
		}

		if (normal.y < 0.0f)
		{
			normal.x = -normal.x;
			normal.y = -normal.y;
			normal.z = -normal.z;
		}

		m_pCurEntity->setPosition(WorldPos(x, y, z));

		if (faceangle % 90 == 0)
		{
			if (faceangle % 180 == 0)
			{
				normal.z = 0;
			}
			else
			{
				normal.x = 0;
			}
		}
		else
		{
			normal.x = (normal.x * Ogre::Tan(90.0f - faceangle) - normal.z) / (Ogre::Tan(90.0f - faceangle) - Ogre::Tan(float(-faceangle)));
			normal.z = normal.x * Ogre::Tan(float(-faceangle));
		}

		Ogre::Quaternion q1, q2;
		Normalize(normal);
		q1.setRotateArc(Vector3(0, 1.0f, 0), normal);
		q2.setEulerAngle(faceangle-90.0f, 0, 0);

		m_pCurEntity->setRotation(q2*q1);
	}

	void ShowActor::bindActor(int anchorid, ShowActor *pactor)
	{
		Entity *pentity = pactor->getCurEntity();
		pentity->setPosition(WorldPos(0,0,0));
		pentity->setRotation(0, 0, 0);

		m_pCurEntity->bindObject(anchorid, pactor->getCurEntity());
	}

	void ShowActor::unbindActor(ShowActor *pactor)
	{	
		m_pCurEntity->unbindObject(pactor->getCurEntity());
	}
	
	bool ShowActor::isInMap()
	{
		return m_pCurEntity->getScene() != NULL;
	}

	void ShowActor::onEnterMap(ShowGameMap *pmap)
	{
		if(m_pFootprintHandler) m_pFootprintHandler->m_pMap = pmap;
		m_pCurEntity->attachToScene(pmap->getGameScene());
		if (m_pRepEntity != NULL)
		{
			m_pRepEntity->attachToScene(pmap->getGameScene());
		}
		if (m_pAppendEntity != NULL)
		{
			m_pAppendEntity->attachToScene(pmap->getGameScene());
		}
	}

	void ShowActor::onLeaveMap(ShowGameMap *pmap)
	{
		if(m_pFootprintHandler && m_pFootprintHandler->m_pMap) m_pFootprintHandler->m_pMap = NULL;
		m_pCurEntity->detachFromScene();
		if (m_pRepEntity != NULL)
		{
			m_pRepEntity->detachFromScene();
		}
		if (m_pAppendEntity != NULL)
		{
			m_pAppendEntity->detachFromScene();
		}
	}

	void ShowActor::onMorph( int resId, ShowGameMap *pmap )
	{
		assert(m_pCurEntity->getRefCount() >= 2);
		const tagNpcDisplayInfo &oldinfo = DBDataManager::getSingleton().GetNpcDisplayInfo(m_CurResID);
		Vector3	scale = m_pCurEntity->getScale();
		float alpha = m_pCurEntity->getTransparent();
		m_pCurEntity->detachFromScene();

		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resId);
		if( info.model[0] )
		{
			m_pCurEntity = new Entity;
			m_pCurEntity->setPostSceneRenderer( ActorManager::getSingleton().getPostSceneRenderer() );
			m_pCurEntity->load( info.model, false );
			
		}else{
			AvatarEntity *pavatar = new AvatarEntity;
			int attrib[MAX_AVATAR_SLOT];
			memset(attrib, 0, sizeof(attrib));
			attrib[0] = info.race;
			attrib[1] = info.gender;
			memcpy(&attrib[2], info.userdata, 24*sizeof(int));
			pavatar->updateAttrib(attrib);

			m_pCurEntity = pavatar;
			m_pCurEntity->setPostSceneRenderer( ActorManager::getSingleton().getPostSceneRenderer() );
		}
		m_lstEntity.push_back(std::pair<int, Entity*>(resId, m_pCurEntity));
		if( info.visuals[0] )
		{
			m_pCurEntity->playMotion( info.visuals, false, 800 );
		}
		m_pCurEntity->setScale(info.scale / oldinfo.scale * scale);
		m_ActorHeight = info.height;
		m_offsetZ = info.ZHigh;
		if (oldinfo.alpha > 0)
		{
			m_pCurEntity->setTransparent(info.alpha / oldinfo.alpha * alpha);
		}
		else
		{
			m_pCurEntity->setTransparent(info.alpha);
		}
		m_pCurEntity->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f, 0));
		if (pmap != NULL)
		{
			m_pCurEntity->attachToScene( pmap->getGameScene() );
		}
		m_CurResID = resId;
	}

	int ShowActor::onRestore( int resId, ShowGameMap *pmap )
	{
		for (std::list<std::pair<int, Entity*>>::iterator i = m_lstEntity.begin();
			i != m_lstEntity.end(); ++i)
		{
			if (i->first == resId)
			{
				Entity *pEntity = i->second;
				m_lstEntity.erase(i);
				if (m_pCurEntity == pEntity)
				{
					Vector3	scale = m_pCurEntity->getScale();
					float alpha = m_pCurEntity->getTransparent();
					WorldPos pos = m_pCurEntity->getPosition();
					m_pCurEntity->detachFromScene();
					m_pCurEntity = m_lstEntity.back().second;
					if (pmap != NULL)
					{
						m_pCurEntity->attachToScene(pmap->getGameScene());
					}
					int resid = m_lstEntity.back().first;
					const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
					m_ActorHeight = info.height;
					m_offsetZ = info.ZHigh;
					const tagNpcDisplayInfo &oldinfo = DBDataManager::getSingleton().GetNpcDisplayInfo(m_CurResID);
					m_pCurEntity->setScale(info.scale / oldinfo.scale * scale);
					if (oldinfo.alpha > 0)
					{
						m_pCurEntity->setTransparent(info.alpha / oldinfo.alpha * alpha);
					}
					m_pCurEntity->setPosition(pos);
					m_CurResID = resid;
				}
				pEntity->release();
				break;
			}
		}
	
		return m_lstEntity.back().first;
	}

	bool ShowActor::isMorphing()
	{
		return m_lstEntity.size() > 1;
	}

	bool ShowActor::intersectRay(const Ogre::Ray &ray)
	{
		return ray.intersectCapsule(m_pCurEntity->getWorldPosition(), Vector3(0, (float)m_ActorHeight, 0), (float)m_ActorRadius);
	}

	void ShowActor::setFootprint(Ogre::Entity *entity, GameActor* pActor, bool enable)
	{
		if (entity == NULL)
		{
			return;
		}
		if(enable)
		{
			if(m_pFootprintHandler == NULL)
			{
				m_pFootprintHandler = new FootprintHandler(pActor);
			}
			entity->registerEvent("Foot_L", m_pFootprintHandler);
			entity->registerEvent("Foot_R", m_pFootprintHandler);
		}
		else
		{
			entity->unregisterEvent("Foot_L", m_pFootprintHandler);
			entity->unregisterEvent("Foot_R", m_pFootprintHandler);
		}
	}

	void ShowActor::stopMotion(int motion_class)
	{
		m_pCurEntity->stopMotion(motion_class);
	}

	void ShowActor::stopMotion()
	{
		m_pCurEntity->stopMotion();
	}

	void ShowActor::hideMeshInstance(int idx)
	{
		if (m_pCurEntity->getMainModel() == NULL)
		{
			m_lstHideMeshs.push_back(idx);
		}
		else
		{
			char name[256];
			sprintf(name, "skin_%d", idx);
			m_pCurEntity->getMainModel()->showSkin(name, false);
		}
	}

	void ShowActor::ResourceLoaded(Resource *pres, ResourceHandle h)
	{
		for (size_t i = 0; i < SKINS_MAX; ++i)
		{
			if (m_SkinResHandle[i] == h)
			{
				m_SkinResHandle[i] = NULL;
				if (pres)
				{
					pres->addRef();
					m_pSkinRes[i] = pres;
				}
				break;
			}
		}
	}

}
