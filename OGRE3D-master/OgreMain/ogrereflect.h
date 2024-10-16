
#ifndef __REFLECTEFFECT_H__
#define __REFLECTEFFECT_H__

#include "OgreSceneRenderer.h"
#include "OgreSingleton.h"

namespace Ogre
{
	class RT_TEXTURE;
	class TextureRenderTarget;

	class _OgreExport ReflectEffect : public SceneRenderer, public Singleton<ReflectEffect>
	{
		DECLARE_RTTI_VIRTUAL(ReflectEffect)

	public:
		ReflectEffect();
		~ReflectEffect();

		virtual void prepare(SceneRenderer* pRenderer, unsigned int framecount);
		virtual void queryShaderEnv(ShaderEnvData &envdata, const Matrix4 &obj_worldtm);
		virtual float getEffectWeight(const Vector3 &point, float radius=0.0f);

		virtual void onLostDevice();
		virtual void onRestoreDevice();
		virtual void doRender();

		bool isValid()
		{
			return m_pReflectTexture!=NULL && m_rendertarget!=NULL;
		}
	private:
		bool ValidateCreateResult();

	public:
		Matrix4 m_ReflectMatrix;
		RT_TEXTURE *m_pReflectTexture;
		RT_TEXTURE *m_pRefractTexture;
		float m_Height;
	public:
		void caculateReflectCamera(SceneRenderer* pRenderer,Camera *pcamera);
		size_t  m_PrepareFrameCount;
		Camera* m_pRelfectCamera;
		TextureRenderTarget* m_rendertarget;
		TextureRenderTarget* m_RefractTarget;
	};
}

#endif
