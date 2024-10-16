#include "ui_common.h"
#include "ui_frame.h"
#include "ui_framemgr.h"
#include "ui_fontstring.h"
#include "ui_button.h"
#include "ui_texture.h"
#include <functional>
#include <algorithm>
#include <string>
#include "OgreSoundSystem.h"

#include "OgreResourceManager.h"
#include "OgreDataStream.h"

//using namespace Ogre;
using namespace std;
//-----------------------------------------------------------------------------
// Script Function
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

static std::vector<std::string> g_UnCloseWindow;

void ClearFocus()
{
	g_pFrameMgr->ClearFocus();
}

void ShowUIPanel(const char *frameName)
{
	if (frameName)
	{
		Frame* pFrame = static_cast<Frame *>(g_pFrameMgr->FindLayoutFrame(frameName));

		if (pFrame)
		{
			if (!strcmp(pFrame->GetTypeName(), "Texture") || !strcmp(pFrame->GetTypeName(), "FontString"))
			{
				pFrame->Show();
			}
			else
			{
				pFrame->ShowUIPanel();
			}
			ClearFocus();
		}
	}
	else
	{
		assert(0);
	}
}

void HideUIPanel(const char *frameName)
{
	if (frameName)
	{
		Frame* pFrame = static_cast<Frame *>(g_pFrameMgr->FindLayoutFrame(frameName));

		if (pFrame)
		{
			if (!strcmp(pFrame->GetTypeName(), "Texture") || !strcmp(pFrame->GetTypeName(), "FontString"))
			{
				pFrame->Hide();
			}
			else
			{
				pFrame->HideUIPanel();
			}
			ClearFocus();
		}
	}
	else
	{
		assert(0);
	}
}

void UIBeginDrag(const char *path, int id1, int id2, int iNum, bool bCompose, int hotspotx, int hotspoty)
{
	g_pFrameMgr->m_pCurCursor->BeginDrag(path, id1, id2, iNum, bCompose, hotspotx, hotspoty);
}

void UIEndDrag()
{
	g_pFrameMgr->m_pCurCursor->EndDrag();
}

bool UIIsInDragState()
{
	return g_pFrameMgr->m_pCurCursor->IsInDragState();
}

int UIGetDragUserData1()
{
	return g_pFrameMgr->m_pCurCursor->GetDragUserData1();
}

int UIGetDragUserData2()
{
	return g_pFrameMgr->m_pCurCursor->GetDragUserData2();
}

int UIGetDragUserData3()
{
	return g_pFrameMgr->m_pCurCursor->GetDragUserData3();
}

Button* CreateButton( const char* name, const char* template_name, Frame* pParent, bool isRoot )
{
	UIObject* pObj = g_pFrameMgr->CreateObject( "Button", name, template_name );
	g_pFrameMgr->RegisterObject( pObj );
	pObj->release();
	Button* pButton = static_cast<Button*>( pObj );
	pButton->ReplaceSpecialName();
	if ( pParent != NULL )
	{
		pParent->AddChildFrame( pButton );
		pButton->m_FrameStrata = pParent->m_FrameStrata;
	}
	g_pFrameMgr->CreateLuaTable( pButton );
	if ( isRoot )
	{
		g_pFrameMgr->AddRootFrame( pButton );
	}

	for ( std::vector<Frame::DrawObj>::iterator iter = pButton->m_DrawRegions.begin(), 
			end = pButton->m_DrawRegions.end(); iter != end; ++iter )
	{
		LayoutFrame* pDrawRegion = iter->pregion;
		g_pFrameMgr->RegisterObject( pDrawRegion );
		
		iter->pregion->m_pParent = pButton;
	}
	
	for ( std::vector<Frame *>::iterator iter = pButton->m_Children.begin(), 
		end = pButton->m_Children.end(); iter != end; ++iter )
	{
		LayoutFrame* pChildFrame = *iter;
		g_pFrameMgr->RegisterObject( pChildFrame );

		(*iter)->m_pParent = pButton;
	}

	return pButton;
}

FontString* FindFontString( const char* name )
{
	return static_cast<FontString*>( g_pFrameMgr->FindLayoutFrame( name ) );
}

Button* FindButton( const char* name )
{
	Button* pFindButton = static_cast<Button*>( g_pFrameMgr->FindLayoutFrame( name ) );
	
	return pFindButton;
}

Texture* FindTexture( const char* name )
{
	return static_cast<Texture*>( g_pFrameMgr->FindLayoutFrame( name ) );
}

bool  isAlreadyExistFrame( const char* name )
{
	return g_pFrameMgr->FindLayoutFrame( name ) != NULL;
}

// 关闭所有的对话泡泡
void closePopWin()
{
	std::map<std::string, tagPopWin>::iterator iter = g_pFrameMgr->m_PopWinMap.begin();
	for( ;iter != g_pFrameMgr->m_PopWinMap.end(); ++iter )
	{
		g_pFrameMgr->hidePopWin( iter->first.c_str() );
	}
	g_pFrameMgr->m_PopWinMap.clear();
}

void SetCurrentCursor(const char *cursorstate)
{
	if (cursorstate)
	{
		g_pFrameMgr->setCursor(cursorstate);
	}
}

void ChangeCursorState(const char *cursorstate)
{
	if (cursorstate)
	{
		g_pFrameMgr->setCursor(cursorstate);
	}
}

const char *GetCurrentCursor()
{
	return g_pFrameMgr->m_pCurCursor->getCursor();
}

int GetCurrentCursorLevel()
{
	size_t index = g_pFrameMgr->m_pCurCursor->GetCursorIndex();
	return s_CursorLevel[index] ;
}

int GetScreenWidth()
{
	return g_pFrameMgr->GetScreenWidth(); 
}

int GetScreenHeight()
{
	return g_pFrameMgr->GetScreenHeight();
}

void SetFocusFrame(const char* frameName)
{
	if (frameName)
	{
		Frame* pFrame = static_cast<Frame *>(g_pFrameMgr->FindLayoutFrame(frameName));
		if (pFrame)
		{
			SetFocusFrame(pFrame);
		}
	}
}

extern void SetFocusFrame(Frame *frame)
{
	g_pFrameMgr->SetFocusFrame(frame);
}

// 获取当前点中控件的父窗口
const char *GetFocusFrameParent()
{
	if( g_pFrameMgr->GetFocusFrame() != NULL )
	{
		return g_pFrameMgr->GetFocusFrame()->GetParent();
	}
	return "";
}

int GetCursorPosX()
{
	return FloatToInt(g_pFrameMgr->m_pCurCursor->m_Pos.x/g_pFrameMgr->GetScreenScaleY());
}

int GetCursorPosY()
{
	return FloatToInt(g_pFrameMgr->m_pCurCursor->m_Pos.y/g_pFrameMgr->GetScreenScaleY());
}

void pushUnCloseWin(const char* frameName)
{
	g_UnCloseWindow.push_back(frameName);
}

const char* GetCurrentDialogFrame()
{
	Frame* pFrame = NULL;
	for (size_t i = 0; i <  g_pFrameMgr->m_RootFrames.size(); ++i)
	{
		if ( g_pFrameMgr->m_RootFrames[i]->m_FrameStrata != FS_DIALOG && g_pFrameMgr->m_RootFrames[i]->m_FrameStrata !=FS_FULLSCREEN_DIALOG && g_pFrameMgr->m_RootFrames[i]->m_FrameStrata != FS_FULLSCREEN  )
		{
			continue;	
		}

		if (g_pFrameMgr->m_RootFrames[i]->m_Name == "UIClient")
		{
			continue;
		}

		if (g_pFrameMgr->m_RootFrames[i]->m_bInputTransparent)
		{
			continue;
		}

		if ( g_pFrameMgr->m_RootFrames[i]->IsShown() )
		{
			bool bFind = false;
			for( int j = 0; j < (int)g_UnCloseWindow.size(); j++ )
			{
				if( g_pFrameMgr->m_RootFrames[i]->m_Name == g_UnCloseWindow[j] )
				{
					bFind = true;
				}
			}
			if ( !bFind )
			{
				if( pFrame == NULL 	|| pFrame->m_FrameStrata < g_pFrameMgr->m_RootFrames[i]->m_FrameStrata 
					|| (pFrame->m_FrameStrata == g_pFrameMgr->m_RootFrames[i]->m_FrameStrata 
						&& pFrame->m_iDrawLevel < g_pFrameMgr->m_RootFrames[i]->m_iDrawLevel ))
				{
					pFrame = g_pFrameMgr->m_RootFrames[i];
				}
			}			
		}
	}
	if( pFrame != NULL )
	{
		return pFrame->m_Name.c_str();
	}else{
		return NULL;
	}
}

bool isPointInFrame( const char* pszFrameName )
{
	if (pszFrameName)
	{
		LayoutFrame* pFrame = static_cast<LayoutFrame *>(g_pFrameMgr->FindLayoutFrame(pszFrameName));
		if ( pFrame != NULL && g_pFrameMgr->m_pCurCursor != NULL )
		{
			return pFrame->PointInLayoutFrame( g_pFrameMgr->m_pCurCursor->m_Pos.x, g_pFrameMgr->m_pCurCursor->m_Pos.y );
		}
	}
	return false;
}

bool isPointInFrame( int x, int	y, const char* pszFrameName )
{
	if ( pszFrameName != NULL )
	{
		LayoutFrame* pFrame = static_cast<LayoutFrame*>( g_pFrameMgr->FindLayoutFrame( pszFrameName ) );

		if ( pFrame != NULL )
		{
			return	pFrame->PointInLayoutFrame( x, y );
		}
	}

	return false;
}

// 要求pLeft初始位置在pRight的左边
bool isTwoFrameXConflict( LayoutFrame* pLeft, LayoutFrame* pRight )
{
	if ( pLeft == NULL || pRight == NULL )
	{
		return false;
	}
	
	return pLeft->m_AbsRect.m_Right > pRight->m_AbsRect.m_Left;
}

void CopyToMemory( const char* pszText )
{
	size_t	nTextLength = strlen( pszText );
	if( nTextLength == 0 ) 
	{
		return;
	}

	//得到共享内存
	HGLOBAL hGlobal=GlobalAlloc( GHND | GMEM_SHARE, nTextLength + 1 );
	if( !hGlobal ) 
	{
		return;
	}

	//填充共享内存
	LPVOID pGlobal=GlobalLock( hGlobal );
	strncpy( (char*)pGlobal, pszText,nTextLength + 1 );
	GlobalUnlock(hGlobal);

	//设置剪贴板
	if( !::OpenClipboard( NULL ) ) 
	{
		return;
	}

	::EmptyClipboard();
	::SetClipboardData( CF_TEXT,hGlobal );
	::CloseClipboard();
}

bool CompareName( Frame* pFrame, const char* pszName )
{
	return std::string( pFrame->GetName() ) == pszName;
}

// TODO: 以后要删除
bool	isFiliation( const char* pszParentFrame, const char* pszChildFrame )
{
	Frame*	pParentFrame	=  static_cast<Frame*>( g_pFrameMgr->FindLayoutFrame( pszParentFrame ) );

	// 先搜寻第一层子节点
	std::vector<Frame*>::iterator pos = std::find_if( pParentFrame->m_Children.begin(), pParentFrame->m_Children.end(), 
														std::bind2nd( std::ptr_fun( CompareName ), pszChildFrame ) ) ;
	if ( pos != pParentFrame->m_Children.end() )
	{
		return true;
	}
	else // 再递归查找子节点
	{
		for ( std::vector<Frame*>::iterator iter = pParentFrame->m_Children.begin(), end = pParentFrame->m_Children.end();
				iter != end; ++iter )
		{
			// 一旦确定他们是父子关系则直接返回,否则继续向下查找,直到找完所有的子节点
			if ( isFiliation( ( *iter )->GetName(), pszChildFrame ) )
			{
				return true;
			}
		}
	}

	return false;
}


void addChangedFrames( const char* pszFrameName )
{
	LayoutFrame* pFrame = static_cast<LayoutFrame*>( g_pFrameMgr->FindLayoutFrame( pszFrameName ) );
	if ( std::find( g_pFrameMgr->m_ReCalPosFrames.begin(), g_pFrameMgr->m_ReCalPosFrames.end(), pFrame ) == 
			g_pFrameMgr->m_ReCalPosFrames.end() )
	{
		g_pFrameMgr->AddReCalFrame( pFrame );
	}
}

void addChangedFrames( LayoutFrame* pFrame )
{
	if ( std::find( g_pFrameMgr->m_ReCalPosFrames.begin(), g_pFrameMgr->m_ReCalPosFrames.end(), pFrame ) == 
			g_pFrameMgr->m_ReCalPosFrames.end() )
	{
		g_pFrameMgr->AddReCalFrame( pFrame );
	}
}

void playUISound( const char* pszSoundPath )
{
	Ogre::SoundSystem::getSingleton().playSound2D( pszSoundPath, 0.5 );
}

float	GetScreenScaleY()
{
	return g_pFrameMgr->GetScreenScaleY();
}

float	GetScreenScaleX()
{
	return g_pFrameMgr->GetScreenScaleX();
}

float	GetFontTextWidth( FontString* pFont )
{
	float	fWidth	= 0.0f;
	float	fHeight	= 0.0f;
	g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[pFont->m_iFontIndex].h, pFont->m_Caption.c_str(), fWidth, fHeight );
	return fWidth;
}

bool IsInExistence( const char* szFileName )
{
	return ( Ogre::ResourceManager::getSingleton().getFileAttrib( szFileName ) != 0);
}
bool isInVisibleArea( const LayoutFrame* pIncludedFrame, const LayoutFrame* pClipFrame )
{

	const Ogre::RectInt& srcRect	= pIncludedFrame->m_AbsRect;
	const Ogre::RectInt& clipRect	= pClipFrame->m_AbsRect;
	if ( srcRect.m_Bottom <= clipRect.m_Top || srcRect.m_Top >= clipRect.m_Bottom ||
			srcRect.m_Left >= clipRect.m_Right || srcRect.m_Right <= clipRect.m_Left )
	{
		return false; 
	}

	return true;
}

bool isKeyPressed(int key)
{
	return (GetKeyState(key) & 0xff00) == 0 ? false:true;
}

int getRand( int nMinNum, int nMaxNum )
{
	return ( rand()%( nMaxNum - nMinNum + 1 ) + nMinNum );
}

const char* GetFocusFrameName()
{
	 if( g_pFrameMgr->GetFocusFrame() != NULL )
	 {
		 return g_pFrameMgr->GetFocusFrame()->GetName();
	 }
	 return "";
}

bool needReplaceSubstrFace( const string& strSubReplacedText )
{
	static string  s_strModelChCollect = "ARGBKYWLbncP";
	if ( strSubReplacedText.length() == 1 )
	{
		return true;
	}

	if ( s_strModelChCollect.find_first_of( strSubReplacedText[1] ) != string::npos )
	{
		return false;
	}
	
	if ( strSubReplacedText.length() < 4 )
	{
		return true;
	}
	
	/*if ( strSubReplacedText[1] == '9' && strSubReplacedText[2] == '9' && strSubReplacedText[3] == '9' )
	{
		return false;
	}*/

	for ( int i = 1; i < 4; ++i )
	{
		char ch = strSubReplacedText[i];
		if ( !( ch >= '0' && ch <= '9' ) )
		{
			return true;
		}
	}

	int n = 0;
	for( int i = 1; i < 4; ++i )
	{
		n = n * 10 + ( strSubReplacedText[i] - '0' );
	}

	FaceData* pFaceData = g_pFrameMgr->m_pCodeMap->GetFaceData(n);
	return pFaceData == NULL;
}

const char* replaceFaceString( const char* pszText )
{
	static string replacedStr;
	replacedStr = pszText;

	string::size_type pos = replacedStr.find( '#' );
	string subStr;
	
	while (  pos != string::npos )
	{
		subStr = replacedStr.substr( pos, 4 );
		if ( !needReplaceSubstrFace( subStr ) )
		{
			pos = replacedStr.find( '#', pos + 1 );
			continue;
		}
		replacedStr.replace( pos, 1, "##" );
		pos = replacedStr.find( '#', pos + 2 );
	}

	return replacedStr.c_str();
}

const char* BigInt2Str( long long val )
{
	static char buf[256];
	sprintf( buf,"%I64d",val );
	return buf;
}

void updateHeadBindingFrame( const char* pszFrameName, int x, int y, int nWidth, int mHeight )
{
	Frame* pAdjustPosFrame = (Frame*)g_pFrameMgr->FindLayoutFrame( pszFrameName );
	if ( pAdjustPosFrame == NULL )
	{
		return;
	}

	float fscale = g_pFrameMgr->GetScreenScaleY();
	pAdjustPosFrame->setAbsRect( x, y, x + nWidth,  y + mHeight );
	pAdjustPosFrame->SetSizeNoRecal( (int)(pAdjustPosFrame->m_AbsRect.getWidth()/fscale), 
		(int)(pAdjustPosFrame->m_AbsRect.getHeight()/fscale) );

// 	if ( pAdjustPosFrame->GetRight() <= 0 || pAdjustPosFrame->GetLeft() >= GetScreenWidth() || 
// 		pAdjustPosFrame->GetBottom() <= 0 || pAdjustPosFrame->GetTop() >= GetScreenHeight() )
// 	{
// 		if ( pAdjustPosFrame->IsShown() )
// 		{
// 			pAdjustPosFrame->Hide();
// 			return;
// 		}
// 	}
// 	else
// 	{
// 		if ( !pAdjustPosFrame->IsShown() )
// 		{
// 			pAdjustPosFrame->Show();
// 		}
// 	}
	std::vector<Frame::DrawObj>& drawObjes = pAdjustPosFrame->m_DrawRegions;
	for ( std::vector<Frame::DrawObj>::iterator iter = drawObjes.begin(), end = drawObjes.end();
		iter != end; ++iter )
	{
		iter->pregion->CalAbsRectSelf( - 1 );
	}
}

void showUIFrames()
{
	g_pFrameMgr->m_bRenderUI = true;
}