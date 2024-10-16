
#ifndef __EffectObject_H__
#define __EffectObject_H__

#include "OgreMovableObject.h"

namespace Ogre
{
	class _OgreExport EffectObject : public MovableObject
	{
		DECLARE_RTTI_VIRTUAL(EffectObject)

	public:
		EffectObject() : m_isGlobal(false)
		{
		}

		virtual void attachToScene(GameScene *pscene,bool hasfather = false);

		virtual void prepare(SceneRenderer* pRenderer, unsigned int framecount ) = 0;
		virtual void queryShaderEnv(ShaderEnvData &envdata, const Matrix4 &obj_worldtm) = 0;
		virtual float getEffectWeight(const Vector3 &point, float radius=0.0f) = 0;

		void setGlobal(bool b)
		{
			m_isGlobal = b;
		}

		bool isGlobal()
		{
			return m_isGlobal;
		}

	protected:
		bool m_isGlobal;
	};
}

#endif