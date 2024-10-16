
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

	//�������
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

		void  Init(int unitsize, MPoint2D nblock, MPoint2D ngrid);//unitsize=1,2һ��ƴ�ӵ�λ��Ӧ�����ͼ�� ��ͼ��nblock��, ÿ��block��ngrid��
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

		//������Ϣ���ü�����Ӧ�ĸ���
		virtual    void     UpdateCullMap(int x,int y );


		virtual    void     UpdatePos( int x,int y ,int width ,int height );

		void        SaveLinkMap(const char* filename);
		virtual      bool        LoadLinkMap(const char* filename);

		//���˲���
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

		//��ͼ��С
		int             m_nMapSize;
		//link�����ͼ
		unsigned char   *m_LinkMap;
		//ʹ��ģ�͵�ͼ����
		unsigned char   *m_UsedModelMap;
		//��Ǽ���
		unsigned char    m_SignSet[SIGN_NUM];
		//��ͼ��Ǽ���
		unsigned char    m_SignMapSet[SIGN_TYPE_NUM] ;
		//���ӵĿ��,�߶�
		float           m_fWidthSize,m_fHeightSize;
		//MESH�ļ���
		std::string     m_MeshName;
		//���浱ǰ�༭ѡ�е��ļ�
		std::string     m_SaveCurEditMeshName;
		//��Ӧ�Ĳü���Ϣ
		typedef   std::map<int , LinkMeshCullInfo>     MESHCULLINFO_MAP;
		MESHCULLINFO_MAP                            m_CullInfoMap;
		//�ƶ��ĸ߶�
		float                                         m_fMoveHeight;
		//����MAP������
		std::deque<CurrLinkMapSaveData>                     m_SaveMapData;
		//LINK��·��
		std::string                                     m_strLinkPath;
		//�Զ�ɾ��BLOCK
		int                                            m_nAutoDelBlock;
		//ģ�Ͷ�Ӧ����
		typedef   std::map < std::string , int >       MODEL_KIND_MAP;
		MODEL_KIND_MAP                                 m_ModelKindMap;
		//�Ƿ�������
		bool                                           m_bLoading;
		//��ǰ������ID
		int                                            m_curLoadingID ;
		//��ǰ����ID
		int                                            m_iLoadingForwardID ;
		//�������ID
		int                                            m_iLoadingBackID;
		//�Ƿ�0,1,0,1˳��
		bool                                           m_bOderLinkMesh;
	public:
		void                                           SetBeginLoadIDPos( MVec3  pos );
		//��ǰѡ������( X,Y )
		int                                             m_nCurSelectX,m_nCurSelectY;
		//�Զ�����ģ������
		LinkMeshInfo *m_ModelMapInfo;
		//�ü���Ϣ����
		unsigned char    *m_SignCullInfo;
		//�Ѿ���������Ϣ����
		unsigned char    *m_bLoadedInfo;
	};

	class LinkTextureManager 
		: public LinkMeshManager
	{
	public:	
		LinkTextureManager();
		virtual ~LinkTextureManager(){};

		virtual    void    UpdateChunkHeight( int x, int y );
		//������Ϣ���ü�����Ӧ�ĸ���
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

	//��Ҫ�����ֱ���ֶ�ƴ�ӵر�
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

