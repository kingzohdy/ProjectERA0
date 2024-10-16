
/* 此三个类为CModelMotionInfo的父类，设计上主要考虑到将CSRTBar对应控件上的数据传送给CModelMotionInfo
** CSTRObject对应的对象层比较多
** CFloatObject对应泛色等之类的对象层
*/
#pragma once
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"

class CSRTObject
{
public:
	CSRTObject(void);
	virtual ~CSRTObject(void);

	virtual Ogre::Vector3	GetPosition();
	virtual Ogre::Quaternion	GetRotation();
	virtual Ogre::Vector3	GetScale();
	virtual float			GetTransparent();
	virtual Ogre::Vector3	GetRotateAngle();

	virtual void	SetPosition(Ogre::Vector3 pos);
	virtual void	SetRotation(Ogre::Quaternion rot);
	virtual void	SetScale(Ogre::Vector3 scale);
	virtual void	SetTransparent(float fTransparent);
	virtual void	SetRotationAngle(Ogre::Vector3 rotAngle);

	virtual void	SetOffColor(Ogre::ColourValue mOffColor);
	virtual Ogre::ColourValue GetOffColor();

	virtual void	SetFade(bool bFade);
	virtual bool	GetFade();

	virtual bool	IsGeomScale()	{ return false; };
	virtual CString GetObjName()	{ return ""; };

	static CSRTObject ms_DummyObj;

};

class CFloatObject
{
public:
	virtual float	GetFloat()				{ return 0.0f ; };
	virtual void	SetFloat(float val)		{ };
	virtual void	GetFloatRange(float* pmin, float* pmax, float* ratio) { *pmin=-100.0f; *pmax=100.0f; *ratio=1.0f; };
	virtual CString	GetFloatName()			{return "无名"; };

	static CFloatObject ms_DummyObj;

};

class CColorObject
{
public:
	virtual Ogre::ColourValue	GetColor()	{ return Ogre::ColourValue(0,0,0,0); };
	virtual void			SetColor(Ogre::ColourValue val) { };

	static CColorObject ms_DummyObj;
};

class CRemanetShadowObject
{
public:
	//残影数据
	virtual int GetIntervalTime(){ return 40; }
	virtual void SetIntervalTime(int nIntervalTime){}

	virtual int GetImages(){ return 8; }
	virtual void SetImages(int nImages){};

	static CRemanetShadowObject ms_DummyObj;
};

class COffColorObject
{
public:
	virtual Ogre::ColourValue	GetOffColor();
	virtual void			SetOffColor(Ogre::ColourValue mOffColor);

	static COffColorObject ms_DummyObj;
};

class CIceFrozenObject
{
public:
	virtual Ogre::BlendMode GetIceFrozenBlend();
	virtual CString GetOverlayTex();
	virtual void SetIceFrozenBlend(Ogre::BlendMode blendmode, CString &texpath,float alpha = 1.0f);
	virtual float GetIceFrozenAlpha();

	static CIceFrozenObject ms_DummyObj;
};

//焦点模糊（景深用CFloatObject表示）
//有4个float参数，按照seq分为1、2、3、4  节约函数数量
class CFocusBlurObject
{
public:
	virtual float	GetFocusBlurParams(int nSeq) { return 0.0f; }
	virtual void	SetFocusBlurParams(int nSeq, float val){ }

	static CFocusBlurObject ms_DummyObj;
};
