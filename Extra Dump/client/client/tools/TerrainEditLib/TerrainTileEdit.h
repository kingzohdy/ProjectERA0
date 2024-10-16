
#pragma once


#include "TerrainEdit.h"

namespace TE
{

	class TileEditMgr
	{
	public:
		TileEditMgr(void);
		~TileEditMgr(void);


		bool  CreateTitle( TerrainEditMgr* pEditor , int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum,
			const char* basetex , int nRepeat,
			int gwidthnum ,int gheightnum );

		bool saveObjFile(TerrainEditMgr* pEditor, const char *filename);
		bool saveTerrainObjFile (TerrainEditMgr* pEditor, const char *filename);
		bool saveTerrainWaterObjFile (TerrainEditMgr* pEditor, const char *filename);
		bool AddObjectsToBeast (TerrainEditMgr* pEditor);

		bool  SaveTerrainFile(  TerrainEditMgr* pEditor ,const char* filename);

		bool  LoadTerrainFile(  TerrainEditMgr* pEditor ,const char* filename);
		//计算崖壁的顶点光照
		void  CountLinkMeshColor( TerrainEditMgr* pEditor );
	private:
		bool  SaveSignToMySql(  TerrainEditMgr* pEditor ,unsigned int index );
		bool  TsetMapDataIsEmpty(unsigned char *data);

		bool  savePlantsForEachBlock(TerrainEditMgr* pEditor , const char* path ,const char* filename);
		bool  CreateTitle_Internal( TerrainEditMgr* pEditor , int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum, const char* basetex, int nRepeat, int gwidthnum ,int gheightnum);

		Ogre::TerrainTileSource *exportToTileResource(TerrainEditMgr *pEditor);
	};

	extern TileEditMgr   g_TileProcess;
}