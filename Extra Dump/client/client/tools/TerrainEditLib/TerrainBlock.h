
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
	//CHUNKҺ������
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
		TerrainVertex		vecData[TERRBLOCK_NUMVERT0];//������
		int					vecNum;//������Ŀ

		int					texNum;//������
		int					texInfo[TERRAIN_TEX_NUM];	//������Ϣ
		float				texUVInfo[TERRAIN_TEX_NUM];

		int					gx,gy;
		bool				bHaswater;//�Ƿ���ˮ

		bool				bShadow;//�Ƿ�����Ӱ
		unsigned char		shadowMap[64*64];//��Ӱ����

		struct ALPHAMAP
		{
			unsigned char data[64*64];
		};
		ALPHAMAP            alpahMap[3];
		unsigned char       baseMap[64*64];

		bool                bColor;//�Ƿ�����ɫ����
		unsigned char		ColorMap[4*64*64];//��ɫ��������
		unsigned char		ColorAlpha[64*64];//��ɫ�����͸��

		Ogre::Vector3       maxVec,minVec;//CHUNK�İ�Χ������
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
		Ogre::Texture *			terrainTex[TERRAIN_TEX_NUM]; //�ر�����ָ�룬Ŀǰ4��
		int						terrainTexID[TERRAIN_TEX_NUM]; //�ر�����ID��Ŀǰ4��

		Ogre::Texture *			alphaTex[TERRAIN_ALPHATEX_NUM]; //ALPHA����ָ�룬Ŀǰ3��
		int						alphaTexID[TERRAIN_ALPHATEX_NUM]; //ALPHA����ID��Ŀǰ3��
		Ogre::AlphaTexture *	alphatexture; //�ϲ���alpha texture

		//�ϲ�����
		Ogre::Texture *			mterrainAlphaTex;//�ϲ�����

		Ogre::Texture *			shadowTex;//��Ӱ����
		int						shadowTexID;//��ӦID 

		Ogre::Texture *			colorTex;//��ɫ����
		int						colorTexID;//��ӦID

		Ogre::VertexData *		vb;//����
		Ogre::IndexData *		ib[2];//����������Ŀǰ��ʱ�����㡣��1���������Ƶ��Σ���0����������mask

		Ogre::VertexDeclHandle  pLayout;
		int						triNum;//��������

		Ogre::Material			*pmtl;
		Ogre::Material			*pcolormaskmtl;
		int						mtlflags;
	};

	struct TerrBlockPlantData
	{
		unsigned char          plantIndex;
		std::vector <Ogre::Vector3>    vecSet;
	};

	// ���ο���Ϣ
	struct TerrBlockOption
	{
		int  showLayer;
		int  drawMode;
		bool  bCull;//�Ƿ񱻼���
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

	//CHUNK�������ͻ��Ƽ���
	struct MChunkRenderPassSet
	{
		std::vector <MChunkRenderPass>    passSet;
		std::vector<unsigned char>        bDraw;
	};

	//CHUNKֲ���ļ�ͷ
	struct MChunkPlantHead_st
	{
		int  numKind;
	};

	//CHUNK��ֲ����Ϣ
	struct MChunkPlantInfo_st
	{
		char plantFileName[128];
		int  numPos;
	};

#pragma pack(pop)
	//CHUNK��״̬
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

		//�ͷ�
		virtual  void Release() ;
		//������Ⱦ��������
		void CreatePass( );

		Ogre::IndexData *createIndexBuffer(int layer);
		//������Ⱦ����
		void CreateRenderPass();
		//������Ⱦ������չ
		void CreateRenderPassEx();
		//���òü���Ϣ
		void SetCullInfo(int x,int z,unsigned char bCull , int mode = 0 );
		//���òü���Ϣ��չ��
		bool SetCullInfoEx(int x,int z, unsigned char bCull ,bool bUpdate = true , int mode = 0);
		//�õ��߶�
		virtual  bool GetHeight(float x,float y,float* height, bool use_vb);
		virtual bool GetWaterHeight (float x, float y, float* height);
		//�õ��߶���չ
		bool GetHeightEx(float x,float y,float* height);
		//�õ�����
		bool GetNormal(float x , float y , Ogre::Vector3* normal);
		//�Ͼ�ȷ�ཻ
		//�õ�CHUNK����;����( ����ָ��)
		TerrBlockData*  GetChunkData(){return &m_ChunkData;};
		//�õ���Ⱦ����;����( ��Ⱦ����ָ��)
		TerrBlockRenderData* GetChunkRenderData(){return &m_ChunkRenderData;}
		//�õ���Ⱦѡ��;����( ��Ⱦѡ��ָ��)
		TerrBlockOption*      GetChunkOptionData(){return &m_ChunkOptionData;}
		//�õ�Һ��; ����( Һ��ָ��)
		Ogre::TLiquid*               GetChunkLiquid(){return m_pLiquid;}
		//�õ�Һ������;����( Һ������ָ��)
		TerrBlockLiquidData*           GetLiquidData(){return &m_LiquidData;}
		//�õ���Ⱦ����;����( ��Ⱦ����ָ��)
		MChunkRenderPassSet*   GetRenderPassSet(){return &m_RenderPassSet;}
		//�õ�CHUNK�߿�;����( �߿�ָ��)
		MapPath*                 GetQuadLine(){return m_pQuadPath;}
		//�õ�CHUNK��Ϣ����;����( ��Ϣ����ָ��)
		TerrBlockLayerInfo*        GetChunkInfo(){return &m_InfoData;}
		//�õ��µĶ�������;����( ��������ָ��)
		TerrBlockVertTable*      GetChunkNewVecData(){return &m_NewVecData;}
		//�õ���Ӧ�Ķ�����Ϣ;����( mx , my ) ;����( �������� )
		Ogre::Vector3                 GetChunkVec( int mx ,int my );
		//����ֲ��������ļ�����λ�ã����ţ�;����( Ϊ���ʾ�ɹ�)
		bool                  AddPlant(const char* filename , Ogre::Vector3  pos , float fScale ,unsigned int color = 0 );
		//ɾ��ֲ������� λ�ã���Χ��;����( Ϊ���ʾ�ɹ�)
		bool                  DelPlant(Ogre::Vector3 pos , float fSize );
		//���ø����Ƿ���ˮ�������� С��������x , y );����(Ϊ���ʾ�ɹ�)
		bool                  GetGridIfWater( int mx , int my );
		//���õƹ���ɫ
		void                  SetLightColor( unsigned int color );
		//�����Դ��ɫ�������� �ƹ����ͣ��ƹ�λ�û��߷��򣬵ƹ���ɫ����������ɫ���ƹⷶΧ����ӰŨ�ȣ�
		void                  CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);//lighttype=0:pointlight, 1:dirlight
		//����ֲ��Ķ������
		void                  UpdatePlantLightColor();
		void				  UpdatePlantHeight();
	public:
		//���������ƴ��������������� ��
		void   UpdateTexRepeat( int nRepeat );
		//������Ⱦ�߶�
		void   UpdateRenderHeight();

		//������ɫMAP
		void   UpdateColorMap();
		//������ɫMAP
		void   CreateColorMap();
	public:
		//�õ�X�����ڸ��ӵ�ƫ��;����( �������� X ) ;����( �������ƫ�� )
		float GetGridPosX(float x);
		//�õ�X�����ڸ��ӵ�ƫ��;����( �������� Y ) ;����( �������ƫ�� )
		float GetGridPosY(float y);
		//�õ�X�����ڸ��ӵ�ƫ��;����( �������� Z ) ;����( �������ƫ�� )
		float GetGridPosZ(float z);
		//��Ӧ��Χ��
		Ogre::BoxBound               m_AABBox;
	public:
		//ֲ������
		std::vector < TerrModel*>      m_PlantSet;
		//ģ������
		std::vector < TerrModel*>      m_ModelSet;
	public:
		//CHUNK����
		TerrBlockData          m_ChunkData;
		//��Ⱦ����
		TerrBlockRenderData    m_ChunkRenderData;
		//ѡ������
		TerrBlockOption    m_ChunkOptionData;
		//Һ��
		Ogre::TLiquid*            m_pLiquid;
		//Һ������
		TerrBlockLiquidData         m_LiquidData;
		//CHUNK�ı߿�
		MapPath              *m_pQuadPath;
		//CHUNK��Ϣ����
		TerrBlockLayerInfo           m_InfoData;
		//��Ⱦ����
		MChunkRenderPassSet       m_RenderPassSet;
		//�¶����ʽ����
		TerrBlockVertTable                m_NewVecData;
	public:
		//chunk��״̬
		CHUNK_STATUS               m_ChunkStatus;
	public:
		//���µ�ͼ��ϢΪ��ɫͼ
		void    UpdateMapInfoToColorMap();
		//���µ�ͼ��ϢΪ��ɫͼ �������� ��Ӧ�㣩
		void    UpdateMapInfoToColorMap(int layer);
	public:
		//���¿���߿�
		void    UpdateQuadPath();
		//��������
		bool    CreateVecData();	
		//�����Ӷ����ʽ
		bool    CreateOtherVecData();
		//�����ر�����
		bool    CreateTextureData(EditorTerrainTile* _title);
		//ֲ������
		std::vector <TerrBlockPlantData*>         m_ChunkPlantData;   
	public:
		void createOrUpdateDepthTexture(Ogre::TLiquid *pliquid);
		//����Һ����ɫ
		void  SetLiquidColor(float r,float g,float b,float a);
		//����Һ�壻������ �ļ������߶� �������أ� Ϊ���ʾ�ɹ� ��
		bool UpdateLiquid(EditOptionWater &water);

		//����Һ��߶ȣ������� �߶ȡ���
		void  SetLiquidHeight(float height);
		bool  CreateRenderData(EditorTerrainTile* _title);
		bool  CreateQuadPath();

	public:
		//���¶�Ӧ���Ӹ߶�
		void  SetGridHeight(int mx ,int my , float height );
	public:
		//����ֲ���ļ��������� �ļ����������أ�Ϊ���ʾ�ɹ���
		bool  SavePlantFile(const char* filename);
		//����ֲ���ļ��������� �ļ����������أ� Ϊ���ʾ�ɹ���
		bool  LoadPlantFile(const char* filename);
		//ÿ��ֲ���Ӧ��MAP
		typedef std::map<std::string , GrassGroup*>       PLANT_MAP;
		PLANT_MAP                                        m_PlantMap; 

		Ogre::TextureData *m_pLightMap;
		std::string m_LightMapTexturename;

	};
}
