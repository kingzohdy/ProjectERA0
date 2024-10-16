
#pragma once


#include "mterrainedit.h"

namespace TE
{

class MTileProcess
{
public:
	MTileProcess(void);
	~MTileProcess(void);

    
	bool  CreateTitle( MTerrainEdit* pEditor , int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum,
								const char* basetex , int nRepeat,
								int gwidthnum ,int gheightnum );

	bool  SaveTerrainFile(  MTerrainEdit* pEditor ,const char* filename);
    
	bool  LoadTerrainFile(  MTerrainEdit* pEditor ,const char* filename);
    //计算崖壁的顶点光照
	void  CountLinkMeshColor( MTerrainEdit* pEditor );
private:
	bool  SaveSignToMySql(  MTerrainEdit* pEditor ,unsigned int index );
	bool  TsetMapDataIsEmpty(unsigned char *data);

    bool  savePlantsForEachBlock(MTerrainEdit* pEditor , const char* path ,const char* filename);
	bool  CreateTitle_Internal( MTerrainEdit* pEditor , int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum, const char* basetex, int nRepeat, int gwidthnum ,int gheightnum);
};

};


extern TE::MTileProcess   g_TileProcess;