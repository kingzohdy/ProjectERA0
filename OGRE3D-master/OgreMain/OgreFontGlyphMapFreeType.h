
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
		int					m_nBaseLine;			// ���ߵ������ϱ߽�ľ��룬��λ������
		int					m_nUnderLinePos;		// �»��ߵ����ߵ�ƫ��
		int					m_nUnderLineThick;		// �»��ߵĿ��
		bool				m_bUnderLine;			// �Ƿ�����»���
		int					m_nFontWidth;			// ������
		int					m_nFontHeight;			// ����߶�
		int					m_nBitmapHeight;

		FT_Face				m_pkFace;

		float				m_fEnCharWidth[256];	// Ϊ��Ч�����⣬����Ӣ���ַ��Ŀ��

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

		// ȡ������UNICODE�ַ��ĳ��Ϳ�
		virtual void GetCharSize(UINT16 wChar, float &fWidth, float &fHeight);

		// ���ݴ����UNICODE�ַ����ض�Ӧ�ĵ���λͼ
		virtual uchar* GetCharBitmap(UINT16 wChar);

		static bool InitFreeType();
		static void TerminateFreeType();
		static FT_Face CreateFontFace(const char* szFontName);

		static std :: vector<FontFaceInfo>	m_vecFontFaces;
	};
}

#endif