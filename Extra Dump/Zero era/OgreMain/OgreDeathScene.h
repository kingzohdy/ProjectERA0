// DeathScene

#ifndef __OgreDeathScene__H__
#define __OgreDeathScene__H__

#include "OgreGameScene.h" 
#include "OgreSingleton.h"
#include "OgreCamera.h"

namespace Ogre
{

	class _OgreExport DeathSceneObject
	{
	public:
		DeathSceneObject ()
		{
			ent = NULL;
			originEntity = NULL;
			fInitAlpha = 1.0f;
			m_fLife = 0.0f;
			m_fTimeLeft = 0.0f;
			originPos = Vector3(0.0f, 0.0f, 0.0f);
		}
		~DeathSceneObject ()
		{

		}

		Ogre::Entity *ent;
		Ogre::Entity *originEntity;
		float fInitAlpha;
		float m_fLife;
		float m_fTimeLeft;
		Ogre::WorldPos originPos;
		Ogre::WorldPos currentPos;
	};

	/**
	* 需要死亡效果的Entity加入此场景
	*/
	class _OgreExport DeathGameScene : public GameScene, public Ogre::Singleton<DeathGameScene>
	{
		DECLARE_RTTI(DeathGameScene)

	public:
		DeathGameScene ();

		virtual void update(uint dtick);

		void addObject (Entity *pobject, Entity *originObject);
		void clear ();

		virtual void onAttachObject(MovableObject *pobject);
		virtual void onDetachObject(MovableObject *pobject);
		virtual void onObjectPosChange(MovableObject *pobject);
		virtual void onRender(SceneRenderer *prenderer);

		//裁剪
		virtual void onCull(Camera *pcamera);	

		virtual MovableObject *pickObject(IntersectType type, const WorldRay &ray, float *collide_dist, uint pickflags);
		virtual bool pickGround(const WorldRay &ray, float *collide_dist);
		virtual bool pickGround(WPOS_T x, WPOS_T z, WPOS_T *y, Vector3 *pnormal=NULL, float *water_height=NULL);

		virtual void updateFocusArea(const WorldPos center, float radius);
		virtual TerrainTile * getTerrainTile() { return NULL;}

	protected:
		~DeathGameScene ();

		void DeleteObjectOnRenderObjectList (RenderableObject *object);

		std::vector<RenderableObject*> m_RenderObjects;
		std::vector<DeathSceneObject*> m_DeathObjects;

		Material *m_pDeathMaterial;
		float m_fOgrinUpSpeed;
		float m_fUpSpeed;
		float m_fLife;
		float m_fOriginMoveStartTime;
		float m_fMoveStartTime;
		Vector3 m_MoveDir;
		Ogre::TextureData *m_pRanderTexture; 
	};

}

#endif