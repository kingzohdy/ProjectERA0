#include "GUILoad.h"
#include "tinyxml.h"
#include "md5.h"
#include "jenkins.h"
#include <assert.h>
#include <vector>
#pragma warning(disable:4996)
__int64 file_size(const char* szFileName)
{
	FILE* f = fopen(szFileName, "rb");
	if(!f)
		return -1;

	fseek(f, 0, SEEK_END);
	__int64 len = ftell(f);
	fclose(f);
	return len;
}

bool file_exist(const char* szFileName)
{
	assert( szFileName != NULL );
	FILE *f = fopen( szFileName, "rb" );
	if( f == NULL )
	{
		return false;
	}

	fclose(f);
	return true;
}

const char *file_checksum(const char *szFileName, int offset)
{
	static char checksum[128] = { '\0' };

	FILE *fp = fopen( szFileName ,"rb" );
	if ( fp == NULL )
	{
		return "";
	}

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp) - offset;
	fseek( fp, offset, SEEK_SET );

	CMd5 md5;
	md5.Begin();

	const int BUF_SIZE	= 4096;
	unsigned char buf[BUF_SIZE] = { '\0' };
	while ( file_size > BUF_SIZE )
	{
		fread( buf, BUF_SIZE, 1, fp );
		file_size -= BUF_SIZE;
		md5.Append(buf, BUF_SIZE);
	}

	fread(buf, file_size, 1, fp);
	md5.Append(buf, file_size);
	md5.GetMD5Base16Str( checksum );
	fclose(fp);

	return checksum;
}

unsigned int string_id(const char *source)
{
	const char *str = string_adjust(source);
	return (unsigned int)hash((unsigned char *)(str), (unsigned int)strlen(str), 0 );
}

const char *string_adjust(const char *source)
{
	assert(source != NULL);

	static char buf[1024];
	strncpy_s(buf, source, sizeof(buf));

	for(int i = 0; buf[i]; i++)
	{
		if(buf[i] >= 'A' && buf[i] <= 'Z')
			buf[i] += 'a' - 'A';
		else if(buf[i] == '\\')
			buf[i] = '/';
	}
	return buf;
}

const char *strip_path(const char *full_path)
{
	assert(full_path != NULL);

	static char buf[1024];
	strncpy_s(buf, full_path, sizeof(buf));
	char *tail = buf + strlen(buf);
	while(--tail != buf)
	{
		if((*tail == '/') || (*tail == '\\'))
		{
			*tail = 0;
			return buf;
		}
	}
	return NULL;
}

const char *strip_name(const char *full_path)
{
	assert(full_path != NULL);

	static char buf[1024];
	memset(buf, 0, sizeof(buf));
	strncpy_s(buf, full_path, sizeof(buf));
	char *tail = buf + strlen(buf);
	while(--tail != buf)
	{
		if((*tail == '/') || (*tail == '\\'))
			return tail + 1;
	}
	return NULL;
}

const char *string_rep(char ch, int count)
{
	static char buf[1024];
	assert(count < sizeof(buf));

	memset(buf, 0, sizeof(buf));
	for(int i = 0; i < count; i++)
		buf[i] = ch;

	return buf;
}

void string_replace( char* szContent, char chOld, char chNew )
{
	while ( *szContent != 0 )
	{
		if ( *szContent == chOld )
		{
			*szContent = chNew;
		}
		++szContent;
	}
}


int pack_version(int hi, int mid, int lo)
{
	return ((hi & 0xFF) << 24) + ((mid & 0xFF)  << 16) + (lo & 0xFFFF);
}

void unpack_version(int version, int& hi, int& mid, int& lo)
{
	hi  = version >> 24;
	mid = (version >> 16) & 0xFF;
	lo  = version & 0xFFFF;
}

const char *format_version(int ver_or_hi, int mid, int lo)
{
	static char str_version[128];

	int hi;
	if(mid == -1)
		unpack_version(ver_or_hi, hi, mid, lo);

	sprintf_s(str_version, "%d.%02d.%03d", hi, mid, lo);
	return str_version;
}

int scan_version(const char *str_version)
{
	while((*str_version != '\0') && (!isdigit(*str_version)))
		str_version++;

	int hi, mid, lo;
	if(sscanf_s(str_version, "%d.%d.%d", &hi, &mid, &lo) != 3)
		return -1;

	return pack_version(hi, mid, lo);
}

void string_replace(string & strBig, const string & strsrc, const string &strdst) {
	string::size_type pos=0;
	string::size_type srclen=strsrc.size();
	string::size_type dstlen=strdst.size();
	while( (pos=strBig.find(strsrc, pos)) != string::npos){
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

// ����ע���ֵ
bool reg_set_key(const char *key, const char *subkey, const char *value)
{
	HKEY regkey;
	if ( RegCreateKeyEx( HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
		&regkey, NULL ) != ERROR_SUCCESS )
	{
		return false;
	}

	bool result = RegSetValueEx( regkey, subkey, 0, REG_SZ, ( BYTE* )value, (DWORD)strlen( value ) ) == ERROR_SUCCESS;
	RegCloseKey(regkey);

	return result;
}

// ��ȡע���ֵ
const char *reg_get_key(const char *key, const char *subkey)
{
	HKEY regkey;
	if(RegCreateKeyEx(HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &regkey, NULL) != ERROR_SUCCESS)
		return NULL;

	static char value[1024];
	DWORD len = sizeof(value);
	bool result = RegQueryValueEx(regkey, "Path", NULL, NULL, (BYTE *)value, &len) == ERROR_SUCCESS;
	RegCloseKey(regkey);

	return result ? value : NULL;
}

const char * node_get_text(TiXmlNode *node)
{
	if ( node == NULL )
	{
		return NULL;
	}

	TiXmlNode* child = node->FirstChild();
	return ( child != NULL ) ? child->Value() : "";
}

void ShowError( const char* pszMsg, HWND hWnd )
{
	::MessageBox( hWnd, pszMsg, TEXT( "2012.��ʾ" ), MB_OK | MB_ICONSTOP );
	::TerminateProcess( GetCurrentProcess(), 0 ); // ������ǰ����
}

void GetFilePath( const char* szFileName, char* szPath, int nDestLen )
{
	char szDirectory[128] = { '\0' };
	::GetCurrentDirectory( 128, szDirectory );
	_snprintf_s( szPath, nDestLen,nDestLen, "%s\\%s", szDirectory, szFileName );
}

bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	bool bSuccess = false;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

void DrawFont( HDC hDC, COLORREF crColor, LPCTSTR strCaption, RECT& rect, int nFontWidth )
{
	COLORREF crOld = SetTextColor( hDC, crColor );
	HFONT hNewFont = CreateFont( nFontWidth, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT( "����" ) );
	int nOldMode = SetBkMode( hDC, TRANSPARENT );
	HGDIOBJ hOldFont = SelectObject( hDC, hNewFont );
	DrawText( hDC, "", 0, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS );
	DrawText( hDC, strCaption, lstrlen( strCaption ), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS );

	// �ָ�ԭ��������
	DeleteObject( SelectObject( hDC, hOldFont ) );
	// �ָ�ԭ���ı���ģʽ
	SetBkMode( hDC, nOldMode );
	// �ָ�ԭ������ɫ
	SetTextColor( hDC, crOld );
}

void ValidateFileName(char *buffer, size_t maxbuf, const char *psrc)
{
	int state = 0;//0:������ 1:ǰһ����'\'
	size_t destlen = 0;
	const char *pcur = psrc;
	while(*pcur != 0)
	{
		if(*pcur == '/' || *pcur=='\\')
		{
			if(state == 0)
			{
				buffer[destlen++] = '\\';
				state = 1;
			}
			else
			{
				int a = 0;
			}
		}
		else
		{
			buffer[destlen++] = *pcur;
			state = 0;
		}

		assert(destlen < maxbuf);
		pcur++;
	}
	buffer[destlen] = 0;
}
