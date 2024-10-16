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

	// 载入制表符分隔文件
	bool		Load( const char* szFileName, char chSplit = ',', bool bNeedReadPack = true );
	// 清空
	void		Clear();

	// 得到行数
	size_t		GetHeight();
	// 得到列数
	size_t		GetWidth();

	// 得到第N行列数
	size_t		GetWidth(int line);
	// 取得当前值，指针指向下一个
	const char* GetFastString();
	// 根据列号得到某行某列
	const char* GetString( size_t uRow, size_t uColumn );
	// 根据列名得到某行某列
	const char* GetString( size_t uRow, const char* szColumnName );
	// 取得当前值，指针指向下一个
	int			GetFastInteger();
	// 取得当前值，指针指向下一个
	int  		GetInteger( int   defaultvalue = 0 );
	// 根据列号得到某行某列
	int  		GetInteger( size_t uRow, size_t uColumn, int   defaultvalue = 0 );
	// 根据列名得到某行某列
	int  		GetInteger( size_t uRow, const char* szColumnName, int   defaultvalue = 0 );
	// 取得当前值，指针指向下一个
	int   		GetUInteger( int    defaultvalue = 0 );
	// 根据列号得到某行某列
	int   		GetUInteger( size_t uRow, size_t uColumn, int    defaultvalue = 0 );
	// 根据列名得到某行某列
	int   		GetUInteger( size_t uRow, const char* szColumnName, int    defaultvalue = 0 );
	// 取得当前值，指针指向下一个
	float		GetFastFloat();
	// 取得当前值，指针指向下一个
	float		GetFloat( float defaultvalue = 0.0f );
	// 根据列号得到某行某列
	float		GetFloat( size_t uRow, size_t uColumn, float defaultvalue = 0.0f );
	// 根据列名得到某行某列
	float		GetFloat( size_t uRow, const char* szColumnName, float defaultvalue = 0.0f );
	// 换行
	void		SetNextRow();
	// 下一列
	void		SetNextColumn();

	// 重载操作符
	// 缺省是所有整型数字类型的输入，包括各种int long short char longlong unsigned等
	template < class T >
	CTabFile& operator >> (T& value);

	CTabFile& operator >> (float& value);
	CTabFile& operator >> (char* str);
	CTabFile& operator >> (std::string& str);
	CTabFile& operator >> (CTabFile& (*_pfn)(CTabFile&));

private:
	void*		m_hFile;
	size_t		m_uRow;     // 当前读到的行号
	size_t		m_uColumn;  // 当前读到的列号
};

CTabFile& endl( CTabFile& _Ostr );
CTabFile& Next( CTabFile& _Ostr );

//////////////////////////////////////////////////////////////////////////
// 接下来都是上面函数的具体实现

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

// 根据列号得到某行某列
inline int   CTabFile::GetInteger( size_t uRow, size_t uColumn, int   defaultvalue )
{
	const char* szStr = GetString( uRow, uColumn );
	return szStr[0] ? atoi( szStr ) : defaultvalue;
}

// 根据列号得到某行某列
inline int    CTabFile::GetUInteger( size_t uRow, size_t uColumn, int    defaultvalue )
{
	const char* szStr = GetString( uRow, uColumn );
	return szStr[0] ? (int   )_atoi64( szStr ) : defaultvalue;
}

// 根据列名得到某行某列
inline int   CTabFile::GetInteger( size_t uRow, const char* szColumnName, int   defaultvalue )
{
	const char* szStr = GetString( uRow, szColumnName );
	return szStr[0] ? atoi( szStr ) : defaultvalue;
}

// 根据列名得到某行某列
inline int    CTabFile::GetUInteger( size_t uRow, const char* szColumnName, int    defaultvalue )
{
	const char* szStr = GetString( uRow, szColumnName );
	return szStr[0] ? (int   )_atoi64( szStr ) : defaultvalue;
}

// 根据列号得到某行某列
inline float CTabFile::GetFloat( size_t uRow, size_t uColumn, float defaultvalue )
{
	const char* szStr = GetString( uRow, uColumn );
	return szStr[0] ? static_cast<float>( atof( szStr ) ) : defaultvalue;
}

// 根据列名得到某行某列
inline float CTabFile::GetFloat( size_t uRow, const char* szColumnName, float defaultvalue )
{
	const char* szStr = GetString( uRow, szColumnName );
	return szStr[0] ? static_cast<float>( atof( szStr ) ): defaultvalue;
}

// 重载操作符
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

// 输入换行
inline void CTabFile::SetNextRow()
{
	m_uColumn = 0;
	m_uRow++;
}

// 下一列
inline void CTabFile::SetNextColumn()
{
	m_uColumn++;
}

// 输入换行
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


