
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
		HUIRES			m_hUiRes;					// ������ͼ���

		int				m_nCharCountW;				// �����ַ�һ�и���
		int				m_nCharCountH;				// �����ַ�������

		// �ַ�Unicode�뵽���ε�ӳ��
		std::map<uint, BitmapFontGlyph> m_mapGlyph;

		// �յ�ʵ�֣���Ϊ����Ҫ������ͼ��������������
		virtual void JustBeforeRender(){}

		virtual void TextureMap(const unsigned char* szChar, HUIRES& hUiRes, RectFloat &rectUV);

		// ȡ������UNICODE�ַ��ĳ��Ϳ�
		virtual void GetCharSize(ushort wChar, float &fWidth, float &fHeight);

		// ���ݴ����UNICODE�ַ����ض�Ӧ�ĵ���λͼ
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

		// ���ݴ���Ĳ����ж���Ҫ�����������Ƿ��뱾�����ǵȼ۵�
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType);
	};
}

#endif