
#include "MudNetMgr.h"
#include "OfflineManager.h"
#include "OgreRoot.h"
#include  <Windows.h>
#include "OgreSceneManager.h"
#include "OgreUIRenderer.h"
#include "OgreScriptLuaVM.h"
#include "ScriptFunctions.h"
#include "ScriptTickAction.h"
#include "OgreInputManager.h"
#include "../UILib/ui_gameui.h"
#include "OfflineManager.h"
#include "OgreRenderWindow.h"
#include "GameDBData.h"
#include "FrameTick.h"
#include "FlickerManager.h"
#include "OfflineChatManager.h"
#include "OfflineWatchPlayer.h"
#include "des.h"
#include "RelationManager.h"
#include "ChatRoomManager.h"
#include "OfflineMainPlayer.h"
#include "OgreResourceManager.h"
#include "OgreOSUtility.h"

extern int  tolua_offlineTolua_open (lua_State* tolua_S);
extern int  tolua_GameEngineTolua_open (lua_State* tolua_S);

#define UI_WIDE	1024
#define UI_HEIGHT 768
#define WIDTH 150
#define HEIGHT 150

using namespace Ogre;
using namespace std;

struct HrgnManager
{
	HRGN	m_FullRegionlogin,m_FullRegionchatroom;
};

HrgnManager m_HrgMgr;
void  addmessage(const char* content,int style)
{
	MudNetMgr::getSingleton().addMessagestr(content,style);
}
HRGN BitmapToRegion(HBITMAP hBmp,COLORREF cTransparentColor,COLORREF cTolerance,int xoffset,int yoffset)
{
	HRGN hRgn=NULL;
	if(hBmp)
	{
		//Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC=CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			//Get bitmap size
			HBITMAP hbm32;
			BITMAP bm;
			BITMAPINFOHEADER RGB32BITSBITMAPINFO;
			VOID *pbits32;
			GetObject(hBmp,sizeof(bm),&bm);

			//Create a 32 bits depth bitmap and select it into the memory DC 
			RGB32BITSBITMAPINFO.biSize=sizeof(BITMAPINFOHEADER);
			RGB32BITSBITMAPINFO.biWidth=bm.bmWidth;
			RGB32BITSBITMAPINFO.biHeight=bm.bmHeight;
			RGB32BITSBITMAPINFO.biPlanes=1;
			RGB32BITSBITMAPINFO.biBitCount=32;
			RGB32BITSBITMAPINFO.biCompression=BI_RGB;
			RGB32BITSBITMAPINFO.biSizeImage=0;
			RGB32BITSBITMAPINFO.biXPelsPerMeter=0;
			RGB32BITSBITMAPINFO.biYPelsPerMeter=0;
			RGB32BITSBITMAPINFO.biClrUsed=0;
			RGB32BITSBITMAPINFO.biClrImportant=0;

			hbm32=CreateDIBSection(hMemDC,(BITMAPINFO*)&RGB32BITSBITMAPINFO,DIB_RGB_COLORS,&pbits32,NULL,0);
			if(hbm32)
			{
				HBITMAP holdBmp=(HBITMAP)SelectObject(hMemDC,hbm32);

				//Create a DC just to copy the bitmap into the memory DC
				HDC hDC=CreateCompatibleDC(hMemDC);
				if(hDC)
				{
					//Get how many bytes per row we have for the bitmap bits (rounded up to 32bits)
					HBITMAP holdBmp;
					BITMAP bm32;
					DWORD maxRects;
					HANDLE hData;
					RGNDATA *pData;
					HRGN h;
					BYTE *p32;
					int y,x;

					//Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr=GetRValue(cTransparentColor);
					BYTE lg=GetGValue(cTransparentColor);
					BYTE lb=GetBValue(cTransparentColor);
					BYTE hr=(BYTE)min(0xff,lr+GetRValue(cTolerance));
					BYTE hg=(BYTE)min(0xff,lg+GetGValue(cTolerance));
					BYTE hb=(BYTE)min(0xff,lb+GetBValue(cTolerance));

					GetObject(hbm32,sizeof(bm32),&bm32);
					while(bm32.bmWidthBytes%4)
						bm32.bmWidthBytes++;

					//Copy the bitmap into the memory DC
					holdBmp=(HBITMAP)SelectObject(hDC,hBmp);
					BitBlt(hMemDC,0,0,bm.bmWidth,bm.bmHeight,hDC,0,0,SRCCOPY);

					//For better performances,we will use the ExtCreateRegion() function to create the
					//region. This function takea RGNDATA structure on entry.We will add rectangles by
					//amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT 100
					maxRects=ALLOC_UNIT;
					hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
					pData=(RGNDATA*)GlobalLock(hData);
					pData->rdh.dwSize=sizeof(RGNDATAHEADER);
					pData->rdh.iType=RDH_RECTANGLES;
					pData->rdh.nCount=pData->rdh.nRgnSize=0;
					SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);

					//Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					p32=(BYTE*)bm32.bmBits+(bm32.bmHeight-1)*bm32.bmWidthBytes;
					for(y=0;y<bm.bmHeight;y++)
					{
						//Scan each bitmap pixel from left to right
						for(x=0;x<bm.bmWidth;x++)
						{
							//Search for a continuous range of "non transparent pixels"
							int x0=x;
							LONG *p=(LONG*)p32+x;
							while(x<bm.bmWidth)
							{
								BYTE b=GetRValue(*p);
								if(b>=lr&&b<=hr)
								{
									b=GetGValue(*p);
									if(b>=lg&&b<=hg)
									{
										b=GetBValue(*p);
										if(b>=lb&&b<=hb)
											//This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if(x>x0)
							{
								RECT *pr;
								//Add the pixels (x0,y) to (x,y+1) as a new rectangle in the region
								if(pData->rdh.nCount>=maxRects)
								{
									GlobalUnlock(hData);
									maxRects+=ALLOC_UNIT;
									hData=GlobalReAlloc(hData,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects),GMEM_MOVEABLE);
									pData=(RGNDATA*)GlobalLock(hData);
								}
								pr=(RECT*)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount],x0+xoffset,y+yoffset,x+xoffset,y+1+yoffset);
								if(x0<pData->rdh.rcBound.left) pData->rdh.rcBound.left=x0+xoffset;
								if(y<pData->rdh.rcBound.top) pData->rdh.rcBound.top=y+yoffset;
								if(x>pData->rdh.rcBound.right) pData->rdh.rcBound.right=x+xoffset;
								if(y+1>pData->rdh.rcBound.bottom) pData->rdh.rcBound.bottom=y+1+yoffset;
								pData->rdh.nCount++;

								//On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								//large (ie:>4000). Therefore, we have to create the region by multiple steps.
								if(pData->rdh.nCount==2000)
								{
									HRGN h=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects),pData);
									if(hRgn)
									{
										CombineRgn(hRgn,hRgn,h,RGN_OR);
										DeleteObject(h);
									}
									else hRgn=h;
									pData->rdh.nCount=0;
									SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
								}
							}
						}
						//Go to next row (remember,the bitmap is inverted vertically)
						p32-=bm32.bmWidthBytes;
					}
					//Create or extend the region with the remain in grectangles
					h=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects),pData);
					if(hRgn)
					{
						CombineRgn(hRgn,hRgn,h,RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn=h;

					//Cleanup
					GlobalUnlock(hData);
					GlobalFree(hData);
					SelectObject(hDC,holdBmp);
					DeleteDC(hDC);
				}
				DeleteObject(SelectObject(hMemDC,holdBmp));
			}
			DeleteDC(hMemDC);
		}
	}

	return hRgn;
}

HRGN getRGN(const char * path)
{
	HBITMAP	hSplashBmp;
	hSplashBmp=(HBITMAP) LoadImage( NULL, 
									path, 
									IMAGE_BITMAP, 
									0, 
									0,
									LR_LOADFROMFILE );
	return	BitmapToRegion(hSplashBmp, RGB(0,0,0), 0, 0, 0);	 
}

void PreSceneRenderUI()
{
	OfflineManager::getSingleton().getGameUI()->Render();
}
OfflineManager::OfflineManager()
{
	m_pDBMgr	= NULL;
	m_pInputMgr	= NULL;
	m_UserName[0] = '\0';
}

OfflineManager::~OfflineManager()
{
	delete Root::getSingletonPtr();
}

bool OfflineManager::bitAnd( int nLhs, int nRhs )
{
	return ( nLhs & nRhs ) != 0;
}

void OfflineManager::end1()
{
	ShowWindow((HWND)m_hwnd, 0);
	m_isRender = false;
}

void OfflineManager::end()
{
	SendMessage((HWND)m_hwnd,WM_CLOSE,0,0);
	MudNetMgr::getSingleton().requestLogout();
}

void OfflineManager::setsize(int i)
{
	HWND hwnd = (HWND)m_hwnd;
	ShowWindow( hwnd, 0 );
	static int nlogin_x = -1000, nlogin_y = -1000,nChatRoom_x= -1000, nChatRoom_y = -1000;
	if ( i == 1 )
	{
		HRGN temp=CreateRectRgn( 0, 0, 0, 0 );
		CombineRgn( temp, m_HrgMgr.m_FullRegionchatroom, m_HrgMgr.m_FullRegionchatroom, RGN_COPY);
		SetWindowRgn((HWND)m_hwnd,temp ,true );
		RECT winRect;
		GetWindowRect( (HWND)m_hwnd, &winRect);
		nlogin_x = winRect.left;
		nlogin_y = winRect.top;
		if ( nChatRoom_x <= -1000)
		{
			nChatRoom_x = nlogin_x;
			nChatRoom_y = nlogin_y;
		}
		if (GetSystemMetrics( SM_CXSCREEN ) - nChatRoom_x < CHAT_WIN_WIDTH )
		{
			nChatRoom_x = GetSystemMetrics( SM_CXSCREEN ) - CHAT_WIN_WIDTH;
		}
		MoveWindow(hwnd,nChatRoom_x,nChatRoom_y, CHAT_WIN_WIDTH,CHAT_WIN_HEIGHT,FALSE);
	}
	else
	{		
		HRGN temp=CreateRectRgn(0,0,0,0);
		CombineRgn(temp,m_HrgMgr.m_FullRegionlogin,m_HrgMgr.m_FullRegionlogin,RGN_COPY);
		SetWindowRgn((HWND)m_hwnd,temp ,true );
		RECT winRect;
		GetWindowRect( (HWND)m_hwnd, &winRect);
		nChatRoom_x = winRect.left;
		nChatRoom_y = winRect.top;

		if ( nlogin_x < 0 ) 
		{
			nlogin_x = 0;
		}
		MoveWindow(hwnd,nlogin_x,nlogin_y, LOGIN_WIN_WIDTH,LOGIN_WIN_HEIGHT,FALSE);

		
	}
	unsigned int temp=1024;
	temp=temp+(768<<16);
	SendMessage(hwnd,WM_SIZE,0,temp);
	Sleep(1000);
	ShowWindow(hwnd, 1);
	SetForegroundWindow(hwnd);
}

void OfflineManager::setUIsize()
{
	unsigned int temp=1024;
	temp=temp+(768<<16);
	SendMessage((HWND)m_hwnd,WM_SIZE,0,temp);
}

bool OfflineManager::initDB()
{
	m_pDBMgr = new DBDataManager;
	if( !m_pDBMgr->LoadData( m_Config ) )
	{
		return false;
	}
	return true;
}

bool OfflineManager::init(const char *config_file, void *hwnd,void *hinstance,bool bdirect,void* hMenu,const char* pdirectstart)
{
	m_hwnd		= hwnd;
	m_hinstance	= hinstance;
	Root *proot = new Root( config_file );
	LoadGameConfigFromXML(m_Config,&proot->m_Config);
	LoadServerListConfig("serverlist.data");
	proot->Initlize();
	Root::getSingleton().initRenderSystem( m_hwnd );
	Root::getSingleton().setCompileShaderEnvKeys( false );
	RenderSystem::getSingleton().SetSaveCompiledShaders( false );
	SceneManager::getSingleton().onLostEffect();
	if (!initDB())
	{
		return false;
	}
	UIRenderer* m_pUIRenderer = new UIRenderer();
	m_pmudMgr	= new MudNetMgr((HWND)hwnd);
	m_pGameUI	= new GameUI();
	m_pScriptVM = new ScriptVM();
	m_flickMgr	= new FlickerManager();
	
	
	m_flickMgr->init((HWND)hwnd, (HINSTANCE)hinstance, (HMENU)hMenu);
	m_pmudMgr->initNetMgr();
	m_ChatMgr		= ChatManager::Create();
	m_ActorMgr		= new ActorManager;
	m_WatchPlayer	= new OfflineWatchPlayer();
	m_RelationMgr	= new RelationManager();
	m_ChatRoomMgr	= new ChatRoomManager();
	m_MainPlayer	= new OfflineMainPlayer();
	m_ActorMgr->setScriptVM( m_pScriptVM );
	tolua_offlineTolua_open( m_pScriptVM->getLuaState() );
	tolua_GameEngineTolua_open( m_pScriptVM->getLuaState() );
	ScriptFunctions::getSingleton().registerToLua( m_pScriptVM->getLuaState() );
	scriptTickRegisterToLua( m_pScriptVM->getLuaState() );
	LoadLastUserName();

	m_pScriptVM->setUserTypePointer("OffMgr",			"OfflineManager",		this );
	m_pScriptVM->setUserTypePointer("MudMgr",			"MudNetMgr",			m_pmudMgr );
	m_pScriptVM->setUserTypePointer("ChatManager",		 "ChatManager",			m_ChatMgr);
	m_pScriptVM->setUserTypePointer("ActorMgr",			"ActorManager",			m_ActorMgr);
	m_pScriptVM->setUserTypePointer("OfflineWatchPlayer",			"OfflineWatchPlayer",m_WatchPlayer);
	m_pScriptVM->setUserTypePointer("RelationMgr",		"RelationManager",		m_RelationMgr);
	m_pScriptVM->setUserTypePointer("ChatRoomMgr",		"ChatRoomManager",      m_ChatRoomMgr);
	m_pScriptVM->setUserTypePointer("MainPlayer",		"OfflineMainPlayer",    m_MainPlayer);
	
	m_pmudMgr->SetConfig(m_Config);
	m_pUIRenderer->setPreRenderCallback(PreSceneRenderUI);
	m_pUIRenderer->setRenderTarget(RenderSystem::getSingleton().getMainWindow());
	SceneManager::getSingleton().addSceneRenderer(0, m_pUIRenderer);
	if(!m_pGameUI->Create( "uires\\YinLiKongJian\\guajidefault.toc", UI_WIDE, UI_HEIGHT, m_pUIRenderer, m_pScriptVM ))
	{
		return false;
	}
	DBDataManager::getSingleton().loadSuitSet( Equip::m_mapSuitSet );
	m_pInputMgr			= new InputManager;
	m_pInputMgr->RegisterInputHandler(m_pGameUI);
	setUIsize();
	m_MainPlayer->setGameState( LOGIN_STATE );
	if (bdirect)
	{
		if ( !m_pmudMgr->requestLoginServerDirect(pdirectstart) )
		{
			m_pmudMgr->ReFreshServer();
		}
	}
	else
	{
		m_pmudMgr->ReFreshServer();
	}
	m_pFrameTick=new FrameTick;
	m_pFrameTick->setFrameLimit(0);
	initRgn();
	HRGN temp=CreateRectRgn(0,0,0,0);
	CombineRgn(temp,m_HrgMgr.m_FullRegionlogin,m_HrgMgr.m_FullRegionlogin,RGN_COPY);
	SetWindowRgn((HWND)m_hwnd,temp ,true );
	m_pFrameTick->setFrameLimit(15);
	m_isRender = true;
	return true;
}

void OfflineManager::LoadLastUserName()
{
	XMLData xmldata;
	if( xmldata.loadFile( "UserInfoLoginRecord.xml" ) )
	{
		XMLNode pRootNode = xmldata.getRootNode();
		XMLNode pChildNode = pRootNode.iterateChild();
		while( !pChildNode.isNull() )
		{
			if( stricmp(pChildNode.getName(),"UserName") == 0 )
			{
				if( pChildNode.hasAttrib("saveuser") )
				{
					m_nSaveUserName = pChildNode.attribToInt( "saveuser" );
				}
				if( pChildNode.hasAttrib("username") )
				{
					strcpy( m_UserName, pChildNode.attribToString("username") );
				}				
			}else if( stricmp(pChildNode.getName(),"selectWorld") == 0 )
			{
				m_nLastSelectWorldId = pChildNode.attribToInt( "worldId" );
			}
			pChildNode = pRootNode.iterateChild(pChildNode);
		}
	}
}

int	OfflineManager::OnWindowMsg( void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam )
{
	if (msgid==WM_SHOWTASK)
	{
		m_flickMgr->OnWindowMsg( hwnd,msgid, wparam, lparam );
	}

	if ( msgid == WM_NCLBUTTONDOWN)
	{
		HWND t_hwnd=GetCapture();
		SetCapture(t_hwnd);
	}
	if ( msgid == WM_NCLBUTTONUP)
	{
		ReleaseCapture();
	}

	if (msgid==WM_LBUTTONDOWN||WM_LBUTTONUP)
	{
		m_lparam=lparam;
	}
	if ( msgid == WM_SIZE )//修正UI的比列
	{
		unsigned int temp = 1024;
		temp	= temp + ( 768 << 16 );
		lparam	= temp;
	}
	RenderWindow *prwin;
	if (m_pInputMgr)
	{
		m_pInputMgr->OnWindowMessage( hwnd, msgid, wparam, lparam);
	}
	switch (msgid)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_SIZE:
		prwin = RenderSystem::getSingleton().findRenderWindow( hwnd );
		if( prwin != NULL )
		{
			prwin->onSizeOrMove();
		}
		break;
	case WM_COMMAND:
		{
			switch (LOWORD(wparam))
			{
			case ID_SHOW:
				{
					m_isRender =true;
					SetForegroundWindow((HWND)m_hwnd);
					ShowWindow((HWND)m_hwnd,1);
					UpdateWindow( (HWND)m_hwnd );
				}
				break;
			case  ID_EXIT:
				{
					end();
				}
				break;
			}
		}
	}
	return DefWindowProc((HWND)hwnd, msgid, wparam, lparam);
}

void OfflineManager::SendUIEvent(const char *pevent)
{
	m_pGameUI->SendEvent( pevent );
}

void OfflineManager::update()
{
	unsigned int dtick = m_pFrameTick->DoFrame();
	float		 dtime = TickToTime(dtick);
	m_pmudMgr->Dispatch( dtick );
	m_pInputMgr->Dispatch();
	m_pGameUI->Update( dtime );
	if ( m_isRender )
	{
		SceneManager::getSingleton().doFrame();
	}
	m_ChatRoomMgr->update(dtick);
	SendUIEvent("GE_TIPS_UPDATE" );
	if ( m_pmudMgr->getMessageNums() > 0)
	{	
		SendUIEvent( "GE_MESSAGE_UPDATE");
	}
	ResourceManager::getSingleton().update( dtick );
}

void OfflineManager::LoadServerListConfig(const char* fileName)
{
	//TODO HERE
	m_Config.dir_num = 0;
	const char* pszDecryptBuffer	= getDecryptFileBuffer( fileName, CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		MessageBox(NULL, "读取serverlist.cfg文件错误", "错误", MB_OK);
		return;
	}

	// 这边全部改为用buffer来读取
	Ogre::XMLData xmldata;
	if ( xmldata.loadBuffer( pszDecryptBuffer, strlen( pszDecryptBuffer ) ) )
	{
		XMLNode root = xmldata.getRootNode();
		XMLNode pnode = root.iterateChild();
		m_Config.dir_num = 0;
		while( !pnode.isNull() && m_Config.dir_num < MAX_DIR_SERV_NUM  )
		{
			if( strcmp( pnode.getName(),"DirServer" ) == 0 )
			{
				NetConfigData& data = m_Config.dir_iplist[m_Config.dir_num];
				strncpy( data.dir_server,pnode.attribToString("address"),128 );
				data.dir_port = pnode.attribToInt("port");
				data.num = 0;
				XMLNode dirNode;
				dirNode = pnode.iterateChild(dirNode);
				while( !dirNode.isNull() && data.num < MAX_DIR_SERV_NUM )
				{
					if( strcmp(dirNode.getName(),"iplist")==0 )
					{
						strncpy(data.dir_data[data.num].dir_ip,dirNode.attribToString("ip"),128);
						data.dir_data[data.num].dir_port = dirNode.attribToInt("port");
						data.num++;
					}
					dirNode = pnode.iterateChild(dirNode);
				}
				m_Config.dir_num++;
			}
			if( strcmp( pnode.getName(),"DirUrl" ) == 0 )
			{
				strncpy( m_Config.dir_url,pnode.attribToString("address"),255);
			}
			if (Root::getSingleton().IsSmartClient())
			{
				pnode = root.getChild("SmartClient");
				if (!pnode.isNull())
				{
					std::string ser = pnode.attribToString("server");
					Root::getSingleton().SetSmartClientServer(ser);
				}
			}
			pnode = root.iterateChild(pnode);
		}		
	}
	delete[] pszDecryptBuffer;
}

void OfflineManager::MoveWindowDown()
{
		PostMessage( (HWND)m_hwnd, WM_NCLBUTTONDOWN, HTCAPTION, m_lparam);
}

void OfflineManager::MoveWindowUp()
{
	PostMessage( (HWND)m_hwnd, WM_NCLBUTTONUP, HTCAPTION, m_lparam);
}


void OfflineManager::MiniWindow()
{
	ShowWindow((HWND)m_hwnd, SW_MINIMIZE);
}

void OfflineManager::initRgn()
{
	m_HrgMgr.m_FullRegionlogin		=	getRGN(	"resource\\KNGJIANMASK.bmp"	);
	m_HrgMgr.m_FullRegionchatroom	=	getRGN(	"resource\\KNGJIANMASK2.bmp" );
}

void OfflineManager::Destroy()
{
	delete m_flickMgr;
	delete m_pFrameTick;
	delete m_pmudMgr;
	delete m_pScriptVM;
	delete m_ChatMgr;
	delete m_WatchPlayer;
	delete m_RelationMgr;
	delete m_ChatRoomMgr;
	delete m_MainPlayer;
	delete UIRenderer::getSingletonPtr();
	delete Root::getSingletonPtr();

	delete m_pDBMgr;
}


void OfflineManager::saveusername(const char* name)
{
	strcpy(m_UserName,name);	
	m_nSaveUserName = 0;
}

void OfflineManager::clearusername()
{
	m_UserName[0] = '\0';
	m_nSaveUserName = 0;
}

int OfflineManager::getlastusercheck()
{
	return m_nSaveUserName;
}


const char* OfflineManager::getlastname()
{
	return m_UserName;
}

void OfflineManager::OpenUrl( const char* url )
{
	if ( ( url != NULL ) && ( strstr( url, ":/" ) ) )
	{
		::ShellExecute( NULL, "open", url, NULL, NULL, SW_SHOWNORMAL );
	}
}

int OfflineManager::getWindowtopleftx()
{
	RECT pRECT;
	GetWindowRect((HWND)m_hwnd, &pRECT);
	return pRECT.left;
}

int OfflineManager::getWindowtoplefty()
{
	RECT pRECT;
	GetWindowRect((HWND)m_hwnd, &pRECT);
	return pRECT.top;
}

void OfflineManager::CallString(const char* pszStr)
{
	m_pScriptVM->callString(pszStr);
}

void OfflineManager::addtips(std::string strTips)
{
	MudNetMgr::getSingleton().m_tiplist.push_back( strTips );
}

int OfflineManager::getGameState()
{
	return OfflineMainPlayer::getSingleton().getGameState();
}