
#ifndef __MovableObject_H__
#define __MovableObject_H__

#include "OgreBaseObject.h"
#include "OgreVector3.h"
#include "OgreWorldPos.h"
#include "OgreQuaternion.h"
#include "OgreMatrix4.h"
#include "OgreBounding.h"

namespace Ogre
{
	class _OgreExport MovableObject : public BaseObject
	{
		DECLARE_RTTI_VIRTUAL(MovableObject)

	public:
		MovableObject();
		virtual ~MovableObject();

		virtual void enableUVMask (bool enable, bool stdmtlUse)
		{

		}

		virtual void setLiuGuangTexture (TextureData *texture)
		{

		}

		virtual void setLiuGuangTexture (const char *filename)
		{

		}

		// 设置强制去除shadermap
		virtual void SetForceShadowMapCull (bool cull)
		{
			m_bForceShadowMapCull = cull;
		}

		bool IsForceShadowMapCull ()
		{
			return m_bForceShadowMapCull;
		}

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff)
		{
			m_bPause = pause;
		}
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();
		virtual bool intersectRay(IntersectType type, const Ray &ray, float *collide_dist);
		virtual Matrix4 getAnchorWorldMatrix(int id)
		{
			return getWorldMatrix();
		}
		virtual void invalidWorldCache();
		virtual void updateWorldCache();

		void setSRTFather(MovableObject* pfather,int anchorid);//位置依赖的父物体
		void setPosition(const WorldPos &pos)
		{
			m_Position = pos;
			invalidWorldCache();
		}
		void setScale(const Vector3 &scale)
		{
			m_Scale = scale;
			invalidWorldCache();
		}
		void setRotation(const Quaternion &rot)
		{
			m_Rotation = rot;
			invalidWorldCache();
		}
		void setRotation(float yaw, float pitch, float roll)//yaw 绕Y轴旋转，  pitch绕 X轴， roll绕 Z轴, 以度为单位
		{
			m_Rotation.setEulerAngle(yaw, pitch, roll);
			invalidWorldCache();
		}
		const WorldPos &getPosition() const
		{
			return m_Position;
		}
		const Vector3 &getScale() const
		{
			return m_Scale;
		}
		const Quaternion &getRotation() const
		{
			return m_Rotation;
		}
		const Matrix4 &getWorldMatrix()
		{
			if(m_bWorldCacheInvalid)
			{
				updateWorldCache();
			}

			return m_WorldTM;
		}
		const Vector3 getWorldPosition()
		{
			return getWorldMatrix().getTranslate();
		}
		const BoxSphereBound &getWorldBounds()
		{
			if(m_bWorldCacheInvalid)
			{
				updateWorldCache();
			}

			return m_WorldBounds;
		}

		void show(bool b){m_bShow = b;}
		bool isShow(){return m_bShow;}

		GameScene *getScene()
		{
			return m_pScene;
		}

		void *getSceneContainer()
		{
			return m_pSceneContainer;
		}

		void setScene(GameScene *pscene)
		{
			m_pScene = pscene;
		}

		void setSceneContainer(void *p)
		{
			m_pSceneContainer = p;
		}

		void setPickParam(uint type, MovableObject *pick_owner)
		{
			m_PickType = type;
			m_pPickOwner = pick_owner;
		}

		uint getPickType()
		{
			return m_PickType;
		}

		MovableObject *getPickOwner()
		{
			return m_pPickOwner;
		}

		bool isSceneAttached()
		{
			return m_bSceneAttached;
		}

		void setFather(MovableObject* pfather)//除SRT外其他属性的父节点
		{
			m_pFather = pfather;
		}

		MovableObject *getFather()
		{
			return m_pFather;
		}

		MovableObject *getSRTFather()
		{
			return m_pSRTFather;
		}

		void setTransparent(float alpha)
		{
			m_Transparent = alpha;
		}

		float getLocalTransparent()
		{
			return m_Transparent;
		}

		float getTransparent()
		{
			if(m_pFather) return m_pFather->getTransparent()*m_Transparent;
			else return m_Transparent;
		}

		static void prepareFrame();

	protected:
		WorldPos m_Position;
		Quaternion m_Rotation;
		Vector3 m_Scale;

		Matrix4 m_WorldTM;
		BoxBound   m_BoxBoundTemp;
		BoxSphereBound m_WorldBounds;

		MovableObject*	m_pSRTFather;
		int				m_AnchorID;
		MovableObject*  m_pFather;

		bool m_bWorldCacheInvalid;
		bool m_bSceneAttachInvalid;
		bool m_bSceneAttached; //已经attach到scene里

		bool m_bShow;
		bool m_bPause;

		float m_Transparent;

		GameScene *m_pScene;
		void *m_pSceneContainer;

		uint m_PickType;//0:不接受任何pick
		MovableObject *m_pPickOwner;//NULL:pick中后返回自己， 否则返回m_pPickOwner

		bool m_bForceShadowMapCull;
	};
}

#endif