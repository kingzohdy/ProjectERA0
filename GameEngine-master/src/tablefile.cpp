#include "TableFile.h"
#include "OgreResourceManager.h"
#include "OgreDataStream.h"

using namespace Ogre;

//------����
struct tagTableFileHandle
{
public:
	std::vector<char>					vecBuf;
	std::vector<std::vector<size_t>>	vecvecOffsetByIndex;
	std::map<std::string, size_t>			mapOffsetByName;

private:
	size_t	m_uCurLine;			// ��ǰ��
	bool	m_bEmptyLine;		// �Ƿ�Ϊ����

public:
	void CreateOffsetTable( char chSplit );

private:
	void OnNextLine();						// �������з��ţ���Ҫ������һ��
	void OnString( size_t uOffset );		// ����һ���ַ��������ݽṹ��
	void OnEmptyString( size_t uOffset );	// ����һ�����ַ���
	void OnParseString( size_t uOffset );	// ����һ���ַ���
};

// �������з��ţ���Ҫ������һ��
void tagTableFileHandle::OnNextLine()
{
	// ����ǰ��
	if ( m_bEmptyLine )
	{
		vecvecOffsetByIndex.pop_back();
		m_uCurLine --;
	}
	// ��������
	vecvecOffsetByIndex.push_back( std::vector<size_t>() );
	m_bEmptyLine = true ;
	m_uCurLine++;
}

// ����һ�����ַ���
void tagTableFileHandle::OnEmptyString( size_t uOffset )
{
	vecvecOffsetByIndex[m_uCurLine].push_back(uOffset);
}

// ����һ���ַ��������ݽṹ��
void tagTableFileHandle::OnString( size_t uOffset )
{
	if( m_uCurLine == 0 )
	{
		mapOffsetByName[ &vecBuf[uOffset] ] = vecvecOffsetByIndex[0].size();
	}

	m_bEmptyLine = false ;
	vecvecOffsetByIndex[m_uCurLine].push_back(uOffset);
}

// ����һ���ַ����������Ƿ����������ַ����մ����� false
void tagTableFileHandle::OnParseString( size_t uOffset )
{
	// ȥ��ͷβ�ո�
	while( vecBuf[uOffset] == ' ' )
	{
		// vecBuf[uOffset] == 0;	// ���пո��� 0 ��һ�������ɲ��������˷������
		uOffset++;
	}
	char* pszString = &vecBuf[uOffset];
	size_t uLen = strlen( pszString );
	while ( uLen > 0 && pszString[uLen - 1] == ' ' )
	{
		uLen--;
		// pszString[uLen] = 0;		// ���пո��� 0 ��һ�������ɲ��������˷������
	}
	pszString[uLen] = 0;
	if ( uLen == 0 )
	{
		OnEmptyString( uOffset );
		return ;
	}

	// ��ʼ�����Ľ���
	switch( vecBuf[uOffset] )
	{
	case '/':
		// �����ע��
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
		// �����˫�����ַ���
		{
			assert( uLen >= 2 );
			assert( pszString[uLen-1] == '"' );	// ��β��Ҫ��һ��˫����
			// ȥ������˫����
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

			// ���м����������˫���ţ����һ��˫����
			for( size_t i = 0; i<uLen; ++i )
			{
				if ( pszString[i] == '"' )
				{
					assert( pszString[i+1] == '"' );
					if( pszString[i+1] != '"' )
						continue;

					// �����ÿ���ַ���ǰ�ƶ�һ��λ�ã��ַ������ȼ��� 1
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
		// ��ͨ�ַ���
		OnString( uOffset );
		break;
	}
}

// ����ƫ�Ʊ�ǰ���� vecBuf �Ѿ���ʼ��
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
		case 0:	// ����
			assert( i == vecBuf.size() - 1 );
			OnParseString( uOffset );
			uOffset = i + 1;
			break;
		case '"':	// ����˫�������������ַ���������ֻȷ��˫�����е����ݲ��ᱻ����
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

// �����Ʊ���ָ��ļ�
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

// ���
void CTabFile::Clear()
{
	tagTableFileHandle* pData = GetTableFileData( m_hFile );
	pData->vecBuf.clear();
	pData->vecvecOffsetByIndex.clear();
	pData->mapOffsetByName.clear();
	m_uRow = 0;
	m_uColumn = 0;
}

// �õ�����
size_t CTabFile::GetHeight()
{
	return GetTableFileData( m_hFile )->vecvecOffsetByIndex.size();
}

// �õ�����
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

// �����кŵõ�ĳ��ĳ��
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

// ���������õ�ĳ��ĳ��
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

