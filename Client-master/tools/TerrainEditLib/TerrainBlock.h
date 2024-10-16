
#pragma once
#include "TerrainEditTypes.h"
#include "MapPath.h"
#include "MapGrass.h"
#include "TerrainGeom.h"

#include <vector>
#include <string>


namespace TE
{
#pragma pack(push,1)
	struct TerrBlockHeader
	{
		unsigned char  gx,gy;
		unsigned char  texNum;
		unsigned char  texInfo[TERRAIN_TEX_NUM];
		unsigned char  vecNum;
		unsigned char  bHasLiquid;
		unsigned char  bShadowMap;
		Ogre::Vector3          MinVec;
		Ogre::Vector3          MaxVec;
		unsigned char  bColorMap;
		unsigned char  texUVInfo[TERRAIN_TEX_NUM];
	};
	//CHUNK液体数据
	struct TerrBlockLiquidData
	{
		int type;
		float waterheight;

		char name[256];
		Ogre::ColourValue shallowcolor;
		Ogre::ColourValue deepcolor;
		float shallowdepth;
		float deepdepth;
		float maxdepth;
		float texrepeat;
		float speed;
		float amp;
	};

	struct TerrBlockData
	{
		TerrainVertex		vecData[TERRBLOCK_NUMVERT0];//顶点数
		int					vecNum;//顶点数目

		int					texNum;//纹理数
		int					texInfo[TERRAIN_TEX_NUM];	//纹理信息
		float				texUVInfo[TERRAIN_TEX_NUM];

		int					gx,gy;
		bool				bHaswater;//是否有水

		bool				bShadow;//是否有阴影
		unsigned char		shadowMap[64*64];//阴影数据

		struct ALPHAMAP
		{
			unsigned char data[64*64];
		};
		ALPHAMAP            alpahMap[3];
		unsigned char       baseMap[64*64];

		bool                bColor;//是否有颜色纹理
		unsigned char		ColorMap[4*64*64];//颜色纹理数据
		unsigned char		ColorAlpha[64*64];//颜色纹理半透明

		Ogre::Vector3       maxVec,minVec;//CHUNK的包围盒数据
	};

	struct TerrBlockVertTable
	{
		int  ngrid;
		std::vector<TerrainVertex> vVer;
	};

	struct TerrBlockLayerInfo
	{
		int           nSelectMode;
		unsigned char color[4][4];
		unsigned char info[4][64*64];
	};

	struct TerrBlockRenderData
	{
		Ogre::Texture *			terrainTex[TERRAIN_TEX_NUM]; //地表纹理指针，目前4层
		int						terrainTexID[TERRAIN_TEX_NUM]; //地表纹理ID，目前4层

		Ogre::Texture *			alphaTex[TERRAIN_ALPHATEX_NUM]; //ALPHA纹理指针，目前3层
		int						alphaTexID[TERRAIN_ALPHATEX_NUM]; //ALPHA纹理ID，目前3层
		Ogre::AlphaTexture *	alphatexture; //合并的alpha texture

		//合并纹理
		Ogre::Texture *			mterrainAlphaTex;//合并纹理

		Ogre::Texture *			shadowTex;//阴影纹理
		int						shadowTexID;//对应ID 

		Ogre::Texture *			colorTex;//颜色纹理
		int						colorTexID;//对应ID

		Ogre::VertexData *		vb;//顶点
		Ogre::IndexData *		ib[2];//顶点索引，目前暂时用两层。第1层用来绘制地形，第0层用来绘制mask

		Ogre::VertexDeclHandle  pLayout;
		int						triNum;//三角形数

		Ogre::Material			*pmtl;
		Ogre::Material			*pcolormaskmtl;
		int						mtlflags;
	};

	struct TerrBlockPlantData
	{
		unsigned char          plantIndex;
		std::vector <Ogre::Vector3>    vecSet;
	};

	// 地形块信息
	struct TerrBlockOption
	{
		int  showLayer;
		int  drawMode;
		bool  bCull;//是否被剪裁
		bool  bShowShadow;
		bool  bVisible;
		int   nRepeat;
		int   bChangeHeight;
	};

	struct MChunkRenderPass
	{
		int startIdx;
		int numTri;
	};

	//CHUNK的三角型绘制集合
	struct MChunkRenderPassSet
	{
		std::vector <MChunkRenderPass>    passSet;
		std::vector<unsigned char>        bDraw;
	};

	//CHUNK植物文件头
	struct MChunkPlantHead_st
	{
		int  numKind;
	};

	//CHUNK的植物信息
	struct MChunkPlantInfo_st
	{
		char plantFileName[128];
		int  numPos;
	};

#pragma pack(pop)
	//CHUNK的状态
	enum  CHUNK_STATUS
	{
		CHUNK_UNLOAD = 0,
		CHUNK_LOADED ,
		CHUNK_DELETE ,
	};

	class TerrModel;
	struct EditOptionWater;
	class EditorTerrainTile;

	class EditorTerrainBlock : public Ogre::RenderableObject
	{
	public:
		EditorTerrainBlock(void);
		~EditorTerrainBlock(void);

		virtual void attachToScene(Ogre::GameScene *pscene);
		virtual void detachFromScene(Ogre::GameScene *pscene);
		virtual void update(unsigned int dtick);

		void drawChunkMesh(Ogre::ShaderContext *pcontext, int fillmode, int layer);
		virtual void render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);
		virtual void getRenderPassRequired(Ogre::RenderPassDesc &desc);

		//释放
		virtual  void Release() ;
		//创建渲染序列数据
		void CreatePass( );

		Ogre::IndexData *createIndexBuffer(int layer);
		//创建渲染序列
		void CreateRenderPass();
		//创建渲染序列扩展
		void CreateRenderPassEx();
		//设置裁减信息
		void SetCullInfo(int x,int z,unsigned char bCull , int mode = 0 );
		//设置裁减信息扩展；
		bool SetCullInfoEx(int x,int z, unsigned char bCull ,bool bUpdate = true , int mode = 0);
		//得到高度
		virtual  bool GetHeight(float x,float y,float* height, bool use_vb);
		virtual bool GetWaterHeight (float x, float y, float* height);
		//得到高度扩展
		bool GetHeightEx(float x,float y,float* height);
		//得到法线
		bool GetNormal(float x , float y , Ogre::Vector3* normal);
		//较精确相交
		//得到CHUNK数据;返回( 数据指针)
		TerrBlockData*  GetChunkData(){return &m_ChunkData;};
		//得到渲染数据;返回( 渲染数据指针)
		TerrBlockRenderData* GetChunkRenderData(){return &m_ChunkRenderData;}
		//得到渲染选项;返回( 渲染选项指针)
		TerrBlockOption*      GetChunkOptionData(){return &m_ChunkOptionData;}
		//得到液体; 返回( 液体指针)
		Ogre::TLiquid*               GetChunkLiquid(){return m_pLiquid;}
		//得到液体数据;返回( 液体数据指针)
		TerrBlockLiquidData*           GetLiquidData(){return &m_LiquidData;}
		//得到渲染序列;返回( 渲染序列指针)
		MChunkRenderPassSet*   GetRenderPassSet(){return &m_RenderPassSet;}
		//得到CHUNK边框;返回( 边框指针)
		MapPath*                 GetQuadLine(){return m_pQuadPath;}
		//得到CHUNK信息数据;返回( 信息数据指针)
		TerrBlockLayerInfo*        GetChunkInfo(){return &m_InfoData;}
		//得到新的顶点数据;返回( 顶点数据指针)
		TerrBlockVertTable*      GetChunkNewVecData(){return &m_NewVecData;}
		//得到对应的顶点信息;参数( mx , my ) ;返回( 顶点坐标 )
		Ogre::Vector3                 GetChunkVec( int mx ,int my );
		//增加植物；参数（文件名，位置，缩放）;返回( 为真表示成功)
		bool                  AddPlant(const char* filename , Ogre::Vector3  pos , float fScale ,unsigned int color = 0 );
		//删除植物；参数（ 位置，范围）;返回( 为真表示成功)
		bool                  DelPlant(Ogre::Vector3 pos , float fSize );
		//设置格子是否有水；参数（ 小格子坐标x , y );返回(为真表示成功)
		bool                  GetGridIfWater( int mx , int my );
		//设置灯光颜色
		void                  SetLightColor( unsigned int color );
		//计算光源颜色；参数（ 灯光类型，灯光位置或者方向，灯光颜色，环境光颜色，灯光范围，阴影浓度）
		void                  CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);//lighttype=0:pointlight, 1:dirlight
		//更新植物的顶点光照
		void                  UpdatePlantLightColor();
		void				  UpdatePlantHeight();
	public:
		//更新纹理环绕次数；参数（次数 ）
		void   UpdateTexRepeat( int nRepeat );
		//更新渲染高度
		void   UpdateRenderHeight();

		//更新颜色MAP
		void   UpdateColorMap();
		//创建颜色MAP
		void   CreateColorMap();
	public:
		//得到X坐标在格子的偏移;参数( 世界坐标 X ) ;返回( 格子相对偏移 )
		float GetGridPosX(float x);
		//得到X坐标在格子的偏移;参数( 世界坐标 Y ) ;返回( 格子相对偏移 )
		float GetGridPosY(float y);
		//得到X坐标在格子的偏移;参数( 世界坐标 Z ) ;返回( 格子相对偏移 )
		float GetGridPosZ(float z);
		//对应包围盒
		Ogre::BoxBound               m_AABBox;
	public:
		//植物数组
		std::vector < TerrModel*>      m_PlantSet;
		//模型数组
		std::vector < TerrModel*>      m_ModelSet;
	public:
		//CHUNK数据
		TerrBlockData          m_ChunkData;
		//渲染数据
		TerrBlockRenderData    m_ChunkRenderData;
		//选项数据
		TerrBlockOption    m_ChunkOptionData;
		//液体
		Ogre::TLiquid*            m_pLiquid;
		//液体数据
		TerrBlockLiquidData         m_LiquidData;
		//CHUNK的边匡
		MapPath              *m_pQuadPath;
		//CHUNK信息数据
		TerrBlockLayerInfo           m_InfoData;
		//渲染序列
		MChunkRenderPassSet       m_RenderPassSet;
		//新顶点格式数组
		TerrBlockVertTable                m_NewVecData;
	public:
		//chunk的状态
		CHUNK_STATUS               m_ChunkStatus;
	public:
		//更新地图信息为颜色图
		void    UpdateMapInfoToColorMap();
		//更新地图信息为颜色图 ；参数（ 对应层）
		void    UpdateMapInfoToColorMap(int layer);
	public:
		//更新块的线框
		void    UpdateQuadPath();
		//创建顶点
		bool    CreateVecData();	
		//新增加顶点格式
		bool    CreateOtherVecData();
		//创建地表纹理
		bool    CreateTextureData(EditorTerrainTile* _title);
		//植物数据
		std::vector <TerrBlockPlantData*>         m_ChunkPlantData;   
	public:
		void createOrUpdateDepthTexture(Ogre::TLiquid *pliquid);
		//设置液体颜色
		void  SetLiquidColor(float r,float g,float b,float a);
		//创建液体；参数（ 文件名，高度 ）；返回（ 为真表示成功 ）
		bool UpdateLiquid(EditOptionWater &water);

		//设置液体高度；参数（ 高度　）
		void  SetLiquidHeight(float height);
		bool  CreateRenderData(EditorTerrainTile* _title);
		bool  CreateQuadPath();

	public:
		//更新对应格子高度
		void  SetGridHeight(int mx ,int my , float height );
	public:
		//保存植物文件；参数（ 文件名）；返回（为真表示成功）
		bool  SavePlantFile(const char* filename);
		//引导植物文件；参数（ 文件名）；返回（ 为真表示成功）
		bool  LoadPlantFile(const char* filename);
		//每种植物对应的MAP
		typedef std::map<std::string , GrassGroup*>       PLANT_MAP;
		PLANT_MAP                                        m_PlantMap; 

		Ogre::TextureData *m_pLightMap;
		std::string m_LightMapTexturename;

	};
}
