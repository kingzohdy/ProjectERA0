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

// 根据文件名取得文件大小
__int64 file_size(const char* szFileName);
// 判断文件是否存在
bool file_exist(const char* szFileName);
// 获取文件的md5校验
const char *file_checksum(const char *szFileName, int offset = 0);
// 哈希转换字符串为id
unsigned int string_id(const char *source);
// 为了适应将字符串转化为唯一的数字ID，调整字符串
// 如果字符大写，就转化为小写，将'\\'转化为'/'
const char *string_adjust(const char *source);
// 去掉全路径中的文件名 c:/path/test.xml -> c:/path/
const char *strip_path(const char *full_path);
// 取得全路径中的文件名 c:/path/test.xml -> test.xml
const char *strip_name(const char *full_path);
// 返回连续字符N个的字符串
const char *string_rep(char ch, int count);
// 替换指定字符串中的特定字符
void string_replace( char* szContent, char chOld, char chNew );

// 版本号格式：主版本(8bit) 中间版本(8bit) 子版本(16bit)
// （主、中间、子）版本号整合成一个版本号
int pack_version(int hi, int mid, int lo);
// 版本号解开成（主、中间、子）版本号
void unpack_version(int version, int& hi, int& mid, int& lo);
// 格式化版本号成一个（"1.00.001"）字符串，mid为-1时，ver_or_hi当作整合后的版本号，否则作为主版本号
const char *format_version(int ver_or_hi, int mid = -1, int lo = -1);
// 扫描字符串返回版本号
int scan_version(const char *str_version);
// 设置注册表值
bool reg_set_key(const char *key, const char *subkey, const char *value);
// 获取注册表值
const char *reg_get_key(const char *key, const char *subkey);

void string_replace(string & strBig, const string & strsrc, const string &strdst);

const char* node_get_text(TiXmlNode *node);

// 显示错误信息，然后退出程序
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
	sprintf_s( msg, "无法打开界面资源图片: %s", image );	\
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
	::MessageBoxW( NULL, L"解析" #name "失败", "错误", MB_OK | MB_ICONERROR);\
	return false;\
	}\
	}\
	else\
	config.##name = def_##name;

