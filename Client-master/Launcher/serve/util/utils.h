#pragma once

#include <stdio.h>
#include <string>
#include <atlstr.h>
#include "Md5.h"
using namespace std;


//#ifdef __cplusplus
//	extern "C"
//	{
//#endif

class TiXmlNode; 

template<typename T>
void SafeDelete( T*& p )
{
	if ( p != NULL )
	{
		delete p;
		p = NULL;
	}
}

// �����ļ���ȡ���ļ���С
__int64 file_size(const char* szFileName);
// �ж��ļ��Ƿ����
bool file_exist(const char* szFileName);
// ��ȡ�ļ���md5У��
const char *file_checksum(const char *szFileName, int offset = 0);
// ��ϣת���ַ���Ϊid
unsigned int string_id(const char *source);
// Ϊ����Ӧ���ַ���ת��ΪΨһ������ID�������ַ���
// ����ַ���д����ת��ΪСд����'\\'ת��Ϊ'/'
const char *string_adjust(const char *source);
// ȥ��ȫ·���е��ļ��� c:/path/test.xml -> c:/path/
const char *strip_path(const char *full_path);
// ȡ��ȫ·���е��ļ��� c:/path/test.xml -> test.xml
const char *strip_name(const char *full_path);
// ���������ַ�N�����ַ���
const char *string_rep(char ch, int count);
// �滻ָ���ַ����е��ض��ַ�
void string_replace( char* szContent, char chOld, char chNew );

// �汾�Ÿ�ʽ�����汾(8bit) �м�汾(8bit) �Ӱ汾(16bit)
// �������м䡢�ӣ��汾�����ϳ�һ���汾��
int pack_version(int hi, int mid, int lo);
// �汾�Ž⿪�ɣ������м䡢�ӣ��汾��
void unpack_version(int version, int& hi, int& mid, int& lo);
// ��ʽ���汾�ų�һ����"1.00.001"���ַ�����midΪ-1ʱ��ver_or_hi�������Ϻ�İ汾�ţ�������Ϊ���汾��
const char *format_version(int ver_or_hi, int mid = -1, int lo = -1);
// ɨ���ַ������ذ汾��
int scan_version(const char *str_version);
// ����ע���ֵ
bool reg_set_key(const char *key, const char *subkey, const char *value);
// ��ȡע���ֵ
const char *reg_get_key(const char *key, const char *subkey);

void string_replace(string & strBig, const string & strsrc, const string &strdst);

const char* node_get_text(TiXmlNode *node);

// ��ʾ������Ϣ��Ȼ���˳�����
void ShowError( const WCHAR* pszMsg, HWND hWnd = 0 );

void GetFilePath( const char* szFileName, char* szPath, int nDestLen );

bool CreateMultipleDirectory(const CString& szPath);

void DrawFont( HDC hDC, COLORREF crColor, LPCTSTR strCaption, RECT& rect, int nFontWidth, bool bCenter = false );

int AnsiToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar=-1 );
void ValidateFileName(char *buffer, size_t maxbuf, const char *psrc);
//#ifdef __cplusplus
//	}
//#endif

#define LOAD_IMAGE( control, image )					\
	if ( !control.Create( hdc, image ) )				\
	{													\
	char msg[256] = { '\0' };							\
	sprintf_s( msg, "�޷��򿪽�����ԴͼƬ: %s", image );	\
	}

#define SURFACE_LOADPATH( config, name, path ) { \
	node = root->FirstChild( #name );\
	if(node_get_text(node))\
	sprintf_s(config.##name, "%s/%s", path, node_get_text(node));\
	else\
	sprintf_s(config.##name, "%s/%s", path, def_##name); \
	}

#define SKINXML_LOADPOINT( config, name, node )\
	node = root->FirstChild(#name);\
	if(node_get_text(node))\
	{\
	if(sscanf_s(node_get_text(node), "%d,%d", &config.##name.x, &config.##name.y) != 2)\
	{\
	::MessageBoxW( NULL, L"����" #name "ʧ��", "����", MB_OK | MB_ICONERROR);\
	return false;\
	}\
	}\
	else\
	config.##name = def_##name;

