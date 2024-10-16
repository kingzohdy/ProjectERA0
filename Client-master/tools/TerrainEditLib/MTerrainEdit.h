
#pragma once

#include "TerrainEditTypes.h"
#include "TerrainBlock.h"
#include "TerrainHeightMap.h"
//#include "GameEngineTypes.h"
#include <deque>

class DBDataManager;
namespace TE
{

//����¼�
enum MMOUSE_EVENT
{
	MouseNormal = 0,
	MouseLDown ,
	MouseMove,
	MouseLUp,
	MouseDDown,
	MouseRDown,
	MouseRUp,
	MouseSewheel,
	MouseDown,
	MouseUp,
};

//�����¼�
enum MKEY_EVENT
{
	KeyDown = 0,
	KeyUp   ,
};

//���β��
enum TERRAIN_LAYER
{
	LAYER_0 = 0,
	LAYER_1,
	LAYER_2,
	LAYER_3,
    LAYER_SHADOW,
	LAYER_COLOR,
	LAYER_PLANT,
};
//�༭�¼�
enum EIDT_EVENT
{
	UNKOWN_ACTION   = 0 ,
	PANIT_EVN ,            //�ر�༭
	CHANGE_HEIGHT_EVN,     //���α༭
	SELECT_OBJECT_EVN,     //ѡ�����
	ADD_OBJECT_EVN,        //�������
	ADD_MORE_OBJECT_EVN,   //���Ӵ������
	MODIFY_LIQUID,         //�༭Һ��
	MODIFY_PATH,          //�༭·��
	ADD_NPC,               //����NPC
	AREA_EIDT_EVEN,        //����༭
	SELECT_AREA_EVEN,     //ѡ������
	MOVE_OBJECT_EVN,       //�ƶ���� 
	ROATE_OBJECT_EVN,      //��ת���
	SCALE_OBJECT_EVN,      //�������
	DECAL_EVN,              //����
	AUTOLINK_OBJECT_EVN,    //ˢ����
	AUTOLINK_TEXTURE_EVN,   //�Զ�ƴ�ӵر�
	PAINT_TEXTURE_MODEL_EVN, //ͨ����������ģ��
	WALLPAPER_EVN,          //���ο�༭
	USER_INPUT_EVN,          //�û�����
	BRAE_EDIT_EVN,           //б�±༭
	LIGHT_EDIT_EVN,          //�ƹ�༭
    PLACE_PLYAER,              //playerԤ��
	SOUND_EDIT_EVN ,           //�����༭
	EVEN_NUMBER,
};

//�༭��Ӧ˵��
const char  g_strEditMode[EVEN_NUMBER][32] =
{
	"δ֪����",
	"�༭�ر�",
	"�༭����",
	"ѡ�����",
	"�������",
	"����ֲ��",
	"�༭Һ��",
	"�༭·��",
	"�༭npc",
	"�༭����",
	"ѡ������",
	"�ƶ����",
	"��ת���",
	"�������",
	"����",
	"�Զ�ƴ��ģ��",
	"�Զ�ƴ�ӵر�",
	"ͨ����������ֲ��",
	"���ο�༭",
	"�û�����",
	"б�±༭",
	"�ƹ�༭",
	"��ͼԤ��",
	"�����༭"
};

//�ı�߶�ģʽ
enum  CHANGE_HEIGHT_MODE
{
	 SOFT_MODE = 0, //����ģʽ
	 HARD_MODE ,    //����ģʽ
	 SMOOTH_MODE ,  //ƽ��
     LEVEL_OFF,     //ƽ��
     RAMP_MODE ,    //б��
	 DITCH_MODE ,   //����
};
//�ü�ģʽ
enum  REDUCE_MODE
{
    NO_REDUCE = 0,  //���ü�
	BEGIN_REDUCE ,   //�ü�
	REDUCE_RESUME,   //�ָ�
};
//ALPģʽ
enum  ALPHA_MODE
{
    ALPHA_ADD = 0, //��
	ALPHA_DEC ,    //��
};
//��Ϸ����Ϣ
enum  PAINT_GAME_IFNO
{
	WALK_LAYER = 0 , // ���߲�
	WATER_LAYER ,     // ˮ��
    MAGICBAR_LAYER ,  // ������
};

//ˢ�ر�ģʽ
enum  PAINT_MODE
{
	PAINT_CIRCLE = 0, //Բ��
	PAINT_QUAD,       //����
	PAINT_RAND_QUAD,  //�������
	PAINT_RAND_CIRCLE,  //���Բ��
};
//�����¼�
struct ActionEvent
{
   MMOUSE_EVENT  mouseEven; //����¼�
   MKEY_EVENT    keyEven;   //�����¼�

   int       mx,my,mz;       //�����Ϣ��X,Y,Z

   MVec3     dir,pos;        //��Ļ�����ߣ�����λ��
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
	LMTex ptexture;

	MWorldPos pos;
	float dir;
	int model_id;
	int model_index;
};

//��ͷ����
struct MCameraData
{
    MVec3 eyeloc;
	MVec3 goal;
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
	TYEP_BIG_MODEL = 0 ,//�����
	TYPE_SMALL_MODEL , //С���
	TYPE_DETAIL_MODEL , //ϸ�����
	TYPE_PLANT_MODEL , //ֲ�����
	TYPE_PLANT_GRASS, //�ݲ�
	TYPE_LINK_MODEL ,//link Model
	TYPE_LINK_TEXTURE ,// Link Texture
	TYPE_PAINT_TEXTURE,// �����
	TYPE_LINK_WATER_MODEL ,// 
	TYPE_PHYSIC_MODEL,//����ģ��
	TYPE_MODEL_NUM,//
};

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

class IEventProcess;
//�༭����
struct TerrainEditOption
{
	bool                bMoveGrid; //�Ƿ񰴸����ƶ����
	PAINT_GAME_IFNO     gameInfo;  //��Ϸ����Ϣ
	bool                bGetWaterHeight; //�Ƿ�õ�ˮ��߶�
	int                 curMaxScale,curMinScale; //��ǰ������ţ���С����
	unsigned  char      uUVRepeat[4]; //��Ӧ�����Ļ��ƴ���

	MODEL_TYPE          mModelType; //ģ������
	bool                bUserPaintPic; //
	unsigned  char       mColorMap[3]; //��ǰcolormapʹ����ɫ

	MODEL_ACTION         mCurModelAction; //��ǰ�༭ģ���¼�
	EIDT_EVENT           mCurEditEvent;   //��ǰ�༭���¼�
	EIDT_EVENT           mSaveCurEditEvent; // ���浱ǰ�༭���¼�

	CHANGE_HEIGHT_MODE   mCurChangeHeightMode;//�༭�߶�ģʽ
    PAINT_MODE           mCurPaintMode;//��ˢģʽ
	ALPHA_MODE           mCurAlphaMode;//��͸�����ģʽ
	REDUCE_MODE          mCurReduceMode;//�ü�ģʽ

	bool                bPickTexture;//�Ƿ�ʰȡ��ǰ����

	int                 nPaintPlantSize;//ֲ�ﻭˢ��С
    int                 nPaintVec;//��ˢǿ��
    int                 nCurMoveY;//Yƫ��
    int                 nCurLayer;//��ǰ��
    int                 nCurChangeHeightSize;//��ǰ�ı�߶ȴ�С
    int                 nCurPaintSize;//��ǰ��ˢ��С
    
	float               fCurHeightScale;//��ǰ�߶�����
    int                 nCurSelectTexID;//��ǰѡ������ID
    MAP_TEX_INFO        PaintTexInfo[4];//������Ϣ��4��
    MAP_TEX_INFO        PlantTexInfo; //
	MVec3               PlantSavePos; //ֲ�ﱣ��λ��

	int                 nCurSelectPaintTexId[5];//ѡ�������ID ��5��
    int                 nCurSelectPaintRad[6];//��ǰ��Ӧ�İ뾶��
    int                 nCurSelectPaintMode[6]; //��ǰѡ�õ�ģʽ
	
	ALPAHA_DATA         alphaPaintData[5];//��Ӧ�İ�͸������
    bool                bBeingModifyHeight;//�Ƿ��޸ĸ߶�

	/*
    char                strLiquidName[128];//Һ���ļ���
    float               fLiquidHeight;//Һ��߶�
	float               fLiquidColor[4];//Һ����ɫ
    float               fSaveLiquidHeight;//����Һ��߶�
    int                 nCurAddLiquidX,nCurAddLiquidZ;//��ǰҺ���Ӧ��X,Z
	*/

	EditOptionWater water;

    int                 nCurAddModelSize; //��ǰ����ģ�͵ķ�Χ
    int                 nCurAddModelNum;  //��ǰ����ģ�͵�����
    int                 nCurCameraTpye;   //��ͷ������
    
	MVec3               nSaveAddPlantPos;  //��������ֲ���λ��
    TerrModel*        pSelectModel;      //���浱ǰѡ�е�ģ��

    int                 nCurSelectNpcID;   //��ǰѡ��NPC ID
	int					CurSelectNpcIndex; //��ǰѡ�еڼ���npc

	bool                bUpdateNpcData;   //�Ƿ����NPC����
	bool                bUpdateLightData;  //�Ƿ���µƹ���Ϣ
	bool                bUpdateAreaData;  //�Ƿ����������Ϣ
	bool                bUpdateWaterData; //�Ƿ����ˮ����Ϣ

	float              fPlantScale;	//�����Ӵ���ֲ�������

	IEventProcess       *pEventProcess;    //�����¼���ָ�� ��������ֲ������ࣩ
};
//����ģ����Ϣ
struct MModelSaveInfo
{
	MVec3  savePos ;
	MVec3  saveDir ;
	float  saveScale;
};
//�༭��������
enum  MODIFY_DECAL_TYPE
{
	DECAL_NORMAL = 0 ,
	DECAL_WATER , 
};
//����CHUNK����
struct MSaveChunkData
{
	int           gx,gz;
    EIDT_EVENT    saveEditEvent;
	TerrBlockData    saveChunkData;
    TerrModel   *pSaveModel;
	MVec3         savePos;
	MVec3         saveDir;
	float         saveScale;
	std::vector <MModelSaveInfo>  SaveSelectModelSet;
};
//CHUNK���ݵļ���
class MSaveChunkDataSet
{
public:
	MSaveChunkDataSet(){ m_SaveSet.clear ();};
	~MSaveChunkDataSet(){Release();};

	void  Release()
	{
		for( unsigned int i = 0 ; i < m_SaveSet.size () ; i++)
		{
			OGRE_DELETE( m_SaveSet[i] );
		}
		m_SaveSet.clear ();
	};

	void  AddChunkData(MSaveChunkData* pData){m_SaveSet.push_back ( pData );};

public:
	std::vector <MSaveChunkData*>   m_SaveSet;
};

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

//�༭�������࣬��Ҫ����������ṩ��Ҫ�Ľӿ�
class MTerrainEdit
{
public:
	MTerrainEdit(void);
	~MTerrainEdit(void);

    bool Init(void *hinst, void *hmainview, void *hrespreview, void *hradarview);
	void doFrame(unsigned int dtick);
	bool isInit(){return m_bInit;}

public:
	int m_MapID;

private:
	void createShowModelDlg(void *hwnd);
	void createShowTextureDlg(void *hwnd);

public:
	void  Process(ActionEvent* event);

	bool  LoadWowMapFile(const char* filename);
	
	bool  SaveTerrainFile(const char* filename);
	bool  LoadTerrainFile(const char* filename);

	bool  SavePathFile(const char* filename); 
	bool  LoadPathFile(const char* filename);

	bool  SaveNaviFile(const char* filename);

	bool  SaveAreaFile(const char* filename );
	bool  LoadAreaFile(const char* filename );

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
	void  SetAmbientLightColor( float r,float g,float b,float a);
	void  SetDirLightColor(float r,float g,float b,float a);
	void  CreateSelfShadow(const char* filename);

	void  SaveCurEditEvent();
	void  ReSetCurEditEvent();

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
	void  AddSaveUndoData();
	void  AddSaveRedoData();

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

    const char*      GetStrCurEditMode();

	void              SetCurSelectModelType( MODEL_TYPE type ) { m_EditOption.mModelType  = type ;}
	unsigned  char    GetCurSelectModelType(){ return m_EditOption.mModelType  ; }

	void              SetUpdateAreaData( bool bUpdate) { m_EditOption.bUpdateAreaData = bUpdate;};
	bool              GetUpdateAreaData(){ return m_EditOption.bUpdateAreaData ;} ;

	void              SetUpdateNpcData( bool bUpdate) { m_EditOption.bUpdateNpcData = bUpdate;};
	bool              GetUpdateNpcData(){ return m_EditOption.bUpdateNpcData ;};

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

	void              SetCurEditEvent(EIDT_EVENT event);
	
	EIDT_EVENT        GetCurEditEvent(){return m_EditOption.mCurEditEvent;};

	void                  SetCurHeightMode(CHANGE_HEIGHT_MODE mode){m_EditOption.mCurChangeHeightMode = mode;};
	CHANGE_HEIGHT_MODE    GetCurHeightMode();

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
	void  SetTerrain(MTerrain* terrain);	

			 
	void  SetChunkDrawMode(int index);
	void  SetCameraType( int index );
	int   GetCameraType();
   
	void   SetModifyHeight(int height){   m_EditOption.nCurMoveY = height;}
	void   SetPaintVec(int vec){ m_EditOption.nPaintVec = vec;};
	
	void   SetPlantSize( int size){ m_EditOption.nCurAddModelSize = size;};
	void   SetPlantNum( int num){ m_EditOption.nCurAddModelNum = num ;}	
	
	void  SetUsePaint( bool bUser ) { 	m_EditOption.bUserPaintPic = bUser;};
public:
	void  DelCurObject(); 
	void  DelSelectObject( TerrModel* pModel);
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

	void  AddObjectTitle();

	MapNpcManager*   getNpcManager(){return m_pNpcSet;}

    int   AddModelName(std::string name);
private:
	int   AddTextureFileToTitle(std::string name);

	void  UpdateCamera();

	void  MoveSelectModel(MVec3 &dir,MVec3 &pos,int mode = 0);
public:
	void   ResetPaintTexture();
public:
	bool  SelectCurPos(MVec3  &dir,MVec3  &pos);
	void  SetChunkGridVisible( int gx ,int gz );

private:
	bool  CullCurGrid(MVec3 &dir, MVec3 &pos);
   
	void  RoateYSelectModel(int dz);
	void  ZoomSelectModel(int  dz);
	void  MoveSelectModel(float length);
	
public:
	void  AddPlantModelToTitle(const char* filename,MVec3 pos,MVec3 dir,float fscale);

private:	
	void  SetEditDisable( bool sign ) { m_bEditDisable = sign;};		
	//���α༭��Ӧ������
	MTileData*        m_pEditData;
public:
	void               SetEditData( MTileData* pData ) { m_pEditData = pData ;}
	MTileData*        GetEditData(){ return m_pEditData ;};
    MCamera*           GetCamera(){return m_pCamera;};
	void				UpdateLiqduiOption(TE::EditOptionWater &water);

public:
	MTerrain *m_pCurTerrain;
	RenderUnit *m_pMainRU;
	RenderUnit *m_pResPreviewRU;
	RenderUnit *m_pRadarViewRU;

private:
	bool m_bInit;

   DBDataManager*    m_pDBMgr;
   Ogre::AvatarManager *m_pAvatarMgr;

	//�༭����ͷ
   MCamera*            m_pCamera;
   //·������
   MPathSet           *m_pPathSet;
   //NPC����
   MapNpcManager            *m_pNpcSet;      

public: 
	bool               m_bEditDisable;
	MPathSet*         GetPathSet(){return m_pPathSet;};

	//���
   MSignSet           *m_pSignSet;   

   //�༭����
	TerrainEditOption   m_EditOption;
	TerrainDrawOption m_DrawOption;
	MTerrainCull   *m_Cull;

	SelectedResData m_SelResData;
	int m_PreviewOrRadar; //0-С��ͼ,  1-ģ����ͼԤ��
	DRAW_FILL_MODE m_DrawFillMode;

private:
	 struct   Select_Info
	 {
		 int  tx,ty;
		 int  gx,gy;
		 int  vx,vy;
		 MVec3  pos;
		 int   size; 
	 };
	 
public:
	//��ǰѡ����Ϣ
     Select_Info       m_curSelectInfo;
	 //����ѡ����Ϣ
	 Select_Info       m_saveSelectInfo;
public:
	//��ǰѡ��ģ��
	 TerrModel*     m_pCurSelectModel;
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

	void   ModifySelectModelPos( MVec3 pos );
	void   ModifySelectModelDir( MVec3 dir );
	void   ModifySelectModelScale( float scale );

	void   ChangeTexToMayFormat();
	void   ChangeDirTexToMayFormat(const char* dir);

	bool   ChangeTexFileToMayFormat(const char* filename);
	
	void   ClearObjectColor(unsigned int color);
	
public:
    bool                            SaveCurData(int gx,int gz,int mode);
public:
	int                             m_nCurSaveDataIndex;

public:

	std::deque <MSaveChunkDataSet>   m_SaveData;
	std::deque <MSaveChunkDataSet>   m_SaveReDoData;
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
	MVec3                        m_SaveCoordinatePos;
	
public:
    void                         MoveMoveAxes ( COORDINATE_SELECT select_mode , int x ,int y  , MVec3 &pos , MVec3 &dir ,int mode = 0 );
	void                         SetMoveAxes( int x ,int y);	

private:
   void                          CreateMayShadow(const char* filename);
   
public:
   void                          CreateMinMap( const char* filename );
   
public:
   void      AddNPC(const char* npcName , const char* npcModelPath ,  unsigned int npcID,
	          MVec3 npcPos, MVec3 npcDir ,const char* npcScript);

   bool      SelectMapNPC(int index);
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
   //ɾ��ѡ��NPC ;����( ���� )
   void                                 DeleteSelectNpc( int index );
   //��õ�ǰѡ�еĵ�
   void                                 GetCurSelectVec();
public:
	void  SmoothHeightMap( int width ,int height );
	   
	//��ҪӦ���ڶ����ܶȵ�����
	void  CreateHeightMapEX(MPoint2D nblock, MPoint2D ngrid, int minheight ,int maxheight, int randsize,int smoothNum);

public:			
	void                                   AddPlant(MVec3 dir , MVec3 pos );
	void                                   PaintPlant(MVec3 dir , MVec3 pos ,int mode = 0);

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
	SlopeSign                       *m_pCampSign;
	//�Ƿ�ʼ��б��
	bool                             m_bBeingRamp;
public:
	void                          ControlModel(int mode);
	void                          MoveCameraToModel();	
public:
	//������Ҫƽ����λ��
	std::vector <int>                m_SaveSmoothPos;
	//������Ҫ���¼��㷨�ߵ�λ��
	std::vector <int>                m_SaveReCountNormal;
    //��ǰ���λ��
	MVec3                             m_CurMousePos;
	//��ѡ�е�ģ������
	std::vector <TerrModel*>        m_CurSelectModelSet;
public:
	//�Ƿ�����ƶ�
	bool                            m_bMouseMove;
	//���浱ǰ������Ļ����X,Y
	int                             m_nSaveMouseX,m_nSaveMouseY;
public:
	//�����������Ϣ
    struct    MCopyObjectInfo
	{
		std::string  name;//��Ӧ�ļ���
		MVec3        pos;//λ��
		MVec3        dir;//����
		float        scale;//����
		unsigned  char type;//����
	};
	std::vector <MCopyObjectInfo>          m_CopyObjectInfo;

	void                            CopySelectObject();
	void                            CopySelectObjectToTile();

	void                            MoveSign(MVec3& dir , MVec3& pos );

	void                            SetShowObjectBox(bool bShow );

    void                            ChangePaintSize(int mode );
	void                            ChangePaintPower(int mode );
	
	void                            UpdateOther ( float dtime);

	void                            SetCurPaintInfoData( unsigned char* pData , int bmpSize = 64);

	std::string                     m_curSaveFile;
	
public:
	void                            RoateModel(int dz );

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

	void                            SelectChunkTexInfo(MVec3 dir , MVec3 pos );
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
	//�����б�
	bool                           m_bUpdateLightList;
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
	CHeightMap                       m_HeightMap;
	
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

};

extern TE::MTerrainEdit  g_TerrainEdit;