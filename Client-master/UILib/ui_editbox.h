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
	//�޸��ܷ�༭
	void enableEdit( bool b );
	int GetTextExtentWidth( const char* pszContent );
	//�޸� ��������ַ���
	void setMaxChar( int nMax );

	int getCursorPos();
	void setCursorPos( int nPos );

	int getSelBegin();
	void setSelBegin( int nPos );
	//tolua_end
public:
	FontString *m_pFontString;

	int m_MaxChar;		//���뱣֤maxchar����ż��
	bool m_bMultiLine;	//�Ƿ������ʾ
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

	int m_CursorPos;	//��ǰ���λ�õ��ַ�����
	int m_DispBegin;	//��ʾ��ʼλ�õ��ַ�����
	int m_SelBegin;		//ѡ���ַ��Ŀ�ʼ����

	int m_CursorTimer;
	int m_CursorFlag; //0--not draw cursor,  1--draw cursor
	int m_CursorLive;//��ʾʱ��
	int m_CursorDeath;//����ʱ��

	double m_Value;

	double m_MaxValue;
	double m_MinValue;

	int m_InputFlag;	//0--�������뺺��,1--�������뺺��,2--��������
	
	std::vector<std::string> m_History;
	int m_CurHistory;

	//�����
	//unsigned char m_EncryptCaption[UI_DEFAULT_BUFSIZE];
	int m_iSizeEncrypt;

	//ѡ��״̬����ɫ
	Ogre::ColorQuad m_EditSelColor;

	//disable״̬������ɫ
	Ogre::ColorQuad m_GrayColor;
	//HUIRES m_hFontString;
};//tolua_export