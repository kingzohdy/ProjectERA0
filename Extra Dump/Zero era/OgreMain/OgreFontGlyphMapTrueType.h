
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
		HDC					m_hDC;					//��������������ʾ�豸���
		HFONT				m_hFont;				//�����������Windows������
		HBITMAP				m_hbmBitmap;			//������������ڴ�λͼ���
		HGDIOBJ				m_hOldBitmap,m_hOldFont;
		uint*				m_pBitmapBits;			//���ڶ�ȡ�ڴ�λͼ������ָ��
		uchar*				m_pBitmapAlpha;

		float				m_fEnCharWidth[256];	// Ϊ��Ч�����⣬����Ӣ���ַ��Ŀ��

		int					m_nFontWidth;			//������
		int					m_nFontHeight;			//����߶�

	public:
		FontGlyphMapTrueType();
		~FontGlyphMapTrueType();

		bool Init(int nFontWidth, int nFontHeight, const char* szFontName, 
			unsigned int nExtType = FONTTYPE_NORMAL);

		// ȡ������UNICODE�ַ��ĳ��Ϳ�
		virtual void GetCharSize(UINT16 wChar, float &fWidth, float &fHeight);

		// ���ݴ����UNICODE�ַ����ض�Ӧ�ĵ���λͼ
		virtual uchar* GetCharBitmap(UINT16 wChar);
	};
}

#endif