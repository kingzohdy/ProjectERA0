/*
*	�����ϵͳ�޹صĶ�дIni�ļ���ʵ��
*/

#ifndef _ARK_VFS_INIFILE_H_
#define _ARK_VFS_INIFILE_H_

#include "OgrePlatform.h"
using Ogre::uint32;
//---------------------------------------------------------------------------
class CIniFile
{
private:
	void*	m_hFileHandle;

public:
	CIniFile();
	~CIniFile();
	
	bool    Prepare(char* lpBuffer); 

	// ����һ���µ� .Ini�ļ���Ϊд(����)��׼��
	bool	Create(const char* szFileName);    

	// ���ļ���ɨ��һ�飬Ϊ�Ժ�Ķ���������׼��(Get...)
	/*bool	Open( FILE* pFileHandle );  */
	bool	Open(const char* szFileName);  

	// �������е����ݱ��浽�����ļ�
	void	Save();    

	// �ر��ļ�,�ͷ��ڴ�
	void	Close();                

	// �õ�ĳ��Section�µ�Key��ֵ
	bool	GetKeyValue(const char*	lpSection, const char*	lpKeyName, char*	lpRString, uint32 nSize); 

	// �õ�ĳ��Section�µ��ַ���Key��ֵ
	void	GetString(const char*	lpSection,	const char*	lpKeyName,	const char*	lpDefault,	char*	lpRString,	uint32	nSize);

	// �õ�ĳ��Section�µ�����Key��ֵ
	void	GetInteger(const char*	lpSection,	const char*	lpKeyName,	int	nDefault,	int& Value);

	// �õ�ĳ��Section�µ�����Key��ֵ
	int		GetInteger(const char*	lpSection,	const char*	lpKeyName,	int	nDefault);

	// д��ĳ��Section�µ�Key��ֵ���ַ���
	void 	WriteString(const char* lpSection,const char* lpKeyName,const char* lpString);

	// д��ĳ��Section�µ�Key��ֵ���ַ���
	void	WriteInteger(const char* lpSection,	const char* lpKeyName, int Value);

	char   GetChar(const char*	lpSection,const char*	lpKeyName, char chDefault);

	// ɾ��һ�� Section�µ�Key��ֵ
	void RemoveSection( const char* lpSection, const char* lpKeyName );
};

#endif

