#pragma once


#include "mterrainedit.h"

namespace TE
{
//chunk植物文件头
struct MChunkPlantHead
{
	int       version;
	int       chunkNum;
};
//CHUNK植物信息
struct MChunkPlantInfo
{
	unsigned char gx,gy;
	int           plantKind;
};
//植物信息
struct MPlantInfo
{
	char   plantTex[256];
	int    plantNum;
};
//CHUNK植物管理
class CChunkPlantManager
{
public:
	CChunkPlantManager(void);
	~CChunkPlantManager(void);

	bool   SaveChunkPlant( MTerrainEdit* pEditor , const char* filename );
	bool   LoadChunkPlant( MTerrainEdit* pEditor , const char* filename );
};

};