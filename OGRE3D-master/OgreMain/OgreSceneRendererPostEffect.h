
#ifndef __SceneRendererPostEffect_H__
#define __SceneRendererPostEffect_H__

#include "OgreSceneRenderer.h"

#define MAX_POSTPRO_INPUT 8

namespace Ogre
{
	enum PostproPhaseType
	{
		PPP_DOWN4			=	0,
		PPP_UP4				=	1,
		PPP_BLURV			=	2,
		PPP_BLURH			=	3,
		PPP_GBLURV			=	4,
		PPP_GBLURH			=	5,
		PPP_BRIGHT			=	6,
		PPP_BLOOMCOM		=	7,
		PPP_DOFCOM			=	8,
		PPP_EDGE			=	9,
		PPP_INVERSE			=	10,
		PPP_GRAY			=	11,
		PPP_MOTIONBLUR		=	12,
		PPP_GLOWCOM			=	13,
		MAX_PPP
	};

	class _OgreExport PostEffectRenderer : public SceneRenderer
	{
	public:
		PostEffectRenderer(const FixedString &mtlname);
		~PostEffectRenderer();

		virtual void doRender(); //compute context pool

	protected:
		Material *m_pMtl;
		HardwareTexture *m_pInputTexture;
		TextureRenderTarget *m_pInputTarget;
	};

	class _OgreExport GlowPostRenderer : public PostEffectRenderer
	{
	public:
		GlowPostRenderer();
		~GlowPostRenderer()
		{
		}

	protected:
		HardwareTexture *m_pInputTexture;
		TextureRenderTarget *m_pInputTarget;
	};

}

#endif