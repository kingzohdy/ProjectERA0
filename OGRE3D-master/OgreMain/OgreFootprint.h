
#ifndef __FOOTPRINT_H__
#define __FOOTPRINT_H__

#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreColourValue.h"
#include "OgreRenderableObject.h"

namespace Ogre
{
	class _OgreExport Footprints : public RenderableObject
	{
		DECLARE_RTTI(Footprints)

	public:
		Footprints();
		Footprints(const char *texpath);

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);

		void addFootprint(const Vector3 &pos, const Vector3 &normal, const Vector3 &dir, float length, float width, float age);

	protected:
		~Footprints();

		Material *m_pMtl;

		struct PrintVertex
		{
			Vector3 pos;
			ColorQuad color;
			Vector2 uv;
		};
		struct OnePrint
		{
			Vector3 pos;
			Vector3 normal;
			Vector3 dir;
			Vector3 binormal;
			float length;
			float width;
			float life;
			float age;
		};

		std::vector<OnePrint>m_Prints;
		TextureData *m_pTexture;
	};
}

#endif