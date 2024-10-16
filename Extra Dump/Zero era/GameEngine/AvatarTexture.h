
#ifndef __AvatarTexture_H__
#define __AvatarTexture_H__

#include "OgreFixedString.h"
#include "OgreModel.h"
#include "SmartPtr.h"
#include <list>

namespace Ogre
{
	enum CharRegions {
		CR_BASE = 0,
		CR_ARM_UPPER,
		CR_ARM_LOWER,
		CR_HAND,
		CR_FACE_UPPER,
		CR_FACE_LOWER,
		CR_TORSO_UPPER,
		CR_TORSO_LOWER,
		CR_PELVIS_UPPER,
		CR_PELVIS_LOWER,
		CR_FOOT,
		NUM_REGIONS,

		CR_LEG_UPPER = CR_PELVIS_UPPER,
		CR_LEG_LOWER = CR_PELVIS_LOWER,

		CR_TEX_A = 0,
		CR_TEX_B = 1,
		CR_TEX_C = 2,
		CR_TEX_D = 3,
		CR_TEX_E = 4,
		CR_TEX_F = 5,
		CR_TEX_G = 6,
		CR_TEX_H = 7
	};

	struct CharTextureComponent
	{
		FixedString name;
		int region;
		int layer;

		const bool operator<(const CharTextureComponent& c) const
		{
			return layer < c.layer;
		}
	};

	struct CharRegionCoords {
		int xpos, ypos, xsize, ysize;
	};


	class AvatarTexture : public AutoRelease<AvatarTexture>
	{
	public:
		std::list<CharTextureComponent> components;
		int m_TextureID;
		int m_Width;
		int m_Height;

	public:
		void init(int id, int width, int height)
		{
			m_TextureID = id;
			m_Width = width;
			m_Height = height;

			components.resize(0);
		}

		void addLayer(const char *fn, int region, int layer)
		{
			if(fn==NULL || fn[0]==0) return;

			CharTextureComponent ct;
			ct.name = fn;
			ct.region = region;
			ct.layer = layer;
			
			//相同的层被替换
			std::list<CharTextureComponent>::iterator iter = components.begin();
			for(; iter!=components.end(); iter++)
			{
				if(iter->layer == layer)
				{
					*iter = ct;
					return;
				}
			}

			components.push_back(ct);
		}

		void compose(TextureData *ptex);
		void compose(TextureData *pdesttex, TextureData *ptex, LockResult &lockresult, void *pbits, int region);
		void composenew(TextureData *pdesttex);
		TextureData *compose();
		TextureDataLoader *asyncompose();
		TextureData *composeIllumSpecular();
		TextureData *composeMask();

		bool composeIllumSpecular(FixedString &path);
		bool composeMask(FixedString &path);
	};

	typedef SmartPtr<AvatarTexture> AvatarTexturePtr;

	class AvatarTextureLoader : public TextureDataLoader
	{
	private:
		AvatarTexturePtr m_pTex;
		TextureData *m_tex;

	public:
		AvatarTextureLoader(AvatarTexture *tex);
		virtual ~AvatarTextureLoader();
		void startLoad(const FixedString &filename);

	protected:
		virtual TextureData * getTextureData(size_t size, TextureData **recs);
	};

}

#endif