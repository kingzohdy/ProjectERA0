#pragma once
#include "ui_layoutframe.h"

//-----------------------------------------------------------------------------
// Font Instance
//-----------------------------------------------------------------------------
class FontInstance { //tolua_export
public:
	FontInstance();
	~FontInstance();
	void SetFont(Ogre::HUIRES iFont); //真正的字体
	void SetJustifyV(JUSTIFYV_T justifyV); //Sets vertical text justification ("TOP","BOTTOM", or "MIDDLE") 
	void SetJustifyH(JUSTIFYH_T justifyh); //Sets horizontal text justification ("LEFT","RIGHT", or "CENTER") 
	void SetShadowColor(Ogre::ColorQuad color);
	void SetSpacing(float space); //Sets the spacing between lines of text in the object

public:
	//Ogre::HUIRES m_hFont;

	//Ogre::HFONT m_hFont;
	int m_iFontIndex;
	std::string m_FontName;

	Ogre::ColorQuad m_TextColor;
	Ogre::ColorQuad m_ShadowColor;
	std::string m_Caption;

	float m_fSpace;
	OUTLINE_T m_outline;
	bool m_bMonochrome;
	bool m_bNonspacewra;

	JUSTIFYV_T m_JustifyV;
	JUSTIFYH_T m_JustifyH;

	unsigned int m_MaxLines;

	int m_FontStyle;	//0 FONTSYTLE_NORMAL 1 FONTSYTLE_SHADOW 2 FONTSYTLE_BORDER

	float m_fLineInterval;
};//tolua_export

//-----------------------------------------------------------------------------
// Font String
//-----------------------------------------------------------------------------
class FontString : public LayoutFrame, public FontInstance {//tolua_export
	DECLARE_UIOBJECT_CLONE(FontString)
public:
	FontString();
	~FontString();

	virtual void Draw();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	//void SetTextHeight(float pixelHeight);

	virtual void UpdateSelf(float deltatime);
	virtual const char *GetTypeName(){ return "FontString"; }

public:
	//tolua_begin
	const char *GetText();
	void SetText(const char *str);
	void SetBufferTimer(float start, float total);

	void SetTextColor(int r, int g, int b);

	void SetBlendAlpha(float alpha);

	void SetScrollNumber(unsigned int begin, unsigned int end, float time);
	void SetScrollNumberWithUint(unsigned int begin, unsigned int end, float time);
	float GetBuffStart()
	{
		return m_fBuffStart;
	}
	float GetBuffTotal()
	{
		return m_fBuffTotal;
	}
	
	int GetTextExtentWidth( const char* pszContent );
	//tolua_end

public:
	int m_OffsetX;
	int m_OffsetY;
	bool m_bAutoWrap;
	float m_fScale;

	//buff的时间设置
	bool m_bBuff;
	float m_fBuffStart;
	float m_fBuffTotal;

	//滚动数字
	bool m_bScrollNum;
	bool m_bScrollNumUint;	//带单位的滚动数字
	int m_iBegin;
	int m_iEnd;
	float m_fScrollNumTotal;
	float m_fScrollNumAcc;
};//tolua_export
