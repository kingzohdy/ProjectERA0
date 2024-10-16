
#ifndef __OgreFontBitmapImpl_H__
#define __OgreFontBitmapImpl_H__

#include "OgreFontBase.h"
#include "OgreFontGlyphMap.h"
#include <map>

namespace Ogre
{
	class RFontBitmapImpl : public RFontBase, public IFontGlyphMap
	{
	public:
		struct BitmapFontGlyph
		{
			int			nX;
			int			nY;
			int			nW;
			int			nH;
			int			nAdvance;
			RectFloat	rectUV;
		};

	protected:
		std::string		m_strType;
		std::string		m_strTexFilename;

		Texture*		m_pFontTex;
		HUIRES			m_hUiRes;					// 缓存贴图句柄

		int				m_nCharCountW;				// 缓存字符一行个数
		int				m_nCharCountH;				// 缓存字符的行数

		// 字符Unicode码到字形的映射
		std::map<uint, BitmapFontGlyph> m_mapGlyph;

		// 空的实现，因为不需要处理贴图锁定、解锁操作
		virtual void JustBeforeRender(){}

		virtual void TextureMap(const unsigned char* szChar, HUIRES& hUiRes, RectFloat &rectUV);

		// 取得所传UNICODE字符的长和宽
		virtual void GetCharSize(ushort wChar, float &fWidth, float &fHeight);

		// 根据传入的UNICODE字符反回对应的点阵位图
		virtual uchar* GetCharBitmap(ushort wChar);

	public:
		RFontBitmapImpl()
		{
			m_nTexWidth = m_nTexHeight = RFONT_TEXTURESIZE;
			m_pFontTex = NULL;
			m_hUiRes = NULL;
			m_nMaxCacheSize = 10000;
		}

		~RFontBitmapImpl();

		bool Init(UIRenderer* pUiRenderer, const char *pFontFileName, ECharacterCoding eCoding);

		// 根据传入的参数判断所要创建的字体是否与本字体是等价的
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType);
	};
}

#endif