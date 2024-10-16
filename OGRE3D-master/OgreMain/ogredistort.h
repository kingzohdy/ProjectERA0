
#ifndef __DISTORT_H__
#define __DISTORT_H__

#include "OgreSceneRenderer.h"
#include "OgreVertexFormat.h"
#include "OgreSingleton.h"

namespace Ogre
{
	class RT_TEXTURE;

	class _OgreExport DistortEffect : public SceneRenderer, public Singleton<DistortEffect>
	{
	public:
		RT_TEXTURE				*m_pSceneTexture;
		TextureRenderTarget		*m_pSceneRT;

		RT_TEXTURE				*m_pDistortTexture;
		TextureRenderTarget		*m_pDistortRT;

		RenderTarget		*m_pOutputRenderTarget;
		Material *m_pMtl;
		VertexFormat m_VertFmt;
		VertexDeclHandle m_VertDecl;

	public:
		DistortEffect();
		~DistortEffect();

		void setGray (bool bGray);

		bool getGray ()
		{
			return m_bGray;
		}

		bool isValid()
		{
			return m_pDistortTexture!=NULL && m_pDistortRT!=NULL && m_pSceneRT!=NULL;
		}

		virtual void onLostDevice();
		virtual void onRestoreDevice();
		virtual void doRender();
	private:
		bool ValidateCreateResult();
	protected:
		bool m_bGray;
	};
}

#endif