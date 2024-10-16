
#ifndef __OgreUIRenderTypes_H__
#define __OgreUIRenderTypes_H__

#include "OgreRect.h"
#include "OgreColourValue.h"

#define FONTSYTLE_NORMAL	0
#define FONTSYTLE_SHADOW	1
#define FONTSYTLE_BORDER	2
#define FONTSYTLE_EMBOLDEN	3

// 字体扩展类型，创建字体时传入
#define FONTTYPE_NORMAL		0		// 普通
#define FONTTYPE_ITALIC		1		// 斜体
#define FONTTYPE_UNDERLINE	2		// 下划线

#define TEXTALIGN_LEFT			1
#define TEXTALIGN_HCENTER		2
#define TEXTALIGN_RIGHT			4
#define TEXTALIGN_TOP			8
#define TEXTALIGN_VCENTER		16
#define TEXTALIGN_BOTTOM		32

const Ogre::ColorQuad FONT_SHADOW_COLOR(0,0,0);

namespace Ogre
{
	typedef void * HFONT;
	typedef void * HUIRES;

	typedef BlendMode UIBlendMode;

	enum ECharacterCoding
	{
		CCODING_GBK,
		CCODING_UTF8
	};

	enum UiUvType
	{
		UI_UVT_NORMAL,				// 普通
		UI_UVT_TURN90,				// 顺时针旋转90度
		UI_UVT_TURN180,				// 顺时针旋转180度
		UI_UVT_TURN270,				// 顺时针旋转270度
		UI_UVT_MIRROEU,				// 按U方向镜像
		UI_UVT_MIRROEV,				// 按V方向镜像
	};

	struct DrawRect
	{
		RectFloat		rect;
		RectFloat		rectUV;
		ColorQuad		color;
	};


	class ICharacterCoding;
	class IFont;
	class IFontGlyphMap;
	class ScreenTexture;

	static RectInt EMPTY_RECT_INT(0,0,0,0);
}

#endif