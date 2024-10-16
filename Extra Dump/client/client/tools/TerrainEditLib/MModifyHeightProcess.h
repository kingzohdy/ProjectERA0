#pragma once


#include "EditEventHandler.h"


namespace TE
{

class MModifyHeightProcess
	: public IEventProcess
{
public:
	MModifyHeightProcess(void);
	~MModifyHeightProcess(void);
	
	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

	virtual  void  Undo( MTerrainEdit *pEditor );
	virtual  void  SaveData( MTerrainEdit* pEditor );

	virtual  void  Clear();

	void  ModifyTerrainHeight( MTerrainEdit* pEditor ,  int height,int mode = 0);
	void  UpdateHeightMapToTileHeight( MTerrainEdit* pEditor );
protected:
	void  ModifyChunkHeight ( MTerrainEdit* pEditor , float height,int mode);

    void  ModifyChunkHeightEx( MTerrainEdit* pEditor , int gx,int gz , float height,int mode);
	
	void  ReCountNormal( MTerrainEdit* pEditor );
	void  CreatRampMap( MTerrainEdit* pEditor );
	
	bool  AddSmoothHeightPos(  MTerrainEdit* pEditor ,int index);
    void  SmoothHeightMap(  MTerrainEdit* pEditor );

	void  UpdataNormalMap( MTerrainEdit* pEditor );
public:
    bool   SaveCurHeightMap(  MTerrainEdit* pEditor ,const char* filename);
    void   UpdateHeightMapToAllTileHeight(  MTerrainEdit* pEditor );

    void    SetHeightMap(  MTerrainEdit* pEditor ,int width,int height , unsigned char  *heightmap );
    void    UpdateTileHeightDataToHeightMap( MTerrainEdit* pEditor );

    void    CreateHeightMapEX(MTerrainEdit* pEditor, MPoint2D nblock, MPoint2D ngrid, int minheight ,int maxheight, int randsize,int smoothNum);

	void   SmoothHeightMap( MTerrainEdit* pEditor ,int width ,int height );

private:
	typedef   std::vector <float>         MAP_HEIGHT_MAP;
	std::deque <MAP_HEIGHT_MAP>      m_SaveMapHeightData; 

};



};