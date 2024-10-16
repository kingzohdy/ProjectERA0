/*
*	与操作系统无关的读写Ini文件的实现
*/

#ifndef _ARK_VFS_INIFILE_H_
#define _ARK_VFS_INIFILE_H_

//---------------------------------------------------------------------------
class CIniFile
{
private:
	void*	m_hFileHandle;
	bool    Prepare(char* lpBuffer); 

public:
	CIniFile();
	~CIniFile();

	// 创建一个新的 .Ini文件，为写(保存)做准备
	bool	Create(const char* szFileName);    

	// 打开文件，扫描一遍，为以后的读操作做好准备(Get...)
	bool	Open(const char* szFileName);  

	// 将链表中的内容保存到磁盘文件
	void	Save();    

	// 关闭文件,释放内存
	void	Close();                

	// 得到某个Section下的Key的值
	bool	GetKeyValue(const char*	lpSection, const char*	lpKeyName, char*	lpRString, uint32 nSize); 

	// 得到某个Section下的字符串Key的值
	void	GetString(const char*	lpSection,	const char*	lpKeyName,	const char*	lpDefault,	char*	lpRString,	uint32	nSize);

	// 得到某个Section下的整数Key的值
	void	GetInteger(const char*	lpSection,	const char*	lpKeyName,	int	nDefault,	int& Value);

	// 得到某个Section下的整数Key的值
	int		GetInteger(const char*	lpSection,	const char*	lpKeyName,	int	nDefault);

	// 写入某个Section下的Key的值，字符串
	void 	WriteString(const char* lpSection,const char* lpKeyName,const char* lpString);

	// 写入某个Section下的Key的值，字符串
	void	WriteInteger(const char* lpSection,	const char* lpKeyName, int Value);

	char   GetChar(const char*	lpSection,const char*	lpKeyName, char chDefault);

};

#endif

