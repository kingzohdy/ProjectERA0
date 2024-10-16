// BorderScene

#ifndef __OgreBorderScene__H__
#define __OgreBorderScene__H__

#include "OgreGameScene.h" 
#include "OgreSingleton.h"

namespace Ogre
{

	/**
	* 加入到BorderGameScene的对象会有包边效果
	*/
	class _OgreExport BorderGameScene : public GameScene, public Ogre::Singleton<BorderGameScene>
	{
		DECLARE_RTTI(BorderGameScene)

	public:
		BorderGameScene();

		void setActiveBorderMaterial ();
		void setActiveBorderMaterial1 ();
		void setNoBorder ();

		void setBorderColor (Vector3 color);
		void setBorderStrength (float fStrength);
		float getBorderStrength ();

		virtual void update(uint dtick);

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
		~BorderGameScene ();

		std::vector<RenderableObject*> m_RenderObjects;

		Material *m_pBorderMaterial;
		Material *m_pBorderMaterial1;

		Vector3 m_Color;

		float m_fLength; // 相机和中心点之间的距离，包边宽度会随着距离变化

		float m_fStrength;
		float m_fTime;
	};

}

#endif