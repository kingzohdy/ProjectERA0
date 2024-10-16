#include "TableFile.h"
#include "OgreResourceManager.h"
#include "OgreDataStream.h"

using namespace Ogre;

//------定义
struct tagTableFileHandle
{
public:
	std::vector<char>					vecBuf;
	std::vector<std::vector<size_t>>	vecvecOffsetByIndex;
	std::map<std::string, size_t>			mapOffsetByName;

private:
	size_t	m_uCurLine;			// 当前行
	bool	m_bEmptyLine;		// 是否为空行

public:
	void CreateOffsetTable( char chSplit );

private:
	void OnNextLine();						// 遇到换行符号，需要处理下一行
	void OnString( size_t uOffset );		// 放入一个字符串到数据结构中
	void OnEmptyString( size_t uOffset );	// 放入一个空字符串
	void OnParseString( size_t uOffset );	// 处理一个字符串
};

// 遇到换行符号，需要处理下一行
void tagTableFileHandle::OnNextLine()
{
	// 处理当前行
	if ( m_bEmptyLine )
	{
		vecvecOffsetByIndex.pop_back();
		m_uCurLine --;
	}
	// 创建新行
	vecvecOffsetByIndex.push_back( std::vector<size_t>() );
	m_bEmptyLine = true ;
	m_uCurLine++;
}

// 放入一个空字符串
void tagTableFileHandle::OnEmptyString( size_t uOffset )
{
	vecvecOffsetByIndex[m_uCurLine].push_back(uOffset);
}

// 放入一个字符串到数据结构中
void tagTableFileHandle::OnString( size_t uOffset )
{
	if( m_uCurLine == 0 )
	{
		mapOffsetByName[ &vecBuf[uOffset] ] = vecvecOffsetByIndex[0].size();
	}

	m_bEmptyLine = false ;
	vecvecOffsetByIndex[m_uCurLine].push_back(uOffset);
}

// 处理一个字符串，返回是否有真正的字符，空串返回 false
void tagTableFileHandle::OnParseString( size_t uOffset )
{
	// 去掉头尾空格
	while( vecBuf[uOffset] == ' ' )
	{
		// vecBuf[uOffset] == 0;	// 所有空格变成 0 这一步可做可不做，做了方便调试
		uOffset++;
	}
	char* pszString = &vecBuf[uOffset];
	size_t uLen = strlen( pszString );
	while ( uLen > 0 && pszString[uLen - 1] == ' ' )
	{
		uLen--;
		// pszString[uLen] = 0;		// 所有空格变成 0 这一步可做可不做，做了方便调试
	}
	pszString[uLen] = 0;
	if ( uLen == 0 )
	{
		OnEmptyString( uOffset );
		return ;
	}

	// 开始真正的解析
	switch( vecBuf[uOffset] )
	{
	case '/':
		// 如果是注释
		if ( vecBuf[uOffset + 1] == '/' )
		{
			vecBuf[uOffset] = 0;
			OnEmptyString( uOffset );
			return ;
		}
		else
		{
			OnString( uOffset );
		}
		break;
	case '"':
		// 如果是双引号字符串
		{
			assert( uLen >= 2 );
			assert( pszString[uLen-1] == '"' );	// 结尾需要另一个双引号
			// 去掉两个双引号
			pszString[0] = 0;
			uOffset++;
			pszString++;
			uLen--;
			uLen--;
			pszString[uLen] = 0;
			if ( uLen <= 0 )
			{
				OnEmptyString( uOffset );
				return ;
			}

			// 把中间的连续两个双引号，变成一个双引号
			for( size_t i = 0; i<uLen; ++i )
			{
				if ( pszString[i] == '"' )
				{
					assert( pszString[i+1] == '"' );
					if( pszString[i+1] != '"' )
						continue;

					// 后面的每个字符往前移动一个位置，字符串长度减少 1
					uLen--;
					for ( size_t j = i+1; j<uLen; ++j )
					{
						pszString[j] = pszString[j+1];
					}
					pszString[uLen] = 0;
				}
			}
			OnParseString( uOffset );
		}
		break;
	default:
		// 普通字符串
		OnString( uOffset );
		break;
	}
}

// 生成偏移表，前提是 vecBuf 已经初始化
void tagTableFileHandle::CreateOffsetTable( char chSplit )
{
	size_t uOffset = 0;
	vecvecOffsetByIndex.push_back( std::vector<size_t>() );
	m_uCurLine = 0;
	m_bEmptyLine = true ;

	for( size_t i = 0; i < vecBuf.size(); i++ )
	{
		switch( vecBuf[i] )
		{
		case '\n':
		case '\r':
			do {
				vecBuf[i] = 0;
				++i;
			}
			while ( vecBuf[i] == '\r' || vecBuf[i] == '\n' );
			--i;
			OnParseString( uOffset );
			uOffset = i + 1;
			OnNextLine();
			break;
		case 0:	// 结束
			assert( i == vecBuf.size() - 1 );
			OnParseString( uOffset );
			uOffset = i + 1;
			break;
		case '"':	// 处理双引号括起来的字符串，这里只确保双引号中的内容不会被处理
			for(;;)
			{
				++i;
				if ( vecBuf[i] == 0 )
				{
					--i;
					break;
				}
				if ( vecBuf[i] == '"' )
					break;
			}
			break;
		default:
			if( chSplit == vecBuf[i] )
			{
				vecBuf[i] = 0;
				OnParseString( uOffset );
				uOffset = i + 1;
			}
		}
	}
	if ( m_bEmptyLine )
		vecvecOffsetByIndex.pop_back();
}

//========================================== CTabFile ==============================================================================
tagTableFileHandle* GetTableFileData( void* hFile )
{
	return reinterpret_cast<tagTableFileHandle*>( hFile );
}

CTabFile::CTabFile(void) : m_hFile( new tagTableFileHandle ), m_uRow(0), m_uColumn(0)
{
}

CTabFile::~CTabFile(void)
{
	Clear();
	delete GetTableFileData(m_hFile);
}

// 载入制表符分隔文件
bool CTabFile::Load( const char* szFileName, char chSplit, bool bNeedReadPack )
{
	Ogre::DataStream *pFile = Ogre::ResourceManager::getSingleton().openFileStream( szFileName, true);
	if (pFile == NULL)
	{
		return false;
	}

	assert( pFile->size() > 0 );
	size_t nLength = pFile->size();

	Clear();
	tagTableFileHandle* pData = GetTableFileData( m_hFile );
	pData->vecBuf.resize( nLength + 1 );
	pFile->read( &pData->vecBuf[0], nLength );
	pData->vecBuf[ nLength ] = 0;

	pData->CreateOffsetTable( chSplit );

	delete pFile;
	return true;
}

// 清空
void CTabFile::Clear()
{
	tagTableFileHandle* pData = GetTableFileData( m_hFile );
	pData->vecBuf.clear();
	pData->vecvecOffsetByIndex.clear();
	pData->mapOffsetByName.clear();
	m_uRow = 0;
	m_uColumn = 0;
}

// 得到行数
size_t CTabFile::GetHeight()
{
	return GetTableFileData( m_hFile )->vecvecOffsetByIndex.size();
}

// 得到列数
size_t CTabFile::GetWidth()
{
	return GetTableFileData( m_hFile )->vecvecOffsetByIndex[0].size();
}
size_t CTabFile::GetWidth(int line)
{
	if ( (int)GetTableFileData( m_hFile )->vecvecOffsetByIndex.size() >= line)
	{
		return  GetTableFileData( m_hFile )->vecvecOffsetByIndex[line].size();
	}else
	{
		return 0;
	}
}
const char* CTabFile::GetFastString()
{
	tagTableFileHandle* pData = GetTableFileData( m_hFile );
	assert( m_uRow < pData->vecvecOffsetByIndex.size() && m_uColumn < pData->vecvecOffsetByIndex[m_uRow].size() );

	size_t uOffset = pData->vecvecOffsetByIndex[m_uRow][m_uColumn];
	m_uColumn++;
	return &pData->vecBuf[uOffset];
}

// 根据列号得到某行某列
const char* CTabFile::GetString( size_t uRow, size_t uColumn )
{
	tagTableFileHandle* pData = GetTableFileData( m_hFile );
	if( uRow < pData->vecvecOffsetByIndex.size() && uColumn < pData->vecvecOffsetByIndex[uRow].size() )
	{
		m_uRow		= uRow;
		m_uColumn	= uColumn+1;
		size_t uOffset = pData->vecvecOffsetByIndex[uRow][uColumn];
		return &pData->vecBuf[uOffset];
	}
	return "";
}

// 根据列名得到某行某列
const char* CTabFile::GetString( size_t uRow, const char* szColumnName )
{
	tagTableFileHandle* pFileHandle = GetTableFileData( m_hFile );
	if (pFileHandle == NULL) {
		return "";
	}

	if (pFileHandle->mapOffsetByName.find(szColumnName) == pFileHandle->mapOffsetByName.end()) {
		return "";
	}

	return GetString( uRow, pFileHandle->mapOffsetByName[szColumnName] );
}

