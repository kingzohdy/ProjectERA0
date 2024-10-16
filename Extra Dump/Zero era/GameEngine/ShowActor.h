
#ifndef __ShowActor_H__
#define __ShowActor_H__

#include "GameEngineTypes.h"
#include "OgreBaseObject.h"
#include "OgreFixedString.h"

#include "GameActor.h"

#include <list>

namespace Ogre
{
	class FootprintHandler;

	class ShowActor : public Ogre::BaseObject, public LoadWrap
	{
	public:
		static const size_t SKINS_MAX = 3;
	public:
		ShowActor(int resid, bool blockload=true);

		Entity *getCurEntity()
		{
			return m_pCurEntity;
		}

		Entity* getRepEntity()
		{
			return m_pRepEntity;
		}

		float getActorScale()
		{
			return m_ActorScale;
		}
		
		int getCurAction()
		{
			return m_CurActionId;
		}
		
		int getActorHeight() 
		{ 
			return m_ActorHeight; 
		}
		int getActorRadius() 
		{ 
			return m_ActorRadius; 
		}

		int getActorOffsetZ()
		{
			return m_offsetZ;
		}

		void setCanSel(bool b)
		{
			if (m_pCurEntity)
			{
				m_pCurEntity->setCanSel(b);
			}
		}

		bool canEquipItem()
		{
			return m_isAvatar;
		}

		void stopMotion();
		void stopMotion(int motion_class);
		void playAction( const char *name );
		void playAction( int seqid, const char **pmotions = NULL, int num_motion = 0, int motion_class = 0 );
		bool hasAnim(int seq);
		void stopCurAction();
		void setPosition(int x, int y, int z);
		void setFaceTo(int angle);
		void snapToMapPoint( int x, int z, int faceangle);
		void bindActor(int anchorid, ShowActor *pactor);
		void unbindActor(ShowActor *pactor);
		void update(Ogre::uint dtick);
		void hideMeshInstance(int idx);
		
		bool isInMap();
		void onEnterMap(ShowGameMap *pmap);
		void onLeaveMap(ShowGameMap *pmap);

		void onMorph( int resId, ShowGameMap *pmap );
		bool isMorphing();
		int onRestore( int resId, ShowGameMap *pmap );

		bool intersectRay(const Ogre::Ray &ray);

		void setFootprint(Ogre::Entity *entity, GameActor* pActor, bool enable);

		void setSnapToMap(bool b){m_bSnapToMap = b;}

		static void initCommon();
		static void releaseCommon();

	protected:
		~ShowActor();

	private:
		virtual void ResourceLoaded(Resource *pres, ResourceHandle h);

	protected:
		int				m_ResID;
		int				m_CurResID;
		Entity*			m_pRepEntity;
		Entity*			m_pCurEntity;
		Entity*			m_pAppendEntity;
		float			m_ActorScale;
		float			m_ActorXparent;
		FixedString		m_CurAction;
		int				m_CurActionId;
		int				m_ActorHeight;
		int				m_ActorRadius;
		bool			m_isAvatar;
		bool			m_bSnapToMap;
		int				m_nFaceTo;

		int				m_offsetZ;

		FootprintHandler *m_pFootprintHandler;

		std::list<std::pair<int, Entity*>>		m_lstEntity;

		std::list<int>							m_lstHideMeshs;
		ResourceHandle		m_SkinResHandle[SKINS_MAX];
		Resource			*m_pSkinRes[SKINS_MAX];
	};
}

#endif
