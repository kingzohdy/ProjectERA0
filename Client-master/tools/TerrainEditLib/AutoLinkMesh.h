
#pragma once
#include "gtitle.h"
#include <string>
#include <deque>

namespace TE
{
#define MAP_U_SIGN   (0x01)
#define MAP_L_SIGN   (0x01)<<1
#define MAP_C_SIGN   (0x01)<<2
#define MAP_R_SIGN   (0x01)<<3
#define MAP_D_SIGN   (0x01)<<4
#define MAP_RU_SIGN  (0x01)<<5
#define MAP_LU_SIGN  (0x01)<<6
#define MAP_RD_SIGN  (0x01)<<7
#define MAP_LD_SIGN  0x0f

#define MAP_CENTER_L_U   MAP_C_SIGN | MAP_R_SIGN | MAP_D_SIGN
#define MAP_CENTER_R_U   MAP_C_SIGN | MAP_L_SIGN | MAP_D_SIGN
#define MAP_CENTER_C_U   MAP_C_SIGN | MAP_L_SIGN | MAP_D_SIGN | MAP_R_SIGN 

#define MAP_CENTER_C_C   MAP_C_SIGN 
#define MAP_CENTER_L_C   MAP_C_SIGN | MAP_U_SIGN | MAP_D_SIGN | MAP_R_SIGN
#define MAP_CENTER_R_C   MAP_C_SIGN | MAP_U_SIGN | MAP_D_SIGN | MAP_L_SIGN

#define MAP_CENTER_L_D   MAP_C_SIGN | MAP_U_SIGN | MAP_R_SIGN
#define MAP_CENTER_C_D   MAP_C_SIGN | MAP_U_SIGN | MAP_R_SIGN | MAP_L_SIGN 
#define MAP_CENTER_R_D   MAP_C_SIGN | MAP_U_SIGN | MAP_L_SIGN

#define MAP_RU_LD        MAP_RU_SIGN | MAP_LD_SIGN  
#define MAP_RD_LU        MAP_LU_SIGN | MAP_RD_SIGN 

	//标记类型
	enum SIGN_TYPE
	{
		SIGN_TYPE_UNKOWN = -1, 
		SIGN_TYPE_L_U = 0 , 
		SIGN_TYPE_R_U ,
		SIGN_TYPE_C_U ,
		SIGN_TYPE_C_C ,
		SIGN_TYPE_L_C ,
		SIGN_TYPE_R_C ,
		SIGN_TYPE_L_D ,
		SIGN_TYPE_C_D ,
		SIGN_TYPE_R_D ,
		SIGN_TYPE_LD_RU,
		SIGN_TYPE_RD_LU,
		SIGN_TYPE_NUM,
	};

	struct LinkMeshInfo
	{
		int modelID ;
		TerrModel* pModel;
		float  fMoveHeight;
	};

	struct LinkMeshCullInfo
	{
		unsigned char  info[4];
	};

	struct LinkMapSaveData
	{
		int index;
		int sign;
	};

	struct CurrLinkMapSaveData
	{
		LinkMapSaveData   saveData[4];
	};

	class LinkMeshManager
	{
	public:
		LinkMeshManager();
		virtual ~LinkMeshManager();

		void  Init(int unitsize, MPoint2D nblock, MPoint2D ngrid);//unitsize=1,2一个拼接单位对应几格地图， 地图有nblock块, 每个block有ngrid格
		void    Release();

		void  SetMapSign( int x,int y, unsigned char sign );
		virtual  void  SetMapSign(MVec3 pos , unsigned char sign );

		unsigned char*  GetLinkMap(){return m_LinkMap ;}

		void  Destory();

		void  UpdateLinkMap(int x,int y ,int width ,int height );

		SIGN_TYPE   GetMapSignType( int x, int y );
		const char* GetMapSingFileName(int x,int y ,bool btest, bool *bUpdate);

		bool        TestLinkMesh( std::string &filename );
		bool        SetSelectLinkMeshName( std::string &filename);

		virtual    void    UpdateChunkHeight( int x, int y );

		//根据信息，裁减，对应的格子
		virtual    void     UpdateCullMap(int x,int y );


		virtual    void     UpdatePos( int x,int y ,int width ,int height );

		void        SaveLinkMap(const char* filename);
		virtual      bool        LoadLinkMap(const char* filename);

		//后退操作
		void        UnDo();

		void        SaveUpdateLinkMapData( int x ,int y ,int width ,int height );
		bool        DelLinkMesh(int x,int y );

		void        Clear();

		void        SetAuotDelBlock( int nDel) 
		{
			m_nAutoDelBlock = nDel;
		}

		int         GetLinkModelKindID( std::string &name );

		virtual   void       LoadingLinkMesh( int loadingID );
		void       Loading();

		void        SetLinkOder( bool bLink ) { m_bOderLinkMesh = bLink ;}
		bool        GetLinkOder() { return m_bOderLinkMesh ; }

		int getBlockLinkGrid(){ return m_nMapSize/m_nBlock.x;}

	public:
		unsigned char   *m_NewMap;

	protected:
		virtual      bool    UpdateMesh(int x,int y,const char* filename,bool *bUpdate);

		enum SIGN_DIR
		{
			SIGN_UP = 0 ,
			SIGN_LEFT,
			SIGN_CENTER,
			SIGN_RIGHT,
			SIGN_DOWN,
			SIGN_NUM,
		};

		MPoint2D m_nBlock;
		MPoint2D m_nGrid;
		int m_UnitSize;

		//地图大小
		int             m_nMapSize;
		//link数组地图
		unsigned char   *m_LinkMap;
		//使用模型地图数组
		unsigned char   *m_UsedModelMap;
		//标记集合
		unsigned char    m_SignSet[SIGN_NUM];
		//地图标记集合
		unsigned char    m_SignMapSet[SIGN_TYPE_NUM] ;
		//格子的宽度,高度
		float           m_fWidthSize,m_fHeightSize;
		//MESH文件名
		std::string     m_MeshName;
		//保存当前编辑选中的文件
		std::string     m_SaveCurEditMeshName;
		//对应的裁减信息
		typedef   std::map<int , LinkMeshCullInfo>     MESHCULLINFO_MAP;
		MESHCULLINFO_MAP                            m_CullInfoMap;
		//移动的高度
		float                                         m_fMoveHeight;
		//保存MAP的数据
		std::deque<CurrLinkMapSaveData>                     m_SaveMapData;
		//LINK的路径
		std::string                                     m_strLinkPath;
		//自动删除BLOCK
		int                                            m_nAutoDelBlock;
		//模型对应类型
		typedef   std::map < std::string , int >       MODEL_KIND_MAP;
		MODEL_KIND_MAP                                 m_ModelKindMap;
		//是否在引导
		bool                                           m_bLoading;
		//当前引导的ID
		int                                            m_curLoadingID ;
		//向前引导ID
		int                                            m_iLoadingForwardID ;
		//向后引导ID
		int                                            m_iLoadingBackID;
		//是否按0,1,0,1顺序
		bool                                           m_bOderLinkMesh;
	public:
		void                                           SetBeginLoadIDPos( MVec3  pos );
		//当前选择坐标( X,Y )
		int                                             m_nCurSelectX,m_nCurSelectY;
		//自动连接模型数组
		LinkMeshInfo *m_ModelMapInfo;
		//裁减信息数组
		unsigned char    *m_SignCullInfo;
		//已经被加载信息数组
		unsigned char    *m_bLoadedInfo;
	};

	class LinkTextureManager 
		: public LinkMeshManager
	{
	public:	
		LinkTextureManager();
		virtual ~LinkTextureManager(){};

		virtual    void    UpdateChunkHeight( int x, int y );
		//根据信息，裁减，对应的格子
		virtual    void     UpdateCullMap(int x,int y );

		void                 SetTextureID( int nID );
		virtual    void     UpdatePos( int x,int y ,int width ,int height );

		virtual      bool        LoadLinkMap(const char* filename);

		virtual      void        LoadingLinkMesh(int loadingID );

	protected:
		virtual     bool     UpdateMesh(int x,int y,const char* filename ,bool *bUpdate);
		int                  m_nCurTextureID;

		bool       TestIfLinkMesh( int x , int y , std::string *strName ,int *index);
		const char*   GetSignName( int x , int y );
	};

	//主要是针对直接手动拼接地表
	class  LinkOverlayManager
		: public LinkTextureManager
	{
	public:
		LinkOverlayManager();
		virtual ~LinkOverlayManager(){};

		virtual  void  SetMapSign(MVec3 pos , unsigned char sign );
		virtual    void     UpdateCullMap(int x,int y );

		virtual      bool        LoadLinkMap(const char* filename);

	protected:
		virtual     bool     UpdateMesh(int x,int y,const char* filename ,bool *bUpdate);

	};
}

