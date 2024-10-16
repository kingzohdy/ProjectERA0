
#pragma once
#include <map>
#include <string>
#include <vector>
#include "TerrainEditTypes.h"

namespace TE
{

class MTexManager
{
public:
	MTexManager(void);
	~MTexManager(void);

	bool    GetTex(const char* texFile,LMTex pTex);
    LMTex   GetTex (const char* texFile);

	LMTex    LoadTextureFile(const char* texFile);

	LMTex    LoadTex(const char* texFile) ;
	bool     DelTex(const char* texFile) ;

	LMTex CreateTex(void* data,int width,int height,int size,int mode,int minMap = 0) ;

	LMTex  CreateTexEx(int width,int height,int size,int mode,int minMap= 0) ;

	typedef struct TexInfo
	{
		LMTex  pTex;
		int    width,height;

		int    texNum;

		TexInfo():pTex(NULL),texNum(0),width(0),height(0){};

		void   Add(){texNum++;};
		bool   Del(){texNum--;if(texNum==0){OGRE_RELEASE(pTex);return true;}else return false;}
	}MTEXINFO;

	void   Release();
	bool   Update(LMTex pTex,void* data,int width,int height,int minmap);

	bool   ChangeImageToMayFormat(const char* filename);
private:
	typedef  std::map<std::string ,TexInfo*>        MTEXINFO_SET;
	MTEXINFO_SET                       m_MTexInfoSet;

	std::vector <LMTex>              m_CreateTex;
private:
	bool                             Find(const char* texFile,LMTex& pTexInfo);
	LMTex                            LoadMayTexture(const char* texFile);


};


};
extern TE::MTexManager g_TexManager;