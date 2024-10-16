
#pragma once
#include <map>
#include <string>
#include <vector>
#include "TerrainEditTypes.h"

namespace TE
{

	class TextureResMgr
	{
	public:
		TextureResMgr(void);
		~TextureResMgr(void);

		bool    GetTex(const char* texFile,Ogre::Texture * pTex);
		Ogre::Texture *   GetTex (const char* texFile);

		Ogre::Texture *    LoadTextureFile(const char* texFile);

		Ogre::Texture *    LoadTex(const char* texFile) ;
		bool     DelTex(const char* texFile) ;

		Ogre::Texture * CreateTex(void* data,int width,int height,int size,int mode,int minMap = 0) ;

		Ogre::Texture *  CreateTexEx(int width,int height,int size,int mode,int minMap= 0) ;

		typedef struct TexInfo
		{
			Ogre::Texture *  pTex;
			int    width,height;

			int    texNum;

			TexInfo():pTex(NULL),texNum(0),width(0),height(0){};

			void   Add(){texNum++;};
			bool   Del(){texNum--;if(texNum==0){OGRE_RELEASE(pTex);return true;}else return false;}
		}MTEXINFO;

		void   Release();
		bool   Update(Ogre::Texture * pTex,void* data,int width,int height,int minmap);

		bool   ChangeImageToMayFormat(const char* filename);
	private:
		typedef  std::map<std::string ,TexInfo*>        MTEXINFO_SET;
		MTEXINFO_SET                       m_MTexInfoSet;

		std::vector <Ogre::Texture *>              m_CreateTex;
	private:
		bool                             Find(const char* texFile,Ogre::Texture *& pTexInfo);
		Ogre::Texture *                            LoadMayTexture(const char* texFile);


	};

	extern TextureResMgr g_TexManager;
}