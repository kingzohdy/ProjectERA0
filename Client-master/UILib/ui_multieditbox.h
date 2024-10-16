#pragma once
#include "ui_frame.h"
#include "ui_framemgr.h"
#include "ui_slider.h"

//-----------------------------------------------------------------------------
// MultiEditBox control
//-----------------------------------------------------------------------------
const int UI_KEYCHAR_MAX = 5;
enum RichCharType
{
	RICH_CHAR_NONE,
	RICH_CHAR_WORD,
	RICH_CHAR_CHAR,
	RICH_CHAR_ICON,
	RICH_CHAR_COLOR,
	RICH_CHAR_FONT,
	RICH_CHAR_STYLE,
	RICH_CHAR_HLINK,
	RICH_CHAR_LF,
	RICH_CHAR_HALF,
};

enum ShortCutKey
{
	VK_X = 88,
	VK_C = 67,
	VK_V = 86,
};

struct stRichChar
{
	int  index;
	int  type;  // �ַ�����
	int  width;	// �ַ����
	int  line;	// �ַ�������
	bool blink;	// �Ƿ��ǳ������ַ�
	char keybuf[UI_KEYCHAR_MAX];

	stRichChar()
	{
		index	= -1;
		type	= RICH_CHAR_NONE;
		width	= 0;
		line	= 0;			
		blink	= false;
		memset( keybuf, 0, UI_KEYCHAR_MAX );
	}
};
class MultiEditBox : public Frame { //tolua_export
	DECLARE_UIOBJECT_CLONE(MultiEditBox)
public:
	MultiEditBox();
	~MultiEditBox();

	virtual void FillDrawItems();

	virtual void Draw();

	virtual const char *GetTypeName()
	{
		return "MultiEditBox"; 
	}

public:
	//tolua_begin

	// ���ݹ�������value����m_RichCaption�ӵ�value����ʾ
	void SetSliderValue( int nValue );
	
	// ����������ı�
    void SetText(const char *ptext);
	void AddText(const char *ptext);

    // �����ı���ɫ
    void SetTextColor( int r, int g, int b );

	// ��ȡm_RichCaption�洢��ȫ���ַ���
	const char* GetText();

	// ��ձ༭��
	void Clear();
	
	// ��������״̬
	void enableIME( bool b );

	unsigned int getTextCount();

	void setMaxWordCount(int nMax){m_MaxWordCount = nMax;};
	
	// ȫѡ
	void SelectAllText();
	//�޸��ܷ�༭
	void enableEdit( bool b );

	int getCursorPos();
	void setCursorPos( int nPos );

	int getSelBegin();
	void setSelBegin( int nPos );
	int getTextBegin( const char* pszText, int nFindPos );
	int getTextEnd( const char* pszText, int nFindPos );

	int getTextIndexFromRichCharIndex( int nRichCharIndex );
	//tolua_end

protected:

	// ����Ƿ�ɼ�
	bool IsCursorVisible();
	
	// ˢ�½���
	void UpdateSelf( float deltatime );

	// �������Ϣ��Ӧ
	int OnInputMessage( const Ogre::InputEvent &event );
	
	// ���������ַ�
	int OnChar( const Ogre::InputEvent &event, bool bVirtual=false );

	// ����������Ӧ
	int OnKeyDown(const Ogre::InputEvent &event, bool bVirtual=false);

	// ճ��ѡ����ַ�
	void OnPaste();

	// ����ѡ����ַ�
	void OnCopy();

	// ����ѡ����ַ�
	void OnCut();

	// ��������ָ��ķ�Χ ʹ���Ǳ����ں���ķ�Χ��
	void AjustForReason();

	//ȷ�����λ�ú���
	void CheckForReason();

	// ���¹�����
	void UpdateScrollBar();
	
	// �õ�ĳ���ַ�����λ��( x��,y�� )
	Ogre::Point2D GetCharPos( int index );

	//posΪ�����captionrect������
	//���ع��λ������
	int PosToChar( Ogre::Point2D pos );

	//�õ���row��,��col�е��ַ�����
	int GetPosChar( int row,int col );

	// ����ÿ��richchar���ڵ���
	void CalcCharsLine();

	// �����ַ�
	bool InputRawChar( unsigned char rawChar );
	
	// �ַ��Ƿ������
	bool IsInputEnable( unsigned char rawChar );
	
	// �ж�ǰһ���ַ��Ƿ�������
	bool IsHalfDBCSPre( int pos );

	// �����ַ���m_RichCaption
	bool InsertRichChar( int pos,stRichChar rChar );

	//��ָ��λ��ɾ��һ���ַ���ͬʱ���Զ��������
	bool EraseRichChar( int pos );

	// ��ȡm_RichCaption�洢��ȫ���ַ����Ĵ�С
	int ParseMaxSize();

	// ��ȡm_RichCaption��start��λ�ÿ�ʼ��length����ȫ���ַ���
	void GetRawString( std::string& strBuf, int start, int length );

	// �õ�ĳһ�е��ַ���Ŀ
	int GetCharsInLine( int line );

	// ѡ��ʼλ��pos��ʾ����һ����ѡ��rich�ַ�Ϊ��pos��rich�ַ�
	void MoveSelBegin( int pos );
	
	// ����λ��pos��ʾ������ڵ�pos��rich�ַ�ǰ��
	void MoveCursor( int pos );

	// ���ù��ѡ��
	int SetSel( int start,int end );
	
	// ������ѡ���ڵ��ַ�
	int ClearSel();
	
	//bLeft:�Ƿ񽫹�꿿�� bMove:�Ƿ��ƶ����
	int CancelSel( bool bLeft, bool bMove=true );

	// �Ƿ���Ҫ���µ����ַ�λ��
	bool IsUpdateDirty()
	{
		return m_bDirty; 
	}

	// �����Ƿ�����ַ�λ��
	void SetUpdateDirty( bool bDirty );

	// ��ʼ��multiedit�Ĺ�����
	void IniMultiEditSlider();

	// ����ĵ��Ƿ���rect������
	bool IsPointInside( Ogre::RectInt rect, int x, int y ) const
	{
		return ( x >= rect.m_Left && y >= rect.m_Top && x <= rect.m_Right && y <= rect.m_Bottom );
	}

public:
	// �༭��Ĺ�����name
	std::string m_SliderName;

	// ָrichchar�ĸ���
	int m_MaxChar;

	// ѡ��״̬����ɫ
	Ogre::ColorQuad m_EditSelColor;

	// ������ɫ
	Ogre::ColorQuad m_CurSorColor;

	// ������ɫ
	Ogre::ColorQuad m_DrawColor;
	
	// ��������
	int m_FontIndex;	

	// �м��
	int m_RowSpace;	

	int m_MaxWordCount;

protected:

	// �༭��Ĺ�����
	Slider *m_pSlider;

	// �洢��������ʾ�ַ���������
	std::vector<stRichChar>m_RichCaption;

	// ��ʾ����ʼ��
	int m_LineBegin;

	// ��ǰ���������
	int m_LinePos;

	int m_SelBegin;

	// rich char�Ĺ��λ��
	int m_CursorPos;

	// enter key flag
	int m_ReturnFlag;

	// ������ɫ
	Ogre::ColorQuad m_BkColor;

	int m_CursorTimer;
	int m_CursorFlag;	//0--not draw cursor,  1--draw cursor
	int m_CursorLive;	//��ʾʱ��
	int m_CursorDeath;	//����ʱ��
	
	int m_TextSize;		//�����С
	bool m_bDirty;		
	std::string m_Caption;

};//tolua_export