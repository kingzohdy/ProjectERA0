
#ifndef __TerrainData_H__
#define __TerrainData_H__

#include "OgreResource.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreBounding.h"
#include "OgreMatrix4.h"
#include "OgreWorldPos.h"
#include "OgreArchive.h"
#include "OgreRenderTypes.h"
#include "OgreColourValue.h"
#include "OgreTexture.h"

namespace Ogre
{
#define  TERRAIN_ALPHATEX_NUM   3
#define  TERRAIN_TEX_NUM        4

#define  TERRAIN_GRID_SOLID		0			// 地形为实体
#define  TERRAIN_GRID_KILL		1			// 地形被挖空
#define  TERRAIN_GRID_REPLACE	2			// 地形被挖空后被拼接模型代替


	const int blockstripsize =  8*18 + 7*2;
	const int blockstripsize1 = 16*18 + 7*2 + 8*2;

	struct TerrainBlockDesc
	{
		unsigned int  flags;
		unsigned char nlayers;
		unsigned char nlayerInfo[4];//每层对应的纹理ID
		unsigned char nlayerUVInfo[4];
		int ngridx,ngridy;
		int ngridVecNum;
		float  gridsize;
		int shadowmap_size;
		int alphamap_size;
		unsigned char has_water;
		unsigned char has_shadow;
		unsigned char pad[2];
		unsigned char visible;
		Vector3 minPos,maxPos;
	};

	struct BlockVertex
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 texcoord;
		unsigned int  color;
		//unsigned char blendalpha[4];
		//Vector2 texcoord2;
	};

	struct BlockFogVertex
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 texcoord;
		unsigned int  color;
		float   state;
	};	
	struct TerrainBlockLiquidData
	{
		int type;
		char name[256];
		float waterheight;
		ColourValue shallowcolor;
		ColourValue deepcolor;
		float shallowdepth;
		float deepdepth;
		float maxdepth;
		float texrepeat;
		float speed;
		float amp;
	};

	struct TerrainBlockOptionData
	{
		bool       bCull;
		int        nShowLayer;
	};

	class _OgreExport AlphaTexture : public Texture
	{
		DECLARE_RTTI(AlphaTexture)
	public:
		AlphaTexture();
		AlphaTexture(uint width, uint height, uint nchannel);
		~AlphaTexture();

		virtual void getDesc(TextureDesc &desc);
		virtual HardwareTexture *getHardwareTexture();
		virtual void *lock(size_t face, size_t level, bool readonly, LockResult &lockresult)
		{
			return NULL;
		}

		virtual void unlock(size_t face, size_t level)
		{
		}

		void setPixel(int x, int y, uchar value, int channel);
		uchar getPixel(int x, int y, int channel);
		void setPixels(uchar *pbits[]);
		void setPixels(int channel, uchar *pbits);

		uint getNumChannel()
		{
			return m_nChannel;
		}

		void setNumChannel(uint nchannel);

	private:
		virtual void _serialize(Archive &ar, int version);

		uint m_Width;
		uint m_Height;
		uint m_nChannel;
		bool m_bDirty;
		HardwareTexture *m_pTexture;

		uchar *m_pBits[4];
	};

	class _OgreExport WaterDepthTexture : public TextureData
	{
		DECLARE_RTTI(WaterDepthTexture)
	public:
		WaterDepthTexture()
		{
		}
		WaterDepthTexture(TerrainBlockSource *pblock);
		~WaterDepthTexture();

	private:
		TerrainBlockSource *m_pBlockSource;

		void calTextureData();
	};

	class _OgreExport TerrainBlockSource : public Resource
	{
		DECLARE_RTTI(TerrainBlockSource)
	public:
		TerrainBlockSource();
		~TerrainBlockSource();

		void initVertexData(int detail_level);
		bool createTextures(TerrainTileSource* ptile);

		Vector3 getGridPos(int mx ,int my);
		bool getHeight(float x, float z , float* height0, int *pGridInfo=NULL, Vector3 *pnormal=NULL, float *waterheight=NULL);

		virtual void _serialize(Archive &ar, int version);

	public:
		TerrainBlockDesc m_Desc;

		std::vector<float>m_Heights;
		std::vector<Vector3>m_Normals;
		std::vector<ColorQuad>m_VertColors;
		std::vector<BlockVertex>  m_VecData;

		TextureData *m_pTerrainTex[TERRAIN_TEX_NUM];
		AlphaTexture *m_pAlphaMap;
		TextureData *m_pLightMap;
		WaterDepthTexture *m_pWaterDepthMap;

		TerrainBlockLiquidData  m_LiquidData;	
		BoxBound   m_AABBox;
		std::vector<unsigned char> m_DrawInfo;

		bool m_bVertexInit;
		bool m_bTextureInit;
	};

	struct TerrainTileDesc
	{
		unsigned char  tileX,tileY;
		unsigned char  gridX,gridY;
		unsigned char  gridVecNum;
		unsigned char  gridsign;
		float          gridSize;
		unsigned short texFileNum;
		unsigned int   ModelFileNum;
		unsigned int   ModelNum;
		int            PlantNum;
		int			drawSize;
		int			linkInfoNum;
	};

	struct TileModelData
	{
		BoxBound      box;
		SphereBound   ball;
		Vector3   pos;
		Vector3   dir;
		float     scale;
		int       gridX,gridY;
		int       modelID;
		unsigned char type;
		bool      reflected; //属于被反射物件
		bool      refracted; //属于被折射物件
		bool	  transable; //属于可透明物件
		int          userdata;
	};

	enum  TerrainModelType
	{
		TERR_BIG_MODEL = 0 ,
		TERR_SMALL_MODEL , 
		TERR_DETAIL_MODEL , 
		TERR_PLANT_MODEL ,
		TERR_PLANT_GRASS,
		TERR_LINK_MODEL ,
		TERR_LINK_TEXTURE ,
		TERR_PAINT_TEXTURE,
		TERR_LINK_WATER_MODEL ,
		TYPE_PHYSIC_MODEL,
		TERR_GLOBAL_MODEL,
		TERR_MODEL_NUM,
	};

	class _OgreExport TileModel
	{
	private:
		TileModelData    m_ModelData;
		Model*     m_pMeshSetNode;

	public:
		TileModel();
		~TileModel();

		TileModelData*   getModelData()
		{
			return &m_ModelData;
		}

		void serialize(Archive &ar, int tile_version);
	};

	struct TerrainLinkMeshData
	{
		float           fHeight;
		unsigned int    ulColor;
	};

	struct  TerrainTileData
	{
		std::vector <std::string>           m_texFileSet;
		std::vector <std::string>           m_modelFileSet;

		std::vector <TileModel*>            m_pModelSet;
		std::vector <TileModel*>            m_pPlantModelSet;
		std::vector <TerrainBlockSource*>   m_pBlockSet;
		std::vector <TerrainLinkMeshData>                m_LinkMeshData;
	};

	class PhysicsScene2;
	class _OgreExport TerrainTileSource : public Resource
	{
		DECLARE_RTTI(TerrainTileSource)

	public:
		TerrainTileSource();
		~TerrainTileSource();

		virtual void _serialize(Archive &ar, int version);

		virtual WPOS_T getLengthX() ;
		virtual WPOS_T getLengthZ() ;
		virtual bool  getHeight(WPOS_T x, WPOS_T z, WPOS_T *height) ;		
		virtual bool  getHeight(float x,float z,float *height, Vector3 *pnormal=NULL, float *waterheight=NULL);

		virtual bool   getStartPos(Vector3* pos); 

		virtual Vector3  getGridPos( int nx ,int ny );

		TerrainBlockSource *getBlockData(size_t i)
		{
			return m_TileData.m_pBlockSet[i];
		}

		// 构建海浪网格
		void buildBeachMeshes();
		// 绘制海浪网格，临时调试用
		void renderBeachMeshes();

		void initVertexData();

		Resource *loadModel(int id);
		int loadModelPath(int id, TerrainModelType modeltype, FixedString &str);

		void buildPhysicsScene2 (PhysicsScene2 *scene);

	public:
		TerrainTileDesc			m_TileDesc;
		TerrainTileData			m_TileData;
		std::list<Vector3>		m_listBeachPoints;
		std::list<Vector3>		m_vecBeaches[100];
		std::list<Vector3>		m_vecBeachesNormal[100];
		size_t					m_nNumBeaches;
	};
}

#endif