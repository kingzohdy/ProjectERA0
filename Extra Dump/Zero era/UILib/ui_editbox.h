#pragma once
#include "ui_frame.h"
#include "ui_framemgr.h"
#include <vector>

//-----------------------------------------------------------------------------
// EditBox control
//-----------------------------------------------------------------------------

//tolua_begin
enum
{
	EDIT_EVENT_CHAR,
	EDIT_EVENT_KEY,
};
//tolua_end

class EditBox : public Frame { //tolua_export
	DECLARE_UIOBJECT_CLONE(EditBox)
public:
	EditBox();
	~EditBox();

	virtual void FillDrawItems();
	virtual void Draw();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	virtual int OnInputMessage(const Ogre::InputEvent &event);
	virtual const char *GetTypeName(){ return "EditBox"; }
	//void SetFont(std::string fontname);
	//HUIRES GetFont();

public:
	//tolua_begin
	int GetSelctTexLen();
	bool IsAnyTextSelect();
	void ReplaceSelectText( const char* pszReplaceText );
	void Clear();
	void SetText(const char *ptext);
	void AddText( const char *ptext );
	const char *GetText();
	const char *GetPassWord();
	void SetTextColor(int r, int g, int b);
	void AddStringToHistory(const char* text);
	void clearHistory();
	void SelectAllText();
	void enableIME( bool b );
	void SetPassword( const char *ptext);
	//修改能否编辑
	void enableEdit( bool b );
	int GetTextExtentWidth( const char* pszContent );
	//修改 最大输入字符数
	void setMaxChar( int nMax );

	int getCursorPos();
	void setCursorPos( int nPos );

	int getSelBegin();
	void setSelBegin( int nPos );
	//tolua_end
public:
	FontString *m_pFontString;

	int m_MaxChar;		//必须保证maxchar总是偶数
	bool m_bMultiLine;	//是否多行显示
	int m_NumHistory;
	int m_EditMethod;
	std::string m_EncryptCaption;

protected:
	bool IsCursorVisible();
	void UpdateSelf(float deltatime);
	void CheckForReason();
	int ClearSel();
	int CancelSel(bool bLeft,bool bMove=true);
	void CaptionToValue();
	bool CheckValueRange();
	void UpdateInputFlag();
	bool QueryInputFlag(char key);

	void QueryInputChar( std::string& strSrc );
	void SetVar(double fValue, const char *sValue);
	void ValueToCaption();
	int SetSel(int start,int end);
	void Encrypt();
	void Decrypt();

	int OnChar(const Ogre::InputEvent &event, bool bVirtual=false);
	int OnKeyDown(const Ogre::InputEvent &event, bool bVirtual=false);
	void OnPaste();
	void OnCopy();
	void OnCut();
	void OnChangeText();
protected:
	enum eUIInputFlag
	{
		INPUT_FLAG_NONE=0,
		INPUT_FLAG_CHAR=1,
		INPUT_FLAG_WORD=2,
	};

	//std::string m_Caption;

	int m_CursorPos;	//当前光标位置的字符索引
	int m_DispBegin;	//显示开始位置的字符索引
	int m_SelBegin;		//选中字符的开始索引

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//显示时间
	int m_CursorDeath;//隐藏时间

	double m_Value;

	double m_MaxValue;
	double m_MinValue;

	int m_InputFlag;	//0--可以输入汉字,1--不能输入汉字,2--不能输入
	
	std::vector<std::string> m_History;
	int m_CurHistory;

	//密码框
	//unsigned char m_EncryptCaption[UI_DEFAULT_BUFSIZE];
	int m_iSizeEncrypt;

	//选中状态条颜色
	Ogre::ColorQuad m_EditSelColor;

	//disable状态绘制颜色
	Ogre::ColorQuad m_GrayColor;
	//HUIRES m_hFontString;
};//tolua_export