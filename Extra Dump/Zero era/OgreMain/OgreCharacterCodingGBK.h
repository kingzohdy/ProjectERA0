
#ifndef __OgreCharacterCodingGBK_H__
#define __OgreCharacterCodingGBK_H__

#include "OgreCharacterCoding.h"

namespace Ogre
{
	class CharacterCodingGbk: public ICharacterCoding
	{
	public:
		virtual ~CharacterCodingGbk(){}

		// 跳过字符串头部的空白，反回空白的字节长度
		virtual int JumpOverSpaces(const char *pText);

		// 从字符串头部取出一个字符，反回字符的字节长度
		virtual int GetAChar(const char *pText, unsigned char* szChar);

		// 取得指定字符的字节长度
		virtual int GetCharBytes(const unsigned char* szChar);

		// 从字符串头部取得控制码，反回控制码的字节长度
		virtual int GetControlCode(const char *pText, EControlCode &eCtrlCode, bool bDoTransfer);

		// 是否半角英文，包括标点
		virtual bool IsEnglish(const unsigned char* pChar);

		// 是否半角英文标点符号
		virtual bool IsPunctuation(const unsigned char* pChar);

		// 将字符转换为Unicode
		virtual ushort ToUnicode(const unsigned char* pChar);

		// 将字符转换为在本字符编码方案范围内的唯一ID，用于hash key
		virtual uint ToUniqueID(const unsigned char* pChar);
	};
}

#endif