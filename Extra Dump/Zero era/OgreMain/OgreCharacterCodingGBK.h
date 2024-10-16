
#ifndef __OgreCharacterCodingGBK_H__
#define __OgreCharacterCodingGBK_H__

#include "OgreCharacterCoding.h"

namespace Ogre
{
	class CharacterCodingGbk: public ICharacterCoding
	{
	public:
		virtual ~CharacterCodingGbk(){}

		// �����ַ���ͷ���Ŀհף����ؿհ׵��ֽڳ���
		virtual int JumpOverSpaces(const char *pText);

		// ���ַ���ͷ��ȡ��һ���ַ��������ַ����ֽڳ���
		virtual int GetAChar(const char *pText, unsigned char* szChar);

		// ȡ��ָ���ַ����ֽڳ���
		virtual int GetCharBytes(const unsigned char* szChar);

		// ���ַ���ͷ��ȡ�ÿ����룬���ؿ�������ֽڳ���
		virtual int GetControlCode(const char *pText, EControlCode &eCtrlCode, bool bDoTransfer);

		// �Ƿ���Ӣ�ģ��������
		virtual bool IsEnglish(const unsigned char* pChar);

		// �Ƿ���Ӣ�ı�����
		virtual bool IsPunctuation(const unsigned char* pChar);

		// ���ַ�ת��ΪUnicode
		virtual ushort ToUnicode(const unsigned char* pChar);

		// ���ַ�ת��Ϊ�ڱ��ַ����뷽����Χ�ڵ�ΨһID������hash key
		virtual uint ToUniqueID(const unsigned char* pChar);
	};
}

#endif