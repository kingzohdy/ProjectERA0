#pragma once
#include "SelfExtractor.h"
#include "Md5.h"

class CCreatePatch
{
public:
	CCreatePatch(void);
	~CCreatePatch(void);

public:
	// ����������
	bool makePatch( string strVersionPath, string strOldVersionPath, string strPackageName );
	// �����Խ�ѹ��
	bool makeSelfExtractor( const char* szExtractorPath, const char* szPatchName );
	// �����汾��Ϣ
	const char* createVer( const char* szVer, const char* szPath );

	// ����ص�
	static void AddCallBack(void* CallbackData, void* Data);
private:
	// �����ļ�Ŀ¼
	bool searchChangeFiles( string sNewDirName, string sOldDirName, map<string,string>& mapChangeFiles );
	// �Ƚ��ļ�����
	size_t diff( FILE* pOldFile, FILE* pNewFile, string& strDiff );

	const char *file_checksum(const char *szFileName, int offset);


public:
	CSelfExtractor			m_SelfExtractor;

private:
	map<string,string>		m_mapChangeFiles;

};
