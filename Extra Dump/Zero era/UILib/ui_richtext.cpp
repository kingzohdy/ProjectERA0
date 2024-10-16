
#include <windows.h>
#include <mmsystem.h>
#include "ui_richtext.h"
#include "fstream"
#include "OgreLog.h"
#include <math.h>
#include "RichTextBuilder.h"

#include "OgreResourceManager.h"
#include "OgreDataStream.h"
#include "OgreScriptLuaVM.h"

using namespace Ogre;
using namespace std;

unsigned int RichText::m_nInstanceCount = 0;

RFaceCodeMap::RFaceCodeMap()
{
	for(int i=0; i<1000; i++)
	{
		m_faceData[i].m_nCode = -1;
	}
}

static char* parse2Int( char *szBuffer, int &nData )
{
	assert( szBuffer != NULL );
	char* szSeq = strchr( szBuffer, ' ' );
	if( szSeq )
	{
		*szSeq = 0;
		nData = atoi( szBuffer );
		return szSeq + 1;
	}else{
		nData = atoi( szBuffer );
		return NULL;
	}
}

bool RFaceCodeMap::Init( const char* szFileName )
{
	Ogre::DataStream *fp = Ogre::ResourceManager::getSingleton().openFileStream( szFileName, true);
	if( fp == NULL )
	{
		return false;
	}
	char szBuffer[OGRE_STREAM_TEMP_SIZE];
	while( !fp->eof() )
	{
		if( fp->readLine( szBuffer, sizeof( szBuffer ) ) == 0 )
		{
			continue;
		}

		std::string strBuffer( szBuffer, sizeof(szBuffer));
		size_t nPos	= 0;
		int data[4]	= {0,0,0,0};
		for( int i = 0; i< sizeof(data)/sizeof(int); i++ )
		{
			std::string sds =  strBuffer.substr( nPos,strBuffer.find( ' ', nPos)-nPos);
			nPos = strBuffer.find( ' ', nPos ) + 1;
			data[i] = atoi( sds.c_str() );
		}

		FaceData facedata;
		facedata.m_nCode			= data[0];
		facedata.m_nStartIdx		= data[1];
		facedata.m_nFrameNum		= data[2];
		facedata.m_nFrameInterval	= data[3];

		if(facedata.m_nCode > 0) 
		{
			m_faceData[facedata.m_nCode] = facedata;
		}
	}
	delete fp;
	return true;
}

FaceData* RFaceCodeMap::GetFaceData(int nCode)
{
	if( nCode < 0 || nCode >= 1000 )
	{
		return NULL;
	}

	if( m_faceData[nCode].m_nCode == -1 )
	{
		return NULL;
	}

	return &m_faceData[nCode];
}

//*******************************************************************************


//-----------------------------------------------------------------------------
// RichEditText control
//-----------------------------------------------------------------------------
IMPLEMENT_UIOBJECT_CLONE(RichText)
void RichText::CopyMembers(RichText *pdest)
{
	if ( pdest != NULL )
	{
		Frame::CopyMembers(pdest);

		pdest->m_nLineInterval	= m_nLineInterval;
		pdest->m_iMaxLines		= m_iMaxLines;
		pdest->m_iFontIndex		= m_iFontIndex;
		pdest->m_hLinkFont		= m_hLinkFont;
		pdest->m_FontStyle		= m_FontStyle;
		pdest->m_bAutoExtend	= m_bAutoExtend;
		pdest->m_bCenterLine	= m_bCenterLine;
	}
}

RichText::RichText() : m_newLineStartPoint(0,0),m_FontStyle( FONTSYTLE_SHADOW ),m_bAutoExtend(false)
{
	m_nLineInterval = 0;
	m_fAlpha		= 1.0f;
	m_iMaxLines		= 100;
	m_nInstanceCount++;

	m_rect				= RectFloat(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLinkHightLineObj = NULL;
	m_viewStartPoint.SetPos(0.0f, 0.0f);
	m_LinkTextColor.set( 255, 80, 80, 255 );
	m_shadowColor.set( 0, 0, 0, 255 );

	m_bCenterLine		= false;
	m_fFaceXInterval	= 0;
	m_fFaceYInterval	= 0;
}

RichText::~RichText()
{
	RichTextLineList::iterator oneLineIter;
	for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
	{
		delete(*oneLineIter);
	}
	m_listLines.clear();
	g_pDisplay->ReleaseTrueTypeFont(m_hLinkFont);
}

void RichText::AddRenderText(const char *szText, const ColorQuad& color)
{
	if (szText)
	{
		RRichTextBuilder builder;
		builder.BuildText(szText, this, color);
	}
}

void RichText::SetRenderText(const char *szText, const ColorQuad& color)
{
	if (szText)
	{
		Clear();
		clearHistory();
		UpdateSelf(0);
		AddRenderText(szText, color);
		if ( m_TextHistory.size() >= m_iMaxLines )
		{
			m_TextHistory.pop_front();
		}

		tagTextHistory textHistory;
		textHistory.m_strTextHistory	= szText;
		textHistory.m_lineColor			= color;
		m_TextHistory.push_back(textHistory);
	}
}

void RichText::Resize()
{
	Clear();
	for (size_t i=0; i<m_TextHistory.size(); i++)
	{
		const tagTextHistory& oneTextHistory = m_TextHistory[i];
		AddRenderText( oneTextHistory.m_strTextHistory.c_str(), oneTextHistory.m_lineColor );
	}
}

//void RichText::CalAbsRectSelf(size_t frame)
//{
//	LayoutFrame::CalAbsRectSelf( frame );
//	m_rect = m_AbsRect;
//}

void RichText::CalAbsRectSelf(size_t frame)
{
	LayoutFrame::CalAbsRectSelf( frame );
	OnGameFontUpdated();
	// 若是一次都还没是使用
	// UpdateSelf在一次都没使用过的情况下，不管它是否显示，都会设置标准显示范围
	if ( !m_rect.isEmpty() )
	{
		SetPosition( m_AbsRect.m_Left, m_AbsRect.m_Top );
	}
}

void RichText::SetPosition(float fX, float fY)
{
	float fW = m_rect.getWidth();
	float fH = m_rect.getHeight();
	UpdateRichRect( fX, fY, fX + fW, fY + fH );
}

void RichText::FillDrawItems()
{
	if ( !m_bFrameDraw )
	{
		return;
	}

	if (m_bShow)
	{
		if (m_pParent)
		{
			m_iDrawLevel = m_pParent->m_iDrawLevel + 1;
		}

		g_pFrameMgr->AddDrawItems(static_cast<LayoutFrame *>(this));

		Frame::FillChildren();
	}
}

void RichText::UpdateSelf(float deltatime)
{
	if (m_bShow)
	{
		Frame::UpdateSelf(deltatime);
		UpdateRichRect( m_AbsRect );
	}
	else if ( m_rect.isEmpty() )
	{
		UpdateRichRect( m_AbsRect );
	}
}

void RichText::DrawNormalText( const RichTextText* pText, const Ogre::RectFloat& rectLine, unsigned int dwCurTime )
{
	// 文字左下角相对于行的左下角的坐标
	static RFPoint ptViewStartPoint;
	static RectFloat rect;
	ptViewStartPoint.SetPos( pText->m_rect.m_Left, pText->m_rect.m_Top );
	
	initViewStartPoint( ptViewStartPoint, rectLine );
	if ( !canShowOneNormalText( ptViewStartPoint, pText ) )
	{
		return;
	}

	// 对显示范围做裁减
	float fHeight	= pText->m_rect.getHeight();
	float fWidth	= pText->m_rect.getWidth();
	
	RFPoint ptViewOffsetPoint;
	calculateNormalTextViewport( ptViewStartPoint, ptViewOffsetPoint, fWidth, fHeight );

	rect.set( ptViewStartPoint.m_x, ptViewStartPoint.m_y, ptViewStartPoint.m_x + fWidth, ptViewStartPoint.m_y + fHeight + 1 );
	FinishDrawNormalText( pText, ptViewOffsetPoint, rect, dwCurTime );
}

void RichText::DrawFace( RichTextFace* pFace, const Ogre::RectFloat& rectLine, unsigned int dwCurTime, 
							int& nFaceNum, Ogre::DrawRect* faces )
{
	static RFPoint ptViewStartPoint;
	static RFPoint ptUVStartPoint;
	static RFSize viewSize;
	static RFSize uvSize;

	ptViewStartPoint.SetPos( pFace->m_rect.m_Left, pFace->m_rect.m_Top );
	initViewStartPoint( ptViewStartPoint, rectLine );
	
	if ( !canShowOneFace( ptViewStartPoint ) )
	{
		return;
	}

	initOneFaceUVStartPoint( ptUVStartPoint, pFace, dwCurTime );

	// 对显示范围做裁减
	viewSize.SetSize( g_pFrameMgr->m_nFaceWidth*g_pFrameMgr->GetScreenScaleY(), 
						g_pFrameMgr->m_nFaceHeight*g_pFrameMgr->GetScreenScaleY() );
	uvSize.SetSize( g_pFrameMgr->m_fUWidth, g_pFrameMgr->m_fVWidth );
	calculateOneFaceViewportAndUVRect( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize );
	
	FinishDrawFace( pFace, ptViewStartPoint, ptUVStartPoint, viewSize, uvSize, nFaceNum, faces );
}

void RichText::DrawPicture( RichTextPicture* pPicture, const Ogre::RectFloat& rectLine, unsigned int dwCurTime, 
						   int& nPictureNum, Ogre::DrawRect* pictures )
{
	// TODO: 美术换资源的要求，同一个动画中的图片宽度必须保持一致
	// TODO: 为了保证动画能够正常播放,有3种方法： 1. 一行有多少个美术资源必须有固定的数量；
	//		 2.要求美术对每一个图片都填写UV; 3.要求一个完整动画的所有图片必须位于同一行;
	
	// NOW: 目前先实现一个完整动画所需的所有图片位于同一行
	static RFPoint ptViewStartPoint;
	static RFPoint ptUVStartPoint;
	static RFSize viewSize;
	static RFSize uvSize;

	RPictureCodeMap*	pPictureMapBuffer	= g_pFrameMgr->getPictureCodeMap();
	PictureData*		pPictureData		= pPictureMapBuffer->GetPictureData( pPicture->m_nCode );
	
	ptViewStartPoint.SetPos( pPicture->m_rect.m_Left, pPicture->m_rect.m_Top );
	initViewStartPoint( ptViewStartPoint, rectLine );
	if ( !canShowOnePicture( ptViewStartPoint, pPictureData ) )
	{
		return;
	}
	
	initOnePictureUVStartPoint( ptUVStartPoint, pPictureData, pPicture, dwCurTime );
	
	viewSize.SetSize( pPictureData->m_nTexWidth  * g_pFrameMgr->GetScreenScaleY(), 
						pPictureData->m_nTexHeight * g_pFrameMgr->GetScreenScaleY() );

	uvSize.SetSize( pPictureData->m_nTexWidth, pPictureData->m_nTexHeight );
	calculateOnePictureViewportAndUVRect( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize, pPictureData, pPictureMapBuffer );
	FinishDrawPicture( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize, nPictureNum, pictures );
}


void RichText::Draw()
{
	if ( m_bTile )
	{
		DrawBackDrop();
	}
        
	static DrawRect	faces[m_sMaxDrawRectNum];	
	int			nFaceNum = 0;

	static DrawRect	pictures[m_sMaxDrawRectNum];
	int			nPictureNum = 0;

	DWORD		dwCurTime	= timeGetTime();

	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(), lineEnd = m_listLines.end(); 
			oneLineIter != lineEnd; ++oneLineIter )
	{
		RichTextLine* pLine			= *oneLineIter;
		const RectFloat& rectLine	= pLine->m_rect;
		
		// 由于RichText不支持显示半行内容，故只要某一行的顶部不在显示范围，则不显示这行
		if ( rectLine.m_Top < m_viewStartPoint.m_y )
		{
			// 不在显示范围
			continue;
		}
		
		// 若某一行的底部不在视口偏移Y值与整个视口高度的范围内，则不显示这行
		if ( ( rectLine.m_Bottom > ( m_viewStartPoint.m_y + m_rect.getHeight() ) ) )
		{
			// 超过显示范围
			break;
		}

		for ( RichTextObjectList::iterator iObj = pLine->m_listObjects.begin(), objEnd = pLine->m_listObjects.end(); 
				iObj != objEnd; ++iObj )
		{
			RichTextObject* pObj = *iObj;

			if ( pObj->m_type == RTOT_TEXT )
			{
				DrawNormalText( ( RichTextText* )pObj, rectLine, dwCurTime );
			}
			else if ( pObj->m_type == RTOT_FACE )
			{
				DrawFace( (RichTextFace*)pObj, rectLine, dwCurTime, nFaceNum, faces );
			}
			else if ( pObj->m_type == RTOT_PICTURE )
			{
				DrawPicture( ( RichTextPicture* )pObj, rectLine, dwCurTime, nPictureNum, pictures );
			}
		}
	}

	if ( nFaceNum != 0 )
	{
		RenderFaces( faces, nFaceNum );
	}

	if ( nPictureNum != 0 )
	{
		RenderPictures( pictures, nPictureNum );
	}
}

TiXmlElement *RichText::Save(TiXmlElement *pElement)
{
	TiXmlElement *pRichText = Frame::Save(pElement);

	//lineInterval
	if (m_nLineInterval != 0)
	{
		pRichText->SetAttribute("lineInterval", m_nLineInterval);
	}

	//maxlines
	if (m_iMaxLines != 100)
	{
		pRichText->SetAttribute("maxlines", m_iMaxLines);
	}

	return pRichText;
}

int RichText::OnInputMessage(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_CHAR:
		return 1;

	case GIE_RBTNDOWN:
	case GIE_LBTNDOWN:
		return OnClick(event);

	case GIE_MOUSEMOVE:
		{
			if( m_bShow )
			{
				OnMouseMoveInLink(event);
				return 0;
			}
		}
		break;

	case GIE_MOUSEIN:
		{
			if (m_bShow)
			{
				//m_bShowTooltip = true;
				if (hasScriptsEvent(SC_ONENTER))
				{
					CallScript( SC_ONENTER, "is", 0, "" );
				}
				m_pLinkHightLineObj = NULL;
			}
			return 0;
		}

	case GIE_MOUSEOUT:
		{
			if (m_bShow)
			{
				if (hasScriptsEvent(SC_ONLEAVE))
				{
					CallScript(SC_ONLEAVE);
				}
				m_pLinkHightLineObj = NULL;
				if( GetCurrentCursorLevel() == eCursorOnlyOnUi )
				{
					g_pFrameMgr->setCursor("normal");
				}
			}
			return 0;
		}

	case GIE_LBTNDBLCLK:
		{
			if (m_bShow)
			{
				if (!hasScriptsEvent(SC_ONDOUBLECLICK))
				{
					Ogre::InputEvent newEvent = event;
					newEvent.msg = GIE_LBTNDOWN;
					return OnClick(newEvent);
				}
				else
				{
					CallScript(SC_ONDOUBLECLICK, "s", "LDoubleClick");
					return 0;
				}
			}
		}
		break;

	default:
		{
			return Frame::OnInputMessage(event);
		}
	}

	return 1;
}

//查找鼠标点击的位置所有的控件
void RichText::FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *> &vFrameTemp )	
{
	// 若RichText透明且鼠标位于超链接上面才处理，则不穿透消息到下一层
	if ( !m_bShow )
	{
		return;
	}

	if ( m_bInputTransparent && isMouseInLink( mousex, mousey ) )
	{
		vFrameTemp.push_back(this);
	}
	else
	{
		return Frame::FindFrameOnPoint( mousex, mousey, vFrameTemp );
	}
}

void RichText::RenderFaces(DrawRect* pPoly, int nNum)
{
	g_pDisplay->BeginDraw(g_pFrameMgr->m_hTexRes);

	for( int i=0; i < nNum; ++i )
	{
		g_pDisplay->StretchRect(
			pPoly[i].rect.m_Left, 
			pPoly[i].rect.m_Top, 
			pPoly[i].rect.getWidth(), 
			pPoly[i].rect.getHeight(), 
			pPoly[i].color.c, 
			false,
			(int)(pPoly[i].rectUV.m_Left * g_pFrameMgr->m_nFaceTexWidth), 
			(int)(pPoly[i].rectUV.m_Top * g_pFrameMgr->m_nFaceTexHeight), 
			(int)(pPoly[i].rectUV.getWidth() * g_pFrameMgr->m_nFaceTexWidth), 
			(int)(pPoly[i].rectUV.getHeight() * g_pFrameMgr->m_nFaceTexHeight)
			);
	}
	g_pDisplay->EndDraw();
}

void RichText::RenderPictures( Ogre::DrawRect* pictures, int nNum )
{
	RPictureCodeMap* pPictureMapBuffer = g_pFrameMgr->getPictureCodeMap();
	g_pDisplay->BeginDraw( pPictureMapBuffer->GetTexRes() );
	for ( int i = 0; i < nNum; ++i )
	{
		g_pDisplay->StretchRect(
			pictures[i].rect.m_Left, 
			pictures[i].rect.m_Top, 
			pictures[i].rect.getWidth(), 
			pictures[i].rect.getHeight(), 
			pictures[i].color.c, 
			false,
			(int)( pictures[i].rectUV.m_Left ), 
			(int)( pictures[i].rectUV.m_Top ), 
			(int)( pictures[i].rectUV.getWidth() ), 
			(int)( pictures[i].rectUV.getHeight() )
			);
	}
	g_pDisplay->EndDraw();
}

void RichText::ReplacePartialText( const char* pszDestText, const char* pszNewText )
{
	
	int nDestLen	= strlen( pszDestText );
	int nNewLen		= strlen( pszNewText );
	for ( std::deque<tagTextHistory>::iterator iter = m_TextHistory.begin(), end = m_TextHistory.end();
			iter != end; ++iter )
	{
		tagTextHistory& oneHistoryText	= *iter;
		std::string& strOneText			= oneHistoryText.m_strTextHistory;

		std::string::size_type nNewFindStartIndex			= 0;
		std::string::size_type nOneTargetStartIndex	= string::npos;
		while ( true )
		{
			nOneTargetStartIndex = strOneText.find( pszDestText, nNewFindStartIndex );
			if ( nOneTargetStartIndex == string::npos )
			{
				break;
			}

			strOneText.replace( nOneTargetStartIndex, nDestLen, pszNewText );
			nNewFindStartIndex = nOneTargetStartIndex + nNewLen;
		}
	}

	Resize();
}

void RichText::setTwoFaceInterval( float fXInterval, float fYInterval )
{
	m_fFaceXInterval	= fXInterval;
	m_fFaceYInterval	= fYInterval;
}

tagRect_ToLua& RichText::getLinkTextRect( const char* pszText )
{
	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++ )
	{
		RichTextLine* pLine			= *oneLineIter;
		const RectFloat& rectLine	= pLine->m_rect;

		for ( RichTextObjectList::iterator iTextObject = pLine->m_listObjects.begin(); iTextObject != pLine->m_listObjects.end(); iTextObject++ )
		{
			RichTextObject*		pObject		= *iTextObject;
			const RectFloat&	rectObject	= pObject->m_rect;
			if ( pObject->m_type != RTOT_TEXT )
			{
				continue;
			}
			
			RichTextText* pOneTextObject = static_cast<RichTextText *>(pObject);
			if ( !pOneTextObject->m_bLinkItem )
			{
				continue;
			}
			
			if ( strcmp( pszText, pOneTextObject->m_szText ) == 0 )
			{
				RFPoint pt( rectObject.m_Left, rectObject.m_Top );
				initViewStartPoint( pt, rectLine );

				m_linkTextRect.m_nLeft		= pt.m_x;
				m_linkTextRect.m_nRight		= pt.m_x + rectObject.getWidth();
				m_linkTextRect.m_nTop		= pt.m_y;
				m_linkTextRect.m_nBottom	= pt.m_y + rectObject.getHeight();
				return m_linkTextRect; 
			}
		}
	}

	m_linkTextRect.empty();
	return m_linkTextRect;
}

int RichText::OnClickOneRichObject(const Ogre::InputEvent &event, const RichTextObject* pOneRichObject )
{
	if ( pOneRichObject->m_type == RTOT_TEXT && static_cast<const RichTextText *>(pOneRichObject)->m_bLinkItem )
	{
		const RichTextText* pOneTextObject	= static_cast<const RichTextText*>(pOneRichObject);
		const char* pszText					= pOneTextObject->m_szText;
		const char* pszItemLinkText			= pOneTextObject->m_strLinkItemHref.c_str();
		if( event.msg == GIE_LBTNDOWN )
		{
			CallScript( SC_ONCLICK, "sss", pszText, pszItemLinkText, "LeftButton" );
		}
		else if ( event.msg == GIE_RBTNDOWN )
		{
			CallScript( SC_ONCLICK, "sss", pszText, pszItemLinkText, "RightButton" );
		}	
		return 0;
	}
	else if ( pOneRichObject->m_type == RTOT_FACE )
	{
		const RichTextFace* pOneFaceObject = static_cast<const RichTextFace*>(pOneRichObject);
		CallScript( SC_ONCLICK, "i", pOneFaceObject->m_nCode );
		return 0;
	}
	else if ( pOneRichObject->m_type == RTOT_PICTURE )
	{
		const RichTextPicture* pOnePictureObject = static_cast<const RichTextPicture*>(pOneRichObject);
		const char* pszAnimText = pOnePictureObject->m_strAnimText.c_str();
		if( event.msg == GIE_LBTNDOWN )
		{
			CallScript( SC_ONCLICK, "sss", pszAnimText, "LeftButton", NULL );
		}
		else if ( event.msg == GIE_RBTNDOWN )
		{
			CallScript( SC_ONCLICK, "sss", pszAnimText, "RightButton", NULL );
		}
		return 0;
	}
	else
	{
		if ( event.msg == GIE_LBTNDOWN  )
		{
			CallScript(SC_ONCLICK, "ss", "LeftButton","");
		}
		else if ( event.msg == GIE_RBTNDOWN )
		{
			CallScript(SC_ONCLICK, "ss", "RightButton","");
		}

		if ( m_bInputTransparent ) 
		{
			return 1;
		}

		return 0;
	}
}

int RichText::OnClick(const Ogre::InputEvent &event)
{
	int x = event.mouse.x;
	int y = event.mouse.y;

	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(), lineEndIter = m_listLines.end(); 
			oneLineIter != lineEndIter; ++oneLineIter )
	{
		RichTextLine*		pOneLine = *oneLineIter;
		const RectFloat&	rectLine = pOneLine->m_rect;
		
		if ( !isMouseOnOneLine( rectLine, y ) )
		{
			continue;
		}
		
		if ( !isPointInFrame( GetName() ) )
		{
			continue;
		}

		for ( RichTextObjectList::iterator oneObjectIter = pOneLine->m_listObjects.begin(), 
				objectEndIter = pOneLine->m_listObjects.end(); 
				oneObjectIter != objectEndIter; ++oneObjectIter )
		{
			RichTextObject*		pOneRichObject		= *oneObjectIter;
			const RectFloat&	oneRichObjectRect	= pOneRichObject->m_rect;
			int nLeft	= m_AbsRect.m_Left + oneRichObjectRect.m_Left;
			int nRight	= m_AbsRect.m_Left + oneRichObjectRect.m_Right;

			if( m_bCenterLine )
			{
				nLeft	+= pOneLine->m_rect.m_Left;
				nRight	+= pOneLine->m_rect.m_Left;
			}

			if ( nLeft >= x || nRight < x )
			{
				continue;
			}

			// 点在该对象上
			if ( hasScriptsEvent(SC_ONCLICK) )
			{
				return OnClickOneRichObject( event, pOneRichObject );
			}

			if ( m_bInputTransparent ) 
			{
				return 1;
			}
			return 0;
		}

		if ( m_bInputTransparent ) 
		{
			return 1;
		}

		return 0;
	}

	if ( m_bInputTransparent )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool RichText::isMouseInLink( int x, int y )
{
	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(), end = m_listLines.end(); oneLineIter != end; ++oneLineIter )
	{
		RichTextLine* pOneLine = *oneLineIter;
		RectFloat rectLine = pOneLine->m_rect;
		if ( !isMouseOnOneLine( rectLine, y ) )
		{
			continue;
		}

		for ( RichTextObjectList::iterator iTextObject = pOneLine->m_listObjects.begin(),
			end = pOneLine->m_listObjects.end(); iTextObject != end; ++iTextObject )
		{
			RichTextObject* pObject = *iTextObject;
			const RectFloat& rectObject	= pObject->m_rect;
			if ( ( m_AbsRect.m_Left + rectObject.m_Left >= x ) ||
				( m_AbsRect.m_Left + rectObject.m_Right < x ) )
			{
				continue;
			}

			if ( ( pObject->m_type == RTOT_TEXT && static_cast<RichTextText *>( pObject)->m_bLinkItem ) ||
				pObject->m_type == RTOT_FACE || pObject->m_type == RTOT_PICTURE )
			{
				return true;
			}
		}
	}

	return false;
}

void RichText::OnMouseMoveInLink(const Ogre::InputEvent &event)
{
	int x = event.mouse.x;
	int y = event.mouse.y;

	m_pLinkHightLineObj = NULL;

	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++ )
	{
		RichTextLine* pOneLine = *oneLineIter;
		const RectFloat& rectLine = pOneLine->m_rect;

		// 点在该行
		if( ( m_AbsRect.m_Top + rectLine.m_Top >= m_viewStartPoint.m_y + y ) ||
			( m_AbsRect.m_Top + rectLine.m_Bottom < m_viewStartPoint.m_y + y ) || 
			!isPointInFrame( this->GetName() ) )
		{
			continue;	
		}
		
		for( RichTextObjectList::iterator iTextObject = pOneLine->m_listObjects.begin(); iTextObject != pOneLine->m_listObjects.end(); iTextObject++)
		{
			RichTextObject* pObject		= *iTextObject;
			const RectFloat& rectObject = pObject->m_rect;
			int nLeft	= m_AbsRect.m_Left + rectObject.m_Left;
			int nRight	= m_AbsRect.m_Left + rectObject.m_Right;
			if( m_bCenterLine )
			{
				nLeft	+= pOneLine->m_rect.m_Left;
				nRight	+= pOneLine->m_rect.m_Left;
			}

			// 点在该对象上
			if ( ( nLeft >= m_viewStartPoint.m_x + x ) ||
				( nRight < m_viewStartPoint.m_x + x ) )
			{
				continue;
			}
			
			if ( pObject->m_type == RTOT_TEXT && static_cast<RichTextText *>( pObject)->m_bLinkItem )
			{
				m_pLinkHightLineObj = pObject;
				if( GetCurrentCursorLevel() != eCursorNoMoveChange )
				{
					g_pFrameMgr->setCursor("link");
				}
				return;
			}else if( pObject->m_type == RTOT_FACE ){
				m_pLinkHightLineObj = pObject;
				CallScript(SC_ONENTER, "is", static_cast<RichTextFace *>(pObject)->m_nCode,"RTOT_FACE_ONENTER" );
				return;
			}
		}
	}

	if ( GetCurrentCursorLevel() == eCursorOnlyOnUi )
	{
		g_pFrameMgr->setCursor("normal");
	}
}

int RichText::getFaceWidth()
{
	return g_pFrameMgr->m_nFaceWidth;
}

int RichText::getFaceHeight()
{
	return g_pFrameMgr->m_nFaceHeight;
}

void RichText::SetFaceTexture(const char *path)
{
	if ( path == NULL || path[0] == '\0' || m_faceBorderTex.m_strTexturePath == path )
	{
		return;
	}

	m_faceBorderTex.m_strTexturePath	= path;
	Ogre::HUIRES hPreRes = m_faceBorderTex.m_hTex;
	m_faceBorderTex.m_hTex = g_pDisplay->CreateTexture( path, Ogre::BLEND_ALPHABLEND, 
														&m_faceBorderTex.m_nRelWidth, &m_faceBorderTex.m_nRealHeight );
	g_pDisplay->ReleaseUIRes( hPreRes );
}

void RichText::SetFaceTexUV(int x, int y, int width, int height)
{
	m_faceBorderTex.m_nTexX		= x;
	m_faceBorderTex.m_nTexY		= y;
	m_faceBorderTex.m_nTexWidth	= width;
	m_faceBorderTex.m_nTexHeight= height;
}

void RichText::SetFaceTexRelUV( float x, float y )
{
	m_faceBorderTex.m_nTexX = 0;
	m_faceBorderTex.m_nTexX = 0;

	m_faceBorderTex.m_nTexWidth		= FloatToInt( x * m_faceBorderTex.m_nRelWidth );
	m_faceBorderTex.m_nTexHeight	= FloatToInt( y * m_faceBorderTex.m_nRealHeight );
}

void RichText::resizeRect( int Width, int Height )
{
	m_AbsRect.m_Right	= m_AbsRect.m_Left + Width;
	m_AbsRect.m_Top		= m_AbsRect.m_Bottom - Height;
	UpdateRichRect( m_AbsRect );
}

void RichText::resizeRichWidth( int Width )
{
	m_AbsRect.m_Right = m_AbsRect.m_Left + Width;
	UpdateRichRect( m_AbsRect );
}

void RichText::resizeRichHeight( int Height )
{
	m_AbsRect.m_Top	= m_AbsRect.m_Bottom - Height;
	UpdateRichRect( m_AbsRect );
}

// 重新设置文本
void RichText::resizeText( int Width, int Height )
{
	m_AbsRect.m_Right	= m_AbsRect.m_Left + Width;
	m_AbsRect.m_Top		= m_AbsRect.m_Bottom - Height;
	
	UpdateRichRect( m_AbsRect );
	Resize();
}

void RichText::AddText(const char *szText, int r, int g, int b)
{
	if (szText)
	{
		if ( m_TextHistory.size() >= m_iMaxLines )
		{
			m_TextHistory.pop_front();
		}
		tagTextHistory textHistory;
		textHistory.m_strTextHistory	= szText;
		textHistory.m_lineColor			= ColorQuad(r, g, b);
		m_TextHistory.push_back(textHistory);
		UpdateSelf(0);
		AddRenderText(szText, ColorQuad(r, g, b));
	}
}

void RichText::SetText(const char *szText, int r, int g, int b)
{
	if (szText)
	{
		SetRenderText(szText, ColorQuad(r, g, b));
	}
}

void RichText::SetAutoExtend( bool bAutoExtend )
{
	m_bAutoExtend = bAutoExtend;
}

void RichText::SetLinkTextColor(int r, int g, int b)
{
	m_LinkTextColor = ColorQuad(r, g, b);
}

// 设置字体描边颜色
void RichText::SetShadowColor(int r, int g, int b)
{
	m_shadowColor = ColorQuad( r, g, b );
}

// 设置描边阴影
void RichText::SetFontType( int nType)
{
	m_FontStyle = nType;
}

void RichText::Clear()
{
	RichTextLineList::iterator oneLineIter;
	for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
	{
		delete(*oneLineIter);
	}

	m_listLines.clear();
	m_newLineStartPoint.SetPos(0,0);
	m_viewStartPoint.SetPos(0.0f, 0.0f);
}

void RichText::clearHistory()
{
	m_TextHistory.clear();
}

void RichText::SetDispPos(float fY)
{
	m_viewStartPoint.m_y = fY;
}

void RichText::IncrDispPos( float fY )
{
	m_viewStartPoint.m_y += fY;
}

float RichText::GetDispPos()
{
	return m_viewStartPoint.m_y;
}

void RichText::SetDispPosX(float fX)
{
	m_viewStartPoint.m_x = fX;
}

float RichText::GetDispPosX()
{
	return m_viewStartPoint.m_x;
}

float RichText::GetStartDispPos()
{
	RichTextLineList::iterator oneLineIter;
	for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
	{
		RichTextLine* pOneLine = *oneLineIter;
		RectFloat rectLine = pOneLine->m_rect;

		if (rectLine.m_Top < m_viewStartPoint.m_y)
		{
			return rectLine.m_Top;
		}
		else
		{
			return m_viewStartPoint.m_y;
		}
	}

	return m_viewStartPoint.m_y;
}

float RichText::GetEndDispPos()
{
	if( !m_listLines.empty() )
	{
		RichTextLine* pLine = m_listLines.back();
		RectFloat rectLine = pLine->m_rect;
		if (rectLine.m_Bottom - m_rect.getHeight() > m_viewStartPoint.m_y)
		{
			/*int nHeight = m_rect.getHeight();
			int nResult = rectLine.m_Bottom - m_rect.getHeight();*/
			return rectLine.m_Bottom - m_rect.getHeight();
		}else
		{
			return m_viewStartPoint.m_y;
		}
	}
	return m_viewStartPoint.m_y;
}

// 向上滚动一行
void RichText::ScrollUp()
{
	if (m_viewStartPoint.m_y > GetStartDispPos())
	{
		float scrollPos = 0.0f;
		RichTextLineList::iterator oneLineIter;
		for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
		{
			RichTextLine* pLine = *oneLineIter;
			RectFloat rectLine = pLine->m_rect;

			if ((rectLine.m_Top >= m_viewStartPoint.m_y))
			{
				m_viewStartPoint.m_y = scrollPos;
				return;
			}

			scrollPos = rectLine.m_Top;
		}

		assert(0 && "scrollup error");
	}
}

// 向下滚动一行
void RichText::ScrollDown()
{
	if ( m_viewStartPoint.m_y >= GetEndDispPos() )
	{
		return;
	}

	bool bFound = false;
	RichTextLineList::iterator oneLineIter;
	for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
	{
		RichTextLine* pLine = *oneLineIter;
		RectFloat rectLine = pLine->m_rect;

		if (bFound)
		{
			m_viewStartPoint.m_y = rectLine.m_Top;
			return;
		}
		if ((rectLine.m_Top <= m_viewStartPoint.m_y) && (rectLine.m_Bottom > m_viewStartPoint.m_y))
		{
			bFound = true;
		}
	}

	assert( false && "scrollDown error");
}


// 滚到第一行
void RichText::ScrollFirst()
{
	if ( m_viewStartPoint.m_y != GetStartDispPos() )
	{
		m_viewStartPoint.m_y = GetStartDispPos();
	}
}

// 滚动到最后一行
void RichText::ScrollEnd()
{
	if (m_viewStartPoint.m_y != GetEndDispPos())
	{
		m_viewStartPoint.m_y = GetEndDispPos();
	}
}

int RichText::GetTextLines()
{
	return (int)m_listLines.size();
}

int RichText::getLineRealWidth( int nLine )
{
	RichTextLineList::iterator oneLineIter;
	int nCount = 0;
	int nWidth = 0;
	for ( oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++,nCount++ )
	{
		if( nCount == nLine )
		{
			
			for ( RichTextObjectList::iterator iter = (*oneLineIter)->m_listObjects.begin(), 
					end = (*oneLineIter)->m_listObjects.end(); iter != end; ++iter )
			{
				RichTextObject* pTextObj = *iter;
				int nCurWidth = pTextObj->m_rect.m_Right + pTextObj->m_rect.m_Left;
				if ( nWidth < nCurWidth )
				{
					nWidth = nCurWidth;
				}
			}
			break;
		}
	}
	return nWidth;
}

// 获得指定行宽度
int RichText::getLineWidth( int nLine )
{
	RichTextLineList::iterator oneLineIter;
	int nCount = 0;
	for ( oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++,nCount++ )
	{
		if( nCount == nLine )
		{
			return (int)abs( (*oneLineIter)->m_rect.m_Left - (*oneLineIter)->m_rect.m_Right );
		}
	}
	return 0;
}

int RichText::GetLineWidth(int index)
{
	RichTextLineList::iterator oneLineIter;
	int i=0;
	for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
	{
		i++;
		RichTextLine* pLine = *oneLineIter;
		RectFloat rectLine = pLine->m_rect;

		if (i == index)
		{
			return (int)rectLine.getWidth();
		}
	}

	return 0;
}

int RichText::GetLineHeight( int index )
{
	RichTextLineList::iterator oneLineIter;
	int i=0;
	for ( oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++ )
	{
		++i;
		RichTextLine* pLine = *oneLineIter;
		RectFloat rectLine = pLine->m_rect;

		if (i == index)
		{
			return (int)rectLine.getHeight();
		}
	}

	return 0;
}

// 获得总高度
int RichText::GetTotalHeight( )
{
	if ( m_listLines.empty() )
	{
		return 0;
	}
	RichTextLine* pFirstLine	= m_listLines.front();
	RichTextLine* pLastLine		= m_listLines.back();
	return (int)ceil(abs( pLastLine->m_rect.m_Bottom - pFirstLine->m_rect.m_Top + 
					m_nLineInterval*g_pFrameMgr->GetScreenScaleY()));
}

int RichText::GetViewLines()
{
	int nTotalCanShowLineNum = 0;
	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++ )
	{
		RichTextLine*		pOneLine	= *oneLineIter;
		const RectFloat&	rectLine	= pOneLine->m_rect;

		if ( rectLine.m_Top - m_viewStartPoint.m_y <= m_rect.getHeight() && rectLine.m_Top - m_viewStartPoint.m_y >= 0)
		{
			++nTotalCanShowLineNum;
		}
	}
	return nTotalCanShowLineNum;
}

int RichText::GetAccurateViewLines()
{
	int nTotalCanShowLineNum = 0;
	for ( RichTextLineList::iterator oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); ++oneLineIter )
	{
		RichTextLine*		pOneLine	= *oneLineIter;
		const RectFloat&	rectLine	= pOneLine->m_rect;
		
		// 由于RichText不支持显示半行内容，故只要某一行的顶部不在显示范围，则不显示这行
		if ( rectLine.m_Top < m_viewStartPoint.m_y )
		{
			// 不在显示范围
			continue;
		}

		// 若某一行的底部不在视口偏移Y值与整个视口高度的范围内，则不显示这行
		if ( ( rectLine.m_Bottom > ( m_viewStartPoint.m_y + m_rect.getHeight() ) ) )
		{
			// 超过显示范围
			break;
		}

		++nTotalCanShowLineNum;
	}

	return nTotalCanShowLineNum;
}

// 向下滚动一行
//void RichText::ScrollDown()
//{
//	if ( m_viewStartPoint.m_y >= GetEndDispPos() )
//	{
//		return;
//	}
//
//	bool bFound = false;
//	RichTextLineList::iterator oneLineIter;
//	for(oneLineIter = m_listLines.begin(); oneLineIter != m_listLines.end(); oneLineIter++)
//	{
//		RichTextLine* pLine = *oneLineIter;
//		RectFloat rectLine = pLine->m_rect;
//
//		if (bFound)
//		{
//			m_viewStartPoint.m_y = rectLine.m_Top;
//			return;
//		}
//		if ((rectLine.m_Top <= m_viewStartPoint.m_y) && (rectLine.m_Bottom > m_viewStartPoint.m_y))
//		{
//			bFound = true;
//		}
//	}
//
//	assert( false && "scrollDown error");
//}

void RichText::setAbsRect(float fLeft,float fTop, float fRight, float fBottom)
{
	m_AbsRect.m_Left	= FloatToInt( fLeft );
	m_AbsRect.m_Top		= FloatToInt( fTop );
	m_AbsRect.m_Right	= FloatToInt( fRight );
	m_AbsRect.m_Bottom	= FloatToInt( fBottom );
	UpdateRichRect( m_AbsRect );
}

int RichText::GetTextExtentWidth( const char* pszContent )
{
	float width, height;
	const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
	g_pDisplay->GetTextExtent( oneUIFontConfig.h, pszContent, width, height );

	return FloatToInt(width);
}

int RichText::GetTextExtentHeight( const char* pszContent )
{
	float width, height;
	const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
	g_pDisplay->GetTextExtent( oneUIFontConfig.h, pszContent, width, height );

	return FloatToInt( height );
}

void RichText::setCursorNormal()
{
	if( GetCurrentCursorLevel() == eCursorOnlyOnUi )
	{
		g_pFrameMgr->setCursor("normal");
	}
}
/*
//-----------------------------------------------------------------------------
// RichText lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(RichText, Frame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//RichtText
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, AddText, SVT_EMPTY, SVT_PTR_STRING, SVT_INT, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, SetText, SVT_EMPTY, SVT_PTR_STRING, SVT_INT, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, Clear);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, SetDispPos, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, GetDispPos, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, GetStartDispPos, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, GetEndDispPos, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, ScrollUp);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, ScrollDown);
DECLARE_MEM_FUNC_TO_SCRIPT(RichText, ScrollEnd);
*/

void RichText::OnGameFontUpdated()
{
	if ( !g_pFrameMgr->m_bUpdatedGameFont )
	{
		return;
	}

	g_pDisplay->ReleaseTrueTypeFont(m_hLinkFont);
	const UIFont& oneUIFontConfig	= g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
	m_hLinkFont = g_pDisplay->CreateTrueTypeFont( oneUIFontConfig.height,  oneUIFontConfig.height, 
		oneUIFontConfig.type.c_str(), CCODING_GBK, FONTTYPE_UNDERLINE );
	UpdateSelf( 0 );
	Resize();
}

// 绘制普通文字
void RichText::calculateNormalTextColor( ColorQuad& color, const RichTextText* pText, unsigned int dwCurTime )
{
	if ( pText->m_bLinkItem )
	{
		int nOffX = 0;
		int nOffY = 0;
		if ( m_pLinkHightLineObj == pText )
		{
			color = ColorQuad( 39,129,194 );
			return;
		}
	}

	if ( pText->m_bBlink )
	{
		int n = dwCurTime % BLINK_CIRCLE;
		if ( n >= BLINK_HALFCIRCLE )
		{
			n = BLINK_CIRCLE - n - 1;
		}

		n = n * 350 / BLINK_HALFCIRCLE;
		if ( n > 255 )
		{
			n = 255;
		}

		color.a = ( unsigned char )n;
	}

	color.a = (unsigned char)( color.a * m_fAlpha );
}

void RichText::calculateNormalTextXViewport( RFPoint& ptViewStartPoint, RFPoint& ptViewOffsetPoint,
											float& fWidth )
{
	float fActualRightestX = ptViewStartPoint.m_x + fWidth;
	if ( ptViewStartPoint.m_x < m_rect.m_Left && fActualRightestX > m_rect.m_Left )
	{
		ptViewOffsetPoint.m_x	= m_rect.m_Left - ptViewStartPoint.m_x;
		fWidth					-= ptViewOffsetPoint.m_x;
		ptViewStartPoint.m_x	 = m_rect.m_Left;
	}

	if ( ( ptViewStartPoint.m_x < m_rect.m_Right ) && ( fActualRightestX > m_rect.m_Right ) )
	{
		fWidth -= fActualRightestX - m_rect.m_Right;
	}
}

void RichText::calculateNormalTextYViewport( RFPoint& ptViewStartPoint, RFPoint& ptViewOffsetPoint,
											float& fHeight )
{
	float fActualBottomestY = ptViewStartPoint.m_y + fHeight;
	if ( ( ptViewStartPoint.m_y < m_rect.m_Top ) && ( fActualBottomestY > m_rect.m_Top ) )
	{
		// 对上边界做裁减
		ptViewOffsetPoint.m_y	= m_rect.m_Top - ptViewStartPoint.m_y;
		fHeight					-= ptViewOffsetPoint.m_y;
		ptViewStartPoint.m_y	= m_rect.m_Top;
	}

	if( ptViewStartPoint.m_y < m_rect.m_Bottom && fActualBottomestY > m_rect.m_Bottom )
	{
		// 对下边界做裁减
		fHeight -= fActualBottomestY - m_rect.m_Bottom;
	}
}

void RichText::FinishDrawNormalText( const RichTextText* pText, const RFPoint& ptViewOffsetPoint, 
									RectFloat& viewRect, unsigned int dwCurTime )
{
	ColorQuad color = pText->m_color;
	calculateNormalTextColor( color, pText, dwCurTime );
	bool isClipSelf = ( m_Style & FRMSTYLE_CLIPSELF ) != 0;
	if ( isClipSelf )
	{
		g_pDisplay->PushClipRect(RectFloat((float)m_ClipRect.m_Left, (float)m_ClipRect.m_Top,
			(float)m_ClipRect.m_Right, (float)m_ClipRect.m_Bottom));
	}

	if ( pText->m_bLinkItem )
	{
		g_pDisplay->renderTextRect( m_hLinkFont, m_FontStyle, pText->m_szText, viewRect, 
			-ptViewOffsetPoint.m_x, -ptViewOffsetPoint.m_y, false, color, 
			1.0f, false, m_shadowColor );	
	}
	else
	{
		const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
		g_pDisplay->renderTextRect( oneUIFontConfig.h, m_FontStyle, pText->m_szText, viewRect, 
			-ptViewOffsetPoint.m_x, -ptViewOffsetPoint.m_y, false, color, 
			/*1.0f */oneUIFontConfig.scale*m_fSelfScale, false, m_shadowColor );

	}

	if ( isClipSelf )
	{
		g_pDisplay->PopClipRect();
	}
}

// 绘制普通文字

// 绘制表情
void RichText::calculateOneFaceXViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
												  RFSize& viewSize, RFSize& uvSize)
{
	float fCurFaceWidth		= g_pFrameMgr->m_nFaceWidth * g_pFrameMgr->GetScreenScaleX();
	float fViewRightestX	= ptViewStartPoint.m_x + fCurFaceWidth;

	if ( ptViewStartPoint.m_x < m_rect.m_Left && fViewRightestX > m_rect.m_Left )
	{
		// 对左边界做裁减
		float fClipWidth		= m_rect.m_Left - ptViewStartPoint.m_x;
		viewSize.m_x			-= fClipWidth;
		ptUVStartPoint.m_x		+= fClipWidth * g_pFrameMgr->m_fUUnit;
		uvSize.m_x				-= fClipWidth * g_pFrameMgr->m_fUUnit;
		ptViewStartPoint.m_x	= m_rect.m_Left;
	}

	if( ptViewStartPoint.m_x < m_rect.m_Right && fViewRightestX > m_rect.m_Right )
	{
		// 对右边界做裁减
		float fClipWidth	= fViewRightestX - m_rect.m_Right;
		viewSize.m_x		-= fClipWidth;
		uvSize.m_x			-= fClipWidth * g_pFrameMgr->m_fUUnit;
	}
}

void RichText::calculateOneFaceYViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
												  RFSize& viewSize, RFSize& uvSize )
{
	float fCurFaceHeight	= g_pFrameMgr->m_nFaceHeight * g_pFrameMgr->GetScreenScaleY();
	float fViewBottomestY	= ptViewStartPoint.m_y + fCurFaceHeight;
	if( ptViewStartPoint.m_y < m_rect.m_Top && fViewBottomestY > m_rect.m_Top )
	{
		// 对上边界做裁减
		float fClipHeight		= m_rect.m_Top - ptViewStartPoint.m_y;
		viewSize.m_y			-= fClipHeight;
		ptUVStartPoint.m_y		+= fClipHeight * g_pFrameMgr->m_fVUnit;
		uvSize.m_y				-= fClipHeight * g_pFrameMgr->m_fVUnit;
		ptViewStartPoint.m_y	= m_rect.m_Top;
	}

	if( ptViewStartPoint.m_y < m_rect.m_Bottom && fViewBottomestY > m_rect.m_Bottom )
	{
		// 对下边界做裁减
		float fClipHeight	= fViewBottomestY - m_rect.m_Bottom;
		viewSize.m_y		-= fClipHeight;
		uvSize.m_y			-= fClipHeight * g_pFrameMgr->m_fVUnit;
	}
}

void RichText::initOneFaceUVStartPoint( RFPoint& ptUVStartPoin, RichTextFace* pFace, unsigned int dwCurTime )
{
	int nCurFrame( 0 );

	if( pFace->m_nIconNum == 1 )
	{
		nCurFrame = pFace->m_nIconStartIdx;
	}
	else
	{
		nCurFrame = ( ( dwCurTime - pFace->m_dwStartTime ) / pFace->m_dwFrameInterval ) % pFace->m_nIconNum;
		nCurFrame += pFace->m_nIconStartIdx;
	}

	int nIdxX	= nCurFrame % g_pFrameMgr->m_nFaceNumX;
	int nIdxY	= nCurFrame / g_pFrameMgr->m_nFaceNumX;
	ptUVStartPoin.SetPos( nIdxX * g_pFrameMgr->m_fUWidth, nIdxY * g_pFrameMgr->m_fVWidth );
}

void RichText::DrawFaceHighlightBackTex( const RFPoint& ptViewStartPoint, const RFSize& viewSize, 
										int& nFaceNum, Ogre::DrawRect* faces )
{
	Ogre::DrawRect& oneDrawRect = faces[nFaceNum];
	Ogre::RectFloat& oneFaceViewRect = oneDrawRect.rect;
	oneFaceViewRect.set(	ptViewStartPoint.m_x,					ptViewStartPoint.m_y, 
		ptViewStartPoint.m_x + viewSize.m_x,	ptViewStartPoint.m_y + viewSize.m_y );

	float fTexU			= (float)m_faceBorderTex.m_nTexX / g_pFrameMgr->m_nFaceTexWidth;
	float fTexV			= (float)m_faceBorderTex.m_nTexY / g_pFrameMgr->m_nFaceTexHeight;
	float fTexUWidth	= (float)m_faceBorderTex.m_nTexWidth / g_pFrameMgr->m_nFaceTexWidth;
	float fTexVHeight	= (float)m_faceBorderTex.m_nTexHeight / g_pFrameMgr->m_nFaceTexHeight;
	Ogre::RectFloat& oneFaceUVRect = oneDrawRect.rectUV;
	oneFaceUVRect.set( fTexU, fTexV, fTexU + fTexUWidth, fTexV + fTexVHeight );

	Ogre::ColorQuad& oneFaceColor = oneDrawRect.color;
	oneFaceColor.set(255, 255, 255, 255 );
	++nFaceNum;
}

void RichText::FinishDrawFace( RichTextFace* pFace, const RFPoint& ptViewStartPoint, const RFPoint& ptUVStartPoint, 
							  const RFSize& viewSize, const RFSize& uvSize, int& nFaceNum, Ogre::DrawRect* faces )
{
	Ogre::DrawRect& oneDrawRect = faces[nFaceNum];

	Ogre::RectFloat& oneFaceViewRect = oneDrawRect.rect;
	oneFaceViewRect.set(	ptViewStartPoint.m_x,					ptViewStartPoint.m_y, 
		ptViewStartPoint.m_x + viewSize.m_x,	ptViewStartPoint.m_y + viewSize.m_y );

	Ogre::RectFloat& oneFaceUVRect = oneDrawRect.rectUV;
	oneFaceUVRect.set(	ptUVStartPoint.m_x,				ptUVStartPoint.m_y, 
		ptUVStartPoint.m_x +uvSize.m_x,	ptUVStartPoint.m_y + uvSize.m_y );

	Ogre::ColorQuad& oneFaceColor = oneDrawRect.color;
	oneFaceColor.set( 255, 255, 255, ( unsigned char )( 255 * m_fAlpha ) );
	++nFaceNum;

	validateMaxFacesToRender( nFaceNum, faces );
	if ( m_pLinkHightLineObj == pFace && m_faceBorderTex.m_hTex != NULL )
	{
		DrawFaceHighlightBackTex( ptViewStartPoint, viewSize, nFaceNum, faces );
	}

	validateMaxFacesToRender( nFaceNum, faces );
}

// 绘制表情

// 绘制图片

void RichText::initOnePictureUVStartPoint( RFPoint& ptUVStartPoin, PictureData* pPictureData, 
										  RichTextPicture* pPicture, unsigned int dwCurTime )
{
	int nCurFrame = 0;
	if( pPicture->m_nIconNum == 1 )
	{
		nCurFrame = 0;
	}
	else
	{
		nCurFrame = ( ( dwCurTime - pPicture->m_dwStartTime ) / pPicture->m_dwFrameInterval ) % pPicture->m_nIconNum;
	}

	int		nTexLeft = pPictureData->m_nTexLeft + nCurFrame * pPictureData->m_nTexWidth;
	int		nTexTop	 = pPictureData->m_nTexTop;
	float fu = (float)nTexLeft;
	float fv = (float)nTexTop;
	ptUVStartPoin.SetPos( fu, fv );
}

void RichText::calculateOnePictureXViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
													 RFSize& viewSize, RFSize& uvSize, const PictureData* pPictureData,
													 RPictureCodeMap* pPictureMapBuffer )
{
	const float PICTURE_WIDTH	= (float)pPictureData->m_nTexWidth * g_pFrameMgr->GetScreenScaleY();
	float fViewRightestX		= ptViewStartPoint.m_x + PICTURE_WIDTH;
	float fUUnit				= 1.0f /  pPictureMapBuffer->GetTexWidth();

	if ( ( ptViewStartPoint.m_x < m_rect.m_Left ) && ( fViewRightestX > m_rect.m_Left ) )
	{
		// 对左边界做裁减
		float fClipWidth		= m_rect.m_Left - ptViewStartPoint.m_x;
		viewSize.m_x			-= fClipWidth;
		ptUVStartPoint.m_x		+= fClipWidth;
		uvSize.m_x				= fClipWidth;
		ptViewStartPoint.m_x	= m_rect.m_Left;
	}

	if ( ( ptViewStartPoint.m_x < m_rect.m_Right ) && ( fViewRightestX > m_rect.m_Right ) )
	{
		// 对右边界做裁减
		float fClipWidth	= fViewRightestX - m_rect.m_Right;
		viewSize.m_x		-= fClipWidth;
		uvSize.m_x			-= fClipWidth; 
	}
}

void RichText::calculateOnePictureYViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
													 RFSize& viewSize, RFSize& uvSize, const PictureData* pPictureData,
													 RPictureCodeMap* pPictureMapBuffer )
{
	const float PICTURE_HEIGHT	= pPictureData->m_nTexHeight  * g_pFrameMgr->GetScreenScaleY();
	float fViewBottomestY		= ptViewStartPoint.m_y + PICTURE_HEIGHT;
	float fVUnit				= 1.0f /  pPictureMapBuffer->GetTexHeight();

	if ( ( ptViewStartPoint.m_y < m_rect.m_Top ) && ( fViewBottomestY > m_rect.m_Top ) )
	{
		// 对上边界做裁减
		float fClipHeight		= m_rect.m_Top - ptViewStartPoint.m_y;
		viewSize.m_y			-= fClipHeight;
		ptUVStartPoint.m_y		+= fClipHeight;
		uvSize.m_y				-= fClipHeight; 
		ptViewStartPoint.m_y	= m_rect.m_Top;
	}

	// ptViewStartPoint.m_y + viewSize.m_y
	if ( ( ptViewStartPoint.m_y < m_rect.m_Bottom ) && ( fViewBottomestY > m_rect.m_Bottom ) )
	{
		// 对下边界做裁减
		float fClipHeight	= fViewBottomestY - m_rect.m_Bottom;
		viewSize.m_y		-= fClipHeight;
		uvSize.m_y			-= fClipHeight;
	}
}

void RichText::FinishDrawPicture( const RFPoint& ptViewStartPoint, const RFPoint& ptUVStartPoint, 
								 const RFSize& viewSize, const RFSize& uvSize, 
								 int& nPictureNum, Ogre::DrawRect* pictures )
{
	Ogre::DrawRect& oneDrawRect = pictures[nPictureNum];

	Ogre::RectFloat& onePictureViewRect = oneDrawRect.rect;
	onePictureViewRect.set( ptViewStartPoint.m_x,					ptViewStartPoint.m_y, 
		ptViewStartPoint.m_x + viewSize.m_x,	ptViewStartPoint.m_y + viewSize.m_y );

	Ogre::RectFloat& onePictureUVRect = oneDrawRect.rectUV;
	onePictureUVRect.set( ptUVStartPoint.m_x,					ptUVStartPoint.m_y, 
		ptUVStartPoint.m_x + uvSize.m_x,	ptUVStartPoint.m_y + uvSize.m_y );

	Ogre::ColorQuad& oneFaceColor = oneDrawRect.color;
	oneFaceColor.set( 255, 255, 255, ( unsigned char )( 255 * m_fAlpha ) );
	++nPictureNum;

	validateMaxPictureToRender( nPictureNum, pictures );
}

// 绘制图片