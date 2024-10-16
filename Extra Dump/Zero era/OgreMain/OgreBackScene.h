
#ifndef __OgreBackScene__
#define __OgreBackScene__

#include "OgreGameScene.h"
#include "OgreSingleton.h"

namespace Ogre
{

	class _OgreExport BackGameScene : public GameScene, public Ogre::Singleton<BackGameScene>
	{
		DECLARE_RTTI(BackGameScene)

	public:
		BackGameScene ();

		void setActiveBackMaterial0 ();
		void setActiveBackMaterial1 ();
		void setNoBack ();

		void update (uint dtick);

		void clear ();
		virtual void onAttachObject(MovableObject *pobject);
		virtual void onDetachObject(MovableObject *pobject);
		virtual void onObjectPosChange(MovableObject *pobject);
		virtual void onRender(SceneRenderer *prenderer);

		//²Ã¼ô
		virtual void onCull(Camera *pcamera);	

		virtual MovableObject *pickObject(IntersectType type, const WorldRay &ray, float *collide_dist, uint pickflags);
		virtual bool pickGround(const WorldRay &ray, float *collide_dist);
		virtual bool pickGround(WPOS_T x, WPOS_T z, WPOS_T *y, Vector3 *pnormal=NULL, float *water_height=NULL);

		virtual void updateFocusArea(const WorldPos center, float radius);
		virtual TerrainTile * getTerrainTile() { return NULL;}

	protected:
		~BackGameScene ();

		std::vector<RenderableObject*> m_RenderObjects;

		Material *m_pBackMaterial0;
		Material *m_pBackMaterial1;
	};

}

#endif