
#ifndef __OgreCharacterCoding_H__
#define __OgreCharacterCoding_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	enum EControlCode							// 控制码
	{
		CTRLCODE_CHARACTER = 0,					// 普通字符
		CTRLCODE_RETURN,						// 回车
		CTRLCODE_NEWLINE,						// 换行
		CTRLCODE_ENDOFSTRING,					// 字符串结束
		CTRLCODE_TRANSFER,						// 转意控制符
		CTRLCODE_UNKNOWN,						// 未知，不做处理
	};

	class ICharacterCoding
	{
	public:
		virtual ~ICharacterCoding(){}

		// 跳过字符串头部的空白，反回空白的字节长度
		virtual int JumpOverSpaces(const char *pText) = 0;

		// 从字符串头部取出一个字符，反回字符的字节长度
		virtual int GetAChar(const char *pText, unsigned char* pChar) = 0;

		// 取得指定字符的字节长度
		virtual int GetCharBytes(const unsigned char* pChar) = 0;

		// 从字符串头部取得控制码，反回控制码的字节长度
		virtual int GetControlCode(const char *pText, EControlCode &eCtrlCode, bool bDoTransfer) = 0;

		// 是否半角英文，包括标点
		virtual bool IsEnglish(const unsigned char* pChar) = 0;

		// 是否半角英文标点符号
		virtual bool IsPunctuation(const unsigned char* pChar) = 0;

		// 将字符转换为Unicode
		virtual ushort ToUnicode(const unsigned char* pChar) = 0;

		// 将字符转换为在本字符编码方案范围内的唯一ID，用于hash key
		virtual uint ToUniqueID(const unsigned char* pChar) = 0;
	};
}

#endif