
#pragma once


#include "EditEventHandler.h"


namespace TE
{

class MPaintTextureProcess 
	: public IEventProcess
{
public:
	MPaintTextureProcess();
	~MPaintTextureProcess();

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
	virtual void Undo( MTerrainEdit* pEditor );

private:
	MVec3  PaintTexture( int x ,int y );
	void   PaintTexture ( MTerrainEdit* pEditor ,MVec3 &dir, MVec3 &pos ,int mode ,int x,int y);
	void   ModifyAlphaTex( MTerrainEdit* pEditor ,int gx,int gz ,int layer,int x,int z,unsigned char* data,int texid);

	void   CreateQuadPaint( MTerrainEdit* pEditor );
    void   CreateRandPaint ( MTerrainEdit* pEditor );
    void   CreateRandCirclePaint(MTerrainEdit* pEditor );
public:
	void   ResetPaintTexture( MTerrainEdit* pEditor );
	int    AddTextureFileToTitle( MTerrainEdit* pEditor , std::string name);

	void   CreateAlphaPaint( MTerrainEdit* pEditor );
    int     AddTextureFileToTile( MTerrainEdit* pEditor , const char* filename);
    
	void    UpdateAlphaData( MTerrainEdit* pEditor ,const char* filename, int layer);
public:
	void    SaveChunkTextureInfo( MTerrainEdit* pEditor );
    void    LoadChunkTextureInfo( MTerrainEdit* pEditor );

public:
	void    UpdateColorData( MTerrainEdit* pEditor ,const char* filename );
    void    UpdateShadowData( MTerrainEdit* pEditor ,const char* filename );

    void    SaveAlphaTex(  MTerrainEdit* pEditor , const char* filename, unsigned char* pData,int layer);
    void    LoadTerainLayerInfo( MTerrainEdit* pEditor );

	void    SetCurSelectPaintTexName( MTerrainEdit* pEditor ,const char* filename);


};

};