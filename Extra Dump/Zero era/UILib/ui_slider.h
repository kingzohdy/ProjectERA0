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

	float			m_fMinValue;	// ��Ϊ��������С�ƶ�����
	float			m_fMaxValue;	// ��Ϊ����������ƶ�����
	float			m_fValue;		// ��Ϊ��������ǰ�ƶ�����
	float			m_fValueStep;	// ��Ϊ������ÿ�ƶ�һ�ξ����ĵ�λ����
	ORIENTATION_T	m_Orientation;

	float m_fLastFrameValue; //����ǰ��ֵ
	float m_fLastMouseValue;
protected:

protected:
	void UpdateSelf(float deltatime);
};//tolua_exprot