
#include "AvatarTexture.h"
#include "OgreVector4.h"
#include "OgreTexture.h"
#include "OgreResourceManager.h"
#include <string>

namespace Ogre
{
	CharRegionCoords regions[] =
	{
		{0, 0, 256, 256},	// base
		{0, 0, 128, 64},	// arm upper
		{0, 64, 128, 64},	// arm lower
		{0, 128, 128, 32},	// hand
		{0, 160, 128, 32},	// face upper
		{0, 192, 128, 64},	// face lower
		{128, 0, 128, 64},	// torso upper
		{128, 64, 128, 32},	// torso lower
		{128, 96, 128, 64}, // pelvis upper
		{128, 160, 128, 64},// pelvis lower
		{128, 224, 128, 32}	// foot
	};

	Vector4 regionsnew[] =
	{
		Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base

			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base
			Vector4(-1.0f,  1.0f, -1.0f, 1.0f),	// base

			/*
			Vector4(-1.0f,  0.0f, -1.0f, -0.5f),	// arm upper
			Vector4(-1.0f,  0.0f, -0.5f, 0.0f),	// arm lower
			Vector4(-1.0f, 0.0f,   0.0f, -0.25f),	// hand
			Vector4(-1.0f, 0.0f, 0.25, 0.5),	// face upper
			Vector4(-1.0f, 192, 128, 64),	// face lower
			Vector4(0.0f, 0, 128, 64),	// torso upper
			Vector4(0.0f, 64, 128, 32),	// torso lower
			Vector4(0.0f, 96, 128, 64), // pelvis upper
			Vector4(0.0f, 160, 128, 64),// pelvis lower
			Vector4(0.0f, 224, 128, 32)	// foot
			*/
	};

	void Path2ChunkName(char *buffer, const char *path, const char *pheader);

	void AvatarTexture::compose(TextureData *pdesttex)
	{
		LockResult lockresult;
		void *pbits = pdesttex->lock(0, 0, false, lockresult);

		char buffer[1024];
		int count = 0;
		components.sort();
		std::list<CharTextureComponent>::iterator iter = components.begin();
		for(; iter!=components.end(); iter++)
		{
			CharTextureComponent &ct = *iter;
			//Path2ChunkName(buffer, (const char *)ct.name, "tex.wow");
			strncpy(buffer, ct.name,1024);

			TextureData *ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
			if(ptex == NULL)
			{
				continue;
			}

			TextureDesc desc;
			ptex->getDesc(desc);

			CharRegionCoords coord = regions[ct.region];
			coord.xpos = coord.xpos * m_Width / 256;
			coord.xsize = coord.xsize * m_Width / 256;
			coord.ypos = coord.ypos * m_Height / 256;
			coord.ysize = coord.ysize * m_Height / 256;

			assert(coord.xsize==desc.width && coord.ysize==desc.height);
			assert(coord.xpos+coord.xsize<=m_Height && coord.ypos+coord.ysize<=m_Height);

			LockResult lockresult2;
			const void *pbits2 = ptex->lock(0, 0, true, lockresult2);
			assert(desc.format == PF_A8R8G8B8);

			for(int y=0; y<coord.ysize; y++)
			{
				const unsigned char *psrcbits = (const unsigned char *)pbits2 + lockresult2.rowpitch*y;
				unsigned char *pdstbits = (unsigned char *)pbits + lockresult.rowpitch*(y+coord.ypos) + coord.xpos*4;

				for(int x=0; x<coord.xsize; x++)
				{
					int r = psrcbits[3];

					pdstbits[0] = (unsigned char)((pdstbits[0]*(255-r) + psrcbits[0]*r)/255);
					pdstbits[1] = (unsigned char)((pdstbits[1]*(255-r) + psrcbits[1]*r)/255);
					pdstbits[2] = (unsigned char)((pdstbits[2]*(255-r) + psrcbits[2]*r)/255);
					pdstbits[3] = (pdstbits[3] + r > 255) ? 255 : (pdstbits[3] + r);

					psrcbits += 4;
					pdstbits += 4;
				}
			}

			ptex->unlock(0, 0);
			ptex->release();
		}

		pdesttex->unlock(0, 0);
	}

	/*
	void AvatarTexture::composenew(TextureData *pdesttex)
	{
		TextureData::LOCKRESULT_T lockresult;
		TextureData::LOCKDESC_T lockdesc;

		IDirect3DSurface9* pSurfRT;
		IDirect3DSurface9* pSysSurf;
		IDirect3DSurface9* pSurf;
		m_pRTTexture->GetSurfaceLevel(0,&pSurf);
		m_pDevice->BeginScene();
		m_pDevice->GetRenderTarget(0,&pSurfRT);

		m_pDevice->SetRenderTarget(0,pSurf);


		m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET,0xff000000,1.0f,0);

		static int cccc = 0;

		char buffer[1024];

		size_t cPasses = m_pComShader->Begin();
		for(size_t p=0;p<cPasses;p++)
		{
			m_pComShader->BeginPass(p);

			std::list<CharTextureComponent>::iterator iter = components.begin();
			for(; iter!=components.end(); iter++)
			{
				CharTextureComponent &ct = *iter;
				//Path2ChunkName(buffer, (const char *)ct.name, "tex.wow");
				strncpy(buffer, ct.name);

				cccc++;
				if(cccc == 10)
				{
					cccc = cccc;
				}

				MAY::ResourceLoader *presloader = m_pEngine->GetResLoader();
				MAY::TextureSource *ptex = static_cast<MAY::TextureSource *>(presloader->BlockLoad(buffer));
				m_pComShader->SetTexture("MeshTexture",ptex->m_pSysTex);
				ptex->m_pSysTex->SaveToFile("1.dds");
				m_pComShader->CommitChanges();
				DrawQuad(regionsnew[ct.region]);
			}

			m_pComShader->EndPass();
		}
		m_pComShader->End();
		m_pDevice->EndScene();

		m_pTex->GetSurfaceLevel(0,&pSysSurf);

		m_pDevice->GetRenderTargetData(pSurf,pSysSurf);

		SAFE_RELEASE(pSysSurf);
		SAFE_RELEASE(pSurf);

		D3DLOCKED_RECT rect;
		m_pTex->LockRect(0,&rect,NULL,0);
		lockdesc.flags = MAY::LOCK_WRITE;
		lockdesc.iface = lockdesc.level = 0;
		if(pdesttex->Lock(lockresult, lockdesc) == NULL)
		{
			return;
		}
		memcpy(lockresult.pbits,rect.pBits,rect.Pitch*m_Height);

		pdesttex->Unlock(0);

		pdesttex->SaveToFile("avatar.dds");
		m_pTex->UnlockRect(0);

		m_pDevice->SetRenderTarget(0,pSurfRT);
		SAFE_RELEASE(pSurfRT);
	}
	*/

	TextureData *AvatarTexture::compose()
	{
		if(m_Width == 0)
		{
			CharTextureComponent &ct = components.front();
			TextureData *ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(ct.name));
			return ptex;
		}
		else
		{
			assert(0);

			TextureDesc texdesc;
			texdesc.type = TEXTURE_2D;
			texdesc.width = m_Width;
			texdesc.height = m_Height;
			texdesc.depth = 1;
			texdesc.format = PF_A8R8G8B8;
			texdesc.nmipmap = 1;
			texdesc.cubefaces = 0;

			TextureData *pdestex = new TextureData(texdesc);

			LockResult lockresult;
			void *pbits = pdestex->lock(0, 0, false, lockresult);

			components.sort();
			
			for(std::list<CharTextureComponent>::iterator i = components.begin(), j = components.end();
				i != j; ++i)
			{
				CharTextureComponent &ct = *i;
				TextureData *ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(ct.name));
				if(ptex == NULL)
				{
					continue;
				}
				compose(pdestex, ptex, lockresult, pbits, ct.region);
				ptex->release();
			}

			pdestex->unlock(0, 0);
			return pdestex;
		}
	}

	void AvatarTexture::compose(TextureData *pdesttex, TextureData *ptex, LockResult &lockresult, void *pbits, int region)
	{
		TextureDesc desc;
		ptex->getDesc(desc);

		CharRegionCoords coord = regions[region];
		coord.xpos = coord.xpos * m_Width / 256;
		coord.xsize = coord.xsize * m_Width / 256;
		coord.ypos = coord.ypos * m_Height / 256;
		coord.ysize = coord.ysize * m_Height / 256;

		assert(coord.xsize==desc.width && coord.ysize==desc.height);
		assert(coord.xpos+coord.xsize<=m_Height && coord.ypos+coord.ysize<=m_Height);

		LockResult lockresult2;
		const void *pbits2 = ptex->lock(0, 0, true, lockresult2);
		assert(desc.format == PF_A8R8G8B8);

		for(int y=0; y<coord.ysize; y++)
		{
			const unsigned char *psrcbits = (const unsigned char *)pbits2 + lockresult2.rowpitch*y;
			unsigned char *pdstbits = (unsigned char *)pbits + lockresult.rowpitch*(y+coord.ypos) + coord.xpos*4;

			for(int x=0; x<coord.xsize; x++)
			{
				int r = psrcbits[3];

				pdstbits[0] = (unsigned char)((pdstbits[0]*(255-r) + psrcbits[0]*r)/255);
				pdstbits[1] = (unsigned char)((pdstbits[1]*(255-r) + psrcbits[1]*r)/255);
				pdstbits[2] = (unsigned char)((pdstbits[2]*(255-r) + psrcbits[2]*r)/255);
				pdstbits[3] = (pdstbits[3] + r > 255) ? 255 : (pdstbits[3] + r);

				psrcbits += 4;
				pdstbits += 4;
			}
		}

		ptex->unlock(0, 0);
	}

	TextureDataLoader *AvatarTexture::asyncompose()
	{
		AvatarTextureLoader *loader = new AvatarTextureLoader(this);
		if (m_Width == 0)
		{
			CharTextureComponent &ct = components.front();
			loader->startLoad(ct.name);
		}
		else
		{
			for(std::list<CharTextureComponent>::iterator i = components.begin(), j = components.end();
				i != j; ++i)
			{
				CharTextureComponent &ct = *i;
				loader->startLoad(ct.name);
			}
		}
		return loader;
	}

	TextureData *AvatarTexture::composeIllumSpecular()
	{
		assert(m_Width == 0);

		TextureData *ptex = NULL;
		char buffer[256];
		CharTextureComponent &ct = components.front();
		do 
		{
			strncpy(buffer, ct.name, sizeof(buffer));
			char *pdot = strrchr(buffer, '.');
			if (pdot == NULL)
				break;
			strcpy(pdot, "_s.dds");
			if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
			{
				break;
			}
			ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
		} while (false);

		if (ptex == NULL)
		{
			do 
			{
				strncpy(buffer, ct.name, sizeof(buffer));
				char *pdot = strrchr(buffer, '_');
				if (pdot == NULL)
					break;
				strcpy(pdot, "_s.dds");
				if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
					break;
				ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
			} while (false);
		}

		return ptex;
	}

	bool AvatarTexture::composeIllumSpecular(FixedString &path)
	{
		assert(m_Width == 0);

		bool result = false;
		char buffer[256];
		CharTextureComponent &ct = components.front();
		do 
		{
			strncpy(buffer, ct.name, sizeof(buffer));
			char *pdot = strrchr(buffer, '.');
			if (pdot == NULL)
				break;
			strcpy(pdot, "_s.dds");
			if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
			{
				break;
			}
			result = true;
			path = buffer;
		} while (false);

		if (!result)
		{
			do 
			{
				strncpy(buffer, ct.name, sizeof(buffer));
				char *pdot = strrchr(buffer, '_');
				if (pdot == NULL)
					break;
				strcpy(pdot, "_s.dds");
				if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
					break;
				result = true;
				path = buffer;
			} while (false);
		}

		return result;
	}

	TextureData *AvatarTexture::composeMask()
	{
		assert(m_Width == 0);

		TextureData *ptex = NULL;
		char buffer[256];
		CharTextureComponent &ct = components.front();
		do 
		{
			strncpy(buffer, ct.name, sizeof(buffer));
			char *pdot = strrchr(buffer, '.');
			if (pdot == NULL)
				break;
			strcpy(pdot, "_m.dds");
			if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
				break;
			ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
		} while (false);

		if (ptex == NULL)
		{
			do 
			{
				strncpy(buffer, ct.name, sizeof(buffer));
				char *pdot = strrchr(buffer, '_');
				if (pdot == NULL)
					break;
				strcpy(pdot, "_m.dds");
				if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
					break;
				ptex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
			} while (false);
		}

		return ptex;
	}

	bool AvatarTexture::composeMask(FixedString &path)
	{
		assert(m_Width == 0);

		bool result = false;
		char buffer[256];
		CharTextureComponent &ct = components.front();
		do 
		{
			strncpy(buffer, ct.name, sizeof(buffer));
			char *pdot = strrchr(buffer, '.');
			if (pdot == NULL)
				break;
			strcpy(pdot, "_m.dds");
			if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
				break;
			result = true;
			path = buffer;
		} while (false);

		if (!result)
		{
			do 
			{
				strncpy(buffer, ct.name, sizeof(buffer));
				char *pdot = strrchr(buffer, '_');
				if (pdot == NULL)
					break;
				strcpy(pdot, "_m.dds");
				if (ResourceManager::getSingleton().getFileAttrib(buffer) == 0)
					break;
				result = true;
				path = buffer;
			} while (false);
		}

		return result;
	}

	AvatarTextureLoader::AvatarTextureLoader(AvatarTexture *tex) : m_pTex(tex), m_tex(NULL)
	{
	}

	AvatarTextureLoader::~AvatarTextureLoader()
	{
		OGRE_RELEASE(m_tex);
	}

	void AvatarTextureLoader::startLoad(const FixedString &filename)
	{
		TextureDataLoader::startLoad(filename, ResourceManager::DP_MONSTER_NPC);
	}

	TextureData * AvatarTextureLoader::getTextureData(size_t size, TextureData **recs)
	{
		m_tex = m_pTex->compose();
		return m_tex;
	}

}