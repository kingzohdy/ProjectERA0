#pragma once


#include "mterrainedit.h"

namespace TE
{
//chunkֲ���ļ�ͷ
struct MChunkPlantHead
{
	int       version;
	int       chunkNum;
};
//CHUNKֲ����Ϣ
struct MChunkPlantInfo
{
	unsigned char gx,gy;
	int           plantKind;
};
//ֲ����Ϣ
struct MPlantInfo
{
	char   plantTex[256];
	int    plantNum;
};
//CHUNKֲ�����
class CChunkPlantManager
{
public:
	CChunkPlantManager(void);
	~CChunkPlantManager(void);

	bool   SaveChunkPlant( MTerrainEdit* pEditor , const char* filename );
	bool   LoadChunkPlant( MTerrainEdit* pEditor , const char* filename );
};

};