#pragma once
#include "ui_frame.h"

class WebBrowerFrame: public Frame{ //tolua_export
	DECLARE_UIOBJECT_CLONE( WebBrowerFrame )
public:
	WebBrowerFrame();
	~WebBrowerFrame();
	virtual void UpdateSelf(float deltatime);
	virtual int OnInputMessage( const Ogre::InputEvent &event );
	virtual const char *GetTypeName()
	{ 
		return "WebBrowerFrame"; 
	}

	//tolua_begin
	void OpenSite(const char* szUrl);
	void ShowWindow( bool bShow );
	//tolua_end

private:
	void*	m_pWindow;
};//tolua_export
