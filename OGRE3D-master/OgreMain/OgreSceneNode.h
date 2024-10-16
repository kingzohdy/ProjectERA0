
#ifndef __SceneNode_H__
#define __SceneNode_H__

/*
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreWorldPos.h"
#include "OgreMatrix4.h"
#include "OgreBaseObject.h"
#include "OgreBounding.h"
#include <vector>

namespace Ogre
{
	class _OgreExport SceneNode : public BaseObject
	{
		DECLARE_RTTI(SceneNode)
	public:
		SceneNode();

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);

		void setPosition(const WorldPos &pos);
		void setScale(const Vector3 &scale);
		void setRotation(const Quaternion &rot);
		void setRotation(float yaw, float pitch, float roll);//yaw 绕Y轴旋转，  pitch绕 X轴， roll绕 Z轴, 以度为单位
		WorldPos getPosition() const;
		Vector3 getScale() const;
		Quaternion getRotation() const;

		const Matrix4 &getWorldMatrix();

		const Vector3 getWorldPosition()
		{
			return getWorldMatrix().getTranslate();
		}

		const BoxSphereBound &getWorldBounds()
		{
			return m_WorldBounds;
		}

		void show(bool b){m_bShow = b;}
		bool isShow(){return m_bShow;}

		void *getContainer()
		{
			return m_pSceneContainer;
		}

		void setContainer(void *p)
		{
			m_pSceneContainer = p;
		}

	protected:
		WorldPos m_Position;
		Quaternion m_Rotation;
		Vector3 m_Scale;

		Matrix4 m_WorldTM;
		BoxSphereBound m_WorldBounds;

		uint m_LastFrameTick;
		uint m_CurFrameTick;
		bool m_bPause;
		bool m_bShow;

		void *m_pSceneContainer;
	};

	class _OgreExport GroupNode : public SceneNode
	{
		DECLARE_RTTI(GroupNode)

	public:
		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);

		void addChild(SceneNode *pnode);
		void removeChild(SceneNode *pnode);

	private:
		std::vector<SceneNode *>m_ChildNodes;
	};

	typedef SceneNode MovableObject;
}
*/

#endif