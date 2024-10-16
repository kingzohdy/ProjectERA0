
#ifndef __OgreFontGlyphMap_H__
#define __OgreFontGlyphMap_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	class IFontGlyphMap
	{
	public:
		virtual ~IFontGlyphMap(){}

		// ȡ������UNICODE�ַ��ĳ��Ϳ�
		virtual void GetCharSize(ushort wChar, float &fWidth, float &fHeight) = 0;

		// ���ݴ����UNICODE�ַ����ض�Ӧ�ĵ���λͼ
		virtual uchar* GetCharBitmap(ushort wChar) = 0;
	};
}

#endif