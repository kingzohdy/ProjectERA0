
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
const int TERRBLOCK_NUMVERT0 = 9*9 + 8*8; //��0��ϸ��block�Ķ�����Ŀ
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

	// �༭���¼�����
	/*
	���ݲ�ͬ�ı༭ģʽ���ɲ�ͬ���¼�������󣬽�����Ӧ�Ĵ���
	*/
	enum EDIT_EVENT
	{
		UNKOWN_ACTION   = 0 ,
		PANIT_EVN ,            //�ر�༭
		CHANGE_HEIGHT_EVN,     //���α༭
		EDITEVENT_OBJECTBRUSH,
		ADD_MORE_OBJECT_EVN,   //���Ӵ������
		MODIFY_LIQUID,         //�༭Һ��
		MODIFY_BEACH,		// ��̲�༭
		MODIFY_PATH,          //�༭·��
		AREA_EIDT_EVEN,        //����༭
		SELECT_AREA_EVEN,     //ѡ������
		DECAL_EVN,              //����
		AUTOLINK_OBJECT_EVN,    //ˢ����
		AUTOLINK_TEXTURE_EVN,   //�Զ�ƴ�ӵر�
		PAINT_TEXTURE_MODEL_EVN, //ͨ����������ģ��
		WALLPAPER_EVN,          //���ο�༭
		USER_INPUT_EVN,          //�û�����
		BRAE_EDIT_EVN,           //б�±༭
		PLACE_PLYAER,              //playerԤ��
		SOUND_EDIT_EVN ,           //�����༭
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


