
#include "MTerrainEdit.h"
#include "MTerrain.h"
#include "MNPC.h"
#include "MapLights.h"
#include "MStream.h"
#include "EditorModel.h"
#include "MSign.h"
#include "Coordinate.h"
#include "SlopeMark.h"
#include "StripBase.h"
#include "EditorCamera.h"
#include "EnvRegion.h"
#include "MTexManager.h"
#include "LinkTile.h"
#include "MEventProcessFactory.h"
#include "MModifyHeightProcess.h"
#include "MPaintTextureProcess.h"
#include "MPaintTextureModelProcess.h"
#include "MNavigationMap.h"
#include "MInputProcess.h"
#include "MTerrainEditMsgProcess.h"
#include "MBraeEditProcess.h"
#include "MAddObjectProcess.h"
#include "MTileProcess.h"
#include "MNavMapProcess.h"
#include "MAddNpcProcess.h"
#include "MTerrainCull.h"

//#include   <algorithm>   
//#include   <string> 
//#include   <functional> 
#include "OgreRoot.h"
#include "OgreModel.h"
#include "OgreLight.h"
#include "OgreGameScene.h"
#include "OgreSceneRenderer.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreCamera.h"
#include "OgreGameTerrainScene.h"
#include "OgreUIRenderer.h"
#include "OgreRenderWindow.h"
#include "OgreTexture.h"
#include "OgreScreenTexture.h"
#include "OgreRenderSystem.h"
#include "OgreRay.h"
#include "ShowActor.h"
#include <Windows.h>
#include "GameDBData.h"
#include "GameEngineTypes.h"
#include "AvatarManager.h"
#include "AvatarEntity.h"

using namespace TE;
using namespace Ogre;

Root *g_pEngine;
extern void *          g_Hinst;
std::string g_skyFile ;
unsigned long g_dwCurTime;

MVec3         g_LightDir;
Camera *g_pCamera;
Camera*   g_pSaveCamera;
extern std::string  g_path ;

//extern MVec4 g_WorldAmbientLight;
//extern MVec4 g_WorldDirLight;
extern Light *g_plightnode;
extern bool  g_DrawModelType[TYPE_MODEL_NUM];

Vector3 g_Viewpoint(0,5.0f, -10.0f);
float   g_ViewPitch = 0;
float   g_ViewYaw = 0;
float   g_fFov = 30.0f ;
float  g_fCameraFar = 30000.0f ;
float  g_fCameraNear = 100.0f;


bool g_bEnableFog;

#define BUILDING_PATH  "actor.wow.World.wmo."

#define KEY_CONTROL_MAX_NUM   256
//编辑快捷键
char  g_KeySetting[KEY_CONTROL_MAX_NUM];
	
BITMAPFILEHEADER g_header;		
BITMAPINFOHEADER  g_headInfo;

float g_fTileSize = TERRTILE_SIZE / 6.0f ;			
float g_fModelSignRad = 20.0f;

MTerrainEdit g_TerrainEdit;

float g_fCos[9];
bool  g_bUseOtherCamera = false;

bool      g_bNewMap = false;

extern void  MMaxVec(MVec3 *srcVec , MVec3 *desVec);
extern void  MMinVec(MVec3 *srcVec , MVec3 *desVec);

enum
{
	RENDERUNIT_MAINVIEW = 0,
	RENDERUNIT_RESPREVIEW,
	RENDERUNIT_RADARVIEW
};

static int GetMapIDFromPath(const char *filename)
{
	char buffer[256];
	const char *psep = strrchr(filename, '\\');
	if(psep == NULL) return 0;

	strcpy(buffer, psep+1);
	char *pdot = strrchr(buffer, '.');
	if(pdot) *pdot = 0;

	return atoi(buffer);
}

static void ComputeModelViewCamera(MCameraData &camdata, const BoxSphereBound &bound)
{
	camdata.eyeloc = bound.getCenter() + bound.getExtension()*2.5f;
	camdata.goal = bound.getCenter();
}

static void RenderRadarMap()
{
	if(g_TerrainEdit.m_pResPreviewRU == NULL) return;

	UIRenderer *puirender = static_cast<UIRenderer *>(g_TerrainEdit.m_pResPreviewRU->m_pUIRenderer);
	RECT rect;
	::GetClientRect((HWND)g_TerrainEdit.m_pResPreviewRU->m_pTarget->getHWnd(), &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	puirender->DrawBar(0, 0, w, h, 0xff000000);

	unsigned long gridcolor = 0x80808080;
	int step = h/16;
	for(int y=0; y<16; y++)
	{
		puirender->DrawLine(0, y*step, w, y*step, gridcolor);
	}
	step = w/16;
	for(int x=0; x<16; x++)
	{
		puirender->DrawLine(x*step, 0, x*step, h, gridcolor);
	}

	Vector3 pos = g_pCamera->getEyeLoc().toVector3();
	int x = int(pos.x/TERRTILE_SIZE*w);
	int y = int(pos.z/TERRTILE_SIZE*h);

	puirender->DrawBox(x-5, y-5, 10, 10, 0xffff0000);
}

void TexturePreviewRenderFunc()
{
	if(g_TerrainEdit.m_pResPreviewRU==NULL || g_TerrainEdit.m_pResPreviewRU->m_pTarget==NULL)
	{
		return;
	}

	UIRenderer *puirender = static_cast<UIRenderer *>(g_TerrainEdit.m_pResPreviewRU->m_pUIRenderer);
	RECT rect;
	::GetClientRect((HWND)g_TerrainEdit.m_pResPreviewRU->m_pTarget->getHWnd(), &rect);

	if(!g_TerrainEdit.m_PreviewOrRadar)
	{
		RenderRadarMap();
		return;
	}

	Texture *ptex = g_TerrainEdit.m_SelResData.ptexture;
	if(ptex == NULL)
	{
		return;
	}
	TextureDesc desc;
	ptex->getDesc(desc);

	ScreenTexture *scrtex = new ScreenTexture(ptex);
	scrtex->SetBlendMode(BLEND_OPAQUE);
	scrtex->AddStretchScreenRect(0, 0, float(rect.right), float(rect.bottom), 0, 0, desc.width, desc.height);
	puirender->DrawScreenTexture(scrtex);
	scrtex->release();
}
RenderUnit::RenderUnit(void *hwnd, int type) : m_pUIRenderer(NULL), m_Type(type)
{
	RECT rect;
	GetClientRect((HWND)hwnd, &rect);
	int priority;

	if(type == RENDERUNIT_MAINVIEW)
	{
		m_pTarget = RenderSystem::getSingleton().getMainWindow();

		m_pScene = &g_Terrain;
		m_pScene->addRef();
		priority = 0;

		m_pCamera = g_pCamera;
		m_pCamera->addRef();
	}
	else
	{
		RenderSystem::InitDesc desc;
		memset(&desc, 0, sizeof(desc));

		desc.width = rect.right - rect.left;
		desc.height = rect.bottom - rect.top;
		desc.colorbits = 32;
		desc.alphabits = 8;
		desc.depthbits = 16;
		desc.stencilbits = 0;
		desc.hwnd = (void *)hwnd;

		if(hwnd == NULL) m_pTarget = NULL;
		else m_pTarget = RenderSystem::getSingleton().createRenderWindow(desc);

		m_pScene = new SimpleGameScene;
		if(type == RENDERUNIT_RADARVIEW) priority = 1;
		else priority = 2;

		m_pCamera = new Camera;
		m_pCamera->setFov(30.0f);
		m_pCamera->setLookAt(Vector3(0, 200.0f, -200.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		m_pCamera->update(0);

		LightNode *plightnode = new Light(LT_DIRECT);
		plightnode->setColor(ColourValue(0.8f, 0.8f, 0.8f, 1.0f));
		plightnode->m_Attenuation = 0.0f;
		plightnode->m_AmbientColor = ColourValue(0.2f, 0.2f, 0.2f, 1.0f);
		plightnode->enableAmbient();
		plightnode->setPosition(Vector3(-300.0f, 300.0f, -300.0f));
		plightnode->setRotation (60.0f, 60.0f, 60.0f);
		plightnode->update(0);

		plightnode->attachToScene(m_pScene);
		OGRE_RELEASE(plightnode);
	}

	m_pRenderer = new NormalSceneRenderer();
	setClearColor(0xff004000);
	m_pRenderer->setRenderTarget(m_pTarget);
	m_pRenderer->setRenderScene(m_pScene);
	m_pRenderer->setCamera(m_pCamera);

	SceneManager::getSingleton().addSceneRenderer(priority, m_pRenderer);

	if(type == RENDERUNIT_RESPREVIEW)
	{
		m_pUIRenderer = new UIRenderer;
		m_pUIRenderer->setPreRenderCallback(TexturePreviewRenderFunc);
		SceneManager::getSingleton().addSceneRenderer(priority+1, m_pUIRenderer);
	}
}

RenderUnit::~RenderUnit()
{
	SceneManager::getSingleton().removeSceneRenderer(m_pRenderer);

	delete m_pRenderer;
	delete m_pUIRenderer;
	OGRE_RELEASE(m_pCamera);
	OGRE_RELEASE(m_pScene);
}

void RenderUnit::update(unsigned int dtick)
{
	if(m_Type == RENDERUNIT_RESPREVIEW)
	{
		m_pCamera->setLookAt(g_TerrainEdit.m_ResPreviewCamera.eyeloc, g_TerrainEdit.m_ResPreviewCamera.goal, Vector3(0, 1.0f, 0));
	}

	m_pCamera->update(dtick);
}

void RenderUnit::setClearColor(unsigned int color)
{
	m_pRenderer->setClearParams(CLEAR_ZBUFFER|CLEAR_TARGET, color, 1.0f, 0);
}

MTerrainEdit::MTerrainEdit(void) : m_bInit(false), m_pEditData(NULL),m_bEditDisable(false),m_pCamera(NULL), m_pCurTerrain(NULL)
{
	memset(&m_EditOption, 0, sizeof(TerrainEditOption));
	m_EditOption.fPlantScale = 1.0f ;
	m_EditOption.fCurHeightScale = 1.0f;
	m_EditOption.nCurChangeHeightSize = 1; 

	memset(&m_DrawOption, 0, sizeof(m_DrawOption));
	m_DrawOption.bDrawChunk = true;
	m_DrawOption.bDrawModel = true;
	m_DrawOption.bDrawQuad = true;
	m_DrawOption.bDrawLiquid = true;
	m_DrawOption.bDrawPlant = true;
	

	for( int i = 0 ; i < 9 ; i++)
	{
		g_fCos[i] = (float)cos( i * MAY_PI / 16.0f );
	}

	m_PreviewOrRadar = 1;

	m_pPathSet = new MPathSet;
	m_pNpcSet = new MapNpcManager;
	m_pSignSet = NULL;
	m_nCurSaveDataIndex = 0;

	m_SaveLevelOffInfo.bCount = false;

	memset(&m_GameMapInfo, 0, sizeof(m_GameMapInfo));
	m_GameMapInfo.texrepeat = 12;

	m_bBeingRamp = false;

	m_CurMousePos.x = m_CurMousePos.y = m_CurMousePos.z = 0;

	m_bShowObjectBox   = true;
	m_bUpdateHeightmap = false;

	m_DecalType = DECAL_NORMAL ;

	memset( &m_CurSelectTexInfo , 0 , sizeof( SelectTexInfo )) ;

	m_bAltCtrlMoveCamera = false;

	LoadBmpHeader("head.bmp");

	m_bDrawTextureWnd = true ;
	m_bDrawModelWnd = true ;
}

//引导地图当前的灯光信息 ; 返回值为 true 表示成功
bool  MTerrainEdit::LoadCurMapLightInfo()
{
	if( m_pEditData == NULL )
		return false;

	if( m_curSaveFile.empty() )
		return false;

	std::string  fileName = m_curSaveFile + ".light" ;

	if( g_LightManager.LoadLightInfo( fileName.c_str() , this ) )
	{
		this->m_bUpdateLightList = true;
		return true;
	}

	return false;

};

//引导BMP文件头，用于保存对应的其他BMP地图信息 ；参数 （ 文件名 ）
void MTerrainEdit::LoadBmpHeader( const char* filename )
{
     MStream  stream;
	 stream.ReadFile  ( filename );

	stream.Read ( g_header );		
    stream.Read ( g_headInfo );
}
	
//移动当前选择模型Y方向 ； 参数 （ 移动距离 ）
void  MTerrainEdit::MoveShowModelY(int pos )
{
	if(m_SelResData.pmodel)
	{
		m_SelResData.pos.y += pos;

		Model *pnode = ((EditorModel*)m_SelResData.pmodel)->GetShowModel()->getMainModel();
		pnode->setPosition(m_SelResData.pos);
	}
}

//移动当前选择模型Z方向 ； 参数 （ 移动距离 ）
void  MTerrainEdit::MoveShowModelZ(int pos)
{
	if(m_SelResData.pmodel)
	{
		m_SelResData.pos.z += pos;

		Model *pnode = ((EditorModel*)m_SelResData.pmodel)->GetShowModel()->getMainModel();
		pnode->setPosition(m_SelResData.pos);
	}
};
	
//旋转物体 yaw 角度 ；参数 （ YAW )
void  MTerrainEdit::RoateShowModel(float rot)
{
	if(m_SelResData.pmodel)
	{
		m_SelResData.dir += rot;
		if(m_SelResData.dir < 0) m_SelResData.dir += 360.0f;
		if(m_SelResData.dir >= 360.0f) m_SelResData.dir -= 360.0f;

		Entity *pnode = ((EditorModel*)m_SelResData.pmodel)->GetShowModel();
		pnode->setRotation(m_SelResData.dir, 0, 0);
	}
};

MTerrainEdit::~MTerrainEdit(void)
{
    Release();
}	
		
//修改选中模型位置 ； 参数 （ 位置 ）
void   MTerrainEdit::ModifySelectModelPos( MVec3 pos )
{
    if( m_EditOption.pSelectModel == NULL )
		return;

	m_EditOption.pSelectModel->GetModelData ()->pos = pos;
	m_EditOption.pSelectModel->ResetMat();
};
	
//修改选中模型旋转； 参数 （ 旋转角度 ）
void   MTerrainEdit::ModifySelectModelDir( MVec3 dir )
{
    if( m_EditOption.pSelectModel == NULL )
		return;

    m_EditOption.pSelectModel->GetModelData ()->dir = dir;
	m_EditOption.pSelectModel->ResetMat();
};

//修改选中模型缩放； 参数（缩放）
void   MTerrainEdit::ModifySelectModelScale( float scale )
{
    if( m_EditOption.pSelectModel == NULL )
		return;
	 
	m_EditOption.pSelectModel->GetModelData ()->scale = scale / 100.f ;
	m_EditOption.pSelectModel->ResetMat();
};	

//获得当前选中的点  
void   MTerrainEdit::GetCurSelectVec()
{
	int gx,gz ;
	int vecIdx;
	gx = m_curSelectInfo.gx ;
	gz = m_curSelectInfo.gy ;
	vecIdx = m_curSelectInfo.vx + 17*m_curSelectInfo.vy ;

};

//旋转选中模型 ，参数 ( yaw )
void  MTerrainEdit::RoateModel(int dz )
{   
	 if( m_pCurSelectModel == NULL)
		 return;

	 AddSaveUndoData();	
	 SaveCurData(0,0,0);

	float roate = dz*0.01f;
	
	m_pCurSelectModel->GetModelData ()->dir .y += roate;

	m_pCurSelectModel->ResetMat ();

	for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i++)
	{ 
		m_CurSelectModelSet[i]->GetModelData ()->dir .y += roate;
		m_CurSelectModelSet[i]->ResetMat();
	}
}

//控制模型，对NPC进行简单旋转 ；参数（模式）
void  MTerrainEdit::ControlModel(int mode)
{
	if( m_pCurSelectModel == NULL)
		return;

	//提供简单旋转NPC模式
	if( m_EditOption.mCurEditEvent == ADD_NPC )
	{
		if ( mode == 0 )
		{
			RoateYSelectModel( 120 );
		}
		else
		{
			RoateYSelectModel( -120 );
		}
		return;
	}

	 AddSaveUndoData();	
	 SaveCurData(0,0,0);

	if ( mode == 0 )
	{
		switch(  m_EditOption.mCurEditEvent )
		{
		case MOVE_OBJECT_EVN:
			MoveSelectModel(1.0f);
			break;
		case ROATE_OBJECT_EVN:      
			RoateYSelectModel( 120 );
			break;
		case SCALE_OBJECT_EVN:	
			ZoomSelectModel( 100 );
			break;
		default:
		    LOG_WARNING( "MTerrainEdit::ControlModel(int mode) mode 错误" );
			break;
		}
	}
	else
	{
		switch(  m_EditOption.mCurEditEvent )
		{
		case MOVE_OBJECT_EVN:
			MoveSelectModel(-1.0f);
			break;
		case ROATE_OBJECT_EVN:      
			RoateYSelectModel( -120 );
			break;
		case SCALE_OBJECT_EVN:	
			ZoomSelectModel( -100 );
			break;
		default:
		    LOG_WARNING( "MTerrainEdit::ControlModel(int mode) mode 错误" );
			break;
		}
	}
};

//得到对应纹理层的环绕次数；参数( 第几层 ） ；返回值  -1 表示失败
int   MTerrainEdit::GetCurTexRepeat( int nLayer )
{	
	if( !m_pEditData )
		return -1;

   	if ( !m_CurSelectTexInfo.bSelect )
		return -1;

	if( m_CurSelectTexInfo.gx < 0 || m_CurSelectTexInfo.gx >= m_pEditData->gw )
		return -1;

	if( m_CurSelectTexInfo.gy < 0 || m_CurSelectTexInfo.gy >= m_pEditData->gh )
		return -1;

	if( nLayer < 0 || nLayer >= 4 )
		return -1;

	return (int)m_pEditData->chunkSet[m_CurSelectTexInfo.gy ][m_CurSelectTexInfo.gx ].GetChunkData ()->texUVInfo[nLayer];
};

//设置当前层对应的纹理环绕次数；参数（ 第几层，次数 ）
void  MTerrainEdit::SetCurTexRepeat( int nLayer , int nRepeat )
{	
	if( !m_pEditData )
		return;
	
	if ( !m_CurSelectTexInfo.bSelect )
		return;

	if( nLayer < 0 || nLayer >= 4 )
	{
		LOG_WARNING( "SetCurTexRepeat( int nLayer , int nRepeat ) nlayer值错误 %d " , nLayer );
		return;
	}

	m_EditOption.uUVRepeat [ nLayer ] = nRepeat;

	m_pEditData->chunkSet[m_CurSelectTexInfo.gy ][m_CurSelectTexInfo.gx ].GetChunkData ()->texUVInfo[nLayer] = (float)nRepeat;
};

//增加数据以便做redo操作
void  MTerrainEdit::AddSaveRedoData()
{
	if( m_pEditData == NULL )
		return ;

	if( m_SaveData.empty () )
		return;

     MSaveChunkDataSet   newdata;

	 size_t nSize = m_SaveReDoData.size ();
	
	 if(m_nCurSaveDataIndex  < (int)m_SaveReDoData.size ())
	 {
		 for( size_t i = 0 ; i <  nSize - m_nCurSaveDataIndex  ; i++)
		 {
			 m_SaveReDoData.pop_back ();
		 }
	 }
	 	 
	 m_SaveReDoData.push_back ( newdata );	 
			
	 //测试时暂时只允许保存６历史纪录
	 if(m_SaveReDoData.size () >= 6)
	 {
         m_SaveReDoData.pop_front ();
	 }

	 if( m_SaveReDoData.size () != m_SaveReDoData.size ())
		 return;
    
	 if(m_nCurSaveDataIndex >= (int)m_SaveReDoData.size ())
		 return;
	 
	 for( unsigned int i = 0 ; i < m_SaveData[m_nCurSaveDataIndex].m_SaveSet .size () ; i++)
	 {
		 SaveCurData( m_SaveData[m_nCurSaveDataIndex].m_SaveSet [i]->gx , m_SaveData[m_nCurSaveDataIndex].m_SaveSet [i]->gz , 1);
	 }
};
	
//从新设置当前编辑状态
void  MTerrainEdit::ReSetCurEditEvent()
{
	if( m_EditOption.mCurEditEvent == ADD_NPC )
	{
		m_EditOption.mCurEditEvent = m_EditOption.mSaveCurEditEvent ;
							
		m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess( m_EditOption.mCurEditEvent );
	} 
}

//保存当前编辑事件
void  MTerrainEdit::SaveCurEditEvent()
{
    if( m_EditOption.mCurEditEvent != ADD_NPC )
	{
		m_EditOption.mSaveCurEditEvent = m_EditOption.mCurEditEvent;
	}
};

//设置当前文件 ；参数（文件名）
void  MTerrainEdit::SetCurBaseTexture( const char* filename )
{
    RECT ClientR;
    GetClientRect((HWND)m_hwndTexShowDlg, &ClientR );
};

//设置水的信息；参数（ 权重，UV次数，速度，角度 )
void  MTerrainEdit::SetWaterInfo( float weight,float UVWrap,float speed, int angle) 
{
	MVec2 vec;
	vec.x = sin( (angle / 360.0f) * 2*MAY_PI);
	vec.y = cos( (angle / 360.0f)* 2*MAY_PI);

/*
     if( g_pRoom )
	 {
		 g_pRoom->SetWavePara( 1, weight , UVWrap , speed , vec );
	 }
	 else if ( g_pTerrainRoom )
	 {
		 g_pTerrainRoom->SetWavePara( 1, weight , UVWrap , speed , vec );
	 }
	 */
};	
 
//设置阴影的强度；参数（ 强度 )
void MTerrainEdit::SetShadowStrength(float fStrength)
{

}

//设置光照强度；参数（强度）
void MTerrainEdit::SetLightStrength(float fStrength)
{
	/*
	if( g_pRoom )
	{
		g_pRoom->SetLightMultiply( fStrength );
	}

	if( g_pTerrainRoom )
	{
		g_pTerrainRoom->SetLightMultiply( fStrength );
	}
	*/
};

//增加保存Undo数据
void  MTerrainEdit::AddSaveUndoData()
{
     MSaveChunkDataSet   newdata;
	 memset( &newdata, 0 , sizeof( MSaveChunkDataSet ));

	 size_t nSize = m_SaveData.size ();

	 if(m_nCurSaveDataIndex +1 < (int)m_SaveData.size ())
	 {
		 for( size_t i = 0 ; i < nSize - m_nCurSaveDataIndex -1 ; i++)
		 {
			 m_SaveData.pop_back ();
		 }
	 }

	 m_SaveData.push_back ( newdata );	 
	 //测试时暂时只允许保存６历史纪录
	 if(m_SaveData.size () >= 6)
	 {
         m_SaveData.pop_front ();
	 }
	
	 m_nCurSaveDataIndex = int(m_SaveData.size ()) -1;
}

//释放对应数据
    
//保存当前数据；参数（格子X，格子Z，模式）
bool MTerrainEdit::SaveCurData(int gx,int gz,int mode)
{
	if( m_nCurSaveDataIndex < 0 )
		m_nCurSaveDataIndex = 0;

	if(m_nCurSaveDataIndex < 0 || m_nCurSaveDataIndex >= (int)m_SaveData.size ())
		return false; 

	if( mode == 0)
	{
		for( int i = 0 ; i < (int)m_SaveData[m_nCurSaveDataIndex].m_SaveSet .size () ; i++)
		{
			if ( m_SaveData[m_nCurSaveDataIndex].m_SaveSet[i]->gx  == gx &&
				m_SaveData[m_nCurSaveDataIndex].m_SaveSet[i]->gz  == gz )
				return true;
		}
	}
	else
	{
		if(m_nCurSaveDataIndex >= (int)m_SaveReDoData.size ())
			return false;

		for( int i = 0 ; i < (int)m_SaveReDoData[m_nCurSaveDataIndex].m_SaveSet .size () ; i++)
		{
			if ( m_SaveReDoData[m_nCurSaveDataIndex].m_SaveSet[i]->gx  == gx &&
				m_SaveReDoData[m_nCurSaveDataIndex].m_SaveSet[i]->gz  == gz )
				return true;
		}
	}

	MSaveChunkData   *saveData = new MSaveChunkData;
	memset(saveData , 0 ,sizeof( MSaveChunkData ));

	saveData->gx = gx;
	saveData->gz = gz;

	saveData->saveEditEvent = m_EditOption.mCurEditEvent ;

	switch( m_EditOption.mCurEditEvent)
	{	
	case PANIT_EVN:
		{
			memcpy(&saveData->saveChunkData , m_pEditData->chunkSet [gz][gx].GetChunkData (),sizeof( TerrBlockData ));	
		}
		break;
	case ADD_OBJECT_EVN :
		{
			
		}
		break;
	case MOVE_OBJECT_EVN:
	case ROATE_OBJECT_EVN:
	case SCALE_OBJECT_EVN:
		{
			if(m_pCurSelectModel)
			{
				saveData->pSaveModel = m_pCurSelectModel;
				saveData->savePos = m_pCurSelectModel->GetModelData ()->pos ;
				saveData->saveDir = m_pCurSelectModel->GetModelData ()->dir ;
				saveData->saveScale = m_pCurSelectModel->GetModelData ()->scale ;

				for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i ++ )
				{
					MModelSaveInfo saveInfo;
					saveInfo.savePos = m_CurSelectModelSet[i]->GetModelData ()->pos ;
					saveInfo.saveDir = m_CurSelectModelSet[i]->GetModelData ()->dir ;
					saveInfo.saveScale = m_CurSelectModelSet[i]->GetModelData ()->scale ;

					saveData->SaveSelectModelSet.push_back ( saveInfo );
				}
			}
			else
			{
				saveData->pSaveModel = NULL;
			}
		}
		break;
	case CHANGE_HEIGHT_EVN:
		{
			memcpy(&saveData->saveChunkData , m_pEditData->chunkSet [gz][gx].GetChunkData (),sizeof( TerrBlockData ));	
		}
		break;
;
	case ADD_MORE_OBJECT_EVN:
		{

		}
		break;
	case MODIFY_LIQUID:
		{

		}
		break;
	case 	MODIFY_PATH:
		{
		}
		break;
	case ADD_NPC:
		{
		}
		break;
	default:
		{
			LOG_WARNING("SaveCurData(int gx,int gz,int mode) 错误!");
		}
		break;
	}

	if(mode == 0)
	{
		m_SaveData[m_nCurSaveDataIndex].m_SaveSet.push_back ( saveData );
	}
	else
	{
		m_SaveReDoData[m_nCurSaveDataIndex].m_SaveSet.push_back ( saveData );
	}
  
	return false;
};


void MTerrainEdit::UpdateLiqduiOption(TE::EditOptionWater &water)
{
	if(m_pEditData == NULL) return;

	if(water.chunk_x<0 || water.chunk_z<0) return;
	m_pEditData->chunkSet[water.chunk_z][water.chunk_x].UpdateLiquid(water);
}

//前进操作
void    MTerrainEdit::Redo ()
{
	if( m_EditOption.pEventProcess )
	{
		m_EditOption.pEventProcess->Redo( this );
	}
}
    
//得到当前编辑模式文本；返回（对应的编辑说明）
const char*     MTerrainEdit::GetStrCurEditMode()
{
     return g_strEditMode[m_EditOption.mCurEditEvent ];
};

//Undo操作
void    MTerrainEdit::Undo ()
{
	if( m_EditOption.pEventProcess )
	{
		m_EditOption.pEventProcess->Undo( this );
	}
}

//保存当前高度图；参数（文件名）；返回（为真表示成功 )
bool MTerrainEdit::SaveCurHeightMap(const char* filename)
{
	MModifyHeightProcess* pProcess = static_cast<MModifyHeightProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	return pProcess->SaveCurHeightMap( this , filename );
}

//旋转选种物体；参数（ x, y , z);
void MTerrainEdit::Roate(float x, float y ,float z)
{
    if(m_pCurSelectModel)
	{
		m_pCurSelectModel->GetModelData ()->dir . x += x ;
		m_pCurSelectModel->GetModelData ()->dir . y += y;
		m_pCurSelectModel->GetModelData ()->dir . z += z;
		m_pCurSelectModel->ResetMat ();
	}
};	

// 改变画刷f度；参数（对应模式 ）
void MTerrainEdit::ChangePaintPower(int mode )
{
	switch( m_EditOption.mCurEditEvent  )
	{
	case 	PANIT_EVN :
		{
			m_EditOption.nPaintVec += mode;

			if( m_EditOption.nPaintVec < 1 ) m_EditOption.nPaintVec = 1;
			if( m_EditOption.nPaintVec >=64 )m_EditOption.nPaintVec = 64; 
		
			SetPaintVec(m_EditOption.nPaintVec);	
			CreateAlphaPaint();
		}
		break;
	case CHANGE_HEIGHT_EVN:
		{  
			float height = m_EditOption.fCurHeightScale*30 + 20*mode;

			if( height < 1 ) 
				height = 1;
			if( height >1000 )
				height = 1000;

			SetHeightScale( (int)height );
		}
		break;
	default:
		{
			LOG_WARNING("ChangePaintPower(int mode ) 错误!");
		}
		break;
	}
}

//改变画刷大小；参数（模式）
void MTerrainEdit::ChangePaintSize(int mode )
{
	switch( m_EditOption.mCurEditEvent  )
	{
	case 	PANIT_EVN :
		{
			m_EditOption.nPaintPlantSize += mode ;

			if( m_EditOption.nPaintPlantSize > 32 )
				m_EditOption.nPaintPlantSize = 32 ;
			if( m_EditOption.nPaintPlantSize < 1 )
				m_EditOption.nPaintPlantSize = 1;

			SetCurAlphaSize(m_EditOption.nPaintPlantSize);
		}
		break;
	case CHANGE_HEIGHT_EVN:
		{  
			m_EditOption.nCurChangeHeightSize += mode ;

			if( m_EditOption.nCurChangeHeightSize > 9 )
				m_EditOption.nCurChangeHeightSize = 9;
			if( m_EditOption.nCurChangeHeightSize < 1 )
				m_EditOption.nCurChangeHeightSize = 1;

			SetChangeHeightSize( m_EditOption.nCurChangeHeightSize );
		}
		break;
	default:
		{
			LOG_WARNING("ChangePaintSize(int mode ) 错误!");
		}
		break;
	}
}

//消息处理
void MTerrainEdit::MsgProc(void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	switch(msg)
	{
	case WM_SIZE:
		{ 
			int width,height;
			width = LOWORD(lParam) ;
			height = HIWORD(lParam) ;
			g_Camera.m_ScreenWidth = width;
			g_Camera.m_ScreenHeight = height;
			g_Camera.m_PreviewHWND= (HWND)hWnd;

			if(g_pCamera)
			{
				g_pCamera->setViewport(0,0,width,height);
				g_pCamera->setRatio(float(width)/float(height));
			}

			if(m_pMainRU)
			{
				m_pMainRU->m_pTarget->onSizeOrMove();
			}
		}
		break;
	}

	g_TerrainEditMsgProcess.MsgProc(this , (HWND)hWnd , msg , wParam , lParam);
};

//移动镜头给模型
void   MTerrainEdit::MoveCameraToModel()
{
	if( m_pCurSelectModel )
	{
		EditorModel*  pModel;
		Model*  pNode;

	   pModel = (EditorModel*)m_pCurSelectModel->GetModelData ()->pModel ;
 
	   pNode = pModel->GetShowModel() ->getMainModel ();

	   const BoxSphereBound &bound = pNode->getWorldBounds();
	   ComputeModelViewCamera(m_ResPreviewCamera, bound);
	}
}

//裁剪当前格子；参数（镜头方向，位置）；返回（为真表示裁剪成功 ）
bool   MTerrainEdit::CullCurGrid(MVec3 &dir, MVec3 &pos)
{
 	MVec3  pos0;
	if(g_Terrain.TestAndRay(dir,pos,pos0))
	{
		int gx , gz;

		g_Terrain.GetGridX(pos0.x,&gx);
		g_Terrain.GetGridZ(pos0.z,&gz);
  		
		int  mdx,mdz;
		mdx = (int)(m_pEditData->chunkSet [gz][gx].GetGridPosX(pos0.x)/TERRBLOCK_SIZE*8);
		mdz = (int)(m_pEditData->chunkSet [gz][gx].GetGridPosZ(pos0.z)/TERRBLOCK_SIZE*8);

		if( mdx >= 8 || mdz >= 8 )
			return false;

		if( m_EditOption.mCurReduceMode == BEGIN_REDUCE)
		{
			m_pEditData->chunkSet [gz][gx].SetCullInfo ( mdx ,mdz , 1 );		
		}
		else if (  m_EditOption.mCurReduceMode == REDUCE_RESUME )
		{		
			m_pEditData->chunkSet [gz][gx].SetCullInfo ( mdx ,mdz , 0 , 1 );
		}
		return true;
	}
	return false;
}

//设置当前位置；参数（镜头方向，位置）；返回（为真表示成功）
bool MTerrainEdit::SelectCurPos(MVec3  &dir,MVec3  &pos)
{
 	MVec3  pos0;
	if(g_Terrain.TestAndRay(dir,pos,pos0))
	{
		m_CurMousePos = pos0 ;

		int gx ,gz;
		
		if(!g_Terrain.GetGridX(pos0.x,&gx))
			return false;

		if(!g_Terrain.GetGridZ(pos0.z,&gz))
			return false;

		{
			int  mdx,mdz;
			int  gridnum = m_pEditData->chunkSet [gz][gx].GetChunkNewVecData()->ngrid ;
			mdx = (int)(m_pEditData->chunkSet [gz][gx].GetGridPosX(pos0.x)/TERRBLOCK_SIZE*gridnum);
			mdz = (int)(m_pEditData->chunkSet [gz][gx].GetGridPosZ(pos0.z)/TERRBLOCK_SIZE*gridnum);

			m_curSelectInfo.gx = gx ;
			m_curSelectInfo.gy = gz ;
			m_curSelectInfo.size = 1;

			m_curSelectInfo.vx = mdx ;
			m_curSelectInfo.vy = mdz ;

			m_curSelectInfo.pos  =  m_pEditData->chunkSet [gz][gx].GetChunkNewVecData()->vVer[mdx+mdz*(gridnum+1)].pos ;

			float _size = ((MTitle*)g_Terrain.GetTitle (0))->GetHeadData ()->gridSize / gridnum;
			float min_select_range = (m_EditOption.nCurChangeHeightSize - 1)*_size;

			if( m_pSignSet )
			{
				if( min_select_range <= 0 )
					min_select_range = _size;

				m_pSignSet->UpdateData ( pos0 , min_select_range , 64 );
			}
		}

		 return true;
	}
	else
	{
		return false;
	}
}	
 
//更新CHUNK的细节顶点
void  MTerrainEdit::UpdateChunkDetailVec()
{
	if( m_pEditData == NULL )
		return;

	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
	{
		for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ;j ++ )
		{
			m_pEditData->chunkSet [j][i].UpdateRenderHeight ();
		}
	}
}

//保存CHUNK的纹理信息
void    MTerrainEdit::SaveChunkTextureInfo()
{
	MPaintTextureProcess* pProcess = static_cast< MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->SaveChunkTextureInfo( this );

};
	
//加载CHUNK的纹理信息
void   MTerrainEdit::LoadChunkTextureInfo()
{
	MPaintTextureProcess* pProcess = static_cast< MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->LoadChunkTextureInfo( this );
};

//保存为导航文件。	参数（文件名）；返回值（为真表示成功 ）
bool  MTerrainEdit::SaveNaviFile(const char* filename)
{
	if( g_NavMap.SaveNavFile ( filename ))
		return true;

	return false;
};

//改变目录的所有TEX文件为MAY格式；参数（目录名）
void    MTerrainEdit::ChangeDirTexToMayFormat(const char* dir)
{
	char m_strTextrueName[256];
	char m_strInitialDir[256];

	char strTextureName[MAX_PATH] ;
	strTextureName[0] = 0;

	char curdir[1024] = {0};
	::GetCurrentDirectory(1024, curdir);

	// Display the OpenFileName dialog. Then, try to load the specified file
    OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL,
                         "Texture Files (.jpg;.dds;.png;.bmp;.tga;)\0*.jpg;*.dds;*.png;*.bmp;*.tga;\0\0", 
                         NULL, 0, 1, strTextureName, MAX_PATH, m_strTextrueName, MAX_PATH, 
                         m_strInitialDir, "Open Texture File", 
						 OFN_PATHMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

    if( TRUE == GetOpenFileName( &ofn ) )
	{
		::SetCurrentDirectory(curdir);
		ChangeTexFileToMayFormat(strTextureName); 
	}
}

//改变单个文件的格式；参数（文件名）；返回（为真表示成功 ）
bool    MTerrainEdit::ChangeTexFileToMayFormat(const char* filename)
{
	return g_TexManager.ChangeImageToMayFormat(filename);
}
	
//设置当前编辑状态；参数（ 事件 ）
void    MTerrainEdit::SetCurEditEvent(EIDT_EVENT event)	
{
	if( m_EditOption.mCurEditEvent != event )
	{
		if ( m_EditOption.pEventProcess )
		{
			m_EditOption.pEventProcess->Release();
		}

		m_EditOption.mCurEditEvent = event;

		m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess( event );

		if ( m_EditOption.pEventProcess )
		{
			m_EditOption.pEventProcess->Init(this);
		}
	}
};

//改变纹理为MAY格式
void   MTerrainEdit::ChangeTexToMayFormat()
{
    for(unsigned int i = 0 ; i < m_pEditData->texData .texFileSet .size () ; i++)
	{
		g_TexManager.ChangeImageToMayFormat(m_pEditData->texData .texFileSet[i].c_str());
	}
};  

//增加植物；参数（方向，位置）
void    MTerrainEdit::AddPlant(MVec3 dir , MVec3 pos )
{
	MVec3 pos0;

	if( g_Terrain.TestAndRay (dir,pos,pos0))
	{
		m_EditOption.PlantSavePos = pos0;
	}
};

//刷植被；参数（光的方向，位置，模式）
void   MTerrainEdit::PaintPlant(MVec3 dir , MVec3 pos , int mode )
{
	MVec3  pos0;

		
	if( m_pSignSet)
	{
		m_pSignSet->m_PosSet .clear ();
	}
   
	if(g_Terrain.TestAndRay (dir,pos,pos0))
	{
		if( m_pSignSet)
		{	
			MVec3  _pos = pos0;
			MVec3  wpos[4];
			float _len = m_EditOption.nCurSelectPaintRad[m_EditOption.nCurLayer-1]*TERRBLOCK_SIZE / 64.0f;
	
			//更新圆
			m_pSignSet->UpdateData ( pos0 , _len , 64 );
		}

		//为1则不坐处理
		if (mode == 0 )
			return;



		MVec3 len = pos0 - m_EditOption.PlantSavePos ;
		float maxlen = 3300.3f * ( (m_EditOption.nPaintPlantSize) / 64.0f );

		if(  len.length () >  maxlen ) 
			return;

		m_EditOption.PlantSavePos = pos0; 

		int gx,gz;
		if(!g_Terrain.GetGridX(pos0.x, &gx)) return;
		if(!g_Terrain.GetGridZ(pos0.z, &gz)) return;


		if( g_MInputProcess.GetUserInput()->bLShift )
		{
			m_pEditData->chunkSet[gz][gx].DelPlant( m_EditOption.PlantSavePos , maxlen );
			return;
		}

		m_pEditData->chunkSet[gz][gx].AddPlant( m_EditOption.PlantTexInfo.texName , pos0 , m_EditOption.fPlantScale );

		std::vector<int> xset,zset;

		MVec3  pos1 ;
		for( int i = 0 ; i < m_EditOption.nPaintVec  ; i ++)
		{
			float nRad = MAY_PI * ( rand() % 360 / 180.0f );

			float nRadSize = ( rand()% ( m_EditOption.nPaintPlantSize*100 / 2 ) ) / 100.0f ; 

			pos1.x = pos0.x +  sin(nRad)*( nRadSize) * 100.0f;
			pos1.z = pos0.z +  cos(nRad)*( nRadSize) * 100.0f;
           
			//float fScale = 1.0f - nRadSize / ( m_EditOption.nPaintPlantSize / 2.0f ) ;
           float fScale = m_EditOption.fPlantScale ;

			if( g_Terrain.GetHeight ( pos1.x ,pos1.z , &pos1.y ) )
			{
				g_Terrain.GetGridX( pos1.x , &gx );
				g_Terrain.GetGridZ( pos1.z , &gz );

				if( xset.empty() && zset.empty() )
				{
					xset.push_back( gx );	
					zset.push_back( gz );
				}
				else
				{
					for( size_t k = 0 ; k < xset.size() ; k ++ )
					{
						if( xset[k] == gx && zset[k] == gz )
							continue;
						else
						{
							xset.push_back( gx );
							zset.push_back( gz );
							break;
						}
					}
				}

				m_pEditData->chunkSet[gz][gx].AddPlant( m_EditOption.PlantTexInfo.texName , pos1 , fScale);
			}
		}		
		for( size_t i = 0 ; i < xset.size() ; i ++ )
		{
			MChunk::PLANT_MAP::iterator iter = m_pEditData->chunkSet[zset[i]][xset[i]].m_PlantMap.begin();
			for( ; iter != m_pEditData->chunkSet[zset[i]][xset[i]].m_PlantMap.end() ; iter ++ )
			{
				iter->second->UpdateDataEx_1();
			}
		}
	}
};

//更新地图的纹理环绕次数；参数（次数）
void  MTerrainEdit::UpdateMapTexRepeat(int nRepeat )
{	
	g_Terrain.GetTitle (0)->UpdateRepeat ( nRepeat );
}

//增加文件文件入修改数据；参数（文件名）；返回 （ 索引ID ） 
int   MTerrainEdit::AddTextureFileToTitle(std::string name)
{
	MPaintTextureProcess* pProcess = static_cast< MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	return pProcess->AddTextureFileToTitle( this , name );         
}
  
//删除选中的NPC；参数（NPC索引 ）
void MTerrainEdit::DeleteSelectNpc(int index)
{
	MNPC *npc = m_pNpcSet->GetNpc(index);
	if(npc == NULL) return;

	//if(npc->GetModel() == m_EditOption.pSelectModel)
	{
		m_pCoordinate->bindObject( NULL );
        g_Camera.BindObject ( NULL );
		m_EditOption.pSelectModel = NULL;
	}
	//if(m_pCurSelectModel == npc->GetModel())
	{
		m_pCurSelectModel = NULL;
	}

	m_pNpcSet->DeleteIthNpc(index);
};

//设置修改高度的范围；参数（大小）
void MTerrainEdit::SetChangeHeightSize ( int size )
{
	m_EditOption.nCurChangeHeightSize = size;

	if(size == 0 )size = 1;

	float fAdd = (float)(  MAY_PI / (2*(size*1.0f)) );

	for( int i = 0 ; i < size ; i++)
	{
       g_fCos[i] = cos( i*fAdd );
	}
}

//增加模型文件为编辑数据；参数（模型文件名）返回（模型的INDEX ）
int  MTerrainEdit::AddModelName(std::string name)
{
	if( m_pEditData == NULL )
		return 0;

	for(unsigned int i = 0 ; i < m_pEditData->modelData.modelFileSet .size () ; i++)
	{	
		if( m_pEditData->modelData.modelFileSet [i] == name)	
		{
			  return i;		
		}
	}
  
	m_pEditData->modelData.modelFileSet .push_back ( name );
	
	return int(m_pEditData->modelData.modelFileSet .size ())-1;
}

//设置当前BuildingModel的文件名；参数（文件名 ）
void MTerrainEdit::SetCurSelectBuildingModelName(const char* filename)
{
	m_SelResData.respath = filename;
	m_SelResData.pmodel = new EditorModel;
	m_SelResData.pmodel->LoadBuildingModel(filename);
    
	//m_SelResData.pmodel->SetPos(m_SelResData.dir);
};

//移动标记；参数（光的方向，位置 ）
void  MTerrainEdit::MoveSign(MVec3& dir , MVec3& pos )
{
	if( m_pSignSet )
	{
		m_pSignSet->m_PosSet.clear ();
         MVec3  pos0;

		if( g_Terrain.TestAndRay ( dir , pos , pos0 ))
		{
			m_pSignSet->UpdateData ( pos0 , g_fModelSignRad , 16 );
		}
	}
}

void MTerrainEdit::setCurSelectRes(const char *filename)
{
	m_SelResData.respath = "";
	OGRE_RELEASE(m_SelResData.ptexture);
	if(m_SelResData.pmodel && m_pResPreviewRU)
	{
		m_SelResData.pmodel->GetShowModel()->detachFromScene();
	}
	OGRE_DELETE(m_SelResData.pmodel);

	m_SelResData.respath = filename;
	const char *pdot = strrchr(filename, '.');
	if(stricmp(pdot,".omod")==0 || stricmp(pdot, ".ent")==0)
	{
		m_SelResData.seltype = SELECT_RES_MODEL;
		setCurSelectModelName(filename);
	}
	else if(stricmp(pdot,".otex")==0 || stricmp(pdot,".dds")==0 ||stricmp(pdot,".jpg")==0 || stricmp(pdot,".tga")==0 ||stricmp(pdot,".bmp")==0)
	{
		m_SelResData.seltype = SELECT_RES_TEXTURE;
		setCurSelectPaintTexName(filename);
	}
	else if(stricmp(pdot,".wav")==0 || stricmp(pdot,".mp3")==0 || stricmp(pdot,".wma")==0)
	{
		m_SelResData.seltype = SELECT_RES_SOUND;
	}
}

//设置当前选择的模型名；参数（文件名 ）
void  MTerrainEdit::setCurSelectModelName(const char* filename)
{
	std::string name = filename;
	int index = AddModelName(name);

	if(m_SelResData.pmodel)
	{
		EditorModel* pModel = static_cast<EditorModel*>(m_SelResData.pmodel);
		pModel->GetShowModel()->detachFromScene();
	}

	delete m_SelResData.pmodel;
	m_SelResData.pmodel = new EditorModel;
	m_SelResData.pmodel->LoadModel(m_SelResData.respath.c_str());
	    
	Entity *pentity = ((EditorModel*)m_SelResData.pmodel)->GetShowModel();

	if(m_pResPreviewRU)
	{
		pentity->attachToScene(m_pResPreviewRU->m_pScene);
	}
	pentity->setPosition(m_SelResData.pos);

	const BoxSphereBound &bound = pentity->getWorldBounds();
	ComputeModelViewCamera(m_ResPreviewCamera, bound);

	if(m_AutoLinkMesh->TestLinkMesh(m_SelResData.respath))
	{
		m_EditOption.mCurEditEvent = AUTOLINK_OBJECT_EVN;
		m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess( AUTOLINK_OBJECT_EVN );
	}
	else if(m_SelResData.respath.find("braemesh") != -1)
	{
		SetCurEditEvent( BRAE_EDIT_EVN );

		MBraeEditProcess* pProcess = static_cast<MBraeEditProcess*>( g_EventProcessFactory.GetEventProcess( BRAE_EDIT_EVN ) );
		pProcess->SetBraeModelName(m_SelResData.respath);
	}
	else 
	{
		m_EditOption.mCurEditEvent = ADD_OBJECT_EVN;
		m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess( ADD_OBJECT_EVN );
	}
}

//设置当前选择刷地表的纹理文件；参数（文件名）
void  MTerrainEdit::setCurSelectPaintTexName(const char* filename)
{
	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ));
	pProcess->SetCurSelectPaintTexName(this , filename);
};

//设置当前显示地表层；参数（层）
void  MTerrainEdit::SetCurShowLayer(int layer)
{
	if(layer<0 || layer >=5)
		return;

	if( g_Terrain.GetTitle (0)!=NULL)
    ((MTitle*)g_Terrain.GetTitle (0))->SetChunkShowLayer ( layer );
}

//得到当前模型类型；返回（镜头类型）
int  MTerrainEdit::GetCameraType()
{
	return (int)m_pCamera->m_CameraType;
}

//设置当前镜头类型；参数（类型）
void MTerrainEdit::SetCameraType( int index )
{
	if( index < 2)
	{
        if(m_pCamera)
		{
			m_pCamera->m_CameraType  = (MCamera::CAMERATYPE)index ;	
			m_pCamera->m_MouseMoveType = MCamera::ROATE_CAMERA;
		}
		m_EditOption.nCurCameraTpye = 0;
	}
	else
	{
        m_EditOption.nCurCameraTpye = 1;
		m_pCamera->m_MouseMoveType = MCamera::MOVE_CAMERA;
	}
}
/* 
//渲染模型
void  MTerrainEdit::RenderModel()
{
	if(m_pModelRU && m_bDrawModelWnd )
	{
	    Vector3 dir;
		dir.y = Sin(m_ResPreviewCamera.viewYaw);
		dir.x = Sin(m_ResPreviewCamera.viewPitch)*Cos(m_ResPreviewCamera.viewYaw );
	    dir.z = Cos(m_ResPreviewCamera.viewPitch)*Cos(m_ResPreviewCamera.viewYaw);

	 	m_pModelRU->m_pCamera->setLookAt(m_ResPreviewCamera.pos, Vector3(0,0,0), Vector3(0,1.0f,0));
	    m_pModelRU->m_pCamera->setFov( 45.0f );
        m_pModelRU->m_pCamera->setDepth( g_fCameraNear , g_fCameraFar );
	}
}*/

//设置CHUNK的绘制模式；参数（绘制信息 ）
void MTerrainEdit::SetChunkDrawMode(int index)
{
	if(m_pEditData == NULL)
		return;

	m_DrawFillMode = (DRAW_FILL_MODE)index;
};

//计算法线		
void  MTerrainEdit::UpdataNormalMap()
{
	m_HeightMap.CountNormal();
}

//更显法线图；参数（x,y )
void  MTerrainEdit::UpdateNormalMap(int indexX ,int indexY)
{
	m_HeightMap.UpdateNormal( indexX ,indexY );
};

//跟新当前的游戏信息为ColorMap 
void  MTerrainEdit::UpateCurGameInfoToColorMap()
{
	if( m_pEditData == NULL )
		return;

	MTitle* pTile = g_Terrain.GetTitle (0) ;
	if( pTile )
	{
		for( int i = 0 ; i < pTile->GetTileData()->gh ; i ++)
		{
			for( int j = 0 ;j < pTile->GetTileData()->gw ; j++)
			{
				pTile->GetTitleData ()->chunkSet [i][j].CreateColorMap();
				pTile->GetTitleData ()->chunkSet [i][j].UpdateMapInfoToColorMap( m_EditOption.gameInfo );
							
				pTile->GetTitleData ()->chunkSet [i][j].UpdateColorMap();
			}
		}
	}
}


void  MTerrainEdit::UpdateTileHeightDataToHeightMap()
{
	MModifyHeightProcess* pProcess = static_cast<MModifyHeightProcess*>( g_EventProcessFactory.GetEventProcess( CHANGE_HEIGHT_EVN ) );
	pProcess->UpdateTileHeightDataToHeightMap( this );
};
	
void  MTerrainEdit::UpdateHeightMapToAllTileHeight()
{
	MModifyHeightProcess*  pProcess = static_cast<MModifyHeightProcess*>( g_EventProcessFactory.GetEventProcess( CHANGE_HEIGHT_EVN ) );
	pProcess->UpdateHeightMapToAllTileHeight( this );
}

//设置需要编辑的地形数据
void MTerrainEdit::SetTerrain(MTerrain* terrain)
{
	if( terrain == NULL)
	{
		assert( 0);
		return;
	}

	if( terrain->GetTitle (0))
	{
		m_pEditData = ((MTitle*)terrain->GetTitle (0))->GetTitleData ();
	}
}
	
/*
Ogre::Vector4 MTerrainEdit::ConvertColorFormat(int color)
{
	MAY::Vector4 vColor;
	vColor.x = (float)(color & 0xff) / 255.0f;
	vColor.y = (float)((color >> 8) & 0xff) / 255.0f;
	vColor.z = (float)((color >> 16) & 0xff) / 255.0f;
	vColor.w = (float)((color >> 24) & 0xff) / 255.0f;

	return vColor;
}
*/

struct EditConfig
{
	bool win_fullscreen;
	int win_width;
	int win_height;
	int resdb_mode;
	char resdb_dir[128];
	char resdb_server[128];
	char resdb_user[32];
	char resdb_passwd[32];
	char resdb_database[32];
	char zone_server[128];
};

//初始化编辑器
bool MTerrainEdit::Init(void *hinst, void *hmainview, void *hrespreview, void *hradarview)
{
	Root *proot = new Root("ogre_mapeditor.cfg");
	if(!proot->initRenderSystem(hmainview))
	{
		LOG_SEVERE("failed to init RenderSystem");
		return false;
	}

	GameConfig config;
	LoadGameConfigFromXML(config, &proot->m_Config);

	m_Cull = new MTerrainCull;

	m_pDBMgr = new DBDataManager;
	m_pDBMgr->LoadData(config);
	m_pAvatarMgr = new AvatarManager;
	m_pAvatarMgr->init();
	AvatarEntity::m_pAvatarMgr = m_pAvatarMgr;

	m_pLinesRenderer = new RenderLines;

	m_EditOption.mCurEditEvent = CHANGE_HEIGHT_EVN;
	m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess(CHANGE_HEIGHT_EVN);

	m_EditOption.mSaveCurEditEvent = m_EditOption.mCurEditEvent ;
	
	m_EditOption.nPaintVec = 20;
	m_EditOption.nCurSelectNpcID = 0;
	m_EditOption.CurSelectNpcIndex = -1;

	EditOptionWater &water = m_EditOption.water;
	water.chunk_x = water.chunk_z = -1;
	water.animtex[0] = 0;
	water.envtex[0] = 0;
	water.maxwaterdepth = 1200.0f;
	water.waterheight = 100.0f;
	water.watertype = -1;

	m_EditOption.nCurMoveY = 10; 

    m_EditOption.nCurAddModelSize = 5;
    m_EditOption.nCurAddModelNum = 1;

	m_EditOption.curMaxScale = 100;
	m_EditOption.curMinScale = 100;	

	m_bOpenTextureInfoDlg = false;
	m_bShowTextureInfoDlg = false;
	
	m_bOpenLightInfoDlg = false;;	
	m_bShowLightInfoDlg = false;;
	
	m_bOpenAreaInfoDlg   = false;
	m_bShowAreaInfoDlg   = false;

	m_bOpenModelInfoDlg = false;
	m_bUpdateLightList  = false;

    m_bUpdateAmbientSettingDlg = false;

	g_Terrain.init();

	m_pSignSet = new MSignSet;
	
	m_pCoordinate = new Coordinate;

	m_pCampSign = new SlopeSign;
	m_pCampSign->Init();

	m_AutoLinkMesh = new LinkMeshManager;
	m_AutoLinkTex = new LinkTextureManager;
	m_PaintWallPaper = new LinkOverlayManager;

	LoadMapEditIni();

	g_StripBaseSet = new CStripBaseSet;
	g_PlantMgr = new CPlantManager;
	g_PlantMgr->Init();

	g_AreaSet.init();
	SetUpdateAreaData(true);

	for( int i = 0 ; i < TYPE_MODEL_NUM ; i++)
	{
		m_bLockModelEdit[i] = false;
	}	

	for( int i = 0 ; i < 4 ; i  ++ )
	{
		m_EditOption.uUVRepeat[i] = 12;
	}

	LoadChunkTextureInfo();

	m_bPessQ = false;

	g_pCamera = new Camera;
	g_pCamera->setDepth(100.0f, 10000000.0f);
	g_Camera.SetCamera(g_pCamera);
	g_Camera.reset();
	g_Camera.Update();
	m_pCamera = &g_Camera;

	m_pMainRU = new RenderUnit(hmainview, RENDERUNIT_MAINVIEW);

	m_hPreviewWnd = hrespreview;
	if(hrespreview != NULL) m_pResPreviewRU = new RenderUnit(m_hPreviewWnd, RENDERUNIT_RESPREVIEW);
	else m_pResPreviewRU = NULL;

	m_hRadarViewWnd = hradarview;
	if(hradarview != NULL) m_pRadarViewRU = new RenderUnit(m_hRadarViewWnd, RENDERUNIT_RADARVIEW);
	else m_pRadarViewRU = NULL;

	m_ResPreviewCamera.eyeloc = Ogre::Vector3(0, 2.0f, -20.0f);
	m_ResPreviewCamera.goal = Ogre::Vector3(0,0,0);
	
	m_DrawFillMode = DRAW_SOLID;

	m_bInit = true;

	return true;
};

void  MTerrainEdit::Release()
{
	if(!m_bInit) return;
	m_bInit = false;

	if(m_SelResData.pmodel)
	{
		EditorModel* pModel = static_cast<EditorModel*>(m_SelResData.pmodel);
		if(m_pResPreviewRU) pModel->GetShowModel()->detachFromScene();
		delete m_SelResData.pmodel;
	}
	OGRE_RELEASE(m_SelResData.ptexture);

	OGRE_DELETE(m_pPathSet);
	OGRE_DELETE(m_pSignSet);

	g_AreaSet.release();
	m_pNpcSet->Release();
	delete m_pNpcSet;
	m_pNpcSet = NULL;

	delete m_pCoordinate;
	delete m_pCampSign;

	m_HeightMap.m_vHeight.clear();
	delete g_StripBaseSet;

	delete g_PlantMgr;
	g_LightManager.Release ();

	//
	delete m_AutoLinkMesh;
	delete m_AutoLinkTex;
	delete m_PaintWallPaper;

	OGRE_RELEASE(m_pLinesRenderer);

	g_Terrain.Release();

	delete m_pAvatarMgr;
	m_pAvatarMgr = NULL;

	delete m_pDBMgr;
	m_pDBMgr = NULL;

	OGRE_DELETE(m_Cull);

	delete m_pResPreviewRU;
	delete m_pRadarViewRU;
	delete m_pMainRU;
	delete Root::getSingletonPtr();
}

void MTerrainEdit::doFrame(unsigned int dtick)
{
	Update(dtick);

	if(m_pResPreviewRU)
		m_pResPreviewRU->m_pCamera->update(dtick);
	g_pCamera->update(dtick);


	RenderHelpLines();
	//RenderOtherWnd();
	GetAutoLinkMesh()->Loading();
	GetAutoLinkTex()->Loading();

	SceneManager::getSingleton().doFrame();
}

//重置colorMAP的alp
void MTerrainEdit::ResetColorMapAlpha()
{
	if( m_pEditData == NULL )
		return;

	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i++)
	{
		for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j++)
		{
			TerrBlockData* pChunkData =  m_pEditData->chunkSet[i][j].GetChunkData ();
			memset( pChunkData->ColorAlpha  , 0 , 64*64); 
		}
	}
}

//更新玩家操作
void  MTerrainEdit::UpdateCamera()
{
	g_MInputProcess.Process( this );   
 }

//跟新其他操作
 void MTerrainEdit::UpdateOther(float dtime)
 {
 	if( m_EditOption.bBeingModifyHeight )
	{
		if ( m_EditOption.pEventProcess->GetProcessType() == CHANGE_HEIGHT_EVN )
		{
			static MModifyHeightProcess* pProcess = NULL ;
			if( pProcess == NULL )
			{
				pProcess = static_cast<MModifyHeightProcess*>( m_EditOption.pEventProcess );
			}
            pProcess->ModifyTerrainHeight( this , 20 );
		}
	}	 
 }
 
//更新NPC
 void  MTerrainEdit::UpdataNpc(float dtime)
 {
     m_pNpcSet->Update(dtime);
 }

 //更新
void  MTerrainEdit::Update(unsigned int dtick)
{
	float dtime = TickToTime(dtick);
	m_pLinesRenderer->reset();

    if(PLACE_PLYAER == m_EditOption.mCurEditEvent)
        m_EditOption.pEventProcess->Update(this , dtime);
    else
        UpdateCamera();

	UpdateOther(dtime);
	m_pCoordinate->update(dtime);
	g_LightManager.update(dtime);

	if( m_pSignSet )
		m_pSignSet->UpdateData();

	RoateShowModel(dtime*90.0f);

	g_PlantMgr->UpdateDataPlant();

	UpdataNpc(dtime);

	if(m_SelResData.pmodel) m_SelResData.pmodel->Update(dtime);

	g_Terrain.update(dtick);

	if(m_pMainRU) m_pMainRU->update(dtick);
	if(m_pResPreviewRU) m_pResPreviewRU->update(dtick);
	if(m_pRadarViewRU) m_pRadarViewRU->update(dtick);
};


void  MTerrainEdit::UpdateBox()
{
	if( m_pEditData == NULL )
		return;

     g_Terrain.GetTitle (0)->UpdateBox ();
}

void  MTerrainEdit::SwitchEditTile()
{
	if( g_Terrain.GetTitle ( 1 ))
	{
		MTitle *pTile = g_Terrain.m_TitleSet [1] ;
		g_Terrain.m_TitleSet[1] = g_Terrain.m_TitleSet  [0];
		g_Terrain.m_TitleSet [0] = pTile;

		m_pEditData = ((MTitle*)g_Terrain.GetTitle (0))->GetTitleData ();
	}
}

void  MTerrainEdit::AddPlantModelToTitle(const char* filename,MVec3 pos,MVec3 dir,float fscale)
{	
	MAddObjectProcess* pProcess = static_cast<MAddObjectProcess*> ( g_EventProcessFactory.GetEventProcess( ADD_OBJECT_EVN ) );
	pProcess->AddPlantModelToTitle( this ,filename , pos ,dir , fscale );
}
	
void  MTerrainEdit::SetAmbientLightColor( float r,float g,float b,float a)
{	
	//g_Terrain.SetAmbientLightColor ( r,g,b,a);
};
	
void  MTerrainEdit::SetDirLightColor(float r,float g,float b,float a)
{	
	//g_Terrain.SetDirLightColor (r , g ,b ,a);
};

void  MTerrainEdit::AddObjectTitle()
{
	MAddObjectProcess* pProcess = static_cast<MAddObjectProcess*> ( g_EventProcessFactory.GetEventProcess( ADD_OBJECT_EVN ) );
	pProcess->AddObjectTitle( this );
};

	
//拷贝选中物件
void  MTerrainEdit::CopySelectObject()
{
	if( m_EditOption.pEventProcess == NULL )
		return;

	int nType = m_EditOption.pEventProcess->GetProcessType();

	if ( nType == SELECT_OBJECT_EVN ||
		nType == ADD_OBJECT_EVN ||
		nType == MOVE_OBJECT_EVN ||
		nType == ROATE_OBJECT_EVN ||
		nType == SCALE_OBJECT_EVN )
	{
		m_CopyObjectInfo.clear () ;

		MCopyObjectInfo  info;
		if( m_pCurSelectModel )
		{
			info.name = m_pCurSelectModel->GetModelData ()->name ;

			info.dir = m_pCurSelectModel->GetModelData ()->dir ;
			info.pos = m_pCurSelectModel->GetModelData ()->pos ;
			info.scale = m_pCurSelectModel->GetModelData ()->scale ;
			info.type = m_pCurSelectModel->GetModelData ()->cType ;

			m_CopyObjectInfo.push_back ( info );
		}

		for( size_t i = 0 ;i < m_CurSelectModelSet.size () ; i ++)
		{
			info.name = m_CurSelectModelSet[i]->GetModelData ()->name ;

			info.dir = m_CurSelectModelSet[i]->GetModelData ()->dir ;
			info.pos = m_CurSelectModelSet[i]->GetModelData ()->pos ;
			info.scale = m_CurSelectModelSet[i]->GetModelData ()->scale ;
			info.type = m_CurSelectModelSet[i]->GetModelData ()->cType ;

			m_CopyObjectInfo.push_back ( info );
		}
	}
	else if ( nType == LIGHT_EDIT_EVN )
	{
        g_LightManager.CopyCurLight();
	}
};

//设置显示物件BOX；参数( 为真表示绘制 ）
void  MTerrainEdit::SetShowObjectBox(bool bShow )
{
	if( m_pEditData == NULL )
		return;
	
	m_bShowObjectBox = bShow ;
/*
	for( size_t i = 0 ; i < m_pEditData->modelData .modelSet .size () ; i ++)
	{
		m_pEditData->modelData .modelSet [i]->GetModelData ()->bDrawBox = bShow ;
	}

	for( size_t i = 0 ; i < m_pEditData->modelData .modelPlantSet  .size () ; i ++)
	{
		m_pEditData->modelData .modelPlantSet [i]->GetModelData ()->bDrawBox = bShow ;
	}*/
}
	
//拷贝当前模型
void  MTerrainEdit::CopySelectObjectToTile()
{
	if( m_EditOption.pEventProcess == NULL )
		return;

	int nType = m_EditOption.pEventProcess->GetProcessType();

	if ( nType == SELECT_OBJECT_EVN ||
		nType == ADD_OBJECT_EVN ||
		nType == MOVE_OBJECT_EVN ||
		nType == ROATE_OBJECT_EVN ||
		nType == SCALE_OBJECT_EVN )
	{
		int index;

		MVec3 dPos(2.0,2.0,2.0);

		if( m_pCurSelectModel )
		{
			m_pCurSelectModel->GetModelData ()->pDrawbox ->SetColor (  0xffaaaaaa   );
		}

		for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i ++)
		{
			m_CurSelectModelSet[i]->GetModelData ()->pDrawbox ->SetColor ( 0xffaaaaaa   );
		}
		m_CurSelectModelSet.clear ();

		for( size_t i = 0 ;i < m_CopyObjectInfo.size () ; i ++)
		{
			TerrModel*  model = new TerrModel;
			strcpy(model->GetModelData ()->name , m_CopyObjectInfo[i].name .c_str ());
			model->GetModelData ()->cType = m_CopyObjectInfo[i].type ;

			index = 0;
			for( size_t j = 0 ; j < m_pEditData->modelData .modelFileSet .size () ; j ++)
			{
				if( m_pEditData->modelData .modelFileSet [j] == m_CopyObjectInfo[i].name ) 
				{
					index = (int)j;
					break;
				}
			}

			model->GetModelData ()->modelID = index;
			model->Init ();

			model->GetModelData ()->pDrawbox ->SetColor ( 0xff00ff00 );
			if( i == 0 )
			{
				model->GetModelData ()->pos = m_CopyObjectInfo[i].pos + dPos;
				model->GetModelData ()->dir =  m_CopyObjectInfo[i].dir ;
				model->GetModelData ()->scale =  m_CopyObjectInfo[i].scale ;
				model->GetModelData ()->cType = m_CopyObjectInfo[i].type ;
				model->ResetMat ();		
				
				m_pCurSelectModel = model;		
				m_pCoordinate->bindObject( model->GetModelData ()->pModel );
				g_Camera.BindObject ( model );

				m_EditOption.pSelectModel = model;

			}
			else
			{
				model->GetModelData ()->pos =  m_CopyObjectInfo[i].pos + dPos;
				model->GetModelData ()->dir =  m_CopyObjectInfo[i].dir ;
				model->GetModelData ()->scale =  m_CopyObjectInfo[i].scale ;
				model->ResetMat ();

				m_CurSelectModelSet.push_back ( model );
			}

			model->AttachRoom();       
			m_pEditData->modelData .modelSet .push_back (model);
		}
	}
	else if( nType == LIGHT_EDIT_EVN )
	{
		g_LightManager.CopySelectLightToTile();
	}
};
	
void   MTerrainEdit::ResetPaintTexture()
{
	MPaintTextureProcess *pProcess = static_cast< MPaintTextureProcess *>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->ResetPaintTexture( this );
};

void MTerrainEdit::MoveSelectModel(float length)
{   
	if( m_pCurSelectModel == NULL)
		return;
 
	switch(m_pCoordinate->getCoordSelect())
	{
	case SELECT_X_AXES :	
		m_pCurSelectModel->GetModelData ()->pos .x += length;
		break;
	case SELECT_Y_AXES :	
	case NO_AXES_SELECT :
		m_pCurSelectModel->GetModelData ()->pos .y += 10*length;
		break;
	case SELECT_Z_AXES:
		m_pCurSelectModel->GetModelData ()->pos .z += length;
		break;
	case SELECT_XY_AXES :	
		m_pCurSelectModel->GetModelData ()->pos .x += length;
		m_pCurSelectModel->GetModelData ()->pos .y += 10*length;
		break;
	case SELECT_YZ_AXES :
		m_pCurSelectModel->GetModelData ()->pos .y += 10*length;
		m_pCurSelectModel->GetModelData ()->pos .z += length;
		break;
	case SELECT_XZ_AXES:
		m_pCurSelectModel->GetModelData ()->pos .x += length;
		m_pCurSelectModel->GetModelData ()->pos .z += length;
		break;
	}

	m_pCurSelectModel->ResetMat ();

	for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i ++ )
	{
		switch( m_pCoordinate->getCoordSelect() )
		{
		case SELECT_X_AXES :	
			m_CurSelectModelSet[i]->GetModelData ()->pos .x += length;
			break;
		case SELECT_Y_AXES :
		case NO_AXES_SELECT :
			m_CurSelectModelSet[i]->GetModelData ()->pos .y += 10*length;
			break;
		case SELECT_Z_AXES:
			m_CurSelectModelSet[i]->GetModelData ()->pos .z += length;
			break; 
		case SELECT_XY_AXES :	
			m_pCurSelectModel->GetModelData ()->pos .x += length;
			m_pCurSelectModel->GetModelData ()->pos .y += 10*length;
			break;
		case SELECT_YZ_AXES :	
			m_pCurSelectModel->GetModelData ()->pos .y += 10*length;
			m_pCurSelectModel->GetModelData ()->pos .z += length;
			break;
		case SELECT_XZ_AXES:
			m_pCurSelectModel->GetModelData ()->pos .x += length;
			m_pCurSelectModel->GetModelData ()->pos .z += length;
			break;
		}		
	
		m_CurSelectModelSet[i]->ResetMat ();
	}
}

void  MTerrainEdit::RoateYSelectModel ( int dz )
{
    if( m_pCurSelectModel == NULL)
		return;

	float roate = dz*0.01f;
	switch( m_pCoordinate->getCoordSelect() )
	{
	case SELECT_X_AXES :	
		m_pCurSelectModel->GetModelData ()->dir .y += roate;
		break;
	case SELECT_Y_AXES :	
	case NO_AXES_SELECT :
		m_pCurSelectModel->GetModelData ()->dir .x += roate;
		break;
	case SELECT_Z_AXES:
		m_pCurSelectModel->GetModelData ()->dir .z += roate;
		break;
	}

	m_pCurSelectModel->ResetMat ();

	for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i++)
	{ 
		switch( m_pCoordinate->getCoordSelect() )
		{
		case NO_AXES_SELECT :
			break;
		case SELECT_X_AXES :	
			m_CurSelectModelSet[i]->GetModelData ()->dir .y += roate;
			break;
		case SELECT_Y_AXES :
			m_CurSelectModelSet[i]->GetModelData ()->dir .x += roate;
			break;
		case SELECT_Z_AXES:
			m_CurSelectModelSet[i]->GetModelData ()->dir .z += roate;
			break;
		}
		m_CurSelectModelSet[i]->ResetMat();
	}

}

void  MTerrainEdit::ZoomSelectModel(int  dz)
{
	if(m_pCurSelectModel == NULL)
		return;

	//暂时不提供X，Y，Z缩放
    float fzoom = dz*0.0001f;

    m_pCurSelectModel->GetModelData ()->scale += fzoom;
	if(m_pCurSelectModel->GetModelData ()->scale <= 0)
         m_pCurSelectModel->GetModelData ()->scale = 0.00004f;

    m_pCurSelectModel->ResetMat();

	for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i++)
	{ 
		m_CurSelectModelSet[i]->GetModelData ()->scale += fzoom;
	
		if(m_CurSelectModelSet[i]->GetModelData ()->scale <= 0)        
			m_CurSelectModelSet[i]->GetModelData ()->scale = 0.00004f;

		m_CurSelectModelSet[i]->ResetMat();
	}
};	
	
void  MTerrainEdit::SetCurSelectPaintTexId(int index)
{
	m_EditOption.nCurSelectTexID = index;
	//m_ShowTex.SetTexture ( g_TexManager.LoadTex  ( m_pEditData->texData .texFileSet [index].c_str () ) );
			
};
		
void  MTerrainEdit::DelSelectObject( TerrModel* pModel)
{
	std::vector <TerrModel*> ::iterator iter;

	for( iter = m_pEditData->modelData .modelSet.begin () ; iter != m_pEditData->modelData .modelSet.end () ; iter ++ )
	{
		if( (*iter) == pModel )
		{
			delete( (*iter ) );
			   
			m_pEditData->modelData .modelSet.erase ( iter );
			break;
		}
	}
}	

void    MTerrainEdit::UpdateProcessData()
{
	if( m_EditOption.pEventProcess )
	{
		m_EditOption.pEventProcess->UpdateData( this );
	}
};

void MTerrainEdit::SetAutoDelBlock(int nDel)
{
	m_AutoLinkMesh->SetAuotDelBlock( nDel );
}

void  MTerrainEdit::UpdateBreaHeight()
{
	if( m_pEditData == NULL )
		return;

	MBraeEditProcess *pProcess = static_cast<MBraeEditProcess*>(g_EventProcessFactory.GetEventProcess( BRAE_EDIT_EVN ) );
	pProcess->Update();

}

//删除当前物体
void  MTerrainEdit::DelCurObject()
{
	if( m_pEditData == NULL )
		return;

	if ( m_EditOption.pEventProcess )
	{
		if( m_EditOption.pEventProcess->GetProcessType() == BRAE_EDIT_EVN )
		{
			MBraeEditProcess *pProcess = static_cast<MBraeEditProcess*>(m_EditOption.pEventProcess);
			pProcess->DelCurBrae( this );
			return;
		}
		else if ( m_EditOption.pEventProcess->GetProcessType() == LIGHT_EDIT_EVN )
		{
			g_LightManager.DelCurSelectLight( this );
			return;
		}
	}

	std::vector <TerrModel*> ::iterator iter;

	for( iter = m_pEditData->modelData .modelSet.begin () ; iter != m_pEditData->modelData .modelSet.end () ; iter ++ )
	{
		if( (*iter) == m_pCurSelectModel )
		{					
			delete( (*iter ) );
			   
			m_pEditData->modelData .modelSet.erase ( iter );

			m_pCoordinate->bindObject(NULL);
			g_Camera.BindObject ( NULL );
			
			m_pCurSelectModel = NULL;
			if( m_EditOption.pSelectModel )
			{
				m_EditOption.pSelectModel = NULL;
			}
			break;
		}
	}

	for( size_t i = 0 ;i < m_CurSelectModelSet.size () ;i ++)
	{
		DelSelectObject( m_CurSelectModelSet[i]);
	}

	m_CurSelectModelSet.clear ();

	m_Cull->Cull(&g_Terrain);
}


void  MTerrainEdit::SmoothHeightMap( int width ,int height )
{
	MModifyHeightProcess* pProcess = static_cast<MModifyHeightProcess*>( g_EventProcessFactory.GetEventProcess( CHANGE_HEIGHT_EVN ) );
	pProcess->SmoothHeightMap( this , width , height );
}


void MTerrainEdit::SetHeightMap(int width,int height , unsigned char  *heightmap )
{
	MModifyHeightProcess* pProcess = static_cast<MModifyHeightProcess*>( g_EventProcessFactory.GetEventProcess( CHANGE_HEIGHT_EVN ) );
	pProcess->SetHeightMap( this , width , height , heightmap );
}
	
//主要应用于多种密度的网格
void  MTerrainEdit::CreateHeightMapEX(MPoint2D nblock, MPoint2D ngrid, int minheight ,int maxheight, int randsize,int smoothNum)
{
	MModifyHeightProcess* pProcess = static_cast<MModifyHeightProcess*>( g_EventProcessFactory.GetEventProcess( CHANGE_HEIGHT_EVN ) );
	pProcess->CreateHeightMapEX(this, nblock, ngrid, minheight , maxheight , randsize , smoothNum );
};

bool  MTerrainEdit::CreateTitle(int tX,int tY,float fSize,int gx,int gy,float height , float heightmax , int randsize,int smoothnum,
								const char* basetex , int nRepeat,
								int gwidthnum ,int gheightnum )
{	
	return g_TileProcess.CreateTitle( this ,tX , tY , fSize , gx , gy , height , heightmax , randsize , smoothnum , basetex , nRepeat ,
		gwidthnum , gheightnum );
}
//编辑器渲染	
void MTerrainEdit::RenderHelpLines()
{
	if(m_pPathSet)
	{
		m_pPathSet->Render();
	}

	g_AreaSet.Render ();
	
	if( m_pSignSet && 
		(m_EditOption.mCurEditEvent == CHANGE_HEIGHT_EVN
		||m_EditOption.mCurEditEvent == PANIT_EVN
		||m_EditOption.mCurEditEvent == ADD_OBJECT_EVN
		||m_EditOption.mCurEditEvent == SELECT_OBJECT_EVN
		||m_EditOption.mCurEditEvent == ADD_NPC 
		||m_EditOption.mCurEditEvent == AREA_EIDT_EVEN 
		||m_EditOption.mCurEditEvent == ADD_MORE_OBJECT_EVN
		||m_EditOption.mCurEditEvent == AUTOLINK_OBJECT_EVN 
		||m_EditOption.mCurEditEvent == AUTOLINK_TEXTURE_EVN 
		||m_EditOption.mCurEditEvent == PAINT_TEXTURE_MODEL_EVN  
		||m_EditOption.mCurEditEvent == BRAE_EDIT_EVN 
		||m_EditOption.mCurEditEvent == LIGHT_EDIT_EVN
		||m_EditOption.mCurEditEvent == MODIFY_LIQUID ) )
	{
		m_pSignSet->Render ();
	}

	if( m_EditOption.mCurEditEvent == MOVE_OBJECT_EVN
		|| m_EditOption.mCurEditEvent == ROATE_OBJECT_EVN 
		||  m_EditOption.mCurEditEvent == SCALE_OBJECT_EVN 
		|| m_EditOption.mCurEditEvent == MODIFY_PATH
		|| m_EditOption.mCurEditEvent == ADD_NPC
		|| m_EditOption.mCurEditEvent == LIGHT_EDIT_EVN)
	{
		m_pCoordinate->render();
	}
   
	if( m_EditOption.mCurEditEvent == CHANGE_HEIGHT_EVN 
		&& m_EditOption.mCurChangeHeightMode == RAMP_MODE
		&&m_bBeingRamp)
	{
		m_pCampSign->Render ();
	}
};
	
void   MTerrainEdit::SetShowObject(bool bShow)
{
    if(m_pEditData)
	{
		m_pEditData->bShowObject = bShow;

		for( size_t i = 0 ; i< m_pEditData->modelData.modelSet.size () ; i ++ )
		{
			EditorModel* pModel = (EditorModel*)m_pEditData->modelData.modelSet[i]->GetModelData()->pModel ;
			pModel->GetShowModel()->getMainModel()->show(bShow);
		}
	}

	m_DrawOption.bDrawModel = bShow;
};

void   MTerrainEdit::SetShowLiquid(bool bShow)
{
    if(m_pEditData)
		m_pEditData->bShowWater  = bShow;

	m_DrawOption.bDrawLiquid = bShow ;
};

void   MTerrainEdit::SetShowChunk(bool bShow)
{
    if(m_pEditData)
		m_pEditData->bShowChunk = bShow;

	m_DrawOption.bDrawChunk = bShow;
};

void   MTerrainEdit::SetShowQuadPath(bool bShow)
{
    if(m_pEditData)
		m_pEditData->bShowQuadPath  = bShow;

	m_DrawOption.bDrawQuad  = bShow;
};

void  MTerrainEdit::SetShowPath(bool bShow)
{

};
void  MTerrainEdit::SetShowShadow(bool bShow)
{
	if(!m_pEditData)
      return;

    for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i++)
	{
		for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
		{
			m_pEditData->chunkSet [i][j].GetChunkOptionData ()->bShowShadow = bShow;
		}
	}
};
	
void  MTerrainEdit::UpdateHeightMapToCamera(int x,int y)
{
	if( m_pEditData == NULL )
		return;

	MVec3 pos ;
	
	g_Terrain.GetGridPos ( &pos , 0 , 0 );

	float fSize = TERRUNIT_SIZE ;

	pos.x += x*fSize;
	pos.z += y*fSize;

	MVec3 savePos;
	savePos = g_Camera.m_Pos ;

	float saveHeight;
	g_Terrain.GetHeight ( g_Camera.m_Pos .x , g_Camera.m_Pos .z , &saveHeight );

	g_Terrain.GetHeight ( pos.x , pos.z , &pos.y );

	pos.y += ( g_Camera.m_Pos . y - saveHeight );

	g_Camera.SetPosition ( pos.x ,pos.y ,pos.z  );

}

void MTerrainEdit::SetChunkGridVisible(int gx ,int gz )
{
     if( m_pEditData == NULL )
		 return;

    TerrBlockVertTable*  pVecData = m_pEditData->chunkSet[gz][gx].GetChunkNewVecData();

    MVec2   pos1,pos2;
	pos1.x = m_curSelectInfo.pos .x ;
	pos1.y = m_curSelectInfo.pos .z ;

	float _size = ((MTitle*)g_Terrain.GetTitle (0))->GetHeadData ()->gridSize / pVecData->ngrid ;

	float select_range = m_EditOption.nCurChangeHeightSize*_size;

	 for( int i = 0 ; i < pVecData->ngrid  ; i ++ )
	 {
		 for( int j = 0 ;j < pVecData->ngrid  ; j ++ )
		 {
			 pos2.x = pVecData->vVer [i*(pVecData->ngrid+1) + j].pos .x ;
			 pos2.y = pVecData->vVer [i*(pVecData->ngrid+1) + j].pos .z ;

			float _dist = Distance( pos2,pos1 ) + 4.0f;

			if( _dist <  select_range  )
			{
				int cullx = j ,cully = i;
						
				if( m_EditOption.mCurReduceMode == BEGIN_REDUCE)
				{
		    		m_pEditData->chunkSet[gz][gx].SetCullInfo( cullx , cully  , 1 );
				}
				else if (  m_EditOption.mCurReduceMode == REDUCE_RESUME )
				{		
		    		m_pEditData->chunkSet[gz][gx].SetCullInfo( cullx , cully  , 0 , 1 );
				}
			}
		 }
	 }
}


void  MTerrainEdit::SetCurAlphaSize(int size)
{
	m_EditOption.nPaintPlantSize = size;

	if(m_EditOption.nCurLayer ==0 || m_EditOption.nCurLayer>6)
		return;

	m_EditOption.nCurSelectPaintRad[0] = size;
	m_EditOption.nCurSelectPaintRad[1] = size;
	m_EditOption.nCurSelectPaintRad[2] = size;
	m_EditOption.nCurSelectPaintRad[3] = size;
	m_EditOption.nCurSelectPaintRad[4] = size;
	m_EditOption.nCurSelectPaintRad[5] = size;

    CreateAlphaPaint();
}
			
bool   MTerrainEdit::DelCurPathPos()
{
	m_pCurSelectModel  =  NULL ;	
	m_EditOption.pSelectModel = NULL ;			
	GetCoordinate()->bindObject( NULL  );

     if( m_pPathSet->DelCurPathPos ())
		 return true;

	 return false;
};

int   MTerrainEdit::DelCurPath()
{
	m_pCurSelectModel  =  NULL ;	
	m_EditOption.pSelectModel = NULL ;			
	GetCoordinate()->bindObject( NULL  );

	return m_pPathSet->DelCurPath() ;
};		

void  MTerrainEdit::SetCurPath(int index)
{
	m_pPathSet->SetCurPath(index);
}

void    MTerrainEdit::SetTerrainReport(int  nReport)
{    
	MTitle *pTile = g_Terrain.GetTitle ( 0 ) ;

	if(pTile)	
	{     
		for( int i = 0 ; i < pTile->GetTileData()->gh ; i++)		
		{
			for( int j = 0 ; j < pTile->GetTileData()->gw ; j ++)
			{
				pTile->GetTileData ()->chunkSet [i][j].GetChunkOptionData ()->nRepeat = nReport ;
			}
		}
	}
};

bool MTerrainEdit::AddArea( int id, int type, const char* name ,const char* scriptName )
{
    MapArea*  pArea = new MapArea();

	pArea->SetType(type);
	pArea->SetID ( id );
	pArea->SetName ( name );
	pArea->SetLuaName ( scriptName );

	g_AreaSet.ClearColor ( 0xffff00ff);

	pArea->SetColor ( GREEN_COLOR );
	g_AreaSet.addArea ( pArea );

	SetUpdateAreaData(true);
	return true;
}

	
void  MTerrainEdit::SetTypeModelShow( int type , bool show )
{
	if( m_pEditData == NULL )
		return;

	if(TYPE_PLANT_GRASS == type)
	{
		g_TerrainEdit.m_DrawOption.bDrawPlant = show;
		return;
	}

	for( size_t i = 0 ; i < m_pEditData->modelData .modelSet .size () ; i ++)
	{
		if( m_pEditData->modelData .modelSet [i]->GetModelData ()->cType == type )
		{
			m_pEditData->modelData .modelSet [i]->SetShow ( show );
		}
	}

	g_DrawModelType[type] = show ;

};

void  MTerrainEdit::AddNewStrip()
{
	CStripBase  *pStrip = new CStripBase;
	pStrip->Init ();

	g_StripBaseSet->AddStrip ( pStrip );
}

void MTerrainEdit::AddPath(int id , unsigned int type , char* description)
{
	m_pPathSet->Add(id , type , description);
}

int  MTerrainEdit::AddPath(const char* srcName,const char* destName)
{
	return m_pPathSet->Add ( srcName , destName );
};

void MTerrainEdit::ModifySelPathInfo(int id , unsigned int type , char* description)
{
	m_pPathSet->Modify(id , type , description);
}

bool MTerrainEdit::ReadPaths()
{
	return m_pPathSet->ReadFromDB();
}

bool MTerrainEdit::WritePaths()
{
	return m_pPathSet->WriteToDB();
}

void  MTerrainEdit::ReplaceCurTexToAllSelectLayer()
{
	if( m_pEditData == NULL )
		return;

	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
	{
		for( int j = 0 ; j < TERRTILE_NUMBLOCK_X ; j ++)
		{			
			if( (int)m_pEditData->texData .texFileSet.size () > m_EditOption.nCurSelectTexID )				
			{
				if( m_pEditData->chunkSet [i][j].GetChunkRenderData ()->terrainTex[m_EditOption.nCurLayer] != NULL )
				{
					m_pEditData->chunkSet [i][j].GetChunkRenderData ()->terrainTex [m_EditOption.nCurLayer] = g_TexManager.LoadTex (m_pEditData->texData .texFileSet [m_EditOption.nCurSelectTexID].c_str());
					m_pEditData->chunkSet [i][j].GetChunkData ()->texInfo [m_EditOption.nCurLayer] = m_EditOption.nCurSelectTexID ;
						
				}
			}
		}
	}
}
	
//自动生成浪花
void  MTerrainEdit::CreateWhiteWater()
{
	//m_AutoLinkWhiteWater.CreateWhiteWater(this);
};
	

void  MTerrainEdit::SelectChunkTexInfo(MVec3 dir , MVec3 pos )
{	
	MVec3 pos0;

	if( g_Terrain.TestAndRay (dir,pos,pos0))
	{
		m_CurMousePos = pos0 ;

		int gx , gz;
		if(!g_Terrain.GetGridX(pos0.x,&gx))
		{
			m_CurSelectTexInfo.bSelect = false;
			return;
		}
		if(!g_Terrain.GetGridZ(pos0.z,&gz))
		{
			m_CurSelectTexInfo.bSelect = false;
			return;
		}

		m_CurSelectTexInfo.bSelect = true;
		m_CurSelectTexInfo.gx = gx;
		m_CurSelectTexInfo.gy = gz;
	}
}

void  MTerrainEdit::Process(ActionEvent* event)
{
	//if( m_pEditData == NULL )
	//	return;

	if(  m_bAltCtrlMoveCamera && event->mouseEven == MouseMove )	
	{							
		if( m_AltCtrlMoveY == 0 )
		{	
			m_AltCtrlMoveY = event->my ;
		}
		else
		{
			int yy = (int)( -2*( event->my - m_AltCtrlMoveY )*g_Camera.m_CameraVec );
			
			g_Camera.WalkTerrain( yy*5.0f,&g_Terrain,2);		
			m_AltCtrlMoveY = event->my ;
		}
		return ;
	}

	if( m_EditOption.pEventProcess )
	{
		m_EditOption.pEventProcess->Process( this , event );
	}
};

bool  MTerrainEdit::DeleteModelForChunk( TerrModel* pModel )
{
	int gx,gy;
	gx = pModel->GetModelData ()->gridX ;
	gy = pModel->GetModelData ()->gridY ;

	if( gx != -1 && gy != -1 )
	{
		MChunk* pChunk = &m_pEditData->chunkSet [gy][gx];

		std::vector < TerrModel*>::iterator   iter;

		for( iter = pChunk->m_ModelSet .begin () ; iter != pChunk->m_ModelSet .end () ; iter ++ )
		{
			if( *iter == pModel )
			{
				pChunk->m_ModelSet .erase ( iter );
				return true;
			}	
		}
	}

	return false;
}

void  MTerrainEdit::MoveSelectModel(MVec3 &dir,MVec3 &pos,int mode)
{
	MVec3  pos0;

	if(m_pSignSet)
		m_pSignSet->m_PosSet .clear ();

	if(g_Terrain.TestAndRay(dir,pos,pos0))
	{
		if ( m_EditOption.bMoveGrid )
		{
			int ggx,ggy ;
			if ( g_Terrain.GetGridX( pos0.x , &ggx ) && g_Terrain.GetGridZ( pos0.z , &ggy ) )
			{
				MVec3 chunkPos ;
				g_Terrain.GetGridPos( &chunkPos , ggx , ggy );
                
				float ddx , ddy ;
				ddx = pos0.x - chunkPos.x ;
				ddy = pos0.z - chunkPos.z ;

				unsigned char  mmx ,mmy ;
				mmx = (unsigned char)( ddx / ( TERRBLOCK_SIZE /32.0f ) );
				mmy = (unsigned char)( ddy / ( TERRBLOCK_SIZE /32.0f ) );
               
				pos0 = m_pEditData->chunkSet[ggy][ggx].GetChunkVec( mmx ,mmy );

			}
		}

		m_CurMousePos = pos0 ;
		if(m_pSignSet)
		{
		    m_pSignSet->UpdateData ( pos0, g_fModelSignRad , 16 );
		}

		if( mode == 1)
			return ;
	
		if(m_pCurSelectModel == NULL)	
			return;

		MVec3   dPos = pos0 - m_pCurSelectModel->GetModelData ()->pos ;

		m_pCurSelectModel->GetModelData ()->pos=pos0;
		m_pCurSelectModel->ResetMat();

		for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i ++)
		{
			m_CurSelectModelSet[i]->GetModelData ()->pos += dPos;
			m_CurSelectModelSet[i]->ResetMat();
		}
	}
};

//设置信息对应的信息颜色
void    MTerrainEdit::SetModifyMapInfoColor( int layer , unsigned char r, unsigned char g, unsigned char b)
{
	if( m_pEditData == NULL  )
		return ;

	if ( layer < 0 || layer >= 4)
		return;

	for( int i = 0 ; i < TERRTILE_NUMBLOCK_Z ; i ++ )
	{
		for( int j = 0 ;j < TERRTILE_NUMBLOCK_X ; j ++ )
		{
			TerrBlockLayerInfo* pInfoData = m_pEditData->chunkSet [i][j].GetChunkInfo() ;

			pInfoData->color[layer][0] = r ;
			pInfoData->color[layer][1] = g ;
			pInfoData->color[layer][2] = b ;
		}
	}
};

void  MTerrainEdit::ClearObjectColor(unsigned int color)
{
	if( !m_pEditData )
		return;
  
	if( m_EditOption.pSelectModel )
	{		
		m_EditOption.pSelectModel ->GetModelData ()->pDrawbox ->SetColor ( color );	 
	}
};

void  MTerrainEdit::SetCurPaintInfoData(unsigned char* pData , int bmpSize)
{ 
	m_EditOption.nPaintPlantSize = bmpSize / 2;
	
	m_EditOption.nCurSelectPaintRad [m_EditOption.nCurLayer-1] = bmpSize / 2;
	m_EditOption.bUserPaintPic = true;

	if( bmpSize == 64 )
	{
		memcpy( m_EditOption.alphaPaintData[m_EditOption.nCurLayer-1].data  , pData , 64*64 );
	}
	else
	{
		memset( m_EditOption.alphaPaintData[m_EditOption.nCurLayer-1].data , 0 , 64*64 );

		for( int i = 0 ; i < 32 ; i ++)
		{
			for( int j = 0 ; j < 32 ; j ++)
			{
				m_EditOption.alphaPaintData[m_EditOption.nCurLayer-1].data[16+16*64 + i*64 +j] = pData[i*32+j];
			}
		}
	}
}

void  MTerrainEdit::CreateAlphaPaint()
{
	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*> ( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->CreateAlphaPaint( this );	
};
	

void   MTerrainEdit::SaveAlphaTex( const char* filename, unsigned char* pData,int layer)
{
 	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*> ( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->SaveAlphaTex( this , filename , pData , layer );
}

bool   TestIfLinkMesh(const char* modelName )
{
	std::string strname = modelName ;

	if( strname.find( "linkmesh") != - 1 )
		return true;

	return false;
}

//清除对应信息
void    MTerrainEdit::ClearAll()
{
	if(m_SelResData.pmodel)
	{
		((EditorModel*)m_SelResData.pmodel)->GetShowModel() ->detachFromScene();
		OGRE_DELETE(m_SelResData.pmodel);
	}
	m_pCurSelectModel = NULL;
	m_SelResData.model_id = m_SelResData.model_index = 0;
	
	m_pPathSet->Release ();

	m_HeightMap.m_vHeight.clear ();

	m_pCoordinate->bindObject( NULL );
	
	g_EventProcessFactory.GetEventProcess( CHANGE_HEIGHT_EVN )->Clear();
	
	g_Terrain.Release();
	g_LightManager.Release();

	m_Cull->m_ShowChunkNum = 0;
	m_Cull->m_ShowModelNum = 0;
	
}

//引导地形文件；参数（文件名）
bool  MTerrainEdit::LoadTerrainFile(const char* filename)
{
	if(g_TileProcess.LoadTerrainFile( this , filename ))
	{
		int mapid = GetMapIDFromPath(filename);
		m_GameMapInfo.mapid = mapid;

		return true;
	}
	return false;
}

//保存路径信息；参数（文件名）；返回为真表示成功
bool  MTerrainEdit::SavePathFile(const char* filename)
{
    if(m_pPathSet)
	{
		m_pPathSet->Write ( filename );
		return true;
	}
	else
	{
		return false;
	}
};

//引导路径文件；参数（文件名）；返回为真表示成功
bool  MTerrainEdit::LoadPathFile(const char* filename)
{
   if(m_pPathSet)
   {
	   m_pPathSet->Read ( filename );
	   return true;
   }
   else
   {
	   return false;
   }
};

//保存文件名；参数（文件名）；返回为真表示成功
bool  MTerrainEdit::SaveTerrainFile(const char* filename)
{
	int mapid = GetMapIDFromPath(filename);
	m_GameMapInfo.mapid = mapid;

	return g_TileProcess.SaveTerrainFile( this , filename );
}	

//测试地形物件；参数（gz mz 位置i,位置 j)
bool  MTerrainEdit::TerrainRayTestObject( int gz,int mz ,int i ,int j )
{
	MVec3  pos;
	g_Terrain.GetGridPos ( &pos , i/64 , gz );

    pos.x += mz*TERRTEX_UNITSIZE;
	pos.z += (i%64)*TERRTEX_UNITSIZE;
   
	MVec3  dir( 0 , 1 , 1 );

	if( g_Terrain.GetHeight ( pos.x, pos.z , &pos.y ) )
	{
        // WorldRay 		
		WorldRay ray;
		ray.m_Origin = pos;
		ray.m_Dir    = dir;
	}

	return false;
}
//移动坐标 ；参数（坐标类型，X,Y,位置，方向） 
void  MTerrainEdit::MoveMoveAxes ( COORDINATE_SELECT select_mode , int x ,int y , MVec3 &spos , MVec3 &dir , int mode )
{		
	if(m_pCurSelectModel == NULL)	
			return;

	MVec3 pos = m_SaveCoordinatePos ;
	MVec3 _pos ;
	switch ( select_mode )
	{
	case SELECT_X_AXES:
		pos.x += ( x - m_SaveCoordinateX  );
		break;
	case SELECT_Y_AXES:
		pos.y += ( m_SaveCoordinateY - y );
		break; 
	case SELECT_Z_AXES:
		pos.z += ( m_SaveCoordinateY - y );
		break;
	case SELECT_XY_AXES:
		{
		_pos  = m_pCoordinate->intersectXY( spos , dir );
		pos.x +=  ( _pos.x - m_pCoordinate->m_CurPos.x     ) ;
		pos.y +=  (_pos.y  -  m_pCoordinate->m_CurPos.y ) ;
		}

		break;
	case SELECT_YZ_AXES:
		{
		 _pos  = m_pCoordinate->intersectYZ( spos , dir );
		pos.z +=  _pos.z - m_pCoordinate->m_CurPos.z   ;
		pos.y +=  _pos.y - m_pCoordinate->m_CurPos.y  ;
		}
		break; 
	case SELECT_XZ_AXES:
		{
			_pos  = m_pCoordinate->intersectXZ( spos , dir );

			if( mode == 1 )
			{
                pos.x = _pos.x ;
				pos.z = _pos.z ;
			}
			else
			{
				pos.x +=  _pos.x - m_pCoordinate->m_CurPos.x;
				pos.z +=   _pos.z - m_pCoordinate->m_CurPos.z;
			}
		}
		break;
	}	

	MVec3 dpos = pos - m_pCurSelectModel->GetModelData ()->pos;
	m_pCurSelectModel->GetModelData ()->pos = pos;
	m_pCurSelectModel->ResetMat();
	//
	for( size_t i = 0 ; i < m_CurSelectModelSet.size () ; i ++)
	{	
		m_CurSelectModelSet[i]->GetModelData ()->pos += dpos;
		m_CurSelectModelSet[i]->ResetMat();		
	}
};

//移动坐标；参数（X,Y ）	
void  MTerrainEdit::SetMoveAxes( int x ,int y)
{
	if( m_pCurSelectModel == NULL )
		return;

	m_SaveCoordinateX = x;
	m_SaveCoordinateY = y;
		
	m_SaveCoordinatePos = m_pCurSelectModel->GetModelData ()->pos ;
};
//显示NPC；参数（npcID )
void      MTerrainEdit::ShowNpc( int npcID )
{
	if( npcID == -1 )
		return;

	ShowActor* pActor = new ShowActor(npcID);
	Entity* pShowModel = pActor->getCurEntity();
	pShowModel->update(0);
	pShowModel->addRef();
	pActor->release();

	if(m_SelResData.pmodel)
	{
		((EditorModel*)m_SelResData.pmodel)->GetShowModel() ->detachFromScene();
		delete m_SelResData.pmodel;
	}

	m_SelResData.pmodel = new EditorModel;
	m_SelResData.pmodel->SetShowModel(pShowModel);

	pShowModel->attachToScene(m_pResPreviewRU->m_pScene);

	const BoxSphereBound &bound = pShowModel->getWorldBounds();
	ComputeModelViewCamera(m_ResPreviewCamera, bound);

	pShowModel->setPosition(m_SelResData.pos);
}

//设置当前选择NPC；参数（NPCID ）  
void      MTerrainEdit::SetSelectNpcID( int npcID )
{ 
	if( npcID == -1 )
		return;

	m_EditOption.nCurSelectNpcID = npcID ;

	GetCoordinate()->bindObject(NULL);
	g_Camera.BindObject(NULL);
};

//默认对应的纹理
char  g_TileTexName[3][128] =
{
	"source.scene.texture.tileset.maydemo.maydemo_02",
	"source.scene.texture.tileset.maydemo.maydemo_02",
	"source.scene.texture.tileset.maydemo.maydemo_03"
};
//更新植物数据；参数文件名
void      MTerrainEdit::UpdatePlantData(const char* filename )
{
	if( m_pEditData == NULL )
		return;

	MStream  stream;
	
	if ( !stream.ReadFile ( filename ) )
		return;

	BITMAPFILEHEADER header;	
	BITMAPINFOHEADER  headInfo;

	stream.Read  ( header );
	stream.Read  ( headInfo );

	if( headInfo.biWidth != 1024 ||
		headInfo.biHeight != 1024 )
		return;

	if( headInfo.biBitCount == 24 )
	{
		int size = headInfo.biWidth * headInfo.biHeight * 3 ;

		unsigned char* alphaMap = new unsigned char [size];
		stream.ReadUnChar ( alphaMap , size );

		for( int i = 0 ; i < headInfo.biHeight ; i ++)
		{
			for( int j = 0 ; j < headInfo.biWidth ; j ++)
			{
				g_PlantMgr->m_PlantMap[i*headInfo.biWidth + j ] = alphaMap[ ( headInfo.biHeight  - 1 - i ) * 3 *headInfo.biWidth + j*3 ] ;
			}
		}
	}

	std::string  sizeInfo = filename ;

	int index = (int)sizeInfo.rfind (".");

	std::string newfile = sizeInfo.substr ( 0 , index ) + "_a.bmp" ;
	
	strcpy( g_PlantMgr->m_strPlantInfoFile , newfile.c_str() );

	g_PlantMgr->CreateChunkPlant ();

}
//更新color数据；参数（文件名）
void      MTerrainEdit::UpdateColorData(const char* filename )
{   
	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*> ( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->UpdateColorData( this , filename );
}
//更新shadow数据；参数（文件名）
void      MTerrainEdit::UpdateShadowData(const char* filename )
{
	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*> ( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );
	pProcess->UpdateShadowData( this , filename );    
}

//增加纹理文件给编辑数据；参数（文件名）
int       MTerrainEdit::AddTextureFileToTile(const char* filename)
{
	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );	
	return pProcess->AddTextureFileToTile( this , filename );
}

//引导地图层信息
void      MTerrainEdit::LoadTerainLayerInfo()
{
	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );	
	pProcess->LoadTerainLayerInfo( this );
}

//更新半透明数据；参数（文件名，对于那个层)
void      MTerrainEdit::UpdateAlphaData(const char* filename, int layer)
{
  	MPaintTextureProcess* pProcess = static_cast<MPaintTextureProcess*>( g_EventProcessFactory.GetEventProcess( PANIT_EVN ) );	
	pProcess->UpdateAlphaData( this , filename , layer );   
};

//增加NPC ; 参数；（NPC名，模型位置；NPCID ，位置，方向，脚本 ）
void      MTerrainEdit::AddNPC(const char* npcName , const char* npcModelPath ,  unsigned int npcID,
	          MVec3 npcPos, MVec3 npcDir ,const char* script )
			  
{
	MAddNpcProcess* pProcess = static_cast<MAddNpcProcess*>( g_EventProcessFactory.GetEventProcess( ADD_NPC ) );	
	pProcess->AddNPC( this , npcName , npcModelPath , npcID , npcPos , npcDir , script );
};
   
  //设置NPC脚本；参数（索引，脚本名） 
void      MTerrainEdit::SetNpcScript (int index , const char* npcScript )
{
	if( index < 0 || index >= (int)m_pNpcSet->m_NpcSet .size () )
	{
		return;
	}

	m_pNpcSet->m_NpcSet [index]->m_NPCScript = npcScript;
};

//选择地图NPC；参数（索引）；返回（为真表示成功）
bool      MTerrainEdit::SelectMapNPC(int index)
{
	if( index <0 || index >= (int)m_pNpcSet->m_NpcSet .size() )
	{
		return false;
	}
 
  //   m_pCurSelectModel = &m_pNpcSet->m_NpcSet [ index ]->m_Model;
	// MVec3 curPos = m_pCurSelectModel->GetModelData ()->pos ;
	 //g_Camera.setTarget(curPos.x, curPos.y, curPos.z);

	TerrModel *model = &m_pNpcSet->m_NpcSet[index]->m_Model;
	 m_EditOption.pSelectModel = model;
	 GetCoordinate()->bindObject ( model->GetModelData ()->pModel );
	 g_Camera.BindObject ( model );

	MVec3 curPos = model->GetModelData ()->pos ;
	 g_Camera.setTarget(curPos.x, curPos.y, curPos.z);
    return true;
};
		
//保存地图编辑信息
bool  MTerrainEdit::SaveMapEditIni()
{
    char  terrainPatch[260];
  
	strcpy(terrainPatch,g_path.c_str ());
	strcat(terrainPatch,"\\mapedit.ini");

	char info[128];
	sprintf(info,"0x%x",(int)m_MapEditInfo.nAmibentLightColor);
	WritePrivateProfileString("Info","AmbientLightColor",info,terrainPatch);	
	 
	sprintf(info,"0x%x",(int)m_MapEditInfo.nDirectLightColor );
	WritePrivateProfileString("Info","DirectLightColor",info,terrainPatch);

	sprintf(info,"%d",(int)m_MapEditInfo.vDirectLightX );
	WritePrivateProfileString("Info","DirectLightX",info,terrainPatch);
	sprintf(info,"%d",(int)m_MapEditInfo.vDirectLightY );
	WritePrivateProfileString("Info","DirectLightY",info,terrainPatch);
	sprintf(info,"%d",(int)m_MapEditInfo.vDirectLightZ );
	WritePrivateProfileString("Info","DirectLightZ",info,terrainPatch);

	return true;
}	

//动态引导斜坡信息
void  MTerrainEdit::LoadingBrea()
{
		MBraeEditProcess* pProcess = static_cast<MBraeEditProcess*>( g_EventProcessFactory.GetEventProcess( BRAE_EDIT_EVN ) );
		pProcess->Loading( this );
}

//引导地图斜坡信息；参数（文件名）
void  MTerrainEdit::LoadBrae( const char* filename )
{
		MBraeEditProcess* pProcess = static_cast<MBraeEditProcess*>( g_EventProcessFactory.GetEventProcess( BRAE_EDIT_EVN ) );
		pProcess->Read( this , filename );
};
	
//保存地图的斜坡信息；参数（文件名）
void  MTerrainEdit::SaveBrae( const char* filename )
{
		MBraeEditProcess* pProcess = static_cast<MBraeEditProcess*>( g_EventProcessFactory.GetEventProcess( BRAE_EDIT_EVN ) );
		pProcess->Save( filename );
};

//引导地图信息;返回（为真表示成功）
bool  MTerrainEdit::LoadMapEditIni()
{
	m_MapEditInfo.nAmibentLightColor      =  0xffffffff;
	m_MapEditInfo.nDirectLightColor       =  0xffffffff;
	m_MapEditInfo.nMouseMoveScreen        =  100;
	m_MapEditInfo.nMoveObject             =  120;
    m_MapEditInfo.nRoateObject            =  100;

	m_MapEditInfo.vDirectLightX           = 0;
	m_MapEditInfo.vDirectLightY           = 0;
	m_MapEditInfo.vDirectLightZ           = 0;

	return true;
};

//保存区域文件；参数（文件名）返回（为真表示成功）
bool  MTerrainEdit::SaveAreaFile(const char* filename )
{
	std::string strFileName ;
    if(NULL == filename)
    {
        if(m_curSaveFile.empty())
        {
            TCHAR szFilename[MAX_PATH] = TEXT("");
            BOOL  bResult = FALSE;
            DWORD dwError = NOERROR;
            OPENFILENAME  ofn = {0};
            ofn.lStructSize = sizeof( OPENFILENAME );
            ofn.lpstrFilter = TEXT("area file\0*.area\0\0");
            ofn.lpstrFile  = szFilename;
            ofn.nMaxFile  = MAX_PATH;
            ofn.lpstrTitle = TEXT("保存区域信息");
            ofn.Flags     = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY |
                OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;

            bResult = GetSaveFileName( &ofn );
            if( !bResult )
                return false;
            SetCurrentDirectory(g_path.c_str ());
        }
        else
            strFileName = m_curSaveFile;

        strFileName = strFileName + ".area" ;
    }
    else
        strFileName = filename;

	if( g_AreaSet.Save ( strFileName.c_str() ) )
		return true;

	return false;
};

//引导区域文件；参数（文件名）返回（真表示成功）
bool  MTerrainEdit::LoadAreaFile(const char* filename )
{
	std::string strFileName ;
	strFileName = m_curSaveFile;

	strFileName = strFileName + ".area" ;

	if( g_AreaSet.Load ( strFileName.c_str() ))
	{
		m_EditOption.bUpdateAreaData = true;
		return true;
	}

	return false;
};
//生成小地图；参数（ 文件名 ）   
void   MTerrainEdit::CreateMinMap( const char* filename )
{
	/*
	TerrainRoom *pTerrainRoom = new TerrainRoom(1,1,53336 );

	MVec3 pos(0,0,0);

	pTerrainRoom->InitKdtree(MVec3(pos.x + 53300.0f / 2.0f, pos.y + 4000.0f, pos.z + 53300.0f / 2.0f),
		MVec3(70000.0f / 2.0f, 20000.0f, 70000.0f / 2.0f));	
	pTerrainRoom->SetAmbientLight ( 1.0f ,1.0f ,1.0f );
    
	ResourceLoader *ploader = g_pEngine->GetResLoader(); 		
	TerrainTile*  pTileSourece  =  static_cast<TerrainTile *>(ploader->BlockLoad ( filename ));
	
	assert( pTileSourece );

	pTerrainRoom->SetTerrainTile (0,0,pTileSourece);

	MSAFE_RELEASE( pTileSourece );

	g_pEngine->GetSceneMgr ()->AttachRoom ( pTerrainRoom );

	GraphicOptions GO;
	GO.m_nBloom = 0;
	GO.m_bUnderWaterFog = false;

	GO.m_nParticalDetailLevel = 3;
	GO.m_nShadowLevel = 1;
	GO.m_nViewDistance = 3;
	GO.m_nWaterLevel   = 1;
	GO.m_nShaderLevel = 3 ;
	GO.m_nTerrainLevel = 3;

	g_pEngine->SetGraphicOptions(GO,pTerrainRoom);

	std::string   lightmapFileName = filename ;

	std::string  smallMapFileName = "chunkdata\\"+ lightmapFileName + "_s.dds" ;

    TerrainMap terrainMap( g_pEngine->GetRenderer () , pTerrainRoom );
	terrainMap.GetTexture ();
	terrainMap.SaveChunk ( "shadowex" ,smallMapFileName.c_str() , lightmapFileName.c_str() );*/
};

//创建MAY阴影图；参数（文件名）
void  MTerrainEdit::CreateMayShadow(const char* filename)
{/*
	//有疑问？为何不用浮点精度?
	TerrainRoom *pTerrainRoom = new TerrainRoom(1,1,53336 );

	MVec3 pos(0,0,0);

	pTerrainRoom->InitKdtree(MVec3(pos.x + 53300.0f / 2.0f, pos.y + 4000.0f, pos.z + 53300.0f / 2.0f),
		MVec3(70000.0f / 2.0f, 20000.0f, 70000.0f / 2.0f));	
	pTerrainRoom->SetAmbientLight ( 1.0f ,1.0f ,1.0f );
    
	ResourceLoader *ploader = g_pEngine->GetResLoader(); 		
	TerrainTile*  pTileSourece  =  static_cast<TerrainTile *>(ploader->BlockLoad ( filename ));
	
	assert( pTileSourece );

	MVec3  lightDir;
	
	lightDir.x = -g_AmbientInfo.LightDirect.x ;
	lightDir.y = -g_AmbientInfo.LightDirect.y ;	
	lightDir.z = -g_AmbientInfo.LightDirect.z ;

	pTerrainRoom->SetTerrainTile (0,0,pTileSourece);

	MSAFE_RELEASE( pTileSourece );

	g_pEngine->GetSceneMgr ()->AttachRoom ( pTerrainRoom );

	GraphicOptions GO;
	GO.m_nBloom = 0;
	GO.m_bUnderWaterFog = false;

	GO.m_nParticalDetailLevel = 3;
	GO.m_nShadowLevel = 1;
	GO.m_nViewDistance = 3;
	GO.m_nWaterLevel   = 1;
	GO.m_nShaderLevel = 3 ;
	GO.m_nTerrainLevel = 3;

	g_pEngine->SetGraphicOptions(GO,pTerrainRoom);

	std::string   lightmapFileName = filename ;

	std::string  smallMapFileName = "chunkdata\\"+ lightmapFileName + "_s.dds" ;
    TerrainMap terrainMap( g_pEngine->GetRenderer () , pTerrainRoom );
	terrainMap.GetTexture ();
	terrainMap.SaveChunk ( "shadowex" ,smallMapFileName.c_str() );

	lightmapFileName = "chunkdata." + lightmapFileName  ;
    
	LightMapGen *pLightMap = new LightMapGen(g_pEngine->GetRenderer() ,pTerrainRoom);


	pLightMap->SetLightDir( lightDir );
	pLightMap->SaveChunk( lightmapFileName.c_str() ,"shadow2.bmp");
   
	g_pEngine->GetSceneMgr ()->DetachRoom  ( pTerrainRoom );

	MAY_SAFERELEASE( pTerrainRoom );
	*/

}

//生成地形自遮挡阴影。
void MTerrainEdit::CreateSelfShadow(const char* filename)
{	/*
	CreateMayShadow(filename);
	return;
		
	if( !m_curSaveFile.empty ())
	{
		SaveTerrainFile( m_curSaveFile.c_str ()) ;	

		std::string shadowfielname = m_curSaveFile ;
         int index = shadowfielname.find("chunkdata");
				
		 shadowfielname = shadowfielname.substr ( index + strlen("chunkdata") + 1 );	
		 CreateMayShadow(shadowfielname.c_str());
     }
	else
	{
		SaveTerrainFile( "chunkdata.test") ;
		CreateMayShadow( "test");
	}	
	return;*/
}
