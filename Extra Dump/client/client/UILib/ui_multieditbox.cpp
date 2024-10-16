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

	//�ڵװ���
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

	// ���õ�ǰʹ�õ����� m_FontIndex ������
	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;

	// ���ƿ���������ַ�
	RectFloat rc;
	rc.m_Top	= (float)m_AbsRect.m_Top;
	rc.m_Left	= (float)m_AbsRect.m_Left;
	rc.m_Bottom = (float)m_AbsRect.m_Bottom;
	rc.m_Right	= (float)m_AbsRect.m_Right;

	// ѡ��Χ
	int selBegin	= min( m_SelBegin, m_CursorPos );
	int selEnd		= max( m_SelBegin, m_CursorPos );

	// ǰһ���ַ��Ĵ�ӡ����Xλ��
	int nCharEndXPos = 0; //ex

	// ǰһ���ַ��Ĵ�ӡ����Yλ��
	int nCharSelYPos = 0; //sy

	//���λ��
	Point2D cursorPos(0,0);

	for( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{
		if( m_RichCaption[i].line < m_LineBegin )
		{
			continue;
		}

		float fCharWidth, fCharHight;
		g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );
		
		//���
		if( i == m_CursorPos ) 
		{
			cursorPos = Point2D( nCharEndXPos, nCharSelYPos );
		}

		// ���ƻ��з������ �� ������ӡ����,����
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
		}

		// ������򳬳�������
		if( ( nCharSelYPos + m_TextSize ) > m_AbsRect.getHeight() )	
		{
			break;
		}

		rc.m_Left	= (float)(m_AbsRect.m_Left + nCharEndXPos);
		rc.m_Top	= (float)(m_AbsRect.m_Top + nCharSelYPos);
		g_pDisplay->renderTextRect( h, FONTSYTLE_NORMAL, m_RichCaption[i].keybuf, rc, 0, 0, false, m_DrawColor, 1.0f );		

		//����ѡ����		
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

		//��¼��ӡ����λ��
		nCharEndXPos += (int)fCharWidth;
	}
	
	//���
	if( (int)m_RichCaption.size() == m_CursorPos )
	{
		cursorPos = Ogre::Point2D( nCharEndXPos, nCharSelYPos );
	}

	//���ƹ��
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
	//���ƹ��
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

// ȷ�����λ�ú���
void MultiEditBox::UpdateSelf( float deltatime )
{
	m_TextSize = g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h );
	CheckForReason();

	// ��˸���
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

	//�Ǵ�ӡ�ַ�
	switch( thechar ) 
	{			
		case VK_RETURN:
			{
				ClearSel();
				
				// ���뻻���ַ�
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

	// �Ǵ�ӡ�ַ�
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
	// �����ӡ�ַ�
	if( InputRawChar( ( unsigned char )thechar ) )
	{
		CalcCharsLine();
	}

	// �����ַ�λ��
	SetUpdateDirty( true );

	m_SelBegin = m_CursorPos;

	// ����ָ����Ϣ,�Լ�����,�����׷���	
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
			//������ctrl��
			if( event.key.flags&MK_CONTROL )
			{
				Point2D curPos = GetCharPos( m_CursorPos );

				// �õ���0��,��curPos.x�е��ַ�����
				int curIdx = GetPosChar( 0, curPos.x );

				if( curIdx >= 0 )
				{
					MoveCursor( curIdx );
				}
			}else{
				//û�а���shift��
				if( !( event.key.flags&MK_SHIFT ) )
				{
					if( CancelSel( true ) )	
					{
						break;
					}
				}
				MoveCursor( m_CursorPos - 1 );	
			}

			//û�а���shift��
			if( !( event.key.flags&MK_SHIFT ) )
			{
				MoveSelBegin( m_CursorPos );
			}
		}
		break;

	case VK_RIGHT:
		//������ctrl��
		if( event.key.flags&MK_CONTROL )
		{
			Point2D curPos = GetCharPos( m_CursorPos );
			int curIdx = GetPosChar( 1000, curPos.x );
			if( curIdx >= 0 )
			{
				MoveCursor( curIdx + 1 );
			}
		}else{
			//û�а���shift��
			if( !( event.key.flags&MK_SHIFT ) )
			{
				if( CancelSel( false ) )
				{
					break;
				}
			}
			MoveCursor( m_CursorPos + 1 );
		}

		//û�а���shift��
		if( !( event.key.flags&MK_SHIFT ) )
		{
			MoveSelBegin( m_CursorPos );
		}
		break;

	case VK_HOME:

		//������ctrl��
		if( ( event.key.flags&MK_CONTROL ) )
		{
			MoveCursor( 0 );
		}else{
			//û�а���shift��
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

		//û�а���shift��
		if( !( event.key.flags&MK_SHIFT ) )
		{
			MoveSelBegin( m_CursorPos );
		}

		break;

	case VK_END:
		//������ctrl��
		if( event.key.flags&MK_CONTROL )
		{
			MoveCursor( (int)m_RichCaption.size() );
		}else{
			MoveCursor( (int)m_RichCaption.size() );

		}
		//û�а���shift��
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

			// �õ���curPos.y-1��,��curPos.x�е��ַ�����
			int curIdx = GetPosChar( curPos.y-1, curPos.x );

			if(curIdx>=0)
			{
				MoveCursor( curIdx );
			}

			//û�а���shift��
			if( !( event.key.flags&MK_SHIFT ) )
			{
				MoveSelBegin( m_CursorPos );
			}
		}
		break;

	case VK_DOWN:
		{
			Ogre::Point2D curPos = GetCharPos( m_CursorPos );

			// �õ���curPos.y+1��,��curPos.x�е��ַ�����
			int curIdx = GetPosChar( curPos.y + 1, curPos.x );

			if( curIdx >= 0 )
			{
				MoveCursor( curIdx );
			}

			//û�а���shift��
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

// ճ��ѡ����ַ�
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
		//���������������
		if( ParseMaxSize() >= m_MaxChar || ( m_MaxWordCount > 0 && getTextCount() >= m_MaxWordCount ) ) 
		{
			break;
		}
		// �����ӡ�ַ�
		if( InputRawChar( caption[i] ) )
		{
			CalcCharsLine();
		}
	}
	MoveSelBegin( m_CursorPos );
	SetUpdateDirty( true );
}

// ����ѡ����ַ�
void MultiEditBox::OnCopy()
{
	if( m_SelBegin != m_CursorPos )
	{
		//�õ�ѡ���ı�
		int selBegin	= min( m_SelBegin, m_CursorPos );
		int selEnd		= max( m_SelBegin, m_CursorPos );

		std::string selText;
		GetRawString( selText,selBegin, selEnd - selBegin );
		if( selText.empty() ) 
		{
			return;
		}

		//�õ������ڴ�
		HGLOBAL hGlobal = GlobalAlloc( GHND|GMEM_SHARE, selText.size() + 1 );
		if(!hGlobal)
		{
			return;
		}

		//��乲���ڴ�
		LPVOID pGlobal = GlobalLock(hGlobal);
		strncpy( (char*)pGlobal, selText.c_str(),selText.size() + 1 );
		GlobalUnlock( hGlobal );

		//���ü�����
		if( !::OpenClipboard(NULL) )
		{
			return;
		}

		::EmptyClipboard();
		::SetClipboardData( CF_TEXT, hGlobal );
		::CloseClipboard();
	}		
}

// ����ѡ����ַ�
void MultiEditBox::OnCut()
{
	OnCopy();
	ClearSel();
	SetUpdateDirty( true );
}


// ��������ָ��ķ�Χʹ���Ǳ����ں���ķ�Χ��
void MultiEditBox::AjustForReason()
{
	// ����ǰ���λ�ó����ַ������Χ,�����λ���õ��������ó�0
	if( m_CursorPos > (int)m_RichCaption.size() ) 
	{
		m_CursorPos = (int)m_RichCaption.size();
	}else if( m_CursorPos < 0 ){
		m_CursorPos = 0;
	}
	
	// ����ǰѡ�е��ַ�����ʼλ�ó����ַ������Χ,��λ���õ��������ó�0
	if( m_SelBegin > (int)m_RichCaption.size() ) 
	{
		m_SelBegin = (int)m_RichCaption.size();
	}else if( m_SelBegin < 0 ){ 
		m_SelBegin = 0;
	}
	
	// �����������
	int lineNum = m_AbsRect.getHeight()/( g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h ) );

	// ����ÿ��richchar���ڵ���
	CalcCharsLine();	
	
	//���������
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
			// �����������ʼ��
			IniMultiEditSlider();
		}
		m_pSlider->SetValue( (float)m_LineBegin );
	}
}

//ȷ�����λ�ú���
void MultiEditBox::CheckForReason()
{
	assert( m_SelBegin >= 0 );
	assert( m_LineBegin >= 0 );
	assert( ( m_CursorPos >= 0 ) && ( m_CursorPos <= (int)m_RichCaption.size() ) );

	//ȷ�����λ�ú���
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
	//���ݼ���ó����������߶���������ֱ������	
	int nFontHight = g_pDisplay->GetFontHeight( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h );

	//�߶ȿ������ɵ�����
	int nContentLines = m_AbsRect.getHeight()/nFontHight;

	//��ǰ��Ҫ��ʾ������
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

// �õ�ĳ���ַ�����λ��(row-y,col-x)
Ogre::Point2D MultiEditBox::GetCharPos( int index )
{
	assert( index >= 0 );

	// ǰһ��rich�ַ��Ĵ�ӡ����Xλ��
	int nCharEndXPos = 0;

	// ǰһ��rich�ַ��Ĵ�ӡ����Yλ��
	int nCharSelYPos = 0;

	Point2D linePos(0,0);
	const UIFont& oneUIFont = g_pFrameMgr->getUIFontByIndex( m_FontIndex );

	for ( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{ 
		const stRichChar& oneRichChar = m_RichCaption[i];
		float fCharWidth = 0.0f, fCharHight = 0.0f;
		g_pDisplay->GetTextExtent( oneUIFont.h, oneRichChar.keybuf, fCharWidth, fCharHight );

		//��ӡһ���ַ�
		if( index == i )
		{
			break;
		}

		// ���ƻ��з������ �� ������ӡ����,����
		if ( oneRichChar.type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			linePos.x = 0;
			linePos.y++;
		}

		//��һ���ַ��ڵ�ǰ�д�ӡλ��
		linePos.x++;

		//��¼��ӡ����λ��
		nCharEndXPos += (int)fCharWidth;
	}

	return linePos;
}

//posΪ�����captionrect������
//���ع��λ������
int MultiEditBox::PosToChar( Ogre::Point2D pos )
{
	Ogre::RectInt captionRect = m_AbsRect;

	//posλ����һ��
	const UIFont& oneUIFont = g_pFrameMgr->getUIFontByIndex( m_FontIndex );
	int line = pos.y / g_pDisplay->GetFontHeight( oneUIFont.h );

	int lineIndex = 0;

	int result = (int)m_RichCaption.size();

	// ǰһ��rich�ַ��Ĵ�ӡ����Xλ��
	int nCharEndXPos = 0;

	// ǰһ��rich�ַ��Ĵ�ӡ����Yλ��
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
		//��¼��ӡ����λ��
		nCharEndXPos += (int)fCharWidth;
		
		// ���ƻ��з������ �� ������ӡ����,����
		if( oneRichChar.type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos =0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			lineIndex++;
		}

		//������򳬳�������
		if( ( nCharSelYPos + m_TextSize ) > m_AbsRect.getHeight() )	break;
	}
	return result;
}

//�õ���row��,��col�е��ַ�����
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

	// ǰһ��rich�ַ��Ĵ�ӡ����Xλ��
	int nCharEndXPos = 0;

	// ǰһ��rich�ַ��Ĵ�ӡ����Yλ��
	int nCharSelYPos = 0;

	Point2D linePos(0,0);

	for( int i = 0; i < (int)m_RichCaption.size(); i++ )
	{
		float fCharWidth, fCharHight;
		HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;
		g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[m_FontIndex].h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );

		// ���ƻ��з������ �� ������ӡ����,����
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );

			//��������
			if( row == linePos.x ) 
			{
				return i;
			}

			linePos.x++;
			linePos.y = 0;
		}	

		//��ǰ��ӡ�ַ�����ƥ��
		if( ( row == linePos.x)&&( col == linePos.y ) ) 
		{
			return i;
		}

		//��һ���ַ��ڵ�ǰ�д�ӡλ��
		linePos.y++;

		//��¼��ӡ����λ��
		nCharEndXPos += (int)fCharWidth;

		//����������,��û���ҵ�
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

// ����ÿ��richchar���ڵ���
void MultiEditBox::CalcCharsLine()
{
	// ǰһ��rich�ַ��Ĵ�ӡ����Xλ��
	int nCharEndXPos = 0;

	// ǰһ��rich�ַ��Ĵ�ӡ����Yλ��
	int nCharSelYPos = 0;

	// ��ǰ������
	int nlineIndex = 0;

	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;

	int nMaxLine = m_AbsRect.getHeight() / ( g_pDisplay->GetFontHeight( h ) );

	for( int i = 0, size = (int)m_RichCaption.size(); i < size; i++ )
	{ 
		float fCharWidth, fCharHight;
		g_pDisplay->GetTextExtent( h, m_RichCaption[i].keybuf, fCharWidth, fCharHight );

		// ���ƻ��з������ �� ������ӡ����,����
		if( m_RichCaption[i].type == RICH_CHAR_LF || ( nCharEndXPos + fCharWidth ) > m_AbsRect.getWidth() )
		{
			// �����п�����0 �߶��ۼ�(��ǰ����ĸ߶�)
			nCharEndXPos = 0;
			nCharSelYPos += ( m_TextSize + m_RowSpace );
			nlineIndex++;
		}

		m_RichCaption[i].line = nlineIndex;
		if( m_RichCaption[i].type == RICH_CHAR_LF )
		{
			//m_RichCaption[i].line--;
		}
		
		// ��¼��ӡ����λ��
		nCharEndXPos += (int)fCharWidth;
	}
}

// �ڹ��λ�ò��룬������������ַ������ǰ���Ƿ��ǰ����������У��ϲ�Ϊһ������
bool MultiEditBox::InputRawChar(unsigned char rawChar)
{
	if( !IsInputEnable( rawChar ) )
	{
		return false;
	}

	bool bNeedUpdate = false;
	// ���з���
	if( rawChar == '\n')
	{
		stRichChar rChar;
		rChar.type		= RICH_CHAR_LF;
		rChar.width		= 0;
		rChar.keybuf[0] = '\n';
		
		bNeedUpdate		= InsertRichChar( m_CursorPos, rChar );
	}

	// Ӣ�Ļ��ߺ��ֺ��ֽ�
	else if( rawChar <= 128 )
	{
		// ���ǰ���а�����֣���ӵ�һ����������ĺ���
		if( IsHalfDBCSPre( m_CursorPos ) )
		{
			m_RichCaption[m_CursorPos-1].type		= RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]	= rawChar;
			m_RichCaption[m_CursorPos-1].width		= m_TextSize;
		}

		// ���ǰ��û�а�����֣�ֱ�Ӳ���Ӣ�� ��IsPrint �ж��Ƿ��ӡ�ַ���
		else if( IsPrint( rawChar ) )
		{
			stRichChar rChar;
			rChar.type		= RICH_CHAR_CHAR;
			rChar.width		= (int)(m_TextSize*0.5);
			rChar.keybuf[0]	= rawChar;

			bNeedUpdate		= InsertRichChar( m_CursorPos, rChar );	
		}
	}

	// ���֣���ǰ���Ƿ��а������	
	else if( rawChar > 128 )
	{
		// ���ǰ���а�����֣���ӵ�һ����������ĺ���
		if( IsHalfDBCSPre( m_CursorPos ) )
		{
			m_RichCaption[m_CursorPos-1].type		= RICH_CHAR_WORD;
			m_RichCaption[m_CursorPos-1].keybuf[1]	= rawChar;
			m_RichCaption[m_CursorPos-1].width		= m_TextSize;
		}

		// ���ǰ��û�а�����֣�ֱ�Ӳ���������
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

// �ж�ǰһ���ַ��Ƿ�������
bool MultiEditBox::IsInputEnable( unsigned char rawChar )
{
	// ��������ַ���Ŀ����
	if( ParseMaxSize() >= m_MaxChar || getTextCount() >= m_MaxWordCount ) 
	{
		return false;
	}

	// ��������ù���������������
 	if( !m_SliderName.empty() )
	{
		return true;
	}

	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_FontIndex].h;
	
	// �����������������
	int nMaxLine = m_AbsRect.getHeight() / ( g_pDisplay->GetFontHeight( h ) );

	// ��������е��ַ���
	std::string strLastLineChars;

	// ��ǰ������ַ�
	char szChar[UI_KEYCHAR_MAX];
	memset( szChar, 0, UI_KEYCHAR_MAX );
	szChar[0] = rawChar;
	
	// ���Ѵﵽ�������
	if( !m_RichCaption.empty() && m_RichCaption[m_RichCaption.size()-1].line >= nMaxLine - 1 )
	{
		if( rawChar == '\n')
		{
			// ��������
			return false;
		}else{

			// ȡ����ǰ������ڵ�����λ��
			Ogre::Point2D CurPos = GetCharPos( m_CursorPos );

			// ȡ����������е��ַ�����
			int nLastLineChars = GetCharsInLine( CurPos.y );

			// ȡ����������еĵ�һ���ַ�������
			int nIndexChar = GetPosChar( CurPos.y, 1 );

			// ȡ����������е��ַ���
			GetRawString( strLastLineChars, nIndexChar - 1, nLastLineChars );
			
			// ȡ����ǰ���ַ����ĳ���
			float fLineWidth, fRawCharWidth, fCharHight;
			g_pDisplay->GetTextExtent( h,strLastLineChars.c_str(), fLineWidth, fCharHight );

			// ȡ����ǰ�����ַ��ĳ���
			g_pDisplay->GetTextExtent( h,szChar, fRawCharWidth, fCharHight );

			// ��ǰ�еĿ�ȳ����������������������
			if( fLineWidth + fRawCharWidth > m_AbsRect.getWidth() )
			{
				return false;
			}
		}
	}
	return true;
}

// �ж�ǰһ���ַ��Ƿ�������
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

// ��ָ��λ��(pos)����һ��rich char��ͬʱ���Զ��������
bool MultiEditBox::InsertRichChar( int pos, stRichChar rChar )
{
	// ��ָ����λ��(pos)�ں���Χ�����ַ����뵽m_RichCaption
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

//��ָ��λ��ɾ��һ��rich char��ͬʱ���Զ��������
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

// ��ȡm_RichCaption�洢��ȫ���ַ����Ĵ�С
int MultiEditBox::ParseMaxSize()
{
	int size=0;
	for( int i = 0, len = (int)m_RichCaption.size();i < len; i++ )
	{ 
		size += (int)strlen( m_RichCaption[i].keybuf );
	}
	return size;
}

// ����������ı�
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

// ��ȡm_RichCaption�洢��ȫ���ַ���
const char* MultiEditBox::GetText()
{
	GetRawString( m_Caption, 0, (int)m_RichCaption.size() );
	return m_Caption.c_str();
}

// ��������״̬
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

// ��ȡm_RichCaption��start��λ�ÿ�ʼ��length����ȫ���ַ���
void MultiEditBox::GetRawString( std::string& strBuf, int start, int length )
{
	// ����ʼλ�ò����������ó�0
	if( start < 0 || start >= (int)m_RichCaption.size() )
	{
		start = 0;
	}
	
	// ����ȡ�ĳ��Ȳ��������ó��ܳ��ȼ�ȥ�������ʼλ��
	if( length < 0 || ( start + length ) > (int)m_RichCaption.size())
	{
		length = (int)m_RichCaption.size() - start;
	}
	
	// �ϲ�startλ�ÿ�ʼ��length���ȵ��ַ���
	strBuf = "";
	for( int i = start; i < ( start + length ); i++ )
	{ 
		strBuf += m_RichCaption[i].keybuf;
	}
}

// �õ�ĳһ�е��ַ���Ŀ
int MultiEditBox::GetCharsInLine( int line )
{
	// ǰһ��rich�ַ��Ĵ�ӡ����Xλ��
	int nCharEndXPos = 0;

	// ǰһ��rich�ַ��Ĵ�ӡ����Yλ��
	int nCharSelYPos = 0;

	// ��ǰ������
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

		// ���ƻ��з������ �� ������ӡ����,����
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
		// ��¼��ӡ����λ��
		nCharEndXPos += (int)fCharWidth;
	}
	return num;
}
// ѡ��ʼλ��pos��ʾ����һ����ѡ��rich�ַ�Ϊ��pos��rich�ַ�
// ע��selend����cursorpos����ʾ�����һ����ѡ���ַ�Ϊcusorpos-1
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


//����λ��pos��ʾ������ڵ�pos��rich�ַ�ǰ��
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

// ��ձ༭��
void MultiEditBox::Clear()
{
	m_RichCaption.clear();
	MoveCursor(0);
	m_LineBegin = 0;
	MoveSelBegin(0);
	CallScript( SC_ONTEXTSET );
}

// ���ù��ѡ��
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

// ������ѡ���ڵ��ַ�
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

//bLeft:�Ƿ񽫹�꿿�� bMove:�Ƿ��ƶ����
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

// �����Ƿ�����ַ�λ��
void MultiEditBox::SetUpdateDirty( bool bDirty)
{ 
	m_bDirty = bDirty;
}

// ���ݹ�������value����m_RichCaption�ӵ�value����ʾ
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

// ȫѡ
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