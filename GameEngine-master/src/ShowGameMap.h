
#ifndef __ShowGameMap_H__
#define __ShowGameMap_H__
#include "GameEngineTypes.h"
#include "GameResDBTypes.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreWorldPos.h"
#include "OgreAmbientRegion.h"

namespace Ogre
{
	class ShowObjAlphaMgr;

	enum MAPTYPE_T
	{
		MAP_EMPTY = 0,
		MAP_TERRAIN
	};

	struct REGION_HEAD
	{
		int  nRegionNum;
		int  nVersion;
	};

	class ShowGameMap
	{
	public:
		ShowGameMap();
		~ShowGameMap();

		void attachToRenderer(Ogre::SceneRenderer *prenderer);
		void detachFromRenderer(Ogre::SceneRenderer *prenderer);

		// 加载地图，以wPosCacheAreaCenter为中心缓存数据
		bool LoadFromMapDesc(const MAPDEF &desc, const WorldPos& wPosCacheAreaCenter);

		void InitMiniMap(const char* szUiTex, const char* szMiniMap);
		void UpdateMiniMap(int nX, int nY , int nMipLevel );

		void CreateEmptyMap();
		void PreloadFiles();

		WPOS_T getGroundPoint(WPOS_T x, WPOS_T z, Vector3 *pnormal, float *pwaterheight);
		bool pickGround(const WorldRay &worldray, float* pfDis);
		Entity *pickActor(const WorldRay& worldray, float* pfDis);
		void getMapOrigin(WPOS_T &x, WPOS_T &z);

		void addFootprint(const WorldPos &pos, const Vector3 &dir);

		void update(unsigned int dtick, Camera* pCamera, const WorldPos &player_pos);
		void setOverlayMusic(const char *path, float volume);

		int				GetMapID()		{ return m_nMapID; }
		const char*		GetMapName()	{ return m_strMapName.c_str (); }
		const char*		GetMapResName()	{ return  m_strMapResName.c_str(); }
		GameScene *		getGameScene()	{ return m_pScene; }
		
		int				getMapWidthGrid()	{ return m_nWidthGrid; }
		int				getMapHeightGrid()	{ return m_nHeightGrid; }
		float			getGridSize()		{ return m_fGridSize; }

		void setOverlayRegion(const char *pname, float fSpeed=0.5f);

	private:
		void updateAreaByTime(float tick);          ///< 基于时间插值更新区域信息

	private:
		//地图资源名
		std::string             m_strMapResName;
		std::string             m_MiniMapResName;

		MAPTYPE_T				m_MapType;

		WorldPos			m_CenterPos;
		GameScene *m_pScene;

		int						m_nMapID;
		int						m_nTileX;
		int						m_nTileZ;

		std::vector<bool>		m_TileTable;

		float					m_LoadProgress;


		//当前地图相关信息
		int                      m_nWidthGrid;//宽度格子
		int                      m_nHeightGrid;//高度格子
		float                    m_fGridSize;//每个格子的尺寸

		//小地图纹理
		TextureData*           m_pMinMapTex;
		//小地图MASK
		TextureData*           m_pMapMaskTex;    
		//地图名称
		std::string              m_strMapName;

		GameMapRegionSet *m_pRegionSet;
		GameMapRegion *m_pCurRegion;
		AmbientManager *m_pAmbientMgr;

		std::string				m_OverlayMusicPath;
		float					m_OverlayMusicVolume;

		Footprints *m_pFootprints;
	};
}

#endif