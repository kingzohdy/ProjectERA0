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
	int  type;  // 字符类型
	int  width;	// 字符宽度
	int  line;	// 字符所在行
	bool blink;	// 是否是超链接字符
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

	// 根据滚动条的value设置m_RichCaption从第value行显示
	void SetSliderValue( int nValue );
	
	// 设置输入框文本
    void SetText(const char *ptext);
	void AddText(const char *ptext);

    // 设置文本颜色
    void SetTextColor( int r, int g, int b );

	// 获取m_RichCaption存储的全部字符串
	const char* GetText();

	// 清空编辑框
	void Clear();
	
	// 中文输入状态
	void enableIME( bool b );

	unsigned int getTextCount();

	void setMaxWordCount(int nMax){m_MaxWordCount = nMax;};
	
	// 全选
	void SelectAllText();
	//修改能否编辑
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

	// 光标是否可见
	bool IsCursorVisible();
	
	// 刷新界面
	void UpdateSelf( float deltatime );

	// 输入的消息响应
	int OnInputMessage( const Ogre::InputEvent &event );
	
	// 捕获输入字符
	int OnChar( const Ogre::InputEvent &event, bool bVirtual=false );

	// 按键按下响应
	int OnKeyDown(const Ogre::InputEvent &event, bool bVirtual=false);

	// 粘贴选择的字符
	void OnPaste();

	// 复制选择的字符
	void OnCopy();

	// 剪切选择的字符
	void OnCut();

	// 调整各个指标的范围 使他们保持在合理的范围内
	void AjustForReason();

	//确保光标位置合理
	void CheckForReason();

	// 更新滚动条
	void UpdateScrollBar();
	
	// 得到某个字符行列位置( x列,y行 )
	Ogre::Point2D GetCharPos( int index );

	//pos为相对于captionrect的坐标
	//返回光标位置索引
	int PosToChar( Ogre::Point2D pos );

	//得到第row行,第col列的字符索引
	int GetPosChar( int row,int col );

	// 计算每个richchar所在的行
	void CalcCharsLine();

	// 插入字符
	bool InputRawChar( unsigned char rawChar );
	
	// 字符是否可输入
	bool IsInputEnable( unsigned char rawChar );
	
	// 判断前一个字符是否半个汉字
	bool IsHalfDBCSPre( int pos );

	// 插入字符到m_RichCaption
	bool InsertRichChar( int pos,stRichChar rChar );

	//在指定位置删除一个字符，同时，自动调整光标
	bool EraseRichChar( int pos );

	// 获取m_RichCaption存储的全部字符串的大小
	int ParseMaxSize();

	// 获取m_RichCaption第start个位置开始的length个的全部字符串
	void GetRawString( std::string& strBuf, int start, int length );

	// 得到某一行的字符数目
	int GetCharsInLine( int line );

	// 选择开始位置pos表示：第一个被选择rich字符为第pos个rich字符
	void MoveSelBegin( int pos );
	
	// 光标的位置pos表示：光标在第pos个rich字符前面
	void MoveCursor( int pos );

	// 设置光标选区
	int SetSel( int start,int end );
	
	// 清除光标选区内的字符
	int ClearSel();
	
	//bLeft:是否将光标靠左 bMove:是否移动光标
	int CancelSel( bool bLeft, bool bMove=true );

	// 是否需要重新调整字符位置
	bool IsUpdateDirty()
	{
		return m_bDirty; 
	}

	// 设置是否调整字符位置
	void SetUpdateDirty( bool bDirty );

	// 初始化multiedit的滚动条
	void IniMultiEditSlider();

	// 传入的点是否在rect区域内
	bool IsPointInside( Ogre::RectInt rect, int x, int y ) const
	{
		return ( x >= rect.m_Left && y >= rect.m_Top && x <= rect.m_Right && y <= rect.m_Bottom );
	}

public:
	// 编辑框的滚动条name
	std::string m_SliderName;

	// 指richchar的个数
	int m_MaxChar;

	// 选中状态条颜色
	Ogre::ColorQuad m_EditSelColor;

	// 光标的颜色
	Ogre::ColorQuad m_CurSorColor;

	// 绘制颜色
	Ogre::ColorQuad m_DrawColor;
	
	// 字体索引
	int m_FontIndex;	

	// 行间距
	int m_RowSpace;	

	int m_MaxWordCount;

protected:

	// 编辑框的滚动条
	Slider *m_pSlider;

	// 存储操作和显示字符串的数组
	std::vector<stRichChar>m_RichCaption;

	// 显示的起始行
	int m_LineBegin;

	// 当前光标所在行
	int m_LinePos;

	int m_SelBegin;

	// rich char的光标位置
	int m_CursorPos;

	// enter key flag
	int m_ReturnFlag;

	// 背景颜色
	Ogre::ColorQuad m_BkColor;

	int m_CursorTimer;
	int m_CursorFlag;	//0--not draw cursor,  1--draw cursor
	int m_CursorLive;	//显示时间
	int m_CursorDeath;	//隐藏时间
	
	int m_TextSize;		//字体大小
	bool m_bDirty;		
	std::string m_Caption;

};//tolua_export