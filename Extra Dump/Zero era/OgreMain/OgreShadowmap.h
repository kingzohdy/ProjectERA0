
#ifndef __SHADOWMAP_H__
#define __SHADOWMAP_H__

#include "OgreMovableObject.h"
#include "OgreSceneRenderer.h"
#include "OgreSingleton.h"

namespace Ogre
{
	class RT_TEXTURE;
	class TextureRenderTarget;

	class _OgreExport Shadowmap : public Singleton<Shadowmap>
	{
		DECLARE_RTTI_VIRTUAL(Shadowmap)

	public:
		Shadowmap();
		~Shadowmap();

		virtual void prepare(SceneRenderer* pRenderer, unsigned int framecount);
		virtual void queryShaderEnv(ShaderEnvData &envdata, const Matrix4 &obj_worldtm);
		virtual float getEffectWeight(const Vector3 &point, float radius=0.0f);

		void setGlobal(bool b)
		{
			m_isGlobal = b;
		}

		bool isGlobal()
		{
			return m_isGlobal;
		}

		void onLostDevice();
		void onRestoreDevice();

		bool isValid()
		{
			return m_pDepthTexture!=NULL && m_rendertarget!=NULL;
		}

	public:
		Matrix4 m_ProjMatrix;
		RT_TEXTURE *m_pDepthTexture;
		Vector3 m_vLightDir;
		float	m_shadowdensity;
		uint	m_ShadowCastBits;
		float	m_fAccuracy;	//影子的精度

	public:
		void caculateShadowCamera(SceneRenderer* pRenderer,Camera *pcamera);
		bool	m_isGlobal;
		size_t  m_PrepareFrameCount;
		Camera* m_pShadowCamera;
		TextureRenderTarget* m_rendertarget;
	};
}

#endif
