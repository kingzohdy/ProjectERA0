#pragma once
#include "ui_button.h"
#include "ui_framemgr.h"

//-----------------------------------------------------------------------------
// CheckButton control
//-----------------------------------------------------------------------------
class CheckButton : public Button { //tolua_export
	DECLARE_UIOBJECT_CLONE(CheckButton)
public:
	CheckButton();
	~CheckButton();

	virtual void FillDrawItems();
	virtual void Draw();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	//void SetCurrentTexture(HUIRES hTex);
	//HUIRES GetCurrentTexture();

	virtual int OnInputMessage(const Ogre::InputEvent &event);
	virtual const char *GetTypeName(){ return "CheckButton"; }

public:
	//tolua_begin
	bool GetCheckState();
	void setCheckState( bool bChecked ); 
	//tolua_end
public:
	std::string m_Caption;

	bool m_bChecked;

	//FontString *m_pFontString;

	//Texture *m_pCheckRegion;

	//HUIRES m_hDisabledTex;
	//HUIRES m_hEnabledTex;
protected:
	void UpdateSelf(float deltatime);
protected:
};//tolua_export