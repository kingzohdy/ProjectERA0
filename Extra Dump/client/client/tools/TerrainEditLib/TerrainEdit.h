
#pragma once

#include "TerrainEditTypes.h"
#include "TerrainBlock.h"
#include "TerrainHeightMap.h"
#include "OgreInterpCurve.h"
#include "OgreBeach.h"
#include "Terrain.h"
#include "OgreMemoryDelegation.h"
#include <deque>

class DBDataManager;
namespace TE
{
	enum  CHANGE_HEIGHT_MODE
	{
		SOFT_MODE = 0, //����ģʽ
		HARD_MODE ,    //����ģʽ
		SMOOTH_MODE ,  //ƽ��
		LEVEL_OFF,     //ƽ��
		RAMP_MODE ,    //б��
		DITCH_MODE ,   //����
		CURVE_MODE,	   //���߱༭ģʽ
	};

	enum  REDUCE_MODE
	{
		NO_REDUCE = 0,  //���ü�
		BEGIN_REDUCE ,   //�ü�
		REDUCE_RESUME,   //�ָ�
	};

	enum  ALPHA_MODE
	{
		ALPHA_ADD = 0, //��
		ALPHA_DEC ,    //��
	};

	enum  PAINT_GAME_IFNO
	{
		WALK_LAYER = 0 , // ���߲�
		WATER_LAYER ,     // ˮ��
		MAGICBAR_LAYER ,  // ������
		SKILL_LAYER,	 // ���ܲ�
	};

	enum  PAINT_MODE
	{
		PAINT_CIRCLE = 0, //Բ��
		PAINT_QUAD,       //����
		PAINT_RAND_QUAD,  //�������
		PAINT_RAND_CIRCLE,  //���Բ��
	};

	struct ActionEvent
	{
		EDITEVENT_MOUSE  mouseEven; //����¼�
		EDITEVENT_KEY    keyEven;   //�����¼�

		int       mx,my,mz;       //�����Ϣ��X,Y,Z

		Ogre::Vector3     dir,pos;        //��Ļ�����ߣ�����λ��
		bool      bMouseDown;     //�Ƿ���갴��
		bool      bKeyDown;        //�Ƿ���̰���
		unsigned char  keyCode;    //����ֵ
	};
	//��͸������	 
	struct  ALPAHA_DATA
	{
		unsigned char data[64*64];
	};
	//����������
	struct MAP_TEX_INFO
	{
		char          texName[256];
	};

	enum
	{
		SELECT_RES_NULL = 0,
		SELECT_RES_MODEL,
		SELECT_RES_TEXTURE,
		SELECT_RES_SOUND
	};
	struct SelectedResData
	{
		SelectedResData() : seltype(SELECT_RES_NULL), pmodel(NULL), ptexture(NULL), model_id(0), model_index(0)
		{
		}

		int seltype;
		std::string respath;
		EditorModel *pmodel;
		Ogre::Texture * ptexture;

		Ogre::WorldPos pos;
		float dir;
		int model_id;
		int model_index;
	};

	//��ͷ����
	struct MCameraData
	{
		Ogre::Vector3 eyeloc;
		Ogre::Vector3 goal;
	};

	//ģ�Ͳ����¼�
	enum MODEL_ACTION
	{
		MODEL_ROATE_Y = 0,//��ת
		MODEL_SCALE ,//����
	};
	//ģ������
	enum MODEL_TYPE
	{
		TYPE_BIG_MODEL = 0 ,//�����
		TYPE_SMALL_MODEL , //С���
		TYPE_DETAIL_MODEL , //ϸ�����
		TYPE_PLANT_MODEL , //ֲ�����
		TYPE_PLANT_GRASS, //�ݲ�
		TYPE_LINK_MODEL ,//link Model
		TYPE_LINK_TEXTURE ,// Link Texture
		TYPE_PAINT_TEXTURE,// �����
		TYPE_LINK_WATER_MODEL ,// 
		TYPE_PHYSIC_MODEL,//����ģ��
		TYPE_GLOBAL_MODEL,

		TYPE_MODEL_NUM,//
	};

	inline bool CanEditModelType(int type)
	{
		return type>=TYPE_BIG_MODEL&&type<=TYPE_PLANT_MODEL || type==TYPE_GLOBAL_MODEL;
	}

	struct EditOptionWater
	{
		int chunk_x;
		int chunk_z;
		float waterheight; //ˮƽ��ĸ߶�:y

		int watertype; //0:general water,  1:reflect water

		char animtex[128];
		char envtex[256];
		Ogre::ColourValue watercolor[2];
		float waterdepth[2];
		float maxwaterdepth;
		float ntexrepeat;
		float speed;
		float amp;
	};

	class BaseEditHandler;
	// �༭����
	/*
	�༭�����еĵ�ǰ�༭���Զ�����ڸýṹ�С�
	*/
	struct TerrainEditOption
	{
		bool					bMoveGrid; //�Ƿ񰴸����ƶ����
		PAINT_GAME_IFNO			gameInfo;  //��Ϸ����Ϣ
		bool					bGetWaterHeight; //�Ƿ�õ�ˮ��߶�
		int						curMaxScale,curMinScale; //��ǰ������ţ���С����
		unsigned  char			uUVRepeat[4]; //��Ӧ�����Ļ��ƴ���

		MODEL_TYPE				mModelType; //ģ������
		bool					bUserPaintPic; //
		unsigned  char			mColorMap[3]; //��ǰcolormapʹ����ɫ

		MODEL_ACTION			mCurModelAction; //��ǰ�༭ģ���¼�
		EDIT_EVENT				mCurEditEvent;   //��ǰ�༭���¼�
		EDIT_EVENT				mSaveCurEditEvent; // ���浱ǰ�༭���¼�

		CHANGE_HEIGHT_MODE		mCurChangeHeightMode;//�༭�߶�ģʽ
		PAINT_MODE				mCurPaintMode;//��ˢģʽ
		ALPHA_MODE				mCurAlphaMode;//��͸�����ģʽ
		REDUCE_MODE				mCurReduceMode;//�ü�ģʽ

		bool					bPickTexture;//�Ƿ�ʰȡ��ǰ����

		int						nPaintPlantSize;//ֲ�ﻭˢ��С
		int						nPaintVec;//��ˢǿ��
		int						nCurMoveY;//Yƫ��
		int						nCurLayer;//��ǰ��
		int						nCurChangeHeightSize;//��ǰ�ı�߶ȴ�С
		int						nCurPaintSize;//��ǰ��ˢ��С

		float					fCurHeightScale;//��ǰ�߶�����
		int						nCurSelectTexID;//��ǰѡ������ID
		MAP_TEX_INFO			PaintTexInfo[4];//������Ϣ��4��
		MAP_TEX_INFO			PlantTexInfo; //
		Ogre::Vector3           PlantSavePos; //ֲ�ﱣ��λ��

		int						nCurSelectPaintTexId[5];//ѡ�������ID ��5��
		int						nCurSelectPaintRad[6];//��ǰ��Ӧ�İ뾶��
		int						nCurSelectPaintMode[6]; //��ǰѡ�õ�ģʽ

		ALPAHA_DATA				alphaPaintData[5];//��Ӧ�İ�͸������
		bool					bBeingModifyHeight;//�Ƿ��޸ĸ߶�

		/*
		char					strLiquidName[128];//Һ���ļ���
		float					fLiquidHeight;//Һ��߶�
		float					fLiquidColor[4];//Һ����ɫ
		float					fSaveLiquidHeight;//����Һ��߶�
		int						nCurAddLiquidX,nCurAddLiquidZ;//��ǰҺ���Ӧ��X,Z
		*/

		EditOptionWater			water;

		int						nCurAddModelSize; //��ǰ����ģ�͵ķ�Χ
		int						nCurAddModelNum;  //��ǰ����ģ�͵�����
		int						nCurCameraTpye;   //��ͷ������

		Ogre::Vector3           nSaveAddPlantPos;  //��������ֲ���λ��

		int						nCurSelectNpcID;   //��ǰѡ��NPC ID
		int						CurSelectNpcIndex; //��ǰѡ�еڼ���npc

		bool					bUpdateNpcData;   //�Ƿ����NPC����
		bool					bUpdateLightData;  //�Ƿ���µƹ���Ϣ
		bool					bUpdateAreaData;  //�Ƿ����������Ϣ
		bool					bUpdateWaterData; //�Ƿ����ˮ����Ϣ

		float					fPlantScale;		//�����Ӵ���ֲ�������

		BaseEditHandler			*pEventProcess;		//�����¼���ָ�� ��������ֲ������ࣩ
	};

	enum  MODIFY_DECAL_TYPE
	{
		DECAL_NORMAL = 0 ,
		DECAL_WATER , 
	};

	// ��¼��ͼ��Ϣ
	struct GameMapInfo
	{
		int mapid;
		int tilex;
		int tilez;
		int ngridx;
		int ngridz;
		int texrepeat;

		char mapname[64];
	};

	//�༭��ͼ��Ϣ
	struct  MapEditInfo
	{
		int   nAmibentLightColor;
		int   nDirectLightColor;
		int   nMouseMoveScreen;
		int   nMoveObject;
		int   nRoateObject;
		int   vDirectLightX;
		int   vDirectLightY;
		int   vDirectLightZ;
	};
	//��ˢ��Ϣ
	struct  PaintInfo
	{
		unsigned char  data[64*64];
	};
	//ѡ��������Ϣ
	struct SelectTexInfo
	{
		bool  bSelect;
		int gx ,gy;
	};
	//chunk�������Ϣ
	struct MChunkTextureInfo
	{
		int            sign;
		int            uvinfo[4];
		MAP_TEX_INFO   texFileInfo[4];	
	};

	struct TerrainDrawOption
	{
		bool   bDrawChunk;
		bool   bDrawModel;
		bool   bDrawQuad;
		bool   bDrawLiquid;
		bool   bUserColorMap;
		bool   bDrawPlant;
		int    nDrawLod;
	};

	class RenderUnit
	{
	public:
		RenderUnit(void *hwnd, int type);
		~RenderUnit();

		void update(unsigned int dtick);
		void setClearColor(unsigned int color);

	public:
		int m_Type;
		Ogre::SceneRenderer *m_pRenderer;
		Ogre::UIRenderer *m_pUIRenderer;
		Ogre::GameScene *m_pScene;
		Ogre::Camera *m_pCamera;
		Ogre::RenderWindow *m_pTarget;
	};

	class UndoSaveDataGroup;

	// ���α༭������
	class TerrainEditMgr : public Ogre::MemoryDelegation
	{
	public:
		TerrainEditMgr(void);
		~TerrainEditMgr(void);

		bool Init(void *hinst, void *hmainview, void *hrespreview, void *hradarview);
		void doFrame(unsigned int dtick);
		bool isInit(){return m_bInit;}

	public:
		int m_MapID;

	private:
		void createShowModelDlg(void *hwnd);
		void createShowTextureDlg(void *hwnd);


	public:
		void addBeach (Ogre::Beach *beach)
		{
			m_BeachList.push_back(beach);

			beach->attachToScene(&g_Terrain);
		}

		void removeBeach (Ogre::Beach *beach)
		{
			if (beach)
				beach->detachFromScene();

			std::vector<Ogre::Beach*>::iterator it = m_BeachList.begin();
			for (it; it!=m_BeachList.end(); it++)
			{
				if ((*it) == beach)
				{
					m_BeachList.erase(it);
					return;
				}
			}
		}
		int getBeachNum () { return (int)m_BeachList.size(); }
		
		void setCurrentEditBeach (Ogre::Beach *beach)
		{
			m_CurEditBeach = beach;
		}

		Ogre::Beach *getCurrentEditBeach ()
		{
			return m_CurEditBeach;
		}

	protected:
		std::vector<Ogre::Beach*> m_BeachList;
		Ogre::Beach *m_CurEditBeach;

	public:
		// ����༭��Ϣ
		void  handleEditEvent(ActionEvent* event);

		bool  LoadWowMapFile(const char* filename);

		bool  SaveTerrainFile(const char* filename);
		bool  LoadTerrainFile(const char* filename);

		void exportToObjFile(const char *filename);
		void exportTerrainToObjFile (const char *filename);
		void exportTerrainAndWater (const char *filename);

		void AddObjectsToBeast ();

		bool  SavePathFile(const char* filename); 
		bool  LoadPathFile(const char* filename);

		bool  SaveNaviFile(const char* filename);

		bool  SaveAreaFile(const char* filename );
		bool  LoadAreaFile(const char* filename );

		//bool SaveBeachFile (const char *filename);
		//bool LoadBeachFile (const char *filename);

		bool  LoadMapEditIni();
		bool  SaveMapEditIni();

		void  LoadBrae( const char* filename );
		void  SaveBrae( const char* filename );

		void  LoadingBrea();

		void RenderHelpLines();
		void  Release();

		bool  TerrainRayTestObject( int gz,int mz ,int i ,int j );

		void  SaveAlphaTex(const char* filename,unsigned char* pData , int layer);
		void  LoadBmpHeader( const char* filename );

		bool  LoadCurMapLightInfo();

	public:
		void  CreateSelfShadow(const char* filename);

		void  saveCurEditEvent(){m_EditOption.mSaveCurEditEvent = m_EditOption.mCurEditEvent;}

		void  SetColorMap(unsigned char r , unsigned char g, unsigned char b ) 
		{
			m_EditOption.mColorMap [0] = r;
			m_EditOption.mColorMap [1] = g;
			m_EditOption.mColorMap [2] = b;
			ResetColorMapAlpha();
		}

	public:
		void  Undo();
		void  Redo();   

	public:
		void  MoveShowModelZ(int pos);
		void  MoveShowModelY(int pos );
		void  RoateShowModel(float rot);

	public:
		void  SetShowObject(bool bShow);
		void  SetShowLiquid(bool bShow);
		void  SetShowChunk(bool bShow);
		void  SetShowQuadPath(bool bShow);
		void  SetShowPath(bool bShow);
		void  SetShowShadow(bool bShow);

	public:
		void             SetTerrainReport(int  nReport);

		void              SetCurSelectModelType( MODEL_TYPE type ) { m_EditOption.mModelType  = type ;}
		unsigned  char    GetCurSelectModelType(){ return m_EditOption.mModelType  ; }

		void              SetUpdateAreaData( bool bUpdate) { m_EditOption.bUpdateAreaData = bUpdate;};
		bool              GetUpdateAreaData(){ return m_EditOption.bUpdateAreaData ;} ;

		void  SetUpdateWaterData(bool b){m_EditOption.bUpdateWaterData = b;}
		bool GetUpdateWaterData(){ return m_EditOption.bUpdateWaterData;}

		void              SetSelectNpcID( int npcID );
		int               GetSelectNpcID() { return m_EditOption.nCurSelectNpcID ; };

		void              SetPickTexture( bool bPick) { m_EditOption.bPickTexture = bPick;};
		bool              GetPickTexture() { return m_EditOption.bPickTexture ;}

		void              SetCurModelAction( MODEL_ACTION mode){ m_EditOption.mCurModelAction = mode;};
		MODEL_ACTION      GetCurModelAction(){ return m_EditOption.mCurModelAction ;}

		void              SetReduceMode(REDUCE_MODE mode ) { m_EditOption.mCurReduceMode = mode ; }
		REDUCE_MODE       GetReduceMode() { return m_EditOption.mCurReduceMode ; }

		// ���õ�ǰ������¼�����ѡ�б༭ҳ�е�ĳ��ҳ�棬��OnSetFocus�л���ô˺�����
		void              SetCurEditEvent(EDIT_EVENT event);

		EDIT_EVENT        GetCurEditEvent(){return m_EditOption.mCurEditEvent;};

		void                  SetCurHeightMode(CHANGE_HEIGHT_MODE mode){m_EditOption.mCurChangeHeightMode = mode;};
		CHANGE_HEIGHT_MODE    GetCurHeightMode() {return m_EditOption.mCurChangeHeightMode;}
		void				SetHeightCurve (Ogre::InterpCurve<float> *pCurve) // ���õ��α༭�е�����
		{
			m_pCurve = pCurve;
		}

		Ogre::InterpCurve<float> *GetHeightCurve ()
		{
			return m_pCurve;
		}

		void               SetCurPaintMode(PAINT_MODE mode){ m_EditOption.mCurPaintMode = mode;};
		PAINT_MODE         GetCurPaintMode();

		void       SetCurLayer(int layer){m_EditOption.nCurLayer = layer;};
		int        GetCurLayer(){ return m_EditOption.nCurLayer ;};

		void        SetCurAlphaMode( ALPHA_MODE mode){m_EditOption.mCurAlphaMode = mode;};
		ALPHA_MODE  GetCurAlphaMode(){return m_EditOption.mCurAlphaMode ;};

		void    SetHeightScale(int  height){ m_EditOption.fCurHeightScale = height / 30.0f;};
		float   GetHeightScale(){ return m_EditOption.fCurHeightScale ;};

		void    SetPaintSize(int size);
		int     GetPaintSize();

		void    SetChangeHeightSize(int size);
		int     GetChangeHeightSize();

		void   SetCurShowLayer(int layer);
		void   SetCurAlphaSize(int size);

		void   SetCurSelectBuildingModelName(const char* filename);

		void setCurSelectPaintTexName(const char* filename);
		void setCurSelectModelName(const char* filename);
		void setCurSelectRes(const char *filename);

		void  SetCurSelectPaintTexId(int texid);
		void  SetCurSelectPaintRad(int size){ m_EditOption.nCurChangeHeightSize = size; };

		void  SetCurSelectPaintMode(int mode,int layer);
		void  SetTerrain(Terrain* terrain);	


		void  SetChunkDrawMode(int index);
		void  SetCameraType( int index );
		int   GetCameraType();

		void   SetModifyHeight(int height)
		{  
			m_EditOption.nCurMoveY = height;
		}

		float GetModifyHeight () { return m_EditOption.nCurMoveY; }

		void SetHeightAddMode (bool bHeightAdd) // depend on ui
		{
			m_bHeightAddMode = bHeightAdd;
		}

		bool GetHeightAddMode ()
		{
			return m_bHeightAddMode;
		}

		void   SetPaintVec(int vec){ m_EditOption.nPaintVec = vec;};

		void   SetPlantSize( int size){ m_EditOption.nCurAddModelSize = size;};
		void   SetPlantNum( int num){ m_EditOption.nCurAddModelNum = num ;}	

		void  SetUsePaint( bool bUser ) { 	m_EditOption.bUserPaintPic = bUser;};
	public:
		void  DelCurObject(); 
		int  AddPath(const char* srcName,const char* destName);
		void AddPath(int id , unsigned int type , char* description);
		void ModifySelPathInfo(int id , unsigned int type , char* description);    ///< �޸ĵ�ǰѡ���·����Ϣ
		bool ReadPaths();
		bool WritePaths();

		//type=0: ��������  1: �߼�����
		bool AddArea(int id, int type, const char *name, const char *scriptName);

		bool  DelCurPathPos();
		int   DelCurPath();
		void  SetCurPath(int index);    ///< ���õ�ǰ�༭·��

		void  CreateAlphaPaint();

		void  Update(unsigned int dtick);
		void  UpdataNpc(float dtime);

		void Roate(float x, float y ,float z);

		MapNpcManager*   getNpcManager(){return m_pNpcSet;}

		int   AddModelName(std::string name);
	private:
		int   AddTextureFileToTitle(std::string name);

		void  UpdateCamera();

		void  MoveSelectModel(Ogre::Vector3 &dir,Ogre::Vector3 &pos,int mode);
	public:
		void   ResetPaintTexture();
	public:
		bool  SelectCurPos(Ogre::Vector3  &dir,Ogre::Vector3  &pos);
		void  SetChunkGridVisible( int gx ,int gz );

	private:
		bool  CullCurGrid(Ogre::Vector3 &dir, Ogre::Vector3 &pos);

		void  RoateYSelectModel(int dz);
		void  ZoomSelectModel(int  dz);
		void  MoveSelectModel(float length);

	private:	
		void  SetEditDisable( bool sign ) { m_bEditDisable = sign;};		
		//���α༭��Ӧ������
		MTileData*        m_pEditData;

		// ���α༭����
		Ogre::InterpCurve<float> *m_pCurve;
	public:
		void               SetEditData( MTileData* pData ) { m_pEditData = pData ;}
		MTileData*        GetEditData(){ return m_pEditData ;};
		EditorCamera*           GetCamera(){return m_pCamera;};
		void				UpdateLiqduiOption(TE::EditOptionWater &water);

	public:
		Terrain *m_pCurTerrain;
		RenderUnit *m_pMainRU;
		RenderUnit *m_pResPreviewRU;
		RenderUnit *m_pRadarViewRU;

	private:
		bool m_bInit;

		DBDataManager*    m_pDBMgr;
		Ogre::AvatarManager *m_pAvatarMgr;

		//�༭����ͷ
		EditorCamera*            m_pCamera;
		//·������
		MapPathGroup           *m_pPathSet;
		//NPC����
		MapNpcManager            *m_pNpcSet;    

		bool m_bHeightAddMode; // �߳��Ƿ�Ϊ����ģʽ

	public: 
		bool               m_bEditDisable;
		MapPathGroup*         GetPathSet(){return m_pPathSet;};

		//���
		MapMarkGroup           *m_pSignSet;   

		//�༭����
		TerrainEditOption   m_EditOption;
		TerrainDrawOption m_DrawOption;
		TerrainCull   *m_Cull;

		SelectedResData m_SelResData;
		int m_PreviewOrRadar; //0-С��ͼ,  1-ģ����ͼԤ��
		DRAW_FILL_MODE m_DrawFillMode;

	private:
		struct   Select_Info
		{
			int  tx,ty;
			int  gx,gy;
			int  vx,vy;
			Ogre::Vector3  pos;
			int   size; 
		};

	public:
		//��ǰѡ����Ϣ
		Select_Info       m_curSelectInfo;
		//����ѡ����Ϣ
		Select_Info       m_saveSelectInfo;

	private:
		TerrModel*     m_pCurSelectModel;

	public:
		std::vector<TerrModel *>        m_CurSelectModelSet;

	private:
	public:
		void *             GetTexShowDlgHWND(){return m_hwndTexShowDlg ;};

	public:
		void *              m_hwndView;	
		void *              m_hwndTexShowDlg;
		void *              m_hwndModelShowDlg;
		void *m_hPreviewWnd;
		void *m_hRadarViewWnd;

		MCameraData m_ResPreviewCamera;

	public:
		void    MsgProc(void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam );
	public:
		void   ShowTexDlg(void *hWnd,void *hinst);
		void   ShowModleDlg(void *hWnd,void *hinst);

	public:

		void   ModifySelectModelPos( Ogre::Vector3 pos );
		void   ModifySelectModelDir( Ogre::Vector3 dir );
		void   ModifySelectModelScale( float scale );

		void   ChangeTexToMayFormat();
		void   ChangeDirTexToMayFormat(const char* dir);

		bool   ChangeTexFileToMayFormat(const char* filename);

	public:
		bool    CreateTitle(int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum
			,const char* basetex,int nRepeat,
			//���Ӳ���������ÿ��chunk�ֶ���С����
			int gwidthnum ,int gheightnum);

		//�������
		void    ClearAll();
	public:
		struct NormalInfo
		{
			unsigned char gx,gz;
			unsigned char vecIndex;
		};
		//��Ҫ�ı�ķ�������
		std::vector <NormalInfo>     m_ChangeNormalSet;
	public:
		void                         CreatNavMap(int gx,int gz,float height);

	public:
		Coordinate*                GetCoordinate(){ return m_pCoordinate;}; 
	private:
		//x,y,z����ϵ
		Coordinate                  *m_pCoordinate;
		int                          m_SaveCoordinateX, m_SaveCoordinateY;
		Ogre::Vector3                        m_SaveCoordinatePos;

	public:
		void  MoveSelectModel(Ogre::Vector3 &dir, Ogre::Vector3 &pos);
		void                         MoveMoveAxes ( COORDINATE_SELECT select_mode , int x ,int y  , Ogre::Vector3 &pos , Ogre::Vector3 &dir ,int mode = 0 );
		void rotateModelAround(COORDINATE_SELECT select_mode, int x, int y);
		void scaleModel(int x, int y);
		void                         saveCoordinatePos( int x ,int y);	
		void clearSelect();
		void setSelectModel(TerrModel *pmodel, OBJSELECT_MODE selectmode=OBJSELECT_NORMAL);
		TerrModel *getSelectModel(){return m_pCurSelectModel;}
		void deleteModel(TerrModel *pmodel);
		Ogre::Vector3 getSelectCenterPos();

	private:
		void                          CreateMayShadow(const char* filename);

	public:
		void                          CreateMinMap( const char* filename );

	public:
		bool selectMapNPC(int index);
		bool SelectMapLight(int index);
		void cameraOnSelectModel();
		void      ShowNpc( int npcID );
		void      SetNpcScript (int index , const char* npcScript );
		//�����޸Ķ�����Ϣ
		struct   SaveModifyVecInfo
		{
			int  gx,gz;
			int  vecIdx;
			float  height;
			bool   bCount;
			float  lenth;
			float  range;
		};
		//�����޸Ķ�����Ϣ����
		std::vector <SaveModifyVecInfo>      m_SaveModifyVecInfo;
		//ƽ���ṹ
		struct LevelOffInfo
		{
			bool  bCount;
			float fHeight;
		};
		//ƽ����Ϣ����
		LevelOffInfo                         m_SaveLevelOffInfo;
		//��õ�ǰѡ�еĵ�
		void                                 GetCurSelectVec();
	public:
		void  SmoothHeightMap( int width ,int height );

		//��ҪӦ���ڶ����ܶȵ�����
		void  CreateHeightMapEX(Ogre::Point2D nblock, Ogre::Point2D ngrid, int minheight ,int maxheight, int randsize,int smoothNum);

	public:			
		void                                   AddPlant(Ogre::Vector3 dir , Ogre::Vector3 pos );
		void                                   PaintPlant(Ogre::Vector3 dir , Ogre::Vector3 pos ,int mode = 0);

		GameMapInfo * getMapInfo(){ return &m_GameMapInfo; }

	private:
		GameMapInfo m_GameMapInfo;

	public:

	public:
		void                            UpdateMapTexRepeat(int nRepeat );

		void                             UpdateTileHeightDataToHeightMap();
		void                             UpdateHeightMapToAllTileHeight();

		void                             UpdateNormalMap(int indexX ,int indexY);
		void                             UpdataNormalMap();

		void                            UpateCurGameInfoToColorMap();

	public:
		//б����ʾ
		SlopeMark                       *m_pCampSign;
		//�Ƿ�ʼ��б��
		bool                             m_bBeingRamp;
	public:
		void                          MoveCameraToModel();	
	public:
		//������Ҫƽ����λ��
		std::vector <int>                m_SaveSmoothPos;
		//������Ҫ���¼��㷨�ߵ�λ��
		std::vector <int>                m_SaveReCountNormal;
		//��ǰ���λ��
		Ogre::Vector3                             m_CurMousePos;
	public:
		//�Ƿ�����ƶ�
		bool                            m_bMouseMove;
	public:
		//�����������Ϣ
		struct    MCopyObjectInfo
		{
			std::string  name;//��Ӧ�ļ���
			Ogre::Vector3        pos;//λ��
			Ogre::Vector3        dir;//����
			float        scale;//����
			unsigned  char type;//����
		};
		std::vector <MCopyObjectInfo>          m_CopyObjectInfo;

		void                            CopySelectObject();
		void                            CopySelectObjectToTile();

		void                            MoveSign(Ogre::Vector3& dir , Ogre::Vector3& pos );

		void                            SetShowObjectBox(bool bShow );

		void                            ChangePaintSize(int mode );
		void                            ChangePaintPower(int mode );

		void                            UpdateOther ( float dtime);

		void                            SetCurPaintInfoData( unsigned char* pData , int bmpSize = 64);

		std::string                     m_curSaveFile;

	public:
		MapEditInfo                     m_MapEditInfo;
		bool                            m_bShowObjectBox;

		void                           UpdateBox();
		void                          SetHeightMap(int width,int height , unsigned char  *heightmap );

		void                           ResetColorMapAlpha();

	public:
		bool	                       m_bUpdateHeightmap;	
	public:
		bool                           SaveCurHeightMap(const char* filename);

		void                            AddNewStrip();

		MODIFY_DECAL_TYPE                    m_DecalType;

		Ogre::SceneRenderer                        *m_pSceneRenderer;

		bool                            SetSkyModel(const char* filename);

		void                            SetCurTexRepeat( int nLayer , int nRepeat );
		int                             GetCurTexRepeat( int nLayer );

		const char *getTextureBrushPath(int layer);
		const char *GetShaowTexturePath();

		void                            SelectChunkTexInfo(Ogre::Vector3 dir , Ogre::Vector3 pos );
		void                            LoadTerainLayerInfo();

		void                            UpdateAlphaData(const char* filename, int layer);
		void                            UpdateShadowData(const char* filename);

		void                            UpdateColorData(const char* filename);
		void                            UpdatePlantData(const char* filename);

	public:
		SelectTexInfo                    m_CurSelectTexInfo;
		void                             SwitchEditTile();

		bool                             m_bAltCtrlMoveCamera;
		int                               m_AltCtrlMoveX,m_AltCtrlMoveY ;

		void                             ReplaceCurTexToAllSelectLayer();
		void                             UpdateHeightMapToCamera(int x ,int y );

		void                             SetTypeModelShow( int type , bool show );
		bool                             m_bLockModelEdit[TYPE_MODEL_NUM];

		void                             ChangeDirectLight( int r ,int q );
		bool                             DeleteModelForChunk( TerrModel *pModel );

		void                            SetLightStrength(float fStrength);
		void                            SetShadowStrength( float fStrength );

		void                              UpdateChunkDetailVec();

		void                              SaveChunkTextureInfo();
		void                              LoadChunkTextureInfo();

		int                                AddTextureFileToTile(const char* filename);

		//���浱ǰˢ���߲㣬ˮ�����ϢΪ��ͼ������Ϣ
		void                               SaveCurNaviMap(int gx,int gz);
		//������Ϣ��Ӧ����Ϣ��ɫ
		void                               SetModifyMapInfoColor( int layer , unsigned char r, unsigned char g, unsigned char b);
	public:
		MChunkTextureInfo                 m_ChunkTextureInfo[16][16];

	public:
		//�����Ҽ������ƹ���Ϣ�Ի���
		bool                            m_bOpenLightInfoDlg;	
		bool                            m_bShowLightInfoDlg;
		//�����Ҽ���������ѡ��Ի���
		bool                            m_bOpenAreaInfoDlg;
		bool                            m_bShowAreaInfoDlg;
		//�����ģ̬�ֲ�ˮ���öԻ���
		bool                            m_bOpenTwoWaterDlg;
		bool                            m_bShowTwoWaterDlg;

		bool                            m_bOpenTextureInfoDlg;	
		bool                            m_bShowTextureInfoDlg;
		//�����Ҽ�����ģ����Ϣ�˵�����
		bool                            m_bOpenModelInfoDlg;	
		bool                            m_bShowModelInfoDlg ;

		bool                             m_bPessQ;
		/// �����������öԻ�������
		bool                            m_bUpdateAmbientSettingDlg;

	public:
		TerrainHeightMap                       m_HeightMap;

	public:
		COORDINATE_SELECT                m_select_coordinate;

	public:
		void                            SetWaterInfo( float weight,float UVWrap,float speed, int angle) ;
		void                            SetCurBaseTexture( const char* filename );

	private:
		//�Զ�ƴ���±�ʵ��
		LinkMeshManager                    *m_AutoLinkMesh;
		//�Զ�ƴ������
		LinkTextureManager                  *m_AutoLinkTex;
		//����ƴ�ر�
		LinkOverlayManager                   *m_PaintWallPaper;
	public:
		LinkMeshManager*                   GetAutoLinkMesh(){ return m_AutoLinkMesh ;}
		LinkTextureManager*                GetAutoLinkTex(){ return m_AutoLinkTex ;}
		LinkOverlayManager*                 GetPaintWallPaper(){return m_PaintWallPaper ;} 

		//����ˮ�ı�Ե�˻�
		void                             CreateWhiteWater();

	public:
		void                              SetAutoDelBlock( int nDel );
		void                              UpdateBreaHeight();

		void                              UpdateProcessData();

	public:
		//�Ƿ����������
		bool                              m_bDrawTextureWnd;
		//�Ƿ����ģ�ʹ���
		bool                              m_bDrawModelWnd;

		Ogre::RenderLines *m_pLinesRenderer;

	};

	extern TerrainEditMgr  g_TerrainEdit;
}

