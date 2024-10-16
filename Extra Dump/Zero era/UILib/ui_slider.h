#pragma once
#include "ui_frame.h"
#include "ui_fontstring.h"
#include "ui_framemgr.h"

//-----------------------------------------------------------------------------
// Slider control
//-----------------------------------------------------------------------------
class Slider : public Frame { //tolua_exprot
	DECLARE_UIOBJECT_CLONE(Slider)
public:
	Slider();
	~Slider();

	virtual void FillDrawItems();
	virtual void Draw();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	virtual int OnInputMessage(const Ogre::InputEvent &event);
	virtual const char *GetTypeName()
	{ 
		return "Slider"; 
	}

	void SetThumbTexture(std::string path, Ogre::UIBlendMode drawmode);

public:
	//tolua_begin
	
	void SetValue(float value);
	float GetValue();
	float GetLastValue();

	void SetMinValue(float value);
	float GetMinValue();

	void SetMaxValue(float value);
	float GetMaxValue();

	void SetValueStep(float value);
	float GetValueStep();
	//tolua_end

public:
	Texture *m_pThumbRegion;

	float			m_fMinValue;	// 此为滚动条最小移动次数
	float			m_fMaxValue;	// 此为滚动条最大移动次数
	float			m_fValue;		// 此为滚动条当前移动次数
	float			m_fValueStep;	// 此为滚动条每移动一次经过的单位距离
	ORIENTATION_T	m_Orientation;

	float m_fLastFrameValue; //更新前的值
	float m_fLastMouseValue;
protected:

protected:
	void UpdateSelf(float deltatime);
};//tolua_exprot