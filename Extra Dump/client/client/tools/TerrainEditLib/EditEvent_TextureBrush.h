
#pragma once
#include "EditEventHandler.h"

namespace TE
{
	class TextureBrushEditHandler : public BaseEditHandler
	{
	public:
		TextureBrushEditHandler();
		~TextureBrushEditHandler();

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
		virtual void Undo( TerrainEditMgr* pEditor );

	private:
		Ogre::Vector3  PaintTexture( int x ,int y );
		void   PaintTexture ( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir, Ogre::Vector3 &pos ,int mode ,int x,int y);
		void   ModifyAlphaTex( TerrainEditMgr* pEditor ,int gx,int gz ,int layer,int x,int z,unsigned char* data,int texid);
		void duplateAlphaTexEdge(TerrainEditMgr* pEditor, int gx, int gz, int layer);

		void   CreateQuadPaint( TerrainEditMgr* pEditor );
		void   CreateRandPaint ( TerrainEditMgr* pEditor );
		void   CreateRandCirclePaint(TerrainEditMgr* pEditor );
	public:
		void   ResetPaintTexture( TerrainEditMgr* pEditor );
		int    AddTextureFileToTitle( TerrainEditMgr* pEditor , std::string name);

		void   CreateAlphaPaint( TerrainEditMgr* pEditor );
		int     AddTextureFileToTile( TerrainEditMgr* pEditor , const char* filename);

		void    UpdateAlphaData( TerrainEditMgr* pEditor ,const char* filename, int layer);
	public:
		void    SaveChunkTextureInfo( TerrainEditMgr* pEditor );
		void    LoadChunkTextureInfo( TerrainEditMgr* pEditor );

	public:
		void    UpdateColorData( TerrainEditMgr* pEditor ,const char* filename );
		void    UpdateShadowData( TerrainEditMgr* pEditor ,const char* filename );

		void    SaveAlphaTex(  TerrainEditMgr* pEditor , const char* filename, unsigned char* pData,int layer);
		void    LoadTerainLayerInfo( TerrainEditMgr* pEditor );

		void    SetCurSelectPaintTexName( TerrainEditMgr* pEditor ,const char* filename);


	};

};