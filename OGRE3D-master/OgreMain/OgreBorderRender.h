
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

	// �ڰ�����Ⱦ������ӵ�ʵ�壬���������Ч����������Ⱦ������NormalRender��
	// UIRender֮�䡣��Ⱦ֮ǰ��Ҫ���ZBuffer��
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