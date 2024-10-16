
#ifndef __String_H__
#define __String_H__

#if 0
#include "OgrePrerequisites.h"

namespace Ogre
{
	class _OgreExport String
	{
	public:
		enum{ INVALID_POS = 0xffffffff };

	public:
		static int UnicodeToAnsi( char *pdestbuf, int maxchar, const wchar_t *psrc, int nsrcchar=-1 );
		static int AnsiToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar=-1 );
		static int UnicodeToUTF8( char *pdestbuf, int maxchar, const wchar_t *psrc, int nsrcchar=-1 );
		static int UTF8ToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar=-1 );
		static bool IsDBCSLeadByte(char c);

		static const char    *UnicodeToAnsi( const wchar_t *psrc, int nsrcchar=-1 );
		static const wchar_t *AnsiToUnicode( const char *psrc, int nsrcchar=-1 );
		static const char    *UnicodeToUTF8( const wchar_t *psrc, int nsrcchar=-1 );
		static const wchar_t *UTF8ToUnicode( const char *psrc, int nsrcchar=-1 );

	public:
		String();
		~String();

		String( const char *pstr );
		String( const String &rhs );

		String &operator =( const char *pstr );
		String &operator =( const String &rhs );
		String &operator +=( const char *pstr );
		String &operator +=( const String &rhs );

		char   operator[]( size_t i )
		{
			assert( i <= m_nChar );
			return m_pBuffer[i];
		}
		operator const char *() const
		{
			return m_pBuffer;
		}
		bool operator==( const String &rhs ) const;
		bool operator!=( const String &rhs ) const;
		bool operator<( const String &rhs ) const;
		bool operator>( const String &rhs ) const;

		size_t GetLength() const{ return m_nChar; }
		bool   IsEmpty() const{ return m_nChar==0; }

		/**
		* �õ�[begin, end)������Ӵ�
		*/
		String SubString( size_t begin=0, size_t end=INVALID_POS ) const;

		/**
		* �õ�[begin, end)������Ӵ�, �������Լ�����
		*/
		void SubStringToSelf( size_t begin=0, size_t end=INVALID_POS );

		/**
		* �ҳ���һ���ַ����Ӵ�[begin, end)�е�λ�ã���λ���������������ʼλ��
		* \return ���û���ҵ�������INVALID_POS�����򷵻��ڴ��е�λ��
		*/
		size_t FindChar( char c, size_t begin=0, size_t end=INVALID_POS ) const;
		size_t RFindChar( char c, size_t begin=0, size_t end=INVALID_POS ) const;
		size_t FindStr( const String &str ) const;

		/**
		* ����Ҳ����ָ��ַ�c, ����false, first, second����ı�
		*/
		bool Split( String &first, String &second, char c ) const;

		/**
		* ��չΪnchar���ַ��Ĵ�������[nchar] = 0
		* param keepdata ����λ�ã�����ԭ��������
		*/
		void Resize( size_t nchar, bool keepdata=true );

		void Set( const char *pbuf, int nchar )
		{
			Insert(0, pbuf, nchar);
		}

		void Append(const char *pbuf, int nchar)
		{
			Insert(m_nChar, pbuf, nchar);
		}

		void TrimWhitespace();

		//���ز�����ַ�����
		size_t Insert( size_t pos, const char *pstr, int len=-1 );
		void Erase( size_t pos, size_t nchar );

		void FormatInput( const char *pfmtstr, ... );

	private:
		char  *m_pBuffer;
		size_t m_BufLen;
		size_t m_nChar;

		void InitBufferWhenConstruct();
	};

	inline String operator +( const String &str1, const String &str2 )
	{
		String result(str1);
		result += str2;
		return result;
	}

	inline String operator +( const String &str1, const char *pstr )
	{
		String result(str1);
		result += pstr;
		return result;
	}

	inline String operator +( const char *pstr, const String &str1 )
	{
		return String(pstr) + str1;
	}

}

#endif

#endif