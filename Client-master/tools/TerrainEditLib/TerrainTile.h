
#pragma once

#include "TerrainBlock.h"

#include "WireframeBox.h"


#include <vector>
#include <string>

using namespace std;

namespace TE
{

#pragma pack(push,1)
	//TILE头结构，用于保存，加载
	struct MTileHead
	{
		unsigned char  tileX,tileY;//TILE在世界空间的坐标
		unsigned char  gridX,gridY;//TILE的格子数
		unsigned char  gridVecNum;//每个格子的顶点数
		unsigned char  gridsign;//格子的标记
		float          gridSize;//格子对应的大小
		unsigned short texFileNum;//有多少地表纹理贴图
		unsigned int   MeshFileNum;//有多少模型文件
		unsigned int   MeshNum;//多少模型
		int            PlantNum;//多少植物
	};
	//纹理文件,用于保存加载
	struct MTileTexture
	{
		char filename[256];
	};
	//模型数据，用于保存加载
	struct MTileMesh_101
	{
		Ogre::Vector3 pos;//位置
		Ogre::Vector3 rot;//旋转
		float scale;//缩放
		unsigned short meshfileID;//模型文件ID
		unsigned char type;//类型
		unsigned char gx,gy;//对应哪个格子
		int          unknow;//预留
		int          index;//索引
	};
	struct MTileMesh_102
	{
		Ogre::Vector3 pos;//位置
		Ogre::Vector3 rot;//旋转
		float scale;//缩放
		unsigned short meshfileID;//模型文件ID
		unsigned char type;//类型
		unsigned char gx,gy;//对应哪个格子
		int          unknow;//预留
		int          index;//索引
		bool  reflected;
		bool  refracted;
	};
	struct MTileMesh
	{
		Ogre::Vector3 pos;//位置
		Ogre::Vector3 rot;//旋转
		float scale;//缩放
		unsigned short meshfileID;//模型文件ID
		unsigned char type;//类型
		unsigned char gx,gy;//对应哪个格子
		int          unknow;//预留
		int          index;//索引
		bool  reflected;
		bool  refracted;
		bool  transable;
	};
#pragma pack(pop)

	//模型数据
	class EditorModel;
	struct EditOptionWater;

	struct TerrModelData
	{
		TerrainBox box;
		TerrainSphere sphere;
		char    name[NAME_LENGTH];//模型文件
		Ogre::Vector3   pos;//位置
		Ogre::Vector3   dir;//方向
		float   scale;//缩放
		Ogre::Matrix4  mat;//矩阵
		int     gridX,gridY;//格子坐标（X，Y）

		EditorModel *   pModel;//模型
		int      modelID;//模型ID
		WireframeBox    *m_pWireBox;//包围盒
		bool          bDrawBox;//是否绘制包围盒
		unsigned  char    cType;//类型
		bool           bShow;//是否显示
		bool           bInit;//是否处始化
		int           unknow;//保留
		int            mapIndex;//地图索引
		bool reflected;
		bool refracted;
		bool transable;
	};

	struct LightInfo;
	//tile模型
	struct EditOptionWater;
	class TerrModel
	{
	public:
		TerrModel();
		~TerrModel();

		void onCull();
		//初始化
		void Init();
		//设置模型 
		bool SetShowModel( Ogre::Entity  * pModel );
		//重置矩阵
		void ResetMat();
		//重置BOX
		void ResetBox();
		//是否显示
		void SetShow(bool show );
		//更新
		void Update(float time);
		//设置半透明
		void  SetTransparent(float alpha);
		//使模型进入渲染
		void  AttachRoom();
		void AddToPhysicsScene2();
		//使模型退出渲染
		void  DetachRoom();

		TerrModelData*   GetModelData(){return &m_ModelData;}
		//测试是否与射线相交
		bool  TestIntersect( Ogre::Vector3 pos , Ogre::Vector3 dir );
		//更新当前位置
		void  UpdatePos();
		//计算模型顶点颜色
		void  CountLightColor( LightInfo &lightInfo );
		//计算顶点光照颜色；参数（ 灯光位置，灯光颜色，灯光范围 ）
		void   CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);
		//清除顶点光颜色
		void  ClearLightColor( );
	private:
		//模型数据
		TerrModelData    m_ModelData;
		Ogre::GameScene *m_pAttachedScene;
		void *m_PhyModelHandle;
	};

	//TILE地表纹理数据
	struct TileTexData
	{
		std::vector <std::string>   texFileSet;
	};

	//tile模型数据
	struct TileModelsData
	{
		std::vector <std::string>   modelFileSet;
		std::vector <TerrModel*>	modelSet;
		//新增加植被集合
		std::vector <TerrModel*>    modelPlantSet;
	};

	//tile总数据
	struct MTileData
	{
		TileTexData                 texData;				//地表纹理数据
		TileModelsData              modelData;				//模型数据
		int                         gw,gh;					//TIEL格的宽度和高度

		TerrainBox					box;
		TerrainSphere				sphere;
		bool                        bShowChunk;				//是否显示CHUNK
		bool                        bShowObject;			//是否显示物件
		bool                        bShowObjectBox;			//是否实现物件BOX
		bool						bShowQuadPath;			//是否显示每个块线框
		bool						bShowWater;				//是否显示液体
		bool						bShowFog;				//是否显示雾
		EditorTerrainBlock          chunkSet[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];//地形块数据
	};

	class EditorTerrainTile
	{
	public:
		EditorTerrainTile(void);
		~EditorTerrainTile(void);

		void update(unsigned int dtick);
		//加载文件； 参数（文件名）；返回（为真表示成功）
		virtual  bool LoadFile(const char* filename);
		//释放
		virtual  void Release() ;
		//获得高度；参数（ X，Z， 返回高度 ） ；返回（ 为真表示成功 ）
		virtual  bool GetHeight(float x,float z,float* height, bool use_vb) ;
		// 获得水面高度
		virtual  bool GetWaterHeight(float x, float z, float* height) ;
		//获得高度扩展；参数（ X，Z， 返回高度 ） ；返回（ 为真表示成功 ）
		bool GetHeightEx(float x, float z ,float* height );
		//得到法线；参数（ X，Z ，返回法线 ） ； 返回（ 位置表示成功 ）
		bool GetNormal(float x, float z ,Ogre::Vector3* normal);

		MTileData* GetTitleData(){return &m_TitleData;};
		MTileHead* GetHeadData(){return &m_TileHead;};
		//得到格子X ；参数（ x位置, 格子x ) ；返回（ 为真表示成功 ）
		bool   GetGridX(float posx,int *gx);
		//得到格子Z ；参数（ z位置, 格子z ) ；返回（ 为真表示成功 ）
		bool   GetGridZ(float posz,int *gz);
		//设置CHUNK显示层；参数（ 层数 ）
		void    SetChunkShowLayer(int layer);
		//创建数据
		bool    CreateData();
		//保存文件；参数（ 文件名） ；返回（ 为真表示成功 ）
		bool    SaveFile(const char* name);
		bool    saveMapFile(const char* name);
		bool	BuildPhysicsScene2 ();

		//引导ADT文件；参数（ 文件名）；返回（ 为真表示成功）
		bool    loadMapFile(const char* name);
		//引导植物文件；参数（ 文件名）；返回（ 为真表示成功）    
		bool    LoadPlantFile(const char* name);
		bool    SavePlantFile(const char* name);
		//得到TILE数据
		MTileData*   GetTileData(){return &m_TitleData;}
		//测试与TILE相交
		bool    InsterectRay(Ogre::Vector3 dir , Ogre::Vector3 pos , Ogre::Vector3 &reslut );
		//设置环绕次数；参数（ 次数 )
		void   SetRepeat(int nRepeat);
		//更新环绕次数；参数（ 次数 ）
		void   UpdateRepeat( int nRepeat );
		//更新包围盒
		void   UpdateBox();
		//包围盒信息
		Ogre::Vector3 m_minPos,m_maxPos;

		//更新地形的半透明纹理；参数（ x , y )
		void UpDateTerrainAlphaTex(int x ,int y);
		//获得TILE的头部信息
		MTileHead* GetTileHead(){ return &m_TileHead;}

	private:
		//创建模型
		bool CreateModel();
		//创建渲染数据
		bool CreateChunkRenderData(int nWidth,int nHeight);

	private:
		MTileData m_TitleData;				// TILE的数据
		MTileHead m_TileHead;				// TILE的头信息
		float m_MapScale;					// 老的地图使用不同的地图大小， 需要放缩到新的大小
	private:
		//提供引擎使用的数据
		//地形块数据
		Ogre::TerrainBlock*          m_pBlock[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];
		//液体数据
		Ogre::TLiquid *              m_pLiquid[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];
		//植物数据
		typedef std::map<std::string ,Ogre::PlantNode *>   PLANT_NODE_SET;
		PLANT_NODE_SET               m_pPlantNodeSet[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];
	};

};
