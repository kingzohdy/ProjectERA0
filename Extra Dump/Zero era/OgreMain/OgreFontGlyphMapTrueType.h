
#ifndef __OgreFontGlyphMapTreeType_H__
#define __OgreFontGlyphMapTreeType_H__

#include "OgreFontGlyphMap.h"
#include "OgreUIRenderTypes.h"
#include <Windows.h>

namespace Ogre
{
	class FontGlyphMapTrueType: public IFontGlyphMap
	{
	private:
		HDC					m_hDC;					//用于输出字体的显示设备句柄
		HFONT				m_hFont;				//用于输出字体Windows字体句柄
		HBITMAP				m_hbmBitmap;			//用于输出字体内存位图句柄
		HGDIOBJ				m_hOldBitmap,m_hOldFont;
		uint*				m_pBitmapBits;			//用于读取内存位图的数据指针
		uchar*				m_pBitmapAlpha;

		float				m_fEnCharWidth[256];	// 为了效率问题，缓冲英文字符的宽度

		int					m_nFontWidth;			//字体宽度
		int					m_nFontHeight;			//字体高度

	public:
		FontGlyphMapTrueType();
		~FontGlyphMapTrueType();

		bool Init(int nFontWidth, int nFontHeight, const char* szFontName, 
			unsigned int nExtType = FONTTYPE_NORMAL);

		// 取得所传UNICODE字符的长和宽
		virtual void GetCharSize(UINT16 wChar, float &fWidth, float &fHeight);

		// 根据传入的UNICODE字符反回对应的点阵位图
		virtual uchar* GetCharBitmap(UINT16 wChar);
	};
}

#endif