#pragma once


#include "EditEventHandler.h"


namespace TE
{

	class HeightMapEditHandler : public BaseEditHandler
	{
	public:
		HeightMapEditHandler(void);
		~HeightMapEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

		virtual  void  Undo( TerrainEditMgr *pEditor );
		virtual  void  SaveData( TerrainEditMgr* pEditor );

		virtual  void  Clear();

		void  ModifyTerrainHeight( TerrainEditMgr* pEditor ,  int height,int mode = 0);
		void  UpdateHeightMapToTileHeight( TerrainEditMgr* pEditor );
	protected:
		void  ModifyChunkHeight ( TerrainEditMgr* pEditor , float height,int mode);

		void  ModifyChunkHeightEx( TerrainEditMgr* pEditor , int gx,int gz , float height,int mode);

		void  ReCountNormal( TerrainEditMgr* pEditor );
		void  CreatRampMap( TerrainEditMgr* pEditor );

		bool  AddSmoothHeightPos(  TerrainEditMgr* pEditor ,int index);
		void  SmoothHeightMap(  TerrainEditMgr* pEditor );

		void  UpdataNormalMap( TerrainEditMgr* pEditor );
	public:
		bool   SaveCurHeightMap(  TerrainEditMgr* pEditor ,const char* filename);
		void   UpdateHeightMapToAllTileHeight(  TerrainEditMgr* pEditor );

		void    SetHeightMap(  TerrainEditMgr* pEditor ,int width,int height , unsigned char  *heightmap );
		void    UpdateTileHeightDataToHeightMap( TerrainEditMgr* pEditor );

		void    CreateHeightMapEX(TerrainEditMgr* pEditor, Ogre::Point2D nblock, Ogre::Point2D ngrid, int minheight ,int maxheight, int randsize,int smoothNum);

		void   SmoothHeightMap( TerrainEditMgr* pEditor ,int width ,int height );

	private:
		typedef   std::vector <float>         MAP_HEIGHT_MAP;
		std::deque <MAP_HEIGHT_MAP>      m_SaveMapHeightData; 

	};



};