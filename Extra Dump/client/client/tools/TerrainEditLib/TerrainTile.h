
#pragma once

#include "TerrainBlock.h"

#include "WireframeBox.h"


#include <vector>
#include <string>

using namespace std;

namespace TE
{

#pragma pack(push,1)
	//TILEͷ�ṹ�����ڱ��棬����
	struct MTileHead
	{
		unsigned char  tileX,tileY;//TILE������ռ������
		unsigned char  gridX,gridY;//TILE�ĸ�����
		unsigned char  gridVecNum;//ÿ�����ӵĶ�����
		unsigned char  gridsign;//���ӵı��
		float          gridSize;//���Ӷ�Ӧ�Ĵ�С
		unsigned short texFileNum;//�ж��ٵر�������ͼ
		unsigned int   MeshFileNum;//�ж���ģ���ļ�
		unsigned int   MeshNum;//����ģ��
		int            PlantNum;//����ֲ��
	};
	//�����ļ�,���ڱ������
	struct MTileTexture
	{
		char filename[256];
	};
	//ģ�����ݣ����ڱ������
	struct MTileMesh_101
	{
		Ogre::Vector3 pos;//λ��
		Ogre::Vector3 rot;//��ת
		float scale;//����
		unsigned short meshfileID;//ģ���ļ�ID
		unsigned char type;//����
		unsigned char gx,gy;//��Ӧ�ĸ�����
		int          unknow;//Ԥ��
		int          index;//����
	};
	struct MTileMesh_102
	{
		Ogre::Vector3 pos;//λ��
		Ogre::Vector3 rot;//��ת
		float scale;//����
		unsigned short meshfileID;//ģ���ļ�ID
		unsigned char type;//����
		unsigned char gx,gy;//��Ӧ�ĸ�����
		int          unknow;//Ԥ��
		int          index;//����
		bool  reflected;
		bool  refracted;
	};
	struct MTileMesh
	{
		Ogre::Vector3 pos;//λ��
		Ogre::Vector3 rot;//��ת
		float scale;//����
		unsigned short meshfileID;//ģ���ļ�ID
		unsigned char type;//����
		unsigned char gx,gy;//��Ӧ�ĸ�����
		int          unknow;//Ԥ��
		int          index;//����
		bool  reflected;
		bool  refracted;
		bool  transable;
	};
#pragma pack(pop)

	//ģ������
	class EditorModel;
	struct EditOptionWater;

	struct TerrModelData
	{
		TerrainBox box;
		TerrainSphere sphere;
		char    name[NAME_LENGTH];//ģ���ļ�
		Ogre::Vector3   pos;//λ��
		Ogre::Vector3   dir;//����
		float   scale;//����
		Ogre::Matrix4  mat;//����
		int     gridX,gridY;//�������꣨X��Y��

		EditorModel *   pModel;//ģ��
		int      modelID;//ģ��ID
		WireframeBox    *m_pWireBox;//��Χ��
		bool          bDrawBox;//�Ƿ���ư�Χ��
		unsigned  char    cType;//����
		bool           bShow;//�Ƿ���ʾ
		bool           bInit;//�Ƿ�ʼ��
		int           unknow;//����
		int            mapIndex;//��ͼ����
		bool reflected;
		bool refracted;
		bool transable;
	};

	struct LightInfo;
	//tileģ��
	struct EditOptionWater;
	class TerrModel
	{
	public:
		TerrModel();
		~TerrModel();

		void onCull();
		//��ʼ��
		void Init();
		//����ģ�� 
		bool SetShowModel( Ogre::Entity  * pModel );
		//���þ���
		void ResetMat();
		//����BOX
		void ResetBox();
		//�Ƿ���ʾ
		void SetShow(bool show );
		//����
		void Update(float time);
		//���ð�͸��
		void  SetTransparent(float alpha);
		//ʹģ�ͽ�����Ⱦ
		void  AttachRoom();
		void AddToPhysicsScene2();
		//ʹģ���˳���Ⱦ
		void  DetachRoom();

		TerrModelData*   GetModelData(){return &m_ModelData;}
		//�����Ƿ��������ཻ
		bool  TestIntersect( Ogre::Vector3 pos , Ogre::Vector3 dir );
		//���µ�ǰλ��
		void  UpdatePos();
		//����ģ�Ͷ�����ɫ
		void  CountLightColor( LightInfo &lightInfo );
		//���㶥�������ɫ�������� �ƹ�λ�ã��ƹ���ɫ���ƹⷶΧ ��
		void   CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);
		//����������ɫ
		void  ClearLightColor( );
	private:
		//ģ������
		TerrModelData    m_ModelData;
		Ogre::GameScene *m_pAttachedScene;
		void *m_PhyModelHandle;
	};

	//TILE�ر���������
	struct TileTexData
	{
		std::vector <std::string>   texFileSet;
	};

	//tileģ������
	struct TileModelsData
	{
		std::vector <std::string>   modelFileSet;
		std::vector <TerrModel*>	modelSet;
		//������ֲ������
		std::vector <TerrModel*>    modelPlantSet;
	};

	//tile������
	struct MTileData
	{
		TileTexData                 texData;				//�ر���������
		TileModelsData              modelData;				//ģ������
		int                         gw,gh;					//TIEL��Ŀ�Ⱥ͸߶�

		TerrainBox					box;
		TerrainSphere				sphere;
		bool                        bShowChunk;				//�Ƿ���ʾCHUNK
		bool                        bShowObject;			//�Ƿ���ʾ���
		bool                        bShowObjectBox;			//�Ƿ�ʵ�����BOX
		bool						bShowQuadPath;			//�Ƿ���ʾÿ�����߿�
		bool						bShowWater;				//�Ƿ���ʾҺ��
		bool						bShowFog;				//�Ƿ���ʾ��
		EditorTerrainBlock          chunkSet[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];//���ο�����
	};

	class EditorTerrainTile
	{
	public:
		EditorTerrainTile(void);
		~EditorTerrainTile(void);

		void update(unsigned int dtick);
		//�����ļ��� �������ļ����������أ�Ϊ���ʾ�ɹ���
		virtual  bool LoadFile(const char* filename);
		//�ͷ�
		virtual  void Release() ;
		//��ø߶ȣ������� X��Z�� ���ظ߶� �� �����أ� Ϊ���ʾ�ɹ� ��
		virtual  bool GetHeight(float x,float z,float* height, bool use_vb) ;
		// ���ˮ��߶�
		virtual  bool GetWaterHeight(float x, float z, float* height) ;
		//��ø߶���չ�������� X��Z�� ���ظ߶� �� �����أ� Ϊ���ʾ�ɹ� ��
		bool GetHeightEx(float x, float z ,float* height );
		//�õ����ߣ������� X��Z �����ط��� �� �� ���أ� λ�ñ�ʾ�ɹ� ��
		bool GetNormal(float x, float z ,Ogre::Vector3* normal);

		MTileData* GetTitleData(){return &m_TitleData;};
		MTileHead* GetHeadData(){return &m_TileHead;};
		//�õ�����X �������� xλ��, ����x ) �����أ� Ϊ���ʾ�ɹ� ��
		bool   GetGridX(float posx,int *gx);
		//�õ�����Z �������� zλ��, ����z ) �����أ� Ϊ���ʾ�ɹ� ��
		bool   GetGridZ(float posz,int *gz);
		//����CHUNK��ʾ�㣻������ ���� ��
		void    SetChunkShowLayer(int layer);
		//��������
		bool    CreateData();
		//�����ļ��������� �ļ����� �����أ� Ϊ���ʾ�ɹ� ��
		bool    SaveFile(const char* name);
		bool    saveMapFile(const char* name);
		bool	BuildPhysicsScene2 ();

		//����ADT�ļ��������� �ļ����������أ� Ϊ���ʾ�ɹ���
		bool    loadMapFile(const char* name);
		//����ֲ���ļ��������� �ļ����������أ� Ϊ���ʾ�ɹ���    
		bool    LoadPlantFile(const char* name);
		bool    SavePlantFile(const char* name);
		//�õ�TILE����
		MTileData*   GetTileData(){return &m_TitleData;}
		//������TILE�ཻ
		bool    InsterectRay(Ogre::Vector3 dir , Ogre::Vector3 pos , Ogre::Vector3 &reslut );
		//���û��ƴ����������� ���� )
		void   SetRepeat(int nRepeat);
		//���»��ƴ����������� ���� ��
		void   UpdateRepeat( int nRepeat );
		//���°�Χ��
		void   UpdateBox();
		//��Χ����Ϣ
		Ogre::Vector3 m_minPos,m_maxPos;

		//���µ��εİ�͸������������ x , y )
		void UpDateTerrainAlphaTex(int x ,int y);
		//���TILE��ͷ����Ϣ
		MTileHead* GetTileHead(){ return &m_TileHead;}

	private:
		//����ģ��
		bool CreateModel();
		//������Ⱦ����
		bool CreateChunkRenderData(int nWidth,int nHeight);

	private:
		MTileData m_TitleData;				// TILE������
		MTileHead m_TileHead;				// TILE��ͷ��Ϣ
		float m_MapScale;					// �ϵĵ�ͼʹ�ò�ͬ�ĵ�ͼ��С�� ��Ҫ�������µĴ�С
	private:
		//�ṩ����ʹ�õ�����
		//���ο�����
		Ogre::TerrainBlock*          m_pBlock[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];
		//Һ������
		Ogre::TLiquid *              m_pLiquid[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];
		//ֲ������
		typedef std::map<std::string ,Ogre::PlantNode *>   PLANT_NODE_SET;
		PLANT_NODE_SET               m_pPlantNodeSet[TERRTILE_NUMBLOCK][TERRTILE_NUMBLOCK];
	};

};
