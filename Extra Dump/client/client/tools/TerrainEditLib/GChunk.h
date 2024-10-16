
#pragma once
#include "TerrainEditTypes.h"
#include "mpath.h"
#include "plant.h"
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
		MVec3          MinVec;
		MVec3          MaxVec;
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
		TerrainVertex      vecData[TERRBLOCK_NUMVERT0];//������
		int               vecNum;//������Ŀ

		int              texNum;//������
		int              texInfo[TERRAIN_TEX_NUM];	//������Ϣ
		float            texUVInfo[TERRAIN_TEX_NUM];

		int              gx,gy;
		bool             bHaswater;//�Ƿ���ˮ

		bool             bShadow;//�Ƿ�����Ӱ
		unsigned   char   shadowMap[64*64];//��Ӱ����

		struct ALPHAMAP
		{
			unsigned char data[64*64];
		};
		ALPHAMAP             alpahMap[3];
		unsigned char        baseMap[64*64];

		bool               bColor;//�Ƿ�����ɫ����
		unsigned   char     ColorMap[4*64*64];//��ɫ��������
		unsigned  char     ColorAlpha[64*64];//��ɫ�����͸��

		MVec3                maxVec,minVec;//CHUNK�İ�Χ������
	};

	struct TerrBlockVertTable
	{
		int  ngrid;
		std::vector<TerrainVertex> vVer;
	};

	struct TerrBlockLayerInfo
	{
		int            nSelectMode;
		unsigned char color[4][4];
		unsigned char info[4][64*64];
	};

	struct TerrBlockRenderData
	{
		LMTex   terrainTex[TERRAIN_TEX_NUM];//�ر�����ָ�룬Ŀǰ4��
		int     terrainTexID[TERRAIN_TEX_NUM];//�ر�����ID��Ŀǰ4��

		LMTex   alphaTex[TERRAIN_ALPHATEX_NUM];//ALPHA����ָ�룬Ŀǰ3��
		int     alphaTexID[TERRAIN_ALPHATEX_NUM];//ALPHA����ID��Ŀǰ3��
		Ogre::AlphaTexture *alphatexture; //�ϲ���alpha texture

		//�ϲ�����
		LMTex   mterrainAlphaTex;//�ϲ�����

		LMTex   shadowTex;//��Ӱ����
		int     shadowTexID;//��ӦID 

		LMTex   colorTex;//��ɫ����
		int     colorTexID;//��ӦID

		LMVB    vb;//����
		LMIB     ib[2];//����������Ŀǰ��ʱ������

		MIInputLayout  pLayout;
		int            triNum;//��������

		Ogre::Material *pmtl;
		Ogre::Material *pcolormaskmtl;
		int mtlflags;
	};

	struct TerrBlockPlantData
	{
		unsigned char          plantIndex;
		std::vector <MVec3>    vecSet;
	};

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
	class MTitle;

	class MChunk : public Ogre::RenderableObject
	{
	public:
		MChunk(void);
		~MChunk(void);

		virtual void attachToScene(Ogre::GameScene *pscene);
		virtual void detachFromScene(Ogre::GameScene *pscene);
		virtual void update(unsigned int dtick);

		void drawChunkMesh(Ogre::ShaderContext *pcontext, int fillmode, int layer);
		virtual void render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);
		virtual Ogre::uint getRenderUsageBits();
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
		//�õ��߶���չ
		bool GetHeightEx(float x,float y,float* height);
		//�õ�����
		bool GetNormal(float x , float y , MVec3* normal);
		//�Ͼ�ȷ�ཻ
		//�õ�CHUNK����;����( ����ָ��)
		TerrBlockData*  GetChunkData(){return &m_ChunkData;};
		//�õ���Ⱦ����;����( ��Ⱦ����ָ��)
		TerrBlockRenderData* GetChunkRenderData(){return &m_ChunkRenderData;}
		//�õ���Ⱦѡ��;����( ��Ⱦѡ��ָ��)
		TerrBlockOption*      GetChunkOptionData(){return &m_ChunkOptionData;}
		//�õ�Һ��; ����( Һ��ָ��)
		MLiquid*               GetChunkLiquid(){return m_pLiquid;}
		//�õ�Һ������;����( Һ������ָ��)
		TerrBlockLiquidData*           GetLiquidData(){return &m_LiquidData;}
		//�õ���Ⱦ����;����( ��Ⱦ����ָ��)
		MChunkRenderPassSet*   GetRenderPassSet(){return &m_RenderPassSet;}
		//�õ�CHUNK�߿�;����( �߿�ָ��)
		MPath*                 GetQuadLine(){return m_pQuadPath;}
		//�õ�CHUNK��Ϣ����;����( ��Ϣ����ָ��)
		TerrBlockLayerInfo*        GetChunkInfo(){return &m_InfoData;}
		//�õ��µĶ�������;����( ��������ָ��)
		TerrBlockVertTable*      GetChunkNewVecData(){return &m_NewVecData;}
		//�õ���Ӧ�Ķ�����Ϣ;����( mx , my ) ;����( �������� )
		MVec3                 GetChunkVec( int mx ,int my );
		//����ֲ��������ļ�����λ�ã����ţ�;����( Ϊ���ʾ�ɹ�)
		bool                  AddPlant(const char* filename , MVec3  pos , float fScale ,unsigned int color = 0 );
		//ɾ��ֲ������� λ�ã���Χ��;����( Ϊ���ʾ�ɹ�)
		bool                  DelPlant(MVec3 pos , float fSize );
		//���ø����Ƿ���ˮ�������� С��������x , y );����(Ϊ���ʾ�ɹ�)
		bool                  GetGridIfWater( int mx , int my );
		//���õƹ���ɫ
		void                  SetLightColor( unsigned int color );
		//�����Դ��ɫ�������� �ƹ�λ�ã��ƹ���ɫ���ƹⷶΧ��
		void                  CountLightColor(int lighttype, MVec3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);//lighttype=0:pointlight, 1:dirlight
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
		MAABBox               m_AABBox;
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
		MLiquid*            m_pLiquid;
		//Һ������
		TerrBlockLiquidData         m_LiquidData;
		//CHUNK�ı߿�
		MPath              *m_pQuadPath;
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
		bool    CreateTextureData(MTitle* _title);
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
		bool  CreateRenderData(MTitle* _title);
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
		typedef std::map<std::string , CPlantSet*>       PLANT_MAP;
		PLANT_MAP                                        m_PlantMap; 

	};
}
