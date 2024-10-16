
#ifndef __OgreUIRenderTypes_H__
#define __OgreUIRenderTypes_H__

#include "OgreRect.h"
#include "OgreColourValue.h"

#define FONTSYTLE_NORMAL	0
#define FONTSYTLE_SHADOW	1
#define FONTSYTLE_BORDER	2
#define FONTSYTLE_EMBOLDEN	3

// ������չ���ͣ���������ʱ����
#define FONTTYPE_NORMAL		0		// ��ͨ
#define FONTTYPE_ITALIC		1		// б��
#define FONTTYPE_UNDERLINE	2		// �»���

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
		UI_UVT_NORMAL,				// ��ͨ
		UI_UVT_TURN90,				// ˳ʱ����ת90��
		UI_UVT_TURN180,				// ˳ʱ����ת180��
		UI_UVT_TURN270,				// ˳ʱ����ת270��
		UI_UVT_MIRROEU,				// ��U������
		UI_UVT_MIRROEV,				// ��V������
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