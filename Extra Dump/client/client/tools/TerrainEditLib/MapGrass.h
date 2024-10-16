
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
	//��Ƭֲ�����Ķ�����
#define   PLANT_VEC_MAX_NUM   4000 
	//��Ƭֲ����������
#define   PLANT_MAX_NUM       PLANT_VEC_MAX_NUM / 4
	//ֲ��
	class EditorTerrainBlock;
	class GrassGroup
	{
	public:
		GrassGroup();
		~GrassGroup();

		//��ʼ����չ
		bool   InitEx_1();
		//�ͷ�
		void   Release();
		virtual void render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);
		//��Ⱦ
		void   Render(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);
		//��Ⱦ��չ
		void   RenderEx(Ogre::SceneRenderer *prenderer, const Ogre::ShaderEnvData &envdata);

		//����λ����չ
		bool   AddPosEx_1(Ogre::Vector3 pos , float fScale = 1.0f , unsigned int color = 0);
		//����λ��
		void   SetPos(Ogre::Vector3 pos);
		//��������
		void   UpdateData();
		//����ֲ�ﶥ����Ϣ��չ
		void   UpdateDataEx(int mode = 0);
		//����ֲ�ﶥ����Ϣ��չ
		void   UpdateDataEx_1();
		//���£�ÿ�ε��ã�
		void   Update(float  tick);
		//������չ
		void   UpdateEx_1(float tick);
		//����������ͼ
		bool   SetTexture(const char* filename );
		//����ֲ���������ͼ
		bool   SetTexture(Ogre::Texture *  tex);
		//����
		void   Reset();
		//ɾ��λ��
		void   DelPos( Ogre::Vector3 pos , float fSize );
		//�õ���ǰֲ������
		int    GetCurPlantNum(){return m_nCurVecNum / 4;}
		//���������ɫ
		void   ClearLightColor();
		//���㶥�������ɫ�������� �ƹ�λ�ã��ƹ���ɫ���ƹⷶΧ ��
		void   CountLightColor(int lighttype, Ogre::Vector3 pos, Ogre::ColourValue color, Ogre::ColourValue ambient, float range, float shadowdensity=0);
		//���µƹ���ɫ
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
	//ֲ����
	struct PlantName_t
	{
		char plantFileName[260];
	};
	//ֲ����Ϣ
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
	//�����ļ���Ϣ
	struct  MTextureInfo
	{
		char   textureName[260];
	};
	//ͨ���Ҷ�ͼ���ɶ�Ӧ�Ĵ���ֲ����Ϣ
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
		//Ŀǰ֧��1024x1024�ĻҶ�ͼ���ƴ��ģֲ������
		unsigned char                 m_PlantMap[1024*1024];
		//��Ӧ�Ĳ�ֲͬ����Ϣ
		std::vector <MTextureInfo>      m_TextureFileSet;
		//��Ӧֲ����Ϣ�ļ�
		char                            m_strPlantInfoFile[260];

	};

	extern GrassGroupManager    *g_PlantMgr;
}
