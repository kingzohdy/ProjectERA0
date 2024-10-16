
#ifndef __RenderTarget_H__
#define __RenderTarget_H__

#include "OgrePrerequisites.h"
#include "OgreBaseObject.h"

namespace Ogre
{
	class _OgreExport RenderTarget : public BaseObject
	{
		DECLARE_RTTI_VIRTUAL(RenderTarget);

	public:
		virtual ~RenderTarget() {}

		virtual void getSize(uint &width, uint &height) = 0;
		virtual bool beginScene() = 0;
		virtual void endScene() = 0;
		virtual bool isSuccessCreateRenderTarget();
		virtual bool isSuccessCreateDSBuffer();
		virtual void setStretchRect( int left, int top, int right, int bottom, int dLeft, int dTop, int dRight, int dBottom ){};
	};

	class _OgreExport TextureRenderTarget : public RenderTarget
	{
		DECLARE_RTTI_VIRTUAL(TextureRenderTarget)
	public:
		virtual ~TextureRenderTarget() {}
		virtual HardwareTexture *getColorTexture() = 0;
	};
}

#endif