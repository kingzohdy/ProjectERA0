
#ifndef __BorderRender_H__
#define __BorderRender_H__

#include "OgreSingleton.h"
#include "OgreRenderTypes.h"
#include "OgreUIRenderTypes.h"
#include "OgreSceneRenderer.h"
#include <vector>

namespace Ogre
{
	class Entity;

	// 在包边渲染器中添加的实体，会产生包边效果。包边渲染器介于NormalRender和
	// UIRender之间。渲染之前需要清空ZBuffer。
	class _OgreExport BorderRender : public Singleton<BorderRender>, public SceneRenderer
	{
	public:
		BorderRender();
		virtual ~BorderRender();

		virtual void doRender();

		void AddEntity (Entity *pEntity);
		void RemoveEntity (Entity *pEntity);
		void Clear ();

	private:
		std::vector<Entity*> m_Entities;

		Material *m_pBorderMaterial;
		Material *m_pBorderMaterial1;
	};
}

#endif