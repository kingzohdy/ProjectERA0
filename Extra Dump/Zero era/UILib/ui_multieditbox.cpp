#include "ui_multieditbox.h"
#include "ui_framemgr.h"

using namespace Ogre;
//-----------------------------------------------------------------------------
// MultiEditBox control
//-----------------------------------------------------------------------------
IMPLEMENT_UIOBJECT_CLONE( MultiEditBox )

MultiEditBox::MultiEditBox() : m_pSlider( NULL )
{
	m_MaxChar		= 1000;
	m_RowSpace		= 0;

	m_CursorPos		= 0;
	m_LineBegin		= 0;
	m_SelBegin		= 0;

	m_CursorTimer	= 0;
	m_CursorFlag	= 1;
	m_ReturnFlag	= 1;
	m_FontIndex		= 10;
	m_CursorLive	= 700;
	m_CursorDeath	= 400;
	m_SliderName	= "";
	m_MaxWordCount	= -1;

	//黑底白字
	m_BkColor		= ColorQuad(0,0,0,255);
	m_DrawColor		= ColorQuad(255,255,255,255);
	m_EditSelColor	= ColorQuad(0,0,255,128);
	m_CurSorColor	= ColorQuad(200,200,200,255);
	m_TextSize		= g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h );
	m_Style = FRMSTYLE_EDITABLE;
}

MultiEditBox::~MultiEditBox()
{

}

void MultiEditBox::CopyMembers(MultiEditBox *pdest)
{
	if (pdest)
	{
		Frame::CopyMembers(pdest);

		pdest->m_MaxChar		= m_MaxChar;
		pdest->m_RowSpace		= m_RowSpace;

		pdest->m_CursorPos		= m_CursorPos;
		pdest->m_LineBegin		= m_LineBegin;
		pdest->m_SelBegin		= m_SelBegin;

		pdest->m_CursorTimer	= m_CursorTimer;
		pdest->m_CursorFlag		= m_CursorFlag;
		pdest->m_ReturnFlag		= m_ReturnFlag;
		pdest->m_FontIndex		= m_FontIndex;
		pdest->m_CursorLive		= m_CursorLive;
		pdest->m_CursorDeath	= m_CursorDeath;
		
		pdest->m_BkColor		= m_BkColor;
		pdest->m_DrawColor		= m_DrawColor;
		pdest->m_EditSelColor	= m_EditSelColor;
		pdest->m_CurSorColor	= m_CurSorColor;
		pdest->m_TextSize		= m_TextSize;
		pdest->m_SliderName		= m_SliderName;
		pdest->m_MaxWordCount	= m_MaxWordCount;
		return;
	}
}

void MultiEditBox::FillDrawItems()
{
	if (m_bShow)
	{
		if (m_pParent)
		{
			m_iDrawLevel = m_pParent->m_iDrawLevel + 1;
		}
		g_pFrameMgr->AddDrawItems( static_cast<LayoutFrame *>( this ) );

		Frame::FillChildren();
	}
}

void MultiEditBox::Draw()
{
	Frame::Draw();

	// 设置当前使用的字体 m_FontIndex 号字体
	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;

	// 绘制可视区域的字符
	RectFloat rc;
	rc.m_Top	= (float)m_AbsRect.m_Top;
	rc.m_Left	= (float)m_AbsRect.m_Left;
	rc.m_Bottom = (float)m_AbsRect.m_Bottom;
	rc.m_Right	= (float)m_AbsRect.m_Right;

	// 选择范围
	int selBegin	= min( m_SelBegin, m_CursorPos );
	int selEnd		= max( m_SelBegin, m_CursorPos );

	// 前一个字符的打印结束X位置
	int nCharEndXPos = 0; //ex

	// 前一个字符的打印结束Y位置
	int nCharSelYPos = 0; //sy

	//光标位置
	Point2D cursorPos(0,0);

	for( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{
		if( m_RichCaption[i].line < m_LineBegin )
		{
			continue;
		}

		float fCharWidth, fCharHight;
		g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );
		
		//光标
		if( i == m_CursorPos ) 
		{
			cursorPos = Point2D( nCharEndXPos, nCharSelYPos );
		}

		// 绘制换行符号完毕 或 超出打印区域,换行
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
		}

		// 如果纵向超出，结束
		if( ( nCharSelYPos + m_TextSize ) > m_AbsRect.getHeight() )	
		{
			break;
		}

		rc.m_Left	= (float)(m_AbsRect.m_Left + nCharEndXPos);
		rc.m_Top	= (float)(m_AbsRect.m_Top + nCharSelYPos);
		g_pDisplay->renderTextRect( h, FONTSYTLE_NORMAL, m_RichCaption[i].keybuf, rc, 0, 0, false, m_DrawColor, 1.0f );		

		//绘制选择条		
		if( ( i >= selBegin )&&( i < selEnd ) )
		{
			g_pDisplay->BeginDraw( g_pDisplay->GetNullTexture() );
			if( ( nCharSelYPos + m_TextSize + m_RowSpace ) > m_AbsRect.getHeight() )
			{
				g_pDisplay->DrawRect( m_AbsRect.m_Left + nCharEndXPos, m_AbsRect.m_Top + nCharSelYPos, 
										(int)fCharWidth, (int)fCharHight, m_EditSelColor.c );
			}
			else
			{
				g_pDisplay->DrawRect( m_AbsRect.m_Left + nCharEndXPos, m_AbsRect.m_Top + nCharSelYPos, 
										(int)fCharWidth, (int)fCharHight, m_EditSelColor.c );
			}
			g_pDisplay->EndDraw();
		}

		//记录打印结束位置
		nCharEndXPos += (int)fCharWidth;
	}
	
	//光标
	if( (int)m_RichCaption.size() == m_CursorPos )
	{
		cursorPos = Ogre::Point2D( nCharEndXPos, nCharSelYPos );
	}

	//绘制光标
	if( IsCursorVisible() )
	{
		g_pDisplay->BeginDraw( g_pDisplay->GetNullTexture() );
		g_pDisplay->DrawRect( m_AbsRect.m_Left + cursorPos.x, m_AbsRect.m_Top + cursorPos.y, 2, 
								g_pDisplay->GetFontHeight( h ), m_CurSorColor.c );
		g_pDisplay->EndDraw();
	}
}

bool MultiEditBox::IsCursorVisible()
{
	//绘制光标
	bool bCursorVis = false;
	if( m_SelBegin == m_CursorPos )
	{
		if( g_pFrameMgr->GetFocusFrame() == this )
		{
			bCursorVis = true;
		}
	}	
	if( bCursorVis && m_CursorFlag ) 
	{
		return true;
	}else{ 
		return false;
	}
}

// 确保光标位置合理
void MultiEditBox::UpdateSelf( float deltatime )
{
	m_TextSize = g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h );
	CheckForReason();

	// 闪烁光标
	if( g_pFrameMgr->GetFocusFrame() == this )
	{
		int dtick = int( deltatime*1000 );
		m_CursorTimer += dtick;
		if( ( m_CursorFlag && m_CursorTimer > m_CursorLive ) || ( !m_CursorFlag && m_CursorTimer > m_CursorDeath ) )
		{
			m_CursorFlag	= 1 - m_CursorFlag;
			m_CursorTimer	= 0;
		}
	}
	else 
	{
		m_CursorFlag	= 1;
		m_CursorTimer	= 0;
	}

	if( IsUpdateDirty() )
	{
		SetUpdateDirty( false );
		AjustForReason();
		UpdateScrollBar();
	}
}

int MultiEditBox::OnInputMessage( const Ogre::InputEvent &event )
{
	switch( event.msg )
	{
	case GIE_CHAR:
		{
			return	OnChar( event );
		}

	case GIE_KEYDOWN:
		{
			return OnKeyDown( event );
		}

	case GIE_MOUSEMOVE:
		{
			int x = event.mouse.x;
			int y = event.mouse.y;

			if( m_State & FRMSTATE_CLICKDOWN )
			{
				Point2D pos( event.mouse.x, event.mouse.y );

				if( IsPointInside( m_AbsRect, pos.x, pos.y ) )
				{
					pos.x -= m_AbsRect.m_Left;
					pos.y -= m_AbsRect.m_Top;

					int idx = PosToChar( pos );
					MoveCursor( idx );
				}
			} 
		}
		return 0;

	case GIE_LBTNUP:
		{
			if( m_State & FRMSTATE_CLICKDOWN )
			{	
				m_State &= ~FRMSTATE_CLICKDOWN;
			}	
		}
		return 0;

	case GIE_LBTNDBLCLK:
		{		
			SelectAllText();
		}
		return 0;

	case GIE_LBTNDOWN:
		{
			m_State |= FRMSTATE_CLICKDOWN;	

			Ogre::Point2D pos( event.mouse.x, event.mouse.y );

			if( IsPointInside( m_AbsRect, pos.x, pos.y ) )
			{
				pos.x -= m_AbsRect.m_Left;
				pos.y -= m_AbsRect.m_Top;

				int idx = PosToChar( pos );
				MoveCursor( idx );
			}

			MoveSelBegin( m_CursorPos );
		}
		return 0;
	

	case GIE_LOSTFOCUS:
		{
			CancelSel( true, false );
		}

		return 0;		

	default:
		{
			return Frame::OnInputMessage( event );
		}
	}
	return 1;
}

int MultiEditBox::OnChar( const Ogre::InputEvent &event, bool bVirtual )
{
	if( !(m_Style&FRMSTYLE_EDITABLE) )
	{
		return 1;
	}
	char thechar = event.chars.str[0];
	if (hasScriptsEvent(SC_ONCHAR))
	{
		CallScript( SC_ONCHAR, "iii", thechar, m_CursorPos, m_SelBegin );
	}

	//非打印字符
	switch( thechar ) 
	{			
		case VK_RETURN:
			{
				ClearSel();
				
				// 插入换行字符
				if( InputRawChar( '\n' ) )
				{
					CalcCharsLine();
				}

				SetUpdateDirty( true );	
				MoveSelBegin( m_CursorPos );
			}
			return 1;

		case VK_BACK:
			{
				if( ClearSel() )
				{
					break;
				}
				
				EraseRichChar( m_CursorPos - 1 );
				SetUpdateDirty( true );	
				MoveSelBegin( m_CursorPos );
			}
			return 1;

		case VK_ESCAPE:
			{
				if ( hasScriptsEvent(SC_ONESCAPEPRESSED) )
				{
					CallScript( SC_ONESCAPEPRESSED );
					return 0;
				}else{
					return Frame::OnInputMessage( event );
				}
			}
			return 1;

		case VK_TAB:
			{
				if ( hasScriptsEvent(SC_ONTABPRESSED) )
				{
					CallScript( SC_ONTABPRESSED );
					return 0;
				}else{
					return Frame::OnInputMessage( event );
				}
			}
			return 1;
		default:
			break;
	}

	// 非打印字符
	if( !IsPrint( thechar ) || !IsLegal( thechar ) )
	{
		return Frame::OnInputMessage( event );
	}

	if ( hasScriptsEvent( SC_ONQUERYCHAR ) )
	{
		bool isContinueGetChar = true;
		CallFunction( SC_ONQUERYCHAR, "ii>b", m_CursorPos, m_SelBegin, &isContinueGetChar );
		if ( !isContinueGetChar )
		{
			return 0;
		}
	}

	ClearSel();
	// 插入打印字符
	if( InputRawChar( ( unsigned char )thechar ) )
	{
		CalcCharsLine();
	}

	// 调整字符位置
	SetUpdateDirty( true );

	m_SelBegin = m_CursorPos;

	// 截流指定消息,自己处理,不向父亲发送	
	return 0;
}

int MultiEditBox::OnKeyDown( const Ogre::InputEvent &event, bool bVirtual )
{
	SetUpdateDirty( true );

	bool bDefault = false;
	switch( event.key.vkey )
	{
	case VK_RETURN:
		{
			if( (GetKeyState(VK_CONTROL) & 0xff00))
			{
				if (hasScriptsEvent(SC_ONCTRLENTERPRESSED))
				{
					CallScript(SC_ONCTRLENTERPRESSED);
				}
			}
		}
		break;
	case VK_LEFT:
		{
			//按下了ctrl键
			if( event.key.flags&MK_CONTROL )
			{
				Point2D curPos = GetCharPos( m_CursorPos );

				// 得到第0行,第curPos.x列的字符索引
				int curIdx = GetPosChar( 0, curPos.x );

				if( curIdx >= 0 )
				{
					MoveCursor( curIdx );
				}
			}else{
				//没有按下shift键
				if( !( event.key.flags&MK_SHIFT ) )
				{
					if( CancelSel( true ) )	
					{
						break;
					}
				}
				MoveCursor( m_CursorPos - 1 );	
			}

			//没有按下shift键
			if( !( event.key.flags&MK_SHIFT ) )
			{
				MoveSelBegin( m_CursorPos );
			}
		}
		break;

	case VK_RIGHT:
		//按下了ctrl键
		if( event.key.flags&MK_CONTROL )
		{
			Point2D curPos = GetCharPos( m_CursorPos );
			int curIdx = GetPosChar( 1000, curPos.x );
			if( curIdx >= 0 )
			{
				MoveCursor( curIdx + 1 );
			}
		}else{
			//没有按下shift键
			if( !( event.key.flags&MK_SHIFT ) )
			{
				if( CancelSel( false ) )
				{
					break;
				}
			}
			MoveCursor( m_CursorPos + 1 );
		}

		//没有按下shift键
		if( !( event.key.flags&MK_SHIFT ) )
		{
			MoveSelBegin( m_CursorPos );
		}
		break;

	case VK_HOME:

		//按下了ctrl键
		if( ( event.key.flags&MK_CONTROL ) )
		{
			MoveCursor( 0 );
		}else{
			//没有按下shift键
			if( !( event.key.flags&MK_SHIFT ) )
			{
				if(CancelSel(false))
				{
					break;
				}
			}
			Point2D curPos = GetCharPos( m_CursorPos );
			int curIdx = GetPosChar( 0,curPos.x );
			if( curIdx >= 0 )
			{
				MoveCursor( 0 );
			}
		}

		//没有按下shift键
		if( !( event.key.flags&MK_SHIFT ) )
		{
			MoveSelBegin( m_CursorPos );
		}

		break;

	case VK_END:
		//按下了ctrl键
		if( event.key.flags&MK_CONTROL )
		{
			MoveCursor( (int)m_RichCaption.size() );
		}else{
			MoveCursor( (int)m_RichCaption.size() );

		}
		//没有按下shift键
		if( !( event.key.flags&MK_SHIFT ) )
			MoveSelBegin( m_CursorPos );

		break;

	case VK_BACK:
		{
			if (hasScriptsEvent(SC_ONKEYDOWN))
			{
				CallScript( SC_ONKEYDOWN, "iii", event.key.vkey, ( m_CursorPos + 1 ),(  m_SelBegin + 1 ) );
			}
		}
		break;

	case VK_DELETE:
		{
			/*EraseRichChar( m_CursorPos );
			SetUpdateDirty( true );*/
			if (hasScriptsEvent(SC_ONKEYDOWN))
			{
				CallScript( SC_ONKEYDOWN, "iii", event.key.vkey, ( m_CursorPos + 1 ),(  m_SelBegin + 1 ) );
			}

			if(!ClearSel())
			{}
		}
		break;

	case VK_UP:
		{
			Point2D curPos = GetCharPos( m_CursorPos );

			// 得到第curPos.y-1行,第curPos.x列的字符索引
			int curIdx = GetPosChar( curPos.y-1, curPos.x );

			if(curIdx>=0)
			{
				MoveCursor( curIdx );
			}

			//没有按下shift键
			if( !( event.key.flags&MK_SHIFT ) )
			{
				MoveSelBegin( m_CursorPos );
			}
		}
		break;

	case VK_DOWN:
		{
			Ogre::Point2D curPos = GetCharPos( m_CursorPos );

			// 得到第curPos.y+1行,第curPos.x列的字符索引
			int curIdx = GetPosChar( curPos.y + 1, curPos.x );

			if( curIdx >= 0 )
			{
				MoveCursor( curIdx );
			}

			//没有按下shift键
			if( !(event.key.flags&MK_SHIFT) )
			{
				MoveSelBegin( m_CursorPos );
			}
		}
		break;

	case VK_V:
		{
			if( ( GetKeyState(VK_CONTROL) & 0xff00 ) )
			{
				OnPaste();
				SetUpdateDirty( true );
			}
		}
		break;

	case VK_C:
		{
			if( ( GetKeyState(VK_CONTROL) & 0xff00 ) )
			{
				OnCopy();
				SetUpdateDirty( true );
			}
		}
		break;

	case VK_X:
		{
			if( ( GetKeyState(VK_CONTROL) & 0xff00 ) )
			{
				OnCut();
				SetUpdateDirty( true );
			}
		}
		break;
	case VK_ESCAPE:
		{
			if (hasScriptsEvent(SC_ONESCAPEPRESSED))
			{
				CallScript(SC_ONESCAPEPRESSED);				
			}
		}
		break;
	
	default:
		{
			bDefault = true;
		}
	}
	return 0;
}

// 粘贴选择的字符
void MultiEditBox::OnPaste()
{
	if( !(m_Style&FRMSTYLE_EDITABLE) )
	{
		return;
	}
	ClearSel();

	if( !::OpenClipboard( NULL ) )
	{
		return;
	}
	HANDLE handle = ::GetClipboardData( CF_TEXT );
	::CloseClipboard();
	if( !handle )
	{
		return;
	}

	std::string  caption = (const char*)handle;
	for( int i = 0, size = (int)caption.size(); i < size; i++ )
	{
		//超过最多文字限制
		if( ParseMaxSize() >= m_MaxChar || ( m_MaxWordCount > 0 && getTextCount() >= m_MaxWordCount ) ) 
		{
			break;
		}
		// 插入打印字符
		if( InputRawChar( caption[i] ) )
		{
			CalcCharsLine();
		}
	}
	MoveSelBegin( m_CursorPos );
	SetUpdateDirty( true );
}

// 复制选择的字符
void MultiEditBox::OnCopy()
{
	if( m_SelBegin != m_CursorPos )
	{
		//得到选择文本
		int selBegin	= min( m_SelBegin, m_CursorPos );
		int selEnd		= max( m_SelBegin, m_CursorPos );

		std::string selText;
		GetRawString( selText,selBegin, selEnd - selBegin );
		if( selText.empty() ) 
		{
			return;
		}

		//得到共享内存
		HGLOBAL hGlobal = GlobalAlloc( GHND|GMEM_SHARE, selText.size() + 1 );
		if(!hGlobal)
		{
			return;
		}

		//填充共享内存
		LPVOID pGlobal = GlobalLock(hGlobal);
		strncpy( (char*)pGlobal, selText.c_str(),selText.size() + 1 );
		GlobalUnlock( hGlobal );

		//设置剪贴板
		if( !::OpenClipboard(NULL) )
		{
			return;
		}

		::EmptyClipboard();
		::SetClipboardData( CF_TEXT, hGlobal );
		::CloseClipboard();
	}		
}

// 剪切选择的字符
void MultiEditBox::OnCut()
{
	OnCopy();
	ClearSel();
	SetUpdateDirty( true );
}


// 调整各个指标的范围使他们保持在合理的范围内
void MultiEditBox::AjustForReason()
{
	// 若当前光标位置超出字符串最大范围,将光标位置置到最后否则置成0
	if( m_CursorPos > (int)m_RichCaption.size() ) 
	{
		m_CursorPos = (int)m_RichCaption.size();
	}else if( m_CursorPos < 0 ){
		m_CursorPos = 0;
	}
	
	// 若当前选中的字符串开始位置超出字符串最大范围,将位置置到最后否则置成0
	if( m_SelBegin > (int)m_RichCaption.size() ) 
	{
		m_SelBegin = (int)m_RichCaption.size();
	}else if( m_SelBegin < 0 ){ 
		m_SelBegin = 0;
	}
	
	// 最多容纳行数
	int lineNum = m_AbsRect.getHeight()/( g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h ) );

	// 计算每个richchar所在的行
	CalcCharsLine();	
	
	//光标所在行
	Ogre::Point2D cursorPos = GetCharPos( m_CursorPos );
	int cursorLine = cursorPos.y;

	if( cursorLine < m_LineBegin )
	{
		m_LineBegin = cursorLine;
	}else if( ( cursorLine + 1 ) > ( lineNum + m_LineBegin ) ){
		m_LineBegin = ( cursorLine + 1 - lineNum );
	}
	
	if( !m_SliderName.empty() )
	{
		if( m_pSlider == NULL )
		{
			// 滚动条必须初始化
			IniMultiEditSlider();
		}
		m_pSlider->SetValue( (float)m_LineBegin );
	}
}

//确保光标位置合理
void MultiEditBox::CheckForReason()
{
	assert( m_SelBegin >= 0 );
	assert( m_LineBegin >= 0 );
	assert( ( m_CursorPos >= 0 ) && ( m_CursorPos <= (int)m_RichCaption.size() ) );

	//确保光标位置合理
	if( m_SelBegin < 0 )
	{
		m_SelBegin = 0;
	}

	if( m_CursorPos < 0 )
	{
		m_CursorPos = 0;
	}else if( m_CursorPos > (int)m_RichCaption.size() ){
		m_CursorPos = (int)m_RichCaption.size();
	}

	if( m_LineBegin < 0 )
	{
		m_LineBegin = 0;
	}
}

void MultiEditBox::UpdateScrollBar()
{
	//根据计算得出的内容区高度来调整竖直滚动条	
	int nFontHight = g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h );

	//高度可以容纳的行数
	int nContentLines = m_AbsRect.getHeight()/nFontHight;

	//当前需要显示的行数
	int dispLines = 0;

	if( (int)m_RichCaption.size() > 0 )
	{
		dispLines = m_RichCaption[m_RichCaption.size()-1].line + 1;
	}

	int nLower = dispLines - nContentLines;
	if( nLower < 0 )
	{
		nLower = 0;
		m_LineBegin = 0;
	}

	if( !m_SliderName.empty() )
	{
		if( nLower <= 0 )
		{
			nLower = 0;
			m_LineBegin = 0;
			m_pSlider->Hide();
		}else{
			m_pSlider->Show();
		}
		m_pSlider->SetMaxValue( (float)nLower );
	}
}

// 得到某个字符行列位置(row-y,col-x)
Ogre::Point2D MultiEditBox::GetCharPos( int index )
{
	assert( index >= 0 );

	// 前一个rich字符的打印结束X位置
	int nCharEndXPos = 0;

	// 前一个rich字符的打印结束Y位置
	int nCharSelYPos = 0;

	Point2D linePos(0,0);
	const UIFont& oneUIFont = g_pFrameMgr->getUIFontByIndex( m_FontIndex );

	for ( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{ 
		const stRichChar& oneRichChar = m_RichCaption[i];
		float fCharWidth = 0.0f, fCharHight = 0.0f;
		g_pDisplay->GetTextExtent( oneUIFont.h, oneRichChar.keybuf, fCharWidth, fCharHight );

		//打印一个字符
		if( index == i )
		{
			break;
		}

		// 绘制换行符号完毕 或 超出打印区域,换行
		if ( oneRichChar.type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			linePos.x = 0;
			linePos.y++;
		}

		//下一个字符在当前行打印位置
		linePos.x++;

		//记录打印结束位置
		nCharEndXPos += (int)fCharWidth;
	}

	return linePos;
}

//pos为相对于captionrect的坐标
//返回光标位置索引
int MultiEditBox::PosToChar( Ogre::Point2D pos )
{
	Ogre::RectInt captionRect = m_AbsRect;

	//pos位于哪一行
	const UIFont& oneUIFont = g_pFrameMgr->getUIFontByIndex( m_FontIndex );
	int line = pos.y / g_pDisplay->GetFontHeight( oneUIFont.h );

	int lineIndex = 0;

	int result = (int)m_RichCaption.size();

	// 前一个rich字符的打印结束X位置
	int nCharEndXPos = 0;

	// 前一个rich字符的打印结束Y位置
	int nCharSelYPos = 0;

	for( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{
		stRichChar& oneRichChar  = m_RichCaption[i];
		float fCharWidth, fCharHight;
		g_pDisplay->GetTextExtent( oneUIFont.h, oneRichChar.keybuf, fCharWidth, fCharHight );

		if( oneRichChar.line < m_LineBegin )
		{
			continue;
		}

		if( lineIndex > line ) 
		{
			break;
		}else if( lineIndex == line ){
			if( pos.x >= nCharEndXPos )
			{
				if( pos.x <= nCharEndXPos + fCharWidth*0.5 )
				{
					result = i;
					break;
				}
				else if( ( pos.x > nCharEndXPos + fCharWidth*0.5 ) && ( pos.x <= nCharEndXPos + fCharWidth ) )
				{
					result = i + 1;
					break;
				}
				else if( oneRichChar.type == RICH_CHAR_LF )
				{
					result = i;
					break;
				}
				else 
				{
					result = i + 1;
				}
			}
		}
		//记录打印结束位置
		nCharEndXPos += (int)fCharWidth;
		
		// 绘制换行符号完毕 或 超出打印区域,换行
		if( oneRichChar.type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos =0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			lineIndex++;
		}

		//如果纵向超出，结束
		if( ( nCharSelYPos + m_TextSize ) > m_AbsRect.getHeight() )	break;
	}
	return result;
}

//得到第row行,第col列的字符索引
int MultiEditBox::GetPosChar(int row, int col)
{
	if( row < 0 ) 
	{
		row = 0;
	}

	if( col < 0 ) 
	{
		col = 0;
	}

	// 前一个rich字符的打印结束X位置
	int nCharEndXPos = 0;

	// 前一个rich字符的打印结束Y位置
	int nCharSelYPos = 0;

	Point2D linePos(0,0);

	for( int i = 0; i < (int)m_RichCaption.size(); i++ )
	{
		float fCharWidth, fCharHight;
		HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;
		g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );

		// 绘制换行符号完毕 或 超出打印区域,换行
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );

			//即将换行
			if( row == linePos.x ) 
			{
				return i;
			}

			linePos.x++;
			linePos.y = 0;
		}	

		//当前打印字符行列匹配
		if( ( row == linePos.x)&&( col == linePos.y ) ) 
		{
			return i;
		}

		//下一个字符在当前行打印位置
		linePos.y++;

		//记录打印结束位置
		nCharEndXPos += (int)fCharWidth;

		//搜索结束了,还没有找到
		if( ( i + 1 ) == (int)m_RichCaption.size() )
		{
			if( row == linePos.x ) 
			{
				return (int)m_RichCaption.size();
			}
		}
	}
	return -1;
}

// 计算每个richchar所在的行
void MultiEditBox::CalcCharsLine()
{
	// 前一个rich字符的打印结束X位置
	int nCharEndXPos = 0;

	// 前一个rich字符的打印结束Y位置
	int nCharSelYPos = 0;

	// 当前行索引
	int nlineIndex = 0;

	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;

	int nMaxLine = m_AbsRect.getHeight() / ( g_pDisplay->GetFontHeight( h ) );

	for( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{ 
		float fCharWidth, fCharHight;
		g_pDisplay->GetTextExtent( h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );

		// 绘制换行符号完毕 或 超出打印区域,换行
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			// 若换行宽度清成0 高度累加(当前字体的高度)
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			nlineIndex++;
		}

		m_RichCaption[i].line = nlineIndex;
		if( m_RichCaption[i].type == RICH_CHAR_LF )
		{
			//m_RichCaption[i].line--;
		}
		
		// 记录打印结束位置
		nCharEndXPos += (int)fCharWidth;
	}
}

// 在光标位置插入，如果遇到汉字字符，检查前面是否是半个汉字如果有，合并为一个汉字
bool MultiEditBox::InputRawChar(unsigned char rawChar)
{
	if( !IsInputEnable( rawChar ) )
	{
		return false;
	}

	bool bNeedUpdate = false;
	// 换行符号
	if( rawChar == '\n')
	{
		stRichChar rChar;
		rChar.type		= RICH_CHAR_LF;
		rChar.width		= 0;
		rChar.keybuf[0] = '\n';
		
		bNeedUpdate		= InsertRichChar( m_CursorPos, rChar );
	}

	// 英文或者汉字后字节
	else if( rawChar <= 128 )
	{
		// 如果前面有半个汉字，添加到一起，组成完整的汉字
		if( IsHalfDBCSPre( m_CursorPos ) )
		{
			m_RichCaption[m_CursorPos-1].type		= RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]	= rawChar;
			m_RichCaption[m_CursorPos-1].width		= m_TextSize;
		}

		// 如果前面没有半个汉字，直接插入英文 （IsPrint 判断是否打印字符）
		else if( IsPrint( rawChar ) )
		{
			stRichChar rChar;
			rChar.type		= RICH_CHAR_CHAR;
			rChar.width		= (int)(m_TextSize*0.5);
			rChar.keybuf[0]	= rawChar;

			bNeedUpdate		= InsertRichChar( m_CursorPos, rChar );	
		}
	}

	// 汉字，看前面是否有半个汉字	
	else if( rawChar > 128 )
	{
		// 如果前面有半个汉字，添加到一起，组成完整的汉字
		if( IsHalfDBCSPre( m_CursorPos ) )
		{
			m_RichCaption[m_CursorPos-1].type		= RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]	= rawChar;
			m_RichCaption[m_CursorPos-1].width		= m_TextSize;
		}

		// 如果前面没有半个汉字，直接插入半个汉字
		else 
		{
			stRichChar rChar;
			rChar.type		= RICH_CHAR_HALF;
			rChar.width		= (int)(m_TextSize*0.5);
			rChar.keybuf[0]	= rawChar;

			bNeedUpdate		= InsertRichChar( m_CursorPos, rChar );		
		}
	}
	if (hasScriptsEvent(SC_ONTEXTSET))
	{
		CallScript( SC_ONTEXTSET );
	}
	return bNeedUpdate;
}

// 判断前一个字符是否半个汉字
bool MultiEditBox::IsInputEnable( unsigned char rawChar )
{
	// 超过最大字符数目限制
	if( ParseMaxSize() >= m_MaxChar || getTextCount() >= m_MaxWordCount ) 
	{
		return false;
	}

	// 如果有设置滚动条则允许输入
 	if( !m_SliderName.empty() )
	{
		return true;
	}

	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;
	
	// 可视区最大允许行数
	int nMaxLine = m_AbsRect.getHeight() / ( g_pDisplay->GetFontHeight( h ) );

	// 光标所在行的字符串
	std::string strLastLineChars;

	// 当前输入的字符
	char szChar[UI_KEYCHAR_MAX];
	memset( szChar, 0, UI_KEYCHAR_MAX );
	szChar[0] = rawChar;
	
	// 若已达到最大行数
	if( !m_RichCaption.empty() && m_RichCaption[m_RichCaption.size()-1].line >= nMaxLine - 1 )
	{
		if( rawChar == '\n')
		{
			// 不允许换行
			return false;
		}else{

			// 取到当前光标所在的行列位置
			Ogre::Point2D CurPos = GetCharPos( m_CursorPos );

			// 取到光标所在行的字符总数
			int nLastLineChars = GetCharsInLine( CurPos.y );

			// 取到光标所在行的第一个字符的索引
			int nIndexChar = GetPosChar( CurPos.y, 1 );

			// 取到光标所在行的字符串
			GetRawString( strLastLineChars, nIndexChar - 1, nLastLineChars );
			
			// 取到当前行字符串的长度
			float fLineWidth, fRawCharWidth, fCharHight;
			g_pDisplay->GetTextExtent( h,strLastLineChars.c_str(), fLineWidth, fCharHight );

			// 取到当前输入字符的长度
			g_pDisplay->GetTextExtent( h,szChar, fRawCharWidth, fCharHight );

			// 当前行的宽度超出可视区宽度则不允许输入
			if( fLineWidth + fRawCharWidth > m_AbsRect.getWidth() )
			{
				return false;
			}
		}
	}
	return true;
}

// 判断前一个字符是否半个汉字
bool MultiEditBox::IsHalfDBCSPre(int pos)
{
	bool bHalfDBCS = false;
	if( pos > 0 && pos <= (int)m_RichCaption.size() )
	{
		if(	( m_RichCaption[pos-1].type == RICH_CHAR_HALF )	)
		{
			bHalfDBCS=true;
		}
	}
	return bHalfDBCS;
}

// 在指定位置(pos)插入一个rich char，同时，自动调整光标
bool MultiEditBox::InsertRichChar( int pos, stRichChar rChar )
{
	// 若指定的位置(pos)在合理范围则新字符插入到m_RichCaption
	if( pos >= 0 && pos <= (int)m_RichCaption.size() )
	{
		if( pos <= m_SelBegin )  
		{
			m_SelBegin++;
		}

		if(pos <= m_CursorPos)
		{
			m_CursorPos++;
		}
		m_RichCaption.insert( m_RichCaption.begin() + pos, rChar );
		return true;
	}
	else 
	{
		return false;
	}
}

//在指定位置删除一个rich char，同时，自动调整光标
bool MultiEditBox::EraseRichChar( int pos )
{
	if( pos >= 0 && pos < (int)m_RichCaption.size() )
	{
		if( m_CursorPos > pos ) 
		{
			m_CursorPos--;
		}	
		m_RichCaption.erase( m_RichCaption.begin() + pos );
		if (hasScriptsEvent(SC_ONTEXTSET))
		{
			CallScript( SC_ONTEXTSET);
		}
		return true;
	}else{
		return false;
	}
}

// 获取m_RichCaption存储的全部字符串的大小
int MultiEditBox::ParseMaxSize()
{
	int size=0;
	for( int i = 0, len = (int)m_RichCaption.size();i < len; i++ )
	{ 
		size += (int)strlen( m_RichCaption[i].keybuf );
	}
	return size;
}

// 设置输入框文本
void MultiEditBox::SetText(const char *ptext)
{
	if (ptext)
	{
		Clear();	
		m_Caption	= ptext;
		m_SelBegin	= m_CursorPos = 0;
		for( int i = 0; i < (int)strlen(ptext); i++ )
		{
			InputRawChar( ptext[i] );
		}
	}
}

void MultiEditBox::AddText(const char *ptext)
{
	if (ptext)
	{
		for( int i = 0; i < (int)strlen(ptext); i++ )
		{
			InputRawChar( ptext[i] );
		}
	}
}

void MultiEditBox::SetTextColor( int r, int g, int b )
{
    m_DrawColor = ColorQuad(r, g, b, 255);
}

// 获取m_RichCaption存储的全部字符串
const char* MultiEditBox::GetText()
{
	GetRawString( m_Caption, 0, (int)m_RichCaption.size() );
	return m_Caption.c_str();
}

// 中文输入状态
void MultiEditBox::enableIME( bool b )
{
	static HIMC m_hImc = (HIMC) g_pFrameMgr->getImeContext() ;	
	if( !b )
	{
		ImmAssociateContext( (HWND)g_pFrameMgr->getWndHandle(), NULL ); 
	}else{
		ImmAssociateContext( (HWND)g_pFrameMgr->getWndHandle(), m_hImc ); 
	}
}

// 获取m_RichCaption第start个位置开始的length个的全部字符串
void MultiEditBox::GetRawString( std::string& strBuf, int start, int length )
{
	// 若开始位置不合理则重置成0
	if( start < 0 || start >= (int)m_RichCaption.size() )
	{
		start = 0;
	}
	
	// 若截取的长度不合理则置成总长度减去传入的起始位置
	if( length < 0 || ( start + length ) > (int)m_RichCaption.size())
	{
		length = (int)m_RichCaption.size() - start;
	}
	
	// 合并start位置开始的length长度的字符串
	strBuf = "";
	for( int i = start; i < ( start + length ); i++ )
	{ 
		strBuf += m_RichCaption[i].keybuf;
	}
}

// 得到某一行的字符数目
int MultiEditBox::GetCharsInLine( int line )
{
	// 前一个rich字符的打印结束X位置
	int nCharEndXPos = 0;

	// 前一个rich字符的打印结束Y位置
	int nCharSelYPos = 0;

	// 当前行索引
	int nlineIndex = 0;

	int num = 0;

	for( int i=0, size = (int)m_RichCaption.size(); i < size; i++ )
	{ 
		if( nlineIndex > line )
		{
			break;
		}

		float fCharWidth, fCharHight;
		HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;
		g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );

		// 绘制换行符号完毕 或 超出打印区域,换行
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			nlineIndex++;
		}

		if( nlineIndex == line )
		{
			num++;
		}
		// 记录打印结束位置
		nCharEndXPos += (int)fCharWidth;
	}
	return num;
}
// 选择开始位置pos表示：第一个被选择rich字符为第pos个rich字符
// 注意selend就是cursorpos，表示：最后一个被选择字符为cusorpos-1
void MultiEditBox::MoveSelBegin( int pos )
{
	int nCharNum = (int)m_RichCaption.size();
	if( pos < 0 ) 
	{
		pos = 0;
	}else if( pos > nCharNum ){
		pos = nCharNum;
	}
	m_SelBegin = pos;
}


//光标的位置pos表示：光标在第pos个rich字符前面
void MultiEditBox::MoveCursor( int pos )
{
	int nCharNum = (int)m_RichCaption.size();
	if( pos < 0 )
	{
		pos = 0;
	}else if( pos > nCharNum ){ 
		pos = nCharNum;
	}
	m_CursorPos = pos;
}

// 清空编辑框
void MultiEditBox::Clear()
{
	m_RichCaption.clear();
	MoveCursor(0);
	m_LineBegin = 0;
	MoveSelBegin(0);
	CallScript( SC_ONTEXTSET );
}

// 设置光标选区
int MultiEditBox::SetSel(int start,int end)
{
	int nCharNum = (int)m_RichCaption.size();
	if( start < 0 ) 
	{
		start = 0;
	}else if( start > nCharNum ){
		start = nCharNum;
	}

	if( end < 0 )
	{
		end = 0;
	}else if( end > nCharNum ){
		end = nCharNum;
	}

	MoveSelBegin( start );
	MoveCursor( end );

	return 1;
}

// 清除光标选区内的字符
int MultiEditBox::ClearSel()
{
	if( m_SelBegin != m_CursorPos )
	{
		int selBegin = min( m_SelBegin, m_CursorPos );
		int selEnd	= max( m_SelBegin, m_CursorPos );

		for( int i = selEnd - 1; i >= selBegin; i-- )
		{
			EraseRichChar( i );
		}

		MoveCursor( selBegin );
		MoveSelBegin( selBegin );
		SetUpdateDirty( true );		
		return 1;
	}else{
		return 0;
	}
}

//bLeft:是否将光标靠左 bMove:是否移动光标
int MultiEditBox::CancelSel( bool bLeft, bool bMove )
{
	if( m_SelBegin != m_CursorPos )
	{
		int selBegin	= min( m_SelBegin, m_CursorPos );
		int selEnd		=  max( m_SelBegin, m_CursorPos );

		if( bMove )
		{
			MoveCursor( ( bLeft )?selBegin:selEnd );
		}

		MoveSelBegin( m_CursorPos );

		return 1;
	}
	else return 0;
}

// 设置是否调整字符位置
void MultiEditBox::SetUpdateDirty( bool bDirty)
{ 
	m_bDirty = bDirty;
}

// 根据滚动条的value设置m_RichCaption从第value行显示
void MultiEditBox::SetSliderValue( int nValue )
{
	m_LineBegin = nValue;
	SetUpdateDirty( false );
}

void MultiEditBox::IniMultiEditSlider()
{
	m_pSlider = static_cast<Slider *>( g_pFrameMgr->FindLayoutFrame( m_SliderName ) );
	assert( m_pSlider != NULL );
}

unsigned int MultiEditBox::getTextCount()
{
	GetRawString(m_Caption,0,(int)m_RichCaption.size());
	return m_RichCaption.size();
}

// 全选
void MultiEditBox::SelectAllText()
{
	SetSel( 0, (int)m_RichCaption.size() );
}

void MultiEditBox::enableEdit( bool b )
{
	if( b )
	{
		m_Style |= FRMSTYLE_EDITABLE;
	}else{
		m_Style &= ~FRMSTYLE_EDITABLE;
	}
}

int MultiEditBox::getCursorPos()
{
	return m_CursorPos;
}

void MultiEditBox::setCursorPos( int nPos )
{
	if ( nPos < 0 )
	{
		nPos = 0;
	}
	m_CursorPos = nPos;
}

int MultiEditBox::getSelBegin()
{
	return m_SelBegin;
}

void MultiEditBox::setSelBegin( int nPos )
{
	if ( nPos < 0 )
	{
		nPos = 0;
	}
	m_SelBegin = nPos;
}

int MultiEditBox::getTextBegin( const char* pszText, int nFindPos )
{
	/*for ( int nIndex = nFindPos, size = m_RichCaption.size(); nIndex < size; ++nIndex )
	{
	}*/

	return 0;
}

int MultiEditBox::getTextEnd( const char* pszText, int nFindPos )
{
	return 0;
}

int MultiEditBox::getTextIndexFromRichCharIndex( int nRichCharIndex )
{
	int nTextIndex = 0;
	for ( int i = 0; i < nRichCharIndex; ++i )
	{
		const stRichChar& oneRichChar = m_RichCaption[i];
		nTextIndex += strlen( oneRichChar.keybuf );
	}

	return nTextIndex;
}