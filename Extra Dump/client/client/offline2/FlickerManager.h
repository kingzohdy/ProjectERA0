#ifndef __FlickerMgr_H__
#define __FlickerMgr_H__

#include <Windows.h>
#include "resource.h"
#include "OgreSingleton.h"

#define WM_SHOWTASK WM_USER + 2
#define HICON_MAX		5
#define HICON_LOGIN		0
#define HICON_ONLINE	1
#define HICON_HUODONG	2
#define HICON_GONGGAO	3
#define HICON_SILIAO	4

class FlickerManager
	:public Ogre::Singleton<FlickerManager>
{
public:
	FlickerManager(){}
	~FlickerManager();
	void init(HWND hwnd,HINSTANCE hinstance,HMENU hmenu);
	void changeico(int i);
	int OnWindowMsg( void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam );
	void setZerohwnd(HWND hwnd);
private:
	HINSTANCE			m_Hinstance;
	PNOTIFYICONDATA		m_Main;
	HICON				m_HIcon[HICON_MAX];
	HMENU				m_hMenu;
	HWND				m_hwndZero;
};

#endif