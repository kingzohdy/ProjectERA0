
#ifndef __OgreFontGlyphMapFreeType_H__
#define __OgreFontGlyphMapFreeType_H__

#include "OgreFontGlyphMap.h"
#include "OgreUIRenderTypes.h"
#include "freetype/ft2build.h"
#include "freetype/ftoutln.h"
#include <Windows.h>
#include <vector>
#include <string>

namespace Ogre
{
	class FontGlyphMapFreeType: public IFontGlyphMap
	{
	private:
		int					m_nBaseLine;			// 基线到文字上边界的距离，单位是像素
		int					m_nUnderLinePos;		// 下划线到基线的偏移
		int					m_nUnderLineThick;		// 下划线的宽度
		bool				m_bUnderLine;			// 是否绘制下划线
		int					m_nFontWidth;			// 字体宽度
		int					m_nFontHeight;			// 字体高度
		int					m_nBitmapHeight;

		FT_Face				m_pkFace;

		float				m_fEnCharWidth[256];	// 为了效率问题，缓冲英文字符的宽度

		uchar*				m_pBitmapAlpha;

		static FT_Library	m_pkFTLibrary;

		void LoadCharGlyph(UINT16 wChar);

	public:
		struct FontFaceInfo
		{
			std::string kFontFaceName;
			FT_Face pkFace;
			DataStream *fp;
		};

		FontGlyphMapFreeType();
		~FontGlyphMapFreeType();

		bool Init(int nFontWidth, int nFontHeight, const char* szFontName, 
			unsigned int nExtType = FONTTYPE_NORMAL);

		// 取得所传UNICODE字符的长和宽
		virtual void GetCharSize(UINT16 wChar, float &fWidth, float &fHeight);

		// 根据传入的UNICODE字符反回对应的点阵位图
		virtual uchar* GetCharBitmap(UINT16 wChar);

		static bool InitFreeType();
		static void TerminateFreeType();
		static FT_Face CreateFontFace(const char* szFontName);

		static std :: vector<FontFaceInfo>	m_vecFontFaces;
	};
}

#endif