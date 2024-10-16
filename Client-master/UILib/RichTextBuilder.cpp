#include "RichTextBuilder.h"
#include "ui_framemgr.h"
#include "ui_richtext.h"

using namespace Ogre;
extern FrameManager *g_pFrameMgr;
RichTextLine::~RichTextLine()
{
	for( RichTextObjectList::iterator iObj = m_listObjects.begin(), end = m_listObjects.end(); iObj != end; ++iObj )
	{
		RichTextObject* pOneTextObj = *iObj;
		delete pOneTextObj;
	}
}

//-----------------------------------------------------------------------------
// RRichTextBuilder
//-----------------------------------------------------------------------------
bool RRichTextBuilder::CanBuildNewOneLine()
{
	if ( m_szPos == NULL )
	{
		return false;
	}
	
	if ( m_szPos[0] == '\0' )
	{
		return false;
	}

	return true;
}

bool RRichTextBuilder::OnParseNewLineChar( char& firstChar, char& secondChar )
{
	if( firstChar == '\n' || (firstChar == '\r' && secondChar != '\n'))
	{
		m_szPos += 1;
	}
	else
	{
		m_szPos += 2;
	}

	return false;
}

bool RRichTextBuilder::OnParseLinkChar()
{
	m_szPos				+= 2;
	m_strLinkItemHref	= m_szPos;
	size_t pos			= m_strLinkItemHref.find( "#n" );
	m_szPos				+= pos;

	if (pos != 0)
	{
		m_strLinkItemHref = m_strLinkItemHref.substr( 0, pos );
	}

	for ( size_t i = 0, size = m_LinkItemList.size(); i < size; ++i )
	{
		RichTextText* pOneText = m_LinkItemList[i];
		pOneText->m_strLinkItemHref = m_strLinkItemHref;
	}

	m_LinkItemList.clear();
	m_bLinkBegin = false;

	return true;
}

bool RRichTextBuilder::IsWidthEnoughToPutOneNewFace()
{
	const Ogre::RectFloat& richViewRect = m_pRichText->m_rect;
	float fScreenScaleY					= g_pFrameMgr->GetScreenScaleY();
	float fCurFaceScreenWidth			= g_pFrameMgr->m_nFaceWidth * fScreenScaleY;
	float fCurNeedWidth					= m_newObjStartPoint.m_x + fCurFaceScreenWidth;
	float fPreDefWidth					= richViewRect.getWidth();

	return fPreDefWidth >= fCurNeedWidth;
}

bool RRichTextBuilder::OnParseFaceChar( RichTextLine* pOneLine, char& firstChar, char& secondChar )
{
	Ogre::RectFloat& lineViewRect	= pOneLine->m_rect;
	float fScreenScaleY				= g_pFrameMgr->GetScreenScaleY();
	if ( IsFaceCharPosSymbol() )
	{
		// 占位符号，不显示表情，用于将行高度撑开到表情高度
		m_szPos += 4;
		lineViewRect.m_Bottom = lineViewRect.m_Top + g_pFrameMgr->m_nFaceHeight * fScreenScaleY;
		return true;
	}
	
	// 处理表情对象
	if ( !IsWidthEnoughToPutOneNewFace() )
	{
		// 宽度不够放下一个表情，退出循环，换行
		return false;
	}

	m_szPos += 1;
	RichTextObject* pObj = NewFaceObject();
	if ( pObj == NULL )
	{
		return true;
	}
	
	const Ogre::RectFloat& faceObjViewRect = pObj->m_rect;
	m_newObjStartPoint.m_x	+= faceObjViewRect.getWidth();
	lineViewRect.m_Right	=  lineViewRect.m_Left + m_newObjStartPoint.m_x;
	if ( lineViewRect.getHeight() < faceObjViewRect.getHeight() )
	{
		lineViewRect.m_Bottom = lineViewRect.m_Top + faceObjViewRect.getHeight();
	}

	pOneLine->m_listObjects.push_back(pObj);
	return true;
}

bool RRichTextBuilder::OnParsePictureChar( RichTextLine* pOneLine )
{
	// 处理图片对象
	const char* pszStartText = m_szPos;
	m_szPos		+= 2;
	int nOffset = 0;
	RichTextObject* pObj = NewPictureObject( nOffset );
	if ( pObj == NULL )
	{
		return true;
	}

	RichTextPicture*	pPictureObj			= static_cast<RichTextPicture*>( pObj );
	RPictureCodeMap*	pPictureMapBuffer	= g_pFrameMgr->getPictureCodeMap();
	PictureData*		pPictureData		= pPictureMapBuffer->GetPictureData( pPictureObj->m_nCode );

	float fCurNeedViewWidth	= m_newObjStartPoint.m_x + pPictureData->m_nTexWidth * g_pFrameMgr->GetScreenScaleY();
	float fPreDefViewWidth	= m_pRichText->m_rect.getWidth();
	// TODO: 这儿也有可能死循环
	if ( fPreDefViewWidth < fCurNeedViewWidth )
	{
		// 将图片对象构造到当前行失败
		m_szPos -= ( 2 + nOffset );
		return false;
	}
	
	pPictureObj->m_strAnimText.assign( pszStartText, 2 + nOffset );

	// 构造图片对象成功
	const Ogre::RectFloat& pictureObjViewRect	= pObj->m_rect;
	Ogre::RectFloat& lineViewRect				= pOneLine->m_rect;
	m_newObjStartPoint.m_x						+= pictureObjViewRect.getWidth();
	lineViewRect.m_Right						= pOneLine->m_rect.m_Left + m_newObjStartPoint.m_x;
	if ( lineViewRect.getHeight() < pictureObjViewRect.getHeight() )
	{
		lineViewRect.m_Bottom = lineViewRect.m_Top + pictureObjViewRect.getHeight();
	}

	pOneLine->m_listObjects.push_back( pObj );

	return true;
}

bool RRichTextBuilder::OnParseSpecialFunctionChar( char& firstChar, char& secondChar )
{
	// 处理其它转意字符串
	switch ( secondChar ) 
	{
	case 'R':
		m_colorCur.set(255,80,80,255);
		m_szPos += 2;
		break;
	case 'G':
		m_colorCur.set(60,255,60,255);
		m_szPos += 2;
		break;
	case 'B':
		m_colorCur.set(80,200,255,255);
		m_szPos += 2;
		break;
	case 'K':
		m_colorCur.set(0,0,0,255);
		m_szPos += 2;
		break;
	case 'Y':
		m_colorCur.set(255,255,0,255);
		m_szPos += 2;
		break;
	case 'W':
		m_colorCur.set(255,255,255,255);
		m_szPos += 2;
		break;
	case 'L':
		{
			m_bLinkBegin		= true;
			m_strLinkItemHref.clear();
			m_bLinkItem			= true;
			m_colorCur			= m_pRichText->m_LinkTextColor;
			m_szPos				+= 2;
			m_LinkItemList.clear();
		}
		break;

	case 'b':
		m_bBlink = true;
		m_szPos += 2;
		break;

	case 'n':
		m_colorCur	= m_colorText;
		m_bBlink	= false;
		m_bLinkItem = false;
		m_szPos		+= 2;
		break;

	case 'c':
		{
			m_szPos			+= 2;
			char chCurPos	= *m_szPos;
			if (	( chCurPos >= '0' && chCurPos <= '9') || 
					( chCurPos >= 'a' && chCurPos <= 'f') || 
					( chCurPos >= 'A' && chCurPos <= 'F') )
			{
				// 解析自定意颜色
				SetCustomColor();
			}
		}
		break;

	case 'P':
		m_szPos += 2;
		m_bCenter = true;
		break;

	default:
		// 无法解析，忽略这个#号
		m_szPos += 1;
		break;
	}

	return true;
}

bool RRichTextBuilder::OnParseSelfDefineFormatText( RichTextLine* pOneLine, char& firstChar, char& secondChar )
{
	Ogre::RectFloat& lineViewRect	= pOneLine->m_rect;
	// 表情符号
	if ( secondChar >= '0' && secondChar <= '9' )
	{
		return OnParseFaceChar( pOneLine, firstChar, secondChar );
	}
	else if ( secondChar == 'A' )
	{
		return OnParsePictureChar( pOneLine );
	}
	else
	{
		return OnParseSpecialFunctionChar( firstChar, secondChar );
	}

	return true;
}

bool RRichTextBuilder::OnParseNormalText( RichTextLine* pOneLine )
{
	Ogre::RectFloat& lineViewRect	= pOneLine->m_rect;
	RichTextObject* pObj			= NewTextObject();
	
	if( pObj == NULL )
	{
		// 对象创建失败，空间不够，退出循环，换行
		return false;
	}

	if ( m_bLinkBegin )
	{
		RichTextText* pTextObj = static_cast<RichTextText*>( pObj );
		m_LinkItemList.push_back( pTextObj );
	}

	m_newObjStartPoint.m_x	+= pObj->m_rect.getWidth();
	lineViewRect.m_Right	=  lineViewRect.m_Left + m_newObjStartPoint.m_x;
	if( lineViewRect.getHeight() < pObj->m_rect.getHeight() )
	{
		lineViewRect.m_Bottom = lineViewRect.m_Top + pObj->m_rect.getHeight();
	}
	pOneLine->m_listObjects.push_back(pObj);
	return true;
}

bool RRichTextBuilder::OnFirstCharNotZero( RichTextLine* pOneLine, char& firstChar, char& secondChar )
{
	Ogre::RectFloat& lineViewRect	= pOneLine->m_rect;
	bool bUpdateTwoParseChar		= false;

	if( IsNewLineSymbol( firstChar, secondChar ) )
	{
		bUpdateTwoParseChar = OnParseNewLineChar( firstChar, secondChar );	
	}
	else if ( IsLinkSymbol( firstChar, secondChar ) )
	{
		bUpdateTwoParseChar = OnParseLinkChar();
	}
	else if( IsSelfDefineFormatSymbol( firstChar, secondChar ) )
	{
		bUpdateTwoParseChar = OnParseSelfDefineFormatText( pOneLine, firstChar, secondChar );
	}
	else
	{
		bUpdateTwoParseChar = OnParseNormalText( pOneLine );
	}
	
	if ( !bUpdateTwoParseChar )
	{
		return false;
	}

	firstChar = *m_szPos;
	if( firstChar == 0 )
	{
		return false;
	}

	secondChar = *( m_szPos + 1 );
	return true;
}

bool RRichTextBuilder::BuildNewOneLine( RichTextLine*& pOneLine )
{
	if ( !CanBuildNewOneLine() )
	{
		return false;
	}

	char firstChar	= *m_szPos;
	char secondChar = '\0';
	
	pOneLine = new RichTextLine;
	Ogre::RectFloat& lineViewRect	= pOneLine->m_rect;
	lineViewRect.m_Left				= lineViewRect.m_Right = m_pRichText->m_newLineStartPoint.m_x;
	lineViewRect.m_Top				= (float)( FloatToInt( m_pRichText->m_newLineStartPoint.m_y ) );
	const UIFont& oneUIFontConfig	= g_pFrameMgr->getUIFontByIndex( m_pRichText->m_iFontIndex );
	lineViewRect.m_Bottom			= (float)( FloatToInt( lineViewRect.m_Top + oneUIFontConfig.height + 1 ) );

	secondChar = *( m_szPos + 1 );
	m_newObjStartPoint.SetPos( 0, 0 );
	m_bCenter = false;

	while ( firstChar != 0  )
	{
		bool isContinueParseText = OnFirstCharNotZero( pOneLine, firstChar, secondChar );
		if ( !isContinueParseText )
		{
			break;
		}
	}
	
	float fScreenScaleY = g_pFrameMgr->GetScreenScaleY();
	if( m_pRichText->m_bCenterLine )
	{
		lineViewRect.m_Left = ( m_pRichText->GetWidth() * fScreenScaleY - lineViewRect.getWidth() ) / 2;
	}

	// 一行结束，将新行开始位置向下推一行
	float fCurLineHeight = lineViewRect.getHeight() + ( m_pRichText->m_nLineInterval * fScreenScaleY );
	m_pRichText->m_newLineStartPoint.m_y += fCurLineHeight;
	
	if ( IsRichViewRectTooSmall( firstChar, secondChar, pOneLine->m_listObjects ) )
	{
		OGRE_DELETE( pOneLine );
		return false;
	}

	return true;
}

bool RRichTextBuilder::ParseTextObjectTwoNeighbourChar( int& nLen, char* szTwoChars, unsigned char* szCurTextObjContent )
{
	unsigned char firstChar = *m_szPos;
	if ( firstChar == 0 || firstChar == '\n' )
	{
		return false;
	}
	
	unsigned char secondChar = *( m_szPos + 1 );
	if ( IsSelfDefineFormatSymbol( firstChar, secondChar ) )
	{
		return false;
	}
	
	if ( IsLinkSymbol( firstChar, secondChar ) )
	{
		return false;
	}
	
	if ( IsChinese( firstChar ) )									//中文
	{
		szCurTextObjContent[nLen++]	= firstChar;
		szCurTextObjContent[nLen++]	= secondChar;
		szTwoChars[0]				= firstChar;
		szTwoChars[1]				= secondChar;
		m_szPos						+= 2;
		return true;
	}

	if ( IsOmitSymbol( firstChar ) )
	{
		szCurTextObjContent[nLen++]	= firstChar;
		szTwoChars[0]				= 0; 
		szTwoChars[1]				= firstChar;
		m_szPos						+= 2;
		return true;
	}

	if ( IsEnglish( firstChar ) )				//英文，包括空格
	{
		szCurTextObjContent[nLen++]	= firstChar;
		szTwoChars[0]				= 0;
		szTwoChars[1]				= firstChar;
		m_szPos						+= 1;
		return true;
	}
	
	szCurTextObjContent[nLen++]	= firstChar;
	szTwoChars[0]				= 0; 
	szTwoChars[1]				= 0;
	m_szPos						+= 1;
	return true;
}

bool RRichTextBuilder::ParseTextObjectTwoNeighbourChar( char* szTwoChars, std::string& strCurTextObjContent )
{
	unsigned char firstChar = *m_szPos;
	if ( firstChar == 0 || firstChar == '\n' )
	{
		return false;
	}

	unsigned char secondChar = *( m_szPos + 1 );
	if ( IsSelfDefineFormatSymbol( firstChar, secondChar ) )
	{
		return false;
	}

	if ( IsLinkSymbol( firstChar, secondChar ) )
	{
		return false;
	}

	if ( IsChinese( firstChar ) )									//中文
	{
		strCurTextObjContent.append( 1, firstChar );
		strCurTextObjContent.append( 1, secondChar );
		szTwoChars[0]				= firstChar;
		szTwoChars[1]				= secondChar;
		m_szPos						+= 2;
		return true;
	}

	if ( IsOmitSymbol( firstChar ) )
	{
		strCurTextObjContent.append( 1, firstChar );
		szTwoChars[0]				= 0; 
		szTwoChars[1]				= firstChar;
		m_szPos					+= 2;
		return true;
	}

	if ( IsEnglish( firstChar ) )				//英文，包括空格
	{
		strCurTextObjContent.append( 1, firstChar );
		szTwoChars[0]				= 0;
		szTwoChars[1]				= firstChar;
		m_szPos					+= 1;
		return true;
	}

	strCurTextObjContent.append( 1, firstChar );
	szTwoChars[0]				= 0; 
	szTwoChars[1]				= 0;
	m_szPos					+= 1;
	return true;
}

bool RRichTextBuilder::CalculateOneTextObjectViewRect( Ogre::RectFloat& textObjViewRect, const char* pszTwoChars )
{
	const UIFont& oneUIFontConfig	= g_pFrameMgr->getUIFontByIndex( m_pRichText->m_iFontIndex );
	float fWidth = 0.0f, fHeight = 0.0f;
	g_pDisplay->GetCharExtent( oneUIFontConfig.h, pszTwoChars, fWidth, fHeight );

	if ( textObjViewRect.m_Right + fWidth > m_pRichText->m_rect.getWidth() )
	{
		return false;
	}

	textObjViewRect.m_Right += fWidth;
	return true;
}

RichTextText* RRichTextBuilder::BuildNewOneTextObject( const char* pszCurTextObjContent, 
														Ogre::RectFloat& textObjViewRect )
{
	if( m_bCenter )
	{
		const RectFloat& richViewRect	= m_pRichText->m_rect;
		float fCenterAdjustWidth		= ( richViewRect.getWidth() - textObjViewRect.getWidth() ) / 2;
		textObjViewRect.m_Left			= textObjViewRect.m_Left	+ fCenterAdjustWidth;
		textObjViewRect.m_Right			= textObjViewRect.m_Right	+ fCenterAdjustWidth;
	}
	
	int nLen			= strlen( pszCurTextObjContent );
	RichTextText *pObj	= new RichTextText;
	pObj->m_rect		= textObjViewRect;
	pObj->m_color		= m_colorCur;
	pObj->m_bBlink		= m_bBlink;
	pObj->m_bLinkItem	= m_bLinkItem;
	pObj->m_szText		= new char[nLen+1];
	strncpy( pObj->m_szText, pszCurTextObjContent, nLen + 1 );
	return pObj;
}

RichTextObject* RRichTextBuilder::NewTextObject()
{
	char szTwoChars[4]			= { '\0' };
	std::string	strCurTextObjContent;

	const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_pRichText->m_iFontIndex );
	RectFloat rect( m_newObjStartPoint.m_x, 
					m_newObjStartPoint.m_y - oneUIFontConfig.height - 1, 
					m_newObjStartPoint.m_x, 
					m_newObjStartPoint.m_y );

	while ( true )
	{
		const char* pBK = m_szPos;
		int			nBK	= strCurTextObjContent.size();
		bool isContinueParseText = ParseTextObjectTwoNeighbourChar( szTwoChars, strCurTextObjContent );
		if ( !isContinueParseText )
		{
			break;
		}

		bool isRichViewRectWidthEnough = CalculateOneTextObjectViewRect( rect, szTwoChars );
		if ( !isRichViewRectWidthEnough )
		{
			// TODO; 这会造成死循环
			// 超出范围，退回上一个字符, 结束查找
			m_szPos = pBK;
			strCurTextObjContent.resize( nBK );
			break;
		}
	}

	if( strCurTextObjContent.empty() )
	{
		return NULL;
	}

	return BuildNewOneTextObject( strCurTextObjContent.c_str(), rect );
}

RichTextObject* RRichTextBuilder::NewFaceObject()
{
	int n		= 0;
	int nLen	= 0;
	for ( int i = 0; i < 3; ++i )
	{
		char chCurPos = m_szPos[i];
		if ( chCurPos >= '0' && chCurPos <= '9' )
		{
			n = n * 10 + ( chCurPos - '0' );
			++nLen;
		}
		else
		{
			break;
		}
	}

	m_szPos += nLen;

	FaceData* pFaceData = g_pFrameMgr->m_pCodeMap->GetFaceData( n );
	if( pFaceData )
	{
		float fScreenScaleY		= g_pFrameMgr->GetScreenScaleY();
		RichTextFace *pObj		= new RichTextFace;
		pObj->m_nIconStartIdx	= pFaceData->m_nStartIdx;
		pObj->m_nIconNum		= pFaceData->m_nFrameNum;
		pObj->m_dwFrameInterval = pFaceData->m_nFrameInterval;
		pObj->m_nCode			= pFaceData->m_nCode;
		pObj->m_rect.m_Left		= m_newObjStartPoint.m_x - m_pRichText->m_fFaceXInterval * fScreenScaleY;
		pObj->m_rect.m_Top		= m_newObjStartPoint.m_y - g_pFrameMgr->m_nFaceHeight * fScreenScaleY - 
									m_pRichText->m_fFaceYInterval * fScreenScaleY;
		pObj->m_rect.m_Right	= m_newObjStartPoint.m_x + g_pFrameMgr->m_nFaceWidth * fScreenScaleY;
		pObj->m_rect.m_Bottom	= m_newObjStartPoint.m_y;
		pObj->m_dwStartTime		= timeGetTime();
		return pObj;
	}

	return NULL;
}

RichTextObject* RRichTextBuilder::NewPictureObject( int& nOffset )
{
	const int MAX_LEN	= 3;
	int nCode			= 0;
	for ( int i = 0; i < MAX_LEN; ++i )
	{
		char chCurPos = m_szPos[i];
		if ( !( chCurPos >= '0' && chCurPos <= '9' ) )
		{
			break;
		}

		int nDiffValue = chCurPos - '0';
		nCode = nCode * 10 + nDiffValue;
		++nOffset;
	}

	m_szPos += nOffset;

	RPictureCodeMap*	pPictureMapBuffer	= g_pFrameMgr->getPictureCodeMap();
	PictureData*		pPictureData		= pPictureMapBuffer->GetPictureData( nCode );
	if ( pPictureData == NULL )
	{
		return NULL;
	}
	
	float fScreenScaleY		= g_pFrameMgr->GetScreenScaleY();
	RichTextPicture *pObj	= new RichTextPicture;
	pObj->m_nIconStartIdx	= pPictureData->m_nStartIdx;
	pObj->m_nIconNum		= pPictureData->m_nFrameNum;
	pObj->m_dwFrameInterval = pPictureData->m_nFrameInterval;
	pObj->m_nCode			= pPictureData->m_nCode;
	pObj->m_rect.m_Left		= m_newObjStartPoint.m_x;
	pObj->m_rect.m_Top		= m_newObjStartPoint.m_y - pPictureData->m_nTexHeight * fScreenScaleY;
	pObj->m_rect.m_Right	= m_newObjStartPoint.m_x + pPictureData->m_nTexWidth  * fScreenScaleY;
	pObj->m_rect.m_Bottom	= m_newObjStartPoint.m_y;
	pObj->m_dwStartTime		= timeGetTime();

	return pObj;
}

void RRichTextBuilder::SetCustomColor()
{
	uint n = 0;
	int nLen = 0;
	unsigned char c;
	for(int i=0; i<6; i++)
	{
		c = m_szPos[i];

		if( c >= '0' && c <= '9' )
		{
			n = n * 16 + (c - '0');
			nLen++;
		}
		else if( c >= 'a' && c <= 'f' )
		{
			n = n * 16 + (c - 'a' + 10);
			nLen++;
		}
		else if( c >= 'A' && c <= 'F' )
		{
			n = n * 16 + (c - 'A' + 10);
			nLen++;
		}
		else
		{
			break;
		}
	}
	n |= 0xff000000;

	m_szPos += nLen;
	m_colorCur.set(n);
}

bool RRichTextBuilder::CanBuildText( const char *szText, RichText *pRichText )
{
	if ( szText == NULL || pRichText == NULL )
	{
		return false;
	}

	if ((pRichText->m_rect.getWidth() == 0) || (pRichText->m_rect.getHeight() == 0))
	{
		return false;
	}

	return true;
}

void RRichTextBuilder::RemoveRedundantLineBeforeBuildNewText()
{
	// 当超过最大显示行数，将不在显示区域的行删除
	const int MAX_LINE_NUM					= m_pRichText->m_iMaxLines;
	const RFPoint& curRichViewStartPoint	= m_pRichText->m_viewStartPoint;
	for ( RichTextLineList::iterator lineIter = m_pRichText->m_listLines.begin(); 
			lineIter != m_pRichText->m_listLines.end(); )
	{
		RichTextLine* pOneLine = *lineIter;
		// 若当前存储行数不超过最大行数的时候，则不作裁剪操作
		if ( m_pRichText->m_listLines.size() < MAX_LINE_NUM )
		{
			break;
		}
		
		const Ogre::RectFloat& oneLineRect = pOneLine->m_rect;
		if ( oneLineRect.m_Top < curRichViewStartPoint.m_y )
		{
			delete pOneLine;
			lineIter = m_pRichText->m_listLines.erase( lineIter );
			continue;
		}

		++lineIter;
	}
}

void RRichTextBuilder::Init( const char *szText, RichText* pRichText, const Ogre::ColorQuad& color )
{
	m_szText		= szText;
	m_pRichText		= pRichText;
	m_szPos			= szText;
	m_colorText		= color;
	m_colorCur		= color;
	m_bBlink		= false;
	m_bLinkItem		= false;
	m_bLinkBegin	= false;
}

void RRichTextBuilder::UpdateAutoExtendRichAfterBuildOneLine( const RichTextLine* pOneLine )
{
	// TODO; 怎么做自动扩展以支持完全显示
}

bool RRichTextBuilder::NeedScrollNotAutoExtendRichAfterBuildOneLine( const RichTextLine* pOneLine )
{
	const Ogre::RectFloat& richViewRect = m_pRichText->m_rect;
	const Ogre::RectFloat& lineViewRect = pOneLine->m_rect;
	
	// TODO: 这个判断可以进一步简化
	float fRichBottomestY = m_pRichText->m_viewStartPoint.m_y + richViewRect.getHeight();
	if ( lineViewRect.m_Top >= m_pRichText->m_viewStartPoint.m_y && lineViewRect.m_Bottom <= fRichBottomestY )
	{
		return false;
	}

	float fCanScrollAdjustY		= ( m_pRichText->m_nLineInterval + 2.0f ) * g_pFrameMgr->GetScreenScaleY();
	float fCanScrollBottomestY	= fRichBottomestY + fCanScrollAdjustY;
	return lineViewRect.m_Top < fCanScrollBottomestY;
}

void RRichTextBuilder::UpdateNotAutoExtendRichAfterBuildOneLine( const RichTextLine* pOneLine )
{
	const Ogre::RectFloat& richViewRect = m_pRichText->m_rect;
	const Ogre::RectFloat& lineViewRect = pOneLine->m_rect;
	if ( NeedScrollNotAutoExtendRichAfterBuildOneLine( pOneLine ) )
	{
		// 自动滚屏幕，让新行显示在区域最下面
		float fY = lineViewRect.m_Bottom - richViewRect.getHeight();
		m_pRichText->SetDispPos( fY );
	}
}

void RRichTextBuilder::UpdateRichAfterBuildOneLine( const RichTextLine* pOneLine )
{
	if ( m_pRichText->m_bAutoExtend )
	{
		UpdateAutoExtendRichAfterBuildOneLine( pOneLine );
	}
	else
	{
		UpdateNotAutoExtendRichAfterBuildOneLine( pOneLine );
	}
}

void RRichTextBuilder::BuildText(const char *szText, RichText *pRichText, const ColorQuad& color)
{
	if ( !CanBuildText( szText, pRichText ) )
	{
		return;
	}

	Init( szText, pRichText, color );
	RemoveRedundantLineBeforeBuildNewText();
	m_bCenter	= false;
	while( true )
	{
		RichTextLine* pOneLine		= NULL;
		bool isSuccessBuildNewLine	= BuildNewOneLine( pOneLine );
		if( !isSuccessBuildNewLine )
		{
			break;
		}

		m_pRichText->m_listLines.push_back( pOneLine );
		UpdateRichAfterBuildOneLine( pOneLine );
	}
}
