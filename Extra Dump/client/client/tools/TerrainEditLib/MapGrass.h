
#pragma once

#include <vector>
#include <map>
#include "TerrainEditTypes.h"

namespace TE
{

	struct CPlantRenderData
	{
		Ogre::Texture *    pTex;
		Ogre::VertexData *     pVb;
		Ogre::IndexData *     pIb;
		Ogre::VertexDeclHandle   pLayout;
		Ogre::Material *pmtl;
	};

	struct CPlantVec_st
	{
		Ogre::Vector3  pos;
		unsigned int  color;
		Ogre::Vector2  texcoord;
	};
	//单片植物最多的顶点数
#define   PLANT_VEC_MAX_NUM   4000 
	//单片植物最多的数量
#define   PLANT_MAX_NUM       PLANT_VEC_MAX_NUM / 4
	//植物
	class EditorTerrainBlock;
	class GrassGroup
	{
	public:
		GrassGroup();
		~GrassGroup();

		//初始化扩展
		bool   InitEx_1();
		//释放
		void   Release();
		virtual void render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);
		//渲染
		void   Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);
		//渲染扩展
		void   RenderEx(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);

		//增加位置扩展
		bool   AddPosEx_1(Ogre::Vector3 pos , float fScale = 1.0f , unsigned int color = 0);
		//设置位置
		void   SetPos(Ogre::Vector3 pos);
		//更新数据
		void   UpdateData();
		//更新植物顶点信息扩展
		void   UpdateDataEx(int mode = 0);
		//更新植物顶点信息扩展
		void   UpdateDataEx_1();
		//更新（每次调用）
		void   Update(float  tick);
		//更新扩展
		void   UpdateEx_1(float tick);
		//设置纹理贴图
		bool   SetTexture(const char* filename );
		//设置植物的纹理贴图
		bool   SetTexture(Ogre::Texture *  tex);
		//重置
		void   Reset();
		//删除位置
		void   DelPos( Ogre::Vector3 pos , float fSize );
		//得到当前植物数量
		int    GetCurPlantNum(){return m_nCurVecNum / 4;}
		//清除顶点颜色
		void   ClearLightColor();
		//计算顶点光照颜色；参数（ 灯光位置，灯光颜色，灯光范围 ）
		void   CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);
		//更新灯光颜色
		void   UpdateLightColor(); 

		void	UpdateHeight(EditorTerrainBlock* pChunk);
	public:
		Ogre::Vector3                       m_SavePosSet[PLANT_MAX_NUM];
		unsigned  int               m_unSaveColorSet[PLANT_MAX_NUM];  

		float                       m_SaveScaleSet[PLANT_MAX_NUM];

		Ogre::Vector3                       m_PlayerPos;
		Ogre::Vector3                       m_PosSet[PLANT_VEC_MAX_NUM];

		int                         m_nCurVecNum;
		int                         m_nCurIndexNum;

		CPlantRenderData             m_RenderData;

		float                        m_fHeight,m_fWidth;
		std::string                  m_TextureName;

		Ogre::Vector3                         m_MovePos[4];
	private:
		float m_fMovetime ;
		float m_fMovetime1 ;
		float m_fMovetime2 ;
		float m_fMovetime3 ;

	};
	//植物名
	struct PlantName_t
	{
		char plantFileName[260];
	};
	//植物信息
	struct  MPlantInfo_t
	{
		int          plantKindNum ;
		int          bUserSizeInfo;

		std::vector <PlantName_t>  plantNameSet;
		std::vector <unsigned char>  indexSet;
		std::vector <unsigned char>  randIdx;

		char      plantFileName[260];
		int             width;
		int             height;
		unsigned char    color;
		int            maxScale;
		int            minScale;
		unsigned char    type;
		int              nAutoNormal;
	};
	//纹理文件信息
	struct  MTextureInfo
	{
		char   textureName[260];
	};
	//通过灰度图生成对应的大量植被信息
	class  GrassGroupManager
	{
	public:
		GrassGroupManager();
		~GrassGroupManager(){ Release(); };

		void    Init();
		void    CreateChunkPlant();
		void    UpdateDataPlant();
		void    Render();
		void    Release();

	public:
		std::vector <MPlantInfo_t>     m_PlantInfoSet;
		std::vector < GrassGroup* >    m_PlantManager; 
		//目前支持1024x1024的灰度图控制大规模植物生成
		unsigned char                 m_PlantMap[1024*1024];
		//对应的不同植物信息
		std::vector <MTextureInfo>      m_TextureFileSet;
		//对应植物信息文件
		char                            m_strPlantInfoFile[260];

	};

	extern GrassGroupManager    *g_PlantMgr;
}
