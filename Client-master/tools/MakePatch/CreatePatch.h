#pragma once
#include "SelfExtractor.h"
#include "Md5.h"

class CCreatePatch
{
public:
	CCreatePatch(void);
	~CCreatePatch(void);

public:
	// 创建补丁包
	bool makePatch( string strVersionPath, string strOldVersionPath, string strPackageName );
	// 创建自解压包
	bool makeSelfExtractor( const char* szExtractorPath, const char* szPatchName );
	// 构建版本信息
	const char* createVer( const char* szVer, const char* szPath );

	// 打包回调
	static void AddCallBack(void* CallbackData, void* Data);
private:
	// 搜索文件目录
	bool searchChangeFiles( string sNewDirName, string sOldDirName, map<string,string>& mapChangeFiles );
	// 比较文件差异
	size_t diff( FILE* pOldFile, FILE* pNewFile, string& strDiff );

	const char *file_checksum(const char *szFileName, int offset);


public:
	CSelfExtractor			m_SelfExtractor;

private:
	map<string,string>		m_mapChangeFiles;

};
