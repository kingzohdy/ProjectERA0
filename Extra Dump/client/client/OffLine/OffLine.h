// OffLine.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "OgreXMLData.h"

class COffLineDlg;
class MudNetMgr;
class Ogre::Root;
// COffLineApp:
// �йش����ʵ�֣������ OffLine.cpp
//

class LocalPlayerInfo
{
public:
	std::string playerName;
	std::string lastServer;
	std::string lastworld;
};

class COffLineApp : public CWinApp
{
public:
	COffLineApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);


public:
	//������Ϣ
	MudNetMgr*	m_NetMgr;
	Ogre::Root*	m_pRoot;
	DBDataManager*	m_pDBMgr;

	Ogre::XMLData m_PlayerList;
	Ogre::XMLNode m_PlayerListRootNode;

	std::string m_LastUserName;
	std::string m_LastServer;
	std::string m_LastWorld;
	std::vector<LocalPlayerInfo> m_LocalPlayInfoList;

	COffLineDlg* m_OffLineDlg;

protected:
	void LoadLocalPlayerInfo (std::string fileName);
	void SaveLocalPlayerInfo (std::string fileName);

public:

	void	addRichString(CRichEditCtrl* pCtrl,CString text,COLORREF color);
	virtual int ExitInstance();
};

extern COffLineApp theApp;