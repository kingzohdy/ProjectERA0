#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

class  CTabFile
{
public:
	CTabFile(void);
	~CTabFile(void);

	// �����Ʊ���ָ��ļ�
	bool		Load( const char* szFileName, char chSplit = ',', bool bNeedReadPack = true );
	// ���
	void		Clear();

	// �õ�����
	size_t		GetHeight();
	// �õ�����
	size_t		GetWidth();

	// �õ���N������
	size_t		GetWidth(int line);
	// ȡ�õ�ǰֵ��ָ��ָ����һ��
	const char* GetFastString();
	// �����кŵõ�ĳ��ĳ��
	const char* GetString( size_t uRow, size_t uColumn );
	// ���������õ�ĳ��ĳ��
	const char* GetString( size_t uRow, const char* szColumnName );
	// ȡ�õ�ǰֵ��ָ��ָ����һ��
	int			GetFastInteger();
	// ȡ�õ�ǰֵ��ָ��ָ����һ��
	int  		GetInteger( int   defaultvalue = 0 );
	// �����кŵõ�ĳ��ĳ��
	int  		GetInteger( size_t uRow, size_t uColumn, int   defaultvalue = 0 );
	// ���������õ�ĳ��ĳ��
	int  		GetInteger( size_t uRow, const char* szColumnName, int   defaultvalue = 0 );
	// ȡ�õ�ǰֵ��ָ��ָ����һ��
	int   		GetUInteger( int    defaultvalue = 0 );
	// �����кŵõ�ĳ��ĳ��
	int   		GetUInteger( size_t uRow, size_t uColumn, int    defaultvalue = 0 );
	// ���������õ�ĳ��ĳ��
	int   		GetUInteger( size_t uRow, const char* szColumnName, int    defaultvalue = 0 );
	// ȡ�õ�ǰֵ��ָ��ָ����һ��
	float		GetFastFloat();
	// ȡ�õ�ǰֵ��ָ��ָ����һ��
	float		GetFloat( float defaultvalue = 0.0f );
	// �����кŵõ�ĳ��ĳ��
	float		GetFloat( size_t uRow, size_t uColumn, float defaultvalue = 0.0f );
	// ���������õ�ĳ��ĳ��
	float		GetFloat( size_t uRow, const char* szColumnName, float defaultvalue = 0.0f );
	// ����
	void		SetNextRow();
	// ��һ��
	void		SetNextColumn();

	// ���ز�����
	// ȱʡ�����������������͵����룬��������int long short char longlong unsigned��
	template < class T >
	CTabFile& operator >> (T& value);

	CTabFile& operator >> (float& value);
	CTabFile& operator >> (char* str);
	CTabFile& operator >> (std::string& str);
	CTabFile& operator >> (CTabFile& (*_pfn)(CTabFile&));

private:
	void*		m_hFile;
	size_t		m_uRow;     // ��ǰ�������к�
	size_t		m_uColumn;  // ��ǰ�������к�
};

CTabFile& endl( CTabFile& _Ostr );
CTabFile& Next( CTabFile& _Ostr );

//////////////////////////////////////////////////////////////////////////
// �������������溯���ľ���ʵ��

inline int   CTabFile::GetFastInteger()
{
	const char* szStr = GetFastString();
	return atoi( szStr );
}

inline float CTabFile::GetFastFloat()
{
	const char* szStr = GetFastString();
	return static_cast<float>(atof( szStr ));
}

inline int CTabFile::GetInteger( int defaultvalue )
{
	return GetInteger( m_uRow, m_uColumn, defaultvalue );
}

inline int CTabFile::GetUInteger( int    defaultvalue )
{
	return GetUInteger( m_uRow, m_uColumn, defaultvalue );
}

inline float CTabFile::GetFloat( float defaultvalue )
{
	return GetFloat( m_uRow, m_uColumn, defaultvalue );
}

// �����кŵõ�ĳ��ĳ��
inline int   CTabFile::GetInteger( size_t uRow, size_t uColumn, int   defaultvalue )
{
	const char* szStr = GetString( uRow, uColumn );
	return szStr[0] ? atoi( szStr ) : defaultvalue;
}

// �����кŵõ�ĳ��ĳ��
inline int    CTabFile::GetUInteger( size_t uRow, size_t uColumn, int    defaultvalue )
{
	const char* szStr = GetString( uRow, uColumn );
	return szStr[0] ? (int   )_atoi64( szStr ) : defaultvalue;
}

// ���������õ�ĳ��ĳ��
inline int   CTabFile::GetInteger( size_t uRow, const char* szColumnName, int   defaultvalue )
{
	const char* szStr = GetString( uRow, szColumnName );
	return szStr[0] ? atoi( szStr ) : defaultvalue;
}

// ���������õ�ĳ��ĳ��
inline int    CTabFile::GetUInteger( size_t uRow, const char* szColumnName, int    defaultvalue )
{
	const char* szStr = GetString( uRow, szColumnName );
	return szStr[0] ? (int   )_atoi64( szStr ) : defaultvalue;
}

// �����кŵõ�ĳ��ĳ��
inline float CTabFile::GetFloat( size_t uRow, size_t uColumn, float defaultvalue )
{
	const char* szStr = GetString( uRow, uColumn );
	return szStr[0] ? static_cast<float>( atof( szStr ) ) : defaultvalue;
}

// ���������õ�ĳ��ĳ��
inline float CTabFile::GetFloat( size_t uRow, const char* szColumnName, float defaultvalue )
{
	const char* szStr = GetString( uRow, szColumnName );
	return szStr[0] ? static_cast<float>( atof( szStr ) ): defaultvalue;
}

// ���ز�����
template< class T >
CTabFile& CTabFile::operator >> (T& value)
{
	value = (T)GetFastInteger();
	return *this;
}

inline CTabFile& CTabFile::operator >> (float& value)
{
	value = GetFastFloat();
	return *this;
}

inline CTabFile& CTabFile::operator >> (char* str)
{
	const char* buf = GetFastString();
	size_t len = strlen(buf);
	memcpy(str,buf,len+1);
	return *this;
}

inline CTabFile& CTabFile::operator >> (std::string& str)
{
	str = GetFastString();
	return *this;
}

// ���뻻��
inline void CTabFile::SetNextRow()
{
	m_uColumn = 0;
	m_uRow++;
}

// ��һ��
inline void CTabFile::SetNextColumn()
{
	m_uColumn++;
}

// ���뻻��
inline CTabFile& CTabFile::operator >> ( CTabFile& (*_pfn)(CTabFile&) )
{
	return ((*_pfn)(*this));
}

inline CTabFile& endl( CTabFile& _Ostr )
{
	_Ostr.SetNextRow();
	return (_Ostr);
}

inline CTabFile& Next( CTabFile& _Ostr )
{
	_Ostr.SetNextColumn();
	return (_Ostr);
}


