//-----------------------------------------------------------------------------
// File: Util.h
// Desc: Game System Util 2.0
// Auth: wizard
// Date: 2003/11/13
//
// Copyright (c) 1985-2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#ifdef _WIN32		//只对win平台有效

#include <Windows.h>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
// 工具类
//-----------------------------------------------------------------------------
class CUtil
{
public:

	DWORD	GetFreeID();
	
	// 随机数
	VOID	Randomize(DWORD dwSeed);
	DWORD	GetRandHolder() { return m_nRandHolder; }
	INT		Rand();
	
	// Win98操作系统下IntersectionRect有BUG, 这里我们重写一个
	BOOL	RectIntersection(RECT& rcOut, RECT rc1, RECT rc2);


	// 将单字节码转成类似UNICODE的双字节码
	static WORD* CharStringToUnicode(CHAR* pString);
	// 去掉std::string首尾的空格
	static VOID	CutSpaceOfStringHeadAndTail(std::string& string);
	// 将长串字符转为token;
	static VOID	StringToToken(std::vector<std::string>& token, LPCSTR szString);

	// 检测注册表中指定键值是否存在
	BOOL CheckReg(HKEY key, LPCTSTR subKey);
	// 将指定键值写入注册表中指定路径,如果指定路径没有,则创建并写入
	BOOL WriteReg(char* KEY_ROOT/*注册表中的路径*/, char* KEY_NAME, LPCSTR sz);
	// 读取注册表中指定路径的键值
	BOOL ReadReg(char *KEY_ROOT, char* KEY_NAME, DWORD& key_value);
	BOOL ReadReg(char* KEY_ROOT, char* KEY_NAME, char outstr[]);

	CUtil();
	~CUtil();

private:

	static WORD m_wUnicodeString[MAX_PATH];
	CRITICAL_SECTION	m_Lock;

	DWORD	m_dwIDHolder;		// 记录ID使用状态
	INT		m_nRandHolder;
	INT		m_nRandCallCount;
};

#endif