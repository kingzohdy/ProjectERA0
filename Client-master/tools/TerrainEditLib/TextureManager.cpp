
#include "TextureManager.h"
#include "OgreResourceManager.h"
#include "OgreTexture.h"
#include "OgreHardwarePixelBuffer.h"

using namespace Ogre;

namespace TE
{
	TextureResMgr g_TexManager;

	TextureResMgr::TextureResMgr(void)
	{
	}

	TextureResMgr::~TextureResMgr(void)
	{
		Release();
	}

	bool   TextureResMgr::Update(Ogre::Texture * pTex,void* data,int width,int height,int minmap)
	{
		int saveWidth = width;
		int saveHeight = height;

		width = width>>minmap;
		height = height>>minmap;

		LockResult lockresult;
		void *pdestbits = pTex->lock(0, minmap, false, lockresult);
		if(pdestbits != NULL)
		{
			unsigned char*  psrcBit = (unsigned char*)data;
			unsigned char* pbit = (unsigned char*)pdestbits;

			int beishu = 1<<minmap;

			if( lockresult.rowpitch / width == 4 )
			{
				for(int i = 0 ; i < height ; i++)
				{
					for(int j = 0 ; j < width ; j++)
					{
						pbit[4*i*width+4*j] = psrcBit[4*i*beishu*saveWidth + 4*j*beishu];
						pbit[4*i*width+4*j+1] = psrcBit[4*i*beishu*saveWidth + 4*j*beishu+1]; 
						pbit[4*i*width+4*j+2] = psrcBit[4*i*beishu*saveWidth + 4*j*beishu+2];
						pbit[4*i*width+4*j+3] = psrcBit[4*i*beishu*saveWidth + 4*j*beishu+3];
					}
				}
			}
			else
			{
				for(int i = 0 ; i < height ; i++)
				{
					for(int j = 0 ; j < width ; j++)
					{
						pbit[i*width+j] = psrcBit[i*beishu*saveWidth + j*beishu];
					}
				}
			}

			pTex->unlock(0, minmap);

			/*int a = 0;
			if(a)
			{
			pTex->getHardwareTexture()->dumpToDDS("colormask.dds");
			}*/
			return true;
		}

		return false;    
	};	


	void   TextureResMgr::Release()
	{
		for(MTEXINFO_SET::iterator it = m_MTexInfoSet.begin(); it != m_MTexInfoSet.end(); ++it)	
		{
			TexInfo* Texinfo = it->second ;
			OGRE_RELEASE(Texinfo->pTex);	
			delete(Texinfo);
		}
		m_MTexInfoSet.clear();

		for( size_t i = 0 ; i < m_CreateTex.size () ; i++)
		{
			OGRE_RELEASE( m_CreateTex[i] );
		}
	};

	Ogre::Texture * TextureResMgr::LoadMayTexture(const char* texFile)
	{	
		Resource *pres = ResourceManager::getSingleton().blockLoad(texFile);
		if(pres)
		{
			assert(IS_KIND_OF(Texture, pres));
			return static_cast<Texture *>(pres);
		}
		return NULL;
	}

	Ogre::Texture *   TextureResMgr::LoadTextureFile(const char* texFile)
	{
		Ogre::Texture * pTex = NULL;

		std::string name = texFile;
		std::string nameEx ;
		bool bChuFile = true;
		int index = (int)name.rfind (".");
		if(index != -1)
		{
			nameEx = name.substr ( index + 1 , name.length () );
			if( nameEx == "jpg" || nameEx == "bmp" || nameEx == "tga" || nameEx == "dds")
				bChuFile = false;
		}
		if(!bChuFile)
		{
			TextureData *ptex = new TextureData;
			ptex->loadFromImageFile(texFile);
			pTex = ptex;
		}
		else
		{
			pTex = LoadMayTexture(texFile);
		}	

		return pTex ;
	};

	Ogre::Texture * TextureResMgr::LoadTex(const char* texFile)
	{
		Ogre::Texture * tex = NULL;
		if(Find(texFile, tex))
		{
			assert(tex != NULL);
			return tex;
		}

		MTEXINFO *pTexInfo = new MTEXINFO;
		pTexInfo->pTex = LoadTextureFile(texFile);
		if(pTexInfo->pTex == NULL)
		{
			delete pTexInfo;
			return NULL;
		}

		std::string texStr = texFile;
		m_MTexInfoSet[texStr] = pTexInfo;

		return pTexInfo->pTex;
	};

	bool  TextureResMgr::Find(const char* texFile,Ogre::Texture *& pTexInfo)
	{
		MTEXINFO_SET::iterator iter;

		std::string texStr = texFile;
		iter = m_MTexInfoSet.find (texStr);

		if(iter == m_MTexInfoSet.end())
			return false;

		iter->second ->Add ();
		pTexInfo = iter->second->pTex ;

		return true;
	};

	bool TextureResMgr::ChangeImageToMayFormat(const char* filename)
	{
		TextureData *ptex = new TextureData;
		ptex->loadFromImageFile(filename);

		std::string strFilename = filename;

		int index = (int)strFilename.rfind (".");
		if(index != -1)
		{
			strFilename = strFilename.substr(0, index);
		}

		strFilename += ".otex";
		ptex->save(strFilename.c_str());

		ptex->release();
		return true;
	}

	bool TextureResMgr::GetTex(const char* texFile,Ogre::Texture * pTex)
	{
		std::string strFile = texFile;

		MTEXINFO_SET::iterator iter = m_MTexInfoSet.find(strFile);

		if(iter == m_MTexInfoSet.end ())
		{
			return false;
		}

		pTex = iter->second ->pTex ;
		return true;
	};

	Ogre::Texture * TextureResMgr::GetTex (const char* texFile)
	{
		std::string strFile = texFile;

		MTEXINFO_SET::iterator iter = m_MTexInfoSet.find(strFile);

		if(iter == m_MTexInfoSet.end ())
		{
			return NULL;
		}

		return  iter->second ->pTex ;

	}

	bool  TextureResMgr::DelTex(const char* texFile)
	{
		if( m_MTexInfoSet.empty () )
			return false;

		std::string strFile = texFile;

		MTEXINFO_SET::iterator iter = m_MTexInfoSet.find(strFile);

		if(iter == m_MTexInfoSet.end ())
		{
			return false;
		}

		if(iter->second ->Del ())
		{
			delete(iter->second );
			m_MTexInfoSet.erase(iter);
			return true;
		}
		else
			return false;

	};

	Ogre::Texture * TextureResMgr::CreateTexEx(int width,int height,int size,int mode,int minMap) 
	{
		//for render target???
		TextureDesc texdesc;

		texdesc.width = width;
		texdesc.height = height;
		texdesc.format = (PixelFormat)mode;
		texdesc.nmipmap = 1;
		texdesc.type = TEXTURE_2D;
		texdesc.depth = 0;

		TextureData *ptex = new TextureData(texdesc);
		if(ptex) m_CreateTex.push_back(ptex);
		return ptex;
	}

	Ogre::Texture * TextureResMgr::CreateTex(void* data,int width,int height,int size,int mode,int minMap) 
	{	
		TextureDesc texdesc;

		texdesc.width = width;
		texdesc.height = height;
		texdesc.format = (PixelFormat)mode;
		texdesc.nmipmap = 0;
		texdesc.type = TEXTURE_2D;
		texdesc.depth = 0;

		TextureData *ptex = new TextureData(texdesc, false);
		if(ptex == NULL) return NULL;

		if(data!=NULL)
		{
			int swidth = width,minmap = 0;

			for(;;)
			{
				Update(ptex,data,width,height,minmap);
				swidth = swidth >> 1;
				minmap++;
				if(swidth == 0)
					break;
			}	
		}

		m_CreateTex.push_back(ptex);
		return ptex;
	}
}


