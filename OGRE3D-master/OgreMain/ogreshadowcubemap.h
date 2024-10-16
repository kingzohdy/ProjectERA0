
#ifndef __SHADOWCUBEMAP_H__
#define __SHADOWCUBEMAP_H__

#include "OgreMovableObject.h"
#include "OgreSingleton.h"

namespace Ogre
{
	//class HardwareTexture;
	class NormalSceneRenderer;
	class RT_TEXTURE;
	class TextureRenderTarget;
	class _OgreExport Shadowcubemap : public Singleton<Shadowcubemap>
	{
		DECLARE_RTTI_VIRTUAL(Shadowcubemap)

	public:
		Shadowcubemap();
		~Shadowcubemap();


		virtual void prepare(SceneRenderer* pRenderer, unsigned int framecount);
		virtual void queryShaderEnv(ShaderEnvData &envdata, const Matrix4 &obj_worldtm);
		virtual float getEffectWeight(const Vector3 &point, float radius=0.0f);
		void	getCameras(Camera* pCameras[]);

		void setGlobal(bool b)
		{
			m_isGlobal = b;
		}

		bool isGlobal()
		{
			return m_isGlobal;
		}

		void SetLightPosRange(Vector3 lightpos,float range);
		

	public:
		RT_TEXTURE *m_pDepthTexture;

		Vector3 m_vLightPos;
		float   m_range;
		float	m_shadowdensity;
	public:
		//void caculateShadowCamera(SceneRenderer* pRenderer,Camera *pcamera);
		bool	m_isGlobal;
		size_t  m_PrepareFrameCount;
		Camera* m_pSCMCamera[6];
		TextureRenderTarget* m_rendertarget[6];

	};
}

#endif
