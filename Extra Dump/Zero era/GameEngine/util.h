//-----------------------------------------------------------------------------
// File: Util.h
// Desc: Game System Util 2.0
// Auth: wizard
// Date: 2003/11/13
//
// Copyright (c) 1985-2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#ifdef _WIN32		//ֻ��winƽ̨��Ч

#include <Windows.h>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
class CUtil
{
public:

	DWORD	GetFreeID();
	
	// �����
	VOID	Randomize(DWORD dwSeed);
	DWORD	GetRandHolder() { return m_nRandHolder; }
	INT		Rand();
	
	// Win98����ϵͳ��IntersectionRect��BUG, ����������дһ��
	BOOL	RectIntersection(RECT& rcOut, RECT rc1, RECT rc2);


	// �����ֽ���ת������UNICODE��˫�ֽ���
	static WORD* CharStringToUnicode(CHAR* pString);
	// ȥ��std::string��β�Ŀո�
	static VOID	CutSpaceOfStringHeadAndTail(std::string& string);
	// �������ַ�תΪtoken;
	static VOID	StringToToken(std::vector<std::string>& token, LPCSTR szString);

	// ���ע�����ָ����ֵ�Ƿ����
	BOOL CheckReg(HKEY key, LPCTSTR subKey);
	// ��ָ����ֵд��ע�����ָ��·��,���ָ��·��û��,�򴴽���д��
	BOOL WriteReg(char* KEY_ROOT/*ע����е�·��*/, char* KEY_NAME, LPCSTR sz);
	// ��ȡע�����ָ��·���ļ�ֵ
	BOOL ReadReg(char *KEY_ROOT, char* KEY_NAME, DWORD& key_value);
	BOOL ReadReg(char* KEY_ROOT, char* KEY_NAME, char outstr[]);

	CUtil();
	~CUtil();

private:

	static WORD m_wUnicodeString[MAX_PATH];
	CRITICAL_SECTION	m_Lock;

	DWORD	m_dwIDHolder;		// ��¼IDʹ��״̬
	INT		m_nRandHolder;
	INT		m_nRandCallCount;
};

#endif