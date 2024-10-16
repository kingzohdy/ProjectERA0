#include "ui_framemgr.h"
#include "ui_frame.h"
#include "ui_fontstring.h"
#include "ui_texture.h"
#include "ui_button.h"
#include "ui_editbox.h"
#include "ui_checkbutton.h"
#include "ui_slider.h"
#include "ui_scrollframe.h"
#include "ui_richtext.h"
#include "ui_editbox.h"
#include "ui_modelview.h"
#include "ui_drawlineframe.h"
#include "ui_multieditbox.h"
#include "ui_webbrowerframe.h"
#include "ui_scriptfunc.h"
#include <algorithm>

#include "OgreScriptLuaVM.h"
#include "OgreRenderSystem.h"
#include "OgreDataStream.h"
#include "OgreResourceManager.h"
#include "OgreSceneManager.h"
#include "OgreLog.h"

/*
extern "C"
{
#include "lua/src/lua.h"
#include "lua/src/lualib.h"
#include "lua/src/lauxlib.h"
}*/


using namespace Ogre;

//-----------------------------------------------------------------------------
// Frame Manager
//-----------------------------------------------------------------------------
FrameManager *g_pFrameMgr = NULL;
stdext::hash_map<std::string, stEventFrameArray> EventMap;

static void onRestoreLostDeviceCallBack()
{
	g_pFrameMgr->m_pCurCursor->updateCursor();
}

bool RPictureCodeMap::Init()
{
	return Init( m_configFilePath.c_str() );
}

bool RPictureCodeMap::Init( const char* szFileName )
{
	Ogre::DataStream* fp = Ogre::ResourceManager::getSingleton().openFileStream( szFileName, true );
	if ( fp == NULL )
	{
		return false;
	}

	char szBuffer[OGRE_STREAM_TEMP_SIZE];
	while ( !fp->eof() )
	{
		if ( fp->readLine( szBuffer, sizeof( szBuffer ) ) == 0 )
		{
			continue;
		}

		std::string strBuffer( szBuffer, sizeof( szBuffer ) );
		size_t nPos	= 0;
		int data[8]	= { 0 };
		std::string sds;
		for ( int i = 0; i< sizeof(data) / sizeof(int); ++i )
		{
			sds		=  strBuffer.substr( nPos,strBuffer.find( ' ', nPos ) - nPos );
			nPos	= strBuffer.find( ' ', nPos ) + 1;
			data[i] = atoi( sds.c_str() );
		}

		PictureData picturedata;
		picturedata.m_nCode				= data[0];
		picturedata.m_nStartIdx			= data[1];
		picturedata.m_nFrameNum			= data[2];
		picturedata.m_nFrameInterval	= data[3];
		picturedata.m_nTexLeft			= data[4];
		picturedata.m_nTexTop			= data[5];
		picturedata.m_nTexWidth			= data[6];
		picturedata.m_nTexHeight		= data[7];
		
		m_pictureData[picturedata.m_nCode] = picturedata;
	}

	m_configFilePath = szFileName;
	delete fp;
	return true;
}

PictureData* RPictureCodeMap::GetPictureData( int nCode )
{
	std::map<int, PictureData>::iterator iter = m_pictureData.find( nCode );
	if ( iter == m_pictureData.end() )
	{
		return NULL;
	}

	return &( iter->second );
}

FrameManager::FrameManager() : m_pFocusFrame(NULL), m_pDragFrame(NULL), m_hIme(NULL), m_lastBeginDragFrame(NULL), m_enableAccel(true)
{
	m_lastBeginDragTime = 0;
	m_lastBeginDragFrame = NULL;
	m_CalAbsRectFrame = 0;
	m_iCurMaxFrmLevel = 1;

	m_iScreenWidth = m_iScreenHeight = 0;
	
	m_fAllScale = 1.0f;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;

	m_ObjectTable.clear();
	m_TemplateTable.clear();
	m_RootFrames.clear();
	m_DrawItems.clear();

	m_pFocusFrame = NULL;

	m_pCurCursor = new UICursor();

	m_HMAYFONT.clear();

	m_Accels.clear();

	m_LuaFile.clear();

	m_LastFrame = NULL;

	m_iEditMode = UI_MODE_DEFAULT;

	//表情数据初始化
	m_nFaceWidth		= 0;
	m_nFaceHeight		= 0;
	m_hTexRes			= NULL;
	m_pCodeMap			= NULL;
	m_pPictureBuffer	= new RPictureCodeMap;

	m_bUpdate			= true;
	m_bUpdatedGameFont	= true;
	m_bRenderUI			= true;
	m_bMouseClickDown	= false;
	RenderSystem::getSingleton().SetRestoreLostDeviceCallBack(onRestoreLostDeviceCallBack);
}

FrameManager::~FrameManager()
{
	size_t i;

	std::map<std::string, UIObject *>::iterator it = m_TemplateTable.begin();
	for( ; it != m_TemplateTable.end(); it++ )
	{
		(*it).second->release();
	}

	for(it=m_ObjectTable.begin(); it!=m_ObjectTable.end(); it++)
	{
		(*it).second->release();
	}

	for(i=0; i<m_RootFrames.size(); i++)
	{
		Frame *frame = m_RootFrames[i];
		frame->release();
	}

	delete m_pCurCursor;
	delete m_pCodeMap;
	delete m_pPictureBuffer;
	if( m_hTexRes )
	{
		g_pDisplay->ReleaseUIRes(m_hTexRes);
		m_hTexRes = NULL;
	}

	for(i=0; i<m_HMAYFONT.size(); i++)
	{
		g_pDisplay->ReleaseTrueTypeFont(m_HMAYFONT[i].h);
	}
}

void FrameManager::UpdateGameFont(double scale)
{
	if ( scale == m_fScaleY*m_fAllScale )
	{
		return;
	}
	m_bUpdatedGameFont = true;
	for ( unsigned int i = 0, size = m_HMAYFONT.size(); i < size; ++i )
	{
		UIFont& oneUIFontConfig = m_HMAYFONT[i];
		g_pDisplay->ReleaseTrueTypeFont( oneUIFontConfig.h );

		if ( ( scale != 0 ) && ( m_fScaleY != 0 ) )
		{
			oneUIFontConfig.height = (int)( m_FONTHEIGHT[i] * scale );
		}

		oneUIFontConfig.h = g_pDisplay->CreateTrueTypeFont(	oneUIFontConfig.height,			oneUIFontConfig.height, 
															oneUIFontConfig.type.c_str(),	CCODING_GBK );

		assert((oneUIFontConfig.h != NULL) && "FontString Huires is null");
	}
}

// 删除一个聊天泡泡
void FrameManager::delPopWindow( const char* pszRoleName )
{
	std::map<std::string, tagPopWin>::iterator iter = m_PopWinMap.find( pszRoleName );
	if ( iter != m_PopWinMap.end() )
	{
		std::string strPopName		= std::string( pszRoleName ) + std::string( "_Pop" );
		std::string strBackName		= std::string( pszRoleName ) + std::string( "_PopBack" );
		RichText* pRichText			= (RichText*)FindLayoutFrame( strPopName.c_str() );
		Frame* pRichBackGroup		= (Frame*)FindLayoutFrame( strBackName.c_str() );
		if ( pRichText != NULL )
		{
			pRichText->Hide();
			pRichBackGroup->Hide();
		}

		m_PopWinMap.erase( iter );
	}
}

// 添加一个聊天泡泡对话
void FrameManager::addPopWindow( const char* szPopBack, const char* szRoleName, const char* szText, 
									int nPopOffsetX, int nPopOffsetY, int nDuration, const char* szArrowTexName, 
										bool bShowTex, int nTexOffsetX, int nTexOffsetY )
{
	std::string strPopName		= std::string( szRoleName ) + std::string( "_Pop" );
	std::string strBackName		= std::string( szRoleName ) + std::string( "_PopBack" );
	
	Frame*		pRichBackGroup	= NULL;
	RichText*	pRichText		= NULL;
	if( FindLayoutFrame( strPopName.c_str() ) == NULL )
	{
		pRichText		= (RichText*)CreateObject( "RichText",	strPopName.c_str(), "ChatPopText" );
		pRichBackGroup	= (RichText*)CreateObject( "Frame",		strBackName.c_str(), szPopBack );
		// TODO
		
		pRichBackGroup->ReplaceSpecialName();
		pRichText->ReplaceSpecialName();
		
		pRichText->RegisterToFrameMgr( this );
		pRichBackGroup->RegisterToFrameMgr( this );

		pRichBackGroup->SetFrameDraw( false );
		pRichText->SetFrameDraw( false );
		
		AddRootFrame( pRichBackGroup );
		AddRootFrame( pRichText );
	}else{
		pRichText		= (RichText*)FindLayoutFrame( strPopName.c_str() );
		pRichBackGroup	= (Frame*)FindLayoutFrame( strBackName.c_str() );
	}

	float fscale = g_pFrameMgr->GetScreenScaleY();
	pRichText->m_bAutoExtend = true;
	pRichText->m_pScriptStr[SC_ONCLICK] = "RichText_OnClick();";
	pRichText->m_pScriptStr[SC_ONHIDE] = "TeamRoleFrameChatPop_OnHide();";
	pRichText->resizeRichWidth( (int)( pRichText->GetWidth()*fscale ) );
	if( strcmp( "NpcGuidePop", szPopBack ) != 0 )
	{
		pRichText->SetText( szText, 255, 255, 255 );
	}else{
		pRichText->SetText( szText, 86, 79, 54 );
	}

	int nHeight	= pRichText->GetTotalHeight();
	float fWidth = pRichText->GetTextLines() > 1 ? pRichText->GetWidth()*fscale:pRichText->getLineWidth(0);

	pRichText->setAbsRect( (float)nPopOffsetX, (float)nPopOffsetY, (float)nPopOffsetX+fWidth, (float)nPopOffsetY+nHeight );
	pRichBackGroup->setAbsRect( nPopOffsetX-10*fscale, nPopOffsetY-10*fscale, nPopOffsetX + fWidth+ 10*fscale, nPopOffsetY + nHeight+ 10*fscale  );
	pRichBackGroup->m_pParent	= GetUIClientFrame();
	pRichText->m_pParent		= GetUIClientFrame();
	pRichBackGroup->Show();
	pRichText->Show();

	Ogre::Point2D PosOffset, TexOffset, InitSize;
	PosOffset.x = nPopOffsetX;
	PosOffset.y = nPopOffsetY;
	TexOffset.x = nTexOffsetX;
	TexOffset.y = nTexOffsetY;

	tagPopWin popwin;
	popwin.m_nDuration			= ( unsigned int )timeGetTime() + nDuration*1000;

	popwin.m_bBottomTex			= bShowTex;
	popwin.m_strArrowTexName	= szArrowTexName;
	popwin.m_PopOffset			= PosOffset;
	popwin.m_TexOffset			= TexOffset;
	
	m_PopWinMap[szRoleName] = popwin; 
}

// 刷新泡泡框位置
void FrameManager::updatePopWinPos( const char* szRoleName, int x, int y, float fAlpha )
{
	std::string strPopName		= std::string( szRoleName ) + std::string( "_Pop" );
	std::string strBackName		= std::string( szRoleName ) + std::string( "_PopBack" );
	std::string strArrowName	= strBackName + "_ArrowTex";

	Frame* pRichBackGroup		= (Frame*)FindLayoutFrame( strBackName.c_str() );
	RichText* pRichText			= (RichText*)FindLayoutFrame( strPopName.c_str() );
	::Texture* pArrowTex		= (::Texture*)FindLayoutFrame( strArrowName.c_str() );

	std::map<std::string, tagPopWin>::iterator iter = g_pFrameMgr->m_PopWinMap.find( szRoleName );
	
	if ( iter == m_PopWinMap.end() )
	{
		return;
	}
	
	if ( pRichBackGroup == NULL || pRichText == NULL || pArrowTex == NULL )
	{
		return;
	}

	if ( !pRichBackGroup->IsShown() || !pRichText->IsShown() || !pArrowTex->IsShown() )
	{
		return;
	}
	
	const tagPopWin& onePopWinConfig = iter->second;
	float fscale = g_pFrameMgr->GetScreenScaleY();
	float fWidth = pRichText->GetTextLines() > 1 ? pRichText->GetWidth()*fscale:pRichText->getLineWidth(0);

	float fHight = (float)pRichText->GetTotalHeight();
	float fOffset = 10 * fscale;
	// nWidth最小必须是表情的宽度
	int nWidth = (int)(( floor(fWidth) > 30 )? floor(fWidth):30);
	int nHight = (int)(( floor(fHight) > 20 )? floor(fHight):20);

	/*if( g_pFrameMgr->m_nFaceWidth > nWidth || g_pFrameMgr->m_nFaceHeight > nHight )
	{
		assert( g_pFrameMgr->m_nFaceWidth < nWidth || g_pFrameMgr->m_nFaceHeight < nHight );
		return;
	}*/

	x = x - nWidth/2 - (int)fOffset;
	y = y - nHight/2;
	
	int nTmpX = FloatToInt( x - fOffset );
	pRichBackGroup->setAbsRect( nTmpX, y - fOffset, x + nWidth + fOffset, y + fOffset + nHight );
	pRichBackGroup->SetBlendAlpha( fAlpha );
	pRichBackGroup->SetBackDropBlendAlpha( fAlpha );
	// 窗口大小变化的时候在DrawBackDrop的时候会乘以缩放系数 为了保证RICH的背景大小不变 这里要先除以缩放系数
	pRichBackGroup->SetSizeNoRecal( (int)(pRichBackGroup->m_AbsRect.getWidth()/fscale), 
		(int)(pRichBackGroup->m_AbsRect.getHeight()/fscale) );

	if ( pRichBackGroup->GetRight() <= 0 || pRichBackGroup->GetLeft() >= GetScreenWidth() || 
			pRichBackGroup->GetBottom() <= 0 || pRichBackGroup->GetTop() >= GetScreenHeight() )
	{
		return;
	}

	pRichText->setAbsRect( (float)x, (float)y, (float)(x+nWidth), (float)(y+nHight) );
	pRichText->SetAlpha( fAlpha );
	pRichText->SetBackDropBlendAlpha( fAlpha );
	if ( pRichText->GetRight() <= 0 || pRichText->GetLeft() >= GetScreenWidth() || 
			pRichText->GetBottom() <= 0 || pRichText->GetTop() >= GetScreenHeight() )
	{
		return;
	}
	// TODO
	
	int nTexOffset = x + onePopWinConfig.m_TexOffset.x;
	if( onePopWinConfig.m_TexOffset.x == 0 )
	{
		nTexOffset = x + nWidth/2;
	}
	
	pArrowTex->SetBlendAlpha( fAlpha );
	float fTexLeft	= nTexOffset - fOffset;
	float fTexTop	=y+nHight+fOffset-3;
	pArrowTex->setAbsRect( fTexLeft, fTexTop, 
							fTexLeft + pArrowTex->GetWidth() * GetScreenScaleY(), 
							fTexTop + pArrowTex->GetHeight() * GetScreenScaleY() );

	pRichBackGroup->Draw();
	pRichText->Draw();
}
void FrameManager::updatePopWinPos(const char* szRoleName, float x,float y, float fAlpha )
{
	updatePopWinPos(szRoleName,(int)x,(int)y, fAlpha );
}


void FrameManager::hideFrame( const char* pszFrameName )
{
	LayoutFrame* pFrame = (LayoutFrame*)FindLayoutFrame( pszFrameName );
	if ( pFrame == NULL )
	{
		return;
	}

	if ( !pFrame->IsShown() )
	{
		return;
	}

	if ( pFrame == m_LastFrame )
	{
		m_LastFrame = NULL;
	}

	if ( pFrame == m_pFocusFrame )
	{
		ClearFocus();
		m_pFocusFrame = NULL;
	}

	if ( pFrame == m_pDragFrame )
	{
		m_pDragFrame = NULL;
	}

	if (m_lastBeginDragFrame == pFrame)
	{
		m_lastBeginDragFrame = NULL;
	}

	pFrame->Hide();
}

// 删除某个UI
void FrameManager::deleteBindingFrame( const char* pszFrameName )
{
	Frame* pFrame = (Frame*)FindLayoutFrame( pszFrameName );
	if ( pFrame == NULL )
	{
		return;
	}

	if ( pFrame == m_LastFrame )
	{
		m_LastFrame = NULL;
	}

	if ( pFrame == m_pFocusFrame )
	{
		ClearFocus();
		m_pFocusFrame = NULL;
	}

	if ( pFrame == m_pDragFrame )
	{
		m_pDragFrame = NULL;
	}

	if (m_lastBeginDragFrame == pFrame)
	{
		m_lastBeginDragFrame = NULL;
	}
	
	if ( strcmp( "FontString", pFrame->GetTypeName() ) != 0 && strcmp( "DrawLineFrame", pFrame->GetTypeName() ) != 0 &&
			strcmp( "ModelView", pFrame->GetTypeName() ) != 0 && strcmp( "Texture", pFrame->GetTypeName() ) != 0 )
	{
		for ( std::vector<Frame::DrawObj>::iterator iter = pFrame->m_DrawRegions.begin(), 
			end = pFrame->m_DrawRegions.end(); iter != end; ++iter )
		{
			deleteBindingFrame( iter->pregion->GetName() );
		}

		for ( std::vector<Frame *>::iterator iter = pFrame->m_Children.begin(), 
			end = pFrame->m_Children.end(); iter != end; ++iter )
		{
			deleteBindingFrame( (*iter)->GetName() );
		}
	}

	std::map<std::string, UIObject *>::iterator iter = m_ObjectTable.find( pszFrameName );
	if ( iter != m_ObjectTable.end() )
	{
		(*iter).second->release();
		m_ObjectTable.erase( iter );
	}

	for ( std::vector<Frame *>::iterator iter = m_RootFrames.begin(), end = m_RootFrames.end(); 
			iter != end; ++iter )
	{
		Frame* pFrame = *iter;
		if ( strcmp( pszFrameName, pFrame->GetName() ) == 0 )
		{
			pFrame->release();
			m_RootFrames.erase( iter );
			break;
		}
	}

	for ( std::vector<LayoutFrame *>::iterator iter = m_ReCalPosFrames.begin(); iter != m_ReCalPosFrames.end(); )
	{
		LayoutFrame* pRecalFrame = *iter;
		if ( strcmp( pszFrameName, pRecalFrame->GetName() ) == 0 )
		{
			iter = m_ReCalPosFrames.erase( iter );
			continue;
		}

		++iter;
	}
}

// 刷新头顶绑定的UI
void FrameManager::updateHeadBindingFrame( const char* pszFrameName, int x, int y, int nWidth, int nHeight )
{
	Frame* pAdjustPosFrame = (Frame*)FindLayoutFrame( pszFrameName );
	if ( pAdjustPosFrame == NULL )
	{
		return;
	}

	float fscale = g_pFrameMgr->GetScreenScaleY();
	pAdjustPosFrame->setAbsRect( x, y, x + nWidth,  y + nHeight );
	pAdjustPosFrame->SetSizeNoRecal( (int)(pAdjustPosFrame->m_AbsRect.getWidth()/fscale), 
								(int)(pAdjustPosFrame->m_AbsRect.getHeight()/fscale) );

	if( std::string( pszFrameName ) == "FurnitureOpFrame" ) {
		pAdjustPosFrame->SetPoint( FP_TOPLEFT, "UIClient", FP_TOPLEFT, x, y );
	}
	if ( pAdjustPosFrame->GetRight() <= 0 || pAdjustPosFrame->GetLeft() >= GetScreenWidth() || 
		pAdjustPosFrame->GetBottom() <= 0 || pAdjustPosFrame->GetTop() >= GetScreenHeight() )
	{
		if ( pAdjustPosFrame->IsShown() )
		{
			pAdjustPosFrame->Hide();
			return;
		}
	}
	else
	{
		if ( !pAdjustPosFrame->IsShown() && std::string( pszFrameName ) != "FurnitureOpFrame" )
		{
			pAdjustPosFrame->Show();
		}
	}
	std::vector<Frame::DrawObj>& drawObjes = pAdjustPosFrame->m_DrawRegions;
	for ( std::vector<Frame::DrawObj>::iterator iter = drawObjes.begin(), end = drawObjes.end();
			iter != end; ++iter )
	{
		iter->pregion->CalAbsRectSelf( - 1 );
	}
}

// 隐藏泡泡框
void FrameManager::hidePopWin( const char* szRoleName )
{
	std::string strPopName		= std::string( szRoleName ) + std::string( "_Pop" );
	std::string strBackName		= std::string( szRoleName ) + std::string( "_PopBack" );
	Frame* pRichBackGroup		= (Frame*)FindLayoutFrame( strBackName.c_str() );
	RichText* pRichText			= (RichText*)FindLayoutFrame( strPopName.c_str() );
	if ( pRichBackGroup != NULL && pRichText != NULL && pRichText->IsShown() && pRichBackGroup->IsShown() )
	{
		pRichBackGroup->Hide();
		pRichText->Hide();
	}
}

// 显示泡泡框
void FrameManager::showPopWin( const char* szRoleName )
{
	std::string strPopName		= std::string( szRoleName ) + std::string( "_Pop" );
	std::string strBackName		= std::string( szRoleName ) + std::string( "_PopBack" );
	Frame* pRichBackGroup		= (Frame*)FindLayoutFrame( strBackName.c_str() );
	RichText* pRichText			= (RichText*)FindLayoutFrame( strPopName.c_str() );
	if ( pRichBackGroup != NULL && pRichText != NULL && (!pRichText->IsShown() || pRichBackGroup->IsShown()) )
	{
		pRichBackGroup->Show();
		pRichText->Show();
	}
}

bool mod_equal ( LayoutFrame* elem1, LayoutFrame* elem2 )
{
	return (DWORD)elem1 == (DWORD)elem2;
};

void FrameManager::UpdateChangedFrames()
{
	const char *ptypename;
	if ( m_ReCalPosFrames.empty() )
	{
		return;
	}
	std::vector<LayoutFrame *>::iterator end = std::unique(m_ReCalPosFrames.begin(),m_ReCalPosFrames.end(),mod_equal);
	//更新位置改变的控件
	for ( std::vector<LayoutFrame *>::iterator iter = m_ReCalPosFrames.begin(); iter != end; ++iter )
	{
		LayoutFrame *playout = *iter;
		if ( playout == NULL )
		{
			continue;
		}

		ptypename = playout->GetTypeName();
		
		if ( strcmp( ptypename,"Texture" ) == 0 || strcmp( ptypename, "FontString" ) == 0 || 
				strcmp( ptypename,"ModelView" ) == 0 || strcmp( ptypename, "DrawLineFrame" ) == 0 )
		{
			playout->CalAbsRectSelf( -1 );
		}else{
			static_cast<Frame *>( playout )->CalFrameRectRecursive( -1 );
		}
	}
	m_ReCalPosFrames.clear();
}

float FrameManager::GetAllSelfScale()
{
	return m_fAllScale;
}

void FrameManager::UpdateAllSelfScale( float fScale )
{
	UpdateGameFont(m_fScaleY*fScale);
	m_fAllScale = fScale;	
	g_pFrameMgr->m_bUpdate = true;
}

void FrameManager::Update(float deltatime)
{
	m_CalAbsRectFrame++;

	unsigned int i;
	if (m_bUpdate)
	{
		for(i=0; i<m_RootFrames.size(); ++i)
		{
			m_RootFrames[i]->CalFrameRectRecursive(m_CalAbsRectFrame);
		}

		m_bUpdate			= false;
		m_bUpdatedGameFont	= false;
	}
	else
	{
		UpdateChangedFrames();
	}

	for(i=0; i<m_RootFrames.size(); ++i)
	{
		m_RootFrames[i]->UpdateSelf(deltatime);
	}

	//如果在updata过程中OnUpdate消息处理某个控件时改变了位置，需要重新计算一下实际位置
	UpdateChangedFrames();
	
	//m_pCurCursor->update(deltatime);
	
	for ( std::map<std::string, tagPopWin>::iterator iter = m_PopWinMap.begin(); iter != m_PopWinMap.end(); ++iter )
	{
		if ( ( unsigned int )timeGetTime() > iter->second.m_nDuration )
		{
			std::string strPopName		= iter->first + std::string( "_Pop" );
			std::string strBackName		= iter->first + std::string( "_PopBack" );

			Frame* pRichBackGroup	= (Frame*)FindLayoutFrame( strPopName.c_str() );
			RichText* pRichText		 = (RichText*)FindLayoutFrame( strBackName.c_str() );

			if( pRichBackGroup != NULL && pRichText != NULL && pRichText->IsShown() && pRichBackGroup->IsShown() )
			{
				pRichText->Hide();
				pRichBackGroup->Hide();
			}
		}
	}
}

void FrameManager::AddDrawItems(LayoutFrame *pObj)
{
	m_DrawItems.push_back(pObj);
}

static bool LessThan(const LayoutFrame *pLayoutFrame1, const LayoutFrame *pLayoutFrame2)
{
	if(pLayoutFrame1->m_FrameStrata == pLayoutFrame2->m_FrameStrata)
	{
		return pLayoutFrame1->m_iDrawLevel < pLayoutFrame2->m_iDrawLevel;
	}
	else return pLayoutFrame1->m_FrameStrata < pLayoutFrame2->m_FrameStrata;
}

static bool MoreThan(const LayoutFrame *pLayoutFrame1, const LayoutFrame *pLayoutFrame2)
{
	//ToolTip控件不参与消息响应, 故排序
	if (pLayoutFrame1->m_FrameStrata == FS_TOOLTIP)
	{
		return false;
	}
	if(pLayoutFrame1->m_FrameStrata == pLayoutFrame2->m_FrameStrata)
	{
		return pLayoutFrame1->m_iDrawLevel > pLayoutFrame2->m_iDrawLevel;
	}
	else return pLayoutFrame1->m_FrameStrata > pLayoutFrame2->m_FrameStrata;
}

bool IsCtrlPress(int code)
{
	return code & ACCEL_CTRL;
}

bool IsShiftPress(int code)
{
	return code & ACCEL_SHIFT;
}

bool IsAltPress(int code)
{
	return code & ACCEL_ALT;
}

int GetControlKeyCode(bool ctrl, bool shift, bool alt)
{
	int code = 0;
	if (ctrl)
	{
		code |= ACCEL_CTRL;
	}
	if (shift)
	{
		code |= ACCEL_SHIFT;
	}
	if (alt)
	{
		code |= ACCEL_ALT;
	}
	return code;
}

void FrameManager::Render()
{
	/*
    if(GISS->GSE->IsInLoading())
    {
        static int fLastTime = timeGetTime();
        g_pFrameMgr->Update(timeGetTime() - fLastTime);
        Button* pLoadingBar = static_cast<Button*>(FindLayoutFrame("LodingProgress"));
        pLoadingBar->SetNormalRegionWidth(GISS->GSE->GetLoadingPercentage());
    }*/

	unsigned int i;
	for(i=0; i<m_RootFrames.size(); ++i)
	{
		m_RootFrames[i]->FillDrawItems();
	}

	//2级排序:FrameStrata为第一级，FrameLevel为第二级
	std::stable_sort(m_DrawItems.begin(), m_DrawItems.end(), LessThan);

	for (i=0; i<m_DrawItems.size(); ++i)
	{
		FRAMESTRATA_T f = m_DrawItems[i]->m_FrameStrata;
		if( !m_bRenderUI && f > FS_PARENT )
		{
			continue;
		}
		LayoutFrame *p = m_DrawItems[i];
		m_DrawItems[i]->Draw();
	}

	m_DrawItems.clear();

	// TODO::由于有些层次在doFrame中还会绘制，所以在在外部最后绘制
	//m_pCurCursor->Draw();
}

void FrameManager::AddGameFont(UIFont font)
{
	m_HMAYFONT.push_back(font);
	// 记录字体原始大小
	m_FONTHEIGHT.push_back(font.height);
}

void FrameManager::AddGameAccels(AccelItem accel)
{
	m_Accels.push_back(accel);
}

int FrameManager::OnInputMessage(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_CHAR:
		if(m_pFocusFrame && m_pFocusFrame->IsShown() && m_bRenderUI )
		{
			return m_pFocusFrame->OnInputMessage(event);
		}
		break;

	case GIE_KEYDOWN:
		{
			if ( m_pFocusFrame && m_pFocusFrame->IsShown() && m_bRenderUI )
			{
				if ( strcmp(m_pFocusFrame->GetTypeName(), "EditBox") && strcmp( m_pFocusFrame->GetTypeName(),"MultiEditBox") )
				{
					if(OnAccelerator(event)) 
					{
						return 0;
					}
				}
				//对快捷键需要响应的编辑框控件...
				else if (!strcmp(m_pFocusFrame->GetName(),"LoginNameEdit" )
					 	||!strcmp(m_pFocusFrame->GetName(),"PassWordEdit")
					 	||!strcmp(m_pFocusFrame->GetName(),"CSCreateName")
					 	)
				{
					OnAccelerator(event);
				}

				if (m_pFocusFrame)
				{
					return m_pFocusFrame->OnInputMessage(event);
				}
			}
			else
			{
				if(OnAccelerator(event)) 
				{
					return 0;
				}
			}
			break;
		}

	case GIE_KEYUP:
		{
			if (m_pFocusFrame)
			{
				if (strcmp(m_pFocusFrame->GetTypeName(), "EditBox") && strcmp( m_pFocusFrame->GetTypeName(),"MultiEditBox") )
				{
					if(OnAccelerator(event)) 
					{
						return 0;
					}
				}

				if (m_pFocusFrame && m_pFocusFrame->IsShown() && m_bRenderUI )
				{
					return m_pFocusFrame->OnInputMessage(event);
				}
			}
			else
			{
				if(OnAccelerator(event)) 
				{
					return 0;
				}
			}
			break;
		}

	case GIE_LBTNDBLCLK:
		{
			Frame* pframe = FindUIObjectOnPoint(event.mouse.x, event.mouse.y);
			if (pframe && pframe->IsShown())
			{
				SetFocusFrame(pframe);
				return pframe->OnInputMessage(event);
			}
		}
		break;

	case GIE_LBTNDOWN:
		{
			m_bMouseClickDown = true;
			Frame* pframe = FindUIObjectOnPoint(event.mouse.x, event.mouse.y);
			if(pframe && pframe->IsShown() && (m_bRenderUI||pframe->m_FrameStrata == FS_PARENT) )
			{
				Frame *pFocusFrame = GetFocusFrame();
				if(pFocusFrame != pframe)
				{
					if( pFocusFrame ) 
					{
						if( std::string( pFocusFrame->GetTypeName() ) == "EditBox" )
						{
							( (EditBox*)pFocusFrame )->enableIME( false );
						}
						if( std::string( pFocusFrame->GetTypeName() ) == "MultiEditBox" )
						{
							( (MultiEditBox*)pFocusFrame )->enableIME( false );
						}

						if ( pFocusFrame->hasScriptsEvent(SC_ONFOCUSLOST) )
						{
							pFocusFrame->CallScript(SC_ONFOCUSLOST);
						}
					}
					SetFocusFrame(pframe);
					/*if( std::string( pframe->GetTypeName() ) == "EditBox" )
					{
						if( ( (EditBox*)pframe )->m_EditMethod & UI_EDITMETHOD_CODER )
						{
							( (EditBox*)pframe )->enableIME( false );
						}else{
							( (EditBox*)pframe )->enableIME( true );
						}
					}*/
				}

				return pframe->OnInputMessage(event);
			}
		}
		break;

	case GIE_LBTNUP:
		{
			SetBeginDragFrame();
			m_bMouseClickDown = false;
			if (m_pFocusFrame && m_pFocusFrame->IsShown() && (m_bRenderUI||m_pFocusFrame->m_FrameStrata == FS_PARENT) )
			{
				return GetFocusFrame()->OnInputMessage(event);
			}
			break;
		}

	case GIE_RBTNDOWN:
		{
			m_bMouseClickDown = true;
			if( !UIIsInDragState() )
			{
				Frame* pframe = FindUIObjectOnPoint(event.mouse.x, event.mouse.y);
				if (pframe && pframe->IsShown() && m_bRenderUI ) 
				{
					Frame *pFocusFrame = GetFocusFrame();

					if( pFocusFrame != NULL && pFocusFrame != pframe )
					{
						if( std::string( pFocusFrame->GetTypeName() ) == "EditBox" )
						{
							( (EditBox*)pFocusFrame )->enableIME( false );
						}
						if( std::string( pFocusFrame->GetTypeName() ) == "MultiEditBox" )
						{
							( (MultiEditBox*)pFocusFrame )->enableIME( false );
						}
						if (pFocusFrame && pFocusFrame->hasScriptsEvent(SC_ONFOCUSLOST))
						{
							pFocusFrame->CallScript(SC_ONFOCUSLOST);
						}
					}

					SetFocusFrame(pframe);
					return pframe->OnInputMessage(event);
				}
			}
			break;
		}

	case GIE_RBTNUP:
		{
			m_bMouseClickDown = false;
			if( !UIIsInDragState() )
			{
				if(m_pFocusFrame && m_pFocusFrame->IsShown() && m_bRenderUI )
				{
					return GetFocusFrame()->OnInputMessage(event);
				}
			}
			break;
		}

	case GIE_MOUSEIN:
		{
			m_pCurCursor->ShowCursor();
		}
		break;

	case GIE_MOUSEOUT:
		m_pCurCursor->HideCursor();
		//m_RootFrames
		for (size_t i = 0 ; i < m_RootFrames.size() ; i++ )
		{
			if (m_RootFrames[i]->IsShown() && m_bRenderUI )
			{
				m_RootFrames[i]->OnInputMessage(event);
			}
		}
		break;

	case GIE_MOUSEMOVE:
		{
			int mousex = event.mouse.x;
			int mousey = event.mouse.y;
			UICursor::m_Pos = Ogre::Point2D( mousex, mousey );
			Frame* pframe = NULL;
			//如果鼠标是在点击状态，而且有焦点界面
			if( m_bMouseClickDown && GetFocusFrame() != NULL 
				&& strcmp(GetFocusFrame()->GetTypeName(),"Button")!=0 )
			{
				pframe = GetFocusFrame();
			}else{
				pframe = FindUIObjectOnPoint( mousex, mousey, false );
			}

			// 筛选的过程进行改变，(1)若是MOUSEMOVE事件的时候，a.若RichText透明且有超链接的时候，则在鼠标位于
			// 超链接的时候保留RichText，则只处理RichText事件；b.若RichText透明，但是鼠标没有位于超链接的时候，
			// 则将消息穿透到下一层
			
			if ( m_pDragFrame != NULL )
			{
				if ( GetKeyState( VK_LBUTTON ) & 0xff00 )
				{
					pframe = m_pDragFrame;
				}
				else
				{
					m_pDragFrame->m_State &= ~FRMSTATE_CLICKDOWN;
					m_pDragFrame = NULL;
				}
			}
			if( pframe != m_LastFrame )
			{
				Ogre::InputEvent ev;
				ev.mouse.x = event.mouse.x;
				ev.mouse.y = event.mouse.y;
				if( m_LastFrame != NULL )
				{
					ev.msg = GIE_MOUSEOUT;
					m_LastFrame->OnInputMessage( ev );
				}
				m_LastFrame = pframe;
				if( pframe != NULL )
				{					
					ev.msg = GIE_MOUSEIN;
					pframe->OnInputMessage( ev );
				}
			}

			//if (pframe && (m_pFocusFrame == pframe))
			if ( pframe != NULL )
			{
				if ( !m_pCurCursor->IsInDragState() && (GetCurrentCursorLevel() ==eCursorMoveChange )
						&& UI_ROOT_NAME!=pframe->GetName() && !pframe->m_bInputTransparent )
				{
					SetCurrentCursor( "normal" );
				}				

				if( pframe->IsShown() && m_bRenderUI )
				{
					int result = pframe->OnInputMessage( event );
					if (event.mouse.flags == VK_LBUTTON && m_lastBeginDragFrame)
					{
						if (m_lastBeginDragFrame->IsShown())
						{
							unsigned int now = timeGetTime();
							int x = ::GetCursorPosX();
							int y = ::GetCursorPosY();
							m_lastBeginDragFrame->OnBeginDrag(now - m_lastBeginDragTime, x - m_lastBeginDragX, y - m_lastBeginDragY);
						}
						else
						{
							SetBeginDragFrame();
						}
					}
					return result;
				}
			}
		}
		break;

	case GIE_WINSIZE:
		{
			const WinSizeEventData& winSizeData = event.winsize;
			if ( winSizeData.sizey==0 || winSizeData.sizex==0 )
			{
				return 0;
			}

			if( (float)winSizeData.sizey <= DEFAULT_UI_HEIGHT )
			{
				UpdateGameFont((double)winSizeData.sizey*m_fAllScale/DEFAULT_UI_HEIGHT);
				g_pFrameMgr->SetScreenScaleX((float)winSizeData.sizex/DEFAULT_UI_WIDTH);
				g_pFrameMgr->SetScreenScaleY((float)winSizeData.sizey/DEFAULT_UI_HEIGHT);
			}
			else
			{
				UpdateGameFont(m_fAllScale);
				g_pFrameMgr->SetScreenScaleX(1.0);
				g_pFrameMgr->SetScreenScaleY(1.0);
			}
			g_pFrameMgr->SetScreenWidth(winSizeData.sizex);
			g_pFrameMgr->SetScreenHeight(winSizeData.sizey);
			Ogre::SceneManager::getSingleton().setScreenSize( winSizeData.sizex, winSizeData.sizey );
			g_pFrameMgr->m_bUpdate = true;
			return 0;
		}

	case GIE_MOUSEWHEEL:
		{
			Frame* pframe	= FindUIObjectOnPoint( GetCursorPosX(), GetCursorPosY(), false);
			if ( pframe != NULL && UI_ROOT_NAME != pframe->GetName() && pframe->IsShown() && m_bRenderUI )
			{
				// 若父窗口不是UIClient，则将滚轮消息转发给父窗口
				for (; pframe->m_pParent != NULL; pframe = pframe->m_pParent)
				{
					if (UI_ROOT_NAME == pframe->m_pParent->GetName() || pframe->ProcessMouseWheel())
					{
						break;
					}
				}
				return pframe->OnInputMessage(event);
			}
		}
		break;

	default:
		break;
	}

	//自己不处理,向父亲发送
	return 1;
}

UIObject* FrameManager::FindLayoutFrame(const std::string &name)
{
	std::map<std::string, UIObject *>::iterator iter = m_ObjectTable.find(name);
	if (iter != m_ObjectTable.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}

	//return m_ObjectTable[name];
}

UIObject* FrameManager::FindTexture(const std::string &name,const std::string &Texturename)
{
	std::map<std::string, UIObject *>::iterator iter = m_ObjectTable.find(name);
	if (iter != m_ObjectTable.end())
	{
		Frame* TempFrame = (Frame *)iter->second;
		for ( int i = 0; i < TempFrame->m_DrawRegions.size(); i++ )
		{
			if ( TempFrame->m_DrawRegions[i].pregion->m_Name == Texturename )
			{
				return (UIObject *)TempFrame->m_DrawRegions[i].pregion;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
}

//void FrameManager::SetScreenScaleY(float scale)
//{ 
//	scale = 0.89843750;
//	m_fScaleY = scale; 
//}

unsigned int FrameManager::GetCalAbsRectFrame()
{
	return m_CalAbsRectFrame;
}

Frame *FrameManager::FindUIObjectOnPoint(int mousex, int mousey, bool bLevelChanged )
{
	unsigned int i;
	std::vector <Frame *> vFrameTemp;
	std::vector <Frame *> vModalFrame;
	for (i=0; i<m_RootFrames.size(); ++i)
	{
		if (m_RootFrames[i]->IsShown())
		{
			m_RootFrames[i]->FindFrameOnPoint(mousex, mousey, vFrameTemp);
		}
	}

	if (vFrameTemp.size() > 0)
	{
		std::stable_sort(vFrameTemp.begin(), vFrameTemp.end(), MoreThan);

#ifdef _DEBUG
		for (unsigned int i=0; i<vFrameTemp.size(); ++i)
		{
			Frame *p = vFrameTemp[i];
			i = i;
		}
#endif
		if (bLevelChanged)
		{
			Frame* pFrame = GetRootFrame( vFrameTemp[0] );
			if (pFrame != NULL)
			{
				FindModalFrame(pFrame->m_Name.c_str(),vModalFrame);
				if( vModalFrame.size() > 0 )
				{
					//将父窗口和最上层模态子窗口的层级提升，并返回找到的子窗口
					vFrameTemp[0]->AddLevelRecursive(1);
					vModalFrame[0]->AddLevelRecursive(1);
					return vModalFrame[0];
				}
			}			
			
			for (unsigned int i=0; i<vFrameTemp.size(); ++i)
			{
				Frame *parent_frame = vFrameTemp[i];
				while (true)
				{
					const char *parent = parent_frame->GetParent();
					if (parent == NULL || UI_ROOT_NAME == parent)
					{
						break;
					}
					parent_frame = (Frame *)FindLayoutFrame(parent);
					if (parent_frame == NULL)
					{
						break;
					}
				}
				if( parent_frame && parent_frame->m_iFrameLevel != -1 && 
					std::find(m_RootFrames.begin(),m_RootFrames.end(),parent_frame)!= m_RootFrames.end() )
				{
					parent_frame->AddLevelRecursive(1);
					break;
				}
			}
		}
		return vFrameTemp[0];
	}
	return NULL;
}

void FrameManager::FindUIObjectOnPoint( std::vector <Frame*>& mouseOnFrames, int mousex, int mousey, bool bLevelChanged )
{
	for ( size_t i=0; i<m_RootFrames.size(); ++i )
	{
		if (m_RootFrames[i]->IsShown())
		{
			m_RootFrames[i]->FindFrameOnPoint( mousex, mousey, mouseOnFrames );
		}
	}

	if (mouseOnFrames.size()==1)
	{
		return;
	}
	else if (mouseOnFrames.size()>1)
	{
		std::stable_sort(mouseOnFrames.begin(), mouseOnFrames.end(), MoreThan);
	}

	if (bLevelChanged)
	{
		for (unsigned int i=0; i<mouseOnFrames.size(); ++i)
		{
			if (mouseOnFrames[i]->m_iFrameLevel != -1)
			{
				mouseOnFrames[i]->AddLevelRecursive(1);
				break;
			}
		}
	}
}

void FrameManager::FindModalFrame( const char *name, std::vector<Frame *> &vModalFrame )
{
	for ( size_t i=0; i<m_RootFrames.size(); ++i )
	{
		if (m_RootFrames[i]->IsShown() && (strcmp(name,m_RootFrames[i]->m_strModalFrame.c_str()) == 0) )
		{
			vModalFrame.push_back( m_RootFrames[i] );
		}
	}

	if( vModalFrame.size() > 0 )
	{
		std::stable_sort(vModalFrame.begin(), vModalFrame.end(), MoreThan);
	}
}

Frame* FrameManager::GetRootFrame( Frame *pFrame )
{
	while ( pFrame->m_pParent != NULL && strcmp(UI_ROOT_NAME.c_str(),(pFrame->m_pParent)->m_Name.c_str()) != 0 )
	{
		pFrame = pFrame->m_pParent;
	}
	return pFrame;
}

bool FrameManager::CreateLuaTable(Frame *pFrame)
{
	if (pFrame)
	{
		if(!CreateObjLuaTable(pFrame, pFrame->GetName()))
		{
			assert(0 && "worong type Res");
			return false;
		}

		for (size_t i=0; i<pFrame->m_DrawRegions.size(); ++i)
		{
			CreateObjLuaTable(pFrame->m_DrawRegions[i].pregion, pFrame->m_DrawRegions[i].pregion->GetName());
			if (pFrame->m_DrawRegions[i].pregion->hasScriptsEvent(SC_ONLOAD))
			{
				pFrame->m_DrawRegions[i].pregion->CallScript(SC_ONLOAD);
			}
		}

		for (size_t i=0; i<pFrame->m_Children.size(); ++i)
		{
			CreateLuaTable(pFrame->m_Children[i]);
		}

		if (pFrame->hasScriptsEvent(SC_ONLOAD))
		{
			pFrame->CallScript(SC_ONLOAD);
		}
		return true;
	}

	return false;
}

static int lines = 0;
bool FrameManager::CreateObjLuaTable(UIObject *pobj, const char *name)
{
	if(pobj)
	{
		if (name)
		{
			g_pUIScriptVM->setUserTypePointer(name, pobj->GetTypeName(), pobj);
			return true;
		}
	}

	return false;
}

UIObject *FrameManager::CreateObject(const char *type_name, const char *name, const char *template_name)
{
	if(type_name==NULL || name==NULL)
	{
		assert(0);
		return NULL;
	}

	UIObject *pobj;
	if(template_name && template_name[0])
	{
		std::string str(template_name);
		std::map<std::string, UIObject *>::iterator iter = m_TemplateTable.find(str);
		assert(iter != m_TemplateTable.end());

		pobj = iter->second->CreateClone();
		pobj->m_Inherits = template_name;
	}
	else
	{
		pobj = InternalCreateUIObj(type_name, name);
	}

	pobj->SetName(name);
	return pobj;
}

void FrameManager::RegisterObject(UIObject *pobj)
{
	pobj->addRef();
	if(pobj->m_bTemplate)
	{
		/*bool bEnd = m_TemplateTable.find(pobj->m_Name) == m_TemplateTable.end();
		if ( !bEnd )
		{
			cout << "模板" << pobj->m_Name << "名字重复" << endl;
			LOG_PRIVATE( "模板%s名字重复", pobj->m_Name.c_str() );
		}*/
		assert(m_TemplateTable.find(pobj->m_Name) == m_TemplateTable.end());
		m_TemplateTable[pobj->m_Name] = pobj;
	}
	else
	{
	/*	bool bEnd = m_ObjectTable.find(pobj->m_Name) == m_ObjectTable.end();
		if ( !bEnd )
		{
			cout << "控件" << pobj->m_Name << "名字重复" << endl;
			LOG_PRIVATE( "控件%s名字重复", pobj->m_Name.c_str() );
		}*/
		assert(m_ObjectTable.find(pobj->m_Name) == m_ObjectTable.end());
		m_ObjectTable[pobj->m_Name] = pobj;
	}
}

UIObject *FrameManager::InternalCreateUIObj(const char *type_name, const char *name)
{
	if (type_name && name)
	{
		UIObject *pretobj = NULL;
		assert(name && name[0]);

		if (!stricmp(type_name, "Frame"))
		{
			Frame *pframe = new Frame;
			if(name)
			{
				pframe->SetName(name);
			}

			pretobj = pframe;
		}
		
		if ( !stricmp( type_name, "DrawLineFrame" ) )
		{
			DrawLineFrame* pline = new DrawLineFrame;
			if ( name )
			{
				pline->SetName( name );
			}
			pretobj = pline;
		}
		if(!stricmp(type_name, "FontString"))
		{
			FontString *pfontstr = new FontString;
			if(name)
			{
				pfontstr->SetName(name);
			}

			pretobj = pfontstr;
		}

		if(stricmp(type_name, "ModelView") == 0)
		{
			ModelView *pview = new ModelView;
			if(name)
			{
				pview->SetName(name);
			}

			pretobj = pview;
		}

		if(!stricmp(type_name, "Texture"))
		{
			::Texture *ptex = new ::Texture;
			if(name)
			{
				ptex->SetName(name);
			}
			pretobj = ptex;
		}

		if(!stricmp(type_name, "Button"))
		{
			Button *pbtn = new Button;
			if(name)
			{
				pbtn->SetName(name);
			}
			pbtn->Init();
			pretobj = pbtn;
		}

		if(!stricmp(type_name, "EditBox"))
		{
			EditBox *peditbox = new EditBox;
			if(name)
			{
				peditbox->SetName(name);
			}
			pretobj = peditbox;
		}

		if(!stricmp(type_name, "CheckButton"))
		{
			CheckButton *pcheckbutton = new CheckButton;
			if(name)
			{
				pcheckbutton->SetName(name);
				pcheckbutton->Init();
			}
			pretobj = pcheckbutton;
		}

		if(!stricmp(type_name, "Slider"))
		{
			Slider *pslider = new Slider;
			if(name)
			{
				pslider->SetName(name);
			}
			pretobj = pslider;
		}

		if(!stricmp(type_name, "ScrollFrame"))
		{
			ScrollFrame *pscrollframe = new ScrollFrame;
			if(name)
			{
				pscrollframe->SetName(name);
			}
			pretobj = pscrollframe;
		}

		if(!stricmp(type_name, "RichText"))
		{
			RichText *prichtext = new RichText;
			if(name)
			{
				prichtext->SetName(name);
			}
			pretobj = prichtext;
		}

		if(!stricmp(type_name, "MultiEditBox"))
		{
			MultiEditBox *pmultieditbox = new MultiEditBox;
			if(name)
			{
				pmultieditbox->SetName(name);
			}
			pretobj = pmultieditbox;
		}

 		if (!stricmp( type_name, "WebBrowerFrame"))
 		{
 			WebBrowerFrame *pWebBrowerFrame = new WebBrowerFrame;
 			if (name)
 			{
 				pWebBrowerFrame->SetName(name);
 			}
 			pretobj = pWebBrowerFrame;
 		}

		return pretobj;
	}

	return NULL;
}

void FrameManager::AddReCalPosFrame( LayoutFrame *pframe )
{
	if ( std::find( g_pFrameMgr->m_ReCalPosFrames.begin(), g_pFrameMgr->m_ReCalPosFrames.end(), pframe ) == 
			g_pFrameMgr->m_ReCalPosFrames.end() )
	{
		g_pFrameMgr->m_ReCalPosFrames.push_back( pframe );
	}
}

void FrameManager::AddRootFrame(Frame *pframe)
{
	pframe->m_pParent = NULL;
	pframe->addRef();
	m_RootFrames.push_back(pframe);

	//if(pframe->m_iDrawLevel > m_iCurMaxFrmLevel)
	//	m_iCurMaxFrmLevel = pframe->m_iDrawLevel;
}

Frame *FrameManager::GetFocusFrame()
{
	return m_pFocusFrame;
}

void FrameManager::SetFocusFrame(Frame *pframe)
{
	if (pframe == m_pFocusFrame) return;

	//if(pframe)
	//{
	//	pframe->AddLevelRecursive(m_iCurMaxFrmLevel - pframe->m_iDrawLevel + 1);
	//	m_iCurMaxFrmLevel++;
	//}
	if (m_pFocusFrame)
	{
		if( std::string( m_pFocusFrame->GetTypeName() ) == "EditBox" )
		{
			( (EditBox*)m_pFocusFrame )->enableIME( false );
		}
		else if( std::string( m_pFocusFrame->GetTypeName() ) == "MultiEditBox" )
		{
			( (MultiEditBox*)m_pFocusFrame )->enableIME( false );
		}
	}
	m_pFocusFrame = pframe;

	if( std::string( pframe->GetTypeName() ) == "EditBox" )
	{
		if( ( (EditBox*)pframe )->m_EditMethod & UI_EDITMETHOD_CODER )
		{
			( (EditBox*)pframe )->enableIME( false );

		}else{
			( (EditBox*)pframe )->enableIME( true );
		}
	}
	else if( std::string( pframe->GetTypeName() ) == "MultiEditBox" )
	{
		( (MultiEditBox*)pframe )->enableIME( true );
	}

	pframe->CallScript(SC_ONFOCUSGAINED);
}

void FrameManager::ClearFocus()
{
	if (m_pFocusFrame == NULL)
	{
		return;
	}
	if (m_pFocusFrame->hasScriptsEvent(SC_ONFOCUSLOST))
	{
		m_pFocusFrame->CallScript(SC_ONFOCUSLOST);
	}

	if( std::string( m_pFocusFrame->GetTypeName() ) == "EditBox" )
	{
		( (EditBox*)m_pFocusFrame )->enableIME( false );
	}
	else if( std::string( m_pFocusFrame->GetTypeName() ) == "MultiEditBox" )
	{
		( (MultiEditBox*)m_pFocusFrame )->enableIME( false );
	}

	m_pFocusFrame = NULL;
}

//返回0:不需要处理快捷键
//返回1:需要处理快捷键
//返回-1:脚本没有返回值,或者出错
int FrameManager::ProcessAccelerator(const AccelItem& accel, bool keydown_or_keyup)
{
	const char *arg1 = keydown_or_keyup?"keydown":"keyup";
	for(size_t i=0; i<m_Accels.size(); ++i)
	{ 
		if(m_Accels[i] == accel)
		{
			g_pFrameMgr->GetUIClientFrame()->CallScript(m_Accels[i].accel_script, "s", arg1);
		}
	}
	{
		int code = accel.accel_key + GetControlKeyCode(accel.modifier_ctrl, accel.modifier_shift, accel.modifier_alt);
		char buff[100];
		sprintf(buff, "OnAccelerator(%d, %s)", code, keydown_or_keyup ? "true" : "false");
		g_pUIScriptVM->callString(buff);
	}

	return 0;
}

////查找接受当前键盘输入的Frame
////先看该快捷键是否在自己里面有处理，然后看是否在自己的上级里面有处理
//Frame *Frame::FindFrameForAccel( const AccelItem& accel )
//{
//	std::string strname = this->GetName();
//
//	Frame *pFrame = this;
//
//	if( pFrame->IsAccelIn(accel) )
//	{
//		return pFrame;
//	}
//
//	for (size_t i=0; i<m_Children.size(); ++i)
//	{
//		pFrame = m_Children[i]->FindFrameForAccel(accel);
//		if (pFrame)
//			return pFrame;
//	}
//
//	return NULL;
//}

//std::vector<AccelItem> &Frame::GetFrmAccels()
//{
//	return m_Accels;
//}

////判断窗口是否能够处理指定的快捷键
//bool Frame::IsAccelIn(const AccelItem &accel)
//{
//	if(m_Accels.empty()) return false;
//
//	for(size_t i=0; i<m_Accels.size(); ++i)
//	{
//		AccelItem a;
//		a = m_Accels[i];
//		if(m_Accels[i] == accel)
//			return  true;			
//	}	
//
//	return false;
//}

#define MK_PREVDOWN             (1<<30)

////查找是否有相应的快捷键
//Frame *Frame::FindAccelFrame(unsigned int msgid, WPARAM wparam, LPARAM lparam)
//{
//	AccelItem accel;
//
//	accel.accel_key = (char)wparam;
//
//	accel.modifier_shift = ((lparam & MK_SHIFT) != 0);
//	accel.modifier_ctrl = ((lparam & MK_CONTROL) != 0);
//	accel.modifier_alt = ((lparam & MK_ALT) != 0);
//
//	if(msgid == MSGTYPE_KEYUP) 
//	{
//		accel.accel_type = UI_KEY_UP;
//	}
//	else if(msgid == MSGTYPE_KEYDOWN) 
//	{
//		if(lparam & MK_PREVDOWN)
//			accel.accel_type = UI_KEY_DOWN;
//		else 
//			accel.accel_type = UI_KEY_PRESS;
//	}
//
//	Frame* pFrame = NULL;
//	for (unsigned int i=0; i<g_pFrameMgr->m_RootFrames.size(); ++i)
//	{
//		pFrame = g_pFrameMgr->m_RootFrames[i]->FindFrameForAccel(accel);
//		if (pFrame)
//		{
//			break;
//		}
//	}
//
//	return pFrame;
//}

//响应快捷键
int FrameManager::OnAccelerator(const Ogre::InputEvent &event)
{
	if (m_enableAccel)
	{
		AccelItem accel;

		accel.accel_key = event.key.vkey;
		accel.modifier_shift	= ((event.key.flags & MK_SHIFT) != 0);
		accel.modifier_ctrl		= ((event.key.flags & MK_CONTROL) != 0);
		accel.modifier_alt		= ((event.key.flags & MK_ALT) != 0);

		//GetKeyboardState
		//accel.modifier_shift = ((GetAsyncKeyState(VK_SHIFT)&0xff00)!=0);
		//accel.modifier_ctrl = ((GetAsyncKeyState(VK_CONTROL)&0xff00)!=0);
		//accel.modifier_alt = ((GetAsyncKeyState(VK_MENU)&0xff00)!=0);
		
		/*
		if (msgid == MSGTYPE_KEYDOWN)
		{
		ScriptVM::InstPtr()->SetGlobalValue(arg1, "keydown");
		}
		else
		{
		ScriptVM::InstPtr()->SetGlobalValue(arg1, "keyup");
		}*/
		
		ProcessAccelerator(accel, event.msg==GIE_KEYDOWN);
	}

	return 0;
}

void FrameManager::GetFramePoint(FRAMEPOINT_T pt, int &x, int &y)
{
	switch(pt)
	{
	case FP_TOPLEFT:
		{
			x = 0;
			y = 0;
			break;
		}

	case FP_TOPRIGHT:
		{
			x = m_iScreenWidth;
			y = 0;
			break;
		}

	case FP_BOTTOMLEFT:
		{
			x = 0;
			y = m_iScreenHeight;
			break;
		}

	case FP_BOTTOMRIGHT:
		{
			x = m_iScreenWidth;
			y = m_iScreenHeight;
			break;
		}

	case FP_TOP:
		{
			x = m_iScreenWidth/2;
			y = 0;
			break;
		}

	case FP_BOTTOM:
		{
			x = m_iScreenWidth/2;
			y = m_iScreenHeight;
			break;
		}
		
	case FP_LEFT:
		{
			x = 0;
			y = m_iScreenHeight/2;
			break;
		}
		
	case FP_RIGHT:
		{
			x = m_iScreenWidth;
			y = m_iScreenHeight/2;
			break;
		}
		
	case FP_CENTER:
		{
			x = m_iScreenWidth/2;
			y = m_iScreenHeight/2;
			break;
		}

	default:
		{
			assert(0 && "FRAMEPOINT_T type is not correct!");
			break;
		}
	}
}

void FrameManager::SendEvent(const char *ClientEvent)
{ 
	if(ClientEvent)
	{
		stdext::hash_map<std::string, stEventFrameArray>::iterator iter = EventMap.find(ClientEvent);
		if(iter != EventMap.end())
		{
			for (unsigned int i=0; i<iter->second.EventFrameArray.size(); ++i)
			{
				iter->second.EventFrameArray[i]->CallScript(SC_ONEVENT, "s", ClientEvent);
			}
		}else
		{
			//assert(0 && "no UI Resource can accept this event");
		}
	}
	else
	{
		assert(0);
	}
}

void FrameManager::InitFaceTexture(int width, int height)
{
	m_pCodeMap = new RFaceCodeMap;
	m_pCodeMap->Init(m_CodeMapName.c_str());

	m_hTexRes = g_pDisplay->CreateTexture(m_PopofaceTexName.c_str(), BLEND_ALPHABLEND);

	m_nFaceTexWidth		= width;
	m_nFaceTexHeight	= height;
	m_fUUnit			= 1.0f / m_nFaceTexWidth;
	m_fVUnit			= 1.0f / m_nFaceTexHeight;
	m_nFaceNumX			= m_nFaceTexWidth / m_nFaceWidth;
	m_nFaceNumY			= m_nFaceTexHeight / m_nFaceHeight;
	m_fUWidth			= m_nFaceWidth / (float)m_nFaceTexWidth;
	m_fVWidth			= m_nFaceHeight/ (float)m_nFaceTexHeight;
}

void FrameManager::InitPictureTexture( int width, int height )
{
	m_pPictureBuffer->Init();
	m_pPictureBuffer->SetTexRes( g_pDisplay->CreateTexture( m_pPictureBuffer->GetResourcePath(), BLEND_ALPHABLEND ) );
	m_pPictureBuffer->SetTexSize( width, height );
	//m_pPictureBuffer->SetUVUnit( 1.0f / width, 1.0f / height );
}

Frame *FrameManager::GetUIClientFrame()
{
	return static_cast<Frame *>(FindLayoutFrame(UI_ROOT_NAME.c_str()));
}

void FrameManager::setCursor(const char *name)
{
	if( UIIsInDragState() )
	{
		return;
	}
	if( name == NULL )
	{
		return;
	}
	//比较level 
	if( strcmp(name,m_pCurCursor->getCursor())==0 )
	{
		return;
	}
	{
		m_pCurCursor->setCursor(name);
	}
}

void FrameManager::setUiCursor(const char* name)
{
	if( GetCurrentCursorLevel() == eCursorNoMoveChange )
	{
		return;
	}
	setCursor(name);
}

void* FrameManager::getImeContext()
{
	if( m_hIme == NULL )
	{
		m_hIme = (void*)ImmGetContext((HWND)m_wndhandle);
	}
	return m_hIme;
}

void* FrameManager::getFontInfoByFontType( const char* pszFontName )
{
	for( int i=0, size = (int)m_HMAYFONT.size(); i < size; ++i )
	{
		const UIFont& uiFontInfo =m_HMAYFONT[i];
		if ( uiFontInfo.name == pszFontName )
		{
			return uiFontInfo.h;
		}
	}

	return NULL;
}

UIObject* FrameManager::getTemplateObject( std::string template_name )
{
	std::map<std::string, UIObject *>::iterator it = g_pFrameMgr->m_TemplateTable.find(template_name);
	if( it == g_pFrameMgr->m_TemplateTable.end() )
	{
		return NULL;
	}
	return it->second;
}

void FrameManager::SetBeginDragFrame(Frame *frame /*= NULL*/)
{
	m_lastBeginDragFrame = frame;
	m_lastBeginDragTime = frame ? timeGetTime() : 0;
	m_lastBeginDragX = ::GetCursorPosX();
	m_lastBeginDragY = ::GetCursorPosY();
}


const UIFont* FrameManager::getUIFontByName( const char* pszFontName )
{
	for ( std::vector<UIFont>::iterator iter = m_HMAYFONT.begin(), end = m_HMAYFONT.end(); iter != end; ++iter )
	{
		const UIFont& oneUIFontConfig = *iter;
		if ( oneUIFontConfig.name == pszFontName )
		{
			return &oneUIFontConfig;
		}
	}

	return NULL;
}

const UIFont& FrameManager::getUIFontByIndex( int nIndex )
{
	assert( nIndex >=0 && nIndex < m_HMAYFONT.size() );
	return m_HMAYFONT[nIndex];
}

int FrameManager::getUIFontIndexByName( const char* pszFontName )
{
	for ( int i = 0, size = m_HMAYFONT.size(); i < size; ++i )
	{
		const UIFont& oneUIFontConfig = m_HMAYFONT[i];
		if ( oneUIFontConfig.name == pszFontName )
		{
			return i;
		}
	}

	return -1;
}

void FrameManager::AddReCalFrame( LayoutFrame* pFrame )
{
	m_ReCalPosFrames.push_back( pFrame );
}

void FrameManager::EnableAccelerator(bool enable)
{
	m_enableAccel = enable;
}
