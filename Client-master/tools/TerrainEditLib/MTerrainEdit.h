
#pragma once

#include "TerrainEditTypes.h"
#include "TerrainBlock.h"
#include "TerrainHeightMap.h"
//#include "GameEngineTypes.h"
#include <deque>

class DBDataManager;
namespace TE
{

//鼠标事件
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

//键盘事件
enum MKEY_EVENT
{
	KeyDown = 0,
	KeyUp   ,
};

//地形层次
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
//编辑事件
enum EIDT_EVENT
{
	UNKOWN_ACTION   = 0 ,
	PANIT_EVN ,            //地表编辑
	CHANGE_HEIGHT_EVN,     //地形编辑
	SELECT_OBJECT_EVN,     //选择物件
	ADD_OBJECT_EVN,        //增加物件
	ADD_MORE_OBJECT_EVN,   //增加大量物件
	MODIFY_LIQUID,         //编辑液体
	MODIFY_PATH,          //编辑路径
	ADD_NPC,               //增加NPC
	AREA_EIDT_EVEN,        //区域编辑
	SELECT_AREA_EVEN,     //选择区域
	MOVE_OBJECT_EVN,       //移动物件 
	ROATE_OBJECT_EVN,      //旋转物件
	SCALE_OBJECT_EVN,      //缩放物件
	DECAL_EVN,              //贴花
	AUTOLINK_OBJECT_EVN,    //刷悬崖
	AUTOLINK_TEXTURE_EVN,   //自动拼接地表
	PAINT_TEXTURE_MODEL_EVN, //通过纹理生成模型
	WALLPAPER_EVN,          //地形块编辑
	USER_INPUT_EVN,          //用户输入
	BRAE_EDIT_EVN,           //斜坡编辑
	LIGHT_EDIT_EVN,          //灯光编辑
    PLACE_PLYAER,              //player预览
	SOUND_EDIT_EVN ,           //声音编辑
	EVEN_NUMBER,
};

//编辑对应说明
const char  g_strEditMode[EVEN_NUMBER][32] =
{
	"未知操作",
	"编辑地表",
	"编辑地形",
	"选择物件",
	"增加物件",
	"增加植被",
	"编辑液体",
	"编辑路径",
	"编辑npc",
	"编辑区域",
	"选择区域",
	"移动物件",
	"旋转物件",
	"缩放物件",
	"贴花",
	"自动拼接模型",
	"自动拼接地表",
	"通过纹理生成植被",
	"地形块编辑",
	"用户输入",
	"斜坡编辑",
	"灯光编辑",
	"地图预览",
	"声音编辑"
};

//改变高度模式
enum  CHANGE_HEIGHT_MODE
{
	 SOFT_MODE = 0, //曲线模式
	 HARD_MODE ,    //线性模式
	 SMOOTH_MODE ,  //平滑
     LEVEL_OFF,     //平整
     RAMP_MODE ,    //斜坡
	 DITCH_MODE ,   //沟渠
};
//裁剪模式
enum  REDUCE_MODE
{
    NO_REDUCE = 0,  //不裁剪
	BEGIN_REDUCE ,   //裁剪
	REDUCE_RESUME,   //恢复
};
//ALP模式
enum  ALPHA_MODE
{
    ALPHA_ADD = 0, //加
	ALPHA_DEC ,    //减
};
//游戏层信息
enum  PAINT_GAME_IFNO
{
	WALK_LAYER = 0 , // 行走层
	WATER_LAYER ,     // 水层
    MAGICBAR_LAYER ,  // 法术层
};

//刷地表模式
enum  PAINT_MODE
{
	PAINT_CIRCLE = 0, //圆形
	PAINT_QUAD,       //方形
	PAINT_RAND_QUAD,  //随机方形
	PAINT_RAND_CIRCLE,  //随机圆形
};
//动作事件
struct ActionEvent
{
   MMOUSE_EVENT  mouseEven; //鼠标事件
   MKEY_EVENT    keyEven;   //键盘事件

   int       mx,my,mz;       //鼠标信息，X,Y,Z

   MVec3     dir,pos;        //屏幕点射线，方向，位置
   bool      bMouseDown;     //是否鼠标按下
   bool      bKeyDown;        //是否键盘按下
   unsigned char  keyCode;    //键盘值
};
//半透明数据	 
struct  ALPAHA_DATA
{
	 unsigned char data[64*64];
};
//纹理名数据
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

//镜头数据
struct MCameraData
{
    MVec3 eyeloc;
	MVec3 goal;
};

//模型操作事件
enum MODEL_ACTION
{
	MODEL_ROATE_Y = 0,//旋转
	MODEL_SCALE ,//缩放
};
//模型类型
enum MODEL_TYPE
{
	TYEP_BIG_MODEL = 0 ,//大物件
	TYPE_SMALL_MODEL , //小物件
	TYPE_DETAIL_MODEL , //细节物件
	TYPE_PLANT_MODEL , //植被物件
	TYPE_PLANT_GRASS, //草层
	TYPE_LINK_MODEL ,//link Model
	TYPE_LINK_TEXTURE ,// Link Texture
	TYPE_PAINT_TEXTURE,// 纹理块
	TYPE_LINK_WATER_MODEL ,// 
	TYPE_PHYSIC_MODEL,//物理模型
	TYPE_MODEL_NUM,//
};

struct EditOptionWater
{
	int chunk_x;
	int chunk_z;
	float waterheight; //水平面的高度:y

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
//编辑属性
struct TerrainEditOption
{
	bool                bMoveGrid; //是否按格子移动物件
	PAINT_GAME_IFNO     gameInfo;  //游戏层信息
	bool                bGetWaterHeight; //是否得到水面高度
	int                 curMaxScale,curMinScale; //当前最大缩放，最小缩放
	unsigned  char      uUVRepeat[4]; //对应纹理层的环绕次数

	MODEL_TYPE          mModelType; //模型类型
	bool                bUserPaintPic; //
	unsigned  char       mColorMap[3]; //但前colormap使用颜色

	MODEL_ACTION         mCurModelAction; //当前编辑模型事件
	EIDT_EVENT           mCurEditEvent;   //当前编辑器事件
	EIDT_EVENT           mSaveCurEditEvent; // 保存当前编辑器事件

	CHANGE_HEIGHT_MODE   mCurChangeHeightMode;//编辑高度模式
    PAINT_MODE           mCurPaintMode;//画刷模式
	ALPHA_MODE           mCurAlphaMode;//半透明混合模式
	REDUCE_MODE          mCurReduceMode;//裁剪模式

	bool                bPickTexture;//是否拾取当前纹理

	int                 nPaintPlantSize;//植物画刷大小
    int                 nPaintVec;//画刷强度
    int                 nCurMoveY;//Y偏移
    int                 nCurLayer;//当前层
    int                 nCurChangeHeightSize;//当前改变高度大小
    int                 nCurPaintSize;//当前画刷大小
    
	float               fCurHeightScale;//当前高度缩放
    int                 nCurSelectTexID;//当前选择纹理ID
    MAP_TEX_INFO        PaintTexInfo[4];//纹理信息，4层
    MAP_TEX_INFO        PlantTexInfo; //
	MVec3               PlantSavePos; //植物保存位置

	int                 nCurSelectPaintTexId[5];//选择的纹理ID ，5层
    int                 nCurSelectPaintRad[6];//当前对应的半径。
    int                 nCurSelectPaintMode[6]; //当前选用的模式
	
	ALPAHA_DATA         alphaPaintData[5];//对应的半透明数据
    bool                bBeingModifyHeight;//是否修改高度

	/*
    char                strLiquidName[128];//液体文件名
    float               fLiquidHeight;//液体高度
	float               fLiquidColor[4];//液体颜色
    float               fSaveLiquidHeight;//保存液体高度
    int                 nCurAddLiquidX,nCurAddLiquidZ;//当前液体对应的X,Z
	*/

	EditOptionWater water;

    int                 nCurAddModelSize; //当前增加模型的范围
    int                 nCurAddModelNum;  //当前增加模型的数量
    int                 nCurCameraTpye;   //镜头的类型
    
	MVec3               nSaveAddPlantPos;  //保存增加植物的位置
    TerrModel*        pSelectModel;      //保存当前选中的模型

    int                 nCurSelectNpcID;   //当前选中NPC ID
	int					CurSelectNpcIndex; //当前选中第几个npc

	bool                bUpdateNpcData;   //是否更新NPC数据
	bool                bUpdateLightData;  //是否更新灯光信息
	bool                bUpdateAreaData;  //是否更新区域信息
	bool                bUpdateWaterData; //是否更新水面信息

	float              fPlantScale;	//新增加处理，植物的缩放

	IEventProcess       *pEventProcess;    //处理事件类指针 （处理各种操作的类）
};
//保存模型信息
struct MModelSaveInfo
{
	MVec3  savePos ;
	MVec3  saveDir ;
	float  saveScale;
};
//编辑贴花类型
enum  MODIFY_DECAL_TYPE
{
	DECAL_NORMAL = 0 ,
	DECAL_WATER , 
};
//保存CHUNK数据
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
//CHUNK数据的集合
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

//编辑地图信息
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
//画刷信息
struct  PaintInfo
{
	unsigned char  data[64*64];
};
//选择纹理信息
struct SelectTexInfo
{
	bool  bSelect;
	int gx ,gy;
};
//chunk纹理的信息
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

//编辑器处理类，主要是面向界面提供需要的接口
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
	void ModifySelPathInfo(int id , unsigned int type , char* description);    ///< 修改当前选择的路径信息
	bool ReadPaths();
	bool WritePaths();

	//type=0: 环境区域，  1: 逻辑区域
	bool AddArea(int id, int type, const char *name, const char *scriptName);

	bool  DelCurPathPos();
	int   DelCurPath();
	void  SetCurPath(int index);    ///< 设置当前编辑路径

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
	//地形编辑对应的数据
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

	//编辑器镜头
   MCamera*            m_pCamera;
   //路径集合
   MPathSet           *m_pPathSet;
   //NPC集合
   MapNpcManager            *m_pNpcSet;      

public: 
	bool               m_bEditDisable;
	MPathSet*         GetPathSet(){return m_pPathSet;};

	//标记
   MSignSet           *m_pSignSet;   

   //编辑属性
	TerrainEditOption   m_EditOption;
	TerrainDrawOption m_DrawOption;
	MTerrainCull   *m_Cull;

	SelectedResData m_SelResData;
	int m_PreviewOrRadar; //0-小地图,  1-模型贴图预览
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
	//当前选择信息
     Select_Info       m_curSelectInfo;
	 //保存选择信息
	 Select_Info       m_saveSelectInfo;
public:
	//当前选择模型
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
		//增加参数，决定每个chunk分多少小格子
		int gwidthnum ,int gheightnum);

    //清楚数据
	void    ClearAll();
public:
	struct NormalInfo
	{
		unsigned char gx,gz;
		unsigned char vecIndex;
	};
	//需要改变的法线数组
	std::vector <NormalInfo>     m_ChangeNormalSet;
public:
    void                         CreatNavMap(int gx,int gz,float height);

public:
	Coordinate*                GetCoordinate(){ return m_pCoordinate;}; 
private:
    //x,y,z坐标系
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
   //保存修改顶点信息
   struct   SaveModifyVecInfo
   {
	   int  gx,gz;
	   int  vecIdx;
	   float  height;
	   bool   bCount;
	   float  lenth;
	   float  range;
   };
   //保存修改顶点信息数组
   std::vector <SaveModifyVecInfo>      m_SaveModifyVecInfo;
   //平整结构
   struct LevelOffInfo
   {
	   bool  bCount;
	   float fHeight;
   };
   //平整信息数组
   LevelOffInfo                         m_SaveLevelOffInfo;
   //删除选中NPC ;参数( 索引 )
   void                                 DeleteSelectNpc( int index );
   //获得当前选中的点
   void                                 GetCurSelectVec();
public:
	void  SmoothHeightMap( int width ,int height );
	   
	//主要应用于多种密度的网格
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
	//斜坡显示
	SlopeSign                       *m_pCampSign;
	//是否开始拉斜坡
	bool                             m_bBeingRamp;
public:
	void                          ControlModel(int mode);
	void                          MoveCameraToModel();	
public:
	//保存需要平滑的位置
	std::vector <int>                m_SaveSmoothPos;
	//保存需要从新计算法线的位置
	std::vector <int>                m_SaveReCountNormal;
    //当前鼠标位置
	MVec3                             m_CurMousePos;
	//被选中的模型数组
	std::vector <TerrModel*>        m_CurSelectModelSet;
public:
	//是否鼠标移动
	bool                            m_bMouseMove;
	//保存当前鼠标的屏幕坐标X,Y
	int                             m_nSaveMouseX,m_nSaveMouseY;
public:
	//拷贝物体的信息
    struct    MCopyObjectInfo
	{
		std::string  name;//对应文件名
		MVec3        pos;//位置
		MVec3        dir;//方向
		float        scale;//缩放
		unsigned  char type;//类型
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

	//保存当前刷行走层，水层等信息为地图导航信息
	void                               SaveCurNaviMap(int gx,int gz);
	//设置信息对应的信息颜色
	void                               SetModifyMapInfoColor( int layer , unsigned char r, unsigned char g, unsigned char b);
public:
	MChunkTextureInfo                 m_ChunkTextureInfo[16][16];

public:
	//更新列表
	bool                           m_bUpdateLightList;
	//处理右键弹出灯光信息对话框
	bool                            m_bOpenLightInfoDlg;	
	bool                            m_bShowLightInfoDlg;
	//处理右键弹出区域选项对话框
	bool                            m_bOpenAreaInfoDlg;
	bool                            m_bShowAreaInfoDlg;
	//处理非模态分层水设置对话框
	bool                            m_bOpenTwoWaterDlg;
	bool                            m_bShowTwoWaterDlg;

	bool                            m_bOpenTextureInfoDlg;	
	bool                            m_bShowTextureInfoDlg;
	//处理右键弹出模型信息菜单控制
	bool                            m_bOpenModelInfoDlg;	
	bool                            m_bShowModelInfoDlg ;

	bool                             m_bPessQ;
	/// 更新区域设置对话框属性
	bool                            m_bUpdateAmbientSettingDlg;

public:
	CHeightMap                       m_HeightMap;
	
public:
	COORDINATE_SELECT                m_select_coordinate;
	
public:
	void                            SetWaterInfo( float weight,float UVWrap,float speed, int angle) ;
	void                            SetCurBaseTexture( const char* filename );
	
private:
	//自动拼接崖壁实现
	LinkMeshManager                    *m_AutoLinkMesh;
	//自动拼接纹理
	LinkTextureManager                  *m_AutoLinkTex;
	//单独拼地表
	LinkOverlayManager                   *m_PaintWallPaper;
public:
	LinkMeshManager*                   GetAutoLinkMesh(){ return m_AutoLinkMesh ;}
	LinkTextureManager*                GetAutoLinkTex(){ return m_AutoLinkTex ;}
	LinkOverlayManager*                 GetPaintWallPaper(){return m_PaintWallPaper ;} 

	//产生水的边缘浪花
	void                             CreateWhiteWater();

public:
	void                              SetAutoDelBlock( int nDel );
	void                              UpdateBreaHeight();

	void                              UpdateProcessData();
	
public:
	//是否绘制纹理窗口
	bool                              m_bDrawTextureWnd;
	//是否绘制模型窗口
	bool                              m_bDrawModelWnd;

	Ogre::RenderLines *m_pLinesRenderer;

};

};

extern TE::MTerrainEdit  g_TerrainEdit;