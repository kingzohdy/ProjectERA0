
#pragma once

#include "OgreTexture.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreVector4.h"
#include "OgreRect.h"
#include "OgreArchive.h"
#include "OgreLight.h"
#include "OgreColourValue.h"
#include "OgreWorldPos.h"
#include "OgreShader.h"
#include "OgreBounding.h"
#include "OgreEntity.h"
#include "OgreLines.h"
#include "OgreRay.h"

#include <map>

#define  RED_COLOR   0x77ff0000
#define  BLUE_COLOR  0x770000ff
#define  WHITE_COLOR 0x77ffffff
#define  BLACK_COLOR  0x77000000
#define  GREEN_COLOR  0x7700ff00

const int TERRTILE_NUMBLOCK = 16;
const int TERRTILE_NUMBLOCK_X = 16;
const int TERRTILE_NUMBLOCK_Z = 16;
const int TERRBLOCK_NUMVERT0 = 9*9 + 8*8; //第0级细节block的顶点数目
const int TERRAIN_TEX_NUM = 4;
const int TERRAIN_ALPHATEX_NUM = 3;
const float TERRTILE_SIZE = 51200.0f;
const float TERRBLOCK_SIZE = ((TERRTILE_SIZE) / (TERRTILE_NUMBLOCK*1.0f));
const float TERRUNIT_SIZE = (TERRBLOCK_SIZE / 8.0f);
const float TERRTEX_UNITSIZE = (TERRUNIT_SIZE / 8.0f);
const int TERRBLOCK_STRIPSIZE = 8*18 + 7*2;

#define  RED_COLOR   0x77ff0000
#define  BLUE_COLOR  0x770000ff
#define  WHITE_COLOR 0x77ffffff
#define  BLACK_COLOR  0x77000000
#define  GREEN_COLOR  0x7700ff00

#define NAME_LENGTH   256

namespace TE
{
	class TerrModel;
	struct TerrBlockData;
	struct TerrBlockLiquidData;
	struct MChunkRenderPassSet;
	class MapPath;
	class GrassGroup;
	class Terrain;
	class EditorModel;
	class Coordinate;
	class MapNpcManager;
	struct MTileData;
	class EditorCamera;
	class MapMarkGroup;
	class M2DPic;
	class MDBFileManager;
	class SlopeMark;
	class LinkMeshManager;
	class LinkTextureManager;
	class LinkOverlayManager;
	class MDBFileManager;
	class TerrainCull;

	enum COORDINATE_SELECT
	{
		NO_AXES_SELECT = -1,
		SELECT_X_AXES = 0,
		SELECT_Y_AXES,
		SELECT_Z_AXES,
		SELECT_XY_AXES,
		SELECT_YZ_AXES,
		SELECT_XZ_AXES,
	};

	enum DRAW_FILL_MODE
	{
		DRAW_SOLID = 0 ,
		DRAW_WIREFRAME ,
		DRAW_POINT,
	};

	enum EDITEVENT_MOUSE
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

	enum EDITEVENT_KEY
	{
		KeyDown = 0,
		KeyUp   ,
	};

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

	// 编辑器事件类型
	/*
	根据不同的编辑模式生成不同的事件处理对象，进行相应的处理。
	*/
	enum EDIT_EVENT
	{
		UNKOWN_ACTION   = 0 ,
		PANIT_EVN ,            //地表编辑
		CHANGE_HEIGHT_EVN,     //地形编辑
		EDITEVENT_OBJECTBRUSH,
		ADD_MORE_OBJECT_EVN,   //增加大量物件
		MODIFY_LIQUID,         //编辑液体
		MODIFY_BEACH,		// 海滩编辑
		MODIFY_PATH,          //编辑路径
		AREA_EIDT_EVEN,        //区域编辑
		SELECT_AREA_EVEN,     //选择区域
		DECAL_EVN,              //贴花
		AUTOLINK_OBJECT_EVN,    //刷悬崖
		AUTOLINK_TEXTURE_EVN,   //自动拼接地表
		PAINT_TEXTURE_MODEL_EVN, //通过纹理生成模型
		WALLPAPER_EVN,          //地形块编辑
		USER_INPUT_EVN,          //用户输入
		BRAE_EDIT_EVN,           //斜坡编辑
		PLACE_PLYAER,              //player预览
		SOUND_EDIT_EVN ,           //声音编辑
		EVEN_NUMBER,
	};

	enum OBJSELECT_MODE
	{
		OBJSELECT_NORMAL = 0,
		OBJSELECT_ADD,
		OBJSELECT_DEL
	};

	struct TerrainVertex
	{
		Ogre::Vector3 pos;
		Ogre::Vector3 normal;
		unsigned int color;
		Ogre::Vector2 texcoord;
		Ogre::Vector2 texcoord2;
	};

	struct TerrainBox
	{
		Ogre::Vector3 minpos;
		Ogre::Vector3 maxpos;
	};

	struct TerrainSphere
	{
		Ogre::Vector3 center;
		float radius;
	};
};

extern Ogre::Root *g_pEngine;
extern Ogre::Camera *g_pCamera;


