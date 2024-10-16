// OffLine.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "OgreXMLData.h"

class COffLineDlg;
class MudNetMgr;
class Ogre::Root;
// COffLineApp:
// 有关此类的实现，请参阅 OffLine.cpp
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

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);


public:
	//网络消息
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