#include "FlickerManager.h"
#include "OfflineManager.h"

FlickerManager::~FlickerManager()
{
	Shell_NotifyIcon(NIM_DELETE,m_Main);
}

void FlickerManager::changeico(int i)
{
	m_Main->hIcon = m_HIcon[i];
	Shell_NotifyIcon(NIM_MODIFY,m_Main);
}

void FlickerManager::init(HWND hwnd,HINSTANCE hinstance,HMENU hmenu)
{
	m_hMenu = hmenu;
	m_hMenu = GetSubMenu(m_hMenu,0);
	m_Hinstance=hinstance;
	m_HIcon[HICON_LOGIN]				= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ZHUTUBIAO));
	m_HIcon[HICON_ONLINE]				= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ONLINE));
	m_HIcon[HICON_HUODONG]				= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_HUODONG));
	m_HIcon[HICON_GONGGAO]				= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_GONGGAO));
	m_HIcon[HICON_SILIAO]				= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_SILIAO));
	
	m_Main						= new NOTIFYICONDATAA;
	m_Main->cbSize				= (DWORD)sizeof(NOTIFYICONDATA);
	m_Main->hWnd				= hwnd;
	m_Main->uID					= IDI_ZHUTUBIAO;
	m_Main->hIcon				= m_HIcon[HICON_LOGIN];
	m_Main->uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP  ; 
	m_Main->uCallbackMessage	= WM_SHOWTASK;//自定义的消息名称 
	strcpy(m_Main->szTip,"Gravitation Space"); //信息提示条为“计划任务提醒”
	Shell_NotifyIcon(NIM_ADD,m_Main);
}

int FlickerManager::OnWindowMsg( void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam )
{
	if ( msgid == WM_SHOWTASK)
	{
		if ( lparam== WM_RBUTTONDOWN )
		{
			POINT point;
			GetCursorPos(&point);
			SetForegroundWindow(m_hwndZero); // 修正当用户按下ESCAPE 键或者在菜单之外单击鼠标时菜单不会消失的情况
			TrackPopupMenu(m_hMenu,TPM_RIGHTBUTTON, point.x, point.y, 0, m_hwndZero, NULL);			
		}
		else if (lparam==WM_LBUTTONDBLCLK)
		{
			OfflineManager::getSingleton().m_isRender= true;
			SetForegroundWindow((HWND)hwnd);
			ShowWindow((HWND)hwnd, 1);
			UpdateWindow( (HWND)hwnd );
		}
	}
	return 1;
}

void FlickerManager::setZerohwnd(HWND hwnd)
{
	m_hwndZero = hwnd;
}