
#ifndef __OgreFog_H__
#define __OgreFog_H__

#include "OgreEffectObject.h"
#include "OgreColourValue.h"

namespace Ogre
{
	enum FOG_TYPE
	{
		FOG_DISTANCE = 0,
		FOG_HEIGHT,
	};

	class _OgreExport FogEffect : public EffectObject
	{
		DECLARE_RTTI_VIRTUAL(FogEffect)
	public:
		FogEffect(FOG_TYPE type);
		
		virtual void prepare(SceneRenderer* pRenderer, unsigned int framecount );
		virtual void queryShaderEnv(ShaderEnvData &envdata, const Matrix4 &obj_worldtm);
		virtual float getEffectWeight(const Vector3 &point, float radius=0.0f);

		virtual void _serialize(Archive &ar, int version);

		void setRange(float near, float far)
		{
			m_Near = near;
			m_Far = far;
		}
	public:
		FOG_TYPE m_Type;
		ColourValue m_Color;
		float m_Density;
		float m_Near;
		float m_Far;
	};
}

#endif