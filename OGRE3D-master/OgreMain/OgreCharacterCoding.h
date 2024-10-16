
#ifndef __OgreCharacterCoding_H__
#define __OgreCharacterCoding_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	enum EControlCode							// ������
	{
		CTRLCODE_CHARACTER = 0,					// ��ͨ�ַ�
		CTRLCODE_RETURN,						// �س�
		CTRLCODE_NEWLINE,						// ����
		CTRLCODE_ENDOFSTRING,					// �ַ�������
		CTRLCODE_TRANSFER,						// ת����Ʒ�
		CTRLCODE_UNKNOWN,						// δ֪����������
	};

	class ICharacterCoding
	{
	public:
		virtual ~ICharacterCoding(){}

		// �����ַ���ͷ���Ŀհף����ؿհ׵��ֽڳ���
		virtual int JumpOverSpaces(const char *pText) = 0;

		// ���ַ���ͷ��ȡ��һ���ַ��������ַ����ֽڳ���
		virtual int GetAChar(const char *pText, unsigned char* pChar) = 0;

		// ȡ��ָ���ַ����ֽڳ���
		virtual int GetCharBytes(const unsigned char* pChar) = 0;

		// ���ַ���ͷ��ȡ�ÿ����룬���ؿ�������ֽڳ���
		virtual int GetControlCode(const char *pText, EControlCode &eCtrlCode, bool bDoTransfer) = 0;

		// �Ƿ���Ӣ�ģ��������
		virtual bool IsEnglish(const unsigned char* pChar) = 0;

		// �Ƿ���Ӣ�ı�����
		virtual bool IsPunctuation(const unsigned char* pChar) = 0;

		// ���ַ�ת��ΪUnicode
		virtual ushort ToUnicode(const unsigned char* pChar) = 0;

		// ���ַ�ת��Ϊ�ڱ��ַ����뷽����Χ�ڵ�ΨһID������hash key
		virtual uint ToUniqueID(const unsigned char* pChar) = 0;
	};
}

#endif