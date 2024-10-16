
#ifndef __OgreFontGlyphMap_H__
#define __OgreFontGlyphMap_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	class IFontGlyphMap
	{
	public:
		virtual ~IFontGlyphMap(){}

		// 取得所传UNICODE字符的长和宽
		virtual void GetCharSize(ushort wChar, float &fWidth, float &fHeight) = 0;

		// 根据传入的UNICODE字符反回对应的点阵位图
		virtual uchar* GetCharBitmap(ushort wChar) = 0;
	};
}

#endif