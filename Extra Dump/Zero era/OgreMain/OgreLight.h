
#ifndef __Light_H__
#define __Light_H__

#include "OgreEffectObject.h"
#include "OgreColourValue.h"
#include "OgreLightData.h"


#define LightNode Light

namespace Ogre
{
	class Shadowmap;
	class _OgreExport Light : public EffectObject
	{
		DECLARE_RTTI_VIRTUAL(Light)
	public:
		Light(LightData *plightdata);
		Light(LightType type);
		virtual ~Light();

		virtual void prepare(SceneRenderer* pRenderer, unsigned int framecount);
		virtual void queryShaderEnv(ShaderEnvData &envdata, const Matrix4 &obj_worldtm);
		virtual float getEffectWeight(const Vector3 &point, float radius=0.0f);
		virtual void updateWorldCache();

		LightType getLightType(){ return m_Type; }
		void setColor(const ColourValue &color)
		{
			m_DiffuseColor = m_AmbientColor = m_SpecularColor = color;
		}

		void setDirection(const Vector3 &dir);

		void setStatic(bool b)
		{
			m_IsStatic = b;
		}

		void enableShadow();
		void disableShadow();

		void enableDiffuse()
		{
			m_DoDiffuse = true;
		}

		void disableDiffuse()
		{
			m_DoDiffuse = false;
		}

		void enableAmbient()
		{
			m_DoAmbient = true;
		}

		void disableAmbient()
		{
			m_DoAmbient = false;
		}

		void enableSpecular()
		{
			m_DoSpecular = true;
		}

		void disableSpecular()
		{
			m_DoSpecular = false;
		}

	protected:
		virtual void _serialize(Archive &ar, int version);

	public:
		LightType m_Type;
		
		bool m_DoShadow;
		bool m_DoSpecular;
		bool m_DoAmbient;
		bool m_DoDiffuse;
		bool m_IsStatic;

		ColourValue m_DiffuseColor;
		ColourValue m_AmbientColor;
		ColourValue m_SpecularColor;
		float m_SpecularPower;

		float m_Attenuation;
		float m_Range;

		Shadowmap*	m_pShadowmap;
	};
}

#endif